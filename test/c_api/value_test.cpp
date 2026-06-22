#include "c_api_test/c_api_test.h"

using namespace gorgonzola::main;
using namespace gorgonzola::common;
using namespace gorgonzola::testing;

class CApiValueTest : public CApiTest {
public:
    std::string getInputDir() override {
        return TestHelper::appendGorgonzolaRootPath("dataset/tinysnb/");
    }
};

TEST(CApiValueTestEmptyDB, CreateNull) {
    gorgonzola_value* value = gorgonzola_value_create_null();
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::ANY);
    ASSERT_EQ(cppValue->isNull(), true);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateNullWithDatatype) {
    gorgonzola_logical_type type;
    gorgonzola_data_type_create(gorgonzola_data_type_id::GORGONZOLA_INT64, nullptr, 0, &type);
    gorgonzola_value* value = gorgonzola_value_create_null_with_data_type(&type);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    gorgonzola_data_type_destroy(&type);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::INT64);
    ASSERT_EQ(cppValue->isNull(), true);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, IsNull) {
    gorgonzola_value* value = gorgonzola_value_create_int64(123);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(value));
    gorgonzola_value_destroy(value);
    value = gorgonzola_value_create_null();
    ASSERT_TRUE(gorgonzola_value_is_null(value));
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, SetNull) {
    gorgonzola_value* value = gorgonzola_value_create_int64(123);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(value));
    gorgonzola_value_set_null(value, true);
    ASSERT_TRUE(gorgonzola_value_is_null(value));
    gorgonzola_value_set_null(value, false);
    ASSERT_FALSE(gorgonzola_value_is_null(value));
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateDefault) {
    gorgonzola_logical_type type;
    gorgonzola_data_type_create(gorgonzola_data_type_id::GORGONZOLA_INT64, nullptr, 0, &type);
    gorgonzola_value* value = gorgonzola_value_create_default(&type);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    gorgonzola_data_type_destroy(&type);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_FALSE(gorgonzola_value_is_null(value));
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::INT64);
    ASSERT_EQ(cppValue->getValue<int64_t>(), 0);
    gorgonzola_value_destroy(value);

    gorgonzola_data_type_create(gorgonzola_data_type_id::GORGONZOLA_STRING, nullptr, 0, &type);
    value = gorgonzola_value_create_default(&type);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    gorgonzola_data_type_destroy(&type);
    cppValue = static_cast<Value*>(value->_value);
    ASSERT_FALSE(gorgonzola_value_is_null(value));
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::STRING);
    ASSERT_EQ(cppValue->getValue<std::string>(), "");
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateBool) {
    gorgonzola_value* value = gorgonzola_value_create_bool(true);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::BOOL);
    ASSERT_EQ(cppValue->getValue<bool>(), true);
    gorgonzola_value_destroy(value);

    value = gorgonzola_value_create_bool(false);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::BOOL);
    ASSERT_EQ(cppValue->getValue<bool>(), false);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateInt8) {
    gorgonzola_value* value = gorgonzola_value_create_int8(12);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::INT8);
    ASSERT_EQ(cppValue->getValue<int8_t>(), 12);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateInt16) {
    gorgonzola_value* value = gorgonzola_value_create_int16(123);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::INT16);
    ASSERT_EQ(cppValue->getValue<int16_t>(), 123);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateInt32) {
    gorgonzola_value* value = gorgonzola_value_create_int32(123);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::INT32);
    ASSERT_EQ(cppValue->getValue<int32_t>(), 123);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateInt64) {
    gorgonzola_value* value = gorgonzola_value_create_int64(123);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::INT64);
    ASSERT_EQ(cppValue->getValue<int64_t>(), 123);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateUInt8) {
    gorgonzola_value* value = gorgonzola_value_create_uint8(12);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::UINT8);
    ASSERT_EQ(cppValue->getValue<uint8_t>(), 12);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateUInt16) {
    gorgonzola_value* value = gorgonzola_value_create_uint16(123);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::UINT16);
    ASSERT_EQ(cppValue->getValue<uint16_t>(), 123);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateUInt32) {
    gorgonzola_value* value = gorgonzola_value_create_uint32(123);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::UINT32);
    ASSERT_EQ(cppValue->getValue<uint32_t>(), 123);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateUInt64) {
    gorgonzola_value* value = gorgonzola_value_create_uint64(123);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::UINT64);
    ASSERT_EQ(cppValue->getValue<uint64_t>(), 123);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateINT128) {
    gorgonzola_value* value = gorgonzola_value_create_int128(gorgonzola_int128_t{211111111, 100000000});
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::INT128);
    auto cppTimeStamp = cppValue->getValue<int128_t>();
    ASSERT_EQ(cppTimeStamp.high, 100000000);
    ASSERT_EQ(cppTimeStamp.low, 211111111);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateFloat) {
    gorgonzola_value* value = gorgonzola_value_create_float(123.456);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::FLOAT);
    ASSERT_FLOAT_EQ(cppValue->getValue<float>(), 123.456);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateDouble) {
    gorgonzola_value* value = gorgonzola_value_create_double(123.456);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::DOUBLE);
    ASSERT_DOUBLE_EQ(cppValue->getValue<double>(), 123.456);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateInternalID) {
    auto internalID = gorgonzola_internal_id_t{1, 123};
    gorgonzola_value* value = gorgonzola_value_create_internal_id(internalID);
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::INTERNAL_ID);
    auto internalIDCpp = cppValue->getValue<internalID_t>();
    ASSERT_EQ(internalIDCpp.tableID, 1);
    ASSERT_EQ(internalIDCpp.offset, 123);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateDate) {
    gorgonzola_value* value = gorgonzola_value_create_date(gorgonzola_date_t{123});
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::DATE);
    auto cppDate = cppValue->getValue<date_t>();
    ASSERT_EQ(cppDate.days, 123);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateTimeStamp) {
    gorgonzola_value* value = gorgonzola_value_create_timestamp(gorgonzola_timestamp_t{123});
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::TIMESTAMP);
    auto cppTimeStamp = cppValue->getValue<timestamp_t>();
    ASSERT_EQ(cppTimeStamp.value, 123);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateTimeStampNonStandard) {
    gorgonzola_value* value_ns = gorgonzola_value_create_timestamp_ns(gorgonzola_timestamp_ns_t{12345});
    gorgonzola_value* value_ms = gorgonzola_value_create_timestamp_ms(gorgonzola_timestamp_ms_t{123456});
    gorgonzola_value* value_sec = gorgonzola_value_create_timestamp_sec(gorgonzola_timestamp_sec_t{1234567});
    gorgonzola_value* value_tz = gorgonzola_value_create_timestamp_tz(gorgonzola_timestamp_tz_t{12345678});

    ASSERT_FALSE(value_ns->_is_owned_by_cpp);
    ASSERT_FALSE(value_ms->_is_owned_by_cpp);
    ASSERT_FALSE(value_sec->_is_owned_by_cpp);
    ASSERT_FALSE(value_tz->_is_owned_by_cpp);
    auto cppValue_ns = static_cast<Value*>(value_ns->_value);
    auto cppValue_ms = static_cast<Value*>(value_ms->_value);
    auto cppValue_sec = static_cast<Value*>(value_sec->_value);
    auto cppValue_tz = static_cast<Value*>(value_tz->_value);
    ASSERT_EQ(cppValue_ns->getDataType().getLogicalTypeID(), LogicalTypeID::TIMESTAMP_NS);
    ASSERT_EQ(cppValue_ms->getDataType().getLogicalTypeID(), LogicalTypeID::TIMESTAMP_MS);
    ASSERT_EQ(cppValue_sec->getDataType().getLogicalTypeID(), LogicalTypeID::TIMESTAMP_SEC);
    ASSERT_EQ(cppValue_tz->getDataType().getLogicalTypeID(), LogicalTypeID::TIMESTAMP_TZ);

    auto cppTimeStamp_ns = cppValue_ns->getValue<timestamp_ns_t>();
    auto cppTimeStamp_ms = cppValue_ms->getValue<timestamp_ms_t>();
    auto cppTimeStamp_sec = cppValue_sec->getValue<timestamp_sec_t>();
    auto cppTimeStamp_tz = cppValue_tz->getValue<timestamp_tz_t>();
    ASSERT_EQ(cppTimeStamp_ns.value, 12345);
    ASSERT_EQ(cppTimeStamp_ms.value, 123456);
    ASSERT_EQ(cppTimeStamp_sec.value, 1234567);
    ASSERT_EQ(cppTimeStamp_tz.value, 12345678);
    gorgonzola_value_destroy(value_ns);
    gorgonzola_value_destroy(value_ms);
    gorgonzola_value_destroy(value_sec);
    gorgonzola_value_destroy(value_tz);
}

TEST(CApiValueTestEmptyDB, CreateInterval) {
    gorgonzola_value* value = gorgonzola_value_create_interval(gorgonzola_interval_t{12, 3, 300});
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::INTERVAL);
    auto cppTimeStamp = cppValue->getValue<interval_t>();
    ASSERT_EQ(cppTimeStamp.months, 12);
    ASSERT_EQ(cppTimeStamp.days, 3);
    ASSERT_EQ(cppTimeStamp.micros, 300);
    gorgonzola_value_destroy(value);
}

TEST(CApiValueTestEmptyDB, CreateString) {
    gorgonzola_value* value = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::STRING);
    ASSERT_EQ(cppValue->getValue<std::string>(), "abcdefg");
    gorgonzola_value_destroy(value);
}

