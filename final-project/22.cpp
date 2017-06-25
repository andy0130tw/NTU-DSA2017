#include <cstdio>
#include <cstring>
#define TREE_LEN 256

struct tree {
    char val;
    short c0, c1;
};

char stack[TREE_LEN];
char encoding[128][TREE_LEN];

tree treePool[1024];
short ptr = 1;

void mktree(int idx, int dep) {
    char buf[8];
    scanf("%s", buf);

    if (buf[0] == 'I') {
        // 'I', internal node
        stack[dep] = '0';
        treePool[idx].c0 = ptr;
        mktree(ptr++, dep + 1);
        stack[dep] = '1';
        treePool[idx].c1 = ptr;
        mktree(ptr++, dep + 1);
    } else {
        // 'L', leaf
        scanf("%s", buf);
        treePool[idx].val = buf[0];
        stack[dep] = '\0';
        memcpy(encoding[(int)buf[0]], stack, TREE_LEN);
    }
}

int main() {
    char _[8];  // placeholder
    char data[1024];
    mktree(0, 0);
    int n;
    scanf("%s%s%d", _, _, &n);  // "E", op, length

    if (_[0] == '0') {
        // encode
        while (n--) {
            scanf("%s", data);
            printf("%s\n", encoding[(int)data[0]]);
        }
    } else {
        // decode
        while (n--) {
            scanf("%s", data);
            short t = 0;
            for (char* p = data; *p; p++) {
                if (*p == '0') {
                    t = treePool[t].c0;
                } else {
                    t = treePool[t].c1;
                }
            }
            printf("%c\n", treePool[t].val);
        }
    }
}
