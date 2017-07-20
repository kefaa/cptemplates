#include <iostream>
#include <cstdio>

template <class T>
struct fenwickTree {

    int n;
    T *ft;

    fenwickTree(int n): n(n) {
        ft = new T[n];
    }

    void add(int r, T val) {
        while(r < n) {
            ft[r] = (ft[r] + val);
            r |= r + 1;
        }
    }

    T sum(int r) {
        T ret = 0;
        while(r >= 0) {
            ret = (ret + ft[r]);
            r = (r & (r + 1)) - 1;
        }
        return ret;
    }

    T sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }
};

int main() {

    fenwickTree<long long> t(10);
    t.add(5, 8);
    t.add(4, -3);
    std::cout << t.sum(3, 6);
    return 0;
}
