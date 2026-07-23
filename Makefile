HEADER=codexion.h
NAME=codexion
CC = cc
CFLAGS = -pthread
RM = rm
RMFLAGS = -rf
SRCS = main.c ft_atoi.c request.c utils.c \
	   coders.c dongles.c min_heap.c parser.c monitor.c \
# 	    simulation.c
OBJECTS = ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJECTS}
	${CC} ${CFLAGS} -o ${NAME} ${OBJECTS}

%.o: %.c ${HEADER}
	${CC} ${CFLAGS} -c $< -o $@

clean:
	${RM} ${RMFLAGS} ${OBJECTS}

fclean: clean
	${RM} ${RMFLAGS} ${NAME}

re: fclean all

.PHONY: all clean