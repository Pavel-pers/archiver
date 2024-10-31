#include <ostream>
#include <cstdint>
#include "../utility.h"

class StreamWriter{
public:
    explicit StreamWriter(std::ostream& out);
    void WriteFast(uint8_t chank, size_t size);
    void Write(uint16_t chank, size_t size);
    void Write(utility_types::VariableLenghCode code);
    void Close();
private:
    using BlockSizeT = uint8_t;
    static const size_t BUFFER_MAX_SIZE = 1024;

    char buffered_[BUFFER_MAX_SIZE];
    size_t bit_index_;
    std::ostream& out_stream_;

    void Flush(size_t size);
    BlockSizeT CompleteLastChank(uint8_t chank, BlockSizeT chank_size); // returns number of bits filled
};