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

const int maxN = (int)3e5 + 10;
const int INF = (int)1e9;
const int mod = (int)1e9 + 7;
const ll LLINF = (ll)1e18;

typedef vector <int> perm;

vector <perm> decompose(const perm &p) {
    int n = (int)p.size();
    vector <int> was(n, -1);

    cout << "permutation: ";
    cout << "[";
    for (int i = 0; i < n; ++i) {
        cout << p[i] + 1 << ",]"[i + 1 == n];
    }
    cout << '\n';

    vector < perm > cycles;

    for(int i = 0; i < n; ++i) {
        int idx = p[i];
        if (was[idx] != -1) continue;
        perm cycle;
        while(was[idx] == -1) {
            cycle.push_back(idx);
            was[idx] = true;
            idx = p[idx];
        }
        cycles.push_back(cycle);
    }

    for(perm cycle: cycles) {
        cout << "(";
        for (size_t i = 0; i < cycle.size(); ++i) {
            cout << cycle[i] + 1;
            if (i + 1 < cycle.size()) cout << ",";
        }
        cout << ")";
    }
    cout << "\n";
    cout << "\n";
    return cycles;
}

perm random_permutation(int n) {
    perm ret(n);
    iota(ret.begin(), ret.end(), 0);
    random_shuffle(ret.begin(), ret.end());
    return ret;
}

perm operator * (const perm &p, const perm &q) {
    int n = p.size();
    perm ret(n);
    for (int i = 0; i < n; ++i) {
        ret[i] = p[q[i]];
    }
    return ret;
}

perm res;

void apply(perm &x) {
    int n = x.size();
    for (int i = 0; i < n; ++i) {
        res[x[i]] = x[(i + 1) % n];
    }
}

int main() {

    freopen(".in", "r", stdin);
    freopen(".out", "w", stdout);

    srand(time(NULL));

    int n;
    n = nextInt<int>();

    perm p(n);
    for (int i = 0; i < n; ++i) {
        p[i] = nextInt<int>() - 1;
    }

    vector <perm> cycles = decompose(p);
    sort(cycles.begin(),cycles.end(), [&](const perm &a, const perm &b) {
        return a.size() < b.size();
    });

    res = perm(n, -1);

    for (int i = 0; i < cycles.size(); ++i) {
        perm p = cycles[i];
        int n = p.size();
        perm x;
        if (n % 2 == 1) {
            for (int j = 0; j < n / 2; j ++){
                x.push_back(p[n / 2 + j]);
                x.push_back(p[j]);
            }
            x.push_back(p.back());
        } else {
            if (i + 1 == cycles.size() || cycles[i + 1].size() != n) {
                printf("%d\n", -1);
                return 0;
            }
            perm q = cycles[i + 1];
            i++;
            for (int j = 0; j < n; ++j) {
                x.push_back(p[j]);
                x.push_back(q[j]);
            }
        }
        apply(x);
    }

    for(int x: res) {
        printf("%d ", x + 1);
    }
    return 0;
}

