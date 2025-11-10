#include "Akinator.h"
#include "tree.h"


int main(int, char** argv) {
    Tree tree = {};

    const char* dump_filename       = argv[1];
    const char* directory           = argv[2];
    const char* name_file_with_tree = argv[3];

    TreeCtor(&tree, "Bla-bla-bla", dump_filename, directory, name_file_with_tree);

    TreeHTMLDump(&tree, DUMP_INFO, NOT_ERROR_DUMP);

    StartAkinator(&tree);

    TreeHTMLDump(&tree, DUMP_INFO, NOT_ERROR_DUMP);

    CreateTreeFile(&tree);

    TreeDtor(&tree);

    return 0;
}