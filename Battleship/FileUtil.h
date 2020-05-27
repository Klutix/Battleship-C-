//
// Created by kiren on 4/4/2020.
//

#ifndef BATTLESHIPOOP_FILEUTIL_H
#define BATTLESHIPOOP_FILEUTIL_H


#include <fstream>
#include <vector>
#include <stdio.h>
#include <cstring>      //strtok
#include <unistd.h>
#include <dirent.h>     //reading files
#include <iostream>
#include <algorithm>

class FileUtil {
public:
    static void SelectFileOfTypeFromCurrentDirectory(std::ifstream &file, std::string extention,std::string message, int setw , std::string fill);
    static std::vector<std::string> GetFilesOfTypeFromDirectory(const char *path, std::string extention);
    static void DisplayAllFilesWithIndexChoice(std::vector<std::string> files, std::string message = "", int setw = 0, std::string fill="");
    static void PrintFile(std::string FilePath);
private:
    FileUtil();
};




#endif //BATTLESHIPOOP_FILEUTIL_H
