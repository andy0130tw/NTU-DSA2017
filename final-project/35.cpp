#include<cstdio>
#include<cstdlib>
#include<cmath>
#define swap(a,b)  do { __typeof(*a) tmp = *(a); *(a) = *(b); *(b) = tmp; } while (0)
#define abs(x)  ((x) > 0 ? (x) : -(x))
#define zero(x)  ((x) == 0)

using elem_t = long long int;

void printVector(int n, elem_t row[]) {
  for (int j = 0; j < n; j++) {
    printf("%4lld ", row[j]);
  }
  puts("");
}

void printMatrix(int n, elem_t* row[]) {
  for (int i = 0; i < n; i++) {
    printVector(n, row[i]);
  }
}

int gauss(int n, elem_t* row[]) {
  int curRow = 0;
  int rowSwap = 0;

  // for each column except the last one
  for (int i = 0; i < n - 1; i++) {
    // printf("i = %d ---\n", i);

    // pick pivot (minimum pivot)
    int pivotRow = -1;
    elem_t pivotValue = (1ULL << 63) - 1;
    for (int p = curRow; p < n; p++) {
      if (!zero(row[p][i]) && abs(row[p][i]) < abs(pivotValue)) {
        pivotRow = p;
        pivotValue = row[p][i];
      }
    }

    if (pivotRow < 0) continue;

    if (pivotRow != curRow) {
      // printf("swap row #%d with #%d...\n", curRow, pivotRow);
      swap(&row[curRow], &row[pivotRow]);
      rowSwap++;
    }

    int ok = 1;

    for (int j = curRow + 1; j < n; j++) {
      if (zero(row[j][i])) continue;
      elem_t mul = row[j][i] / row[curRow][i];
      // printf("row #%d -= %d\n", j, mul);
      for (int k = i; k < n; k++) {
        row[j][k] -= mul * row[curRow][k];
      }
      // printMatrix(n, row);
      // puts("");
      if (row[j][i] != 0) ok = 0;
    }

    if (!ok) {
      i--;
      continue;
    }
    curRow++;
  }

  if (rowSwap % 2 == 1) {
    for (int i = 0; i < n; i++) {
      row[0][i] *= -1;
    }
  }
  return curRow;
}

int main() {
  int t;
  scanf("%d", &t);
  while (t--) {
    int n;
    scanf("%d", &n);

    elem_t matrix[n][n];
    elem_t* row[n];

    for (int i = 0; i < n; i++) {
      row[i] = matrix[i];
      for (int j = 0; j < n; j++)
        scanf("%lld", &row[i][j]);
    }

#ifdef VERBOSE
    puts("<< before ===============");
    printMatrix(n, row);
    puts("=========================");
#endif  // VERBOSE

    gauss(n, row);

#ifdef VERBOSE
    printMatrix(n, row);
    puts(">> after  ===============");
#endif  // VERBOSE

    __int128 prod = 1;
    for (int i = 0; i < n; i++) {
      prod *= row[i][i];
      if (prod == 0) break;
    }
    auto ans = (long long int) prod;

    printf("%lld\n", ans);

  }
}
