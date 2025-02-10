#include <string>
#include <assert.h>
#include "../parser.hpp"

static void test_empty_object() {
    const std::string emptyObject = "{}";
    auto root = CJSON::parse(emptyObject);
    assert(root->type() == CJSON::Type::OBJECT);
}

static void test_empty_array() {
    const std::string emptyArray = "[]";
    auto root = CJSON::parse(emptyArray);
    assert(root->type() == CJSON::Type::ARRAY);
}

static void test_primitive_values() {
    const std::string string = "\"\"";
    auto root = CJSON::parse(string);
    assert(root->type() == CJSON::Type::STRING);
    assert(root->value().string == "");
    root->destroy();

    const std::string int64 = "-125";
    root = CJSON::parse(int64);
    assert(root->type() == CJSON::Type::INT64);
    assert(root->value().int64 == -125);
    root->destroy();

    const std::string uint64 = "2500";
    root = CJSON::parse(uint64);
    assert(root->type() == CJSON::Type::UINT64);
    assert(root->value().uint64 == 2500);
    root->destroy();

    const std::string trueValue = "true";
    root = CJSON::parse(trueValue);
    assert(root->type() == CJSON::Type::BOOL);
    assert(root->value().boolean);
    root->destroy();

    const std::string falseValue = "false";
    root = CJSON::parse(falseValue);
    assert(root->type() == CJSON::Type::BOOL);
    assert(!root->value().boolean);
    root->destroy(); 

    const std::string nullValue = "null";
    root = CJSON::parse(nullValue);
    assert(root->type() == CJSON::Type::NULL_T);
    assert(root->value().null == NULL);
}

static void test_key_value() {
    const std::string keyValue = "{\"key\": \"value\"}";
    auto root = CJSON::parse(keyValue);
    assert(root->type() == CJSON::Type::OBJECT);

    CJSON::JSON *const node = (*root)["key"].get();
    assert(node != nullptr);
    assert(node->value().string == "value");

    bool success;
    std::string *const value = (*root)["key"].getString(success);
    assert(success);
    assert(*value == "value");
}

static void test_nested_objects() {
    const std::string nestedObjects = "{"
        "\"key1\": {\"innerKey\": \"innerValue\"},"
        "\"key2\": \"value\""
    "}";

    auto root = CJSON::parse(nestedObjects);
    assert(root->type() == CJSON::Type::OBJECT);
    
    CJSON::JSON *innerObject = (*root)["key1"].get();
    assert(innerObject != nullptr);
    assert(innerObject->type() == CJSON::Type::OBJECT);

    bool success;
    std::string *value = innerObject->value().object.getString("innerKey", success);
    assert(success);
    assert(*value == "innerValue");

    CJSON::JSON *innerValue = (*root)["key1"]["innerKey"].get();
    assert(innerValue != nullptr);
    assert(innerValue->type() == CJSON::Type::STRING);
    assert(innerValue->value().string == "innerValue");

    innerValue = (*root)["key2"].get();
    assert(innerValue != nullptr);
    assert(innerValue->type() == CJSON::Type::STRING);
    assert(innerValue->value().string == "value");
}

static void test_object_array() {
    std::string objectArray = "["
        "{\"key1\": \"value1\"},"
        "{\"key2\": \"value2\"}"
    "]";

    auto root = CJSON::parse(objectArray);
    assert(root->type() == CJSON::Type::ARRAY);
    assert(root->value().array.size() == 2U);

    CJSON::JSON *object = (*root)[0].get();
    assert(object != nullptr);
    assert(object->type() == CJSON::Type::OBJECT);

    bool success;
    std::string *value = object->value().object.getString("key1", success);
    assert(success);
    assert(*value == "value1");

    object = (*root)[1].get();
    assert(object != nullptr);
    assert(object->type() == CJSON::Type::OBJECT);

    value = object->value().object.getString("key2", success);
    assert(success);
    assert(*value == "value2");
}

static void test_escaped_characters() {
    const std::string escapedCharacters = "{\"key\": \"Line 1\\nLine 2\\\\\"}";

    auto root = CJSON::parse(escapedCharacters);
    assert(root->type() == CJSON::Type::OBJECT);
    bool success;

    std::string *value = (*root)["key"].getString(success);
    assert(success);
    assert((*value)[6] == '\n');   
}

static void test_escaped_unicode() {
    const std::string escapedUnicode = "{\"key\": \"Unicode test: \\u00A9\\u03A9\\uD840\\uDC00\"}";

    auto root = CJSON::parse(escapedUnicode);
    assert(root->type() == CJSON::Type::OBJECT);

    bool success;
    std::string *const value = (*root)["key"].getString(success);
    assert(success);

    //\u00A9\u03A9 == ©Ω == (unsigned utf8) {194, 169, 206, 169} == (signed utf8) {-62, -87, -50, -87 }
    assert((*value)[14] == -62);
    assert((*value)[15] == -87);
    assert((*value)[16] == -50);
    assert((*value)[17] == -87);
    //\uD840\\uDC00 == 𠀀 == (unsigned utf8) {240, 160, 128, 128} == (signed utf8) {-16, -96, -128, -128}
    assert((*value)[18] == -16);
    assert((*value)[19] == -96);
    assert((*value)[20] == -128);
    assert((*value)[21] == -128);
}

