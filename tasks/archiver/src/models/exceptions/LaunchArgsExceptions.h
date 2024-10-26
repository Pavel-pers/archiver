#include <exception>
#include <stdexcept>
#include <string>

class ArgsLogicException: public std::logic_error {
public:
    explicit ArgsLogicException(const std::string& message);
};

class ArgsRuntimeException: public std::runtime_error {
public:
    explicit ArgsRuntimeException(const std::string& message, const std::string& invalid_arg = "");
};


