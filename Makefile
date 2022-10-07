NAME = webserv

CXX = c++
CXXFLAGS = -g -std=c++98 #-Wall -Wextra -Werror 
DEPFLAGS = -MMD -MP

SRCS = $(wildcard src/*.cpp src/*/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)
DEPS = $(SRCS:src/%.cpp=obj/%.d)

OBJS_DIR := $(sort $(dir $(OBJS)))
OBJS_DIR := $(addsuffix .keep, $(OBJS_DIR))

all: $(NAME)

-include $(DEPS)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

obj/%.o: src/%.cpp $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -o $@ -c $<

$(OBJS_DIR):
	mkdir -p $(@D)
	touch $@

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
