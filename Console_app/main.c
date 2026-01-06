#include <stdio.h>
#include "temp_api.h"

#define MAX_RECORDS 1000

int main(void) {
    TemperatureRecord records[MAX_RECORDS];
    int count = 0;
    
    printf("=== Система учета температуры ===\n\n");
    
    // Добавление тестовых данных за 2025 год
    // Январь
    addRecord(records, &count, 2025, 1, 1, 12, 0, -15);
    addRecord(records, &count, 2025, 1, 5, 14, 30, -20);
    addRecord(records, &count, 2025, 1, 10, 9, 0, -10);
    addRecord(records, &count, 2025, 1, 15, 18, 45, -18);
    addRecord(records, &count, 2025, 1, 20, 8, 15, -25);
    addRecord(records, &count, 2025, 1, 25, 16, 0, -12);
    
    // Февраль
    addRecord(records, &count, 2025, 2, 1, 10, 0, -8);
    addRecord(records, &count, 2025, 2, 5, 13, 20, -5);
    addRecord(records, &count, 2025, 2, 10, 11, 30, -3);
    addRecord(records, &count, 2025, 2, 15, 15, 0, 0);
    addRecord(records, &count, 2025, 2, 20, 9, 45, -2);
    
    // Март
    addRecord(records, &count, 2025, 3, 1, 12, 0, 2);
    addRecord(records, &count, 2025, 3, 5, 14, 0, 5);
    addRecord(records, &count, 2025, 3, 10, 10, 30, 8);
    addRecord(records, &count, 2025, 3, 15, 16, 15, 10);
    addRecord(records, &count, 2025, 3, 20, 11, 0, 12);
    addRecord(records, &count, 2025, 3, 25, 13, 30, 15);
    
    // Апрель
    addRecord(records, &count, 2025, 4, 1, 12, 0, 18);
    addRecord(records, &count, 2025, 4, 10, 14, 0, 20);
    addRecord(records, &count, 2025, 4, 20, 10, 30, 22);
    
    // Май
    addRecord(records, &count, 2025, 5, 1, 12, 0, 25);
    addRecord(records, &count, 2025, 5, 10, 14, 0, 28);
    addRecord(records, &count, 2025, 5, 20, 10, 30, 30);
    
    // Июнь
    addRecord(records, &count, 2025, 6, 1, 12, 0, 32);
    addRecord(records, &count, 2025, 6, 10, 14, 0, 35);
    addRecord(records, &count, 2025, 6, 20, 10, 30, 33);
    
    // Июль
    addRecord(records, &count, 2025, 7, 1, 12, 0, 36);
    addRecord(records, &count, 2025, 7, 10, 14, 0, 38);
    addRecord(records, &count, 2025, 7, 15, 16, 0, 40);
    addRecord(records, &count, 2025, 7, 20, 10, 30, 37);
    // Август
    addRecord(records, &count, 2025, 8, 1, 12, 0, 35);
    addRecord(records, &count, 2025, 8, 10, 14, 0, 33);
    addRecord(records, &count, 2025, 8, 20, 10, 30, 30);
    
    // Сентябрь
    addRecord(records, &count, 2025, 9, 1, 12, 0, 25);
    addRecord(records, &count, 2025, 9, 10, 14, 0, 20);
    addRecord(records, &count, 2025, 9, 20, 10, 30, 15);
    
    // Октябрь
    addRecord(records, &count, 2025, 10, 1, 12, 0, 10);
    addRecord(records, &count, 2025, 10, 10, 14, 0, 8);
    addRecord(records, &count, 2025, 10, 20, 10, 30, 5);
    
    // Ноябрь
    addRecord(records, &count, 2025, 11, 1, 12, 0, 0);
    addRecord(records, &count, 2025, 11, 10, 14, 0, -5);
    addRecord(records, &count, 2025, 11, 20, 10, 30, -8);
    
    // Декабрь
    addRecord(records, &count, 2025, 12, 1, 12, 0, -10);
    addRecord(records, &count, 2025, 12, 10, 14, 0, -15);
    addRecord(records, &count, 2025, 12, 20, 10, 30, -20);
    addRecord(records, &count, 2025, 12, 25, 18, 0, -18);
    
    // Вывод всех записей
    printAllRecords(records, count);
    
    // Статистика по каждому месяцу 2025 года
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║   СТАТИСТИКА ПО МЕСЯЦАМ (2025 год)       ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    
    for (int month = 1; month <= 12; month++) {
        MonthStatistics monthStats = getMonthStatistics(records, count, 2025, month);
        printMonthStatistics(monthStats);
    }
    
    // Статистика за весь год
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║      ГОДОВАЯ СТАТИСТИКА (2025 год)        ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    
    YearStatistics yearStats = getYearStatistics(records, count, 2025);
    printYearStatistics(yearStats);
    
    return 0;
}
