#include <iostream>
#include <cassert>
#include <algorithm>
#include "libWad/Wad.h"

using namespace std;
int testContentNDirs(Wad* wad);
Wad* setwad(const string &path);
int testGetSize(Wad* wad);
int testGetContents(Wad* wad);
int testGetDirectory(Wad* wad);




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





int main() {

    Wad* wad = setwad("libWad/P3Files/sample1.wad");

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
    cout << "Overall Test Score: " << score << "/" << tests << endl;
    delete wad;
}

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