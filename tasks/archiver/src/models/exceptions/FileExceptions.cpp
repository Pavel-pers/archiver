#include "FileExceptions.h"

FileException::FileException(const char *message, const char *file_name) : message_(message), file_name_(file_name) {}

const char *FileException::what() const noexcept {
    return message_;
}

const char *FileException::GetFile() {
    return file_name_;
}

void FileException::AddNoteFile(const char *file_name) {
    file_name_ = file_name;
}

// TODO file name should included in what
// TODO dont forget about adding notes

FilePermitionException::FilePermitionException(const char *file_name) : FileException("Unable to access file",
                                                                                      file_name) {}

FileFormatException::FileFormatException(const char *file_name) : FileException("Could't read the file current format",
                                                                                file_name) {}

InvalidArchiveException::InvalidArchiveException(const char *file_name) : FileException(
        "Archive file is probably broken") {}
