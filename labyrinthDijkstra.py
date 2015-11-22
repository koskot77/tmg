from labyrinthBFS import Node, makeGraph, backTrack 

# data structure that allows log(n) insert/delete/extractMin operations
class Heap:

    labels = [] # original graph vertex's 
    scores = [] # dijkstra's score, nodes of the tree

    def __init__(self,vertices,source):
        '''
        Build a random tree rooted in the source vertex
        '''
        self.labels.append( source )
        self.labels.extend( [ v for v in vertices if v != source ] )

        self.scores.append( 0 )
        self.scores.extend( [ 1000000 for i in range(len(vertices)-1) ] )

    def extractMin(self):
        if len(self.labels) == 0:
            return 0,-1

        root  = self.labels[0]
        score = self.scores[0]

        tail  = self.labels.pop()
        tailS = self.scores.pop()

        if len(self.labels) > 0:
            self.labels[0] = tail
            self.scores[0] = tailS
            self.bubbleDown(0)

        return root,score

    def insert(self,l,s):
        pos = len(self.labels)
        self.labels.append( l )
        self.scores.append( s )
        self.bubbleUp( pos )

    def bubbleUp(self,pos):
        parent = int( (pos-1)/2 )
        if self.scores[pos] < self.scores[parent]:
            self.labels[parent], self.labels[pos] = self.labels[pos], self.labels[parent]
            self.scores[parent], self.scores[pos] = self.scores[pos], self.scores[parent]
            if parent != 0:
                self.bubbleUp(parent);

    def delete(self,l):
        pos = self.labels.index(l) 
        end = self.bubbleDown(pos)
        return self.labels.pop(end), self.scores.pop(end)

    def bubbleDown(self,pos):
        smallestChild = 2*pos+1

        if smallestChild >= len(self.labels):
            return pos

        if smallestChild+1 < len(self.labels) and self.scores[ smallestChild ] > self.scores[ smallestChild + 1 ]:
            smallestChild = smallestChild + 1

        self.labels[smallestChild], self.labels[pos] = self.labels[pos], self.labels[smallestChild]
        self.scores[smallestChild], self.scores[pos] = self.scores[pos], self.scores[smallestChild]

        return self.bubbleDown(smallestChild)

    def prn(self):
        for i in range(0,len(self.labels)):
            print i,": label=",self.labels[i],' score=',self.scores[i]



def exploreGraph(vertices,edges):

    h = Heap( vertices, start )

    explored = {}

    print len(explored) ,' ', len(vertices)

    while len(explored) != len(vertices) :


        v,s = h.extractMin()
        explored[v] = s

        for edge in vertices[v]:
            if edge[1] not in explored:

                (vert,score) = h.delete( edge[1] )
                score = min( score, s + edges[edge] )
                h.insert( edge[1], score )

    return explored


############################################################################################

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

nodes, moves = makeGraph(simple_grid)

maxZ = len(simple_grid)
maxY = len(simple_grid[0])
maxX = len(simple_grid[0][0])

node = Node(maxX,maxY,maxZ)

start  = node.num(0,0,0)
finish = node.num(3,3,2)

# adjust the graph's representation a bit
vertices = {}
edges    = {}
for (v,neighbours) in moves.iteritems():
    vertices[v] = []
    for w in neighbours:
        vertices[v].append( (v,w) )
        edges[(v,w)] = 5

d = exploreGraph(vertices, edges)

print 'Distance: ',d[finish]

p = backTrack(moves,d,start,finish) 

step=0
for i in p:
   x,y,z = node.pos(i)
   print ' step {0}: x={1},y={2},z={3}'.format(step,x,y,z)
   step += 1

