#include "binder/query/query_graph_label_analyzer.h"

#include "catalog/catalog.h"
#include "catalog/catalog_entry/node_table_catalog_entry.h"
#include "catalog/catalog_entry/rel_group_catalog_entry.h"
#include "common/exception/binder.h"
#include "common/string_format.h"
#include "transaction/transaction.h"

using namespace gorgonzola::common;
using namespace gorgonzola::catalog;
using namespace gorgonzola::transaction;

namespace gorgonzola {
namespace binder {

QueryGraphLabelAnalyzer::QueryGraphLabelAnalyzer(const main::ClientContext& clientContext_,
    bool throwOnViolate_)
    : throwOnViolate{throwOnViolate_}, clientContext{clientContext_} {
    tx = Transaction::Get(clientContext);
    catalog = Catalog::Get(clientContext);
}

// NOLINTNEXTLINE(readability-non-const-parameter): graph is supposed to be modified.
void QueryGraphLabelAnalyzer::pruneLabel(QueryGraph& graph) const {
    for (auto i = 0u; i < graph.getNumQueryNodes(); ++i) {
        pruneNode(graph, *graph.getQueryNode(i));
    }
    for (auto i = 0u; i < graph.getNumQueryRels(); ++i) {
        pruneRel(*graph.getQueryRel(i));
    }
}

struct Candidates {
    table_id_set_t idSet;
    std::unordered_set<std::string> nameSet;

    void insert(const table_id_set_t& idsToInsert, Catalog* catalog, Transaction* transaction) {
        for (auto id : idsToInsert) {
            auto name = catalog->getTableCatalogEntry(transaction, id)->getName();
            idSet.insert(id);
            nameSet.insert(name);
        }
    }

    bool empty() const { return idSet.empty(); }

    bool contains(const table_id_t& id) const { return idSet.contains(id); }