TEST_F(CApiValueTest, CreateList) {
    auto connection = getConnection();
    gorgonzola_value* value1 = gorgonzola_value_create_int64(123);
    gorgonzola_value* value2 = gorgonzola_value_create_int64(456);
    gorgonzola_value* value3 = gorgonzola_value_create_int64(789);
    gorgonzola_value* value4 = gorgonzola_value_create_int64(101112);
    gorgonzola_value* value5 = gorgonzola_value_create_int64(131415);
    gorgonzola_value* elements[] = {value1, value2, value3, value4, value5};
    gorgonzola_value* value = nullptr;
    gorgonzola_state state = gorgonzola_value_create_list(5, elements, &value);
    ASSERT_EQ(state, GorgonzolaSuccess);
    // Destroy the original values, the list should still be valid
    for (int i = 0; i < 5; ++i) {
        gorgonzola_value_destroy(elements[i]);
    }
    ASSERT_FALSE(value->_is_owned_by_cpp);
    gorgonzola_prepared_statement stmt;
    state = gorgonzola_connection_prepare(connection, (char*)"RETURN $1", &stmt);
    ASSERT_EQ(state, GorgonzolaSuccess);
    state = gorgonzola_prepared_statement_bind_value(&stmt, "1", value);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_query_result result;
    state = gorgonzola_connection_execute(connection, &stmt, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    gorgonzola_flat_tuple flatTuple;
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value outValue;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &outValue), GorgonzolaSuccess);
    ASSERT_TRUE(outValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&outValue));
    uint64_t size;
    ASSERT_EQ(gorgonzola_value_get_list_size(&outValue, &size), GorgonzolaSuccess);
    ASSERT_EQ(size, 5);
    gorgonzola_value listElement;
    ASSERT_EQ(gorgonzola_value_get_list_element(&outValue, 0, &listElement), GorgonzolaSuccess);
    ASSERT_TRUE(listElement._is_owned_by_cpp);
    int64_t int64Result;
    ASSERT_EQ(gorgonzola_value_get_int64(&listElement, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 123);
    gorgonzola_value_destroy(&listElement);
    ASSERT_EQ(gorgonzola_value_get_list_element(&outValue, 1, &listElement), GorgonzolaSuccess);
    ASSERT_TRUE(listElement._is_owned_by_cpp);
    ASSERT_EQ(gorgonzola_value_get_int64(&listElement, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 456);
    gorgonzola_value_destroy(&listElement);
    ASSERT_EQ(gorgonzola_value_get_list_element(&outValue, 2, &listElement), GorgonzolaSuccess);
    ASSERT_TRUE(listElement._is_owned_by_cpp);
    ASSERT_EQ(gorgonzola_value_get_int64(&listElement, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 789);
    gorgonzola_value_destroy(&listElement);
    ASSERT_EQ(gorgonzola_value_get_list_element(&outValue, 3, &listElement), GorgonzolaSuccess);
    ASSERT_TRUE(listElement._is_owned_by_cpp);
    ASSERT_EQ(gorgonzola_value_get_int64(&listElement, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 101112);
    gorgonzola_value_destroy(&listElement);
    ASSERT_EQ(gorgonzola_value_get_list_element(&outValue, 4, &listElement), GorgonzolaSuccess);
    ASSERT_TRUE(listElement._is_owned_by_cpp);
    ASSERT_EQ(gorgonzola_value_get_int64(&listElement, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 131415);
    gorgonzola_value_destroy(&listElement);
    gorgonzola_value_destroy(&outValue);
    gorgonzola_value_destroy(value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&stmt);
}

TEST(CApiValueTestEmptyDB, CreateListDifferentTypes) {
    gorgonzola_value* value1 = gorgonzola_value_create_int64(123);
    gorgonzola_value* value2 = gorgonzola_value_create_string((char*)"abcdefg");
    gorgonzola_value* elements[] = {value1, value2};
    gorgonzola_value* value = nullptr;
    gorgonzola_state state = gorgonzola_value_create_list(2, elements, &value);
    ASSERT_EQ(state, GorgonzolaError);
    gorgonzola_value_destroy(value1);
    gorgonzola_value_destroy(value2);
}

TEST(CApiValueTestEmptyDB, CreateListEmpty) {
    gorgonzola_value* elements[] = {nullptr}; // Must be non-empty
    gorgonzola_value* value = nullptr;
    gorgonzola_state state = gorgonzola_value_create_list(0, elements, &value);
    ASSERT_EQ(state, GorgonzolaError);
}

TEST_F(CApiValueTest, CreateListNested) {
    auto connection = getConnection();
    gorgonzola_value* value1 = gorgonzola_value_create_int64(123);
    gorgonzola_value* value2 = gorgonzola_value_create_int64(456);
    gorgonzola_value* value3 = gorgonzola_value_create_int64(789);
    gorgonzola_value* value4 = gorgonzola_value_create_int64(101112);
    gorgonzola_value* value5 = gorgonzola_value_create_int64(131415);
    gorgonzola_value* elements1[] = {value1, value2, value3};
    gorgonzola_value* elements2[] = {value4, value5};
    gorgonzola_value* list1 = nullptr;
    gorgonzola_value* list2 = nullptr;
    gorgonzola_value_create_list(3, elements1, &list1);
    ASSERT_FALSE(list1->_is_owned_by_cpp);
    gorgonzola_value_create_list(2, elements2, &list2);
    ASSERT_FALSE(list2->_is_owned_by_cpp);
    gorgonzola_value* elements[] = {list1, list2};
    gorgonzola_value* nestedList = nullptr;
    gorgonzola_state state = gorgonzola_value_create_list(2, elements, &nestedList);
    ASSERT_EQ(state, GorgonzolaSuccess);
    // Destroy the original values, the list should still be valid
    for (int i = 0; i < 3; ++i) {
        gorgonzola_value_destroy(elements1[i]);
    }
    for (int i = 0; i < 2; ++i) {
        gorgonzola_value_destroy(elements2[i]);
    }
    gorgonzola_value_destroy(list1);
    gorgonzola_value_destroy(list2);
    ASSERT_FALSE(nestedList->_is_owned_by_cpp);
    gorgonzola_prepared_statement stmt;
    state = gorgonzola_connection_prepare(connection, (char*)"RETURN $1", &stmt);
    ASSERT_EQ(state, GorgonzolaSuccess);
    state = gorgonzola_prepared_statement_bind_value(&stmt, "1", nestedList);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_query_result result;
    state = gorgonzola_connection_execute(connection, &stmt, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    gorgonzola_flat_tuple flatTuple;
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value outValue;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &outValue), GorgonzolaSuccess);
    ASSERT_TRUE(outValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&outValue));
    uint64_t size;
    ASSERT_EQ(gorgonzola_value_get_list_size(&outValue, &size), GorgonzolaSuccess);
    ASSERT_EQ(size, 2);
    gorgonzola_value listElement;
    ASSERT_EQ(gorgonzola_value_get_list_element(&outValue, 0, &listElement), GorgonzolaSuccess);
    ASSERT_TRUE(listElement._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&listElement));
    ASSERT_EQ(gorgonzola_value_get_list_size(&listElement, &size), GorgonzolaSuccess);
    ASSERT_EQ(size, 3);
    gorgonzola_value innerListElement;
    ASSERT_EQ(gorgonzola_value_get_list_element(&listElement, 0, &innerListElement), GorgonzolaSuccess);
    ASSERT_TRUE(innerListElement._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&innerListElement));
    int64_t int64Result;
    ASSERT_EQ(gorgonzola_value_get_int64(&innerListElement, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 123);
    gorgonzola_value_destroy(&innerListElement);
    ASSERT_EQ(gorgonzola_value_get_list_element(&listElement, 1, &innerListElement), GorgonzolaSuccess);
    ASSERT_TRUE(innerListElement._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&innerListElement));
    ASSERT_EQ(gorgonzola_value_get_int64(&innerListElement, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 456);
    gorgonzola_value_destroy(&innerListElement);
    ASSERT_EQ(gorgonzola_value_get_list_element(&listElement, 2, &innerListElement), GorgonzolaSuccess);
    ASSERT_TRUE(innerListElement._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&innerListElement));
    ASSERT_EQ(gorgonzola_value_get_int64(&innerListElement, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 789);
    gorgonzola_value_destroy(&innerListElement);
    gorgonzola_value_destroy(&listElement);
    ASSERT_EQ(gorgonzola_value_get_list_element(&outValue, 1, &listElement), GorgonzolaSuccess);
    ASSERT_TRUE(listElement._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&listElement));
    ASSERT_EQ(gorgonzola_value_get_list_size(&listElement, &size), GorgonzolaSuccess);
    ASSERT_EQ(size, 2);
    gorgonzola_value_destroy(&listElement);
    gorgonzola_value_destroy(&outValue);
    gorgonzola_value_destroy(nestedList);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&stmt);
}

