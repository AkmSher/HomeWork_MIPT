#include "temp_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char filename[MAX_FILENAME] = "";
    int target_month = -1;
    int show_help = 0;
    
    // Парсим аргументы командной строки
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            show_help = 1;
        } 
        else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                strncpy(filename, argv[i + 1], MAX_FILENAME - 1);
                filename[MAX_FILENAME - 1] = '0';
                i++;
            } else {
                fprintf(stderr, "Ошибка: опция -f требует аргумента\n");
                print_help();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-m") == 0) {
            if (i + 1 < argc) {
                target_month = atoi(argv[i + 1]);
                i++;
            } else {
                fprintf(stderr, "Ошибка: опция -m требует аргумента\n");
                print_help();
                return 1;
            }
        }
        else {
            fprintf(stderr, "Ошибка: неизвестная опция '%s'\n", argv[i]);
            print_help();
            return 1;
        }
    }
    
    // Если нет параметров или задана опция -h
    if (argc == 1 || show_help) {
        print_help();
        return 0;
    }
    
    // Проверяем, задан ли файл
    if (strlen(filename) == 0) {
        fprintf(stderr, "Ошибка: необходимо указать входной файл с опцией -f\n");
        print_help();
        return 1;
    }
    
    // Выделяем память для записей
    TemperatureRecord *records = 
        (TemperatureRecord *)malloc(MAX_RECORDS * sizeof(TemperatureRecord));
    
    if (records == NULL) {
        fprintf(stderr, "Ошибка: не удается выделить память\n");
        return 1;
    }
    
    // Читаем файл
    int record_count = 0;
    if (!read_csv_file(filename, records, &record_count)) {
        free(records);
        return 1;
    }
    
    if (record_count == 0) {
        fprintf(stderr, "Ошибка: в файле нет корректных записей\n");
        free(records);
        return 1;
    }
    
    printf("\nУспешно загружено записей: %d\n", record_count);
    
    // Вычисляем статистику
    MonthlyStats monthly_stats[12];
    YearlyStats yearly_stats;
    
    calculate_monthly_stats(records, record_count, monthly_stats);
    calculate_yearly_stats(records, record_count, &yearly_stats);
    
    // Выводим результаты
    if (target_month != -1) {
        // Вывод статистики за конкретный месяц
        print_monthly_statistics(monthly_stats, target_month);
    } else {
        // Вывод полной статистики
        print_monthly_statistics(monthly_stats, -1);
        print_yearly_statistics(&yearly_stats);
    }
    
    // Освобождаем память
    free(records);
    
    return 0;
}
