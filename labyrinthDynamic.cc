// The Prince of Persia has been thrown onto the top level of Jaffar's underground labyrinth.
// The labyrinth consists of h levels strictly on top of each other.
// Each level is split into m by n areas.
// Some areas have columns that support ceiling, some areas are free.
// The Prince can move only to free areas.
// To move to the level below the Prince can break the floor underneath him and jump down if there is no column underneath.
// Every move takes the Prince 5 seconds.
// A Princess is waiting for the Prince at the lowest level.
// Write a program that will help the Prince to save the Princess as fast as possible by finding the shortest path between them and outputting time it took the Prince to find the Princess.
// The structure of the labyrinth is given bellow.
// The Prince’s location is marked with '1', the Princess’s location is marked with '2'. ‘.’ - marks a free spot and ‘o’ marks a column.

#include <iostream>
#include <vector>
#include <string.h>
using namespace std;

// Compile: g++ -o lab labyrinthDynamic.cc

/*Given a grid, return a solution object.
*
* A solution object is a struct that contains the following.
* = amountOfTime == an int stating how much time it took to complete the maze.
* = directions == an array of Directions that dictates the path through the maze.
* = isPossible == a boolean that says *if* there is a path through the maze.
*
* A Direction is an enum with six states {EAST, WEST, SOUTH,
* NORTH, UP, DOWN} you can access this via Problem5::Direction::NORTH etc.
*
* A ThreeDimCharArray is a std::vector<std::vector<std::vector>>>.
*/

enum Direction {EAST, WEST, SOUTH, NORTH, UP, DOWN};

struct Solution {
    int amountOfTime;
    std::vector<Direction> directions;
    bool isPossible;
};

typedef std::vector< std::vector< std::vector<char> > > ThreeDimCharArray;

#define MIN2(A,B) ( A<B ? A : B )
#define MIN3(A,B,C) ( A<B ? ( B<C ? A : ( A<C ? A : C ) ) : ( B<C ? B : C ) )

