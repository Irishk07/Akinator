#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Akinator.h"

#include "colors.h"
#include "tree.h"


Tree_status AkinatorCtor(Akinator* akinator, const char *dump_filename, const char *directory) {
    akinator->tree = {};
    TREE_CHECK_AND_RETURN_ERRORS(TreeCtor(&akinator->tree, dump_filename, directory));
    // TREE_CHECK_AND_RETURN_ERRORS(CreateRoot(&akinator->tree, "Bla-bla-bla"));

    akinator->begin_buffer = NULL;
    akinator->end_buffer   = NULL;
    akinator->size_buffer  = 0;

    return SUCCESS;
}

Tree_status StartAkinator(Akinator* akinator) {
    color_printf(COLOR_PURPLE, " - Hello, friend!\n");

    color_printf(COLOR_PURPLE, " - Are you ready to start game?\n");

    type_answer answer = GetAnswerYesNo();
    if (answer == ERROR)
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR);

    if (answer == WRONG_ANSWER) {
        color_printf(COLOR_RED, " - You try to do it 5 times... What's wrong with you?\n");
        return SUCCESS; 
    }

    if (answer == NO) {
        color_printf(COLOR_PURPLE, " - Very sad... Bye-bye\n");
        return SUCCESS; 
    }

    color_printf(COLOR_PURPLE, " - Let's gooooooo\n");

    TREE_CHECK_AND_RETURN_ERRORS(ChooseOption(akinator));
    
    return SUCCESS;
}

type_answer GetAnswerYesNo() {
    type_answer result = WRONG_ANSWER;

    int cnt_attempts = CNT_ATTEMPTS;

    while (cnt_attempts-- > 0) {
        color_printf(COLOR_BLUE, " - Enter <Yes> or <No>\n");

        char* answer = ReadAnswer();

        if (strcmp(answer, "Yes") == 0) {
            result = YES;
            free(answer);
            break;
        }
    
        if (strcmp(answer, "No") == 0) {
            result = NO;
            free(answer);
            break;
        }

        free(answer);
    }

    return result;
}

#define DO_CASE(function)                       \
        TREE_CHECK_AND_RETURN_ERRORS(function); \
        break;

Tree_status ChooseOption(Akinator* akinator) {
    color_printf(COLOR_YELLOW, " - Please, chose option:\n");
    color_printf(COLOR_YELLOW, "   [1] Guess a character\n");
    color_printf(COLOR_YELLOW, "   [2] Printf path to the character\n");
    color_printf(COLOR_YELLOW, "   [3] Compare two characters\n");
    color_printf(COLOR_YELLOW, "   [4] Save tree to file\n");
    color_printf(COLOR_YELLOW, "   [5] Upload tree from file\n");
    color_printf(COLOR_YELLOW, "   [6] Finish game\n");

    type_options option = ReadSelectedOption();

    switch (option) {
        case ONE:  DO_CASE(PlayAkinator(akinator, akinator->tree.root));
        // case TWO: DO_CASE();
        // case THREE: DO_CASE();
        case FOUR: DO_CASE(SaveTree(akinator));
        case FIVE: DO_CASE(UploadTree(akinator));
        case SIX:  DO_CASE(EndAkinator(akinator));
        case WRONG_ANSWER: TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR);
        default: break;
    }

    return SUCCESS;
}

#undef DO_CASE

type_options ReadSelectedOption() {
    type_options option = WRONG_OPTION;

    int cnt_attempts = CNT_ATTEMPTS;
    while (cnt_attempts-- > 0) {
        color_printf(COLOR_YELLOW, " - Enter 1, 2, 3, 4, 5 or 6\n");

        if (scanf("%d%*c", &option) != 1) {
            scanf("%*[^\n]%*c");
        }
        
        if (ONE <= option && option <= SIX)
            return option;
    }

    return option;
}

Tree_status PlayAkinator(Akinator* akinator, Tree_node* cur_node) {
    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(&akinator->tree));

    while (cur_node->left_node != NULL && cur_node->right_node != NULL) {
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
            color_printf(COLOR_GREEN,  " - I guessed right!\n");
            color_printf(COLOR_PURPLE, " - It was a wonderful game!\n");
        }
        else {
            color_printf(COLOR_PURPLE, " - Oh no:(\n");
            TREE_CHECK_AND_RETURN_ERRORS(AskAndAddRightAnswer(akinator, cur_node));
        }
    }
    else {
        TREE_CHECK_AND_RETURN_ERRORS(WRONG_SITUATION);
    }

    EndAkinator(akinator);

    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(&akinator->tree));

    return SUCCESS;
}

