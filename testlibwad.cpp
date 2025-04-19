#include <iostream>
#include <cassert>
#include <algorithm>
#include "libWad/Wad.h"
#include "gtest/gtest.h"

using namespace std;

// ================================= THEIR TESTS ================================= //

const std::string setupWorkspace(){

    const std::string wad_path = "./testfiles/sample1.wad";
    const std::string test_wad_path = "./testfiles/sample1_copy.wad";

    std::string command = "cp " + wad_path + " " + test_wad_path;
    int returnCode = system(command.c_str());

    if(returnCode == EXIT_FAILURE){
            throw("Copy failure");
    }

    return test_wad_path;
}

TEST(LibWriteTests, createDirectoryTest1){
    std::string wad_path = setupWorkspace();
    Wad* testWad = Wad::loadWad(wad_path);

    //createDirectory Test 1, create dir at root
    std::string testPath = "/ex/";

    testWad->createDirectory(testPath);

    std::cout << "[TEST] PathMap contents after createDirectory(\"" << testPath << "\"):" << std::endl;
    for (const auto& [key, val] : testWad->pathMap) {
        std::cout << "  key: \"" << key 
        << "\" | isDir: " << val->isDirectory 
        << " | isMap: " << val->isMap 
        << " | name: " << val->name 
        << std::endl;
    }
    
    ASSERT_TRUE(testWad->isDirectory(testPath));
    ASSERT_FALSE(testWad->isContent(testPath));

    std::vector<std::string> testVector;
    int ret = testWad->getDirectory(testPath, &testVector);
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(testVector.size(), 0);

    testVector.clear();
    ret = testWad->getDirectory("/", &testVector);
    ASSERT_EQ(ret, 4);
    ASSERT_EQ(testVector.size(), 4);

    std::vector<std::string> expectedVector = {
            "E1M0",
            "Gl",
            "mp.txt",
            "ex",
    };

    ASSERT_EQ(expectedVector, testVector);

    //Deleting and reinitiating object
    delete testWad;
    testWad = Wad::loadWad(wad_path);

    ASSERT_TRUE(testWad->isDirectory(testPath));
    ASSERT_FALSE(testWad->isContent(testPath));

    testVector.clear();
    ret = testWad->getDirectory(testPath, &testVector);
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(testVector.size(), 0);

    testVector.clear();
    ret = testWad->getDirectory("/", &testVector);
    ASSERT_EQ(ret, 4);
    ASSERT_EQ(testVector.size(), 4);

    ASSERT_EQ(expectedVector, testVector);

    delete testWad;
}

