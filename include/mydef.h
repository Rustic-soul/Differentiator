#define uchar_t   unsigned char
#define uint_t    unsigned int
#define lint_t    long int
#define ulint_t   unsigned long int
#define llint_t   long long int
#define ullint_t  unsigned long long int
#define ldouble_t long double

#ifdef INFO
    #define PRINT_INFO(str ...) printf(str);
#else
    #define PRINT_INFO(str ...) {};
#endif

#ifdef DEBUG
    #define PRINT_DEBUG(str ...) fprintf(stderr, "DEBUG: " str);
#else
    #define PRINT_DEBUG(str ...) {};
#endif

#define PRINT_ERROR(str ...)                                                                 \
{                                                                                            \
    fprintf(stderr, "ERROR: " str);                                                          \
    fprintf(stderr, "FILE: %s; FUNC: %s; LINE: %d; \n", __FILE__, __func__, __LINE__);       \
}
