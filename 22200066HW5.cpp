// Lecture slide ch22 94 -> 98
// Blog: https://yabmoons.tistory.com/409
// Blog: https://velog.io/@kimdukbae/%EC%9C%84%EC%83%81-%EC%A0%95%EB%A0%AC-Topological-Sorting
// Blog: https://m.blog.naver.com/ndb796/221236874984
// Blog: https://oi.readthedocs.io/en/latest/ds&algo/others/topological_sort.html


#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

#define MAX_NODES 10 // Maximum number of nodes

// Enum for vertex colors during DFS
enum COLOR {
  WHITE, // Not visited
  GRAY,  // Currently in recursion stack
  BLACK  // Finished processing
};

// Structure for a node in the adjacency list
struct AdjListNode {
  char label;
  AdjListNode *next;
};

// Structure for the graph
struct Graph {
  char nodeLabels[MAX_NODES];  // Array to store node labels ('A', 'B', 'C' ...)
  AdjListNode *adj[MAX_NODES]; // Adjacency list head pointer array
  int numVertices;
};

// Function to create a new graph
Graph *createGraph() {
  Graph *graph = new Graph();
  for (int i = 0; i < MAX_NODES; i++) {
    graph->adj[i] = nullptr;
    graph->nodeLabels[i] = '\0'; // Initialize label array
  }
  graph->numVertices = 0;
  return graph;
}

// Function to add a vertex to the graph
void addVertex(Graph *graph, char label) {
  if (graph->numVertices < MAX_NODES) {
    // Check if the label already exists
    for (int i = 0; i < graph->numVertices; i++) {
      if (graph->nodeLabels[i] == label) {
        cout << "Vertex " << label << " already exists." << endl;
        return;
      }
    }
    graph->nodeLabels[graph->numVertices] = label;
    graph->numVertices++;
  } else {
    cout << "Maximum number of nodes reached." << endl;
  }
}

// Function to find the index of a vertex by its label
int getVertexIndex(Graph *graph, char label) {
  for (int i = 0; i < graph->numVertices; i++) {
    if (graph->nodeLabels[i] == label) {
      return i;
    }
  }
  return -1; // Not found
}

// Function to add an edge
void addEdge(Graph *graph, char srcLabel, char destLabel) {
  int srcIndex = getVertexIndex(graph, srcLabel);
  int destIndex = getVertexIndex(graph, destLabel);

  if (srcIndex == -1 || destIndex == -1) {
    cout << "Invalid vertex label." << endl;
    return;
  }

  // Check if edge already exists
  AdjListNode *curr = graph->adj[srcIndex];
  while (curr != nullptr) {
    if (curr->label == destLabel) {
      return;
    }
    curr = curr->next;
  }

  AdjListNode *newNode = new AdjListNode{destLabel, nullptr};

  // Insert in alphabetical order to satisfy constraint:
  // "The adjacency list must store neighbors of each vertex in alphabetical
  // order."
  AdjListNode *prev = nullptr;
  curr = graph->adj[srcIndex];
  while (curr != nullptr && curr->label < destLabel) {
    prev = curr;
    curr = curr->next;
  }

  if (prev == nullptr) {
    newNode->next = graph->adj[srcIndex];
    graph->adj[srcIndex] = newNode;
  } else {
    newNode->next = curr;
    prev->next = newNode;
  }
}

// Function to free the memory allocated for the graph
void freeGraph(Graph *graph) {
  for (int i = 0; i < MAX_NODES; i++) {
    AdjListNode *current = graph->adj[i];
    while (current != nullptr) {
      AdjListNode *temp = current;
      current = current->next;
      delete temp;
    }
  }
  delete graph;
}

// Function to print the graph (adjacency list representation)
void printGraph(Graph *graph) {
  cout << "Adjacency List:" << endl;
  for (int i = 0; i < graph->numVertices; i++) {
    cout << graph->nodeLabels[i] << ": ";
    AdjListNode *current = graph->adj[i];
    while (current != nullptr) {
      cout << current->label << " -> ";
      current = current->next;
    }
    cout << "NULL" << endl;
  }
}

