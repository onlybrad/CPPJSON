#include <string>
#include <assert.h>
#include "../parser.hpp"

static void test_empty_object(void) {
    const std::string emptyObject = "{}";
    auto root = CJSON::JSON::parse(emptyObject);
    assert(root->type() == CJSON::Type::OBJECT);
}

static void test_empty_array(void) {
    const std::string emptyArray = "[]";
    auto root = CJSON::JSON::parse(emptyArray);
    assert(root->type() == CJSON::Type::ARRAY);
}

static void test_primitive_values(void) {
    const std::string string = "\"\"";
    auto root = CJSON::JSON::parse(string);
    assert(root->type() == CJSON::Type::STRING);
    assert(root->value().string == "");
    root->destroy();

    const std::string int64 = "-125";
    root = CJSON::JSON::parse(int64);
    assert(root->type() == CJSON::Type::INT64);
    assert(root->value().int64 == -125);
    root->destroy();

    const std::string uint64 = "2500";
    root = CJSON::JSON::parse(uint64);
    assert(root->type() == CJSON::Type::UINT64);
    assert(root->value().uint64 == 2500);
    root->destroy();

    const std::string trueValue = "true";
    root = CJSON::JSON::parse(trueValue);
    assert(root->type() == CJSON::Type::BOOL);
    assert(root->value().boolean);
    root->destroy();

    const std::string falseValue = "false";
    root = CJSON::JSON::parse(falseValue);
    assert(root->type() == CJSON::Type::BOOL);
    assert(!root->value().boolean);
    root->destroy(); 

    const std::string nullValue = "null";
    root = CJSON::JSON::parse(nullValue);
    assert(root->type() == CJSON::Type::NULL_T);
    assert(root->value().null == NULL);
}

static void test_key_value(void) {
    const std::string keyValue = "{\"key\": \"value\"}";
    auto root = CJSON::JSON::parse(keyValue);
    assert(root->type() == CJSON::Type::OBJECT);

    CJSON::Node *const node = (*root)["key"].get();
    assert(node != nullptr);
    assert(node->value().string == "value");

    bool success;
    std::string *const value = (*root)["key"].getString(success);
    assert(success);
    assert(*value == "value");
}

static void test_nested_objects(void) {
    const std::string nestedObjects = "{"
        "\"key1\": {\"innerKey\": \"innerValue\"},"
        "\"key2\": \"value\""
    "}";

    auto root = CJSON::JSON::parse(nestedObjects);
    assert(root->type() == CJSON::Type::OBJECT);
    
    CJSON::Node *innerObject = (*root)["key1"].get();
    assert(innerObject != nullptr);
    assert(innerObject->type() == CJSON::Type::OBJECT);

    bool success;
    std::string *value = innerObject->value().object.getString("innerKey", success);
    assert(success);
    assert(*value == "innerValue");

    CJSON::Node *innerValue = (*root)["key1"]["innerKey"].get();
    assert(innerValue != nullptr);
    assert(innerValue->type() == CJSON::Type::STRING);
    assert(innerValue->value().string == "innerValue");

    innerValue = (*root)["key2"].get();
    assert(innerValue != nullptr);
    assert(innerValue->type() == CJSON::Type::STRING);
    assert(innerValue->value().string == "value");
}

