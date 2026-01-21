#include <string>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "../cppjson.hpp"

using namespace CPPJSON;

static void testEmptyObject() {
    const std::string emptyObject = "{}";

    Parser parser;
    const ParserResult parserResult = parser.parse(emptyObject);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::OBJECT);
}

static void testEmptyArray() {
    const std::string emptyArray = "[]";

    Parser parser;
    const ParserResult parserResult = parser.parse(emptyArray);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::ARRAY);
}

static void testPrimitiveValues() {
    const std::string string = "\"\"";

    Parser parser;
    ParserResult parserResult = parser.parse(string);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::STRING);
    assert(json.asString().getRef() == "");
    assert(json.unsafeAsString() == "");
    json.asString().onSuccess([](const String &value) {
        assert(value == "");
    }).onError([]() {
        assert(false);
    });

    const std::string int64 = "-125";
    parserResult = parser.parse(int64);
    assert(parserResult.isSuccess());
    const JSON &json2 = parserResult.getRef();
    assert(json2.getType() == JSON::Type::INT64);
    assert(json2.asInt64().isSuccess());
    assert(json2.asInt64().getValue() == -125);
    assert(json2.unsafeAsInt64() == -125);
    assert(!json2.asUint64().isSuccess());
    assert(json2.unsafeAsFloat64() == -125.0F);

    json2.asInt64().onSuccess([](const std::int64_t value) {
        assert(value == -125);
    }).onError([]() {
        assert(false);
    });

    const std::string uint64 = "2500";
    parserResult = parser.parse(uint64);
    assert(parserResult.isSuccess());
    const JSON &json3 = parserResult.getRef();
    assert(json3.getType() == JSON::Type::UINT64);
    assert(json3.asUint64().isSuccess());
    assert(json3.asUint64().getValue() == 2500);
    assert(json3.unsafeAsUint64() == 2500);
    assert(json3.asInt64().isSuccess());
    assert(json3.unsafeAsInt64() == 2500);
    assert(json3.asFloat64().isSuccess());
    assert(json3.unsafeAsFloat64() == 2500.0F);

    json3.asUint64().onSuccess([](const std::int64_t value) {
        assert(value == 2500);
    }).onError([]() {
        assert(false);
    });

    const std::string trueValue = "true";
    parserResult = parser.parse(trueValue);
    assert(parserResult.isSuccess());
    const JSON &json4 = parserResult.getRef();
    assert(json4.getType() == JSON::Type::BOOL);
    assert(json4.asBool().isSuccess());
    assert(json4.asBool().getValue());
    json4.asBool().onSuccess([](const bool value) {
        assert(value);
    }).onError([]() {
        assert(false);
    });

    const std::string falseValue = "false";
    parserResult = parser.parse(falseValue);
    assert(parserResult.isSuccess());
    const JSON &json5 = parserResult.getRef();
    assert(json5.getType() == JSON::Type::BOOL);
    assert(json5.asBool().isSuccess());
    assert(!json5.asBool().getValue());
    json5.asBool().onSuccess([](const bool value) {
        assert(!value);
    }).onError([](){
        assert(false);
    });
}

static void testKeyValue() {
    const std::string keyValue = "{\"key\": \"value\"}";

    Parser parser;
    const ParserResult parserResult = parser.parse(keyValue);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::OBJECT);
    const String key = parser.createString("key");
    assert(json[key].get().isSuccess());

    const Result<const JSON&> jsonResult = json["key"].get();
    const JSON &json2 = jsonResult.getRef();
    assert(json2.asString().isSuccess());
    assert(json2.asString().getRef() == "value");
}

