#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Constant for the buffer size
#define MAX_INPUT_LENGTH 1024

// Function prototypes for clarity
int** allocate_matrix_single_block(int rows, int cols);
void free_matrix_single_block(int** matrix);

/**
 * @brief Safely reads a line of text from standard input.
 *
 * This function uses a fixed-size buffer to read input and then dynamically
 * allocates memory to store the string. It ensures proper null termination
 * and handles potential input errors.
 *
 * @return A dynamically allocated string read from standard input, or NULL on error.
 */
char* read_string() {
    char buffer[MAX_INPUT_LENGTH];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return NULL;
    }
    // Remove the newline character if it exists
    buffer[strcspn(buffer, "\n")] = '\0';

    char* str = (char*)malloc(strlen(buffer) + 1);
    if (str == NULL) {
        perror("Memory allocation error for string");
        return NULL;
    }
    strcpy(str, buffer);
    return str;
}

/**
 * @brief Calculates the length of the Longest Common Subsequence (LCS) of two strings.
 *
 * This function uses a dynamic programming approach to fill a matrix representing the lengths
 * of common subsequences. It modifies the provided matrix and returns the total length of the LCS.
 *
 * @param s1 The first input string.
 * @param s2 The second input string.
 * @param matrix A 2D integer array (matrix) to store the LCS lengths.
 * @return The length of the LCS. Returns -1 on error.
 */
int calculate_lcs_length(const char* s1, const char* s2, int** matrix) {
    if (!s1 || !s2 || !matrix) {
        fprintf(stderr, "Error: Invalid input strings or matrix.\n");
        return -1;
    }

    int m = strlen(s1);
    int n = strlen(s2);

    // Initialize the first row and column with zeros.
    for (int i = 0; i <= m; ++i) {
        matrix[i][0] = 0;
    }
    for (int j = 0; j <= n; ++j) {
        matrix[0][j] = 0;
    }

    // Fill the matrix based on character comparison.
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                // If characters match, add 1 to the value from the diagonal cell.
                matrix[i][j] = 1 + matrix[i - 1][j - 1];
            } else {
                // If characters don't match, take the maximum value from the cell above or to the left.
                matrix[i][j] = (matrix[i - 1][j] > matrix[i][j - 1]) ? matrix[i - 1][j] : matrix[i][j - 1];
            }
        }
    }

    return matrix[m][n];
}

/**
 * @brief Reconstructs and prints one of the Longest Common Subsequences.
 *
 * This function backtracks through the filled matrix from the bottom-right corner
 * to reconstruct the LCS string.
 *
 * @param s1 The first input string.
 * @param s2 The second input string.
 * @param m The length of the first string.
 * @param n The length of the second string.
 * @param matrix The 2D integer matrix containing LCS lengths.
 */
void print_lcs(const char* s1, const char* s2, int m, int n, int** matrix) {
    int i = m;
    int j = n;
    int length = matrix[m][n];

    if (length == 0) {
        printf("The longest common subsequence is: (empty)\n");
        return;
    }

    char* lcs = (char*)malloc((length + 1) * sizeof(char));
    if (!lcs) {
        perror("Memory allocation error for LCS");
        return;
    }
    lcs[length] = '\0';

    int index = length - 1;

    // Traverse the matrix from the bottom-right corner.
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            // If characters match, it's part of the LCS. Add the character and move diagonally.
            lcs[index] = s1[i - 1];
            i--;
            j--;
            index--;
        } else if (matrix[i - 1][j] > matrix[i][j - 1]) {
            // Move up if the value above is greater.
            i--;
        } else {
            // Otherwise, move left.
            j--;
        }
    }
    printf("The longest common subsequence is: %s\n", lcs);
    free(lcs);
}

/**
 * @brief Safely allocates a 2D matrix in a single, contiguous memory block.
 *
 * This utility function allocates a single memory block for all matrix data
 * and then sets up pointers to point to the start of each row. This is more
 * efficient than allocating each row separately.
 *
 * @param rows The number of rows to allocate.
 * @param cols The number of columns to allocate.
 * @return A pointer to the allocated matrix, or NULL on failure.
 */
int** allocate_matrix_single_block(int rows, int cols) {
    // Allocate memory for the row pointers
    int** matrix = (int**)malloc(rows * sizeof(int*));
    if (!matrix) {
        perror("Memory allocation error for matrix rows");
        return NULL;
    }
    
    // Allocate a single block of memory for all the data
    int* data = (int*)malloc(rows * cols * sizeof(int));
    if (!data) {
        perror("Memory allocation error for matrix data");
        free(matrix); // Cleanup the pointer array if data allocation fails
        return NULL;
    }

    // Set up the row pointers to point into the data block
    for (int i = 0; i < rows; i++) {
        matrix[i] = data + i * cols;
    }

    return matrix;
}

/**
 * @brief Safely frees a 2D matrix allocated as a single contiguous block.
 *
 * This utility function deallocates both the data block and the pointer array,
 * ensuring all memory associated with the matrix is released.
 *
 * @param matrix The matrix to free.
 */
void free_matrix_single_block(int** matrix) {
    if (matrix) {
        free(matrix[0]); // Free the single data block
        free(matrix);    // Free the array of pointers
    }
}

int main() {
    printf("Enter the first string: ");
    char* s1 = read_string();
    if (!s1) {
        return 1;
    }

    printf("Enter the second string: ");
    char* s2 = read_string();
    if (!s2) {
        free(s1);
        return 1;
    }

    int m = strlen(s1);
    int n = strlen(s2);
    
    // Allocate the LCS matrix using the more efficient single-block method.
    int** lcs_matrix = allocate_matrix_single_block(m + 1, n + 1);
    if (!lcs_matrix) {
        free(s1);
        free(s2);
        return 1;
    }

    // Calculate the length of the LCS and fill the matrix.
    int lcs_length_val = calculate_lcs_length(s1, s2, lcs_matrix);
    if (lcs_length_val < 0) {
        free_matrix_single_block(lcs_matrix);
        free(s1);
        free(s2);
        return 1;
    }
    
    printf("\nThe length of the longest common subsequence is: %d\n", lcs_length_val);
    print_lcs(s1, s2, m, n, lcs_matrix);

    // Clean up all dynamically allocated memory.
    free_matrix_single_block(lcs_matrix);
    free(s1);
    free(s2);

    // Wait for the user to press Enter before exiting.
    printf("\nPress Enter to exit the program...");
    
    // Wait for a single Enter press. The input buffer is already clear from fgets().
    getchar();

    return 0;
}