# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = 

# Имя исполняемого файла
TARGET = temperature

# Объектные файлы
OBJS = main.o temp_api.o

# Правило по умолчанию
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "✓ Сборка завершена: $(TARGET)"

# Компиляция main.c
main.o: main.c temp_api.h
	$(CC) $(CFLAGS) -c main.c
# Компиляция temp_api.c
temp_api.o: temp_api.c temp_api.h
	$(CC) $(CFLAGS) -c temp_api.c

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "✓ Очистка выполнена"

# Очистка + удаление временных файлов
distclean: clean
	rm -f *~ *.bak

# Запуск с тестовыми данными
run: $(TARGET)
	./$(TARGET) -f temperature.csv

# Справка
help: $(TARGET)
	./$(TARGET) -h

# Отметка фиктивных целей
.PHONY: all clean distclean run help
