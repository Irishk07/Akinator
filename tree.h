#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>

#define TREE_CHECK_AND_RETURN_ERRORS(error, ...)                        \
        {                                                               \
            Tree_status now_error = error;                              \
            if (now_error != SUCCESS) {                                 \
                fprintf(stderr, "Error is: %d, %d\n", error, __LINE__); \
                __VA_ARGS__;                                            \
                return now_error;                                       \
            }                                                           \
        }

#define TREE_DUMP_AND_RETURN_ERRORS(error, ...)                            \
        {                                                                        \
            Tree_status now_error = error;                                       \
            if (now_error != SUCCESS) {                                          \
                TreeHTMLDump(tree, tree->root, DUMP_INFO, ERROR_DUMP, now_error); \
                return now_error;                                                \
            }                                                                    \
        }

#define DUMP_INFO __LINE__, __FILE__
#define NOT_ERROR_DUMP USUAL_DUMP, SUCCESS


typedef char* type_t;
typedef const char* const_type_t;

const int MAX_LEN_NAME = 100;
const int LEN_NIL      = 3;

struct Tree_node {
    type_t info;
    Tree_node* left_node;
    Tree_node* right_node;
    Tree_node* parent;
};

struct Dump_information {
    FILE* dump_file;
    const char* directory;
    int num_dump = 0;
};

struct Tree {
    Tree_node* root;
    size_t size;
    Dump_information dump_info;
};


enum Tree_status {
    SUCCESS                  = 0,
    NOT_ENOUGH_MEMORY        = 1,
    OPEN_ERROR               = 2,
    EXECUTION_FAILED         = 3,
    CLOSE_ERROR              = 4,
    WRONG_SITUATION          = 5,
    READ_ERROR               = 6,
    NULL_POINTER_ON_TREE     = 7,
    WRONG_SIZE               = 8,
    NULL_POINTER_ON_NODE     = 9,
    PARENT_AND_CHILD_UNEQUAL = 10,
    WRONG_ROOT               = 11,
    WRONG_NODE               = 12,
    CHARACTER_NOT_FIND       = 13,
    SYNTAX_ERROR             = 14
};

enum Type_dump {
    USUAL_DUMP = 0,
    ERROR_DUMP = 1
};


Tree_status TreeCtor(Tree* tree, const char* dump_filename, const char* directory);

Tree_status NodeCtor(Tree* tree, Tree_node** new_node, Tree_node* parent);

Tree_status CreateRoot(Tree* tree, const_type_t info);

Tree_status FillNodeInfo(Tree_node* tree_node, const_type_t info);

Tree_status TreeVerify(Tree* tree);

Tree_status AllNodesVerify(Tree* tree, Tree_node* tree_node);

Tree_status OneNodeVerify(Tree* tree, Tree_node* tree_node);

size_t TreeSize(Tree_node* tree_node);

Tree_status InsertTwoLeaves(Tree* tree, Tree_node** node, const_type_t answer, const_type_t question);

char* ReadAnswer();

Tree_status CreateTreeFile(Tree* tree, const char* name_file_with_tree);

void PrintTreeToFile(Tree_node* tree_node, FILE* stream);

void SkipSpaces(char** buffer);

Tree_status TreeDtor(Tree* tree);

void NodeDtor(Tree_node* tree_node);

Tree_status TreeHTMLDump(Tree* tree, Tree_node* tree_node, int line, const char* file, Type_dump type_dump, Tree_status tree_status);

Tree_status GenerateGraph(Tree* tree, Tree_node* tree_node);

void PrintNodeToDot(Tree* tree, FILE *file, Tree_node* tree_node);

void PrintErrors(int error, FILE* stream);


#endif // TREE_H_