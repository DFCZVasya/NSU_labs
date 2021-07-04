#include <stdio.h>
#include <stdlib.h>


void swap (int* a, int* b) 
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}



void quick_sort(int *arr, int length) 
{
    int subArray = 0, top = -1;
    int stack[length+1];
    stack[++top] = 0;
    stack[++top] = length - 1;
    do 
    {
        int right = stack[top--];
        int left = stack[top--];
        --subArray;
        do 
        {
            int _left = left;
            int _right = right;
            int pivot = arr[(left + right) / 2];
            do 
            {
                while (pivot < arr[_right]) 
                {
                    _right--;
                }
                while (pivot > arr[_left]) 
                {
                    _left++;
                }
                if (_left <= _right) 
                {
                    if (_left != _right) 
                    {
                        swap(&arr[_left], &arr[_right]);
                    }
                    _right--;
                    _left++;
                }
            } while (_right >= _left);
            if (_left < right) 
            {
                ++subArray;
                stack[++top] =_left;
                stack[++top] = right;
            }
            right = _right;
        } while (left < right);
    } while (subArray > -1);
}

int main(void) 
{
    int array_size;
    if(!scanf("%i", &array_size))
    {
        fprintf(stderr, "input error");        
    }
    int *array = (int*)malloc(array_size * sizeof(int));

    for (int i = 0; i < array_size; i++)
    {
        if (!scanf("%i", &array[i]))
        {
            fprintf(stderr, "input error");
        }
    }
    
    quick_sort(array, array_size);

    for (int i = 0; i < array_size; i++)
    {
        printf("%i ", array[i]);
    }
    free(array);
    return 0;
}
