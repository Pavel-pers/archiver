#include "catch.hpp"
#include <iostream>
#include "../src/huffman.h"

TEST_CASE("Build treap") {
    utility_types::ByteFrequancy freq1;
    freq1.fill(0);
    freq1[5] = 1;
    freq1[6] = 2;
    freq1[7] = 3;
    Treap tr1 = huffman::BuildTreapByFrequancy(freq1);
    auto mt1 = tr1.GetMappingTable();
    REQUIRE(mt1[5].lengh == 3);
    REQUIRE(mt1[6].lengh == 2);
    REQUIRE(mt1[7].lengh == 1);

    utility_types::ByteFrequancy freq2;
    for (size_t i = 0; i < freq2.size(); i++) {
        freq2[i] = i;
    }
    Treap tr2 = huffman::BuildTreapByFrequancy(freq2);
    auto mt2 = tr2.GetMappingTable();
    for (size_t i = 0; i + 1 < freq2.size(); i++) {
        REQUIRE(mt2[i].lengh >= mt2[i + 1].lengh);
    }
}

TEST_CASE("Utility, RefactoringTest, Recover test") {
    utility_types::ByteMappingTable sample_bmt;
    sample_bmt.fill(utility_types::VariableLenghCode(0, 0));
    sample_bmt['A'] = utility_types::VariableLenghCode(0b11, 2);
    sample_bmt['B'] = utility_types::VariableLenghCode(0b0, 1);
    sample_bmt['C'] = utility_types::VariableLenghCode(0b101, 3);
    sample_bmt['D'] = utility_types::VariableLenghCode(0b001, 3);
    auto order = huffman::GetIndexOrderByCanonical(sample_bmt);
    std::vector<utility_types::PaddedByte> req_order = {'B', 'A', 'D', 'C'};
    REQUIRE(order == req_order);

    utility_types::MappingTableInfo info = utility_bit::GetMappingTableInfo(sample_bmt);
    REQUIRE(info.symbols_count == 4);
    REQUIRE(info.lengh_count[1] == 1);
    REQUIRE(info.lengh_count[2] == 1);
    REQUIRE(info.lengh_count[3] == 2);

    huffman::RefactorToCanonical(sample_bmt);
    REQUIRE(sample_bmt['B'] == utility_types::VariableLenghCode{0b0, 1});
    REQUIRE(sample_bmt['A'] == utility_types::VariableLenghCode{0b01, 2});
    REQUIRE(sample_bmt['D'] == utility_types::VariableLenghCode{0b011, 3});
    REQUIRE(sample_bmt['C'] == utility_types::VariableLenghCode{0b111, 3});

    Treap tr1;
    huffman::RestoreTreapByInfo(info, order, tr1);
    auto map_table = tr1.GetMappingTable();

    REQUIRE(map_table['B'] == utility_types::VariableLenghCode{0b0, 1});
    REQUIRE(map_table['A'] == utility_types::VariableLenghCode{0b01, 2});
    REQUIRE(map_table['D'] == utility_types::VariableLenghCode{0b011, 3});
    REQUIRE(map_table['C'] == utility_types::VariableLenghCode{0b111, 3});
}

