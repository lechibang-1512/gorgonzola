#include "function/gds/rec_joins.h"

namespace gorgonzola {
namespace function {

RJBindData::RJBindData(const RJBindData& other) {
    graphEntry = other.graphEntry.copy();
    nodeInput = other.nodeInput;
    nodeOutput = other.nodeOutput;
    lowerBound = other.lowerBound;
    upperBound = other.upperBound;
    semantic = other.semantic;
    extendDirection = other.extendDirection;
    extendRightToLeft = other.extendRightToLeft;
    writePath = other.writePath;
    directionExpr = other.directionExpr;
    lengthExpr = other.lengthExpr;
    pathNodeIDsExpr = other.pathNodeIDsExpr;
    pathEdgeIDsExpr = other.pathEdgeIDsExpr;
    weightPropertyExpr = other.weightPropertyExpr;
    weightOutputExpr = other.weightOutputExpr;
    stepFromLeftActivationRelInfos = other.stepFromLeftActivationRelInfos;
    stepFromRightActivationRelInfos = other.stepFromRightActivationRelInfos;
}

PathsOutputWriterInfo RJBindData::getPathWriterInfo() const {
    auto info = PathsOutputWriterInfo();
    info.semantic = semantic;
    info.lowerBound = lowerBound;
    info.extendRightToLeft = extendRightToLeft;
    info.writeEdgeDirection = writePath && extendDirection == common::ExtendDirection::BOTH;
    info.writePath = writePath;
    return info;
}

std::vector<common::table_id_set_t> RJBindData::getStepActiveRelTableIDs() const {
    if (extendRightToLeft) {
        return stepFromRightActivationRelInfos;
    } else {
        return stepFromLeftActivationRelInfos;
    }
}

} // namespace function
} // namespace gorgonzola
