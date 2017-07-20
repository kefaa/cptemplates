#include <functional>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <numeric>
#include <iomanip>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <math.h>
#include <queue>
#include <stack>
#include <ctime>
#include <set>
#include <map>

using namespace std;

typedef long long ll;

int nextInt() {
    int x = 0, p = 1;
    char ch;
    do { ch = getchar(); } while(ch <= ' ');
    if (ch == '-') {
        p = -1;
        ch = getchar();
    }
    while(ch >= '0' && ch <= '9') {
        x = x * 10 + (ch - '0');
        ch = getchar();
    }
    return x * p;
}

const int maxN = 100500;
const int maxL = 17;
const int INF = (int)1e9;

struct segmentTree {

    int n;
    int *t;

    segmentTree() {}
    segmentTree(int n): n(n) {
        t = new int[4 * n];
        for (int i = 0; i < 4 * n; ++i) {
            t[i] = 0;
        }
    }

    void modify(int pos, int val) {
        modify(1, pos, val, 0, n - 1);
    }

    void modify(int v, int pos, int val, int tl, int tr) {
        if (tl == tr) {
            t[v] += val;
            return;
        }
        int tm = (tl + tr) / 2;
        if (pos <= tm) {
            modify(v + v, pos, val, tl, tm);
        } else {
            modify(v + v + 1, pos, val, tm + 1, tr);
        }
        t[v] = max(t[v + v], t[v + v + 1]);
    }

    int queryOnSuffix(int r) {
        return query(r, n - 1);
    }

    int query(int l, int r) {
        if (l > r) return -INF;
        return query(1, l, r, 0, n - 1);
    }

    int query(int v, int l, int r, int tl, int tr) {
        if (l <= tl && tr <= r) {
            return t[v];
        }
        int tm = (tl + tr) / 2;
        if (r <= tm) {
            return query(v + v, l, r, tl, tm);
        }
        if (l > tm) {
            return query(v + v + 1, l, r, tm + 1, tr);
        }
        return max(
                query(v + v, l, r, tl, tm),
                query(v + v + 1, l, r, tm + 1, tr)
                );
    }

};

enum types { LIGHT, HEAVY };

vector <int> g[maxN];

int timer;
int sz[maxN];
int tin[maxN];
int tout[maxN];
int up[maxN][maxL];
types typeOfEdge[maxN];

void dfs(int v, int p) {
    tin[v] = timer++;
    sz[v] = 1;
    up[v][0] = p;
    for (int i = 1; i < maxL; ++i) {
        up[v][i] = up[up[v][i - 1]][i - 1];
    }

    for (size_t i = 0; i < g[v].size(); ++i) {
        int x = g[v][i];
        if (x == p) continue;
        dfs(x, v);
        sz[v] += sz[x];
    }

    for (size_t i = 0; i < g[v].size(); ++i) {
        int x = g[v][i];
        if (x == p) continue;
        typeOfEdge[x] = (2 * sz[x] >= sz[v] ? HEAVY : LIGHT);
    }

    tout[v] = timer++;
}

bool isAncestor(int x, int y) {
    return tin[x] <= tin[y] && tout[x] >= tout[y];
}

int lca(int x, int y) {
    if (isAncestor(x, y)) return x;
    if (isAncestor(y, x)) return y;
    for (int i = maxL - 1; i >= 0; --i) {
        if (up[x][i] != 0 && !isAncestor(up[x][i], y)) {
            x = up[x][i];
        }
    }
    return up[x][0];
}

int n, q;

int pathNumber[maxN];
int posInPath[maxN];

int lastInPath[maxN];
segmentTree path[maxN];

void buildHLD() {
    dfs(1, 0);

    int curPath = 0;
    for (int v = 1; v <= n; ++v) {
        bool wasHeavyEdges = false;
        for (size_t i = 0; i < g[v].size(); ++i) {
            int x = g[v][i];
            if (isAncestor(v, x)) {
                if (typeOfEdge[x] == HEAVY) {
                    wasHeavyEdges = true;
                }
            }
        }
        if (wasHeavyEdges) continue;

        int curPos = 0;
        int curVertex = v;
        while(true) {
            pathNumber[curVertex] = curPath;
            posInPath[curVertex] = curPos++;
            lastInPath[curPath] = curVertex;
            if (typeOfEdge[curVertex] == LIGHT) break;
            curVertex = up[curVertex][0];
        }
        path[curPath++] = segmentTree(curPos);
    }
}

void modify(int v, int x) {
    int a = pathNumber[v];
    int b = posInPath[v];
    path[a].modify(b, x);
}

int getMax(int x, int y) {
    int a = pathNumber[x];
    int b = pathNumber[y];
    int l = posInPath[x];
    int r = posInPath[y];

    if (a == b) {
        return path[a].query(l, r);
    }

    x = up[lastInPath[a]][0];
    return max(
              path[a].queryOnSuffix(l),
              getMax(x, y)
            );
}

int query(int x, int y) {
    int l = lca(x, y);
    return max(
              getMax(x, l),
              getMax(y, l)
            );
}

int main() {

    n = nextInt();
    for (int i = 0; i < n - 1; ++i) {
        int x, y;
        x = nextInt();
        y = nextInt();
        g[x].push_back(y);
        g[y].push_back(x);
    }

    buildHLD();

    q = nextInt();
    while(q--) {

        char qq = getchar();
        int x, y;
        x = nextInt();
        y = nextInt();
        if (qq == 'I') {
            modify(x, y);
        } else {
            printf("%d\n", query(x, y));
        }
    }
    return 0;
}
