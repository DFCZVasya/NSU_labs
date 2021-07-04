#include <stdio.h>
#include <stdlib.h>

#define INPUT_STR_LEN 14

#define STRINGIFY(n) STRINGIFy(n)
#define STRINGIFy(n) # n


int get_number_length(char *input_str)
{
    int i = 0;
    while(input_str[i] != '\0')
        i++;
    return i;
}

//takes all input parameters and check if them compare with requirements
//return 1 if all parameters is ok and 0 if we have bad input 
int check_input(int b1, int b2, char *input_string, int input_number_length)
{
    if((b1 < 2 || b1 > 16) || (b2 < 2 || b2 > 16))
    {
        printf("bad input");
        return 0;
    }
    for (int i = 0; i < input_number_length; i++)
    {

        if(input_string[i] >= '0' && input_string[i] <= '9')
        {
            if((input_string[i] - '0') >= b1 )
            {
                printf("bad input");
                return 0;
            }
        }
        else if(input_string[i] >= 'A' && input_string[i] <= 'F')
        {
            if((input_string[i] - 'A') + 10 >= b1 )
            {
                printf("bad input");
                return 0;
            }
        }
        else if(input_string[i] >= 'a' && input_string[i] <= 'f')
        {
            if((input_string[i] - 'a') + 10 >= b1 )
            {
                printf("bad input");
                return 0;
            }
        }
        else if(input_string[i] == '.')
        {
            //check if input string will be "0..123" or "0123." or "." or ".123"
            if(input_string[i+1] == '.' || input_string[i+1] == '\0'  || i == 0)
            {
                printf("bad input");
                return 0;
            }
        }
        else
        {
            printf("bad input");
            return 0;
        }
           
    }
    return 1;
}

int char_to_number(char s)
{
    if(s >= '0' && s <= '9')
        return (s - '0');
    else if(s >= 'A' && s <= 'F')
        return (s - 'A' + 10);
    else if(s >= 'a' && s <= 'f')
        return (s - 'a' + 10);
    return 0;
}

void get_number_parts(char *input_str, int str_length, int b1 ,long long int *first_part,long long int *second_part,long long int *det)
{
    int i = 0;
    while(i < str_length && input_str[i] != '.')
    {
        *first_part = *first_part * b1 + char_to_number(input_str[i]);
        i++;
    }
    if(input_str[i] == '.')
    {
        i++;
        while(i < str_length)
        {
            *second_part = *second_part * b1 + char_to_number(input_str[i]);
            *det *= b1;
            i++;
        }
    }
}

char find_symbol(int position)
{
	char symbol_list[] = "0123456789ABCDEF";

	return symbol_list[position];
}

void cswap(char *a, char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp; 
}

void final_translation(long long int first_part, long long int second_part, long long int det, int b2)
{
    char output_string[64] = "";
    int i = 0;
    if(first_part == 0)
    {
        output_string[i] = '0';
        i++;
    }
    while(first_part != 0)
    {
        output_string[i] = find_symbol(first_part % b2);
        i++;
        first_part /= b2;

    }
    
    for (int l = 0, j = i - 1; l < j; l++, j--) 
    {
       cswap(&output_string[l], &output_string[j]);
    }

    if(second_part > 0)
    {
        output_string[i] = '.';
        i++;

        int counter = 0;
        while (second_part != 0 && counter != 12)
        {
            second_part *= b2;
            output_string[i] = find_symbol(second_part / det);
            i++;
            second_part %= det;
            counter++;
        }
    }
    printf("%s", output_string);
}

int main()
{   
    int b1, b2;
    //errno = 0;
    
    
    if(scanf("%i%i", &b1, &b2))
    {}
    else 
    {
        fprintf(stderr, "No matching characters\n");
    }
    
    //allocate space for the string
    char line[INPUT_STR_LEN + 1] = "";
    if(scanf("%" STRINGIFY(INPUT_STR_LEN) "s", line))
    {   
        int length = get_number_length(line);

        if(!check_input(b1, b2, line, length))
        {
            //system("pause");
            return 0; 
        }

        long long int first_part = 0, second_part = 0, det = 1;
        get_number_parts(line, length, b1, &first_part, &second_part, &det);
        final_translation(first_part, second_part, det, b2);
    }
    else 
    {
        fprintf(stderr, "No matching characters\n");
    }


    //system("pause");
    return 0;
}
