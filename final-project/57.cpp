#include <cstdio>
#define COST_MAX 150016

#define min(a,b)  ((a) < (b) ? (a) : (b))

int n, m;
int sol[COST_MAX];

int main() {
    scanf("%d%d", &n, &m);

    int amount[n], cost[n], sat[n];

    for (int i = 0; i < n; i++) {
        scanf("%d%d%d", &amount[i], &cost[i], &sat[i]);
    }

    // scaling method
    // http://www.csie.ntnu.edu.tw/~u91029/KnapsackProblem.html#3
    for (int i = 0; i < n; ++i) {
        int num = min(amount[i], m / cost[i]);
        for (int k = 1; num > 0; k <<= 1) {
            if (k > num) k = num;
            num -= k;
            for (int j = m; j >= cost[i] * k; j--) {
                int cand = sol[j - cost[i] * k] + sat[i] * k;
                if (cand > sol[j])
                    sol[j] = cand;
            }
        }
    }

    printf("%d\n", sol[m]);
}
