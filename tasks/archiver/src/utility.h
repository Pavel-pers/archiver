#include <inttypes.h>
#include <array>
#include <bitset>
#include <limits>
#include <cstdint>
#include <vector>


enum SpecialSymbols: uint16_t {
    FILENAME_END = 256,
    ONE_MORE_FILE = 257,
    ARCHIVE_END = 258,
    ALP_POWER = 259
};

enum DataLimits: size_t {
    BITS_IN_SYMBOL_COUNT = 9,
    BITS_IN_LENGH_COUNT = 9,
    BITS_IN_PADDED_BYTE = 9
};

namespace utility_types {
    const size_t MAX_CODE_LENGH = 258;

    using BitCode = uint16_t;
    using PaddedByte = uint16_t; // TODO update types
    using CodeLengh = uint16_t;
    using ByteFrequancy = std::array<size_t, SpecialSymbols::ALP_POWER>;

    struct VariableLenghCode {
        BitCode code;
        CodeLengh lengh;

        explicit VariableLenghCode(BitCode code = 0, CodeLengh lengh = 0) : code(code), lengh(lengh) {}
    };
    bool operator==(VariableLenghCode lhs, VariableLenghCode rhs);

    struct MappingTableInfo {
        uint16_t symbols_count;
        std::array<uint16_t, SpecialSymbols::ALP_POWER> lengh_count;
        MappingTableInfo(): symbols_count(0) {
            lengh_count.fill(0);
        }
    };

    using ByteMappingTable = std::array<VariableLenghCode, SpecialSymbols::ALP_POWER>;
}

namespace utility_bit {
    uint8_t ReverseChar(uint8_t c);

    char ReverseChar(char c);

    utility_types::MappingTableInfo GetMappingTableInfo(utility_types::ByteMappingTable map_table);

    bool IsSpecialSymbol(utility_types::PaddedByte);
}


