test: all
	./a.out ./conf/sample2.conf

all:
	c++ parser.cpp Config.cpp lexer.cpp test_main.cpp

fclean:
	rm -f a.out

re: fclean all

.PHONY: all fclean re
