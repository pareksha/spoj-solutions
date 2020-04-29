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

const int N = 300000;

ll original[N + 1];
map<ll, int> compressed;

struct qnode {
    ll r1;
    ll r2;
    int in;
    int dup;
};

bool sortHelper(struct qnode a, struct qnode b) {
    if(a.r1 != b.r1) return a.r1 < b.r1;
    else return a.r2 < b.r2;
}

struct node {
    int count;
};

struct node st[4*N + 1];

void updateOperation(int in) {
    st[in].count = st[2*in].count + st[2*in + 1].count;
}

void init(int l, int r, int in) {
    if(l == r) {
        st[in].count = 0;
        return;
    }

    int mid = (l + r)/2;
    init(l, mid, 2*in);
    init(mid + 1, r, 2*in + 1);

    updateOperation(in);
}

void insert(int l, int r, int in, int arri) {
    if(l == r) {
        st[in].count += 1;
        return;
    }

    int mid = (l + r)/2;
    if(arri <= mid) {
        insert(l, mid, 2*in, arri);
    } else {
        insert(mid + 1, r, 2*in + 1, arri);
    }

    updateOperation(in);
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
    int n;
    cin >> n;
    struct qnode arr[n];
    for(int i = 0; i < n; i++) {
        cin >> arr[i].r1 >> arr[i].r2;
        arr[i].in = i;
        arr[i].dup = 0;
    }
    sort(arr, arr + n, sortHelper);
    for(int i = 1; i < n; i++) {
        if((arr[i].r1 == arr[i - 1].r1) && (arr[i].r2 == arr[i - 1].r2)) {
            arr[i].dup = arr[i - 1].dup + 1;
        }
    }

    vector<ll> v;
    for(int i = 0; i < n; i++) v.push_back(arr[i].r2);
    sort(v.begin(), v.end());

    int index = 1;
    for(auto it = v.begin(); it != v.end(); it++) {
        if(compressed.find(*it) == compressed.end()) {
            compressed[*it] = index;
            original[index] = *it;
            index += 1;
        }
    }
    int finaln = index - 1;

    init(1, finaln, 1);

    int ans[n];
    for(int i = 0; i < n; i++) {
        ans[arr[i].in] = count(1, finaln, 1, 1, compressed[arr[i].r2]) - arr[i].dup;
        insert(1, finaln, 1, compressed[arr[i].r2]);
    }

    for(int i = 0; i < n; i++) {
        cout << ans[i] << endl;
    }
}