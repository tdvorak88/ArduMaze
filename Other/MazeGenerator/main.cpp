/*
TODO:
port na Arduino (rnd zmìnit seed na pin; printf na 2D rendered maze)
*/
#define MAXX 11
#define MAXY 11
//both must be odd numbers because of the maze generation
#define WALL -1
#define EMPTY -2
#define PILLAR -3
#define PLAYER -4
#define FINISH -5

char maze[MAXX][MAXY];

struct cell
{
    int x;
    int y;
};

const int plrsmax = ((MAXX-3)/2)*((MAXY-3)/2);
cell pillar [plrsmax];

void setupMaze()
{
    for(int x=0; x<MAXX; x++)       //create empty cells
    {
        for(int y=0; y<MAXY; y++)
        {
            maze[x][y] = FREE;
        }
    }
    for(int x=0; x<MAXX; x++)       //create outer row WALLs
    {
        maze[x][0] = WALL;
        maze[x][MAXY-1] = WALL;
    }
    for(int y=0; y<MAXY; y++)       //create outer column walls
    {
        maze[0][y] = WALL;
        maze[MAXX-1][y] = WALL;
    }
    int idx;
    for(int x=2; x<MAXX-1; x+=2)    //create pillars
    {
        for(int y=2; y<MAXY-1; y+=2)
        {
            maze[x][y] = PILLAR;           //fill the maze with them
            pillar[idx].x = x;        //store them in an array for future use
            pillar[idx].y = y;
            idx++;
        }
    }
    for (int plrs = plrsmax; plrs>0; plrs--)         //connect pillars and walls
    {
        int idx = rand()%plrs;               //random pillar selection
        int dirplrs = rand()%4;                 //random direction
        if (dirplrs == 0)                   //up
        {
            for (int y = pillar[idx].y; maze[pillar[idx].x][y] != WALL; y--)
            {
                maze[pillar[idx].x][y] = WALL;
            }
        }
        if (dirplrs == 1)                   //right
        {
            for (int x = pillar[idx].x; maze[x][pillar[idx].y] != WALL; x++)
            {
                maze[x][pillar[idx].y] = WALL;
            }
        }
        if (dirplrs == 2)                   //down
        {
            for (int y = pillar[idx].y; maze[pillar[idx].x][y] != WALL; y++)
            {
                maze[pillar[idx].x][y] = WALL;
            }
        }
        if (dirplrs == 3)                   //left
        {
            for (int x = pillar[idx].x; maze[x][pillar[idx].y] != WALL; x--)
            {
                maze[x][pillar[idx].y] = WALL;
            }
        }
        pillar[idx] = pillar[plrs-1];
    }
    int startcell = 2*(rand()%(MAXX-1)/2)+1;        //random start (odd only)
    maze[startcell][MAXY-1] = FREE;
    int exitcell;
    do
    {
        exitcell = 2*(rand()%(MAXX-1)/2)+1;         //random exit (odd only)
    }
    while(exitcell == startcell);                    //prevents exit being exact opposite of start
    maze[exitcell][0] = FREE;
}

void printMaze()        //draws textures
{
    for(int y=0; y<MAXY; y++)
    {
        for(int x=0; x<MAXX; x++)
        {
            //char smbl = (maze[x][y]==1 ? 219 : (maze[x][y]==2 ? '*' : ' '));
            char smbl = (maze[x][y]==WALL ? 219 : (maze[x][y]==FREE ? ' ' : '0'+maze[x][y]));
            printf("%c%c",smbl,smbl);
        }
        printf("\n");
    }
}


int main()
{
        srand(time(NULL));
        setupMaze();
        printMaze();
}
