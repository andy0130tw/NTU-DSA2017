#include <cstdio>

short chord[5000];
short dp[5000][5000];

// MSIC: referenced from
//   https://ir.nctu.edu.tw/bitstream/11536/3521/1/A1992HF69200007.pdf
int msic(int i, int j) {
    // printf("%d %d\n", i, j);
    if (dp[i][j] != 0) return dp[i][j];
    if (i >= j) return 0;
    int ans = msic(i, j - 1);
    if (i <= chord[j] && chord[j] < j) {
        int cand = msic(i, chord[j] - 1) + msic(chord[j] + 1, j - 1) + 1;
        if (cand > ans) ans = cand;
    }
    return dp[i][j] = ans;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n / 2; i++) {
        int a, b;
        scanf("%d%d", &a, &b);
        chord[a] = b;
        chord[b] = a;
    }

    printf("%d\n", msic(0, n - 1));
}
