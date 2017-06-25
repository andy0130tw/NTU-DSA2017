#include <cstdio>
#include <utility>
#define MAX 1000010

struct tree {
    int value;
    int numBranch;
    int childListOffs;
};

int elem[MAX];
tree pool[MAX];
int elemPtr, poolPtr;

void makeTree(int index) {
    int nchild;
    scanf("%d", &nchild);
    tree newTree {
        .value = elem[elemPtr++],
        .numBranch = nchild,
        .childListOffs = poolPtr + 1
    };
    pool[index] = std::move(newTree);
    if (nchild == 0) return;

    int childBase = poolPtr + 1;
    poolPtr += nchild;
    for (int i = 0; i < nchild; i++) {
        makeTree(childBase + i);
    }
}

void postorder(int index) {
    static int dep;
    tree& root = pool[index];
    for (int i = 0; i < root.numBranch; i++) {
        dep++;
        postorder(root.childListOffs + i);
        dep--;
    }
    printf(dep ? "%d " : "%d\n", root.value);
}

void levelorder() {
    // use elem as queue
    int front = 0, rear = 1;
    elem[0] = 0;
    while (front != rear) {
        int pop = elem[front++];
        tree& node = pool[pop];
        printf(front == 1 ? "%d" : " %d", node.value);
        for (int i = 0; i < node.numBranch; i++) {
            elem[rear++] = node.childListOffs + i;
        }
    }
    puts("");
}

int main() {
    int n, t;
    scanf("%d%d", &n, &t);

    while (n--) {
        for (int i = 0; i < t; i++) {
            scanf("%d", &elem[i]);
        }
        elemPtr = 0, poolPtr = 0;
        makeTree(0);
        postorder(0);
        levelorder();
    }
}
