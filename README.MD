# CPPJSON

**JSON parser in C++.**

## Table of contents
- [Table of contents](#table-of-contents)
- [Requirements](#requirements)
- [Compilation](#compilation)
- [Usage](#usage)
- [Parse From a File](#parse-from-a-file)
- [Parse From a String](#parse-from-a-string)
- [JSON](#json)
    - [Check Type](#check-the-json-type)
    - [Query Specific Value](#query-a-specific-value)
    - [Query Specific Value of a Specific Type](#query-a-specific-value-of-a-specific-type)
    - [Set A Value](#set-a-value)
    - [Get Parsing Error as a string](#get-parsing-error-as-a-string)
- [Object](#object)
    - [Get Value with a Key](#get-a-value-with-a-key)
    - [Get Value of a Specific Type with a Key](#get-a-value-of-a-specific-type-with-a-key)
    - [Delete a key](#delete-a-key)
    - [Set Value to a Key](#set-a-value-to-a-key)
- [Array](#array)
    - [Get Value With an Index](#get-a-value-with-an-index)
    - [Get Value of a Specific Type With an Index](#get-a-value-with-an-index)
    - [Loop](#loop)
    - [Set Value at an Index](#set-value-at-index)
    - [Push Value](#push-value)

## Requirements
- Makefile
- MinGW (for Windows)

## Compilation
Static library:

```bash
make static
```

This will create a release build of the static library as cppjson.a, link your program to this static library when compiling.

## Usage.

### Parse From a File.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    Parser parser;
    JSON &json = parser.parseFile("path/to/file");
    if(json.getError() != JSON::Error::NONE) {
        return EXIT_FAILURE;
    }

    //...

    return EXIT_SUCCESS;
}
```

### Parse From a String.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
#include <string>
using namespace CPPJSON;

int main() {
    const std::string jsonExample = "{"
        "\"key1\": {\"innerKey\": \"innerValue\"},"
        "\"key2\": \"value\""
    "}";

    Parser parser;
    JSON &json = parser.parse(jsonExample);
    if(json.getError() != JSON::Error::NONE) {
        return EXIT_FAILURE;
    }

    //...

    return EXIT_SUCCESS;
}
```

### JSON.

### Check The Json Type.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    switch(json.getType()) {
    case JSON::Type::OBJECT:
        Object *const object = json.asObject();
        break;
    case JSON::Type::ARRAY:
        Array *const array = json.asArray();
        break;
    case JSON::Type::STRING:
        String *const string = json.asString();
        break;
    case JSON::Type::FLOAT64:
        const double float64 = json.asFloat64();
        break;
    case JSON::Type::INT64:
        const std::int64_t int64 = json.asInt64();
        break;
    case JSON::Type::UINT64:
        const std::uint64_t uint64 = json.asUint64();
        break;
    case JSON::Type::BOOL:
        const bool boolean = json.asBool();
        break;
    case JSON::Type::NUL:
        const std::nullptr_t null = json.asNull();
        break;
    case JSON::Type::ERROR:
        const JSON::Error error = json.getError();
        break;
    }

    //...

    return EXIT_SUCCESS;
}
```

### Query a Specific Value.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    JSON *const myValues = json["key1"]["innerKey1"].get();
    if(myValues != nullptr) {
        //if myValues is not nullptr then the JSON object has this form: {"key1": {"innerKey": myValues}}
        
        //...
    }

    return EXIT_SUCCESS;
}
```

### Query a Specific Value of a Specific Type.

Available functions (all come in 2 versions: with no parameter and with a reference to a success bool):

- asArray   => Array*
- asObject  => Object*
- asString  => const char*
- asFloat64 => double
- asInt64   => std::int64_t
- asUint64  => std::uint64_t
- asBool    => bool
- asNull    => std::nullptr_t

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    String *const myString = json["key1"]["innerKey1"].asString();
    if(myString != nullptr) {
        //if myString is not nullptr then the JSON object has this form: {"key1": {"innerKey1": myString}} and myString is a string
        
        //...
    }

    bool success;
    uint64_t myUint64 = json["key1"]["innerKey2"].asUint64(success);
    if(!success) {
        //if success is true then the JSON object has this form: {"key1": {"innerKey2": myUint64}} and myUint64 is stored as an unsigned 64 bit integer
    }

    //Note: if the value is stored as int64_t (the parser will only use int64_t with negative numbers) and you request an uint64_t, it will fail.

    return EXIT_SUCCESS;
}
```

### Set a Value.


```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    json.set(Object(parser.getObjectAllocator())); //first way
    json = Object(parser.getObjectAllocator());    //second way
    json.makeObject(parser.getObjectAllocator());  //third way

    json.set(Array(parser.getArrayAllocator())); //first way
    json = Array(parser.getArrayAllocator());    //second way
    json.makeArray(parser.getArrayAllocator());  //third way

    json.set(String(parser.getStringAllocator())); //first way
    json = String(parser.getStringAllocator());    //second way
    json.makeString(parser.getStringAllocator());  //third way

    //become string
        //with cstring
    json.set("some string", parser.getStringAllocator());
    
        //with std::string
    std::string stdstring = "some string";
    json.set(stdstring, parser.getStringAllocator()); 

        //with CPPJSON::String
    String someString(parser.getStringAllocator()); 
    someString = "some string";

    json.set(std::move(someString)); //first way, moving
    json = std::move(someString);    //second way, moving
    
    json.set(someString)             //first way, copying
    json = someString                //second way, copying

    //becomes double
    json.set(10.0);  //first way
    json = 10.0;     //second way
    json.set(-5.0F); //first way
    json = -5.0F;    //second way

    //becomes int64_t (signed => int64_t)
    json.set(100); // first way
    json = 100;    // second way

    //becomes uint64_t (unsigned => uint64_t)
    json.set(200U); // first way
    json = 200U;    // second way

    //becomes nullptr
    json.set();        //first way (default argument)
    json.set(nullptr); //first way
    json = nullptr;    //second way

    //becomes bool
    json.set(true); //first way
    json = true;    //second way

    //...

    return EXIT_SUCCESS;
}
```

### Get Parsing Error as a String.

use the Parser::getError function to get the last error as a statically allocated c-string.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
#include <iostream>

using namespace CPPJSON;

int main() {
    Parser parser;
    JSON &json = parser.parseFile("/some/path/to/file.json");
    if(json.getError() != JSON::Error::NONE) {
        std::cerr << parser.getError() << '\n';
        return EXIT_FAILURE;
    }

    //...

    return EXIT_SUCCESS;
}
```
### Object.

### Get a Value With a Key.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    Object *object;

    //...

    JSON *const someValue = object->get("some-key");
    if(someValue != nullptr) {
        //if someValue is not nullptr then the Object contains a key "some-key"
    }

    JSON *const someOtherValue = object["some-other-key"];
    if(someOtherValue != nullptr) {
        //if someOtherValue is not nullptr then the Object contains a key "some-other-key" or the Object does not contain a key "some-other-key" and successfully allocated memory for a new entry with "some-other-key" as the key.
    }

    return EXIT_SUCCESS;
}
```

### Get a Value of a Specific Type With a Key.

Available functions (all come in 2 versions: with no parameter and with a reference to a success bool)

- getString  => const char*
- getFloat64 => double
- getInt64   => std::int64_t
- getUint64  => std::uint64_t
- getObject  => Object*
- getArray   => struct CJSON_Array*
- getBool    => bool
- getNull    => void*

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    Object *object;

    //...

    bool success;
    Array *const someArray =  object->getArray("some-key", success);
    if(success) {
        //if sucess is true then the Object contains a key "some-key" and the type of the underlying JSON object value is JSON::ARRAY. someArray is not nullptr
    } else {
        //someArray is nullptr
        //numerical types will be equal to 0, bool will be equal to false, null is always nullptr
    }

    return EXIT_SUCCESS;
}
```

### Delete a Key.
```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    Object *object;

    //...

    object->remove("some-key");

    return EXIT_SUCCESS;
}
```

### Set a Value to a Key.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    JSON   *someValue;
    Object *object;

    //...

    //first way
    JSON *json = *object["someKey"];
    if(json == nullptr) {
        //key didn't exist and memory allocation failed
        return EXIT_FAILURE;
    }
    *json = *someValue;

    //second way
    if(object->set("someKey", std::move(*someValue))) {
        //success
    } else {
        //key didn't exist and memory allocation failed
        return EXIT_FAILURE;
    }

    //...

    Array *someArray;

    //first way
    json = *object["someOtherKey"];
    if(json == nullptr) {
        //key didn't exist and memory allocation failed
        return EXIT_FAILURE;
    }
    *json = *someArray;

    //second way
    if(object->set("someKey", std::move(*someArray))) {
        //success
    } else {
        //key didn't exist and memory allocation failed
        return EXIT_FAILURE;
    }

    //...

    //first way
    json = *object["somePrimitiveValue"];
    if(json == nullptr) {
        //key didn't exist and memory allocation failed
        return EXIT_FAILURE;
    }
    *json = 10;

    //second way
    if(object->set("somePrimitiveValue", 10)) {
        //success
    } else {
        //key didn't exist and memory allocation failed
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

### Array.

### Get a Value With an Index.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    Array *array;

    //...

    JSON *const someValue = array->get(10U);
    if(someValue != nullptr) {
        //if someValue is not nullptr then the Array has a value at index 10.
    }

    return EXIT_SUCCESS;
}
```

### Get a Value of a Specific Type With an Index.

Available functions (all come in 2 versions: with no parameter and with a reference to a success bool)

- getString  => const char*
- getFloat64 => double
- getInt64   => std::int64_t
- getUint64  => std::uint64_t
- getObject  => Object*
- getArray   => struct CJSON_Array*
- getBool    => bool
- getNull    => std::nullptr_t

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    Array *array;

    //...

    bool success;
    Array *const someArray = array->getArray(10U, &success);
    if(success) {
        //if sucess is true then the Array contains a value at index 10 and the type of the underlying JSON object value is JSON::Array. someArray is not nullptr
    } else {
        //someArray is nullptr
        //numerical types will be equal to 0, bool will be equal to false, null is always nullptr
    }

    return EXIT_SUCCESS;
}
```

### Loop.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    Array *array;

    //...

    //first way
    for(unsigned i = 0U; i < array->size(); i++) {
        JSON *const value = array[i];
        
        //...
    }

    //second way
    for(JSON &value : *array) {
        //...
    }

    return EXIT_SUCCESS;
}
```

### Set Value at Index.

If the index is greater than the length of the array, all JSON object values between last element and the index will be zeroed (JSON::Type == JSON::Type::NUL)

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    JSON  *someValue;
    Array *array;

    //...

    //first way
    JSON *json = *array[10U];
    if(json == nullptr) {
        //index dit not exist and memory allocation failed
        return EXIT_FAILURE;
    }
    *json = *someValue;

    //second way
    if(array->set(10U, std::move(*someValue))) {
        //success
    } else {
        //index did not exist and memory allocation failed
        return EXIT_FAILURE;
    }

        //...

    Array *someArray;

    //first way
    json = *array[11U];
    if(json == nullptr) {
        //index didn't exist and memory allocation failed
        return EXIT_FAILURE;
    }
    *json = *someArray;

    //second way
    if(array->set(11U, std::move(*someArray))) {
        //success
    } else {
        //key didn't exist and memory allocation failed
        return EXIT_FAILURE;
    }

    //...

    //first way
    json = *array[12U];
    if(json == nullptr) {
        //key didn't exist and memory allocation failed
        return EXIT_FAILURE;
    }
    *json = 10;

    //second way
    if(array->set(12U, 10)) {
        //success
    } else {
        //key didn't exist and memory allocation failed
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

### Push Value.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    Array *array;

    //...

    JSON number = 1024U;

    if(array->push(number)) {
        //success
    } else {
        //memory reallocation failed
    }

    return EXIT_SUCCESS;
}
```