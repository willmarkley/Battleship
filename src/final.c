// Will Markley
// CSE 20211
// Final Project

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include "gfx3.h"
#include "gfxe.h"

#define WIDTH 750
#define HEIGHT 750

#define MISS '|'
#define WATER '~'
#define AC 'a'      // Aircraft Carrier
#define BS 'b'      // Battleship
#define SUB 's'     // Submarine
#define DESTROY 'd' // Destroyer
#define PATROL 'p'  // Patrol Boat
#define ACHIT 'v'      // Aircraft Carrier Hit
#define BSHIT 'w'      // Battleship Hit
#define SUBHIT 'x'     // Submarine Hit
#define DESTROYHIT 'y' // Destroyer Hit
#define PATROLHIT 'z'  // Patrol Boat Hit

#define ACS 5      // Aircraft Carrier Size
#define BSS 4      // Battleship Size
#define SUBS 3     // Submarine Size
#define DESTROYS 3 // Destroyer Size
#define PATROLS 2  // Patrol Boat Size

#define ACROSS 1
#define DOWN 2

void getLocation(int *r, int *c);
int checkXPoint(int x);
int checkYPoint(int y);

void initializeBoard(char board[10][10]);
void placeAllShips(char board[10][10]);
void placeShip(char board[10][10], char type, int size);
void drawPlaceScreen(char board[10][10], char type);
int shipDirection(char board[10][10], int row, int col, int ship);

void play(char board1[10][10], char board2[10][10]);
void drawScreen(char board1[10][10], char board2[10][10],int your);
void attack(char board[10][10], int row, int col);
int checkGameOver(char board[10][10]);
int isSunk(char board[10][10],char c);

void openWindow(int width, int height);
void drawYourBoard(char board[10][10], int dy);
void drawOppBoard(char board[10][10]);
void drawGrid(int startX, int startY, int w, int h);



int main(){
	srand(time(NULL));
	char board1[10][10];
	char board2[10][10];
	int input;

	initializeBoard(board1);
	initializeBoard(board2);

	openWindow(WIDTH,HEIGHT);

	gfx_clear();
	gfx_color(255,255,255);
	gfx_text( WIDTH/4, HEIGHT/2,"PLAYER 1: Hit the Spacebar when you are ready to place your ships");
	do{
		input=gfx_wait();
	}while(input!=' ');

	placeAllShips(board1);
	gfx_clear();

	gfx_clear();
	gfx_color(255,255,255);
	gfx_text( WIDTH/4, HEIGHT/2,"PLAYER 2: Hit the Spacebar when you are ready to place your ships");
	do{
		input=gfx_wait();
	}while(input!=' ');
	placeAllShips(board2);
	gfx_clear();

	play(board1,board2);

	return 0;
}



/*
	GET LOCATION FUNCTIONS:
	Retrieve and Check user input
	
*/

void getLocation(int *r, int *c){
	int input=0, x=0, y=0, i;
	
	do{
		input=gfx_wait();
	}while(input!=1);
	
	x=checkXPoint(gfx_xpos());
	y=checkYPoint(gfx_ypos());

	while(x==-1  || y==-1){
		gfx_color(255,255,255);
		gfx_text( WIDTH/3, (HEIGHT/2)+15,"Please click a point on the board");
		do{
			input=gfx_wait();
		}while(input!=1);
		
		x=checkXPoint(gfx_xpos());
		y=checkYPoint(gfx_ypos());
	}

	*r=y;
	*c=x;

	gfx_clear();

}

int checkXPoint(int x){
	// Dimensions of square
	int w=WIDTH/24;
	// Used to center the board
	int dx=(WIDTH/4)+w;

	int i;
	for(i=0; i<10; i++){
		if(x>=(dx+i*w)){		// Point is to right of left edge of square i (including edge)
			if(x<(dx+(i+1)*w)){	// Point is to left of the right edge of square i (not including edge)
				return i;	// column number (0 to 9)
			}
		}
	}
	// Check edge of last column
	if(x==(dx+10*w))
		return 9;
	// X position of point is not valid
	return -1;
				
}

