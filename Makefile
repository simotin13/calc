SRCS=		\
	main.c	\
	calclib.c

CFLAGS=-Wall 

OBJS := $(SRCS:.c=.o)

TARGET=calc

all: ${OBJS}
	${CC} ${CFLAGS} ${SRCS} -o ${TARGET}

clean:
	rm -f ${TARGET} ${OBJS}
