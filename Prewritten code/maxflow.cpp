#include <functional>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <numeric>
#include <iomanip>
#include <stdio.h>
#include <cstring>
#include <cassert>
#include <bitset>
#include <vector>
#include <math.h>
#include <queue>
#include <stack>
#include <ctime>
#include <set>
#include <map>

using namespace std;

typedef long long ll;
typedef long double ld;

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

const int maxN = (int)3e2 + 10;
const int INF = (int)1e9;
const int mod = (int)1e9 + 7;
const ll LLINF = (ll)1e18;
const ld pi = acos(-1.0);

int n, m;
int S, T;

struct edge {
    int to;
    int cap;
    int flow;
    edge() {}
    edge(int to, int flow, int cap): to(to), flow(flow), cap(cap) {}
};

vector <edge> edges;
vector <int> g[maxN];

void addEdge(int x, int y, int cap) {
    g[x].push_back((int)edges.size());
    edges.push_back(edge(y, 0, cap));
    g[y].push_back((int)edges.size());
    edges.push_back(edge(x, 0, 0));
}

int d[maxN];
int ptr[maxN];

bool bfs() {
    memset(d, -1, sizeof d);
    queue <int> q;
    q.push(S);
    d[S] = 0;
    while(!q.empty() && d[T] == -1) {
        int v = q.front(); q.pop();
        for (auto idx: g[v]) {
            edge e = edges[idx];
            if (e.cap > e.flow) {
                if (d[e.to] == -1) {
                    d[e.to] = d[v] + 1;
                    q.push(e.to);
                }
            }
        }
    }
    return d[T] == -1 ? false : true;
}

int dfs(int v, int flow) {
    if (v == T || flow == 0) return flow;
    for (int &idx = ptr[v]; idx < g[v].size(); ++idx) {
        int e = g[v][idx];
        if (d[edges[e].to] == d[v] + 1) {
            int ret = dfs(edges[e].to, min(flow, edges[e].cap - edges[e].flow));
            if (ret == 0) continue;
            edges[e].flow += ret;
            edges[e ^ 1].flow -= ret;
            return ret;
        }
    }
    return 0;
}

int maxFlow() {
    int ret = 0;
    while(bfs()) {
        memset(ptr, 0, sizeof ptr);
        int curFlow = dfs(S, INF);
        if (curFlow == 0) break;
        ret += curFlow;
    }
    return ret;
}

int main() {

   // freopen(".in", "r", stdin);
    //freopen(".out", "w", stdout);

    return 0;
}
