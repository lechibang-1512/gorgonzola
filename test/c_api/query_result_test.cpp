#include <fstream>

#include "c_api_test/c_api_test.h"

using namespace gorgonzola::main;
using namespace gorgonzola::common;
using namespace gorgonzola::processor;
using namespace gorgonzola::testing;

class CApiQueryResultTest : public CApiTest {
public:
    std::string getInputDir() override {
        return TestHelper::appendGorgonzolaRootPath("dataset/tinysnb/");
    }
};

static gorgonzola_value* copy_flat_tuple(gorgonzola_flat_tuple* tuple, uint32_t tupleLen) {
    gorgonzola_value* ret = (gorgonzola_value*)malloc(sizeof(gorgonzola_value) * tupleLen);
    for (uint32_t i = 0; i < tupleLen; i++) {
        gorgonzola_flat_tuple_get_value(tuple, i, &ret[i]);
    }
    return ret;
}

TEST_F(CApiQueryResultTest, GetNextExample) {
    auto conn = getConnection();

    gorgonzola_query_result result;
    gorgonzola_connection_query(conn, "MATCH (p:person) RETURN p.*", &result);

    uint64_t num_tuples = gorgonzola_query_result_get_num_tuples(&result);
    gorgonzola_value** tuples = (gorgonzola_value**)malloc(sizeof(gorgonzola_value*) * num_tuples);
    for (uint64_t i = 0; i < num_tuples; ++i) {
        gorgonzola_flat_tuple tuple;
        gorgonzola_query_result_get_next(&result, &tuple);
        tuples[i] = copy_flat_tuple(&tuple, gorgonzola_query_result_get_num_columns(&result));
        gorgonzola_flat_tuple_destroy(&tuple);
    }

    for (uint64_t i = 0; i < num_tuples; ++i) {
        for (uint64_t j = 0; j < gorgonzola_query_result_get_num_columns(&result); ++j) {
            ASSERT_FALSE(gorgonzola_value_is_null(&tuples[i][j]));
            gorgonzola_value_destroy(&tuples[i][j]);
        }
        free(tuples[i]);
    }

    free((void*)tuples);

    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiQueryResultTest, GetErrorMessage) {
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, "MATCH (a:person) RETURN COUNT(*)", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    char* errorMessage = gorgonzola_query_result_get_error_message(&result);
    gorgonzola_query_result_destroy(&result);

    state = gorgonzola_connection_query(connection, "MATCH (a:personnnn) RETURN COUNT(*)", &result);
    ASSERT_EQ(state, GorgonzolaError);
    ASSERT_FALSE(gorgonzola_query_result_is_success(&result));
    errorMessage = gorgonzola_query_result_get_error_message(&result);
    ASSERT_EQ(std::string(errorMessage), "Binder exception: Table personnnn does not exist.");
    gorgonzola_query_result_destroy(&result);
    gorgonzola_destroy_string(errorMessage);
}

TEST_F(CApiQueryResultTest, ToString) {
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, "MATCH (a:person) RETURN COUNT(*)", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    char* str_repr = gorgonzola_query_result_to_string(&result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_destroy_string(str_repr);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiQueryResultTest, ToStringDoesNotExhaustIterator) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple row;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        "MATCH (a:person) RETURN a.fName, a.age ORDER BY a.fName", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));

    // Convert to string first
    char* str_repr = gorgonzola_query_result_to_string(&result);
    gorgonzola_destroy_string(str_repr);

    // We should still be able to iterate the result normally!
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &row);
    ASSERT_EQ(state, GorgonzolaSuccess);
    auto flatTupleCpp = (FlatTuple*)(row._flat_tuple);
    ASSERT_EQ(flatTupleCpp->getValue(0)->getValue<std::string>(), "Alice");
    gorgonzola_flat_tuple_destroy(&row);

    gorgonzola_query_result_destroy(&result);
}


TEST_F(CApiQueryResultTest, GetNumColumns) {
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, "MATCH (a:person) RETURN a.fName, a.age, a.height",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 3);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiQueryResultTest, GetColumnName) {
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, "MATCH (a:person) RETURN a.fName, a.age, a.height",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    char* columnName;
    ASSERT_EQ(gorgonzola_query_result_get_column_name(&result, 0, &columnName), GorgonzolaSuccess);
    ASSERT_EQ(std::string(columnName), "a.fName");
    gorgonzola_destroy_string(columnName);
    ASSERT_EQ(gorgonzola_query_result_get_column_name(&result, 1, &columnName), GorgonzolaSuccess);
    ASSERT_EQ(std::string(columnName), "a.age");
    gorgonzola_destroy_string(columnName);
    ASSERT_EQ(gorgonzola_query_result_get_column_name(&result, 2, &columnName), GorgonzolaSuccess);
    ASSERT_EQ(std::string(columnName), "a.height");
    gorgonzola_destroy_string(columnName);
    ASSERT_EQ(gorgonzola_query_result_get_column_name(&result, 222, &columnName), GorgonzolaError);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiQueryResultTest, GetColumnDataType) {
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, "MATCH (a:person) RETURN a.fName, a.age, a.height",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    gorgonzola_logical_type type;
    ASSERT_EQ(gorgonzola_query_result_get_column_data_type(&result, 0, &type), GorgonzolaSuccess);
    auto typeCpp = (LogicalType*)(type._data_type);
    ASSERT_EQ(typeCpp->getLogicalTypeID(), LogicalTypeID::STRING);
    gorgonzola_data_type_destroy(&type);
    ASSERT_EQ(gorgonzola_query_result_get_column_data_type(&result, 1, &type), GorgonzolaSuccess);
    typeCpp = (LogicalType*)(type._data_type);
    ASSERT_EQ(typeCpp->getLogicalTypeID(), LogicalTypeID::INT64);
    gorgonzola_data_type_destroy(&type);
    ASSERT_EQ(gorgonzola_query_result_get_column_data_type(&result, 2, &type), GorgonzolaSuccess);
    typeCpp = (LogicalType*)(type._data_type);
    ASSERT_EQ(typeCpp->getLogicalTypeID(), LogicalTypeID::FLOAT);
    gorgonzola_data_type_destroy(&type);
    ASSERT_EQ(gorgonzola_query_result_get_column_data_type(&result, 222, &type), GorgonzolaError);
    gorgonzola_query_result_destroy(&result);
}

