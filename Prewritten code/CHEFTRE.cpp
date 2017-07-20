#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <numeric>
#include <iomanip>
#include <cstring>
#include <cassert>
#include <memory>
#include <chrono>
#include <cstdio>
#include <bitset>
#include <vector>
#include <math.h>
#include <queue>
#include <stack>
#include <ctime>
#include <set>
#include <map>
 
using namespace std;
 
#define rep(i,a,n) for (int i=(a);i<(n);i++)
#define per(i,n,a) for (int i=(n)-1;i>=(a);i--)
#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second
 
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef pair<int,int> pii;
typedef pair<ll,ll> pll;
 
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
 
const int maxn = (int)2e6+10;
const int inf = (int)1e9+5;
const int mod = (int)1e9 + 7;
const ll llinf = (ll)1e18 + 5;
 
const int base=1009;
ll pw[maxn];
 
struct myhash {
    ll val;
    ll rval;
    ll sz;
    myhash():rval(0),val(0),sz(0){}
    myhash(int x): val(x),rval(x),sz(1){}
    myhash rev(){
        myhash ret;
        ret.val=rval;
        ret.rval=val;
        ret.sz=sz;
        return ret;
    }
    bool operator == (const myhash &rhs) const {
        return val==rhs.val&&rval==rhs.rval&&sz==rhs.sz;
    }
 
    void print(){
        cout<<"hash: {" << val<<' '<<rval<<' '<<sz<<"}"<<endl;
 
    }
};
 
myhash comb(const myhash &a,const myhash&b){
    if(a.sz==0)return b;
    if(b.sz==0)return a;
    myhash ret;
    ret.val=a.val+b.val*pw[a.sz];
    ret.rval=a.rval*pw[b.sz]+b.rval;
    ret.sz=a.sz+b.sz;
    return ret;
}
 
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
mt19937 generator (seed);
 
int nextRand(int x){
    return (generator())%x+1;
}
 
struct node;
 
typedef shared_ptr<node> nptr;
 
struct node {
 
    int x,sz;
    nptr l,r;
    bool rev;
 
    myhash h;
 
    node(): l(0),r(0),x(0),sz(0),rev(0),h(myhash()) {}
    node(int x): l(0),r(0),x(x),sz(1),rev(0),h(x) {}
    node(nptr v) {
        if (!v) {
            l=r=0,x=sz=rev=0;
            h=myhash();
            return;
        }
        l=v->l,r=v->r,x=v->x,sz=v->sz,rev=v->rev,h=v->h;
    }
 
    node(nptr l,nptr r,int x,myhash h,int sz):l(l),r(r),x(x),h(h),sz(sz),rev(0){}
 
    int getsz(nptr v);
 
    myhash hashof(){
        return this?h:myhash();
    }
 
    void upd(){
        sz=1+getsz(l)+getsz(r);
        h=comb(l->hashof(),comb(myhash(x),r->hashof()));
    }
 
    void print(){
        if(!this)return;
        if(l) push(l)->print();
        printf("%d ",x);
        if(r) push(r)->print();
    }
 
    void operator delete(void *x);
    void * operator new(size_t sz);
 
    nptr make(nptr v){
        if(!v)return nptr();
        nptr ret=nptr(new node(v));
        ret->rev^=true;
        ret->h=ret->h.rev();
        return ret;
    }
 
    nptr push(nptr v){
        if(!v) return nptr(nullptr);
        if(!v->rev) return nptr(new node(v));
        return nptr(new node(v->r?make(v->r):0,v->l?make(v->l):0,v->x,v->h,v->sz));
    }
};
 
char pool[maxn][sizeof(node)];
void * ptrs[maxn];
int ptrIt;
 
nptr make(nptr v){
    return v->make(v);
}
 
nptr push(nptr v){
    return v->push(v);
}
 
int node::getsz(nptr v){
    return v?v->sz:0;
}
 
int getsz(nptr v){
    return v?v->sz:0;
}
 
void * dallocate() {
    void *ret=ptrs[--ptrIt];
    return ret;
}
 
void ddeallocate(void *p){
    ptrs[ptrIt++]=p;
}
 
void * node::operator new(size_t sz) {
    return dallocate();
}
 
void node::operator delete(void *p){
    ddeallocate(p);
}
 
void merge(nptr tl, nptr tr, nptr &v){
    nptr l=push(tl);
    nptr r=push(tr);
    if(!l||!r){
        v=nptr(new node(l?l:r));
        return;
    }
    if (nextRand(getsz(l)+getsz(r))<getsz(l)){
        v=nptr(new node(l));
        merge(l->r,r,v->r);
    } else {
        v=nptr(new node(r));
        merge(l,r->l,v->l);
    }
    v->upd();
}
 
void split(nptr tt, int x, nptr &l, nptr &r){
    if(!tt)return void(l=r=0);
    nptr v=push(tt);
    int q=getsz(v->l)+1;
    if(q>x){
        r=v;
        split(v->l,x,l,r->l);
        r->upd();
    }else{
        l=v;
        split(v->r,x-q,l->r,r);
        l->upd();
    }
}
 
struct treap;
 
typedef shared_ptr<treap> tptr;
 
struct treap {
    nptr root;
    treap():root(nptr(nullptr)){}
    treap(tptr t):root(t->root){}
 
    void add(int x){
        merge(root, nptr(new node(x)), root);
    }
 
    void reverse(int l,int r){
        nptr a,b,c,d;
        split(root,r,a,b);
        split(a,l-1,d,c);
        nptr cc=make(c);
        nptr e,f;
        merge(d,cc,e);
        merge(e,b,f);
        root=nptr(new node(f));
    }
 
