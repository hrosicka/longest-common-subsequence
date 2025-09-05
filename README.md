# Longest Common Subsequence (LCS) 🧬
This project is an implementation of the Longest Common Subsequence (LCS) algorithm in C. 
It uses a dynamic programming approach to efficiently find the length and reconstruct one of the longest common subsequences of two input strings.

---

## 💡 How It Works
The algorithm uses a 2D matrix where each element matrix[i][j] stores the length of the longest common subsequence of the first i characters of the first string and the first j characters of the second string.

If the characters s1[i-1] and s2[j-1] are the same, the length of the LCS is incremented by 1 (i.e., matrix[i][j] = 1 + matrix[i-1][j-1]).

If the characters are different, the algorithm takes the maximum value from the neighboring cells (matrix[i-1][j] and matrix[i][j-1]).

Once the entire matrix is filled, the length of the LCS is found in the bottom-right cell, matrix[m][n], where m and n are the lengths of the strings. The program then backtracks through the matrix from this cell to reconstruct the actual subsequence.

---

## 🚀 Compilation and Execution
To compile and run this program, you need a C compiler like GCC installed on your system.

---

## 💻 Example Usage
Here's an example of how the program works when executed:

Enter the first string: GAC
Enter the second string: AGGCTAC

The length of the longest common subsequence is: 3
The longest common subsequence is: GAC
In this case, the longest common subsequence of "GAC" and "AGGCTAC" is "GAC," with a length of 3.

---

## 🛠️ Technologies Used
- C
- GCC

---

## 🙌 Author

Created by [hrosicka](https://github.com/hrosicka)

---

## 📄 License

This project is licensed under the [MIT License](./LICENSE).