// Tree_status PathToCharacter(Tree* tree) {
//     color_printf(COLOR_PURPLE, " - Enter name of the character\n");

//     char character[100] = {}; //FIXME
//     if (scanf("%s[^\n]%*c", character) != 1) 
//         return READ_ERROR;

//     Tree_node* result = FindCharacter(tree->root, character);
//     if (result == NULL)
//         return CHARACTER_NOT_FIND;

//     color_printf(COLOR_GREEN, "Path to your character:\n");

//     while (result != tree->root) {
//         printf("%s -> ", result->info);
//         result = result->parent;
//     }
//     printf("%s", result->info);

//     return SUCCESS;
// }

// Tree_node* FindCharacter(Tree_node* tree_node, type_t character) {
//     if (strcmp(tree_node->info, character) == 0)
//         return tree_node;

//     if (tree_node->left_node)
//         return FindCharacter(tree_node->left_node, character);

//     if (tree_node->right_node)
//         return FindCharacter(tree_node->right_node, character);

//     return NULL;
// }

Tree_status SaveTree(Akinator* akinator) {
    color_printf(COLOR_PURPLE, " - Please, enter the name of the file where the tree will be saved\n");

    char* file_name = ReadAnswer();

    if (file_name == NULL)
        color_printf(COLOR_RED, " - Sorry, I couldn't do it, I have some problems. Try later please\n");

    else
        TREE_CHECK_AND_RETURN_ERRORS(CreateTreeFile(&akinator->tree, file_name));

    color_printf(COLOR_GREEN, " - All right, tree is saved in your file %s\n", file_name);

    free(file_name);

    EndAkinator(akinator);

    return SUCCESS;
}

Tree_status UploadTree(Akinator* akinator) {
    color_printf(COLOR_PURPLE, " - Please, enter the name of the file from which the tree will be uploaded\n");

    char* file_name = ReadAnswer();

    if (file_name == NULL)
        color_printf(COLOR_RED, " - Sorry, I couldn't do it, I have some problems. Try later please\n");

    else
        TREE_CHECK_AND_RETURN_ERRORS(ReadTree(akinator, file_name));

    color_printf(COLOR_GREEN, " - All right, tree is uploaded from your file %s\n", file_name);

    TreeHTMLDump(&akinator->tree, akinator->tree.root, DUMP_INFO, NOT_ERROR_DUMP);

    free(file_name);

    EndAkinator(akinator);

    return SUCCESS;
}

Tree_status ReadTree(Akinator* akinator, const char* file_with_tree) {
    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(&akinator->tree));

    FILE* file = fopen(file_with_tree, "r");
    if (file == NULL) {
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);
    }

    char* new_tree = NULL;
    size_t buffer_size = 0;
    if (getline(&new_tree, &buffer_size, file) == -1) {
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR);
    }

    akinator->size_buffer  = buffer_size;
    akinator->begin_buffer = new_tree;

    akinator->tree.root = NULL;
    TREE_CHECK_AND_RETURN_ERRORS(ReadNode(akinator, &akinator->tree.root, &new_tree, NULL));

    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(&akinator->tree));

    return SUCCESS;
}

#define DUMP_CURRENT_SITUATION(tree, tree_node, current_pos)                          \
    TreeHTMLDump(&tree, tree_node, DUMP_INFO, NOT_ERROR_DUMP);                          \
    fprintf(tree.dump_info.dump_file, "Current situation: \":%s\"\n", *current_pos);

