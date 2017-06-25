#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MEMORY_SIZE 4096
#define CODE_LEN_LIMIT 65536

// expand our syntax sugar (similar to run-length encoding) to traditional
// brainfuck program; return NULL if the code exceeds the limit
//
// in order to know how long it will be, the expansion will be done in two
// passes: decide the length the first pass, allocate a space to be filled,
// and fill it in the second pass.
char* bfexpand(char* code) {
    char* p = code;
    int count = 0;
    // first pass
    int actlen = 0;
    while (*p) {
        if (isdigit(*p)) {
            if (count == 0 && *p == '0') {
                return NULL;
            }
            count = count * 10 + *p - '0';
        } else {
            if (count != 0) {
                actlen += count;
                count = 0;
            } else actlen++;
        }
        p++;
    }

    // reject
    if (count != 0) {
        return NULL;
    }

    if (actlen > CODE_LEN_LIMIT) {
        return NULL;
    }

    // second pass
    p = code;
    char* ret = (char*) malloc(actlen + 1);
    int filled = 0;
    while (*p) {
        if (isdigit(*p)) {
            count = count * 10 + *p - '0';
        } else {
            if (count != 0) {
                while (--count) {
                    ret[filled++] = *p;
                }
            }
            ret[filled++] = *p;
        }
        p++;
    }
    ret[actlen] = '\0';
    return ret;
}

// run on some brainfuck code
// return  0 if success,
//        -1 if interpret error,
//         1 if runtime error,
//         2 if tick limit exceeded
int brainfuck(char* code, const char* input, char** output,
              size_t* output_sz, size_t* tick_lim) {
    char memory[MEMORY_SIZE] = {};
    int memptr = 0;
    int stack[32];  // XXX: what is the stack size?
    int stsize = 0;
    int jmptable[CODE_LEN_LIMIT];  // XXX: can be improved by using map

    size_t tick_cnt = 0;
    int do_tick_check = tick_lim && *tick_lim > 0;

    // syntax checking; build jump table
    int pos = 0;
    while (code[pos]) {
        if (code[pos] == '[') {
            stack[stsize++] = pos;
        } else if (code[pos] == ']') {
            if (stsize == 0) {
                // stack underflow owo
                return -1;
            }
            // fill it to jump table; bi-directional
            stsize--;
            jmptable[pos] = stack[stsize];
            jmptable[stack[stsize]] = pos;
        } else if (strchr(".,<>+-", code[pos]) == NULL) {
            // the character is not in allowing list of characters
            return -1;
        }
        pos++;
    }

    if (stsize > 0) {
        return -1;
    }

    // syntax checked; start execution
    int len = pos;
    size_t output_len = 0;
    pos = 0;
    while (pos != len) {
        char c = code[pos];
        switch (c) {
        case '<':  // move left
            if (memptr == 0) {
                return 1;
            }
            memptr--;
            break;
        case '>':  // move right
            if (memptr == MEMORY_SIZE - 1) {
                return 1;
            }
            memptr++;
            break;
        case '+':  // increment
            memory[memptr]++; tick_cnt++; break;
        case '-':  // decrement
            memory[memptr]--; tick_cnt++; break;
        case ',':  // read from input; no-op if exhausted
            if (*input) {
                memory[memptr] = *input;
                input++;
            }
            tick_cnt++; break;
        case '.':  // write to output
            if (memory[memptr] == 0) {
                return 1;
            }
            (*output)[output_len++] = memory[memptr];
            // if output is longer than expected, the space is relocated
            if (output_len == *output_sz) {
                *output_sz *= 2;
                // printf("relocating memory to expand output to"
                //        " size %zd\n", *output_sz);
                *output = (char*) realloc(*output, *output_sz);
            }
            tick_cnt++; break;
        case '[':  // loop construct left
            if (memory[memptr] == 0) {
                pos = jmptable[pos];
            }
            tick_cnt++; break;
        case ']':  // loop construct right
            if (memory[memptr] != 0) {
                pos = jmptable[pos];
            }
            tick_cnt++; break;
        }

        // check tick limit
        if (do_tick_check && tick_cnt > *tick_lim) {
            return 2;
        }
        pos++;
    }

    // zero-terminate the output string
    (*output)[output_len] = '\0';
    if (tick_lim) {
        *tick_lim = tick_cnt;
    }
    return 0;
}

struct probdesc {
    int tick_limit;
    int length;
    char** list_in;
    char** list_out;
};

char* getsamp(size_t* szptr) {
    char* line = NULL;
    ssize_t len = getline(&line, szptr, stdin);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
        len--;
        line[len] = '\0';
    }
    return line;
}

int main() {
    int n;
    scanf("%d", &n);

    struct probdesc pdes[n];

    for (int i = 0; i < n; i++) {
        int sampcnt, tlim;
        scanf("%d%d", &sampcnt, &tlim);
        size_t sz = sampcnt * sizeof(char*);
        pdes[i] = (struct probdesc) {
            .tick_limit = tlim,
            .length = sampcnt,
            .list_in = (char**) malloc(sz),
            .list_out = (char**) malloc(sz),
        };

        while (getchar() != '\n'); // bogus newline

        for (int j = 0; j < sampcnt; j++) {
            size_t linesz;
            linesz = 0, pdes[i].list_in[j]  = getsamp(&linesz);
            // printf("get sample in [%s]\n", pdes[i].list_in[j]);
            linesz = 0, pdes[i].list_out[j] = getsamp(&linesz);
            // printf("get sample out [%s]\n", pdes[i].list_out[j]);
        }
    }

    int s;
    scanf("%d", &s);
    for (int i = 0; i < s; i++) {
        int pid;
        char code[CODE_LEN_LIMIT];

        int correct_count = 0;
        int wtf_count = 0;
        int tle_count = 0;

        scanf("%d%s", &pid, code);

        char* code_bf = bfexpand(code);
        if (!code_bf) {
            printf("IE 0\n");
        } else {
            int syntax_error = 0;

            for (int tid = 0, tlen = pdes[pid].length; tid < tlen; tid++) {
                size_t tick_limit = pdes[pid].tick_limit;
                size_t outsz = 4096;
                char* out = (char*) malloc(outsz);

                int ret = brainfuck(code_bf, pdes[pid].list_in[tid], &out, &outsz, &tick_limit);
                if (ret < 0) {
                    syntax_error = 1;
                    break;
                } else if (ret == 0) {
                    if (strcmp(out, pdes[pid].list_out[tid]) == 0) {
                        correct_count++;
                    } else {
                        wtf_count++;
                    }
                } else if (ret == 1) {
                    wtf_count++;
                } else if (ret == 2) {
                    tle_count++;
                }
                free(out);
            }

            if (syntax_error) {
                printf("IE 0\n");
                // skip all the tests
                continue;
            }

            // verdict
            const char* verdict = "AC";
            if (correct_count < pdes[pid].length || wtf_count > 0) {
                verdict = "WTF";
            }
            if (tle_count > 0) {
                verdict = "TLE";
            }

            // rounding
            int score = correct_count * 1000 / pdes[pid].length;
            if (score % 10 >= 5) score += 10 - score % 10;
            score /= 10;

            printf("%s %d\n", verdict, score);
        }
    }
}
