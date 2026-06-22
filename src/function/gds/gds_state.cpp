#include "function/gds/gds_state.h"

#include "graph/graph.h"
#include "graph/graph_entry.h"

namespace gorgonzola {
namespace function {

void GDSComputeState::initSource(common::nodeID_t sourceNodeID) const {
    frontierPair->pinNextFrontier(sourceNodeID.tableID);
    frontierPair->addNodeToNextFrontier(sourceNodeID);
    frontierPair->setActiveNodesForNextIter();
    auxiliaryState->initSource(sourceNodeID);
}

void GDSComputeState::beginFrontierCompute(common::table_id_t currTableID,
    common::table_id_t nextTableID) const {
    frontierPair->beginFrontierComputeBetweenTables(currTableID, nextTableID);
    auxiliaryState->beginFrontierCompute(currTableID, nextTableID);
}

common::table_id_set_t GDSComputeState::getActiveRelTableIDs(size_t index, graph::Graph* graph) {
    if (stepActiveRelTableIDs.empty()) {
        common::table_id_set_t set;
        for (auto info : graph->getGraphEntry()->nodeInfos) {
            for (auto& relInfo : graph->getRelInfos(info.entry->getTableID())) {
                set.insert(relInfo.relTableID);
            }
        }
        stepActiveRelTableIDs.push_back(set);
    }
    if (index < stepActiveRelTableIDs.size()) {
        return stepActiveRelTableIDs[index];
    } else {
        return stepActiveRelTableIDs.back();
    }
}

void GDSComputeState::switchToDense(processor::ExecutionContext* context,
    graph::Graph* graph) const {
    frontierPair->switchToDense(context, graph);
    auxiliaryState->switchToDense(context, graph);
}

} // namespace function
} // namespace gorgonzola
