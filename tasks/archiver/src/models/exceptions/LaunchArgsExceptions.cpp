#include "LaunchArgsExceptions.h"

ArgsLogicException::ArgsLogicException(const char *message) : logic_error(message) {}

ArgsRuntimeException::ArgsRuntimeException(const char *message, const char *invalid_arg):
        std::runtime_error(message){}
