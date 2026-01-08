#include <string>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "../cppjson.hpp"

using namespace CPPJSON;

static bool success;

static void testEmptyObject() {
    const std::string emptyObject = "{}";

    Parser parser;
    JSON &json = parser.parse(emptyObject);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::OBJECT);
}

static void testEmptyArray() {
    const std::string emptyArray = "[]";

    Parser parser;
    JSON &json = parser.parse(emptyArray);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::ARRAY);
}

static void testPrimitiveValues() {
    const std::string string = "\"\"";

    Parser parser;
    const JSON &json = parser.parse(string);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::STRING);

    const std::string int64 = "-125";
    const JSON &json2 = parser.parse(int64);
    assert(json.getError() == JSON::Error::NONE);
    assert(json2.getType() == JSON::Type::INT64);
    assert(json2.asInt64() == -125);
    assert(json2.asInt64(success) == -125);
    assert(success);

    const std::string uint64 = "2500";
    const JSON &json3 = parser.parse(uint64);
    assert(json.getError() == JSON::Error::NONE);
    assert(json3.getType() == JSON::Type::UINT64);
    assert(json3.asUint64() == 2500);
    assert(json3.asUint64(success) == 2500);
    assert(success);

    const std::string trueValue = "true";
    const JSON &json4 = parser.parse(trueValue);
    assert(json.getError() == JSON::Error::NONE);
    assert(json4.getType() == JSON::Type::BOOL);
    assert(json4.asBool());
    assert(json4.asBool(success));
    assert(success);

    const std::string falseValue = "false";
    const JSON &json5 = parser.parse(falseValue);
    assert(json.getError() == JSON::Error::NONE);
    assert(json5.getType() == JSON::Type::BOOL);
    assert(!json5.asBool());
    assert(!json5.asBool(success));
    assert(success);
}

static void testKeyValue() {
    const std::string keyValue = "{\"key\": \"value\"}";

    Parser parser;
    JSON &json = parser.parse(keyValue);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::OBJECT);

    JSON *const json2 = json["key"].get();
    assert(json2 != nullptr);
    assert(json2->asString() != nullptr);
    assert(json2->asString(success) != nullptr);
    assert(success);
    assert(*json2->asString() == "value");
    assert(*json2->asString(success) == "value");
    assert(success);

    assert(json["key"].asString() != nullptr);
    assert(json["key"].asString(success) != nullptr);
    assert(success);
}

static void testNestedObjects() {
    const std::string nestedObjects = "{"
        "\"key1\": {\"innerKey\": \"innerValue\"},"
        "\"key2\": \"value\""
    "}";

    Parser parser;
    JSON &json = parser.parse(nestedObjects);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::OBJECT);
    
    JSON *const innerObject = json["key1"].get();
    assert(innerObject != nullptr);
    assert(innerObject->getType() == JSON::Type::OBJECT);
    assert(innerObject->asObject() != nullptr);
    assert(innerObject->asObject(success) != nullptr);
    assert(success);
    
    success = json.asObject()->getObject("key1", [innerObject](Object &object) {
        assert(&object == innerObject->asObject());
    });
    assert(success);

    const String *const value = innerObject->asObject()->getString("innerKey");
    assert(value != nullptr);
    assert(*value == "innerValue");

    JSON *innerValue = json["key1"]["innerKey"].get();
    assert(innerValue != nullptr);
    assert(innerValue->getType() == JSON::Type::STRING);
    assert(innerValue->asString() != nullptr);
    assert(innerValue->asString(success) != nullptr);
    assert(success);
    assert(*innerValue->asString() == "innerValue");
    assert(*innerValue->asString(success) == "innerValue");
    assert(success);

    innerValue = json["key2"].get();
    assert(innerValue != nullptr);
    assert(innerValue->getType() == JSON::Type::STRING);
    assert(innerValue->asString() != nullptr);
    assert(innerValue->asString(success) != nullptr);
    assert(success);
    assert(*innerValue->asString() == "value");
    assert(*innerValue->asString(success) == "value");
    assert(success);


}

