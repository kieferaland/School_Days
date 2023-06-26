CFLAGS = -Wall -g 
LDFLAGS =

OBJS = shell.o

all: shell

shell: $(OBJS)
	$(CC) $(CFLAGS) -o $(OBJS) shell.c $(LDFLAGS) -lreadline -ltermcap

clean: 
	rm -rf $(OBJS)
