// Program to query and update on tree -
// 1. Query - Sum of edges on path from node1 to node2
// 2. Update - Update value of an edge

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

struct edgenode {
    int nodep;
    int nodec;
    ll val;
};

struct node {
    ll val;
};

const int N = 100000;

// HLD vars
int currID;
int seqnum;
bool newchain;
int parent[N + 1];
int childcount[N + 1];
int chainID[N + 1];
int chainHead[N + 1];
int sequence[N + 1];    // Sequence to be used by st (same nodes grouped together)
int nodetoseq[N + 1];     // Mapping from node to it's position in sequence

// LCA vars
const int SIZE = (int)ceil(log2(N + 1));
int table[N + 1][SIZE + 1];
int level[N + 1];

// Segment Tree vars
ll arr[N + 1];
struct node st[4*N + 1];

// HLD functions
void fillParentDFS(int node, vector<int> graph[]) {
    for(auto it = graph[node].begin(); it != graph[node].end(); it++) {
        if(*it != parent[node]) {
            parent[*it] = node;
            fillParentDFS(*it, graph);
        }
    }
}

void fillChildrenNumDFS(int node, vector<int> graph[]) {
    int children = 0;
    for(auto it = graph[node].begin(); it != graph[node].end(); it++) {
        if(*it != parent[node]) {
            fillChildrenNumDFS(*it, graph);
            children += childcount[*it] + 1;
        }
    }
    childcount[node] = children;
}

void hldDFS(int node, vector<int> graph[]) {
    // set the chain ID & add to sequence & store chain Head
    chainID[node] = currID;
    sequence[seqnum++] = node;
    if(newchain) {
        chainHead[currID] = node;
        newchain = false;
    }

    // find max child node
    int maxchildi = -1;
    int maxchildval = -1;
    for(auto it = graph[node].begin(); it != graph[node].end(); it++) {
        if(*it != parent[node] && childcount[*it] > maxchildval) {
            maxchildi = *it;
            maxchildval = childcount[*it];
        }
    }
    
    // chain ends here (base case)
    if(maxchildi == -1) {
        currID += 1;
        newchain = true;
        return;
    }

    // continue the chain
    hldDFS(maxchildi, graph);

    // do the same for rest of the nodes
    for(auto it = graph[node].begin(); it != graph[node].end(); it++) {
        if(*it != maxchildi && *it != parent[node]) {
            hldDFS(*it, graph);
        }
    }
}

// ST functions
void updateOperation(int in) {
    st[in].val = max(st[2*in].val, st[2*in + 1].val);
}

void init(int l, int r, int in) {
    if(l == r) {
        st[in].val = arr[sequence[l]];
        return;
    }

    int mid = (l + r)/2;
    init(l, mid, 2*in);
    init(mid + 1, r, 2*in + 1);

    updateOperation(in);
}

void update(int l, int r, int in, int arri, ll val) {
    if(l == r) {
        st[in].val = val;
        return;
    }

    int mid = (l + r)/2;
    if(arri <= mid) {
        update(l, mid, 2*in, arri, val);
    } else {
        update(mid + 1, r, 2*in + 1, arri, val);
    }

    updateOperation(in);
}

ll queryOperation(ll lans, ll rans) {
    return max(lans, rans);
}

ll query(int l, int r, int in, int ql, int qr) {
    if(ql <= l && r <= qr) {
        return st[in].val;
    }
    if(qr < l || r < ql) {
        return INT_MIN;
    }
    int mid = (l + r)/2;
    ll lans = query(l, mid, 2*in, ql, qr);
    ll rans = query(mid + 1, r, 2*in + 1, ql, qr);

    return queryOperation(lans, rans);
}

// LCA
void findLevel(int node, vector<int> graph[]) {
    if(parent[node] != -1) level[node] = level[parent[node]] + 1;
    for(auto it = graph[node].begin(); it != graph[node].end(); it++) {
        if(*it != parent[node]) {
            findLevel(*it, graph);
        }
    }
}

