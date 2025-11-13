#include "Akinator.h"
#include "tree.h"


int main(int, char** argv) {
    const char* dump_filename = argv[1];
    const char* directory     = argv[2];

    Akinator akinator = {};

    AkinatorCtor(&akinator, dump_filename, directory);

    StartAkinator(&akinator);

    AkinatorDtor(&akinator);

    return 0;
}