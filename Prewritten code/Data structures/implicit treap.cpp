#include <bits/stdc++.h>

using namespace std;

template <typename T>
class Treap {

    struct Node {
        Node *l, *r;
        int sz;
        T val;
        int y;

        Node() {}
        Node(T val): l(0), r(0), sz(1), val(val), y(rand() << 15 | rand()) {}

        int sizeOf() {
            return this ? sz : 0;
        }

        void Update() {
            sz = 1 + l->sizeOf() + r->sizeOf();
        }

        void Print() {
            if (!this) return;
            l->Print();
            cout << val;
            r->Print();
        }
    };

    void Merge(Node *l, Node *r, Node *&v) {
        if (!l) return void(v = r);
        if (!r) return void(v = l);
        if (l->y > r->y) {
            v = l;
            Merge(l->r, r, v->r);
        } else {
            v = r;
            Merge(l, r->l, v->l);
        }
        v->Update();
    }

    void Split(Node *t, int key, Node *&l, Node *&r) {
        if (!t) return void(l = r = 0);
        int x = t->l->sizeOf() + 1;
        if (x > key) {
            r = t;
            Split(t->l, key, l, r->l);
            r->Update();
        } else {
            l = t;
            Split(t->r, key - x, l->r, r);
            l->Update();
        }
    }

public:

    Node *root;

    Treap(): root(0) {}

    void Insert(T x) { //inserts x in the end of treap
        Merge(root, new Node(x), root);
    }

    void Insert(int pos, T x) { //inserts x after pos elements (t[1], t[2], t[3], ..., t[pos], x, t[pos+1], ..., t[n])
        Node *l, *r;
        Split(root, pos, l, r);
        Merge(l, new Node(x), l);
        Merge(l, r, root);
    }

    void MoveToFront(int l, int r) { //t[1], t[2], t[3], ..., t[l], t[l + 1], ..., t[r], ..., t[n - 1], t[n] ---> /
        Node *L, *R;                 //t[l], t[l + 1], ..., t[r], t[1], t[2], ..., t[l - 1], t[r + 1], ..., t[n - 1], t[n]
        Split(root, r, L, R);
        Node *LL, *RR;
        Split(L, l - 1, LL, RR);
        Merge(RR, LL, L);
        Merge(L, R, root);
    }

    void Print() {
        root->Print();
        cout << '\n';
    }

};

namespace aa {
    Treap <char> *t = new Treap<char>();

    char getNextChar() {
        return char(rand() % 26 + 'a');
    }

    vector <char> Generate(int n) {
        vector <char> ret(n);
        for (int i = 0; i < n; ++i) {
            ret[i] = getNextChar();
        }
        return ret;
    }

    vector < pair <int, char> > __Generate(int n) {
        vector < pair <int, char> > ret(n + 2);
        for (int qq = 0; qq < n; ++qq) {
            int x = rand() % (n + qq + 1);
            char ch = getNextChar();
            ret[qq] = make_pair(x, ch);
        }
        ret[n] = make_pair(0, getNextChar());
        ret[n + 1] = make_pair(1 + n + n, getNextChar());
        return ret;
    }

    void Try(int n) {
        vector <char> a = Generate(n);
        for (int i = 0; i < n; ++i) {
            t->Insert(a[i]);
        }
        t->Print();

        vector < pair <int, char> > queries = __Generate(n);
        for (int i = 0; i < queries.size(); ++i) {
            int x = queries[i].first;
            char ch = queries[i].second;
            cout << x << ' ' << ch << '\n';
            t->Insert(x, ch);
            t->Print();
        }

        t->MoveToFront(2, 5);
        t->Print();
    }
}

int main() {

    srand(time(NULL));
    aa::Try(10);

    return 0;
}
