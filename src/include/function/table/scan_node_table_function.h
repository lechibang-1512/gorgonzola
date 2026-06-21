#pragma once

#include <atomic>
#include <mutex>
#include "function/table/bind_data.h"
#include "function/table/table_function.h"
#include "storage/predicate/column_predicate.h"
#include "storage/table/node_table.h"
#include "processor/operator/scan/scan_table.h"
#include "expression_evaluator/expression_evaluator.h"

namespace kuzu {
namespace function {

struct ScanNodeTableProgressSharedState {
    std::atomic<common::node_group_idx_t> numGroupsScanned;
    common::node_group_idx_t numGroups;

    ScanNodeTableProgressSharedState() : numGroupsScanned{0}, numGroups{0} {}
};

class ScanNodeTableSharedState : public TableFuncSharedState {
public:
    explicit ScanNodeTableSharedState(std::unique_ptr<common::SemiMask> semiMask)
        : TableFuncSharedState{}, table{nullptr},
          currentCommittedGroupIdx{common::INVALID_NODE_GROUP_IDX},
          currentUnCommittedGroupIdx{common::INVALID_NODE_GROUP_IDX}, numCommittedNodeGroups{0},
          numUnCommittedNodeGroups{0}, semiMask{std::move(semiMask)},
          progressSharedState{std::make_shared<ScanNodeTableProgressSharedState>()} {}

    void initialize(const transaction::Transaction* transaction, storage::NodeTable* table);

    void nextMorsel(storage::NodeTableScanState& scanState);

    common::SemiMask* getSemiMask() const { return semiMasks.getOffsetMask(table->getTableID()); }

    std::shared_ptr<ScanNodeTableProgressSharedState> progressSharedState;

private:
    std::mutex mtx;
    storage::NodeTable* table;
    common::node_group_idx_t currentCommittedGroupIdx;
    common::node_group_idx_t currentUnCommittedGroupIdx;
    common::node_group_idx_t numCommittedNodeGroups;
    common::node_group_idx_t numUnCommittedNodeGroups;
    std::unique_ptr<common::SemiMask> semiMask;
};

struct ScanNodeTableBindData : public TableFuncBindData {
    storage::NodeTable* table;
    std::vector<common::column_id_t> columnIDs;
    std::vector<processor::ColumnCaster> columnCasters;
    std::vector<storage::ColumnPredicateSet> columnPredicates;
    main::ClientContext* context;

    ScanNodeTableBindData(binder::expression_vector columns, uint64_t numRows,
        storage::NodeTable* table, std::vector<common::column_id_t> columnIDs,
        std::vector<processor::ColumnCaster> columnCasters,
        std::vector<storage::ColumnPredicateSet> columnPredicates,
        main::ClientContext* context)
        : TableFuncBindData{std::move(columns), numRows}, table{table},
          columnIDs{std::move(columnIDs)}, columnCasters{std::move(columnCasters)},
          columnPredicates{std::move(columnPredicates)}, context{context} {}

    ScanNodeTableBindData(const ScanNodeTableBindData& other)
        : TableFuncBindData{other}, table{other.table}, columnIDs{other.columnIDs},
          columnCasters{copyVector(other.columnCasters)},
          columnPredicates{copyVector(other.columnPredicates)}, context{other.context} {}

    void castColumns() {
        for (auto& caster : columnCasters) {
            if (caster.hasCast()) {
                caster.cast();
            }
        }
    }

    std::unique_ptr<TableFuncBindData> copy() const override {
        return std::make_unique<ScanNodeTableBindData>(*this);
    }
};

struct ScanNodeTableLocalState : public TableFuncLocalState {
    std::unique_ptr<storage::NodeTableScanState> scanState;

    explicit ScanNodeTableLocalState(std::unique_ptr<storage::NodeTableScanState> scanState)
        : scanState{std::move(scanState)} {}
};

struct ScanNodeTableFunction {
    static constexpr const char* name = "scan_node_table";
    static function_set getFunctionSet();
};

class PrimaryKeyScanSharedState : public TableFuncSharedState {
public:
    PrimaryKeyScanSharedState() : cursor{0} {}

    common::idx_t getTableIdx();

private:
    std::mutex mtx;
    common::idx_t cursor;
};

struct PrimaryKeyScanNodeTableBindData : public ScanNodeTableBindData {
    std::shared_ptr<evaluator::ExpressionEvaluator> indexEvaluator;

    PrimaryKeyScanNodeTableBindData(binder::expression_vector columns, uint64_t numRows,
        storage::NodeTable* table, std::vector<common::column_id_t> columnIDs,
        std::vector<processor::ColumnCaster> columnCasters,
        std::vector<storage::ColumnPredicateSet> columnPredicates,
        main::ClientContext* context,
        std::shared_ptr<evaluator::ExpressionEvaluator> indexEvaluator)
        : ScanNodeTableBindData{std::move(columns), numRows, table, std::move(columnIDs),
          std::move(columnCasters), std::move(columnPredicates), context},
          indexEvaluator{std::move(indexEvaluator)} {}

    PrimaryKeyScanNodeTableBindData(const PrimaryKeyScanNodeTableBindData& other)
        : ScanNodeTableBindData{other}, indexEvaluator{other.indexEvaluator} {}

    std::unique_ptr<TableFuncBindData> copy() const override {
        return std::make_unique<PrimaryKeyScanNodeTableBindData>(*this);
    }
};

struct PrimaryKeyScanNodeTableLocalState : public TableFuncLocalState {
    std::unique_ptr<storage::NodeTableScanState> scanState;

    explicit PrimaryKeyScanNodeTableLocalState(std::unique_ptr<storage::NodeTableScanState> scanState)
        : scanState{std::move(scanState)} {}
};

struct PrimaryKeyScanNodeTableFunction {
    static constexpr const char* name = "primary_key_scan_node_table";
    static function_set getFunctionSet();
};

} // namespace function
} // namespace kuzu
