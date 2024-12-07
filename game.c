#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>   // srand()를 위해 추가

#define WIDTH 40
#define HEIGHT 20

typedef struct {
    int x, y;
    int health;
    char symbol;
} Player;

typedef struct {
    int x, y;
    char symbol;
    int isAlive;
} Enemy;

typedef struct {
    Player player;
    Enemy enemies[5];
    int gameOver;
    int score;
} Game;

void clearScreen() {
    system("cls");
}

void initializeGame(Game* game) {
    // 플레이어 초기화
    game->player.x = WIDTH / 2;
    game->player.y = HEIGHT - 2;
    game->player.health = 100;
    game->player.symbol = '@';
    game->gameOver = 0;
    game->score = 0;

    // 적 초기화
    for (int i = 0; i < 5; i++) {
        game->enemies[i].x = rand() % WIDTH;
        game->enemies[i].y = rand() % (HEIGHT / 2);
        game->enemies[i].symbol = 'E';
        game->enemies[i].isAlive = 1;
    }
}

void draw(Game* game) {
    clearScreen();
    // 상단 경계선
    for (int x = 0; x < WIDTH + 2; x++) printf("-");
    printf("\n");
    
    for (int y = 0; y < HEIGHT; y++) {
        printf("|"); // 왼쪽 경계선
        for (int x = 0; x < WIDTH; x++) {
            if (x == game->player.x && y == game->player.y) {
                printf("%c", game->player.symbol);
            } else {
                int isEnemy = 0;
                for (int i = 0; i < 5; i++) {
                    if (game->enemies[i].x == x && game->enemies[i].y == y && game->enemies[i].isAlive) {
                        printf("%c", game->enemies[i].symbol);
                        isEnemy = 1;
                        break;
                    }
                }
                if (!isEnemy) {
                    printf(" ");
                }
            }
        }
        printf("|\n"); // 오른쪽 경계선
    }
    
    // 하단 경계선
    for (int x = 0; x < WIDTH + 2; x++) printf("-");
    printf("\n");
    
    // 상태 표시
    printf("HP: %d | Score: %d | Use WASD to move, Q to quit\n", 
           game->player.health, game->score);
}

void update(Game* game) {
    if (_kbhit()) {
        char input = _getch();
        int newX = game->player.x;
        int newY = game->player.y;
        
        switch (input) {
            case 'w': newY--; break;
            case 's': newY++; break;
            case 'a': newX--; break;
            case 'd': newX++; break;
            case 'q': game->gameOver = 1; break;  // 종료 옵션
        }
        
        // 경계 체크
        if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
            game->player.x = newX;
            game->player.y = newY;
        }
    }

    // 적과 충돌 체크
    for (int i = 0; i < 5; i++) {
        if (game->enemies[i].isAlive && 
            game->enemies[i].x == game->player.x && 
            game->enemies[i].y == game->player.y) {
            game->player.health -= 10;
            game->enemies[i].isAlive = 0;
            game->score += 100;
        }
    }

    // 게임 오버 체크
    if (game->player.health <= 0) {
        game->gameOver = 1;
    }
}

int main() {
    Game game;
    srand(time(NULL));  // 난수 초기화
    initializeGame(&game);

    while (!game.gameOver) {
        draw(&game);
        update(&game);
        Sleep(50);  // 게임 속도 조절
    }

    // 게임 오버 메시지
    clearScreen();
    printf("Game Over!\n");
    printf("Final Score: %d\n", game.score);
    printf("Press any key to exit...");
    _getch();

    return 0;
}