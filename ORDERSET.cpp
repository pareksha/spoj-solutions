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
};

const int Q = 200000;

ll original[Q + 1];
map<ll, int> compressed;

struct node st[4*Q + 1];

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
        st[in].count = 1;
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
    int q;
    cin >> q;

    vector<pair<char, ll>> vq;
    for(int i = 0; i < q; i++) {
        char c;
        ll num;
        cin >> c >> num;
        vq.push_back(make_pair(c, num));
    }

    set<ll> s;   // Contains all numbers to be compressed
    for(int i = 0; i < q; i++) s.insert(vq[i].second);

    int index = 1;
    for(auto it = s.begin(); it != s.end(); it++) {
        compressed[*it] = index;
        original[index] = *it;
        index += 1;
    }
    int finaln = index - 1;

    // Segment tree
    init(1, finaln, 1);
    for(int i = 0; i < q; i++) {
        char c = vq[i].first;
        ll num = vq[i].second;
        if(c == 'I') {
            insert(1, finaln, 1, compressed[num]);
        } else if(c == 'D') {
            remove(1, finaln, 1, compressed[num]);
        } else if(c == 'K') {
            if(num > st[1].count) {
                cout << "invalid" << endl;
                continue;
            }
            int ansq = position(1, finaln, 1, num);
            cout << original[ansq] << endl;
        } else {
            if(compressed[num] == 1) {
                cout << 0 << endl;
                continue;
            }
            int ansq = count(1, finaln, 1, 1, compressed[num] - 1);
            cout << ansq << endl;
        }
    }
}