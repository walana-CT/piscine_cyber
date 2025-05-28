#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void no() {
    printf("Nope!\n");
    exit(1);
}

void yes() {
    printf("Good job.\n");
    return;
}

int main(void) {
    char user_input[32] = {0};           // buffer brut (simulateur de pile)
    char final_string[9] = {0};         // pour stocker les caractères reconstruits
    int i;
    int j;
    int len1;
    int len2;
    int value;
    char* chunk;

    printf("Please enter key: ");
    if (scanf("%23s", user_input) != 1) {
        no();
    }

    if (user_input[6] != '0') {
        no();
    }
    if (user_input[5] != '0') {
        no();
    }

    memset(final_string, 0, 9);
    final_string[0] = 100; // add a * 

    chunk[3] = 0;
    i = 2;
    j = 1;

    while(1) {
        len1 = strlen(final_string);
        len2 = i;
       if(len1 < 8) {
            len1 = strlen(user_input + 5);
            user_input[0] = len2 < len1;
        }
        if (!user_input[0]) break;
        chunk[0] = user_input[i ];
        chunk[1] = user_input[i + 1];
        chunk[2] = user_input[i + 2];
        value = atoi(chunk);
        final_string[j] = value;
        i += 3;
        j += 1;
    }
    final_string[j] = 0;
    if (strcmp(final_string, "delabere") == 0) {
        yes();
    } else {
        no();
    }

    return 0;
}