/*

TEST(LibWriteTests, createDirectoryTest2){
        std::string wad_path = setupWorkspace();
        Wad* testWad = Wad::loadWad(wad_path);

        //createDirectory Test 2, create dir at existing directory
        std::string testPath = "/Gl/ex";

        testWad->createDirectory(testPath);
        
        ASSERT_TRUE(testWad->isDirectory(testPath));
        ASSERT_FALSE(testWad->isContent(testPath));

        std::vector<std::string> testVector;
        int ret = testWad->getDirectory(testPath, &testVector);
        ASSERT_EQ(ret, 0);
        ASSERT_EQ(testVector.size(), 0);

        testVector.clear();
        ret = testWad->getDirectory("/Gl", &testVector);
        ASSERT_EQ(ret, 2);
        ASSERT_EQ(testVector.size(), 2);

        std::vector<std::string> expectedVector = {
                "ad",
                "ex",
        };

        ASSERT_EQ(expectedVector, testVector);

        //Deleting and reinitiating object
        delete testWad;
        testWad = Wad::loadWad(wad_path);

        ASSERT_TRUE(testWad->isDirectory(testPath));
        ASSERT_FALSE(testWad->isContent(testPath));

        testVector.clear();
        ret = testWad->getDirectory(testPath, &testVector);
        ASSERT_EQ(ret, 0);
        ASSERT_EQ(testVector.size(), 0);

        testVector.clear();
        ret = testWad->getDirectory("/Gl", &testVector);
        ASSERT_EQ(ret, 2);
        ASSERT_EQ(testVector.size(), 2);

        ASSERT_EQ(expectedVector, testVector);

        delete testWad;
}

TEST(LibWriteTests, createDirectoryTest3){
        std::string wad_path = setupWorkspace();
        Wad* testWad = Wad::loadWad(wad_path);

        //createDirectory Test 3, create nested directories back to back
        std::string testPath = "/ex/";
        std::string testPath2 = "/ex/am/";

        //Creating dir '/ex/'
        testWad->createDirectory(testPath);
        
        ASSERT_TRUE(testWad->isDirectory(testPath));
        ASSERT_FALSE(testWad->isContent(testPath));

        std::vector<std::string> testVector;
        int ret = testWad->getDirectory(testPath, &testVector);
        ASSERT_EQ(ret, 0);
        ASSERT_EQ(testVector.size(), 0);

        testVector.clear();
        ret = testWad->getDirectory("/", &testVector);
        ASSERT_EQ(ret, 4);
        ASSERT_EQ(testVector.size(), 4);

        std::vector<std::string> expectedVector = {
                "E1M0",
                "Gl",
                "mp.txt",
                "ex",
        };

        ASSERT_EQ(expectedVector, testVector);

        //Creating dir '/ex/am/'
        testWad->createDirectory(testPath2);

        ASSERT_TRUE(testWad->isDirectory(testPath2));
        ASSERT_FALSE(testWad->isContent(testPath2));

        testVector.clear();
        ret = testWad->getDirectory(testPath, &testVector);
        ASSERT_EQ(ret, 1);
        ASSERT_EQ(testVector.size(), 1);

        expectedVector.clear();
        expectedVector = {
                "am"
        };

        ASSERT_EQ(testVector, expectedVector);

        testVector.clear();
        ret = testWad->getDirectory(testPath2, &testVector);
        ASSERT_EQ(ret, 0);
        ASSERT_EQ(testVector.size(), 0);

        //Deleting and reinitiating object
        delete testWad;
        testWad = Wad::loadWad(wad_path);

        ASSERT_TRUE(testWad->isDirectory(testPath));
        ASSERT_FALSE(testWad->isContent(testPath));

        ASSERT_TRUE(testWad->isDirectory(testPath2));
        ASSERT_FALSE(testWad->isContent(testPath2));

        testVector.clear();
        ret = testWad->getDirectory(testPath, &testVector);
        ASSERT_EQ(ret, 1);
        ASSERT_EQ(testVector.size(), 1);

        expectedVector.clear();
        expectedVector = {
                "am"
        };

        ASSERT_EQ(testVector, expectedVector);

        testVector.clear();
        ret = testWad->getDirectory(testPath2, &testVector);
        ASSERT_EQ(ret, 0);
        ASSERT_EQ(testVector.size(), 0);

        delete testWad;
}

TEST(LibWriteTests, createFileTest1){
    std::string wad_path = setupWorkspace();
    Wad* testWad = Wad::loadWad(wad_path);

    //createFile Test 1, creating file in root
    std::string testPath = "/file.txt";

    testWad->createFile(testPath);

    ASSERT_TRUE(testWad->isContent(testPath));
    ASSERT_FALSE(testWad->isDirectory(testPath));

    std::vector<std::string> testVector;
    int ret = testWad->getDirectory("/", &testVector);
    ASSERT_EQ(ret, 4);
    ASSERT_EQ(testVector.size(), 4);

    std::vector<std::string> expectedVector = {
            "E1M0",
            "Gl",
            "mp.txt",
            "file.txt"
    };

    ASSERT_EQ(expectedVector, testVector);

    //Reinstantiating object and rerunning tests
    delete testWad;
    testWad = Wad::loadWad(wad_path);

    ASSERT_TRUE(testWad->isContent(testPath));
    ASSERT_FALSE(testWad->isDirectory(testPath));

    testVector.clear();
    ret = testWad->getDirectory("/", &testVector);
    ASSERT_EQ(ret, 4);
    ASSERT_EQ(testVector.size(), 4);

    ASSERT_EQ(expectedVector, testVector);

    delete testWad;
}

TEST(LibWriteTests, createFileTest2){
    std::string wad_path = setupWorkspace();
    Wad* testWad = Wad::loadWad(wad_path);

    //createFile Test 2, creating file in directory
    std::string testPath = "/Gl/ad/example";

    testWad->createFile(testPath);

    std::vector<std::string> testVector;
    int ret = testWad->getDirectory("/Gl/ad/", &testVector);

    ASSERT_TRUE(testWad->isContent(testPath));
    ASSERT_FALSE(testWad->isDirectory(testPath));

    ASSERT_EQ(ret, 2);
    ASSERT_EQ(testVector.size(), 2);

    std::vector<std::string> expectedVector = {
            "os",
            "example"
    };

    ASSERT_EQ(expectedVector, testVector);

    //Reinstantiating object and rerunning tests
    delete testWad;
    testWad = Wad::loadWad(wad_path);

    ASSERT_TRUE(testWad->isContent(testPath));
    ASSERT_FALSE(testWad->isDirectory(testPath));

    testVector.clear();
    ret = testWad->getDirectory("/Gl/ad/", &testVector);
    ASSERT_EQ(ret, 2);
    ASSERT_EQ(testVector.size(), 2);

    ASSERT_EQ(expectedVector, testVector);

    delete testWad;
}

TEST(LibWriteTests, writeToFileTest1){

    std::string wad_path = setupWorkspace();
    Wad* testWad = Wad::loadWad(wad_path);

    //writeToFile Test 1, creating and writing to a text file
    std::string testPath = "/file.txt";

    //testing file creation

    testWad->createFile(testPath);

    ASSERT_TRUE(testWad->isContent(testPath));
    ASSERT_FALSE(testWad->isDirectory(testPath));

    std::vector<std::string> testVector;
    int ret = testWad->getDirectory("/", &testVector);
    ASSERT_EQ(ret, 4);
    ASSERT_EQ(testVector.size(), 4);

    std::vector<std::string> expectedVector = {
            "E1M0",
            "Gl",
            "mp.txt",
            "file.txt"
    };

    ASSERT_EQ(expectedVector, testVector);

    //testing file writing
    const char expectedFileContents[] = "Hello! This is a test to make sure I can write to files.\n";
    int expectedSizeOfFile = 57;

    ret = testWad->writeToFile(testPath, expectedFileContents, expectedSizeOfFile);
    ASSERT_EQ(ret, expectedSizeOfFile);
    ASSERT_EQ(testWad->getSize(testPath), expectedSizeOfFile);

    char buffer[100];
    memset(buffer, 0, 100);
    ret = testWad->getContents(testPath, buffer, expectedSizeOfFile);
    ASSERT_EQ(ret, expectedSizeOfFile);
    ASSERT_EQ(memcmp(buffer, expectedFileContents, expectedSizeOfFile), 0)
    << "Expected string: " << buffer
    << "Returned string: " << expectedFileContents;        

    //Reinstantiating object and rerunning tests
    delete testWad;
    testWad = Wad::loadWad(wad_path);

    ASSERT_EQ(testWad->getSize(testPath), expectedSizeOfFile);

    memset(buffer, 0, 100);
    ret = testWad->getContents(testPath, buffer, expectedSizeOfFile);
    ASSERT_EQ(ret, expectedSizeOfFile);
    ASSERT_EQ(memcmp(buffer, expectedFileContents, expectedSizeOfFile), 0)
    << "Expected string: " << buffer
    << "Returned string: " << expectedFileContents;

    //Attemping to write to the file again

    ret = testWad->writeToFile(testPath, expectedFileContents, expectedSizeOfFile);
    //path is technically valid, but the file already exists.
    //function should return 0, to signify that the path is valid
    //but the the write should still fail.
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(testWad->getSize(testPath), expectedSizeOfFile);

    memset(buffer, 0, 100);
    ret = testWad->getContents(testPath, buffer, expectedSizeOfFile);
    ASSERT_EQ(ret, expectedSizeOfFile);
    ASSERT_EQ(memcmp(buffer, expectedFileContents, expectedSizeOfFile), 0)
    << "Expected string: " << buffer
    << "Returned string: " << expectedFileContents;

    delete testWad;
}

TEST(LibFunctionalityTests, bigTest){
    std::string wad_path = setupWorkspace();
    Wad* testWad = Wad::loadWad(wad_path);

    //Creating dir '/Ex'
    testWad->createDirectory("/Ex");
    std::vector<std::string> expectedVector = {"E1M0","Gl","mp.txt","Ex"};
    ASSERT_TRUE(testWad->isDirectory("/Ex"));
    std::vector<std::string> testVector;
    int ret = testWad->getDirectory("/", &testVector);
    ASSERT_EQ(ret, 4);
    ASSERT_EQ(testVector.size(), 4);
    ASSERT_EQ(testVector, expectedVector);

    //Creating dir '/Ex/ad/'
    testWad->createDirectory("/Ex/ad/");
    ASSERT_TRUE(testWad->isDirectory("/Ex/ad/"));
    testVector.clear();
    expectedVector.clear();
    expectedVector = {"ad"};
    ret = testWad->getDirectory("/Ex", &testVector);
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(testVector.size(), 1);
    ASSERT_EQ(testVector, expectedVector);

    //Creating and writing to '/Ex/ad/test.txt'
    testWad->createFile("/Ex/ad/test.txt");
    ASSERT_TRUE(testWad->isContent("/Ex/ad/test.txt"));
    testVector.clear();
    expectedVector.clear();
    expectedVector = {"test.txt"};
    ret = testWad->getDirectory("/Ex/ad/", &testVector);
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(testVector.size(), 1);
    ASSERT_EQ(testVector, expectedVector);

    const char inputText[] =
            "This text is pretty long, but will be "
            "concatenated into just a single string. "
            "The disadvantage is that you have to quote "
            "each part, and newlines must be literal as "
            "usual.";

    //Removing one from size to exclude null term
    int inputSize = 170;

    ret = testWad->writeToFile("/Ex/ad/test.txt", inputText, inputSize);
    ASSERT_EQ(ret, inputSize);

    char buffer[200];
    memset(buffer, 0 , 200);
    ret = testWad->getContents("/Ex/ad/test.txt", buffer, inputSize);
    ASSERT_EQ(ret, inputSize);
    ASSERT_EQ(testWad->getSize("/Ex/ad/test.txt"), inputSize);
    ASSERT_EQ(memcmp(buffer, inputText, inputSize), 0);

    //Creating dir '/Ex/ad/os/'
    testWad->createDirectory("/Ex/ad/os");
    ASSERT_TRUE(testWad->isDirectory("/Ex/ad/os"));
    testVector.clear();
    expectedVector.clear();
    expectedVector = {"test.txt", "os"};
    ret = testWad->getDirectory("/Ex/ad", &testVector);
    ASSERT_EQ(ret, 2);
    ASSERT_EQ(testVector.size(), 2);
    ASSERT_EQ(testVector, expectedVector);

    //Creating and writing to '/Ex/ad/os/test.txt'
    testWad->createFile("/Ex/ad/os/test.txt");
    ASSERT_TRUE(testWad->isContent("/Ex/ad/os/test.txt"));
    testVector.clear();
    expectedVector.clear();
    expectedVector = {"test.txt"};
    ret = testWad->getDirectory("/Ex/ad/os", &testVector);
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(testVector.size(), 1);
    ASSERT_EQ(testVector, expectedVector);

    ret = testWad->writeToFile("/Ex/ad/os/test.txt", inputText, inputSize);
    ASSERT_EQ(ret, inputSize);

    memset(buffer, 0 , 200);
    ret = testWad->getContents("/Ex/ad/os/test.txt", buffer, inputSize);
    ASSERT_EQ(ret, inputSize);
    ASSERT_EQ(testWad->getSize("/Ex/ad/os/test.txt"), inputSize);
    ASSERT_EQ(memcmp(buffer, inputText, inputSize), 0);


    //Deleting and reinstantiating object
    delete testWad;      
    testWad = Wad::loadWad(wad_path);

    testVector.clear();
    expectedVector.clear();
    expectedVector = {"ad"};
    ret = testWad->getDirectory("/Ex/", &testVector);
    ASSERT_TRUE(testWad->isDirectory("/Ex/"));
    ASSERT_TRUE(testWad->isDirectory("/Ex/ad"));
    ASSERT_FALSE(testWad->isContent("/Ex/test.txt"));
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(testVector.size(), 1);
    ASSERT_EQ(testVector, expectedVector);

    testVector.clear();
    expectedVector.clear();
    expectedVector = {"test.txt", "os"};
    ret = testWad->getDirectory("/Ex/ad", &testVector);
    ASSERT_EQ(ret, 2);
    ASSERT_EQ(testVector.size(), 2);
    ASSERT_EQ(testVector, expectedVector);

    testVector.clear();
    expectedVector.clear();
    expectedVector = {"test.txt"};
    ret = testWad->getDirectory("/Ex/ad/os", &testVector);
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(testVector.size(), 1);
    ASSERT_EQ(testVector, expectedVector);

    ASSERT_TRUE(testWad->isContent("/Ex/ad/test.txt"));
    ASSERT_EQ(testWad->getSize("/Ex/ad/test.txt"), inputSize);
    memset(buffer, 0 , 200);
    ret = testWad->getContents("/Ex/ad/test.txt", buffer, inputSize);
    ASSERT_EQ(ret, inputSize);
    ASSERT_EQ(memcmp(buffer, inputText, inputSize), 0);

    ASSERT_TRUE(testWad->isContent("/Ex/ad/os/test.txt"));
    ASSERT_EQ(testWad->getSize("/Ex/ad/os/test.txt"), inputSize);
    memset(buffer, 0 , 200);
    ret = testWad->getContents("/Ex/ad/os/test.txt", buffer, inputSize);
    ASSERT_EQ(ret, inputSize);
    ASSERT_EQ(memcmp(buffer, inputText, inputSize), 0);

    delete testWad;
}

*/









