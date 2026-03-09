#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MIN_Y  2
enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};
enum {MAX_TAIL_SIZE=10, START_TAIL_SIZE=3, MAX_FOOD_SIZE=20, SEED_NUMBER=3, FOOD_EXPIRE_SECONDS=10};


// Здесь храним коды управления змейкой

/*
 Голова змейки содержит в себе
 x,y - координаты текущей позиции
 direction - направление движения
 tsize - размер хвоста
 *tail -  ссылка на хвост
 */
typedef struct snake_t
{
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t *tail;
    int color;
    int is_main_snake;
} snake_t;

/*
 Хвост это массив состоящий из координат x,y
 */
typedef struct tail_t
{
    int x;
    int y;
} tail_t;

struct food
{
        int x;
        int y;
        time_t put_time;
        char point;
        uint8_t enable;
} food[MAX_FOOD_SIZE];

void initFood(struct food f[], size_t size) 
{
        struct food init = {0,0,0,0,0};
        int max_y = 0, max_x = 0;
        getmaxyx(stdscr, max_y, max_x);
        for(size_t i = 0; i < size; i++)
        {
                f[i] = init;
    }
} 

void putFoodSeed(struct food *fp) 
{
        int max_x = 0, max_y = 0;
        char spoint[2] = {0};
        getmaxyx(stdscr, max_y, max_x);
        mvprintw(fp->y, fp->x, " " );
        fp->x = rand() % (max_x - 1);
        fp->y = rand() % (max_y - 2) + 1;
        fp->put_time = time(NULL);
        fp->point = '$';
        fp->enable = 1;
        spoint[0] = fp->point;
        mvprintw(fp->y, fp->x, "%s", spoint);
} 

void putFood(struct food f[], size_t number_seeds) 
{
        for(size_t i = 0; i<number_seeds; i++) 
        {
                putFoodSeed(&f[i] );
        } 
} 

