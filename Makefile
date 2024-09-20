# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpancorb <jpancorb@student.42barcel>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/22 17:58:31 by jpancorb          #+#    #+#              #
#    Updated: 2024/09/18 20:06:59 by jpancorb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell


SRC_FILES 	= main.c tokenizer_1.c tokenizer_2.c utils.c parser_1.c parser_2.c\
			parser_3.c expander.c parsing_free.c a_env_variables.c\
			a_execute.c errors_fn.c a_fd_utils.c a_list_fn.c parsing_test.c\
			a_herdoc.c  a_exit_fn.c
SRCS		= $(addprefix src/, $(SRC_FILES))
OBJS		= $(patsubst src/%.c, obj/%.o, $(SRCS))
HEADER		= include/minishell.h
MODE		= none

LIBFT		= libft/libft.a

CFLAGS		= -Wall -Werror -Wextra
LDFLAGS		= -lreadline -Llibft -lft

ifeq ($(MODE),debug)
	CFLAGS += -fsanitize=address -g
endif

ifeq ($(MODE),noflag)
	CFLAGS = -g
endif


all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
		cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c $(HEADER)
		@mkdir -p obj
		cc $(CFLAGS) -c $< -o $@

$(LIBFT):
		make --silent -C libft

clean:
		rm -f obj/*.o
		make --silent -C libft clean

fclean: clean
		rm -f $(NAME)
		rm -rf obj
		make --silent -C libft fclean

re: fclean all

.PHONY: all clean fclean re