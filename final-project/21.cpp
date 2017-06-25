#include <cstdio>
#include <cstring>
#include <algorithm>
#include <utility>

#define VIS_OFFSET 512
bool vis[1024];
std::pair<short, short> queue[1024];

int main() {
    int w, n;
    scanf("%d%d", &w, &n);

    int capa[n], upper = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &capa[i]);
        if (capa[i] > upper)
            upper = capa[i];
    }

    int capa_gcd = capa[0];
    for (int i = 1; i < n; i++) {
        capa_gcd = std::__gcd(capa_gcd, capa[i]);
    }

    if (w % capa_gcd != 0) {
        puts("0");
        return 0;
    }

    upper += w;

    queue[0] = { 0, 0 };
    int front = 0, rear = 1;
    while (front != rear) {
        int curCapa = queue[front].first;
        int curStep = queue[front].second;
        if (curCapa == w) break;
        front++;

        for (int i = 0; i < n; i++) {
            int trial;
            trial = curCapa + capa[i];
            if (abs(trial) <= upper && !vis[VIS_OFFSET + trial]) {
                queue[rear++] = { trial, curStep + 2 };
                vis[VIS_OFFSET + trial] = 1;
            }
            trial = curCapa - capa[i];
            if (abs(trial) <= upper && !vis[VIS_OFFSET + trial]) {
                queue[rear++] = { trial, curStep + 2 };
                vis[VIS_OFFSET + trial] = 1;
            }
        }
    }

    printf("%d\n", queue[front].second);
}
