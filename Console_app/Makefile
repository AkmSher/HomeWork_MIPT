# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Имя исполняемого файла
TARGET = temperature_app

# Объектные файлы
OBJS = main.o temp_api.o

# Правило по умолчанию
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Сборка завершена успешно!"

# Компиляция main.c
main.o: main.c temp_api.h
	$(CC) $(CFLAGS) -c main.c

# Компиляция temp_api.c
temp_api.o: temp_api.c temp_api.h
	$(CC) $(CFLAGS) -c temp_api.c

# Очистка скомпилированных файлов
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Очистка завершена!"

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Пересборка проекта
rebuild: clean all

# Информация о проекте
info:
	@echo "Проект: Система учета температуры"
	@echo "Файлы:"
	@echo "  - main.c (главный файл)"
	@echo "  - temp_api.h (заголовочный файл)"
	@echo "  - temp_api.c (реализация функций)"
	@echo "Команды:"
	@echo "  make        - собрать проект"
	@echo "  make clean  - очистить файлы"
	@echo "  make run    - запустить программу"
	@echo "  make rebuild - пересобрать проект"

.PHONY: all clean run rebuild info