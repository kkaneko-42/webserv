all: io

io:
	c++ src/*.cpp

fclean:
	rm a.out

re: fclean io

test: all
	bash test.sh

.PHONY: fclean
