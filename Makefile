# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svidot <svidot@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/28 13:41:27 by seblin            #+#    #+#              #
#    Updated: 2024/10/17 12:48:34 by svidot           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CXX = c++

HDRDIRS := $(shell find . \( -name '*.h' -o -name '*.hpp' -o -name '*.tpp' \) \
	-exec dirname {} \; | sort -u)
SRC_DIR = .
OBJ_DIR = objects
CFLAGS = $(HDRFLAGS) -g -Wall -Wextra -Werror -Wconversion -std=c++98
LDFLAGS = 

HDRFLAGS := $(foreach dir, $(HDRDIRS), -I$(dir))
HDR = $(shell find . \( -name "*.hpp" -o -name "*.h" -o -name "*.tpp" \))
SRC = $(shell find . -name "*.cpp" | sed 's|^\./||')
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean fclean re intro l newline backline emoticon nof

all: design emoticon $(NAME) 

nof:
	@$(eval CFLAGS = $(HDRFLAGS))
	@:
	
TOG = 0

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(HDR)
	@if [ $(TOG) -eq 0 ]; then \
		echo "\033[0;32m compiling...        🚀 "; \
	fi;	
	$(eval TOG=1)
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $< -c -o $@

$(NAME) : $(OBJ)
	@echo -n "\033[?25l"
	@$(MAKE) -s backline
	@echo "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b compiled            ✅\033[0m"	
	@sleep .2
	@echo "\033[0;36m linking...          🚀 "
	@sleep .2
	@$(MAKE) -s backline
	@$(CXX) $(OBJ) $(LDFLAGS) -o $@
	@echo "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b linked              ☑️\n\033[0m"
	@echo -n "\033[?25h"

emoticon:
	@echo "\n 💗 😀 😃 😍\n"
	@mkdir -p $(OBJ_DIR)

newline: 
	@echo ""

backline: 
	@echo "\033[A\033[A"

design:
	@echo "\e[8;50;120t"
	@echo "\033[0;32m"
	@./makescript.sh
	@echo "					the HardTeam Compagny Copyright\033[0m"

clean:
	@echo "\n ▫️  cleanning $(NAME) objects 🧻"
	@rm -rf $(OBJ_DIR) 
	@$(MAKE) -s newline	

fclean:
	@echo "\n ▫️  cleanning $(NAME) exec 🚽" 
	@rm -f $(NAME)	
	@$(MAKE) -s clean	

re: fclean backline all
