#include <cstdio>
#define COST_MAX 16384
#define NSTATION_MAX 16

#define min(a,b)  ((a) < (b) ? (a) : (b))

int nstation, fuel;
int dist[NSTATION_MAX];
int profAssign[NSTATION_MAX][16384], profFuel[NSTATION_MAX][16384];
int profCnt[NSTATION_MAX];
int sol[COST_MAX];

int main() {
    scanf("%d%d", &nstation, &fuel);

    for (int i = 0; i < nstation; i++) {
        scanf("%d", &dist[i]);
    }

    for (int i = 0; i < nstation; i++) {
        scanf("%d", &profCnt[i]);
        for (int j = 0; j < profCnt[i]; j++) {
            scanf("%d", &profAssign[i][j]);
        }
        for (int j = 0; j < profCnt[i]; j++) {
            scanf("%d", &profFuel[i][j]);
        }
    }

    // ordinary knapsack owo
    for (int i = 0; i < nstation; i++) {
        for (int j = 0; j < profCnt[i]; j++) {
            int fuelAccu[nstation + 1], stAll = 0;
            fuelAccu[0] = 0;
            for (; stAll < nstation; stAll++) {
                fuelAccu[stAll + 1] = fuelAccu[stAll] + profFuel[i][j] * dist[stAll + i];
                if (fuelAccu[stAll] > fuel) break;
            }

            // try to update maximum until no fuel is allowed
            for (int k = fuel; k >= 0; k--) {
                int fuelAccu = 0;
                int opti = 0;
                for (int st = i; st < nstation; st++) {
                    // a trip from i to (st + 1),
                    // using fuel profFuel[i][j] * dist[i .. st]
                    fuelAccu += dist[st] * profFuel[i][j];
                    if (k - fuelAccu < 0) break;
                    int cand = sol[k - fuelAccu] + profAssign[i][j] * (st + 1 - i);
                    if (cand > opti) opti = cand;
                }

                if (opti > sol[k]) {
                    sol[k] = opti;
                }
            }
        }
    }

    printf("%d\n", sol[fuel]);
}
