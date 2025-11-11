#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Akinator.h"

#include "colors.h"
#include "tree.h"


Tree_status StartAkinator(Tree* tree) {
    color_printf(COLOR_PURPLE, " - Hello, friend!\n");

    color_printf(COLOR_PURPLE, " - Are you ready to start game? ");

    type_answer answer = GetAnswerYesNo();
    if (answer == ERROR)
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR);

    if (answer == NO) {
        color_printf(COLOR_PURPLE, " - Very sad... Bye-bye\n");
        return SUCCESS;
    }

    color_printf(COLOR_PURPLE, " - Let's gooooooo\n");

    TREE_CHECK_AND_RETURN_ERRORS(PlayAkinator(tree, tree->root));  
    
    return SUCCESS;
}

Tree_status PlayAkinator(Tree* tree, Tree_node* cur_node) {
    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(tree));

    while ((cur_node)->left_node != NULL && (cur_node)->right_node != NULL) {
        AskQuestion(cur_node);

        type_answer answer = GetAnswerYesNo();
        if (answer == ERROR)
            TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR);

        if (answer == YES)
            cur_node = (cur_node)->left_node;
        else
            cur_node = (cur_node)->right_node;
    }

    if ((cur_node)->left_node == NULL && (cur_node)->right_node == NULL) {
        type_answer answer = GiveAndCheckMyAnswer(cur_node);
        if (answer == ERROR)
            TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR);

        if (answer == YES) {
            color_printf(COLOR_GREEN, " - I guessed right!\n");
            color_printf(COLOR_PURPLE, " - It was a wonderful game!\n");
        }
        else {
            color_printf(COLOR_PURPLE, " - Oh no:(\n");
            TREE_CHECK_AND_RETURN_ERRORS(AskAndAddRightAnswer(tree, cur_node));
        }
    }
    else
        TREE_CHECK_AND_RETURN_ERRORS(WRONG_SITUATION);

    EndAkinator(tree, cur_node);

    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(tree));

    return SUCCESS;
}

Tree_status EndAkinator(Tree* tree, Tree_node* cur_node) {
    cur_node = tree->root;
    color_printf(COLOR_PURPLE, " - So, our game is over. Do you want to continue? ");

    type_answer answer = GetAnswerYesNo();
    if (answer == ERROR)
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR);

    if (answer == YES) {
        color_printf(COLOR_PURPLE, " - Let's gooooooo\n");
        TREE_CHECK_AND_RETURN_ERRORS(PlayAkinator(tree, cur_node));
    }

    else {
        color_printf(COLOR_PURPLE, " - Bye-bye\n");
    }

    return SUCCESS;
}

void AskQuestion(Tree_node* cur_node) {
    color_printf(COLOR_GREEN, " - %s?\n", cur_node->info);
}

type_answer GetAnswerYesNo() {
    char* answer = NULL;

    while (true) {
        color_printf(COLOR_BLUE, "Enter <Yes> or <No>\n");

        if (scanf("%m[^\n]%*c", &answer) != 1) {
            free(answer);
            return ERROR;
        }

        if (strcmp(answer, "Yes") == 0) {
            free(answer); // TODO it more beautiful
            return YES;
        }
    
        if (strcmp(answer, "No") == 0) {
            free(answer);
            return NO;
        }

        free(answer);
    }

    free(answer);

    return WRONG_ANSWER;
}

type_answer GiveAndCheckMyAnswer(Tree_node* cur_node) {
    color_printf(COLOR_GREEN, " - So, my answer: %s\n", cur_node->info);

    color_printf(COLOR_PURPLE, " - Is it right? ");

    return GetAnswerYesNo();
}

Tree_status AskAndAddRightAnswer(Tree* tree, Tree_node* cur_node) {
    color_printf(COLOR_CYAN, " - What is the right answer?\n");

    char* right_answer = NULL;
    if (scanf("%m[^\n]%*c", &right_answer) != 1)
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR,    free(right_answer));

    color_printf(COLOR_CYAN, " - Ask, please, how is %s different from %s. It...?\n", right_answer, cur_node->info);

    char* difference = NULL;
    if (scanf("%m[^\n]%*c", &difference) != 1)
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR,    free(right_answer);
                                                    free(difference););

    TREE_CHECK_AND_RETURN_ERRORS(NodeInsertAtTheEnd(tree, &cur_node, right_answer, difference),     free(right_answer);
                                                                                                    free(difference););

    color_printf(COLOR_GREEN, " - Thanks! It's very interesting. I remembered it\n");

    free(right_answer);
    free(difference);

    return SUCCESS;
}