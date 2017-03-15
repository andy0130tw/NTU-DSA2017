#include<unistd.h>
#include<cstring>
#include<algorithm>

#define MAX 100012
#define INPUT_SIZE 3000000
#define OUTPUT_SIZE 1010000
#define mymax(a,b)  ((a) > (b) ? (a) : (b))
#define pc(x) myputch(x)

struct range {
    int idx;
    int lbnd, rbnd;
};

int tbl[17][MAX];
range smallbucket[12][MAX / 20];
range bigbucket[5][MAX / 3];
int len[17];

static char buffer[INPUT_SIZE];
static char outbuf[OUTPUT_SIZE];
static int outloc = 0;

static inline int log2(int k) {
    return 31 - __builtin_clz(k);
}

static inline char mygetch() {
    static int idx = 0;
    return buffer[idx++];
}

static inline void myputch(char c) {
    outbuf[outloc++] = c;
}

static inline void outint(int n) {
    int N = n, rev, count = 0;
    rev = N;
    if (N == 0) { pc('0'); pc('\n'); return ;}
    while ((rev % 10) == 0) { count++; rev /= 10;}
    rev = 0;
    while (N != 0) { rev = (rev << 3) + (rev << 1) + N % 10; N /= 10;}
    while (rev != 0) { pc(rev % 10 + '0'); rev /= 10;}
    while (count--) pc('0');
}

static inline void input(int* p) {
    char c;
    int val = 0;
    while (c = mygetch(), c <= 32);
    val = c - '0';
    while ((c = mygetch())) {
        if (c <= 32) break;
        val = val * 10 + c - '0';
    }
    *p = val;
}

int main() {
    int n, m;
    read(STDIN_FILENO, buffer, INPUT_SIZE);

    input(&n), input(&m);

    int bukSize = log2(n);
    // [i][j] contains minimum of range starting from j and of size (2^i).

    for (int i = 0; i < n; i++) {
        input(&tbl[0][i]);
    }

    int step = 1;
    for (int i = 1; i <= bukSize; i++) {
        for (int j = 0, c = n - (1 << i); j <= c; j++) {
            tbl[i][j] = mymax(tbl[i - 1][j], tbl[i - 1][j + step]);
        }
        step <<= 1;
    }

    for (int i = 0; i < m; i++) {
        int lbnd, rbnd;
        input(&lbnd), input(&rbnd);
        int size = log2(rbnd - lbnd + 1);
        range& r = (size < 12 ? smallbucket[size][len[size]] : bigbucket[size - 12][len[size]]);
        r.idx = i;
        r.lbnd = lbnd - 1;
        r.rbnd = rbnd - 1;  // 1-based to 0-based
        len[size]++;
    }

    int ans[m];
    for (int b = 0; b < 12; b++) {
        for (int i = 0, c = len[b]; i < c; i++) {
            range& r = smallbucket[b][i];
            ans[r.idx] = mymax(tbl[b][r.lbnd], tbl[b][r.rbnd - (1 << b) + 1]);
        }
    }
    for (int b = 12; b <= bukSize; b++) {
        for (int i = 0, c = len[b]; i < c; i++) {
            range& r = bigbucket[b - 12][i];
            ans[r.idx] = mymax(tbl[b][r.lbnd], tbl[b][r.rbnd - (1 << b) + 1]);
        }
    }

    for (int i = 0; i < m; i++) {
        outint(ans[i]);
        pc('\n');
    }

    write(STDOUT_FILENO, outbuf, outloc);
}
