#include <cstdio>
#include <vector>

int LIS(char*s, char*t, int l) {
    int top = 0;
    t[top++] = s[0];
    for (int i = 1; i < l; ++i) {
        if (s[i] > t[top - 1])
            t[top++] = s[i];
        else
            *std::lower_bound(t, t + top, s[i]) = s[i];
    }
    return top;
}

int main() {
    int n;
    char str[4096], tmp[4096];
    scanf("%d", &n);
    while (n--) {
        int k;
        scanf("%s%n", str, &k);
        printf("%d\n", LIS(str, tmp, k - 1));
    }
}
