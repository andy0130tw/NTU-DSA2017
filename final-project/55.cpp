#include <cstdio>
#include <cstring>
#include <utility>

// referenced code of myself:
//   https://judgegirl.csie.org/source/highlight/100142

char inp[32][32];
short board[32][32];
int cols, rows;
int undecide_cnt;

std::pair<short, short> undec[8];
int nundec;  // surr. undecided; used when backtracking

const std::pair<short, short> adj_tab[8] = {
    { -1, -1 }, { -1, 0 }, { -1, 1 },
    {  0, -1 },            {  0, 1 },
    {  1, -1 }, {  1, 0 }, {  1, 1 }
};

int solveOnce() {
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
            // solve for trivial condition
            int nhiding = inp[i][j];
            // either unknown or uncovered cell
            if (nhiding == 0) continue;

            nundec = 0;
            for (int mv = 0; mv < 8; mv++) {
                int cell = board[i + adj_tab[mv].first][j + adj_tab[mv].second];
                if (cell > 1) {
                    undec[nundec++] = { i + adj_tab[mv].first, j + adj_tab[mv].second };
                } else if (cell == 1) {
                    nhiding--;
                }
            }
            // all surrounding cell are decided; nothing to do
            if (nundec == 0) continue;
            if (nhiding == nundec) {
                // remainings are mines
                for (int k = 0; k < nundec; k++) {
                    board[undec[k].first][undec[k].second] = 1;
                    undecide_cnt--;
                }
                return 1;
            } else if (nhiding == 0) {
                // remainings are not mines
                for (int k = 0; k < nundec; k++) {
                    board[undec[k].first][undec[k].second] = 0;
                    undecide_cnt--;
                }
                return 1;
            }
        }
    }
    return 0;
}

void solveRecur() {
    while (undecide_cnt > 0) {
        while (solveOnce());

        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                if (board[i][j] <= 1) continue;
                undecide_cnt--;
                board[i][j] = 1;
                solveRecur();
                board[i][j] = 0;
                solveRecur();
                board[i][j] = 0x0202;
                undecide_cnt++;
            }
        }
    }
}

int main() {
    int k;
    scanf("%d", &k);

    while (k--) {
        scanf("%d%d", &rows, &cols);
        memset(inp, 0, sizeof(inp));
        memset(board, 0, sizeof(board));
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                scanf("%hhd", &inp[i][j]);
            }
        }

        undecide_cnt = 0;
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                if (inp[i][j] > 0) {
                    continue;
                }
                for (int mv = 0; mv < 8; mv++) {
                    if (inp[i + adj_tab[mv].first][j + adj_tab[mv].second] != 0) {
                        // need to be determined only if it is 0 surrounding by non-0 cell
                        board[i][j] = 0x0202;  // 0x7f
                        undecide_cnt++;
                        break;
                    }
                }
            }
        }

        solveRecur();

        for (int i = 1; i <= rows; i++) {
            printf("%hd", board[i][1]);
            for (int j = 2; j <= cols; j++) {
                printf(" %hd", board[i][j]);
            }
            puts("");
        }
        // printf("undecide_cnt = %d\n", undecide_cnt);
        // puts("-----------------------------------------------------");
    }
}
