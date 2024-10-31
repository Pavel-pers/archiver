#include "catch.hpp"
#include <cstring>
#include <iostream>
#include <sstream>
#include <bitset>
#include "../src/models/Treap.h"

using namespace utility_types;

TEST_CASE("treap test") {
    std::vector<std::pair<utility_types::VariableLenghCode, utility_types::PaddedByte>>code_to_word;
    code_to_word = {
            std::pair<VariableLenghCode, PaddedByte>{VariableLenghCode{0b001, 3}, 'a'},
            std::pair<VariableLenghCode, PaddedByte>{VariableLenghCode{0b101, 3}, 'b'},
            std::pair<VariableLenghCode, PaddedByte>{VariableLenghCode{0b11, 2}, 'c'}
    };

    Treap funfun(code_to_word);
    auto mapping = funfun.GetMappingTable();
    REQUIRE(mapping['b'] == VariableLenghCode{0b101, 3});
    REQUIRE(mapping['c'] == VariableLenghCode{0b11, 2});
    REQUIRE(mapping['a'] == VariableLenghCode{0b001, 3});

    funfun.AddWord(VariableLenghCode{0b01010101010100, 14}, 'd');
    mapping = funfun.GetMappingTable();
    REQUIRE(mapping['d'] == VariableLenghCode{0b01010101010100, 14});
}

TEST_CASE("treap automata test") {
    std::vector<std::pair<utility_types::VariableLenghCode, utility_types::PaddedByte>>code_to_word;
    code_to_word = {
            std::pair<VariableLenghCode, PaddedByte>{VariableLenghCode{0b001, 3}, 'a'},
            std::pair<VariableLenghCode, PaddedByte>{VariableLenghCode{0b101, 3}, 'b'},
            std::pair<VariableLenghCode, PaddedByte>{VariableLenghCode{0b11, 2}, 'c'},
            std::pair<VariableLenghCode, PaddedByte>{VariableLenghCode{0b01010101010100, 14}, 'd'}
    };

    Treap badbad(code_to_word);
    TreapAutomata automata = badbad.BuildAutomata();
    std::queue<PaddedByte> buffer;
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