add_catch(test_args_parser
        argsTest.cpp
        ../src/models/ArchiverArgs.cpp
        ../src/models/exceptions/LaunchArgsExceptions.cpp
        ../src/models/LaunchArgs.cpp
)
add_catch(test_StreamReader
        readerTest.cpp
        ../src/utility.cpp
        ../src/models/StreamReader.cpp
        ../src/models/exceptions/FileExceptions.cpp
)
add_catch(test_StreamWriter
        writerTest.cpp
        ../src/utility.cpp
        ../src/models/StreamWriter.cpp
        ../src/models/exceptions/FileExceptions.cpp
)
add_catch(test_Treap
        treapTest.cpp
        ../src/utility.cpp
        ../src/models/Treap.cpp
)
add_catch(test_Huffman
        huffmanTest.cpp
        ../src/utility.cpp
        ../src/models/Treap.cpp
        ../src/huffman.cpp
)

#add_custom_target(
#        test_archiver
#        WORKING_DIRECTORY
#        DEPENDS archiver
#        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/test.py ${CMAKE_BINARY_DIR}/archiver ${CMAKE_CURRENT_SOURCE_DIR}/data
#)
