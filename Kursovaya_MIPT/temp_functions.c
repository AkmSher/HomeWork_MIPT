#include "temp_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Проверка корректности даты и времени
int is_valid_date(int year, int month, int day, int hour, int minute) {
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    
    // Проверка дней в месяце
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Проверка на високосный год
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_month[2] = 29;
    }
    
    if (day > days_in_month[month]) return 0;
    
    return 1;
}

// Проверка корректности температуры
int is_valid_temperature(int temp) {
    return (temp >= -99 && temp <= 99);
}

// Парсинг и валидация одной строки CSV
int validate_record(const char *line, TemperatureRecord *record, int line_number) {
    int year, month, day, hour, minute, temperature;
    int parsed;
    
    // Пропускаем заголовок
    if (strstr(line, "YEAR") != NULL && strstr(line, "TEMPERATURE") != NULL) {
        return 0; // Это заголовок, пропускаем
    }
    
    // Парсим строку
    parsed = sscanf(line, "%d;%d;%d;%d;%d;%d", 
                    &year, &month, &day, &hour, &minute, &temperature);
    
    if (parsed != 6) {
        fprintf(stderr, "Ошибка в строке %d: неверный формат\n", line_number);
        return 0;
    }
    
    // Проверяем валидность данных
    if (!is_valid_date(year, month, day, hour, minute)) {
        fprintf(stderr, "Ошибка в строке %d: некорректная дата/время\n", line_number);
        return 0;
    }
    
    if (!is_valid_temperature(temperature)) {
        fprintf(stderr, "Ошибка в строке %d: температура вне диапазона [-99, 99]\n", 
                line_number);
        return 0;
    }
    
    record->year = year;
    record->month = month;
    record->day = day;
    record->hour = hour;
    record->minute = minute;
    record->temperature = temperature;
    
    return 1;
}

// Чтение CSV файла
int read_csv_file(const char *filename, TemperatureRecord *records, int *record_count) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        fprintf(stderr, "Ошибка: не удается открыть файл '%s'\n", filename);
        return 0;
    }
    
    char line[256];
    int line_number = 0;
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        
        // Удаляем символ новой строки
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '0';
        }
        
        // Пропускаем пустые строки
        if (strlen(line) == 0) {
            continue;
        }
        
        // Валидируем и добавляем запись
        if (validate_record(line, &records[count], line_number)) {
            count++;
            
            if (count >= MAX_RECORDS) {
                fprintf(stderr, "Предупреждение: превышен максимум записей (%d)\n", 
                        MAX_RECORDS);
                break;
            }
        }
    }
    
    fclose(file);
    *record_count = count;
    
    return 1;
}

// Расчет статистики по месяцам
void calculate_monthly_stats(TemperatureRecord *records, int record_count, 
                             MonthlyStats *monthly_stats) {
    // Инициализируем массив для 12 месяцев
    for (int i = 0; i < 12; i++) {
        monthly_stats[i].average = 0.0;
        monthly_stats[i].min = 100;
        monthly_stats[i].max = -100;
        monthly_stats[i].count = 0;
    }
    
    // Обрабатываем все записи
    for (int i = 0; i < record_count; i++) {
        int month = records[i].month - 1; // Индекс от 0 до 11
        int temp = records[i].temperature;
        
        monthly_stats[month].average += temp;
        monthly_stats[month].count++;
        
        if (temp < monthly_stats[month].min) {
            monthly_stats[month].min = temp;
        }
        if (temp > monthly_stats[month].max) {
            monthly_stats[month].max = temp;
        }
    }
    
    // Вычисляем средние значения
    for (int i = 0; i < 12; i++) {
        if (monthly_stats[i].count > 0) {
            monthly_stats[i].average /= monthly_stats[i].count;
        }
    }
}

