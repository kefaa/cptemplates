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

namespace tree {

    struct edge {
        int x, y;
        int weight;
        edge() {}
        edge(int x, int y, int w): x(x), y(y), weight(w) {}
    };

    int nextRand(int modulo) {
        int t = (rand() << 15) | rand();
        return t % modulo;
    }

    vector <edge> edges;

    int n;
    vector <int> leaves;

    void printQueries(int n) {
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (i == j) continue;
                printf("QUERY %d %d\n", i, j);
            }
        }
    }

    void generateNextTree(int n) {
        srand(time(NULL));


        int mod = (int)sqrt(n);

        int v = 1;
        leaves.push_back(1);

        while(v < n) {
            int node = leaves[nextRand(leaves.size())];
            leaves.erase(find(leaves.begin(), leaves.end(), node));
            int cntLeaves = min(n - v, nextRand(mod) + 1);
            for (int i = 0; i < cntLeaves; ++i) {
                ++v;
                int value = nextRand(1000000000);
                edges.push_back(edge(node, v, value));
                leaves.push_back(v);
            }
        }

        if (edges.size() != n - 1) {
            puts("FAILED");
            return;
        }
        cout << n << ' ' << 10* n<< '\n';
        for (int i = 0; i < n; ++i) {
            int x = rand() % 60 + 1;
            cout << x << ' ';
        }
        random_shuffle(edges.begin(), edges.end());
        for (int i = 0; i < n - 1; ++i) {
            if (rand() & 1) swap(edges[i].x, edges[i].y);
            cout << edges[i].x << ' ' << edges[i].y << ' ' << char(edges[i].weight % 26 + 'a') << '\n';
        }
        for (int o = 0; o < 10; ++o) {
            int x = rand() % n + 1;
            int y = rand() % 60 + 1;
            cout << 1 << ' ' << x << ' ' << y << '\n';
            for (int i = n - 1; i >=0; --i) {
                cout << 1 << ' ' << i+ 1 << '\n';
            }

        }
    }

}

int main() {

    freopen(".in", "w", stdout);
    cout << "1\n\n";
    //tree::generateNextTree(15);
    tree::generateNextTree(1000);
    cout << "DONE\n";
    return 0;
}
