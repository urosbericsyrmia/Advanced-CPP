#include <iostream>
#include <vector>
#include <type_traits>
#include <cstring>

template <typename T>
T sum(T val) {
    return val;
}

template <typename T, typename... Args>
auto sum(T val, Args... args) {
    return val + sum(args...);
}

template <typename T, typename U>
auto min(T a, U b) {
    return a < b ? a : b;
}

template <typename T, typename... Args>
auto min(T val, Args... args) {
    auto result = min(args...);
    return val < result ? val : result;
}

enum class Printf_State { NORMAL = 0, PERCENT = 1};

std::vector<char> get_types(const char* s) {    
    std::vector<char> types;
    Printf_State state = Printf_State::NORMAL;
    
    for (int i = 0; i < std::strlen(s); i++) {
        if (s[i] == '%') {
            if (state == Printf_State::NORMAL) {
                state = Printf_State::PERCENT;
            }
            else if (state == Printf_State::PERCENT) {
                state = Printf_State::NORMAL;
            }
        }
        else if (s[i] == 'd' || s[i] == 'f' || s[i] == 's') {
            if (state == Printf_State::PERCENT) {
                types.push_back(s[i]);
                state = Printf_State::NORMAL;
            }
        }
        else if (s[i] != '\\') {
            state = Printf_State::NORMAL;
        }
    }

    return types;
}

template <typename T>
bool check_types(const char& c, const T& elem) {
    if (c == 'd' && std::is_integral<T>::value) return true;
    if (c == 'f' && std::is_floating_point<T>::value) return true;
    if (c == 's' && std::is_same <const char*, T>::value) return true;
    return false;
}

bool check_types(const std::vector<char>& types, const int& idx) {
    return true;
}

template <typename T>
bool check_types(const std::vector<char>& types, const int& idx, const T& elem) {
    return check_types(types[idx], elem);
}

template <typename T, typename... Args>
bool check_types(const std::vector<char>& types, const int& idx, const T& elem, const Args&... args) {
    return check_types(types[idx], elem) && check_types(types, idx + 1, args...);
}

std::string get_string_value(const char* elem) {
    return elem;
}

template <typename T>
std::string get_string_value(T elem) {
    return std::to_string(elem);
}

std::string get_output(const char* s, int idx) {
    std::string return_value = "";
    for (int i = idx; i < std::strlen(s); i++) {
        return_value += s[i];
    }
    return return_value;
}

template <typename T>
std::string get_output(const char* s, int idx, T elem) {
    std::string return_value = "";
    Printf_State state = Printf_State::NORMAL;

    int i = idx;
    for (; i < std::strlen(s); i++) {
        if (s[i] == '%') {
            if (state == Printf_State::NORMAL) {
                state = Printf_State::PERCENT;
            }
            else if (state == Printf_State::PERCENT) {
                return_value += '%';
                state = Printf_State::NORMAL;
            }
        }
        else if (s[i] == 'd' || s[i] == 'f' || s[i] == 's') {
            if (state == Printf_State::PERCENT) {
                return_value += get_string_value(elem);
                break;
            }
            else {
                return_value += s[i];
            }
        }
        else if (s[i] != '\\') {
            if (state == Printf_State::PERCENT) {
                return_value += '%';
            }
            return_value += s[i];
            state = Printf_State::NORMAL;
        }
    }

    return return_value + get_output(s, i + 1);
}

template <typename T, typename... Args>
std::string get_output(const char* s, int idx, T elem, Args... args) {
    std::string return_value = "";
    Printf_State state = Printf_State::NORMAL;

    int i = idx;
    for (; i < std::strlen(s); i++) {
        if (s[i] == '%') {
            if (state == Printf_State::NORMAL) {
                state = Printf_State::PERCENT;
            }
            else if (state == Printf_State::PERCENT) {
                return_value += '%';
                state = Printf_State::NORMAL;
            }
        }
        else if (s[i] == 'd' || s[i] == 'f' || s[i] == 's') {
            if (state == Printf_State::PERCENT) {
                return_value += get_string_value(elem);
                break;
            }
            else {
                return_value += s[i];
            }
        }
        else if (s[i] != '\\') {
            if (state == Printf_State::PERCENT) {
                return_value += '%';
            }
            return_value += s[i];
            state = Printf_State::NORMAL;
        }
    }

    return return_value + get_output(s, i + 1, args...);
}

template <typename... Args>
auto my_printf(const char* s, Args... args) {
    std::vector <char> types = get_types(s);

    if (types.size() != sizeof...(args)) {
        std::cout << "Invalid number of arguments" << std::endl;
        return;
    }

    if (!check_types(types, 0, args...)) {
        std::cout << "Incorrect type" << std::endl;
        return;
    }

    std::string output = get_output(s, 0, args...);
    std::cout << output;
}

using namespace std;

int main()
{
	// sum = 17.4
	cout << sum(2, 1.8, 3.8, 4, 5.8) << endl;
	// sum = 17.4
	cout << sum(1.8, 2, 3.8, 4, 5.8) << endl;
	
	// min = 1.1
	cout << min(2.2, 1.1, 4, 4, 58) << endl;
	// min = -58
	cout << min(2.2, 1.1, 4, 4, -58) << endl;

	// Test #1: OK
	my_printf("Simple check\nNew line\nAgain new line\n");
	// Test #2: Invalid number of arguments
	my_printf(" text %s, integral value %d than something %d\n", "SUCCESSFULLY", 85);
	// Test #3: Incorrect type / Invalid number of arguments
	my_printf("Some text %s, some floating point number %f again something %k\n", "45", 89.7, 56);
	// Test #4: OK
	my_printf("Some text %s, some floating point number %f again something %d\n", "45", 89.7, 56);

	return 0;
}
