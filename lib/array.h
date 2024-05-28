#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <unistd.h>

/**
 * @brief Structure of array_t
 * 
 * @details The structure describes an array of characters, which is stored in the 
 *           memory, allocated by the function ctor_array. The member array contains
 *           the array of characters, and the member size_array contains the size of
 *           the array.
 */
typedef struct Array_t {
    char*    array;    /**< Array of characters */
    ssize_t  size_array; /**< Size of the array */
} array_t;

/**
 * @brief Creates an array from a file 
 * 
 * @param[in] fp - File pointer to the file, from which an array is created
 * 
 * @return A pointer to the array
 */
array_t* ctor_array (FILE*    fp);

/**
 * @brief Deletes an array
 * 
 * @param[in] arr - Pointer to the array to be deleted
 * 
 * @return 0 if the array was deleted successfully, or -1 if there was an error
 */
int      dtor_array (array_t* arr);

/**
 * @brief Calculates the size of a file
 * 
 * @param[in] fp - File pointer to the file, from which the size is calculated
 * 
 * @return The size of the file
 */
ssize_t calculate_size_file(FILE* fp);

#endif