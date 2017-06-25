#include <cstdio>
#include <cstring>
#include <algorithm>

int findNext(char s[], int n) {
    int i, j;
    for (i = n - 1; i > 0; i--)
        if (s[i] > s[i - 1])
           break;
    if (i == 0) {
        return 0;
    }
    int x = s[i - 1], smallest = i;
    for (j = i + 1; j < n; j++)
        if (s[j] > x && s[j] < s[smallest])
            smallest = j;
    std::swap(s[smallest], s[i-1]);
    std::sort(s + i, s + n);
    return 1;
}

int main() {
    int n;
    scanf("%d", &n);
    while (n--) {
        int e;
        char digits[12];
        scanf("%d%s", &e, digits);

        int len = strlen(digits);
        while (e--) {
            if (findNext(digits, len) == 0) {
                break;
            }
        }

        if (e < 0) {
            printf("%s\n", digits);
        } else {
            printf("NOPE!!\n");
        }
    }

    return 0;
}
