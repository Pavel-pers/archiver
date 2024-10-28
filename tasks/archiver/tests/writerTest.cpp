#include "catch.hpp"
#include <cstring>
#include <iostream>
#include <sstream>
#include <bitset>
#include "../src/models/StreamWriter.h"
#include "../src/utility.h"
#include "../src/models/exceptions/FileExceptions.h"


TEST_CASE("Sstream Simple") {
    std::stringstream out1;
    StreamWriter sw1(out1);
    sw1.Write(utility_bit::ReverseChar(static_cast<uint8_t>('a')), 8);
    sw1.Write(utility_bit::ReverseChar(static_cast<uint8_t>('b')), 8);
    sw1.Write(utility_bit::ReverseChar(static_cast<uint8_t>('m')), 8);
    sw1.Close();

    std::stringstream out2;
    StreamWriter sw2(out2);

    sw2.Write(static_cast<uint8_t>(utility_bit::ReverseChar('a')) |
                      (static_cast<uint16_t>(static_cast<uint8_t>(utility_bit::ReverseChar('b'))) << 8), 16);
    sw2.Write(static_cast<uint8_t>(utility_bit::ReverseChar('c')) |
                      (static_cast<uint16_t>(static_cast<uint8_t>(utility_bit::ReverseChar('d'))) << 8), 16);
    sw2.Close();

    REQUIRE(out2.str() == "abcd");
}

TEST_CASE("Sstream big") {
    std::stringstream out1;
    StreamWriter sw1(out1);

    for (size_t i = 0; i < 3e3; i++) {
        sw1.Write(0b0000001010101010, 10);
    }

    for (size_t i = 0; i < out1.str().size(); i++) {
        REQUIRE(out1.str()[i] == static_cast<char>(0b01010101));
    }
}


