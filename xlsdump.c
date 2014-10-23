#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "libxls/xls.h"


char *slugify(char *name) {
    int len = strlen(name);
    for (int i = 0; i < len; i++){
        name[i] = tolower(name[i]);
        if ( ispunct(name[i]) || isspace(name[i]) ) {
            name[i] = '-';
        }
    }
    return name;
}

int main(int argc, char*argv[]) {
    xlsWorkBook* pWorkbook;
    xlsWorkSheet* pSheet;

    if ( argc != 2 ) {
        fprintf(stderr, "usage: %s filename.xls\n", argv[0] );
        return EXIT_FAILURE;
    }

    pWorkbook = xls_open(argv[1], "iso-8859-15//TRANSLIT");
    if ( pWorkbook == NULL ) {
        fprintf(stderr, "Couldn't find %s. Sorry\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Iterate over the sheets
    for (int i = 0; i < pWorkbook->sheets.count; i++) {
        char *name = (char *)pWorkbook->sheets.sheet[i].name;
        char *filename = malloc(strlen(name)+5); // null and .csv
        sprintf(filename, "%s.csv", slugify(name));

        fprintf(stdout, "Sheet %d is called %s\n", i, filename);

        free(filename);
    }

    xls_close(pWorkbook);
    return EXIT_SUCCESS;
}
