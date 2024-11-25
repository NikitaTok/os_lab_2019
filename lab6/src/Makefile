CC = gcc
CFLAGS = -Wall -Werror -g -pthread
LDFLAGS = -pthread

# Цель по умолчанию: собирает и сервер, и клиент
all: server client

# Цель для сервера
server: server.c
	$(CC) $(CFLAGS) server.c -o server $(LDFLAGS)

# Цель для клиента
client: client.c
	$(CC) $(CFLAGS) client.c -o client $(LDFLAGS)

# Цель для очистки: удаляет исполняемые файлы
clean:
	rm -f server client

# Цель для полной очистки: удаляет исполняемые файлы и объектные файлы (если есть)
cleanall: clean
	rm -f *.o

# Цель для пересборки: очищает и пересобирает все
	rebuild: cleanall all