static void test_bools() {
    const std::string bools = "{\"isTrue\": true, \"isFalse\": false}";

    auto root = CJSON::parse(bools);
    assert(root->type() == CJSON::Type::OBJECT);

    CJSON::JSON *node = (*root)["isTrue"].get();
    assert(node != nullptr);
    assert(node->type() == CJSON::Type::BOOL);
    assert(node->value().boolean);

    node = (*root)["isFalse"].get();
    assert(node != nullptr);
    assert(node->type() == CJSON::Type::BOOL);
    assert(!node->value().boolean);
}

static void test_exponent() {
    const std::string exponent = "{\"largeNumber\": 1e15, \"negativeLarge\": -1e15}";

    auto root = CJSON::parse(exponent);
    assert(root->type() == CJSON::Type::OBJECT);

    bool success;
    const uint64_t positive_number = (*root)["largeNumber"].getUint64(success);
    assert(success);
    assert(positive_number == (uint64_t)1e15);

    const int64_t negative_number = (*root)["negativeLarge"].getInt64(success);
    assert(success);
    assert(negative_number == (int64_t)-1e15);
}

static void test_null() {
    const std::string nullValue = "{\"key\": null}";

    auto root = CJSON::parse(nullValue);
    assert(root->type() == CJSON::Type::OBJECT);

    CJSON::JSON *const node = (*root)["key"].get();
    assert(node != nullptr);
    assert(node->type() == CJSON::Type::NULL_T);
    assert(node->value().null == nullptr);

    bool success;
    void *null = (*root)["key"].getNull(success);
    assert(success);
    assert(null == nullptr);
}

static void test_missing_value() {
    const std::string missingKey = "{\"key1\": \"value1\", \"key2\": }";

    auto root = CJSON::parse(missingKey);
    assert(root->type() == CJSON::Type::ERROR);
    assert(root->value().error == CJSON::Error::OBJECT_FAILED_TO_PARSE);
}

static void test_comments() {
    const std::string comments = "{"
        "// This is a comment"
        "\"key\": \"value\""
    "}";

    auto root = CJSON::parse(comments);
    assert(root->type() == CJSON::Type::ERROR);
    assert(root->value().error == CJSON::Error::TOKEN_ERROR);
}

static void test_deep_nesting() {
    const std::string deepNesting = "{\"key1\": {\"key2\": {\"key3\": {\"key4\": {\"key5\": \"value\"}}}}}";

    auto root = CJSON::parse(deepNesting);
    assert(root->type() == CJSON::Type::OBJECT);

    bool success;
    std::string *const value = (*root)["key1"]["key2"]["key3"]["key4"]["key5"].getString(success);
    assert(success);
    assert(*value == "value");
}

static void test_no_quotes_key() {
    const std::string noQuotesKey = "{ key: 1 }";

    auto root = CJSON::parse(noQuotesKey);
    assert(root->type() == CJSON::Type::ERROR);
    assert(root->value().error == CJSON::Error::TOKEN_ERROR);
}

static void test_nested_arrays() {
    const std::string nestedArrays = "[[1, 2, [3, 4]], [5, 6]]";

    auto root = CJSON::parse(nestedArrays);
    assert(root->type() == CJSON::Type::ARRAY);
    assert(root->value().array.size() == 2U);

    //[1, 2, [3, 4]]
    CJSON::JSON *level1_node = (*root)[0].get();
    assert(level1_node != nullptr);
    assert(level1_node->type() == CJSON::Type::ARRAY);
    assert(level1_node->value().array.size() == 3U);

    //1
    CJSON::JSON *level2_node = (*level1_node)[0].get();
    assert(level2_node != nullptr);
    assert(level2_node->type() == CJSON::Type::UINT64);
    assert(level2_node->value().uint64 == 1U);
    //2
    level2_node = (*level1_node)[1].get();
    assert(level2_node != nullptr);
    assert(level2_node->type() == CJSON::Type::UINT64);
    assert(level2_node->value().uint64 == 2U);
    //[3, 4]
    level2_node = (*level1_node)[2].get();
    assert(level2_node != nullptr);
    assert(level2_node->type() == CJSON::Type::ARRAY);
    assert(level2_node->value().array.size() == 2U);
    //3
    CJSON::JSON *level3_node = (*level2_node)[0].get();
    assert(level3_node != nullptr);
    assert(level3_node->type() == CJSON::Type::UINT64);
    assert(level3_node->value().uint64 == 3U);
    //4
    level3_node = (*level2_node)[1].get();
    assert(level3_node != nullptr);
    assert(level3_node->type() == CJSON::Type::UINT64);
    assert(level3_node->value().uint64 == 4U);

    //[5, 6]
    level1_node = (*root)[1].get();
    assert(level1_node != nullptr);
    assert(level1_node->type() == CJSON::Type::ARRAY);
    assert(level1_node->value().array.size() == 2U);

    //5
    level2_node = (*level1_node)[0].get();
    assert(level2_node != nullptr);
    assert(level2_node->type() == CJSON::Type::UINT64);
    assert(level2_node->value().uint64 == 5U);
    //6
    level2_node = (*level1_node)[1].get();
    assert(level2_node != nullptr);
    assert(level2_node->type() == CJSON::Type::UINT64);
    assert(level2_node->value().uint64 == 6U);
}

