#include "main/query_result.h"

#include "c_api/helpers.h"
#include "c_api/gorgonzola.h"

using namespace gorgonzola::main;
using namespace gorgonzola::common;
using namespace gorgonzola::processor;

void gorgonzola_query_result_destroy(gorgonzola_query_result* query_result) {
    if (query_result == nullptr) {
        return;
    }
    if (query_result->_query_result != nullptr) {
        if (!query_result->_is_owned_by_cpp) {
            delete static_cast<QueryResult*>(query_result->_query_result);
        }
    }
}

bool gorgonzola_query_result_is_success(gorgonzola_query_result* query_result) {
    return static_cast<QueryResult*>(query_result->_query_result)->isSuccess();
}

char* gorgonzola_query_result_get_error_message(gorgonzola_query_result* query_result) {
    auto error_message = static_cast<QueryResult*>(query_result->_query_result)->getErrorMessage();
    if (error_message.empty()) {
        return nullptr;
    }
    return convertToOwnedCString(error_message);
}

uint64_t gorgonzola_query_result_get_num_columns(gorgonzola_query_result* query_result) {
    return static_cast<QueryResult*>(query_result->_query_result)->getNumColumns();
}

gorgonzola_state gorgonzola_query_result_get_column_name(gorgonzola_query_result* query_result, uint64_t index,
    char** out_column_name) {
    auto column_names = static_cast<QueryResult*>(query_result->_query_result)->getColumnNames();
    if (index >= column_names.size()) {
        return GorgonzolaError;
    }
    *out_column_name = convertToOwnedCString(column_names[index]);
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_query_result_get_column_data_type(gorgonzola_query_result* query_result, uint64_t index,
    gorgonzola_logical_type* out_column_data_type) {
    auto column_data_types =
        static_cast<QueryResult*>(query_result->_query_result)->getColumnDataTypes();
    if (index >= column_data_types.size()) {
        return GorgonzolaError;
    }
    const auto& column_data_type = column_data_types[index];
    out_column_data_type->_data_type = new LogicalType(column_data_type.copy());
    return GorgonzolaSuccess;
}

uint64_t gorgonzola_query_result_get_num_tuples(gorgonzola_query_result* query_result) {
    return static_cast<QueryResult*>(query_result->_query_result)->getNumTuples();
}

gorgonzola_state gorgonzola_query_result_get_query_summary(gorgonzola_query_result* query_result,
    gorgonzola_query_summary* out_query_summary) {
    if (out_query_summary == nullptr) {
        return GorgonzolaError;
    }
    auto query_summary = static_cast<QueryResult*>(query_result->_query_result)->getQuerySummary();
    out_query_summary->_query_summary = query_summary;
    return GorgonzolaSuccess;
}

bool gorgonzola_query_result_has_next(gorgonzola_query_result* query_result) {
    return static_cast<QueryResult*>(query_result->_query_result)->hasNext();
}

bool gorgonzola_query_result_has_next_query_result(gorgonzola_query_result* query_result) {
    return static_cast<QueryResult*>(query_result->_query_result)->hasNextQueryResult();
}

gorgonzola_state gorgonzola_query_result_get_next_query_result(gorgonzola_query_result* query_result,
    gorgonzola_query_result* out_query_result) {
    if (!gorgonzola_query_result_has_next_query_result(query_result)) {
        return GorgonzolaError;
    }
    auto next_query_result =
        static_cast<QueryResult*>(query_result->_query_result)->getNextQueryResult();
    if (next_query_result == nullptr) {
        return GorgonzolaError;
    }
    out_query_result->_query_result = next_query_result;
    out_query_result->_is_owned_by_cpp = true;
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_query_result_get_next(gorgonzola_query_result* query_result,
    gorgonzola_flat_tuple* out_flat_tuple) {
    try {
        auto flat_tuple = static_cast<QueryResult*>(query_result->_query_result)->getNext();
        out_flat_tuple->_flat_tuple = flat_tuple.get();
        out_flat_tuple->_is_owned_by_cpp = true;
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

char* gorgonzola_query_result_to_string(gorgonzola_query_result* query_result) {
    auto* qr = static_cast<QueryResult*>(query_result->_query_result);
    auto savedIterPos = qr->getNextTupleIdx();
    std::string result_string = qr->toString();
    qr->resetIteratorTo(savedIterPos);
    return convertToOwnedCString(result_string);
}

void gorgonzola_query_result_reset_iterator(gorgonzola_query_result* query_result) {
    static_cast<QueryResult*>(query_result->_query_result)->resetIterator();
}

gorgonzola_state gorgonzola_query_result_get_arrow_schema(gorgonzola_query_result* query_result,
    ArrowSchema* out_schema) {
    try {
        *out_schema = *static_cast<QueryResult*>(query_result->_query_result)->getArrowSchema();
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_query_result_get_next_arrow_chunk(gorgonzola_query_result* query_result,
    int64_t chunk_size, ArrowArray* out_arrow_array) {
    try {
        *out_arrow_array =
            *static_cast<QueryResult*>(query_result->_query_result)->getNextArrowChunk(chunk_size);
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}