Tree_status ReadNode(Akinator* akinator, Tree_node** tree_node, char** current_pos, Tree_node* parent) {
    SkipSpaces(current_pos);

    if (**current_pos == '(') {
        NodeCtor(&akinator->tree, tree_node, parent);
        DUMP_CURRENT_SITUATION(akinator->tree, *tree_node, current_pos);

        (*current_pos)++;
        DUMP_CURRENT_SITUATION(akinator->tree, *tree_node, current_pos);

        int read_bytes = 0;
        sscanf(*current_pos, "\"%*[^\"]\"%n", &read_bytes);
        *(*current_pos + read_bytes - 1) = '\0'; // close " -> '\0'

        FillNodeInfo(*tree_node, *current_pos + 1); // ++ because skip "
        DUMP_CURRENT_SITUATION(akinator->tree, *tree_node, current_pos);

        *current_pos += read_bytes;
        DUMP_CURRENT_SITUATION(akinator->tree, *tree_node, current_pos);

        TREE_CHECK_AND_RETURN_ERRORS(ReadNode(akinator, &(*tree_node)->left_node, current_pos, *tree_node));

        TREE_CHECK_AND_RETURN_ERRORS(ReadNode(akinator, &(*tree_node)->right_node, current_pos, *tree_node));

        (*current_pos)++; // ++ because skip ')'
        DUMP_CURRENT_SITUATION(akinator->tree, *tree_node, current_pos);

        SkipSpaces(current_pos);
    }

    else if (strncmp(*current_pos, "nil", LEN_NIL) == 0) {
        *current_pos += LEN_NIL;
        DUMP_CURRENT_SITUATION(akinator->tree, *tree_node, current_pos);

        *tree_node = NULL;
    }

    else {
        fprintf(stderr, "error %s\n", *current_pos);
        TREE_CHECK_AND_RETURN_ERRORS(SYNTAX_ERROR);
    }

    return SUCCESS;
}

#undef DUMP_CURRENT_SITUATION

Tree_status EndAkinator(Akinator* akinator) {
    color_printf(COLOR_PURPLE, " - So, our game is over. Do you want to continue?\n");

    type_answer answer = GetAnswerYesNo();
    if (answer == ERROR)
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR);

    if (answer == YES) {
        color_printf(COLOR_PURPLE, " - Let's gooooooo\n");
        TREE_CHECK_AND_RETURN_ERRORS(ChooseOption(akinator));
    }

    if (answer == WRONG_ANSWER)
        color_printf(COLOR_RED, " - You try to do it 5 times... What's wrong with you?\n");

    if (answer == NO)
        color_printf(COLOR_PURPLE, " - Bye-bye\n");

    return SUCCESS;
}

void AskQuestion(Tree_node* cur_node) {
    color_printf(COLOR_GREEN, " - %s?\n", cur_node->info);
}

type_answer GiveAndCheckMyAnswer(Tree_node* cur_node) {
    color_printf(COLOR_GREEN, " - So, my answer: %s\n", cur_node->info);

    color_printf(COLOR_PURPLE, " - Is it right?\n");

    return GetAnswerYesNo();
}

Tree_status AskAndAddRightAnswer(Akinator* akinator, Tree_node* cur_node) {
    color_printf(COLOR_CYAN, " - What is the right answer?\n");

    char* right_answer = ReadAnswer();

    if (right_answer == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR);

    color_printf(COLOR_CYAN, " - Ask, please, how is %s different from %s. It...?\n", right_answer, cur_node->info);

    char* difference = ReadAnswer();

    if (difference == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR,    free(right_answer));

    TREE_CHECK_AND_RETURN_ERRORS(InsertTwoLeaves(&akinator->tree, &cur_node, right_answer, difference),     free(right_answer);
                                                                                                            free(difference););

    color_printf(COLOR_GREEN, " - Thanks! It's very interesting. I remembered it\n");

    free(right_answer);
    free(difference);

    return SUCCESS;
}

Tree_status AkinatorDtor(Akinator* akinator) {
    Tree_status status = TreeVerify(&akinator->tree);

    free(akinator->begin_buffer);

    AkinatorNodeDtor(akinator, akinator->tree.root);
    akinator->tree.root = NULL;

    if (fclose(akinator->tree.dump_info.dump_file) == EOF) {
        status = CLOSE_ERROR;
        perror("Error is: ");
    }

    akinator->tree.dump_info.dump_file = NULL;

    akinator->tree.size = 0;

    return status;
}

void AkinatorNodeDtor(Akinator* akinator, Tree_node* tree_node) {
    if (tree_node == NULL)
        return;

    if (tree_node->info < akinator->begin_buffer || tree_node->info > akinator->end_buffer)
        free(tree_node->info);
    tree_node->info = NULL;

    AkinatorNodeDtor(akinator, tree_node->left_node);

    AkinatorNodeDtor(akinator, tree_node->right_node);

    free(tree_node);
}