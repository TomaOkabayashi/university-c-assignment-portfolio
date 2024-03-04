#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include "game_logic.h"
#include "file_io.h"


struct LinkedList* playerHistory = NULL;
struct LinkedList* carHistory = NULL;



void disableBuffer(void) {
    struct termios mode;
    tcgetattr(0, &mode);
    mode.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(0, TCSANOW, &mode);
}

void enableBuffer(void) {
    struct termios mode;
    tcgetattr(0, &mode);
    mode.c_lflag |= (ECHO | ICANON);
    tcsetattr(0, TCSANOW, &mode);
}

int initialiseGame(struct GameMap* game, const char* filename) {
    if (readMapFile(filename, &(game->rows), &(game->columns), &(game->map)) != 0) {
        return 1;
    }
    game->history = NULL;
    game->isFirstPrint = 1;
     /* Find the initial position of the player ('P') */
    for (int i = 1; i <= game->rows; i++) {
        for (int j = 1; j <= game->columns; j++) {
            if (game->map[i][j] == 'P') {
                game->playerX = i;
                game->playerY = j;
                /* Don't replace the initial 'P' with a space here */
                break;
            }
        }
    }

    /* Create a separate array for tracking player movement */
    game->playerMovement = (char**)malloc((game->rows + 2) * sizeof(char*));
    for (int i = 0; i < game->rows + 2; i++) {
        game->playerMovement[i] = (char*)malloc((game->columns + 2) * sizeof(char));
    }

    /* Initialise the playerMovement array based on the initial map contents */
    for (int i = 0; i < game->rows + 2; i++) {
        for (int j = 0; j < game->columns + 2; j++) {
            game->playerMovement[i][j] = game->map[i][j];
        }
    }
    return 0;
}


int movePlayer(struct GameMap* game, char direction) {
    // Create a new node and add it to the history before updating the player's position
    node playerNode = createNode();
    playerNode->playerX = game->playerX;
    playerNode->playerY = game->playerY;
    playerNode->playerChar = game->map[game->playerX][game->playerY]; /* store the current map character in the history */
    playerHistory = addNode(playerHistory, playerNode);

    int newX = game->playerX;
    int newY = game->playerY;

    switch (direction) {
        case 'w':  /* Move north */
            newX--;
            break;
        case 'a':  /* Move west */
            newY--;
            break;
        case 's':  /* Move south */
            newX++;
            break;
        case 'd':  /* Move east */
            newY++;
            break;
        case 'u':  /* Undo the last move */
            if (playerHistory != NULL) {
                node lastMove = getLastNode(playerHistory);
                game->playerX = lastMove->playerX;
                game->playerY = lastMove->playerY;

                if (lastMove->playerChar == '1') {
                    // If the area had a '1', restore it when undoing
                    game->map[lastMove->playerX][lastMove->playerY] = '1';
                } else {
                    game->map[lastMove->playerX][lastMove->playerY] = 'P';
                }

                game->map[newX][newY] = lastMove->playerChar;
                playerHistory = removeLastNode(playerHistory);
                return 0;
            }
            break;
        default:
            return 0;
    }

    char currentChar = game->map[newX][newY];

    if (currentChar == 'G') {
        // Player wins, update the map
        game->playerMovement[newX][newY] = 'P';
        game->playerMovement[game->playerX][game->playerY] = ' ';
        game->playerX = newX;
        game->playerY = newY;

        printGame(game); /* print the final state of the game */
        printf("You Win!\n"); /* print the result */
        freeGame(game); /* free all dynamically allocated memory */
        playerHistory = NULL; /* avoid dangling pointers */
        carHistory = NULL; /* avoid dangling pointers */
        return 1;
    } else if (currentChar == ' ' || currentChar == '.') {
        game->playerMovement[newX][newY] = 'P';
        game->playerMovement[game->playerX][game->playerY] = ' ';
        game->playerX = newX;
        game->playerY = newY;
    } else if (currentChar == '<' || currentChar == '>' || currentChar == '^' || currentChar == 'v') {
        // If the player is about to move into a car, switch their positions
        game->playerMovement[newX][newY] = 'P';
        game->playerMovement[game->playerX][game->playerY] = currentChar;
        game->playerX = newX;
        game->playerY = newY;
    }

    return 0;
}







