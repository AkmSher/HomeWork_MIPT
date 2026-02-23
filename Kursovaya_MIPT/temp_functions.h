#ifndef TEMP_FUNCTIONS_H
#define TEMP_FUNCTIONS_H

#include <time.h>

#define MAX_RECORDS 10000
#define MAX_FILENAME 256

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int temperature;
} TemperatureRecord;

typedef struct {
    double average;
    int min;
    int max;
    int count;
} MonthlyStats;

typedef struct {
    double average;
    int min;
    int max;
    int total_count;
} YearlyStats;

// Функции для работы с файлами
int read_csv_file(const char *filename, TemperatureRecord *records, int *record_count);
int validate_record(const char *line, TemperatureRecord *record, int line_number);

// Функции для расчета статистики
void calculate_monthly_stats(TemperatureRecord *records, int record_count, 
                             MonthlyStats *monthly_stats);
void calculate_yearly_stats(TemperatureRecord *records, int record_count, 
                            YearlyStats *yearly_stats);

// Функции для вывода
void print_help(void);
void print_monthly_statistics(MonthlyStats *monthly_stats, int month);
void print_yearly_statistics(YearlyStats *yearly_stats);

// Вспомогательные функции
int is_valid_date(int year, int month, int day, int hour, int minute);
int is_valid_temperature(int temp);

#endif // TEMP_FUNCTIONS_H