TEST_F(CApiValueTest, CreateStruct) {
    auto connection = getConnection();
    gorgonzola_value* value1 = gorgonzola_value_create_int16(32);
    gorgonzola_value* value2 = gorgonzola_value_create_string((char*)"Wong");
    gorgonzola_value* value3 = gorgonzola_value_create_string((char*)"Kelley");
    gorgonzola_value* value4 = gorgonzola_value_create_int64(123456);
    gorgonzola_value* value5 = gorgonzola_value_create_string((char*)"CEO");
    gorgonzola_value* value6 = gorgonzola_value_create_bool(true);
    gorgonzola_value* employmentElements[] = {value5, value6};
    const char* employmentFieldNames[] = {(char*)"title", (char*)"is_current"};
    gorgonzola_value* employment = nullptr;
    gorgonzola_state state =
        gorgonzola_value_create_struct(2, employmentFieldNames, employmentElements, &employment);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_FALSE(employment->_is_owned_by_cpp);
    gorgonzola_value_destroy(value5);
    gorgonzola_value_destroy(value6);
    gorgonzola_value* personElements[] = {value1, value2, value3, value4, employment};
    const char* personFieldNames[] = {(char*)"age", (char*)"first_name", (char*)"last_name",
        (char*)"id", (char*)"employment"};
    gorgonzola_value* person = nullptr;
    state = gorgonzola_value_create_struct(5, personFieldNames, personElements, &person);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value_destroy(value1);
    gorgonzola_value_destroy(value2);
    gorgonzola_value_destroy(value3);
    gorgonzola_value_destroy(value4);
    gorgonzola_value_destroy(employment);
    ASSERT_FALSE(person->_is_owned_by_cpp);
    gorgonzola_prepared_statement stmt;
    state = gorgonzola_connection_prepare(connection, (char*)"RETURN $1", &stmt);
    ASSERT_EQ(state, GorgonzolaSuccess);
    state = gorgonzola_prepared_statement_bind_value(&stmt, "1", person);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_query_result result;
    state = gorgonzola_connection_execute(connection, &stmt, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    gorgonzola_flat_tuple flatTuple;
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value outValue;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &outValue), GorgonzolaSuccess);
    ASSERT_TRUE(outValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&outValue));
    uint64_t size;
    state = gorgonzola_value_get_struct_num_fields(&outValue, &size);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(size, 5);
    char* structFieldName;
    gorgonzola_value structFieldValue;
    state = gorgonzola_value_get_struct_field_name(&outValue, 0, &structFieldName);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_STREQ(structFieldName, "age");
    state = gorgonzola_value_get_struct_field_value(&outValue, 0, &structFieldValue);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(structFieldValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&structFieldValue));
    int16_t int16Result;
    state = gorgonzola_value_get_int16(&structFieldValue, &int16Result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(int16Result, 32);
    gorgonzola_value_destroy(&structFieldValue);
    gorgonzola_destroy_string(structFieldName);
    state = gorgonzola_value_get_struct_field_name(&outValue, 1, &structFieldName);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_STREQ(structFieldName, "first_name");
    state = gorgonzola_value_get_struct_field_value(&outValue, 1, &structFieldValue);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(structFieldValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&structFieldValue));
    char* stringResult;
    state = gorgonzola_value_get_string(&structFieldValue, &stringResult);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_STREQ(stringResult, "Wong");
    gorgonzola_value_destroy(&structFieldValue);
    gorgonzola_destroy_string(structFieldName);
    gorgonzola_destroy_string(stringResult);
    state = gorgonzola_value_get_struct_field_name(&outValue, 2, &structFieldName);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_STREQ(structFieldName, "last_name");
    state = gorgonzola_value_get_struct_field_value(&outValue, 2, &structFieldValue);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(structFieldValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&structFieldValue));
    state = gorgonzola_value_get_string(&structFieldValue, &stringResult);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_STREQ(stringResult, "Kelley");
    gorgonzola_value_destroy(&structFieldValue);
    gorgonzola_destroy_string(structFieldName);
    gorgonzola_destroy_string(stringResult);
    state = gorgonzola_value_get_struct_field_name(&outValue, 3, &structFieldName);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_STREQ(structFieldName, "id");
    state = gorgonzola_value_get_struct_field_value(&outValue, 3, &structFieldValue);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(structFieldValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&structFieldValue));
    int64_t int64Result;
    state = gorgonzola_value_get_int64(&structFieldValue, &int64Result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 123456);
    gorgonzola_value_destroy(&structFieldValue);
    gorgonzola_destroy_string(structFieldName);
    state = gorgonzola_value_get_struct_field_name(&outValue, 4, &structFieldName);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_STREQ(structFieldName, "employment");
    state = gorgonzola_value_get_struct_field_value(&outValue, 4, &structFieldValue);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(structFieldValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&structFieldValue));
    state = gorgonzola_value_get_struct_num_fields(&structFieldValue, &size);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(size, 2);
    char* employmentFieldName;
    gorgonzola_value employmentFieldValue;
    state = gorgonzola_value_get_struct_field_name(&structFieldValue, 0, &employmentFieldName);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_STREQ(employmentFieldName, "title");
    state = gorgonzola_value_get_struct_field_value(&structFieldValue, 0, &employmentFieldValue);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(employmentFieldValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&employmentFieldValue));
    state = gorgonzola_value_get_string(&employmentFieldValue, &stringResult);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_STREQ(stringResult, "CEO");
    gorgonzola_value_destroy(&employmentFieldValue);
    gorgonzola_destroy_string(employmentFieldName);
    gorgonzola_destroy_string(stringResult);
    state = gorgonzola_value_get_struct_field_name(&structFieldValue, 1, &employmentFieldName);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_STREQ(employmentFieldName, "is_current");
    state = gorgonzola_value_get_struct_field_value(&structFieldValue, 1, &employmentFieldValue);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(employmentFieldValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&employmentFieldValue));
    bool boolResult;
    state = gorgonzola_value_get_bool(&employmentFieldValue, &boolResult);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(boolResult, true);
    gorgonzola_value_destroy(&employmentFieldValue);
    gorgonzola_destroy_string(employmentFieldName);
    gorgonzola_value_destroy(&structFieldValue);
    gorgonzola_destroy_string(structFieldName);
    gorgonzola_value_destroy(&outValue);
    gorgonzola_value_destroy(person);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&stmt);
}

TEST(CApiValueTestEmptyDB, CreateStructEmpty) {
    const char* fieldNames[] = {(char*)"name"}; // Must be non-empty
    gorgonzola_value* values[] = {nullptr};           // Must be non-empty
    gorgonzola_value* value = nullptr;
    gorgonzola_state state = gorgonzola_value_create_struct(0, fieldNames, values, &value);
    ASSERT_EQ(state, GorgonzolaError);
}

TEST_F(CApiValueTest, CreateMap) {
    auto connection = getConnection();
    gorgonzola_value* key1 = gorgonzola_value_create_int64(1);
    gorgonzola_value* value1 = gorgonzola_value_create_string((char*)"one");
    gorgonzola_value* key2 = gorgonzola_value_create_int64(2);
    gorgonzola_value* value2 = gorgonzola_value_create_string((char*)"two");
    gorgonzola_value* key3 = gorgonzola_value_create_int64(3);
    gorgonzola_value* value3 = gorgonzola_value_create_string((char*)"three");
    gorgonzola_value* keys[] = {key1, key2, key3};
    gorgonzola_value* values[] = {value1, value2, value3};
    gorgonzola_value* map = nullptr;
    gorgonzola_state state = gorgonzola_value_create_map(3, keys, values, &map);
    ASSERT_EQ(state, GorgonzolaSuccess);
    // Destroy the original values, the map should still be valid
    for (int i = 0; i < 3; ++i) {
        gorgonzola_value_destroy(keys[i]);
        gorgonzola_value_destroy(values[i]);
    }
    ASSERT_FALSE(map->_is_owned_by_cpp);
    gorgonzola_prepared_statement stmt;
    state = gorgonzola_connection_prepare(connection, (char*)"RETURN $1", &stmt);
    ASSERT_EQ(state, GorgonzolaSuccess);
    state = gorgonzola_prepared_statement_bind_value(&stmt, "1", map);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_query_result result;
    state = gorgonzola_connection_execute(connection, &stmt, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    gorgonzola_flat_tuple flatTuple;
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value outValue;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &outValue), GorgonzolaSuccess);
    ASSERT_TRUE(outValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&outValue));
    uint64_t size;
    ASSERT_EQ(gorgonzola_value_get_map_size(&outValue, &size), GorgonzolaSuccess);
    ASSERT_EQ(size, 3);
    gorgonzola_value mapValue;
    ASSERT_EQ(gorgonzola_value_get_map_value(&outValue, 0, &mapValue), GorgonzolaSuccess);
    ASSERT_TRUE(mapValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&mapValue));
    char* stringResult;
    ASSERT_EQ(gorgonzola_value_get_string(&mapValue, &stringResult), GorgonzolaSuccess);
    ASSERT_STREQ(stringResult, "one");
    gorgonzola_value_destroy(&mapValue);
    gorgonzola_destroy_string(stringResult);
    ASSERT_EQ(gorgonzola_value_get_map_value(&outValue, 1, &mapValue), GorgonzolaSuccess);
    ASSERT_TRUE(mapValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&mapValue));
    ASSERT_EQ(gorgonzola_value_get_string(&mapValue, &stringResult), GorgonzolaSuccess);
    ASSERT_STREQ(stringResult, "two");
    gorgonzola_value_destroy(&mapValue);
    gorgonzola_destroy_string(stringResult);
    ASSERT_EQ(gorgonzola_value_get_map_value(&outValue, 2, &mapValue), GorgonzolaSuccess);
    ASSERT_TRUE(mapValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&mapValue));
    ASSERT_EQ(gorgonzola_value_get_string(&mapValue, &stringResult), GorgonzolaSuccess);
    ASSERT_STREQ(stringResult, "three");
    gorgonzola_value_destroy(&mapValue);
    gorgonzola_destroy_string(stringResult);
    ASSERT_EQ(gorgonzola_value_get_map_key(&outValue, 0, &mapValue), GorgonzolaSuccess);
    ASSERT_TRUE(mapValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&mapValue));
    int64_t int64Result;
    ASSERT_EQ(gorgonzola_value_get_int64(&mapValue, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 1);
    gorgonzola_value_destroy(&mapValue);
    ASSERT_EQ(gorgonzola_value_get_map_key(&outValue, 1, &mapValue), GorgonzolaSuccess);
    ASSERT_TRUE(mapValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&mapValue));
    ASSERT_EQ(gorgonzola_value_get_int64(&mapValue, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 2);
    gorgonzola_value_destroy(&mapValue);
    ASSERT_EQ(gorgonzola_value_get_map_key(&outValue, 2, &mapValue), GorgonzolaSuccess);
    ASSERT_TRUE(mapValue._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&mapValue));
    ASSERT_EQ(gorgonzola_value_get_int64(&mapValue, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 3);
    gorgonzola_value_destroy(&mapValue);
    gorgonzola_value_destroy(&outValue);
    gorgonzola_value_destroy(map);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&stmt);
}