int checkYPoint(int y){
	// Dimensions of square
	int h=HEIGHT/24;

	int i;
	for(i=0; i<10; i++){
		if(y>=(h+i*h)){			// Point is below top edge of square i (including edge)
			if(y<(h+(i+1)*h)){	// Point is above bottom edge of square i (not including edge)
				return i;	// column number (0 to 9)
			}
		}
	}

	// Check edge of last column
	if(y==(h+10*h))
		return 9;

	// X position of point is not valid
	return -1;
}



/*
	PLACEMENT FUNCTIONS:
		Initialie Board and Place Ships
*/

void initializeBoard(char board[10][10]){
	int i,j;

	for(i=0; i<10; i++){
		for(j=0; j<10; j++){
			board[i][j]=WATER;
		}
	}
}

void placeAllShips(char board[10][10]){
	placeShip(board,AC,ACS);
	placeShip(board,BS,BSS);
	placeShip(board,SUB,SUBS);	
	placeShip(board,DESTROY,DESTROYS);
	placeShip(board,PATROL,PATROLS);
}


void placeShip(char board[10][10], char type, int size){
	int direction;
	int row;
	int col;
	int r,c;

	drawPlaceScreen(board, type);
	getLocation(&row,&col);
	drawPlaceScreen(board, type);	// Redraw to after calling getLocation

	direction=shipDirection(board,row,col,size);

	while(direction==0){
		drawPlaceScreen(board, -1);

		gfx_color(255,255,255);
		gfx_text( WIDTH/3, HEIGHT/2,"Please click a valid point for this type of ship");

		getLocation(&row,&col);
		drawPlaceScreen(board, type);	// Redraw to after calling getLocation

		direction=shipDirection(board,row,col,size);
	}
	
	if(direction==DOWN){
		for(r=row; r<(row+size); r++){
			board[r][col]=type;
		}
	}
	else{
		for(c=col; c<(col+size); c++){
			board[row][c]=type;
		}
	}

}

void drawPlaceScreen(char board[10][10], char type){
	gfx_clear();
	drawYourBoard(board,0);

	gfx_color(255,255,255);

	switch (type){
		case AC:
			gfx_text( WIDTH/4, HEIGHT/2,"Please pick a starting square for your Aircraft Carrier(5)");
			break;
		case BS:
			gfx_text( WIDTH/4, HEIGHT/2,"Please pick a starting square for your Battleship(4)");
			break;
		case SUB:
			gfx_text( WIDTH/4, HEIGHT/2,"Please pick a starting square for your Submarine(3)");
			break;
		case DESTROY:
			gfx_text( WIDTH/4, HEIGHT/2,"Please pick a starting square for your Destroyer(3)");
			break;
		case PATROL:
			gfx_text( WIDTH/4, HEIGHT/2,"Please pick a starting square for your Patrol Boat(2)");
			break;
		default:
			break;
	}

	char *bufferPtr;
	bufferPtr = readRAWImage( "dat/BattleshipLogo2.dat", 0);
	printRAWImage(225,(HEIGHT*0.75),300,69,bufferPtr);
}

int shipDirection(char board[10][10], int row, int col, int ship){
	int across=0, down=0, r, c, count=0;

	// Checks Across
	if((col+ship)<=10){				// Doesn't go off board to right
		for(c=col; c<(col+ship); c++){		// Checks that it is being placed over water
			if(board[row][c]==WATER) count++;
		}
	}
	
	if(count==ship) across=1;

	count=0;

	// Checks Down
	if((row+ship)<=10){				// Doesn't go off bottom of board
		for(r=row; r<(row+ship); r++){		// Checks that it is being placed over water
			if(board[r][col]==WATER) count++;
		}
	}

	if(count==ship){
		down=1;
	}


	if(down && across)
		return (rand()%2+1);
	else if(down)
		return DOWN;
	else if(across)
		return ACROSS;
	else
		return 0;
		
}



/*
	GAME EXECUTION FUNCTIONS:
		Simulate the game after the game has been set up
*/

