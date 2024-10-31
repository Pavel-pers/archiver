#include "encode.h"

using utility_types::ByteFrequancy, utility_types::ByteMappingTable, utility_types::CodeLengh, utility_types::BitCode;
using utility_types::VariableLenghCode;
using utility_types::MappingTableInfo;

Encoder::Encoder(const ByteMappingTable &mapping_table, StreamWriter &archive_writer) :
        mapping_table_(mapping_table), writer_(archive_writer), already_print_any_file_(false) {}

void Encoder::PrintArchiveHeader() {
    MappingTableInfo map_info = utility_bit::GetMappingTableInfo(mapping_table_);
    writer_.Write(map_info.symbols_count, DataLimits::BITS_IN_SYMBOL_COUNT);

    std::vector<utility_types::PaddedByte> ordered_padded_bytes = huffman::GetIndexOrderByCanonical(mapping_table_);
    for (size_t byte : ordered_padded_bytes) {
        writer_.Write(byte, DataLimits::BITS_IN_PADDED_BYTE);
    }

    {
        CodeLengh cur_lengh = 1;
        BitCode summary_codes_out = 0;
        while (summary_codes_out < map_info.symbols_count) {
            writer_.Write(map_info.lengh_count[cur_lengh], DataLimits::BITS_IN_LENGH_COUNT);
            summary_codes_out += map_info.lengh_count[cur_lengh];
            cur_lengh++;
        }
    }
}


void Encoder::PrintFileHeader(const char *file_name) {
    for (const char *byte_pointer = file_name; byte_pointer; byte_pointer++) {
        uint8_t byte = *byte_pointer;
        writer_.Write(mapping_table_[byte]);
    }
}

void Encoder::PrintFileBody(StreamReader &file_to_archive) {
    while (!file_to_archive.Eof()) {
        uint8_t orig_byte = file_to_archive.ReadByte();
        writer_.Write(mapping_table_[orig_byte]);
    }
}

void Encoder::PrintFile(const char *file_name, StreamReader &file_reader) {
    if (already_print_any_file_) {
        PrintOneMoreFileSymbol();
    }
    already_print_any_file_ = true;
    PrintFileHeader(file_name);
    PrintFileBody(file_reader);
}

void Encoder::PrintEof() {
    writer_.Write(mapping_table_[SpecialSymbols::ARCHIVE_END]);
}

void Encoder::PrintOneMoreFileSymbol() {
    writer_.Write(mapping_table_[SpecialSymbols::ONE_MORE_FILE]);
}
