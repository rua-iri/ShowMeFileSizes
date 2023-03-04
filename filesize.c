#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

// function prototypes
int checkCmdArgs(int numArgs);
void getDirectoryFiles(char dirPath[]);
struct sizeUnit getFileSize(char dirPath[], char fileName[]);

struct sizeUnit {
    float size;
    char unit;
};



int checkCmdArgs(int numArgs)
{

    if (numArgs == 2)
    {
        return 1;
    }
    else
    {
        printf("%i Arguments Provided\t 1 Expected\n", (numArgs-1));
        return 0;
    }
}



void getDirectoryFiles(char dirPath[])
{

    DIR *directory;
    struct dirent *dir;
    struct sizeUnit fileSizeUnit;
    char filePrefix[] = " B";

    directory = opendir(dirPath);

    if (directory)
    {

        printf("%6s\t%-6s%s\n\n", "Size", "Unit", "File Name");

        // iterate through each file in the directory
        while ((dir = readdir(directory)) != NULL)
        {
            // check that file is not a hidden file
            if (dir->d_name[0] != '.')
            {
                fileSizeUnit = getFileSize(dirPath, dir->d_name);
                filePrefix[0] = fileSizeUnit.unit;

                printf("%5.1f\t%-6s", fileSizeUnit.size, filePrefix);
                printf("%s\n", dir->d_name);
            }
        }

        closedir(directory);
    }
}




struct sizeUnit getFileSize(char dirPath[], char fileName[])
{
    int fileSize;
    struct stat st;

    // concatenate the path to the
    int pathLen = strlen(dirPath) + strlen(fileName) + 1;
    char fullPath[pathLen];
    strcpy(fullPath, dirPath);
    strcat(fullPath, "/");
    strcat(fullPath, fileName);

    stat(fullPath, &st);
    fileSize = st.st_size;

    struct sizeUnit fileSizeUnit;


    // format the filesize depending on whether it is 
    // larger than a certain number of bytes
    if (fileSize>1000000000) {
        fileSizeUnit.size = ((float) fileSize / 1000000000);
        fileSizeUnit.unit = 'G';
    } else if (fileSize>1000000) {
        fileSizeUnit.size = ((float) fileSize / 1000000);
        fileSizeUnit.unit = 'M';
    } else if (fileSize>1000) {
        fileSizeUnit.size = ((float) fileSize / 1000);
        fileSizeUnit.unit = 'k';
    } else {
        fileSizeUnit.size = (float) fileSize;
        fileSizeUnit.unit = ' ';
    }


    return fileSizeUnit;
}



int main(int argc, char *argv[])
{

    // TODO add the value for the size of each file to a total value
    // stored as a long type variable b/c it supports values up to 1x10^18

    // TODO check if directory exists and do some error handling if it does not

    // TODO maybe call getDirectoryFiles() recursively once so that users 
    // can see the real size of subdirectories and not 4096
    
    
    // getDirectoryFiles("testdir");

    // ensure that the right arguments have been provided
    if (checkCmdArgs(argc)==1) {
        printf("Files in %s:\n\n", argv[1]);
        getDirectoryFiles(argv[1]);
    }




    return 0;
}
