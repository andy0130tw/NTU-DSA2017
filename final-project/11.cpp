#include <cstdio>

int arr[131072];

int main() {
    int inp;
    int slotMin = 1;
    while (~scanf("%d", &inp)) {
        if (inp == 1) {
            scanf("%d", &inp);
            while (arr[slotMin] > 0) slotMin++;
            arr[slotMin] = inp;
        } else {
            int drop = arr[1];
            int i = 1;
            while (arr[i] > 0) {
                int l = arr[i * 2];
                int r = arr[i * 2 + 1];
                if (l != 0 || r != 0) {
                    if (l >= r) {
                        arr[i] = l;
                        i = i * 2;
                    } else {
                        arr[i] = r;
                        i = i * 2 + 1;
                    }
                } else {
                    arr[i] = 0;
                    if (i < slotMin) slotMin = i;
                }
            }
            printf("%d\n", drop);
        }
    }
}
