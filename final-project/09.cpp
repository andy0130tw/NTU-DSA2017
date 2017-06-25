#include <cstdio>
#include <algorithm>

int main() {
    int n;
    scanf("%d", &n);

    int box[n], dp[n + 1][n];

    for (int i = 0; i < n; i++) {
        scanf("%d", &box[i]);
    }

    // dp[len][k]: [k, k + len) difference for first player

    if (n % 2) {
        for (int i = 0; i <= n - 1; i++) {
            dp[1][i] = box[i];
        }
    } else {
        for (int i = 0; i <= n; i++) {
            dp[0][i] = 0;
        }
    }

    for (int len = 2 + n % 2; len <= n; len += 2) {
        for (int k = 0; k <= n - len; k++) {
            int r = k + len - 1;
            int val[4] = {
                box[k] - box[k + 1] + dp[len - 2][k + 2],
                box[k] - box[r]     + dp[len - 2][k + 1],
                box[r] - box[k]     + dp[len - 2][k + 1],
                box[r] - box[r - 1] + dp[len - 2][k]
            };
            dp[len][k] = std::max(std::min(val[0], val[1]), std::min(val[2], val[3]));
        }
    }

    putchar(dp[n][0] >= 0 ? 'Y' : 'N');
    putchar('\n');
    printf("%d\n", abs(dp[n][0]));
}
