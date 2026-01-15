#include <iostream>
using namespace std;

struct Point {
    int x, y;
};

const int MAXN = 50;
char grid[MAXN][MAXN];
bool visited[MAXN][MAXN];
Point parent[MAXN][MAXN];
int N, M;

int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

bool isValid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < M && !visited[x][y] && grid[x][y] != '1' && grid[x][y] != 'F';
}

int bfs(Point start, Point end) {
    Point queue[MAXN * MAXN];
    int front = 0, back = 0;
    queue[back++] = start;
    visited[start.x][start.y] = true;
    parent[start.x][start.y] = { -1,-1 };

    while (front < back) {
        Point cur = queue[front++];
        if (cur.x == end.x && cur.y == end.y) {
            // reconstruct path
            Point path[MAXN * MAXN];
            int len = 0;
            Point p = cur;
            while (p.x != -1) {
                path[len++] = p;
                p = parent[p.x][p.y];
            }
            cout << "Escape possible in " << len - 1 << " steps\n";
            cout << "Path (coordinates):\n";
            for (int i = len - 1; i >= 0; i--) {
                cout << "(" << path[i].x << "," << path[i].y << ") ";
            }
            cout << "\n";
            return len - 1;
        }
        for (int d = 0; d < 4; d++) {
            int nx = cur.x + dx[d];
            int ny = cur.y + dy[d];
            if (isValid(nx, ny)) {
                visited[nx][ny] = true;
                parent[nx][ny] = cur;
                queue[back++] = { nx,ny };
            }
        }
    }
    cout << "No Escape\n";
    return -1;
}

int main() {
    cin >> N >> M;
    Point start, end;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> grid[i][j];
            if (grid[i][j] == 'S') start = { i,j };
            if (grid[i][j] == 'E') end = { i,j };
            visited[i][j] = false;
        }
    }

    bfs(start, end);
    return 0;
}
