#include "LaunchArgs.h"
#include "cstring"
#include <vector>

enum class LaunchFlag{
    COMPRESS,
    DECOMPRESS,
    HELP
};

struct CompressParams{
    const char* archive_name;
    const std::vector<const char*> files;
    explicit CompressParams(const char*, const std::vector<const char*>);
};

struct DecompressParams{
    const char* archive_name;
    explicit DecompressParams(const char*);
};


class ArchiverArgs: private LaunchArgs{
public:
    ArchiverArgs(const int, const char**);
    CompressParams GetCompressParams() const;
    DecompressParams GetDecompressParams() const;
private:
    void CompressHandler(const std::vector<const char*>& params);
    void DecompressHandler(const std::vector<const char*>& params);
    void HelpHandler(const std::vector<const char*>& params);

    LaunchFlag launch_flag_;
    std::vector<const char*> opt_info_;
};