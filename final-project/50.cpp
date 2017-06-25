#include <cstdio>
#include <queue>
#include <utility>

using heap = std::priority_queue<double>;

template <class T> struct cmp_first {
    bool operator() (const T& x, const T& y) const noexcept {
        if (x.first == y.first) return x.second > y.second;
        return x.first < y.first;
    }
};

template <class T1, class T2>
using heap_pair_first = std::priority_queue<
    std::pair<T1, T2>,
    std::vector<std::pair<T1, T2> >,
    cmp_first<std::pair<T1, T2> >
>;

using pfheap = heap_pair_first<double, int>;

__attribute((pure))
static double average(const heap& h) {
    int cnt = h.size();
    double total = 0;
    auto c = &h.top();
    for (int i = 0; i < cnt; i++) {
        total += c[i];
    }
    return total / cnt;
}

static void heap_print(const heap& h) {
    int cnt = h.size();
    if (cnt == 0) {
        puts(""); return;
    }
    auto c = &h.top();
    printf("%g", c[0]);
    for (int i = 1; i < cnt; i++) {
        printf(" %g", c[i]);
    }
    puts("");
}

static void heap_print(const pfheap& h) {
    int cnt = h.size();
    if (cnt == 0) {
        puts(""); return;
    }
    auto c = &h.top();
    printf("%g", c[0].first);
    for (int i = 1; i < cnt; i++) {
        printf(" %g", c[i].first);
    }
    puts("");
}

static double find_nth(const heap& h, int n) {
    heap h2 {h};
    double top = 9e99;
    if (n == 0) n = 1;  // FUCK THE SPEC
    while (!h2.empty()) {
        double ext = h2.top();
        if (ext != top) {
            n--;
            top = ext;
            if (n == 0) return top;
        }
        // printf("popped %g\n", ext);
        h2.pop();
    }
    return top;
}

static int find_nth_class(const pfheap& h, int n) {
    pfheap h2 {h};
    auto top = std::make_pair(9e99, -1);
    if (n == 0) n = 1;  // FUCK THE SPEC
    while (!h2.empty()) {
        auto ext = h2.top();
        if (ext.first != top.first) {
            n--;
            top = ext;
            if (n == 0) return top.second;
        }
        // printf("popped class avg=%g, id=%d\n", ext.first, ext.second);
        h2.pop();
    }
    return top.second;
}

int main() {
    int nclass;
    scanf("%d", &nclass);

    heap score_heaps[nclass];

    while (1) {
        int cid;
        double score;
        scanf("%d", &cid);
        if (cid == -1) break;
        scanf("%lf", &score);
        score_heaps[cid - 1].emplace(score);
    }

    double averages[nclass];
    for (int i = 0; i < nclass; i++) {
        averages[i] = average(score_heaps[i]);
    }

    pfheap avg_heap;
    for (int i = 0; i < nclass; i++) {
        avg_heap.emplace(averages[i], i);
    }

    int op, s1, s2;
    while (scanf("%d", &op) != EOF) {
        switch (op) {
        case 1:
            printf("%g\n", avg_heap.top().first);
            break;
        case 2:
            scanf("%d", &s1);
            printf("%g\n", score_heaps[s1 - 1].top());
            break;
        case 3:
            scanf("%d%d", &s1, &s2);
            printf("%g\n", find_nth(score_heaps[find_nth_class(avg_heap, s1)], s2));
            break;
        case 4:
            scanf("%d", &s1);
            heap_print(score_heaps[s1 - 1]);
            break;
        case 5:
            heap_print(avg_heap);
            break;
        }
    }
}
