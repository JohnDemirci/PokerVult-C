#include "roulette.h"



// serial code needs to parallelized
//
// compilation
// gcc -fopenmp -Wall roulette.c -o roulette
//
//
//run by
//./roulette
//
//notes
//	- in this iteration there is no dealer
// 	- in this iteration they only bet once and thenprogram ends (please let me know if you wnat it to be done differently)
//	- we have no dealer right now
//  - The program is currently serial and parallelization will be done later
//	- needs parallelization
//	- i might create a CMakeLists.txt file and run the program through cmake-build-debug folder
//		it requires cmake installed in the device
//		if you are in an UNIX OS do the following
//		if there is an existing cmake-build-debug folder
//		remove it by rm -r cmake-build-debug
//		then create your own cmake-build-debug by 
//		mkdir cmake-build-debug
//		afterwards go tot he folder by cd cmake-build-debug
//		to compile the code do the following
//		cmake ..
//		make
//
//		it should create an executable file in the cmake-build-debug folder
//		write ./roulette to run the program
//
//


// global variable for the roulette table
// since almost every function is going to get data from this variable
// we will use it as a global variable instead of passing it to everywhere
RESULT* table;

PLAYER bet (PLAYER playerNo, int ID) {

	// i get a random value between 0 - 130
	// based on that random value i assign the
	// likelyhood of the odds to make the casino simulation
	// more realistic
	// becasue it would take an idiot to bet on a single number
	int dummyType;
	dummyType = rand() % 130;
        if (dummyType < 5) {
            playerNo.theType = SPECIFIC;
            playerNo.spe = rand() % 36;
            printf("player %d betting on a specific %d\n",ID, playerNo.spe);
        } else if (dummyType > 4 && dummyType < 16) {
            playerNo.theType = ODD;
            printf("player %d betting on ODDS\n", ID);
        } else if (dummyType >= 16 && dummyType <= 26) {
            playerNo.theType = EVEN;
            printf("player %d betting on EVENS\n", ID);
        } else if (dummyType >= 27 && dummyType <= 37) {
            playerNo.theType = BLACK;
            printf("player %d betting on BLACKS\n", ID);
        } else if (dummyType >= 38 && dummyType <= 48) {
            playerNo.theType = RED;
            printf("player %d betting on REDS\n", ID);
        } else if (dummyType >= 49 && dummyType <= 59) {
            playerNo.theType = ONE_TWELVE;
            printf("player %d betting on 1-12\n", ID);
        } else if (dummyType >= 60 && dummyType <= 70) {
            playerNo.theType = TWELVE_TWENTYFOUR;
            printf("player %d betting on 13-24\n", ID);
        } else if (dummyType > 70 && dummyType <= 80) {
            playerNo.theType = TWENTYFOUR_THIRTYSIX;
            printf("player %d betting on 25-36\n", ID);
        } else if (dummyType > 80 && dummyType <= 90) {
            playerNo.theType = DOZEN1;
            printf("player %d betting on DOZEN1\n", ID);
        } else if (dummyType > 90 && dummyType <= 100) {
            playerNo.theType = DOZEN2;
            printf("player %d betting on DOZEN2\n", ID);
        } else if (dummyType > 100 && dummyType <= 110 ) {
            playerNo.theType = DOZEN3;
            printf("player %d betting on DOZEN3\n", ID);
        } else if (dummyType > 110 && dummyType <= 120) {
            playerNo.theType = HALF1;
            printf("player %d betting on HALF1\n", ID);
        } else if (dummyType > 120 && dummyType <= 130) {
            playerNo.theType = HALF2;
            printf("player %d betting on HALF2\n", ID);
        }
	return playerNo;
}