void refreshFood(struct food f[], int nfood) 
{
        int max_x = 0, max_y = 0;
        //char spoint[2] = {0};
        getmaxyx(stdscr, max_y, max_x);
        for(size_t i=0; i<nfood; i++) 
        {
                if(f[i].put_time)
                {
                        if(!f[i].enable || (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS ) 
                        {
                                putFoodSeed(&f[i] );
                        } 
                } 
        } 
} 

_Bool haveEat(struct snake_t *head, struct food f[] ) 
{
        for(size_t i=0; i<MAX_FOOD_SIZE; i++) 
        {
                if(f[i].enable && head->x == f[i].x && head->y == f[i].y) 
                {
                        f[i].enable = 0;
                        return 1;
                } 
        } 
        return 0;
} 

void initTail(struct tail_t t[], size_t size)
{
    struct tail_t init_t={0,0};
    for(size_t i=0; i<size; i++)
    {
        t[i]=init_t;
    }
}
void initHead(struct snake_t *head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

void initSnake(snake_t *head, size_t size, int x, int y, int set_color, int is_main)
{
tail_t*  tail  = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    head->tail = tail; // прикрепляем к голове хвост
    head->tsize = size+1;
    head->color = set_color;
    head->is_main_snake = is_main;
}

/*
 Движение головы с учетом текущего направления движения
 */
void go(struct snake_t *head)
{
    start_color();
    init_pair(1, COLOR_RED,   COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    char ch = '@';
    int max_x=0, max_y=0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала
    mvprintw(head->y, head->x, " "); // очищаем один символ
       // init_pair(номер_пары, цвет_текста, цвет_фона)
       attron(COLOR_PAIR(head->color));       // включить цвет
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
    attroff(COLOR_PAIR(head->color));      // выключить цвет
    refresh();
}



void changeDirection(struct snake_t* snake, const int32_t key, int player)
{
if(player==1) 
  {  if (key == KEY_DOWN && snake->direction != UP)
    snake->direction = DOWN;
else if (key == KEY_UP && snake->direction != DOWN)
    snake->direction = UP;
else if (key == KEY_RIGHT && snake->direction != LEFT)
    snake->direction = RIGHT;
else if (key == KEY_LEFT && snake->direction != RIGHT)
    snake->direction = LEFT;
   } 
if(player==2){
		if(key == 's' && snake->direction != UP) 
		snake->direction = DOWN;
		else if(key == 'S' && snake->direction != UP)
                snake->direction = DOWN;
		else if(key == 'w' && snake->direction!=DOWN) 
		snake->direction = UP;
		else if(key == 'W' && snake->direction!=DOWN)
                snake->direction = UP;
		else if(key == 'd' && snake->direction!=LEFT) 
		snake->direction = RIGHT;
		else if(key == 'D' && snake->direction!=LEFT)
                snake->direction = RIGHT;
		else if(key == 'a' && snake->direction!=RIGHT) 
		snake->direction = LEFT;
		else if(key == 'A' && snake->direction!=RIGHT)
                snake->direction = LEFT;
		} 
}

/*
 Движение хвоста с учетом движения головы
 */
void goTail(struct snake_t *head)
{    start_color();
    init_pair(1, COLOR_RED,   COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    char ch = '*';
    mvprintw(head->tail[head->tsize-1].y, head->tail[head->tsize-1].x, " ");
    attron(COLOR_PAIR(head->color));
    for(size_t i = head->tsize-1; i>0; i--)
    {
        head->tail[i] = head->tail[i-1];
        if( head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
    }
    attroff(COLOR_PAIR(head->color));
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

void addTail(struct snake_t *head)
{
    if(head->tsize<=MAX_TAIL_SIZE)head->tsize++;
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

// Автоматическое движение змейки к еде (A* алгоритм упрощенный)
void autoMoveSnake(struct snake_t *snake, struct food *f)
{
    int dx = f->x - snake->x;
    int dy = f->y - snake->y;
    
    int new_direction = snake->direction;
    
    // Определяем приоритет движения
    if(abs(dx) > abs(dy))
    {
        // Движемся по оси X
        if(dx > 0 && snake->direction != LEFT)
            new_direction = RIGHT;
        else if(dx < 0 && snake->direction != RIGHT)
            new_direction = LEFT;
        else if(dy > 0 && snake->direction != UP)
            new_direction = DOWN;
        else if(dy < 0 && snake->direction != DOWN)
            new_direction = UP;
    }
    else
    {
        // Движемся по оси Y
        if(dy > 0 && snake->direction != UP)
            new_direction = DOWN;
        else if(dy < 0 && snake->direction != DOWN)
            new_direction = UP;
        else if(dx > 0 && snake->direction != LEFT)
            new_direction = RIGHT;
        else if(dx < 0 && snake->direction != RIGHT)
            new_direction = LEFT;
    }
    
    snake->direction = new_direction;
}

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

void initColors()
{
    start_color();
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
}

int main()
{
    
    initscr();
    initColors();
    keypad(stdscr, TRUE); // Включаем F1, F2, стрелки и т.д.
    raw();                // Откдючаем line buffering
    noecho();            // Отключаем echo() режим при вызове getch
    curs_set(FALSE);    //Отключаем курсор

    show_start_screen();
    int mode = show_mode_menu();
    int color1 = show_color_menu(1);
    int color2 = show_color_menu(2);

    snake_t* snake = (snake_t*)malloc(sizeof(snake_t));
    snake_t* snake2 = (snake_t*)malloc(sizeof(snake_t));
 
    initSnake(snake, START_TAIL_SIZE,10,10, color1, 1 );
    initSnake(snake2, START_TAIL_SIZE,15,15, color2, 0 );
    clear();
    mvprintw(0, 0,"Use arrows for control. Press 'F10' for EXIT");
    timeout(0);    //Отключаем таймаут после нажатия клавиши в цикле
    initFood(food, MAX_FOOD_SIZE);
    putFood(food, SEED_NUMBER);
    int score = 0;
    int score1 = 0;
    int score2 = 0;
    int key_pressed=0;
    while( key_pressed != STOP_GAME )
    {
        key_pressed = getch(); // Считываем клавишу

        go(snake);
        goTail(snake);
        go(snake2);
        goTail(snake2);
       // if(checkCollisionWithSelf(snake))break;
        timeout(100); // Задержка при отрисовке
        if(haveEat(snake, food)){
            addTail(snake);
        }
        if(haveEat(snake2, food)){
            addTail(snake2);
        }
        changeDirection(snake, key_pressed, 1);
	    if(mode == 0){changeDirection(snake2, key_pressed, 2);
        }else {autoMoveSnake(snake2, food);}
        refreshFood(food, SEED_NUMBER);// Обновляем еду
         // Вывод счета
        score1 = snake->tsize - START_TAIL_SIZE; 
        score2 = snake2->tsize - START_TAIL_SIZE;
         mvprintw(1, 0, "Player1 Score : %d | Player2 Score : %d", score1, score2);
         refresh();
         if(((checkCollisionWithSelf(snake))) || (snake->tsize==MAX_TAIL_SIZE) || (snake2->tsize==MAX_TAIL_SIZE))
        {   
            int player;
            int final_score;
            (snake->tsize==MAX_TAIL_SIZE) ? (player = 1) : (player = 2);
            (score1>score2) ? (final_score = score1) : (final_score = score2);
            mvprintw(10, 10, "GAME OVER!");
            mvprintw(11, 10, "Win Player: %d. Final Score: %d", player, final_score);
            refresh();
            sleep(8);
            break;
        }
    }
    free(snake->tail);
    free(snake); 
    free(snake2->tail);
    free(snake2);
    endwin(); // Завершаем режим curses mod
    return 0;
}
