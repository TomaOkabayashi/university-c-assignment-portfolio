#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

typedef struct LinkedList *node; /* Define node as pointer of data type struct */
extern struct LinkedList* playerHistory;
extern struct LinkedList* carHistory;

struct GameMap {
    int rows;
    int columns;
    char** map;
    char** playerMovement;  /* Add the playerMovement array */
    int playerX;
    int playerY;
    int isFirstPrint;
    char carDirection;
    int carX;
    int carY;
    node history;
};

struct LinkedList{
    int playerX;
    int playerY;
    char playerChar;
    int carX;
    int carY;
    char carChar;
    struct LinkedList *next;
};



#define TRUE 1
#define FALSE 0


void disableBuffer(void);
void enableBuffer(void);
int initialiseGame(struct GameMap* game, const char* filename);
int movePlayer(struct GameMap* game, char direction);
void printInstructions();
void printGame(struct GameMap* game);

int moveCar(struct GameMap* game);

void freeGame(struct GameMap* game);

node createNode();
node addNode(node head, node newNode);
node getLastNode(node head);
node removeLastNode(node head);

#endif
