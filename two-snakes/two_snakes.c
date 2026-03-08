#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

#define MIN_X  2
#define MIN_Y  2
#define CONTROLS 8
#define MAX_TAIL_SIZE 30
#define START_TAIL_SIZE 3
#define MAX_FOOD_SIZE 20
#define FOOD_EXPIRE_SECONDS 10

// Направления
enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_BACKSPACE, UP_TIME=KEY_IC, DOWN_TIME=KEY_DC};

// Режимы для второй змейки
enum {MANUAL_MODE=0, AUTO_MODE=1};

// Структура для хранения еды
typedef struct food_t
{
    int x;
    int y;
    int exists;
} food_t;

// Структура для управления клавишами
typedef struct control_buttons
{
    int code;
    int direction;
} control_buttons;

// Управление для первой змейки (стрелки)
control_buttons player1_controls[4] = {
    {KEY_UP, UP},
    {KEY_DOWN, DOWN},
    {KEY_LEFT, LEFT},
    {KEY_RIGHT, RIGHT}
};

// Управление для второй змейки (W, A, S, D)
control_buttons player2_controls[CONTROLS] = {
    {'W', UP},
    {'w', UP},
    {'S', DOWN},
    {'s', DOWN},
    {'A', LEFT},
    {'a', LEFT},
    {'D', RIGHT},
    {'d', RIGHT}
};

// Структура для хвоста
typedef struct tail_t
{
    int x;
    int y;
} tail_t;

// Структура для змейки
typedef struct snake_t
{
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t *tail;
    int color_pair;
    int player_id; // 1 или 2
    int mode; // MANUAL_MODE или AUTO_MODE (только для player 2)
} snake_t;

// Структура для состояния игры
typedef struct GameState
{
    snake_t *snake1;
    snake_t *snake2;
    food_t *food;
    int score1;
    int score2;
    int game_over;
    int winner; // 0 - ничья, 1 - первая змейка, 2 - вторая змейка
} GameState;

// ==================== ИНИЦИАЛИЗАЦИЯ ====================

void initTail(struct tail_t t[], size_t size)
{
    struct tail_t init_t = {0, 0};
    for(size_t i = 0; i < size; i++)
    {
        t[i] = init_t;
    }
}

