#ifndef TEMP_API_H
#define TEMP_API_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для хранения данных о температуре
typedef struct {
    int year;        // Год (4 цифры)
    int month;       // Месяц (1-12)
    int day;         // День (1-31)
    int hour;        // Час (0-23)
    int minute;      // Минута (0-59)
    int temperature; // Температура (-99 до 99)
} TemperatureRecord;

// Структура для хранения массива записей
typedef struct {
    TemperatureRecord *records;
    int count;
    int capacity;
} TemperatureArray;

// Функции работы с массивом
TemperatureArray* createArray(int capacity);
void freeArray(TemperatureArray *arr);
int addRecord(TemperatureArray *arr, TemperatureRecord record);
int deleteRecord(TemperatureArray *arr, int index);
void printArray(const TemperatureArray *arr);

// Функции загрузки данных
int loadFromCSV(TemperatureArray *arr, const char *filename);

// Функции сортировки
void sortByDate(TemperatureArray *arr);
void sortByTemperature(TemperatureArray *arr);

// Статистика по месяцу
typedef struct {
    float average;
    int min;
    int max;
    int count;
} MonthStats;

MonthStats getMonthStatistics(const TemperatureArray *arr, int year, int month);
void printMonthStatistics(int month, MonthStats stats);

// Статистика за год
typedef struct {
    float average;
    int min;
    int max;
    int count;
} YearStats;

YearStats getYearStatistics(const TemperatureArray *arr, int year);
void printYearStatistics(int year, YearStats stats);

// Вспомогательные функции
void printHelp(const char *program_name);
const char* getMonthName(int month);

#endif // TEMP_API_H
