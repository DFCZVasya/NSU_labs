#include <stdlib.h>
#include <stdio.h>

void swap (int* a, int* b) 
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


void sift_down(int *numbers, int root, int bottom)
{
	int max_child = 0;
	while (root * 2 + 1 < bottom) {
		if ((root * 2 + 1 == bottom - 1) || (numbers[root * 2 + 1] > numbers[root * 2 + 2]))
			max_child = root * 2 + 1;
		else
			max_child = root * 2 + 2;
		if (numbers[root] < numbers[max_child]) {
			swap(&numbers[root], &numbers[max_child]);
			root = max_child;
		}
		else
			break;
	}
}

void heap_sort(int *numbers, int array_size)
{
	for (int i = array_size / 2 - 1; i >= 0; --i)
		sift_down(numbers, i, array_size);
	for (int i = array_size - 1; i > 0; --i) {
		swap(&numbers[0], &numbers[i]);
		sift_down(numbers, 0, i);
	}
}

int main(void) {
    
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
    
    heap_sort(array, array_size);

    for (int i = 0; i < array_size; i++)
    {
        printf("%i ", array[i]);
    }
    free(array);
    //system("pause");
    return 0;

}
