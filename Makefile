CXX = g++
CC = gcc
CXXFLAGS = -Wall -Wno-unused-function -Wconversion -std=c++23 -g -lsfml-graphics -lsfml-window -lsfml-system -fsanitize=address -fsanitize=undefined 
CFLAGS = -Wall -Wno-unused-function -Wconversion -std=c99 -g
LDFLAGS =
SNAKE_DEPS = src/snake.o src/utils.o src/game.o
INTERACTIVE_DEPS = src/interactive_game.o src/utils.o src/game.o
SNKAE_DEPS = src/main_snk.o src/map_control.o src/render.o src/game_control.o src/game.o src/utils.o

COLOR_GREEN =
COLOR_RESET =
ifeq (,${NO_COLOR})
	ifneq (,$(findstring xterm,${TERM}))
		override COLOR_GREEN := $(shell tput setaf 2)
		override COLOR_RESET := $(shell tput sgr0)
	endif
endif

interactive-snake: $(INTERACTIVE_DEPS)
	$(CXX) -o $@ $^ -pthread $(CXXFLAGS) $(LDFLAGS)

snk_gui:$(SNKAE_DEPS)
	$(CXX) -o $@ $^ -pthread $(CXXFLAGS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f interactive-snake snake unit-tests custom-tests unit-test-*.snk *.exe *.o src/*.o tests/*-out.snk
