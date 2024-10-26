#include <exception>
#include <string>

class FileException : public std::exception {
public:
    explicit FileException(const char* message, const char *file_name = "");

    const char* what() const noexcept override;
    const char *getFile();
    void AddNoteFile(const char *file_name);

protected:
    const char *message_;
    const char *file_name_;
};

class FileReaderException: public FileException {
public:
    explicit FileReaderException(const char* message, const char *file_name = "");
};

class FileWriterException: public FileException {
public:
    explicit FileWriterException(const char* message, const char *file_name = "");
};

