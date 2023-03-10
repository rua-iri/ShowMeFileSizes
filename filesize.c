#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>


// function prototypes
int checkIsDir(char dirPath[], char fileName[]);
long getDirectoryFilesHigher(char dirPath[]);
long getDirectoryFilesLower(char dirPath[]);
int getFileSize(char dirPath[], char fileName[]);
struct sizeUnit formatFileSize(long fileSize);

struct sizeUnit
{
    float size;
    char unit;
};



struct sizeUnit formatFileSize(long fileSize)
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
}



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




long getDirectoryFilesHigher(char dirPath[])
{

    DIR *directory;
    struct dirent *dir;

    long totalDirSize = 0;
    long fSize;
    struct sizeUnit fileSizeUnit;
    char filePrefix[] = " B";

    directory = opendir(dirPath);

    if (directory) {

        printf("%6s\t%-6s%s\n\n", "Size", "Unit", "File Name");

        // iterate through each file in the directory
        while ((dir = readdir(directory)) != NULL)
        {
            // check that file is not a hidden file (we will ignore those)
            if (dir->d_name[0] != '.')
            {

                // call function recursively if the file is a directory
                if (checkIsDir(dirPath, dir->d_name)) {
                    char fullPath[strlen(dirPath)+strlen(dir->d_name) + 1];
                    strcpy(fullPath, dirPath); strcat(fullPath, "/"); strcat(fullPath, dir->d_name);
                    fSize = getDirectoryFilesLower(fullPath);
                    totalDirSize += 4096;
                } else {
                    fSize = getFileSize(dirPath, dir->d_name);
                }

                totalDirSize += fSize;

                fileSizeUnit = formatFileSize(fSize);
                filePrefix[0] = fileSizeUnit.unit;
                printf("%5.1f\t%-6s", fileSizeUnit.size, filePrefix);
                printf("%s\n", dir->d_name);

            }
        }

        closedir(directory);

        struct sizeUnit totalFileSizeUnit = formatFileSize(totalDirSize);
        filePrefix[0] = totalFileSizeUnit.unit;
        printf("\n%5.1f\t%-6s", totalFileSizeUnit.size, filePrefix);
        printf("Total\n\n");

        // TODO there seems to be an overflow for the root directory
        // because ints are not large enough to hold this value
        return 0;
    }

}



long getDirectoryFilesLower(char dirPath[]) {

    DIR *directory;
    struct dirent *dir;
    long totalDirSize = 0;
    long fSize;

    directory = opendir(dirPath);

    if (directory) {
        while ((dir = readdir(directory)) != NULL) {

            // check that file is not a hidden file (we will ignore those)
            if (dir->d_name[0] != '.') {
                // call function recursively if the file is a directory
                if (checkIsDir(dirPath, dir->d_name)) {
                    char fullPath[strlen(dirPath)+strlen(dir->d_name) + 1];
                    strcpy(fullPath, dirPath); strcat(fullPath, "/"); strcat(fullPath, dir->d_name);
                    fSize = getDirectoryFilesLower(fullPath);
                    totalDirSize += 4096;
                } else {
                    fSize = getFileSize(dirPath, dir->d_name);
                }

                totalDirSize += fSize;
            }
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

    // TODO check that argument passed in is a directory

    // ensure that the right arguments have been provided
    if (argc == 2) {
        printf("Files in %s:\n\n", argv[1]);
        getDirectoryFilesHigher(argv[1]);


    } else {
        printf("%i Arguments Provided\t 1 Expected\n", (argc - 1));

    }


    return 0;
}