void play(char board1[10][10], char board2[10][10]){
	int gameOver=0, row, col, input;
	char winner[10], output[50];

	while(gameOver!=1 && input!='q'){
		// Player 1's turn
		drawScreen(board1,board2,1);
		getLocation(&row,&col);
		// Redraw to after calling getLocation
		drawScreen(board1,board2,1);

		attack(board2,row,col);
		do{
			input=gfx_wait();
		}while(input!=' ' && input!='q');
	
		if(input=='q') continue;
		gfx_clear();

		gameOver=checkGameOver(board2);
		if(gameOver){
			strcpy(winner,"Player 1");
			continue;
		}

		// Player 2's turn
		drawScreen(board1,board2,2);
		getLocation(&row,&col);
		// Redraw to after calling getLocation
		drawScreen(board1,board2,2);

		attack(board1,row,col);
		do{
			input=gfx_wait();
		}while(input!=' ' && input!='q');

		gfx_clear();
		gameOver=checkGameOver(board1);
		if(gameOver){
			strcpy(winner,"Player 2");
		}
	}

	gfx_clear();
	if(input!='q'){
		strcpy(output,winner);
		strcat(output," is the winner!      Press any button to exit");
		gfx_color(255,255,255);
		gfx_text( WIDTH/4, HEIGHT/2,output);
		char *bufferPtr;
		bufferPtr = readRAWImage( "dat/BattleshipLogo.dat", 0);
		printRAWImage(25,100,700,211,bufferPtr);
		gfx_wait();
	}

}

void drawScreen(char board1[10][10], char board2[10][10],int your){
	if(your==1){
		drawYourBoard(board1,HEIGHT/2);
		drawOppBoard(board2);
		gfx_color(255,255,255);
		gfx_text( WIDTH/3, 12,"BATTLESHIP");
		gfx_text( WIDTH/24, HEIGHT/4,"Opponent's Grid: Player 2");
		gfx_text( WIDTH/24, HEIGHT*0.75,"Your Grid: Player 1");
	}

	else if(your==2){
		drawYourBoard(board2,HEIGHT/2);
		drawOppBoard(board1);
		gfx_color(255,255,255);
		gfx_text( WIDTH/3, 12,"BATTLESHIP");
		gfx_text( WIDTH/24, HEIGHT/4,"Opponent's Grid: Player 1");
		gfx_text( WIDTH/24, HEIGHT*0.75,"Your Grid: Player 2");
	}

	else	return;
}

