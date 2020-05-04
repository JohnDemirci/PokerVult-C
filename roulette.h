#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <unistd.h>
#include <time.h> // we will use this library to get a random library with suystem clock
#include <stdlib.h>


// here are the possible bet types
// threads will choose one of them when they are placing their
// bet.
typedef enum {
	ODD = 0,
	EVEN,
	RED,
	BLACK,
	ONE_TWELVE,
	TWELVE_TWENTYFOUR,
	TWENTYFOUR_THIRTYSIX,
	DOZEN1,
	DOZEN2,
	DOZEN3,
	HALF1,
	HALF2,
	SPECIFIC
}BETTYPE;

// struct for the result after the spon
typedef struct {
	int number;
	char color[10];
}RESULT;

// player struct for each player to keep track their actions
typedef struct {
	double totalMoney; // each person will have money to bet
	double moneyBet; // the amount of money they bet
	BETTYPE theType; // what type of bet they bet on
	int spe; // if they are stupid and bet for a single number
	char* name; // name of the each customer (used by the dealer)
}PLAYER;



RESULT* initTable();
PLAYER winner (int ID, int mult, PLAYER playerNo);
RESULT* evenRed(int lower, int upper, RESULT* table);
RESULT* evenBlack(int lower, int upper, RESULT* table);
BETTYPE bet(PLAYER playerNo);


