#include <sys/stat.h>
#include <stdio.h>

// function prototypes
int getFileSize(char[]);


int getFileSize(char fileName[])
{
    int size;
    struct stat st;

    stat(fileName, &st);
    size = st.st_size;

    return size;
}



void main()
{

    // TODO get list of all files in given directory
    //
    // TODO add the value for the size of each file to a total value
    // stored as a long type variable b/c it supports values up to 1x10^18

    char fileName[] = "test.txt";
    int size = getFileSize(fileName);


    printf("File Name: %s\n", fileName);
    printf("File Size: %d\n", size);
}
