# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nali <nali@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/15 17:20:20 by nali              #+#    #+#              #
#    Updated: 2023/06/02 11:50:02 by nali             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ircserv

SRCS	= main.cpp Server.cpp Client.cpp Channel.cpp Mode.cpp pass.cpp parsing.cpp\
		nick.cpp user.cpp quit.cpp Privmsg.cpp

OBJS	= ${SRCS:.cpp=.o}

CC		= c++

CFLAGS	=  -std=c++98

RM		= rm -f

all:	${NAME}

.c.o:	${CC} ${CFLAGS} -I -c $< -o ${<:.cpp=.o}

${NAME}:	${OBJS}
		$(CC) ${CFLAGS} ${SRCS} -o $(NAME)

clean:		
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re .c.o