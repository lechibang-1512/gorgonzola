#pragma once

#include "common/enums/rel_direction.h"
#include "main/client_context.h"
#include "query_graph.h"

namespace gorgonzola {
namespace catalog {
class Catalog;
} // namespace catalog
namespace transaction {
class Transaction;
} // namespace transaction
namespace binder {

class QueryGraphLabelAnalyzer {
public:
    explicit QueryGraphLabelAnalyzer(const main::ClientContext& clientContext, bool throwOnViolate);

    void pruneLabel(QueryGraph& graph) const;

private:
    void pruneNode(const QueryGraph& graph, NodeExpression& node) const;
    void pruneRel(RelExpression& rel) const;

    std::vector<common::table_id_set_t> pruneRecursiveRel(
        const std::unordered_map<common::table_id_t, std::unordered_map<common::table_id_t, common::table_id_vector_t>>& graph,
        const std::unordered_map<common::table_id_t, std::unordered_map<common::table_id_t, common::table_id_vector_t>>& reseveGraph,
        const common::table_id_set_t& startTableIDSet, const common::table_id_set_t& endTableIDSet, size_t lowerBound,
        size_t upperBound, common::table_id_t maxTableID) const;

private:
    bool throwOnViolate;
    const main::ClientContext& clientContext;
    transaction::Transaction* tx;
    catalog::Catalog* catalog;
};

} // namespace binder
} // namespace gorgonzola
