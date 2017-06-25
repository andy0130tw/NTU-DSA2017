#include <cstdio>
#include <cstring>
#define readcord(B, X, Y)  (((X) = (B)[0] - 'a'), ((Y) = (B)[1] - '1'))

short mblank[10][10];
short dirx[] = { 2, 1, -1, -2, -2, -1, 1, 2 };
short diry[] = { 1, 2, 2, 1, -1, -2, -2, -1 };

int main() {
    char buf[4];  // FUCK THE PROBLEM STATEMENT FOR WA
    int nholes;
    int ncases;
    scanf("%d", &nholes);

    for (int i = 0; i < nholes; i++) {
        short x, y;
        getchar();
        scanf("%s", buf); readcord(buf, x, y);
        mblank[x][y] = -1;
    }

    scanf("%d", &ncases);
    while (ncases--) {
        short srcx, srcy, distx, disty;
        scanf("%s", buf); readcord(buf, srcx, srcy);
        scanf("%s", buf); readcord(buf, distx, disty);

        int ans = -1;
        short src = (srcx << 4) | srcy;
        short dist = (distx << 4) | disty;

        short m[10][10];
        memcpy(m, mblank, sizeof(m));

        short queue[128], front = 0, rear = 1;
        queue[0] = src;
        if (src == dist) ans = 0;
        while (front != rear) {
            short x = (queue[front] >> 4) & 0xf;
            short y = queue[front] & 0xf;
            for (int i = 0; i < 8; i++) {
                int nx = x + dirx[i];
                int ny = y + diry[i];
                if (nx < 0 || nx >= 9 || ny < 0 || ny >= 9)
                    continue;
                short steps = (queue[front] >> 8) + 1;
                short nstate = (nx << 4) | ny;

                if (nstate == dist) {
                    ans = steps;
                    break;
                }
                if (m[nx][ny] < 0)
                    continue;

                queue[rear++] = (steps << 8) | nstate;
                m[nx][ny] = -1;
            }
            front++;

            if (ans >= 0) {
                printf("Going from %c%c to %c%c needs %d moves.\n",
                    srcx + 'a', srcy + '1', distx + 'a', disty + '1', ans);
                break;
            }
        }
    }
}
