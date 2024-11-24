CC = gcc
CFLAGS = -Wall -Werror -g -pthread # Добавлено -pthread для поддержки потоков

# Цель по умолчанию: собирает и сервер, и клиент
all: server client

# Цель для сервера
server: server.c
 $(CC) $(CFLAGS) server.c -o server

# Цель для клиента
client: client.c
 $(CC) $(CFLAGS) client.c -o client

# Цель для очистки
clean:
 rm -f server client

# Цель для полной очистки (включая объектные файлы)
cleanall: clean
 rm -f *.o

# Цель для пересборки
rebuild: cleanall all