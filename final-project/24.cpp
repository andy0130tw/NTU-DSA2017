#include <cstdio>
#include <cstring>

int T[108][108], F[108][108];

int solve(char op[], int n) {
    for (int lenMax = 1; lenMax < n; lenMax++) {
        for (int i = 0, j = lenMax; j < n; i++, j++) {
            T[i][j] = F[i][j] = 0;
            for (int len = 0; len < lenMax; len++) {
                int k = i + len;

                int leftT = T[i][k], rightT = T[k + 1][j];
                int leftF = F[i][k], rightF = F[k + 1][j];
                int cTT = leftT * rightT;
                int cTF = leftT * rightF;
                int cFT = leftF * rightT;
                int cFF = leftF * rightF;
                int totLeft = leftT + leftF;
                int totRight = rightT + rightF;
                int all = totLeft * totRight;

                switch (op[k]) {
                case 'a':  /* and */
                    T[i][j] += cTT; F[i][j] += all - cTT;
                    break;
                case 'o':  /* or */
                    F[i][j] += cFF; T[i][j] += all - cFF;
                    break;
                case 'x':  /* xor */
                    T[i][j] += cFT + cTF;
                    F[i][j] += cTT + cFF;
                }
            }
        }
    }
    return T[0][n - 1];
}

int main() {
    int n;
    scanf("%d", &n);
    while (n--) {
        memset(T, 0, sizeof(T));
        memset(F, 0, sizeof(F));
        int w;
        scanf("%d", &w);
        w /= 2;
        char op[w];
        char buf[8];
        for (int i = 0; i <= w; i++) {
            scanf("%s", buf);
            if (buf[0] == 't') {
                T[i][i] = 1;
            } else {
                F[i][i] = 1;
            }

            if (i == w) break;

            scanf("%s", buf);
            op[i] = buf[0];
        }
        printf("%d\n", solve(op, w + 1));
    }
}