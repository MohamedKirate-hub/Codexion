HEADER = coders/codexion.h
NAME=codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm
RMFLAGS = -rf
SRCS = coders/cleaner.c coders/error.c coders/main.c coders/monitor.c coders/setup_coders.c coders/utils.c \
	   coders/coder_utils.c coders/data_race.c coders/ft_atoi.c coders/min_heap.c coders/parser.c \
	   coders/setup_dongles.c coders/coders.c coders/dongle_utils.c coders/init.c coders/min_heap_utils.c \
	   coders/request_dongles.c coders/take_dongle.c


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

.PHONY: all clean re fclean