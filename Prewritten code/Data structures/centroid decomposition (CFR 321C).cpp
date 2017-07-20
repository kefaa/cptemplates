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

const int N = 100500;
const int INF = (int)1e9;
const int mod = (int)1e9 + 7;

vector <int> g[N];
int level[N];
int sz[N];
int anc[N];

void calcSizes(int v, int p, vector <int> &nodes) {
    nodes.push_back(v);
    anc[v] = p;
    sz[v] = 1;
    for (size_t i = 0; i < g[v].size(); ++i) {
        int x = g[v][i];
        if (x == p) continue;
        calcSizes(x, v, nodes);
        sz[v] += sz[x];
    }
}

void findCentroid(int v, int n, int lev) {
    vector <int> nodes;
    calcSizes(v, -1, nodes);

    int centroid = v, maxSubTree = INF;
    for (size_t i = 0; i < nodes.size(); ++i) {
        int x = nodes[i];
        int curMaxSubTree = -INF;
        for (size_t j = 0; j < g[x].size(); ++j) {
            int y = g[x][j];
            if (y == anc[x]) {
                curMaxSubTree = max(curMaxSubTree, n - sz[x]);
            } else {
                curMaxSubTree = max(curMaxSubTree, sz[y]);
            }
        }
        if (curMaxSubTree < maxSubTree) {
            maxSubTree = curMaxSubTree;
            centroid = x;
        }
    }

    level[centroid] = lev;
    vector <int> adj = g[centroid];
    g[centroid].clear();
    for (size_t i = 0; i < adj.size(); ++i) {
        int x = adj[i];
        g[x].erase(find(g[x].begin(), g[x].end(), centroid));
        if (x == anc[centroid]) {
            findCentroid(x, n - sz[centroid], lev + 1);
        } else {
            findCentroid(x, sz[x], lev + 1);
        }
    }
}

int main() {

    int n;
    n = nextInt <int>();

    for (int i = 0; i < n - 1; ++i) {
        int x, y;
        x = nextInt <int>() - 1;
        y = nextInt <int>() - 1;
        g[x].push_back(y);
        g[y].push_back(x);
    }

    findCentroid(0, n, 0);

    for (int i = 0; i < n; ++i) {
        putchar(char('A' + level[i]));
        putchar(i < n - 1 ? ' ' : '\n');
    }
    return 0;
}
