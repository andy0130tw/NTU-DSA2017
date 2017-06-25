#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

struct trie {
    int count;
    int child[31];
};

struct trie_pool {
    trie* arr;
    int tlen;
};

trie persist_trie[6000000];
trie_pool tpool {persist_trie, 2};

std::vector<std::string> suffixList;

char stack[48];
int thershold;

void strrev(char dst[], char src[], int len) {
    for (int i = 0; i < len; i++) {
        dst[i] = src[len - i - 1];
    }
    dst[len] = '\0';
}

int insertTrie(trie_pool& _tp, int root, char word[], int len) {
    int t = root;
    for (int i = 0; i < len; i++) {
        int* next = _tp.arr[t].child;
        int offs = word[i] - 'a';
        _tp.arr[t].count++;
        if (next[offs] == 0) {
            next[offs] = _tp.tlen++;
        }
        t = next[offs];
    }
    _tp.arr[t].count++;
    return t;
}

void scanTrie(trie_pool& _tp, int idx, int dep) {
    int isLeaf = 1;
    for (int i = 0; i < 26; i++) {
        int desc = _tp.arr[idx].child[i];
        if (desc != 0) {
            stack[dep] = i + 'a';
            if (_tp.arr[desc].count >= thershold) {
                scanTrie(_tp, _tp.arr[idx].child[i], dep + 1);
                isLeaf = 0;
            }
        }
    }
    if (isLeaf && dep > 0) {
        stack[dep] = '\0';
        puts(stack);
    }
}

void scanTrieSave(trie_pool& _tp, int idx, int dep) {
    int isLeaf = 1;
    for (int i = 0; i < 26; i++) {
        int desc = _tp.arr[idx].child[i];
        if (desc != 0) {
            stack[dep] = i + 'a';
            if (_tp.arr[desc].count >= thershold) {
                scanTrieSave(_tp, _tp.arr[idx].child[i], dep + 1);
                isLeaf = 0;
            }
        }
    }
    if (isLeaf && dep > 0) {
        char s[48];
        strrev(s, stack, dep);
        suffixList.emplace_back(s);
    }
}

int main() {
    int n;
    scanf("%d%d", &n, &thershold);

    while (n--) {
        char word[48], word_rev[48];
        scanf("%s", word);
        int len = strlen(word);
        // root of prefix
        insertTrie(tpool, 0, word, len);
        // root of suffix
        strrev(word_rev, word, len);
        insertTrie(tpool, 1, word_rev, len);
    }

    puts("prefix:");
    scanTrie(tpool, 0, 0);

    puts("suffix:");
    scanTrieSave(tpool, 1, 0);

    std::sort(suffixList.begin(), suffixList.end());
    for (auto it = suffixList.begin(); it != suffixList.end(); ++it) {
        puts((*it).c_str());
    }
}
