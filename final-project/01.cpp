#include <cstdio>

struct trie {
    int stop;
    int child[31];
    int childNum[31];
};

trie tpool[1048576];
int tlen = 1;

char stack[2048];

int findTrie(int idx, char c[], int dep) {
    if (c[dep] == '\0') {
        return 0;
    }
    int ret = tpool[idx].stop, offs = c[dep] - 'a';
    for (int i = 0; i < offs; i++) {
        // printf("!!%d\n", tpool[idx].childNum[i]);
        ret += tpool[idx].childNum[i];
    }
    return ret + findTrie(tpool[idx].child[offs], c, dep + 1);
}

int main() {
    int n;
    scanf("%d", &n);
    getchar();

    while (n--) {
        int op = getchar();
        if (op == '1') {
            getchar();  // space
            char c;
            int t = 0;  // root
            while ((c = getchar()) > 32) {
                int* next = tpool[t].child;
                int offs = c - 'a';
                tpool[t].childNum[offs]++;
                if (next[offs] == 0) {
                    next[offs] = tlen++;
                }
                t = next[offs];
            }
            // mark as stop
            tpool[t].stop = 1;
        } else {
            int idx1, idx2;
            char str[256];
            scanf("%s", str);
            idx1 = findTrie(0, str, 0);
            scanf("%s", str);
            idx2 = findTrie(0, str, 0);
            printf("%d\n", idx2 - idx1 + 1);
            getchar();
        }
    }
}
