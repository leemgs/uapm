// log_parser.c - Minimal C program to parse CSV logs produced by experiments
// Compile: gcc -o log_parser log_parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <csvfile>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        return 1;
    }
    char line[LINE_MAX];
    int count = 0;
    while (fgets(line, sizeof(line), f)) {
        if (count == 0) {
            printf("Header: %s", line);
        } else {
            char *tok = strtok(line, ",");
            if (tok) {
                printf("Row %d first column: %s\n", count, tok);
            }
        }
        count++;
    }
    fclose(f);
    printf("Parsed %d lines.\n", count);
    return 0;
}
