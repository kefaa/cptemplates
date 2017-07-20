#include <functional>
#include <algorithm>
#include <iostream>
#include <complex>
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
typedef long double ld;

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
const int mod = (int)1e9 + 7;
const int INF = (int)1e9 + 5;
const ll LLINF = (ll)1e18 + 5;

    //convex hull for minimum
    //for maximum change signs in functions bad and get
    //note k1 < k2 < k3 < ... < kn

struct convexHull {
    Line *hull;
    int sz;

    convexHull(){}
    convexHull(int n){
        sz=0;
        hull=new Line[n+10];
    }

    bool bad(Line x,Line y,Line z){
        return (z.b-x.b)*(x.k-y.k)>=(y.b-x.b)*(x.k-z.k);
    }

    void add(ll k,ll b){
        Line z=Line(k,b);
        while(sz>=2&&bad(hull[sz-2],hull[sz-1],z))sz--;
        hull[sz++]=z;
    }

    ll get(ll x){
        int lf=0,rg=sz-1;
        rep(i,0,20){
            int md=(lf+rg)/2;
            if(hull[md].eval(x)>=hull[md+1].eval(x))lf=md;
            else rg=md;
        }
        return hull[rg].eval(x);
    }

};


int main() {

   // freopen("input.txt", "r", stdin);
   // freopen("output.txt", "w", stdout);

    return 0;
}
