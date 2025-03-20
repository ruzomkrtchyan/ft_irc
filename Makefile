NAME		= ircserv
SRC_DIR		= src
SRC			= $(wildcard $(SRC_DIR)/*.cpp)

OBJ_DIR		= obj
OBJ			= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

CC			= g++
CFLAGS		= -Wall -Wextra -Werror -std=c++98 $(INCLUDES)
INCLUDES	= -Iincs
MK			= mkdir -p

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR) :
		@$(MK) $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(HEADERS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	@rm -f $(OBJ)
	@rm -rf $(OBJ_DIR)
	
fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re