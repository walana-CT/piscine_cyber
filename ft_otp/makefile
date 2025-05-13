# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rficht <rficht@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/20 09:23:49 by rficht            #+#    #+#              #
#    Updated: 2025/05/13 12:00:00 by rficht           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BOLD	:= \033[1m
BLACK	:= \033[30;1m
RED		:= \033[31;1m
GREEN	:= \033[32;1m
YELLOW	:= \033[33;1m
BLUE	:= \033[34;1m
MAGENTA	:= \033[35;1m
CYAN	:= \033[36;1m
WHITE	:= \033[37;1m
RESET	:= \033[0m

SHELL = /bin/sh
NAME = ft_otp

FLAGS = -Wall -Wextra -Werror
CFLAG = $(FLAGS)  $(INCLUDES) -fsanitize=address -static-libasan
GFLAG = -g3
CC = g++

FILES +=	crypto.cpp\
			debug.cpp\
			hmac_sha1.cpp\
			hotp.cpp\
			main.cpp

# Directory for object files
OBJ_DIR = objects

# Create a list of .o files in the object directory
OBJ = $(addprefix $(OBJ_DIR)/, $(FILES:.cpp=.o))

# Default rule
all: $(NAME)

# Create object directory if needed
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compile .cpp -> .o inside objects/
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@printf '$(GREEN)Compiling: $(RESET)$@\n'
	@$(CC) $(CFLAG) -o $@ -c $<

# Link the final binary
$(NAME): $(OBJ)
	@printf '$(RED)Linking: $(RESET)$@\n'
	$(CC) $(CFLAG) $(GFLAG) -o $(NAME) $(OBJ);

clean:
	@printf ' -- cleaning objects $(NAME) -- \n'
	@rm -rf $(OBJ_DIR)

fclean: clean
	@printf ' -- cleaning program $(NAME) -- \n'
	@rm -f $(NAME)

re: fclean all