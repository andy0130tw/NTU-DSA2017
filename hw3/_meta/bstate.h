using bstate = unsigned long long;

struct table {
    int arr[4][4];
};

bstate nextMap(bstate map, int action);
table decode(bstate map);
bstate encode(table);

#include<list>
bstate process(list<int>& x);  // do the merge owo
