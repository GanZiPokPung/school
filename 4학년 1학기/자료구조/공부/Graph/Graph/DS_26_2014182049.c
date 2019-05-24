#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_INIT_SLOT_SIZE 8

#define MAX_LINE_BUFFER 2048
#define WHITESPACES " \n\r\t"

typedef char VertexData;

typedef struct GraphNodeVector {
	int *ids;
	int size;
	int maxSize;
} GraphNodeVector;

void GraphNodeVector_init(GraphNodeVector *v)
{
	v->ids = NULL;
	v->size = 0;
	v->maxSize = 0;
}

void GraphNodeVector_destroy(GraphNodeVector *v)
{
	printf("Destroying GraphNodeVector of size %d\n", v->size);
	if (v->ids) free(v->ids);
}

int GraphNodeVector_addNode(GraphNodeVector *v, int id)
{
	if (v->size == v->maxSize) {
		if (v->maxSize == 0) {
			// 최초 사이즈 8부터 받아서 할당한다.
			v->maxSize = VECTOR_INIT_SLOT_SIZE;
			v->ids = (int *)malloc(v->maxSize * sizeof(int));
		}
		else {
			v->maxSize = 2 * v->size;
			v->ids = (int *)realloc(v->ids, v->maxSize * sizeof(int));
		}
	}
	v->ids[v->size] = id;
	return v->size++;
}

typedef struct _GraphVertex {
	// 버텍스 데이터
	VertexData data;
	// 노드들
	GraphNodeVector nodes;
}GraphVertex;

GraphVertex *GraphVertex_alloc(VertexData data)
{
	GraphVertex *v = (GraphVertex *)malloc(sizeof(GraphVertex));
	v->data = data;
	GraphNodeVector_init(&v->nodes);
	return v;
}

void GraphVertex_destroy(GraphVertex *v)
{
	printf("Destroying GraphVertex: %c\n", v->data);
	GraphNodeVector_destroy(&v->nodes);
	free(v);
}

typedef struct _Graph {
	//그래프 Vertex의 포인터에 대한 벡터
	//포인터의 생성 소멸을 사용자가 직접 하겠다.
	GraphVertex **vertices;
	int size;
	int maxSize;
}Graph;

void Graph_init(Graph *g)
{
	g->vertices = NULL;
	g->size = 0;
	g->maxSize = 0;
}

void Graph_destroy(Graph *g)
{
	printf("Destroying Graph of size %c\n", g->size);
	if (g->vertices) {
		// 벡터안에 존재하는 Vertex포인터들을 해제하고 
		for (int i = 0; i < g->size; ++i)
		{
			GraphVertex_destroy(g->vertices[i]);
		}
		// Vertex포인터의 벡터를 해제
		free(g->vertices);
	}
	Graph_init(g);
}

void Graph_isEmpty(Graph *g)
{
	return (0 == g->size);
}

int Graph_addVertex(Graph *g, GraphVertex *v)
{
	if (g->size == g->maxSize) {
		if (0 == g->maxSize) {
			// 새로 할당 받는 상황
			g->maxSize = VECTOR_INIT_SLOT_SIZE;
			g->vertices = (GraphVertex **)malloc(
				g->maxSize * sizeof(GraphVertex *));
		}
		else {
			// 공간을 확장해야 하는 상황
			g->maxSize = 2 * g->size;
			g->vertices = (GraphVertex **)malloc(
				g->vertices, g->maxSize * sizeof(GraphVertex *));
		}
	}
	g->vertices[g->size] = v;

	// 몇 번째 인덱스에 넣었는지 알기 위해 인덱스를 반환 
	return g->size++;
}

int Graph_addEdge(Graph *g, int from, int to)
{
	if (0 > from || g->size <= from) return 0;
	if (0 > to || g->size <= to) return 0;

	// check i from-to link already exists
	GraphVertex *v = g->vertices[from];
	GraphNodeVector_addNode(&v->nodes, to);
	return 1;
}

Graph graph;