void formBinaryLiftingTable(int n) {
    for(int i = 1; i <= n; i++) {
        for(int j = 0; j <= SIZE; j++) {
            table[i][j] = -1;
        }
    }
    for(int i = 1; i <= n; i++) {
        table[i][0] = parent[i];
    }
    for(int i = 1; i <= SIZE; i++) {
        for(int j = 1; j <= n; j++) {
            int par = table[j][i - 1];
            if(par != -1) table[j][i] = table[par][i - 1];
        }
    }
}

int findLCA(int l, int r) {
    int x, y;
    if(level[l] > level[r]) {
        x = l;
        y = r;
    } else {
        x = r;
        y = l;
    }
    int d = abs(level[l] - level[r]);
    while(d > 0) {
        int jump = log2(d);
        x = table[x][jump];
        d -= (1 << jump);
    }
    if(x == y) return x;
    
    for(int i = SIZE; i >= 0; i--) {
        if(table[x][i] != table[y][i] && table[x][i] != -1 && table[y][i] != -1) {
            x = table[x][i];
            y = table[y][i];
        }
    }
    return table[x][0];
}

// Query using HLD
ll queryHLD(int nodedown, int nodeup, int n) {
    if(chainID[nodedown] == chainID[nodeup]) {
        return query(1, n, 1, nodetoseq[nodeup], nodetoseq[nodedown]);
    }
    ll ans = INT_MIN;
    ans = max(ans, query(1, n, 1, nodetoseq[chainHead[chainID[nodedown]]], nodetoseq[nodedown]));
    ans = max(ans, queryHLD(parent[chainHead[chainID[nodedown]]], nodeup, n));
    return ans;
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

        // Input graph
        vector<int> graph[n + 1];   // Tree with n nodes
        struct edgenode edges[n];
        for(int i = 0; i < n - 1; i++) {    // and n - 1 edges
            int x, y;
            cin >> x >> y;
            graph[x].push_back(y);
            graph[y].push_back(x);
            edges[i].nodep = x;
            edges[i].nodec = y;
            ll val;
            cin >> val;
            edges[i].val = val;
        }

        // Store parent
        parent[1] = -1;
        fillParentDFS(1, graph);

        // Set values 
        // x -> y edge stored in child coz child has only one parent
        // but a parent can have multiple children
        arr[1] = -INF;    // Tree rooted at 1
        for(int i = 0; i < n - 1; i++) {
            if(parent[edges[i].nodec] == edges[i].nodep) {
                arr[edges[i].nodec] = edges[i].val;
            } else {
                arr[edges[i].nodep] = edges[i].val;
                int temp = edges[i].nodec;
                edges[i].nodec = edges[i].nodep;
                edges[i].nodep = temp;
            }
        }

        // Store count of children
        fillChildrenNumDFS(1, graph);

        // HLD
        currID = 1;
        seqnum = 1;
        newchain = true;
        hldDFS(1, graph);

        // Map node to it's position in the sequence
        for(int i = 1; i <= n; i++) {
            nodetoseq[sequence[i]] = i;
        }

        // LCA    
        formBinaryLiftingTable(n);

        level[1] = 0;
        findLevel(1, graph);

        // Segment Tree
        init(1, n, 1);

        while(1) {
            string s;
            cin >> s;
            if(s[0] == 'D') break;
            if(s[0] == 'Q') {
                // query
                int node1, node2;
                cin >> node1 >> node2;
                if(node1 == node2) {
                    cout << 0 << endl;
                    continue;
                }
                int lca = findLCA(node1, node2);

                // Set the value of lca to parent edge
                // such that it does not effect the query
                update(1, n, 1, nodetoseq[lca], INT_MIN);

                ll ans = 0;
                ans = max(ans, queryHLD(node1, lca, n));
                ans = max(ans, queryHLD(node2, lca, n));
                cout << ans << endl;

                // Reset the correct value
                update(1, n, 1, nodetoseq[lca], arr[lca]);
            } else {
                // update
                int edgei;
                ll val;
                cin >> edgei >> val;
                edgei -= 1;
                update(1, n, 1, nodetoseq[edges[edgei].nodec], val);
                arr[edges[edgei].nodec] = val;
            }
        }
    }
    return 0;
}