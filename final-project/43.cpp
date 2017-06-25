#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>

using state_t = int;

static inline void fromState(int s, state_t arr[5]) {
    arr[0] = s & 0x3f;
    arr[1] = (s >> 6) & 0x3f;
    arr[2] = (s >> 12) & 0x3f;
    arr[3] = (s >> 18) & 0x3f;
    arr[4] = (s >> 24) & 0x3f;
}

static inline int toState(state_t arr[5]) {
    return (arr[4] << 24) | (arr[3] << 18) | (arr[2] << 12) | (arr[1] << 6) | arr[0];
}

int ncup, minstep;
state_t stateInit, stateFinal;
state_t qstate[10485760];
std::map<state_t, std::set<state_t> > transit;
std::vector<std::string> solarr;

state_t stack[1024];

static int backtrace(state_t s, int dep) {
    stack[dep] = s;
    if (dep == minstep) {
        if (s != stateInit) return 0;
        std::ostringstream solstr;
        for (int i = dep; i >= 0; i--) {
            int cups[5];
            fromState(stack[i], cups);
            solstr << "(" << cups[0];
            for (int j = 1; j < ncup; j++) {
                solstr << "," << cups[j];
            }
            solstr << ")";
        }
        solarr.push_back(solstr.str());
        return 1;
    }
    int ret = 0;
    auto& curPoint = transit[s];
    for (auto it = curPoint.begin(); it != curPoint.end(); ++it) {
        ret += backtrace(*it, dep + 1);
    }
    return ret;
}

int main() {
    scanf("%d", &ncup);
    int capa[5], water[5] = {};
    std::set<state_t> vis;

    for (int i = 0; i < ncup; i++) {
        scanf("%d", &capa[i]);
    }

    for (int i = 0; i < ncup; i++) {
        scanf("%d", &water[i]);
    }
    stateInit = toState(water);

    for (int i = 0; i < ncup; i++) {
        scanf("%d", &water[i]);
        if (water[i] > capa[i]) {
            puts("-1"); return 0;
        }
    }
    stateFinal = toState(water);

    int findSol = 0;

    int front = 0, rear = 1, rear_old = 1;
    qstate[0] = stateInit;

    while (front != rear) {
        while (front != rear_old) {
            state_t curr = qstate[front++];
            if (vis.find(curr) != vis.end()) continue;

            if (curr == stateFinal) {
                findSol++;
                break;
                // continue;
            }

            fromState(curr, water);

            for (int i = 0; i < ncup; i++) {
                if (water[i] != capa[i]) {
                    // not full; add to full
                    int ctemp = water[i];
                    water[i] = capa[i];
                    int ns = toState(water);
                    if (vis.find(ns) == vis.end()) {
                        qstate[rear++] = ns;
                        transit[ns].insert(curr);
                    }
                    water[i] = ctemp;
                }

                if (water[i] != 0) {
                    // not empty; drop to empty
                    int ctemp = water[i];
                    water[i] = 0;
                    int ns = toState(water);
                    if (vis.find(ns) == vis.end()) {
                        qstate[rear++] = ns;
                        transit[ns].insert(curr);
                    }
                    water[i] = ctemp;

                    // not empty; find a non-full cup and transfer to it
                    for (int j = 0; j < ncup; j++) {
                        if (i == j || water[j] == capa[j]) continue;
                        int ctemp_i = water[i], ctemp_j = water[j];
                        if (water[i] + water[j] <= capa[j]) {
                            // complete transfer
                            water[j] += water[i], water[i] = 0;
                        } else {
                            // transfer until j is full
                            water[i] -= capa[j] - water[j], water[j] = capa[j];
                        }
                        int ns = toState(water);
                        if (vis.find(ns) == vis.end()) {
                            qstate[rear++] = ns;
                            transit[ns].insert(curr);
                        }
                        water[i] = ctemp_i, water[j] = ctemp_j;
                    }
                }
            }

            vis.emplace(curr);
        }
        if (!findSol) {
            // printf("found no solution, continuing\n");
            rear_old = rear;
            minstep++;
        } else {
            // printf("find solution; break\n");
            break;
        }
    }

    if (findSol) {
        int solcnt = backtrace(stateFinal, 0);
        printf("minimum steps: %d\n", minstep);
        printf("output: %d\n", solcnt);
        std::sort(solarr.begin(), solarr.end());
        for (int i = 0; i < solcnt; i++) {
            puts(solarr[i].c_str());
        }
    } else {
        puts("-1");
    }
}
