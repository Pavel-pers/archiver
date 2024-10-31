#include "decode.h"
#include "huffman.h"
#include "models/exceptions/FileExceptions.h"

using utility_types::ByteFrequancy, utility_types::ByteMappingTable, utility_types::CodeLengh, utility_types::BitCode, utility_types::PaddedByte;
using utility_types::VariableLenghCode;
using utility_types::MappingTableInfo;


Decoder::Decoder(StreamReader &reader) : reader_(reader), archive_finish_(false) {}

void Decoder::InitHeaderInfo() {
    auto [map_info, ordered_bytes] = ParseHeaderInfo();
    huffman::RestoreTreapByInfo(map_info, ordered_bytes, code_treap_);
}

std::pair<utility_types::MappingTableInfo, std::vector<utility_types::PaddedByte>> Decoder::ParseHeaderInfo() {
    utility_types::MappingTableInfo map_info;
    map_info.symbols_count = reader_.Read2Bytes(DataLimits::BITS_IN_SYMBOL_COUNT);

    std::vector<PaddedByte> ordered_bytes(map_info.symbols_count);
    for (PaddedByte &byte_i: ordered_bytes) {
        byte_i = reader_.Read2Bytes(DataLimits::BITS_IN_PADDED_BYTE);
    }

    {
        CodeLengh cur_lengh = 1;
        BitCode codes_counter = 0;
        while (codes_counter < map_info.symbols_count) {
            map_info.lengh_count[cur_lengh] = reader_.Read2Bytes(DataLimits::BITS_IN_LENGH_COUNT);
            codes_counter += map_info.lengh_count[cur_lengh];
            cur_lengh++;
        }
    }

    return {map_info, ordered_bytes};
}

std::string Decoder::ParseFileName() {
    if (archive_finish_) {
        throw std::logic_error("parsing after archive finish");
    }

    std::string file_name;
    TreapAutomata fn_automata = code_treap_.BuildAutomata({SpecialSymbols::FILENAME_END});
    while (!fn_automata || !reader_.Eof()) {
        fn_automata.Feed(reader_.Read2Bytes(), unprocessed_bytes_);
        if (unprocessed_bytes_.size() > MAX_BUFFER_SIZE) {
            ProcessBufferFileName(file_name);
        }
    }
    if (!fn_automata) {
        throw InvalidArchiveException();
    }

    ProcessBufferFileName(file_name);
    return file_name;
}

void Decoder::ProcessBufferFileName(std::string &file_name) {
    while (!unprocessed_bytes_.empty() && unprocessed_bytes_.front() != SpecialSymbols::FILENAME_END) {
        if (utility_bit::IsSpecialSymbol(unprocessed_bytes_.front())) {
            throw InvalidArchiveException();
        }
        file_name += static_cast<char>(unprocessed_bytes_.front());
        unprocessed_bytes_.pop();
    }

    if (!unprocessed_bytes_.empty()) {
        unprocessed_bytes_.pop();
    }
}

void Decoder::UnpackFile(StreamWriter &writer) {
    if (archive_finish_) {
        throw std::logic_error("parsing after archive finish");
    }

    std::string file_name;
    TreapAutomata fn_automata = code_treap_.BuildAutomata({SpecialSymbols::ONE_MORE_FILE, SpecialSymbols::ARCHIVE_END});
    while (!fn_automata || !reader_.Eof()) {
        fn_automata.Feed(reader_.Read2Bytes(), unprocessed_bytes_);
        if (unprocessed_bytes_.size() > MAX_BUFFER_SIZE) {
            ProcessBufferUnpackFile(writer);
        }
    }
    if (!fn_automata) {
        throw InvalidArchiveException();
    }

    ProcessBufferUnpackFile(writer);
}


void Decoder::ProcessBufferUnpackFile(StreamWriter &writer) {
    while (!unprocessed_bytes_.empty() && (unprocessed_bytes_.front() != SpecialSymbols::ONE_MORE_FILE &&
                                           unprocessed_bytes_.front() != SpecialSymbols::ARCHIVE_END)) {
        if (utility_bit::IsSpecialSymbol(unprocessed_bytes_.front())) {
            throw InvalidArchiveException();
        }

        writer.Write(static_cast<char>(unprocessed_bytes_.front()), CHAR_BIT);
        unprocessed_bytes_.pop();
    }

    if (!unprocessed_bytes_.empty()) {
        if (unprocessed_bytes_.front() == SpecialSymbols::ARCHIVE_END) {
            archive_finish_ = true;
        }
        unprocessed_bytes_.pop();
    }
}

bool Decoder::EndOfArchive() const {
    return archive_finish_;
}

