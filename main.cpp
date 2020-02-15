/*
 *File: main.cpp
 * Author: Bryan Soto
 * Created on Feb 7,2020 5:24pm
 * Purpose: Battleship V3.0
 */

//System Libraries
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;

//User Libraries

//Global Constants, no Global Variables are allowed
//Math/Physics/Conversions/Higher Dimensions - i.e. PI, e, etc...


const int ROWS=10;
const int COLMS=10;
const int SHPTYPE=3;

const char WATER='*'; //
const char HIT='X';
const char isSHIP='S';
const char MISS='0';

struct POINT{
	int X;//Variable for the X axis(horizontal) coordinates
	int Y;//variable for the Y axis(vertical) coordinates
};

struct SHIP{
	
	string name;//Ship name
	//Total points on the grid
	int length;
	//Coordinates of those points
	POINT onGrid[5]; //0-4 max length of biggest ship
	
	bool hitFlag[5];//Whether or not those points are a "hit"
}ship[SHPTYPE];

struct PLAYER {
	char grid[ROWS][COLMS];
}player[3]; //Ignore player 0, just using player's 1 & 2

enum DIRECTION 
{
    HORIZONTAL,VERTICAL
};
struct PLACESHIPS {
	DIRECTION direction;
	SHIP shipType;
};

bool Runchk = false;

//Function Prototypes
//Planned function prototypes for project 2
void prntBrd(int);
void genShip();
bool UsrAtck(int&,int&,int);
void RsetGrd();
bool chckBrd(int);
PLACESHIPS ShpPlce();

int main()
{
    bool Runchk=false;
    genShip();
    RsetGrd();

    //"PLACE SHIPS" phase of game
    //Loop through each player... 
    for (int plyr=1;plyr<3;++plyr)
    {
        //Loop through each ship type to place
        for (int thisShip=0; thisShip<SHPTYPE; ++thisShip)
        {                
                prntBrd(plyr);//Display the grid for the player
                //Give instructions...
                cout<<"\n";
                cout<<"Instructions  (Player "<<plyr<<")\n\n";
                cout<<"Format for placing ships should be:\n";
                cout<<"(0:Horizontal)(1:Vertical), X(top row) coords, Y(left side) coords\n";
                cout<<"Example: 0 2 2  This places a ship beginning at X:2 Y:2 going horizontally\n\n";
                cout<<"You are about to place a "<<ship[thisShip].name
                    <<" which has a length of "<<ship[thisShip].length <<"\n";
                cout<<"Choose coordinates to place ship: ";

                //Get input from user and loop until good data is returned
                PLACESHIPS aShip;
                aShip.shipType.onGrid[0].X= -1;
                while(aShip.shipType.onGrid[0].X== -1)
                    {
                            aShip=ShpPlce();
                    }

                //Combine user data with "this ship" data
                aShip.shipType.length = ship[thisShip].length;
                aShip.shipType.name = ship[thisShip].name;

                //Add the FIRST grid point to the current player's game board
                player[plyr].grid[aShip.shipType.onGrid[0].X][aShip.shipType.onGrid[0].Y]=isSHIP;

                //Determine ALL grid points based on length and direction
                for(int i=1; i<aShip.shipType.length; ++i)
                {
                        if (aShip.direction==HORIZONTAL){
                                aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X+1;
                                aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y; }
                        if (aShip.direction == VERTICAL){
                                aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y+1;
                                aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X; }

                        //Add the REMAINING grid points to our current players game board
                        player[plyr].grid[aShip.shipType.onGrid[i].X][aShip.shipType.onGrid[i].Y]=isSHIP;
                }
                //Loop back through each ship type
        }
        //Loop back through each player
    }


    //Ready to play the game
    Runchk=true;
    int thisPlayer=1;
    do {
            //Because we are ATTACKING now, the 
            //opposite players board is the display board
            int enemyPlayer;
            if (thisPlayer==1) enemyPlayer=2;
            if (thisPlayer==2) enemyPlayer=1;
            prntBrd(enemyPlayer);

            //Get attack coords from this player
            bool goodInput=false;
            int x,y;
            while (goodInput==false) {
                    goodInput=UsrAtck(x,y,thisPlayer);
            }

            //Check board; if a ship is there, set as HIT otherwise MISS
            if (player[enemyPlayer].grid[x][y]==isSHIP) player[enemyPlayer].grid[x][y]=HIT;
            if (player[enemyPlayer].grid[x][y]==WATER) player[enemyPlayer].grid[x][y]=MISS;

            //Check to see if the game is over
            //If 0 is returned, nobody has won yet
            int aWin = chckBrd(enemyPlayer);
            if (aWin != 0) {
                    Runchk=false;
                    break;
            }
            //Alternate between each player as we loop back around
            thisPlayer = (thisPlayer == 1) ? 2 : 1;
    } while (Runchk);

    cout<<"\n\nCONGRATULATIONS!!!  PLAYER "
        <<thisPlayer<<" HAS WON THE GAME! Thank you for playing!\n\n\n\n";

    return 0;
}

