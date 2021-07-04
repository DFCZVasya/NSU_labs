#include <stdlib.h>
#include <stdio.h>


typedef struct Node // структура для представления узлов дерева
{
    int key;
    unsigned char height; //возможно поможет уменьшить успользование памяти. в 4 раза меньше как-никак 
    struct Node *left;
    struct Node *right;
}Node;

unsigned char height(Node *p)
{
    return p ? p->height : 0;
}

int bfactor(Node *p)
{
    return height(p->right) - height(p->left);
}

void fixheight(Node *p)
{
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

Node* rotateright(Node *p) // правый поворот вокруг p
{
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

Node* rotateleft(Node *q) // левый поворот вокруг q
{
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

Node* balance(Node *p) // балансировка узла p
{
    fixheight(p);
    if (bfactor(p) == 2)
    {
        if (bfactor(p->right) < 0)
        {
            p->right = rotateright(p->right);
        }
        return rotateleft(p);
    }
    if (bfactor(p) == -2)
    {
        if (bfactor(p->left) > 0)
        {
            p->left = rotateleft(p->left);
        }
        return rotateright(p);
    }
    return p; // балансировка не нужна
}

Node* insert(Node *p, int k, Node *nodes, int node_number) // вставка ключа k в дерево с корнем p
{
    if (!p)
    {
        Node *n = nodes + node_number;
        n->key = k;
        n->height = 1;
        n->left = n->right = 0;
        return n;
    }
    if (k < p->key)
    {
        p->left = insert(p->left, k, nodes, node_number);
    }
    else
    {
        p->right = insert(p->right, k, nodes, node_number);
    }
    return balance(p);
}


int main(void) 
{
    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-Чтение входных данных+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
    int count;
    if(!scanf("%d", &count))
    {
        fprintf(stderr, "bad input");
    }
    Node *nodes = (Node*)malloc(sizeof(Node) * count);
    
    Node *head = 0;
    for (int i = 0; i < count; i++)
    {
        int value;
        if(!scanf("%i", &value))
        {
            fprintf(stderr, "bad input");
        }
        head = insert(head, value, nodes, i);
    }
    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-
    
    printf("%d\n", height(head));
    free(nodes);
    return 0;
}