TEST(CApiValueTestEmptyDB, CreateMapEmpty) {
    gorgonzola_value* keys[] = {nullptr};   // Must be non-empty
    gorgonzola_value* values[] = {nullptr}; // Must be non-empty
    gorgonzola_value* map = nullptr;
    gorgonzola_state state = gorgonzola_value_create_map(0, keys, values, &map);
    ASSERT_EQ(state, GorgonzolaError);
}

TEST(CApiValueTestEmptyDB, Clone) {
    gorgonzola_value* value = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_FALSE(value->_is_owned_by_cpp);
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::STRING);
    ASSERT_EQ(cppValue->getValue<std::string>(), "abcdefg");

    gorgonzola_value* clone = gorgonzola_value_clone(value);
    gorgonzola_value_destroy(value);

    ASSERT_FALSE(clone->_is_owned_by_cpp);
    auto cppClone = static_cast<Value*>(clone->_value);
    ASSERT_EQ(cppClone->getDataType().getLogicalTypeID(), LogicalTypeID::STRING);
    ASSERT_EQ(cppClone->getValue<std::string>(), "abcdefg");
    gorgonzola_value_destroy(clone);
}

TEST(CApiValueTestEmptyDB, Copy) {
    gorgonzola_value* value = gorgonzola_value_create_string((char*)"abc");

    gorgonzola_value* value2 = gorgonzola_value_create_string((char*)"abcdefg");
    gorgonzola_value_copy(value, value2);
    gorgonzola_value_destroy(value2);

    ASSERT_FALSE(gorgonzola_value_is_null(value));
    auto cppValue = static_cast<Value*>(value->_value);
    ASSERT_EQ(cppValue->getDataType().getLogicalTypeID(), LogicalTypeID::STRING);
    ASSERT_EQ(cppValue->getValue<std::string>(), "abcdefg");
    gorgonzola_value_destroy(value);
}

TEST_F(CApiValueTest, GetListSize) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.workedHours ORDER BY a.ID", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    uint64_t size;
    ASSERT_EQ(gorgonzola_value_get_list_size(&value, &size), GorgonzolaSuccess);
    ASSERT_EQ(size, 2);

    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_list_size(badValue, &size), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetListElement) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.workedHours ORDER BY a.ID", &result);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    uint64_t size;
    ASSERT_EQ(gorgonzola_value_get_list_size(&value, &size), GorgonzolaSuccess);
    ASSERT_EQ(size, 2);

    gorgonzola_value listElement;
    ASSERT_EQ(gorgonzola_value_get_list_element(&value, 0, &listElement), GorgonzolaSuccess);
    ASSERT_TRUE(listElement._is_owned_by_cpp);
    int64_t int64Result;
    ASSERT_EQ(gorgonzola_value_get_int64(&listElement, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 10);

    ASSERT_EQ(gorgonzola_value_get_list_element(&value, 1, &listElement), GorgonzolaSuccess);
    ASSERT_TRUE(listElement._is_owned_by_cpp);
    ASSERT_EQ(gorgonzola_value_get_int64(&listElement, &int64Result), GorgonzolaSuccess);
    ASSERT_EQ(int64Result, 5);
    gorgonzola_value_destroy(&listElement);

    ASSERT_EQ(gorgonzola_value_get_list_element(&value, 222, &listElement), GorgonzolaError);

    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiValueTest, GetStructNumFields) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (m:movies) WHERE m.name=\"Roma\" RETURN m.description", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value);
    uint64_t numFields;
    ASSERT_EQ(gorgonzola_value_get_struct_num_fields(&value, &numFields), GorgonzolaSuccess);
    ASSERT_EQ(numFields, 14);

    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_struct_num_fields(badValue, &numFields), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetStructFieldName) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (m:movies) WHERE m.name=\"Roma\" RETURN m.description", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    char* fieldName;
    ASSERT_EQ(gorgonzola_value_get_struct_field_name(&value, 0, &fieldName), GorgonzolaSuccess);
    ASSERT_STREQ(fieldName, "rating");
    gorgonzola_destroy_string(fieldName);

    ASSERT_EQ(gorgonzola_value_get_struct_field_name(&value, 1, &fieldName), GorgonzolaSuccess);
    ASSERT_STREQ(fieldName, "stars");
    gorgonzola_destroy_string(fieldName);

    ASSERT_EQ(gorgonzola_value_get_struct_field_name(&value, 2, &fieldName), GorgonzolaSuccess);
    ASSERT_STREQ(fieldName, "views");
    gorgonzola_destroy_string(fieldName);

    ASSERT_EQ(gorgonzola_value_get_struct_field_name(&value, 3, &fieldName), GorgonzolaSuccess);
    ASSERT_STREQ(fieldName, "release");
    gorgonzola_destroy_string(fieldName);

    ASSERT_EQ(gorgonzola_value_get_struct_field_name(&value, 4, &fieldName), GorgonzolaSuccess);
    ASSERT_STREQ(fieldName, "release_ns");
    gorgonzola_destroy_string(fieldName);

    ASSERT_EQ(gorgonzola_value_get_struct_field_name(&value, 5, &fieldName), GorgonzolaSuccess);
    ASSERT_STREQ(fieldName, "release_ms");
    gorgonzola_destroy_string(fieldName);

    ASSERT_EQ(gorgonzola_value_get_struct_field_name(&value, 6, &fieldName), GorgonzolaSuccess);
    ASSERT_STREQ(fieldName, "release_sec");
    gorgonzola_destroy_string(fieldName);

    ASSERT_EQ(gorgonzola_value_get_struct_field_name(&value, 7, &fieldName), GorgonzolaSuccess);
    ASSERT_STREQ(fieldName, "release_tz");
    gorgonzola_destroy_string(fieldName);

    ASSERT_EQ(gorgonzola_value_get_struct_field_name(&value, 8, &fieldName), GorgonzolaSuccess);
    ASSERT_STREQ(fieldName, "film");
    gorgonzola_destroy_string(fieldName);

    ASSERT_EQ(gorgonzola_value_get_struct_field_name(&value, 222, &fieldName), GorgonzolaError);

    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiValueTest, GetStructFieldValue) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (m:movies) WHERE m.name=\"Roma\" RETURN m.description", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);

    gorgonzola_value fieldValue;
    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 0, &fieldValue), GorgonzolaSuccess);
    gorgonzola_logical_type fieldType;
    gorgonzola_value_get_data_type(&fieldValue, &fieldType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&fieldType), GORGONZOLA_DOUBLE);
    double doubleValue;
    ASSERT_EQ(gorgonzola_value_get_double(&fieldValue, &doubleValue), GorgonzolaSuccess);
    ASSERT_DOUBLE_EQ(doubleValue, 1223);
    gorgonzola_data_type_destroy(&fieldType);

    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 1, &fieldValue), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&fieldValue, &fieldType);
    gorgonzola_data_type_destroy(&fieldType);

    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 2, &fieldValue), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&fieldValue, &fieldType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&fieldType), GORGONZOLA_INT64);
    int64_t int64Value;
    ASSERT_EQ(gorgonzola_value_get_int64(&fieldValue, &int64Value), GorgonzolaSuccess);
    gorgonzola_data_type_destroy(&fieldType);

    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 3, &fieldValue), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&fieldValue, &fieldType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&fieldType), GORGONZOLA_TIMESTAMP);
    gorgonzola_timestamp_t timestamp;
    ASSERT_EQ(gorgonzola_value_get_timestamp(&fieldValue, &timestamp), GorgonzolaSuccess);
    ASSERT_EQ(timestamp.value, 1297442662000000);
    gorgonzola_data_type_destroy(&fieldType);

    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 4, &fieldValue), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&fieldValue, &fieldType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&fieldType), GORGONZOLA_TIMESTAMP_NS);
    gorgonzola_timestamp_ns_t timestamp_ns;
    ASSERT_EQ(gorgonzola_value_get_timestamp_ns(&fieldValue, &timestamp_ns), GorgonzolaSuccess);
    ASSERT_EQ(timestamp_ns.value, 1297442662123456000);
    gorgonzola_data_type_destroy(&fieldType);

    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 5, &fieldValue), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&fieldValue, &fieldType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&fieldType), GORGONZOLA_TIMESTAMP_MS);
    gorgonzola_timestamp_ms_t timestamp_ms;
    ASSERT_EQ(gorgonzola_value_get_timestamp_ms(&fieldValue, &timestamp_ms), GorgonzolaSuccess);
    ASSERT_EQ(timestamp_ms.value, 1297442662123);
    gorgonzola_data_type_destroy(&fieldType);

    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 6, &fieldValue), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&fieldValue, &fieldType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&fieldType), GORGONZOLA_TIMESTAMP_SEC);
    gorgonzola_timestamp_sec_t timestamp_sec;
    ASSERT_EQ(gorgonzola_value_get_timestamp_sec(&fieldValue, &timestamp_sec), GorgonzolaSuccess);
    ASSERT_EQ(timestamp_sec.value, 1297442662);
    gorgonzola_data_type_destroy(&fieldType);

    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 7, &fieldValue), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&fieldValue, &fieldType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&fieldType), GORGONZOLA_TIMESTAMP_TZ);
    gorgonzola_timestamp_tz_t timestamp_tz;
    ASSERT_EQ(gorgonzola_value_get_timestamp_tz(&fieldValue, &timestamp_tz), GorgonzolaSuccess);
    ASSERT_EQ(timestamp_tz.value, 1297442662123456);
    gorgonzola_data_type_destroy(&fieldType);

    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 8, &fieldValue), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&fieldValue, &fieldType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&fieldType), GORGONZOLA_DATE);
    gorgonzola_date_t date;
    ASSERT_EQ(gorgonzola_value_get_date(&fieldValue, &date), GorgonzolaSuccess);
    ASSERT_EQ(date.days, 15758);
    gorgonzola_data_type_destroy(&fieldType);
    gorgonzola_value_destroy(&fieldValue);

    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 222, &fieldValue), GorgonzolaError);

    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiValueTest, getMapNumFields) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (m:movies) WHERE m.length = 2544 RETURN m.audience", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_FALSE(gorgonzola_query_result_has_next(&result));
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);

    uint64_t mapFields;
    ASSERT_EQ(gorgonzola_value_get_map_size(&value, &mapFields), GorgonzolaSuccess);
    ASSERT_EQ(mapFields, 1);

    gorgonzola_query_result_destroy(&result);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
}

