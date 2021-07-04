#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int get_element_priority(char element)
{
    switch (element) {
    case '(':
        return 1;
    case ')':
        return 2;
    case '=':
        return 3;
    case '+':
    case '-':
        return 4;
    case '*':
    case '/':
        return 5;
    case '^':
        return 6;
    default:
        return -1;
    }
}

void calculate_and_print(char *postfix_str)
{
//calculating
    int int_Stack[1002] = {0}, int_top = -1, length = strlen(postfix_str);
    for (int i = 0, l = 0; i < length; i++)//for every element of the result string
    {
        char elem[1002] = "";
        l = 0;
        while (postfix_str[i] != ' ')
        {
            elem[l++] = postfix_str[i++]; 
        }
        if (isdigit(elem[0]))
        {
            int_Stack[++int_top] = atoi(elem);
        }
        else
        {
            int right_n = int_Stack[int_top--];
            int left_n = int_Stack[int_top--];
            char oper = elem[0];
            int ans = 0;
            switch (oper)
            {
                case '+': ans = left_n + right_n; break;
                case '-': ans = left_n - right_n; break;
                case '*': ans = left_n * right_n; break;
                case '/': if (right_n != 0) ans = left_n / right_n; else{ printf("division by zero\n"); return; } break;
            }
            int_Stack[++int_top] = ans;
        }
    }
    printf("%i\n", int_Stack[int_top--]);
}

char* create_postfix_string(char *input_str)
{
//conversion from infix to postfix with spaces between the elements
//returns string in postfix format for next step(calculating) or 0 if there are some problems in input string
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
    int bracket_count = 0;
    int length = strlen(input_str);
    char *postfix_str = (char*)malloc(2004 * sizeof(char));// = ""; //Не красиво, знаю, но что поделать, зато точно работает
    char char_Stack[1002] = "";
    int char_top = -1, j = 0;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~input check~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (length == 0)//не пустая строка
    {
        free(postfix_str);
        return 0;
    }
    if ((!isdigit(input_str[0])) && (input_str[0] != '('))//первым не стоит знак
    {
        free(postfix_str);
        return 0;
    }
    for (int i = 0; i < length; i++)//for every element of the input string
    {   
        if (input_str[i] == '(')
        {
            bracket_count++;
            if (i+1 < length)//проверка на то что в скобках стоят цифры
            {
                if ((!isdigit(input_str[i+1])) && input_str[i+1] != '(')
                {
                    free(postfix_str);
                    return 0;
                }
            }
            else
            {
                free(postfix_str);
            return 0;
            }
        }
        if (input_str[i] == ')')//если ')' стоит перед '(', то тоже ошибка
        {
            bracket_count--;
            if (bracket_count < 0)
            {
                free(postfix_str);
                return 0;
            }
        }
        if ((!isdigit(input_str[i])) && (input_str[i] != '(') && (input_str[i] != ')'))//проверка на два знака подряд
        {    
            if (i+1 < length)
            {    
                if ((!isdigit(input_str[i+1])) && (input_str[i+1] != '('))
                {
                    free(postfix_str);
                    return 0;
                }    
            }
            else
            {
                free(postfix_str);
                return 0;
            }
        }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int j_check = 0;
        while(input_str[i] >= '0' && input_str[i] <= '9') //catches numbers for result string here
        {
            postfix_str[j] = input_str[i];
            j++;
            i++;
            j_check++;
        }
        if (j_check != 0)
        {
            postfix_str[j] = ' ';
            j++;
            j_check = 0;;
            i--;
        }
        else 
        {
            if (char_top == -1)
            {
                char_Stack[++char_top] = input_str[i];
            }
            else
            {
                if (input_str[i] == '(')
                {
                    char_Stack[++char_top] = input_str[i];
                }
                else
                {
                    while (get_element_priority(char_Stack[char_top]) >= get_element_priority(input_str[i]))
                    {
                        if (get_element_priority(char_Stack[char_top]) == -1)//not valid symbol
                        {
                            free(postfix_str);
                            return 0;
                        }
                        postfix_str[j] = char_Stack[char_top--];
                        j++;
                        postfix_str[j] = ' ';
                        j++;
                        if (char_top < 0)
                            break;
                    }
                    if (input_str[i] == ')')
                    {
                        while (char_Stack[char_top] != '(')
                        {
                            if (get_element_priority(char_Stack[char_top]) == -1)//not valid symbol
                            {
                                free(postfix_str);
                                return 0;
                            }
                            postfix_str[j] = char_Stack[char_top--];
                            j++;
                            postfix_str[j] = ' ';
                            j++;
                        }
                        char_top--;
                    }
                    else
                    {
                        char_Stack[++char_top] = input_str[i];
                    }
                }
            }
        }
        if (i == length-1)//if some operators left in stack, we put them in the result string
        {
            while (char_top != -1)
            {
                if (char_Stack[char_top] != ')' && char_Stack[char_top] != '(')
                {
                    if (get_element_priority(char_Stack[char_top]) == -1)//not valid symbol
                    {
                        free(postfix_str);
                        return 0;
                    }
                    postfix_str[j] = char_Stack[char_top--];
                    j++;
                    postfix_str[j] = ' ';
                    j++;
                }
            }
        }    
    }
    if (bracket_count != 0)//проверка на колличество скобок (колличество '(' должно быть равно колличеству ')')
    {
        free(postfix_str);
        return 0;
    }
postfix_str[j] = 0;
return postfix_str;
//end of creating string in postfix format for next step(calculating)
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
}

int main(void) 
{
    char str[1002] = "";
    if (!scanf("%1001[^\n]", str)) 
    {
        fprintf(stderr, "bad input");
    }
    char *postfix_str = create_postfix_string(str); 
    if(postfix_str)
    {
        calculate_and_print(postfix_str);
        free(postfix_str);
    }
    else
    {
        printf("syntax error\n");   
    }
    
    return 0;
}
