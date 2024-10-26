#include "catch.hpp"
#include <cstring>
#include <iostream>
#include "../src/models/ArchiverArgs.h"
#include "../src/models/exceptions/LaunchArgsExceptions.h"

TEST_CASE("Exceptions1") {
    const char *arg1[2] = {"fn", "--abc"};
    REQUIRE_THROWS(ArchiverArgs(2, arg1));
    const char *arg2[3] = {"fn", "-c", "-d"};
    REQUIRE_THROWS(ArchiverArgs(3, arg2));
    const char *arg3[3] = {"fn", "-h", "aaa"};
    REQUIRE_THROWS(ArchiverArgs(3, arg3));
    const char *arg4[2] = {"fn", "-c"};
    REQUIRE_THROWS(ArchiverArgs(2, arg4));
    const char *arg5[2] = {"fn", "-d"};
    REQUIRE_THROWS(ArchiverArgs(2, arg5));
    const char *arg6[2] = {"fn", "-obeme"};
    REQUIRE_THROWS(ArchiverArgs(2, arg6));
    const char *arg7[4] = {"fn", "-d", "hi", "aa"};
    REQUIRE_THROWS(ArchiverArgs(4, arg7));
}

TEST_CASE("Exceptions2") {
    const char *arg1[4] = {"fn", "-c", "arch", "file"};
    ArchiverArgs aa1(4, arg1);
    REQUIRE_THROWS(aa1.GetDecompressParams());

    const char *arg2[4] = {"fn", "-h"};
    ArchiverArgs aa2(2, arg2);
    REQUIRE_THROWS(aa2.GetCompressParams());
    REQUIRE_THROWS(aa2.GetDecompressParams());
    const char *arg3[3] = {"fn", "-d", "aa"};
    ArchiverArgs aa3(3, arg3);
    REQUIRE_THROWS(aa3.GetCompressParams());
}

TEST_CASE("Simple Dimple") {
    const char *arg1[5] = {"fn", "-c", "arch", "file1", "file2"};
    ArchiverArgs aa1(5, arg1);
    REQUIRE(strcmp(aa1.GetCompressParams().archive_name, "arch") == 0);
    REQUIRE(aa1.GetCompressParams().files.size() == 2);
    REQUIRE(strcmp(aa1.GetCompressParams().files[0], "file1") == 0);
    REQUIRE(strcmp(aa1.GetCompressParams().files[1], "file2") == 0);

    const char *arg2[3] = {"fn", "-d", "arch2"};
    ArchiverArgs aa2(3, arg2);
    REQUIRE(strcmp(aa2.GetDecompressParams().archive_name, "arch2") == 0);
}
