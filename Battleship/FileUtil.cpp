//
// Created by kiren on 4/4/2020.
//

#include "FileUtil.h"
#include "FileUtil.h"
#include <iomanip>

using namespace std;

void FileUtil::PrintFile(string FilePath) {
    ifstream file(FilePath);
    string line;
    if(file.is_open()){
        while(!file.eof()){
            getline(file, line);
            cout << line << endl;
        }
    }
    file.close();
}

void FileUtil::SelectFileOfTypeFromCurrentDirectory(ifstream &file, string extention, string message, int setw , string fill) {
    string exceptionMessage; //all exceptions will result in message
    /*Get current directory*/
    char * cd = getcwd(nullptr,0);
    /*Get all CSV files in current directory*/
    auto files = GetFilesOfTypeFromDirectory(cd, "csv");
    /*if NO files are available throw error and end program*/
    if(files.empty()) {
        exceptionMessage = "No " + extention + " in current directory. Add files to working directory and restart application";
        throw exceptionMessage;
    }
    /*select file if error return here or exit*/
    selectCSV:
    try {
        DisplayAllFilesWithIndexChoice(files, message,setw, fill);
        std::cout << endl << std::setw(setw) << fill << message;
        int choice = 0;
        while(!(cin >> choice)){
            cin.clear();
            cin.ignore(123,'\n');
            exceptionMessage = "Choice does not represent a file\nSelect another?  (y/n)";
            throw exceptionMessage;
        }
        if( choice < 0 || choice > files.size()){
            exceptionMessage = "Choice does not represent a file\nSelect another?  (y/n)";
            throw exceptionMessage;
        }

        std::sort(files.begin(),files.end());
        file.open(files[choice-1]);
        if(!file.is_open()){
            exceptionMessage = "Failed to Open File\nSelect another?  (y/n)";
            throw exceptionMessage;
        }

        /*if we have reached this point file is good and player ships populated*/
        /*now populate enemy ships*/
    }catch (string &e){
        cout << std::setw(setw) << fill << e << endl;
        string choice;
        cin >> choice;
        if(choice == "y"){
            goto selectCSV;
        } else if (choice  == "n"){
            cout << "exiting program...";
            exit(1);
        }else{
            exit(1);
        }
    }
}

vector<string> FileUtil::GetFilesOfTypeFromDirectory(const char *path, string extention) {
    vector<string> fileList;
    DIR *dir;
    dirent *ent;
    if ((dir = opendir (path)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            //build file with file path
            std::string file = path;
            file+= + "\\";
            file+=ent->d_name;
            if(file.length() > extention.size() && file.substr(file.length()-extention.size()) == extention){
                //found a csv push on the list
                fileList.push_back(file);
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("Failed to open Directory");
        return fileList;
    }
}

void FileUtil::DisplayAllFilesWithIndexChoice(vector<string> files, string message, int setw , string fill) {
    int choiceOption = 1;
    if(files.size() == 0){
        cout  << message;
    }

    for (int i; i < files.size();++i) {
        cout << std::setw(setw) << "" << i + 1 << ") " << files[i].substr(files[i].find_last_of('\\')) << endl;
    }
}