void readFromFile(char *file)
{
	FILE *fp = fopen(file, "r");
	if (!fp) {
		printf("File not open: %s\n", file);
		return;
	}
	// 기존 그래프 해제
	Graph_destroy(&graph);
	char buffer[MAX_LINE_BUFFER];
	while (fgets(buffer, MAX_LINE_BUFFER, fp)) {
		char *ch = strtok(buffer, WHITESPACES);
		if (!ch) {
			printf("ignoring empty line\n");
			continue;
		}
		if (*ch == '#') {
			printf("ignoring the line starting with #\n");
			continue;
		}
		if (strlen(ch) != 1) {
			printf("Only 1 char node can be added\n");
			continue;
		}
		// 그래프 추가
		GraphVertex *v = GraphVertex_alloc(*ch);
		Graph_addVertex(&graph, v);
		printf("Added vector %c:\n", *ch);
		while (1) {
			char *p = strtok(NULL, WHITESPACES);
			if (!p || !*p) break;
			int id;
			if (1 != sscanf(p, "%d", &id)) {
				printf(" ignoring non-number argument (%s) and thereafter\n", p);
				break;
			}
			// 노드 추가
			GraphNodeVector_addNode(&v->nodes, id);
			printf(" added node to %d\n", id);
		}
	}
	fclose(fp);
}

int main(void)
{
	// 그래프 초기화
	Graph_init(&graph);

	while (1) {
		char buffer[MAX_LINE_BUFFER];
		if (!fgets(buffer, MAX_LINE_BUFFER, stdin)) {
			break;
		}
		// strtok string tokenizing 
		// 빈칸 캐리지 리턴 탭 등이 나오면 그곳에 널 캐릭터를 넣어준다.
		// Whitespace가 아닌 곳의 포인터를 리턴
		// 앞쪽 
		// buffer이후 어딘가의 포인터

		char *cmd = strtok(buffer, WHITESPACES);
		if (!cmd || !*cmd) {
			// whitespace와 이것이 아닌것도 못찾았다.
			// 사용자의 입력이 없었으므로 다시.
			continue;
		}
		// 작동원리 감을 잡기 위한 코드 116줄
		printf("command: %s buf=%p cmd=%p buf=%s\n", cmd, buffer, cmd, buffer);
		if (strcmp(cmd, "load") == 0) {
			char *file = strtok(NULL, WHITESPACES);
			printf("filename: %s\n", file);
			readFromFile(file);
			continue;
		}
		if (!strcmp(cmd, "v")) {
			char *ch = strtok(NULL, WHITESPACES);
			if (strlen(ch) != 1) {
				printf("Only 1 char node can be added\n");
				continue;
			}
			 GraphVertex *v = GraphVertex_alloc(*ch);
			 int index = Graph_addVertex(&graph, v);
			 printf("Vertex added at index %d\n", index);
			continue;
		}
		if (!strcmp(cmd, "e")) {
			// char *p = strtok(NULL, WHITESPACES);
			// int n1, n2;
			// int read = sscanf(p, "%d%d", &n1, &n2);
			// if (read != 2) {
			//     printf("Two indices should be provided (returned %d, p=%s)\n", read, p);
			//     continue;
			// }
			char *p1 = strtok(NULL, WHITESPACES);
			char *p2 = strtok(NULL, WHITESPACES);
			if (!p1 || !p2) {
				printf("Two indices should be provided\n");
				continue;
			}
			int n1, n2, read = 0;
			if (sscanf(p1, "%d", &n1) == 1) {
				read++;
			}
			if (sscanf(p2, "%d", &n2) == 1) {
				read++;
			}
			if (read != 2) {
				printf("Two indices should be provided\n");
				continue;
			}
			if (n1 == n2) {
				printf("Two indices should differ\n");
				continue;
			}
			int success = Graph_addEdge(&graph, n1, n2);
			printf("Adding edge from %d to %d %s\n", n1, n2, success ? "succeeded" : "failed");
			continue;
		}
		if (!strcmp(cmd, "reset")) {
			Graph_destroy(&graph);
			continue;
		}
		if (strcmp(cmd, "q") == 0) {
			break;
		}
		printf("Unknown command: %s\n", cmd);
	}
	return 0;
}