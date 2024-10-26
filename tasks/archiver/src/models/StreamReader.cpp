#include "StreamReader.h"
#include "exceptions/FileExceptions.h"
#include "../utility.h"

StreamReader::StreamReader(std::istream &in) : in_(in), bit_index_(0) {
    FlushBuffer();
}

void StreamReader::FlushBuffer() {
    if (in_.bad()) {
        throw FileReaderException("file permition denied");
    }
    if (in_.fail() && !in_.eof()) {
        throw FileReaderException("incorrect file format");
    }

    std::fill(buffer_, buffer_ + MAX_BUFFER_SIZE, 0);
    in_.read(buffer_, MAX_BUFFER_SIZE);
    last_byte_readed_ = in_.gcount();
    for (char &ci: buffer_) { // reformat to Little-Endian
        ci = utility_bit::ReverseChar(ci);
    }
    bit_index_ = 0;
}

uint8_t StreamReader::GetFromFirstBlock(uint8_t n) {
    uint8_t index_in_block = bit_index_ & (CHAR_BIT - 1);
    if (n > CHAR_BIT - index_in_block) {
        throw std::logic_error("count of requested bits exceed first block len");
    }
    if (bit_index_ == MAX_BUFFER_SIZE * CHAR_BIT) { // if the end reached
        FlushBuffer();
    }

    uint8_t res = (buffer_[bit_index_ / CHAR_BIT] >> index_in_block) & ((1 << n) - 1);
    bit_index_ += n;
    return res;
}

uint8_t StreamReader::ReadByte(size_t count_of_bits) {
    if (count_of_bits > CHAR_BIT) {
        throw std::logic_error("count of bits must be less or equal 8");
    }

    uint8_t prefix_size = CHAR_BIT - (bit_index_ & (CHAR_BIT - 1));
    if (count_of_bits <= prefix_size) {
        return GetFromFirstBlock(count_of_bits);
    }

    uint8_t prefix = GetFromFirstBlock(prefix_size);
    uint8_t suffix = GetFromFirstBlock(count_of_bits - prefix_size);
    return prefix | (suffix << prefix_size);
}

uint16_t StreamReader::Read2Bytes(size_t count_of_bits) {
    if (count_of_bits > CHAR_BIT * 2) {
        throw std::logic_error("count of bits must be less or equal 16");
    }

    if (count_of_bits <= CHAR_BIT) {
        return static_cast<uint16_t>(ReadByte(CHAR_BIT));
    }
    return static_cast<uint16_t>(ReadByte(CHAR_BIT)) |
           (static_cast<uint16_t>(ReadByte(count_of_bits - CHAR_BIT)) << CHAR_BIT);
}

bool StreamReader::Eof() {
    return bit_index_ >= last_byte_readed_ * CHAR_BIT && in_.eof();
}