void prntBrd(int thisPlayer)
{
    //Draws the board for a player (thisPlayer)
    cout<<"\nWelcome to Battleship"<<endl;
    cout<<"This is a two player game"<<endl;
    cout<<"PLAYER " << thisPlayer << "'s GAME BOARD\n";
    cout<<"-------------------------------\n";

    //Loop through top row (board_width) and number columns
    cout << "   ";
    for (int w=0; w<ROWS; ++w)
    {
        if (w < 10)
                //Numbers only 1 character long, add two spaces after
                cout << w << "  ";
        else if (w >= 10)
                //Numbers 2 characters long, add only 1 space after
                cout << w << " ";
    }
    cout << "\n";

    //Loop through each grid point and display to console
    for (int h=0; h<COLMS; ++h)
    {
        for (int w=0; w<ROWS; ++w)
        {

            //If this is the FIRST (left) grid point, number the grid first
            if (w==0) cout << h << " ";

            //If h was 1 character long, add an extra space to keep numbers lined up
            if (w<10 && w==0) cout << " ";

            //Display contents of this grid (if game isn't running yet, we are placing ships
            //so display the ships
            if (Runchk==false) 
                cout << player[thisPlayer].grid[w][h] << "  ";

            //show damage if ship is hit
            if (Runchk == true && player[thisPlayer].grid[w][h] != isSHIP) 
            {
                cout << player[thisPlayer].grid[w][h] << "  ";
            }

            else if (Runchk == true && player[thisPlayer].grid[w][h] == isSHIP)
            {
                cout<<WATER<<"  ";
            }	

            //If we have reached the border
            if (w == ROWS-1) cout << "\n";
        }
    }
}

void genShip()
{
    //Sets the default data for the ships
    ship[0].name="Cruiser"; ship[0].length=1;
    ship[1].name="Frigate"; ship[1].length=2;
    ship[2].name="Battleship"; ship[2].length=4;
}

void RsetGrd()
{
    //Loop through each player
    for (int plyr=1; plyr<3; ++plyr)
    {
        //For each grid point, set contents to 'water'
        for (int w=0; w<ROWS; ++w)
        {
            for (int h=0; h<COLMS; ++h)
            {
                player[plyr].grid[w][h]=WATER;
            }
        }
        //Loop back to next player
    }
}

bool chckBrd(int enemyPLAYER)
{
    bool Victory = true;
    //Loop through enemy board
    for (int w=0; w<ROWS; ++w)
    {
        for (int h=0; h<COLMS; ++h)
        {
            //If any ships remain, game is NOT over
            if (player[enemyPLAYER].grid[w][h]==isSHIP)
                    {
                        Victory = false;
                        return Victory;
                    }
        }
    }
    //If we get here, somebody won, game over!
    return Victory;
}


bool UsrAtck(int& x, int& y, int theplayer)
{
    cout << "\nPLAYER " << theplayer << ", ENTER COORDINATES TO ATTACK: ";
    bool goodInput = false;
    cin >> x >> y;
    if (x<0 || x>=ROWS) return goodInput;
    if (y<0 || y>=COLMS) return goodInput;
    goodInput = true; 
    return goodInput;
}

PLACESHIPS ShpPlce()
{
    int d, x, y;//variables for the ship coords
    PLACESHIPS tmp;
    //Using this as a bad return
    tmp.shipType.onGrid[0].X = -1;
    //Get 3 integers from user
    cin >>d >>x >>y;
    if (d!=0 && d!=1) return tmp;
    if (x<0 || x>=ROWS) return tmp;
    if (y<0 || y>=COLMS) return tmp;
    //Good data
    tmp.direction=(DIRECTION)d;
    tmp.shipType.onGrid[0].X=x;
    tmp.shipType.onGrid[0].Y=y;
    return tmp;
}
