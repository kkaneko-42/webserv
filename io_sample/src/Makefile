test: conf_parse_test req_parse_test

conf_parse:
	c++ parser.cpp Config.cpp lexer.cpp test_main.cpp -o $@

req_parse:
	c++ parse_request_test.cpp HttpRequest.cpp -o $@

conf_parse_test: conf_parse
	./$< ./conf/sample2.conf

req_parse_test: req_parse
	./$<

all: conf_parse req_parse

fclean:
	rm -f req_parse conf_parse

re: fclean all

.PHONY: all fclean re test
