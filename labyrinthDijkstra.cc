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

#include<iostream>
#include<set>
#include<vector>
#include<utility>
#include<string.h>

// Compile: g++ -o lab labyrinthDijkstra.cc

using namespace std;

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

//////////////////////////////////////
class Node {
private:
    unsigned int maxX, maxY, maxZ;

public:
    unsigned int x,y,z;

    operator unsigned int(void) const {
        return x + y*maxX + z*maxX*maxY;
    }

    Node& operator =(unsigned int number) {
        z =   number / maxX / maxY;
        y = ( number / maxX ) % maxY;
        x =   number % maxX;
        return *this;
    }
    
    Node(unsigned int _maxX, unsigned int _maxY, unsigned int _maxZ):
        maxX(_maxX),
        maxY(_maxY),
        maxZ(_maxZ) { }

    Node(unsigned int _x, unsigned int _maxX, unsigned int _y, unsigned int _maxY, unsigned int _z, unsigned int _maxZ):
        x(_x),
        maxX(_maxX),
        y(_y),
        maxY(_maxY),
        z(_z),
        maxZ(_maxZ) { }

};

// adjacent graph representation
set<unsigned int> *edges; //[numElem];

// auxiliary data structures for dijkstra
set<unsigned int> X;
set< pair<unsigned int,unsigned int> > crossingEdges;
unsigned int *score; //[numElem];

void moveToX(unsigned int label){
    if( X.insert(label).second == false ) return;
    for(set<unsigned int>::const_iterator edge = edges[label].begin(); edge != edges[label].end(); edge++){
        if( X.find(*edge) == X.end() ){
           crossingEdges.insert(pair<unsigned int, unsigned int>(label,*edge));
        } else {
           crossingEdges.erase(pair<unsigned int, unsigned int>(*edge,label));
        }
    }
}

//////////////////////////////////////

typedef std::vector< std::vector< std::vector<char> > > ThreeDimCharArray;

Solution savePrincess(ThreeDimCharArray grid){

    Solution sn;
    sn.amountOfTime = 0;
    sn.directions = std::vector<Direction>();
    sn.isPossible = false;

    unsigned int depth  = grid.size();
    if( depth == 0 ) return sn;  //safety
    unsigned int widthX = grid[0].size();
    if( widthX == 0 ) return sn; //safety
    unsigned int widthY = grid[0][0].size();
    if( widthY == 0 ) return sn; //safety

    const unsigned int BIG_NUMBER = 100000000;

    // starting point
    unsigned int beginX = BIG_NUMBER;
    unsigned int beginY = BIG_NUMBER;
    unsigned int beginZ = BIG_NUMBER;
    // and finishing
    unsigned int endX = BIG_NUMBER;
    unsigned int endY = BIG_NUMBER;
    unsigned int endZ = BIG_NUMBER;

    edges = new set<unsigned int>[ widthX * widthY * depth ];
    score = new     unsigned int [ widthX * widthY * depth ];
    bzero(score, sizeof(score) );

    // make a graph out of the maze
    for(size_t z=0; z < depth; z++)
        for(size_t y=0; y < widthY; y++)
            for(size_t x=0; x < widthX; x++){

                if(x+1<widthX && grid[z][y][x+1]!='o')
                    edges[ Node(x,widthX,y,widthY,z,depth) ].insert( Node(x+1,widthX,y,widthY,z,depth) );

                if(x  >0      && grid[z][y][x-1]!='o') 
                    edges[ Node(x,widthX,y,widthY,z,depth) ].insert( Node(x-1,widthX,y,widthY,z,depth) );

                if(y+1<widthY && grid[z][y+1][x]!='o') 
                    edges[ Node(x,widthX,y,widthY,z,depth) ].insert( Node(x,widthX,y+1,widthY,z,depth) );

                if(y  >0      && grid[z][y-1][x]!='o') 
                    edges[ Node(x,widthX,y,widthY,z,depth) ].insert( Node(x,widthX,y-1,widthY,z,depth) );

                if(z+1<depth  && grid[z+1][y][x]!='o') 
                    edges[ Node(x,widthX,y,widthY,z,depth) ].insert( Node(x,widthX,y,widthY,z+1,depth) );

                if(z  >0      && grid[z-1][y][x]!='o') 
                    edges[ Node(x,widthX,y,widthY,z,depth) ].insert( Node(x,widthX,y,widthY,z-1,depth) );

                if( grid[z][y][x] == '1' ){
                    beginX = x;
                    beginY = y;
                    beginZ = z;
                }

                if( grid[z][y][x] == '2' ){
                    endX = x;
                    endY = y;
                    endZ = z;
                }

            }


    // start X
    moveToX( Node( beginX,widthX, beginY,widthY, beginZ,depth) );
    // straitforward dijkstra's algorithm
    while(1){
        unsigned int dist = BIG_NUMBER;
        unsigned int next = 0;
        for(set< pair<unsigned int,unsigned int> >::const_iterator edge = crossingEdges.begin(); edge != crossingEdges.end(); edge++){
            if( dist > score[edge->first] + 5/*matrix[edge->first][edge->second]*/ ){
                dist = score[edge->first] + 5/*matrix[edge->first][edge->second]*/;
                next = edge->second;
            }
        }
        score[next] = dist;

        moveToX(next);
        if( crossingEdges.size()==0 ) break;
    }

    sn.amountOfTime = score[ Node( endX,widthX, endY,widthY, endZ,depth) ]; 

    if( sn.amountOfTime ){
        //sn.directions = std::vector<Direction>();
        sn.isPossible = true;
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

//    for(unsigned int step = 0; step < sol.directions.size(); step++)
//       cout<<sol.directions[step]<<endl;

    return 0;
}