TEST_F(CApiValueTest, getMapKey) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (m:movies) WHERE m.length = 2544 RETURN m.audience", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_FALSE(gorgonzola_query_result_has_next(&result));
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);

    gorgonzola_value key;
    ASSERT_EQ(gorgonzola_value_get_map_key(&value, 0, &key), GorgonzolaSuccess);
    gorgonzola_logical_type keyType;
    gorgonzola_value_get_data_type(&key, &keyType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&keyType), GORGONZOLA_STRING);
    char* mapName;
    ASSERT_EQ(gorgonzola_value_get_string(&key, &mapName), GorgonzolaSuccess);
    ASSERT_STREQ(mapName, "audience1");
    gorgonzola_destroy_string(mapName);
    gorgonzola_data_type_destroy(&keyType);
    gorgonzola_value_destroy(&key);

    ASSERT_EQ(gorgonzola_value_get_map_key(&value, 1, &key), GorgonzolaError);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
}

TEST_F(CApiValueTest, getMapValue) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (m:movies) WHERE m.length = 2544 RETURN m.audience", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_FALSE(gorgonzola_query_result_has_next(&result));
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);

    gorgonzola_value mapValue;
    ASSERT_EQ(gorgonzola_value_get_map_value(&value, 0, &mapValue), GorgonzolaSuccess);
    gorgonzola_logical_type mapType;
    gorgonzola_value_get_data_type(&mapValue, &mapType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&mapType), GORGONZOLA_INT64);
    int64_t mapIntValue;
    ASSERT_EQ(gorgonzola_value_get_int64(&mapValue, &mapIntValue), GorgonzolaSuccess);
    ASSERT_EQ(mapIntValue, 33);

    ASSERT_EQ(gorgonzola_value_get_map_value(&value, 1, &mapValue), GorgonzolaError);

    gorgonzola_data_type_destroy(&mapType);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_value_destroy(&mapValue);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
}

TEST_F(CApiValueTest, getDecimalAsString) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"UNWIND [1] AS A UNWIND [5.7, 8.3, 8.7, 13.7] AS B WITH cast(CAST(A AS DECIMAL) "
               "* "
               "CAST(B AS DECIMAL) AS DECIMAL(18, 1)) AS PROD RETURN COLLECT(PROD) AS RES",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);

    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);

    gorgonzola_logical_type dataType;
    gorgonzola_value_get_data_type(&value, &dataType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&dataType), GORGONZOLA_LIST);
    uint64_t list_size;
    ASSERT_EQ(gorgonzola_value_get_list_size(&value, &list_size), GorgonzolaSuccess);
    ASSERT_EQ(list_size, 4);
    gorgonzola_data_type_destroy(&dataType);

    gorgonzola_value decimal_entry;
    char* decimal_value;
    std::string decimal_string_value;
    ASSERT_EQ(gorgonzola_value_get_list_element(&value, 0, &decimal_entry), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&decimal_entry, &dataType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&dataType), GORGONZOLA_DECIMAL);
    ASSERT_EQ(gorgonzola_value_get_decimal_as_string(&decimal_entry, &decimal_value), GorgonzolaSuccess);
    decimal_string_value = std::string(decimal_value);
    ASSERT_EQ(decimal_string_value, "5.7");
    gorgonzola_destroy_string(decimal_value);
    gorgonzola_data_type_destroy(&dataType);

    ASSERT_EQ(gorgonzola_value_get_list_element(&value, 1, &decimal_entry), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&decimal_entry, &dataType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&dataType), GORGONZOLA_DECIMAL);
    ASSERT_EQ(gorgonzola_value_get_decimal_as_string(&decimal_entry, &decimal_value), GorgonzolaSuccess);
    decimal_string_value = std::string(decimal_value);
    ASSERT_EQ(decimal_string_value, "8.3");
    gorgonzola_destroy_string(decimal_value);
    gorgonzola_data_type_destroy(&dataType);

    ASSERT_EQ(gorgonzola_value_get_list_element(&value, 2, &decimal_entry), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&decimal_entry, &dataType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&dataType), GORGONZOLA_DECIMAL);
    ASSERT_EQ(gorgonzola_value_get_decimal_as_string(&decimal_entry, &decimal_value), GorgonzolaSuccess);
    decimal_string_value = std::string(decimal_value);
    ASSERT_EQ(decimal_string_value, "8.7");
    gorgonzola_destroy_string(decimal_value);
    gorgonzola_data_type_destroy(&dataType);

    ASSERT_EQ(gorgonzola_value_get_list_element(&value, 3, &decimal_entry), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&decimal_entry, &dataType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&dataType), GORGONZOLA_DECIMAL);
    ASSERT_EQ(gorgonzola_value_get_decimal_as_string(&decimal_entry, &decimal_value), GorgonzolaSuccess);
    decimal_string_value = std::string(decimal_value);
    ASSERT_EQ(decimal_string_value, "13.7");
    gorgonzola_destroy_string(decimal_value);
    gorgonzola_data_type_destroy(&dataType);

    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_value_destroy(&decimal_entry);
}

TEST_F(CApiValueTest, GetDataType) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.fName, a.isStudent, a.workedHours", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    gorgonzola_logical_type dataType;
    gorgonzola_value_get_data_type(&value, &dataType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&dataType), GORGONZOLA_STRING);
    gorgonzola_data_type_destroy(&dataType);

    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 1, &value), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&value, &dataType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&dataType), GORGONZOLA_BOOL);
    gorgonzola_data_type_destroy(&dataType);

    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 2, &value), GorgonzolaSuccess);
    gorgonzola_value_get_data_type(&value, &dataType);
    ASSERT_EQ(gorgonzola_data_type_get_id(&dataType), GORGONZOLA_LIST);
    gorgonzola_data_type_destroy(&dataType);
    gorgonzola_value_destroy(&value);

    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiValueTest, GetBool) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.isStudent ORDER BY a.ID", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    bool boolValue;
    ASSERT_EQ(gorgonzola_value_get_bool(&value, &boolValue), GorgonzolaSuccess);
    ASSERT_TRUE(boolValue);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_bool(badValue, &boolValue), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetInt8) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) -[r:studyAt]-> (b:organisation) RETURN r.level ORDER BY a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    int8_t int8Value;
    ASSERT_EQ(gorgonzola_value_get_int8(&value, &int8Value), GorgonzolaSuccess);
    ASSERT_EQ(int8Value, 5);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_int8(badValue, &int8Value), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetInt16) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) -[r:studyAt]-> (b:organisation) RETURN r.length ORDER BY a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    int16_t int16Value;
    ASSERT_EQ(gorgonzola_value_get_int16(&value, &int16Value), GorgonzolaSuccess);
    ASSERT_EQ(int16Value, 5);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_int16(badValue, &int16Value), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetInt32) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (m:movies) RETURN m.length ORDER BY m.name", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    int32_t int32Value;
    ASSERT_EQ(gorgonzola_value_get_int32(&value, &int32Value), GorgonzolaSuccess);
    ASSERT_EQ(int32Value, 298);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_int32(badValue, &int32Value), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetInt64) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, (char*)"MATCH (a:person) RETURN a.ID ORDER BY a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    int64_t int64Value;
    ASSERT_EQ(gorgonzola_value_get_int64(&value, &int64Value), GorgonzolaSuccess);
    ASSERT_EQ(int64Value, 0);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_int64(badValue, &int64Value), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetUInt8) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) -[r:studyAt]-> (b:organisation) RETURN r.ulevel ORDER BY a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    uint8_t uint8Value;
    ASSERT_EQ(gorgonzola_value_get_uint8(&value, &uint8Value), GorgonzolaSuccess);
    ASSERT_EQ(uint8Value, 250);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_uint8(badValue, &uint8Value), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetUInt16) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) -[r:studyAt]-> (b:organisation) RETURN r.ulength ORDER BY "
               "a.ID",
        &result);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    uint16_t uint16Value;
    ASSERT_EQ(gorgonzola_value_get_uint16(&value, &uint16Value), GorgonzolaSuccess);
    ASSERT_EQ(uint16Value, 33768);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_uint16(badValue, &uint16Value), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetUInt32) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) -[r:studyAt]-> (b:organisation) "
               "RETURN r.temperature ORDER BY a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    uint32_t uint32Value;
    ASSERT_EQ(gorgonzola_value_get_uint32(&value, &uint32Value), GorgonzolaSuccess);
    ASSERT_EQ(uint32Value, 32800);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_uint32(badValue, &uint32Value), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetUInt64) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) -[r:studyAt]-> (b:organisation) RETURN r.code ORDER BY a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    uint64_t uint64Value;
    ASSERT_EQ(gorgonzola_value_get_uint64(&value, &uint64Value), GorgonzolaSuccess);
    ASSERT_EQ(uint64Value, 9223372036854775808ull);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_uint64(badValue, &uint64Value), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetInt128) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) -[r:studyAt]-> (b:organisation) RETURN r.hugedata ORDER BY "
               "a.ID",
        &result);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    gorgonzola_int128_t int128;
    ASSERT_EQ(gorgonzola_value_get_int128(&value, &int128), GorgonzolaSuccess);
    ASSERT_EQ(int128.high, 100000000);
    ASSERT_EQ(int128.low, 211111111);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_int128(badValue, &int128), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, StringToInt128Test) {
    char input[] = "1844674407370955161811111111";
    gorgonzola_int128_t int128_val;
    ASSERT_EQ(gorgonzola_int128_t_from_string(input, &int128_val), GorgonzolaSuccess);
    ASSERT_EQ(int128_val.high, 100000000);
    ASSERT_EQ(int128_val.low, 211111111);

    char badInput[] = "this is not a int128";
    gorgonzola_int128_t int128_val2;
    ASSERT_EQ(gorgonzola_int128_t_from_string(badInput, &int128_val2), GorgonzolaError);
}

