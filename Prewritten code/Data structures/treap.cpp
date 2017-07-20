#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

template <typename T>
T GCD(T a, T b) {
    for(;a && b;) {
        if (a >= b) {
            a %= b;
        } else {
            b %= a;
        }
    }
    return a | b;
}

template <typename T>
class Treap {


    struct Node {
        Node *l, *r;
        int y;
        int sz;
        T gcd;
        T x;

        Node() {}
        Node(T x): x(x), y(rand() << 15 | rand()), sz(1), gcd(x), l(0), r(0) {}

        int sizeOf() {
            return this ? sz : 0;
        }

        T gcdOf() {
            return this ? gcd : 0;
        }

        void Update() {
            sz = 1 + l->sizeOf() + r->sizeOf();
            gcd = GCD(x, GCD(l->gcdOf(), r->gcdOf()));
        }

        void Print() {
            if (!this) return;
            l->Print();
            cout << x << ' ';
            r->Print();
        }
    };

    void Merge(Node *l, Node *r, Node *&v) {
        if (!l || !r) return void(v = l ? l : r);
        if (l->y > r->y) {
            v = l;
            Merge(l->r, r, v->r);
        } else {
            v = r;
            Merge(l, r->l, v->l);
        }
        v->Update();
    }

    void Split(Node *t, T key, Node *&l, Node *&r) {
        if (!t) return void(l = r = 0);
        if (t->x < key) {
            l = t;
            Split(t->r, key, l->r, r);
            l->Update();
        } else {
            r = t;
            Split(t->l, key, l, r->l);
            r->Update();
        }
    }

public:

    Node *root;

    Treap(): root(0) {}

    void Insert(T x) {
        Node *l, *r;
        Split(root, x, l, r);
        Merge(l, new Node(x), l);
        Merge(l, r, root);
    }

    void Print() {
        root->Print();
    }
};

int main() {

    vector <int> a(20);
    for (int i = 0; i < 20; ++i) {
        a[i] = i + 1;
    }

    random_shuffle(a.begin(), a.end());

    for (int i = 0; i < 20; ++i) {
        cout << a[i] << ' ';
    }

    cout << '\n';

    Treap <int> *t = new Treap <int>();
    for (int i = 0; i < 20; ++i) {
        t->Insert(a[i]);
        t->Print();
        cout << '\n';
    }
    return 0;
}

/*

struct node {
    node *l, *r;
    int x, y;
    int sz;

    node() {}
    node(int x): l(0), r(0), x(x), sz(1) {
        y = (rand() << 15) | rand();
    }

    int sizeOf() {
        if (this) return sz;
        return 0;
    }

    void update() {
        sz = l->sizeOf() + r->sizeOf() + 1;
    }
};

class treap {
    node *root;

    void merge(node *l, node *r, node *&v) {
        if (!l) return void(v = r);
        if (!r) return void(v = l);
        if (l->y > r->y) {
            v = l;
            merge(l->r, r, v->r);
        } else {
            v = r;
            merge(l, r->l, v->l);
        }
        v->update();
    }

    void split(node*t, int key, node *&l, node *&r) {
        if (!t) return void(l = r = 0);
        if (t->x <= key) {
            l = t;
            split(t->r, key, l->r, r);
            l->update();
        } else {
            r = t;
            split(t->l, key, l, r->l);
            r->update();
        }
    }

public:

    treap(): root(0) {}

    void add(int x) {
        node *l, *r;
        split(root, x, l, r);
        merge(l, new node(x), l);
        merge(l, r, root);
    }

    int getGreaterThan(int x) {
        node *l, *r;
        split(root, x - 1, l, r);
        int ret = r->sizeOf();
        merge(l, r, root);
        return ret;
    }

    void extract(int x) {
        node *l, *r;
        split(root, x, l, r);
        node *tl, *tr;
        split(l, x - 1, tl, tr);
        merge(tr->l, tr->r, tr);
        merge(tl, tr, l);
        merge(l, r, root);
    }

};
*/