static void testNestedObjects() {
    const std::string nestedObjects = "{"
        "\"key1\": {\"innerKey\": \"innerValue\"},"
        "\"key2\": \"value\""
    "}";

    Parser parser;
    const ParserResult parserResult = parser.parse(nestedObjects);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::OBJECT);
    assert(json.asObject().isSuccess());
    
    const Result<const JSON&> innerJSONResult = json["key1"].get();
    assert(innerJSONResult.isSuccess());
    const JSON &innerJSON = innerJSONResult.getRef();
    assert(json.getType() == JSON::Type::OBJECT);
    assert(innerJSON.asObject().isSuccess());

    const Result<const Object&> innerJSON2Result = innerJSON.asObject();
    assert(innerJSON2Result.isSuccess());
    const Object &innerObject2 = innerJSON2Result.getRef();
    innerJSON.asObject().onSuccess([&innerObject2](const Object &value) {
        assert(&value == &innerObject2);    
    }).onError([](){ 
        assert(false);
    });

    const String innerKey = parser.createString("innerKey");
    assert(innerObject2.getString(innerKey).isSuccess());
    assert(innerObject2.getString(innerKey).getRef() == "innerValue");

    const Result<const JSON&> &innerJSON3Result = json["key2"].get();
    assert(innerJSON3Result.isSuccess());
    const JSON &innerJSON3 = innerJSON3Result.getRef();
    assert(innerJSON3.getType() == JSON::Type::STRING);

    innerJSON3.asString().onSuccess([](const String &string) {
        assert(string == "value");
    })
    .onError([]() {
        assert(false);
    });
}

static void testObjectArray() {
    std::string objectArray = "["
        "{\"key1\": \"value1\"},"
        "{\"key2\": \"value2\"}"
    "]";

    Parser parser;
    const ParserResult parserResult = parser.parse(objectArray);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::ARRAY);
    assert(json.asArray().isSuccess());
    assert(json.asArray().getRef().size() == 2U);
    
    json.asArray().onSuccess([](const Array &array) {
        assert(array.size() == 2U);
    }).onError([]() {
        assert(false);
    });

    Result<const Array&> arrayResult = json.asArray();
    assert(arrayResult.isSuccess());
    const Array &array = arrayResult.getRef();
    const JSON &json2 = array[0U];
    assert(json2.getType() == JSON::Type::OBJECT);
    assert(json2.asObject().isSuccess());

    Result<const Object&> objectResult = json2.asObject();
    assert(objectResult.isSuccess());
    const Object &object = objectResult.getRef();

    const String key1 = parser.createString("key1");
    assert(object.get(key1).isSuccess());
    object.get(key1).onSuccess([](const JSON &innerJSON){
        assert(innerJSON.getType() == JSON::Type::STRING);
        assert(innerJSON.asString().isSuccess());
        assert(innerJSON.asString().getRef() == "value1");
    }).onError([]() {
        assert(false);
    });

    const JSON &json3 = array[1U];
    assert(json3.getType() == JSON::Type::OBJECT);
    assert(json3.asObject().isSuccess());

    objectResult = json3.asObject();
    assert(objectResult.isSuccess());
    const Object &object2 = objectResult.getRef();

    const String key2 = parser.createString("key2");
    assert(object2.get(key2).isSuccess());
    object2.get(key2).onSuccess([](const JSON &innerJSON){
        assert(innerJSON.getType() == JSON::Type::STRING);
        assert(innerJSON.asString().isSuccess());
        assert(innerJSON.asString().getRef() == "value2");
    }).onError([]() {
        assert(false);
    });

    const String key3 = parser.createString("key3");
    assert(!object2.get(key3).isSuccess());
    object2.get(key3).onSuccess([](const JSON&) {
        assert(false);
    }).onError([]() {
        assert(true);
    });
}

static void testEscapedCharacters() {
    const std::string escapedCharacters = "{\"key\": \"Line 1\\nLine 2\\\\\"}";

    Parser parser;
    const ParserResult parserResult = parser.parse(escapedCharacters);
    assert(parserResult.isSuccess());
    assert(parserResult->getType() == JSON::Type::OBJECT);
    
    const Result<String&> stringResult = parserResult.getRef()["key"].asString();
    assert(stringResult.isSuccess());
    const String &value = stringResult.getRef();
    assert(value[6] == '\n');   
}