    std::string toString() const {
        auto names = std::vector<std::string>{nameSet.begin(), nameSet.end()};
        auto result = names[0];
        for (auto j = 1u; j < names.size(); ++j) {
            result += ", " + names[j];
        }
        return result;
    }
};

void QueryGraphLabelAnalyzer::pruneNode(const QueryGraph& graph, NodeExpression& node) const {
    for (auto i = 0u; i < graph.getNumQueryRels(); ++i) {
        auto queryRel = graph.getQueryRel(i);
        if (queryRel->isRecursive()) {
            continue;
        }
        Candidates candidates;
        auto isSrcConnect = *queryRel->getSrcNode() == node;
        auto isDstConnect = *queryRel->getDstNode() == node;
        if (queryRel->getDirectionType() == RelDirectionType::BOTH) {
            if (isSrcConnect || isDstConnect) {
                for (auto entry : queryRel->getEntries()) {
                    auto& relEntry = entry->constCast<RelGroupCatalogEntry>();
                    candidates.insert(relEntry.getSrcNodeTableIDSet(), catalog, tx);
                    candidates.insert(relEntry.getDstNodeTableIDSet(), catalog, tx);
                }
            }
        } else {
            if (isSrcConnect) {
                for (auto entry : queryRel->getEntries()) {
                    auto& relEntry = entry->constCast<RelGroupCatalogEntry>();
                    candidates.insert(relEntry.getSrcNodeTableIDSet(), catalog, tx);
                }
            } else if (isDstConnect) {
                for (auto entry : queryRel->getEntries()) {
                    auto& relEntry = entry->constCast<RelGroupCatalogEntry>();
                    candidates.insert(relEntry.getDstNodeTableIDSet(), catalog, tx);
                }
            }
        }
        if (candidates.empty()) { // No need to prune.
            continue;
        }
        std::vector<TableCatalogEntry*> prunedEntries;
        for (auto entry : node.getEntries()) {
            if (!candidates.contains(entry->getTableID())) {
                continue;
            }
            prunedEntries.push_back(entry);
        }
        node.setEntries(prunedEntries);
        if (prunedEntries.empty()) {
            if (throwOnViolate) {
                throw BinderException(
                    stringFormat("Query node {} violates schema. Expected labels are {}.",
                        node.toString(), candidates.toString()));
            }
        }
    }
}

bool hasOverlap(const table_id_set_t& left, const table_id_set_t& right) {
    for (auto id : left) {
        if (right.contains(id)) {
            return true;
        }
    }
    return false;
}



std::vector<common::table_id_set_t> QueryGraphLabelAnalyzer::pruneRecursiveRel(
    const std::unordered_map<common::table_id_t, std::unordered_map<common::table_id_t, common::table_id_vector_t>>& graph,
    const std::unordered_map<common::table_id_t, std::unordered_map<common::table_id_t, common::table_id_vector_t>>& reseveGraph,
    const common::table_id_set_t& startTableIDSet, const common::table_id_set_t& endTableIDSet, size_t lowerBound,
    size_t upperBound, common::table_id_t maxTableID) const {

    std::unordered_map<common::table_id_t, std::vector<bool>> f, g;

    auto initFunc = [upperBound](const std::unordered_map<common::table_id_t,
                                     std::unordered_map<common::table_id_t, common::table_id_vector_t>>& _graph,
                        std::unordered_map<common::table_id_t, std::vector<bool>>& ans,
                        const common::table_id_set_t& beginTableIDSet) {
        for (auto& [_, map] : _graph) {
            for (auto& [_, rels] : map) {
                for (auto rel : rels) {
                    ans.emplace(rel, std::vector<bool>(upperBound + 1, false));
                }
            }
        }
        for (auto tableID : beginTableIDSet) {
            if (!_graph.contains(tableID)) continue;
            for (auto& [dst, rels] : _graph.at(tableID)) {
                for (auto rel : rels) {
                    ans[rel][1] = true;
                    ans[rel][0] = true;
                }
            }
        }
    };

    initFunc(graph, f, startTableIDSet);
    initFunc(reseveGraph, g, endTableIDSet);

    auto isOk = [&](const common::table_id_vector_t& rels, int j,
                    std::unordered_map<common::table_id_t, std::vector<bool>>& map) -> bool {
        for (auto rel : rels) {
            if (map[rel][j - 1]) return true;
        }
        return false;
    };

    auto bfsFunc =
        [upperBound, maxTableID, isOk](
            const std::unordered_map<common::table_id_t, std::unordered_map<common::table_id_t, common::table_id_vector_t>>& _graph,
            const std::unordered_map<common::table_id_t, std::unordered_map<common::table_id_t, common::table_id_vector_t>>& _reseveGraph,
            std::unordered_map<common::table_id_t, std::vector<bool>>& map) {
            for (int j = 2; j <= upperBound; ++j) {
                for (auto v = 0u; v <= maxTableID; ++v) {
                    bool flag = false;
                    if (_reseveGraph.contains(v)) {
                        for (auto& [_, rels] : _reseveGraph.at(v)) {
                            if (isOk(rels, j, map)) {
                                flag = true;
                                break;
                            }
                        }
                    }
                    if (flag && _graph.contains(v)) {
                        for (auto& [dst, rels] : _graph.at(v)) {
                            for (auto rel : rels) {
                                map[rel][j] = true;
                            }
                        }
                    }
                }
            }
        };

    bfsFunc(graph, reseveGraph, f);
    bfsFunc(reseveGraph, graph, g);

    std::vector<common::table_id_set_t> stepActiveTableIDs(upperBound);
    for (auto& [rel, vector] : f) {
        for (int j = 0; j <= upperBound; ++j) {
            if (!vector[j]) continue;
            for (int k = 0; k <= upperBound; ++k) {
                if (!g[rel][k]) continue;
                auto step = j + k;
                if (step != upperBound) step--;
                if (step < lowerBound) continue;
                else if (step > upperBound) break;
                else {
                    int index = j == 0 ? 0 : j - 1;
                    stepActiveTableIDs[index].emplace(rel);
                    break;
                }
            }
        }
    }
    return stepActiveTableIDs;
}

void QueryGraphLabelAnalyzer::pruneRel(RelExpression& rel) const {
    auto srcTableIDSet = rel.getSrcNode()->getTableIDsSet();
    auto dstTableIDSet = rel.getDstNode()->getTableIDsSet();
    std::vector<catalog::TableCatalogEntry*> prunedEntries;

    if (rel.isRecursive()) {
        auto recursiveInfo = rel.getRecursiveInfoUnsafe();
        auto lowerBound = recursiveInfo->bindData->lowerBound;
        auto upperBound = recursiveInfo->bindData->upperBound;
        if (upperBound == 0 || rel.getEntries().empty()) {
            return;
        }

        std::unordered_map<common::table_id_t, std::unordered_map<common::table_id_t, common::table_id_vector_t>> stepFromLeftGraph;
        std::unordered_map<common::table_id_t, std::unordered_map<common::table_id_t, common::table_id_vector_t>> stepFromRightGraph;
        common::table_id_t maxTableID = 0;

        for (auto entry : rel.getEntries()) {
            auto& groupEntry = entry->constCast<catalog::RelGroupCatalogEntry>();
            for (auto& relInfo : groupEntry.getRelEntryInfos()) {
                auto srcTableID = relInfo.nodePair.srcTableID;
                auto dstTableID = relInfo.nodePair.dstTableID;
                auto tableID = relInfo.oid;
                stepFromLeftGraph[srcTableID][dstTableID].push_back(tableID);
                stepFromRightGraph[dstTableID][srcTableID].push_back(tableID);
                if (rel.getDirectionType() == RelDirectionType::BOTH) {
                    stepFromLeftGraph[dstTableID][srcTableID].push_back(tableID);
                    stepFromRightGraph[srcTableID][dstTableID].push_back(tableID);
                }
                maxTableID = std::max(maxTableID, (common::table_id_t)std::max(srcTableID, dstTableID));
            }
        }

        auto stepFromLeftTableIDs = pruneRecursiveRel(stepFromLeftGraph, stepFromRightGraph, srcTableIDSet,
            dstTableIDSet, lowerBound, upperBound, maxTableID);
        auto stepFromRightTableIDs = pruneRecursiveRel(stepFromRightGraph, stepFromLeftGraph, dstTableIDSet,
            srcTableIDSet, lowerBound, upperBound, maxTableID);
        std::reverse(stepFromRightTableIDs.begin(), stepFromRightTableIDs.end());

        recursiveInfo->bindData->stepFromLeftActivationRelInfos = stepFromLeftTableIDs;
        recursiveInfo->bindData->stepFromRightActivationRelInfos = stepFromRightTableIDs;

        common::table_id_set_t activeRelTableIDs;
        for (auto& set : stepFromLeftTableIDs) {
            activeRelTableIDs.insert(set.begin(), set.end());
        }
        for (auto& set : stepFromRightTableIDs) {
            activeRelTableIDs.insert(set.begin(), set.end());
        }

        for (auto entry : rel.getEntries()) {
            auto& groupEntry = entry->constCast<catalog::RelGroupCatalogEntry>();
            bool keep = false;
            for (auto& relInfo : groupEntry.getRelEntryInfos()) {
                if (activeRelTableIDs.contains(relInfo.oid)) {
                    keep = true;
                    break;
                }
            }
            if (keep) {
                prunedEntries.push_back(entry);
            }
        }

        if (prunedEntries.size() != rel.getEntries().size() || !std::equal(prunedEntries.begin(), prunedEntries.end(), rel.getEntries().begin())) {
            rel.setEntries(prunedEntries);
            recursiveInfo->rel->setEntries(prunedEntries);

            std::unordered_set<common::table_id_t> forwardRelNodes;
            if (!stepFromLeftTableIDs.empty()) {
                for (auto oid : stepFromLeftTableIDs.front()) {
                    for (auto entry : rel.getEntries()) {
                        auto& groupEntry = entry->constCast<catalog::RelGroupCatalogEntry>();
                        for (auto& relInfo : groupEntry.getRelEntryInfos()) {
                            if (relInfo.oid == oid) {
                                forwardRelNodes.insert(relInfo.nodePair.srcTableID);
                            }
                        }
                    }
                }
            }

            std::unordered_set<common::table_id_t> backwardRelNodes;
            for (auto i = lowerBound; i <= upperBound; ++i) {
                if (i == 0) continue;
                if (i - 1 < stepFromLeftTableIDs.size()) {
                    for (auto oid : stepFromLeftTableIDs.at(i - 1)) {
                        for (auto entry : rel.getEntries()) {
                            auto& groupEntry = entry->constCast<catalog::RelGroupCatalogEntry>();
                            for (auto& relInfo : groupEntry.getRelEntryInfos()) {
                                if (relInfo.oid == oid) {
                                    backwardRelNodes.insert(relInfo.nodePair.dstTableID);
                                }
                            }
                        }
                    }
                }
            }

            if (rel.getDirectionType() == RelDirectionType::BOTH) {
                forwardRelNodes.insert(backwardRelNodes.begin(), backwardRelNodes.end());
                backwardRelNodes = forwardRelNodes;
            }

            std::vector<catalog::TableCatalogEntry*> newSrcNodeEntries;
            for (auto entry : rel.getSrcNode()->getEntries()) {
                if (forwardRelNodes.contains(entry->getTableID())) {
                    newSrcNodeEntries.push_back(entry);
                }
            }
            rel.getSrcNode()->setEntries(newSrcNodeEntries);

            std::vector<catalog::TableCatalogEntry*> newDstNodeEntries;
            for (auto entry : rel.getDstNode()->getEntries()) {
                if (backwardRelNodes.contains(entry->getTableID())) {
                    newDstNodeEntries.push_back(entry);
                }
            }
            rel.getDstNode()->setEntries(newDstNodeEntries);
        }
    } else {
        if (rel.getDirectionType() == RelDirectionType::BOTH) {
    // Note the pruning for node should guarantee the following exception won't be triggered.
    // For safety (and consistency) reason, we still write the check but skip coverage check.
    // LCOV_EXCL_START
    if (prunedEntries.empty()) {
        if (throwOnViolate) {
            throw BinderException(stringFormat("Cannot find a label for relationship {} that "
                                               "connects to all of its neighbour nodes.",
                rel.toString()));
        }
    }
    // LCOV_EXCL_STOP
}

} // namespace binder
} // namespace gorgonzola
}
}