int main () {
	// starting timer to check the performance
	double t0 = omp_get_wtime();
	// number of players i might increment it by 1 for the 
	// dealer since in the lab it is said that
	// we need a dealer as well or i might 
	// create a different variable
	int numOfPlayers;

	// allocating space for the table array
	// 0 .... <= 36 = 37 entries
	table = (RESULT*)malloc(37 * sizeof(RESULT));
	
	// this function will fill the table
	initTable();

	// asking the user how many people will play the game
	// i want to put a limit to the max players
	// i might add an if statement if the number is 
	// greater than 5-7 then i ask the user to pick a lesseer number
	printf("enter the number of players\n");


    // i want the players to bet between 15-55% of their total money
    // i will get a random value between 15-55 and assign it to the percent variable
    // then i will take the percentage of the total money and make a bet
	double percent;



    // normally when we call the rand() function we usually get the exact same
    // numbers eaach tome we run the program from scratch
    // this line prevents that scenerio from happening so we truly get
    // a random value
    srand (time(NULL));

    char decision;


    // since a smart person would not bet on a specific number
    // i have decided to implement a roulette wheel algorithm where i will drag a random
    // number between 0 - 130
    // if the number is < 5 then the player will bet on a specific number
    // otherwise i'll distribute bet times lose to evenly each other
    // the point is to make the players bet on other things rather than specific numbers more often
    // the random value between 0-130 will be stored in dummyType
	int dummyType;
	// to test my runs i set it equal to 5
	// i will remove this line later on
	numOfPlayers = 5;

	// the result after the spin will be assigned to this variable
    // it is going to be accessed by each thread
	int spin;

    // i dont want more than 5 players on my table
	if (numOfPlayers > 5)
		printf("too many players");

	// creating the player profile 
    // see the header file for more information
	PLAYER* playerNo = (PLAYER*)malloc(numOfPlayers * sizeof(PLAYER));

    //setting the number of threads we want in the parallel region in the next line
	omp_set_num_threads(numOfPlayers);
    // each player starts ad 100k i might find more creative ways
    // to come up with some cool shit but for now this is the placeholder
    for (int ID = 0; ID < numOfPlayers; ID++) {
        playerNo[ID].totalMoney = 100000;
    }
    restart:

    // go to the parallel region
	#pragma omp parallel shared(spin)
	{
        // each thread will be associated with their ID
        // we will be getting that ID in the next line
		int ID = omp_get_thread_num();

        

        // entering critical section
        // if i do not do it with a critical section the output becomes
        // way too messy
		#pragma omp critical
		{
			printf("\n\nplayer %d deciding how much to bet\n", ID);	
			sleep(1);
            // gettin the random value for the percent 
			percent = (rand() % (55 - 15 + 1)) + 15;

            // betting the money based on the percentage
			playerNo[ID].moneyBet = (playerNo[ID].totalMoney * percent)/100;
			printf("player %d preparing %lf to bet\n", ID, playerNo[ID].moneyBet);
			// removing the money from the total money
            playerNo[ID].totalMoney -= playerNo[ID].moneyBet;
		}
        // calling the bet function here so that we can get a random
        // value and based on that random value the player will make a bet
        // the letters that has smaler multipliter will have a better chance to be picked
		playerNo[ID] = bet(playerNo[ID], ID);
		#pragma omp barrier
		#pragma omp for
		for (int i = 3; i > -1; i--) {
            #pragma omp critical 
            {
                printf("bets are closing soon\n");
                sleep(1);
            }
		}
        // i just want this message to be printed only once
        // so i am putting this job on a single thread only
        if (ID == 0) {
            printf("done\n");
        }
        //srand (time(NULL));
		spin = rand() % 37;
        if (ID == 3) {
            for (int i = 0; i < 3; i++) {
                printf("spinning\n");
                sleep(1);
            }
            for (int i = 0 ; i < 3; i ++) {
                printf("slowing down\n");
                sleep(1);
            }
            printf("we got\n");
            printf("%d of %s\n", table[spin].number, table[spin].color);
            sleep(2);
        }
        #pragma omp barrier
		
		#pragma omp critical
        {
            switch (playerNo[ID].theType) {
            case SPECIFIC:
                if (spin == playerNo[ID].spe) {
                    playerNo[ID] = winner(ID, 36, playerNo[ID]);
                    playerNo[ID].spe = -1;
                    break;
                } else {
                    printf("player %d lost\n", ID);
                    printf("total money left: %lf\n", playerNo[ID].totalMoney);
                    playerNo[ID].moneyBet = 0;
                    printf("\n\n");
                    break;
                }
            case ODD:
                if (spin % 2 != 0) {
                    // the integer in the function parameter is the multiplier of the bet
                    playerNo[ID] = winner(ID, 2, playerNo[ID]);
                    break;
                } else {
                    printf("player %d lost\n", ID);
                    playerNo[ID].moneyBet = 0;
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case EVEN:
                if (spin % 2 == 0) {
                    playerNo[ID] = winner(ID, 2, playerNo[ID]);
                    break;
                } else {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case RED:
                if (strcmp(table[spin].color, "RED") == 0 ) {
                    playerNo[ID] = winner(ID, 2, playerNo[ID]);
                    break;
                } else {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case BLACK:
                if (strcmp(table[spin].color, "BLACK") == 0 ) {
                    playerNo[ID] = winner(ID, 2, playerNo[ID]);
                    break;
                } else {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case ONE_TWELVE:
                if (spin > 0 && spin <= 12) {
                    playerNo[ID] = winner(ID, 3, playerNo[ID]);
                    break;
                } else {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case TWELVE_TWENTYFOUR:
                if (spin > 12 && spin <= 24) {
                    playerNo[ID] = winner(ID, 3, playerNo[ID]);
                    break;
                } else {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case TWENTYFOUR_THIRTYSIX:
                if (spin > 24 && spin <= 36) {
                    playerNo[ID] = winner(ID, 3, playerNo[ID]);
                    break;
                } else {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case DOZEN1:
                if (spin % 3 == 1) {
                    playerNo[ID] = winner(ID, 3, playerNo[ID]);
                    break;
                } else  {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case DOZEN2:
                if (spin % 3 == 2) {
                    playerNo[ID] = winner(ID, 3, playerNo[ID]);
                    break;
                } else  {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case DOZEN3:
                if (spin % 3 == 0) {
                    playerNo[ID] = winner(ID, 3, playerNo[ID]);
                    break;
                } else  {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case HALF1:
                if (spin > 0 && spin <= 18) {
                    playerNo[ID] = winner(ID, 2, playerNo[ID]);
                    break;
                } else {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            case HALF2:
                if (spin > 18 && spin <= 36) {
                    playerNo[ID] = winner(ID,2, playerNo[ID]);
                    break;
                } else {
                    playerNo[ID].moneyBet = 0;
                    printf("player %d lost\n", ID);
                    printf("total money: %lf\n", playerNo[ID].totalMoney);
                    printf("\n\n");
                    break;
                }
            }
        }
	}

	printf("would you like to fo again ? [y/n]\n");
    
    scanf("%c", &decision);
    switch (decision){
        case 'y':
            goto restart;
            break;
        case 'n':
            break;
    }
	
	// free memory
	free(playerNo);
	free(table);
	// stop the timer
	double t1 = omp_get_wtime();
	printf("\ntime spent %lf\n", t1-t0);
}



void initTable() {
	table[0].number = 0;
	strcpy(table[0].color, "GREEN");

    #pragma omp parallel sections
    {
        #pragma omp section
        evenBlack(1,11);
        // based on the patterns we foudn we fill the table
        // from 1 ... < 11 all even numbers are black and all odd numbers are red
        #pragma omp section
        evenRed(11,19);
        // from 11 ... < 19 all even numbers are red all odd numbers are black
        #pragma omp section
        evenBlack(19,29);
        // you get the logic
        #pragma omp section
        evenRed(29,37);
    }
	printf("------------------------------------------------------\n");
	for (int  i = 0; i < 37; i++) {
		printf("table[%d].number = %d\n", i, table[i].number);
		printf("table[%d].color = %s\n", i, table[i].color);
	}


		// here is the table

	// table is  done
	// 		0 G						
	// 1 R		2 B	 	3 R	       	|
	// 4 B		5 R 	6 B         |
	// 7 R		8 B	 	9 R	       	|  HALF1
	// 10 B		11 B	12 R		|
	// 13 B		14 R	15 B		|
	// 16 R		17 B	18 R		-
	// 19 R		20 B	21 R		|
	// 22 B		23 R	24 B		|
	// 25 R		26 B	27 R		|  HALF2
	// 28 B		29 B	30 R		|
	// 31 B		32 R 	33 B		|
	// 34 R		35 B	36 R		|
	//
	// DOZEN1	DOZEN2	DOZEN3
	//
}


// if the player wins the bet then we run this function
// the mult is the mmultiplier of the bet type
PLAYER winner (int ID, int mult, PLAYER playerNo) {
    printf("player %d win\nand won\n", ID);
    printf("%lf\n", playerNo.moneyBet * 2);
    playerNo.totalMoney =
            playerNo.totalMoney + (playerNo.moneyBet * mult);
    printf("total money: %lf\n", playerNo.totalMoney);
    playerNo.moneyBet = 0;
    printf("\n\n");
    return playerNo;
}

void evenBlack(int lower, int upper) {
	// this one is just a debug statement
	// will get rid of the print statement later
	printf("\nyou are in evenblack function\n");
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = lower; i < upper; i++) {
        // if i % 2 = 0 which means it is an even number
        // so we set it as black
        // if not then we set it as red
        if (i % 2 == 0) {   
            strcpy(table[i].color, "BLACK");
        } else {
            strcpy(table[i].color, "RED");
        }
        table[i].number = i;
        }
    }
}


void evenRed(int lower, int upper) {
	// this one is just a debug statement
	// will get rid of the print statement later
	printf("\nyou are in evenblack function\n");
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = lower; i < upper; i++) {
        // if i % 2 = 0 which means it is an even number
        // so we set it as black
        // if not then we set it as red
        if (i % 2 == 0) {   
            strcpy(table[i].color, "RED");
        } else {
            strcpy(table[i].color, "BLACK");
        }
        table[i].number = i;
        }
    }
}


