#include <cstdio>
#include <cstring>
#include <vector>
#include <utility>
#include <algorithm>

#define MAX 20016

using int_pair = std::pair<int, int>;

int n, m;
int disj_par[MAX];
int_pair edges[MAX];
int weight[MAX];
bool commonMST[MAX];
int_pair widx_pair[MAX];
std::vector<int_pair> adj_list[MAX];
int ts, ord[MAX];

int udroot(int x) {
    if (disj_par[x] < 0) return x;
    return (disj_par[x] = udroot(disj_par[x]));
}

bool udset(int x, int y) {
    x = udroot(x), y = udroot(y);
    if (x == y) return false;
    if (disj_par[y] < disj_par[x]) {
        std::swap(x, y);
    }
    disj_par[x] += disj_par[y];
    disj_par[y] = x;
    return true;
}

// Tarjan's algorithm finding bridges by a DFS
int tarjan(int at, int pre) {
    ord[at] = ts++;
    int h = ord[at];
    for (size_t i = 0; i < adj_list[at].size(); i++) {
        int to = adj_list[at][i].first;
        if (to == pre) continue;
        int th = ord[to];
        if (th == -1) {
            th = tarjan(to, at);
            if (ord[at] < th) {
                commonMST[adj_list[at][i].second] = true;
            }
        }
        if (th < h) h = th;
    }
    return h;
}

int main() {
    memset(&disj_par, -1, sizeof(disj_par));
    memset(ord, -1, sizeof(ord));
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; i++) {
        scanf("%d%d%d", &edges[i].first, &edges[i].second, &weight[i]);
        edges[i].first--;
        edges[i].second--;
        widx_pair[i] = {weight[i], i};
    }
    std::sort(widx_pair, widx_pair + m);

    int cur = 0;
    while (cur < m) {
        int nxt = cur;
        while (nxt < m && widx_pair[cur].first == widx_pair[nxt].first) nxt++;

        std::vector<int> v;
        for (int i = cur; i < nxt; i++) {
            int idx = widx_pair[i].second;
            int a = udroot(edges[idx].first),
                b = udroot(edges[idx].second);
            if (a != b) {
                v.push_back(a);
                v.push_back(b);
                adj_list[a].push_back({b, idx});
                adj_list[b].push_back({a, idx});
            }
        }

        std::sort(v.begin(), v.end());
        v.erase(std::unique(v.begin(), v.end()), v.end());

        for (size_t i = 0; i < v.size(); i++) {
            if (ord[v[i]] == -1) {
                ts = 0;
                tarjan(v[i], -1);
            }
        }
        for (size_t i = 0; i < v.size(); i++) {
            int idx = v[i];
            auto& eidx_cur = adj_list[idx];
            std::sort(eidx_cur.begin(), eidx_cur.end());

            for (size_t j = 1; j < eidx_cur.size(); j++) {
                if (eidx_cur[j - 1].first == eidx_cur[j].first) {
                    commonMST[eidx_cur[j - 1].second] = \
                    commonMST[eidx_cur[j].second] = false;
                }
            }
            ord[idx] = -1;
            eidx_cur.clear();
        }
        for (int i = cur; i < nxt; i++) {
            auto& edge = edges[widx_pair[i].second];
            udset(edge.first, edge.second);
        }
        cur = nxt;
    }

    int ans = 0;
    for (int i = 0; i < m; i++) {
        if (commonMST[i]) {
            ans += weight[i];
        }
    }
    printf("%d\n", ans);
}
