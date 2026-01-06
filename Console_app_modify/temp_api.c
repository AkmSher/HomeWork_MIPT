
#include "temp_api.h"
#include <limits.h>

// Названия месяцев
const char* getMonthName(int month) {
    static const char *months[] = {
        "", "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
    };
    if (month >= 1 && month <= 12) {
        return months[month];
    }
    return "Неизвестный месяц";
}

// Создание массива
TemperatureArray* createArray(int capacity) {
    TemperatureArray *arr = (TemperatureArray*)malloc(sizeof(TemperatureArray));
    if (!arr) return NULL;
    
    arr->records = (TemperatureRecord*)malloc(capacity * sizeof(TemperatureRecord));
    if (!arr->records) {
        free(arr);
        return NULL;
    }
    
    arr->count = 0;
    arr->capacity = capacity;
    return arr;
}

// Освобождение памяти
void freeArray(TemperatureArray *arr) {
    if (arr) {
        free(arr->records);
        free(arr);
    }
}

// Добавление записи
int addRecord(TemperatureArray *arr, TemperatureRecord record) {
    if (!arr) return -1;
    
    // Увеличение размера массива при необходимости
    if (arr->count >= arr->capacity) {
        int new_capacity = arr->capacity * 2;
        TemperatureRecord *new_records = (TemperatureRecord*)realloc(
            arr->records, new_capacity * sizeof(TemperatureRecord)
        );
        if (!new_records) return -1;
        
        arr->records = new_records;
        arr->capacity = new_capacity;
    }
    
    arr->records[arr->count] = record;
    arr->count++;
    return 0;
}

// Удаление записи по индексу
int deleteRecord(TemperatureArray *arr, int index) {
    if (!arr || index < 0 || index >= arr->count) return -1;
    
    // Сдвиг элементов
    for (int i = index; i < arr->count - 1; i++) {
        arr->records[i] = arr->records[i + 1];
    }
    arr->count--;
    return 0;
}

// Печать массива
void printArray(const TemperatureArray *arr) {
    if (!arr || arr->count == 0) {
        printf("Массив пуст\n");
        return;
    }
    
    printf("\n%-5s %-10s %-8s %-12s\n", "№", "Дата", "Время", "Температура");
    printf("─────────────────────────────────────────\n");
    
    for (int i = 0; i < arr->count; i++) {
        TemperatureRecord *r = &arr->records[i];
        printf("%-5d %04d-%02d-%02d %02d:%02d   %+4d°C\n",
               i + 1, r->year, r->month, r->day, 
               r->hour, r->minute, r->temperature);
    }
    printf("\nВсего записей: %d\n", arr->count);
}

// Загрузка из CSV файла
int loadFromCSV(TemperatureArray *arr, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Ошибка: не удалось открыть файл '%s'\n", filename);
        return -1;
    }
    
    char line[256];
    int line_num = 0;
    int loaded = 0;
    
    // Пропускаем заголовок, если есть
    if (fgets(line, sizeof(line), file)) {
        line_num++;
    }
    
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        TemperatureRecord record;
        
        // Формат: год;месяц;день;час;минута;температура
        int parsed = sscanf(line, "%d;%d;%d;%d;%d;%d",
                           &record.year, &record.month, &record.day,
                           &record.hour, &record.minute, &record.temperature);
        
        if (parsed == 6) {
            // Проверка корректности данных
            if (record.year >= 1900 && record.year <= 2100 &&
                record.month >= 1 && record.month <= 12 &&
                record.day >= 1 && record.day <= 31 &&
                record.hour >= 0 && record.hour <= 23 &&
                record.minute >= 0 && record.minute <= 59 &&
                record.temperature >= -99 && record.temperature <= 99) {
                
                if (addRecord(arr, record) == 0) {
                    loaded++;
                }
            } else {
                fprintf(stderr, "Предупреждение: некорректные данные в строке %d\n", line_num);
            }
        }
    }
    
    fclose(file);
    printf("Загружено записей: %d\n", loaded);
    return loaded;
}

// Сравнение для сортировки по дате
static int compareDates(const void *a, const void *b) {
    const TemperatureRecord *r1 = (const TemperatureRecord*)a;
    const TemperatureRecord *r2 = (const TemperatureRecord*)b;
    
    if (r1->year != r2->year) return r1->year - r2->year;
    if (r1->month != r2->month) return r1->month - r2->month;
    if (r1->day != r2->day) return r1->day - r2->day;
    if (r1->hour != r2->hour) return r1->hour - r2->hour;
    return r1->minute - r2->minute;
}

// Сортировка по дате
void sortByDate(TemperatureArray *arr) {
    if (!arr || arr->count <= 1) return;
    qsort(arr->records, arr->count, sizeof(TemperatureRecord), compareDates);
}

