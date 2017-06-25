#include <cstdio>
#include <list>
#include <queue>

int k;
char buffer[131072];

template <class T> struct iter_greater {
    bool operator() (const T& x, const T& y) const noexcept {
        return *x > *y;
    }
};

template <class T>
using min_heap = std::priority_queue<T, std::vector<T>, iter_greater<T> >;

void readLBR() {
    char c;
    int n = 0;
    while ((c = getchar()) > 32) {
        if (c == 'B') continue;
        if (n == 0 && c == 'L') continue;
        buffer[n++] = c;
    }
    while (n > 0 && buffer[n - 1] == 'R') n--;
    buffer[n] = '\0';
}

int task1() {
    int cnt = 0;
    for (char* p = buffer; *p; p++) {
        if (*p == 'R' && *(p + 1) == 'L')
            cnt++;
    }
    return cnt << 1;
}

int task2() {
    int lcnt = 0;
    for (char* p = buffer; *p; p++) {
        if (*p == 'L') lcnt++;
    }
    int cnt = 0;
    for (int i = 0; i < lcnt; i++) {
        if (buffer[i] == 'R') {
            cnt++;
        }
    }
    return cnt;
}

int task3() {
    int ans = task1();
    std::list<std::pair<int, bool>> arr = { {1 << 20, 0} };
    using list_iter = decltype(arr)::iterator;
    min_heap<list_iter> heap;
    char c = 'R';  // must starting with R
    int cnt = 0;
    for (char* p = buffer; *p; p++) {
        if (*p != c) {
            auto it = arr.insert(arr.end(), {cnt, 0});
            heap.emplace(it);
            cnt = 0;
            c = *p;
        }
        cnt++;
    }
    auto it = arr.insert(arr.end(), {cnt, 0});
    heap.emplace(it);
    arr.push_back({1 << 20, 0});

    list_iter min_node;
    while (min_node = heap.top(), k >= (*min_node).first) {
        if (heap.empty() || (*min_node).first == 0) break;
        heap.pop();
        if ((*min_node).second) continue;  // invalidate
        k -= (*min_node).first;
        ans -= 2;
        auto prev = min_node;
        auto next = min_node;
        do { --prev; } while ((*prev).second);
        do { ++next; } while ((*next).second);
        (*min_node).first = (*prev).first + (*next).first - (*min_node).first;
        (*prev).second = true;
        (*next).second = true;
        heap.push(min_node);
    }

    return ans;
}

int (*tasks [])(void) = { task1, task2, task3 };

int main() {
    int n;
    scanf("%d", &n);
    getchar();  // newline
    while (n--) {
        readLBR();
        // printf("read str: %s\n", buffer);

        int task;
        scanf("%d", &task);

        if (task == 3) {
            scanf("%d", &k);
        }
        printf("%d\n", tasks[task - 1]());
        getchar();  // newline
    }
}
