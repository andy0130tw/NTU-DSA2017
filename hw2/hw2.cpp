#include<cstdio>
#include<cstring>
#include<algorithm>
#include<vector>
#include<unordered_set>

extern "C" {
#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/times.h>
#include<sys/resource.h>
}

using namespace std;

#define NGRAM_MAX_N      5
#define RESULT_SET_MAX   5
#define WORD_STORE_SIZE  4194304
#define HASH_SIZE        1048576  // MUST be power of 2
#define HASH_NUM_ENTRIES 300000
#define NGRAM_ENTRIES    42000000

#define eprintf(...) fprintf(stderr, __VA_ARGS__)
// #define eprintf(...)

using hashKey = unsigned int;
using docCollection = vector<unsigned int>;

static struct _wordStore {
    char chunk[WORD_STORE_SIZE];
    unsigned int len;
    char* insert(char w[]) {
        int wlen = strlen(w) + 1;
#ifdef CHECK_WORDTABLE
        if (len + wlen >= WORD_STORE_SIZE) abort();
#endif  // CHECK_WORDTABLE
        char* ret = chunk + len;
        strcpy(ret, w);
        len += wlen;
        return ret;
    }
    unsigned int convOffset(char* c) {
        return c - chunk;
    }
    char* get(unsigned int offset) {
        return chunk + offset;
    }
    void dump() {
        for (size_t i = 0; i < len; i++) {
            putchar(chunk[i] ? chunk[i] : ' ');
            if (i % 100 == 99) puts("");
        }
        puts("");
    }
} wordStore;

struct doc {
    unsigned int freq;
    unsigned int word[NGRAM_MAX_N];
};

static doc docArr[NGRAM_ENTRIES];
static unsigned int clusterResult[4][RESULT_SET_MAX];

struct hashNode {
    unsigned int word;
    docCollection doclist[4];  // n-gram
 };

struct queryString {
    char** query;
    int length;
};

static hashNode* hashMap[HASH_SIZE];
static int hashEntriesUsed = 0;
static hashNode* getHashEntry() {
    static hashNode hashEntries[HASH_NUM_ENTRIES];
#ifdef CHECK_HASHTABLE
    if (hashEntriesUsed >= HASH_NUM_ENTRIES) {
        eprintf("Hash node pool exhausted (%d entries)!! Aborting...\n", HASH_NUM_ENTRIES);
        abort();
    }
#endif  // CHECK_HASHTABLE
    return &hashEntries[hashEntriesUsed++];
}

hashKey hashFunc(const char s[]) {
    // sdbm hash!
    hashKey ret = 0;
    hashKey c;
    while ((c = *s++)) {
        ret = c + (ret << 6) + (ret << 16) - ret;
    }
    // return ret % HASH_SIZE;
    return ret & (HASH_SIZE - 1);
}

// from DSnP. The method is platform-specific.
double checkMem() {
    struct rusage usage;
    if (0 == getrusage(RUSAGE_SELF, &usage))
        return usage.ru_maxrss;
    else
        return 0;
}

// search if the word is in the hashMap; return a pointer to it if found,
// otherwise return NULL with position (if specified) set to an offset
// that can be used to safely store the item
hashNode* hashSearch(const char w[], hashKey* pos = nullptr) {
    hashKey hashVal = hashFunc(w);
    hashKey v = hashVal;
    while (hashMap[v] != nullptr) {
        if (strcmp(w, wordStore.get(hashMap[v]->word)) == 0) {
            return hashMap[v];
        }
        v++;
        if (v == HASH_SIZE) v = 0;
        if (v == hashVal) break;  // table full; should not happen
    }
    // hashMap[v] is safe to insert (as long as the table is not full)
    if (pos != nullptr) {
        *pos = v;
    }
    return nullptr;
}

void dumpHashMap() {
    for (int i = 0; i < HASH_SIZE; i++) {
        putchar(hashMap[i] ? 'x' : '_');
        if (i % 100 == 99) putchar('\n');
    }
    putchar('\n');
}

static inline bool docInstCmp(const doc& a, const doc& b) {
    if (a.freq > b.freq) return true;
    if (a.freq < b.freq) return false;
    for (unsigned int i = 0; i < NGRAM_MAX_N; i++) {
        int cmp = strcmp(
            wordStore.get(a.word[i]),
            wordStore.get(b.word[i]));
        if (cmp < 0) return true;
        else if (cmp > 0) return false;
    }
    return false;
}