TEST_F(CApiValueTest, Int128ToStringTest) {
    auto int128_val = gorgonzola_int128_t{211111111, 100000000};
    char* str;
    ASSERT_EQ(gorgonzola_int128_t_to_string(int128_val, &str), GorgonzolaSuccess);
    ASSERT_STREQ(str, "1844674407370955161811111111");
    gorgonzola_destroy_string(str);
}

TEST_F(CApiValueTest, GetFloat) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.height ORDER BY a.ID", &result);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    float floatValue;
    ASSERT_EQ(gorgonzola_value_get_float(&value, &floatValue), GorgonzolaSuccess);
    ASSERT_FLOAT_EQ(floatValue, 1.731);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_float(badValue, &floatValue), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetDouble) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.eyeSight ORDER BY a.ID", &result);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    double doubleValue;
    ASSERT_EQ(gorgonzola_value_get_double(&value, &doubleValue), GorgonzolaSuccess);
    ASSERT_DOUBLE_EQ(doubleValue, 5.0);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_double(badValue, &doubleValue), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetInternalID) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, (char*)"MATCH (a:person) RETURN a ORDER BY a.ID",
        &result);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    gorgonzola_value nodeIDVal;
    ASSERT_EQ(gorgonzola_value_get_struct_field_value(&value, 0 /* internal ID field idx */, &nodeIDVal),
        GorgonzolaSuccess);
    gorgonzola_internal_id_t internalID;
    ASSERT_EQ(gorgonzola_value_get_internal_id(&nodeIDVal, &internalID), GorgonzolaSuccess);
    ASSERT_EQ(internalID.table_id, 0);
    ASSERT_EQ(internalID.offset, 0);
    gorgonzola_value_destroy(&nodeIDVal);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_internal_id(badValue, &internalID), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetRelVal) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) -[r:knows]-> (b:person) RETURN r ORDER BY a.ID, b.ID", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value rel;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &rel), GorgonzolaSuccess);
    ASSERT_TRUE(rel._is_owned_by_cpp);
    gorgonzola_value relIdVal;
    ASSERT_EQ(gorgonzola_rel_val_get_id_val(&rel, &relIdVal), GorgonzolaSuccess);
    gorgonzola_internal_id_t relInternalID;
    ASSERT_EQ(gorgonzola_value_get_internal_id(&relIdVal, &relInternalID), GorgonzolaSuccess);
    ASSERT_EQ(relInternalID.table_id, 3);
    ASSERT_EQ(relInternalID.offset, 0);
    gorgonzola_value relSrcIDVal;
    ASSERT_EQ(gorgonzola_rel_val_get_src_id_val(&rel, &relSrcIDVal), GorgonzolaSuccess);
    gorgonzola_internal_id_t relSrcID;
    ASSERT_EQ(gorgonzola_value_get_internal_id(&relSrcIDVal, &relSrcID), GorgonzolaSuccess);
    ASSERT_EQ(relSrcID.table_id, 0);
    ASSERT_EQ(relSrcID.offset, 0);
    gorgonzola_value relDstIDVal;
    ASSERT_EQ(gorgonzola_rel_val_get_dst_id_val(&rel, &relDstIDVal), GorgonzolaSuccess);
    gorgonzola_internal_id_t relDstID;
    ASSERT_EQ(gorgonzola_value_get_internal_id(&relDstIDVal, &relDstID), GorgonzolaSuccess);
    ASSERT_EQ(relDstID.table_id, 0);
    ASSERT_EQ(relDstID.offset, 1);
    gorgonzola_value relLabel;
    ASSERT_EQ(gorgonzola_rel_val_get_label_val(&rel, &relLabel), GorgonzolaSuccess);
    char* relLabelStr;
    ASSERT_EQ(gorgonzola_value_get_string(&relLabel, &relLabelStr), GorgonzolaSuccess);
    ASSERT_STREQ(relLabelStr, "knows");
    uint64_t propertiesSize;
    ASSERT_EQ(gorgonzola_rel_val_get_property_size(&rel, &propertiesSize), GorgonzolaSuccess);
    ASSERT_EQ(propertiesSize, 7);
    gorgonzola_destroy_string(relLabelStr);
    gorgonzola_value_destroy(&relLabel);
    gorgonzola_value_destroy(&relIdVal);
    gorgonzola_value_destroy(&relSrcIDVal);
    gorgonzola_value_destroy(&relDstIDVal);
    gorgonzola_value_destroy(&rel);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_rel_val_get_src_id_val(badValue, &relSrcIDVal), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetDate) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.birthdate ORDER BY a.ID", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    gorgonzola_date_t date;
    ASSERT_EQ(gorgonzola_value_get_date(&value, &date), GorgonzolaSuccess);
    ASSERT_EQ(date.days, -25567);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_date(badValue, &date), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetTimestamp) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.registerTime ORDER BY a.ID", &result);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    gorgonzola_timestamp_t timestamp;
    ASSERT_EQ(gorgonzola_value_get_timestamp(&value, &timestamp), GorgonzolaSuccess);
    ASSERT_EQ(timestamp.value, 1313839530000000);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_timestamp(badValue, &timestamp), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetInterval) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.lastJobDuration ORDER BY a.ID", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    gorgonzola_interval_t interval;
    ASSERT_EQ(gorgonzola_value_get_interval(&value, &interval), GorgonzolaSuccess);
    ASSERT_EQ(interval.months, 36);
    ASSERT_EQ(interval.days, 2);
    ASSERT_EQ(interval.micros, 46920000000);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_interval(badValue, &interval), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetString) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.fName ORDER BY a.ID", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    char* str;
    ASSERT_EQ(gorgonzola_value_get_string(&value, &str), GorgonzolaSuccess);
    ASSERT_STREQ(str, "Alice");
    gorgonzola_destroy_string(str);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_int32(123);
    ASSERT_EQ(gorgonzola_value_get_string(badValue, &str), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetBlob) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state =
        gorgonzola_connection_query(connection, (char*)R"(RETURN BLOB('\xAA\xBB\xCD\x1A');)", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    uint8_t* blob;
    ASSERT_EQ(gorgonzola_value_get_blob(&value, &blob), GorgonzolaSuccess);
    ASSERT_EQ(blob[0], 0xAA);
    ASSERT_EQ(blob[1], 0xBB);
    ASSERT_EQ(blob[2], 0xCD);
    ASSERT_EQ(blob[3], 0x1A);
    ASSERT_EQ(blob[4], 0x00);
    gorgonzola_destroy_blob(blob);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_blob(badValue, &blob), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetBlobWithSize) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    // Use a blob containing null bytes to ensure it's not truncated
    state =
        gorgonzola_connection_query(connection, (char*)R"(RETURN BLOB('\xAA\x00\xCC\x1A\x00');)", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    
    uint8_t* blob;
    uint64_t size;
    ASSERT_EQ(gorgonzola_value_get_blob_with_size(&value, &blob, &size), GorgonzolaSuccess);
    ASSERT_EQ(size, 5);
    ASSERT_EQ(blob[0], 0xAA);
    ASSERT_EQ(blob[1], 0x00);
    ASSERT_EQ(blob[2], 0xCC);
    ASSERT_EQ(blob[3], 0x1A);
    ASSERT_EQ(blob[4], 0x00);
    gorgonzola_destroy_blob(blob);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_blob_with_size(badValue, &blob, &size), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}