static void testObjectArray() {
    std::string objectArray = "["
        "{\"key1\": \"value1\"},"
        "{\"key2\": \"value2\"}"
    "]";

    Parser parser;
    JSON &json = parser.parse(objectArray);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::ARRAY);
    assert(json.asArray() != nullptr);
    assert(json.asArray(success) != nullptr);
    assert(success);
    assert(json.asArray()->size() == 2U);
    assert(json.asArray(success)->size() == 2U);
    assert(success);

    JSON *object = json[0U].get();
    assert(object != nullptr);
    assert(object->getType() == JSON::Type::OBJECT);
    assert(object->asObject() != nullptr);
    assert(object->asObject(success) != nullptr);
    assert(success);

    String *value = object->asObject()->getString("key1", success);
    assert(value != nullptr);
    assert(success);
    assert(*value == "value1");
    value = object->asObject()->getString("key1");
    assert(value != nullptr);
    assert(*value == "value1");

    success = object->asObject()->getString("key1", [](String& v){
        assert(v == "value1");
    });
    assert(success);
    success = object->asObject()->getString("key1", [](String& v){
        assert(v == "value1");
    }, []() {
        assert(false);
    });
    assert(success);
    success = object->asObject()->getString("key112345", [](String& v){
        (void)v;
        assert(false);
    }, []() {
        assert(true);
    });
    assert(!success);

    object = json[1].get();
    assert(object != nullptr);
    assert(object->getType() == JSON::Type::OBJECT);

    value = object->asObject()->getString("key2", success);
    assert(value != nullptr);
    assert(success);
    assert(*value == "value2");
    value = object->asObject()->getString("key2");
    assert(value != nullptr);
    assert(*value == "value2");
}

static void testEscapedCharacters() {
    const std::string escapedCharacters = "{\"key\": \"Line 1\\nLine 2\\\\\"}";

    Parser parser;
    JSON &json = parser.parse(escapedCharacters);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::OBJECT);

    String *value = json["key"].asString(success);
    assert(value != nullptr);
    assert(success);
    assert((*value)[6] == '\n');   
    value = json["key"].asString();
    assert(value != nullptr);
    assert((*value)[6] == '\n');   
}

static void testEscapedUnicode() {
    const std::string escapedUnicode = "{\"key\": \"Unicode test: \\u00A9\\u03A9\\uD840\\uDC00\"}";

    Parser parser;
    JSON &json = parser.parse(escapedUnicode);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::OBJECT);

    String *value = json["key"].asString(success);
    assert(value != nullptr);
    assert(success);
    value = json["key"].asString();
    assert(value != nullptr);

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

static void testBools() {
    const std::string bools = "{\"isTrue\": true, \"isFalse\": false}";

    Parser parser;
    JSON &json = parser.parse(bools);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::OBJECT);

    JSON *boolean = json["isTrue"].get();
    assert(boolean != nullptr);
    assert(boolean->getType() == JSON::Type::BOOL);
    assert(boolean->asBool());
    assert(boolean->asBool(success));
    assert(success);

    boolean = json["isFalse"].get();
    assert(boolean != nullptr);
    assert(boolean->getType() == JSON::Type::BOOL);
    assert(!boolean->asBool());
    assert(!boolean->asBool(success));
    assert(success);
}

static void testExponent() {
    const std::string exponent = "{\"largeNumber\": 1e15, \"negativeLarge\": -1e15}";

    Parser parser;
    JSON &json = parser.parse(exponent);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::OBJECT);

    uint64_t positive_number = json["largeNumber"].asUint64(success);
    assert(success);
    assert(positive_number == (uint64_t)1e15);
    positive_number = json["largeNumber"].asUint64();
    assert(positive_number == (uint64_t)1e15);

    int64_t negative_number = json["negativeLarge"].asInt64(success);
    assert(success);
    assert(negative_number == (int64_t)-1e15);
    negative_number = json["negativeLarge"].asInt64();
    assert(negative_number == (int64_t)-1e15);
}