static inline bool resultCmp(const unsigned int& a, const unsigned int& b) {
    return docInstCmp(docArr[a], docArr[b]);
}

/**************** HIGH-LEVEL OPERATIONS START HERE ****************/

// return 1 iff input (in document) matches pattern (in queryString).
int patternMatch(queryString* qs, doc* inp, int inpLen) {
    // have consumed 0, 1, ..., N, tokens, where N is the input length
    // so there are N + 1 states at most, and we use N + 1 bits.
    int states[2], flip = 0;
    // the cursor remains at the first position
    states[0] = 1;

    for (int i = 0; i < qs->length; i++) {
        // clean state
        states[!flip] = 0;
        if (qs->query[i][0] == '*') {
            for (int j = 0; j <= inpLen; j++) {
                if (states[flip] & (1 << j)) {
                    // set flag to (the first one met from start) and beyond
                    while (j <= inpLen) states[!flip] |= 1 << j, j++;
                    break;
                }
            }
        } else {
            for (int j = 0; j < inpLen; j++) {
                char* inpStr = wordStore.get(inp->word[j]);
                // for all mark set, if match (either wildcard or word equal)
                // then pass the mark to the next position
                if ((states[flip] & (1 << j)) &&
                   (qs->query[i][0] == '_' || strcmp(qs->query[i], inpStr) == 0)) {
                    states[!flip] |= 1 << (j + 1);
                }
            }
        }
        flip = !flip;
    }
    // peek the last state at the end,
    // accept if it is marked, reject if not
    return states[flip] & (1 << inpLen);
}

// preform an n-way merge on lists of documents and append the intersection to the result collection
void filterDocByWords(docCollection* lists[], int cnt, docCollection* result) {
    docCollection::iterator iters[cnt];
    for (int i = 0; i < cnt; i++) {
        iters[i] = lists[i]->begin();
    }

    while (1) {
        int tarIdx = -1;
        unsigned int leastDocId = 1U << 30;
        for (int i = 0; i < cnt; i++) {
            if (iters[i] != lists[i]->end() && *iters[i] <= leastDocId) {
                tarIdx = i;
                leastDocId = *iters[i];
            }
        }
        if (tarIdx < 0) return;

        int ok = 1;
        for (int i = 0; i < cnt; i++) {
            // halt if any list reaches end
            if (iters[i] == lists[i]->end()) return;
            if (*iters[i] != leastDocId) {
                ok = 0;
                break;
            }
        }

        if (ok) {
            result->push_back(leastDocId);
        }
        iters[tarIdx]++;
    }
}

void buildQueryRecursive(char** boffs[], vector<queryString>* result, char* aux[], int toks) {
    static int dep = 0;
    if (dep == toks) {
        char** tmp = new char*[toks];
        int len = 0;
        for (int i = 0; i < toks; i++) {
            // skip NOTHING here (see queryPlanner() function for why)
            if (((size_t)aux[i]) == 1) continue;
            tmp[len++] = aux[i];
        }
        result->push_back((queryString) {
            .query = tmp,
            .length = len
        });
        while (len < toks) tmp[len++] = nullptr;
        return;
    }

    for (int i = 0; boffs[dep][i]; i++) {
        aux[dep] = boffs[dep][i];
        dep++;
        buildQueryRecursive(boffs, result, aux, toks);
        dep--;
    }
}

