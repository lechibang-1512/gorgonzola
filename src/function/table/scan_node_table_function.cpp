#include "function/table/scan_node_table_function.h"
#include "processor/execution_context.h"
#include "storage/local_storage/local_node_table.h"
#include "storage/local_storage/local_storage.h"
#include "common/mask.h"
#include "transaction/transaction.h"
#include "storage/buffer_manager/memory_manager.h"
#include "processor/result/result_set.h"

using namespace gorgonzola::common;
using namespace gorgonzola::storage;
using namespace gorgonzola::processor;
using namespace gorgonzola::transaction;

namespace gorgonzola {
namespace function {

void ScanNodeTableSharedState::initialize(const Transaction* transaction, NodeTable* table) {
    this->table = table;
    this->currentCommittedGroupIdx = 0;
    this->currentUnCommittedGroupIdx = 0;
    this->numCommittedNodeGroups = table->getNumCommittedNodeGroups();
    if (transaction->isWriteTransaction()) {
        if (const auto localTable =
                transaction->getLocalStorage()->getLocalTable(this->table->getTableID())) {
            auto& localNodeTable = localTable->cast<LocalNodeTable>();
            this->numUnCommittedNodeGroups = localNodeTable.getNumNodeGroups();
        }
    }
    progressSharedState->numGroups += numCommittedNodeGroups;
    semiMasks.addMask(table->getTableID(), std::move(semiMask));
}

void ScanNodeTableSharedState::nextMorsel(NodeTableScanState& scanState) {
    std::unique_lock lck{mtx};
    if (currentCommittedGroupIdx < numCommittedNodeGroups) {
        scanState.nodeGroupIdx = currentCommittedGroupIdx++;
        progressSharedState->numGroupsScanned++;
        scanState.source = TableScanSource::COMMITTED;
        return;
    }
    if (currentUnCommittedGroupIdx < numUnCommittedNodeGroups) {
        scanState.nodeGroupIdx = currentUnCommittedGroupIdx++;
        scanState.source = TableScanSource::UNCOMMITTED;
        return;
    }
    scanState.source = TableScanSource::NONE;
}

static void initScanStateVectors(TableScanState& scanState,
    const std::vector<ValueVector*>& outVectors,
    std::vector<ColumnCaster>& columnCasters,
    MemoryManager* memoryManager) {
    bool hasColumnCaster = false;
    for (auto& caster : columnCasters) {
        if (caster.hasCast()) {
            hasColumnCaster = true;
            break;
        }
    }
    if (!hasColumnCaster) {
        scanState.outputVectors = outVectors;
        return;
    }
    scanState.outputVectors.clear();
    for (auto i = 0u; i < columnCasters.size(); ++i) {
        auto& caster = columnCasters[i];
        auto vector = outVectors[i];
        if (!caster.hasCast()) {
            scanState.outputVectors.push_back(vector);
        } else {
            caster.init(vector, memoryManager);
            scanState.outputVectors.push_back(caster.getVectorBeforeCasting());
        }
    }
}

static common::offset_t scanNodeTableFunc(const TableFuncInput& input, TableFuncOutput& output) {
    auto localState = input.localState->ptrCast<ScanNodeTableLocalState>();
    auto sharedState = input.sharedState->ptrCast<ScanNodeTableSharedState>();
    auto bindData = input.bindData->ptrCast<ScanNodeTableBindData>();
    auto transaction = Transaction::Get(*input.context->clientContext);

    while (true) {
        if (localState->scanState == nullptr) {
            auto nodeIDVector = &output.dataChunk.getValueVectorMutable(0);
            std::vector<common::ValueVector*> outVectors;
            for (auto i = 1u; i < output.dataChunk.getNumValueVectors(); ++i) {
                outVectors.push_back(&output.dataChunk.getValueVectorMutable(i));
            }
            localState->scanState = std::make_unique<NodeTableScanState>(
                nodeIDVector, outVectors, nodeIDVector->state);
            localState->scanState->setToTable(transaction, bindData->table, bindData->columnIDs,
                copyVector(bindData->columnPredicates));
            initScanStateVectors(*localState->scanState, outVectors, bindData->columnCasters,
                MemoryManager::Get(*bindData->context));
            localState->scanState->semiMask = sharedState->getSemiMask();
            sharedState->nextMorsel(*localState->scanState);
            if (localState->scanState->source == TableScanSource::NONE) {
                return 0;
            }
            bindData->table->initScanState(transaction, *localState->scanState);
        }

        if (bindData->table->scan(transaction, *localState->scanState)) {
            auto outputSize = localState->scanState->outState->getSelVector().getSelSize();
            if (outputSize > 0) {
                bindData->castColumns();
                localState->scanState->outState->setToUnflat();
                output.setOutputSize(outputSize);
                return outputSize;
            }
        }
        sharedState->nextMorsel(*localState->scanState);
        if (localState->scanState->source == TableScanSource::NONE) {
            return 0;
        }
        bindData->table->initScanState(transaction, *localState->scanState);
    }
}

static std::unique_ptr<TableFuncSharedState> initSharedState(
    const TableFuncInitSharedStateInput& input) {
    auto bindData = input.bindData->constPtrCast<ScanNodeTableBindData>();
    auto transaction = Transaction::Get(*bindData->context);
    auto semiMask = SemiMaskUtil::createMask(bindData->table->getNumTotalRows(transaction));
    auto sharedState = std::make_unique<ScanNodeTableSharedState>(std::move(semiMask));
    sharedState->initialize(transaction, bindData->table);
    return sharedState;
}

static std::unique_ptr<TableFuncLocalState> initLocalState(
    const TableFuncInitLocalStateInput&) {
    return std::make_unique<ScanNodeTableLocalState>(nullptr);
}

static double progressFunc(TableFuncSharedState* sharedState) {
    auto state = sharedState->ptrCast<ScanNodeTableSharedState>();
    if (state->progressSharedState->numGroups == 0) {
        return 0.0;
    }
    return static_cast<double>(state->progressSharedState->numGroupsScanned) /
           state->progressSharedState->numGroups;
}

function_set ScanNodeTableFunction::getFunctionSet() {
    function_set functionSet;
    auto function = std::make_unique<TableFunction>(name, std::vector<LogicalTypeID>{});
    function->tableFunc = scanNodeTableFunc;
    function->initSharedStateFunc = initSharedState;
    function->initLocalStateFunc = initLocalState;
    function->progressFunc = progressFunc;
    functionSet.push_back(std::move(function));
    return functionSet;
}

common::idx_t PrimaryKeyScanSharedState::getTableIdx() {
    std::unique_lock lck{mtx};
    return cursor++;
}

static common::offset_t primaryKeyScanNodeTableFunc(const TableFuncInput& input, TableFuncOutput& output) {
    auto localState = input.localState->ptrCast<PrimaryKeyScanNodeTableLocalState>();
    auto sharedState = input.sharedState->ptrCast<PrimaryKeyScanSharedState>();
    auto bindData = input.bindData->ptrCast<PrimaryKeyScanNodeTableBindData>();
    auto transaction = Transaction::Get(*input.context->clientContext);

    auto tableIdx = sharedState->getTableIdx();
    if (tableIdx >= 1) {
        return 0;
    }

    if (localState->scanState == nullptr) {
        auto nodeIDVector = &output.dataChunk.getValueVectorMutable(0);
        std::vector<common::ValueVector*> outVectors;
        for (auto i = 1u; i < output.dataChunk.getNumValueVectors(); ++i) {
            outVectors.push_back(&output.dataChunk.getValueVectorMutable(i));
        }
        localState->scanState = std::make_unique<NodeTableScanState>(
            nodeIDVector, std::vector<common::ValueVector*>{}, nodeIDVector->state);

        ResultSet dummyResultSet{};
        bindData->indexEvaluator->init(dummyResultSet, bindData->context);

        bindData->indexEvaluator->evaluate();
        auto indexVector = bindData->indexEvaluator->resultVector.get();
        auto& selVector = indexVector->state->getSelVector();
        KU_ASSERT(selVector.getSelSize() == 1);
        auto pos = selVector.getSelectedPositions()[0];
        if (indexVector->isNull(pos)) {
            return 0;
        }
        offset_t nodeOffset = 0;
        if (!bindData->table->lookupPK(transaction, indexVector, pos, nodeOffset)) {
            return 0;
        }
        auto nodeID = nodeID_t{nodeOffset, bindData->table->getTableID()};
        localState->scanState->nodeIDVector->setValue<nodeID_t>(0, nodeID);

        localState->scanState->setToTable(transaction, bindData->table, bindData->columnIDs,
            copyVector(bindData->columnPredicates));
        initScanStateVectors(*localState->scanState, outVectors, bindData->columnCasters,
            MemoryManager::Get(*bindData->context));
        bindData->table->initScanState(transaction, *localState->scanState, nodeID.tableID, nodeOffset);
        localState->scanState->nodeIDVector->state->getSelVectorUnsafe().setToUnfiltered(1);
        auto succeeded = bindData->table->lookup(transaction, *localState->scanState);
        if (succeeded) {
            bindData->castColumns();
            output.setOutputSize(1);
            return 1;
        }
    }
    return 0;
}

static std::unique_ptr<TableFuncSharedState> initPrimaryKeySharedState(
    const TableFuncInitSharedStateInput&) {
    return std::make_unique<PrimaryKeyScanSharedState>();
}

static std::unique_ptr<TableFuncLocalState> initPrimaryKeyLocalState(
    const TableFuncInitLocalStateInput&) {
    return std::make_unique<PrimaryKeyScanNodeTableLocalState>(nullptr);
}

function_set PrimaryKeyScanNodeTableFunction::getFunctionSet() {
    function_set functionSet;
    auto function = std::make_unique<TableFunction>(name, std::vector<LogicalTypeID>{});
    function->tableFunc = primaryKeyScanNodeTableFunc;
    function->initSharedStateFunc = initPrimaryKeySharedState;
    function->initLocalStateFunc = initPrimaryKeyLocalState;
    functionSet.push_back(std::move(function));
    return functionSet;
}

} // namespace function
} // namespace gorgonzola
