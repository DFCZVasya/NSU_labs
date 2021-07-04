#include <stdlib.h>
#include <stdio.h>
#define MAX_VERTICES 5000
#define INF 2147483648
#define INTMAX 2147483647

void print_answer(int from, int to, FILE* out) {
	fprintf(out, "%d %d\n", from, to);
}

int get_min(long long int* distance, int N) {
	long long int min = INF;
	int	tmp = 0;
	for (int i = 0; i < N; ++i) {
		if (distance[i] && min > distance[i]) {
			min = distance[i];
			tmp = i;
		}
	}
	return tmp;
}

void build_span(int* graph, long long int* distance, int vertice, int N, int *vertex_arr, int step, FILE* out) {
	long long int edge = 0, min = 0;
	long long int tmp = INF;
	for (int i = vertice; i < N; ++i) {
		edge = graph[N * vertice + i - (vertice * (vertice + 1)) / 2];
		if (edge && distance[i] && (distance[i] > edge)) {
			distance[i] = edge;
			vertex_arr[i] = vertice;
			if (tmp > edge) {
				tmp = edge;
				min = i;
			}
		}
	}
	for (int j = 0; j < vertice; ++j) {
		edge = graph[N * j + vertice - (j * (j + 1)) / 2];
		if (edge && distance[j] && (distance[j] > edge)) {
			distance[j] = edge;
			vertex_arr[j] = vertice;
			if (tmp > edge) {
				tmp = edge;
				min = j;
			}
		}
	}
	graph[N * vertice + min - (vertice * (vertice + 1)) / 2] = 0;
	if (min) {
		print_answer(vertex_arr[min] + 1, min + 1, out);
		distance[min] = 0;
		step++;
		build_span(graph, distance, min, N, vertex_arr, step, out);
	}
	if (step != N - 1) {
		min = get_min(distance, N);
		if (min) {
			print_answer(vertex_arr[min] + 1, min + 1, out);
			distance[min] = 0;
			step++;
			build_span(graph, distance, min, N, vertex_arr, step, out);
		}
	}
	return;
}

int main() {
	FILE* in = fopen("in.txt", "r");
	FILE* out = fopen("out.txt", "w");
	
	int N = 0, M = 0; 
	int from = 0, to = 0;
	long long int weight = 0;
	if (fscanf(in, "%d %d", &N, &M)) {
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
		if (!M || M < N - 1) {
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
	int* graph = (int*)calloc(N * N - (N * (N - 1)) / 2, sizeof(int));
	int lines_counter = 0;
	while (fscanf(in, "%d %d %lld", &from, &to, &weight) != EOF) {
		if (0 > from || from > N || 0 > to || to > N) {
			fprintf(out, "bad vertex");
			fclose(in);
			fclose(out);
			free(graph);
			return 0;
		}
		if (0 > weight || weight > INTMAX) {
			fprintf(out, "bad length");
			fclose(in);
			fclose(out);	
			free(graph);
			return 0;
		}
		int idx;
		if (from > to) {
			idx = (to - 1) * N + (from - 1) - ((to - 1) * to) / 2 ;
		}
		else {
			idx = (from - 1) * N + (to - 1) - ((from - 1) * from) / 2;
		}
		graph[idx] = weight;
		lines_counter++;
	}
	if (lines_counter != M) {
		fprintf(out, "bad number of lines");
		fclose(in);
		fclose(out);
		free(graph);
		return 0;
	}
	long long int* distance = (long long int*)malloc(N * sizeof(long long int));
	for (int i = 1; i < N; ++i) {
		distance[i] = INF;
	}
	int step = 0;
	int spanned_vertice = 0;
	distance[0] = 0;
	int* vertex_arr = (int*)calloc(N, sizeof(int));
	build_span(graph, distance, spanned_vertice, N, vertex_arr, step, out);

	for (int i = 0; i < N; ++i) {
		if (distance[i]) {
			fclose(out);
			out = fopen("out.txt", "w");
			fprintf(out, "no spanning tree\n");
			break;
		}
	}
	fclose(in);
	fclose(out);
	free(graph);
	free(distance);
	free(vertex_arr);
	return 0;
}