    nptr cut(int l,int r){
        nptr a,b,c,d;
        split(root,r,a,b);
        split(a,l-1,d,c);
        return c;
    }
 
    myhash get(int l,int r){
        return cut(l,r)->hashof();
    }
 
    void cpy(int l,int r,nptr x){
        nptr a,b,c,d;
        split(root,r,a,b);
        split(a,l-1,c,d);
        nptr e,f;
        merge(c,x,e);
        merge(e,b,f);
        root = nptr(new node(f));
    }
    void print(){
        nptr v = push(root);
        v->print();
        putchar('\n');
    }
};
 
char poolBlocks[maxn][16];
void * ptrsBlocks[maxn];
int ptrsBlocksIt;
 
void * operator new(size_t sz) {
    assert(sz<=16);
    return ptrsBlocks[--ptrsBlocksIt];
}
 
void operator delete(void * p) {
    ptrsBlocks[ptrsBlocksIt++] = p;
}
 
bool init() {
    pw[0]=1;
    rep(i,0,maxn){
        ptrs[ptrIt++]=pool+i;
        ptrsBlocks[ptrsBlocksIt++] = poolBlocks[i];
        if(i>0)pw[i]=pw[i-1]*base;
    }
    return true;
}
 
bool flag_Init = init();
int a[maxn];
int n,q;
 
int fi[maxn],nxt[maxn],es[maxn];
int c=0;
void addedge(int x,int y){
    nxt[++c]=fi[x];
    fi[x]=c;
    es[c]=y;
}
 
int root;
int timer=0;
int sz[maxn];
int tin[maxn],tout[maxn];
int up[maxn][17];
int depth[maxn];
 
void dfs(int v,int par=0){
    tin[v]=timer++;
    up[v][0]=par;
    depth[v]=depth[par]+1;
    sz[v]=1;
    rep(i,1,17)up[v][i]=up[up[v][i-1]][i-1];
    for(int h=fi[v];h!=0;h=nxt[h]){
        int to=es[h];
        if(to!=par)dfs(to,v),sz[v]+=sz[to];
    }
    tout[v]=timer++;
}
 
bool isanc(int x,int y){
    return tin[x]<=tin[y]&&tout[x]>=tout[y];
}
 
int lca(int x,int y){
    if(isanc(x,y))return x;
    if(isanc(y,x))return y;
    per(i,17,0){
        if(up[x][i]!=0&&!isanc(up[x][i],y)){
            x=up[x][i];
        }
    }
    return up[x][0];
}
 
int nodes[maxn],nsz=0;
 
int high[maxn],num[maxn],pos[maxn];
tptr t[maxn];
int curp=0;
 
void process(){
    curp++;
    high[curp]=nodes[0];
    t[curp]=tptr(new treap());
    per(i,nsz,0){
        int v=nodes[i];
        num[v]=curp;
        pos[v]=nsz-i;
        t[curp]->add(a[v]);
    }
    nsz=0;
}
 
void build(int v,int par=-1){
    nodes[nsz++]=v;
    int heavy=-1;
    for(int h=fi[v];h!=0;h=nxt[h]){
        int x=es[h];
        if(x==par)continue;
        if(2*sz[x]>=sz[v]){
            heavy=x;
        }
    }
    if(heavy==-1){
        process();
    }else{
        build(heavy,v);
    }
    for(int h=fi[v];h!=0;h=nxt[h]){
        int x=es[h];
        if(x==par||x==heavy)continue;
        build(x,v);
    }
}
 
nptr go(int x,int y){
    int a=num[x],b=pos[x];
    int c=num[y],d=pos[y];
    if(a==c){
        return t[a]->cut(b,d);
    }
    nptr Q=t[a]->cut(b,getsz(t[a]->root));
    nptr W=go(up[high[a]][0],y);
    nptr E;
    merge(Q,W,E);
    return E;
}
 
nptr getpath(int x,int y){
    int z=lca(x,y);
    nptr a=go(x,z);
    nptr b=go(y,z);
    nptr c,d;
    nptr e=make(b);
    split(e,1,c,d);
    nptr ret;
    merge(a,d,ret);
    return ret;
}
 
void assign(int x,nptr T){
    if(getsz(T)==0)return;
    int a=num[x];
    int b=pos[x];
    int c=min(getsz(T),getsz(t[a]->root)-b+1);
    nptr Q,W;
    split(T,c,Q,W);
    t[a]->cpy(b,b+c-1,Q);
    assign(up[high[a]][0],W);
}
 
void assignp(int x,int y,nptr t){
    int z=lca(x,y);
    nptr a,b;
    split(t,depth[x]-depth[z],a,b);
    b=make(b);
    assign(x,a);
    assign(y,b);
}
 
void revpath(int x,int y){
    nptr path=getpath(x,y);
    nptr real=make(path);
    assignp(x,y,real);
}
 
int main() {
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
 
    cin>>n>>q;
    rep(i,1,n+1)cin>>a[i];
 
    rep(i,0,n-1){
        int x,y;
        cin>>x>>y;
        addedge(x,y);
        addedge(y,x);
    }
    root=1;
    dfs(root,0);
    build(root,-1);
 
    while(q--){
        int type;
        cin>>type;
        int x,y,x2,y2;
        cin>>x>>y;
        if(type==1){
            revpath(x,y);
        }
        if(type==2){
            cin>>x2>>y2;
            cout<<(getpath(x,y)->h==getpath(x2,y2)->h?"Yes":"No")<<'\n';
        }
        if(type==3){
            cin>>x2>>y2;
            assignp(x2,y2,getpath(x,y));
        }
    }
    return 0;
}
