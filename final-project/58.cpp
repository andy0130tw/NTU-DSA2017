#include<cstdio>

int stack[100], top, out[100], l, acc, n, lim;

void printSol() {
    for (int i = 0; i < n; i++) {
        printf("%d ", out[i] + 1);
    }
    putchar('\n');
}

void dfs(int dep) {
    if (dep == lim) {
        printSol();
        return;
    }

    if (top < n && l < n) {
        stack[top++] = l++;
        dfs(dep + 1);
        top--, l--;
    }

    if (top > 0) {
        int tmp = stack[--top];
        out[acc++] = tmp;
        dfs(dep + 1);
        stack[top++] = tmp, acc--;
    }
}

int main() {
    scanf("%d", &n);
    lim = 2 * n;
    dfs(0);
}
