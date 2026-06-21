#pragma once

#include <algorithm>
#include <type_traits>
#include <vector>

#include "common/exception/runtime.h"
#include "common/string_utils.h"
#include "common/vector/value_vector.h"

namespace kuzu {
namespace function {

struct BaseListSortOperation {
public:
    static inline bool isAscOrder(const std::string& sortOrder) {
        std::string upperSortOrder = common::StringUtils::getUpper(sortOrder);
        if (upperSortOrder == "ASC") {
            return true;
        } else if (upperSortOrder == "DESC") {
            return false;
        } else {
            throw common::RuntimeException("Invalid sortOrder");
        }
    }

    static inline bool isNullFirst(const std::string& nullOrder) {
        std::string upperNullOrder = common::StringUtils::getUpper(nullOrder);
        if (upperNullOrder == "NULLS FIRST") {
            return true;
        } else if (upperNullOrder == "NULLS LAST") {
            return false;
        } else {
            throw common::RuntimeException("Invalid nullOrder");
        }
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, int>::type = 0>
    static void sortValuesInternal(T* arr, size_t n, bool ascOrder) {
        if (n <= 1) return;
        using UT = typename std::make_unsigned<T>::type;
        std::vector<UT> uarr(n);
        UT signBit = UT(1) << (sizeof(T) * 8 - 1);
        for (size_t i = 0; i < n; ++i) {
            if (std::is_signed<T>::value) {
                uarr[i] = static_cast<UT>(arr[i]) ^ signBit;
            } else {
                uarr[i] = static_cast<UT>(arr[i]);
            }
        }

        std::vector<UT> temp(n);
        constexpr size_t numPasses = sizeof(T);
        for (size_t pass = 0; pass < numPasses; ++pass) {
            size_t count[256] = {0};
            size_t shift = pass * 8;
            for (size_t i = 0; i < n; ++i) {
                count[(uarr[i] >> shift) & 0xFF]++;
            }
            if (ascOrder) {
                size_t sum = 0;
                for (size_t i = 0; i < 256; ++i) {
                    size_t tempCount = count[i];
                    count[i] = sum;
                    sum += tempCount;
                }
                for (size_t i = 0; i < n; ++i) {
                    size_t byteVal = (uarr[i] >> shift) & 0xFF;
                    temp[count[byteVal]++] = uarr[i];
                }
            } else {
                size_t sum = 0;
                for (int i = 255; i >= 0; --i) {
                    size_t tempCount = count[i];
                    count[i] = sum;
                    sum += tempCount;
                }
                for (size_t i = 0; i < n; ++i) {
                    size_t byteVal = (uarr[i] >> shift) & 0xFF;
                    temp[count[byteVal]++] = uarr[i];
                }
            }
            uarr = temp;
        }
        for (size_t i = 0; i < n; ++i) {
            if (std::is_signed<T>::value) {
                arr[i] = static_cast<T>(uarr[i] ^ signBit);
            } else {
                arr[i] = static_cast<T>(uarr[i]);
            }
        }
    }

    template<typename T, typename std::enable_if<!std::is_integral<T>::value || std::is_same<T, bool>::value, int>::type = 0>
    static void sortValuesInternal(T* arr, size_t n, bool ascOrder) {
        if (ascOrder) {
            std::sort(arr, arr + n, std::less{});
        } else {
            std::sort(arr, arr + n, std::greater{});
        }
    }

    template<typename T>
    static void sortValues(common::list_entry_t& input, common::list_entry_t& result,
        common::ValueVector& inputVector, common::ValueVector& resultVector, bool ascOrder,
        bool nullFirst) {
        auto inputDataVector = common::ListVector::getDataVector(&inputVector);
        auto inputPos = input.offset;

        // Calculate null count.
        auto nullCount = 0;
        for (auto i = 0u; i < input.size; i++) {
            if (inputDataVector->isNull(input.offset + i)) {
                nullCount += 1;
            }
        }

        result = common::ListVector::addList(&resultVector, input.size);
        auto resultDataVector = common::ListVector::getDataVector(&resultVector);
        auto resultPos = result.offset;

        // Add nulls first.
        if (nullFirst) {
            setVectorRangeToNull(*resultDataVector, result.offset, 0, nullCount);
            resultPos += nullCount;
        }

        // Add actual data.
        for (auto i = 0u; i < input.size; i++) {
            if (inputDataVector->isNull(inputPos)) {
                inputPos++;
                continue;
            }
            resultDataVector->copyFromVectorData(resultPos++, inputDataVector, inputPos++);
        }

        // Add nulls in the end.
        if (!nullFirst) {
            setVectorRangeToNull(*resultDataVector, result.offset, input.size - nullCount,
                input.size);
        }

        // Determine the starting and ending position of the data to be sorted.
        auto sortStart = nullCount;
        auto sortEnd = input.size;
        if (!nullFirst) {
            sortStart = 0;
            sortEnd = input.size - nullCount;
        }

        // Sort the data based on order.
        auto sortingValues =
            reinterpret_cast<T*>(common::ListVector::getListValues(&resultVector, result));
        sortValuesInternal(sortingValues + sortStart, sortEnd - sortStart, ascOrder);
    }

    static void setVectorRangeToNull(common::ValueVector& vector, uint64_t offset,
        uint64_t startPos, uint64_t endPos) {
        for (auto i = startPos; i < endPos; i++) {
            vector.setNull(offset + i, true);
        }
    }
};

} // namespace function
} // namespace kuzu