TEST_F(CApiValueTest, GetUUID) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)R"(RETURN UUID("A0EEBC99-9C0B-4EF8-BB6D-6BB9BD380A11");)", &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value value;
    gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value);
    ASSERT_TRUE(value._is_owned_by_cpp);
    ASSERT_FALSE(gorgonzola_value_is_null(&value));
    char* str;
    ASSERT_EQ(gorgonzola_value_get_uuid(&value, &str), GorgonzolaSuccess);
    ASSERT_STREQ(str, "a0eebc99-9c0b-4ef8-bb6d-6bb9bd380a11");
    gorgonzola_destroy_string(str);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_value_get_uuid(badValue, &str), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, ToSting) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) RETURN a.fName, a.isStudent, a.workedHours ORDER BY "
               "a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));

    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);

    gorgonzola_value value;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &value), GorgonzolaSuccess);
    char* str = gorgonzola_value_to_string(&value);
    ASSERT_STREQ(str, "Alice");
    gorgonzola_destroy_string(str);

    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 1, &value), GorgonzolaSuccess);
    str = gorgonzola_value_to_string(&value);
    ASSERT_STREQ(str, "True");
    gorgonzola_destroy_string(str);

    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 2, &value), GorgonzolaSuccess);
    str = gorgonzola_value_to_string(&value);
    ASSERT_STREQ(str, "[10,5]");
    gorgonzola_destroy_string(str);
    gorgonzola_value_destroy(&value);

    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiValueTest, NodeValGetLabelVal) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, (char*)"MATCH (a:person) RETURN a ORDER BY a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));

    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value nodeVal;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &nodeVal), GorgonzolaSuccess);
    gorgonzola_value labelVal;
    ASSERT_EQ(gorgonzola_node_val_get_label_val(&nodeVal, &labelVal), GorgonzolaSuccess);
    char* labelStr;
    ASSERT_EQ(gorgonzola_value_get_string(&labelVal, &labelStr), GorgonzolaSuccess);
    ASSERT_STREQ(labelStr, "person");
    gorgonzola_destroy_string(labelStr);
    gorgonzola_value_destroy(&labelVal);
    gorgonzola_value_destroy(&nodeVal);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_node_val_get_label_val(badValue, &labelVal), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, NodeValGetID) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, (char*)"MATCH (a:person) RETURN a ORDER BY a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));

    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value nodeVal;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &nodeVal), GorgonzolaSuccess);
    gorgonzola_value nodeIDVal;
    ASSERT_EQ(gorgonzola_node_val_get_id_val(&nodeVal, &nodeIDVal), GorgonzolaSuccess);
    ASSERT_NE(nodeIDVal._value, nullptr);
    gorgonzola_internal_id_t internalID;
    ASSERT_EQ(gorgonzola_value_get_internal_id(&nodeIDVal, &internalID), GorgonzolaSuccess);
    ASSERT_EQ(internalID.table_id, 0);
    ASSERT_EQ(internalID.offset, 0);
    gorgonzola_value_destroy(&nodeIDVal);
    gorgonzola_value_destroy(&nodeVal);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_node_val_get_id_val(badValue, &nodeIDVal), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, NodeValGetLabelName) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, (char*)"MATCH (a:person) RETURN a ORDER BY a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));

    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value nodeVal;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &nodeVal), GorgonzolaSuccess);
    gorgonzola_value labelVal;
    ASSERT_EQ(gorgonzola_node_val_get_label_val(&nodeVal, &labelVal), GorgonzolaSuccess);
    char* labelStr;
    ASSERT_EQ(gorgonzola_value_get_string(&labelVal, &labelStr), GorgonzolaSuccess);
    ASSERT_STREQ(labelStr, "person");
    gorgonzola_destroy_string(labelStr);
    gorgonzola_value_destroy(&labelVal);
    gorgonzola_value_destroy(&nodeVal);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_node_val_get_label_val(badValue, &labelVal), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, NodeValGetProperty) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection, (char*)"MATCH (a:person) RETURN a ORDER BY a.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value node;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &node), GorgonzolaSuccess);
    char* propertyName;
    ASSERT_EQ(gorgonzola_node_val_get_property_name_at(&node, 0, &propertyName), GorgonzolaSuccess);
    ASSERT_STREQ(propertyName, "ID");
    gorgonzola_destroy_string(propertyName);
    ASSERT_EQ(gorgonzola_node_val_get_property_name_at(&node, 1, &propertyName), GorgonzolaSuccess);
    ASSERT_STREQ(propertyName, "fName");
    gorgonzola_destroy_string(propertyName);
    ASSERT_EQ(gorgonzola_node_val_get_property_name_at(&node, 2, &propertyName), GorgonzolaSuccess);
    ASSERT_STREQ(propertyName, "gender");
    gorgonzola_destroy_string(propertyName);
    ASSERT_EQ(gorgonzola_node_val_get_property_name_at(&node, 3, &propertyName), GorgonzolaSuccess);
    ASSERT_STREQ(propertyName, "isStudent");
    gorgonzola_destroy_string(propertyName);

    gorgonzola_value propertyValue;
    ASSERT_EQ(gorgonzola_node_val_get_property_value_at(&node, 0, &propertyValue), GorgonzolaSuccess);
    int64_t propertyValueID;
    ASSERT_EQ(gorgonzola_value_get_int64(&propertyValue, &propertyValueID), GorgonzolaSuccess);
    ASSERT_EQ(propertyValueID, 0);
    ASSERT_EQ(gorgonzola_node_val_get_property_value_at(&node, 1, &propertyValue), GorgonzolaSuccess);
    char* propertyValuefName;
    ASSERT_EQ(gorgonzola_value_get_string(&propertyValue, &propertyValuefName), GorgonzolaSuccess);
    ASSERT_STREQ(propertyValuefName, "Alice");
    gorgonzola_destroy_string(propertyValuefName);
    ASSERT_EQ(gorgonzola_node_val_get_property_value_at(&node, 2, &propertyValue), GorgonzolaSuccess);
    int64_t propertyValueGender;
    ASSERT_EQ(gorgonzola_value_get_int64(&propertyValue, &propertyValueGender), GorgonzolaSuccess);
    ASSERT_EQ(propertyValueGender, 1);
    ASSERT_EQ(gorgonzola_node_val_get_property_value_at(&node, 3, &propertyValue), GorgonzolaSuccess);
    bool propertyValueIsStudent;
    ASSERT_EQ(gorgonzola_value_get_bool(&propertyValue, &propertyValueIsStudent), GorgonzolaSuccess);
    ASSERT_EQ(propertyValueIsStudent, true);
    gorgonzola_value_destroy(&propertyValue);

    gorgonzola_value_destroy(&node);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_node_val_get_property_name_at(badValue, 0, &propertyName), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, NodeValToString) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (b:organisation) RETURN b ORDER BY b.ID", &result);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value node;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &node), GorgonzolaSuccess);
    ASSERT_TRUE(node._is_owned_by_cpp);

    char* str = gorgonzola_value_to_string(&node);
    ASSERT_STREQ(str,
        "{_ID: 1:0, _LABEL: organisation, ID: 1, name: ABFsUni, orgCode: 325, mark: 3.700000, "
        "score: -2, history: 10 years 5 months 13 hours 24 us, licenseValidInterval: 3 years "
        "5 days, rating: 1.000000, state: {revenue: 138, location: ['toronto','montr,eal'], "
        "stock: {price: [96,56], volume: 1000}}, info: 3.120000}");
    gorgonzola_destroy_string(str);

    gorgonzola_value_destroy(&node);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);
}

