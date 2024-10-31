#include <cctype>
#include <string>
#include <vector>

class LaunchArgs {
public:
    LaunchArgs(const int argc, const char** argv);
    LaunchArgs(): run_name_(nullptr) {}

    virtual void InitArgs(const int argc, const char** argv);
    const std::pair<std::vector<const char*>&, std::vector<const char*>&> GetFlags();
    const std::vector<const char*>& GetParams();
private:
    const char* run_name_;
    std::vector<const char*> flags_;
    std::vector<const char*> fflags_;
    std::vector<const char*> params_;
};