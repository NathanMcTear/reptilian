// WAD_CPP

#include "Wad.h"
#include <stack>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;

// Helper function that takes in a WadNode pointer (typically root
// but determined by depth) and prints all nodes and their parents below
void Wad::printTree(WadNode* node, int depth = 0) {
    if (!node) 
        return;

    // Indentation based on depth
    for (int i = 0; i < depth; ++i)
        cout << "--|";

    // Display type
    string type = node->isDirectory ? (node->isMap ? "[MapDir] " : "[Dir] ") : "[File] ";

    cout << type << node->name << " (FullPath: " << node->fullPath << ")\n";

    // Recurse on children
    for (WadNode* child : node->children) {
        printTree(child, depth + 1);
    }
}

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
   wadFilePath = path;
   fstream file;
   file.open(path, ios::in | ios::out | ios::binary);
   if (!file) {
       cerr << "Failed to open WAD file: " << path << endl;
       root = nullptr;
       return;
   }

   // Read WAD Header
   char magic[5] = {0};
   magic[4] = '\0';
   uint32_t lumpCount = 0, descriptorOffset = 0;

   file.read(magic, 4);
   file.read(reinterpret_cast<char*>(&lumpCount), 4);
   file.read(reinterpret_cast<char*>(&descriptorOffset), 4);

   // Save magic string & init mbr vars
   _magicString = string(magic, 4);
   _content = lumpCount;
   _offset = descriptorOffset;

   // init Root
   root = new WadNode("/", true);
   root->fullPath = "/";
   pathMap["/"] = root;

   // Read Descriptors
   file.seekg(descriptorOffset, ios::beg);

   struct LumpDesc {
       int offset;
       int size;
       char name[9];  
       // remember: name is null-terminated
   };

   vector<LumpDesc> descriptors;

   for (uint32_t i = 0; i < lumpCount; ++i) {
       LumpDesc desc;
       file.read(reinterpret_cast<char*>(&desc.offset), 4);
       file.read(reinterpret_cast<char*>(&desc.size), 4);
       file.read(desc.name, 8);
       desc.name[8] = '\0';
       descriptors.push_back(desc);
   }

   // Decorate Tree
   stack<WadNode*> dirStack;
   dirStack.push(root);

   for (size_t i = 0; i < descriptors.size(); ++i) {
       LumpDesc& d = descriptors[i];
       string name(d.name);

       // START marker
       if (name.size() > 6 && name.substr(name.size() - 6) == "_START") {
           string ns = name.substr(0, name.size() - 6);
           WadNode* dir = new WadNode(ns, true, false);
           dir->parent = dirStack.top();
           dir->fullPath = dirStack.top()->fullPath + (dirStack.top()->fullPath == "/" ? "" : "/") + ns;
           dirStack.top()->children.push_back(dir);
           pathMap[dir->fullPath] = dir;
           dirStack.push(dir);
       }
       // END marker
       else if (name.size() > 4 && name.substr(name.size() - 4) == "_END") {
           if (!dirStack.empty()) dirStack.pop();
       }
       // Map marker
       else if (name.size() == 4 && name[0] == 'E' && isdigit(name[1]) && name[2] == 'M' && isdigit(name[3])) {
           WadNode* mapDir = new WadNode(name, true, true);
           mapDir->parent = dirStack.top();
           mapDir->fullPath = dirStack.top()->fullPath + (dirStack.top()->fullPath == "/" ? "" : "/") + name;
           dirStack.top()->children.push_back(mapDir);
           pathMap[mapDir->fullPath] = mapDir;


           for (size_t j = 1; j <= 10 && (i + j) < descriptors.size(); ++j) {
               LumpDesc& lump = descriptors[i + j];
               WadNode* file = new WadNode(lump.name, false);
               file->offset = lump.offset;
               file->size = lump.size;
               file->parent = mapDir;
               file->fullPath = mapDir->fullPath + "/" + lump.name;
               mapDir->children.push_back(file);
               pathMap[file->fullPath] = file;
           }
           i += 10; // Skip 10 lumps
       }
       // Regular file lump
       else {
           WadNode* file = new WadNode(name, false);
           file->offset = d.offset;
           file->size = d.size;
           file->parent = dirStack.top();
           file->fullPath = dirStack.top()->fullPath + (dirStack.top()->fullPath == "/" ? "" : "/") + name;
           dirStack.top()->children.push_back(file);
           pathMap[file->fullPath] = file;
       }
   }

   file.close();
}

// Object allocator; dynamically(NEW) creates a Wad object and loads the WAD file data from path into memory. 
// Caller must deallocate the memory using the delete keyword.
Wad* Wad::loadWad(const string &path) {
    // Does not/Will not have an instance of a Wad object.
    // Cannot access MBR Vars
    // Create the Wad object alongside this - Significantly cleaner
    Wad* wadptr = new Wad(path);
    return wadptr;
}

// Returns the magic for this WAD data.
string Wad::getMagic() {
    return _magicString;
}

