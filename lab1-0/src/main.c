#include <stdio.h>
#include <string.h>

#define MAX_BUF 100000

void create_shift_array(char* stop_shift, char* substring, int len)
{
    unsigned i = 1;
    while (i < 256)
    {
        stop_shift[i] = len;
        i++;
    }

    i = len;
    while (i > 1)
    {
        stop_shift[(unsigned char) substring[len-i]] = i - 1;
        i--;
    }
}

int BoyerMoore(char* stop_shift, char *substring, int substring_length, char* buf, int str_len, int count, int posit)
{
    while (posit < str_len)
    {
        printf("%d ", count + posit + 1);
        int i = 0;
        while (i < substring_length - 1 && buf[posit - i] == substring[substring_length - 1 - i])
        {
            i++;
            printf("%d ", count + posit - i + 1);
        }
        posit += stop_shift[(unsigned char)buf[posit]];
    }
    return posit - str_len;
}

int main(void) 
{
    char substring[18] = { 0 };
    char buf[MAX_BUF] = { 0 };
    int substring_length;
    int read_len;
    char stop_shift[256] = { 0 };

    if(NULL == fgets(substring, 18, stdin))
    {
        printf("bad input");
        return 0;
    }

    substring_length = strlen(substring);
    if(substring[substring_length - 1] == '\n')
    {
        substring[--substring_length] = 0;
    }

    create_shift_array(stop_shift, substring, substring_length);

    int count = 0;
    int offset = 0;
    int posit = substring_length - 1;
    
    while ((read_len = fread(buf + offset, 1, MAX_BUF - offset, stdin)))
    {
        int str_len = read_len + offset;
        
        posit = BoyerMoore(stop_shift, substring, substring_length, buf, str_len, count, posit) + substring_length;

        offset = substring_length;
        count = count + str_len - substring_length;
        
        if (read_len <= str_len)
            memcpy(buf, buf + MAX_BUF - offset, offset);
    }
}
