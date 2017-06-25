#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("Usage: gen <dimen>");
        return 1;
    }

    srand(time(NULL));
    int dimen = atoi(argv[1]);

    puts("1");
    printf("%d\n", dimen);

    for (int i = 0; i < dimen; i++) {
        for (int j = 0; j < dimen; j++) {
            printf("%d ", rand() % 3 - 1);
        }
        puts("");
    }

}