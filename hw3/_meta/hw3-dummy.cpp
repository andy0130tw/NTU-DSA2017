#include<cstdio>

using namespace std;

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


int main() {
    table x = decode(2ULL);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d\n", x.arr[i][j]);
        }
        puts("");
    }
}
