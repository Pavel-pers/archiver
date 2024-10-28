#include <istream>
#include <fstream>
#include <cstdint>
#include <stdexcept>

class StreamReader {
public:
    explicit StreamReader(std::istream &in);
    uint8_t ReadByte(size_t count_of_bits = CHAR_BIT); // !reader returns in Little-Endian format
    uint16_t Read2Bytes(size_t count_of_bits = CHAR_BIT * 2); // !reader returns in Little-Endian format
    bool Eof();
private:
    using BlockSizeT = uint8_t;
    static const size_t MAX_BUFFER_SIZE = 1024;

    std::istream &in_stream_;
    char buffer_[MAX_BUFFER_SIZE];
    size_t bit_index_;
    size_t last_byte_readed_;

    void FlushBuffer();
    uint8_t GetFromFirstBlock(uint8_t n);
};
