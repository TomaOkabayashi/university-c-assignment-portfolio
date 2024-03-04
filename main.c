#include <stdio.h>
#include <stdlib.h>
#include "file_io.h"
#include "game_logic.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Needs two arguements\nPlease enter (./traffic game_[n].txt) \n");
        return 1;
    }

    struct GameMap game;
    int gameResult = initialiseGame(&game, argv[1]);

    if (gameResult == 1) {
        // Handle the error condition, e.g., print an error message.
        printf("Failed to initialize the game.\n");
        return 1;
    }


    while (gameResult == 0) {
        printGame(&game);
        disableBuffer();
        char input = getchar();
        enableBuffer();

        if (input == 'u') {
            /* Retrieve the last nodes from the player and car history */
            node lastPlayerNode = getLastNode(playerHistory);
            node lastCarNode = getLastNode(carHistory);

            if (lastPlayerNode != NULL && lastCarNode != NULL) {
                /* If the player's previous position was '.', replace the current position with '.' */
                if (lastPlayerNode->playerChar == '.') {
                    game.playerMovement[game.playerX][game.playerY] = '.';
                }

                /* Restore the previous positions and characters of the player and car */
                game.playerX = lastPlayerNode->playerX;
                game.playerY = lastPlayerNode->playerY;
                game.playerMovement[game.playerX][game.playerY] = 'P';

                /* Find the current position of the car and replace it with '.' */
                for (int i = 1; i <= game.rows; i++) {
                    for (int j = 1; j <= game.columns; j++) {
                        if (game.map[i][j] == '>' || game.map[i][j] == '<' || game.map[i][j] == '^' || game.map[i][j] == 'v') {
                            game.map[i][j] = '.'; /* replace car's current position with '.' */
                            break;
                        }
                    }
                }

                /* Restore the car's previous position */
                game.map[lastCarNode->carX][lastCarNode->carY] = lastCarNode->carChar;

                /* Remove the last nodes from the player and car history */
                playerHistory = removeLastNode(playerHistory);
                carHistory = removeLastNode(carHistory);
            }
        } else if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
            gameResult = movePlayer(&game, input);
            if (gameResult == 0) {
                gameResult = moveCar(&game);
            }
        }
    }
    return 0;
}