// TODO(Guodong): Fix this test by adding support of STRUCT in arrow table export.
// TEST_F(CApiQueryResultTest, GetArrowSchema) {
//    auto connection = getConnection();
//    auto result = gorgonzola_connection_query(
//        connection, "MATCH (p:person)-[k:knows]-(q:person) RETURN p.fName, k, q.fName");
//    ASSERT_TRUE(gorgonzola_query_result_is_success(result));
//    auto schema = gorgonzola_query_result_get_arrow_schema(result);
//    ASSERT_STREQ(schema.name, "gorgonzola_query_result");
//    ASSERT_EQ(schema.n_children, 3);
//    ASSERT_STREQ(schema.children[0]->name, "p.fName");
//    ASSERT_STREQ(schema.children[1]->name, "k");
//    ASSERT_STREQ(schema.children[2]->name, "q.fName");
//
//    schema.release(&schema);
//    gorgonzola_query_result_destroy(result);
//}

TEST_F(CApiQueryResultTest, GetQuerySummary) {
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, "MATCH (a:person) RETURN a.fName, a.age, a.height",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    gorgonzola_query_summary summary;
    state = gorgonzola_query_result_get_query_summary(&result, &summary);
    ASSERT_EQ(state, GorgonzolaSuccess);
    auto compilingTime = gorgonzola_query_summary_get_compiling_time(&summary);
    ASSERT_GT(compilingTime, 0);
    auto executionTime = gorgonzola_query_summary_get_execution_time(&summary);
    ASSERT_GT(executionTime, 0);
    gorgonzola_query_summary_destroy(&summary);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiQueryResultTest, GetNext) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple row;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        "MATCH (a:person) RETURN a.fName, a.age ORDER BY a.fName", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));

    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &row);
    ASSERT_EQ(state, GorgonzolaSuccess);
    auto flatTupleCpp = (FlatTuple*)(row._flat_tuple);
    ASSERT_EQ(flatTupleCpp->getValue(0)->getValue<std::string>(), "Alice");
    ASSERT_EQ(flatTupleCpp->getValue(1)->getValue<int64_t>(), 35);

    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &row);
    ASSERT_EQ(state, GorgonzolaSuccess);
    flatTupleCpp = (FlatTuple*)(row._flat_tuple);
    ASSERT_EQ(flatTupleCpp->getValue(0)->getValue<std::string>(), "Bob");
    ASSERT_EQ(flatTupleCpp->getValue(1)->getValue<int64_t>(), 30);
    gorgonzola_flat_tuple_destroy(&row);

    while (gorgonzola_query_result_has_next(&result)) {
        gorgonzola_query_result_get_next(&result, &row);
    }
    ASSERT_FALSE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &row);
    ASSERT_EQ(state, GorgonzolaError);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiQueryResultTest, ResetIterator) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple row;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        "MATCH (a:person) RETURN a.fName, a.age ORDER BY a.fName", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));

    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &row);
    ASSERT_EQ(state, GorgonzolaSuccess);
    auto flatTupleCpp = (FlatTuple*)(row._flat_tuple);
    ASSERT_EQ(flatTupleCpp->getValue(0)->getValue<std::string>(), "Alice");
    ASSERT_EQ(flatTupleCpp->getValue(1)->getValue<int64_t>(), 35);

    gorgonzola_query_result_reset_iterator(&result);

    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &row);
    ASSERT_EQ(state, GorgonzolaSuccess);
    flatTupleCpp = (FlatTuple*)(row._flat_tuple);
    ASSERT_EQ(flatTupleCpp->getValue(0)->getValue<std::string>(), "Alice");
    ASSERT_EQ(flatTupleCpp->getValue(1)->getValue<int64_t>(), 35);
    gorgonzola_flat_tuple_destroy(&row);

    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiQueryResultTest, MultipleQuery) {
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, "return 1; return 2; return 3;", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));

    char* str = gorgonzola_query_result_to_string(&result);
    ASSERT_EQ(std::string(str), "1\n1\n");
    gorgonzola_destroy_string(str);

    ASSERT_TRUE(gorgonzola_query_result_has_next_query_result(&result));
    gorgonzola_query_result next_query_result;
    ASSERT_EQ(gorgonzola_query_result_get_next_query_result(&result, &next_query_result), GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&next_query_result));
    str = gorgonzola_query_result_to_string(&next_query_result);
    ASSERT_EQ(std::string(str), "2\n2\n");
    gorgonzola_destroy_string(str);
    gorgonzola_query_result_destroy(&next_query_result);

    ASSERT_EQ(gorgonzola_query_result_get_next_query_result(&result, &next_query_result), GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&next_query_result));
    str = gorgonzola_query_result_to_string(&next_query_result);
    ASSERT_EQ(std::string(str), "3\n3\n");
    gorgonzola_destroy_string(str);

    ASSERT_FALSE(gorgonzola_query_result_has_next_query_result(&result));
    ASSERT_EQ(gorgonzola_query_result_get_next_query_result(&result, &next_query_result), GorgonzolaError);
    gorgonzola_query_result_destroy(&next_query_result);

    gorgonzola_query_result_destroy(&result);
}
