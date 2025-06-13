# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: user <user@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/11 00:00:00 by user              #+#    #+#              #
#    Updated: 2025/06/11 00:00:00 by user             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME = philo

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
DEBUG_FLAGS = -g -fsanitize=thread
INCLUDE = -I includes

# Directories
SRC_DIR = src
INC_DIR = includes
OBJ_DIR = obj

# Source files
SRC_FILES = main.c \
			init.c \
			philosophers.c \
			monitor.c \
			utils.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Colors for pretty output
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[1;33m
BLUE = \033[0;34m
NC = \033[0m # No Color

# Default target
all: $(NAME)

# Create object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@printf "$(BLUE)Compiling$(NC) %-20s " $<
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@printf "$(GREEN)[OK]$(NC)\n"

# Link the program
$(NAME): $(OBJS)
	@printf "$(YELLOW)Linking$(NC) $(NAME)... "
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@printf "$(GREEN)[OK]$(NC)\n"
	@printf "$(GREEN)$(NAME) compiled successfully!$(NC)\n"

# Debug build with thread sanitizer
debug: CFLAGS += $(DEBUG_FLAGS)
debug: fclean $(NAME)
	@printf "$(YELLOW)Debug build with thread sanitizer enabled$(NC)\n"

# Test targets for common test cases
test: $(NAME)
	@printf "$(BLUE)Running basic tests...$(NC)\n"
	@printf "\n$(YELLOW)Test 1: Single philosopher (should die)$(NC)\n"
	./$(NAME) 1 800 200 200
	@printf "\n$(YELLOW)Test 2: 5 philosophers, standard timing$(NC)\n"
	timeout 10s ./$(NAME) 5 800 200 200 || true
	@printf "\n$(YELLOW)Test 3: 5 philosophers with meal limit$(NC)\n"
	./$(NAME) 5 800 200 200 7
	@printf "\n$(YELLOW)Test 4: Tight timing$(NC)\n"
	timeout 5s ./$(NAME) 4 410 200 200 || true

# Stress test
stress: $(NAME)
	@printf "$(BLUE)Running stress tests...$(NC)\n"
	@printf "\n$(YELLOW)Stress test: 100 philosophers$(NC)\n"
	timeout 10s ./$(NAME) 100 800 200 200 || true
	@printf "\n$(YELLOW)Stress test: Very tight timing$(NC)\n"
	timeout 5s ./$(NAME) 4 310 200 100 || true

# Memory leak check
valgrind: $(NAME)
	@printf "$(BLUE)Running valgrind memory check...$(NC)\n"
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./$(NAME) 5 800 200 200 7

# Thread safety check
helgrind: $(NAME)
	@printf "$(BLUE)Running helgrind thread safety check...$(NC)\n"
	valgrind --tool=helgrind ./$(NAME) 5 800 200 200 7

# Clean object files
clean:
	@if [ -d $(OBJ_DIR) ]; then \
		printf "$(RED)Cleaning$(NC) object files... "; \
		rm -rf $(OBJ_DIR); \
		printf "$(GREEN)[OK]$(NC)\n"; \
	fi

# Clean everything
fclean: clean
	@if [ -f $(NAME) ]; then \
		printf "$(RED)Removing$(NC) $(NAME)... "; \
		rm -f $(NAME); \
		printf "$(GREEN)[OK]$(NC)\n"; \
	fi

# Rebuild everything
re: fclean all

# Show help
help:
	@printf "$(BLUE)Available targets:$(NC)\n"
	@printf "  $(GREEN)all$(NC)      - Build the program\n"
	@printf "  $(GREEN)debug$(NC)    - Build with debug flags and thread sanitizer\n"
	@printf "  $(GREEN)test$(NC)     - Run basic functionality tests\n"
	@printf "  $(GREEN)stress$(NC)   - Run stress tests\n"
	@printf "  $(GREEN)valgrind$(NC) - Check for memory leaks\n"
	@printf "  $(GREEN)helgrind$(NC) - Check for thread safety issues\n"
	@printf "  $(GREEN)clean$(NC)    - Remove object files\n"
	@printf "  $(GREEN)fclean$(NC)   - Remove object files and executable\n"
	@printf "  $(GREEN)re$(NC)       - Rebuild everything\n"
	@printf "  $(GREEN)help$(NC)     - Show this help message\n"

# Ensure these targets don't conflict with files
.PHONY: all clean fclean re debug test stress valgrind helgrind help

# Show compilation progress
.SILENT:
