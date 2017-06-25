#include <cstdio>
#include <algorithm>
#include <utility>

using point = std::pair<short, short>;

short n, m;
point flame[1024];

static short dist(const point& a, const point& b) {
    short p = abs(a.first - b.first),
          q = abs(a.second - b.second);
    return std::max(p, q);
}

static inline short distFlame(const point& x) {
    short k = 1 << 14;
    for (short i = 0; i < m; i++) {
        const short cand = dist(x, flame[i]);
        if (cand < k) k = cand;
    }
    return k;
}

static inline short updateAns(const short k) {
    short ret = -1;
    for (short i = -k; i < k; i++) {
        short cand;
        cand = distFlame({k, -i}); if (cand >= k && cand > ret) ret = cand;
        cand = distFlame({-k, i}); if (cand >= k && cand > ret) ret = cand;
        cand = distFlame({i, k}); if (cand >= k && cand > ret) ret = cand;
        cand = distFlame({-i, -k}); if (cand >= k && cand > ret) ret = cand;
    }
    return ret;
}

int main() {
    short t;
    scanf("%hd", &t);

    while (t--) {
        scanf("%hd%hd", &n, &m);

        for (short i = 0; i < m; i++) {
            scanf("%hd%hd", &flame[i].first, &flame[i].second);
        }
        short ans = distFlame({0, 0});

        if (ans == 0) {
            puts("0");
            continue;
        }

        for (int k = 1; k <= n; k++) {
            short upd = updateAns(k);
            if (upd >= ans) {
                ans = upd;
            } else break;
        }

        printf("%hd\n", ans);
    }
}
