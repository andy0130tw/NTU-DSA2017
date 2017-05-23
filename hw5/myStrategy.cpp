#include "game.h"

// My strategy to play the dice sum game
// Returns 0 for ten's position, 1 for one's position
int myStrategy(int gameState[4][2], int diceValue){
	static int initialized;		// Static variable, default to zero
	// Add your declaration of DP structure here
	if (!initialized){
		initialized=1;
		// Populate your structure here		
	}
	int tenAvailCount=0;	// No. of available ten's position
	for (int j=0; j<4; j++)
		if (gameState[j][0]<=0) {tenAvailCount=4-j; break;}
	int oneAvailCount=0;	// No. of available one's position
	for (int j=0; j<4; j++)
		if (gameState[j][1]<=0) {oneAvailCount=4-j; break;}
	if ((tenAvailCount!=0)&&(oneAvailCount==0)) return(0);
	if ((tenAvailCount==0)&&(oneAvailCount!=0)) return(1);
//	My strategy
	int strategy=1;
	int nextPos, total;
	switch(strategy){
		case 1:
			nextPos=rand()%2;
			break;
		case 2:
			nextPos=0;
			total=(gameState[0][0]+gameState[1][0]+gameState[2][0]+gameState[3][0])*10
					+(gameState[0][1]+gameState[1][1]+gameState[2][1]+gameState[3][1]);
			if (total+diceValue*10>SCOREMAX)
				nextPos=1;
			break;
		case 3:
			// Add your own strategy (which uses the DP structure) here
			break;
		default:
			printf("Error!\n");
	}
	return(nextPos);
}
