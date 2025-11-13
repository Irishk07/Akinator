#ifndef AKINATOR_H_
#define AKINATOR_H_

#include "tree.h"
#include "stack.h"


const int DEFAULT_SIZE = 128;
const int CNT_ATTEMPTS = 5;

enum type_answer {
    YES          = 0,
    NO           = 1,
    WRONG_ANSWER = 2,
    ERROR        = 3,
};

enum type_options {
    ONE          = 1,
    TWO          = 2,
    THREE        = 3,
    FOUR         = 4,
    FIVE         = 5,
    SIX          = 6,
    WRONG_OPTION = 7
};

enum type_path {
    LEFT  = 0,
    RIGHT = 1,
};


struct Akinator {
    Tree tree;
    Dump_information dump_info;
    char* begin_buffer;
    char* end_buffer;
    size_t size_buffer;
};


Tree_status AkinatorCtor(Akinator* akinator, const char *dump_filename, const char *directory);

Tree_status StartAkinator(Akinator* akinator);

Tree_status ChooseOption(Akinator* akinator);

type_options ReadSelectedOption();

Tree_status PlayAkinator(Akinator* akinator, Tree_node* cur_node);

Tree_status PathToCharacter(Akinator* akinator);

Tree_node* FindCharacter(stack_t* stack, Tree_node* tree_node, type_t character);

Tree_status SaveTree(Akinator* akinator);

Tree_status UploadTree(Akinator* akinator);

Tree_status ReadTree(Akinator* akinator, const char* file_with_tree);

Tree_status ReadNode(Akinator* akinator, Tree_node** tree_node, char** current_pos, Tree_node* parent);

Tree_status EndAkinator(Akinator* akinator);

void AskQuestion(Tree_node* cur_node);

type_answer GetAnswerYesNo();

type_answer GiveAndCheckMyAnswer(Tree_node* cur_node);

Tree_status AskAndAddRightAnswer(Akinator* akinator, Tree_node* cur_node);

Tree_status AkinatorDtor(Akinator* akinator);

void AkinatorNodeDtor(Akinator* akinator, Tree_node* tree_node);


#endif // AKINATOR_H_