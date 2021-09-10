CC = clang++
NAME = gomoku
SRC = 
OFILES = $(addprefix .objects/, $(SRC:%=%.o))
FLAGS = -Wall -Werror -Wextra --std=c++11

all: $(NAME)

$(NAME): $(OFILES)
	@$(CC) -o $(NAME) $(FLAGS) $(OFILES)
	@echo "compiled $(NAME)"

.objects/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) -o $@ -c $< $(FLAGS)
	@echo "+ $@"

clean:
	@rm -rf .objects

fclean: clean
	@rm -f $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

