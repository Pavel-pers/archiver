#include "models/StreamReader.h"
#include "models/StreamWriter.h"
#include "models/exceptions/FileExceptions.h"
#include "huffman.h"
#include "utility.h"
#include <array>

class Encoder {
public:
    Encoder(const utility_types::ByteMappingTable &mapping_table, StreamWriter &archive_writer);

    void PrintEof();

    void PrintFile(char *file_name, StreamReader &file_reader);

private:
    static const size_t BITS_IN_SYMBOL_COUNT = 9;
    static const size_t BITS_IN_LENGH_COUNT = 9;
    static const size_t BITS_IN_PADDED_BYTE = 9;

    const utility_types::ByteMappingTable &mapping_table_;
    StreamWriter &writer_;
    bool already_print_any_file_;

    void PrintArchiveHeader();

    void PrintFileHeader(char *file_name);

    void PrintFileBody(StreamReader &file_to_archive);

    void PrintOneMoreFileSymbol();
};