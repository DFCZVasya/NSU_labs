#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAX_VERTICES 5000

typedef struct Edge_ {
	short from, to;
	int weight;
} Edge;

void swap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int compare(const void* a, const void* b) {
	return((*(Edge*)a).weight - (*(Edge*)b).weight);
}

void make_set(int vertice, int *rank, int *parent) {
	parent[vertice] = vertice;
	rank[vertice] = 0;
}

int find_set(int vertice, int *rank, int *parent) {
	if (parent[vertice] == vertice)
		return vertice;
	return parent[vertice] = find_set(parent[vertice], rank, parent);
}

void combine_sets(int a, int b, int *rank, int *parent) {
	a = find_set(a, rank, parent);
	b = find_set(b, rank, parent);
	if (a != b) {
		if (rank[a] < rank[b]) {
			swap(&a, &b);
		}
		parent[b] = a;
		if (rank[a] == rank[b]) {
			rank[a]++;
		}
	}
}

int main() {
	FILE* in = fopen("in.txt", "r");
	FILE* out = fopen("out.txt", "w");
	int parent[MAX_VERTICES];
	int rank[MAX_VERTICES];
	short N = 0, from = 0, to = 0;
	int  weight = 0, M = 0;
	if (fscanf(in, "%hd %d", &N, &M)) {
		if (0 > N || N > MAX_VERTICES) {
			fprintf(out, "bad number of vertices");
			fclose(in);
			fclose(out);
			return 0;
		}
		if (0 > M || M > (N * (N - 1)) / 2) {
			fprintf(out, "bad number of edges");
			fclose(in);
			fclose(out);
			return 0;
		}
		if (!M && N == 1) {
			fclose(in);
			fclose(out);
			return 0;
		}
		if (!M) {
			fprintf(out, "no spanning tree");
			fclose(in);
			fclose(out);
			return 0;
		}
	}
	else {
		fprintf(stderr, "so bad input");
		fclose(in);
		fclose(out);
		return 0;
	}

	Edge* graph = (Edge*)malloc(sizeof(Edge) * M);
	int lines_counter = 0;
	int index = 0;
	while (fscanf(in, "%hd %hd %d", &from, &to, &weight) != EOF) {
		if (0 > from || from > N || 0 > to || to > N) {
			fprintf(out, "bad vertex");
			fclose(in);
			fclose(out);
			free(graph);
			return 0;
		}
		if (0 > weight || weight > INT_MAX) {
			fprintf(out, "bad length");
			fclose(in);
			fclose(out);
			free(graph);
			return 0;
		}
		lines_counter++;
		graph[index].from = from - 1;
		graph[index].to = to - 1;
		graph[index].weight = weight;
		index++;
	}
	if (lines_counter != (M)) {
		fprintf(out, "bad number of lines");
		fclose(in);
		fclose(out);
		free(graph);
		return 0;
	}
	int spanned_vertices = 1;
	qsort(graph, M, sizeof(Edge), compare);
	for (int i = 0; i < N; ++i)
		make_set(i, rank, parent);
	for (int i = 0; i < M; ++i) {
		Edge curent_Edge = graph[i];
		if (find_set(curent_Edge.from, rank, parent) != find_set(curent_Edge.to, rank, parent)) {
			fprintf(out, "%d %d\n", curent_Edge.from + 1, curent_Edge.to + 1);
			combine_sets(curent_Edge.from, curent_Edge.to, rank, parent);
			spanned_vertices++; 	
		}
	}
	if (spanned_vertices != N) {
		fclose(out);
		out = fopen("out.txt", "w");
		fprintf(out, "no spanning tree\n");
	}
	fclose(in);
	fclose(out);
	free(graph);
	return 0;
}
