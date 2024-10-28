#include "StreamWriter.h"
#include "exceptions/FileExceptions.h"
#include "../utility.h"

StreamWriter::StreamWriter(std::ostream &out): bit_index_(0), out_stream_(out) {
    std::fill(buffered_, buffered_ + BUFFER_MAX_SIZE, 0);
}

void StreamWriter::Flush(size_t size) {
    out_stream_.write(buffered_, static_cast<std::streamsize>(size));
    std::fill(buffered_, buffered_ + BUFFER_MAX_SIZE, 0);
    bit_index_ = 0;

    if (out_stream_.fail()) {
        throw FileFormatException();
    }
    if (out_stream_.bad()) {
        throw FileBrokenException();
    }
}

void StreamWriter::Close() {
    Flush((bit_index_ + CHAR_BIT - 1) / (CHAR_BIT)); // flush not complited buffer
}


StreamWriter::BlockSizeT StreamWriter::CompleteLastChank(uint8_t chank, BlockSizeT chank_size) {
    BlockSizeT act_size = std::min(chank_size, static_cast<BlockSizeT>(CHAR_BIT - (bit_index_ % CHAR_BIT)));
    chank &= (1 << act_size) - 1;
    chank <<= (bit_index_ % CHAR_BIT);
    chank = utility_bit::ReverseChar(chank);

    buffered_[bit_index_ / CHAR_BIT] |= static_cast<char>(chank); // NOLINT
    bit_index_ += act_size;
    if (bit_index_ == BUFFER_MAX_SIZE * CHAR_BIT) {
        Flush(BUFFER_MAX_SIZE);
    }

    return act_size;
}


void StreamWriter::WriteFast(uint8_t chank, size_t size) {
    if (size > CHAR_BIT) {
        std::logic_error("size should be less or equal 8");
    }

    BlockSizeT act_bits = CompleteLastChank(chank, size);
    if (act_bits != size) {
        CompleteLastChank(chank >> act_bits, size - act_bits);
    }
}

void StreamWriter::Write(uint16_t chank, size_t size) {
    if (size > CHAR_BIT * 2) {
        std::logic_error("size should be less or equal 16");
    }

    if (size > CHAR_BIT) {
        uint8_t left_part = chank & ((1 << CHAR_BIT) - 1);
        uint8_t right_part = chank >> CHAR_BIT;
        WriteFast(left_part, CHAR_BIT);
        WriteFast(right_part, size - CHAR_BIT);
    } else {
        WriteFast(static_cast<uint8_t>(chank), size);
    }
}





