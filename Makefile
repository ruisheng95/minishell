SRCS = \
main.c rl_redisplay_line.c \
lexer.c lexer2.c lexer3.c lexer4.c \
expansion.c expansion2.c \
execve.c execve2.c execve3.c \
cd.c echo.c env.c pwd.c export.c export2.c unset.c exit.c \
parsing_list.c parsing_list2.c parsing_list3.c parsing_list4.c \
utils.c utils2.c utils3.c \
command_list.c command_list2.c \
free.c remove_quotes.c 


OBJ = $(SRCS:.c=.o)

NAME = minishell

CC = gcc

CFLAGS = -Wall -Werror -Wextra

READ = -lreadline 

LIB = libft.a

LIBDIR = libft

#COLORS!
RED = \033[0;31m
GREEN = \033[0;32m
NC = \033[0m

all : $(NAME)

$(NAME): $(OBJ) $(LIB)
	@echo "${GREEN}compiling minishell...${NC}"
	@$(CC) $(OBJ) -lft -L. -o $(NAME) libft.a ${READ} 
	@echo "${GREEN}compile minishell success!!${NC}"
	@echo "${GREEN}------------------------------${NC}"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(LIB) :
	@echo "${GREEN}compiling libft...${NC}"
	@$(MAKE) -C $(LIBDIR)
	@mv $(LIBDIR)/$(LIB) .
	@echo "${GREEN}compile libft success!!${NC}"
	@echo "${GREEN}------------------------------${NC}"

clean :
	@echo "${GREEN}cleaning...${NC}"
	@rm -f ${OBJ}
	@rm -f ${BONUS_OBJ}
	@rm -f $(LIB)
	@$(MAKE) -C $(LIBDIR) clean
	@echo "${GREEN}clean success!!${NC}"
	@echo "${GREEN}------------------------------${NC}"

fclean : clean
	@rm -f $(NAME)
	@rm -f $(BONUS_NAME)

re: fclean all

bonus : fclean $(BONUS_NAME)

testlex:
	gcc expansion.c lexer.c lexer2.c lexer3.c libft.a expansion2.c lexer4.c remove_quotes.c

testlist:
	gcc expansion.c lexer.c lexer2.c parsing_list.c libft.a lexer3.c expansion2.c lexer4.c

testcl:
	gcc expansion.c lexer.c lexer2.c parsing_list.c command_list.c libft.a lexer3.c expansion2.c lexer4.c remove_quotes.c

test: re
	cd minishell_tester && ./tester

.PHONY: all clean fclean re leak bonus