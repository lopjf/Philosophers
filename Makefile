# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: loris <loris@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/27 15:10:47 by loris             #+#    #+#              #
#    Updated: 2022/12/27 18:09:29 by loris            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo.a

SRCS = philo.c

OBJS = $(SRCS:.c=.o)

CC				= gcc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror
RUN				= ar rcs

%.o: %.c
	$(CC) -c $(CFLAGS) -I headers $< -o $(<:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)

	$(CC) $(CFLAGS) -pthread $(OBJS) -fsanitize=address -o philo


clean:
	$(RM) $(NAME) $(OBJS)

fclean:
	$(RM) $(NAME) $(OBJS)

re: fclean all

.PHONY: all clean fclean re