// extract the query and expand some (not all) wildcards
// puts the expanded queries and token length to result array for further analysis
// no good ways I can figure out to avoid duplications :(
void queryPlanner(char query[], vector<queryString>* result) {
    // lexing
    int tokenCnt = 0;
    char* tokens[NGRAM_MAX_N + 1] = { strtok(query, " ") };
    while (tokens[tokenCnt]) {
        tokens[++tokenCnt] = strtok(nullptr, " ");

#ifdef CHECK_QUERY
        // ensure that tokenCnt <= NGRAM_MAX_N
        if (tokenCnt > NGRAM_MAX_N) {
            eprintf("FATAL: Too many tokens (> %d given)!! Aborting...\n", NGRAM_MAX_N);
            abort();
        }
#endif  // CHECK_QUERY
    }

    for (int i = 0; i < tokenCnt; i++) {
        eprintf("%d: [%s], ", i, tokens[i]);
    }
    eprintf("end\n");

    // expanding 0/1 ("?") and slashes ("/")
    int queryEstCnt = 1;
    char* boffs[tokenCnt][24];  // at most 24 branches (or 23 slashes) in each token

    for (int i = 0; i < tokenCnt; i++) {
        int bcnt = 0, bbase = 0;
        if (tokens[i][0] == '?') {
            // when "?X" is met, we make a branch that preserves as NOTHING,
            // (that is, when building queries later, the special value is simply skipped).
            // NULL cannot be used because it is used to mark the end!
            // the small hack allows us to treat "?" transparently!!
            boffs[i][0] = (char*)((size_t) 1);
            boffs[i][1] = strtok(tokens[i] + 1, "/");
            bcnt = bbase = 1;
        } else {
            // the repeat is intentional lest I forget the logic
            boffs[i][0] = strtok(tokens[i], "/");
        }

        while (boffs[i][bcnt]) {
            char* nextBranch = strtok(nullptr, "/");
            if (nextBranch) {
                int ignore = 0;
                // ignore the token if we once met
                for (int k = bbase; k <= bcnt; k++) {
                    if (strcmp(nextBranch, boffs[i][k]) == 0) {
                        ignore = 1;
                        break;
                    }
                }
                if (ignore) continue;
            }
            // always set the final NULL
            boffs[i][++bcnt] = nextBranch;
        }

        for (int k = 0; k < bcnt; k++) {
            eprintf("%d-%d: [%s], ",
                i, k, ((size_t)boffs[i][k]) != 1 ? boffs[i][k] : "-");
        }
        eprintf("\n");
        queryEstCnt *= bcnt;
    }
    eprintf("Expanding to %d queries...\n", queryEstCnt);

    result->reserve(queryEstCnt);

    // construct inputs for building queries recursively
    char* aux[tokenCnt];

    char** boffsVec[tokenCnt];
    for (int i = 0; i < tokenCnt; i++) {
        boffsVec[i] = (char**) boffs[i];
    }

    buildQueryRecursive(boffsVec, result, aux, tokenCnt);

    for (int i = 0; i < queryEstCnt; i++) {
        eprintf(" - #%d. ", i + 1);
        int len = (*result)[i].length;
        for (int j = 0; j < len; j++) {
            eprintf("%s ", (*result)[i].query[j]);
        }
        eprintf(" (%d)\n", len);
    }

}

