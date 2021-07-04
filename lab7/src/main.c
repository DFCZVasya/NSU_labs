#include <stdio.h>
#include <stdlib.h>
#define WHITE 0
#define GREY 1
#define BLACK 2

typedef struct Stack
{
    int *answer;
    int length;
} Stack;

void init(Stack *stack, int vertices)
{
    stack->answer = (int *)calloc(vertices, sizeof(int));
    stack->length = -1;
}

void free_stack(Stack *stack)
{
    free(stack->answer);
}

void free_and_close_all(Stack *stack, unsigned char *graph, char *flag_array, FILE* in, FILE* out) {
    free_stack(stack);
    free(graph);
    free(flag_array);
    fclose(in);
    fclose(out);
}

int sort(Stack *stack,unsigned char *graph, char *colors, int n, int i)
{
    if (colors[i] == BLACK)
        return 0;

    if (colors[i] == GREY)
    {
        printf("impossible to sort");
        return 1;
    }

    if (colors[i] == WHITE)
    {
        colors[i] = GREY;
        for (int j = 0; j < n; ++j)
        {
            int nb = i * n + j;
            if (graph[nb / 8] & (1 << nb % 8))
            {
                sort(stack, graph, colors, n, j);
            }
        }
    }

    colors[i] = BLACK;
    stack->answer[++(stack->length)] = i + 1;
    return 0;
}

int top_sort(Stack *stack, unsigned char *graph, char *colors, int n) {
    for (int i = 0; i < n; ++i)
    {
        if (sort(stack, graph, colors, n, i))
            return 0;
    }
    return 1;
}

int main() {
    FILE* in = fopen("in.txt", "r");
    FILE* out = fopen("out.txt", "w");
    int n = 0, m = 0, from = 0, to = 0;
    if (fscanf(in, "%i", &n) == EOF) {
        fprintf(out, "bad number of lines");
        fclose(in);
        fclose(out);
        return 0;
    }
    if (fscanf(in, "%i", &m) == EOF) {
        fprintf(out, "bad number of lines");
        fclose(in);
        fclose(out);
        return 0;
    }
    if (n < 0 || n > 2000) {
        fprintf(out, "bad number of vertices");
        fclose(in);
        fclose(out);
        return 0;
    }
    if (m < 0 || m > n * (n - 1) / 2) {
        fprintf(out, "bad number of edges");
        fclose(in);
        fclose(out);
        return 0;
    }


    char* colors = (char*)malloc(n * sizeof(char));
    for (int i = 0; i < n; ++i) {
        colors[i] = 0;
    }
    
    int lines_counter = 2;
    Stack stack;
    init(&stack, n);

    unsigned char *graph = (unsigned char*)malloc(sizeof(unsigned char) * ((n * n) / 8 + 1));
    for (int i = 0; i < ((n * n) / 8 + 1); i++)
    {
        graph[i] = 0;
    }

    while (fscanf(in, "%i %i", &from, &to) != EOF) {
        if (1 > from || from > n || 1 > to || to > n) {
            fprintf(out, "bad vertex");
            free_and_close_all(&stack, graph, colors, in, out);
            return 0;
        }
        if (from == to) {
            fprintf(out,"impossible to sort");
            free_and_close_all(&stack, graph, colors, in, out);
            return 0;
        }
        lines_counter++;
        int nb = (from - 1) * n + (to - 1);
        graph[nb / 8] |= (1 << (nb % 8));
    }

    

    if (lines_counter != (m + 2)) {
        fprintf(out, "bad number of lines");
        free_and_close_all(&stack, graph, colors, in, out);
        return 0;
    }
    if (!m) {
        for (int i = 0; i < n; ++i) {
            fprintf(out, "%d ", i + 1);
        }
        free_and_close_all(&stack, graph, colors, in, out);
        return 0;
    }
    
    if (top_sort(&stack, graph, colors, n)) {
        for (int i = stack.length; i >= 0; --i)
        {
            printf("%d ", stack.answer[i]);
        }
        free_and_close_all(&stack, graph, colors, in, out);
        return 0;
    }
    free_and_close_all(&stack, graph, colors, in, out);
    return 0;
} 
