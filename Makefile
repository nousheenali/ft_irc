# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/15 17:20:20 by nali              #+#    #+#              #
#    Updated: 2023/06/04 13:54:25 by nali             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ircserv

SRCS	= main.cpp Server.cpp Client.cpp Channel.cpp pass.cpp parsing.cpp\
		nick.cpp user.cpp quit.cpp Privmsg.cpp utils.cpp Mode.cpp pong.cpp \
		join.cpp kick.cpp part.cpp invite.cpp topic.cpp list.cpp

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