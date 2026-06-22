#include <filesystem>
#include <fstream>

#include "c_api/gorgonzola.h"
#include "c_api_test/c_api_test.h"
#include "gtest/gtest.h"

using namespace gorgonzola::main;
using namespace gorgonzola::testing;
using namespace gorgonzola::common;

class CApiVersionTest : public CApiTest {
public:
    std::string getInputDir() override {
        return TestHelper::appendGorgonzolaRootPath("dataset/tinysnb/");
    }

    void TearDown() override { APIDBTest::TearDown(); }
};

class EmptyCApiVersionTest : public CApiVersionTest {
public:
    std::string getInputDir() override { return "empty"; }
};

TEST_F(EmptyCApiVersionTest, GetVersion) {
    gorgonzola_connection_destroy(&connection);
    gorgonzola_database_destroy(&_database);
    auto version = gorgonzola_get_version();
    ASSERT_NE(version, nullptr);
    ASSERT_STREQ(version, GORGONZOLA_CMAKE_VERSION);
    gorgonzola_destroy_string(version);
}

TEST_F(CApiVersionTest, GetStorageVersion) {
    auto storageVersion = gorgonzola_get_storage_version();
    if (inMemMode) {
        GTEST_SKIP();
    }
    // Reset the database to ensure that the lock on db file is released.
    gorgonzola_connection_destroy(&connection);
    gorgonzola_database_destroy(&_database);
    auto data = std::filesystem::path(databasePath);
    std::ifstream dbFile;
    dbFile.open(data, std::ios::binary);
    ASSERT_TRUE(dbFile.is_open());
    char magic[11];
    dbFile.read(magic, 10);
    magic[10] = '\0';
    ASSERT_STREQ(magic, "GORGONZOLA");
    uint64_t actualVersion;
    dbFile.read(reinterpret_cast<char*>(&actualVersion), sizeof(actualVersion));
    dbFile.close();
    ASSERT_EQ(storageVersion, actualVersion);
}

TEST_F(EmptyCApiVersionTest, GetStorageVersion) {
    auto storageVersion = gorgonzola_get_storage_version();
    if (inMemMode) {
        GTEST_SKIP();
    }
    // Reset the database to ensure that the lock on db file is released.
    gorgonzola_connection_destroy(&connection);
    gorgonzola_database_destroy(&_database);
    auto data = std::filesystem::path(databasePath);
    std::ifstream dbFile;
    dbFile.open(data, std::ios::binary);
    ASSERT_TRUE(dbFile.is_open());
    char magic[11];
    dbFile.read(magic, 10);
    magic[10] = '\0';
    ASSERT_STREQ(magic, "GORGONZOLA");
    uint64_t actualVersion;
    dbFile.read(reinterpret_cast<char*>(&actualVersion), sizeof(actualVersion));
    dbFile.close();
    ASSERT_EQ(storageVersion, actualVersion);
}
