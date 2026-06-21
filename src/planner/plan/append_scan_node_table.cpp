#include "binder/expression/property_expression.h"
#include "planner/operator/scan/logical_scan_node_table.h"
#include "planner/planner.h"

using namespace kuzu::common;
using namespace kuzu::binder;

namespace kuzu {
namespace planner {

static expression_vector removeInternalIDProperty(const expression_vector& expressions) {
    expression_vector result;
    for (auto expr : expressions) {
        if (expr->constCast<PropertyExpression>().isInternalID()) {
            continue;
        }
        result.push_back(expr);
    }
    return result;
}

void Planner::appendScanNodeTable(std::shared_ptr<Expression> nodeID,
    std::vector<table_id_t> tableIDs, const expression_vector& properties, LogicalPlan& plan) {
    KU_ASSERT(!tableIDs.empty());
    auto propertiesToScan_ = removeInternalIDProperty(properties);
    if (tableIDs.size() > 1) {
        std::vector<LogicalPlan> childrenPlans;
        for (auto tableID : tableIDs) {
            LogicalPlan childPlan;
            appendScanNodeTable(nodeID, {tableID}, properties, childPlan);
            childrenPlans.push_back(std::move(childPlan));
        }
        expression_vector expressionsToUnion;
        expressionsToUnion.push_back(nodeID);
        for (auto& prop : propertiesToScan_) {
            expressionsToUnion.push_back(prop);
        }
        plan = createUnionPlan(childrenPlans, expressionsToUnion, true /* isUnionAll */);
    } else {
        auto scan = make_shared<LogicalScanNodeTable>(std::move(nodeID), tableIDs[0],
            propertiesToScan_);
        scan->computeFactorizedSchema();
        scan->setCardinality(cardinalityEstimator.estimateScanNode(*scan));
        plan.setLastOperator(std::move(scan));
    }
}

} // namespace planner
} // namespace kuzu
