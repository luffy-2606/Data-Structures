#include <iostream>
using namespace std;

const int MAXN = 50;

struct Node {
   int v, w;
   Node* next;
};

Node* graph[MAXN];
bool visited[MAXN];
int parent[MAXN];
int low[MAXN], disc[MAXN];
bool cycleFound = false;
int timer = 0;
int N;

void addEdge(int u, int v, int w) {
   Node* a = new Node;
   a->v = v; 
   a->w = w;
   a->next = graph[u];
   graph[u] = a;

   Node* b = new Node;
   b->v = u; 
   b->w = w;
   b->next = graph[v];
   graph[v] = b;
}

void resetVisited() {
   for (int i = 0; i < N; i++) 
       visited[i] = false;
}

void dfsCycle(int u, int p) {
   visited[u] = true;
   Node* t = graph[u];
   while (t) {
       if (!visited[t->v]) 
           dfsCycle(t->v, u);
       else if (t->v != p) 
           cycleFound = true;
       t = t->next;
   }
}

void dfsConn(int u) {
   visited[u] = true;
   Node* t = graph[u];
   while (t) {
       if (!visited[t->v]) 
           dfsConn(t->v);
       t = t->next;
   }
}

void findBridges(int u) {
   visited[u] = true;
   disc[u] = low[u] = timer++;
   Node* t = graph[u];
   while (t) {
       int v = t->v;
       if (!visited[v]) {
           parent[v] = u;
           findBridges(v);
           low[u] = (low[u] < low[v]) ? low[u] : low[v];
           if (low[v] > disc[u])
               cout << "Bridge: " << u << " - " << v << endl;
       }
       else if (v != parent[u])
           low[u] = (low[u] < disc[v]) ? low[u] : disc[v];
       t = t->next;
   }
}

int minDistance(int dist[], bool used[]) {
   int min = 100000, idx = -1;
   for (int i = 0; i < N; i++) {
       if (!used[i] && dist[i] < min) {
           min = dist[i];
           idx = i;
       }
   }
   return idx;
}

void dijkstra(int src, int dest) {
   int dist[MAXN];
   bool used[MAXN];
   for (int i = 0; i < N; i++) {
       dist[i] = 100000;
       used[i] = false;
   }
   dist[src] = 0;
   for (int i = 0; i < N - 1; i++) {
       int u = minDistance(dist, used);
       if (u == -1) break;
       used[u] = true;
       Node* t = graph[u];
       while (t) {
           if (dist[u] + t->w < dist[t->v])
               dist[t->v] = dist[u] + t->w;
           t = t->next;
       }
   }
   if (dist[dest] == 100000) cout << "No path exists\n";
   else cout << "Shortest time: " << dist[dest] << endl;
}

int main() {
   int E;
   cout << "Enter number of intersections: ";
   cin >> N;
   cout << "Enter number of roads: ";
   cin >> E;

   for (int i = 0; i < N; i++) 
       graph[i] = NULL;

   for (int i = 0; i < E; i++) {
       int u, v, w;
       cin >> u >> v >> w;
       addEdge(u, v, w);
   }

   int src, dest;
   cout << "Enter source and destination for Dijkstra: ";
   cin >> src >> dest;
   dijkstra(src, dest);

   resetVisited();
   cycleFound = false;
   dfsCycle(0, -1);
   if (cycleFound) 
       cout << "Cycle exists\n";
   else 
       cout << "No cycles\n";

   resetVisited();
   int components = 0;
   for (int i = 0; i < N; i++) {
       if (!visited[i]) {
           components++;
           dfsConn(i);
       }
   }
   if (components == 1) 
       cout << "Graph is connected\n";
   else 
       cout << "Graph is NOT connected, components: " << components << endl;

   resetVisited();
   for (int i = 0; i < N; i++) {
       parent[i] = -1;
       low[i] = disc[i] = 0;
   }
   timer = 0;
   cout << "Critical Roads:\n";
   for (int i = 0; i < N; i++) {
       if (!visited[i]) 
           findBridges(i);
   }

   return 0;
}
