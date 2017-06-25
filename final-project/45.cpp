#include <cstdio>

int k;
int arr[127];

void inorder(int idx) {
    int next = idx * 2;
    if (next <= k && arr[next] != -1) inorder(next);
    printf("%d ", arr[idx]);
    next++;
    if (next <= k && arr[next] != -1) inorder(next);
}

int main() {
    int n;
    scanf("%d", &n);
    while (n--) {
        scanf("%d", &k);

        for (int i = 1; i <= k; i++) {
            scanf("%d", &arr[i]);
        }
        if (arr[1] != -1) {
            inorder(1);
        }
        if (n) puts("");
    }
}
