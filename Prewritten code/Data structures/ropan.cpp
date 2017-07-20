#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstdio>

using namespace std;

template <typename T>
T myFunction(T a, T b) {
    return a + b;
}

template <class T>
struct segmentTree {

    const static int N = 100500;

    int n;
    T t[4 * N];

    segmentTree(int n, T *a): n(n) {
        build(1, a, 0, n - 1);
    }

    void build(int v, T *a, int l, int r) {
        if (l == r) {
            t[v] = a[l];
        } else {
            build(v + v, a, l, (l + r) / 2);
            build(v + v + 1, a, (l + r) / 2 + 1, r);
            t[v] = myFunction(t[v + v], t[v + v + 1]);
        }
    }

    T get(int l, int r) {
        return get(1, l, r, 0, n - 1);
    }

    T get(int v, int l, int r, int tl, int tr) {
        if (tl >= l && tr <= r) {
            return t[v];
        }
        int tm = (tl + tr) / 2;
        if (l > tm) {
            return get(v + v + 1, l, r, tm + 1, tr);
        }
        if (r <= tm) {
            return get(v + v, l, r, tl, tm);
        }
        return myFunction(get(v + v, l, r, tl, tm), get(v + v + 1, l, r, tm + 1, tr));
    }

    void modifyElement(int pos, T val) {
        modifyElement(1, pos, val, 0, n - 1);
    }

    void modifyElement(int v, int pos, T val, int tl, int tr) {
        if (tl == tr) {
            //t[v] += val;
            //t[v] = val;
            return;
        }
        int tm = (tl + tr) / 2;
        if (pos <= tm) {
            modifyElement(v + v, pos, val, tl, tm);
        } else {
            modifyElement(v + v + 1, pos, val, tm + 1, tr);
        }
        t[v] = myFunction(t[v + v], t[v + v + 1]);
    }

};

int main() {

    return 0;
}
