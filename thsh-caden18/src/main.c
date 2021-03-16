#include <stdio.h>
#include <stdlib.h>

#include "Vec.h"
#include "Str.h"

#define MAX_AMT_OF_LINES 600000
#define MAXIMUM_LINE_LENGTH 8000
int main()
{
    Str s = Str_value(MAX_AMT_OF_LINES);
    char *line;
    char *copy = malloc(MAXIMUM_LINE_LENGTH);
    if (copy == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    while ((line = fgets(copy, MAXIMUM_LINE_LENGTH, stdin)) != NULL) {
        Str_append(&s, line);
    }
    for (size_t i = 0; i < Str_length(&s); ++i) {
        printf("%c", Str_get(&s, i));
    }
    printf("\n");
    

    return EXIT_SUCCESS;
}
