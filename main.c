#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Deklarace konstant pro velikost bufferu
#define MAX_INPUT_LENGTH 1024

/**
 * @brief Safely reads a line of text from standard input.
 * * This function dynamically allocates memory for the input string, 
 * ensuring it can handle strings of various lengths up to MAX_INPUT_LENGTH.
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
 * @brief Safely allocates a 2D matrix.
 *
 * This utility function handles the dynamic allocation of a 2D integer array,
 * including proper error checking and cleanup.
 *
 * @param rows The number of rows to allocate.
 * @param cols The number of columns to allocate.
 * @return A pointer to the allocated matrix, or NULL on failure.
 */
int** allocate_matrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    if (!matrix) {
        perror("Memory allocation error for matrix rows");
        return NULL;
    }
    for (int i = 0; i < rows; ++i) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if (!matrix[i]) {
            // Clean up previously allocated rows.
            for (int j = 0; j < i; ++j) {
                free(matrix[j]);
            }
            free(matrix);
            perror("Memory allocation error for matrix columns");
            return NULL;
        }
    }
    return matrix;
}

/**
 * @brief Safely frees a 2D matrix.
 *
 * This utility function deallocates all memory associated with a 2D integer array.
 *
 * @param matrix The matrix to free.
 * @param rows The number of rows in the matrix.
 */
void free_matrix(int** matrix, int rows) {
    if (matrix) {
        for (int i = 0; i < rows; ++i) {
            free(matrix[i]);
        }
        free(matrix);
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
    
    // Allocate the LCS matrix.
    int** lcs_matrix = allocate_matrix(m + 1, n + 1);
    if (!lcs_matrix) {
        free(s1);
        free(s2);
        return 1;
    }

    // Calculate the length of the LCS and fill the matrix.
    int lcs_length_val = calculate_lcs_length(s1, s2, lcs_matrix);
    if (lcs_length_val < 0) {
        free_matrix(lcs_matrix, m + 1);
        free(s1);
        free(s2);
        return 1;
    }
    
    printf("\nThe length of the longest common subsequence is: %d\n", lcs_length_val);
    print_lcs(s1, s2, m, n, lcs_matrix);

    // Clean up all allocated memory.
    free_matrix(lcs_matrix, m + 1);
    free(s1);
    free(s2);

    // Wait for the user to press Enter before exiting.
    printf("\nPress Enter to exit the program...");
    
    // Wait for a single Enter press. The input buffer is already clear from fgets().
    getchar();

    return 0;
}