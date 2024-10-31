#include <exception>
#include <stdexcept>
#include <string>

class ArgsLogicException: public std::logic_error {
public:
    explicit ArgsLogicException(const char* message);
};

class ArgsRuntimeException: public std::runtime_error {
public:
    explicit ArgsRuntimeException(const char* message, const char* invalid_arg = "");
    const char* GetInvalidArgument();
private:
    const char* invalid_arg_;
};


