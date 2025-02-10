#include <iostream>
#include "../parser.hpp"
#include "../util.hpp"

int main() {
    size_t filesize;
    auto data = CJSON::file_get_contents("tests\\really-big-json-file.json", filesize);
    
    const long start = CJSON::usec_timestamp();
    auto json = CJSON::parse(data.get());
    const long end = CJSON::usec_timestamp();

    std::cout << "Execution time: " << end - start << "\n";
    
    return 0;
}
