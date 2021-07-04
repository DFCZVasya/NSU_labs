#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define MAX_VERTICES 5000


int find_next_vertice(long long* distance, int* vertex_marker, int N) {
	int tmp = -1;
	long long min = LLONG_MAX;
	for (int i = 0; i < N; ++i) {
		if (!vertex_marker[i] && min > distance[i]) {
			min = distance[i];
			tmp = i;
		}
	}
	return tmp;
}

void print_min_weights(long long *distance, int N) {
	for(int i = 0; i < N; ++i){
		if (distance[i] == LLONG_MAX) {
			printf("oo ");
		}
		else if (distance[i] > INT_MAX) {
			printf("INT_MAX+ ");
		}
		else {
			printf("%lld ", distance[i]);
		}
	}
	printf("\n");
}

void find_min_weights(int* graph, long long* distance, int* vertex_marker, int* min_vertex_way , int vertice, int N, int step) {
	int tmp = 0;
	for (int i = 0; i < N; ++i) {
		long long edge = graph[vertice * N + i];
		if (edge && !vertex_marker[i] && (distance[i] > edge)) {
			distance[i] = distance[vertice] + edge;
			min_vertex_way[i] = vertice;
		}
	}
	vertex_marker[vertice] = 1;
	tmp = find_next_vertice(distance, vertex_marker, N);
	step++;
	if (step != N && tmp != -1) {
		find_min_weights(graph, distance, vertex_marker, min_vertex_way, tmp, N, step);
	}
	return;
}

void free_all(int *a, long long *b, int *c)
{
	free(a); free(b); free(c);
}

void Dijkstra(int *graph, int N, int F, int S)
{
	long long* distance = (long long*)calloc(N, sizeof(long long));
	for (int i = 0; i < N; ++i) {
		distance[i] = LLONG_MAX;
	}
	int* min_vertex_way = (int*)malloc(N * sizeof(int));
	for (int i = 0; i < N; ++i) {
		min_vertex_way[i] = -1;
	}
	min_vertex_way[S - 1] = S - 1;
	distance[S - 1] = 0;
	int step = 0;
	int* vertex_marker = (int*)calloc(N, sizeof(int));
	
	find_min_weights(graph, distance, vertex_marker, min_vertex_way, (S - 1), N, step);
	print_min_weights(distance, N);
	if (distance[F - 1] > INT_MAX && distance[F - 1] != LLONG_MAX) {
		int overflow_counter = 0;
		for (int i = 0; i < N; ++i) {
			int edge = graph[N * (F - 1) + i];
			if (edge && (distance[i] + edge > INT_MAX)) {
				++overflow_counter;
			}
			if (overflow_counter > 1) {
				printf("overflow\n");
				free_all(min_vertex_way, distance, vertex_marker);
				return;
			}
		}
	}
	int tmp = F - 1;
	while (tmp != S - 1) {	
		if (min_vertex_way[tmp] == -1) {
			printf("no path\n");
			free_all(min_vertex_way, distance, vertex_marker);
			return;
		}
		printf("%d ", tmp + 1);
		tmp = min_vertex_way[tmp];
	}
	printf("%d ", tmp + 1);
	free_all(min_vertex_way, distance, vertex_marker);
} 


int main() {
	int N = 0, S = 0, F = 0, M = 0, from = 0, to = 0;
	long long weight = 0;
	if (scanf("%d %d %d %d", &N, &S, &F, &M)) {
		if (0 > N || N > MAX_VERTICES) {
			printf("bad number of vertices");
			return 0;
		}
		if (0 > M || M > (N * (N - 1)) / 2) {
			printf("bad number of edges");
			return 0;
		}
		if (1 > S || S > N || 1 > F || F > N) {
			printf("bad vertex");
			return 0;
		}
	}
	else {
		fprintf(stderr, "so bad input");
		return 0;
	}
	int* graph = (int*)calloc(N * N, sizeof(int));
	int lines_counter = 0;
	while (scanf("%d %d %lld", &from, &to, &weight) != EOF) {
		if (1 > from || from > N || 1 > to || to > N) {
			printf("bad vertex");
			free(graph);
			return 0;
		}
		if (0 > weight || weight > INT_MAX) {
			printf("bad length");
			free(graph);
			return 0;
		}
		*(graph + (from - 1) * N + (to - 1)) = weight;
		*(graph + (to - 1) * N + (from - 1)) = weight;
		lines_counter++;
	}
	if (lines_counter != M) {
		printf("bad number of lines");
		free(graph);
		return 0;
	}
	
	Dijkstra(graph, N, F, S);
	free(graph);
	return 0;
}
