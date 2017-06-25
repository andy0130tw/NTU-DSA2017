#include <cstdio>
#include <cstring>
#include <algorithm>

int tab[10000][100];
int njump, nforb, nmax;
int jump[100];
bool forb[10000];

int solve(int curr, int steptk) {
    if (curr > nmax || curr < 0 || forb[curr]) return -1;
    if (steptk == njump) return curr;
    if (tab[curr][steptk]) return tab[curr][steptk];

    return tab[curr][steptk] =
        std::max(solve(curr + jump[steptk], steptk + 1),
                 solve(curr - jump[steptk], steptk + 1));
}

int main() {
    int n;
    scanf("%d", &n);
    while (n--) {
        scanf("%d%d%d", &njump, &nforb, &nmax);

        for (int i = 0; i < njump; i++) {
            scanf("%d", &jump[i]);
        }
        for (int i = 0; i < nforb; i++) {
            int tmp;
            scanf("%d", &tmp);
            forb[tmp] = true;
        }

        printf("%d\n", solve(0, 0));

        // clean after usage
        memset(tab, 0, sizeof(tab));
        memset(forb, 0, sizeof(forb));
    }
}