void initHead(struct snake_t *head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

void initSnake(snake_t *head, size_t size, int x, int y, int player_id, int color_pair, int mode)
{
    tail_t* tail = (tail_t*) malloc(MAX_TAIL_SIZE * sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    head->tail = tail;
    head->tsize = size + 1;
    head->player_id = player_id;
    head->color_pair = color_pair;
    head->mode = mode;
}

void initFood(food_t *food)
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    
    food->x = rand() % (max_x - MIN_X) + MIN_X;
    food->y = rand() % (max_y - MIN_Y) + MIN_Y;
    food->exists = 1;
}

void initColors()
{
    start_color();
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
}

// ==================== СТАРТОВОЕ МЕНЮ ====================

int show_start_screen()
{
    clear();
    int cy = LINES / 2;
    int cx = COLS / 2;

    attron(A_BOLD | COLOR_PAIR(COLOR_CYAN));
    mvprintw(cy - 5, cx - 10, "=== SNAKE GAME ===");
    attroff(A_BOLD | COLOR_PAIR(COLOR_CYAN));

    attron(COLOR_PAIR(COLOR_YELLOW));
    mvprintw(cy - 2, cx - 15, "Player 1: Arrow Keys");
    mvprintw(cy - 1, cx - 15, "Player 2: W/A/S/D");
    attroff(COLOR_PAIR(COLOR_YELLOW));

    attron(COLOR_PAIR(COLOR_GREEN));
    mvprintw(cy + 1, cx - 15, "Press any key to continue...");
    attroff(COLOR_PAIR(COLOR_GREEN));

    refresh();
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
    return 0;
}

int show_mode_menu()
{
    clear();
    int cy = LINES / 2;
    int cx = COLS / 2;
    int selected = 0;

    while(1)
    {
        clear();
        attron(A_BOLD | COLOR_PAIR(COLOR_CYAN));
        mvprintw(cy - 3, cx - 15, "Select Mode for Player 2:");
        attroff(A_BOLD | COLOR_PAIR(COLOR_CYAN));

        if(selected == 0)
        {
            attron(A_REVERSE | COLOR_PAIR(COLOR_GREEN));
            mvprintw(cy, cx - 10, "Manual Mode (WASD)");
            attroff(A_REVERSE | COLOR_PAIR(COLOR_GREEN));
        }
        else
        {
            mvprintw(cy, cx - 10, "Manual Mode (WASD)");
        }

        if(selected == 1)
        {
            attron(A_REVERSE | COLOR_PAIR(COLOR_RED));
            mvprintw(cy + 2, cx - 10, "Auto Mode (AI)");
            attroff(A_REVERSE | COLOR_PAIR(COLOR_RED));
        }
        else
        {
            mvprintw(cy + 2, cx - 10, "Auto Mode (AI)");
        }

        attron(COLOR_PAIR(COLOR_YELLOW));
        mvprintw(cy + 4, cx - 20, "Use UP/DOWN arrows to select, ENTER to confirm");
        attroff(COLOR_PAIR(COLOR_YELLOW));

        refresh();

        int key = getch();
        if(key == KEY_UP && selected > 0)
            selected--;
        else if(key == KEY_DOWN && selected < 1)
            selected++;
        else if(key == '\n' || key == '\r')
            return selected;
    }
}

int show_color_menu(int player_num)
{
    clear();
    int cy = LINES / 2;
    int cx = COLS / 2;
    int selected = 0;

    while(1)
    {
        clear();
        attron(A_BOLD | COLOR_PAIR(COLOR_CYAN));
        mvprintw(cy - 3, cx - 15, "Select Color for Player %d:", player_num);
        attroff(A_BOLD | COLOR_PAIR(COLOR_CYAN));

        if(selected == 0)
        {
            attron(A_REVERSE | COLOR_PAIR(COLOR_RED));
            mvprintw(cy, cx - 5, "Red");
            attroff(A_REVERSE | COLOR_PAIR(COLOR_RED));
        }
        else
        {
            attron(COLOR_PAIR(COLOR_RED));
            mvprintw(cy, cx - 5, "Red");
            attroff(COLOR_PAIR(COLOR_RED));
        }

        if(selected == 1)
        {
            attron(A_REVERSE | COLOR_PAIR(COLOR_YELLOW));
            mvprintw(cy + 2, cx - 5, "Yellow");
            attroff(A_REVERSE | COLOR_PAIR(COLOR_YELLOW));
        }
        else
        {
            attron(COLOR_PAIR(COLOR_YELLOW));
            mvprintw(cy + 2, cx - 5, "Yellow");
            attroff(COLOR_PAIR(COLOR_YELLOW));
        }

        if(selected == 2)
        {
            attron(A_REVERSE | COLOR_PAIR(COLOR_GREEN));
            mvprintw(cy + 4, cx - 5, "Green");
            attroff(A_REVERSE | COLOR_PAIR(COLOR_GREEN));
        }
        else
        {
            attron(COLOR_PAIR(COLOR_GREEN));
            mvprintw(cy + 4, cx - 5, "Green");
            attroff(COLOR_PAIR(COLOR_GREEN));
        }

        attron(COLOR_PAIR(COLOR_YELLOW));
        mvprintw(cy + 6, cx - 20, "Use UP/DOWN arrows to select, ENTER to confirm");
        attroff(COLOR_PAIR(COLOR_YELLOW));

        refresh();

        int key = getch();
        if(key == KEY_UP && selected > 0)
            selected--;
        else if(key == KEY_DOWN && selected < 2)
            selected++;
        else if(key == '\n' || key == '\r')
            return selected + 1; // Возвращаем 1, 2 или 3 (COLOR_RED, COLOR_YELLOW, COLOR_GREEN)
    }
}

// ==================== ОТРИСОВКА ====================

void drawFood(food_t *food)
{
    if(food->exists)
    {
        attron(COLOR_PAIR(COLOR_MAGENTA) | A_BOLD);
        mvprintw(food->y, food->x, "#");
        attroff(COLOR_PAIR(COLOR_MAGENTA) | A_BOLD);
    }
}

void drawSnake(snake_t *snake)
{ 
    char ch_h = '@';		
    attron(COLOR_PAIR(snake->color_pair) | A_BOLD);
    mvprintw(snake->y, snake->x, "%c", ch_h);
    
    char ch = '*';
    for(size_t i = 0; i < snake->tsize - 1; i++)
    {
        if(snake->tail[i].y || snake->tail[i].x)
            mvprintw(snake->tail[i].y, snake->tail[i].x, "%c", ch);
    }
    attroff(COLOR_PAIR(snake->color_pair) | A_BOLD);
}

// ==================== ЛОГИКА ИГРЫ ====================

int checkFoodCollision(struct snake_t *head, food_t *food)
{
    if(food->exists && head->x == food->x && head->y == food->y)
    {
        return 1;
    }
    return 0;
}
int checkCollisionWithSnake(struct snake_t *head1, struct snake_t *head2)
{
    // Проверка столкновения головы head1 с телом head2
    for(size_t i = 0; i < head2->tsize; i++)
    {
        if(head1->x == head2->tail[i].x && head1->y == head2->tail[i].y)
        {
            return 1;
        }
    }
    // Проверка столкновения головы head2 с головой head1
    if(head1->x == head2->x && head1->y == head2->y)
    {
        return 1;
    }
    return 0;
}

int checkCollisionWithSelf(struct snake_t *head)
{
    for(size_t i = 1; i < head->tsize; i++)
    {
        if(head->x == head->tail[i].x && head->y == head->tail[i].y)
        {
            return 1;
        }
    }
    return 0;
}

int checkWallCollision(struct snake_t *head)
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    
    if(head->x <= 0 || head->x >= max_x || head->y <= MIN_Y || head->y >= max_y)
    {
        return 1;
    }
    return 0;
}

