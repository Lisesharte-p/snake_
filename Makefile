# 编译器定义
CXX = g++
CC = gcc

# 编译选项（仅编译，不含链接库）
CXXFLAGS = -Wall -Wno-unused-function -Wconversion -std=c++23 -g -O2
# 调试模式编译选项
DBGFLAGS = -Wall -Wno-unused-function -Wconversion -std=c++23 -g -fsanitize=address -fsanitize=undefined
# C语言编译选项
CFLAGS = -Wall -Wno-unused-function -Wconversion -std=c99 -g

# 链接路径（SFML安装路径，优先用/usr/local/lib，兼容x86_64路径）
LDFLAGS = -L/usr/local/lib -L/lib/x86_64-linux-gnu
# 链接库（仅链接阶段使用，顺序：graphics依赖window，window依赖system）
LDLIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -pthread -ludev -lX11 -lXrandr -lXcursor -lfreetype -lXi

# 目标文件依赖
SNAKE_DEPS = src/snake.o src/utils.o src/game.o
INTERACTIVE_DEPS = src/interactive_game.o src/utils.o src/game.o
SNKAE_DEPS = src/main_snk.o src/map_control.o src/render.o src/game_control.o src/game.o src/utils.o

# 颜色配置（保留原有逻辑）
COLOR_GREEN =
COLOR_RESET =
ifeq (,${NO_COLOR})
	ifneq (,$(findstring xterm,${TERM}))
		override COLOR_GREEN := $(shell tput setaf 2)
		override COLOR_RESET := $(shell tput sgr0)
	endif
endif

# 目标规则（链接阶段：编译选项 + 路径 + 库）
interactive-snake: $(INTERACTIVE_DEPS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

snk_gui_dbg: $(SNKAE_DEPS)
	$(CXX) -o $@ $^ $(DBGFLAGS) $(LDFLAGS) $(LDLIBS)

snk_gui: $(SNKAE_DEPS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

# 编译规则（仅编译，不含链接库）
%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# 清理规则
.PHONY: clean
clean:
	rm -f interactive-snake snake unit-tests custom-tests unit-test-*.snk *.exe *.o src/*.o tests/*-out.snk
