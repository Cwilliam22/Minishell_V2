# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: root <root@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/31 17:40:12 by root              #+#    #+#              #
#    Updated: 2025/05/31 17:40:12 by root             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                               VARIABLES                                      #
# **************************************************************************** #

# Program name
NAME		= minishell

# Directories
SRCDIR		= src
OBJDIR		= obj
INCDIR		= include
LIBFTDIR	= libft

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3 -g

# Include paths
INCLUDES	= -I$(INCDIR) -I$(LIBFTDIR)/include

# Readline paths (ajustez selon votre système)
ifeq ($(shell uname), Darwin)
	# macOS avec Homebrew
	INCLUDES += -I$(HOME)/.brew/opt/readline/include
	LDFLAGS = -L$(HOME)/.brew/opt/readline/lib -L$(LIBFTDIR) -lreadline -lft
else ifeq ($(shell uname), Linux)
	# Linux standard
	LDFLAGS = -L$(LIBFTDIR) -lreadline -lft
else
	# WSL ou autres
	LDFLAGS = -L$(LIBFTDIR) -lreadline -lft
endif

# Source files organized by modules
MAIN_SRCS	=	cleanup.c \
				init.c \
				main.c \
				shell_loop.c \

BUILTIN_SRCS = 	builtins/cd/cd.c \
				builtins/echo/echo.c \
				builtins/env/env.c \
				builtins/exit/exit.c \
				builtins/export/export_assign.c \
				builtins/export/export_no_args.c \
				builtins/export/export.c \
				builtins/pwd/pwd.c \
				builtins/unset/unset.c \
				builtins/builtins.c \

DEBUG_SRCS = 	debug/debug_cmds.c \
				debug/debug_token.c \

ENV_SRCS =		environment/env_get.c \
				environment/env_set.c \
				environment/env_utils.c \
				environment/env_var_utils.c \
				environment/shell_lvl.c \
				
EXEC_SRCS =		execution/exec_externe.c \
				execution/exec_single_cmd.c \
				execution/exec_utils.c \
				execution/exec.c \
				execution/heredoc.c \
				execution/redir.c

EXPAND_SRCS =	expansion/expand_utils.c \
				expansion/expand.c \
				expansion/quotes_helpers.c \
				expansion/quotes_utils.c \
				expansion/quotes.c \

LEXER_SRCS =	lexer/lexer_utils.c \
				lexer/lexer.c \
				lexer/token_utils.c \

PARSER_SRCS	=	parser/check_token_syntax.c \
				parser/parser_args.c \
				parser/parser_redir.c \
				parser/parser_utils.c \
				parser/parser.c \

SIGNAL_SRCS = 

STRUCT_SRCS =	structures/append_assignment.c \
				structures/assignment.c \
				structures/t_cmd_utils.c \
				structures/t_redir_utils.c \

UTILS_SRCS	= 	utils/check_args.c \
				utils/cmd_args_utils.c \
				utils/cmd_check.c \
				utils/cmd_utils.c \
				utils/error.c \
				utils/fd_utils.c \
				utils/malloc.c \
				utils/path_utils.c \
				utils/shell_utils.c \
				utils/utils.c \

# All source files
SRCS		=	$(MAIN_SRCS) \
				$(BUILTIN_SRCS) \
				$(DEBUG_SRCS) \
				$(ENV_SRCS) \
				$(EXEC_SRCS) \
				$(EXPAND_SRCS) \
				$(LEXER_SRCS) \
				$(PARSER_SRCS) \
				$(SIGNAL_SRCS) \
				$(STRUCT_SRCS) \
				$(UTILS_SRCS) \
				$(TEST_SRCS)

# Object files
OBJS		= $(SRCS:%.c=$(OBJDIR)/%.o)

# Libft
LIBFT		= $(LIBFTDIR)/libft.a

# Colors for output
RED			= \033[0;31m
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
PURPLE		= \033[0;35m
CYAN		= \033[0;36m
WHITE		= \033[0;37m
RESET		= \033[0m

# **************************************************************************** #
#                                RULES                                         #
# **************************************************************************** #

# Default rule
all: $(NAME)

# Main target
$(NAME): $(LIBFT) $(OBJS)
	@echo "$(CYAN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) compiled successfully!$(RESET)"

# Compile object files with automatic directory creation
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile libft
$(LIBFT):
	@echo "$(PURPLE)Compiling libft...$(RESET)"
	@$(MAKE) -C $(LIBFTDIR) --no-print-directory
	@echo "$(GREEN)✅ libft compiled!$(RESET)"

# Clean object files
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)
	@$(MAKE) -C $(LIBFTDIR) clean --no-print-directory
	@echo "$(GREEN)✅ Object files cleaned!$(RESET)"

# Clean everything
fclean: clean
	@echo "$(RED)Cleaning executable...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFTDIR) fclean --no-print-directory
	@echo "$(GREEN)✅ Everything cleaned!$(RESET)"

# Rebuild everything
re: fclean all

# Debug version with additional flags
debug: CFLAGS += -fsanitize=address -DDEBUG
debug: $(NAME)
	@echo "$(BLUE)✅ Debug version compiled!$(RESET)"

# Exécution avec valgrind (Linux uniquement)
valgrind:
	@echo "Running with valgrind..."
	valgrind -s --leak-check=full --log-file=valgrind.log --show-leak-kinds=all --track-fds=all --track-origins=yes --default-suppressions=yes --suppressions=.valgrind_readline ./$(NAME)

# Check for norm errors (if norminette is installed)
norm:
	@echo "$(PURPLE)Checking norminette...$(RESET)"
	@norminette $(SRCDIR) $(INCDIR) $(LIBFTDIR) 2>/dev/null || echo "$(YELLOW)norminette not found or errors detected$(RESET)"

# Create project structure
setup:
	@echo "$(CYAN)Creating project structure...$(RESET)"
	@mkdir -p $(SRCDIR)/parser
	@mkdir -p $(SRCDIR)/executor
	@mkdir -p $(SRCDIR)/builtins
	@mkdir -p $(SRCDIR)/utils
	@mkdir -p $(SRCDIR)/signals
	@mkdir -p $(INCDIR)
	@mkdir -p $(OBJDIR)
	@echo "$(GREEN)✅ Project structure created!$(RESET)"

# Phony targets
.PHONY: all clean fclean re debug test_compile test_libft norm setup help show install_readline debug_vars