// ================================= MY TESTS ================================= //

// ==== UNIT TESTS ==== //

void unitTestContentNDirs(Wad* wad, const string& path, bool expectContent, bool expectDirectory) {
    bool actualContent = wad->isContent(path);
    bool actualDirectory = wad->isDirectory(path);
    
    assert(actualContent == expectContent);
    assert(actualDirectory == expectDirectory);
}

void unitTestGetSize(Wad* wad) {
    
    assert(wad->getSize("/E1M0/01.txt") == 17);
    assert(wad->getSize("/Gl/ad/os/cake.jpg") == 29869);
    assert(wad->getSize("/fake/path") == -1);
    assert(wad->getSize("/") == -1);
}

void unitTestGetContents(Wad* wad) {
    char buffer[100] = {};

    // Read and check bytes
    int bytes = wad->getContents("/E1M0/01.txt", buffer, 100);
    assert(bytes == 17);

    // Edge: invalid path
    char badBuffer[20] = {};
    int result = wad->getContents("/not/real/path", badBuffer, 20);
    assert(result == -1);

    // Edge: directory path
    result = wad->getContents("/Gl", badBuffer, 20);
    assert(result == -1);
}

void unitTestGetDirectory(Wad* wad) {
    vector<string> entries;

    // Test root directory
    entries.clear();
    int count = wad->getDirectory("/", &entries);
    assert(count == 3);  // E1M0, Gl, mp.txt
    assert(find(entries.begin(), entries.end(), string("E1M0")) != entries.end());
    assert(find(entries.begin(), entries.end(), string("Gl")) != entries.end());
    assert(find(entries.begin(), entries.end(), string("mp.txt")) != entries.end());

    // Test map marker directory
    entries.clear();
    count = wad->getDirectory("/E1M0", &entries);
    assert(count == 10);
    assert(find(entries.begin(), entries.end(), string("01.txt")) != entries.end());

    // Test nested namespace
    entries.clear();
    count = wad->getDirectory("/Gl", &entries);
    assert(count == 1);
    assert(entries[0] == "ad");

    // Invalid: path is content
    entries.clear();
    count = wad->getDirectory("/E1M0/01.txt", &entries);
    assert(count == -1);

    // Invalid: path doesn't exist
    entries.clear();
    count = wad->getDirectory("/not/real/path", &entries);
    assert(count == -1);
}

