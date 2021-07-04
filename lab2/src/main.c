#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_invalid(char* str)
{

    int check[10] = { 0 };

    for (int i = 0; i < (int)strlen(str); ++i) {
        if (!isdigit(str[i])) {
            printf("bad input\n");
            return 0;
        }
        if (check[str[i] - '0']) {
            printf("bad input\n");
            return 0;
        }
        check[str[i] - '0'] = 1;
    }
    return 1;
}

void swap(char* a, char* b) {
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

int next_perm(char* str) {
    for (int i = strlen(str) - 2; i >= 0; --i) {
        if (str[i] >= str[i + 1]) continue;

        int min = i + 1;

        for (int j = i + 1; j < (int)strlen(str) - i; ++j) {
            if (str[j] < str[min] && str[j] > str[i]) {
                min = j;
            }
        }
        swap(&str[i], &str[min]);

        for (int j = 1; j <= ((int)strlen(str) - i) / 2; j++) {
            swap(&str[i + j], &str[strlen(str) - j]);
        }
        return 1;
    }
    return 0;
}

int main() {
    char str[64] = { 0 };

    if(!fgets(str, 63, stdin))
        return 0;

    if(str[strlen(str) - 1] == '\n')
    {
        str[strlen(str) - 1] = 0;
    }
    else
    {
        printf("bad input\n");
        return 0;
    }

    if (!is_invalid(str))
    {
        return 0;
    }

    int count;
    if (!scanf("%d", &count))
    {
        printf("input error");
    }
    for (int i = 0; i < count; ++i) {
        if (!next_perm(str))
        {
            return 0;
        }
        for (int j = 0; j < (int)strlen(str); ++j) {
            printf("%c", str[j]);
        }
        printf("\n");
    }
    return 0;

}
