#ifndef FILE_SEARCHER_H
#define FILE_SEARCHER_H

#include <vector>
#include <string>

class FileSearcher {
public:
    static std::vector<std::string> GetAllFiles(const std::string& folderPath);
    
};

#endif // FILE_SEARCHER_H
