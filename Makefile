# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: root <root@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/31 17:40:12 by root              #+#    #+#              #
#    Updated: 2025/08/25 13:28:04 by root             ###   ########.fr        #
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
CPPFLAGS    = -I$(INCDIR) -I$(LIBFTDIR)/include
LDFLAGS     =
LDLIBS      =

# Readline paths (ajustez selon votre système)
UNAME := $(shell uname -s)

ifeq ($(UNAME),Darwin)
    # macOS (Homebrew)
    # Essaye d'abord /opt/homebrew (Apple Silicon), puis /usr/local (Intel),
    # puis le prefix dynamique si 'brew' existe.
    ifneq ("$(wildcard /opt/homebrew/opt/readline)","")
        CPPFLAGS += -I/opt/homebrew/opt/readline/include
        LDFLAGS  += -L/opt/homebrew/opt/readline/lib
    else ifneq ("$(wildcard /usr/local/opt/readline)","")
        CPPFLAGS += -I/usr/local/opt/readline/include
        LDFLAGS  += -L/usr/local/opt/readline/lib
    else ifeq ($(shell command -v brew >/dev/null 2>&1 && echo yes),yes)
        READLINE_PREFIX := $(shell brew --prefix readline 2>/dev/null)
        ifneq ($(READLINE_PREFIX),)
            CPPFLAGS += -I$(READLINE_PREFIX)/include
            LDFLAGS  += -L$(READLINE_PREFIX)/lib
        endif
    endif
    # Sur macOS, -lreadline suffit en général
    LDLIBS += -lreadline
else
    # Linux — privilégie pkg-config
    ifeq ($(shell pkg-config --exists readline && echo yes),yes)
        CPPFLAGS += $(shell pkg-config --cflags readline)
        LDLIBS   += $(shell pkg-config --libs readline)
    else
        # Fallback : souvent besoin de ncurses/tinfo
        LDLIBS   += -lreadline -lncurses
    endif
endif

LDFLAGS += -L$(LIBFTDIR)
LDLIBS  += -lft

# --- Valgrind via Linux container (pour macOS) ---
VALGRIND_FLAGS = -s --leak-check=full --log-file=valgrind.log \
                 --show-leak-kinds=all --track-origins=yes --track-fds=all \
                 --default-suppressions=yes --suppressions=.valgrind_readline

VALGRIND_IMAGE ?= debian:stable-slim

DOCKER ?= docker
HAS_DOCKER := $(shell command -v $(DOCKER) >/dev/null 2>&1 && echo yes || echo no)

# Source files organized by modules
MAIN_SRCS	=	cleanup.c \
				init.c \
				main.c \
				shell_loop.c \

BUILTIN_SRCS = 	builtins/cd/cd.c \
				builtins/echo/echo_utils.c \
				builtins/echo/echo.c \
				builtins/env/env.c \
				builtins/exit/exit_utils.c \
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
				
EXEC_SRCS =		execution/exec/exec_single_cmd.c \
				execution/exec/exec_utils.c \
				execution/exec/exec.c \
				execution/exec/process_utils.c \
				execution/heredoc/heredoc_expand.c \
				execution/heredoc/heredoc.c \
				execution/heredoc/process_heredoc.c \
				execution/heredoc/quotes_heredoc.c \
				execution/heredoc/setup_heredoc.c \
				execution/pipe/pipe_process.c \
				execution/pipe/pipe.c \
				execution/redir/apply_redir.c \

EXPAND_SRCS =	expansion/expand_cmd_arg.c \
				expansion/expand_utils.c \
				expansion/expand.c \
				expansion/quotes_helpers.c \
				expansion/quotes_utils.c \
				expansion/quotes.c \

LEXER_SRCS =	lexer/lexer_utils.c \
				lexer/lexer.c \
				lexer/token_utils.c \

PARSER_SRCS	=	parser/check_token_syntax.c \
				parser/parser_args.c \
				parser/parser_assignments.c \
				parser/parser_redir.c \
				parser/parser_utils.c \
				parser/parser.c \

SIGNAL_SRCS = 	signals/signal_child.c \
				signals/signal_parent.c \
				signals/signals.c

STRUCT_SRCS =	structures/append_assignment.c \
				structures/assignment.c \
				structures/t_cmd_utils.c \
				structures/t_heredoc.c \
				structures/t_redir_utils.c \

UTILS_SRCS	= 	utils/check_args.c \
				utils/cmd_args_utils.c \
				utils/cmd_check.c \
				utils/cmd_path.c \
				utils/cmd_utils.c \
				utils/error.c \
				utils/fd_utils.c \
				utils/malloc.c \
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
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME) $(LDLIBS)
	@echo "$(GREEN)✅ $(NAME) compiled successfully!$(RESET)"

# Compile object files with automatic directory creation
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

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


# Exécution avec Valgrind
valgrind:
ifeq ($(UNAME),Darwin)
ifeq ($(HAS_DOCKER),yes)
	@echo "$(BLUE)[macOS] Running Valgrind inside $(DOCKER) ($(VALGRIND_IMAGE))...$(RESET)"
	@$(DOCKER) run --rm -it \
		-v "$$(pwd)":/work -w /work $(VALGRIND_IMAGE) \
		bash -lc 'apt-get update && apt-get install -y build-essential valgrind libreadline-dev pkg-config && \
		          make -s re && \
		          valgrind $(VALGRIND_FLAGS) ./$(NAME)'
else
	@echo "$(RED)Docker introuvable.$(RESET) Installe Docker Desktop (brew install --cask docker) ou Colima (brew install colima docker && colima start)."
	@echo "Ou bien lance:  make asan  (sanitizer natif macOS)."
	@false
endif
else
	@echo "$(BLUE)[Linux] Running Valgrind natively...$(RESET)"
	@valgrind $(VALGRIND_FLAGS) ./$(NAME)
endif

# Check for norm errors (if norminette is installed)
norm:
	@echo "$(PURPLE)Checking norminette...$(RESET)"
	@norminette $(SRCDIR) $(INCDIR) $(LIBFTDIR) 2>/dev/null || echo "$(YELLOW)norminette not found or errors detected$(RESET)"

# Phony targets
.PHONY: all clean fclean re debug test_compile test_libft norm setup help show install_readline debug_vars
