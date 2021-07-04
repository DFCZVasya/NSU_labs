#include <stdio.h>
#include <string.h>

int get_hash(const char* data, int start_index, int data_len, int* maxpow)
{
    int res = 0;
    int pow = 1;

    for (int i = start_index; i < data_len; i++)
    {
        res += ((unsigned char) data[i] % 3) * pow;
        pow *= 3;
    }

    *maxpow = pow/3;

    return res;
}


void RabinKarp(const char* template, const char* data, int template_length,
               int data_length, int template_hash, unsigned long count)
{
    int maxpow;
    int data_hash = get_hash(data, 0, template_length, &maxpow);

    int pos = 0;
    while (pos < data_length - template_length + 1)
    {
        if (template_hash == data_hash)
        {
            int i = 0;
            while ((i < template_length) && (i + pos < data_length) && ((unsigned char)template[i] == (unsigned char)data[i + pos]))
            {
                printf("%lu ", pos + count + 1 + i++);
            }

            if (i < template_length)
                printf("%lu ", pos + count + 1 + i);
        }

        data_hash = (data_hash - (unsigned char)data[pos]%3) / 3 + (unsigned char)data[pos + template_length]%3 * maxpow;
        pos++;
    }
}


int main(void) {
    char temp[18];
    char text[101];
    unsigned long count = 0;
    int templen,
        temphash;
    int maxpow;

    if (!fgets(temp, 18, stdin))
        return 1;


    templen = strlen(temp) - 1;

    temphash = get_hash(temp, 0, templen, &maxpow);

    printf("%d\n", temphash);

    while (fgets(text, 100, stdin))
    {
        int textlen = strlen(text);
        if(text[textlen - 1] == '\n')
            textlen--;

        RabinKarp(temp, text, templen, textlen, temphash, count);

        count = count + textlen + 1;
    }

    return 0;
}