// Сравнение для сортировки по температуре
static int compareTemperature(const void *a, const void *b) {
    const TemperatureRecord *r1 = (const TemperatureRecord*)a;
    const TemperatureRecord *r2 = (const TemperatureRecord*)b;
    return r1->temperature - r2->temperature;
}

// Сортировка по температуре
void sortByTemperature(TemperatureArray *arr) {
    if (!arr || arr->count <= 1) return;
    qsort(arr->records, arr->count, sizeof(TemperatureRecord), compareTemperature);
}

// Статистика по месяцу
MonthStats getMonthStatistics(const TemperatureArray *arr, int year, int month) {
    MonthStats stats = {0.0, INT_MAX, INT_MIN, 0};
    
    if (!arr) return stats;
    
    long long sum = 0;
    
    for (int i = 0; i < arr->count; i++) {
        TemperatureRecord *r = &arr->records[i];
        
        if (r->year == year && r->month == month) {
            sum += r->temperature;
            stats.count++;
            
            if (r->temperature < stats.min) {
                stats.min = r->temperature;
            }
            if (r->temperature > stats.max) {
                stats.max = r->temperature;
            }
        }
    }
    
    if (stats.count > 0) {
        stats.average = (float)sum / stats.count;
    } else {
        stats.min = 0;
        stats.max = 0;
    }
    
    return stats;
}

// Печать статистики по месяцу
void printMonthStatistics(int month, MonthStats stats) {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   Статистика за %s   \n", getMonthName(month));
    printf("╠════════════════════════════════════════╣\n");
    
    if (stats.count == 0) {
        printf("║ Нет данных за этот месяц               ║\n");
    } else {
        printf("║ Средняя температура:  %+6.1f°C         ║\n", stats.average);
        printf("║ Минимальная:          %+6d°C           ║\n", stats.min);
        printf("║ Максимальная:         %+6d°C           ║\n", stats.max);
        printf("║ Количество измерений: %6d              ║\n", stats.count);
    }
    
    printf("╚════════════════════════════════════════╝\n");
}

// Статистика за год
YearStats getYearStatistics(const TemperatureArray *arr, int year) {
    YearStats stats = {0.0, INT_MAX, INT_MIN, 0};
    
    if (!arr) return stats;
    
    long long sum = 0;
    
    for (int i = 0; i < arr->count; i++) {
        TemperatureRecord *r = &arr->records[i];
        
        if (r->year == year) {
            sum += r->temperature;
            stats.count++;
            
            if (r->temperature < stats.min) {
                stats.min = r->temperature;
            }
            if (r->temperature > stats.max) {
                stats.max = r->temperature;
            }
        }
    }
    
    if (stats.count > 0) {
        stats.average = (float)sum / stats.count;
    } else {
        stats.min = 0;
        stats.max = 0;
    }
    
    return stats;
}

// Печать статистики за год
void printYearStatistics(int year, YearStats stats) {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      Статистика за %d год         ║\n", year);
    printf("╠════════════════════════════════════════╣\n");
    
    if (stats.count == 0) {
        printf("║ Нет данных за этот год                ║\n");
    } else {
        printf("║ Среднегодовая температура: %+6.1f°C   ║\n", stats.average);
        printf("║ Минимальная температура:   %+6d°C     ║\n", stats.min);
        printf("║ Максимальная температура:  %+6d°C     ║\n", stats.max);
        printf("║ Количество измерений:      %6d        ║\n", stats.count);
    }
    
    printf("╚════════════════════════════════════════╝\n");
}

// Справка
void printHelp(const char *program_name) {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║        Анализатор температурных данных v1.0                ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║ ИСПОЛЬЗОВАНИЕ:                                             ║\n");
    printf("║   %s [опции]                                     ║\n", program_name);
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║ ОПЦИИ:                                                     ║\n");
    printf("║   -h              Показать эту справку                     ║\n");
    printf("║   -f <файл>       Входной CSV файл для обработки           ║\n");
    printf("║   -m <месяц>      Показать статистику за указанный месяц   ║\n");
    printf("║                   (число от 1 до 12)                       ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║ ПРИМЕРЫ:                                                   ║\n");
    printf("║   %s -f temperature.csv                          ║\n", program_name);
    printf("║       Загрузить данные и показать годовую статистику       ║\n");
    printf("║                                                            ║\n");
    printf("║   %s -f temperature.csv -m 6                     ║\n", program_name);
    printf("║       Показать статистику только за июнь                   ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║ ФОРМАТ CSV ФАЙЛА:                                          ║\n");
    printf("║   год;месяц;день;час;минута;температура                    ║\n");
    printf("║   2025;01;15;14;30;-5                                      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
}
