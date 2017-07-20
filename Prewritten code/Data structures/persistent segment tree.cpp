#include <iostream>
#include <cstdio>

using namespace std;

int n;

struct Node {

    Node *l, *r;
    int val;

    Node() {}
    Node(int val): l(0), r(0), val(val) {}
    Node(Node *l, Node *r): l(l), r(r), val(l->val + r->val) {}
    Node(int l, int r): val(0) {
        if (l != r) {
            this->l = new Node(l, (l + r) / 2);
            this->r = new Node((l + r) / 2 + 1, r);
        }
    }

    Node* modify(int pos) {
        return modify(pos, 0, n - 1);
    }

    Node* modify(int pos, int tl, int tr) {
        if (tl == tr) {
            return new Node(val + 1);
        }
        int tm = (tl + tr) / 2;
        if (pos <= tm) {
            return new Node(l->modify(pos, tl, tm), r);
        }
        return new Node(l, r->modify(pos, tm + 1, tr));
    }

    int get(int l, int r) {
        return get(l, r, 0, n - 1);
    }

    int get(int ll, int rr, int tl, int tr) {
        if (tl >= ll && tr <= rr) {
            return val;
        }
        int tm = (tl + tr) / 2;
        if (ll > tm) {
            return r->get(ll, rr, tm + 1, tr);
        }
        if (rr <= tm) {
            return l->get(ll, rr, tl, tm);
        }
        return l->get(ll, rr, tl, tm) + r->get(ll, rr, tm + 1, tr);
    }
};

int main() {


    return 0;
}
