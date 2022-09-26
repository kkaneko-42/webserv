all: mini_nginx echo_server

mini_nginx: src/main.cpp
	c++ $^ -o $@

echo_server: src/echo_server.cpp
	c++ $^ -o $@

fclean:
	rm -f mini_nginx echo_server

re: fclean all

.PHONY: all fclean re