static void testEscapedUnicode() {
    const std::string escapedUnicode = "{\"key\": \"Unicode test: \\u00A9\\u03A9\\uD840\\uDC00\"}";

    Parser parser;
    const ParserResult parserResult = parser.parse(escapedUnicode);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(parserResult->getType() == JSON::Type::OBJECT);
    assert(json["key"].asString().isSuccess());

    const Result<const String &> stringResult = json["key"].asString();
    assert(stringResult.isSuccess());
    const String &value = stringResult.getRef();

    //\u00A9\u03A9 == ©Ω == (unsigned utf8) {194, 169, 206, 169} == (signed utf8) {-62, -87, -50, -87 }
    assert(value[14] == -62);
    assert(value[15] == -87);
    assert(value[16] == -50);
    assert(value[17] == -87);
    //\uD840\\uDC00 == 𠀀 == (unsigned utf8) {240, 160, 128, 128} == (signed utf8) {-16, -96, -128, -128}
    assert(value[18] == -16);
    assert(value[19] == -96);
    assert(value[20] == -128);
    assert(value[21] == -128);
}

static void testBools() {
    const std::string bools = "{\"isTrue\": true, \"isFalse\": false}";

    Parser parser;
    const ParserResult parserResult = parser.parse(bools);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::OBJECT);
    assert(json["isTrue"].get().isSuccess());

    Result<const JSON&> booleanResult = json["isTrue"].get();
    assert(booleanResult.isSuccess());
    const JSON &boolean = booleanResult.getRef();
    assert(boolean.getType() == JSON::Type::BOOL);
    assert(boolean.asBool().getValue());

    booleanResult = json["isFalse"].get();
    assert(booleanResult.isSuccess());
    const JSON &boolean2 = booleanResult.getRef();
    assert(boolean2.getType() == JSON::Type::BOOL);
    assert(!boolean2.asBool().getValue());
}

static void testExponent() {
    const std::string exponent = "{\"largeNumber\": 1e15, \"negativeLarge\": -1e15}";

    Parser parser;
    const ParserResult parserResult = parser.parse(exponent);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::OBJECT);

    assert(json["largeNumber"].asUint64().isSuccess());
    const std::int64_t positive_number = json["largeNumber"].asUint64().getValue();
    assert(positive_number == (std::int64_t)1e15);

    assert(json["negativeLarge"].asInt64().isSuccess());
    const std::int64_t negative_number = json["negativeLarge"].asInt64().getValue();
    assert(negative_number == (std::int64_t)-1e15);

}

static void testNull() {
    const std::string nullValue = "{\"key\": null}";

    Parser parser;
    const ParserResult parserResult = parser.parse(nullValue);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::OBJECT);
    assert(json["key"].asNull().isSuccess());
}

static void testMissingValue() {
    const std::string missingKey = "{\"key1\": \"value1\", \"key2\": }";

    Parser parser;
    const ParserResult parserResult = parser.parse(missingKey);
    assert(!parserResult.isSuccess());
    assert(parserResult.getError() == Error::OBJECT);
}

static void testComments() {
    const std::string comments = "{"
        "// This is a comment"
        "\"key\": \"value\""
    "}";

    Parser parser;
    const ParserResult parserResult = parser.parse(comments);
    assert(!parserResult.isSuccess());
    assert(parserResult.getError() == Error::TOKEN);
}

static void testDeepNesting() {
    const std::string deepNesting = "{\"key1\": {\"key2\": {\"key3\": {\"key4\": {\"key5\": [0, 1, 2, 3, 4, \"value\"]}}}}}";

    Parser parser;
    const ParserResult parserResult = parser.parse(deepNesting);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();

    Result<const String&> stringResult = json["key1"]["key2"]["key3"]["key4"]["key5"][5U].asString();
    assert(stringResult.isSuccess());
    const String &string = stringResult.getRef();
    assert(string == "value");
    
    json["key1"]["key2"]["key3"]["key4"]["key5"][5U].asString().onSuccess([](const String &v) {
        assert(v == "value");
        assert(true);
    });
}

static void testNoQuotesKey() {
    const std::string noQuotesKey = "{ key: 1 }";

    Parser parser;
    const ParserResult parserResult = parser.parse(noQuotesKey);
    assert(!parserResult.isSuccess());
    assert(parserResult.getError() == Error::TOKEN);
}

