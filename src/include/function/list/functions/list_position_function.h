#pragma once

#include <iostream>
#include "common/vector/value_vector.h"
#include "function/comparison/comparison_functions.h"

namespace gorgonzola {
namespace function {

struct ListPosition {
    // Note: this function takes in a 1-based element (The index of the first element in the list
    // is 1).
    template<typename T>
    static void operation(common::list_entry_t& list, T& element, int64_t& result,
        common::ValueVector& listVector, common::ValueVector& elementVector,
        common::ValueVector& /*resultVector*/) {
        if (common::ListType::getChildType(listVector.dataType) != elementVector.dataType) {
            result = 0;
            return;
        }
        auto listElements =
            reinterpret_cast<T*>(common::ListVector::getListValues(&listVector, list));
        std::cout << "DEBUG ListPosition: list.offset=" << list.offset << " list.size=" << list.size << " element=";
        if constexpr (std::is_fundamental_v<T>) {
            std::cout << element;
        } else {
            std::cout << "(non-fundamental)";
        }
        std::cout << std::endl;
        for (auto i = 0u; i < list.size; i++) {
            std::cout << "  i=" << i << " val=";
            if constexpr (std::is_fundamental_v<T>) {
                std::cout << listElements[i];
            } else {
                std::cout << "(non-fundamental)";
            }
            std::cout << std::endl;
        }
        uint8_t comparisonResult = 0;
        for (auto i = 0u; i < list.size; i++) {
            Equals::operation(listElements[i], element, comparisonResult,
                common::ListVector::getDataVector(&listVector), &elementVector);
            if (comparisonResult) {
                result = i + 1;
                return;
            }
        }
        result = 0;
    }
};

} // namespace function
} // namespace gorgonzola