void unitTestCreateDirectory(Wad* wad) {
    // Should succeed
    wad->createDirectory("/Gl/ad/xy");
    assert(wad->isDirectory("/Gl/ad/xy"));

    vector<string> entries;
    wad->getDirectory("/Gl/ad", &entries);
    assert(find(entries.begin(), entries.end(), string("xy")) != entries.end());

    // Invalid: name too long
    wad->createDirectory("/Gl/ad/thisnameistoolong");
    assert(!wad->isDirectory("/Gl/ad/thisnameistoolong"));

    // Invalid: parent doesn't exist
    wad->createDirectory("/nonexistent/xy");
    assert(!wad->isDirectory("/nonexistent/xy"));

    // Invalid: parent is a map marker
    wad->createDirectory("/E1M0/ab");
    assert(!wad->isDirectory("/E1M0/ab"));
}

void unitTestCreateFile(Wad* wad) {
    // Valid creation
    wad->createFile("/Gl/ad/banana");
    assert(wad->isContent("/Gl/ad/banana"));
    assert(!wad->isDirectory("/Gl/ad/banana"));

    vector<string> entries;
    wad->getDirectory("/Gl/ad", &entries);
    assert(find(entries.begin(), entries.end(), string("banana")) != entries.end());

    // Invalid: name too long
    wad->createFile("/Gl/ad/superlongfilename");
    assert(!wad->isContent("/Gl/ad/superlongfilename"));

    // Invalid: parent doesn't exist
    wad->createFile("/notarealparent/ghost");
    assert(!wad->isContent("/notarealparent/ghost"));

    // Invalid: map-style name (E1M1)
    wad->createFile("/Gl/ad/E2M2");
    assert(!wad->isContent("/Gl/ad/E2M2"));

    // Invalid: parent is a map marker
    wad->createFile("/E1M0/newfile");
    assert(!wad->isContent("/E1M0/newfile"));
}

