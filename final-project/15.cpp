#include <cstdio>
#include <queue>

using namespace std;

int size;
int median;
priority_queue<int> less_max;
priority_queue<int, vector<int>, greater<int> > greater_min;

void balance() {
    int lsz = less_max.size();
    int gsz = greater_min.size();
    if (lsz - gsz >= 1) {
        greater_min.push(median);
        median = less_max.top();
        less_max.pop();
    } else if (lsz - gsz < -1) {
        less_max.push(median);
        median = greater_min.top();
        greater_min.pop();
    }
}

int insert(int k) {
    if (size == 0) {
        median = k;
    } else {
        k < median ? (less_max.push(k)) : (greater_min.push(k));
    }
    size++;
    balance();
}

int get_median() {
    if (size & 1) return median;
    return (median + greater_min.top()) / 2;
}

void remove_median() {
    median = greater_min.top();
    greater_min.pop();
    size--;
    balance();
}


int main() {
    int n;
    scanf("%d", &n);
    while (n--) {
        int op;
        scanf("%d", &op);
        if (op == 1) {
            scanf("%d", &op);
            insert(op);
        } else if (op == 2) {
            printf(n ? "Current median ID:%d\n" : "The winner ID: %d!!!\n",
                   n ? get_median()             : median);
        } else if (op == 3) {
            printf("Player ID:%d is out!\n", median);
            remove_median();
        }
    }
}

/*

template <class T, class R, class C>
void printAll(priority_queue<T,R,C>& pq) {
    R x {};
    while (pq.size()) {
        printf("%d ", pq.top());
        x.push_back(pq.top());
        pq.pop();
    }
    printf("\n");
    for (auto it = x.begin(); it != x.end(); it++) {
        pq.push(*it);
    }
}

printf("<< min: ");
printAll(less_max);
printf(">> max: ");
printAll(greater_min);

 */
