#include "FileExceptions.h"

FileException::FileException(const char* message, const char *file_name) : message_(message), file_name_(file_name) {}

const char *FileException::what() const noexcept {
    return message_;
}

const char *FileException::getFile() {
    return file_name_;
}

void FileException::AddNoteFile(const char* file_name) {
    file_name_ = file_name;
}

// TODO file name should included in what
// TODO dont forget about adding notes
FileReaderException::FileReaderException(const char *message, const char *problem_file) :
        FileException(message, problem_file) {}

FileWriterException::FileWriterException(const char *message, const char *problem_file) :
        FileException(message, problem_file) {}

