#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Funkce pro výpočet délky LCS a vyplnění matice
// Vrací délku LCS, matice je modifikována "side-effectem"
int lcs_length(const char* s1, const char* s2, int** matrix) {
    int m = strlen(s1);
    int n = strlen(s2);

    // Inicializace prvního řádku a sloupce nulami
    for (int i = 0; i <= m; i++) matrix[i][0] = 0;
    for (int j = 0; j <= n; j++) matrix[0][j] = 0;

    // Plnění matice na základě dynamického programování
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                // Shoda znaku - přičteme 1 k diagonální buňce
                matrix[i][j] = 1 + matrix[i - 1][j - 1];
            } else {
                // Neshoda znaku - bereme maximum z horní nebo levé buňky
                if (matrix[i - 1][j] > matrix[i][j - 1]) {
                    matrix[i][j] = matrix[i - 1][j];
                } else {
                    matrix[i][j] = matrix[i][j - 1];
                }
            }
        }
    }

    return matrix[m][n];
}

// Funkce pro rekonstrukci a tisk LCS
// Prochází matici od konce a rekonstruuje řetězec
void print_lcs(const char* s1, const char* s2, int m, int n, int** matrix) {
    int i = m;
    int j = n;
    int length = matrix[m][n];

    // Alokace paměti pro výsledný řetězec LCS
    char* lcs = (char*)malloc((length + 1) * sizeof(char));
    if (lcs == NULL) {
        printf("Chyba alokace pameti pro LCS.\n");
        return;
    }
    lcs[length] = '\0'; // Ukončovací znak

    int index = length - 1;

    // Procházení matice "pozpátku"
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            // Znaky se shodují, je to část LCS
            lcs[index] = s1[i - 1];
            i--;
            j--;
            index--;
        } else if (matrix[i - 1][j] > matrix[i][j - 1]) {
            // Jdi nahoru
            i--;
        } else {
            // Jdi doleva
            j--;
        }
    }
    printf("Nejdelsi spolecna podsekvence je: %s\n", lcs);
    free(lcs); // Uvolnění alokované paměti
}

// Hlavní funkce
int main() {
    const char* s1 = "KORAB";
    const char* s2 = "KOLOBEZKA";
    
    int m = strlen(s1);
    int n = strlen(s2);
    
    // Dynamická alokace paměti pro matici
    int** matrix = (int**)malloc((m + 1) * sizeof(int*));
    if (matrix == NULL) {
        printf("Chyba alokace pameti pro matici.\n");
        return 1;
    }
    for (int i = 0; i <= m; i++) {
        matrix[i] = (int*)malloc((n + 1) * sizeof(int));
        if (matrix[i] == NULL) {
            // Uvolnění paměti v případě chyby
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            printf("Chyba alokace pameti pro radky matice.\n");
            return 1;
        }
    }

    int length = lcs_length(s1, s2, matrix);

    printf("Delka nejdelssi spolecne podsekvence je: %d\n", length);
    print_lcs(s1, s2, m, n, matrix);

    // Uvolnění veškeré alokované paměti
    for (int i = 0; i <= m; i++) {
        free(matrix[i]);
    }
    free(matrix);

    // Wait for the user to press Enter before exiting.
    printf("\nPress Enter to exit the program...");
    
    // Clear the input buffer from the previous `scanf`.
    // This part can be tricky and platform-dependent.
    // A safer way is to use a loop to consume characters.
    while (getchar() != '\n'); 
    getchar(); // Reads the final newline character.

    return 0;
}