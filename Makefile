# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/15 17:20:20 by nali              #+#    #+#              #
#    Updated: 2023/06/15 12:23:03 by sfathima         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ircserv

SRCS	= main.cpp Server.cpp Client.cpp Channel.cpp pass.cpp parsing.cpp\
		nick.cpp user.cpp quit.cpp Privmsg.cpp utils.cpp Mode.cpp pong.cpp \
		join.cpp kick.cpp part.cpp invite.cpp topic.cpp list.cpp notice.cpp

OBJS	= ${SRCS:.cpp=.o}

CXX		= c++

CXXFLAGS	=  -Werror -Wextra -Wall -std=c++98

RM		= rm -f

all:	${NAME}

.cpp.o:	${CXX} ${CXXFLAGS} -I -c $< -o ${<:.cpp=.o}

${NAME}:	${OBJS}
		$(CXX) ${CXXFLAGS} ${OBJS} -o $(NAME)

clean:		
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re .c.o