static void test_object_array(void) {
    std::string objectArray = "["
        "{\"key1\": \"value1\"},"
        "{\"key2\": \"value2\"}"
    "]";

    auto root = CJSON::JSON::parse(objectArray);
    assert(root->type() == CJSON::Type::ARRAY);
    assert(root->value().array.size() == 2U);

    CJSON::Node *object = (*root)[0].get();
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

static void test_escaped_characters(void) {
    const std::string escapedCharacters = "{\"key\": \"Line 1\\nLine 2\\\\\"}";

    auto root = CJSON::JSON::parse(escapedCharacters);
    assert(root->type() == CJSON::Type::OBJECT);
    bool success;

    std::string *value = (*root)["key"].getString(success);
    assert(success);
    assert((*value)[6] == '\n');   
}

static void test_escaped_unicode(void) {
    const std::string escapedUnicode = "{\"key\": \"Unicode test: \\u00A9\\u03A9\\uD840\\uDC00\"}";

    auto root = CJSON::JSON::parse(escapedUnicode);
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

static void test_bools(void) {
    const std::string bools = "{\"isTrue\": true, \"isFalse\": false}";

    auto root = CJSON::JSON::parse(bools);
    assert(root->type() == CJSON::Type::OBJECT);

    CJSON::Node *node = (*root)["isTrue"].get();
    assert(node != nullptr);
    assert(node->type() == CJSON::Type::BOOL);
    assert(node->value().boolean);

    node = (*root)["isFalse"].get();
    assert(node != nullptr);
    assert(node->type() == CJSON::Type::BOOL);
    assert(!node->value().boolean);
}

static void test_exponent(void) {
    const std::string exponent = "{\"largeNumber\": 1e15, \"negativeLarge\": -1e15}";

    auto root = CJSON::JSON::parse(exponent);
    assert(root->type() == CJSON::Type::OBJECT);

    bool success;
    const uint64_t positive_number = (*root)["largeNumber"].getUint64(success);
    assert(success);
    assert(positive_number == (uint64_t)1e15);

    const int64_t negative_number = (*root)["negativeLarge"].getInt64(success);
    assert(success);
    assert(negative_number == (int64_t)-1e15);
}

static void test_null(void) {
    const std::string nullValue = "{\"key\": null}";

    auto root = CJSON::JSON::parse(nullValue);
    assert(root->type() == CJSON::Type::OBJECT);

    CJSON::Node *const node = (*root)["key"].get();
    assert(node != nullptr);
    assert(node->type() == CJSON::Type::NULL_T);
    assert(node->value().null == nullptr);

    bool success;
    void *null = (*root)["key"].getNull(success);
    assert(success);
    assert(null == nullptr);
}

static void test_missing_value(void) {
    const std::string missingKey = "{\"key1\": \"value1\", \"key2\": }";

    auto root = CJSON::JSON::parse(missingKey);
    assert(root->type() == CJSON::Type::ERROR);
    assert(root->value().error == CJSON::Error::OBJECT_FAILED_TO_PARSE);
}

static void test_comments(void) {
    const std::string comments = "{"
        "// This is a comment"
        "\"key\": \"value\""
    "}";

    auto root = CJSON::JSON::parse(comments);
    assert(root->type() == CJSON::Type::ERROR);
    assert(root->value().error == CJSON::Error::TOKEN_ERROR);
}

static void test_deep_nesting(void) {
    const std::string deepNesting = "{\"key1\": {\"key2\": {\"key3\": {\"key4\": {\"key5\": \"value\"}}}}}";

    auto root = CJSON::JSON::parse(deepNesting);
    assert(root->type() == CJSON::Type::OBJECT);

    bool success;
    std::string *const value = (*root)["key1"]["key2"]["key3"]["key4"]["key5"].getString(success);
    assert(success);
    assert(*value == "value");
}

static void test_no_quotes_key(void) {
    const std::string noQuotesKey = "{ key: 1 }";

    auto root = CJSON::JSON::parse(noQuotesKey);
    assert(root->type() == CJSON::Type::ERROR);
    assert(root->value().error == CJSON::Error::TOKEN_ERROR);
}

static void test_nested_arrays(void) {
    const std::string nestedArrays = "[[1, 2, [3, 4]], [5, 6]]";

    auto root = CJSON::JSON::parse(nestedArrays);
    assert(root->type() == CJSON::Type::ARRAY);
    assert(root->value().array.size() == 2U);

    //[1, 2, [3, 4]]
    CJSON::Node *level1_node = (*root)[0].get();
    assert(level1_node != nullptr);
    assert(level1_node->type() == CJSON::Type::ARRAY);
    assert(level1_node->value().array.size() == 3U);

    //1
    CJSON::Node *level2_node = (*level1_node)[0].get();
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
    CJSON::Node *level3_node = (*level2_node)[0].get();
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

static void test_duplicate_keys(void) {
    const std::string duplicatedKeys = "{\"key\": \"value1\", \"key\": \"value2\"}";

    auto root = CJSON::JSON::parse(duplicatedKeys);
    assert(root->type() == CJSON::Type::OBJECT);

    bool success;
    std::string *const value = (*root)["key"].getString(success);
    assert(success);
    assert(*value == "value2");
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

    return 0;
}