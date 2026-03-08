#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ─── Структура узла списка ───────────────────────────────────────────────────
struct list {
    char word[20];
    struct list *next;
};

// ─── Добавление слова в конец списка ────────────────────────────────────────
struct list *add_to_list(struct list *head, const char *word) {
    // Создаём новый узел
    struct list *new_node = (struct list *)malloc(sizeof(struct list));
    if (new_node == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(1);
    }

    strncpy(new_node->word, word, 19);
    new_node->word[19] = '\0'; // защита от переполнения
    new_node->next = NULL;

    // Если список пуст — новый узел становится головой
    if (head == NULL) {
        return new_node;
    }

    // Иначе идём до конца и добавляем
    struct list *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;

    return head;
}

// ─── Обмен содержимым двух узлов (без перестановки указателей) ──────────────
void swap_elements(struct list *a, struct list *b) {
    char temp[20];
    strncpy(temp,    a->word, 20);
    strncpy(a->word, b->word, 20);
    strncpy(b->word, temp,    20);
}

// ─── Сортировка пузырьком через swap_elements ────────────────────────────────
void sort_list(struct list *head) {
    if (head == NULL) return;

    int swapped;
    struct list *current;
    struct list *last = NULL; // граница отсортированной части

    do {
        swapped = 0;
        current = head;

        while (current->next != last) {
            if (strcmp(current->word, current->next->word) > 0) {
                swap_elements(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }
        last = current; // последний элемент встал на место
    } while (swapped);
}

// ─── Вывод списка ────────────────────────────────────────────────────────────
void print_list(struct list *head) {
    struct list *current = head;
    while (current != NULL) {
        if (current->next != NULL) {
            printf("%s ", current->word);
        } else {
            printf("%s",  current->word); // последнее слово без пробела
        }
        current = current->next;
    }
    printf("\n");
}

// ─── Удаление всего списка ───────────────────────────────────────────────────
void delete_list(struct list **head) {
    struct list *current = *head;
    struct list *next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }

    *head = NULL; // обнуляем указатель на голову
}

// ─── Главная функция ─────────────────────────────────────────────────────────
int main() {
    struct list *head = NULL;
    char buffer[1024];
    char word[20];

    // Читаем всю строку
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }

    // Убираем точку в конце (и '\n' если есть)
    int len = strlen(buffer);
    for (int i = len - 1; i >= 0; i--) {
        if (buffer[i] == '.' || buffer[i] == '\n' || buffer[i] == '\r') {
            buffer[i] = '\0';
        } else {
            break;
        }
    }

    // Разбиваем строку на слова и добавляем в список
    char *token = strtok(buffer, " ");
    while (token != NULL) {
        strncpy(word, token, 19);
        word[19] = '\0';
        head = add_to_list(head, word);
        token = strtok(NULL, " ");
    }

    // Сортируем список
    sort_list(head);

    // Выводим результат
    print_list(head);

    // Удаляем список и освобождаем память
    delete_list(&head);

    // Проверяем что указатель обнулён
    if (head == NULL) {
        // Список успешно удалён
    }

    return 0;
}

