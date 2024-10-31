#include <iostream>
#include <fstream>
#include <string>
#include "models/ArchiverArgs.h"
#include "models/exceptions/LaunchArgsExceptions.h"
#include "models/exceptions/FileExceptions.h"
#include "encode.h"
#include "decode.h"

const int USER_ERROR_CODE = 111;

void Compress(CompressParams params) {
    // calc freq
    FilesByteFrequancy frequancy;
    for (const char *file_name: params.files) {
        std::ifstream input_stream(file_name);
        StreamReader reader(input_stream);
        try {
            frequancy.ScanFile(reader);
        } catch (FileException e) {
            e.AddNoteFile(file_name);
            throw e;
        }
    }

    Treap mapping_treap = huffman::BuildTreapByFrequancy(frequancy.GetSummaryFrequancy());
    utility_types::ByteMappingTable mapping_table = mapping_treap.GetMappingTable();
    // begin encoding
    std::ofstream archive_stream(params.archive_name);
    StreamWriter archive_writer(archive_stream);

    Encoder encoder(mapping_table, archive_writer);
    try {
        encoder.PrintArchiveHeader();
    } catch (FileException e) {
        e.AddNoteFile(params.archive_name);
        throw e;
    }

    for (const char *file_name: params.files) {
        std::ifstream input_stream(file_name);
        StreamReader reader(input_stream);
        try {
            encoder.PrintFile(file_name, reader);
        } catch (FileException e) {
            e.AddNoteFile(params.archive_name);
            throw e;
        }
    }

    encoder.PrintEof();
}

void Decompress(DecompressParams params) {
    // init decoder, parse header
    std::ifstream archive_stream(params.archive_name);
    StreamReader archive_reader(archive_stream);
    Decoder decoder(archive_reader);
    try {
        decoder.InitHeaderInfo();
    } catch (FileException e) {
        e.AddNoteFile(params.archive_name);
        throw e;
    }
    // print archied files
    while (!decoder.EndOfArchive()) {
        std::ofstream file_stream(decoder.ParseFileName());
        StreamWriter writer(file_stream);
        decoder.UnpackFile(writer);
    }
}

void PrintHelp() {
    std::cout <<
              "This is arhciver project\n"
              "Flags:\n\n"
              "-c archive_name file1 [file2 ...] -- means compress files\n"
              "-d archive_name -- means decompress file\n"
              "-h -- print help info";
}

int main(const int argc, const char **argv) {
    ArchiverArgs main_args;
    try {
        main_args.InitArgs(argc, argv);
    } catch (ArgsRuntimeException e) {
        std::string exception_msg = e.what();
        if (strcmp(e.GetInvalidArgument(), "") != 0) {
            exception_msg += e.GetInvalidArgument();
        }
        throw std::ios_base::failure(exception_msg, std::error_code(USER_ERROR_CODE, std::generic_category()));
    }

    try {
        switch (main_args.GetFlag()) {
            case args::LaunchFlag::COMPRESS:
                Compress(main_args.GetCompressParams());
                break;
            case args::LaunchFlag::DECOMPRESS:
                Decompress(main_args.GetDecompressParams());
                break;
            case args::LaunchFlag::HELP:
                PrintHelp();
                break;
            case args::LaunchFlag::UNDEFINED:
                throw ArgsRuntimeException("Uncknown flag");
        }
    } catch (FileException e) {
        throw std::ios_base::failure(e.what(), std::error_code(USER_ERROR_CODE, std::generic_category()));
    }
}
