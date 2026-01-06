#include "temp_api.h"
#include <stdio.h>
#include <limits.h>

// Добавление записи в массив
void addRecord(TemperatureRecord *records, int *count, int year, int month, 
               int day, int hour, int minute, int temperature) {
    records[*count].year = year;
    records[*count].month = month;
    records[*count].day = day;
    records[*count].hour = hour;
    records[*count].minute = minute;
    records[*count].temperature = temperature;
    (*count)++;
}

// Получение статистики по месяцу
MonthStatistics getMonthStatistics(TemperatureRecord *records, int count, 
                                   int year, int month) {
    MonthStatistics stats;
    stats.month = month;
    stats.average = 0.0;
    stats.min = INT_MAX;
    stats.max = INT_MIN;
    stats.count = 0;
    
    long long sum = 0;
    
    for (int i = 0; i < count; i++) {
        if (records[i].year == year && records[i].month == month) {
            int temp = records[i].temperature;
            sum += temp;
            stats.count++;
            
            if (temp < stats.min) {
                stats.min = temp;
            }
            if (temp > stats.max) {
                stats.max = temp;
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

// Вывод статистики по месяцу
void printMonthStatistics(MonthStatistics stats) {
    if (stats.count == 0) {
        printf("Нет данных за %s\n", getMonthName(stats.month));
        return;
    }
    
    printf("\n=== Статистика за %s ===\n", getMonthName(stats.month));
    printf("Количество записей: %d\n", stats.count);
    printf("Среднемесячная температура: %.2f°C\n", stats.average);
    printf("Минимальная температура: %d°C\n", stats.min);
    printf("Максимальная температура: %d°C\n", stats.max);
}

// Получение статистики по году
YearStatistics getYearStatistics(TemperatureRecord *records, int count, int year) {
    YearStatistics stats;
    stats.year = year;
    stats.average = 0.0;
    stats.min = INT_MAX;
    stats.max = INT_MIN;
    stats.count = 0;
    
    long long sum = 0;
    
    for (int i = 0; i < count; i++) {
        if (records[i].year == year) {
            int temp = records[i].temperature;
            sum += temp;
            stats.count++;
            
            if (temp < stats.min) {
                stats.min = temp;
            }
            if (temp > stats.max) {
                stats.max = temp;
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

// Вывод статистики по году
void printYearStatistics(YearStatistics stats) {
    if (stats.count == 0) {
        printf("Нет данных за %d год\n", stats.year);
        return;
    }
    
    printf("\n========== Статистика за %d год ==========\n", stats.year);
    printf("Количество записей: %d\n", stats.count);
    printf("Среднегодовая температура: %.2f°C\n", stats.average);
    printf("Минимальная температура: %d°C\n", stats.min);
    printf("Максимальная температура: %d°C\n", stats.max);
    printf("==========================================\n");
}

// Вывод одной записи
void printRecord(TemperatureRecord record) {
    printf("%04d-%02d-%02d %02d:%02d | Температура: %+3d°C\n",
           record.year, record.month, record.day,
           record.hour, record.minute, record.temperature);
}

// Вывод всех записей
void printAllRecords(TemperatureRecord *records, int count) {
    printf("\n=== Все записи температуры ===\n");
    for (int i = 0; i < count; i++) {
        printf("%3d. ", i + 1);
        printRecord(records[i]);
    }
    printf("\n");
}

// Получение названия месяца
const char* getMonthName(int month) {
    static const char* months[] = {
        "Неизвестно",
        "Январь", "Февраль", "Март", "Апрель",
        "Май", "Июнь", "Июль", "Август",
        "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
    };
    
    if (month >= 1 && month <= 12) {
        return months[month];
    }
    return months[0];
}
