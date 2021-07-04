#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef struct Tree {
    int Count; 
    unsigned char Char; 
    struct Tree* L;
    struct Tree* R;
} Tree;


typedef struct {
    unsigned char Code; 
    unsigned char NextByte;
    long long BitCount; 
    long long BitCountNext;
    FILE* File;
} BitReader;


typedef struct {
    long long BitCount; 
    long long Bits; 
} Code;


typedef struct {
    unsigned char Byte; 
    int BitCount; 
    FILE* File;
} BitWriter;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


int is_file_empty(FILE *in) {
    size_t curPos = ftell(in);
    int c;
    if ((c = fgetc(in)) == EOF) {
        return 1;
    } else {
        fseek(in, curPos, SEEK_SET);
        return 0;
    }
}

void count_freq(FILE* in, int* tab) {
    int c;
    while ((c = getc(in)) != EOF) {
        tab[c] += 1;
    }
}

Tree* new_node(int n, unsigned char c, Tree* l, Tree* r) {
    Tree* newNode = (Tree*)malloc(sizeof(Tree));
    newNode->Count = n;
    newNode->Char = c;
    newNode->L = l;
    newNode->R = r;
    return newNode;
}

void insert(Tree* node, Tree* nodes[], int num) {
    nodes[num] = node;
}

int compare(const void* p1, const void* p2) {
    int arg1 = ((*(Tree**)p1)->Count);
    int arg2 = ((*(Tree**)p2)->Count);
    return arg2 - arg1;
}

Tree* build_tree(int* tab) {
    Tree* nodes[256];
    for (int i = 0; i < 256; ++i) {
        nodes[i] = 0;
    }
    int numNodes = 0;
    for (int i = 0; i < 256; ++i) {
        if (tab[i]) {
            Tree* node = new_node(tab[i], i, NULL, NULL);
            insert(node, nodes, numNodes++);
            qsort(nodes, numNodes, sizeof(Tree*), (int (*)(const void*, const void*)) compare);
        }
    }

    while (numNodes != 1) {
        Tree* r = nodes[--numNodes];
        Tree* l = nodes[--numNodes];
        Tree* n = new_node(l->Count + r->Count, 0, l, r);
        insert(n, nodes, numNodes++);
        qsort(nodes, numNodes, sizeof(Tree*), (int (*)(const void*, const void*)) compare);
    }
    return nodes[0];
}

void set_bit(int v, long long* a, long long s) {
    long long o = s % (CHAR_BIT * sizeof(long long));
    if (v) {
        (*a) |= 1 << o;
    } else {
        (*a) &= ~(1 << o);
    }
}

void build_code(Code* code, Tree* tree, long long curBitCount, long long curBits) {
    assert(tree);
    if (tree->L && tree->R) {
        curBitCount++;
        curBits <<= 1;
        set_bit(0, &curBits, 0);
        build_code(code, tree->L, curBitCount, curBits);
        set_bit(1, &curBits, 0);
        build_code(code, tree->R, curBitCount, curBits);
    } else {
        if (!curBitCount) curBitCount++; 
        code[tree->Char].BitCount = curBitCount;
        code[tree->Char].Bits = curBits;

    }
}

void write_bits(BitWriter* bw, long long bitCount, long long bits) {
    for (int i = (bitCount - 1); i >= 0; --i) {
        int bit = !!((1 << i) & bits);
        bw->BitCount++;
        bw->Byte = bw->Byte | (bit << (CHAR_BIT - bw->BitCount));
        if (bw->BitCount == 8) {
            fputc(bw->Byte, bw->File);
            bw->Byte = 0;
            bw->BitCount = 0;
        }
    }
}

long long count_total_bits(Tree* tree, Code* code) {
    if (tree->L && tree->R) {
        return count_total_bits(tree->L, code) + count_total_bits(tree->R, code);
    }
    return tree->Count * code[tree->Char].BitCount;
}

long long count_bits_in_Tree(Tree *tree) {
    long long c = 0;
    if (tree->L && tree->R) {
        c += count_bits_in_Tree(tree->L) + count_bits_in_Tree(tree->R) + 1;
    } else {
        c += CHAR_BIT + 1;
    }
    return c;
}

void write_add_info(BitWriter* bw, Tree* tree, Code* code) {

    long long res = (count_bits_in_Tree(tree) + count_total_bits(tree, code) + 3) % 8;
    write_bits(bw, 3, res);
}

