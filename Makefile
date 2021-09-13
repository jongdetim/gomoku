CC = clang++
NAME = gomoku
SRCDIR = app/src/
SRC = algorithm
OFILES = $(addprefix $(SRCDIR).objects/, $(SRC:%=%.o))
FLAGS = --std=c++17

all: $(NAME)

$(NAME): $(OFILES)
	@$(CC) -o app/$(NAME) $(FLAGS) $(OFILES)
	@echo "compiled $(NAME)"

$(SRCDIR).objects/%.o: $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CC) -o $@ -c $< $(FLAGS)
	@echo "+ $@"

clean:
	@rm -rf $(SRCDIR).objects

fclean: clean
	@rm -f app/$(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all
