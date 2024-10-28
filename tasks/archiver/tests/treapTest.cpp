#include "catch.hpp"
#include <cstring>
#include <iostream>
#include <sstream>
#include <bitset>
#include "../src/models/Treap.h"


TEST_CASE("treap test") {
    std::vector<std::pair<std::pair<uint16_t, uint8_t>, uint8_t>>code_to_word;
    code_to_word = {
            std::pair<std::pair<uint16_t, uint8_t>, uint8_t>{std::pair<uint16_t, uint8_t>{0b001, 3}, 'a'},
            std::pair<std::pair<uint16_t, uint8_t>, uint8_t>{std::pair<uint16_t, uint8_t>{0b101, 3}, 'b'},
            std::pair<std::pair<uint16_t, uint8_t>, uint8_t>{std::pair<uint16_t, uint8_t>{0b11, 2}, 'c'}
    };

    Treap funfun(code_to_word);
    auto mapping = funfun.GetMappingTable();
    REQUIRE(mapping['b'] == std::pair<uint16_t, uint8_t>{0b101, 3});
    REQUIRE(mapping['c'] == std::pair<uint16_t, uint8_t>{0b11, 2});
    REQUIRE(mapping['a'] == std::pair<uint16_t, uint8_t>{0b001, 3});

    funfun.AddWord(0b01010101010100, 14, 'd');
    mapping = funfun.GetMappingTable();
    REQUIRE(mapping['d'] == std::pair<uint16_t, uint8_t>{0b01010101010100, 14});
}

TEST_CASE("treap automata test") {
    std::vector<std::pair<std::pair<uint16_t, uint8_t>, uint8_t>>code_to_word;
    code_to_word = {
            std::pair<std::pair<uint16_t, uint8_t>, uint8_t>{std::pair<uint16_t, uint8_t>{0b001, 3}, 'a'},
            std::pair<std::pair<uint16_t, uint8_t>, uint8_t>{std::pair<uint16_t, uint8_t>{0b101, 3}, 'b'},
            std::pair<std::pair<uint16_t, uint8_t>, uint8_t>{std::pair<uint16_t, uint8_t>{0b11, 2}, 'c'},
            std::pair<std::pair<uint16_t, uint8_t>, uint8_t>{std::pair<uint16_t, uint8_t>{0b01010101010100, 14}, 'd'}
    };

    Treap badbad(code_to_word);
    TreapAutomata automata = badbad.buildAutomata();
    std::queue<uint8_t> buffer;
    //                          ~bbdddddddddddddd  <-
    bool found = automata.Feed(0b0101010101010100, buffer);
    REQUIRE(found == true);
    //                     ~ccb  <-
    found = automata.Feed(0b111, buffer);
    REQUIRE(found == false);

    REQUIRE(buffer.front() == 'd');
    buffer.pop();
    REQUIRE(buffer.front() == 'b');
    buffer.pop();
    REQUIRE(buffer.front() == 'c');
    buffer.pop();
}