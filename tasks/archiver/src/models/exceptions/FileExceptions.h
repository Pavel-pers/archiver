#include <exception>
#include <string>

class FileException : public std::exception {
public:
    explicit FileException(const char* message, const char *file_name = "");

    const char* what() const noexcept override;
    const char *GetFile();
    void AddNoteFile(const char *file_name);

protected:
    const char *message_;
    const char *file_name_;
};


class FileBrokenException: public FileException {
public:
    explicit FileBrokenException(const char* file_name = "");
};

class FileFormatException: public FileException {
public:
    explicit FileFormatException(const char* file_name = "");
};
