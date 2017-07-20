#include <bits/stdc++.h>

using namespace std;

#define rep(i,a,n) for (int i=(a);i<(n);i++)
#define per(i,a,n) for (int i=(n)-1;i>=(a);i--)
#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second
#define SZ(x) (int)x.size()

typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef pair<int,int> pii;
typedef pair<ll,ll>pll;
typedef vector<pii> vpii;

template<typename T>
T getint() {
    T x=0,p=1;
    char ch;
    do{ch=getchar();}while(ch <= ' ');
    if(ch=='-')p=-1,ch=getchar();
    while(ch>='0'&&ch<='9')x=x*10+ch-'0',ch=getchar();
    return x*p;
}

mt19937 gen(chrono::system_clock::now().time_since_epoch().count());

template<typename T1,typename T2>bool umin(T1 &x,const T2&y){if(x>y)return x=y,true;return false;}
template<typename T1,typename T2>bool umax(T1 &x,const T2&y){if(x<y)return x=y,true;return false;}

const int maxn=(int)5e4+10;
const int inf=(int)1e9+5;
const int mod=(int)1e9+7;
const ll llinf=(ll)1e18+5;
const ld pi=acos(-1.0);
const ld PI=acos(-1.0);

template <typename T>
struct mycomplex {
    T _real;
    T _imag;
    mycomplex(): _real(0), _imag(0) {}
    mycomplex(int _real): _real(T(_real)), _imag(0) {}
    mycomplex(T _real, T _imag): _real(_real), _imag(_imag) {}
    friend mycomplex operator * (const mycomplex<T> &a, const mycomplex<T> &b) {
        mycomplex ret;
        ret._real = a._real * b._real - a._imag * b._imag;
        ret._imag = a._imag * b._real + b._imag * a._real;
        return ret;
    }
    friend mycomplex operator + (const mycomplex<T> &a, const mycomplex<T> &b) {
        mycomplex ret(a._real + b._real, a._imag + b._imag);
        return ret;
    }
    friend mycomplex operator - (const mycomplex<T> &a, const mycomplex<T> &b) {
        mycomplex ret(a._real - b._real, a._imag - b._imag);
        return ret;
    }

    friend mycomplex operator *= (mycomplex<T> &a, const mycomplex<T> &b) {
        return a = a * b;
    }

    friend mycomplex operator /= (mycomplex<T> &a, int x) {
        return a = mycomplex(a._real / (T)x, a._imag / (T)x);
    }

    T real() { return _real; };
    T imag() { return _imag; };
};

typedef mycomplex<double> base;

void fft (vector<base> & a, bool invert) {
	int n = (int) a.size();

	for (int i=1, j=0; i<n; ++i) {
		int bit = n >> 1;
		for (; j>=bit; bit>>=1)
			j -= bit;
		j += bit;
		if (i < j)
			swap (a[i], a[j]);
	}

	for (int len=2; len<=n; len<<=1) {
		double ang = 2*PI/len * (invert ? -1 : 1);
		base wlen (cos(ang), sin(ang));
		for (int i=0; i<n; i+=len) {
			base w (1);
			for (int j=0; j<len/2; ++j) {
				base u = a[i+j],  v = a[i+j+len/2] * w;
				a[i+j] = u + v;
				a[i+j+len/2] = u - v;
				w *= wlen;
			}
		}
	}
	if (invert)
		for (int i=0; i<n; ++i)
			a[i] /= n;
}

void multiply (const vector<int> & a, const vector<int> & b, vector<int> & res) {
	vector<base> fa (a.begin(), a.end()),  fb (b.begin(), b.end());
	size_t n = 1;
	while (n < max (a.size(), b.size()))  n <<= 1;
	n <<= 1;
	fa.resize (n),  fb.resize (n);

	fft (fa, false),  fft (fb, false);
	for (size_t i=0; i<n; ++i)
		fa[i] *= fb[i];
	fft (fa, true);

	res.resize (n);
	for (size_t i=0; i<n; ++i)
		res[i] = int (fa[i].real() + 0.5);
}
int n;
vi g[maxn];

int anc[maxn];
int sz[maxn];
vi nodes;
int block[maxn];

void dfs(int v,int par=-1) {
    nodes.pb(v);
    sz[v]=1;
    anc[v]=par;
    for(int x:g[v]){
        if(x==par||block[x])continue;
        dfs(x,v);
        sz[v]+=sz[x];
    }
}

vector<int> now;

int ans[maxn];

void dfs2(int v,int par,int depth){
    if(now.size()<=depth)now.pb(0);
    now[depth]++;
    ans[depth]++;
    for(int x:g[v]){
        if(block[x]||x==par)continue;
        dfs2(x,v,depth+1);
    }
}


void build(int v){
    nodes.clear();
    dfs(v);
    int cen=-1;
    for(int x:nodes){
        int mxsz=nodes.size()-sz[x];
        for(int y:g[x]){
            if(block[y]||anc[x]==y)continue;
            umax(mxsz,sz[y]);
        }
        if(mxsz<=nodes.size()/2)cen=x;
    }
    block[cen]=true;
    multiset<pair<int,vi>>polys;
    for(int x:g[cen]){
        if(block[x])continue;
        now=vi(1,1);
        dfs2(x,cen,1);
        polys.insert(mp(now.size(),now));
    }
    while(polys.size()>=2){
        auto x=polys.begin()->se;polys.erase(polys.begin());
        auto y=polys.begin()->se;polys.erase(polys.begin());
        x[0]=0,y[0]=0;
        vi z;
        multiply(x,y,z);
        rep(j,1,z.size())ans[j]+=z[j];
        vi r(max(x.size(),y.size()));
        rep(i,0,r.size()){
            if(i<x.size())r[i]+=x[i];
            if(i<y.size())r[i]+=y[i];
        }
        r[0]=1;
        polys.insert(mp(r.size(),r));
    }

    if(!polys.empty()){;
    }
    for(int x:g[cen]){
        if(!block[x])build(x);
    }
}

int main(){

   // freopen("choco.in","r",stdin);
   // freopen("choco.out","w",stdout);
   // freopen("input.txt","r",stdin);
   // freopen("output.txt","w",stdout);

    ios_base::sync_with_stdio(0);

    cin>>n;
    rep(i,0,n-1){
        int x,y;
        cin>>x>>y;
        g[x].pb(y);
        g[y].pb(x);
    }

    build(1);
    for(int i=1;i<=n-1;++i){
        cout<<ans[i]<<'\n';
    }
    return 0;
}