int moveCar(struct GameMap* game) {
    /* Find the initial position of the car ('>', '<', '^', 'v') */
    int carX = 0;
    int carY = 0;
    char carDirection = '>';

    for (int i = 1; i <= game->rows; i++) {
        for (int j = 1; j <= game->columns; j++) {
            if (game->map[i][j] == '>' || game->map[i][j] == '<' || game->map[i][j] == '^' || game->map[i][j] == 'v') {
                carX = i;
                carY = j;
                carDirection = game->map[i][j];
                break;
            }
        }
    }

    /* Variables to hold new position */
    int newX = carX;
    int newY = carY;

    /* Check the cell in front of the car */
    switch (carDirection) {
        case '>':  /* Car is facing east */
            newY++;
            break;
        case '<':  /* Car is facing west */
            newY--;
            break;
        case '^':  /* Car is facing north */
            newX--;
            break;
        case 'v':  /* Car is facing south */
            newX++;
            break;
    }

    char currentChar = game->map[newX][newY];

    if (currentChar != '.') {
        /* If there's no path in front, check east and west sides of the car */
        switch (carDirection) {
            case '>':  /* Car is facing east */
                if (game->map[carX + 1][carY] == '.') {  /* Check east of car */
                    newX = carX + 1;
                    newY = carY;
                    carDirection = 'v';
                } else if (game->map[carX - 1][carY] == '.') {  /* Check west of car */
                    newX = carX - 1;
                    newY = carY;
                    carDirection = '^';
                }
                break;
            case '<':  /* Car is facing west */
                if (game->map[carX - 1][carY] == '.') {  /* Check east of car */
                    newX = carX - 1;
                    newY = carY;
                    carDirection = '^';
                } else if (game->map[carX + 1][carY] == '.') {  /* Check west of car */
                    newX = carX + 1;
                    newY = carY;
                    carDirection = 'v';
                }
                break;
            case '^':  /* Car is facing north */
                if (game->map[carX][carY + 1] == '.') {  /* Check east of car */
                    newX = carX;
                    newY = carY + 1;
                    carDirection = '>';
                } else if (game->map[carX][carY - 1] == '.') {  /* Check west of car */
                    newX = carX;
                    newY = carY - 1;
                    carDirection = '<';
                }
                break;
            case 'v':  // Car is facing south
                if (game->map[carX][carY - 1] == '.') {  /* Check east of car */
                    newX = carX;
                    newY = carY - 1;
                    carDirection = '<';
                } else if (game->map[carX][carY + 1] == '.') {  /* Check west of car */
                    newX = carX;
                    newY = carY + 1;
                    carDirection = '>';
                }
                break;
        }
    }

    /* Create a new node and add it to the history before updating the car's position */
    node carNode = createNode();
    carNode->carX = carX;
    carNode->carY = carY;
    carNode->carChar = game->map[carX][carY];
    carHistory = addNode(carHistory, carNode);

    /* Move the car to the new position and direction */
    game->map[carX][carY] = '.';
    game->map[newX][newY] = carDirection;

    if (newX == game->playerX && newY == game->playerY) {
        printGame(game); /* print the final state of the game */
        printf("You Lose!\n"); /* print the result */
        freeGame(game); /* free all dynamically allocated memory */
        playerHistory = NULL; /* avoid dangling pointers */
        carHistory = NULL; /* avoid dangling pointers */
        return 1;  /* game ends */
    }

    return 0;  /* game continues */
}

void printInstructions() {
    printf("Press w to move up\n");
    printf("Press s to move down\n");
    printf("Press a to move left\n");
    printf("Press d to move right\n");
    printf("Press u to undo\n");
}


void printGame(struct GameMap* game) {
    system("clear");
    for (int i = 0; i < game->rows + 2; i++) {
        for (int j = 0; j < game->columns + 2; j++) {
            if (game->playerMovement[i][j] == 'P') {
                if (game->isFirstPrint || (i == game->playerX && j == game->playerY)) {
                    printf("%c", 'P');
                } else {
                    printf(" ");
                }
            } else {
                char mapChar = game->map[i][j];
                if (mapChar == '3') {
                    mapChar = ' ';
                }
                printf("%c", mapChar);
            }
        }
        printf("\n");
    }

    /* Print the instructions */
    printInstructions();

    

    /* After the first print, set isFirstPrint to 0 and replace the initial 'P' with a space */
    if (game->isFirstPrint) {
        game->isFirstPrint = 0;
        game->map[game->playerX][game->playerY] = ' ';
    }
}


node createNode() {
    node temp = (node)malloc(sizeof(struct LinkedList));
    temp->next = NULL;
    return temp;
}

node addNode(node head, node newNode) {
    if (head == NULL) {
        head = newNode;
    } else {
        node p = head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = newNode;
    }
    return head;
}

node getLastNode(node head) {
    if (head == NULL) {
        return NULL;
    } else {
        node p = head;
        while (p->next != NULL) {
            p = p->next;
        }
        return p;
    }
}

node removeLastNode(node head) {
    if (head == NULL) {
        return NULL;
    } else if (head->next == NULL) {
        free(head);
        return NULL;
    } else {
        node p = head;
        while (p->next->next != NULL) {
            p = p->next;
        }
        free(p->next);
        p->next = NULL;
        return head;
    }
}

void freeGame(struct GameMap* game) {
    /* Free the map */
    for (int i = 0; i < game->rows + 2; i++) {
        free(game->map[i]);
        free(game->playerMovement[i]);
    }
    free(game->map);
    free(game->playerMovement);

    /* Free the player history linked list */
    node p = playerHistory;
    while (p != NULL) {
        node next = p->next;
        free(p);
        p = next;
    }

    /* Free the car history linked list */
    p = carHistory;
    while (p != NULL) {
        node next = p->next;
        free(p);
        p = next;
    }
}
