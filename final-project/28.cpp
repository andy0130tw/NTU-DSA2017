#include <cstdio>
#include <cstring>
#include <queue>

using dpair = std::pair<int, int>;
using heap = std::priority_queue<dpair, std::vector<dpair>, std::greater<dpair> >;

int main() {
    int n, m;
    scanf("%d%d", &n, &m);

    char rstate[n][m + 2];
    const char vismask = 0x80, typemask = ~vismask;

    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j <= m + 1; j++) {
            scanf("%hhd", &rstate[i][j]);
        }
    }

    // floor: [0, n); room: [0, m+1]
    int x, y, p, q, cost_r, cost_s, cost_l;
    scanf("%d%d%d%d%d%d%d", &x, &y, &p, &q, &cost_r, &cost_s, &cost_l);
    // offset floor
    x--, p--;

    int dist[n][m + 2];
    heap pq;

    memset(dist, 0x3f, sizeof(dist));
    dist[x][y] = 0;
    pq.emplace(0, x * 512 + y);

    while (!pq.empty()) {
        int idx = pq.top().second;
        int floor = idx / 512, room = idx & 511;
        pq.pop();
        if (rstate[floor][room] & vismask) continue;
        rstate[floor][room] |= vismask;
        int curDist = dist[floor][room];

        if (room > 0 && (rstate[floor][room - 1] & typemask) != 0) {
            if (dist[floor][room - 1] > curDist + cost_r) {
                dist[floor][room - 1] = curDist + cost_r;
                pq.emplace(curDist + cost_r, floor * 512 | room - 1);
            }
        }
        if (room < m + 1 && (rstate[floor][room + 1] & typemask) != 0) {
            if (dist[floor][room + 1] > curDist + cost_r) {
                dist[floor][room + 1] = curDist + cost_r;
                pq.emplace(curDist + cost_r, floor * 512 | room + 1);
            }
        }
        if (floor > 0 && (room == 0 || room == m + 1) && (rstate[floor - 1][room] & typemask) != 0) {
            if (dist[floor - 1][room] > curDist + cost_s) {
                dist[floor - 1][room] = curDist + cost_s;
                pq.emplace(curDist + cost_s, (floor - 1) * 512 | room);
            }
        }
        if (floor < n - 1 && (room == 0 || room == m + 1) && (rstate[floor + 1][room] & typemask) != 0) {
            if (dist[floor + 1][room] > curDist + cost_s) {
                dist[floor + 1][room] = curDist + cost_s;
                pq.emplace(curDist + cost_s, (floor + 1) * 512 | room);
            }
        }
        if ((rstate[floor][room] & typemask) == 2) {
            if (floor > 0 && (rstate[floor - 1][room] & typemask) == 2) {
                if (dist[floor - 1][room] > curDist + cost_l) {
                    dist[floor - 1][room] = curDist + cost_l;
                    pq.emplace(curDist + cost_l, (floor - 1) * 512 | room);
                }
            }
            if (floor < n - 1 && (rstate[floor + 1][room] & typemask) == 2) {
                if (dist[floor + 1][room] > curDist + cost_l) {
                    dist[floor + 1][room] = curDist + cost_l;
                    pq.emplace(curDist + cost_l, (floor + 1) * 512 | room);
                }
            }
        }
    }
    printf("%d\n", dist[p][q]);
}
