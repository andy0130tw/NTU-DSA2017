#include <cstdio>
#include <algorithm>

int arr[102400], stack[102400], top;

int main() {

    int n = 0;

    while (~scanf("%d", &arr[n])) n++;

    int ans = 0, cur = 0;

    while (cur < n) {
        while (top > 0 && arr[cur] > arr[stack[top - 1]]) {
            int elem = stack[--top];
            if (!top) break;
            int dist = cur - stack[top - 1] - 1;
            int height = std::min(arr[cur], arr[stack[top - 1]]) - arr[elem];
            ans += dist * height;
        }
        if (arr[cur] > 0) {
            stack[top++] = cur;
        } else {
            top = 0;
        }
        cur++;
    }

    printf("%d\n", ans);
}
