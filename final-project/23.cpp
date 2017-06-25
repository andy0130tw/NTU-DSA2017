#include <cstdio>

int arr[16777220], q[16777220];
int cnt;

void preorder(int n) {
    printf("%d ", arr[n]);
    int child = n * 2;
    while (child <= cnt && arr[child] != -1) {
        preorder(child);
        child = child * 2 + 1;
    }
}

void postorder(int n) {
    int child = n * 2;
    while (child <= cnt && arr[child] != -1) {
        postorder(child);
        child = child * 2 + 1;
    }
    printf(" %d", arr[n]);
}

void levelorder() {
    int f = 0, r = 1;
    q[0] = 1;
    while (f != r) {
        printf("%d ", arr[q[f]]);
        int child = q[f] * 2;
        while (child <= cnt && arr[child] != -1) {
            q[r++] = child;
            child = child * 2 + 1;
        }
        f++;
    }
}

int main() {
    int n;
    while (scanf("%d", &n)) {
        if (n == 0) break;
        cnt = (1 << n) - 1;
        for (int i = 1; i <= cnt; i++) {
            scanf("%d", &arr[i]);
        }
        printf("preorder: ");    preorder  (1); putchar('\n');
        printf("postorder:");    postorder (1); putchar('\n');
        printf("level order: "); levelorder();  putchar('\n');
        puts("");
    }
}