static void testNestedArrays() {
    const std::string nestedArrays = "[[1, 2, [3, 4]], [5, 6]]";

    Parser parser;
    const ParserResult parserResult = parser.parse(nestedArrays);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::ARRAY);
    assert(json.asArray().getRef().size() == 2U);

    //[1, 2, [3, 4]]
    Result<const JSON&> level1Result = json[0U].get();
    assert(level1Result.isSuccess());
    const JSON &level1 = level1Result.getRef();
    assert(level1.getType() == JSON::Type::ARRAY);
    assert(level1.asArray().isSuccess());
    assert(level1.asArray().getRef().size() == 3U);

    //1
    Result<const JSON&> level2Result = level1[0U].get();
    assert(level2Result.isSuccess());
    const JSON &level2 = level2Result.getRef();
    assert(level2.getType() == JSON::Type::UINT64);
    assert(level2.asUint64().getValue() == 1U);

    //2
    level2Result = level1[1U].get();
    assert(level2Result.isSuccess());
    const JSON &level2_2 = level2Result.getRef();
    assert(level2_2.getType() == JSON::Type::UINT64);
    assert(level2_2.asUint64().getValue() == 2U);

    //[3, 4]
    level2Result = level1[2].get();
    assert(level2Result.isSuccess());
    const JSON &level2_3 = level2Result.getRef();

    assert(level2_3.getType() == JSON::Type::ARRAY);
    assert(level2_3.asArray().isSuccess());
    assert(level2_3.asArray().getRef().size() == 2U);

    //3
    Result<const JSON&> level3Result = level2_3[0U].get();
    assert(level3Result.isSuccess());
    const JSON &level3 = level3Result.getRef();
    assert(level3.getType() == JSON::Type::UINT64);
    assert(level3.asUint64().getValue() == 3U);

    //4
    level3Result = level2_3[1].get();
    assert(level3Result.isSuccess());
    const JSON &level3_2 = level3Result.getRef();
    assert(level3_2.getType() == JSON::Type::UINT64);
    assert(level3_2.asUint64().getValue() == 4U);

    //[5, 6]
    level1Result = json[1].get();
    assert(level1Result.isSuccess());
    const JSON &level1_2 = level1Result.getRef();
    assert(level1_2.getType() == JSON::Type::ARRAY);
    assert(level1_2.asArray().isSuccess());
    assert(level1_2.asArray().getRef().size() == 2U);

    //5
    level2Result = level1_2[0U].get();
    assert(level2Result.isSuccess());
    const JSON &level2_4 = level2Result.getRef();
    assert(level2_4.getType() == JSON::Type::UINT64);
    assert(level2_4.asUint64().getValue() == 5U);

    //6
    level2Result = level1_2[1U].get();
    assert(level2Result.isSuccess());
    const JSON &level2_5 = level2Result.getRef();
    assert(level2_5.getType() == JSON::Type::UINT64);
    assert(level2_5.asUint64().getValue() == 6U);
}

static void testDuplicateKeys() {
    const std::string duplicatedKeys = "{\"key\": \"value1\", \"key\": \"value2\"}";

    Parser parser;
    const ParserResult parserResult = parser.parse(duplicatedKeys);
    assert(parserResult.isSuccess());
    const JSON &json = parserResult.getRef();
    assert(json.getType() == JSON::Type::OBJECT);

    Result<const String&> stringResult = json["key"].asString();
    assert(stringResult.isSuccess());
    const String &value = stringResult.getRef();
    assert(value == "value2");
}

static void testCreateString() {
    std::string value = "test";

    Parser parser;
    const ParserResult parserResult = parser.init();
    assert(parserResult.isSuccess());
    JSON &json = parserResult.getRef();

    json.set(value, parser.getStringAllocator());
    assert(json.getType() == JSON::Type::STRING);
    assert(json.asString().getRef() == "test");
}

