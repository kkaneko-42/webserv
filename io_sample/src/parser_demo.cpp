#include <string>
#include <iostream>
#include <exception>
#include <cctype>

int expr(const std::string& s, size_t& i);

int number(const std::string& s, size_t& i) {
    int val = 0;
    while (i < s.size() && isdigit(s[i])) {
        val = val * 10 + (s[i] - '0');
        ++i;
    }

    return (val);
}

int factor(const std::string& s, size_t& i) {
    if (isdigit(s[i])) {
        return (number(s, i));
    }
    ++i;
    int val = expr(s, i);
    ++i;
    return (val);
}

int term(const std::string& s, size_t& i) {
    int val = factor(s, i);
    while (i < s.size() && (s[i] == '*' || s[i] == '/')) {
        char op = s[i++];

        int val2 = factor(s, i);
        if (op == '*') {
            val *= val2;
        } else {
            val /= val2;
        }
    }

    return (val);
}

int expr(const std::string& s, size_t& i) {
    int val = term(s, i);
    while (i < s.size() && (s[i] == '+' || s[i] == '-')) {
        char op = s[i++];

        int val2 = term(s, i);
        if (op == '+') {
            val += val2;
        } else {
            val -= val2;
        }
    }

    return (val);
}

int main(int ac, char **av) {
    if (ac < 2) {
        return (1);
    }

    std::string input = av[1];
    size_t i = 0;
    try {
        std::cout << "res: " << expr(input, i) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return (0);
}

