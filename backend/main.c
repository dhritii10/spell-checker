#include <stdio.h>

void load_dictionary();
void process_text(FILE *fp);

int main(int argc, char *argv[]) {

    load_dictionary();

    FILE *fp = stdin;

    if (argc > 1)
        fp = fopen(argv[1], "r");

    process_text(fp);

    return 0;
}