int serveQuery() {
    static char* query = nullptr;
    static size_t queryLen = 1024;
    ssize_t len = getline(&query, &queryLen, stdin);
    if (len == -1) return -1;
    // trim the delimiter
    query[len - 1] = '\0', len--;
    if (!len) return 0;
    printf("query: %s\n", query);

    vector<queryString> qColl;
    queryPlanner(query, &qColl);

    int collSize = qColl.size();

    hashNode* termsHashNode[collSize][NGRAM_MAX_N];
    int termCnt[collSize];      // to record numbers of words to filter with

    int lowerBound[collSize];  // to record numbers of words a query should have at least
    int upperBound[collSize];   // whether the query has Kleene-star (will allow arbitrary)
    int clusterGlobal[4] = {};  // whether the cluster is global search

    // pre-processing... calculate the bound of n-gram for each sub-query
    for (int i = 0; i < collSize; i++) {
        queryString& qs = qColl[i];
        termCnt[i] = lowerBound[i] = 0;
        upperBound[i] = qs.length;
        for (int j = 0; j < qs.length; j++) {
            if (qs.query[j][0] == '*') {
                // if containing Kleene-star, every n-gram is under consideration
                upperBound[i] = NGRAM_MAX_N;
            } else {
                // represent exactly an actual word
                lowerBound[i]++;
                if (qs.query[j][0] != '_') {
                    hashNode* node = hashSearch(qs.query[j], nullptr);
                    if (node == nullptr) {
                        eprintf("FATAL: Word \"%s\" is not found in any files\n", qs.query[j]);
                        printf("output: 0\n");
                        // XXX: memory leak
                        return 1;
                    }
                    termsHashNode[i][termCnt[i]++] = node;
                }
            }
        }

        if (lowerBound[i] <= 1) lowerBound[i] = 2;

        if ((qs.length <= 1 && upperBound[i] <= 1) || qs.length > NGRAM_MAX_N) {
            // there is no n-gram with such length, so do not perform this query!
            eprintf("register SKIP(%d) %d-word query\n", i + 1, qs.length);
            upperBound[i] = 0;
            continue;
        }

        // classify local search / global search
        if (termCnt[i] == 0) {
            for (int arity = lowerBound[i]; arity <= upperBound[i]; arity++) {
                eprintf("register GLOBAL(%d) query for %d-gram\n", i + 1, arity);
                clusterGlobal[arity - 2] = 1;
            }
        }
    }

    docCollection qResult;

    // construct searching set
    for (int arity = 2; arity <= NGRAM_MAX_N; arity++) {
        int d = arity - 2;
        eprintf("Starting queries that are on %d-gram (%s)...\n",
            arity, clusterGlobal[d] ? "global" : "local");

        // the result candidate
        docCollection qResultCand;

        // if global search then the result is trivial
        if (clusterGlobal[d]) {
            for (int k = 0; k < RESULT_SET_MAX; k++) {
                qResult.push_back(clusterResult[d][k]);
            }
            // no further check is needed; n-grams are mutually exclusive
            continue;
        }

        // only used to check for duplication
        unordered_set<unsigned int> qResultMap;

        // otherwise we need to dig into words
        for (int i = 0; i < collSize; i++) {
            if (arity < lowerBound[i] || arity > upperBound[i])
                continue;

            eprintf("[%d-gram] Processing query #%d (%d~%d-gram)...\n",
                arity, i + 1, lowerBound[i], upperBound[i]);

            docCollection* term[termCnt[i]];
            // XXX: for termCnt[i] == 1, this copying is unnecessary
            for (int k = 0; k < termCnt[i]; k++) {
                term[k] = &termsHashNode[i][k]->doclist[d];
            }
            if (termCnt[i] == 1) {
                int sz = term[0]->size();
                qResultCand.reserve(sz);
                for (int k = 0; k < sz; k++) {
                    qResultCand.push_back((*term[0])[k]);
                }
            } else {
                // perform n-way merge on each n-gram query
                filterDocByWords(term, termCnt[i], &qResultCand);
            }

            // still, "candidates" are not the final result
            // filter down the result set by automata
            for (int k = 0, c = qResultCand.size(); k < c; k++) {
                doc* tarWord = docArr + qResultCand[k];
                if (patternMatch(&qColl[i], tarWord, arity)) {
                    qResultMap.insert(qResultCand[k]);
                }
            }
        }

        qResult.insert(qResult.end(), qResultMap.begin(), qResultMap.end());
    }


    int needPartialSort = qResult.size() > RESULT_SET_MAX;

    // sort by frequencies
    if (needPartialSort) {
        partial_sort(
            qResult.begin(), qResult.begin() + RESULT_SET_MAX,
            qResult.end(), resultCmp);
    } else {
        sort(qResult.begin(), qResult.end(), resultCmp);
    }

    // output our final answer
    int finalCount = needPartialSort ? RESULT_SET_MAX : qResult.size();

    printf("output: %d\n", finalCount);
    for (int i = 0; i < finalCount; i++) {
        doc* tarDoc = docArr + qResult[i];
        printf("%s", wordStore.get(tarDoc->word[0]));
        for (unsigned int k = 1; k < NGRAM_MAX_N && tarDoc->word[k] > 0; k++) {
            printf(" %s", wordStore.get(tarDoc->word[k]));
        }
        printf("\t%u\n", tarDoc->freq);
    }

    // clean up
    for (size_t i = 0; i < qColl.size(); i++) {
        delete [] qColl[i].query;
    }

    return 0;
}

static char* getstr(char* inp, char* buf) {
    while ((*inp) > 0 && (*inp) <= 32) inp++;
    while ((*inp) > 32) {
        *buf = *inp, buf++, inp++;
    }
    *buf = '\0';
    return (*inp) ? inp : nullptr;
}

