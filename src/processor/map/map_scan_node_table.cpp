#include "binder/binder.h"
#include "binder/expression/property_expression.h"
#include "binder/expression_binder.h"
#include "common/mask.h"
#include "planner/operator/scan/logical_scan_node_table.h"
#include "processor/expression_mapper.h"
#include "processor/plan_mapper.h"
#include "storage/storage_manager.h"
#include "function/table/scan_node_table_function.h"
#include "processor/operator/table_function_call.h"

using namespace kuzu::binder;
using namespace kuzu::common;
using namespace kuzu::planner;
using namespace kuzu::function;

namespace kuzu {
namespace processor {

std::unique_ptr<PhysicalOperator> PlanMapper::mapScanNodeTable(
    const LogicalOperator* logicalOperator) {
    auto storageManager = storage::StorageManager::Get(*clientContext);
    auto catalog = catalog::Catalog::Get(*clientContext);
    auto transaction = transaction::Transaction::Get(*clientContext);
    auto& scan = logicalOperator->constCast<LogicalScanNodeTable>();
    const auto outSchema = scan.getSchema();
    auto nodeIDPos = getDataPos(*scan.getNodeID(), *outSchema);
    std::vector<DataPos> outVectorsPos;
    for (auto& expression : scan.getProperties()) {
        outVectorsPos.emplace_back(getDataPos(*expression, *outSchema));
    }
    
    std::vector<DataPos> outPosV;
    outPosV.push_back(nodeIDPos);
    for (auto& pos : outVectorsPos) {
        outPosV.push_back(pos);
    }

    auto tableID = scan.getTableID();
    auto tableEntry = catalog->getTableCatalogEntry(transaction, tableID);
    auto nodeTable = storageManager->getTable(tableID)->ptrCast<storage::NodeTable>();

    std::vector<common::column_id_t> columnIDs;
    std::vector<ColumnCaster> columnCasters;
    auto binder = Binder(clientContext);
    auto expressionBinder = ExpressionBinder(&binder, clientContext);
    for (auto& expr : scan.getProperties()) {
        auto& property = expr->constCast<PropertyExpression>();
        if (property.hasProperty(tableID)) {
            auto propertyName = property.getPropertyName();
            auto& columnType = tableEntry->getProperty(propertyName).getType();
            auto columnCaster = ColumnCaster(columnType.copy());
            if (property.getDataType() != columnType) {
                auto columnExpr = std::make_shared<PropertyExpression>(property);
                columnExpr->dataType = columnType.copy();
                columnCaster.setCastExpr(
                    expressionBinder.forceCast(columnExpr, property.getDataType()));
            }
            columnIDs.push_back(tableEntry->getColumnID(propertyName));
            columnCasters.push_back(std::move(columnCaster));
        } else {
            columnIDs.push_back(INVALID_COLUMN_ID);
            columnCasters.push_back(ColumnCaster(LogicalType::ANY()));
        }
    }

    binder::expression_vector columns;
    columns.push_back(scan.getNodeID());
    for (auto& prop : scan.getProperties()) {
        columns.push_back(prop);
    }

    TableFunctionCallInfo info;
    std::unique_ptr<TableFunctionCallPrintInfo> printInfo;
    std::shared_ptr<TableFuncSharedState> sharedState;

    switch (scan.getScanType()) {
    case LogicalScanNodeTableType::SCAN: {
        auto tableFunc = ScanNodeTableFunction::getFunctionSet()[0]->ptrCast<TableFunction>()->copy();
        auto bindData = std::make_unique<ScanNodeTableBindData>(
            std::move(columns), nodeTable->getNumTotalRows(transaction),
            nodeTable, std::move(columnIDs), std::move(columnCasters),
            copyVector(scan.getPropertyPredicates()), clientContext);

        info.function = *tableFunc;
        info.bindData = std::move(bindData);
        info.outPosV = outPosV;

        auto initInput = TableFuncInitSharedStateInput(info.bindData.get(), executionContext);
        sharedState = info.function.initSharedStateFunc(initInput);
        printInfo = std::make_unique<TableFunctionCallPrintInfo>(
            ScanNodeTableFunction::name, info.bindData->columns);
    } break;

    case LogicalScanNodeTableType::PRIMARY_KEY_SCAN: {
        auto& primaryKeyScanInfo = scan.getExtraInfo()->constCast<PrimaryKeyScanInfo>();
        auto exprMapper = ExpressionMapper(outSchema);
        auto indexEvaluator = exprMapper.getEvaluator(primaryKeyScanInfo.key);

        auto tableFunc = PrimaryKeyScanNodeTableFunction::getFunctionSet()[0]->ptrCast<TableFunction>()->copy();
        auto bindData = std::make_unique<PrimaryKeyScanNodeTableBindData>(
            std::move(columns), 1 /* numRows */,
            nodeTable, std::move(columnIDs), std::move(columnCasters),
            copyVector(scan.getPropertyPredicates()), clientContext,
            std::move(indexEvaluator));

        info.function = *tableFunc;
        info.bindData = std::move(bindData);
        info.outPosV = outPosV;

        auto initInput = TableFuncInitSharedStateInput(info.bindData.get(), executionContext);
        sharedState = info.function.initSharedStateFunc(initInput);
        printInfo = std::make_unique<TableFunctionCallPrintInfo>(
            PrimaryKeyScanNodeTableFunction::name, info.bindData->columns);
    } break;

    default:
        KU_UNREACHABLE;
    }

    return std::make_unique<TableFunctionCall>(
        std::move(info), std::move(sharedState), getOperatorID(), std::move(printInfo));
}

} // namespace processor
} // namespace kuzu
