#include <cstdio>
#include <cstring>
using namespace std;

const int MAXN = 100010;
const int MAXM = 200020;

struct Edge {
    int to, next, w;
    bool valid;
};

Edge edges[MAXM];
int head[MAXN];
int cnt;

void init() {
    memset(head, -1, sizeof(head));
}

void add(int from, int to, int weight) {
    edges[++cnt] = {to, head[from], weight, true};
    head[from] = cnt;
}

void remove(int from, int to) {
    for (int i = head[from]; i != -1; i = edges[i].next) {
        if (edges[i].to == to && edges[i].valid == true) {
            edges[i].valid = false;
            return;
        }
    }
}


void bfs(int start) {
    bool visited[MAXN] = {false};
    int q[MAXN];
    int front = 0, rear = 0;

    q[rear++] = start;
    visited[start] = true;

    while (front < rear) {
        int u = q[front++];
        printf("%d ", u);

        for (int i = head[u]; i != -1; i = edges[i].next) {
            if (!edges[i].valid) continue;
            int v = edges[i].to;
            if (!visited[v]) {
                visited[v] = true;
                q[rear++] = v;
            }
        }
    }
}


bool dfs_visited[MAXN];

void dfs(int u) {
    dfs_visited[u] = true;
    printf("%d ", u);

    for (int i = head[u]; i != -1; i = edges[i].next) {
        if (!edges[i].valid) continue;
        int v = edges[i].to;
        if (!dfs_visited[v]) {
            dfs(v);
        }
    }
}

int main() {
    init();

    add(1, 2, 1);
    add(2, 1, 1);
    add(2, 4, 1);
    add(4, 2, 1);
    add(3, 4, 1);
    add(4, 3, 1);

    printf("BFS from 1: \n");
    bfs(1);

    printf("\nDFS from 1: \n");
    dfs(1);
    return 0;
}