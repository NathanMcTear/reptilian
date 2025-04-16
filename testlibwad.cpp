#include <iostream>
#include <cassert>
#include "libWad/Wad.h"

using namespace std;
int testContentNDirs(Wad* wad);
Wad* setwad(const string &path);
int testGetSize(Wad* wad);

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

int main() {

    Wad* wad = setwad("libWad/P3Files/sample1.wad");

    int score = 0;
    int tests = 0;
    score = testContentNDirs(wad);
    tests++;
    score += testGetSize(wad);
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
