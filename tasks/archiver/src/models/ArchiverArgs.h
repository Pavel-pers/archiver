#include "LaunchArgs.h"
#include "cstring"
#include <vector>

namespace args {
    enum class LaunchFlag {
        COMPRESS,
        DECOMPRESS,
        HELP,
        UNDEFINED
    };
}

struct CompressParams {
    const char *archive_name;
    const std::vector<const char *> files;

    explicit CompressParams(const char *, const std::vector<const char *>);
};

struct DecompressParams {
    const char *archive_name;

    explicit DecompressParams(const char *);
};


class ArchiverArgs : private LaunchArgs {
public:
    ArchiverArgs(): launch_flag_(args::LaunchFlag::UNDEFINED) {};
    ArchiverArgs(const int, const char **);

    void InitArgs(const int, const char **) override;

    CompressParams GetCompressParams() const;

    DecompressParams GetDecompressParams() const;

    args::LaunchFlag GetFlag() const;

private:
    void CompressHandler(const std::vector<const char *> &params);

    void DecompressHandler(const std::vector<const char *> &params);

    void HelpHandler(const std::vector<const char *> &params);

    args::LaunchFlag launch_flag_;
    std::vector<const char *> opt_info_;
};