static char* getfreq(char* inp, unsigned int* buf) {
    int ret = 0;
    while ((*inp) > 32) {
        ret = ret * 10 + *inp - '0', inp++;
    }
    *buf = ret;
    return (*inp) ? inp : nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        eprintf("Usage: %s <ngram-dataset-path>\n", argv[0]);
        return 1;
    }

    // ios::sync_with_stdio(false);
    // cin.tie(0);

    double baseMem = checkMem();
    eprintf("Base memory used: %4.lfKB\n", baseMem);

    FILE* dataset[4];

    size_t pathLen = strlen(argv[1]);
    char pathBuf[pathLen + 48];
    for (int i = 0; i < 4; i++) {
        int arity = i + 2;
        sprintf(pathBuf, "%s/%1dgm.small.txt", argv[1], arity);
        eprintf("Opening %d-gram file \"%s\"\n", arity, pathBuf);
        dataset[i] = fopen(pathBuf, "r");
        if (dataset[i] == nullptr) {
            perror("fopen");
            return 1;
        }
    }

    // insert empty string to location 0 for a dummy word,
    // so we no longer need to keep the arity of a document
    wordStore.insert((char*) "");

    unsigned int docCnt = 0;

    for (int i = 0; i < 4; i++) {
#ifdef VERBOSE
        int c = 0;
#endif
        int arity = i + 2;
        int isEOF = 0;
        int docStartLoc = docCnt;

        int fd = fileno(dataset[i]);
        int fileLen = lseek(fd, 0, SEEK_END);
        char* fileBuf = (char *) mmap(nullptr, fileLen, PROT_READ, MAP_PRIVATE, fd, 0);
        char* fileCur = fileBuf;

        while (1) {
            char buf[48];  // longest string is 40 characters

            // collecting nodes for later fetching
            hashNode* wordsEntry[arity];

            for (int k = 0; k < arity; k++) {
                fileCur = getstr(fileCur, buf);
                if (!fileCur) {
                    isEOF = 1;
                    break;
                }
                // insert word if not found
                hashKey insPos;
                hashNode* node = hashSearch(buf, &insPos);
                if (node == nullptr) {
#ifdef CHECK_HASHTABLE
                    if (hashMap[insPos] != nullptr) {
                        eprintf("Hash table full (%u entries)!! Aborting...\n", HASH_SIZE);
                        abort();
                    }
#endif  // CHECK_HASHTABLE
                    node = (hashMap[insPos] = getHashEntry());
                    char* wordPtr = wordStore.insert(buf);
                    node->word = wordStore.convOffset(wordPtr);
                }
                // node now points to the word object
                // node->count++;
                wordsEntry[k] = node;
            }
            if (isEOF) break;

            // create document and assign words to it
            doc* d = &docArr[docCnt++];
            unsigned int doffs = d - docArr;
            // advance to skip the tab character
            fileCur = getfreq(++fileCur, &(d->freq));

            for (int k = 0; k < arity; k++) {
                // fill in documents
                auto& tarDoclist = wordsEntry[k]->doclist[i];
                // prevent repeats
                if (tarDoclist.empty() || tarDoclist.back() != doffs)
                    tarDoclist.push_back(doffs);
                // put n-gram (i-th iter) in every word's i-th bucket
                d->word[k] = wordsEntry[k]->word;
            }

#ifdef VERBOSE
            c++;
            if ((c & 0x1ffff) == 0) {
                eprintf("\rReading %d-gram (%8d entries)... (memory = %4.lfKB)",
                    i + 2, c, checkMem() - baseMem);
            }
#endif  // VERBOSE
        }

        // clean up
        munmap(fileBuf, fileLen);
        fclose(dataset[i]);

        // pre-process the local result
        doc val[RESULT_SET_MAX + 1];
        partial_sort_copy(
            docArr + docStartLoc, docArr + docCnt,
            val, val + RESULT_SET_MAX + 1, docInstCmp);

        int collected = 0;
        for (unsigned int r = docStartLoc; r < docCnt; r++) {
            // re-use the useful compare function
            // no specific order is needed
            if (docInstCmp(docArr[r], val[RESULT_SET_MAX]))
                clusterResult[i][collected++] = r;
        }

#ifdef VERBOSE
        eprintf("\rReading %d-gram (%8d entries), ok\033[K\n", i + 2, c);
#endif  // VERBOSE
    }

    eprintf("Character table used: %8d / %8d\n", wordStore.len, WORD_STORE_SIZE);
    eprintf("Hash entries used:    %8d / %8d\n", hashEntriesUsed, HASH_SIZE);
    eprintf("Data initialized, ready for querying... (memory used = %4.lfKB)\n",
        checkMem() - baseMem);

    while (serveQuery() >= 0);
}
