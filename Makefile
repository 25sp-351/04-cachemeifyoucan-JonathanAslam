CC = clang
CFLAGS = -g -Wall
RM = rm -f

main: main.c classes/rodcut.c classes/dll.c
	$(CC) $(CFLAGS) -o main main.c classes/rodcut.c classes/dll.c

lru: mainLRU.c classes/rodcut.c classes/dll.c classes/bst.c classes/lru.c 
	$(CC) $(CFLAGS) -o main mainLRU.c classes/rodcut.c classes/dll.c classes/bst.c classes/lru.c

random: mainLRU.c classes/rodcut.c classes/dll.c classes/bst.c classes/randCache.c 
	$(CC) $(CFLAGS) -o main mainLRU.c classes/rodcut.c classes/dll.c classes/bst.c classes/randCache.c

clean:
	$(RM) main