static void testNull() {
    const std::string nullValue = "{\"key\": null}";

    Parser parser;
    JSON &json = parser.parse(nullValue);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::OBJECT);

    JSON *const null = json["key"].get();
    assert(null != nullptr);
    assert(null->getType() == JSON::Type::NUL);
    assert(null->asNull() == nullptr);
    assert(null->asNull(success) == nullptr);
    assert(success);
}

static void testMissingValue() {
    const std::string missingKey = "{\"key1\": \"value1\", \"key2\": }";

    Parser parser;
    JSON &json = parser.parse(missingKey);
    assert(json.getType() == JSON::Type::ERROR);
    assert(json.getError() == JSON::Error::OBJECT);
}

static void testComments() {
    const std::string comments = "{"
        "// This is a comment"
        "\"key\": \"value\""
    "}";

    Parser parser;
    JSON &json = parser.parse(comments);
    assert(json.getType() == JSON::Type::ERROR);
    assert(json.getError() == JSON::Error::TOKEN);
}

static void testDeepNesting() {
    const std::string deepNesting = "{\"key1\": {\"key2\": {\"key3\": {\"key4\": {\"key5\": [0, 1, 2, 3, 4, \"value\"]}}}}}";

    Parser parser;
    JSON &json = parser.parse(deepNesting);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::OBJECT);

    String *value = json["key1"]["key2"]["key3"]["key4"]["key5"][5U].asString();
    assert(value != nullptr);
    assert(*value == "value");
    value = json["key1"]["key2"]["key3"]["key4"]["key5"][5U].asString(success);
    assert(value != nullptr);
    assert(success);
    assert(*value == "value");
    
    success = json["key1"]["key2"]["key3"]["key4"]["key5"][5U].asString([](String &v) {
        assert(v == "value");
        assert(true);
    });
    assert(success);

    success = json["key1"]["key2"]["key3"]["key4"]["key5"][5U].asString([](String &v) {
        assert(v == "value");
        assert(true);
    }, []() {
        assert(false);
    });
    assert(success);

    success = json["key1"]["key2"]["key3"]["key4"]["key6"][5U].asString([](String &v) {
        (void)v;
        assert(false);
    }, []() {
        assert(true);
    });
    assert(!success);
}

static void testNoQuotesKey() {
    const std::string noQuotesKey = "{ key: 1 }";

    Parser parser;
    JSON &json = parser.parse(noQuotesKey);
    assert(json.getType() == JSON::Type::ERROR);
    assert(json.getError() == JSON::Error::TOKEN);
}