void unitTestWriteToFile(Wad* wad) {
    const char* message = "hello";
    int length = 5;

    // Write to a newly created file
    int written = wad->writeToFile("/Gl/ad/banana", message, length);
    assert(written == 5);

    // Confirm size updated
    assert(wad->getSize("/Gl/ad/banana") == 5);

    // Confirm content matches
    char buffer[10] = {};
    int read = wad->getContents("/Gl/ad/banana", buffer, 10);
    assert(read == 5);
    assert(string(buffer, read) == "hello");

    // Try writing again (should fail because it's not empty anymore)
    written = wad->writeToFile("/Gl/ad/banana", "test", 4);
    assert(written == 0);

    // Try writing to a directory
    written = wad->writeToFile("/Gl/ad", "fail", 4);
    assert(written == -1);

    // Try writing to a nonexistent path
    written = wad->writeToFile("/not/real/path", "fail", 4);
    assert(written == -1);
}


// ==== HELPER TESTS ==== //

Wad* setwad(const string &path) {
    Wad* wad = Wad::loadWad("libWad/P3Files/sample1.wad");
    if (!wad) {
        cerr << "Failed to load WAD." << endl;
        return nullptr;
    }
    return wad;
}

int testContentNDirs(Wad* wad) {
    cout << "Testing isContent & isDirectory:" << endl;

    // Root
    unitTestContentNDirs(wad, "/", false, true);

    // Namespace directories
    unitTestContentNDirs(wad, "/E1M0", false, true);
    unitTestContentNDirs(wad, "/E1M0/01.txt", true, false);
    unitTestContentNDirs(wad, "/E1M0/02.txt", true, false);
    unitTestContentNDirs(wad, "/E1M0/03.txt", true, false);
    unitTestContentNDirs(wad, "/E1M0/04.txt", true, false);
    unitTestContentNDirs(wad, "/E1M0/05.txt", true, false);
    unitTestContentNDirs(wad, "/E1M0/06.txt", true, false);
    unitTestContentNDirs(wad, "/E1M0/07.txt", true, false);
    unitTestContentNDirs(wad, "/E1M0/08.txt", true, false);
    unitTestContentNDirs(wad, "/E1M0/09.txt", true, false);
    unitTestContentNDirs(wad, "/E1M0/10.txt", true, false);

    // Name Space dir
    unitTestContentNDirs(wad, "/Gl", false, true);

    // File inside Gl
    unitTestContentNDirs(wad, "/Gl/ad", false, true);

    // File inside ad
    unitTestContentNDirs(wad, "/Gl/ad/os", false, true);

    // File inside os
    unitTestContentNDirs(wad, "/Gl/ad/os/cake.jpg", true, false);

    // root file
    unitTestContentNDirs(wad, "/mp.txt", true, false);

    // Made-up path
    unitTestContentNDirs(wad, "/fake/path/ghost", false, false);

    cout << "Passed!" << endl;
    return 1;
}

