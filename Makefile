# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edforte <edforte@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/23 13:09:57 by edforte           #+#    #+#              #
#    Updated: 2024/09/23 13:11:13 by edforte          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CFILES = \
			main.c \
			


OBJ = $(CFILES:.c=.o)


CC = @ gcc
FLAGS = -g -Wall -Wextra -Werror
RM = rm -rf

name : $(NAME)

all : $(NAME)

$(NAME) : $(OBJ)
	 $(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean :
	@ $(RM) $(OBJ)

fclean : clean
	@ $(RM) $(NAME)

re : fclean all