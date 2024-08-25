#pragma once

constexpr unsigned int name_hash(const char* str) {
    unsigned int hash{ 0 };

    while (*str)
        hash = *str++ + 31 * hash;

    return hash & 0x7FFFFFFF;
}
