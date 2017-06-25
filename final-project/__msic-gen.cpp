#include <cstdio>
#include <algorithm>
#include <utility>

int main() {
    int arr[2500];

    for (int i = 0; i < 2500; i++) {
        arr[i] = i;
    }

    std::random_shuffle(arr, arr + 2500);

    for (int i = 0; i < 2500; i += 2) {
        printf("%d %d\n", arr[i], arr[i + 1]);
    }
}
