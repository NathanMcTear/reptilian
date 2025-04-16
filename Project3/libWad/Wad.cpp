// WAD_CPP

#include "Wad.h"
using namespace std;


// private Wad constructor, takes in path to a .WAD file from your real filesystem
// Init a fstream object and store as a mbm var. Use to read hdr data
// and construct the n-ary tree. No lump data needed.
Wad::Wad(const string &path) {
    /* 
    - << path from real filesystem
    - Init _myf = fstream(object)
    - build tree *TOKEN-IZE THE FILE STRUCTURE (/F/F1/F2 => "F" -> "F1" -> "F2")
    - No lumps
    */
    root = new WadNode("/", true);
    root->fullPath = "/";
    pathMap["/"] = root;
}

// Object allocator; dynamically(NEW) creates a Wad object and loads the WAD file data from path into memory. 
// Caller must deallocate the memory using the delete keyword.
Wad* Wad::loadWad(const string &path) {
    // Does not/Will not have an instance of a Wad object.
    // Cannot access MBR Vars
    // Create the Wad object alongside this - Significantly cleaner
    Wad* wadptr = new Wad("path");
    return wadptr;
}

// Returns the magic for this WAD data.
string Wad::getMagic() {
    return "Testing the getMagic()!";
}

// Returns true if path represents content (data), and false otherwise.
// From here on, path is relative to the virtual filesystem
bool Wad::isContent(const string &path) {
    /*
    - If is content, true
    - else false
    */
    return 0;
}

// Returns true if path represents a directory, and false otherwise.
// &path is relative to the virtual filesystem
bool Wad::isDirectory(const string &path) {
    /*
    - If is directory, true
    - else false
    */
    return 0;
}

// bool isMarker() {}
// bool isFile() {}


// If path represents content, returns the number of bytes in its data; otherwise, returns -1.
// &path is relative to the virtual filesystem
int Wad::getSize(const string &path) {
    /*
    - if !isContent(), ret -1;
    - else ret Descriptor.Elementsize
    */
   return 0;
}

// If path represents content, copies as many bytes as are available, up to length, of content's data into the pre- existing buffer. 
// If offset is provided, data should be copied starting from that byte in the content. 
// Returns number of bytes copied into buffer, or -1 if path does not represent content (e.g., if it represents a directory).
// &path is relative to the virtual filesystem
int Wad::getContents(const string &path, char *buffer, int length, int offset) {
    int off;
    if (offset != 0)
        off = offset;
    /*
    *offset is def to 0
    - if isContent, ret -1
    - if offset > length, ret 0
    - else, init buffer and for-loop (i = offset; i < length; i++)
    - fencepost the null terminator
    - return number of chars copied to buffer
    */
   return off;
}

// If path represents a directory, places entries for immediately contained elements in directory. 
// The elements should be placed in the directory in the same order as they are found in the WAD file. 
// Returns the number of elements in the directory, or -1 if path does not represent a directory 
// (e.g., if it represents content).
// &path is relative to the virtual filesystem
int Wad::getDirectory(const string &path, vector<string> *directory) {
    /*
    *add the children of the path to directory vector
    *DO NOT SORT
    */
    return 0;
}

// FOR THE NEXT THREE FUNCTIONS:
// make a function that takes in some common parameter
// and creates space in the WAD file based on the needs of the function.

// Will need to searach through decriptor list to find placement.
// path includes the name of the new directory to be created. 
//If given a valid path, creates a new directory using namespace markers at path. 
// The two new namespace markers will be added just before the “_END” marker of its parent directory. 
// New directories cannot be created inside map markers.
// &path is relative to the virtual filesystem
void Wad::createDirectory(const string &path) {
    /*
    *Make space in the WAD file for the '<name>_START' and '<name>_END' descriptors (32 bytes)
    - if newfile is >2, return
    - if parent is !isDir || isMapDir, return
    - if newfile is MapDir, return
    - Make space for '<name>_START' and '<name>_END'
    - ADD to n-ary tree
    - UPDATE # of descriptors(_content) to +2 (32 bytes)
    */
    return;
}

// path includes the name of the new file to be created. If given a valid path, creates an empty file at path, with an offset and length of 0. 
// The file will be added to the descriptor list just before the “_END” marker of its parent directory. 
// New files cannot be created inside map markers.
// &path is relative to the virtual filesystem
void Wad::createFile(const string &path) {
    /*
    *files can only be created in namespace dirs
    - If !isContent() return
    - If parentDir !isDirectory || isMapMarker, return
    - If newfile name is Map/namespace marker || >8 chars, return
    - Create newfile and init offset and length to 0
    - make space for newfile in the WAD file
    - ADD to n-ary tree
    - UPDATE # of descriptors(_content) to +1 (16 bytes)
    */
    return;
}

// If given a valid path to an empty file, augments file size and generates a lump offset, 
// then writes length amount of bytes from the buffer into the file’s lump data. 
// If offset is provided, data should be written starting from that byte in the lump content. 
// Returns number of bytes copied from buffer, or -1 if path does not represent content 
// (e.g., if it represents a directory).
// &path is relative to the virtual filesystem
// DO NOT WORRY ABOUT OFFSET, unless EC (AFTER DAEMON/FUNCTIONAL)
int Wad::writeToFile(const string &path, const char *buffer, int length, int offset) {
    /*
    *Buffer will always be valid, UP TO LENGTH
    - If parentDir !isDirectory || isMapMarker, return -1
    - If !isContent || exists, return -1
    - If isDirectory || isMarker, return -1
    - If hasContents, return 0
    - Read bytes from buffer
    - writes bytes into lump data AT offset
    - UPDATE n-ary tree file's length and offset
    - UPDATE descriptor offset + length(IN BYTES)
    */
    int off;
    if (offset != 0)
        off = offset;
    return off;
}

// NOTE: If a file or directory is created inside the root directory, it will be placed at the very end of the descriptor list, 
// instead of before an "_END" namespace marker.
