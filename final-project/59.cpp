#include <cstdio>

int arr[100040];
int stack[100040], top;

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        int n;
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }

        int k = 1;
        int i = 0;

        top = 0;
        for (; i < n; i++) {
            if (top == 0 || stack[top - 1] != arr[i]) {
                if (k > arr[i]) break;
                while (k < arr[i]) {
                    stack[top++] = k++;
                }
                k++;
            } else {
                top--;
            }
        }

        printf(i == n ? "Y 1\n" : "Y 2\n");
    }
}