static void testCreatePrimitives() {
    const std::int64_t value1 = -25000000000LL;

    Parser parser;
    const ParserResult parserResult = parser.init();
    assert(parserResult.isSuccess());
    JSON &json = parserResult.getRef();

    json.set(value1);
    assert(json.getType() == JSON::Type::INT64);
    assert(json.asInt64().getValue() == value1);

    const std::uint64_t value2 = 25000000000ULL;
    json.set(value2);
    assert(json.getType() == JSON::Type::UINT64);
    assert(json.asUint64().getValue() == value2);

    const double value3 = 25000000000.50;
    json.set(value3);
    assert(json.getType() == JSON::Type::FLOAT64);
    assert(json.asFloat64().getValue() == value3);

    const bool value4 = true;
    json.set(value4);
    assert(json.getType() == JSON::Type::BOOL);
    assert(json.asBool().getValue() == value4);

    json.set(nullptr);
    assert(json.getType() == JSON::Type::NUL);
}

static void testCreateArray() {
    Parser parser;
    const ParserResult parserResult = parser.init();
    assert(parserResult.isSuccess());
    JSON &json = parserResult.getRef();

    Result<Array&> array1Result = json.makeArray(parser.getArrayAllocator());
    assert(array1Result.isSuccess());
    Array &array1 = array1Result.getRef();
    assert(json.getType() == JSON::Type::ARRAY);
    assert(&json.asArray().getRef() == &array1);

    const std::uint64_t value1 = 5ULL;
    const bool value2 = true;
    const std::int64_t value3 = -25000000000LL;

    Array array2(parser.getArrayAllocator());
    array2.set(0, value1);

    assert(array2.get(0U).isSuccess());
    assert(array2.get(0U).getRef().getType() == JSON::Type::UINT64);
    assert(array2.get(0).getRef().asUint64().getValue() == value1);

    array1[0] = std::move(array2);
    array1[1] = value2;
    assert(array1.set(2, value3));

    assert(array1[0].getType() == JSON::Type::ARRAY);
    assert(array1.get(1).isSuccess());
    assert(array1.get(1).getRef().getType() == JSON::Type::BOOL);
    assert(array1.get(1).getRef().asBool().getValue() == value2);
    assert(array1[1].asBool().getValue() == value2);
    assert(array1.get(2).isSuccess());
    assert(array1.get(2).getRef().getType() == JSON::Type::INT64);   
    assert(array1[2].asInt64().getValue() == value3);
}

static void testCreateObject() {
    Parser parser;
    const ParserResult parserResult = parser.init();
    assert(parserResult.isSuccess());
    JSON &json = parserResult.getRef();

    const Result<Object&> objectResult = json.makeObject(parser.getObjectAllocator());
    assert(objectResult.isSuccess());
    Object &object1 = objectResult.getRef();
    assert(json.getType() == JSON::Type::OBJECT);
    assert(json.asObject().isSuccess());
    assert(&json.asObject().getRef() == &object1);

    const std::uint64_t value1 = 5ULL;
    const bool value2 = true;
    const std::int64_t value3 = -25000000000LL;

    Object object2(parser.getObjectAllocator());
    const String key1 = parser.createString("key1"),
                 key2 = parser.createString("key2"),
                 key3 = parser.createString("key3");

    object2.set(key1, value1);
    assert(object2.get(key1).isSuccess());
    assert(object2.get(key1).getRef().getType() == JSON::Type::UINT64);
    assert(object2[key1].unsafeAsInt64() == value1);
    assert(object2.get(key1).getRef().asUint64().getValue() == value1);

    object1[key1] = std::move(object2);
    object1[key2] = value2;
    assert(object1.set(key3, value3));

    assert(object1[key1] != nullptr);
    assert(object1[key1].getType() == JSON::Type::OBJECT);
    assert(object1[key2] != nullptr);
    assert(object1[key2].getType() == JSON::Type::BOOL);
    assert(object1[key2].unsafeAsBool() == value2);
    assert(object1[key3] != nullptr);
    assert(object1[key3].getType() == JSON::Type::INT64);
    assert(object1[key3].unsafeAsInt64() == value3);
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