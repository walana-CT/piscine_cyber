#include <stdio.h>
#include <string.h>

int main() {
    char input[100];

    printf("Please enter key: ");
    scanf("%99s", input);  // pour éviter débordement

    if (strcmp(input, "toto123") == 0) {
        printf("Good job.\n");
    } else {
        printf("Nope.\n");
    }

    return 0;
}