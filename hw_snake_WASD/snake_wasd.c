#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MIN_X  2
#define MIN_Y  2
#define CONTROLS 8  // Количество элементов массива управления

enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_BACKSPACE, UP_TIME=KEY_IC, DOWN_TIME=KEY_DC};
enum {MAX_TAIL_SIZE=100, START_TAIL_SIZE=10, MAX_FOOD_SIZE=20, FOOD_EXPIRE_SECONDS=10};

// Структура для хранения еды
typedef struct food_t
{
    int x;
    int y;
    int exists;
} food_t;

// Здесь храним коды управления змейкой
typedef struct control_buttons
{
    int code;
    int direction;
} control_buttons;

// Массив управления с поддержкой W, A, S, D и стрелок
control_buttons default_controls[CONTROLS] = {
    {'W', UP},
    {'w', UP},
    {'S', DOWN},
    {'s', DOWN},
    {'A', LEFT},
    {'a', LEFT},
    {'D', RIGHT},
    {'d', RIGHT}
};

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
} snake_t;

/*
 Хвост это массив состоящий из координат x,y
 */
typedef struct tail_t
{
    int x;
    int y;
} tail_t;

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

void initSnake(snake_t *head, size_t size, int x, int y)
{
    tail_t* tail = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    head->tail = tail; // прикрепляем к голове хвост
    head->tsize = size+1;
}

// Инициализация еды
void initFood(food_t *food)
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    
    food->x = rand() % (max_x - MIN_X) + MIN_X;
    food->y = rand() % (max_y - MIN_Y) + MIN_Y;
    food->exists = 1;
}

// Отрисовка еды
void drawFood(food_t *food)
{
    if(food->exists)
    {
        mvprintw(food->y, food->x, "#");
    }
}

// Проверка столкновения головы с едой
int checkFoodCollision(struct snake_t *head, food_t *food)
{
    if(food->exists && head->x == food->x && head->y == food->y)
    {
        return 1;
    }
    return 0;
}

int checkCollision(struct snake_t *head)
{
    // Начинаем проверку со второго элемента хвоста (первый - это позиция головы)
    for(size_t i = 1; i < head->tsize; i++)
    {
        if(head->x == head->tail[i].x && head->y == head->tail[i].y)
        {
            return 1; // Столкновение обнаружено
        }
    }
    return 0; // Столкновения нет
}

/*
 Движение головы с учетом текущего направления движения
 */
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

// Функция для изменения направления с поддержкой W, A, S, D и стрелок
int checkDirection(snake_t* snake, int32_t key)
{
    int new_direction = -1;
    
    // Определяем новое направление из нажатой клавиши
    // Проверяем управление W, A, S, D
    for(int i = 0; i < CONTROLS; i++)
    {
        if(key == default_controls[i].code)
        {
            new_direction = default_controls[i].direction;
            break;
        }
    }
    
    // Проверяем стрелки
    if(key == KEY_DOWN)
        new_direction = DOWN;
    else if(key == KEY_UP)
        new_direction = UP;
    else if(key == KEY_RIGHT)
        new_direction = RIGHT;
    else if(key == KEY_LEFT)
        new_direction = LEFT;
    
    // Если клавиша не является направлением, возвращаем 0
    if(new_direction == -1)
        return 0;
    
    // Проверяем запрещенные комбинации
    // Если движемся RIGHT, запрещаем LEFT
    if(snake->direction == RIGHT && new_direction == LEFT)
        return 0;
    
    // Если движемся LEFT, запрещаем RIGHT
    if(snake->direction == LEFT && new_direction == RIGHT)
        return 0;
    
    // Если движемся UP, запрещаем DOWN
    if(snake->direction == UP && new_direction == DOWN)
        return 0;
    
    // Если движемся DOWN, запрещаем UP
    if(snake->direction == DOWN && new_direction == UP)
        return 0;
    
    // Направление корректно, обновляем направление змейки
    snake->direction = new_direction;
    return 1;
}

/*
 Движение хвоста с учетом движения головы
 */
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

/*
 Движение хвоста БЕЗ увеличения (для поедания еды)
 */
void goTailWithoutGrow(struct snake_t *head)
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

int main()
{
    srand(time(NULL)); // Инициализация генератора случайных чисел
    
    snake_t* snake = (snake_t*)malloc(sizeof(snake_t));
    food_t* food = (food_t*)malloc(sizeof(food_t));
    
    initSnake(snake, START_TAIL_SIZE, 10, 10);
    initFood(food);
    
    initscr();
    keypad(stdscr, TRUE); // Включаем F1, F2, стрелки и т.д.
    raw();                // Отключаем line buffering
    noecho();             // Отключаем echo() режим при вызове getch
    curs_set(FALSE);      // Отключаем курсор
    
    mvprintw(0, 0, " Press 'Delete' for Fast. Press 'Insert' for Slow Use WASD. Press 'Backspace' for EXIT");
    timeout(0);           // Отключаем таймаут после нажатия клавиши в цикле
    
    int key_pressed = 0;
    int score = 0;
    int delay = 100;
    while(key_pressed != STOP_GAME)
    {   
        key_pressed = getch(); // Считываем клавишу
        go(snake);
        
        // Проверка столкновения с едой
        if(checkFoodCollision(snake, food))
        {
            // Увеличиваем размер хвоста
            if(snake->tsize < MAX_TAIL_SIZE)
            {
                snake->tsize++;
                score++;
            }
            // Генерируем новую еду
            initFood(food);
            // Отрисовываем хвост БЕЗ удаления последнего элемента
            goTail(snake);
        }
        else
        {
            // Обычное движение хвоста
            goTailWithoutGrow(snake);
        }
        
        // Отрисовка еды
        drawFood(food);
        
        // Вывод счета
        mvprintw(1, 0, "Score: %d | Tail size: %zu", score, snake->tsize);
        refresh();
        
        // Проверка столкновения с самим собой
        if(checkCollision(snake))
        {
            mvprintw(10, 10, "GAME OVER! You hit yourself!");
            mvprintw(11, 10, "Final Score: %d", score);
            refresh();
            sleep(2);
            break;
        }
        
        if(key_pressed == UP_TIME && delay<=400)
        {
            delay = delay + 25;
        }
        else if(key_pressed == DOWN_TIME && delay>50)
        {
            delay = delay - 25;
        }
        
        timeout(delay); // Задержка при отрисовке
        checkDirection(snake, key_pressed);
    }
    
    free(snake->tail);
    free(snake);
    free(food);
    endwin(); // Завершаем режим curses mod
    return 0;
}

