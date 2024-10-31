#include "LaunchArgs.h"
#include "exceptions/LaunchArgsExceptions.h"

void LaunchArgs::InitArgs(const int argc, const char **argv) {
    if (run_name_ != nullptr) {
        throw std::logic_error("init already inited flags");
    }

    run_name_ = argv[0];
    for (size_t arg_i = 1; arg_i < static_cast<size_t>(argc); arg_i++) {
        std::string_view param_i = argv[arg_i];
        if (param_i[0] == '-') { // flag handler
            if (param_i.size() == 1) {
                throw ArgsRuntimeException("empty flag error", argv[arg_i]);
            }

            if (param_i[1] == '-') { // fflag handler
                if (param_i.size() == 2) {
                    throw ArgsRuntimeException("empty fflag error", argv[arg_i]);
                }
                fflags_.push_back(argv[arg_i]);
            } else {
                flags_.push_back(argv[arg_i]);
            }
        } else { // simple paramters handler
            params_.push_back(argv[arg_i]);
        }
    }
}

LaunchArgs::LaunchArgs(const int argc, const char **argv) {
    InitArgs(argc, argv);
}

const std::pair<std::vector<const char *>&, std::vector<const char *>&> LaunchArgs::GetFlags() {
    return {flags_, fflags_};
}

const std::vector<const char *>& LaunchArgs::GetParams() {
    return params_;
}

