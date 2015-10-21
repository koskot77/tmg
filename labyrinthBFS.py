import Queue

class Node:
    maxX = 0
    maxY = 0
    maxZ = 0

    def __init__(self,maxX,maxY,maxZ):
        self.maxX = maxX
        self.maxY = maxY
        self.maxZ = maxZ

    def num(self,x,y,z):
        '''
        Turn (x,y,z) cell of the labirynth of (maxX,maxY,maxZ) dimension into a unique id number
        '''
        return x + y*self.maxX + z*self.maxX*self.maxY

    def pos(self,num):
        '''
        Turn the id number (num) into a tuple of (x,y,z) cell coordinates
        '''
        return num % self.maxX , ( num / self.maxX ) % self.maxY , num / self.maxX / self.maxY


def makeGraph(labyrinth):
    '''
    Turn labyrinth in an array representation into a graph in adjacent representation of a tuple (set of nodes, dictionary of edges)
    '''

    maxZ = len(simple_grid)
    maxY = len(simple_grid[0])
    maxX = len(simple_grid[0][0])

    node = Node(maxX,maxY,maxZ)

    nodes = set()
    edges = {}
    for z in range(0,maxZ):
        for y in range(0,maxY):
            for x in range(0,maxX):

                if node.num(x,y,z) not in edges.keys():
                   edges[ node.num(x,y,z) ] = []

                if x+1 < maxX and labyrinth[z][y][x+1] != 'o':
                   edges[ node.num(x,y,z) ].append( node.num(x+1,y,z) )

                if x   > 0    and labyrinth[z][y][x-1] != 'o':
                   edges[ node.num(x,y,z) ].append( node.num(x-1,y,z) )

                if y+1 < maxY and labyrinth[z][y+1][x] != 'o':
                   edges[ node.num(x,y,z) ].append( node.num(x,y+1,z) )

                if y   > 0    and labyrinth[z][y-1][x] != 'o':
                   edges[ node.num(x,y,z) ].append( node.num(x,y-1,z) )

                if z+1 < maxZ and labyrinth[z+1][y][x] != 'o':
                   edges[ node.num(x,y,z) ].append( node.num(x,y,z+1) )

                if z   > 0    and labyrinth[z-1][y][x] != 'o':
                   edges[ node.num(x,y,z) ].append( node.num(x,y,z-1) )

                nodes.add( node.num(x,y,z) )

    return nodes, edges


def BFS(edges,start,finish):
    '''
    Breadth first search implementation
    '''

    d = {}               # distance
    fifo = Queue.Queue() # fifo
    explored = set()     # explored nodes

    fifo.put(start)
    d[start] = 0
    explored.add(start)

    while not fifo.empty() and finish not in explored:

        v = fifo.get()
        for w in edges[v]:

            if w not in explored:

                explored.add(w)
                fifo.put(w)
                d[w] = d[v] + 5

            if w == finish:
                break               

    if fifo.empty() and finish not in explored:
       return -1
    else:
       return d

def backTrack(edges,d,start,finish):
    '''
    Backtracking from finish to start
    '''
    p = []  # path

    v = finish
    p.append(v)

    while v != start:
        for w in edges[v]:
            if w in d and d[v] == d[w] + 5:
                v = w
                p.append(v)
                break

    return p[::-1]

simple_grid = [
               [
                  [ '1', '.', '.', '.' ],
                  [ 'o', 'o', '.', '.' ],
                  [ '.', '.', '.', 'o' ],
                  [ '.', '.', '.', 'o' ]
               ],[
                  [ 'o', 'o', 'o', 'o' ],
                  [ '.', '.', 'o', '.' ],
                  [ '.', 'o', 'o', 'o' ],
                  [ 'o', 'o', 'o', 'o' ]
               ],[
                  [ 'o', 'o', 'o', '.' ],
                  [ 'o', '.', '.', '.' ],
                  [ 'o', '.', '2', '.' ],
                  [ 'o', '.', 'o', '.' ]
               ]
              ]

nodes, edges = makeGraph(simple_grid)

maxZ = len(simple_grid)
maxY = len(simple_grid[0])
maxX = len(simple_grid[0][0])

node = Node(maxX,maxY,maxZ)

start  = node.num(0,0,0)
finish = node.num(3,3,2)

d = BFS(edges, start, finish)

print 'Distance: ',d[finish]

p = backTrack(edges,d,start,finish) 

step=0
for i in p:
   x,y,z = node.pos(i)
   print ' step {0}: x={1},y={2},z={3}'.format(step,x,y,z)
   step += 1
