#include <functional>
#include <algorithm>
#include <complex>
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

const int maxN = (int)3e5 + 10;
const int mod = (int)1e9 + 7;
const int INF = (int)1e9 + 5;
const ll LLINF = (ll)1e18 + 5;
const double PI = acos(-1.0);

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
int main() {

    freopen(".in", "r", stdin);
    freopen(".out", "w", stdout);

    int n, m;
    cin >> n >> m;
    if (m > n) {
        cout << 0 << '\n';
        return 0;
    }
    vector <int> T(n + m+ 1);
    vector <int> a(n);
    for(int i = 0; i < n; ++i ){
        string s;
        cin >> s;
        for (int j = 0; j < 7; ++j) {
            T[i] |= (s[j] - '0') << (6 - j);
        }
        a[i] |= (s[7] - '0');
    }
    T[n] = -1;

    vector <int> b(m);
    for(int i = 0; i < m; ++i ){
        string s;
        cin >> s;
        for (int j = 0; j < 7; ++j) {
            T[i + n + 1] |= (s[j] - '0') << (6 - j);
        }
        b[i] |= (s[7] - '0');
    }
    for (int i = 0; i < m; ++i) b[i] ^= 1;
    while(b.size() < n) b.push_back(0);
    reverse(b.begin(), b.end());
    vector <int> res;
    multiply(a, b, res);
    vector <int> res2;
    for (int i = n - 1; n - 1 - i + 1 <= m; --i) b[i] ^= 1;
    for (int i = 0; i < n; ++i) a[i] ^= 1;
    multiply(a, b, res2);

    for (int i =0 ; i < T.size(); ++i) {
        cout << T[i] << ' ';
    }
    cout << '\n';
    vector <int> z(n + m + 1);
    for (int l = 0, r= 0, i = 1; i < n + m + 1; ++i) {
        if(i <= r) {
            z[i] = min(z[i - l], r - i + 1);
        }
        while(i + z[i] < n + m + 1 && T[z[i]] == T[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] >= r) {
            l = i;
            r = i + z[i] - 1;
        }
        cout << z[i] << ' ';
    }
    cout << '\n';

    int best = INF;
    int idx = -1;
    int cur = 0;
    for (int i = n - 1; i - (n - m + 1) + 1 <= m; ++i) {
       cout << res[i] << ' ' << res2[i] << '\n';
       ++cur;
       if (z[n + cur] >= m) {
            if (best > res[i] + res2[i]) {
                best = res[i] + res2[i];
                idx = cur;
            }
        }
    }

    if (idx == -1) {
        puts("No");
    } else {
        puts("Yes");
        printf("%d %d\n", best, idx);
    }
    return 0;
}
