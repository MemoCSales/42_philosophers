NAME	= philo

OBJ_DIR	= obj/
MAKEFLAGS += --no-print-directory
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -g -lpthread
SRC =	main.c \
		utils.c \
		time.c

all: $(NAME)

OBJ = $(patsubst %.c, $(OBJ_DIR)%.o, $(SRC))

$(NAME): $(OBJ)
			@echo "$(YELLOW)Calling the philosophers$(DEFAULT)"
			@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)%.o: %.c
			@mkdir -p $(@D)
			@$(CC) $(CFLAGS) -c $< -o $@
			@printf "$(YELLOW).$(DEFAULT)"

clean:
		@rm -rf $(OBJ_DIR)
		@echo "$(GREEN)┌──────────────────────────────────────────────────────────────────────────┐$(DEFAULT)"
		@echo "$(GREEN)│                          Object files deleted!                           │$(DEFAULT)"

fclean: clean
		@rm -f ./$(NAME)
		@echo "$(GREEN)│                          All  files   deleted!                           │$(DEFAULT)"
		@echo "$(GREEN)└──────────────────────────────────────────────────────────────────────────┘$(DEFAULT)"

re: fclean all

norminette: 
		norminette $(SRC) philo.h

val:
		# valgrind --tool=helgrind ./philo 5 450 200 200 5
		valgrind --tool=helgrind ./philo 2 410 200 200
		

.PHONY: all clean fclean re

LIGHT_RED = \033[1;31m
RED = \033[0;31m
DARK_RED = \033[0;35m
GREEN = \033[1;32m
YELLOW = \033[1;33m
LIGHT_CYAN = \033[1;36m
CYAN = \033[0;36m
LIGHT_BLUE = \033[1;34m
DARK_BLUE = \033[0;34m
DEFAULT = \033[0m

# gdb --args program_name arg1 arg2 arg3