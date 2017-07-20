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

const int N = 100500;
const int INF = (int)1e9;

const int L = 20;

vector <int> g[N];
int timer;
int tin[N];
int tout[N];
int depth[N];
int up[N][L];

void dfs(int v, int p) {
    tin[v] = timer++;
    depth[v] = depth[p] + 1;
    up[v][0] = p;
    for (int i = 1; i < L; ++i) {
        up[v][i] = up[up[v][i - 1]][i - 1];
    }
    for (size_t i = 0; i < g[v].size(); ++i) {
        int x = g[v][i];
        if (x == p) continue;
        dfs(x, v);
    }
    tout[v] = timer++;
}

bool upper(int x, int y) {
    return tin[x] <= tin[y] && tout[x] >= tout[y];
}

int lca(int x, int y) {
    if (upper(x, y)) return x;
    if (upper(y, x)) return y;
    for (int i = L - 1; i >= 0; --i) {
        if (!upper(up[x][i], y)) {
            x = up[x][i];
        }
    }
    return up[x][0];
}

int go(int v, int depth) {
    for (int i = 0; i < L; ++i) {
        if ((depth >> i) & 1) {
            v = up[v][i];
        }
    }
    return v;
}

int main() {


    return 0;
}
