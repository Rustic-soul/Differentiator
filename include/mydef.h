#define uchar_t   unsigned char
#define uint_t    unsigned int
#define lint_t    long int
#define ulint_t   unsigned long int
#define llint_t   long long int
#define ullint_t  unsigned long long int
#define ldouble_t long double

#define SIZEOF_ARR(a) (sizeof(a) / sizeof(*a))


#ifdef INFO
    #define PRINT_INFO(...) printf("[INFO] " __VA_ARGS__)
#else
    #define PRINT_INFO(...)
#endif


#ifdef DEBUG
    #define PRINT_DEBUG(...) fprintf(stderr, "[DEBUG] " __VA_ARGS__)
#else
    #define PRINT_DEBUG(...)
#endif


#define PRINT_ERROR(...)                                                                     \
{                                                                                            \
    fprintf(stderr, "[ERROR] " __VA_ARGS__);                                                 \
    fprintf(stderr, "FILE: %s; FUNC: %s; LINE: %d; \n", __FILE__, __func__, __LINE__);       \
}
