#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fake_malloc_fail() {
    printf("Nope!\n");
    exit(1);
    printf("Good job.\n");
    return;
}

void fake_maloc_succes() {
    printf("Good job.\n");
    return;
}

int main(void) {
    char raw_input[32] = {0};           // buffer brut (simulateur de pile)
    char *user_input = raw_input + 5;   // l’entrée réelle commence à +5 octets
    char final_string[9] = {0};         // pour stocker les caractères reconstruits
    int i;
    int j;
    int len1;
    int len2;
    int value;

    printf("Please enter key: ");
    if (scanf("%23s", user_input) != 1) {
        fake_malloc_fail();
    }

    if (raw_input[6] != '2') {
        fake_malloc_fail();
    }
    if (raw_input[5] != '4') {
        fake_malloc_fail();
    }

    memset(final_string, 0, 9);
    final_string[0] = 0x2a; // add a * 

    raw_input[3] = 0;
    i = 2;
    j = 1;

    while(1) {
        len1 = strlen(final_string);
        len2 = i;
       if(len1 < 8) {
            len1 = strlen(raw_input + 5);
            raw_input[0] = len2 < len1;
        }
        if (!raw_input[0]) break;
        raw_input[1] = raw_input[i + 5];
        raw_input[2] = raw_input[i + 6];
        raw_input[3] = raw_input[i + 7];
        value = atoi(raw_input + 1);
        final_string[j] = value;
        i += 3;
        j += 1;
    }
    final_string[j] = 0;
    if (strcmp(final_string, "********") == 0) {
        fake_maloc_succes();
    } else {
        fake_malloc_fail();
    }


    return 0;
}