#include <cstdio>
#define INF (1 << 30)

int noper, nplan;
int ans[10000010], plan[16];

int solve(int k) {
    if (ans[k] != 0) return ans[k];
    int opti = INF;
    for (int pl = 0; pl < nplan; pl++) {
        if (k < plan[pl]) continue;
        int cand = solve(k - plan[pl]) + 1;
        if (cand > 0 && cand < opti) {
            opti = cand;
        }
    }
    return ans[k] = (opti == INF ? -1 : opti);
}

int main() {
    scanf("%d%d", &noper, &nplan);

    ans[0] = 0;
    int minProc = 0;
    for (int i = 0; i < nplan; i++) {
        scanf("%d", &plan[i]);
        ans[plan[i]] = 1;
    }

    for (int i = 1; i < plan[0]; i++) {
        ans[i] = -1;
    }
    minProc = plan[0];

    while (noper--) {
        int op, arg;
        scanf("%d%d", &op, &arg);
        if (op == 1) {
            printf("%d\n", solve(arg));
        } else {
            for (int i = minProc + 1; i <= arg; i++) {
                solve(i);
            }
            minProc = arg;
            for (int i = 1; i <= arg; i++) {
                if (ans[i] == -1) {
                    printf("%d\t", i);
                }
            }
        }
    }
}