Solution savePrincess(ThreeDimCharArray grid){

    Solution sn;
    sn.amountOfTime = 0;
    sn.directions = std::vector<Direction>();
    sn.isPossible = false;

    unsigned int depth  = grid.size();
    if( depth == 0 ) return sn; //safety
    unsigned int widthX = grid[0].size();
    if( widthX == 0 ) return sn;//safety
    unsigned int widthY = grid[0][0].size();
    if( widthY == 0 ) return sn;//safety

    // dynamic programing score for current and previous iterations
    unsigned int score_cur [depth][widthY][widthX]; 
    unsigned int score_prev[depth][widthY][widthX]; 

    // initialize starting point
    const unsigned int BIG_NUMBER = 100000000;
    for(size_t z=0; z < depth; z++)
        for(size_t y=0; y < widthY; y++)
            for(size_t x=0; x < widthX; x++){
                score_cur [z][y][x] = BIG_NUMBER;
                score_prev[z][y][x] = BIG_NUMBER;
            }

    // find the starting point
    unsigned int beginX = BIG_NUMBER;
    unsigned int beginY = BIG_NUMBER;
    unsigned int beginZ = BIG_NUMBER;

    for(size_t z=0; z < depth && beginX == BIG_NUMBER; z++){
        for(size_t y=0; y < widthY && beginX == BIG_NUMBER; y++){
            for(size_t x=0; x < widthX; x++){
                if( grid[z][y][x] == '1' ){
                    beginX = x;
                    beginY = y;
                    beginZ = z;
                    break;
                }
            }
        }
    }

    // safety: no source?
    if( beginX==BIG_NUMBER || beginY==BIG_NUMBER || beginZ==BIG_NUMBER )
        return sn;

    // penalty of the move (e.g. 5 seconds)
    const unsigned int cost = 5;

    // boundary condition:
    score_prev[beginZ][beginY][beginX] = 0;
    if( beginX+1 < widthX && grid[beginZ][beginY][beginX+1]!='o' ) score_prev[beginZ][beginY][beginX+1] = cost;
    if( beginX   > 0      && grid[beginZ][beginY][beginX-1]!='o' ) score_prev[beginZ][beginY][beginX-1] = cost;
    if( beginY+1 < widthY && grid[beginZ][beginY+1][beginX]!='o' ) score_prev[beginZ][beginY+1][beginX] = cost;
    if( beginY   > 0      && grid[beginZ][beginY-1][beginX]!='o' ) score_prev[beginZ][beginY-1][beginX] = cost;
    if( beginZ+1 < depth  && grid[beginZ+1][beginY][beginX]!='o' ) score_prev[beginZ+1][beginY][beginX] = cost;
    if( beginZ   > 0      && grid[beginZ-1][beginY][beginX]!='o' ) score_prev[beginZ-1][beginY][beginX] = cost;

    // fill the score
    bool finished = false;
    while( !finished ){
        for(size_t z=0; z < depth && !finished; z++)
            for(size_t y=0; y < widthY && !finished; y++)
                for(size_t x=0; x < widthX && !finished; x++){

                   if( grid[z][y][x]=='o' ) continue;

                    unsigned int minX = MIN2(
                                            (x+1<widthX && grid[z][y][x+1]!='o' ? MIN2( score_prev[z][y][x], score_prev[z][y][x+1] + cost) : BIG_NUMBER), 
                                            (x  >0      && grid[z][y][x-1]!='o' ? MIN2( score_prev[z][y][x], score_prev[z][y][x-1] + cost) : BIG_NUMBER) 
                                            );
                    unsigned int minY = MIN2(
                                            (y+1<widthY && grid[z][y+1][x]!='o' ? MIN2( score_prev[z][y][x], score_prev[z][y+1][x] + cost) : BIG_NUMBER), 
                                            (y  >0      && grid[z][y-1][x]!='o' ? MIN2( score_prev[z][y][x], score_prev[z][y-1][x] + cost) : BIG_NUMBER) 
                                            );
                    unsigned int minZ = MIN2(
                                            (z+1<depth  && grid[z+1][y][x]!='o' ? MIN2( score_prev[z][y][x], score_prev[z+1][y][x] + cost) : BIG_NUMBER), 
                                            (z  >0      && grid[z-1][y][x]!='o' ? MIN2( score_prev[z][y][x], score_prev[z-1][y][x] + cost) : BIG_NUMBER) 
                                            );

                    score_cur[z][y][x] = MIN3(minX,minY,minZ);

                    if(  grid[z][y][x] == '2' && score_cur[z][y][x] < BIG_NUMBER ){
                        finished = true;
                        break;
                    }
                }

        if( memcmp(score_prev,score_cur,sizeof(score_cur))==0 ) break;

        memcpy(score_prev,score_cur,sizeof(score_cur));
    }

    // check if there is a way between start and finish
    if( !finished ) return sn; // no
    sn.isPossible = true;

    // find the finish
    unsigned int endX = BIG_NUMBER;
    unsigned int endY = BIG_NUMBER;
    unsigned int endZ = BIG_NUMBER;


    for(size_t z=0; z < depth && endX == BIG_NUMBER; z++){
        for(size_t y=0; y < widthY && endX == BIG_NUMBER; y++){
            for(size_t x=0; x < widthX; x++){
                if( grid[z][y][x] == '2' ){
                    endX = x;
                    endY = y;
                    endZ = z;
                    break;
                }
            }
        }
    }

    // safety: no destination?
    if( endX==BIG_NUMBER || endY==BIG_NUMBER || endZ==BIG_NUMBER )
        return sn;

    sn.amountOfTime = score_cur[endZ][endY][endX];

    // Now using the score, reconstruct one of chapest the ways
    std::vector<Direction> wayBack;
    while( true ){
        if( endX+1 < widthX && score_cur[endZ][endY][endX] == score_cur[endZ][endY][endX+1] + cost ){
            wayBack.push_back(WEST);
            endX = endX + 1;
            continue;
        }
        if( endX   > 0      && score_cur[endZ][endY][endX] == score_cur[endZ][endY][endX-1] + cost ){
            wayBack.push_back(EAST);
            endX = endX - 1;
            continue;
        }
        if( endY+1 < widthY && score_cur[endZ][endY][endX] == score_cur[endZ][endY+1][endX] + cost ){
            wayBack.push_back(NORTH);
            endY = endY + 1;
            continue;
        }
        if( endY   > 0      && score_cur[endZ][endY][endX] == score_cur[endZ][endY-1][endX] + cost ){
            wayBack.push_back(SOUTH);
            endY = endY - 1;
            continue;
        }
        if( endZ+1 < depth  && score_cur[endZ][endY][endX] == score_cur[endZ+1][endY][endX] + cost ){
            wayBack.push_back(UP);
            endZ = endZ + 1;
            continue;
        }
        if( endZ   > 0      && score_cur[endZ][endY][endX] == score_prev[endZ-1][endY][endX] + cost ){
            wayBack.push_back(DOWN);
            endZ = endZ - 1;
            continue;
        }
        // starting point
        if( grid[endZ][endY][endX] == '1' ) break;
    }


    for(int step = wayBack.size()-1; step >= 0 ; step--)
       sn.directions.push_back(wayBack[step]);


    for(size_t z=0; z < depth; z++){
        for(size_t y=0; y < widthY; y++){
            for(size_t x=0; x < widthX; x++)
               cout<<score_cur[z][y][x]<<", ";
            cout<<endl;
        }
        cout<<endl;
    }



    return sn;
}


int main(int argc, char *argv[]){

    char simple_grid[3][4][4] =
	{
		{
			{ '1', '.', '.', '.' },
			{ 'o', 'o', '.', '.' },
			{ '.', '.', '.', 'o' },
			{ '.', '.', '.', 'o' }
		},
		{
			{ 'o', 'o', 'o', 'o' },
			{ '.', '.', 'o', '.' },
			{ '.', 'o', 'o', 'o' },
			{ 'o', 'o', 'o', 'o' }
		},
		{
			{ 'o', 'o', 'o', '.' },
			{ 'o', '.', '.', '.' },
			{ 'o', '.', '2', '.' },
			{ 'o', '.', 'o', '.' }
		}
	};

    ThreeDimCharArray arg;
    arg.resize(3); // 3 levels in example
    for(size_t z=0; z<3; z++){
        arg[z].resize(4); // 3 steps in y
        for(size_t y=0; y<4; y++){
            arg[z][y].resize(4); // 3 steps in y
            for(size_t x=0; x<4; x++){
                arg[z][y][x] = simple_grid[z][y][x];
            }
        }
    }

Solution sol = savePrincess(arg);
cout<<"isPossible: "<<sol.isPossible<<" amountOfTime="<<sol.amountOfTime<<endl;

    for(unsigned int step = 0; step < sol.directions.size(); step++)
       cout<<sol.directions[step]<<endl;

    return 0;
}
