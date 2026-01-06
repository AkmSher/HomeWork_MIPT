#include "temp_api.h"
#include <getopt.h>

int main(int argc, char *argv[]) {
    char *filename = NULL;
    int month = -1;  // -1 означает показать статистику за весь год
    int opt;
    
    // Обработка аргументов командной строки
    while ((opt = getopt(argc, argv, "hf:m:")) != -1) {
        switch (opt) {
            case 'h':
                printHelp(argv[0]);
                return 0;
                
            case 'f':
                filename = optarg;
                break;
                
            case 'm':
                month = atoi(optarg);
                if (month < 1 || month > 12) {
                    fprintf(stderr, "Ошибка: номер месяца должен быть от 1 до 12\n");
                    return 1;
                }
                break;
                
            default:
                fprintf(stderr, "Используйте -h для справки\n");
                return 1;
        }
    }
    
    // Проверка обязательных параметров
    if (filename == NULL) {
        fprintf(stderr, "Ошибка: не указан входной файл\n");
        fprintf(stderr, "Используйте: %s -f <файл> [-m <месяц>]\n", argv[0]);
        fprintf(stderr, "Для справки: %s -h\n", argv[0]);
        return 1;
    }
    
    // Создание массива
    TemperatureArray *arr = createArray(100);
    if (!arr) {
        fprintf(stderr, "Ошибка: не удалось создать массив\n");
        return 1;
    }
    
    // Загрузка данных из файла
    printf("Загрузка данных из файла: %s\n", filename);
    int loaded = loadFromCSV(arr, filename);
    
    if (loaded <= 0) {
        fprintf(stderr, "Ошибка: не удалось загрузить данные\n");
        freeArray(arr);
        return 1;
    }
    
    // Сортировка по дате
    sortByDate(arr);
    
    // Определение года (берем из первой записи)
    int year = arr->records[0].year;
    
    // Вывод статистики
    if (month != -1) {
        // Статистика за конкретный месяц
        MonthStats stats = getMonthStatistics(arr, year, month);
        printMonthStatistics(month, stats);
    } else {
        // Статистика за весь год
        YearStats yearStats = getYearStatistics(arr, year);
        printYearStatistics(year, yearStats);
        
        // Дополнительно: статистика по каждому месяцу
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║           Статистика по месяцам за %d год           ║\n", year);
        printf("╠════════════════════════════════════════════════════════════╣\n");
        printf("║ Месяц      │ Средняя  │ Мин.  │ Макс. │ Измерений       ║\n");
        printf("╠════════════════════════════════════════════════════════════╣\n");
        
        for (int m = 1; m <= 12; m++) {
            MonthStats mStats = getMonthStatistics(arr, year, m);
            if (mStats.count > 0) {
                printf("║ %-10s │ %+6.1f°C │ %+5d │ %+5d │ %6d          ║\n",
                       getMonthName(m), mStats.average, 
                       mStats.min, mStats.max, mStats.count);
            }
        }
        
        printf("╚════════════════════════════════════════════════════════════╝\n");
    }
    
    // Освобождение памяти
    freeArray(arr);
    
    return 0;
}
