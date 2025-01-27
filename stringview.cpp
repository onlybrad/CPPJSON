#include <cstring>
#include <climits>
#include <algorithm>
#include <cassert>
#include "stringview.hpp"

namespace CJSON {

StringView::StringView() : StringView(nullptr) {}

StringView::StringView(const char *const d) : data(d) {
    if(d == nullptr) {
        size = 0U;
    } else {
        size = (unsigned int)std::min(std::strlen(d), (size_t)UINT_MAX);
    }
}

StringView::StringView(const char *const d, const unsigned int s) : data(d), size(s){}

char StringView::operator[](unsigned int index) const {
    assert(index < size);
    return data[index];
}

char StringView::operator*() const {
    return *data;
}

StringView::operator const char *() {
    return data;
}

const char *StringView::operator+(const unsigned int offset) const {
    assert(offset < size);
    return data + offset;
}

}