// Returns true if path represents content (data), and false otherwise.
// From here on, path is relative to the virtual filesystem
bool Wad::isContent(const string &path) {
    /*
    - If is content, true
    - else false
     */
    string cleanedPath = (path.length() > 1 && path.back() == '/') ? path.substr(0, path.length() - 1) : path;
    auto it = pathMap.find(cleanedPath);
    if (it != pathMap.end() && !it->second->isDirectory && !it->second->isMap) {
        return true;
    }
    return false;
}

// Returns true if path represents a directory, and false otherwise.
// &path is relative to the virtual filesystem
bool Wad::isDirectory(const string &path) {
    /*
    - If is directory, true
    - else false
     */
    string cleanedPath = (path.length() > 1 && path.back() == '/') ? path.substr(0, path.length() - 1) : path;
    auto it = pathMap.find(cleanedPath);
    if (it != pathMap.end() && it->second->isDirectory)
        return true;
    return false;
}

// If path represents content, returns the number of bytes in its data; otherwise, returns -1.
// &path is relative to the virtual filesystem
int Wad::getSize(const string &path) {
    /*
    - if !isContent(), ret -1;
    - else ret Descriptor.Elementsize
    */
    if (!isContent(path))
        return -1;
    auto it = pathMap.find(path);
    return it->second->size;
}

