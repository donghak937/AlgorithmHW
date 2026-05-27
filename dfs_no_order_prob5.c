#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 10 // Maximum number of nodes

// Enum for vertex colors during DFS
typedef enum {
    WHITE, // Not visited
    GRAY,  // Currently in recursion stack
    BLACK  // Finished processing
} COLOR;

// Structure for a node in the adjacency list
typedef struct AdjListNode {
    char label;
    struct AdjListNode* next;
} AdjListNode;

// Structure for the graph
typedef struct Graph {
    char nodeLabels[MAX_NODES]; // Array to store node labels ('A', 'B', 'C' ...)
    AdjListNode* adj[MAX_NODES]; // Adjacency list head pointer array
    int numVertices;
} Graph;

// Function to create a new graph
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    for (int i = 0; i < MAX_NODES; i++) {
        graph->adj[i] = NULL;
        graph->nodeLabels[i] = '\0'; // Initialize label array
    }
    graph->numVertices = 0;
    return graph;
}

// Function to add a vertex to the graph
void addVertex(Graph* graph, char label) {
    if (graph->numVertices < MAX_NODES) {
        // Check if the label already exists
        for (int i = 0; i < graph->numVertices; i++) {
            if (graph->nodeLabels[i] == label) {
                printf("Vertex %c already exists.\n", label);
                return;
            }
        }
        graph->nodeLabels[graph->numVertices] = label;
        graph->numVertices++;
    } else {
        printf("Maximum number of nodes reached.\n");
    }
}

// Function to find the index of a vertex by its label
int getVertexIndex(Graph* graph, char label) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->nodeLabels[i] == label) {
            //printf("index is %d\n", i);
            return i;
        }
    }
    return -1; // Not found
}

// Function to add an edge
void addEdge(Graph* graph, char srcLabel, char destLabel) {
    int srcIndex = getVertexIndex(graph, srcLabel);
    int destIndex = getVertexIndex(graph, destLabel);

    if (srcIndex == -1 || destIndex == -1) {
        printf("Invalid vertex label.\n");
        return;
    }

    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->label = destLabel;

    // Check if edge already exists
    for (AdjListNode* current = graph->adj[srcIndex]; current != NULL; current = current->next) {
        if (current->label == destLabel) {
            free(newNode);
            return;
        }
    }

    // Insert in alphabetical order
    AdjListNode* prev = NULL;
    AdjListNode* curr = graph->adj[srcIndex];
    while (curr != NULL && curr->label < destLabel) {
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        newNode->next = graph->adj[srcIndex];
        graph->adj[srcIndex] = newNode;
    } else {
        newNode->next = curr;
        prev->next = newNode;
    }
}

// Function to free the memory allocated for the graph
void freeGraph(Graph* graph) {
    for (int i = 0; i < MAX_NODES; i++) {
        AdjListNode* current = graph->adj[i];
        while (current != NULL) {
            AdjListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}

// Function to print the graph (adjacency list representation)
void printGraph(Graph* graph) {
    printf("Adjacency List:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%c: ", graph->nodeLabels[i]);
        AdjListNode* current = graph->adj[i];
        while (current != NULL) {
            printf("%c -> ", current->label);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Depth First Search Visit function
void DFS_VISIT(Graph* graph, int u, COLOR color[], int* time, int d[], int f[]) {

  // Write code here
      // 1. u를 처음 방문
    color[u] = GRAY;
    (*time)++;
    d[u] = *time;

    // 2. u의 인접 정점들을 확인
    AdjListNode* current = graph->adj[u];

    while (current != NULL) {
        int v = getVertexIndex(graph, current->label);

        // 아직 방문 안 한 정점이면 DFS 재귀 호출
        if (v != -1 && color[v] == WHITE) {
            DFS_VISIT(graph, v, color, time, d, f);
        }

        current = current->next;
    }

    // 3. u의 모든 인접 정점 처리가 끝남
    color[u] = BLACK;
    (*time)++;
    f[u] = *time;
}

// Depth First Search function
void DFS(Graph* graph, COLOR color[], int* time, int d[], int f[]) {

  // Write code here
    // 1. 모든 정점 초기화
    for (int i = 0; i < graph->numVertices; i++) {
        color[i] = WHITE;
        d[i] = -1;
        f[i] = -1;
    }

    // time 초기화
    *time = 0;

    // 2. 모든 정점을 알파벳 순서대로 돌면서 DFS 시작
    for (char c = 'A'; c < 'A' + graph->numVertices; c++) {
        int idx = getVertexIndex(graph, c);
        if (idx != -1 && color[idx] == WHITE) {
            DFS_VISIT(graph, idx, color, time, d, f);
        }
    }
}

int main() {

    int size;
    Graph* graph = createGraph();
    printf("Enter the number of nodes: ");
    scanf(" %d", &size);

    // Add nodes
    for (int i = 0; i < size; i++) {
        addVertex(graph, 'A' + i);
    }

    addEdge(graph, 'A', 'H');
    addEdge(graph, 'B', 'A');
    addEdge(graph, 'B', 'D');
    addEdge(graph, 'B', 'F');
    addEdge(graph, 'C', 'D');
    addEdge(graph, 'C', 'F');
    addEdge(graph, 'D', 'A');
    addEdge(graph, 'D', 'G');
    addEdge(graph, 'D', 'H');
    addEdge(graph, 'D', 'I');
    addEdge(graph, 'E', 'I');

    printGraph(graph);
    printf("\n");

    COLOR color[MAX_NODES];
    int time = 0;
    int d[MAX_NODES]; // Discovery times
    int f[MAX_NODES]; // Finishing times

    DFS(graph, color, &time, d, f);

    printf("\nDFS Results:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Vertex %c: Discovery Time = %d, Finishing Time = %d, Color = ",
               graph->nodeLabels[i], d[i], f[i]);
        if (color[i] == WHITE) printf("WHITE");
        else if (color[i] == GRAY) printf("GRAY");
        else if (color[i] == BLACK) printf("BLACK");
        printf("\n");
    }

    freeGraph(graph);

    return 0;
}
