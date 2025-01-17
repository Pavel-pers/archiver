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

    void PrintFile(const char *file_name, StreamReader &file_reader);

    void PrintArchiveHeader();

private:
    const utility_types::ByteMappingTable &mapping_table_;
    StreamWriter &writer_;
    bool already_print_any_file_;


    void PrintFileHeader(const char *file_name);

    void PrintFileBody(StreamReader &file_to_archive);

    void PrintOneMoreFileSymbol();
};
