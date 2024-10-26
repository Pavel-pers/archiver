#include "ArchiverArgs.h"
#include "exceptions/LaunchArgsExceptions.h"

CompressParams::CompressParams(const char *archive_name, const std::vector<const char *> files) :
        archive_name(archive_name), files(files) {}

DecompressParams::DecompressParams(const char *archive_name):
        archive_name(archive_name){}

void ArchiverArgs::CompressHandler(const std::vector<const char *> &params) {
    if (params.empty()) {
        throw ArgsRuntimeException("expected archive_name got 0 parametrs");
    }
    if (params.size() == 1) {
        throw ArgsRuntimeException("expected file to compress got 1 paramets");
    }

    launch_flag_ = args::LaunchFlag::COMPRESS;
    opt_info_ = params;
}

void ArchiverArgs::DecompressHandler(const std::vector<const char *> &params) {
    if (params.empty()) {
        throw ArgsRuntimeException("expected archive_name got 0 parametrs");
    }
    if (params.size() > 1) {
        throw ArgsRuntimeException("unexpected parameter", params[1]);
    }

    launch_flag_ = args::LaunchFlag::DECOMPRESS;
    opt_info_ = params;
}

void ArchiverArgs::HelpHandler(const std::vector<const char *> &params) {
    if (!params.empty()) {
        throw ArgsRuntimeException("unexpected parameter", params[0]);
    }
    launch_flag_ = args::LaunchFlag::HELP;
}


ArchiverArgs::ArchiverArgs(const int argc, const char **argv) : LaunchArgs(argc, argv) {
    const auto &[flags, fflags] = GetFlags();
    const std::vector<const char *> &params = GetParams();

    if (!fflags.empty()) {
        throw ArgsRuntimeException("unexpected flag", fflags[0]);
    }

    if (flags.size() > 1) {
        throw ArgsRuntimeException("more then one flag");
    }
    if (flags.empty()) {
        throw ArgsRuntimeException("unexpected non empty flag list, check -h");
    }

    if (std::strcmp(flags[0], "-c") == 0) {
        CompressHandler(params);
    } else if (std::strcmp(flags[0], "-d") == 0) {
        DecompressHandler(params);
    } else if (std::strcmp(flags[0], "-h") == 0) {
        HelpHandler(params);
    } else {
        throw ArgsRuntimeException("uncknown flag, check -h");
    }
}

DecompressParams ArchiverArgs::GetDecompressParams() const {
    if (launch_flag_ != args::LaunchFlag::DECOMPRESS) {
        throw ArgsLogicException("unexpected use decompress getter");
    }

    return static_cast<DecompressParams>(opt_info_[0]);
}

CompressParams ArchiverArgs::GetCompressParams() const {
    if (launch_flag_ != args::LaunchFlag::COMPRESS) {
        throw ArgsLogicException("unexpected use compress getter");
    }

    const char *file_name = opt_info_[0];
    auto files_to_compress = std::vector<const char *>(opt_info_.begin() + 1, opt_info_.end());
    return CompressParams(file_name, files_to_compress);
}
