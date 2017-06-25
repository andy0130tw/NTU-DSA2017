#include <cstdio>
#define MAX 100010
using ull = unsigned long long int;
using ll = unsigned long long int;

struct oper_t {
    int ll;
    int rr;
};

oper_t oper[MAX];
int oper_count[MAX];
ull oper_amount[MAX];
int oper_cnt_diff[MAX];
ll x_diff[MAX];

int main() {
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);

    ull x[n];

    for (int i = 0; i < n; i++) {
        scanf("%lld", &x[i]);
    }
    for (int i = 0; i < m; i++) {
        scanf("%d%d%lld", &oper[i].ll, &oper[i].rr, &oper_amount[i]);
    }
    for (int i = 0; i < k; i++) {
        int ll, rr;
        scanf("%d%d", &ll, &rr);
        oper_cnt_diff[ll - 1]++;
        oper_cnt_diff[rr]--;
    }

    int inc = 0;
    for (int i = 0; i < m; i++) {
        inc += oper_cnt_diff[i];
        oper_count[i] = inc;
        ull diff = oper_count[i] * oper_amount[i];
        x_diff[oper[i].ll - 1] += diff;
        x_diff[oper[i].rr    ] -= diff;
    }

    ll incll = x_diff[0];
    printf("Yes\n%lld", x[0] + incll);

    for (int i = 1; i < n; i++) {
        incll += x_diff[i];
        printf(" %lld", x[i] + incll);
    }

    puts("");
}
