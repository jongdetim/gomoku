CC = clang++
NAME = gomoku
SRCDIR = app/src/
INCLUDES = app/includes
SRC = Board
OFILES = $(addprefix $(SRCDIR).objects/, $(SRC:%=%.o))
FLAGS = -Wall -Werror -Wextra --std=c++11

all: $(NAME)

$(NAME): $(OFILES)
	@$(CC) -o app/$(NAME) $(FLAGS) $(OFILES) -I $(INCLUDES)
	@echo "compiled $(NAME)"

$(SRCDIR).objects/%.o: $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CC) -o $@ -c $< $(FLAGS) -I $(INCLUDES)
	@echo "+ $@"

clean:
	@rm -rf $(SRCDIR).objects

fclean: clean
	@rm -f app/$(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all
