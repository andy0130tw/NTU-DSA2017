#include <cstdio>
#include <cstring>
#include <algorithm>
#define MAX 1016

int n, m, t, songs[MAX], tab[MAX][MAX];

static inline int merge(int state, int dur) {
    int num = state / 128, timext = state % 128;
    if (dur <= t - timext) {
        timext += dur;
    } else {
        num++;
        timext = dur;
    }
    return num * 128 + timext;
}

int main() {
    int x;
    scanf("%d", &x);
    while (x--) {
        scanf("%d%d%d", &n, &m, &t);
        memset(tab, 0, sizeof(tab));

        int valid = 0;
        for (int i = 0; i < n; i++) {
            scanf("%d", &songs[valid]);
            if (songs[valid] <= t) {
                valid++;
            }
        }
        n = valid;

        for (int i = 0; i < n; i++) {
            for (int j = m; j >= 1; j--) {
                for (int k = t; k >= songs[i]; k--) {
                    tab[j][k] = std::max(tab[j][k], tab[j][k - songs[i]] + 1);
                }
            }
            for (int j = 1; j <= m; j++) {
                tab[j][0] = std::max(tab[j][0], tab[j - 1][t]);
                for (int k = 1; k <= t; k++) {
                    tab[j][k] = std::max(tab[j][k], tab[j][k - 1]);
                }
            }
        }
        printf("%d\n", tab[m][t]);
    }
}
