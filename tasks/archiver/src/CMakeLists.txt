set(CMAKE_CXX_STANDARD 20)
add_executable(
        archiver
        archiver.cpp
        models/ArchiverArgs.h
        models/ArchiverArgs.cpp
        models/exceptions/LaunchArgsExceptions.h
        models/exceptions/LaunchArgsExceptions.cpp
        models/LaunchArgs.cpp
        models/LaunchArgs.h
)
