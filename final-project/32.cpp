#include <cstdio>
#include <algorithm>

int main() {
    int n;
    scanf("%d", &n);

    while (n--) {
        int npair, nquery;
        scanf("%d%d", &npair, &nquery);

        unsigned int pp[2 * npair];

        int ll, rr;
        for (int i = 0; i < npair; i++) {
            scanf("%u%u", &ll, &rr);
            pp[i * 2] = ll;
            pp[i * 2 + 1] = rr + 1;
        }

        std::sort(pp, pp + 2 * npair);

        int qry;
        for (int i = 0; i < nquery; i++) {
            scanf("%d", &qry);
            printf("%d\n", int(std::upper_bound(pp, pp + 2 * npair, qry) - pp) & 1);
        }
    }
}
