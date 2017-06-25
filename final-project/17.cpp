#include <cstdio>
#include <utility>

std::pair<int, int> stack[160010];

int main() {
    int n;
    scanf("%d", &n);

    while (n--) {
        int top = 0;
        const char* ans = "0";
        int inp;

        while (scanf("%d", &inp)) {
            if (inp == -1) break;

            if (top == 0 || inp < stack[top - 1].first) {
                stack[top++] = {inp, inp};
            } else {
                while (top > 0 && inp >= stack[top - 1].second) {
                    top--;
                }
                // implied `top == 0 || inp < stack[top - 1].second` now
                if (top > 0 && stack[top - 1].first < inp) {
                    ans = "1";
                    while (scanf("%d", &inp), inp != -1);
                    break;
                } else {
                    top++;
                    stack[top - 1].second = inp;
                }
            }
        }

        puts(ans);
    }
}
