#include<cstdio>

extern "C" {
#include<unistd.h>
}

using namespace std;

#define INPUT_SIZE 524288
#define OUTPUT_SIZE 16777216
#define STEPS_MAX 2048
#define DETECT_EMPTY(x)  (((x) - 0x1111111111111111ULL) & ~(x) & 0x8888888888888888ULL)
#define DETECT_2048(x)  DETECT_EMPTY((x) ^ 0xbbbbbbbbbbbbbbbbULL)
#define __NOSOLBS_R0  "-1 -1 -1 -1\n"
#define __NOSOLBS_R1  __NOSOLBS_R0 __NOSOLBS_R0
#define NO_SOLUTION_BSTATE  __NOSOLBS_R1 __NOSOLBS_R1

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

struct tabletabletable {
    int arr[4][4];
};

using bstate = unsigned long long;
using table = tabletabletable;

// for linking purpose only
bstate nextMap(bstate map, int action);
table decodedecode(bstate map);
bstate encodeencode(table);

// but choose more convenient names
auto decode = decodedecode;
auto encode = encodeencode;

static inline int log2(int k) {
    return 31 - __builtin_clz(k);
}

static char inpbuf[INPUT_SIZE];
static char outbuf[OUTPUT_SIZE];
static int outloc;

static inline char mygetch() {
    static int idx = 0;
    if (idx == INPUT_SIZE) idx = 0;
    if (idx == 0) {
        int ret = read(STDIN_FILENO, inpbuf, INPUT_SIZE);
        if (ret <= 0) return '\0';
        if (ret < INPUT_SIZE) inpbuf[ret] = '\0';
    }
    return inpbuf[idx++];
}

static inline void myputch(char c) {
    outbuf[outloc++] = c;
}

static inline int input() noexcept {
    char c;
    int val = 0;
    while (c = mygetch(), c <= 32);
    val = c - '0';
    while ((c = mygetch())) {
        if (c <= 32) break;
        val = val * 10 + c - '0';
    }
    return val;
}

#define pc(x) myputch(x)

static inline void outint(int n) {
    int N = n, rev, count = 0;
    rev = N;
    if (N == 0) { pc('0'); return ;}
    while ((rev % 10) == 0) { count++; rev /= 10;}
    rev = 0;
    while (N != 0) { rev = (rev << 3) + (rev << 1) + N % 10; N /= 10;}
    while (rev != 0) { pc(rev % 10 + '0'); rev /= 10;}
    while (count--) pc('0');
}

static inline void outstr(const char* str) {
    while (*str) {
        pc(*str++);
    }
}

void outTable(const table& k) noexcept {
    for (int i = 0; i < 4; i++) {
        outint(k.arr[i][0] ? 1 << k.arr[i][0] : 0); pc(' ');
        outint(k.arr[i][1] ? 1 << k.arr[i][1] : 0); pc(' ');
        outint(k.arr[i][2] ? 1 << k.arr[i][2] : 0); pc(' ');
        outint(k.arr[i][3] ? 1 << k.arr[i][3] : 0); pc('\n');
    }
}

static bstate finalState;
static char stack[STEPS_MAX];

int solve(const bstate k, int dep) {
    static const int stepCand[4] = { 0, 1, 2, 3 };
    if (DETECT_2048(k)) {
        finalState = k;
        return dep;
    }

    for (int i = 0; i < 4; i++) {
        stack[dep] = stepCand[i];
        bstate newState = nextMap(k, stepCand[i]);
        int soldep;
        if (newState != k && (soldep = solve(newState, dep + 1)) >= 0)
            return soldep;
    }
    // if no step is eligible
    return -1;
}

int main() {
    read(STDIN_FILENO, inpbuf, INPUT_SIZE);
    int n = input();
    while (n--) {
        table x;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                // avoid the case that input equals to zero
                x.arr[i][j] = log2(input() | 1);
            }
        }

        finalState = 0ULL;
        int solStep = solve(encode(x), 0);
        if (solStep >= 0) {
            outstr("Action:\n");
            if (solStep > 0) {
                outint(stack[0]);
                for (int i = 1; i < solStep; i++) {
                    pc(' ');
                    pc('0' + stack[i]);
                }
            }
            outstr("\nFinal:\n");
            table final = decode(finalState);
            outTable(final);
        } else {
            outstr("Action:\n-1\nFinal:\n" NO_SOLUTION_BSTATE);
        }
    }
    write(STDOUT_FILENO, outbuf, outloc);
}