void attack(char board[10][10], int row, int col){
	gfx_color(255,255,255);
	gfx_text( WIDTH/3, 12,"BATTLESHIP");

	switch (board[row][col]){
		case WATER:
			board[row][col]=MISS;
			gfx_color(255,255,255);
			gfx_text( WIDTH/4, HEIGHT/2,"MISS: Hit Space to continue, Press q to quit");
			break;
		case MISS:
			gfx_color(255,255,255);
			gfx_text( WIDTH/4, HEIGHT/2,"You have already missed here: Hit Space to continue, Press q to quit");
			break;
		case ACHIT:
			gfx_color(255,255,255);
			gfx_text( WIDTH/4, HEIGHT/2,"You have already hit this target: Hit Space to continue, Press q to quit");
			break;
		case BSHIT:
			gfx_color(255,255,255);
			gfx_text( WIDTH/4, HEIGHT/2,"You have already hit this target: Hit Space to continue, Press q to quit");
			break;
		case SUBHIT:
			gfx_color(255,255,255);
			gfx_text( WIDTH/4, HEIGHT/2,"You have already hit this target: Hit Space to continue, Press q to quit");
			break;
		case DESTROYHIT:
			gfx_color(255,255,255);
			gfx_text( WIDTH/4, HEIGHT/2,"You have already hit this target: Hit Space to continue, Press q to quit");
			break;
		case PATROLHIT:
			gfx_color(255,255,255);
			gfx_text( WIDTH/4, HEIGHT/2,"You have already hit this target: Hit Space to continue, Press q to quit");
			break;
		case AC:
			board[row][col]=ACHIT;
			gfx_color(255,255,255);
			if(isSunk(board,AC)){
				gfx_text( WIDTH/4, HEIGHT/2,"You have sunk the Aircraft Carrier!");
				gfx_text( WIDTH/4, (HEIGHT/2)+15,"Hit Space to continue, Press q to quit");
			}
			else
				gfx_text( WIDTH/4, HEIGHT/2,"HIT: Hit Space to continue, Press q to quit");
			break;
		case BS:
			board[row][col]=BSHIT;
			gfx_color(255,255,255);
			if(isSunk(board,BS)){
				gfx_text( WIDTH/4, HEIGHT/2,"You have sunk the Battleship!");
				gfx_text( WIDTH/4, (HEIGHT/2)+15,"Hit Space to continue, Press q to quit");
			}
			else
				gfx_text( WIDTH/4, HEIGHT/2,"HIT: Hit Space to continue, Press q to quit");
			break;
		case SUB:
			board[row][col]=SUBHIT;
			gfx_color(255,255,255);
			if(isSunk(board,SUB)){
				gfx_text( WIDTH/4, HEIGHT/2,"You have sunk the Submarine!");
				gfx_text( WIDTH/4, (HEIGHT/2)+15,"Hit Space to continue, Press q to quit");
			}
			else
				gfx_text( WIDTH/4, HEIGHT/2,"HIT: Hit Space to continue, Press q to quit");
			break;
		case DESTROY:
			board[row][col]=DESTROYHIT;
			gfx_color(255,255,255);
			if(isSunk(board,DESTROY)){
				gfx_text( WIDTH/4, HEIGHT/2,"You have sunk the Destroyer!");
				gfx_text( WIDTH/4, (HEIGHT/2)+15,"Hit Space to continue, Press q to quit");
			}
			else
				gfx_text( WIDTH/4, HEIGHT/2,"HIT: Hit Space to continue, Press q to quit");
			break;
		case PATROL:
			board[row][col]=PATROLHIT;
			gfx_color(255,255,255);
			if(isSunk(board,PATROL)){
				gfx_text( WIDTH/4, HEIGHT/2,"You have sunk the Patrol Boat!");
				gfx_text( WIDTH/4, (HEIGHT/2)+15,"Hit Space to continue, Press q to quit");
			}
			else
				gfx_text( WIDTH/4, HEIGHT/2,"HIT: Hit Space to continue, Press q to quit");
			break;
		default:
			break;
	}
}

int checkGameOver(char board[10][10]){
	int i,j, count=0;

	for(i=0; i<10; i++){
		for(j=0; j<10; j++){
			if(board[i][j]< ACHIT) count++;		// The ascii codes of the pound defined ships are all less than ACHIT
		}
	}

	if(count) return 0;
	else return 1;
}

int isSunk(char board[10][10],char c){
	int i,j, count=0;

	for(i=0; i<10; i++){
		for(j=0; j<10; j++){
			if(board[i][j]==c) count++;
		}
	}

	if(count) return 0;
	else return 1;

}



/*
	GRAPHICS FUNCTIONS
		Handle most of the Graphics contained in the program (not all)
*/

void openWindow(int width, int height){
	gfx_open(width,height,"BATTLESHIP");
	gfx_clear_color(124,124,145);
	gfx_clear();

	char *bufferPtr;
	bufferPtr = readRAWImage( "dat/BattleshipLogo.dat", 0);
	printRAWImage(25,100,700,211,bufferPtr);

	gfx_color(255,255,255);
	gfx_text( WIDTH/2.75, HEIGHT/2,"Press Spacebar to Begin Game");
	
	int input;
	do{
		input=gfx_wait();
	}while(input!=' ');
}

