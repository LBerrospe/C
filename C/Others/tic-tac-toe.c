#include <stdio.h>
#define bool int
#define true 1
#define false 0
int fill= 0;


char grid[3][3];
bool isP1Turn = true;
char m;

void printGrid() {
    int row, col;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            printf("[%c]", grid[row][col]);
        }
        printf("\n");
    }
}

void reset() {
    int row, col;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            grid[row][col] = ' ';
        }
    }
    printGrid();
}

bool validateMark(int row, int col) {
    if (grid[row][col] == ' ') {
        return true;
    }
    return false;
}

bool isWinningMark(char mark) {
    int row, col;
    for (row = 0; row < 3; row++) {
        if ((grid[row][0] == mark) && (grid[row][1] == mark)
                && (grid[row][2] == mark)) {
            return true;
        }
    }

    for (col = 0; col < 3; col++) {
        if ((grid[0][col] == mark) && (grid[1][col] == mark)
                && (grid[2][col] == mark)) {
            return true;
        }
    }

    if ((grid[0][0] == mark) && (grid[1][1] == mark)
            && (grid[2][2] == mark)) {
        return true;
    }

    if ((grid[0][2] == mark) && (grid[1][1] == mark)
            && (grid[2][0] == mark)) {
        return true;
    }

    return false;
}

void mark(int row, int col) {
    if (isP1Turn) {
        m = 'X';
    } else {
        m = 'O';
    }
    grid[row][col] = m;
    isP1Turn = !isP1Turn;
    printGrid();
    fill++;

}

int main() {
  char mov;
  bool isntValidMove = true;
  bool gameWon = false;
  reset();
  while (!gameWon) {
    isntValidMove = true;
    if (isP1Turn) {
      printf("P1 turn:\n");
    } else {
      printf("P2 turn:\n");
    }
    while (isntValidMove) {
      scanf("%c", &mov);
      switch (mov) {
        case 'q':
          f (validateMark(0, 0)) {
            mark(0, 0);
            if (isWinningMark(m)) {
              printf("%s\n", "You win!!!!");
              gameWon = true;
            }
            if (fill == 9 && isWinningMark(m)) {
              printf("%s\n", "Its a tie" );
            }

            isntValidMove = false;
          }
        break;

        case 'w':
          if (validateMark(0, 1)) {
            mark(0, 1);
            if (isWinningMark(m)) {
              printf("%s\n", "You win!!!!");
              gameWon = true;
            }
            if (fill == 9 && isWinningMark(m)) {
              printf("%s\n", "Its a tie" );
            }
            isntValidMove = false;
          }

        break;

        case 'e':
          if (validateMark(0, 2)) {
            mark(0, 2);
            if (isWinningMark(m)) {
              printf("%s\n", "You win!!!!");
              gameWon = true;
            }
            if (fill == 9 && isWinningMark(m)) {
              printf("%s\n", "Its a tie" );
            }
            isntValidMove = false;
          }
        break;

        case 'a':
          if (validateMark(1, 0)) {
            mark(1, 0);
            if (isWinningMark(m)) {
              printf("%s\n", "You win!!!!");
              gameWon = true;
            }
            if (fill == 9 && isWinningMark(m)) {
              printf("%s\n", "Its a tie" );
            }
            isntValidMove = false;
          }
        break;

        case 's':
          if (validateMark(1, 1)) {
            mark(1, 1);
            if (isWinningMark(m)) {
              printf("%s\n", "You win!!!!");
              gameWon = true;
            }
            if (fill == 9 && isWinningMark(m)) {
              printf("%s\n", "Its a tie" );
            }
            isntValidMove = false;
          }
        break;

        case 'd':
          if (validateMark(1, 2)) {
            mark(1, 2);
            if (isWinningMark(m)) {
              printf("%s\n", "You win!!!!");
              gameWon = true;
            }
            if (fill == 9 && isWinningMark(m)) {
              printf("%s\n", "Its a tie" );
            }
            isntValidMove = false;
          }
        break;

        case 'z':
          if (validateMark(2, 0)) {
            mark(2, 0);
            if (isWinningMark(m)) {
              printf("%s\n", "You win!!!!");
              gameWon = true;
            }
            if (fill == 9 && isWinningMark(m)) {
              printf("%s\n", "Its a tie" );
            }
            isntValidMove = false;
          }
        break;

        case 'x':
          if (validateMark(2, 1)) {
            mark(2, 1);
            if (isWinningMark(m)) {
              printf("%s\n", "You win!!!!");
              gameWon = true;
            }
            if (fill == 9 && isWinningMark(m)) {
              printf("%s\n", "Its a tie" );
            }
            isntValidMove = false;
          }
        break;

        case 'c':
          if (validateMark(2, 2)) {
            mark(2, 2);
            if (isWinningMark(m)) {
              printf("%s\n", "You win!!!!");
              gameWon = true;
            }
            if (fill == 9 && isWinningMark(m)){
              printf("%s\n", "Its a tie" );
            }
            isntValidMove = false;
          }
        break;
      }
    }

  }
  return 0;
}