static void testNestedArrays() {
    const std::string nestedArrays = "[[1, 2, [3, 4]], [5, 6]]";

    Parser parser;
    JSON &json = parser.parse(nestedArrays);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::ARRAY);
    assert(json.asArray() != nullptr);
    assert(json.asArray(success) != nullptr);
    assert(success);
    assert(json.asArray()->size() == 2U);
    assert(json.asArray(success)->size() == 2U);
    assert(success);

    //[1, 2, [3, 4]]
    JSON *level1 = json[0U].get();
    assert(level1 != nullptr);
    assert(level1->getType() == JSON::Type::ARRAY);
    assert(level1->asArray() != nullptr);
    assert(level1->asArray(success) != nullptr);
    assert(success);
    assert(level1->asArray()->size() == 3U);
    assert(level1->asArray(success)->size() == 3U);
    assert(success);

    //1
    JSON *level2 = (*level1)[0U].get();
    assert(level2 != nullptr);
    assert(level2->getType() == JSON::Type::UINT64);
    assert(level2->asUint64() == 1U);
    assert(level2->asUint64(success) == 1U);
    assert(success);

    //2
    level2 = (*level1)[1].get();
    assert(level2 != nullptr);
    assert(level2->getType() == JSON::Type::UINT64);
    assert(level2->asUint64() == 2U);
    assert(level2->asUint64(success) == 2U);
    assert(success);

    //[3, 4]
    level2 = (*level1)[2].get();
    assert(level2 != nullptr);
    assert(level2->getType() == JSON::Type::ARRAY);
    assert(level2->asArray() != nullptr);
    assert(level2->asArray(success) != nullptr);
    assert(success);
    assert(level2->asArray()->size() == 2U);
    assert(level2->asArray(success)->size() == 2U);
    assert(success);

    //3
    JSON *level3 = (*level2)[0U].get();
    assert(level3 != nullptr);
    assert(level3->getType() == JSON::Type::UINT64);
    assert(level3->asUint64() == 3U);
    assert(level3->asUint64(success) == 3U);
    assert(success);

    //4
    level3 = (*level2)[1].get();
    assert(level3 != nullptr);
    assert(level3->getType() == JSON::Type::UINT64);
    assert(level3->asUint64() == 4U);
    assert(level3->asUint64(success) == 4U);
    assert(success);

    //[5, 6]
    level1 = json[1].get();
    assert(level1 != nullptr);
    assert(level1->getType() == JSON::Type::ARRAY);
    assert(level1->asArray() != nullptr);
    assert(level1->asArray(success) != nullptr);
    assert(success);
    assert(level1->asArray()->size() == 2U);
    assert(level1->asArray(success)->size() == 2U);
    assert(success);

    //5
    level2 = (*level1)[0U].get();
    assert(level2 != nullptr);
    assert(level2->getType() == JSON::Type::UINT64);
    assert(level2->asUint64() == 5U);
    assert(level2->asUint64(success) == 5U);
    assert(success);

    //6
    level2 = (*level1)[1].get();
    assert(level2 != nullptr);
    assert(level2->getType() == JSON::Type::UINT64);
    assert(level2->asUint64() == 6U);
    assert(level2->asUint64(success) == 6U);
    assert(success);
}

static void testDuplicateKeys() {
    const std::string duplicatedKeys = "{\"key\": \"value1\", \"key\": \"value2\"}";

    Parser parser;
    JSON &json = parser.parse(duplicatedKeys);
    assert(json.getError() == JSON::Error::NONE);
    assert(json.getType() == JSON::Type::OBJECT);

    String *value = json["key"].asString();
    assert(value != nullptr);
    assert(*value == "value2");
    value = json["key"].asString(success);
    assert(value != nullptr);
    assert(success);
    assert(*value == "value2");
}

static void testCreateString() {
    std::string value = "test";

    Parser parser;
    JSON &json = parser.init();
    assert(json.getError() == JSON::Error::NONE);

    json.set(value, parser.getStringAllocator());
    assert(json.getType() == JSON::Type::STRING);
    assert(json.asString() != nullptr);
    assert(json.asString(success) != nullptr);
    assert(success);
    assert(*json.asString() == "test");
    assert(*json.asString(success) == "test");
    assert(success);
}

static void testCreatePrimitives() {
    const int64_t value1 = -25000000000LL;

    Parser parser;
    JSON &json = parser.init();
    assert(json.getError() == JSON::Error::NONE);

    json.set(value1);
    assert(json.getType() == JSON::Type::INT64);
    assert(json.asInt64() == value1);
    assert(json.asInt64(success) == value1);
    assert(success);

    const uint64_t value2 = 25000000000ULL;
    json.set(value2);
    assert(json.getType() == JSON::Type::UINT64);
    assert(json.asUint64() == value2);
    assert(json.asUint64(success) == value2);
    assert(success);

    const double value3 = 25000000000.50;
    json.set(value3);
    assert(json.getType() == JSON::Type::FLOAT64);
    assert(json.asFloat64() == value3);
    assert(json.asFloat64(success) == value3);
    assert(success);

    const bool value4 = true;
    json.set(value4);
    assert(json.getType() == JSON::Type::BOOL);
    assert(json.asBool() == value4);
    assert(json.asBool(success) == value4);
    assert(success);

    json.set(nullptr);
    assert(json.getType() == JSON::Type::NUL);
    assert(json.asNull() == nullptr);
    assert(json.asNull(success) == nullptr);
    assert(success);
}