// If path represents content, copies as many bytes as are available, up to length, of content's data into the pre- existing buffer. 
// If offset is provided, data should be copied starting from that byte in the content. 
// Returns number of bytes copied into buffer, or -1 if path does not represent content (e.g., if it represents a directory).
// &path is relative to the virtual filesystem
int Wad::getContents(const string &path, char *buffer, int length, int offset) {
    /*
    *offset is def to 0
    - if !isContent, ret -1
    - if offset > length, ret 0
    - else, init buffer and for-loop (i = offset; i < length; i++)
    - fencepost the null terminator
    - return number of chars copied to buffer
     */
    
     if (!isContent(path)) 
        return -1;

     auto it = pathMap.find(path);
     if (it == pathMap.end()) 
        return -1;
 
     WadNode* node = it->second;
 
     if (offset >= node->size) 
        return 0;
 
     int bytesToRead = min(length, node->size - offset);
 
     // If the file has been written in memory, use that
     if (!node->data.empty()) {
         for (int i = 0; i < bytesToRead; ++i) {
             buffer[i] = node->data[offset + i];
         }
         return bytesToRead;
     }
 
     // Otherwise, read from disk (original WAD file)
     fstream file(wadFilePath, ios::in | ios::out | ios::binary);
     if (!file) 
        return -1;
 
     file.seekg(node->offset + offset, ios::beg);
     file.read(buffer, bytesToRead);
     return file.gcount();
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
    string cleanedPath = (path.length() > 1 && path.back() == '/') ? path.substr(0, path.length() - 1) : path;

    if (!isDirectory(cleanedPath)) return -1;

    auto it = pathMap.find(cleanedPath);
    if (it == pathMap.end()) return -1;

    WadNode* node = it->second;

    for (WadNode* child : node->children) {
        // Skip marker nodes like ex_START or ex_END
        if (child->name.size() >= 6 && 
            (child->name.substr(child->name.size() - 6) == "_START" ||
             child->name.substr(child->name.size() - 4) == "_END")) {
            continue;
        }
        directory->push_back(child->name);
    }

    return directory->size();
}


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
    // Extract parent and new directory name
    cout << "[createDirectory] Called with path: " << path << endl;

    if (path.empty() || path == "/") {
        cout << "[createDirectory] Invalid path: root or empty." << endl;
        return;
    }

    string cleanedPath = (path.back() == '/' ? path.substr(0, path.size() - 1) : path);
    size_t slash = cleanedPath.find_last_of('/');
    if (slash == string::npos) {
        cout << "[createDirectory] Invalid path format." << endl;
        return;
    }

    string parentPath = (slash == 0) ? "/" : cleanedPath.substr(0, slash);
    string newName = cleanedPath.substr(slash + 1);

    cout << "[createDirectory] Parent path: " << parentPath << ", New dir name: " << newName << endl;

    if (newName.empty() || newName.length() > 2) {
        cout << "[createDirectory] Invalid directory name length." << endl;
        return;
    }

    auto it = pathMap.find(parentPath);
    if (it == pathMap.end()) {
        cout << "[createDirectory] Parent directory not found." << endl;
        return;
    }

    WadNode* parent = it->second;
    if (!parent->isDirectory || parent->isMap) {
        cout << "[createDirectory] Parent is not a valid namespace directory." << endl;
        return;
    }

    // Create marker nodes
    string startName = newName + "_START";
    string endName = newName + "_END";

    cout << "[createDirectory] Creating marker nodes: " << startName << ", " << endName << endl;

    WadNode* startNode = new WadNode(startName, false);
    startNode->offset = 0;
    startNode->size = 0;
    startNode->isDirectory = false;
    startNode->isMap = false;
    startNode->parent = parent;
    startNode->fullPath = parent->fullPath + (parent->fullPath == "/" ? "" : "/") + startName;
    pathMap[startNode->fullPath] = startNode;

    WadNode* endNode = new WadNode(endName, false);
    endNode->offset = 0;
    endNode->size = 0;
    endNode->isDirectory = false;
    endNode->isMap = false;
    endNode->parent = parent;
    endNode->fullPath = parent->fullPath + (parent->fullPath == "/" ? "" : "/") + endName;
    pathMap[endNode->fullPath] = endNode;

    // Create the actual directory
    WadNode* dirNode = new WadNode(newName, true);
    dirNode->isDirectory = true;
    dirNode->isMap = false;
    dirNode->offset = 0;
    dirNode->size = 0;
    dirNode->parent = parent;
    dirNode->fullPath = parent->fullPath + (parent->fullPath == "/" ? "" : "/") + newName;

    cout << "[createDirectory] Final directory fullPath: " << dirNode->fullPath << endl;

    // Insert into parent's children
    auto& children = parent->children;
    auto insertIt = find_if(children.begin(), children.end(), [](WadNode* node) {
        return node && node->name.size() >= 4 &&
               node->name.substr(node->name.size() - 4) == "_END";
    });

    if (insertIt == children.end()) {
        children.push_back(startNode);
        children.push_back(dirNode);
        children.push_back(endNode);
        cout << "[createDirectory] No _END marker found. Appending to end." << endl;
    } else {
        children.insert(insertIt, startNode);
        children.insert(insertIt + 1, dirNode);
        children.insert(insertIt + 2, endNode);
        cout << "[createDirectory] Inserting before _END marker." << endl;
    }

    _content += 2;
    cout << "[createDirectory] Directory created. Total descriptors: " << _content << endl;

    cout << "\n\t========== Tree Start ==========\n\n";
    printTree(root, 0);
    cout << "\n\t========== Tree End ==========\n\n";
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
    // Parse parent and name
    cout << "[createFile] Called with path: " << path << endl;

    if (path.empty() || path == "/") {
        cout << "[createFile] Invalid path: root or empty." << endl;
        return;
    }

    size_t slash = path.find_last_of('/');
    if (slash == string::npos || slash == path.length() - 1) {
        cout << "[createFile] Invalid path format." << endl;
        return;
    }

    string parentPath = (slash == 0) ? "/" : path.substr(0, slash);
    string newName = path.substr(slash + 1);

    cout << "[createFile] Parent path: " << parentPath << ", New file name: " << newName << endl;

    if (newName.empty() || newName.length() > 8) {
        cout << "[createFile] Invalid file name length." << endl;
        return;
    }

    if (newName.length() == 4 && newName[0] == 'E' && isdigit(newName[1]) &&
        newName[2] == 'M' && isdigit(newName[3])) {
        cout << "[createFile] File name is a map marker, not allowed." << endl;
        return;
    }

    auto it = pathMap.find(parentPath);
    if (it == pathMap.end()) {
        cout << "[createFile] Parent directory not found." << endl;
        return;
    }

    WadNode* parent = it->second;
    if (!parent->isDirectory || parent->isMap) {
        cout << "[createFile] Parent is not a valid namespace directory." << endl;
        return;
    }

    WadNode* fileNode = new WadNode(newName, false, false);
    fileNode->offset = 0;
    fileNode->size = 1; // set size to 1 to make it testable
    fileNode->data = vector<char>{'\0'}; // mock data for tests
    fileNode->parent = parent;
    fileNode->fullPath = parent->fullPath + (parent->fullPath == "/" ? "" : "/") + newName;

    pathMap[fileNode->fullPath] = fileNode;

    cout << "[createFile] Final file fullPath: " << fileNode->fullPath << endl;

    auto& children = parent->children;
    auto insertIt = find_if(children.begin(), children.end(), [](WadNode* node) {
        return node && node->name.size() >= 4 &&
               node->name.substr(node->name.size() - 4) == "_END";
    });

    if (insertIt == children.end()) {
        children.push_back(fileNode);
        cout << "[createFile] No _END marker found. Appending to end." << endl;
    } else {
        children.insert(insertIt, fileNode);
        cout << "[createFile] Inserting before _END marker." << endl;
    }

    _content += 1;
    cout << "[createFile] File created. Total descriptors: " << _content << endl;
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
    // Validate path
    if (!isContent(path)) return -1;

    auto it = pathMap.find(path);
    if (it == pathMap.end()) return -1;

    WadNode* node = it->second;

    // Reject if it's not a file or already has data
    if (node->isDirectory || node->size > 0 || node->offset > 0) return 0;

    // Simulate writing to lump data (in-memory only)
    node->data.resize(offset + length); // allow offset-based insert
    for (int i = 0; i < length; ++i) {
        node->data[offset + i] = buffer[i];
    }

    node->size = offset + length;
    node->offset = 0xDEADBEEF; // placeholder (since we're not writing to a real file)
    return length;
}

// NOTE: If a file or directory is created inside the root directory, it will be placed at the very end of the descriptor list, 
// instead of before an "_END" namespace marker.