void go(struct snake_t *head)
{
    char ch = '@';
    int max_x=0, max_y=0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала
    mvprintw(head->y, head->x, " "); // очищаем один символ
    switch (head->direction)
    {
        case LEFT:
            if(head->x <= 0)
                head->x = max_x;
            mvprintw(head->y, --(head->x), "%c", ch);
        break;
        case RIGHT:
            if(head->x >= max_x)
                head->x = 0;
            mvprintw(head->y, ++(head->x), "%c", ch);
        break;
        case UP:
            if(head->y <= MIN_Y)
                head->y = max_y;
            mvprintw(--(head->y), head->x, "%c", ch);
        break;
        case DOWN:
            if(head->y >= max_y)
                head->y = MIN_Y;
            mvprintw(++(head->y), head->x, "%c", ch);
        break;
        default:
        break;
    }
    refresh();
}

void goTail(struct snake_t *head)
{
    char ch = '*';
    mvprintw(head->tail[head->tsize-1].y, head->tail[head->tsize-1].x, " ");
    for(size_t i = head->tsize-1; i>0; i--)
    {
        head->tail[i] = head->tail[i-1];
        if(head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
    }
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
} 


void goTailWithGrow(struct snake_t *head)
{
    char ch = '*';
    mvprintw(head->tail[head->tsize-1].y, head->tail[head->tsize-1].x, " ");
    for(size_t i = head->tsize-1; i>0; i--)
    {
        head->tail[i] = head->tail[i-1];
        if(head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
    }
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

int checkDirection(snake_t* snake, int32_t key, int is_player1)
{
    int new_direction = -1;
    
    if(is_player1)
    {
        // Управление для первой змейки (стрелки)
        if(key == KEY_DOWN)
            new_direction = DOWN;
        else if(key == KEY_UP)
            new_direction = UP;
        else if(key == KEY_RIGHT)
            new_direction = RIGHT;
        else if(key == KEY_LEFT)
            new_direction = LEFT;
    }
    else
    {
        // Управление для второй змейки (W, A, S, D)
        for(int i = 0; i < CONTROLS; i++)
        {
            if(key == player2_controls[i].code)
            {
                new_direction = player2_controls[i].direction;
                break;
            }
        }
    }
    
    if(new_direction == -1)
        return 0;
    
    // Проверка запрещенных комбинаций
    if(snake->direction == RIGHT && new_direction == LEFT)
        return 0;
    if(snake->direction == LEFT && new_direction == RIGHT)
        return 0;
    if(snake->direction == UP && new_direction == DOWN)
        return 0;
    if(snake->direction == DOWN && new_direction == UP)
        return 0;
    
    snake->direction = new_direction;
    return 1;
}

// AI для автоматического поиска еды
void autoMove(snake_t *snake, food_t *food)
{
    if(!food->exists)
        return;
    
    int dx = food->x - snake->x;
    int dy = food->y - snake->y;
    
    // Приоритет: сначала пытаемся двигаться по горизонтали
    if(dx != 0)
    {
        // Движемся по горизонтали
        if(dx > 0 && snake->direction != LEFT)
            snake->direction = RIGHT;
        else if(dx < 0 && snake->direction != RIGHT)
            snake->direction = LEFT;
    }
    else if(dy != 0)
    {
        // Движемся по вертикали только если еда на одной линии по X
        if(dy > 0 && snake->direction != UP)
            snake->direction = DOWN;
        else if(dy < 0 && snake->direction != DOWN)
            snake->direction = UP;
    }
}


void show_game_over(GameState *gs)
{
    int cy = LINES / 2;
    int cx = COLS / 2;

    clear();
    attron(COLOR_PAIR(COLOR_RED) | A_BOLD);
    mvprintw(cy - 3, cx - 8, "  GAME OVER!  ");
    attroff(COLOR_PAIR(COLOR_RED) | A_BOLD);

    attron(COLOR_PAIR(COLOR_YELLOW) | A_BOLD);
    mvprintw(cy - 1, cx - 15, "Player 1 Score: %d", gs->score1);
    mvprintw(cy, cx - 15, "Player 2 Score: %d", gs->score2);
    
    if(gs->winner == 1)
    {
        attron(COLOR_PAIR(COLOR_GREEN) | A_BOLD);
        mvprintw(cy + 2, cx - 10, "Player 1 Wins!");
        attroff(COLOR_PAIR(COLOR_GREEN) | A_BOLD);
    }
    else if(gs->winner == 2)
    {
        attron(COLOR_PAIR(COLOR_GREEN) | A_BOLD);
        mvprintw(cy + 2, cx - 10, "Player 2 Wins!");
        attroff(COLOR_PAIR(COLOR_GREEN) | A_BOLD);
    }
    else
    {
        attron(COLOR_PAIR(COLOR_CYAN) | A_BOLD);
        mvprintw(cy + 2, cx - 10, "It's a Draw!");
        attroff(COLOR_PAIR(COLOR_CYAN) | A_BOLD);
    }
    
    attron(COLOR_PAIR(COLOR_YELLOW));
    mvprintw(cy + 4, cx - 14, "Press Backspace for Exit");
    attroff(COLOR_PAIR(COLOR_YELLOW));

    refresh();

    nodelay(stdscr, FALSE);
    int ch;
    while ((ch = getch()) != 'q' && ch != 'Q' &&
                                     ch != 'r' && ch != 'R');
    nodelay(stdscr, TRUE);

    if (ch == 'q' || ch == 'Q') {
        endwin();
        exit(0);
    }
}

// ==================== MAIN ====================

int main()
{
    srand(time(NULL));
    
    initscr();
    initColors();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    
    show_start_screen();
    
    int mode2 = show_mode_menu();
    int color1 = show_color_menu(1);
    int color2 = show_color_menu(2);
    
    GameState gs;
    gs.snake1 = (snake_t*)malloc(sizeof(snake_t));
    gs.snake2 = (snake_t*)malloc(sizeof(snake_t));
    gs.food = (food_t*)malloc(sizeof(food_t));
    
    initSnake(gs.snake1, START_TAIL_SIZE, 10, 10, 1, color1, MANUAL_MODE);
    initSnake(gs.snake2, START_TAIL_SIZE, COLS - 20, LINES - 5, 2, color2, mode2);
    initFood(gs.food);
    
    gs.score1 = 0;
    gs.score2 = 0;
    gs.game_over = 0;
    gs.winner = 0;
    
    int key_pressed = 0;
    int delay = 100;
    
    clear();
    mvprintw(0, 0, "Player1: Arrows | Player2: WASD | Backspace: EXIT | Insert/Delete: Speed");
    
    while(key_pressed != STOP_GAME && !gs.game_over)
    {
        key_pressed = getch();
        
        // Управление первой змейкой
        checkDirection(gs.snake1, key_pressed, 1);
        
        // Управление второй змейкой
        if(gs.snake2->mode == MANUAL_MODE)
        {
            checkDirection(gs.snake2, key_pressed, 0);
        }
        else
        {
            // AI режим
            autoMove(gs.snake2, gs.food);
        }
        
        // Движение змеек
        go(gs.snake1);
	goTail(gs.snake1);
        go(gs.snake2);
	goTail(gs.snake2);
        
        // Проверка столкновения первой змейки с едой
        if(checkFoodCollision(gs.snake1, gs.food))
        {
            gs.score1++;
            goTailWithGrow(gs.snake1);
            initFood(gs.food);
	    gs.snake1->tsize++;
        }
        else
        {
            goTail(gs.snake1);
        }
        
        // Проверка столкновения второй змейки с едой
        if(checkFoodCollision(gs.snake2, gs.food))
        {
            gs.score2++;
            goTailWithGrow(gs.snake2);
            initFood(gs.food);
	    gs.snake2->tsize++;
        }
        else
        {
            goTail(gs.snake2);
        }
        
        // Отрисовка
        drawSnake(gs.snake1);
        drawSnake(gs.snake2);
        drawFood(gs.food);
        
        // Вывод информации
        attron(COLOR_PAIR(color1) | A_BOLD);
        mvprintw(1, 0, "P1: %d", gs.score1);
        attroff(COLOR_PAIR(color1) | A_BOLD);
        
        attron(COLOR_PAIR(color2) | A_BOLD);
        mvprintw(1, 15, "P2: %d", gs.score2);
        attroff(COLOR_PAIR(color2) | A_BOLD);
        
        refresh();
        
        // Проверка столкновений
        
        // Управление скоростью
        if(key_pressed == DOWN_TIME && delay <= 400)
        {
            delay += 25;
        }
        else if(key_pressed == UP_TIME && delay > 50)
        {
            delay -= 25;
        }
        
        timeout(delay);
    }
    
    if(gs.game_over)
    {
        show_game_over(&gs);
    }
    
    free(gs.snake1->tail);
    free(gs.snake1);
    free(gs.snake2->tail);
    free(gs.snake2);
    free(gs.food);
    
    endwin();
    return 0;
}

