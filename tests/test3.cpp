#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../cppjson.hpp"

using namespace CPPJSON;

static Parser parser;

static void testToString() {
    const std::string array = "["
        "{\"key1\": \"value1\"},"
        "{\"key2\": {\"key3\": [true, {\"key4\": false}, null]}},"
        "{\"key5\": {}},"
        "{\"key6\": []},"
        "{\"key7\": \"\"},"
        "{\"key8\": 1e5}"
    "]";

    const std::string expected_indentation0 = "[{\"key1\":\"value1\"},{\"key2\":{\"key3\":[true,{\"key4\":false},null]}},{\"key5\":{}},{\"key6\":[]},{\"key7\":\"\"},{\"key8\":100000}]";
    const std::string expected_indentation2 = "[\n  {\n    \"key1\": \"value1\"\n  },\n  {\n    \"key2\": {\n      \"key3\": [\n        true,\n        {\n          \"key4\": false\n        },\n        null\n      ]\n    }\n  },\n  {\n    \"key5\": {}\n  },\n  {\n    \"key6\": []\n  },\n  {\n    \"key7\": \"\"\n  },\n  {\n    \"key8\": 100000\n  }\n]";
    const std::string expected_indentation4 = "[\n    {\n        \"key1\": \"value1\"\n    },\n    {\n        \"key2\": {\n            \"key3\": [\n                true,\n                {\n                    \"key4\": false\n                },\n                null\n            ]\n        }\n    },\n    {\n        \"key5\": {}\n    },\n    {\n        \"key6\": []\n    },\n    {\n        \"key7\": \"\"\n    },\n    {\n        \"key8\": 100000\n    }\n]";

    ParserResult parserResult = parser.parse(array);

    parserResult.onSuccess([&](JSON &json) {
        std::string string = json.toString();
        assert(string == expected_indentation0);
        string = json.toString(2U);
        assert(string == expected_indentation2);
        string = json.toString(4U);
        assert(string == expected_indentation4);
        assert(json.toFile("tests/test3-0-identation.json"));
        remove("tests/test3-0-identation.json");
        assert(json.toFile("tests/test3-2-identation.json", 2U));
        remove("tests/test3-2-identation.json");
        assert(json.toFile("tests/test3-4-identation.json", 4U));
        remove("tests/test3-4-identation.json");
    }).onError([](){
        assert(false);
    });

    JSON::format(array).onSuccess([&](const std::string &formatted) {
        assert(formatted == expected_indentation0);
    }).onError([]() {
        assert(false);
    });

    JSON::format(array, 2U).onSuccess([&](const std::string &formatted) {
        assert(formatted == expected_indentation2);
    }).onError([]() {
        assert(false);
    });

    JSON::format(array, 4U).onSuccess([&](const std::string &formatted) {
        assert(formatted == expected_indentation4);
    }).onError([]() {
        assert(false);
    });


}

int main() {
    testToString();

    std::cout << "All tests successful\n";

    return EXIT_SUCCESS;
}

