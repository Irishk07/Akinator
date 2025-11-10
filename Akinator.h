#ifndef AKINATOR_H_
#define AKINATOR_H_

#include "tree.h"

enum type_answer {
    YES          = 0,
    NO           = 1,
    WRONG_ANSWER = 2,
    ERROR        = 3
};


Tree_status StartAkinator(Tree* tree);

Tree_status PlayAkinator(Tree* tree, Tree_node* cur_node);

Tree_status EndAkinator(Tree* tree, Tree_node* cur_node);

void AskQuestion(Tree_node* cur_node);

type_answer GetAnswerYesNo();

type_answer GiveAndCheckMyAnswer(Tree_node* cur_node);

Tree_status AskAndAddRightAnswer(Tree* tree, Tree_node* cur_node);


#endif // AKINATOR_H_