static void test_duplicate_keys() {
    const std::string duplicatedKeys = "{\"key\": \"value1\", \"key\": \"value2\"}";

    auto root = CJSON::parse(duplicatedKeys);
    assert(root->type() == CJSON::Type::OBJECT);

    bool success;
    std::string *const value = (*root)["key"].getString(success);
    assert(success);
    assert(*value == "value2");
}

static void test_create_string() {
    std::string value = "test";
    CJSON::JSON root;
    root.set(std::move(value));
    assert(root.type() == CJSON::Type::STRING);
    assert(root.value().string == "test");
}

static void test_create_primitives() {
    const int64_t value1 = -25000000000LL;
    CJSON::JSON root;
    root.set(value1);
    assert(root.type() == CJSON::Type::INT64);
    assert(root.value().int64 == value1);

    const uint64_t value2 = 25000000000ULL;
    root.set(value2);
    assert(root.type() == CJSON::Type::UINT64);
    assert(root.value().uint64 == value2);

    const double value3 = 25000000000.50;
    root.set(value3);
    assert(root.type() == CJSON::Type::FLOAT64);
    assert(root.value().float64 == value3);

    const bool value4 = true;
    root.set(value4);
    assert(root.type() == CJSON::Type::BOOL);
    assert(root.value().boolean == value4);

    root.set(nullptr);
    assert(root.type() == CJSON::Type::NULL_T);
    assert(root.value().null == nullptr);
}

static void test_create_array() {
    CJSON::JSON root;
    CJSON::Array &array1 = root.makeArray();
    assert(root.type() == CJSON::Type::ARRAY);
    assert(&root.value().array == &array1);

    const uint64_t value1 = 5ULL;
    const bool value2 = true;
    const int64_t value3 = -25000000000LL;

    CJSON::Array array2;
    array2.set(0, value1);

    bool success;
    assert(array2.get(0, success)->type() == CJSON::Type::UINT64);
    assert(success);
    assert(array2.get(0, success)->value().uint64 == value1);

    array1.set(0, std::move(array2));
    array1.set(1, value2);
    array1.set(2, value3);

    assert(array1.get(0, success)->type() == CJSON::Type::ARRAY);
    assert(success);
    assert(array1.get(1, success)->type() == CJSON::Type::BOOL);
    assert(success);
    assert(array1.get(1, success)->value().boolean == value2);
    assert(array1.get(2, success)->type() == CJSON::Type::INT64);   
    assert(success);
    assert(array1.get(2, success)->value().int64 == value3);
}

static void test_create_object() {
    CJSON::JSON root;
    CJSON::Object &object1 = root.makeObject();
    assert(root.type() == CJSON::Type::OBJECT);
    assert(&root.value().object == &object1);

    const uint64_t value1 = 5ULL;
    const bool value2 = true;
    const int64_t value3 = -25000000000LL;

    CJSON::Object object2;
    object2.set("key1", value1);
    assert(object2["key1"] != nullptr);
    assert(object2["key1"]->type() == CJSON::Type::UINT64);
    assert(object2["key1"]->value().uint64 == value1);
    
    object1.set("key1", std::move(object2));
    object1.set("key2", value2);
    object1.set("key3", value3);

    assert(object1["key1"] != nullptr);
    assert(object1["key1"]->type() == CJSON::Type::OBJECT);
    assert(object1["key2"] != nullptr);
    assert(object1["key2"]->type() == CJSON::Type::BOOL);
    assert(object1["key2"]->value().boolean == value2);
    assert(object1["key3"] != nullptr);
    assert(object1["key3"]->type() == CJSON::Type::INT64);
    assert(object1["key3"]->value().int64 == value3);
}

int main() {
    test_empty_object();
    test_empty_array();
    test_primitive_values();
    test_key_value();
    test_nested_objects();
    test_object_array();
    test_escaped_characters();
    test_escaped_unicode();
    test_bools();
    test_exponent();
    test_null();
    test_missing_value();
    test_comments();
    test_deep_nesting();
    test_no_quotes_key();
    test_nested_arrays();
    test_duplicate_keys();
    test_create_string();
    test_create_primitives();
    test_create_array();
    test_create_object();

    return 0;
}