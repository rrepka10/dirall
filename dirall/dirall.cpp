// dirall.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h>

void listFilesRecursively(char* basePath, int archive);


// 
int main(int argc, char *argv[]) {
    if (argc == 2) {
        listFilesRecursively(argv[1], 0);
    }
    else if ((argc == 3) && (strcmp(argv [2], "-a") == 0)) {
        listFilesRecursively(argv[1], 1);
    }
    else {
        printf("Program to recursively list all the files in a directory, \n");
        printf(" optionally with the archive bit set.  Used for batch scripts.");
        printf("dirall dir [-a]\n");
        printf(" dir - directory to recursively list (required)\n");
        printf(" -a  - optionally, return files with archive bit only\n");
        return(99);
    }
    return 0;
}


void listFilesRecursively(char* basePath, int archive) {
    char path[MAX_PATH];
    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    // buid the search command
    sprintf_s(path, MAX_PATH, "%s\\*", basePath);
    hFind = FindFirstFileA(path, &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    // Process the data returned
    do {
        // Ignore dot directories
        if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {

            // only print files
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // This is a file
                if ((!archive) || (findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)) {
                    // Print the answer, skip Thumbs.db files
                    if (strcmp(findData.cFileName, "Thumbs.db")) {
                        printf("\"%s\\%s\"\n", basePath, findData.cFileName);
                    }
                } // end archive
            } // end directory

            // Is this a directory?
            sprintf_s(path, MAX_PATH, "%s\\%s", basePath, findData.cFileName);
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                listFilesRecursively(path, archive);
            } // End directory
        } // End not dot
    } while (FindNextFileA(hFind, &findData));

    FindClose(hFind);
}

