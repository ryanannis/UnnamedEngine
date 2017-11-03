#pragma once
#include <array>
#include <type_traits>
#include <utility>

template <typename... T>
constexpr std::array<std::decay<typename std::common_type<T...>::type>, sizeof...(T)> 
makeArray(T&&... values)
{
	return(
		std::array<std::decay<typename std::common_type<T...>::type>, sizeof...(T)>
		{std::forward<T>(values)...}
	);
}

#define IGNORE_ASSIGN_SINGLE(expression) (ignore_assign)expression,
#define IGNORE_ASSIGN(...) MAP(IGNORE_ASSIGN_SINGLE, __VA_ARGS__)
#define STRINGIZE_SINGLE(expression) #expression,
#define STRINGIZE(...) MAP(STRINGIZE_SINGLE, __VA_ARGS__)

struct ignore_assign {
	ignore_assign(int value) : _value(value) { }
	operator int() const { return _value; }
	const ignore_assign& operator =(int dummy) { return *this; }
	int _value;
};

// from https://www.codeproject.com/Articles/1002895/Clean-Reflective-Enums-Cplusplus-Enum-to-String-wi

#include <cstddef>
#include <cstring>


#define MAP(macro, ...) \
    IDENTITY( \
        APPLY(CHOOSE_MAP_START, COUNT(__VA_ARGS__)) \
            (macro, __VA_ARGS__))

#define CHOOSE_MAP_START(count) MAP ## count

#define APPLY(macro, ...) IDENTITY(macro(__VA_ARGS__))

// Needed to expand __VA_ARGS__ "eagerly" on the MSVC preprocessor.
#define IDENTITY(x) x

#define MAP1(m, x)      m(x)
#define MAP2(m, x, ...) m(x) IDENTITY(MAP1(m, __VA_ARGS__))
#define MAP3(m, x, ...) m(x) IDENTITY(MAP2(m, __VA_ARGS__))
#define MAP4(m, x, ...) m(x) IDENTITY(MAP3(m, __VA_ARGS__))
#define MAP5(m, x, ...) m(x) IDENTITY(MAP4(m, __VA_ARGS__))
#define MAP6(m, x, ...) m(x) IDENTITY(MAP5(m, __VA_ARGS__))
#define MAP7(m, x, ...) m(x) IDENTITY(MAP6(m, __VA_ARGS__))
#define MAP8(m, x, ...) m(x) IDENTITY(MAP7(m, __VA_ARGS__))

#define EVALUATE_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, count, ...) count

#define COUNT(...) \
    IDENTITY(EVALUATE_COUNT(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1))


struct ignore_assign {
	ignore_assign(int value) : _value(value) { }
	operator int() const { return _value; }

	const ignore_assign& operator =(int dummy) { return *this; }

	int _value;
};

#define IGNORE_ASSIGN_SINGLE(expression) (ignore_assign)expression,
#define IGNORE_ASSIGN(...) IDENTITY(MAP(IGNORE_ASSIGN_SINGLE, __VA_ARGS__))

#define STRINGIZE_SINGLE(expression) #expression,
#define STRINGIZE(...) IDENTITY(MAP(STRINGIZE_SINGLE, __VA_ARGS__))


#define ENUM(EnumName, ...)                                            \
struct EnumName {                                                      \
    enum _enumerated { __VA_ARGS__ };                                  \
                                                                       \
    _enumerated     _value;                                            \
                                                                       \
    EnumName(_enumerated value) : _value(value) { }                    \
    operator _enumerated() const { return _value; }                    \
                                                                       \
    const char* _to_string() const                                     \
    {                                                                  \
        for (size_t index = 0; index < _count; ++index) {              \
            if (_values()[index] == _value)                            \
                return _names()[index];                                \
        }                                                              \
                                                                       \
        return NULL;                                                   \
    }                                                                  \
                                                                       \
    static const size_t _count = IDENTITY(COUNT(__VA_ARGS__));         \
                                                                       \
    static const int* _values()                                        \
    {                                                                  \
        static const int values[] =                                    \
            { IDENTITY(IGNORE_ASSIGN(__VA_ARGS__)) };                  \
        return values;                                                 \
    }                                                                  \
                                                                       \
    static const char* const* _names()                                 \
    {                                                                  \
        static const char* const    raw_names[] =                      \
            { IDENTITY(STRINGIZE(__VA_ARGS__)) };                      \
                                                                       \
        static char*                processed_names[_count];           \
        static bool                 initialized = false;               \
                                                                       \
        if (!initialized) {                                            \
            for (size_t index = 0; index < _count; ++index) {          \
                size_t length =                                        \
                    std::strcspn(raw_names[index], " =\t\n\r");        \
                                                                       \
                processed_names[index] = new char[length + 1];         \
                                                                       \
                std::strncpy(                                          \
                    processed_names[index], raw_names[index], length); \
                processed_names[index][length] = '\0';                 \
            }                                                          \
        }                                                              \
                                                                       \
        return processed_names;                                        \
    }                                                                  \
};

#include <iostream>

ENUM(ColorChannel, Red = 1, Green, Blue);

int main()
{
	ColorChannel    channel = ColorChannel::Green;
	std::cout << channel._to_string() << std::endl;

	switch (channel) {
	case ColorChannel::Red:   return 0;
	case ColorChannel::Green: return 1;
	case ColorChannel::Blue:  return 2;
	}
}