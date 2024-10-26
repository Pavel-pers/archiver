#include "LaunchArgsExceptions.h"

ArgsLogicException::ArgsLogicException(const std::string &message) : logic_error(message) {}

ArgsRuntimeException::ArgsRuntimeException(const std::string &message, const std::string &invalid_arg)
        : std::runtime_error(message + (invalid_arg.empty()? "" : "|In argument: " + invalid_arg)) {}
