#include "catch.hpp"
#include <cstring>
#include <iostream>
#include <sstream>
#include "../src/models/StreamReader.h"
#include "../src/utility.h"
#include "../src/models/exceptions/FileExceptions.h"


TEST_CASE("Sstream Simple") {
    std::stringstream inp1;
    inp1 << "0123";
    StreamReader sr1(inp1);
    REQUIRE(char(sr1.ReadByte(8)) == utility_bit::ReverseChar('0'));
    REQUIRE(char(sr1.ReadByte(8)) == utility_bit::ReverseChar('1'));
    REQUIRE(sr1.Read2Bytes(16) == 0b1100110001001100);
}

TEST_CASE("Sstream Small") {
    std::stringstream inp1;
    inp1 << "0123";
    StreamReader sr1(inp1);
    REQUIRE(char(sr1.ReadByte(1)) == 0);
    REQUIRE(char(sr1.ReadByte(1)) == 0);
    REQUIRE(char(sr1.ReadByte(1)) == 1);
    REQUIRE(char(sr1.ReadByte(2)) == 1);
    REQUIRE(sr1.Read2Bytes(11) == (1 << 5) + (1 << 6) + (1 << 10));
    REQUIRE(char(sr1.ReadByte(7)) == (1 << 2) + (1 << 3) + (1 << 6));
}

TEST_CASE("Sstream big") {
    const size_t test_size = 1e3;
    std::stringstream inp1;
    std::string str_inp;
    for (size_t i = 0; i < test_size; i++) {
        str_inp += static_cast<char>(-i);
    }
    inp1 << str_inp;

    bool origin_bin_str[static_cast<size_t>(test_size * 8)];
    for (size_t bi = 0; bi < test_size * 8; bi++) {
        origin_bin_str[bi] = (str_inp[bi / 8] >> (7 - (bi % 8))) & 1;
    }

    StreamReader sr1(inp1);
    bool res_bin_str[static_cast<size_t>(test_size * 8) + 1000];
    size_t res_i = 0;

    while (!sr1.Eof()) {
        uint16_t chank = sr1.Read2Bytes(11);
        for (size_t i = 11; i--;) {
            res_bin_str[res_i++] = chank & 1;
            chank /= 2;
        }
    }

    for (size_t i = 0; i < test_size * 8; i++) {
        REQUIRE(res_bin_str[i] == origin_bin_str[i]);
    }
}


