HEADER=codexion.h
NAME=codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm
RMFLAGS = -rf
SRCS = cleaner.c error.c min_heap.c request_dongles.c coders.c ft_atoi.c \
		min_heap_utils.c setup_dongles.c setup_coders.c init.c monitor.c \
		take_dongle.c dongle_utils.c main.c parser.c utils.c data_race.c coder_utils.c \
		data_race_utils.c
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