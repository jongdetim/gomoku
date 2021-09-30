CC = clang++
NAME = gomoku
SRCDIR = src/
TARGETDIR = bin/
INCLUDES = includes
SRC = main algorithm Board heuristic misc test TranspositionTable
OFILES = $(addprefix $(SRCDIR).objects/, $(SRC:%=%.o))
FLAGS = -std=c++17 -Ofast

all: $(NAME)

$(NAME): $(OFILES)
	@mkdir -p $(TARGETDIR)
	@$(CC) -o $(TARGETDIR)$(NAME) $(FLAGS) $(OFILES) -I $(INCLUDES)
	@echo "compiled $(NAME)"

$(SRCDIR).objects/%.o: $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CC) -o $@ -c $< $(FLAGS) -I $(INCLUDES)
	@echo "+ $@"

clean:
	@rm -rf $(SRCDIR).objects

fclean: clean
	@rm -rf $(TARGETDIR)

re:
	@$(MAKE) fclean
	@$(MAKE) all
