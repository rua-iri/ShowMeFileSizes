#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

// function prototypes
int checkCmdArgs(int numArgs);
int checkIsDir(char dirPath[], char fileName[]);
int getDirectoryFiles(char dirPath[], int isFirstLevel);
int getFileSize(char dirPath[], char fileName[]);
struct sizeUnit formatFileSize(int fileSize);

struct sizeUnit
{
    float size;
    char unit;
};

struct sizeUnit formatFileSize(int fileSize)
{
    struct sizeUnit fileSizeUnit;

    // format the filesize depending on whether it is
    // larger than a certain number of bytes
    if (fileSize > 1000000000)
    {
        fileSizeUnit.size = ((float)fileSize / 1000000000);
        fileSizeUnit.unit = 'G';
    }
    else if (fileSize > 1000000)
    {
        fileSizeUnit.size = ((float)fileSize / 1000000);
        fileSizeUnit.unit = 'M';
    }
    else if (fileSize > 1000)
    {
        fileSizeUnit.size = ((float)fileSize / 1000);
        fileSizeUnit.unit = 'k';
    }
    else
    {
        fileSizeUnit.size = (float)fileSize;
        fileSizeUnit.unit = ' ';
    }

    return fileSizeUnit;
};

int checkIsDir(char dirPath[], char fileName[])
{

    struct stat st;

    // concatenate the path to the directory
    int pathLen = strlen(dirPath) + strlen(fileName) + 1;
    char fullPath[pathLen];
    strcpy(fullPath, dirPath);
    strcat(fullPath, "/");
    strcat(fullPath, fileName);

    stat(fullPath, &st);

    return S_ISDIR(st.st_mode);
}

// function to check that user has only provided one argument
int checkCmdArgs(int numArgs)
{
    if (numArgs == 2)
    {
        return 1;
    }
    else
    {
        printf("%i Arguments Provided\t 1 Expected\n", (numArgs - 1));
        return 0;
    }
}

int getDirectoryFiles(char dirPath[], int isFirstLevel)
{

    DIR *directory;
    struct dirent *dir;

    int totalDirSize = 0;
    int fSize;
    int isDir;
    struct sizeUnit fileSizeUnit;
    char filePrefix[] = " B";

    directory = opendir(dirPath);

    if (directory)
    {
        if(isFirstLevel) {
            printf("%6s\t%-6s%s\n\n", "Size", "Unit", "File Name");
        }


        // iterate through each file in the directory
        while ((dir = readdir(directory)) != NULL)
        {
            // check that file is not a hidden file (we will ignore those)
            if (dir->d_name[0] != '.')
            {

                isDir = checkIsDir(dirPath, dir->d_name);

                // TODO if isDir then call this function recursively

                if (isDir) {
                    char fullPath[strlen(dirPath)+strlen(dir->d_name) + 1];
                    strcpy(fullPath, dirPath); strcat(fullPath, "/"); strcat(fullPath, dir->d_name);
                    fSize = getDirectoryFiles(fullPath, 0);
                } else {
                    fSize = getFileSize(dirPath, dir->d_name);
                }

                    totalDirSize += fSize;

                if(isFirstLevel) {
                    fileSizeUnit = formatFileSize(fSize);
                    filePrefix[0] = fileSizeUnit.unit;
                    printf("%5.1f\t%-6s", fileSizeUnit.size, filePrefix);
                    printf("%s\n", dir->d_name);
                }
            }
        }

        if(isFirstLevel) {
            struct sizeUnit totalFileSizeUnit = formatFileSize(totalDirSize);
            filePrefix[0] = totalFileSizeUnit.unit;
            printf("\n%5.1f\t%-6s", totalFileSizeUnit.size, filePrefix);
            printf("Total\n\n");
        }


        closedir(directory);
    }

    return totalDirSize;
}

int getFileSize(char dirPath[], char fileName[])
{
    int fileSize;
    struct stat st;

    // concatenate the path to the directory
    int pathLen = strlen(dirPath) + strlen(fileName) + 1;
    char fullPath[pathLen];
    strcpy(fullPath, dirPath); strcat(fullPath, "/"); strcat(fullPath, fileName);

    stat(fullPath, &st);
    fileSize = st.st_size;

    return fileSize;
}

int main(int argc, char *argv[])
{

    // TODO check if directory exists and do some error handling if it does not

    // TODO maybe call getDirectoryFiles() recursively so that users
    // can see the real size of subdirectories and not 4096
    // but do so only once!!

    // getDirectoryFiles("testdir");

    // ensure that the right arguments have been provided
    if (checkCmdArgs(argc) == 1)
    {
        printf("Files in %s:\n\n", argv[1]);
        getDirectoryFiles(argv[1], 1);
    }

    return 0;
}