void drawYourBoard(char board[10][10], int dy){
	// Dimensions of square
	int w=WIDTH/24;
	int h=HEIGHT/24;
	// Used to center the board
	int dx=WIDTH/4;
	int i, j, x=dx, y=dy;
	char *bufferPtr;
	int acp=0, bsp=0, sbp=0, dsp=0, pbp=0;		// Whether or not that image has been placed

	for(i=0; i<10; i++){
		y+=h;
		x=dx;
		for(j=0; j<10; j++){
			x+=w;
			switch(board[i][j]){
				case AC:
					if(acp==0){
						if(board[i][j+1]==AC || board[i][j+1]==ACHIT){		// Set image across
							bufferPtr = readRAWImage( "dat/AircraftCarrierA.dat", 0);
							printRAWImage(x,y,155,30,bufferPtr);
						}

						else if(board[i+1][j]==AC || board[i+1][j]==ACHIT){		// Set image down
							bufferPtr = readRAWImage( "dat/AircraftCarrierD.dat", 0);
							printRAWImage(x,y,31,154,bufferPtr);
						}
						acp=1;
					}
					break;
				case BS:
					if(bsp==0){
						if(board[i][j+1]==BS || board[i][j+1]==BSHIT){		// Set image across
							bufferPtr = readRAWImage( "dat/BattleshipA.dat", 0);
							printRAWImage(x,y,124,30,bufferPtr);
						}

						else if(board[i+1][j]==BS || board[i+1][j]==BSHIT){		// Set image down
							bufferPtr = readRAWImage( "dat/BattleshipD.dat", 0);
							printRAWImage(x,y,31,123,bufferPtr);
						}
						bsp=1;
					}
					break;
				case SUB:
					if(sbp==0){
						if(board[i][j+1]==SUB || board[i][j+1]==SUBHIT){		// Set image across
							bufferPtr = readRAWImage( "dat/SubmarineA.dat", 0);
							printRAWImage(x,y,93,30,bufferPtr);
						}

						else if(board[i+1][j]==SUB || board[i+1][j]==SUBHIT){	// Set image down
							bufferPtr = readRAWImage( "dat/SubmarineD.dat", 0);
							printRAWImage(x,y,31,92,bufferPtr);
						}
						sbp=1;
					}
					break;
				case DESTROY:
					if(dsp==0){
						if(board[i][j+1]==DESTROY || board[i][j+1]==DESTROYHIT){		// Set image across
							bufferPtr = readRAWImage( "dat/DestroyerA.dat", 0);
							printRAWImage(x,y,93,30,bufferPtr);
						}

						else if(board[i+1][j]==DESTROY || board[i+1][j]==DESTROYHIT){	// Set image down
							bufferPtr = readRAWImage( "dat/DestroyerD.dat", 0);
							printRAWImage(x,y,31,92,bufferPtr);
						}
						dsp=1;
					}
					break;
				case PATROL:
					if(pbp==0){
						if(board[i][j+1]==PATROL || board[i][j+1]==PATROLHIT){		// Set image across
							bufferPtr = readRAWImage( "dat/PatrolBoatA.dat", 0);
							printRAWImage(x,y,62,30,bufferPtr);
						}

						else if(board[i+1][j]==PATROL || board[i+1][j]==PATROLHIT){		// Set image down
							bufferPtr = readRAWImage( "dat/PatrolBoatD.dat", 0);
							printRAWImage(x,y,31,61,bufferPtr);
						}
						pbp=1;
					}
					break;
				case ACHIT:
					if(acp==0){
						if(board[i][j+1]==AC || board[i][j+1]==ACHIT){		// Set image across
							bufferPtr = readRAWImage( "dat/AircraftCarrierA.dat", 0);
							printRAWImage(x,y,155,30,bufferPtr);
						}

						else if(board[i+1][j]==AC || board[i+1][j]==ACHIT){		// Set image down
							bufferPtr = readRAWImage( "dat/AircraftCarrierD.dat", 0);
							printRAWImage(x,y,31,154,bufferPtr);
						}
						acp=1;
					}
					gfx_color(255,0,0);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				case BSHIT:
					if(bsp==0){
						if(board[i][j+1]==BS || board[i][j+1]==BSHIT){		// Set image across
							bufferPtr = readRAWImage( "dat/BattleshipA.dat", 0);
							printRAWImage(x,y,124,30,bufferPtr);
						}

						else if(board[i+1][j]==BS || board[i+1][j]==BSHIT){		// Set image down
							bufferPtr = readRAWImage( "dat/BattleshipD.dat", 0);
							printRAWImage(x,y,31,123,bufferPtr);
						}
						bsp=1;
					}
					gfx_color(255,0,0);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				case SUBHIT:
					if(sbp==0){
						if(board[i][j+1]==SUB || board[i][j+1]==SUBHIT){		// Set image across
							bufferPtr = readRAWImage( "dat/SubmarineA.dat", 0);
							printRAWImage(x,y,93,30,bufferPtr);
						}

						else if(board[i+1][j]==SUB || board[i+1][j]==SUBHIT){	// Set image down
							bufferPtr = readRAWImage( "dat/SubmarineD.dat", 0);
							printRAWImage(x,y,31,92,bufferPtr);
						}
						sbp=1;
					}
					gfx_color(255,0,0);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				case DESTROYHIT:
					if(dsp==0){
						if(board[i][j+1]==DESTROY || board[i][j+1]==DESTROYHIT){		// Set image across
							bufferPtr = readRAWImage( "dat/DestroyerA.dat", 0);
							printRAWImage(x,y,93,30,bufferPtr);
						}

						else if(board[i+1][j]==DESTROY || board[i+1][j]==DESTROYHIT){	// Set image down
							bufferPtr = readRAWImage( "dat/DestroyerD.dat", 0);
							printRAWImage(x,y,31,92,bufferPtr);
						}
						dsp=1;
					}
					gfx_color(255,0,0);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				case PATROLHIT:
					if(pbp==0){
						if(board[i][j+1]==PATROL || board[i][j+1]==PATROLHIT){		// Set image across
							bufferPtr = readRAWImage( "dat/PatrolBoatA.dat", 0);
							printRAWImage(x,y,62,30,bufferPtr);
						}

						else if(board[i+1][j]==PATROL || board[i+1][j]==PATROLHIT){		// Set image down
							bufferPtr = readRAWImage( "dat/PatrolBoatD.dat", 0);
							printRAWImage(x,y,31,61,bufferPtr);
						}
						pbp=1;
					}
					gfx_color(255,0,0);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				case MISS:
					gfx_color(43,101,236);
					gfx_fill_rectangle(x,y,w,h);
					gfx_color(255,255,255);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				default:
					gfx_color(43,101,236);
					gfx_fill_rectangle(x,y,w,h);
					break;
			}
		}
	}

	drawGrid(dx,dy,w,h);
}