int testGetSize(Wad* wad) {

    cout << "Testing getSize:" << endl;
    unitTestGetSize(wad);
    cout << "Passed!" << endl;
    return 1;
}

int testGetContents(Wad* wad) {
    cout << "Testing getContents:" << endl;
    unitTestGetContents(wad);
    cout << "Passed!" << endl;
    return 1;
}

int testGetDirectory(Wad* wad) {
    cout << "Testing getDirectory:" << endl;
    unitTestGetDirectory(wad);
    cout << "Passed!" << endl;
    return 1;
}

int testCreateDirectory(Wad* wad) {
    cout << "Testing createDirectory:" << endl;
    unitTestCreateDirectory(wad);
    cout << "Passed!" << endl;
    return 1;
}

int testCreateFile(Wad* wad) {
    cout << "Testing createFile:" << endl;
    unitTestCreateFile(wad);
    cout << "Passed!" << endl;
    return 1;
}

int testWriteToFile(Wad* wad) {
    cout << "Testing writeToFile:" << endl;
    unitTestWriteToFile(wad);
    cout << "Passed!" << endl;
    return 1;
}

int testAllFunctions(Wad* wad) {
    cout << "Testing all major functions together:" << endl;

    // Step 1: Create new namespace directory
    wad->createDirectory("/Gl/ad/ts");
    assert(wad->isDirectory("/Gl/ad/ts"));
    assert(!wad->isContent("/Gl/ad/ts"));

    // Step 2: Create new file inside that directory
    wad->createFile("/Gl/ad/ts/note.txt");
    assert(wad->isContent("/Gl/ad/ts/note.txt"));
    assert(!wad->isDirectory("/Gl/ad/ts/note.txt"));

    // Step 3: Write to file
    const char* msg = "WAD file test!";
    int written = wad->writeToFile("/Gl/ad/ts/note.txt", msg, 14);
    assert(written == 14);
    assert(wad->getSize("/Gl/ad/ts/note.txt") == 14);

    // Step 4: Read back and confirm contents
    char buffer[20] = {};
    int read = wad->getContents("/Gl/ad/ts/note.txt", buffer, 20);
    assert(read == 14);
    assert(string(buffer, read) == "WAD file test!");

    // Step 5: Check directory listing
    vector<string> entries;
    int count = wad->getDirectory("/Gl/ad/ts", &entries);
    assert(count == 1);
    assert(find(entries.begin(), entries.end(), string("note.txt")) != entries.end());

    // Step 6: Negative case: writing again should fail
    int retry = wad->writeToFile("/Gl/ad/ts/note.txt", "overwrite", 9);
    assert(retry == 0);

    cout << "Passed!" << endl;
    return 1;
}

void myTests() {

    std::string wad_path = setupWorkspace();
    Wad* wad = Wad::loadWad(wad_path);

    int score = 0;
    int tests = 0;
    score = testContentNDirs(wad);
    tests++;
    score += testGetSize(wad);
    tests++;
    score += testGetContents(wad);
    tests++;
    score += testGetDirectory(wad);
    tests++;
    score += testCreateDirectory(wad);
    tests++;
    score += testCreateFile(wad);
    tests++;
    score += testWriteToFile(wad);
    tests++;
    score += testAllFunctions(wad);
    tests++;
    cout << "Overall Test Score: " << score << "/" << tests << endl;
    delete wad;
}

/*
int main() {
    
//myTests();

}
*/
