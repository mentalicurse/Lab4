#include "../include/FileSearcher.h"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>

std::vector<std::string> FileSearcher::GetAllFiles(const std::string& folderPath) {
    std::vector<std::string> files;
    DIR* dir = opendir(folderPath.c_str());
    
    if (!dir) {
        perror(("Error! Not opening directory: " + folderPath).c_str());
        return files;
    }

    dirent* entry;
    while ((entry = readdir(dir))) {
        std::string name = entry->d_name;
        if (name == "." || name == "..") continue;
        
        std::string fullPath = folderPath + "/" + name;
        
        struct stat fileInfo;
        if (stat(fullPath.c_str(), &fileInfo) != 0) {
            std::cerr << "Error accessing: " << fullPath << std::endl;
            continue;
        }

        if (S_ISDIR(fileInfo.st_mode)) {
            auto subFiles = GetAllFiles(fullPath);
            files.insert(files.end(), subFiles.begin(), subFiles.end());
        } else {
            files.push_back(fullPath);
        }
    }
    closedir(dir);
    return files;
}