void drawOppBoard(char board[10][10]){
	// Dimensions of square
	int w=WIDTH/24;
	int h=HEIGHT/24;
	// Used to center the board
	int dx=WIDTH/4;
	int i, j, x=dx, y=0;

	for(i=0; i<10; i++){
		y+=h;
		x=dx;
		for(j=0; j<10; j++){
			x+=w;
			switch(board[i][j]){
				case MISS:
					gfx_color(43,101,236);
					gfx_fill_rectangle(x,y,w,h);
					gfx_color(255,255,255);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				case ACHIT:
					gfx_color(43,101,236);
					gfx_fill_rectangle(x,y,w,h);
					gfx_color(255,0,0);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				case BSHIT:
					gfx_color(43,101,236);
					gfx_fill_rectangle(x,y,w,h);
					gfx_color(255,0,0);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				case SUBHIT:
					gfx_color(43,101,236);
					gfx_fill_rectangle(x,y,w,h);
					gfx_color(255,0,0);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				case DESTROYHIT:
					gfx_color(43,101,236);
					gfx_fill_rectangle(x,y,w,h);
					gfx_color(255,0,0);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				case PATROLHIT:
					gfx_color(43,101,236);
					gfx_fill_rectangle(x,y,w,h);
					gfx_color(255,0,0);
					gfx_fill_arc(x+(w/4),y+(h/4),w/2,h/2,0,360);
					break;
				default:
					gfx_color(43,101,236);
					gfx_fill_rectangle(x,y,w,h);
					break;
			}
		}
	}

	drawGrid(dx,0,w,h);
}

void drawGrid(int startX, int startY, int w, int h){
	int i, j, x=startX+h, y=startY;

	// Horizontal Lines
	for(i=0; i<11; i++){
		y+=h;
		gfx_color(0,0,0);
		gfx_line(x,y,(x+10*w),y);
	}
	
	// Vertical Lines
	x=startX;
	y=startY+h;
	for(i=0; i<11; i++){
		x+=w;
		gfx_color(0,0,0);
		gfx_line(x,y,x,(y+10*h));
	}
}



