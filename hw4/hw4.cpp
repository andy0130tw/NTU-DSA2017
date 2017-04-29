#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

extern "C" {
    #include <unistd.h>
}

#define INPUT_SIZE  33554432
#define OUTPUT_SIZE 33554432
#define RECORD_MAX  1000002

using score_t = long long;
struct record {
    score_t score;
    unsigned int ts;
};

#define eprintf(...)  fprintf(stderr, __VA_ARGS__)
// #define eprintf(...)

unsigned int n, k;
unsigned int version = 0;
record recArr[RECORD_MAX];
unsigned int head_id;
record head_norize;

static int inpptr;
static int outptr;
static char inpbuf[INPUT_SIZE];
static char outbuf[OUTPUT_SIZE];

static inline char getch() {
    return inpbuf[inpptr++];
}

static inline void putch(char c) {
    outbuf[outptr++] = c;
}

#define mygetch(x)  getch(x)

static inline unsigned int input() {
    char c;
    unsigned int val = 0;
    val = mygetch() - '0';
    while ((c = mygetch()) > 32) {
        val = val * 10 + c - '0';
    }
    return val;
}

#define pc(x)  putch(x)

static inline void outint(unsigned int n) {
    unsigned int N = n;
    int rev, count = 0;
    rev = N;
    if (N == 0) { pc('0'); return ;}
    while ((rev % 10) == 0) { count++; rev /= 10;}
    rev = 0;
    while (N != 0) { rev = (rev << 3) + (rev << 1) + N % 10; N /= 10;}
    while (rev != 0) { pc(rev % 10 + '0'); rev /= 10;}
    while (count--) pc('0');
}

static inline score_t norscr(record& rec) {
    return rec.score - (score_t)(version - rec.ts) * k;
}

static inline void tryReplaceMax(unsigned int id, score_t scr) {
    record& rec = recArr[id];
    if (scr > head_norize.score ||
        (scr == head_norize.score && rec.ts > head_norize.ts)) {
        eprintf("replacing max (%u, %lld) -> (%u, %lld)\n",
            head_id, head_norize.score, id, norscr(rec));
        head_id = id;
        head_norize = (record) {
            .score = scr,
            .ts = rec.ts,
        };
    }
}

int main() {
    int inplen = read(STDIN_FILENO, inpbuf, INPUT_SIZE);
    n = input(), k = input();

    char op;
    unsigned int id;
    score_t score;

    // populate the very first item
    mygetch() /* 1 */, mygetch() /* space */;
    id = input();
    score = input();
    version++;
    recArr[id] = (record) {
        .score = score,
        .ts = version,
    };
    head_id = id;
    head_norize = recArr[id];
    n--;

    while (n--) {
        op = mygetch(), mygetch() /* space */;
        if (op == '1') {
            // insert, <id> <score>
            id = input();
            score = input();
            version++;
            recArr[id] = (record) {
                .score = score,
                .ts = version,
            };
            head_norize.score -= k;
            tryReplaceMax(id, score);
        } else if (op == '2') {
            // update <score> <count> <id>...
            score = input();
            int count = input();
            for (int i = 0; i < count; i++) {
                id = input();
                recArr[id].score += score;
                if (id != head_id) {
                    tryReplaceMax(id, norscr(recArr[id]));
                } else {
                    head_norize.score += score;
                }
            }
        } else {
            // query
            // sprintf(buf, "id: %u, value: %lld\n", head_id, norscr(head));
            // fputs_unlocked("id: ", stdout);
            memcpy(outbuf + outptr, "id: ", 4); outptr += 4;
            outint(head_id);
            memcpy(outbuf + outptr, ", value: ", 9); outptr += 9;
            outint(head_norize.score);
            pc('\n');
        }
    }

    int dummy = write(STDOUT_FILENO, outbuf, outptr);
}
