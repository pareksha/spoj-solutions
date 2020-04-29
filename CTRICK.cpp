#include<bits/stdc++.h>
#define vint vector<int>
#define vll vector<long long>
#define pii pair<int,int>
#define pll pair<long long,long long>
#define pi 3.1415926535898
#define INF 1000000000000
#define INFM -1000000000000
#define mod1 1000000007
#define mod2 998244353
#define fastIO ios_base::sync_with_stdio(false);cin.tie(NULL);
#define endl '\n'
#define endlf endl
using namespace std;
typedef long long ll;

struct node {
    int count;
    int val;
};

const int N = 200000;
struct node st[4*N + 1];

void updateOperation(int in) {
    st[in].count = st[2*in].count + st[2*in + 1].count;
}

void init(int l, int r, int in) {
    if(l == r) {
        st[in].count = 1;
        st[in].val = l;
        return;
    }

    int mid = (l + r)/2;
    init(l, mid, 2*in);
    init(mid + 1, r, 2*in + 1);

    updateOperation(in);
}

void remove(int l, int r, int in, int arri) {
    if(l == r) {
        st[in].count = 0;
        return;
    }

    int mid = (l + r)/2;
    if(arri <= mid) {
        remove(l, mid, 2*in, arri);
    } else {
        remove(mid + 1, r, 2*in + 1, arri);
    }

    updateOperation(in);
}

int position(int l, int r, int in, ll k) {
    if(l == r) {
        return l;
    }

    int mid = (l + r)/2;
    if(k <= st[2*in].count) {
        return position(l, mid, 2*in, k);
    } else {
        return position(mid + 1, r, 2*in + 1, k - st[2*in].count);
    }
}

int queryOperation(int lans, int rans) {
    return lans + rans;
}

int count(int l, int r, int in, int ql, int qr) {
    if(ql <= l && r <= qr) {
        return st[in].count;
    }
    if(qr < l || r < ql) {
        return 0;
    }
    int mid = (l + r)/2;
    int lans = count(l, mid, 2*in, ql, qr);
    int rans = count(mid + 1, r, 2*in + 1, ql, qr);

    return queryOperation(lans, rans);
}

int main() {
    fastIO
    // ---------------------------- //
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif
    // ---------------------------- //
    int test_n;
    cin >> test_n;
    while(test_n--) {
        int n;
        cin >> n;
        init(1, n, 1);
        int ans[n + 1];
        int previ = 0;
        for(int i = 0; i < n; i++) {
            int pos = (previ + (i + 1)%(n - i))%(n - i);
            int posi = position(1, n, 1, pos + 1);
            ans[posi] = i + 1;
            remove(1, n, 1, posi);
            previ = pos;
        }
        for(int i = 1; i <= n; i++) {
            cout << ans[i] << " ";
        }
        cout << endl;
    }
}