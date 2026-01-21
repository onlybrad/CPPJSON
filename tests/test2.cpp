#include <iostream>
#include <cstdlib>
#include "../cppjson.hpp"
#include "../util.hpp"
#include "../file.hpp"
#include "../memory.hpp"

using namespace CPPJSON;

int main() {
{
    const FileContents fileContents = FileContents::get("tests\\really-big-json-file.json");
    if(fileContents.getError() != FileContents::Error::NONE) {
        return EXIT_FAILURE;
    }
    
    const std::uint64_t start = Util::usecTimestamp();

    Parser parser;
    ParserResult parserResult = parser.parse(reinterpret_cast<const char*>(fileContents.getData()), fileContents.getLength());
    if(!parserResult.isSuccess()) {
        std::cerr << "Failed to parse the file" << '\n';
        return EXIT_FAILURE;
    }

    const std::uint64_t end = Util::usecTimestamp();
    std::cout << "Execution time: " << end - start << '\n';
}

#ifndef NDEBUG
    const AllocationStats &allocationStats = AllocationStats::get();

    std::cout << "times allocated on the heap    : " << allocationStats.allocated << '\n';
    std::cout << "times deallocated from the heap: " << allocationStats.deallocated << '\n';
#endif
    
    return 0;
}
