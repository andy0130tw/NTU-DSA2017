#include "game.h"

// Print the game state. For debugging only.
void gameStatePrint(int gameState[4][2]){
	printf("Game state:\n");
	for (int i=0; i<4; i++)
		printf("%d %d\n", gameState[i][0], gameState[i][1]);
}

// One round of dice sum game
int diceSumGame(){
	int gameState[4][2]={0};
	int tenAvailCount, oneAvailCount;
	int diceValue, nextPos, posToBeFilled, total;
	for (int i=0; i<8; i++){
	//	printf("%d/%d:\n", i, 8);
	//	gameStatePrint(gameState);
		diceValue=1+rand()%6;
		nextPos=myStrategy(gameState, diceValue);
	//	PRINT(diceValue); PRINT(nextPos);
		// Available position counts
		tenAvailCount=0;
		for (int j=0; j<4; j++)
			if (gameState[j][0]<=0) {tenAvailCount=4-j; break;}
		oneAvailCount=0;
		for (int j=0; j<4; j++)
			if (gameState[j][1]<=0) {oneAvailCount=4-j; break;}
	//	PRINT(tenAvailCount); PRINT(oneAvailCount);
		// Unreasonable output
		if ((nextPos!=0)&&(nextPos!=1)) return 0;
		if ((nextPos==0)&&(tenAvailCount==0)) return 0;
		if ((nextPos==1)&&(oneAvailCount==0)) return 0;
		// Next available pos
		if (nextPos==0) posToBeFilled=4-tenAvailCount;
		if (nextPos==1) posToBeFilled=4-oneAvailCount;
		gameState[posToBeFilled][nextPos]=diceValue;
		// Compute total
		total=(gameState[0][0]+gameState[1][0]+gameState[2][0]+gameState[3][0])*10
			+(gameState[0][1]+gameState[1][1]+gameState[2][1]+gameState[3][1]);
	//	PRINT(total);
		if (total>SCOREMAX) return 0;
	}
	return(total);
}

int main(){	
	srand(time(NULL));	// Set random seed by using the second of time
	// Play the games TESTNUM times
	int score, total=0;
	for (int i=0; i<TESTNUM; i++){
		score=diceSumGame();
		total=total+score;
	}
	// Compute the average score
	float average=(float)total/TESTNUM;
	printf("Average score %f after %d tests\n\n", average, TESTNUM);	
}