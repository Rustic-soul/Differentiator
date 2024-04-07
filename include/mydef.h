#ifdef INFO
    #define PRINT_INFO(...) printf("[INFO] " __VA_ARGS__)
#else
    #define PRINT_INFO(...)
#endif


#ifdef DEBUG
    #define PRINT_DEBUG(...) { fprintf(stderr, "[DEBUG] " __VA_ARGS__); }
#else
    #define PRINT_DEBUG(...) {}
#endif


#define PRINT_ERROR(...)                                                                     \
{                                                                                            \
    fprintf(stderr, "[ERROR] " __VA_ARGS__);                                                 \
    fprintf(stderr, "FILE: %s; FUNC: %s; LINE: %d; \n", __FILE__, __func__, __LINE__);       \
}

#define CHECK_OPEN_FILE(fp) if (fp == NULL) { PRINT_ERROR("File was not opened\n"); }