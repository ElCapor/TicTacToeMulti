#define LoggerError(...) Logger::Error(__VA_ARGS__)

/**
 * Attempt to remove a key and warn you if it didnt work out
 */
#define ERASE_MAP_KEY(expression)                                              \
  if (expression != 1)                                                         \
  LoggerError("Erase failed for ", #expression)
/*
 * Warn if the expression has an error
 */
#define RESULT_CHECK(expression)                                               \
  if (expression.has_error())                                                  \
  LoggerError("ERROR : ", #expression)

#ifdef _MSC_VER // MSVC Compiler
  #define IGNORE_RETURN_TYPE_START  __pragma(warning(push)) __pragma(warning(disable: 4716))
  #define IGNORE_RETURN_TYPE_END    __pragma(warning(pop))
#elif defined(__GNUC__) || defined(__clang__) // GCC and Clang
  #define IGNORE_RETURN_TYPE_START  _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wreturn-type\"")
  #define IGNORE_RETURN_TYPE_END    _Pragma("GCC diagnostic pop")
#else
  #define IGNORE_RETURN_TYPE_START
  #define IGNORE_RETURN_TYPE_END
#endif