#ifndef TEMP_API_H
#define TEMP_API_H

// Структура для хранения данных о температуре
typedef struct {
    int year;        // Год (4 цифры)
    int month;       // Месяц (1-12)
    int day;         // День (1-31)
    int hour;        // Час (0-23)
    int minute;      // Минута (0-59)
    int temperature; // Температура (-99 до 99)
} TemperatureRecord;

// Структура для хранения статистики за месяц
typedef struct {
    int month;
    float average;
    int min;
    int max;
    int count;
} MonthStatistics;

// Структура для хранения статистики за год
typedef struct {
    int year;
    float average;
    int min;
    int max;
    int count;
} YearStatistics;

// Функции для работы с данными
void addRecord(TemperatureRecord *records, int *count, int year, int month, 
               int day, int hour, int minute, int temperature);

// Функции статистики по месяцу
MonthStatistics getMonthStatistics(TemperatureRecord *records, int count, 
                                   int year, int month);
void printMonthStatistics(MonthStatistics stats);

// Функции статистики по году
YearStatistics getYearStatistics(TemperatureRecord *records, int count, int year);
void printYearStatistics(YearStatistics stats);

// Вспомогательные функции
void printRecord(TemperatureRecord record);
void printAllRecords(TemperatureRecord *records, int count);
const char* getMonthName(int month);

#endif // TEMP_API_H
