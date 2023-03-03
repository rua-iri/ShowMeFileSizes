#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

// function prototypes
int checkCmdArgs(int numArgs);
void getDirectoryFiles(char dirPath[]);
int getFileSize(char dirPath[], char fileName[]);



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

    directory = opendir(dirPath);

    if (directory)
    {
        // iterate through each file in the directory
        while ((dir = readdir(directory)) != NULL)
        {
            // check that file is not a hidden file
            if (dir->d_name[0] != '.')
            {
                printf("%-25s%-12d\n", dir->d_name, getFileSize(dirPath, dir->d_name));
            }
        }

        closedir(directory);
    }
}




int getFileSize(char dirPath[], char fileName[])
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

    return fileSize;
}



int main(int argc, char *argv[])
{

    // TODO add the value for the size of each file to a total value
    // stored as a long type variable b/c it supports values up to 1x10^18

    // TODO check if directory exists and do some error handling if it does not
    
    // getDirectoryFiles("testdir");

    // ensure that the right arguments have been provided
    if (checkCmdArgs(argc)==1) {
        printf("Files in %s:\n\n", argv[1]);
        getDirectoryFiles(argv[1]);
    }




    return 0;
}