// Depth First Search Visit function
void DFS_VISIT(Graph *graph, int u, COLOR color[], int &time, int d[], int f[],
               vector<char> &topoOrder) {
  // 1. u를 처음 방문
  color[u] = GRAY;
  time++;
  d[u] = time;

  // 2. u의 인접 정점들을 확인 (이미 알파벳 순서대로 정렬되어 있음)
  AdjListNode *current = graph->adj[u];
  while (current != nullptr) {
    int v = getVertexIndex(graph, current->label);
    // 아직 방문 안 한 정점이면 DFS 재귀 호출
    if (v != -1 && color[v] == WHITE) {
      DFS_VISIT(graph, v, color, time, d, f, topoOrder);
    }
    current = current->next;
  }

  // 3. u의 모든 인접 정점 처리가 끝남
  color[u] = BLACK;
  time++;
  f[u] = time;

  // Topological sort order stores vertices when they finish
  topoOrder.push_back(graph->nodeLabels[u]);
}

// Depth First Search function
void DFS(Graph *graph, COLOR color[], int &time, int d[], int f[],
         vector<char> &topoOrder) {
  // 1. 모든 정점 초기화
  for (int i = 0; i < graph->numVertices; i++) {
    color[i] = WHITE;
    d[i] = -1;
    f[i] = -1;
  }

  // time 초기화
  time = 0;

  // 2. DFS traversal starts specifically from vertex 'A'
  int idxA = getVertexIndex(graph, 'A');
  if (idxA != -1 && color[idxA] == WHITE) {
    DFS_VISIT(graph, idxA, color, time, d, f, topoOrder);
  }

  // 3. 모든 정점을 알파벳 순서대로 돌면서 방문하지 않은 노드에 대해 DFS 시작
  for (char c = 'A'; c < 'A' + graph->numVertices; c++) {
    int idx = getVertexIndex(graph, c);
    if (idx != -1 && color[idx] == WHITE) {
      DFS_VISIT(graph, idx, color, time, d, f, topoOrder);
    }
  }
}

int main() {
  Graph *graph = createGraph();

  int size;
  cout << "Enter the number of nodes: ";
  cin >> size;

  for (int i = 0; i < size; i++) {
    addVertex(graph, 'A' + i);
  }

  // Add edges corresponding to the DAG in Homework 5 diagram
  addEdge(graph, 'A', 'B');
  addEdge(graph, 'C', 'B');
  addEdge(graph, 'C', 'E');
  addEdge(graph, 'C', 'H');
  addEdge(graph, 'D', 'A');
  addEdge(graph, 'D', 'B');
  addEdge(graph, 'E', 'B');
  addEdge(graph, 'E', 'G');
  addEdge(graph, 'F', 'A');
  addEdge(graph, 'F', 'G');
  addEdge(graph, 'G', 'D');
  addEdge(graph, 'H', 'G');

  // Print adjacency list
  printGraph(graph);

  COLOR color[MAX_NODES];
  int time = 0;
  int d[MAX_NODES]; // Discovery times
  int f[MAX_NODES]; // Finishing times
  vector<char> topoOrder;

  // Run DFS
  DFS(graph, color, time, d, f, topoOrder);

  // DFS 결과 출력 (발견 및 완료 시간)
  cout << endl << "DFS Results:" << endl;
  for (int i = 0; i < graph->numVertices; i++) {
    cout << "Vertex " << graph->nodeLabels[i]
         << ": Discovery Time = " << d[i]
         << ", Finishing Time = " << f[i]
         << ", Color = ";
    if (color[i] == WHITE) cout << "WHITE";
    else if (color[i] == GRAY) cout << "GRAY";
    else if (color[i] == BLACK) cout << "BLACK";
    cout << endl;
  }

  // 위상 정렬 순서 결정 (DFS 탐색이 끝난 정점을 역순으로 배치)
  reverse(topoOrder.begin(), topoOrder.end());

  // 최종 위상 정렬 결과 출력
  cout << endl << "Topological Sort:" << endl;
  for (size_t i = 0; i < topoOrder.size(); ++i) {
    cout << topoOrder[i];
    if (i < topoOrder.size() - 1) {
      cout << " -> ";
    }
  }
  cout << endl;

  freeGraph(graph);

  return 0;
}
