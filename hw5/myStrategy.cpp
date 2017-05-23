#include "game.h"
#define POSITION_TEN 0
#define POSITION_ONE 1

static int dp[5][5][152];
static int score;
static const int penalty = -100;

static int expScore(int tens, int ones, int score) {
	if (score > SCOREMAX)
		return penalty;

	if (dp[tens][ones][score])
		return dp[tens][ones][score];

	if (tens == 0 && ones == 0) {
		return dp[0][0][score] = score <= SCOREMAX ? score : penalty;
	}

	int expval = 0;
	for (int i = 1; i <= 6; i++) {
		expval += max(tens > 0 ? expScore(tens - 1, ones, score + i * 10) : 0,
		              ones > 0 ? expScore(tens, ones - 1, score + i)      : 0);
	}

	return dp[tens][ones][score] = expval;
}

static int getAvailCount(int state[][2], int offs) {
	for (int i = 0; i < 4; i++)
		if (state[i][offs] > 0) {
			score += state[i][offs] * (offs == POSITION_TEN ? 10 : 1);
		} else return 4 - i;
	return 0;
}

__attribute__((constructor))
static void initializeTable() {
	expScore(4, 4, 0);
}

// My strategy to play the dice sum game
// Returns 0 for ten's position, 1 for one's position
int myStrategy(int gameState[4][2], int diceValue) {
	score = 0;
	int tenAvailCount = getAvailCount(gameState, POSITION_TEN);
	int oneAvailCount = getAvailCount(gameState, POSITION_ONE);
	// score is calculated now (bad coding style, I know)

	if (oneAvailCount == 0) return POSITION_TEN;
	if (tenAvailCount == 0) return POSITION_ONE;

	return (expScore(tenAvailCount - 1, oneAvailCount, score + diceValue * 10) >
	        expScore(tenAvailCount, oneAvailCount - 1, score + diceValue)) ?
	       POSITION_TEN : POSITION_ONE;
}
