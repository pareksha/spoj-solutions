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
    ll val;
};

const int N = 100000;
ll arr[N + 1];     // 1 - indexed
struct node st[4*N + 1];    // segment tree (1 - indexed)

set<int> s;

void updateOperation(int in) {
    st[in].val = st[2*in].val + st[2*in + 1].val;
}

void init(int l, int r, int in) {
    if(l == r) {
        st[in].val = arr[l];
        return;
    }

    int mid = (l + r)/2;
    init(l, mid, 2*in);
    init(mid + 1, r, 2*in + 1);

    updateOperation(in);
}

void update(int l, int r, int in, int arri) {
    if(l == r) {
        st[in].val = sqrtl(st[in].val);
        return;
    }

    int mid = (l + r)/2;
    if(arri <= mid) {
        update(l, mid, 2*in, arri);
    } else {
        update(mid + 1, r, 2*in + 1, arri);
    }

    updateOperation(in);
}

ll queryOperation(ll lans, ll rans) {
    return lans + rans;
}

ll query(int l, int r, int in, int ql, int qr) {
    if(ql <= l && r <= qr) {
        return st[in].val;
    }
    if(qr < l || r < ql) {
        return 0;
    }
    int mid = (l + r)/2;
    ll lans = query(l, mid, 2*in, ql, qr);
    ll rans = query(mid + 1, r, 2*in + 1, ql, qr);

    return queryOperation(lans, rans);
}

void swap(int &x, int &y) {
    int temp = x;
    x = y;
    y = temp;
}

int main() {
    fastIO
    // ---------------------------- //
    // #ifndef ONLINE_JUDGE
    //     freopen("input.txt", "r", stdin);
    //     freopen("output.txt", "w", stdout);
    // #endif
    // ---------------------------- //
    int x = 0;
    int n;
    while(1) {
        string st;
        cin >> st;
        if(st == "") break;
        n = stoi(st);
        x += 1;
        cout << "Case #" << x << ":" << endl;
        for(int i = 1; i <= n; i++) {
            cin >> arr[i];
            s.insert(i);
        }
        init(1, n, 1);
        int q;
        cin >> q;
        for(int i = 0; i < q; i++) {
            int k;
            cin >> k;
            if(k == 1) {
                // query
                int ql, qr;
                cin >> ql >> qr;
                if(qr < ql) swap(ql, qr);
                ll ansq = query(1, n, 1, ql, qr);
                cout << ansq << endl;
            } else {
                // update
                int upl, upr;
                cin >> upl >> upr;
                if(upr < upl) swap(upl, upr);
                auto it = s.lower_bound(upl);
                if(it == s.end()) continue;
                while(*it <= upr) {
                    update(1, n, 1, *it);
                    arr[*it] = sqrtl(arr[*it]);
                    auto itx = it;
                    it++;
                    if(arr[*itx] == 1) {
                        s.erase(itx);
                    }
                    if(it == s.end()) break;
                }
            }
            // for(int i = 1; i <= n; i++) cout << arr[i] << " ";
            // cout << endl;
        }
        cout << endl;
        s.clear();
    }
}