static void testCreateArray() {
    Parser parser;
    JSON &json = parser.init();
    assert(json.getError() == JSON::Error::NONE);

    Array *const array1 = json.makeArray(parser.getArrayAllocator());
    assert(array1 != nullptr);
    assert(json.getType() == JSON::Type::ARRAY);
    assert(json.asArray() == array1);
    assert(json.asArray(success) == array1);
    assert(success);

    const uint64_t value1 = 5ULL;
    const bool value2 = true;
    const int64_t value3 = -25000000000LL;

    Array array2(parser.getArrayAllocator());
    array2.set(0, value1);

    assert(array2[0] != nullptr);
    assert(array2[0]->getType() == JSON::Type::UINT64);
    assert(array2.get(0)->asUint64() == value1);
    assert(array2.get(0)->asUint64(success) == value1);
    assert(success);

    *(*array1)[0] = std::move(array2);
    *(*array1)[1] = value2;
    array1->set(2, value3);

    assert((*array1)[0] != nullptr);
    assert((*array1)[0]->getType() == JSON::Type::ARRAY);
    assert(array1->get(1) != nullptr);
    assert((*array1)[1]->getType() == JSON::Type::BOOL);
    assert((*array1)[1]->asBool() == value2);
    assert((*array1)[1]->asBool(success) == value2);
    assert(success);
    assert(array2[2] != nullptr);
    assert(array1->get(2)->getType() == JSON::Type::INT64);   
    assert((*array1)[2]->asInt64() == value3);
    assert((*array1)[2]->asInt64(success) == value3);
    assert(success);
}

static void testCreateObject() {
    Parser parser;
    JSON &json = parser.init();
    Object *const object1 = json.makeObject(parser.getObjectAllocator());
    assert(object1 != nullptr);
    assert(json.getType() == JSON::Type::OBJECT);
    assert(json.asObject() == object1);
    assert(json.asObject(success) == object1);
    assert(success);

    const uint64_t value1 = 5ULL;
    const bool value2 = true;
    const int64_t value3 = -25000000000LL;

    Object object2(parser.getObjectAllocator());
    object2.set("key1", value1);
    assert(object2["key1"] != nullptr);
    assert(object2["key1"]->getType() == JSON::Type::UINT64);
    assert(object2["key1"]->asUint64() == value1);
    assert(object2["key1"]->asUint64(success) == value1);
    assert(success);

    *(*object1)["key1"] = std::move(object2);
    *(*object1)["key2"] = value2;
    object1->set("key3", value3);

    assert((*object1)["key1"] != nullptr);
    assert((*object1)["key1"]->getType() == JSON::Type::OBJECT);
    assert((*object1)["key2"] != nullptr);
    assert((*object1)["key2"]->getType() == JSON::Type::BOOL);
    assert((*object1)["key2"]->asBool() == value2);
    assert((*object1)["key2"]->asBool(success) == value2);
    assert(success);
    assert((*object1)["key3"] != nullptr);
    assert((*object1)["key3"]->getType() == JSON::Type::INT64);
    assert((*object1)["key3"]->asInt64() == value3);
    assert((*object1)["key3"]->asInt64(success) == value3);
    assert(success);
}

int main() {
    testEmptyObject();
    testEmptyArray();
    testPrimitiveValues();
    testKeyValue();
    testNestedObjects();
    testObjectArray();
    testEscapedCharacters();
    testEscapedUnicode();
    testBools();
    testExponent();
    testNull();
    testMissingValue();
    testComments();
    testDeepNesting();
    testNoQuotesKey();
    testNestedArrays();
    testDuplicateKeys();
    testCreateString();
    testCreatePrimitives();
    testCreateArray();
    testCreateObject();

    std::cout << "All tests successful\n";

    return EXIT_SUCCESS;
}