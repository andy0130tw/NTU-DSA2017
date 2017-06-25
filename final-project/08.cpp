#include<iostream>
using namespace std;
char map[12][1004][1004];
int n;

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                char c;
                while ((c = getchar()) <= 32);
                map[t][i][j] = c == '1';
            }
        }

        int ans = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (map[t][i][j] != 0) {
                    map[t][i][j] = min(map[t][i - 1][j], min(map[t][i][j - 1], map[t][i - 1][j - 1])) + 1;
                    ans += map[t][i][j];
                }
            }
        }
        printf("%d\n", ans);
    }
}
