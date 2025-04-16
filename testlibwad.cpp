#include <iostream>
#include <cassert>
#include "libWad/Wad.h"

using namespace std;

void test(Wad* wad, const string& path, bool expectContent, bool expectDirectory) {
    bool actualContent = wad->isContent(path);
    bool actualDirectory = wad->isDirectory(path);

    assert(actualContent == expectContent);
    assert(actualDirectory == expectDirectory);
}

int main() {
    cout << "Testing isContent & isDirectory:" << endl;
    Wad* wad = Wad::loadWad("libWad/P3Files/sample1.wad");
    if (!wad) {
        cerr << "Failed to load WAD." << endl;
        return 1;
    }

    // Root
    test(wad, "/", false, true);

    // Namespace directories
    test(wad, "/E1M0", false, true);
    test(wad, "/E1M0/01.txt", true, false);
    test(wad, "/E1M0/02.txt", true, false);
    test(wad, "/E1M0/03.txt", true, false);
    test(wad, "/E1M0/04.txt", true, false);
    test(wad, "/E1M0/05.txt", true, false);
    test(wad, "/E1M0/06.txt", true, false);
    test(wad, "/E1M0/07.txt", true, false);
    test(wad, "/E1M0/08.txt", true, false);
    test(wad, "/E1M0/09.txt", true, false);
    test(wad, "/E1M0/10.txt", true, false);

    // Name Space dir
    test(wad, "/Gl", false, true);

    // File inside Gl
    test(wad, "/Gl/ad", false, true);

    // File inside ad
    test(wad, "/Gl/ad/os", false, true);

    // File inside os
    test(wad, "/Gl/ad/os/cake.jpg", true, false);

    // root file
    test(wad, "/mp.txt", true, false);

    // Made-up path
    test(wad, "/fake/path/ghost", false, false);

    delete wad;
    cout << "All tests passed!" << endl;
    return 0;
}
