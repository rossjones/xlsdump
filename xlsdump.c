/**
*
* Uses libxls and derived largely from work by Christophe Leitienne and
* David Hoerl
**/
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "libxls/xls.h"


char stringSeparator = '"';
char *fieldSeparator = ",";


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

static void write_string(FILE *fd, const char *string) {
    const char *str;

    fprintf(fd, "%c", stringSeparator);
    for (str = string; *str; str++) {
        if (*str == stringSeparator) {
            fprintf(fd, "%c%c", stringSeparator, stringSeparator);
        } else if (*str == '\\') {
            fprintf(fd, "\\\\");
        } else {
            fprintf(fd, "%c", *str);
        }
    }
    fprintf(fd, "%c", stringSeparator);
}

int process(char *filename) {
    xlsWorkBook* pWorkbook;
    xlsWorkSheet* pSheet;

    pWorkbook = xls_open(filename, "iso-8859-15//TRANSLIT");
    if ( pWorkbook == NULL ) {
        fprintf(stderr, "Couldn't find %s.\n", filename);
        return EXIT_FAILURE;
    }

    // Iterate over the sheets
    for (int i = 0; i < pWorkbook->sheets.count; i++) {
        char *name = (char *)pWorkbook->sheets.sheet[i].name;
        char *filename = malloc(strlen(name)+5); // null and .csv
        sprintf(filename, "%s.csv", slugify(name));

        FILE *fd = fopen(filename, "w");

        pSheet = xls_getWorkSheet(pWorkbook, i);
        xls_parseWorkSheet(pSheet);

        for (int cellRow = 0; cellRow <= pSheet->rows.lastrow; cellRow++) {
            int isFirstCol = 1;
            for (int cellCol = 0; cellCol <= pSheet->rows.lastcol; cellCol++) {
                xlsCell *cell = xls_cell(pSheet, cellRow, cellCol);

                if ((!cell) || (cell->isHidden)) {
                    continue;
                }

                if (!isFirstCol) {
                    fprintf(fd, "%s", fieldSeparator);
                } else {
                    isFirstCol = 0;
                }

                if (cell->rowspan > 1) {
                    fprintf(stderr, "Warning: %d rows spanned at col=%d row=%d: output will not match the Excel file.\n", cell->rowspan, cellCol+1, cellRow+1);
                }

                // display the value of the cell (either numeric or string)
                if (cell->id == 0x27e || cell->id == 0x0BD || cell->id == 0x203) {
                    fprintf(fd, "%.15g", cell->d);
                } else if (cell->id == 0x06) {
                    if (cell->l == 0) {
                        fprintf(fd, "%.15g", cell->d);
                    } else {
                        /* Handle macros */
                        if (!strcmp((char *)cell->str, "bool")) {
                            write_string(fd, (int) cell->d ? "True" : "False");
                        } else if (!strcmp((char *)cell->str, "error")) {
                            write_string(fd, "*error*");
                        } else {
                            write_string(fd, (char *)cell->str);
                        }
                    }
                } else if (cell->str != NULL) {
                    write_string(fd, (char *)cell->str);
                } else {
                    write_string(fd, "");
                }

            }
            fprintf(fd, "\n");
        }

        fclose(fd);
        free(filename);
    }

    xls_close(pWorkbook);
    return EXIT_SUCCESS;
}

int main(int argc, char*argv[]) {
    if ( argc != 2 ) {
        fprintf(stderr, "usage: %s filename.xls\n", argv[0] );
        return EXIT_FAILURE;
    }

    return process(argv[1]);
}
