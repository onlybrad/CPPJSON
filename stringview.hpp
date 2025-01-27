#pragma once

namespace CJSON {

struct StringView {
    const char *data = nullptr;
    unsigned int size = 0;

    StringView();
    StringView(const char *const d);
    StringView(const char *const d, const unsigned int s);

    char operator[](unsigned int index) const;
    char operator*() const;
    const char *operator+(const unsigned int offset) const;
    operator const char *();
};

}
