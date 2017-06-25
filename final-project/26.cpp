#include <cstdio>
#include <algorithm>


int main() {
    int n;
    scanf("%d", &n);

    int id[n], par[n], q[n];

    for (int i = 0; i < n; i++) {
        scanf("%d", &id[i]);
    }
    for (int i = 0; i < n; i++) {
        scanf("%d", &par[i]);
    }

    scanf("%d", &q[0]);

    int f = 0, r = 1;
    while (f != r) {
        int el = q[f++];
        for (int i = 0; i < n; i++) {
            if (par[i] == el) {
                q[r++] = id[i];
            }
        }
    }

    std::sort(q, q + r);
    printf("%d", q[0]);
    for (int i = 1; i < r; i++) {
        printf(" %d", q[i]);
    }
    puts("");
}
