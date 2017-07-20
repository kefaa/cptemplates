#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <numeric>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <vector>
#include <math.h>
#include <queue>
#include <stack>
#include <ctime>
#include <set>
#include <map>

using namespace std;

typedef long long ll;
typedef double ld;

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
const int INF = (int)1e9;
const int mod = (int)1e9 + 7;
const ll LLINF = (ll)1e18 + 5;

int q;

ll h[maxN];
int c[maxN];
ll pw[maxN];

int n = 0;

ll BASE = 31;
ll lcp_sum = 0;

bool equalHashes(int x, int y, int m) {
    if (m == 0) return true;
    ll h1 = h[x] - h[x - m];
    ll h2 = h[y] - h[y - m];
    if (x < y) {
        h1 = h1 * pw[y - x];
    } else {
        h2 = h2 * pw[x - y];
    }
    return h1 == h2;
}

int lcp(int x, int y) {
    if (!equalHashes(x, y, 1)) return 0;
    int l = 1, r = min(x, y);
    for (int i = 0; i < 17; ++i) {
        int m = (l + r) / 2;
        if (equalHashes(x, y, m)) {
            l = m;
        } else {
            r = m;
        }
    }
    l = max(l - 3, 0);
    for (; ; l++) {
        if (l + 1 > min(x, y) || !equalHashes(x, y, l + 1)) {
            return l;
        }
    }
    return l;
}

struct cmp {
    bool operator ()(int x, int y) {
        int l = lcp(x, y);
        return c[x - l] < c[y - l];
    }
};

set <int, cmp> s;

void pop() {
    auto iter = s.find(n);
    --n;
    if (iter != s.begin()) {
        lcp_sum -= lcp(*iter, *prev(iter));
    }
    if (next(iter) != s.end()) {
        lcp_sum -= lcp(*iter, *next(iter));
    }
    if (iter != s.begin() && next(iter) != s.end()) {
        lcp_sum += lcp(*prev(iter), *next(iter));
    }
    s.erase(iter);
}

void add(int x) {
    c[++n] = x;
    h[n] = h[n - 1] + c[n] * pw[n];
    auto iter = s.insert(n).first;
    if (iter != s.begin()) {
        lcp_sum += lcp(*iter, *prev(iter));
    }
    if (next(iter) != s.end()) {
        lcp_sum += lcp(*iter, *next(iter));
    }
    if (iter != s.begin() && next(iter) != s.end()) {
        lcp_sum -= lcp(*prev(iter), *next(iter));
    }
}

void print() {
    for (int i = n; i <= n; ++i) {
        for(int j = i; j >= 1; --j) {
            putchar(char(c[j] + 'a' - 1));
        }
        putchar('\n');
    }
    if (n == 3) {
        cout << lcp(1, 3) << endl;
        cout << lcp(2, 3) << endl;
    }
}

int main() {

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    ios_base::sync_with_stdio(0);
    pw[0] = 1;
    for(int i = 1; i < maxN; ++i) {
        pw[i] = pw[i - 1] * BASE;
    }
    char c;
    for(int i = 0; cin >> c; ++i) {
        if(c == '-') {
            pop();
        } else {
            add(c - 'a' + 1);
        }
        cout << n * 1LL * (n + 1) / 2 - lcp_sum << '\n';
    }
    return 0;
}
