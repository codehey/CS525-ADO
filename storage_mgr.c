#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dberror.h"
#include "storage_mgr.h"



// Define the page size (assumes PAGE_SIZE is defined in dberror.h)
#define PAGE_SIZE 4096

// Define a structure to hold information about open files
typedef struct OpenFile {
    char *fileName;
    int totalNumPages;
    int curPagePos;
    FILE *filePtr;
} OpenFile;

// Global variable to track open files
static OpenFile *openFiles[10];  // Assuming a maximum of 10 open files

// Function to get an open file slot
static OpenFile *getOpenFileSlot() {
    for (int i = 0; i < 10; i++) {
        if (openFiles[i] == NULL) {
            openFiles[i] = malloc(sizeof(OpenFile));
            return openFiles[i];
        }
    }
    return NULL; // No available slots
}

// Function to find an open file by filename
static OpenFile *findOpenFile(const char *fileName) {
    for (int i = 0; i < 10; i++) {
        if (openFiles[i] != NULL && strcmp(openFiles[i]->fileName, fileName) == 0) {
            return openFiles[i];
        }
    }
    return NULL; // File not found
}

// Function to create a page file
RC createPageFile(char *fileName) {
    RC returncode;  // Declare a return code variable
    file = fopen(fileName, "w+");  // Open a file with the given filename in write mode

    // Allocate a memory block with size equal to PAGE_SIZE
    char *memoryBlock = malloc(PAGE_SIZE * sizeof(char));

    if (file == 0) {
        // If the file couldn't be opened, set return code to RC_FILE_NOT_FOUND
        returncode = RC_FILE_NOT_FOUND;
    } else {
        // Initialize the memory block with '\0' characters
        memset(memoryBlock, '\0', PAGE_SIZE);

        // Write the initialized memory block to the file
        fwrite(memoryBlock, sizeof(char), PAGE_SIZE, file);

        // Free the memory block after writing
        free(memoryBlock);

        // Close the file after creating it
        fclose(file);

        // Set return code to indicate success (RC_OK)
        returncode = RC_OK;
    }

    // Return the appropriate return code
    return returncode;
}

// Function to open a page file
RC openPageFile(char *fileName, SM_FileHandle *fHandle) {
    file = fopen(fileName, "r+"); // Open the file with the given filename in read and write mode

    if (file == 0) {
        // If the file couldn't be opened, return RC_FILE_NOT_FOUND
        return RC_FILE_NOT_FOUND;
    } else {
        fseek(file, 0, SEEK_END); // Move the file pointer to the end of the file
        int endByte = ftell(file); // Get the position of the file pointer, which is the last byte of the file
        int totalLength = endByte + 1; // Calculate the total length of the file, including the last byte
        int totalNumPages = totalLength / PAGE_SIZE; // Calculate the total number of pages in the file

        // Initialize the file handle attributes
        (*fHandle).fileName = fileName; // Set the fileName attribute
        (*fHandle).totalNumPages = totalNumPages; // Set the total number of pages attribute
        (*fHandle).curPagePos = 0; // Set the current page position attribute to 0
        rewind(file); // Rewind the file pointer to the start of the file

        // Return RC_OK to indicate success
        return RC_OK;
    }
}



// Function to close a page file
RC closePageFile(SM_FileHandle *fHandle) {
    RC isFileClosed;
    isFileClosed = fclose(file); // fclose closes the file successfully and returns 0
    if (isFileClosed == 0) {
        // If the file is closed successfully, return RC_OK
        return RC_OK;
    } else {
        // If there was an issue closing the file, return RC_FILE_NOT_FOUND
        return RC_FILE_NOT_FOUND;
    }
}

// Function to destroy (delete) a page file
RC destroyPageFile(char *fileName) {
    if (remove(fileName) != 0) {
        // If remove fails to delete the file, return RC_FILE_NOT_FOUND
        return RC_FILE_NOT_FOUND;
    } else {
        // If the file is successfully deleted, return RC_OK
        return RC_OK;
    }
}


