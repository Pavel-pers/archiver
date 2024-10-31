#include "models/StreamReader.h"
#include "models/StreamWriter.h"
#include "models/Treap.h"
#include <queue>


class Decoder{
public:
    explicit Decoder(StreamReader& reader);
    void InitHeaderInfo();
    bool EndOfArchive() const;
    std::string ParseFileName();
    void UnpackFile(StreamWriter& writer);
private:
    static const size_t MAX_BUFFER_SIZE = 64;
    StreamReader& reader_;
    Treap code_treap_;
    std::queue<utility_types::PaddedByte>unprocessed_bytes_;
    bool archive_finish_;

    std::pair<utility_types::MappingTableInfo, std::vector<utility_types::PaddedByte>>ParseHeaderInfo();

    void ProcessBufferFileName(std::string& file_name);
    void ProcessBufferUnpackFile(StreamWriter& writer);
};
