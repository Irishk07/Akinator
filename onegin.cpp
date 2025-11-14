#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "onegin.h"

#include "common.h"


int SizeOfText(const char *text_name) {
    assert(text_name);

    struct stat text_info = {};

    if (stat(text_name, &text_info) == -1) {
        perror("Error is");

        return -1;
    }

    return (int)text_info.st_size;
}

Tree_status ReadOnegin(Akinator* akinator, const char* name_file) {
    assert(akinator != NULL);    

    FILE *text = fopen(name_file, "r");
    if (text == NULL) {
        perror("Error is");
        return OPEN_ERROR;
    }

    int size = SizeOfText(name_file);
    if (size == -1) {
        return STAT_ERROR;
    }
    akinator->size_buffer = (size_t)size;

    akinator->buffer_with_tree = (char*)calloc((akinator->size_buffer + 1), sizeof(char));
    if (akinator->buffer_with_tree == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(MEMORY_ERROR);

    fread((char *)akinator->buffer_with_tree, sizeof(char), akinator->size_buffer, text);
    if (ferror(text) != 0) {
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR,    free(akinator->buffer_with_tree));
    }

    return SUCCESS;
}