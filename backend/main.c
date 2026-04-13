#include <stdio.h>
#include <stdlib.h>

extern void load_dictionary();
extern void process_text(FILE *fp);

int main(int argc, char *argv[]) {

    load_dictionary();

    FILE *fp;

    if (argc > 1) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            printf("Error opening file\n");
            return 1;
        }
    } else {
        fp = stdin;
    }

    process_text(fp);

    if (argc > 1)
        fclose(fp);

    return 0;
}