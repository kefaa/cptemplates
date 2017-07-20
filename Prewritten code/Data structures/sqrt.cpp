#include <bits/stdc++.h>

using namespace std;

template <typename T>
struct SQRTDecomposition {

    const static int BLOCK_SIZE = 3;

    struct BLOCK {
        T add;
        T sum;
        T a[BLOCK_SIZE];

        void Add(int l, int r, T x) {
            l %= BLOCK_SIZE;
            r %= BLOCK_SIZE;
            for (int i = l; i <= r; ++i) {
                a[i] += x;
                sum += x;
            }
        }

        T Sum(int l, int r) {
            l %= BLOCK_SIZE;
            r %= BLOCK_SIZE;
            T res = (r - l + 1) * add;
            for (int i = l; i <= r; ++i) {
                res += a[i];
            }
            return res;
        }

        BLOCK(): add(0), sum(0) {}
    };

    int n;
    int CNT_BLOCKS;
    vector <BLOCK> blocks;

    void buildDecomposition(int n, T a[]) {
        int i = -1, j = 0, it = 0;
        while(it < n) {
            if (j == 0) {
                blocks.push_back(BLOCK());
                i++;
            }
            blocks[i].a[j] = a[it];
            blocks[i].sum += a[it++];
            if (++j >= BLOCK_SIZE) j -= BLOCK_SIZE;
        }
    }

    SQRTDecomposition() {}
    SQRTDecomposition(int n, T a[]): n(n) {
        buildDecomposition(n, a);
    }

    void add(int l, int r, T x) {
        int L = l / BLOCK_SIZE;
        int R = r / BLOCK_SIZE;
        if (L == R) {
            blocks[L].Add(l, r, x);
            return;
        }
        blocks[L].Add(l, BLOCK_SIZE - 1, x);
        blocks[R].Add(0, r, x);
        for (int i = L + 1; i < R; ++i) {
            blocks[i].add += x;
        }
    }

    T sum(int l, int r) {
        int L = l / BLOCK_SIZE;
        int R = r / BLOCK_SIZE;
        if (L == R) {
            return blocks[L].Sum(l, r);
        }
        T res = blocks[L].Sum(l, BLOCK_SIZE - 1) + blocks[R].Sum(0, r);
        for (int i = L + 1; i < R; ++i) {
            res += BLOCK_SIZE * blocks[i].add + blocks[i].sum;
        }
        return res;
    }

};

int NextInt() {
    return rand() % 12 + 1;
}

namespace foo {

    int n;
    int a[100500];

    void print() {
        puts("");
        for (int i = 0; i < n; ++i) {
            printf("%d%c", a[i], " \n"[i + 1 == n]);
        }
        puts("");
    }

    void add(int l, int r, int x) {
        for (int i = l; i <= r; ++i) a[i] += x;
    }

    int sum(int l, int r) {
        int ret = 0;
        for (int i = l; i <= r; ++i) ret += a[i];
        return ret;
    }

    void bar() {
        srand(time(NULL));
        n = rand() % 8 + 5;
        for (int i = 0; i < n; ++i) {
            a[i] = NextInt();
        }
        print();

        SQRTDecomposition <int> s(n, a);

        int q = 20;
        while(q --) {
             int l = rand() % n;
             int r = rand() % n;
             if (l > r) swap(l, r);
             int t = rand() % 2;
             if (t == 0) { //add x in [l; r]
                int x = NextInt();
                printf("add %d from %d to %d\n", x, l + 1, r + 1);
                s.add(l, r, x);
                add(l, r, x);
                printf("new array = ");
                print();
            } else { //find sum in [l; r]
                printf("sum from %d to %d = %d\n", l + 1, r + 1, s.sum(l, r));
                printf("naive solution is %s to SQRT\n", (s.sum(l, r) == sum(l, r) ? "equal" : "not equal"));
            }

            puts("");
        }
    }

};
int main() {

    foo::bar();
    return 0;
}
