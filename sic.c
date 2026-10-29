#include <stdio.h>
#include <locale.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUFFER_CAPACITY 4096

int8_t char_bytes(const unsigned char val) {
    if      (val < 128) return 1;
    else if (val < 224) return 2;
    else if (val < 240) return 3;
    else                return 4;
}

int8_t search(const char* stanza, const char* in) {
    if (stanza == NULL) return -1;

    FILE* fh = fopen(in, "r");
    if (fh == NULL) {
        fprintf(stderr, "ERROR: Could not read %s", in);
        return -1;
    }

    // both 1-based
    uint16_t linum = 0;
    uint16_t conum = 0;

    char buf[MAX_BUFFER_CAPACITY];
    while (fgets(buf, MAX_BUFFER_CAPACITY, fh)) {
        linum++;

        char* cm = strstr(buf, stanza);
        if (cm != NULL) {
            cm[strcspn(cm, "\n")] = 0;
            conum = (int)((cm - buf + 1) / (sizeof(char)));
            // standard grep and Emacs compile in format
            // in:line:column: msg
            printf("%s:%d:%d: %s", in, linum, conum, buf);
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc > 1) search(argv[argc-1], argv[argc-2]);
    return 0;
}
