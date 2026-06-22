#pragma once

#include "binder/expression/expression.h"
#include "common/enums/extend_direction.h"
#include "common/enums/path_semantic.h"
#include "function/gds/gds_state.h"
#include "graph/graph_entry.h"
#include "processor/operator/recursive_extend_shared_state.h"
#include "rj_output_writer.h"

namespace gorgonzola {
namespace function {

struct RJBindData {
    graph::NativeGraphEntry graphEntry;

    std::shared_ptr<binder::Expression> nodeInput = nullptr;
    std::shared_ptr<binder::Expression> nodeOutput = nullptr;
    // For any form of shortest path lower bound must always be 1.
    // If lowerBound equals to 0, an empty path with source node only will be returned.
    uint16_t lowerBound = 0;
    uint16_t upperBound = 0;
    common::PathSemantic semantic = common::PathSemantic::WALK;

    common::ExtendDirection extendDirection = common::ExtendDirection::FWD;

    bool extendRightToLeft = false; // See PathsOutputWriterInfo::extendRightToLeft for comments.
    bool writePath = true;

    std::shared_ptr<binder::Expression> directionExpr = nullptr;
    std::shared_ptr<binder::Expression> lengthExpr = nullptr;
    std::shared_ptr<binder::Expression> pathNodeIDsExpr = nullptr;
    std::shared_ptr<binder::Expression> pathEdgeIDsExpr = nullptr;

    std::shared_ptr<binder::Expression> weightPropertyExpr = nullptr;
    std::shared_ptr<binder::Expression> weightOutputExpr = nullptr;

    std::vector<common::table_id_set_t> stepFromLeftActivationRelInfos;
    std::vector<common::table_id_set_t> stepFromRightActivationRelInfos;

    explicit RJBindData(graph::NativeGraphEntry graphEntry) : graphEntry{std::move(graphEntry)} {}
    RJBindData(const RJBindData& other);

    PathsOutputWriterInfo getPathWriterInfo() const;

    std::vector<common::table_id_set_t> getStepActiveRelTableIDs() const;
};

class RJAlgorithm {
public:
    virtual ~RJAlgorithm() = default;

    virtual std::string getFunctionName() const = 0;
    virtual binder::expression_vector getResultColumns(const RJBindData& bindData) const = 0;

    virtual std::unique_ptr<GDSComputeState> getComputeState(processor::ExecutionContext* context,
        const RJBindData& bindData, processor::RecursiveExtendSharedState* sharedState) = 0;
    virtual std::unique_ptr<RJOutputWriter> getOutputWriter(processor::ExecutionContext* context,
        const RJBindData& bindData, GDSComputeState& computeState, common::nodeID_t sourceNodeID,
        processor::RecursiveExtendSharedState* sharedState) = 0;

    virtual std::unique_ptr<RJAlgorithm> copy() const = 0;
};

} // namespace function
} // namespace gorgonzola
