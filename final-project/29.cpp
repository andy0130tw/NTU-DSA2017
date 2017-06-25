#include <cstdio>

struct trie {
    int child[2];
    int lease_time;
    int submask_size;
    int expire_ts;
};

static inline unsigned int merge(unsigned int ipseg[4]) {
    return (ipseg[0] << 24) | (ipseg[1] << 16) | (ipseg[2] << 8) | ipseg[3];
}

trie tpool[10485760];
int tlen = 1;

int main() {
    int n;
    scanf("%d", &n);

    for (int time = 0; time < n; time++) {
        int op;
        scanf("%d", &op);

        char _;
        int t = 0;
        unsigned int ip, ipseg[4], mask = 1U << 31;
        int submask_size, lease = -1;
        if (op == 1) {
            scanf("%d%c%d%c%d%c%d%c%d%d",
                &ipseg[0], &_, &ipseg[1], &_,
                &ipseg[2], &_, &ipseg[3], &_, &submask_size, &lease);
            ip = merge(ipseg);
            for (int i = 0; i < submask_size; i++, mask >>= 1) {
                int* newId = &tpool[t].child[!!(ip & mask)];
                if (*newId == 0) {
                    *newId = tlen++;
                }
                t = *newId;
            }
            if (submask_size >= tpool[t].submask_size) {
                tpool[t].lease_time = lease;
                tpool[t].submask_size = submask_size;
            }
        } else {
            scanf("%d%c%d%c%d%c%d",
                &ipseg[0], &_, &ipseg[1], &_,
                &ipseg[2], &_, &ipseg[3]);
            ip = merge(ipseg);

            for (; mask != 0; mask >>= 1) {
                if (lease < 0 && tpool[t].lease_time) {
                    // find valid submask; record lease and prevent updating again
                    lease = tpool[t].lease_time;
                }
                int* newId = &tpool[t].child[!!(ip & mask)];
                if (*newId == 0) {
                    if (lease >= 0) {
                        // continue on looking till end
                        *newId = tlen++;
                    } else {
                        // no valid submask yet
                        t = -1; break;
                    }
                }
                t = *newId;
            }

            if (t < 0) {
                // submask not found
                puts("Invalid");
            } else if (tpool[t].expire_ts > 0 && time <= tpool[t].expire_ts) {
                // not expired yet
                puts("HahahaXD You are too late");
            } else {
                // expired or not assigned
                printf("Your device has the ip address %d.%d.%d.%d\n",
                    ipseg[0], ipseg[1], ipseg[2], ipseg[3]);
                tpool[t].expire_ts = time + lease;
            }
        }
    }
}