void write_tree(BitWriter* bw, Tree* tree) {

    assert(tree);
    if (tree->R && tree->L) {
        write_bits(bw, 1, 1);
        write_tree(bw, tree->L);
        write_tree(bw, tree->R);
    } else {
        write_bits(bw, 1, 0);
        write_bits(bw, CHAR_BIT, tree->Char);
    }
}

void delete_tree(Tree *t) {
    if (t->L) {
        delete_tree(t->L);
    }
    if (t->R) {
        delete_tree(t->R);
    }
    free(t);
}

void flush(BitWriter* bw) {
    if (bw->BitCount) {
        fputc(bw->Byte, bw->File);
        bw->Byte = 0;
        bw->BitCount = 0;
    }
}

int code(FILE* in, FILE *out) {
    if (is_file_empty(in)) {
        return 1;
    }
    size_t inPos = ftell(in);

    int tab[256];
    for (int i = 0; i < 256; ++i) {
        tab[i] = 0;
    }
    count_freq(in, tab);

    BitWriter bw = {0, 0, out};
    Tree* tree = build_tree(tab);
 
    Code code[256];
    memset(&code, 0, 256 * sizeof(Code));
    build_code(code, tree, 0, 0);

    write_add_info(&bw, tree, code);
    write_tree(&bw, tree);
    delete_tree(tree);
    fseek(in, inPos, SEEK_SET);
    int c;
    while (EOF != (c = getc(in))) {
        write_bits(&bw, code[c].BitCount, code[c].Bits);
    }
    flush(&bw);
    return 0;
}

int read_one_bit(BitReader* br, unsigned char* bit) {
    if (!br->BitCount) {
        if (!br->BitCountNext) { 
            br->BitCountNext = CHAR_BIT * fread(&(br->NextByte), sizeof(br->NextByte), 1, br->File);
        }
        br->Code = br->NextByte;
        br->BitCount = br->BitCountNext;
        br->BitCountNext = CHAR_BIT * fread(&(br->NextByte), sizeof(br->NextByte), 1, br->File);
    }
    br->BitCount--;
    *bit = !!(br->Code & (1 << br->BitCount));
    if (br->BitCountNext == 0) {
        return 1;
    }
    return 0;
}


unsigned char read_bits(BitReader* br, int c) {
    unsigned char tmp = 0, x = 0;
    for (int i = 0; i < c; ++i) {
        read_one_bit(br, &x);
        tmp = (tmp << 1) | x;
    }
    return tmp;
}


Tree* read_tree(BitReader* br) {
    unsigned char c = 0;
    read_one_bit(br, &c);
    if (c) {
        Tree* l = read_tree(br);
        Tree* r = read_tree(br);
        Tree* t = new_node(0, 0, l, r);
        return t;
    } else {
        Tree* t = new_node(0, read_bits(br, 8), 0, 0);
        return t;
    }
}

//проверка на дерево из одного элемента
int is_stump(Tree *t) {
    if (!t->L && !t->R) {
        return 1;
    } else {
        return 0;
    }
}

//дерево для того самого единственного элемента
Tree* create_left_child(Tree *t) {
    assert(t);
    Tree *l = t;
    Tree *newT = new_node(0, l->Char, l, 0);
    return newT;
}

//запись раскодированного сообщения в файл (и раскодировка)
int write_decode(BitReader* br, Tree* tree, int x, FILE *out) {

    assert(tree);
    unsigned char bit = 0;
    if (tree->L || tree->R) {
        int c = read_one_bit(br, &bit);
        if (c == 0 || (c == 1 && ((CHAR_BIT - x) % 8 <= br->BitCount))) {
            if (bit) {
                write_decode(br, tree->R, x, out);
            } else {
                write_decode(br, tree->L, x, out);
            }
        } else {
            return 1;
        }
    } else {
        fprintf(out, "%c", tree->Char);
    }
    return 0;
}

int decode(FILE* in, FILE *out) {
    if (is_file_empty(in)) {
        return 1;
    }
    BitReader br = {0, 0, 0, 0, in};
    unsigned char x = read_bits(&br, 3);
    Tree* tree = read_tree(&br);
    if (is_stump(tree)) {
        tree = create_left_child(tree);
    }
    while (1) {
        if (write_decode(&br, tree, x, out)) break;
    }
    delete_tree(tree);
    return 0;
}

int main() {
    FILE* in = fopen("in.txt", "rb");
    FILE* out = fopen("out.txt", "wb");
    int c = getc(in);
    getc(in);
    getc(in);
    if (c == 'c') {
        code(in, out);
    }
    else if (c == 'd') {
        decode(in, out);
    }
    else {
        fprintf(stdout, "%s", "bad input\n");
    }
    return 0;
}