// Расчет статистики за год
void calculate_yearly_stats(TemperatureRecord *records, int record_count, 
                            YearlyStats *yearly_stats) {
    yearly_stats->average = 0.0;
    yearly_stats->min = 100;
    yearly_stats->max = -100;
    yearly_stats->total_count = record_count;
    
    if (record_count == 0) {
        return;
    }
    
    for (int i = 0; i < record_count; i++) {
        int temp = records[i].temperature;
        yearly_stats->average += temp;
        
        if (temp < yearly_stats->min) {
            yearly_stats->min = temp;
        }
        if (temp > yearly_stats->max) {
            yearly_stats->max = temp;
        }
    }
    
    yearly_stats->average /= record_count;
}

// Вывод справки
void print_help(void) {
    printf("\n========================================\n");
    printf("  ПРОГРАММА: Статистика температуры\n");
    printf("========================================\n\n");
    printf("Описание:\n");
    printf("  Приложение анализирует данные температуры из CSV файла\n");
    printf("  и выводит статистику по месяцам и за год.\n\n");
    printf("Использование:\n");
    printf("  program [опции]\n\n");
    printf("Опции:\n");
    printf("  -h                    Вывести эту справку\n");
    printf("  -f <filename.csv>     Путь к входному CSV файлу (обязательно)\n");
    printf("  -m <номер_месяца>     Вывести статистику только за указанный месяц (1-12)\n\n");
    printf("Примеры:\n");
    printf("  program -h\n");
    printf("  program -f temperature.csv\n");
    printf("  program -f temperature.csv -m 1\n\n");
    printf("Формат CSV файла:\n");
    printf("  YEAR;MONTH;DAY;HOUR;MINUTE;TEMPERATURE\n");
    printf("  2021;1;1;23;1;-5\n");
    printf("  2021;1;1;23;3;-6\n\n");
    printf("========================================\n\n");
}

// Вывод статистики по месяцам
void print_monthly_statistics(MonthlyStats *monthly_stats, int month) {
    const char *month_names[] = {
        "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
    };
    
    if (month == -1) {
        // Вывод всех месяцев
        printf("\n========== СТАТИСТИКА ПО МЕСЯЦАМ ==========\n\n");
        
        for (int i = 0; i < 12; i++) {
            if (monthly_stats[i].count > 0) {
                printf("%s (месяц %d):\n", month_names[i], i + 1);
                printf("  Среднемесячная температура: %.2f°C\n", monthly_stats[i].average);
                printf("  Минимальная температура:    %d°C\n", monthly_stats[i].min);
                printf("  Максимальная температура:   %d°C\n", monthly_stats[i].max);
                printf("  Количество измерений:       %d\n\n", monthly_stats[i].count);
            } else {
                printf("%s (месяц %d): нет данных\n\n", month_names[i], i + 1);
            }
        }
    } else {
        // Вывод конкретного месяца
        int idx = month - 1;
        
        if (idx < 0 || idx >= 12) {
            fprintf(stderr, "Ошибка: номер месяца должен быть от 1 до 12\n");
            return;
        }
        
        printf("\n========== СТАТИСТИКА ЗА %s ==========\n\n", 
               month_names[idx]);
        
        if (monthly_stats[idx].count > 0) {
            printf("Среднемесячная температура: %.2f°C\n", monthly_stats[idx].average);
            printf("Минимальная температура:    %d°C\n", monthly_stats[idx].min);
            printf("Максимальная температура:   %d°C\n", monthly_stats[idx].max);
            printf("Количество измерений:       %d\n\n", monthly_stats[idx].count);
        } else {
            printf("Нет данных за этот месяц.\n\n");
        }
    }
}

// Вывод годовой статистики
void print_yearly_statistics(YearlyStats *yearly_stats) {
    printf("\n========== ГОДОВАЯ СТАТИСТИКА ==========\n\n");
    
    if (yearly_stats->total_count > 0) {
        printf("Среднегодовая температура: %.2f°C\n", yearly_stats->average);
        printf("Минимальная температура:   %d°C\n", yearly_stats->min);
        printf("Максимальная температура:  %d°C\n", yearly_stats->max);
        printf("Всего измерений:           %d\n\n", yearly_stats->total_count);
    } else {
        printf("Нет данных для анализа.\n\n");
    }
}
