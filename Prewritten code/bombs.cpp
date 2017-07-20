#include <functional>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <numeric>
#include <cassert>
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

const int maxN = (int)4e5 + 10;
const int maxL = 20;
const int INF = (int)2e9 + 42;
const int mod = (int)1e9 + 7;

int n;

vector <int> g[maxN];
vector <int> gr[maxN];

void addEdge(int x, int y) {
  //  cout << x << ' ' << y << '\n';
    g[x].push_back(y);
    gr[y].push_back(x);
}

int num[maxN];
int renum[maxN];
int isLeaf[maxN];

void add(int v, int l, int r, int x, int tl, int tr) {
    if (l <= tl && tr <= r) {
        addEdge(num[x], v);
      //  addEdge(v, x + 4 * n);
        return;
    }
    int tm = (tl + tr) / 2;
    if (l <= tm) {
        add(v + v, l, r, x, tl, tm);
    }
    if (r > tm) {
        add(v + v + 1, l, r, x, tm + 1, tr);
    }
}

struct my {
    ll x, r, idx;
    my() {}
    my(ll x): x(x), r(-INF), idx(-INF) {}
    bool operator < (const my& a) const {
        return x < a.x;
    }
};

my a[maxN];

int used[maxN];

vector <int> order;

void dfsRev(int v) {
    used[v] = 1;
   // cout << v << '\n';
    for (int x: gr[v]) {
        if (!used[x]) {
            dfsRev(x);
        }
    }
    order.push_back(v);
}

set <int> s[maxN];

int curComp = 1;
int dpL[maxN];
int dpR[maxN];
int was[maxN];
int compNum[maxN];

void dfs(int x) {
    compNum[x] = curComp;
    if (isLeaf[x]) {
        dpL[curComp] = min(dpL[curComp], renum[x]);
        dpR[curComp] = max(dpR[curComp], renum[x]);
    }
    used[x] = 1;
    for (int v: g[x]) {
        if (!used[v]) dfs(v);
    }
}

void solve(int x) {
    if (was[x]) return;
    was[x] = true;
    for (int y: s[x]) {
        solve(y);
        dpL[x] = min(dpL[x], dpL[y]);
        dpR[x] = max(dpR[x], dpR[y]);
       // cout << x << ' ' << dpL[x] << ' ' << dpR[x] << '\n';
    }
}

void scc() {
    for (int i = 1; i <= 4 * n; ++i) {
        if (!used[i]) {
            dfsRev(i);
        }
    }
    reverse(order.begin(), order.end());
    for (int i = 0; i <= 4 * n; ++i) {
        dpL[i] = INF;
        dpR[i] = -INF;
        used[i] = 0;
    }
    for (int x: order) {
        if (used[x]) continue;
        dfs(x);
        curComp++;
    }
    for (int i = 1;i <= 4 * n; ++i) {
        for (int x: g[i]) {
            int a = compNum[i];
            int b = compNum[x];
            if (a != b && !s[a].count(b)) {
                s[a].insert(b);
            }
        }
    }
    for (int i = 1; i < curComp; ++i) {
        solve(i);
    }
}

int answer[maxN];

void init() {
    for (int i = 0; i <= 4 * n; ++i) {
        answer[i] = 0;
        used[i] = 0;
        dpL[i] = INF;
        dpR[i] = -INF;
        g[i].clear();
        gr[i].clear();
        s[i].clear();
        num[i] = 0;
        renum[i] = 0;
        a[i] = my();
        compNum[i] = 0;
        isLeaf[i] = 0;
        was[i] = 0;
    }
        curComp = 1;
    order.clear();
}

void calcNum(int v,int tl, int tr) {
    if (tl == tr){
        num[tl] = v;
        renum[v] = tl;
        isLeaf[v] = 1;
        return;
    }
    int tm = (tl + tr) / 2;
    calcNum(v + v, tl, tm);
    calcNum(v + v + 1, tm + 1, tr);;
}

void solve() {
    init();
    n = nextInt<int>();
    for (int i =1 ; i <= n; ++i) {
        a[i].x = nextInt<ll>();
        a[i].r = nextInt<ll>();
        a[i].idx = i;
    }

    calcNum(1, 1, n);

    sort(a + 1, a + 1 + n);
    for (int i =1; i <= n; ++i) {
        int l = lower_bound(a + 1, a + 1 + n, my(a[i].x - a[i].r)) - a;
        int r = upper_bound(a + 1, a + 1 + n, my(a[i].x + a[i].r)) - a; --r;
        add(1, l, r, i, 1, n);
        //cout << l << ' ' << r << '\n';
    }

    for (int i = 1; i <= 4 * n; ++i) {
        if (2 * i <= 4 * n) addEdge(i, 2* i);
        if (2* i + 1 <= 4 * n) addEdge(i, 1 + 2 * i);
    }

    scc();

    for (int i = 1; i <= n; ++i) {
        answer[a[i].idx] = dpR[compNum[num[i]]] - dpL[compNum[num[i]]] + 1;
    }
    for (int i = 1; i <= n; ++i) {
        printf("%d ", answer[i]);
    }
    printf("\n");
}

int main() {

   // freopen(".in", "r", stdin);
   // freopen(".out", "w", stdout);

    int T;
    T = nextInt<int>();
    while(T--)
    solve();
    return 0;
}
