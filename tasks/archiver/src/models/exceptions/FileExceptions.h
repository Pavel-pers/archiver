#include <exception>

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


class FilePermitionException: public FileException {
public:
    explicit FilePermitionException(const char* file_name = "");
};

class FileFormatException: public FileException {
public:
    explicit FileFormatException(const char* file_name = "");
};

class InvalidArchiveException: public FileException {
public:
    explicit InvalidArchiveException(const char* file_name = "");
};