TEST_F(CApiValueTest, RelValGetProperty) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) -[e:workAt]-> (b:organisation) RETURN e ORDER BY a.ID, b.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value rel;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &rel), GorgonzolaSuccess);
    ASSERT_TRUE(rel._is_owned_by_cpp);
    uint64_t propertiesSize;
    ASSERT_EQ(gorgonzola_rel_val_get_property_size(&rel, &propertiesSize), GorgonzolaSuccess);
    ASSERT_EQ(propertiesSize, 3);

    char* propertyName;
    ASSERT_EQ(gorgonzola_rel_val_get_property_name_at(&rel, 0, &propertyName), GorgonzolaSuccess);
    ASSERT_STREQ(propertyName, "year");
    gorgonzola_destroy_string(propertyName);

    ASSERT_EQ(gorgonzola_rel_val_get_property_name_at(&rel, 1, &propertyName), GorgonzolaSuccess);
    ASSERT_STREQ(propertyName, "grading");
    gorgonzola_destroy_string(propertyName);
    ASSERT_EQ(gorgonzola_rel_val_get_property_name_at(&rel, 2, &propertyName), GorgonzolaSuccess);
    ASSERT_STREQ(propertyName, "rating");
    gorgonzola_destroy_string(propertyName);

    gorgonzola_value propertyValue;
    ASSERT_EQ(gorgonzola_rel_val_get_property_value_at(&rel, 0, &propertyValue), GorgonzolaSuccess);
    int64_t propertyValueYear;
    ASSERT_EQ(gorgonzola_value_get_int64(&propertyValue, &propertyValueYear), GorgonzolaSuccess);
    ASSERT_EQ(propertyValueYear, 2015);

    ASSERT_EQ(gorgonzola_rel_val_get_property_value_at(&rel, 1, &propertyValue), GorgonzolaSuccess);
    gorgonzola_value listValue;
    ASSERT_EQ(gorgonzola_value_get_list_element(&propertyValue, 0, &listValue), GorgonzolaSuccess);
    double listValueGrading;
    ASSERT_EQ(gorgonzola_value_get_double(&listValue, &listValueGrading), GorgonzolaSuccess);
    ASSERT_DOUBLE_EQ(listValueGrading, 3.8);
    ASSERT_EQ(gorgonzola_value_get_list_element(&propertyValue, 1, &listValue), GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_value_get_double(&listValue, &listValueGrading), GorgonzolaSuccess);
    ASSERT_DOUBLE_EQ(listValueGrading, 2.5);
    gorgonzola_value_destroy(&listValue);

    ASSERT_EQ(gorgonzola_rel_val_get_property_value_at(&rel, 2, &propertyValue), GorgonzolaSuccess);
    float propertyValueRating;
    ASSERT_EQ(gorgonzola_value_get_float(&propertyValue, &propertyValueRating), GorgonzolaSuccess);
    ASSERT_FLOAT_EQ(propertyValueRating, 8.2);
    gorgonzola_value_destroy(&propertyValue);

    gorgonzola_value_destroy(&rel);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_rel_val_get_property_name_at(badValue, 0, &propertyName), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, RelValToString) {
    gorgonzola_query_result result;
    gorgonzola_flat_tuple flatTuple;
    gorgonzola_state state;
    auto connection = getConnection();
    state = gorgonzola_connection_query(connection,
        (char*)"MATCH (a:person) -[e:workAt]-> (b:organisation) RETURN e ORDER BY a.ID, b.ID",
        &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    state = gorgonzola_query_result_get_next(&result, &flatTuple);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_value rel;
    ASSERT_EQ(gorgonzola_flat_tuple_get_value(&flatTuple, 0, &rel), GorgonzolaSuccess);
    ASSERT_TRUE(rel._is_owned_by_cpp);
    char* str;
    ASSERT_EQ(gorgonzola_rel_val_to_string(&rel, &str), GorgonzolaSuccess);
    ASSERT_STREQ(str, "(0:2)-{_LABEL: workAt, _ID: 7:0, year: 2015, grading: [3.800000,2.500000], "
                      "rating: 8.200000}->(1:1)");
    gorgonzola_destroy_string(str);
    gorgonzola_value_destroy(&rel);
    gorgonzola_flat_tuple_destroy(&flatTuple);
    gorgonzola_query_result_destroy(&result);

    gorgonzola_value* badValue = gorgonzola_value_create_string((char*)"abcdefg");
    ASSERT_EQ(gorgonzola_rel_val_to_string(badValue, &str), GorgonzolaError);
    gorgonzola_value_destroy(badValue);
}

TEST_F(CApiValueTest, GetTmFromNonStandardTimestamp) {
    gorgonzola_timestamp_ns_t timestamp_ns = gorgonzola_timestamp_ns_t{17515323532900000};
    gorgonzola_timestamp_ms_t timestamp_ms = gorgonzola_timestamp_ms_t{1012323435341};
    gorgonzola_timestamp_sec_t timestamp_sec = gorgonzola_timestamp_sec_t{1432135648};
    gorgonzola_timestamp_tz_t timestamp_tz = gorgonzola_timestamp_tz_t{771513532900000};
    struct tm tm;
    ASSERT_EQ(gorgonzola_timestamp_ns_to_tm(timestamp_ns, &tm), GorgonzolaSuccess);
    ASSERT_EQ(tm.tm_year, 70);
    ASSERT_EQ(tm.tm_mon, 6);
    ASSERT_EQ(tm.tm_mday, 22);
    ASSERT_EQ(tm.tm_hour, 17);
    ASSERT_EQ(tm.tm_min, 22);
    ASSERT_EQ(tm.tm_sec, 3);
    ASSERT_EQ(gorgonzola_timestamp_ms_to_tm(timestamp_ms, &tm), GorgonzolaSuccess);
    ASSERT_EQ(tm.tm_year, 102);
    ASSERT_EQ(tm.tm_mon, 0);
    ASSERT_EQ(tm.tm_mday, 29);
    ASSERT_EQ(tm.tm_hour, 16);
    ASSERT_EQ(tm.tm_min, 57);
    ASSERT_EQ(tm.tm_sec, 15);
    ASSERT_EQ(gorgonzola_timestamp_sec_to_tm(timestamp_sec, &tm), GorgonzolaSuccess);
    ASSERT_EQ(tm.tm_year, 115);
    ASSERT_EQ(tm.tm_mon, 4);
    ASSERT_EQ(tm.tm_mday, 20);
    ASSERT_EQ(tm.tm_hour, 15);
    ASSERT_EQ(tm.tm_min, 27);
    ASSERT_EQ(tm.tm_sec, 28);
    ASSERT_EQ(gorgonzola_timestamp_tz_to_tm(timestamp_tz, &tm), GorgonzolaSuccess);
    ASSERT_EQ(tm.tm_year, 94);
    ASSERT_EQ(tm.tm_mon, 5);
    ASSERT_EQ(tm.tm_mday, 13);
    ASSERT_EQ(tm.tm_hour, 13);
    ASSERT_EQ(tm.tm_min, 18);
    ASSERT_EQ(tm.tm_sec, 52);
}

TEST_F(CApiValueTest, GetTmFromTimestamp) {
    gorgonzola_timestamp_t timestamp = gorgonzola_timestamp_t{171513532900000};
    struct tm tm;
    ASSERT_EQ(gorgonzola_timestamp_to_tm(timestamp, &tm), GorgonzolaSuccess);
    ASSERT_EQ(tm.tm_year, 75);
    ASSERT_EQ(tm.tm_mon, 5);
    ASSERT_EQ(tm.tm_mday, 9);
    ASSERT_EQ(tm.tm_hour, 2);
    ASSERT_EQ(tm.tm_min, 38);
    ASSERT_EQ(tm.tm_sec, 52);
}

TEST_F(CApiValueTest, GetTmFromDate) {
    gorgonzola_date_t date = gorgonzola_date_t{-255};
    struct tm tm;
    ASSERT_EQ(gorgonzola_date_to_tm(date, &tm), GorgonzolaSuccess);
    ASSERT_EQ(tm.tm_year, 69);
    ASSERT_EQ(tm.tm_mon, 3);
    ASSERT_EQ(tm.tm_mday, 21);
    ASSERT_EQ(tm.tm_hour, 0);
    ASSERT_EQ(tm.tm_min, 0);
    ASSERT_EQ(tm.tm_sec, 0);
}

TEST_F(CApiValueTest, GetTimestampFromTm) {
    struct tm tm;
    tm.tm_year = 75;
    tm.tm_mon = 5;
    tm.tm_mday = 9;
    tm.tm_hour = 2;
    tm.tm_min = 38;
    tm.tm_sec = 52;
    gorgonzola_timestamp_t timestamp;
    ASSERT_EQ(gorgonzola_timestamp_from_tm(tm, &timestamp), GorgonzolaSuccess);
    ASSERT_EQ(timestamp.value, 171513532000000);
}

TEST_F(CApiValueTest, GetNonStandardTimestampFromTm) {
    struct tm tm;
    tm.tm_year = 70;
    tm.tm_mon = 6;
    tm.tm_mday = 22;
    tm.tm_hour = 17;
    tm.tm_min = 22;
    tm.tm_sec = 3;
    gorgonzola_timestamp_ns_t timestamp_ns;
    ASSERT_EQ(gorgonzola_timestamp_ns_from_tm(tm, &timestamp_ns), GorgonzolaSuccess);
    ASSERT_EQ(timestamp_ns.value, 17515323000000000);
    tm.tm_year = 102;
    tm.tm_mon = 0;
    tm.tm_mday = 29;
    tm.tm_hour = 16;
    tm.tm_min = 57;
    tm.tm_sec = 15;
    gorgonzola_timestamp_ms_t timestamp_ms;
    ASSERT_EQ(gorgonzola_timestamp_ms_from_tm(tm, &timestamp_ms), GorgonzolaSuccess);
    ASSERT_EQ(timestamp_ms.value, 1012323435000);
    tm.tm_year = 115;
    tm.tm_mon = 4;
    tm.tm_mday = 20;
    tm.tm_hour = 15;
    tm.tm_min = 27;
    tm.tm_sec = 28;
    gorgonzola_timestamp_sec_t timestamp_sec;
    ASSERT_EQ(gorgonzola_timestamp_sec_from_tm(tm, &timestamp_sec), GorgonzolaSuccess);
    ASSERT_EQ(timestamp_sec.value, 1432135648);
    tm.tm_year = 94;
    tm.tm_mon = 5;
    tm.tm_mday = 13;
    tm.tm_hour = 13;
    tm.tm_min = 18;
    tm.tm_sec = 52;
    gorgonzola_timestamp_tz_t timestamp_tz;
    ASSERT_EQ(gorgonzola_timestamp_tz_from_tm(tm, &timestamp_tz), GorgonzolaSuccess);
    ASSERT_EQ(timestamp_tz.value, 771513532000000);
}

TEST_F(CApiValueTest, GetDateFromTm) {
    struct tm tm;
    tm.tm_year = 69;
    tm.tm_mon = 3;
    tm.tm_mday = 21;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    gorgonzola_date_t date;
    ASSERT_EQ(gorgonzola_date_from_tm(tm, &date), GorgonzolaSuccess);
    ASSERT_EQ(date.days, -255);
}

TEST_F(CApiValueTest, GetDateFromString) {
    char input[] = "1969-04-21";
    gorgonzola_date_t date;
    ASSERT_EQ(gorgonzola_date_from_string(input, &date), GorgonzolaSuccess);
    ASSERT_EQ(date.days, -255);

    char badInput[] = "this is not a date";
    ASSERT_EQ(gorgonzola_date_from_string(badInput, &date), GorgonzolaError);
}

TEST_F(CApiValueTest, GetStringFromDate) {
    gorgonzola_date_t date = gorgonzola_date_t{-255};
    char* str;
    ASSERT_EQ(gorgonzola_date_to_string(date, &str), GorgonzolaSuccess);
    ASSERT_STREQ(str, "1969-04-21");
    gorgonzola_destroy_string(str);
}

TEST_F(CApiValueTest, GetDifftimeFromInterval) {
    gorgonzola_interval_t interval = gorgonzola_interval_t{36, 2, 46920000000};
    double difftime;
    gorgonzola_interval_to_difftime(interval, &difftime);
    ASSERT_DOUBLE_EQ(difftime, 93531720);
}

TEST_F(CApiValueTest, GetIntervalFromDifftime) {
    double difftime = 211110160.479;
    gorgonzola_interval_t interval;
    gorgonzola_interval_from_difftime(difftime, &interval);
    ASSERT_EQ(interval.months, 81);
    ASSERT_EQ(interval.days, 13);
    ASSERT_EQ(interval.micros, 34960479000);
}
