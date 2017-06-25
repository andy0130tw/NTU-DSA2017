#include <cstdio>
#include <cstring>

int arr[2][100];
long long gg[2][100];

void __attribute((constructor)) prepare() {
    for (int i = 3; i <= 80; i += 3) {
        arr[0][i] = 1;
    }
    for (int i = 3; i <= 80; i += 3) {
        arr[1][i] = 1;
    }
    gg[0][1] = 1;
    gg[0][2] = 1;
    gg[0][3] = 2;
    for (int i = 4; i <= 80; i++) {
        gg[0][i] = gg[0][i - 1] + gg[0][i - 3];
    }

    gg[1][0] = 1;
    gg[1][3] = 3;
    gg[1][6] = 13;
    for (int i = 9; i <= 80; i += 3) {
        gg[1][i] = 5 * gg[1][i - 3] - 3 * gg[1][i - 6] + gg[1][i - 9];
    }
}

int main() {
    int k, m, n;
    scanf("%d", &k);

    while (k--) {
        scanf("%d%d", &m, &n);
        if (m >= 3) {
            printf("%lld\n", gg[m - 3][n]);
        } else {
            printf("%d\n", arr[m - 1][n]);
        }
    }
}
