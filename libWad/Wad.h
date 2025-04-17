// WAD_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

struct WadNode {
    string name;                // Lump name (e.g., "LOLWUT", "E1M1", "F1_START")
    bool isDirectory = false;
    bool isMap = false;
    int offset = 0;             // File offset
    int size = 0;               // Lump size

    WadNode* parent = nullptr;
    vector<WadNode*> children;
    string fullPath;            // pwd

    WadNode(string n, bool isDir = false, bool isMapMarker = false)
        : name(n), isDirectory(isDir), isMap(isMapMarker) {}
};

class Wad {

    // WAD header values (_magic 4 bytes : _content 4 bytes : _offset 4 bytes)
    int _magic; 
    int _content;
    int _offset;
    string _magicString;
    WadNode* root;
    unordered_map<string, WadNode*> pathMap;
    // some data structure to track lumps
    string wadFilePath;
    
    Wad(const string &path);
    
public:

    // Object allocator; dynamically creates a Wad object and loads the WAD file data from path into memory. 
    // Caller must deallocate the memory using the delete keyword.
    static Wad* loadWad(const string &path);
    
    // Returns the magic for this WAD data.
    string getMagic();
    
    // Returns true if path represents content (data), and false otherwise.
    bool isContent(const string &path);
    
    // Returns true if path represents a directory, and false otherwise.
    bool isDirectory(const string &path);
    
    // If path represents content, returns the number of bytes in its data; otherwise, returns -1.
    int getSize(const string &path);
    
    // If path represents content, copies as many bytes as are available, up to length, of content's data into the pre- existing buffer. 
    // If offset is provided, data should be copied starting from that byte in the content. 
    // Returns number of bytes copied into buffer, or -1 if path does not represent content (e.g., if it represents a directory).
    int getContents(const string &path, char *buffer, int length, int offset = 0); 
    
    // If path represents a directory, places entries for immediately contained elements in directory. 
    // The elements should be placed in the directory in the same order as they are found in the WAD file. Returns the number of elements in the directory, or -1 if path does not represent a directory (e.g., if it represents content).
    int getDirectory(const string &path, vector<string> *directory);

    // path includes the name of the new directory to be created. If given a valid path, creates a new directory using namespace markers at path. 
    // The two new namespace markers will be added just before the “_END” marker of its parent directory. 
    // New directories cannot be created inside map markers.
    void createDirectory(const string &path);

    // path includes the name of the new file to be created. If given a valid path, creates an empty file at path, with an offset and length of 0. 
    // The file will be added to the descriptor list just before the “_END” marker of its parent directory. 
    // New files cannot be created inside map markers.
    void createFile(const string &path);
    
    // If given a valid path to an empty file, augments file size and generates a lump offset, 
    // then writes length amount of bytes from the buffer into the file’s lump data. 
    // If offset is provided, data should be written starting from that byte in the lump content. 
    // Returns number of bytes copied from buffer, or -1 if path does not represent content 
    // (e.g., if it represents a directory).
    int writeToFile(const string &path, const char *buffer, int length, int offset = 0); 

    // NOTE: If a file or directory is created inside the root directory, it will be placed at the very end of the descriptor list, 
    // instead of before an "_END" namespace marker.

};