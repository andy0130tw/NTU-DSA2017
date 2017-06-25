#include <cstdio>

int tree[1050000];
int ndepth, nnode;
int stack[24];
int sum, tar;

void printAns(int dep) {
    printf("%d", stack[0]);
    for (int i = 1; i <= dep; i++) {
        printf(" %d", stack[i]);
    }
    puts("");
}

void dfs(int idx, int dep) {
    stack[dep] = tree[idx];
    sum += tree[idx];
    if (sum == tar) {
        printAns(dep);
    }
    int next = idx << 1;
    if (next <= nnode) dfs(next, dep + 1);
    next++;
    if (next <= nnode) dfs(next, dep + 1);
    sum -= tree[idx];
}

int main() {
    scanf("%d", &ndepth);
    nnode = (1 << (ndepth + 1)) - 1;
    for (int i = 1; i <= nnode; i++) {
        scanf("%d", &tree[i]);
    }
    scanf("%d", &tar);
    dfs(1, 0);
}
