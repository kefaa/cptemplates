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

template <typename T>
T nextInt() {
    T x = 0, p = 1;
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

const int maxN = (int)1e5 + 10;
const int maxL = 17;
const int INF = (int)1e9;

struct segmentTree {
    int *t;
    int n;

    int lastNode;

    segmentTree() {}
    segmentTree(const vector <int> &a, int last): lastNode(last) {
        n = (int)a.size();
        t = new int[4 * n];
        build(1, a, 0, n - 1);
    }

    void build(int v, const vector <int> &a, int tl, int tr) {
        if (tl == tr) {
            t[v] = a[tl];
        } else {
            build(v + v, a, tl, (tl + tr) / 2);
            build(v + v + 1, a, (tl + tr) / 2 + 1, tr);
            t[v] = max(t[v + v], t[v + 1 + v]);
        }
    }

    void modify(int pos, int x) {
        modify(1, pos, x, 0, n - 1);
    }

    void modify(int v, int pos, int x, int tl, int tr) {
        if (tl == tr) {
            t[v] = x;
            return;
        }
        int tm = (tl + tr) / 2;
        if (pos <= tm) {
            modify(v + v, pos, x, tl, tm);
        } else {
            modify(v + v + 1, pos, x, tm + 1, tr);
        }
        t[v] = max(t[v + v], t[v + v + 1]);
    }

    int getMaxOnSuffix(int p) {
        return getMax(p, n - 1);
    }

    int getMax(int l, int r) {
        if (l > r) return -INF;
        return getMax(1, l, r, 0, n - 1);
    }

    int getMax(int v, int l, int r, int tl, int tr) {
        if (l <= tl && tr <= r) {
            return t[v];
        }
        int tm = (tl + tr) / 2;
        if (r <= tm) {
            return getMax(v + v, l, r, tl, tm);
        }
        if (l > tm) {
            return getMax(v + v + 1, l, r, tm + 1, tr);
        }
        return max(
                getMax(v + v, l, r, tl, tm),
                getMax(v + v + 1, l, r, tm + 1, tr)
            );
    }
};

struct edge {
    int from;
    int to;
    int weight;
    int idx;
    edge() {}
    edge(int to, int weight, int idx): to(to), weight(weight), idx(idx) {}
    edge(int from, int to, int weight, int idx): from(from), to(to), weight(weight), idx(idx) {}
};

vector < edge > g[maxN];

int timer;
int sz[maxN];
int tin[maxN];
int tout[maxN];
int up[maxN][maxL];

int come[maxN];

bool isHeavy[maxN];
bool wasHeavyEdge[maxN];

void dfs(int v, int p, int idx) {
    tin[v] = timer++;
    sz[v] = 1;
    up[v][0] = p;
    come[v] = idx;
    for (int i = 1; i < maxL; ++i) {
        up[v][i] = up[up[v][i - 1]][i - 1];
    }

    for (size_t i = 0; i < g[v].size(); ++i) {
        edge &e = g[v][i];
        if (e.to == p) continue;
        dfs(e.to, v, e.idx);
        sz[v] += sz[e.to];
    }

    for (int i = 0; i < g[v].size(); ++i) {
        edge &e = g[v][i];
        if (e.to == p) continue;
        if (2 * sz[e.to] >= sz[v]) {
            isHeavy[e.idx] = true;
            wasHeavyEdge[v] = true;
        }
    }
    tout[v] = timer++;
}

int n;

int pathNum[maxN];
int posInPath[maxN];

vector < edge > edges;

int cntPath;
segmentTree paths[maxN];

void buildHLD() {
    timer = 0;
    dfs(1, 0, n);

    cntPath = 0;
    for (int i = 2; i <= n; ++i) {
        if (!wasHeavyEdge[i]) {
            vector < int > edges;
            int v = i, last = i;
            while(v != 1) {
                pathNum[v] = cntPath;
                posInPath[v] = (int)edges.size();
                edge &e = ::edges[come[v]];
                edges.push_back(e.weight);
                last = v;
                if (!isHeavy[e.idx]) break;
                v = up[v][0];
            }
            paths[cntPath++] = segmentTree(edges, last);
        }
    }

    pathNum[1] = cntPath;
    posInPath[1] = 0;
    paths[cntPath] = segmentTree(vector <int>(1, -1), 1);

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

int findMax(int x, int y) {
    int Xpath = pathNum[x];
    int Ypath = pathNum[y];
    int Xpos = posInPath[x];
    int Ypos = posInPath[y];
    if (Xpath == Ypath) {
        return paths[Xpath].getMax(Xpos, Ypos - 1);
    }
    int last = paths[Xpath].lastNode;
    last = up[last][0];
    return max(
            paths[Xpath].getMaxOnSuffix(Xpos),
            findMax(last, y)
        );
}

int query(int x, int y) {
    int l = lca(x, y);
    return max(findMax(x, l), findMax(y, l));
}

void modify(int v, int value) {
    int Vpath = pathNum[v];
    int Vpos = posInPath[v];
    paths[Vpath].modify(Vpos, value);
}

int getAncestor(int x, int y) {
    if (up[x][0] == y) return y;
    return x;
}

void solve() {
    for (;;) {
        //reading edges
        int x, y, w;
        x = nextInt <int>();
        y = nextInt <int>();
        w = nextInt <int>();
        g[x].push_back(edge(y, w, i));
        g[y].push_back(edge(x, w, i));
        edges.push_back(edge(x, y, w, i));
    }

    buildHLD();
    //answering queries;
}

int main() {

    return 0;
}
