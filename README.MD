# CPPJSON

**JSON parser in C++.**

## Table of contents
- [Table of contents](#table-of-contents)
- [Requirements](#requirements)
- [Compilation](#compilation)
- [Usage](#usage)
- [Parse From a File](#parse-from-a-file)
- [Parse From a String](#parse-from-a-string)
- [Init empty JSON](#init-empty-json)
- [JSON](#json)
    - [Check Type](#check-the-json-type)
    - [Query Specific Value](#query-a-specific-value)
    - [Query Specific Value of a Specific Type](#query-a-specific-value-of-a-specific-type)
    - [Set A Value](#set-a-value)
    - [Get Parsing Error as a c-string](#get-parsing-error-as-a-c-string)
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

    //method 1:
    const ParserResult parserResult = parser.parseFile("path/to/file");
    if(parserResult.isSuccess()) {
        JSON &json = parserResult.getRef();
    }

    //method 2:
    parser.parseFile("path/to/file")
        .onSuccess([](JSON &json) {
            //success callback
        })
        .onError([](const Error error) {
            //error callback with the CPPJSON::Error
        })
        .onError([]() {
            //error callback without parameter
        })

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

    //method 1:
    const ParserResult parserResult = parser.parse(jsonExample);
    if(parserResult.isSuccess()) {
        const JSON &json = parserResult.getRef();
    }

    //method 2:
    parser.parse(jsonExample)
        .onSuccess([](JSON &json) {
            //success callback
        })
        .onError([](const Error error) {
            //error callback with the CPPJSON::Error
        })
        .onError([]() {
            //error callback without parameter
        })
    //...

    return EXIT_SUCCESS;
}
```

### Init empty JSON.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    Parser parser;

    //method 1:
    const ParserResult parserResult = parser.init();
    if(parserResult.isSuccess()) {
        const JSON &json = parserResult.getRef();
    }

    //method 2:
    parser.parse(jsonExample)
        .onSuccess([](JSON &json) {
            //success callback
        })
        .onError([](const Error error) {
            //error callback with the CPPJSON::Error
        })
        .onError([]() {
            //error callback without parameter
        })
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
        const Result<Object&> objectResult = json.asObject();
        break;
    case JSON::Type::ARRAY:
        const Result<Array&> objectResult = json.asArray();
        break;
    case JSON::Type::STRING:
        const Result<String&> objectResult = json.asString();
        break;
    case JSON::Type::FLOAT64:
        const Result<double> float64Result = json.asFloat64();
        break;
    case JSON::Type::INT64:
        const Result<std::int64_t> int64Result = json.asInt64();
        break;
    case JSON::Type::UINT64:
        const Result<std::uint64_t> uint64Result = json.asUint64();
        break;
    case JSON::Type::BOOL:
        const Result<bool> boolResult = json.asBool();
        break;
    case JSON::Type::NUL:
        const Result<std::nullptr_t> nullResult = json.asNull();
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

    //method 1:
    const Result<JSON&> jsonResult = json["key1"]["innerKey1"].get();
    if(jsonResult.isSuccess()) {
        //if jsonResult is a success then the JSON object has this form: {"key1": {"innerKey": myValues}}
        const JSON &value = jsonResult.getRef();
        //...
    }

    //method 2:
    json["key1"]["innerKey1"].get()
        .onSuccess([](JSON &json) {
            //success callback
        })
        .onError([]() {
            //error callback
        });

    //method 3:
    const JSON &value = json["key1"]["innerKey1"].unsafeGet();

    return EXIT_SUCCESS;
}
```

### Query a Specific Value of a Specific Type.

Available functions

- asArray         => Result<Array&>
- asObject        => Result<Object&>
- asString        => Result<String&>
- asFloat64       => Result<double>
- asInt64         => Result<std::int64_t>
- asUint64        => Result<std::uint64_t>
- asBool          => Result<bool>
- asNull          => Result<std::nullptr_t>
- unsafeAsArray   => Array&
- unsafeAsObject  => Object&
- unsafeAsString  => String&
- unsafeAsFloat64 => double
- unsafeAsInt64   => std::int64_t
- unsafeAsUint64  => std::uint64_t
- unsafeAsBool    => bool
- unsafeAsNull    => std::nullptr_t

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    //method 1:
    const Result<String&> stringResult = json["key1"]["innerKey1"].asString();
    if(stringResult.isSuccess()) {
        const String &value = stringResult.getRef();
        //if stringResult is a success then the JSON object has this form: {"key1": {"innerKey1": value}} and value is a string
        
        //...
    }
    const Result<std::uint64_t> uint64Result = json["key1"]["innerKey2"].asUint64();
    if(uint64Result.isSuccess()) {
        const std::uint64_t value = stringResult.getValue();
        //if uint64Result is a success then the JSON object has this form: {"key1": {"innerKey2": value}} and value is a std::uint64_t
    }

    //method 2:
    json["key1"]["innerKey1"].asString()
        .onSuccess([](String &value) {
            //success callback
        })
        .onError([]() {
            //error callback
        });

    json["key1"]["innerKey2"].asUint64()
        .onSuccess([](const std::uint64_t value) {
            //success callback
        })
        .onError([]() {
            //error callback
        });

    //method 3:
    const String       &value1 = json["key1"]["innerKey1"].unsafeAsString();
    const std::uint64_t value2 = json["key1"]["innerKey1"].unsafeAsUint64();


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

    //becomes std::int64_t (signed => std::int64_t)
    json.set(100); // first way
    json = 100;    // second way

    //becomes std::uint64_t (unsigned => std::uint64_t)
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

### Get Parsing Error as a c-string.

use the getErrorString function to get the error as a statically allocated c-string.

```cpp
#include <cppjson.hpp>
#include <cstdlib>
#include <iostream>

using namespace CPPJSON;

int main() {
    Parser parser;
    const ParserResult parserResult = parser.parseFile("/some/path/to/file.json");
    if(!parserResult.isSuccess()) {
        std::cerr << getErrorString(parserResult::getError()) << '\n';
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

    Object &object = ...;

    //...

    //method 1:
    const Result<JSON&> jsonResult = object.get("some-key");
    if(jsonResult.isSuccess()) {
        //if objectResult is a success then the Object contains a key "some-key"
        const JSON &json = jsonResult.getRef();
    }

    //method 2:
    object.get("some-key")
        .onSuccess([](JSON &json) {
            //success callback
        })
        .onError([]() {
            //error callback
        });

    //method 3:
    const JSON &json = object.unsafeGet("some-key");

    //method 4:
    //CPPJSON::JSON store strings as CPPJSON::String, which require an allocator provided by the parser. You can access an object with const char* and std::string key in read-only mode. If the key doesn't exist, a reference to JSON::INVALID_JSON is returned instead.
    const JSON &json = object["some-other-key"];
    if(json.isValid()) {
        //key "some-other-key" exists in the Object and json is a reference to the value
    }

    JSON &json2 = object[parser.createString("some-other-string")];
    //returns a reference to the JSON object, if the key doesn't exist, a new entry is created in the Object with that key and a reference to the new value is returned. This operator can throw an exception if memory allocation failed.

    return EXIT_SUCCESS;
}
```

### Get a Value of a Specific Type With a Key.

Available functions

- getString        => Result<String&>
- getFloat64       => Result<double>
- getInt64         => Result<std::int64_t>
- getUint64        => Result<std::uint64_t>
- getObject        => Result<Object&>
- getArray         => Result<Array*&>
- getBool          => Result<bool>
- getNull          => Result<std::nullptr_t>
- UnsafeGetString  => String&
- UnsafeGetFloat64 => double
- UnsafeGetInt64   => std::int64_t
- UnsafeGetUint64  => std::uint64_t
- UnsafeGetObject  => Object&
- UnsafeGetArray   => Array&
- UnsafeGetBool    => bool
- UnsafeGetNull    => std::nullptr_t

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    Object &object = ...;

    //...

    //method 1:
    const Result<Array&> arrayResult = object.getArray("some-key");
    if(arrayResult.isSuccess()) {
        const Array& array = array.getRef();
        //if arrayResult is a success then the Object contains a key "some-key" and the type of the underlying JSON object is JSON::TYPE::ARRAY.
    }
    const Result<std::int64_t> int64Result = object.getInt64("some-key-2");
    if(int64Result.isSuccess()) {
        const std::int64_t value = int64Result.getValue();
        //if int64Result is a success then the Object contains a key "some-key-2" and the type of the underlying JSON object is JSON::TYPE::INT64.
    }


    //method 2:
    object.getArray("some-key")
        .onSuccess([](Array &value) {
            //success callback
        })
        .onError([]() {
            //error callback
        });
    object.getInt64("some-key-2")
        .onSuccess([](const std::int64_t value) {
            //success callback
        })
        .onError([]() {
            //error callback
        });

    //method 3:
    const Array       &value1 = object.unsafeGetArray("some-key");
    const std::int64_t value2 = object.unsafeGetInt64("some-key-2");

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

    Object &object = ...;

    //...

    object.remove("some-key");

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

    JSON   &someValue = ...;
    Object &object = ...;
    String someKey = parser.createString("someKey");

    //method 1:
    //key is copied, can throw
    object[someKey]            = std::move(someValue);
    object[someKey]            = 10U;
    object[someKey]            = true;
     //key is moved
    object[std::move(someKey)] = std::move(someValue);
    object[std::move(someKey)] = 10U;
    object[std::move(someKey)] = true;

    //method 2:
    //key is copied
    if(object.set(someKey, std::move(someValue))) {
        //no allocation or successful allocation
    }
    if(object.set(someKey, 10U)) {
        //no allocation or successful allocation
    }
    if(object.set(someKey, true)) {
        //no allocation or successful allocation
    }
    //key is moved
    object.set(std::move(someKey), std::move(someValue));
    object.set(std::move(someKey), 10U);
    object.set(std::move(someKey), true);


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

    Array &array;

    //...

    //method 1:
    const Result<JSON&> jsonResult = array.get(10U);
    if(jsonResult.isSuccess()) {
        //if jsonResult is a success then the Array has a value at index 10.
        const JSON &json = jsonResult.getRef();
    }

    //method 2:
    array.get(10U)
        .onSuccess([](JSON &json) {
            //success callback
        })
        .onError([]() {
            //error callback
        });

    //method 3:
    const JSON &json = array.unsafeGet(10U);

    //method 4:
    //if the Array is const then the bracket operator will return a const JSON reference if the index exists, otherwise a reference to JSON::INVALID_JSON is returned
    const Array &array = ...;
    const JSON &json = array[10U];
    if(json.isValid()) {
        //... Array index 10 exists
    }

    //if the Array is not const, the bracket operator will populate all the indices between the last one and the index provided with JSON objects with the type JSON::Type::NUL then return a reference to that last one. This operator can throw an exception if memory allocation fails
    Array &array = ...;
    const JSON &json = array[10U];

    return EXIT_SUCCESS;
}
```

### Get a Value of a Specific Type With an Index.

Available functions (all come in 2 versions: with no parameter and with a reference to a success bool)

- getString        => Result<String&>
- getFloat64       => Result<double>
- getInt64         => Result<std::int64_t>
- getUint64        => Result<std::uint64_t>
- getObject        => Result<Object&>
- getArray         => Result<Array&>
- getBool          => Result<bool>
- getNull          => Result<std::nullptr_t>
- unsafeGetString  => String&
- unsafeGetFloat64 => double
- unsafeGetInt64   => std::int64_t
- unsafeGetUint64  => std::uint64_t
- unsafeGetObject  => Object&
- unsafeGetArray   => Array&
- unsafeGetBool    => bool
- unsafeGetNull    => std::nullptr_t

```cpp
#include <cppjson.hpp>
#include <cstdlib>
using namespace CPPJSON;

int main() {
    //...

    Array &array;

    //...

    //method 1:
    const Result<String&> stringResult = array.getString(10U);
    if(stringResult.isSuccess()) {
        const String &value = stringResult.getRef();
        //if stringResult is a success then the Array contains the index 10 and the type of the underlying JSON object is JSON::TYPE::STRING.
    }
    const Result<bool> boolResult = array.getBool(11U);
    if(boolResult.isSuccess()) {
        const bool value = boolResult.getValue();
        //if boolResult is a success then the Array contains the index 11 and the type of the underlying JSON object is JSON::TYPE::BOOL.
    }

    //method 2:
    array.getString(10U)
        .onSuccess([](String &value) {
            //success callback
        })
        .onError([]() {
            //error callback
        });
    array.getBool(11U)
        .onSuccess([](const bool value) {
            //success callback
        })
        .onError([]() {
            //error callback
        });

    //method 3:
    const String &value1 = array.unsafeGetString(10U);
    const bool    value2 = array.unsafeGetBool(11U);

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

    Array &array;

    //...

    //first way
    for(unsigned i = 0U; i < array.size(); i++) {
        JSON &value = array.unsafeGet(i);
        
        //...
    }

    //second way
    for(JSON &value : array) {
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

    JSON  &someValue = ...;
    Array &array = ...;
    
    //the bracket operator will populate all the indices between the last one and the index provided with JSON objects with the type JSON::Type::NUL then return a reference to that last one. Can throw if memory allocation fails.
    array[10U] = std::move(someValue);
    array[10U] = 10U;
    array[10U] = false;

    //will populate all the indices between the last one and the index provided with JSON objects with the type JSON::Type::NUL, return false if the allocation fails
    if(array.set(10U, std::move(someValue))) {
        //no allocation or successful allocation
    }
    if(array.set(10U, 10U)) {
        //no allocation or successful allocation
    }
    if(array.set(10U, false)) {
        //no allocation or successful allocation
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

    Array &array = ...;

    //...

    JSON jsonNumber = 1024U;
    if(array.push(jsonNumber)) {
        //successful allocation
    }

    return EXIT_SUCCESS;
}
```