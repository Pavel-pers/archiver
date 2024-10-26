add_catch(test_args_parser
        localtest.cpp
        ../src/models/ArchiverArgs.h
        ../src/models/ArchiverArgs.cpp
        ../src/models/exceptions/LaunchArgsExceptions.h
        ../src/models/exceptions/LaunchArgsExceptions.cpp
        ../src/models/LaunchArgs.cpp
        ../src/models/LaunchArgs.h
)

add_custom_target(
        test_archiver
        WORKING_DIRECTORY
        DEPENDS archiver
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/test.py ${CMAKE_BINARY_DIR}/archiver ${CMAKE_CURRENT_SOURCE_DIR}/data
)
