#include <cstdio>

struct trie {
    int stop;
    int child[31];
};

trie tpool[1048576];
int tlen = 1;

char stack[2048];

void printTrie(int idx, int dep) {
    if (tpool[idx].stop) {
        char p = stack[dep];
        stack[dep] = '\0';
        puts(stack);
        stack[dep] = p;
    }
    for (int i = 0; i < 26; i++) {
        if (tpool[idx].child[i] != 0) {
            stack[dep] = i + 'a';
            printTrie(tpool[idx].child[i], dep + 1);
        }
    }
}

int main() {
    int n, m;
    scanf("%d", &n);
    getchar();

    while (n--) {
        char c;
        int t = 0;  // root
        while ((c = getchar()) > 32) {
            int* next = tpool[t].child;
            int offs = c - 'a';
            if (next[offs] == 0) {
                next[offs] = tlen++;
            }
            t = next[offs];
        }
        if (t == 0) {
            n++; continue;
        }
        // mark as stop
        tpool[t].stop = 1;
    }

    scanf("%d", &m);
    getchar();

    while (m--) {
        char c;
        int t = 0;  // root
        int dep = 0;
        int found = 1;
        while ((c = getchar()) > 32) {
            int* next = tpool[t].child;
            int offs = c - 'a';
            if (next[offs] == 0) {
                found = 0;
                while (getchar() > 32);
                break;
            }
            stack[dep++] = c;
            t = next[offs];
        }
        if (t == 0 && found == 1) {
            m++; continue;
        }
        if (found) {
            printTrie(t, dep);
        } else {
            puts("NO MATCHING!");
        }
    }
}
