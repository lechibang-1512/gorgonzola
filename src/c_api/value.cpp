#include "common/types/value/value.h"

#include "c_api/helpers.h"
#include "c_api/gorgonzola.h"
#include "common/constants.h"
#include "common/types/types.h"
#include "common/types/value/nested.h"
#include "common/types/value/node.h"
#include "common/types/value/recursive_rel.h"
#include "common/types/value/rel.h"
#include "function/cast/functions/cast_from_string_functions.h"

using namespace gorgonzola::common;

gorgonzola_value* gorgonzola_value_create_null() {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(Value::createNullValue());
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_null_with_data_type(gorgonzola_logical_type* data_type) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value =
        new Value(Value::createNullValue(*static_cast<LogicalType*>(data_type->_data_type)));
    return c_value;
}

bool gorgonzola_value_is_null(gorgonzola_value* value) {
    return static_cast<Value*>(value->_value)->isNull();
}

void gorgonzola_value_set_null(gorgonzola_value* value, bool is_null) {
    static_cast<Value*>(value->_value)->setNull(is_null);
}

gorgonzola_value* gorgonzola_value_create_default(gorgonzola_logical_type* data_type) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value =
        new Value(Value::createDefaultValue(*static_cast<LogicalType*>(data_type->_data_type)));
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_bool(bool val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_int8(int8_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_int16(int16_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_int32(int32_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_int64(int64_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_uint8(uint8_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_uint16(uint16_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_uint32(uint32_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_uint64(uint64_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_int128(gorgonzola_int128_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    int128_t int128(val_.low, val_.high);
    c_value->_value = new Value(int128);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_float(float val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_double(double val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_internal_id(gorgonzola_internal_id_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    internalID_t id(val_.offset, val_.table_id);
    c_value->_value = new Value(id);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_date(gorgonzola_date_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    auto date = date_t(val_.days);
    c_value->_value = new Value(date);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_timestamp_ns(gorgonzola_timestamp_ns_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    auto timestamp_ns = timestamp_ns_t(val_.value);
    c_value->_value = new Value(timestamp_ns);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_timestamp_ms(gorgonzola_timestamp_ms_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    auto timestamp_ms = timestamp_ms_t(val_.value);
    c_value->_value = new Value(timestamp_ms);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_timestamp_sec(gorgonzola_timestamp_sec_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    auto timestamp_sec = timestamp_sec_t(val_.value);
    c_value->_value = new Value(timestamp_sec);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_timestamp_tz(gorgonzola_timestamp_tz_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    auto timestamp_tz = timestamp_tz_t(val_.value);
    c_value->_value = new Value(timestamp_tz);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_timestamp(gorgonzola_timestamp_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    auto timestamp = timestamp_t(val_.value);
    c_value->_value = new Value(timestamp);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_interval(gorgonzola_interval_t val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    auto interval = interval_t(val_.months, val_.days, val_.micros);
    c_value->_value = new Value(interval);
    return c_value;
}

gorgonzola_value* gorgonzola_value_create_string(const char* val_) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(val_);
    return c_value;
}

gorgonzola_state gorgonzola_value_create_list(uint64_t num_elements, gorgonzola_value** elements,
    gorgonzola_value** out_value) {
    if (num_elements == 0) {
        return GorgonzolaError;
    }
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    std::vector<std::unique_ptr<Value>> children;

    auto first_element = static_cast<Value*>(elements[0]->_value);
    auto type = first_element->getDataType().copy();

    for (uint64_t i = 0; i < num_elements; ++i) {
        auto child = static_cast<Value*>(elements[i]->_value);
        if (child->getDataType() != type) {
            free(c_value);
            return GorgonzolaError;
        }
        // Copy the value to the list value to transfer ownership to the C++ side.
        children.push_back(child->copy());
    }
    auto list_type = LogicalType::LIST(first_element->getDataType().copy());
    c_value->_value = new Value(list_type.copy(), std::move(children));
    c_value->_is_owned_by_cpp = false;
    *out_value = c_value;
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_create_struct(uint64_t num_fields, const char** field_names,
    gorgonzola_value** field_values, gorgonzola_value** out_value) {
    if (num_fields == 0) {
        return GorgonzolaError;
    }
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    std::vector<std::unique_ptr<Value>> children;
    auto struct_fields = std::vector<StructField>{};
    for (uint64_t i = 0; i < num_fields; ++i) {
        auto field_name = std::string(field_names[i]);
        auto field_value = static_cast<Value*>(field_values[i]->_value);
        auto field_type = field_value->getDataType().copy();
        struct_fields.emplace_back(std::move(field_name), std::move(field_type));
        children.push_back(field_value->copy());
    }
    auto struct_type = LogicalType::STRUCT(std::move(struct_fields));
    c_value->_value = new Value(std::move(struct_type), std::move(children));
    c_value->_is_owned_by_cpp = false;
    *out_value = c_value;
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_create_map(uint64_t num_fields, gorgonzola_value** keys, gorgonzola_value** values,
    gorgonzola_value** out_value) {
    if (num_fields == 0) {
        return GorgonzolaError;
    }
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    std::vector<std::unique_ptr<Value>> children;

    auto first_key = static_cast<Value*>(keys[0]->_value);
    auto first_value = static_cast<Value*>(values[0]->_value);
    auto key_type = first_key->getDataType().copy();
    auto value_type = first_value->getDataType().copy();

    for (uint64_t i = 0; i < num_fields; ++i) {
        auto key = static_cast<Value*>(keys[i]->_value);
        auto value = static_cast<Value*>(values[i]->_value);
        if (key->getDataType() != key_type || value->getDataType() != value_type) {
            free(c_value);
            return GorgonzolaError;
        }
        std::vector<StructField> struct_fields;
        struct_fields.emplace_back(InternalKeyword::MAP_KEY, key_type.copy());
        struct_fields.emplace_back(InternalKeyword::MAP_VALUE, value_type.copy());
        std::vector<std::unique_ptr<Value>> struct_values;
        struct_values.push_back(key->copy());
        struct_values.push_back(value->copy());
        auto struct_type = LogicalType::STRUCT(std::move(struct_fields));
        auto struct_value = new Value(std::move(struct_type), std::move(struct_values));
        children.push_back(std::unique_ptr<Value>(struct_value));
    }
    auto map_type = LogicalType::MAP(key_type.copy(), value_type.copy());
    c_value->_value = new Value(map_type.copy(), std::move(children));
    c_value->_is_owned_by_cpp = false;
    *out_value = c_value;
    return GorgonzolaSuccess;
}

gorgonzola_value* gorgonzola_value_clone(gorgonzola_value* value) {
    auto* c_value = (gorgonzola_value*)calloc(1, sizeof(gorgonzola_value));
    c_value->_value = new Value(*static_cast<Value*>(value->_value));
    return c_value;
}

void gorgonzola_value_copy(gorgonzola_value* value, gorgonzola_value* other) {
    static_cast<Value*>(value->_value)->copyValueFrom(*static_cast<Value*>(other->_value));
}

void gorgonzola_value_destroy(gorgonzola_value* value) {
    if (value == nullptr) {
        return;
    }
    if (!value->_is_owned_by_cpp) {
        if (value->_value != nullptr) {
            delete static_cast<Value*>(value->_value);
        }
        free(value);
    }
}

gorgonzola_state gorgonzola_value_get_list_size(gorgonzola_value* value, uint64_t* out_result) {
    if (static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID() !=
        LogicalTypeID::LIST) {
        return GorgonzolaError;
    }
    *out_result = NestedVal::getChildrenSize(static_cast<Value*>(value->_value));
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_list_element(gorgonzola_value* value, uint64_t index, gorgonzola_value* out_value) {
    auto physical_type_id = static_cast<Value*>(value->_value)->getDataType().getPhysicalType();
    if (physical_type_id != PhysicalTypeID::ARRAY && physical_type_id != PhysicalTypeID::STRUCT &&
        physical_type_id != PhysicalTypeID::LIST) {
        return GorgonzolaError;
    }
    auto listValue = static_cast<Value*>(value->_value);
    if (index >= NestedVal::getChildrenSize(listValue)) {
        return GorgonzolaError;
    }
    try {
        auto val = NestedVal::getChildVal(listValue, index);
        out_value->_value = val;
        out_value->_is_owned_by_cpp = true;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_struct_num_fields(gorgonzola_value* value, uint64_t* out_result) {
    auto physical_type_id = static_cast<Value*>(value->_value)->getDataType().getPhysicalType();
    if (physical_type_id != PhysicalTypeID::STRUCT) {
        return GorgonzolaError;
    }
    auto val = static_cast<Value*>(value->_value);
    const auto& data_type = val->getDataType();
    try {
        *out_result = StructType::getNumFields(data_type);
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_value_get_struct_field_name(gorgonzola_value* value, uint64_t index, char** out_result) {
    auto physical_type_id = static_cast<Value*>(value->_value)->getDataType().getPhysicalType();
    if (physical_type_id != PhysicalTypeID::STRUCT) {
        return GorgonzolaError;
    }
    auto val = static_cast<Value*>(value->_value);
    const auto& data_type = val->getDataType();
    if (index >= StructType::getNumFields(data_type)) {
        return GorgonzolaError;
    }
    std::string struct_field_name = StructType::getFields(data_type)[index].getName();
    if (struct_field_name.empty()) {
        return GorgonzolaError;
    }
    *out_result = convertToOwnedCString(struct_field_name);
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_struct_field_value(gorgonzola_value* value, uint64_t index,
    gorgonzola_value* out_value) {
    return gorgonzola_value_get_list_element(value, index, out_value);
}

gorgonzola_state gorgonzola_value_get_map_size(gorgonzola_value* value, uint64_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::MAP) {
        return GorgonzolaError;
    }
    auto listValue = static_cast<Value*>(value->_value);
    *out_result = NestedVal::getChildrenSize(listValue);
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_map_key(gorgonzola_value* value, uint64_t index, gorgonzola_value* out_key) {
    gorgonzola_value map_entry;
    if (gorgonzola_value_get_list_element(value, index, &map_entry) == GorgonzolaError) {
        return GorgonzolaError;
    }
    return gorgonzola_value_get_struct_field_value(&map_entry, 0, out_key);
}

gorgonzola_state gorgonzola_value_get_map_value(gorgonzola_value* value, uint64_t index, gorgonzola_value* out_value) {
    gorgonzola_value map_entry;
    if (gorgonzola_value_get_list_element(value, index, &map_entry) == GorgonzolaError) {
        return GorgonzolaError;
    }
    return gorgonzola_value_get_struct_field_value(&map_entry, 1, out_value);
}

gorgonzola_state gorgonzola_value_get_recursive_rel_node_list(gorgonzola_value* value, gorgonzola_value* out_value) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::RECURSIVE_REL) {
        return GorgonzolaError;
    }
    out_value->_is_owned_by_cpp = true;
    try {
        out_value->_value = RecursiveRelVal::getNodes(static_cast<Value*>(value->_value));
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_recursive_rel_rel_list(gorgonzola_value* value, gorgonzola_value* out_value) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::RECURSIVE_REL) {
        return GorgonzolaError;
    }
    out_value->_is_owned_by_cpp = true;
    try {
        out_value->_value = RecursiveRelVal::getRels(static_cast<Value*>(value->_value));
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

void gorgonzola_value_get_data_type(gorgonzola_value* value, gorgonzola_logical_type* out_data_type) {
    out_data_type->_data_type =
        new LogicalType(static_cast<Value*>(value->_value)->getDataType().copy());
}

gorgonzola_state gorgonzola_value_get_bool(gorgonzola_value* value, bool* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::BOOL) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<bool>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_int8(gorgonzola_value* value, int8_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::INT8) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<int8_t>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_int16(gorgonzola_value* value, int16_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::INT16) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<int16_t>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_int32(gorgonzola_value* value, int32_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::INT32) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<int32_t>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_int64(gorgonzola_value* value, int64_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::INT64) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<int64_t>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_uint8(gorgonzola_value* value, uint8_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::UINT8) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<uint8_t>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_uint16(gorgonzola_value* value, uint16_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::UINT16) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<uint16_t>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_uint32(gorgonzola_value* value, uint32_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::UINT32) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<uint32_t>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_uint64(gorgonzola_value* value, uint64_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::UINT64) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<uint64_t>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_int128(gorgonzola_value* value, gorgonzola_int128_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::INT128) {
        return GorgonzolaError;
    }
    try {
        auto int128_val = static_cast<Value*>(value->_value)->getValue<int128_t>();
        out_result->low = int128_val.low;
        out_result->high = int128_val.high;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_int128_t_from_string(const char* str, gorgonzola_int128_t* out_result) {
    int128_t int128_val = 0;
    try {
        gorgonzola::function::CastString::operation(ku_string_t{str, strlen(str)}, int128_val);
        out_result->low = int128_val.low;
        out_result->high = int128_val.high;
    } catch (ConversionException& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_int128_t_to_string(gorgonzola_int128_t int128_val, char** out_result) {
    int128_t c_int128 = 0;
    c_int128.low = int128_val.low;
    c_int128.high = int128_val.high;
    try {
        *out_result = convertToOwnedCString(TypeUtils::toString(c_int128));
    } catch (ConversionException& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}
// TODO: bind all int128_t supported functions

gorgonzola_state gorgonzola_value_get_float(gorgonzola_value* value, float* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::FLOAT) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<float>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_double(gorgonzola_value* value, double* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::DOUBLE) {
        return GorgonzolaError;
    }
    try {
        *out_result = static_cast<Value*>(value->_value)->getValue<double>();
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_internal_id(gorgonzola_value* value, gorgonzola_internal_id_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::INTERNAL_ID) {
        return GorgonzolaError;
    }
    try {
        auto id = static_cast<Value*>(value->_value)->getValue<internalID_t>();
        out_result->offset = id.offset;
        out_result->table_id = id.tableID;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_date(gorgonzola_value* value, gorgonzola_date_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::DATE) {
        return GorgonzolaError;
    }
    try {
        auto date_val = static_cast<Value*>(value->_value)->getValue<date_t>();
        out_result->days = date_val.days;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_timestamp(gorgonzola_value* value, gorgonzola_timestamp_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::TIMESTAMP) {
        return GorgonzolaError;
    }
    try {
        auto timestamp_val = static_cast<Value*>(value->_value)->getValue<timestamp_t>();
        out_result->value = timestamp_val.value;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_timestamp_ns(gorgonzola_value* value, gorgonzola_timestamp_ns_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::TIMESTAMP_NS) {
        return GorgonzolaError;
    }
    try {
        auto timestamp_val = static_cast<Value*>(value->_value)->getValue<timestamp_ns_t>();
        out_result->value = timestamp_val.value;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_timestamp_ms(gorgonzola_value* value, gorgonzola_timestamp_ms_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::TIMESTAMP_MS) {
        return GorgonzolaError;
    }
    try {
        auto timestamp_val = static_cast<Value*>(value->_value)->getValue<timestamp_ms_t>();
        out_result->value = timestamp_val.value;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_timestamp_sec(gorgonzola_value* value, gorgonzola_timestamp_sec_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::TIMESTAMP_SEC) {
        return GorgonzolaError;
    }
    try {
        auto timestamp_val = static_cast<Value*>(value->_value)->getValue<timestamp_sec_t>();
        out_result->value = timestamp_val.value;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_timestamp_tz(gorgonzola_value* value, gorgonzola_timestamp_tz_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::TIMESTAMP_TZ) {
        return GorgonzolaError;
    }
    try {
        auto timestamp_val = static_cast<Value*>(value->_value)->getValue<timestamp_tz_t>();
        out_result->value = timestamp_val.value;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_decimal_as_string(gorgonzola_value* value, char** out_result) {
    auto decimal_val = static_cast<Value*>(value->_value);
    auto logical_type_id = decimal_val->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::DECIMAL) {
        return GorgonzolaError;
    }

    *out_result = convertToOwnedCString(decimal_val->toString());
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_interval(gorgonzola_value* value, gorgonzola_interval_t* out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::INTERVAL) {
        return GorgonzolaError;
    }
    try {
        auto interval_val = static_cast<Value*>(value->_value)->getValue<interval_t>();
        out_result->months = interval_val.months;
        out_result->days = interval_val.days;
        out_result->micros = interval_val.micros;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_string(gorgonzola_value* value, char** out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::STRING) {
        return GorgonzolaError;
    }
    try {
        *out_result =
            convertToOwnedCString(static_cast<Value*>(value->_value)->getValue<std::string>());
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_blob(gorgonzola_value* value, uint8_t** out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::BLOB) {
        return GorgonzolaError;
    }
    try {
        auto blob = static_cast<Value*>(value->_value)->getValue<std::string>();
        *out_result = (uint8_t*)convertToOwnedCString(blob);
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_blob_with_size(gorgonzola_value* value, uint8_t** out_result,
    uint64_t* out_size) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::BLOB) {
        return GorgonzolaError;
    }
    try {
        auto blob = static_cast<Value*>(value->_value)->getValue<std::string>();
        *out_size = blob.size();
        // Use malloc + memcpy instead of convertToOwnedCString to preserve null bytes (#5977)
        *out_result = (uint8_t*)malloc(blob.size());
        if (*out_result == nullptr) {
            return GorgonzolaError;
        }
        memcpy(*out_result, blob.data(), blob.size());
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_value_get_uuid(gorgonzola_value* value, char** out_result) {
    auto logical_type_id = static_cast<Value*>(value->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::UUID) {
        return GorgonzolaError;
    }
    try {
        *out_result =
            convertToOwnedCString(static_cast<Value*>(value->_value)->getValue<std::string>());
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

char* gorgonzola_value_to_string(gorgonzola_value* value) {
    return convertToOwnedCString(static_cast<Value*>(value->_value)->toString());
}

gorgonzola_state gorgonzola_node_val_get_id_val(gorgonzola_value* node_val, gorgonzola_value* out_value) {
    auto logical_type_id = static_cast<Value*>(node_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::NODE) {
        return GorgonzolaError;
    }
    try {
        auto id_val = NodeVal::getNodeIDVal(static_cast<Value*>(node_val->_value));
        out_value->_value = id_val;
        out_value->_is_owned_by_cpp = true;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_node_val_get_label_val(gorgonzola_value* node_val, gorgonzola_value* out_value) {
    auto logical_type_id = static_cast<Value*>(node_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::NODE) {
        return GorgonzolaError;
    }
    try {
        auto label_val = NodeVal::getLabelVal(static_cast<Value*>(node_val->_value));
        out_value->_value = label_val;
        out_value->_is_owned_by_cpp = true;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_node_val_get_property_size(gorgonzola_value* node_val, uint64_t* out_result) {
    auto logical_type_id = static_cast<Value*>(node_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::NODE) {
        return GorgonzolaError;
    }
    try {
        *out_result = NodeVal::getNumProperties(static_cast<Value*>(node_val->_value));
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_node_val_get_property_name_at(gorgonzola_value* node_val, uint64_t index,
    char** out_result) {
    auto logical_type_id = static_cast<Value*>(node_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::NODE) {
        return GorgonzolaError;
    }
    try {
        std::string property_name =
            NodeVal::getPropertyName(static_cast<Value*>(node_val->_value), index);
        if (property_name.empty()) {
            return GorgonzolaError;
        }
        *out_result = convertToOwnedCString(property_name);
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_node_val_get_property_value_at(gorgonzola_value* node_val, uint64_t index,
    gorgonzola_value* out_value) {
    auto logical_type_id = static_cast<Value*>(node_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::NODE) {
        return GorgonzolaError;
    }
    try {
        auto value = NodeVal::getPropertyVal(static_cast<Value*>(node_val->_value), index);
        out_value->_value = value;
        out_value->_is_owned_by_cpp = true;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_node_val_to_string(gorgonzola_value* node_val, char** out_result) {
    auto logical_type_id = static_cast<Value*>(node_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::NODE) {
        return GorgonzolaError;
    }
    try {
        *out_result =
            convertToOwnedCString(NodeVal::toString(static_cast<Value*>(node_val->_value)));
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_rel_val_get_id_val(gorgonzola_value* rel_val, gorgonzola_value* out_value) {
    auto logical_type_id = static_cast<Value*>(rel_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::REL) {
        return GorgonzolaError;
    }
    try {
        auto id_val = RelVal::getIDVal(static_cast<Value*>(rel_val->_value));
        out_value->_value = id_val;
        out_value->_is_owned_by_cpp = true;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_rel_val_get_src_id_val(gorgonzola_value* rel_val, gorgonzola_value* out_value) {
    auto logical_type_id = static_cast<Value*>(rel_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::REL) {
        return GorgonzolaError;
    }
    try {
        auto src_id_val = RelVal::getSrcNodeIDVal(static_cast<Value*>(rel_val->_value));
        out_value->_value = src_id_val;
        out_value->_is_owned_by_cpp = true;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_rel_val_get_dst_id_val(gorgonzola_value* rel_val, gorgonzola_value* out_value) {
    auto logical_type_id = static_cast<Value*>(rel_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::REL) {
        return GorgonzolaError;
    }
    try {
        auto dst_id_val = RelVal::getDstNodeIDVal(static_cast<Value*>(rel_val->_value));
        out_value->_value = dst_id_val;
        out_value->_is_owned_by_cpp = true;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_rel_val_get_label_val(gorgonzola_value* rel_val, gorgonzola_value* out_value) {
    auto logical_type_id = static_cast<Value*>(rel_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::REL) {
        return GorgonzolaError;
    }
    try {
        auto label_val = RelVal::getLabelVal(static_cast<Value*>(rel_val->_value));
        out_value->_value = label_val;
        out_value->_is_owned_by_cpp = true;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_rel_val_get_property_size(gorgonzola_value* rel_val, uint64_t* out_result) {
    auto logical_type_id = static_cast<Value*>(rel_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::REL) {
        return GorgonzolaError;
    }
    try {
        *out_result = RelVal::getNumProperties(static_cast<Value*>(rel_val->_value));
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}
gorgonzola_state gorgonzola_rel_val_get_property_name_at(gorgonzola_value* rel_val, uint64_t index,
    char** out_result) {
    auto logical_type_id = static_cast<Value*>(rel_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::REL) {
        return GorgonzolaError;
    }
    try {
        std::string property_name =
            RelVal::getPropertyName(static_cast<Value*>(rel_val->_value), index);
        if (property_name.empty()) {
            return GorgonzolaError;
        }
        *out_result = convertToOwnedCString(property_name);
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_rel_val_get_property_value_at(gorgonzola_value* rel_val, uint64_t index,
    gorgonzola_value* out_value) {
    auto logical_type_id = static_cast<Value*>(rel_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::REL) {
        return GorgonzolaError;
    }
    try {
        auto value = RelVal::getPropertyVal(static_cast<Value*>(rel_val->_value), index);
        out_value->_value = value;
        out_value->_is_owned_by_cpp = true;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_rel_val_to_string(gorgonzola_value* rel_val, char** out_result) {
    auto logical_type_id = static_cast<Value*>(rel_val->_value)->getDataType().getLogicalTypeID();
    if (logical_type_id != LogicalTypeID::REL) {
        return GorgonzolaError;
    }
    try {
        *out_result = convertToOwnedCString(RelVal::toString(static_cast<Value*>(rel_val->_value)));
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

void gorgonzola_destroy_string(char* str) {
    free(str);
}

void gorgonzola_destroy_blob(uint8_t* blob) {
    free(blob);
}

gorgonzola_state gorgonzola_timestamp_ns_to_tm(gorgonzola_timestamp_ns_t timestamp, struct tm* out_result) {
    time_t time = timestamp.value / 1000000000;
#ifdef _WIN32
    if (convertTimeToTm(time, out_result) != 0) {
        return GorgonzolaError;
    }
#else
    if (gmtime_r(&time, out_result) == nullptr) {
        return GorgonzolaError;
    }
#endif
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_timestamp_ms_to_tm(gorgonzola_timestamp_ms_t timestamp, struct tm* out_result) {
    time_t time = timestamp.value / 1000;
#ifdef _WIN32
    if (convertTimeToTm(time, out_result) != 0) {
        return GorgonzolaError;
    }
#else
    if (gmtime_r(&time, out_result) == nullptr) {
        return GorgonzolaError;
    }
#endif
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_timestamp_sec_to_tm(gorgonzola_timestamp_sec_t timestamp, struct tm* out_result) {
    time_t time = timestamp.value;
#ifdef _WIN32
    if (convertTimeToTm(time, out_result) != 0) {
        return GorgonzolaError;
    }
#else
    if (gmtime_r(&time, out_result) == nullptr) {
        return GorgonzolaError;
    }
#endif
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_timestamp_tz_to_tm(gorgonzola_timestamp_tz_t timestamp, struct tm* out_result) {
    time_t time = timestamp.value / 1000000;
#ifdef _WIN32
    if (convertTimeToTm(time, out_result) != 0) {
        return GorgonzolaError;
    }
#else
    if (gmtime_r(&time, out_result) == nullptr) {
        return GorgonzolaError;
    }
#endif
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_timestamp_to_tm(gorgonzola_timestamp_t timestamp, struct tm* out_result) {
    time_t time = timestamp.value / 1000000;
#ifdef _WIN32
    if (convertTimeToTm(time, out_result) != 0) {
        return GorgonzolaError;
    }
#else
    if (gmtime_r(&time, out_result) == nullptr) {
        return GorgonzolaError;
    }
#endif
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_timestamp_ns_from_tm(struct tm tm, gorgonzola_timestamp_ns_t* out_result) {
#ifdef _WIN32
    int64_t time = convertTmToTime(tm);
#else
    int64_t time = timegm(&tm);
#endif
    if (time == -1) {
        return GorgonzolaError;
    }
    out_result->value = time * 1000000000;
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_timestamp_ms_from_tm(struct tm tm, gorgonzola_timestamp_ms_t* out_result) {
#ifdef _WIN32
    int64_t time = convertTmToTime(tm);
#else
    int64_t time = timegm(&tm);
#endif
    if (time == -1) {
        return GorgonzolaError;
    }
    out_result->value = time * 1000;
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_timestamp_sec_from_tm(struct tm tm, gorgonzola_timestamp_sec_t* out_result) {
#ifdef _WIN32
    int64_t time = convertTmToTime(tm);
#else
    int64_t time = timegm(&tm);
#endif
    if (time == -1) {
        return GorgonzolaError;
    }
    out_result->value = time;
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_timestamp_tz_from_tm(struct tm tm, gorgonzola_timestamp_tz_t* out_result) {
#ifdef _WIN32
    int64_t time = convertTmToTime(tm);
#else
    int64_t time = timegm(&tm);
#endif
    if (time == -1) {
        return GorgonzolaError;
    }
    out_result->value = time * 1000000;
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_timestamp_from_tm(struct tm tm, gorgonzola_timestamp_t* out_result) {
#ifdef _WIN32
    int64_t time = convertTmToTime(tm);
#else
    int64_t time = timegm(&tm);
#endif
    if (time == -1) {
        return GorgonzolaError;
    }
    out_result->value = time * 1000000;
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_date_to_tm(gorgonzola_date_t date, struct tm* out_result) {
    time_t time = date.days * 86400;
#ifdef _WIN32
    if (convertTimeToTm(time, out_result) != 0) {
        return GorgonzolaError;
    }
#else
    if (gmtime_r(&time, out_result) == nullptr) {
        return GorgonzolaError;
    }
#endif
    out_result->tm_hour = 0;
    out_result->tm_min = 0;
    out_result->tm_sec = 0;
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_date_from_tm(struct tm tm, gorgonzola_date_t* out_result) {
#ifdef _WIN32
    int64_t time = convertTmToTime(tm);
#else
    int64_t time = timegm(&tm);
#endif
    if (time == -1) {
        return GorgonzolaError;
    }
    out_result->days = time / 86400;
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_date_to_string(gorgonzola_date_t date, char** out_result) {
    tm tm{};
    if (gorgonzola_date_to_tm(date, &tm) != GorgonzolaSuccess) {
        return GorgonzolaError;
    }
    char buffer[80];
    if (strftime(buffer, 80, "%Y-%m-%d", &tm) == 0) {
        return GorgonzolaError;
    }
    *out_result = convertToOwnedCString(buffer);
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_date_from_string(const char* str, gorgonzola_date_t* out_result) {
    try {
        date_t date = Date::fromCString(str, strlen(str));
        out_result->days = date.days;
    } catch (ConversionException& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

void gorgonzola_interval_to_difftime(gorgonzola_interval_t interval, double* out_result) {
    auto micros = interval.micros + interval.months * Interval::MICROS_PER_MONTH +
                  interval.days * Interval::MICROS_PER_DAY;
    double seconds = micros / 1000000.0;
    *out_result = seconds;
}

void gorgonzola_interval_from_difftime(double difftime, gorgonzola_interval_t* out_result) {
    int64_t total_micros = static_cast<int64_t>(difftime * 1000000);
    out_result->months = total_micros / Interval::MICROS_PER_MONTH;
    total_micros -= out_result->months * Interval::MICROS_PER_MONTH;
    out_result->days = total_micros / Interval::MICROS_PER_DAY;
    total_micros -= out_result->days * Interval::MICROS_PER_DAY;
    out_result->micros = total_micros;
}
