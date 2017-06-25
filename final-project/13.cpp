#include <cstdio>
#include <cstring>
#include <cctype>

int pred(int c) {
    switch (c) {
    case '(': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    __builtin_unreachable();
};

int main() {
    int n;
    scanf("%d", &n);
    getchar();
    while (n--) {
        char x[120];
        char* line = x;
        size_t sz = 120;
        getline(&line, &sz, stdin);

        char out[120];
        int outp = 0;
        int stack[60], top = 0;
        for (char* pch = strtok(line, " \n"); pch != NULL; pch = strtok(NULL, " \n")) {
            if (isdigit(*pch)) {
                while (*pch) { out[outp++] = *pch; pch++; }
                out[outp++] = ' ';
            } else if (*pch == '(') {
                stack[top++] = *pch;
            } else if (*pch == ')') {
                while (top > 0 && stack[top - 1] != '(') {
                    out[outp++] = stack[--top];
                    out[outp++] = ' ';
                }
                top--;
            } else {
                while (top > 0 && pred(stack[top - 1]) >= pred(*pch)) {
                    out[outp++] = stack[--top];
                    out[outp++] = ' ';
                }
                stack[top++] = *pch;
            }
        }

        while (top > 0) {
            out[outp++] = stack[--top];
            out[outp++] = ' ';
        }
        while (out[--outp] == ' ');
        out[outp + 1] = '\0';

        puts(out);

    }
}
