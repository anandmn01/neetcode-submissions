# Unique Paths — LeetCode 62

- **Difficulty:** Medium
- **Topics:** Math, Dynamic Programming, Combinatorics
- **Pattern:** Grid DP
- **Goal:** Count the number of unique paths from the top-left corner to the bottom-right corner.

## 1. Problem Understanding

We are given an `m x n` grid.

We start at the top-left cell:

```text
grid[0][0]
```

We need to reach the bottom-right cell:

```text
grid[m-1][n-1]
```

We are allowed to move only:

1. Right
2. Down

We need to return the total number of unique paths.

### Example 1

```text
Input:
m = 3, n = 6

Output:
21
```

### Example 2

```text
Input:
m = 3, n = 2

Output:
3
```

Possible paths:

```text
Right → Down → Down

Down → Right → Down

Down → Down → Right
```

## 2. Key Observation

To reach any cell `(i, j)`, we have only two possible previous positions:

1. From the top: `(i-1, j)`
2. From the left: `(i, j-1)`

Therefore:

```text
ways(i, j) =
    ways(i-1, j) + ways(i, j-1)
```

This is the main DP recurrence.

### Base Cases

There is only one way to reach any cell in the first row because we can move only right.

Similarly, there is only one way to reach any cell in the first column because we can move only down.

```text
dp[0][j] = 1

dp[i][0] = 1
```

## 3. Brute Force Approach — Recursion

### Idea

Start from the destination and recursively calculate the number of ways to reach it.

At every cell, we consider:

- Moving up.
- Moving left.

This is equivalent to counting paths that originally move down or right from the starting position.

### C++ Code

```cpp
class Solution {
public:

    int solve(int i, int j) {

        // Reached the starting cell
        if (i == 0 && j == 0) {
            return 1;
        }

        // Outside the grid
        if (i < 0 || j < 0) {
            return 0;
        }

        int up = solve(i - 1, j);

        int left = solve(i, j - 1);

        return up + left;
    }

    int uniquePaths(int m, int n) {

        return solve(m - 1, n - 1);
    }
};
```

### Complexity Analysis

- **Time:** Exponential, approximately O(2^(m+n)).
- **Auxiliary Space:** O(m+n), due to recursion depth.

### Limitation

The same cells are calculated repeatedly.

For example, different recursive paths may reach the same cell `(1,1)`.

We can avoid repeated calculations using memoization.

## 4. Better Approach — DP with Memoization

### Idea

The recursive function depends on two parameters:

- `i`: Current row.
- `j`: Current column.

We store previously calculated answers in a 2D DP array.

### DP State

```cpp
dp[i][j]
```

Represents the number of unique paths from the starting cell `(0,0)` to cell `(i,j)`.

### Recurrence

```text
dp[i][j] = dp[i-1][j] + dp[i][j-1]
```

### C++ Code

```cpp
class Solution {
public:

    int solve(int i, int j, vector<vector<int>>& dp) {

        if (i == 0 && j == 0) {
            return 1;
        }

        if (i < 0 || j < 0) {
            return 0;
        }

        if (dp[i][j] != -1) {
            return dp[i][j];
        }

        int up = solve(i - 1, j, dp);

        int left = solve(i, j - 1, dp);

        return dp[i][j] = up + left;
    }

    int uniquePaths(int m, int n) {

        vector<vector<int>> dp(
            m,
            vector<int>(n, -1)
        );

        return solve(m - 1, n - 1, dp);
    }
};
```

### Complexity Analysis

- **Time:** O(m × n), because every cell is calculated once.
- **Auxiliary Space:** O(m × n) for the DP table.
- **Recursion Stack:** O(m+n).

## 5. Optimized Approach — 2D DP Tabulation

### Idea

Instead of using recursion, build the DP table iteratively.

We calculate the number of paths for every cell, starting from the top-left corner.

### DP State

```cpp
dp[i][j]
```

Represents the number of unique paths to reach cell `(i,j)`.

### Initialization

```cpp
dp[0][j] = 1;

dp[i][0] = 1;
```

The first row and first column have exactly one possible path.

### Transition

For every remaining cell:

```cpp
dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
```

Why?

- `dp[i-1][j]`: Ways to arrive from the top.
- `dp[i][j-1]`: Ways to arrive from the left.

### C++ Code

```cpp
class Solution {
public:

    int uniquePaths(int m, int n) {

        vector<vector<int>> dp(
            m,
            vector<int>(n, 1)
        );

        for (int i = 1; i < m; i++) {

            for (int j = 1; j < n; j++) {

                dp[i][j] =
                    dp[i - 1][j] +
                    dp[i][j - 1];
            }
        }

        return dp[m - 1][n - 1];
    }
};
```

### Complexity Analysis

- **Time:** O(m × n).
- **Auxiliary Space:** O(m × n).

## 6. Dry Run — 2D DP

Input:

```text
m = 3
n = 6
```

We have a grid containing 3 rows and 6 columns.

### Step 1: Initialize DP

Initially, every cell contains 1.

```text
1  1  1  1  1  1
1  1  1  1  1  1
1  1  1  1  1  1
```

Why?

The first row and first column each have exactly one possible path.

### Step 2: Calculate Remaining Cells

Formula:

```text
dp[i][j] = dp[i-1][j] + dp[i][j-1]
```

For cell `(1,1)`:

```text
dp[1][1] = dp[0][1] + dp[1][0]

          = 1 + 1

          = 2
```

For cell `(1,2)`:

```text
dp[1][2] = dp[0][2] + dp[1][1]

          = 1 + 2

          = 3
```

For cell `(1,3)`:

```text
dp[1][3] = 1 + 3 = 4
```

Continue the same process for every cell.

### Step 3: Final DP Table

```text
1  1  1  1  1  1

1  2  3  4  5  6

1  3  6  10 15 21
```

The destination is:

```text
dp[2][5] = 21
```

Final answer:

```text
Output: 21
```

## 7. Most Optimized Approach — 1D DP (Space Optimization)

### Key Observation

In the 2D DP solution, each cell depends only on:

```text
dp[i-1][j]  // Previous row

dp[i][j-1]  // Current row, previous column
```

We do not need to store the entire 2D matrix.

We can solve the problem using a single 1D array.

### DP State

```cpp
dp[j]
```

Represents the number of ways to reach column `j` in the current row.

### Initialization

```cpp
vector<int> dp(n, 1);
```

Initially, every position contains 1 because the first row has only one possible path to each cell.

### Transition

```cpp
dp[j] = dp[j] + dp[j - 1];
```

Here:

- `dp[j]` contains the value from the previous row.
- `dp[j-1]` contains the updated value from the current row.

Therefore:

```text
Current cell =
    Previous row's current cell +
    Current row's previous cell
```

### C++ Code — Recommended Solution

```cpp
class Solution {
public:

    int uniquePaths(int m, int n) {

        vector<int> dp(n, 1);

        for (int i = 1; i < m; i++) {

            for (int j = 1; j < n; j++) {

                dp[j] = dp[j] + dp[j - 1];
            }
        }

        return dp[n - 1];
    }
};
```

### Complexity Analysis

- **Time:** O(m × n).
- **Auxiliary Space:** O(n).

This is the recommended DP solution because it maintains the same time complexity while reducing space usage.

## 8. Dry Run — 1D DP

Input:

```text
m = 3
n = 6
```

### Initial DP

```text
dp = [1, 1, 1, 1, 1, 1]
```

### Row 1

Update from left to right.

```text
dp[1] = 1 + 1 = 2

dp[2] = 1 + 2 = 3

dp[3] = 1 + 3 = 4

dp[4] = 1 + 4 = 5

dp[5] = 1 + 5 = 6
```

Array after row 1:

```text
[1, 2, 3, 4, 5, 6]
```

### Row 2

Again update from left to right.

```text
dp[1] = 2 + 1 = 3

dp[2] = 3 + 3 = 6

dp[3] = 4 + 6 = 10

dp[4] = 5 + 10 = 15

dp[5] = 6 + 15 = 21
```

Array after row 2:

```text
[1, 3, 6, 10, 15, 21]
```

Final answer:

```cpp
return dp[n - 1];
```

```text
Output: 21
```

## 9. Alternative Approach — Combinatorics

This problem can also be solved using mathematics.

### Key Observation

To reach the bottom-right corner of an `m x n` grid, we must perform:

- `m - 1` downward moves.
- `n - 1` rightward moves.

Total moves:

```text
(m - 1) + (n - 1)

= m + n - 2
```

We only need to decide which positions among these total moves will be downward (or rightward).

Therefore:

```text
Answer = C(m+n-2, m-1)
```

Or equivalently:

```text
Answer = C(m+n-2, n-1)
```

### Example

```text
m = 3
n = 6
```

Downward moves:

```text
3 - 1 = 2
```

Rightward moves:

```text
6 - 1 = 5
```

Total moves:

```text
2 + 5 = 7
```

Number of unique paths:

```text
C(7,2) = 7! / (2! × 5!)

       = 21
```

### C++ Code

```cpp
class Solution {
public:

    int uniquePaths(int m, int n) {

        long long result = 1;

        int total = m + n - 2;

        int r = min(m - 1, n - 1);

        for (int i = 1; i <= r; i++) {

            result = result * (total - r + i) / i;
        }

        return (int)result;
    }
};
```

### Complexity Analysis

- **Time:** O(min(m,n)).
- **Auxiliary Space:** O(1).

**Important:** The multiplication uses `long long` to avoid intermediate integer overflow. The problem guarantees that the final answer fits in a 32-bit integer.

## 10. Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force Recursion | Exponential | O(m+n) stack |
| Memoization | O(m × n) | O(m × n) + stack |
| 2D DP | O(m × n) | O(m × n) |
| 1D DP | O(m × n) | O(n) |
| Combinatorics | O(min(m,n)) | O(1) |

## 11. Common Mistakes

1. Confusing the number of rows `m` with the number of columns `n`.
2. Forgetting that movement is allowed only right and down.
3. Using incorrect base cases.
4. Forgetting that the first row and first column contain only one possible path.
5. Accessing `dp[i-1][j]` or `dp[i][j-1]` without handling boundaries.
6. Returning `dp[m][n]` instead of `dp[m-1][n-1]` in 2D DP.
7. Returning `dp[n]` instead of `dp[n-1]` in 1D DP.
8. Updating the 1D DP array in the wrong direction.

## 12. Important Interview Questions

### Q1. Why do we add the top and left cells?

Because the only possible ways to enter a cell are from its top or left neighbor.

These represent two disjoint groups of paths, so we add their counts.

### Q2. Why is the first row initialized with 1?

There is exactly one way to reach each cell in the first row: continuously move right.

### Q3. Why is the first column initialized with 1?

There is exactly one way to reach each cell in the first column: continuously move down.

### Q4. Why does 1D DP work?

Each cell depends only on the previous row's value at the same column and the current row's value at the previous column.

Therefore, the entire 2D table is unnecessary.

### Q5. Why do we iterate left to right in 1D DP?

Because `dp[j]` must retain the previous row's value until it is updated, while `dp[j-1]` must already contain the current row's updated value.

### Q6. Can this problem be solved without DP?

Yes. We can use combinatorics:

```text
C(m+n-2, m-1)
```

### Q7. What is the difference between Unique Paths and Unique Paths II?

- **Unique Paths:** Every cell is accessible.
- **Unique Paths II:** Some cells contain obstacles, so those cells cannot be visited.

## 13. Final Revision Notes

### Main DP Formula

```cpp
dp[i][j] = dp[i-1][j] + dp[i][j-1];
```

### Base Cases

```cpp
dp[0][j] = 1;

dp[i][0] = 1;
```

### 2D DP

```cpp
vector<vector<int>> dp(m, vector<int>(n, 1));

for (int i = 1; i < m; i++) {

    for (int j = 1; j < n; j++) {

        dp[i][j] = dp[i-1][j] + dp[i][j-1];
    }
}
```

### 1D DP

```cpp
vector<int> dp(n, 1);

for (int i = 1; i < m; i++) {

    for (int j = 1; j < n; j++) {

        dp[j] += dp[j-1];
    }
}
```

### Most Important Points

1. Only two movements are allowed: right and down.
2. Every cell depends on its top and left neighbors.
3. First row and first column are initialized with 1.
4. The recurrence is `dp[i][j] = dp[i-1][j] + dp[i][j-1]`.
5. 2D DP can be optimized to 1D DP.
6. The 1D array must be updated from left to right.
7. Combinatorics provides an alternative mathematical solution.

**Recommended DP Approach:** 1D DP.

- **Time Complexity:** O(m × n).

  # Longest Common Subsequence (LCS) — LeetCode 1143

- **Difficulty:** Medium
- **Topics:** String, Dynamic Programming
- **Pattern:** 2D DP / String DP
- **Goal:** Find the length of the longest subsequence common to two strings.

## 1. Problem Understanding

We are given two strings:

```cpp
string text1;
string text2;
```

We need to return the length of their longest common subsequence.

### What is a Subsequence?

A subsequence is a sequence that can be obtained by deleting zero or more characters without changing the relative order of the remaining characters.

For example:

```text
Original string: "crabt"

Valid subsequences:
"cat"
"car"
"rat"
"ct"
```

The relative order of characters must remain the same.

### Example 1

```text
Input:
text1 = "cat"
text2 = "crabt"

Output:
3
```

Explanation:

The longest common subsequence is:

```text
"cat"
```

Length = 3.

### Example 2

```text
Input:
text1 = "abcd"
text2 = "abcd"

Output:
4
```

Explanation:

Both strings are identical, so the entire string is the LCS.

### Example 3

```text
Input:
text1 = "abc"
text2 = "def"

Output:
0
```

Explanation:

There are no common characters.

## 2. Key Observation

We compare characters from both strings.

There are two possible situations:

### Case 1: Characters Match

If:

```cpp
text1[i] == text2[j]
```

We include this character in the common subsequence.

Therefore:

```text
1 + LCS of the remaining characters
```

### Case 2: Characters Do Not Match

If:

```cpp
text1[i] != text2[j]
```

We have two choices:

1. Skip the current character from `text1`.
2. Skip the current character from `text2`.

We take the maximum of both possibilities.

```text
max(
    LCS after skipping text1[i],
    LCS after skipping text2[j]
)
```

This gives us the main recurrence.

## 3. Brute Force Approach — Recursion

### Idea

Use two pointers:

- `i` represents the current index in `text1`.
- `j` represents the current index in `text2`.

Start from index 0 in both strings.

If characters match, move both pointers forward.

If characters do not match, try skipping one character from either string and take the maximum result.

### Base Case

If either string is completely processed, no more common characters can be found.

```cpp
if (i == n || j == m) {
    return 0;
}
```

### C++ Code

```cpp
class Solution {
public:

    int solve(int i, int j, string& text1, string& text2) {

        int n = text1.size();
        int m = text2.size();

        // Base case
        if (i == n || j == m) {
            return 0;
        }

        // Characters match
        if (text1[i] == text2[j]) {

            return 1 + solve(
                i + 1,
                j + 1,
                text1,
                text2
            );
        }

        // Characters do not match
        int skipFirst = solve(
            i + 1,
            j,
            text1,
            text2
        );

        int skipSecond = solve(
            i,
            j + 1,
            text1,
            text2
        );

        return max(skipFirst, skipSecond);
    }

    int longestCommonSubsequence(string text1, string text2) {

        return solve(0, 0, text1, text2);
    }
};
```

### Complexity Analysis

- **Time:** Exponential in the worst case, approximately O(2^(n+m)).
- **Auxiliary Space:** O(n+m), due to recursion depth.

### Limitation

The same states `(i,j)` are calculated repeatedly.

We can avoid these repeated calculations using memoization.

## 4. Better Approach — DP with Memoization

### Idea

The recursive function depends on two variables:

- `i`: Index in the first string.
- `j`: Index in the second string.

Therefore, we use a 2D DP array to store previously calculated results.

### DP State

```cpp
dp[i][j]
```

Represents the length of the longest common subsequence between:

```text
text1[i ... n-1]

text2[j ... m-1]
```

### Recurrence

If characters match:

```text
dp[i][j] = 1 + dp[i+1][j+1]
```

If characters do not match:

```text
dp[i][j] = max(
    dp[i+1][j],
    dp[i][j+1]
)
```

### C++ Code

```cpp
class Solution {
public:

    int solve(
        int i,
        int j,
        string& text1,
        string& text2,
        vector<vector<int>>& dp
    ) {

        int n = text1.size();
        int m = text2.size();

        if (i == n || j == m) {
            return 0;
        }

        if (dp[i][j] != -1) {
            return dp[i][j];
        }

        if (text1[i] == text2[j]) {

            return dp[i][j] = 1 + solve(
                i + 1,
                j + 1,
                text1,
                text2,
                dp
            );
        }

        int skipFirst = solve(
            i + 1,
            j,
            text1,
            text2,
            dp
        );

        int skipSecond = solve(
            i,
            j + 1,
            text1,
            text2,
            dp
        );

        return dp[i][j] = max(skipFirst, skipSecond);
    }

    int longestCommonSubsequence(string text1, string text2) {

        int n = text1.size();
        int m = text2.size();

        vector<vector<int>> dp(
            n,
            vector<int>(m, -1)
        );

        return solve(0, 0, text1, text2, dp);
    }
};
```

### Complexity Analysis

- **Time:** O(n × m), because every state is calculated once.
- **Auxiliary Space:** O(n × m) for the DP table.
- **Recursion Stack:** O(n+m).

## 5. Optimized Approach — 2D DP Tabulation

### Idea

Instead of using recursion, we build the DP table iteratively.

To make boundary handling easier, we create a DP table of size:

```text
(n+1) × (m+1)
```

The extra row and column represent the case where one string is empty.

### DP State

```cpp
dp[i][j]
```

Represents the length of the LCS between:

```text
text1[0 ... i-1]

text2[0 ... j-1]
```

In other words, we consider the first `i` characters of `text1` and the first `j` characters of `text2`.

### Initialization

If either string is empty, the LCS length is zero.

Therefore:

```cpp
dp[0][j] = 0;

dp[i][0] = 0;
```

### Transition

**Case 1: Characters Match**

```cpp
if (text1[i-1] == text2[j-1]) {

    dp[i][j] = 1 + dp[i-1][j-1];
}
```

Why?

We include the matching character and add 1 to the LCS of the previous prefixes.

**Case 2: Characters Do Not Match**

```cpp
else {

    dp[i][j] = max(
        dp[i-1][j],
        dp[i][j-1]
    );
}
```

Why?

We either:

- Exclude the current character from `text1`.
- Exclude the current character from `text2`.

We take the better result.

### C++ Code

```cpp
class Solution {
public:

    int longestCommonSubsequence(string text1, string text2) {

        int n = text1.size();
        int m = text2.size();

        vector<vector<int>> dp(
            n + 1,
            vector<int>(m + 1, 0)
        );

        for (int i = 1; i <= n; i++) {

            for (int j = 1; j <= m; j++) {

                if (text1[i - 1] == text2[j - 1]) {

                    dp[i][j] = 1 + dp[i - 1][j - 1];

                } else {

                    dp[i][j] = max(
                        dp[i - 1][j],
                        dp[i][j - 1]
                    );
                }
            }
        }

        return dp[n][m];
    }
};
```

### Complexity Analysis

- **Time:** O(n × m).
- **Auxiliary Space:** O(n × m).

## 6. Dry Run — 2D DP

Input:

```text
text1 = "abc"
text2 = "crabt"
```

Lengths:

```text
n = 3
m = 5
```

### Step 1: Initialize DP Table

Create a `(n+1) × (m+1)` table.

Initially, every cell contains 0.

```text
      ""  c  r  a  b  t

""     0  0  0  0  0  0
a      0  0  0  0  0  0
b      0  0  0  0  0  0
c      0  0  0  0  0  0
```

### Step 2: Compare Characters

Compare characters of both strings.

**Comparison 1:**

```text
text1[0] = 'a'

text2[0] = 'c'
```

Characters do not match.

```text
dp[1][1] = max(dp[0][1], dp[1][0])

         = max(0, 0)

         = 0
```

**Comparison 2:**

```text
text1[0] = 'a'

text2[2] = 'a'
```

Characters match.

```text
dp[1][3] = 1 + dp[0][2]

         = 1
```

We found one common character: `"a"`.

**Comparison 3:**

```text
text1[1] = 'b'

text2[3] = 'b'
```

Characters match.

```text
dp[2][4] = 1 + dp[1][3]

         = 1 + 1

         = 2
```

The common subsequence is now `"ab"`.

**Comparison 4:**

```text
text1[2] = 'c'

text2[0] = 'c'
```

Characters match.

```text
dp[3][1] = 1 + dp[2][0]

         = 1
```

### Step 3: Final DP Table

After processing every cell:

```text
      ""  c  r  a  b  t

""     0  0  0  0  0  0
a      0  0  0  1  1  1
b      0  0  0  1  2  2
c      0  1  1  1  2  3
```

The final answer is:

```cpp
dp[n][m]
```

```text
dp[3][5] = 3
```

Output:

```text
3
```

The longest common subsequence is:

```text
"abc"
```

## 7. Most Optimized Approach — 1D DP

### Key Observation

In the 2D DP solution, each cell depends on only three values:

```cpp
dp[i-1][j-1]

dp[i-1][j]

dp[i][j-1]
```

These represent:

- Previous row, previous column.
- Previous row, current column.
- Current row, previous column.

We do not need to store the entire 2D table.

We can optimize the space using a single 1D array.

### DP State

```cpp
dp[j]
```

Represents the LCS length for the current row and column `j`.

### Important Concept — Diagonal Variable

Before updating `dp[j]`, we must preserve the old diagonal value:

```cpp
int diagonal = 0;
```

For every row:

```cpp
int prev = 0;
```

When processing each column:

- `dp[j]` contains the previous row's value.
- `dp[j-1]` contains the current row's updated value.
- `prev` stores the previous row's previous column value.

### Transition

If characters match:

```cpp
dp[j] = 1 + prev;
```

Otherwise:

```cpp
dp[j] = max(dp[j], dp[j-1]);
```

After processing the current cell:

```cpp
prev = old dp[j];
```

### C++ Code — Recommended Space-Optimized Solution

```cpp
class Solution {
public:

    int longestCommonSubsequence(string text1, string text2) {

        int n = text1.size();
        int m = text2.size();

        vector<int> dp(m + 1, 0);

        for (int i = 1; i <= n; i++) {

            int prev = 0;

            for (int j = 1; j <= m; j++) {

                int temp = dp[j];

                if (text1[i - 1] == text2[j - 1]) {

                    dp[j] = 1 + prev;

                } else {

                    dp[j] = max(dp[j], dp[j - 1]);
                }

                prev = temp;
            }
        }

        return dp[m];
    }
};
```

### Complexity Analysis

- **Time:** O(n × m).
- **Auxiliary Space:** O(m).

This is the space-optimized DP solution.

## 8. Dry Run — 1D DP

Input:

```text
text1 = "abc"
text2 = "crabt"
```

Initial array:

```text
dp = [0, 0, 0, 0, 0, 0]
```

### Process Row 1 — Character 'a'

Compare `'a'` with every character in `"crabt"`.

After processing:

```text
dp = [0, 0, 0, 1, 1, 1]
```

### Process Row 2 — Character 'b'

Continue updating the array.

```text
dp = [0, 0, 0, 1, 2, 2]
```

### Process Row 3 — Character 'c'

Continue updating the array.

```text
dp = [0, 1, 1, 1, 2, 3]
```

Final answer:

```cpp
return dp[m];
```

```text
Output = 3
```

## 9. Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force Recursion | Exponential | O(n+m) stack |
| Memoization | O(n × m) | O(n × m) + stack |
| 2D DP Tabulation | O(n × m) | O(n × m) |
| 1D DP Optimization | O(n × m) | O(m) |

## 10. Common Mistakes

1. Confusing a subsequence with a substring.
2. Forgetting that subsequences must maintain relative order.
3. Moving only one pointer when characters match.
4. Taking the minimum instead of maximum when characters do not match.
5. Using incorrect DP indices.
6. Forgetting the extra row and column in tabulation.
7. Accessing invalid indices when one string is empty.
8. In 1D DP, overwriting the diagonal value before using it.
9. Updating the 1D DP array without preserving the previous row's values.

## 11. Important Interview Questions

### Q1. What is the difference between a substring and a subsequence?

**Substring:**

A contiguous sequence of characters.

Example:

```text
String: "abcde"

Valid substring: "bcd"
```

**Subsequence:**

Characters can be deleted, but relative order must remain unchanged.

Example:

```text
String: "abcde"

Valid subsequence: "ace"
```

### Q2. Why do we take the maximum when characters do not match?

Because we have two choices:

- Skip the current character from the first string.
- Skip the current character from the second string.

We choose the option that gives the longer common subsequence.

### Q3. Why do we add 1 when characters match?

Because the matching character becomes part of the common subsequence.

We add 1 to the answer for the remaining prefixes.

### Q4. Why do we use an `(n+1) × (m+1)` DP table?

The extra row and column represent empty prefixes.

This simplifies base cases and prevents invalid index access.

### Q5. Can LCS be solved using greedy?

A simple greedy approach is not sufficient because choosing a locally matching character may not produce the globally longest subsequence.

Dynamic programming considers both possible choices when characters do not match.

### Q6. Can we optimize the space complexity?

Yes.

Each DP state depends only on the previous row and the current row's previous column.

Therefore, we can use a 1D array and a variable to preserve the diagonal value.

### Q7. Can we reconstruct the actual LCS string?

Yes.

Using the 2D DP table, start from `dp[n][m]`:

- If characters match, include the character and move diagonally.
- Otherwise, move toward the neighboring cell with the larger DP value.

Reverse the collected characters to obtain the LCS.

## 12. Related Problems and Concepts

LCS is an important foundation for several other string DP problems.

| Problem | Connection |
|---|---|
| Longest Palindromic Subsequence | LCS of string and its reverse |
| Shortest Common Supersequence | Uses LCS length |
| Delete Operation for Two Strings | Based on LCS |
| Minimum Insertions to Make a String Palindrome | Uses LCS with reversed string |
| Edit Distance | Similar 2D string DP, but different transitions |

### Important Formula

For two strings of lengths `n` and `m`:

```text
Shortest Common Supersequence Length

= n + m - LCS Length
```

For deleting characters to make two strings equal:

```text
Minimum Deletions

= n + m - 2 × LCS Length
```

## 13. Final Revision Notes

### Main Recurrence — 2D DP

If characters match:

```cpp
dp[i][j] = 1 + dp[i-1][j-1];
```

If characters do not match:

```cpp
dp[i][j] = max(
    dp[i-1][j],
    dp[i][j-1]
);
```

### Base Cases

```cpp
dp[0][j] = 0;

dp[i][0] = 0;
```

### Recommended 2D DP Code

```cpp
vector<vector<int>> dp(
    n + 1,
    vector<int>(m + 1, 0)
);

for (int i = 1; i <= n; i++) {

    for (int j = 1; j <= m; j++) {

        if (text1[i - 1] == text2[j - 1]) {

            dp[i][j] = 1 + dp[i - 1][j - 1];

        } else {

            dp[i][j] = max(
                dp[i - 1][j],
                dp[i][j - 1]
            );
        }
    }
}
```

### Most Important Points

1. LCS means Longest Common Subsequence.
2. A subsequence does not need to be contiguous.
3. Matching characters: add 1 and move diagonally.
4. Non-matching characters: take the maximum of top and left.
5. Use a 2D DP table of size `(n+1) × (m+1)`.
6. Space can be optimized to O(m) using 1D DP.
7. Preserve the previous diagonal value during space optimization.

**Recommended Approach:** 2D DP for understanding and 1D DP for space optimization.

- **Time Complexity:** O(n × m).
- **Auxiliary Space:** O(m) with 1D DP.

**Interview Takeaway:** Whenever two strings are involved and the problem asks for a longest common subsequence, think about two pointers, matching characters, and 2D DP.

# Best Time to Buy and Sell Stock with Cooldown — LeetCode 309

- **Difficulty:** Medium
- **Topics:** Array, Dynamic Programming
- **Pattern:** State Machine DP / Stock DP
- **Goal:** Maximize profit with unlimited transactions and a one-day cooldown after selling.

## 1. Problem Understanding

We are given an integer array:

```cpp
vector<int> prices;
```

Where:

- `prices[i]` represents the stock price on day `i`.
- We can buy and sell multiple times.
- We can hold at most one stock at a time.
- After selling a stock, we cannot buy another stock on the next day.
- We need to return the maximum profit.

### Important Rules

1. We can buy a stock only when we are not holding one.
2. We must sell a stock before buying another.
3. After selling, we must wait one day before buying again.
4. We can perform unlimited transactions.

### Example

```text
Input:
prices = [1,3,4,0,4]

Output:
6
```

Explanation:

| Day | Price | Action | Profit |
|---|---:|---|---:|
| 0 | 1 | Buy | -1 |
| 1 | 3 | Sell | 2 |
| 2 | 4 | Cooldown | 2 |
| 3 | 0 | Buy | 2 |
| 4 | 4 | Sell | 6 |

Total profit:

```text
(3 - 1) + (4 - 0)

= 2 + 4

= 6
```

## 2. Key Observation

The cooldown restriction makes this problem different from normal stock buy-and-sell problems.

After selling, we cannot immediately buy again.

We need to track our current situation.

There are three possible states:

1. **Hold:** We currently own a stock.
2. **Sold:** We sold a stock today.
3. **Rest:** We do not own a stock and are not selling today.

These three states form the foundation of the optimized solution.

## 3. Brute Force Approach — Recursion

### Idea

At every day, we have two possible situations:

- We are not holding a stock.
- We are holding a stock.

We use recursion to explore the possible decisions.

### State

```cpp
solve(i, holding)
```

Where:

- `i` = current day.
- `holding = 0` means we do not own a stock.
- `holding = 1` means we own a stock.

### Case 1: We Are Not Holding a Stock

We have two choices:

**Choice A: Do Nothing**

Move to the next day.

```cpp
solve(i + 1, 0)
```

**Choice B: Buy a Stock**

Pay the current stock price.

```cpp
-prices[i] + solve(i + 1, 1)
```

Take the maximum.

### Case 2: We Are Holding a Stock

We have two choices:

**Choice A: Continue Holding**

```cpp
solve(i + 1, 1)
```

**Choice B: Sell the Stock**

Receive the current stock price.

After selling, we must skip the next day because of cooldown.

```cpp
prices[i] + solve(i + 2, 0)
```

Take the maximum.

### C++ Code

```cpp
class Solution {
public:

    int solve(int i, int holding, vector<int>& prices) {

        int n = prices.size();

        if (i >= n) {
            return holding ? -1000000000 : 0;
        }

        if (holding == 0) {

            // Option 1: Do nothing
            int skip = solve(i + 1, 0, prices);

            // Option 2: Buy
            int buy = -prices[i] + solve(i + 1, 1, prices);

            return max(skip, buy);
        }

        else {

            // Option 1: Continue holding
            int hold = solve(i + 1, 1, prices);

            // Option 2: Sell and skip the next day
            int sell = prices[i] + solve(i + 2, 0, prices);

            return max(hold, sell);
        }
    }

    int maxProfit(vector<int>& prices) {

        return solve(0, 0, prices);
    }
};
```

### Complexity Analysis

- **Time:** Exponential in the worst case, O(2^n).
- **Auxiliary Space:** O(n), due to recursion depth.

### Limitation

The same states are calculated repeatedly.

For example, the same day and holding status may be reached through different transaction choices.

We can optimize this using memoization.

## 4. Better Approach — DP with Memoization

### Idea

The recursive function depends on two variables:

- Current day `i`.
- Holding status `holding`.

Therefore, we can store the answer for each state in a 2D DP array.

### DP State

```cpp
dp[i][holding]
```

Represents the maximum profit we can achieve starting from day `i`, given whether we currently hold a stock.

### Recurrence

If we are not holding:

```cpp
dp[i][0] = max(
    dp[i + 1][0],
    -prices[i] + dp[i + 1][1]
);
```

If we are holding:

```cpp
dp[i][1] = max(
    dp[i + 1][1],
    prices[i] + dp[i + 2][0]
);
```

Notice that selling moves us to `i + 2` because the next day is a cooldown day.

### C++ Code

```cpp
class Solution {
public:

    int solve(
        int i,
        int holding,
        vector<int>& prices,
        vector<vector<int>>& dp
    ) {

        int n = prices.size();

        if (i >= n) {
            return holding ? -1000000000 : 0;
        }

        if (dp[i][holding] != -1) {
            return dp[i][holding];
        }

        if (holding == 0) {

            int skip = solve(
                i + 1, 0, prices, dp
            );

            int buy = -prices[i] + solve(
                i + 1, 1, prices, dp
            );

            return dp[i][holding] = max(skip, buy);
        }

        int hold = solve(
            i + 1, 1, prices, dp
        );

        int sell = prices[i] + solve(
            i + 2, 0, prices, dp
        );

        return dp[i][holding] = max(hold, sell);
    }

    int maxProfit(vector<int>& prices) {

        int n = prices.size();

        vector<vector<int>> dp(
            n,
            vector<int>(2, -1)
        );

        return solve(0, 0, prices, dp);
    }
};
```

### Complexity Analysis

- **Time:** O(n), because there are at most `2 × n` states.
- **Auxiliary Space:** O(n) for the DP table.
- **Recursion Stack:** O(n).

## 5. Optimized Approach — State Machine DP (Three States)

This is the most important approach to understand for this problem.

Instead of tracking only whether we hold a stock, we explicitly maintain three states.

### State 1: Hold

We currently own a stock.

We can:

1. Continue holding the stock.
2. Buy a stock today if we were in the Rest state yesterday.

Transition:

```cpp
hold = max(
    hold,
    rest - price
);
```

### State 2: Sold

We sell our stock today.

To sell, we must have held a stock yesterday.

Transition:

```cpp
sold = oldHold + price;
```

### State 3: Rest

We are not holding a stock and are not selling today.

We can:

1. Continue resting.
2. Move from Sold to Rest after the cooldown day.

Transition:

```cpp
rest = max(
    rest,
    oldSold
);
```

### Important: Why Do We Need Three States?

Consider:

```text
prices = [1,3,4,0,4]
```

After selling at price 3, we cannot buy at price 4 on the next day.

The Sold state remembers that we sold today.

The Rest state allows us to buy only after the cooldown restriction has been satisfied.

## 6. DP Initialization

Before processing any days:

```cpp
hold = -prices[0];

sold = INT_MIN;

rest = 0;
```

Explanation:

- `hold = -prices[0]`: We buy the stock on day 0.
- `sold = INT_MIN`: We cannot sell before owning a stock.
- `rest = 0`: We start with no stock and zero profit.

## 7. C++ Code — State Machine DP

```cpp
class Solution {
public:

    int maxProfit(vector<int>& prices) {

        int n = prices.size();

        int hold = -prices[0];
        int sold = INT_MIN;
        int rest = 0;

        for (int i = 1; i < n; i++) {

            int price = prices[i];

            int oldHold = hold;
            int oldSold = sold;
            int oldRest = rest;

            // Buy or continue holding
            hold = max(
                oldHold,
                oldRest - price
            );

            // Sell the stock today
            sold = oldHold + price;

            // Continue resting or finish cooldown
            rest = max(
                oldRest,
                oldSold
            );
        }

        return max(sold, rest);
    }
};
```

### Complexity Analysis

- **Time:** O(n), because we process the array once.
- **Auxiliary Space:** O(1), because we use only three state variables.

This is the recommended solution.

## 8. Dry Run — State Machine DP

Input:

```text
prices = [1,3,4,0,4]
```

Initialization:

```text
hold = -1
sold = INT_MIN
rest = 0
```

We process the remaining days.

### Day 1 — Price = 3

```cpp
hold = max(-1, 0 - 3)
     = -1;

sold = -1 + 3
     = 2;

rest = max(0, INT_MIN)
     = 0;
```

State:

```text
hold = -1
sold = 2
rest = 0
```

We bought at 1 and sold at 3.

Profit = 2.

### Day 2 — Price = 4

Use the previous day's states.

```cpp
hold = max(-1, 0 - 4)
     = -1;

sold = -1 + 4
     = 3;

rest = max(0, 2)
     = 2;
```

State:

```text
hold = -1
sold = 3
rest = 2
```

We can also sell the stock at 4 for profit 3.

Alternatively, we can enter Rest with profit 2 after selling on day 1.

### Day 3 — Price = 0

```cpp
hold = max(-1, 2 - 0)
     = 2;

sold = -1 + 0
     = -1;

rest = max(2, 3)
     = 3;
```

State:

```text
hold = 2
sold = -1
rest = 3
```

We can now buy at price 0 because the cooldown after the earlier sale has completed.

### Day 4 — Price = 4

```cpp
hold = max(2, 3 - 4)
     = 2;

sold = 2 + 4
     = 6;

rest = max(3, -1)
     = 3;
```

Final state:

```text
hold = 2
sold = 6
rest = 3
```

Answer:

```cpp
max(sold, rest)
```

```text
max(6, 3) = 6
```

Output:

```text
6
```

## 9. DP State Summary

| State | Meaning | Transition |
|---|---|---|
| Hold | Currently own a stock | max(oldHold, oldRest - price) |
| Sold | Sold a stock today | oldHold + price |
| Rest | Not holding and not selling today | max(oldRest, oldSold) |

### Visual Understanding

```text
                 BUY
          REST ----------> HOLD
            ^                |
            |                |
            |                | SELL
            |                v
            +-------------- SOLD
                 COOLDOWN
```

Remember:

- Rest can transition to Hold by buying.
- Hold can transition to Sold by selling.
- Sold transitions to Rest on the next day.
- Sold cannot directly transition to Hold.

## 10. Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force Recursion | O(2^n) | O(n) stack |
| Memoization | O(n) | O(n) + stack |
| State Machine DP | O(n) | O(1) |

## 11. Common Mistakes

1. Forgetting the cooldown day after selling.
2. Buying another stock while already holding one.
3. Selling without previously buying.
4. Updating states in place without preserving old values.
5. Allowing a direct Sold-to-Hold transition.
6. Initializing `sold` as 0 instead of an impossible-state value.
7. Returning `hold` as the final answer even though an unsold stock does not represent realized profit.
8. Confusing the cooldown restriction with a rule that limits the total number of transactions.

## 12. Important Interview Questions

### Q1. Why do we need three states?

Because we must distinguish between:

- Holding a stock.
- Selling a stock today.
- Not holding a stock and being eligible to buy.

The cooldown rule requires us to remember whether we sold on the previous day.

### Q2. Why can't we buy immediately after selling?

Because the problem explicitly requires a one-day cooldown.

If we sell on day `i`, the earliest we can buy again is day `i + 2`.

### Q3. Why do we use old state variables?

Each transition must use the previous day's state.

If we update `hold`, `sold`, and `rest` directly without saving their old values, we may accidentally use a state calculated for the current day.

### Q4. Why is the final answer `max(sold, rest)`?

At the end, the best completed profit must be in one of these states:

- Sold: We have completed a transaction today.
- Rest: We are not holding a stock.

The Hold state represents an unsold stock, so it is not included.

### Q5. What is the difference between this problem and normal Stock II?

In Stock II, we can buy immediately after selling.

In this problem, we must wait one day after selling.

That additional restriction requires cooldown state tracking.

### Q6. Can we solve this problem using recursion and memoization?

Yes.

We track the day and whether we are holding a stock.

When selling, we jump to `i + 2` to account for the cooldown.

## 13. Related Stock DP Problems

| Problem | Key Concept |
|---|---|
| Best Time to Buy and Sell Stock | One transaction |
| Best Time to Buy and Sell Stock II | Unlimited transactions |
| Best Time to Buy and Sell Stock III | At most two transactions |
| Best Time to Buy and Sell Stock IV | At most k transactions |
| Best Time to Buy and Sell Stock with Cooldown | One-day cooldown |
| Best Time to Buy and Sell Stock with Transaction Fee | Transaction fee |

## 14. Final Revision Notes

### Three States

```cpp
int hold;
int sold;
int rest;
```

### Initialization

```cpp
hold = -prices[0];

sold = INT_MIN;

rest = 0;
```

### Transitions

```cpp
newHold = max(oldHold, oldRest - price);

newSold = oldHold + price;

newRest = max(oldRest, oldSold);
```

### Final Answer

```cpp
max(sold, rest);
```

### Complexity

```text
Time Complexity: O(n)

Auxiliary Space: O(1)
```

### Most Important Interview Takeaways

1. Recognize the cooldown restriction.
2. Model the problem using three states.
3. Preserve previous-day states before updating.
4. Selling moves us into the Sold state.
5. Sold moves to Rest on the next day.
6. Buying is allowed only from Rest.
7. The optimized solution uses O(1) auxiliary space.

**Recommended Approach:** State Machine DP.

**Interview Pattern:** Whenever a stock problem includes cooldown, transaction fees, or transaction limits, consider defining explicit DP states for the different actions and restrictions.

# Coin Change II — LeetCode 518

- **Difficulty:** Medium
- **Topics:** Array, Dynamic Programming
- **Pattern:** Unbounded Knapsack / Combination Counting
- **Goal:** Count the number of distinct combinations that make up a target amount.

## 1. Problem Understanding

We are given:

```cpp
int amount;
vector<int> coins;
```

Each element in `coins` represents a different coin denomination.

We need to return the number of distinct combinations that add up to `amount`.

### Important Rules

1. We have an unlimited supply of each coin.
2. We can use a coin multiple times.
3. Different orders of the same coins are considered the same combination.
4. If the amount cannot be formed, return `0`.

### Example 1

```text
Input:
amount = 4
coins = [1,2,3]

Output:
4
```

The four valid combinations are:

```text
1 + 1 + 1 + 1 = 4

1 + 1 + 2 = 4

2 + 2 = 4

1 + 3 = 4
```

Notice that `1 + 1 + 2` and `2 + 1 + 1` are the same combination.

### Example 2

```text
Input:
amount = 3
coins = [2]

Output:
0
```

We cannot form 3 using only coins of denomination 2.

---

## 2. Key Observation

This problem is different from Coin Change I.

| Coin Change I | Coin Change II |
|---|---|
| Find minimum number of coins | Count distinct combinations |
| Minimize the number of coins | Count possible ways |
| Return -1 if impossible | Return 0 if impossible |

The most important observation:

**We need combinations, not permutations.**

For example:

```text
coins = [1,2]
amount = 3
```

Valid combinations:

```text
1 + 1 + 1
1 + 2
```

We should not count `2 + 1` separately from `1 + 2`.

Therefore, we process coins one by one to avoid counting different orders as separate combinations.

---

## 3. Brute Force Approach — Recursion

### Idea

For every coin, we have two choices:

1. Take the current coin.
2. Skip the current coin.

We recursively explore both possibilities.

### Recursive State

```cpp
solve(index, amount)
```

Where:

- `index` represents the current coin.
- `amount` represents the remaining amount.

### Choice 1: Take the Current Coin

If the current coin is not greater than the remaining amount, we can take it.

```cpp
solve(index, amount - coins[index])
```

Notice that we do not increase `index`.

Why?

Because we have an unlimited supply of each coin, so we can use the same coin again.

### Choice 2: Skip the Current Coin

Move to the next denomination.

```cpp
solve(index + 1, amount)
```

### Base Cases

**Case 1: Amount becomes zero**

We found one valid combination.

```cpp
if (amount == 0) return 1;
```

**Case 2: All coins are exhausted**

No more combinations can be formed.

```cpp
if (index == coins.size()) return 0;
```

### C++ Code

```cpp
class Solution {
public:

    int solve(int index, int amount, vector<int>& coins) {

        if (amount == 0) {
            return 1;
        }

        if (index == coins.size()) {
            return 0;
        }

        int take = 0;

        if (coins[index] <= amount) {
            take = solve(
                index,
                amount - coins[index],
                coins
            );
        }

        int skip = solve(
            index + 1,
            amount,
            coins
        );

        return take + skip;
    }

    int change(int amount, vector<int>& coins) {

        return solve(0, amount, coins);
    }
};
```

### Complexity Analysis

- **Time:** Exponential in the worst case.
- **Auxiliary Space:** O(amount + n), due to recursion depth.

### Limitation

The same states are calculated repeatedly.

We can optimize this using memoization.

---

## 4. Better Approach — DP with Memoization

### Idea

Our recursive function depends on two variables:

- `index`
- `amount`

Therefore, we can store previously calculated answers in a 2D DP array.

### DP State

```cpp
dp[index][amount]
```

Meaning:

The number of combinations required to form the remaining amount using coins from `index` onward.

### Recurrence

**Take the current coin:**

```cpp
dp[index][amount - coins[index]]
```

We stay at the same index because unlimited coins are available.

**Skip the current coin:**

```cpp
dp[index + 1][amount]
```

Move to the next coin.

Therefore:

```cpp
dp[index][amount] =
    take + skip;
```

### C++ Code

```cpp
class Solution {
public:

    int solve(
        int index,
        int amount,
        vector<int>& coins,
        vector<vector<int>>& dp
    ) {

        if (amount == 0) {
            return 1;
        }

        if (index == coins.size()) {
            return 0;
        }

        if (dp[index][amount] != -1) {
            return dp[index][amount];
        }

        int take = 0;

        if (coins[index] <= amount) {
            take = solve(
                index,
                amount - coins[index],
                coins,
                dp
            );
        }

        int skip = solve(
            index + 1,
            amount,
            coins,
            dp
        );

        return dp[index][amount] = take + skip;
    }

    int change(int amount, vector<int>& coins) {

        int n = coins.size();

        vector<vector<int>> dp(
            n,
            vector<int>(amount + 1, -1)
        );

        return solve(0, amount, coins, dp);
    }
};
```

### Complexity Analysis

- **Time:** O(n × amount)
- **Auxiliary Space:** O(n × amount) for the DP table.
- **Recursion Stack:** O(n + amount) in the worst case.

---

## 5. Optimized Approach — Bottom-Up DP (Tabulation)

### Idea

Instead of solving the problem recursively, we build the answer using a DP table.

Define:

```cpp
dp[i][j]
```

As the number of combinations to make amount `j` using the first `i` coin denominations.

### Initialization

```cpp
dp[i][0] = 1;
```

Why?

There is exactly one way to make amount zero: choose no coins.

### Recurrence

For every coin, we have two choices.

**Case 1: Do not take the current coin**

```cpp
dp[i - 1][j]
```

**Case 2: Take the current coin**

```cpp
dp[i][j - coins[i - 1]]
```

We stay in the same row because unlimited coins are available.

Therefore:

```cpp
dp[i][j] =
    dp[i - 1][j] +
    dp[i][j - coins[i - 1]];
```

If the coin value is greater than the current amount, we cannot take it.

```cpp
dp[i][j] = dp[i - 1][j];
```

### C++ Code

```cpp
class Solution {
public:

    int change(int amount, vector<int>& coins) {

        int n = coins.size();

        vector<vector<int>> dp(
            n + 1,
            vector<int>(amount + 1, 0)
        );

        // One way to make amount 0
        for (int i = 0; i <= n; i++) {
            dp[i][0] = 1;
        }

        for (int i = 1; i <= n; i++) {

            for (int j = 1; j <= amount; j++) {

                // Do not take the current coin
                dp[i][j] = dp[i - 1][j];

                // Take the current coin
                if (coins[i - 1] <= j) {

                    dp[i][j] += dp[i][j - coins[i - 1]];
                }
            }
        }

        return dp[n][amount];
    }
};
```

### Complexity Analysis

- **Time:** O(n × amount)
- **Space:** O(n × amount)

---

## 6. Most Optimized Approach — 1D DP

This is the recommended solution for interviews.

### Key Observation

In the 2D DP recurrence:

```cpp
dp[i][j] =
    dp[i - 1][j] +
    dp[i][j - coins[i - 1]];
```

We need:

- The previous row's value at the same amount.
- The current row's value at a smaller amount.

We can optimize the space by using only one DP array.

### DP State

```cpp
dp[j]
```

Represents the number of combinations that make amount `j` using the coins processed so far.

### Initialization

```cpp
vector<int> dp(amount + 1, 0);

dp[0] = 1;
```

There is exactly one way to make zero: select no coins.

### Transition

For every coin:

```cpp
for (int coin : coins) {
    for (int j = coin; j <= amount; j++) {

        dp[j] += dp[j - coin];
    }
}
```

### Why Do We Process Coins in the Outer Loop?

Because we want combinations, not permutations.

Processing one coin at a time ensures that previously processed denominations are not rearranged into new combinations.

### Why Does the Inner Loop Go Forward?

```cpp
for (int j = coin; j <= amount; j++)
```

Because each coin can be used unlimited times.

When we move forward, `dp[j - coin]` may already include combinations using the current coin.

This allows us to reuse the same denomination.

### C++ Code

```cpp
class Solution {
public:

    int change(int amount, vector<int>& coins) {

        vector<int> dp(amount + 1, 0);

        // One way to make amount 0
        dp[0] = 1;

        for (int coin : coins) {

            for (int j = coin; j <= amount; j++) {

                dp[j] += dp[j - coin];
            }
        }

        return dp[amount];
    }
};
```

### Complexity Analysis

- **Time:** O(n × amount)
- **Auxiliary Space:** O(amount)

This is the optimal space-efficient solution.

---

## 7. Dry Run — 1D DP

Input:

```text
amount = 4
coins = [1,2,3]
```

Initialization:

```text
dp = [1,0,0,0,0]
```

Index represents the target amount.

| Amount | 0 | 1 | 2 | 3 | 4 |
|---|---:|---:|---:|---:|---:|
| Initial | 1 | 0 | 0 | 0 | 0 |

### Step 1: Process Coin = 1

For every amount from 1 to 4:

```cpp
dp[j] += dp[j - 1];
```

Updated DP:

```text
[1,1,1,1,1]
```

Explanation:

Using only coin 1:

```text
Amount 1 = 1
Amount 2 = 1 + 1
Amount 3 = 1 + 1 + 1
Amount 4 = 1 + 1 + 1 + 1
```

### Step 2: Process Coin = 2

Start from amount 2.

```cpp
dp[2] += dp[0];
dp[3] += dp[1];
dp[4] += dp[2];
```

Updated DP:

```text
[1,1,2,2,3]
```

Explanation:

For amount 4, we now have three combinations:

```text
1 + 1 + 1 + 1

1 + 1 + 2

2 + 2
```

### Step 3: Process Coin = 3

Start from amount 3.

```cpp
dp[3] += dp[0];
dp[4] += dp[1];
```

Updated DP:

```text
[1,1,2,3,4]
```

### Final DP Table

| Processed Coins | 0 | 1 | 2 | 3 | 4 |
|---|---:|---:|---:|---:|---:|
| Initial | 1 | 0 | 0 | 0 | 0 |
| Coin 1 | 1 | 1 | 1 | 1 | 1 |
| Coin 2 | 1 | 1 | 2 | 2 | 3 |
| Coin 3 | 1 | 1 | 2 | 3 | 4 |

Final answer:

```cpp
return dp[4];
```

Output:

```text
4
```

---

## 8. Important Difference — Combination vs Permutation

This is one of the most important concepts in this problem.

Consider:

```text
amount = 4
coins = [1,2,3]
```

### Combinations

```text
1 + 1 + 1 + 1
1 + 1 + 2
2 + 2
1 + 3
```

Total = 4.

### Permutations

If order mattered, these would be counted separately:

```text
1 + 1 + 2
1 + 2 + 1
2 + 1 + 1
```

But Coin Change II does not count them separately.

### Correct Loop Order for Combinations

```cpp
for (int coin : coins) {

    for (int j = coin; j <= amount; j++) {

        dp[j] += dp[j - coin];
    }
}
```

### Loop Order for Permutations

If order mattered, the amount would be the outer loop:

```cpp
for (int j = 1; j <= amount; j++) {

    for (int coin : coins) {

        if (coin <= j) {
            dp[j] += dp[j - coin];
        }
    }
}
```

**Interview takeaway:**

- Coins outer loop = combinations.
- Amount outer loop = permutations.

---

## 9. Common Mistakes

1. Using `j++` incorrectly as `j--` in the 1D DP loop.
2. Starting the inner loop from zero instead of the current coin value.
3. Using `dp[j - coin]` without checking that the amount is at least the coin value.
4. Confusing combinations with permutations.
5. Using `dp[i - 1][j - coin]` in the 2D recurrence instead of `dp[i][j - coin]` for unlimited coins.
6. Initializing `dp[0] = 0` instead of `dp[0] = 1`.
7. Moving to the next coin after taking a coin in the recursive approach, which would prevent unlimited reuse.

---

## 10. Important Interview Questions

### Q1. Why is dp[0] initialized to 1?

There is exactly one way to form amount zero: select no coins.

This also allows the first valid coin selection to contribute one combination.

### Q2. Why do we use the same row when taking a coin in 2D DP?

Because we can use the same coin multiple times.

```cpp
dp[i][j - coins[i - 1]]
```

The same coin remains available.

### Q3. Why does the 1D DP inner loop move forward?

Because coins can be reused unlimited times.

Moving forward allows the current coin to contribute repeatedly.

### Q4. What happens if we reverse the loop order?

We count permutations instead of combinations.

For example, `1 + 2` and `2 + 1` may be counted separately.

### Q5. What is the difference between Coin Change I and Coin Change II?

Coin Change I asks for the minimum number of coins.

Coin Change II asks for the number of distinct combinations.

### Q6. What DP pattern does this problem represent?

Unbounded Knapsack.

We can select an item repeatedly, and we need to count the ways to reach a target.

---

## 11. Approach Comparison

| Approach | Time | Auxiliary Space |
|---|---|---|
| Recursion | Exponential | O(n + amount) stack |
| Memoization | O(n × amount) | O(n × amount) + stack |
| 2D Tabulation | O(n × amount) | O(n × amount) |
| 1D Tabulation | O(n × amount) | O(amount) |

---

## 12. Related Problems

| Problem | Main Concept |
|---|---|
| Coin Change | Minimum number of coins |
| Coin Change II | Number of combinations |
| Combination Sum | Unlimited reuse with backtracking |
| Combination Sum IV | Number of ordered combinations |
| Partition Equal Subset Sum | 0/1 Knapsack |
| Target Sum | Subset Sum / Counting DP |

---

## 13. Final Revision Notes

### DP State

```cpp
dp[j] = number of combinations to make amount j
```

### Initialization

```cpp
dp[0] = 1;
```

### Transition

```cpp
dp[j] += dp[j - coin];
```

### Loop Order

```cpp
for (int coin : coins) {

    for (int j = coin; j <= amount; j++) {

        dp[j] += dp[j - coin];
    }
}
```

### Complexity

```text
Time: O(n × amount)

Space: O(amount)
```

### Most Important Takeaways

1. This is an Unbounded Knapsack problem.
2. Each coin can be used unlimited times.
3. We need combinations, not permutations.
4. Coins must be processed in the outer loop.
5. The amount loop must move forward.
6. `dp[0] = 1` is essential.
7. 1D DP provides O(amount) auxiliary space.

**Recommended Approach:** 1D Bottom-Up DP.

**Interview Pattern:** Whenever a problem asks for the number of ways to form a target using reusable elements, consider Unbounded Knapsack DP.

# Target Sum — LeetCode 494

- **Difficulty:** Medium
- **Topics:** Array, Dynamic Programming
- **Pattern:** 0/1 Knapsack / Subset Sum / Counting DP
- **Goal:** Count the number of ways to assign `+` and `-` signs to array elements so that their sum equals the target.

## 1. Problem Understanding

We are given:

```cpp
vector<int> nums;
int target;
```

For every number in the array, we must choose either:

- Add the number using `+`.
- Subtract the number using `-`.

We need to return the total number of different expressions that produce the given target.

### Example 1

```text
Input:
nums = [2,2,2]
target = 2

Output:
3
```

Valid expressions:

```text
+2 +2 -2 = 2

+2 -2 +2 = 2

-2 +2 +2 = 2
```

Total ways = 3.

### Example 2

```text
Input:
nums = [1,1]
target = 0

Output:
2
```

Valid expressions:

```text
+1 -1 = 0

-1 +1 = 0
```

Total ways = 2.

### Important Observation

Every element must be used exactly once.

Unlike Coin Change II, we cannot reuse an element multiple times.

Also, duplicate values at different indices are considered separate choices.

For example:

```text
nums = [1,1]
target = 0
```

The two different sign assignments are counted separately.

---

## 2. Brute Force Approach — Recursion

### Idea

For every element, we have exactly two choices:

1. Add the element.
2. Subtract the element.

We explore both possibilities recursively.

### Recursive State

```cpp
solve(index, currentSum)
```

Where:

- `index` represents the current array position.
- `currentSum` represents the sum calculated so far.

### Choice 1: Add the Current Element

```cpp
solve(index + 1, currentSum + nums[index])
```

### Choice 2: Subtract the Current Element

```cpp
solve(index + 1, currentSum - nums[index])
```

### Base Case

When all elements have been processed:

```cpp
if (index == nums.size()) {
    return currentSum == target;
}
```

If the current sum equals the target, return 1.

Otherwise, return 0.

### C++ Code

```cpp
class Solution {
public:

    int solve(
        int index,
        int currentSum,
        vector<int>& nums,
        int target
    ) {

        if (index == nums.size()) {
            return currentSum == target;
        }

        int add = solve(
            index + 1,
            currentSum + nums[index],
            nums,
            target
        );

        int subtract = solve(
            index + 1,
            currentSum - nums[index],
            nums,
            target
        );

        return add + subtract;
    }

    int findTargetSumWays(vector<int>& nums, int target) {

        return solve(0, 0, nums, target);
    }
};
```

### Complexity Analysis

- **Time:** O(2^n)
- **Auxiliary Space:** O(n) for recursion stack.

### Limitation

For every element, we explore two possibilities.

This produces an exponential number of recursive calls.

We can optimize this using memoization.

---

## 3. Better Approach — DP with Memoization

### Idea

In recursion, the same state can be calculated multiple times.

A state is determined by:

```cpp
(index, currentSum)
```

We can store the answer for each state in a DP structure.

### DP State

```cpp
dp[index][currentSum]
```

Meaning:

The number of ways to reach the target using the remaining elements, given the current sum.

Since the current sum can be negative, a normal array cannot directly use it as an index.

One solution is to use an `unordered_map`.

### C++ Code

```cpp
class Solution {
public:

    unordered_map<string, int> memo;

    int solve(
        int index,
        int currentSum,
        vector<int>& nums,
        int target
    ) {

        if (index == nums.size()) {
            return currentSum == target;
        }

        string key =
            to_string(index) + "," + to_string(currentSum);

        if (memo.count(key)) {
            return memo[key];
        }

        int add = solve(
            index + 1,
            currentSum + nums[index],
            nums,
            target
        );

        int subtract = solve(
            index + 1,
            currentSum - nums[index],
            nums,
            target
        );

        return memo[key] = add + subtract;
    }

    int findTargetSumWays(vector<int>& nums, int target) {

        memo.clear();

        return solve(0, 0, nums, target);
    }
};
```

### Complexity Analysis

Let:

- `n` = number of elements.
- `S` = sum of all elements.

The possible current sums range from `-S` to `+S`.

- **Time:** O(n × S) expected with hash-map operations.
- **Auxiliary Space:** O(n × S) for memoization, plus recursion stack.

---

## 4. Optimized Approach — Convert to Subset Sum

This is the most important mathematical observation in this problem.

Instead of considering positive and negative signs separately, we can transform the problem into a subset sum problem.

### Step 1: Divide the Array into Two Groups

Let:

- `P` = sum of elements assigned positive signs.
- `N` = sum of elements assigned negative signs.

We know:

```text
P - N = target
```

Also, every element must be used exactly once:

```text
P + N = totalSum
```

Where:

```cpp
totalSum = accumulate(nums.begin(), nums.end(), 0);
```

### Step 2: Add Both Equations

```text
P - N = target

P + N = totalSum
------------------

2P = totalSum + target
```

Therefore:

```text
P = (totalSum + target) / 2
```

### Final Transformation

Now the problem becomes:

**Count the number of subsets whose sum equals:**

```text
subsetTarget = (totalSum + target) / 2
```

This is a standard 0/1 Knapsack counting problem.

### Step 3: Check Invalid Conditions

Before applying DP, check:

**Condition 1: Target is too large**

```cpp
if (abs(target) > totalSum) {
    return 0;
}
```

We cannot create a target whose absolute value is greater than the sum of all elements.

**Condition 2: Invalid parity**

```cpp
if ((totalSum + target) % 2 != 0) {
    return 0;
}
```

The subset target must be an integer.

If the sum is odd, no valid partition exists.

---

## 5. Bottom-Up DP — 1D Tabulation

### DP State

```cpp
dp[j]
```

Represents the number of subsets that can produce sum `j` using the elements processed so far.

### Initialization

```cpp
vector<int> dp(subsetTarget + 1, 0);

dp[0] = 1;
```

Why?

There is exactly one way to form sum zero without selecting any elements: choose an empty subset.

### Transition

For each number:

```cpp
for (int j = subsetTarget; j >= num; j--) {

    dp[j] += dp[j - num];
}
```

### Why Does the Inner Loop Move Backward?

This is a 0/1 Knapsack problem.

Each element can be selected only once.

By iterating backward, we ensure that the current element is not reused during the same iteration.

If we iterate forward, the same element could contribute multiple times, which would be incorrect.

### C++ Code — Recommended Solution

```cpp
class Solution {
public:

    int findTargetSumWays(vector<int>& nums, int target) {

        int totalSum = 0;

        for (int num : nums) {
            totalSum += num;
        }

        // Target cannot exceed the total sum
        if (abs(target) > totalSum) {
            return 0;
        }

        // Subset target must be an integer
        if ((totalSum + target) % 2 != 0) {
            return 0;
        }

        int subsetTarget = (totalSum + target) / 2;

        vector<int> dp(subsetTarget + 1, 0);

        // One way to make sum 0: choose nothing
        dp[0] = 1;

        for (int num : nums) {

            for (int j = subsetTarget; j >= num; j--) {

                dp[j] += dp[j - num];
            }
        }

        return dp[subsetTarget];
    }
};
```

### Complexity Analysis

Let:

- `n` = number of elements.
- `S` = sum of all elements.

- **Time:** O(n × subsetTarget), bounded by O(n × S).
- **Auxiliary Space:** O(subsetTarget), bounded by O(S).

This is the recommended solution.

---

## 6. Dry Run — Subset Sum DP

Input:

```text
nums = [2,2,2]
target = 2
```

### Step 1: Calculate Total Sum

```text
totalSum = 2 + 2 + 2

totalSum = 6
```

### Step 2: Calculate Subset Target

Formula:

```text
subsetTarget = (totalSum + target) / 2
```

Substitute:

```text
subsetTarget = (6 + 2) / 2

subsetTarget = 4
```

Now we need to count the number of subsets whose sum equals 4.

### Step 3: Initialize DP

```text
dp = [1,0,0,0,0]
```

The indices represent sums from 0 to 4.

| Sum | 0 | 1 | 2 | 3 | 4 |
|---|---:|---:|---:|---:|---:|
| Initial DP | 1 | 0 | 0 | 0 | 0 |

### Step 4: Process First Element (2)

Backward iteration:

```text
j = 4:
dp[4] += dp[2]

j = 3:
dp[3] += dp[1]

j = 2:
dp[2] += dp[0]
```

Updated DP:

```text
[1,0,1,0,0]
```

### Step 5: Process Second Element (2)

Updated DP:

```text
[1,0,2,0,1]
```

Explanation:

There are two ways to form sum 2 because there are two different elements with value 2.

There is one way to form sum 4 by selecting both elements.

### Step 6: Process Third Element (2)

Updated DP:

```text
[1,0,3,0,3]
```

Final DP:

| Processed Elements | 0 | 1 | 2 | 3 | 4 |
|---|---:|---:|---:|---:|---:|
| Initial | 1 | 0 | 0 | 0 | 0 |
| First 2 | 1 | 0 | 1 | 0 | 0 |
| Second 2 | 1 | 0 | 2 | 0 | 1 |
| Third 2 | 1 | 0 | 3 | 0 | 3 |

Final answer:

```cpp
return dp[4];
```

Output:

```text
3
```

### Why Are There Three Ways?

We need to select two elements out of three:

```text
Index 0 + Index 1

Index 0 + Index 2

Index 1 + Index 2
```

Each selection produces a positive subset with sum 4.

The remaining element receives a negative sign.

---

## 7. Important Edge Cases

### Case 1: Target Greater Than Total Sum

```text
nums = [1,2,3]
target = 10
```

Total sum = 6.

Since:

```text
abs(target) > totalSum
```

Return:

```text
0
```

### Case 2: Invalid Parity

```text
nums = [1,2,3]
target = 1
```

Total sum = 6.

```text
subsetTarget = (6 + 1) / 2
```

The numerator is odd.

Return:

```text
0
```

### Case 3: Zero Elements

```text
nums = [0,0,1]
target = 1
```

There are four valid ways:

```text
+0 +0 +1

+0 -0 +1

-0 +0 +1

-0 -0 +1
```

Output:

```text
4
```

### Why Does the DP Handle Zeros Correctly?

When `num = 0`:

```cpp
dp[j] += dp[j - 0];
```

This becomes:

```cpp
dp[j] += dp[j];
```

Therefore, every existing count doubles.

This is correct because a zero can receive either a positive or negative sign, creating two distinct choices.

**Important:** The inner loop must include `j = 0` when processing zero. The condition `j >= num` ensures that it does.

---

## 8. Common Mistakes

1. Forgetting that every array element must be used exactly once.
2. Treating duplicate values as identical choices.
3. Using forward iteration in 1D subset sum DP.
4. Forgetting to check whether `abs(target) > totalSum`.
5. Forgetting the parity condition.
6. Incorrectly initializing `dp[0] = 0` instead of `dp[0] = 1`.
7. Ignoring zero elements.
8. Using the wrong subset transformation formula.
9. Confusing 0/1 Knapsack with Unbounded Knapsack.

### Most Important Loop Difference

**0/1 Knapsack — each element used once:**

```cpp
for (int j = target; j >= num; j--)
```

**Unbounded Knapsack — elements can be reused:**

```cpp
for (int j = num; j <= target; j++)
```

---

## 9. Approach Comparison

| Approach | Time | Auxiliary Space |
|---|---|---|
| Brute Force Recursion | O(2^n) | O(n) stack |
| Memoization | O(n × S) expected | O(n × S) + stack |
| 1D Subset Sum DP | O(n × subsetTarget) | O(subsetTarget) |

Where `S` is the sum of all array elements.

---

## 10. Related Problems

| Problem | Pattern |
|---|---|
| Partition Equal Subset Sum | 0/1 Knapsack |
| Target Sum | Subset Sum Counting |
| Coin Change | Unbounded Knapsack / Minimum |
| Coin Change II | Unbounded Knapsack / Counting |
| Combination Sum | Backtracking |
| Last Stone Weight II | Partition / Subset Sum |
| Ones and Zeroes | 0/1 Knapsack |

---

## 11. Important Interview Questions

### Q1. Why can Target Sum be converted into Subset Sum?

Because we can divide the array into positive and negative groups.

Using:

```text
P - N = target

P + N = totalSum
```

We derive:

```text
P = (totalSum + target) / 2
```

Therefore, we only need to count subsets with this sum.

### Q2. Why do we iterate backward in 1D DP?

Because every array element can be selected only once.

Backward iteration prevents the current element from being reused.

### Q3. Why is dp[0] initialized to 1?

There is exactly one way to form sum zero without selecting any elements.

This initialization also allows the first valid subset to be counted.

### Q4. Why do zeros double the number of ways?

Because zero can be assigned either `+` or `-`.

Both choices produce the same numerical sum but represent different expressions.

### Q5. What happens if the total sum and target have different parity?

No valid subset target can be calculated as an integer.

Therefore, return zero.

### Q6. What is the difference between Target Sum and Coin Change II?

| Target Sum | Coin Change II |
|---|---|
| Each element is used exactly once | Coins can be reused |
| Positive or negative sign | Select or skip coin |
| 0/1 Knapsack | Unbounded Knapsack |
| Inner loop backward | Inner loop forward |

---

## 12. Final Revision Notes

### Mathematical Formula

```text
subsetTarget = (totalSum + target) / 2
```

### Validation

```cpp
if (abs(target) > totalSum) return 0;

if ((totalSum + target) % 2 != 0) return 0;
```

### DP Initialization

```cpp
vector<int> dp(subsetTarget + 1, 0);

dp[0] = 1;
```

### DP Transition

```cpp
dp[j] += dp[j - num];
```

### Loop Order

```cpp
for (int num : nums) {

    for (int j = subsetTarget; j >= num; j--) {

        dp[j] += dp[j - num];
    }
}
```

### Complexity

```text
Time: O(n × subsetTarget)

Space: O(subsetTarget)
```

### Most Important Takeaways

1. Convert the sign assignment problem into subset sum.
2. Use the formula `(totalSum + target) / 2`.
3. Check target range and parity before DP.
4. This is a 0/1 Knapsack counting problem.
5. Iterate backward because each element can be used only once.
6. Zero elements correctly double the number of ways.
7. Duplicate values at different indices represent different choices.

**Recommended Approach:** 1D Subset Sum DP.

**Interview Pattern:** When a problem asks for the number of ways to assign signs or partition elements to reach a target, consider transforming it into a subset sum counting problem.

# Interleaving String — LeetCode 97

- **Difficulty:** Medium
- **Topics:** String, Dynamic Programming
- **Pattern:** 2D DP / String DP
- **Goal:** Determine whether `s3` can be formed by interleaving `s1` and `s2`.

## 1. Problem Understanding

We are given three strings:

```cpp
string s1;
string s2;
string s3;
```

We need to determine whether `s3` can be formed by interleaving `s1` and `s2`.

### What Does Interleaving Mean?

Interleaving means combining characters from two strings while maintaining the original relative order of characters in each string.

We can take a character from either string at every step.

However, we cannot change the order of characters within `s1` or `s2`.

### Example 1

```text
Input:
s1 = "aabcc"
s2 = "dbbca"
s3 = "aadbbcbcac"

Output:
true
```

One possible interleaving is:

```text
s1: a a b c c
s2:     d b b c a

s3: a a d b b c b c a c
```

The characters from both strings are used in their original relative order.

### Example 2

```text
Input:
s1 = "aabcc"
s2 = "dbbca"
s3 = "aadbbbaccc"

Output:
false
```

The characters cannot be combined while preserving the order of both original strings.

### Important Observation

We must use every character from both `s1` and `s2` exactly once.

The following length condition is necessary:

```cpp
if (s1.size() + s2.size() != s3.size()) {
    return false;
}
```

If the lengths do not match, interleaving is impossible.

---

## 2. Brute Force Approach — Recursion

### Idea

At every position in `s3`, we have two possible choices:

1. Take the next character from `s1`.
2. Take the next character from `s2`.

We check whether either choice can successfully form the remaining part of `s3`.

### Recursive State

```cpp
solve(i, j)
```

Where:

- `i` = current index in `s1`.
- `j` = current index in `s2`.

The corresponding index in `s3` is:

```cpp
k = i + j;
```

Why?

Because we have already selected `i` characters from `s1` and `j` characters from `s2`.

Therefore, we have selected `i + j` characters from `s3`.

### Choice 1: Take Character from s1

If:

```cpp
s1[i] == s3[i + j]
```

We can move forward in `s1`:

```cpp
solve(i + 1, j)
```

### Choice 2: Take Character from s2

If:

```cpp
s2[j] == s3[i + j]
```

We can move forward in `s2`:

```cpp
solve(i, j + 1)
```

### Base Case

If both strings are completely processed:

```cpp
if (i == s1.size() && j == s2.size()) {
    return true;
}
```

### C++ Code

```cpp
class Solution {
public:

    bool solve(
        int i,
        int j,
        string& s1,
        string& s2,
        string& s3
    ) {

        if (i == s1.size() && j == s2.size()) {
            return true;
        }

        int k = i + j;

        bool takeS1 = false;
        bool takeS2 = false;

        if (i < s1.size() && s1[i] == s3[k]) {
            takeS1 = solve(i + 1, j, s1, s2, s3);
        }

        if (j < s2.size() && s2[j] == s3[k]) {
            takeS2 = solve(i, j + 1, s1, s2, s3);
        }

        return takeS1 || takeS2;
    }

    bool isInterleave(string s1, string s2, string s3) {

        if (s1.size() + s2.size() != s3.size()) {
            return false;
        }

        return solve(0, 0, s1, s2, s3);
    }
};
```

### Complexity Analysis

In the worst case, every character can create two recursive choices.

- **Time:** O(2^(m+n))
- **Auxiliary Space:** O(m+n) recursion stack.

Where:

- `m` = length of `s1`.
- `n` = length of `s2`.

### Limitation

Different recursive paths can reach the same state `(i, j)`.

Recalculating these states repeatedly is unnecessary.

We can optimize this using Dynamic Programming.

---

## 3. Better Approach — DP with Memoization

### Idea

The recursive solution may calculate the same state multiple times.

We can store the result of each state:

```cpp
dp[i][j]
```

This avoids repeated calculations.

### DP State

```cpp
dp[i][j]
```

Represents whether the first `i` characters of `s1` and the first `j` characters of `s2` can form the first `i + j` characters of `s3`.

### State Transition

We have two possibilities.

**Choice 1: Take from s1**

If the current character of `s1` matches the corresponding character in `s3`:

```cpp
s1[i - 1] == s3[i + j - 1]
```

Then:

```cpp
dp[i][j] = dp[i - 1][j];
```

**Choice 2: Take from s2**

If the current character of `s2` matches the corresponding character in `s3`:

```cpp
s2[j - 1] == s3[i + j - 1]
```

Then:

```cpp
dp[i][j] = dp[i][j - 1];
```

Combining both:

```cpp
dp[i][j] =
    (s1[i - 1] == s3[i + j - 1] && dp[i - 1][j]) ||
    (s2[j - 1] == s3[i + j - 1] && dp[i][j - 1]);
```

### C++ Code — Memoization

```cpp
class Solution {
public:

    int dp[101][101];

    bool solve(
        int i,
        int j,
        string& s1,
        string& s2,
        string& s3
    ) {

        if (i == s1.size() && j == s2.size()) {
            return true;
        }

        if (dp[i][j] != -1) {
            return dp[i][j];
        }

        int k = i + j;

        bool takeS1 = false;
        bool takeS2 = false;

        if (i < s1.size() && s1[i] == s3[k]) {
            takeS1 = solve(i + 1, j, s1, s2, s3);
        }

        if (j < s2.size() && s2[j] == s3[k]) {
            takeS2 = solve(i, j + 1, s1, s2, s3);
        }

        return dp[i][j] = takeS1 || takeS2;
    }

    bool isInterleave(string s1, string s2, string s3) {

        if (s1.size() + s2.size() != s3.size()) {
            return false;
        }

        memset(dp, -1, sizeof(dp));

        return solve(0, 0, s1, s2, s3);
    }
};
```

### Complexity Analysis

There are at most `(m + 1) × (n + 1)` different states.

Each state is calculated once.

- **Time:** O(m × n)
- **Auxiliary Space:** O(m × n) for memoization, plus O(m+n) recursion stack.

---

## 4. Optimized Approach — Bottom-Up 2D DP

This is the standard iterative Dynamic Programming solution.

### Step 1: Define the DP Table

```cpp
vector<vector<bool>> dp(
    m + 1,
    vector<bool>(n + 1, false)
);
```

Meaning:

```cpp
dp[i][j]
```

is true if the first `i` characters of `s1` and first `j` characters of `s2` can form the first `i + j` characters of `s3`.

### Step 2: Base Case

```cpp
dp[0][0] = true;
```

Why?

Two empty strings can form an empty string.

### Step 3: First Column

When `j = 0`, we are only using characters from `s1`.

```cpp
for (int i = 1; i <= m; i++) {

    dp[i][0] =
        dp[i - 1][0] &&
        s1[i - 1] == s3[i - 1];
}
```

### Step 4: First Row

When `i = 0`, we are only using characters from `s2`.

```cpp
for (int j = 1; j <= n; j++) {

    dp[0][j] =
        dp[0][j - 1] &&
        s2[j - 1] == s3[j - 1];
}
```

### Step 5: Main DP Transition

For every cell, we check whether the current character can come from either string.

```cpp
for (int i = 1; i <= m; i++) {

    for (int j = 1; j <= n; j++) {

        dp[i][j] =
            (s1[i - 1] == s3[i + j - 1] && dp[i - 1][j]) ||
            (s2[j - 1] == s3[i + j - 1] && dp[i][j - 1]);
    }
}
```

### C++ Code — Recommended 2D DP Solution

```cpp
class Solution {
public:

    bool isInterleave(string s1, string s2, string s3) {

        int m = s1.size();
        int n = s2.size();

        if (m + n != s3.size()) {
            return false;
        }

        vector<vector<bool>> dp(
            m + 1,
            vector<bool>(n + 1, false)
        );

        dp[0][0] = true;

        // First column: use only s1
        for (int i = 1; i <= m; i++) {

            dp[i][0] =
                dp[i - 1][0] &&
                s1[i - 1] == s3[i - 1];
        }

        // First row: use only s2
        for (int j = 1; j <= n; j++) {

            dp[0][j] =
                dp[0][j - 1] &&
                s2[j - 1] == s3[j - 1];
        }

        // Fill the remaining DP table
        for (int i = 1; i <= m; i++) {

            for (int j = 1; j <= n; j++) {

                dp[i][j] =
                    (s1[i - 1] == s3[i + j - 1] &&
                     dp[i - 1][j]) ||

                    (s2[j - 1] == s3[i + j - 1] &&
                     dp[i][j - 1]);
            }
        }

        return dp[m][n];
    }
};
```

### Complexity Analysis

- **Time:** O(m × n)
- **Space:** O(m × n)

This is the recommended solution when learning the 2D DP state transition.

---

## 5. Further Optimization — 1D DP

We can reduce the space complexity from O(m × n) to O(n).

### Observation

In the 2D DP solution, each cell depends only on:

```cpp
dp[i - 1][j]  // Previous row

dp[i][j - 1]  // Current row, previous column
```

We do not need the entire DP table.

We can use a single 1D array.

### C++ Code — Space Optimized

```cpp
class Solution {
public:

    bool isInterleave(string s1, string s2, string s3) {

        int m = s1.size();
        int n = s2.size();

        if (m + n != s3.size()) {
            return false;
        }

        vector<bool> dp(n + 1, false);

        dp[0] = true;

        // Initialize the first row
        for (int j = 1; j <= n; j++) {

            dp[j] =
                dp[j - 1] &&
                s2[j - 1] == s3[j - 1];
        }

        // Process each character of s1
        for (int i = 1; i <= m; i++) {

            // Update the first column
            dp[0] =
                dp[0] &&
                s1[i - 1] == s3[i - 1];

            for (int j = 1; j <= n; j++) {

                bool takeS1 =
                    dp[j] &&
                    s1[i - 1] == s3[i + j - 1];

                bool takeS2 =
                    dp[j - 1] &&
                    s2[j - 1] == s3[i + j - 1];

                dp[j] = takeS1 || takeS2;
            }
        }

        return dp[n];
    }
};
```

### Understanding the 1D DP Update

Before updating `dp[j]`:

- `dp[j]` represents the previous row's value: `dp[i-1][j]`.
- `dp[j-1]` represents the current row's previous column: `dp[i][j-1]`.

Therefore:

```cpp
bool takeS1 = dp[j] && ...;

bool takeS2 = dp[j - 1] && ...;
```

After calculating both choices:

```cpp
dp[j] = takeS1 || takeS2;
```

### Complexity Analysis

- **Time:** O(m × n)
- **Space:** O(n)

This is the most space-efficient approach.

---

## 6. Dry Run — 2D DP

Input:

```text
s1 = "ab"
s2 = "cd"
s3 = "acbd"
```

Expected output:

```text
true
```

### Step 1: Calculate Lengths

```text
m = 2
n = 2

s3.length() = 4
```

Length condition:

```text
m + n = 2 + 2 = 4
```

The lengths match.

### Step 2: Initialize DP Table

Create a table of size `(m+1) × (n+1)`.

```text
       j
       0    1    2

i  0  false false false

   1  false false false

   2  false false false
```

Set:

```cpp
dp[0][0] = true;
```

### Step 3: Initialize First Column

We can form `"a"` using the first character of `s1`.

```text
s1[0] = 'a'
s3[0] = 'a'
```

Therefore:

```cpp
dp[1][0] = true;
```

Next:

```text
s1[1] = 'b'
s3[1] = 'c'
```

They do not match.

Therefore:

```cpp
dp[2][0] = false;
```

### Step 4: Initialize First Row

We can form `"c"` using the first character of `s2`.

```text
s2[0] = 'c'
s3[0] = 'a'
```

They do not match.

Therefore:

```cpp
dp[0][1] = false;
```

The remaining first-row state also remains false.

### Step 5: Fill Remaining Cells

For `dp[1][1]`:

```text
s1[0] = 'a'
s2[0] = 'c'
s3[1] = 'c'
```

We can take `'c'` from `s2`, because `dp[1][0]` is true.

Therefore:

```cpp
dp[1][1] = true;
```

For `dp[1][2]`:

```text
s2[1] = 'd'
s3[2] = 'b'
```

No valid transition exists.

Therefore:

```cpp
dp[1][2] = false;
```

For `dp[2][1]`:

```text
s1[1] = 'b'
s3[2] = 'b'
```

We can take `'b'` from `s1`, because `dp[1][1]` is true.

Therefore:

```cpp
dp[2][1] = true;
```

For `dp[2][2]`:

```text
s2[1] = 'd'
s3[3] = 'd'
```

We can take `'d'` from `s2`, because `dp[2][1]` is true.

Therefore:

```cpp
dp[2][2] = true;
```

### Final DP Table

```text
       0      1      2

0     true   false  false

1     true   true   false

2     false  true   true
```

Final answer:

```cpp
return dp[2][2];
```

Output:

```text
true
```

One valid interleaving is:

```text
s1: a   b
s2:   c   d

s3: a c b d
```

---

## 7. Important Edge Cases

### Case 1: Different Lengths

```text
s1 = "a"
s2 = "b"
s3 = "abc"
```

Output:

```text
false
```

Because:

```text
s1.length() + s2.length() != s3.length()
```

### Case 2: One String Is Empty

```text
s1 = ""
s2 = "abc"
s3 = "abc"
```

Output:

```text
true
```

All characters come from `s2`.

### Case 3: Both Strings Are Empty

```text
s1 = ""
s2 = ""
s3 = ""
```

Output:

```text
true
```

### Case 4: Repeated Characters

```text
s1 = "aa"
s2 = "ab"
s3 = "aaba"
```

Output:

```text
true
```

Repeated characters can create multiple possible paths, so we must consider both choices.

### Case 5: Character Order Is Violated

```text
s1 = "ab"
s2 = "cd"
s3 = "acdb"
```

Output:

```text
true
```

One valid sequence is:

```text
a (s1)
c (s2)
d (s2)
b (s1)
```

But consider:

```text
s1 = "ab"
s2 = "cd"
s3 = "adcb"
```

Output:

```text
false
```

The order of characters from `s2` would have to change from `"cd"` to `"dc"`.

---

## 8. Common Mistakes

1. Forgetting the length condition.
2. Checking only one string instead of considering both choices.
3. Using `s3[i]` instead of `s3[i + j]` in recursive solutions.
4. Using incorrect indices in the bottom-up transition.
5. Forgetting `dp[0][0] = true`.
6. Forgetting to initialize the first row and first column.
7. Assuming matching characters always produce a valid interleaving.
8. Using `&&` between the two choices instead of `||`.
9. Updating the 1D DP array without understanding which values represent the previous row and current row.

### Most Important Transition

```cpp
dp[i][j] =
    (s1[i - 1] == s3[i + j - 1] && dp[i - 1][j]) ||

    (s2[j - 1] == s3[i + j - 1] && dp[i][j - 1]);
```

Remember:

- `dp[i-1][j]` means we selected the character from `s1`.
- `dp[i][j-1]` means we selected the character from `s2`.

---

## 9. Approach Comparison

| Approach | Time | Space |
|---|---|---|
| Brute Force Recursion | O(2^(m+n)) | O(m+n) stack |
| Memoization | O(m × n) | O(m × n) + stack |
| Bottom-Up 2D DP | O(m × n) | O(m × n) |
| Space Optimized 1D DP | O(m × n) | O(n) |

Where:

- `m` = length of `s1`.
- `n` = length of `s2`.

---

## 10. Related Problems

| Problem | Pattern |
|---|---|
| Longest Common Subsequence | 2D String DP |
| Edit Distance | 2D String DP |
| Distinct Subsequences | Counting DP |
| Regular Expression Matching | 2D DP |
| Wildcard Matching | 2D DP |
| Word Break | String DP |
| Interleaving String | 2D DP |

---

## 11. Important Interview Questions

### Q1. What does dp[i][j] represent?

It represents whether the first `i` characters of `s1` and first `j` characters of `s2` can form the first `i+j` characters of `s3`.

### Q2. Why do we need two transitions?

Because the next character in `s3` can come from either `s1` or `s2`.

Both possibilities must be checked.

### Q3. Why do we use OR instead of AND?

Only one valid path is required.

If either choice produces a valid interleaving, the state is true.

### Q4. Why is the length condition necessary?

Every character from both strings must be used exactly once.

Therefore:

```text
s1.length() + s2.length() = s3.length()
```

### Q5. Why does this problem use 2D DP?

The state depends on two independent indices:

- Position in `s1`.
- Position in `s2`.

Therefore, a 2D state naturally represents the problem.

### Q6. Can we optimize the space complexity?

Yes. Each state depends only on the previous row and current row's previous column.

We can therefore use a 1D DP array.

---

## 12. Final Revision Notes

### Length Check

```cpp
if (s1.size() + s2.size() != s3.size()) {
    return false;
}
```

### DP State

```cpp
dp[i][j]
```

Whether the first `i` characters of `s1` and first `j` characters of `s2` form the first `i+j` characters of `s3`.

### Base Case

```cpp
dp[0][0] = true;
```

### Transition

```cpp
dp[i][j] =
    (s1[i - 1] == s3[i + j - 1] && dp[i - 1][j]) ||
    (s2[j - 1] == s3[i + j - 1] && dp[i][j - 1]);
```

### Final Answer

```cpp
return dp[m][n];
```

### Complexity

```text
2D DP:
Time: O(m × n)
Space: O(m × n)

1D DP:
Time: O(m × n)
Space: O(n)
```

### Most Important Takeaways

1. Interleaving preserves the relative order of both source strings.
2. Every character must be used exactly once.
3. The third string index is always `i + j`.
4. At each state, try taking a character from either string.
5. Use OR because either valid choice is sufficient.
6. The problem naturally maps to 2D Dynamic Programming.
7. Space can be optimized to O(n) using a 1D DP array.

**Recommended Approach:** Bottom-Up 2D DP first, followed by 1D space optimization.

**Interview Pattern:** When a problem asks whether one string can be formed by combining two strings while preserving their individual character order, consider a 2D DP state based on the two source-string indices.

# Distinct Subsequences — LeetCode 115

- **Difficulty:** Hard
- **Topics:** Dynamic Programming, Strings
- **Pattern:** 2D DP on Strings
- **Language:** C++

## 1. Problem Understanding

Given two strings `s` and `t`, return the number of distinct subsequences of `s` that are equal to `t`.

A subsequence is formed by deleting zero or more characters without changing the relative order of the remaining characters. Characters do not need to be contiguous.

### Examples

```text
Input:  s = "caaat", t = "cat"
Output: 3
```

```text
Input:  s = "xxyxv", t = "xy"
Output: 5
```

### Important Observations

- Relative character order must be maintained.
- Characters do not need to be adjacent.
- Different index selections count as different subsequences.
- We must count all possible ways, not merely check whether a match exists.

---

## 2. Brute Force — Recursion

At every position in `s`, we have two choices:

1. If the characters match, use the current character.
2. Skip the current character of `s`.

`solve(i, j)` represents the number of ways to form `t[j...]` using `s[i...]`.

### Base Cases

- If `j == t.size()`, the entire target is matched. Return `1`.
- If `i == s.size()` while the target is incomplete, return `0`.

### Recurrence

If characters match, add both choices:

```cpp
ways += solve(i + 1, j + 1, s, t); // Use s[i]
ways += solve(i + 1, j, s, t);     // Skip s[i]
```

If they do not match, only the skip choice is possible.

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, int j, string& s, string& t) {
        if (j == t.size()) return 1;
        if (i == s.size()) return 0;

        int ways = 0;

        if (s[i] == t[j]) {
            ways += solve(i + 1, j + 1, s, t);
        }

        ways += solve(i + 1, j, s, t);
        return ways;
    }

    int numDistinct(string s, string t) {
        return solve(0, 0, s, t);
    }
};
```

**Complexity**
- Time: `O(2^m)` upper bound
- Space: `O(m)` recursion stack

Here, `m = s.size()`. This approach repeats calculations for identical states.

---

## 3. Optimized Approach — Memoization

Store the result of each recursive state to avoid recalculating it.

### DP State

`dp[i][j]` = number of ways to form `t[j...]` using `s[i...]`.

Initialize the DP matrix with `-1`.

### Recurrence

If characters match:

```cpp
dp[i][j] = solve(i + 1, j + 1) + solve(i + 1, j);
```

Otherwise:

```cpp
dp[i][j] = solve(i + 1, j);
```

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, int j, string& s, string& t,
              vector<vector<int>>& dp) {
        if (j == t.size()) return 1;
        if (i == s.size()) return 0;

        if (dp[i][j] != -1) return dp[i][j];

        int ways = 0;

        if (s[i] == t[j]) {
            ways += solve(i + 1, j + 1, s, t, dp);
        }

        ways += solve(i + 1, j, s, t, dp);
        return dp[i][j] = ways;
    }

    int numDistinct(string s, string t) {
        int m = s.size(), n = t.size();
        vector<vector<int>> dp(m, vector<int>(n, -1));

        return solve(0, 0, s, t, dp);
    }
};
```

**Complexity**
- Time: `O(m * n)`
- Space: `O(m * n)` for DP, plus `O(m)` recursion stack.

---

## 4. Bottom-Up Approach — Tabulation

### Step 1: Define the DP State

`dp[i][j]` = number of ways to form the first `j` characters of `t` using the first `i` characters of `s`.

### Step 2: Initialize DP

Create a matrix of size `(m + 1) * (n + 1)`.

```cpp
vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
```

### Step 3: Base Cases

An empty target can always be formed by selecting no characters:

```cpp
for (int i = 0; i <= m; i++) {
    dp[i][0] = 1;
}
```

An empty source cannot form a non-empty target, so the other values in the first row remain zero.

### Step 4: Transition

If characters match, we can either use the current source character or skip it:

```cpp
dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
```

If they do not match, we must skip the current source character:

```cpp
dp[i][j] = dp[i - 1][j];
```

### C++ Code

```cpp
class Solution {
public:
    int numDistinct(string s, string t) {
        int m = s.size(), n = t.size();

        vector<vector<int>> dp(
            m + 1, vector<int>(n + 1, 0)
        );

        for (int i = 0; i <= m; i++) {
            dp[i][0] = 1;
        }

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (s[i - 1] == t[j - 1]) {
                    dp[i][j] =
                        dp[i - 1][j - 1] + dp[i - 1][j];
                } else {
                    dp[i][j] = dp[i - 1][j];
                }
            }
        }

        return dp[m][n];
    }
};
```

**Complexity**
- Time: `O(m * n)`
- Space: `O(m * n)`

---

## 5. Dry Run — Tabulation

**Input:** `s = "caaat"`, `t = "cat"`

Expected output: `3`

### Initial DP Matrix

| s / t | "" | c | a | t |
|---|---:|---:|---:|---:|
| "" | 1 | 0 | 0 | 0 |
| c | 1 | 0 | 0 | 0 |
| a | 1 | 0 | 0 | 0 |
| a | 1 | 0 | 0 | 0 |
| a | 1 | 0 | 0 | 0 |
| t | 1 | 0 | 0 | 0 |

### Processing Characters

**1. First character: `c`**

```text
dp[1][1] = dp[0][0] + dp[0][1]
         = 1 + 0
         = 1
```

**2. First `a`**

```text
dp[2][2] = dp[1][1] + dp[1][2]
         = 1 + 0
         = 1
```

**3. Second `a`**

```text
dp[3][2] = dp[2][1] + dp[2][2]
         = 0 + 1
         = 1
```

**4. Third `a`**

```text
dp[4][2] = dp[3][1] + dp[3][2]
         = 0 + 1
         = 1
```

**5. Final `t`**

```text
dp[5][3] = dp[4][2] + dp[4][3]
         = 3 + 0
         = 3
```

### Final DP Matrix

| s / t | "" | c | a | t |
|---|---:|---:|---:|---:|
| "" | 1 | 0 | 0 | 0 |
| c | 1 | 1 | 0 | 0 |
| a | 1 | 1 | 1 | 0 |
| a | 1 | 1 | 2 | 0 |
| a | 1 | 1 | 3 | 0 |
| t | 1 | 1 | 3 | 3 |

**Answer: `dp[5][3] = 3`.**

---

## 6. Space Optimization — 1D DP

### Main Idea

The 2D transition only uses the previous row:

```cpp
dp[i - 1][j - 1]
dp[i - 1][j]
```

Therefore, we can use a single DP array.

### DP State

`dp[j]` = number of ways to form the first `j` characters of `t` using the source characters processed so far.

### Initialization

```cpp
vector<int> dp(n + 1, 0);
dp[0] = 1;
```

### Transition

Process the target string backwards:

```cpp
for (int j = n; j >= 1; j--) {
    if (s[i] == t[j - 1]) {
        dp[j] += dp[j - 1];
    }
}
```

### Why Iterate Backwards?

Backward iteration ensures that `dp[j - 1]` still contains the result from the previous source character.

If we iterate forwards, an updated value could be reused in the same iteration, incorrectly using one source character multiple times.

### C++ Code — Space-Optimized Solution

```cpp
class Solution {
public:
    int numDistinct(string s, string t) {
        int m = s.size(), n = t.size();
        vector<int> dp(n + 1, 0);

        dp[0] = 1;

        for (int i = 0; i < m; i++) {
            for (int j = n; j >= 1; j--) {
                if (s[i] == t[j - 1]) {
                    dp[j] += dp[j - 1];
                }
            }
        }

        return dp[n];
    }
};
```

**Complexity**
- Time: `O(m * n)`
- Space: `O(n)`

This is the recommended space-optimized solution.

---

## 7. Approach Comparison

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Recursion | `O(2^m)` upper bound | `O(m)` stack |
| Memoization | `O(m * n)` | `O(m * n)` + stack |
| Tabulation | `O(m * n)` | `O(m * n)` |
| 1D DP | `O(m * n)` | `O(n)` |

---

## 8. Common Mistakes

1. Treating subsequences as contiguous substrings.
2. Forgetting that different source indices count as different ways.
3. Returning `0` when the target becomes empty. The correct value is `1`.
4. Forgetting the skip choice when characters match.
5. Iterating forwards in the 1D DP solution.
6. Confusing prefix-based and suffix-based DP definitions.
7. Forgetting to initialize `dp[i][0] = 1`.

---

## 9. Important Interview Questions

**Q1. Why do we have two choices when characters match?**

We can either use the current source character or skip it.

**Q2. Why does an empty target have one possible formation?**

Selecting no characters is exactly one way to form an empty string.

**Q3. Why do we return zero when the source is exhausted but the target is incomplete?**

There are no remaining source characters to complete the target.

**Q4. Why do we add two DP values when characters match?**

The two independent choices contribute separate counts, so their results must be added.

**Q5. Why is backward iteration necessary in 1D DP?**

It prevents the current source character from being reused multiple times in the same iteration.

**Q6. What is the difference between a subsequence and a substring?**

- **Subsequence:** Characters maintain relative order but need not be contiguous.
- **Substring:** Characters must be contiguous.

---

## 10. Final Revision Notes

### 2D DP Recurrence

```cpp
if (s[i - 1] == t[j - 1]) {
    dp[i][j] =
        dp[i - 1][j - 1] + dp[i - 1][j];
} else {
    dp[i][j] = dp[i - 1][j];
}
```

### Base Case

```cpp
dp[i][0] = 1;
```

### 1D DP Recurrence

```cpp
for (int j = n; j >= 1; j--) {
    if (s[i] == t[j - 1]) {
        dp[j] += dp[j - 1];
    }
}
```

### Key Takeaways

1. This is a counting DP problem.
2. Matching characters create two choices: match or skip.
3. Non-matching characters can only be skipped.
4. An empty target means exactly one valid formation.
5. Backward iteration is essential for 1D DP.

**Recommended Solution:** 1D DP with backward iteration.

**Interview Pattern:** Counting the number of ways to form one string from another while preserving character order.

# Edit Distance — LeetCode 72

- **Difficulty:** Medium
- **Topics:** Dynamic Programming, Strings
- **Pattern:** 2D DP on Two Strings
- **Language:** C++

## 1. Problem Understanding

Given two strings `word1` and `word2`, return the minimum number of operations required to make `word1` equal to `word2`.

We are allowed to perform three operations on `word1` an unlimited number of times:

1. **Insert:** Insert a character at any position.
2. **Delete:** Delete a character from any position.
3. **Replace:** Replace a character at any position.

### Example 1

```text
Input:  word1 = "monkeys", word2 = "money"
Output: 2
```

Explanation:

```text
monkeys -> monkey  (delete 's')
monkey  -> money   (delete 'k')
```

Total operations = 2.

### Example 2

```text
Input:  word1 = "neatcdee", word2 = "neetcode"
```

The goal is to find the minimum number of insertions, deletions, and replacements needed to transform the first string into the second.

### Important Observations

- We need the minimum number of operations, not the number of ways.
- We can insert, delete, or replace characters.
- The order of characters must match the target string.
- This is a classic 2D Dynamic Programming problem.

---

## 2. Brute Force Approach — Recursion

### Main Idea

Compare the characters from the end of both strings.

Let:

`solve(i, j)` = minimum operations required to convert `word1[0...i]` into `word2[0...j]`.

We have two possible situations.

### Case 1: Characters Match

If:

```cpp
word1[i] == word2[j]
```

No operation is required.

Move to the previous characters:

```cpp
solve(i - 1, j - 1);
```

### Case 2: Characters Do Not Match

We have three choices:

**1. Insert**

Insert the character `word2[j]` into `word1`.

```cpp
solve(i, j - 1);
```

**2. Delete**

Delete the current character from `word1`.

```cpp
solve(i - 1, j);
```

**3. Replace**

Replace the current character of `word1` with `word2[j]`.

```cpp
solve(i - 1, j - 1);
```

Take the minimum of all three operations and add one.

### Recurrence

```cpp
if (word1[i] == word2[j]) {
    return solve(i - 1, j - 1);
}

return 1 + min({
    solve(i, j - 1),     // Insert
    solve(i - 1, j),     // Delete
    solve(i - 1, j - 1)  // Replace
});
```

### Base Cases

**Case 1:** If `word1` becomes empty:

We must insert all remaining characters of `word2`.

```cpp
if (i < 0) return j + 1;
```

**Case 2:** If `word2` becomes empty:

We must delete all remaining characters of `word1`.

```cpp
if (j < 0) return i + 1;
```

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, int j, string& word1, string& word2) {

        if (i < 0) return j + 1;
        if (j < 0) return i + 1;

        if (word1[i] == word2[j]) {
            return solve(i - 1, j - 1, word1, word2);
        }

        int insertOp = solve(i, j - 1, word1, word2);
        int deleteOp = solve(i - 1, j, word1, word2);
        int replaceOp = solve(i - 1, j - 1, word1, word2);

        return 1 + min({
            insertOp,
            deleteOp,
            replaceOp
        });
    }

    int minDistance(string word1, string word2) {
        int m = word1.size();
        int n = word2.size();

        return solve(m - 1, n - 1, word1, word2);
    }
};
```

### Complexity

- **Time:** Exponential in the worst case.
- **Space:** `O(m + n)` recursion stack.

The same states are calculated multiple times, making this approach inefficient.

---

## 3. Optimized Approach — Memoization

### Main Idea

The recursive solution repeatedly calculates the same states.

We can store previously calculated answers in a DP matrix.

### DP State

```cpp
dp[i][j]
```

Represents the minimum operations required to convert:

```text
word1[0...i] into word2[0...j]
```

### Transition

If the characters match:

```cpp
dp[i][j] = dp[i - 1][j - 1];
```

If the characters do not match:

```cpp
dp[i][j] = 1 + min({
    dp[i][j - 1],     // Insert
    dp[i - 1][j],     // Delete
    dp[i - 1][j - 1]  // Replace
});
```

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, int j, string& word1, string& word2,
              vector<vector<int>>& dp) {

        if (i < 0) return j + 1;
        if (j < 0) return i + 1;

        if (dp[i][j] != -1) {
            return dp[i][j];
        }

        if (word1[i] == word2[j]) {
            return dp[i][j] =
                solve(i - 1, j - 1, word1, word2, dp);
        }

        int insertOp =
            solve(i, j - 1, word1, word2, dp);

        int deleteOp =
            solve(i - 1, j, word1, word2, dp);

        int replaceOp =
            solve(i - 1, j - 1, word1, word2, dp);

        return dp[i][j] = 1 + min({
            insertOp,
            deleteOp,
            replaceOp
        });
    }

    int minDistance(string word1, string word2) {
        int m = word1.size();
        int n = word2.size();

        vector<vector<int>> dp(
            m, vector<int>(n, -1)
        );

        return solve(m - 1, n - 1, word1, word2, dp);
    }
};
```

### Complexity

- **Time:** `O(m * n)`
- **Space:** `O(m * n)` for DP + `O(m + n)` recursion stack.

---

## 4. Bottom-Up Approach — Tabulation

### Main Idea

Instead of solving the problem recursively, we build the DP table iteratively.

We use a DP matrix of size:

```cpp
(m + 1) * (n + 1)
```

The extra row and column represent empty strings.

### Step 1: Define DP State

```cpp
dp[i][j]
```

Represents the minimum operations required to convert the first `i` characters of `word1` into the first `j` characters of `word2`.

### Step 2: Initialize DP

```cpp
vector<vector<int>> dp(
    m + 1, vector<int>(n + 1, 0)
);
```

### Step 3: Base Cases

If `word1` is empty, we must insert all characters of `word2`.

```cpp
for (int j = 0; j <= n; j++) {
    dp[0][j] = j;
}
```

If `word2` is empty, we must delete all characters of `word1`.

```cpp
for (int i = 0; i <= m; i++) {
    dp[i][0] = i;
}
```

### Step 4: Write the Transition

If characters match:

```cpp
dp[i][j] = dp[i - 1][j - 1];
```

If characters do not match:

```cpp
dp[i][j] = 1 + min({
    dp[i][j - 1],     // Insert
    dp[i - 1][j],     // Delete
    dp[i - 1][j - 1]  // Replace
});
```

### Why These Three Directions?

| Operation | Previous DP State | Meaning |
|---|---|---|
| Insert | `dp[i][j-1]` | Match the remaining target prefix, then insert the current target character |
| Delete | `dp[i-1][j]` | Delete the current character from `word1` |
| Replace | `dp[i-1][j-1]` | Replace the current character to match the target |

### C++ Code

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size();
        int n = word2.size();

        vector<vector<int>> dp(
            m + 1, vector<int>(n + 1, 0)
        );

        // Base cases
        for (int i = 0; i <= m; i++) {
            dp[i][0] = i;
        }

        for (int j = 0; j <= n; j++) {
            dp[0][j] = j;
        }

        // Fill the DP table
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {

                if (word1[i - 1] == word2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else {
                    dp[i][j] = 1 + min({
                        dp[i][j - 1],     // Insert
                        dp[i - 1][j],     // Delete
                        dp[i - 1][j - 1]  // Replace
                    });
                }
            }
        }

        return dp[m][n];
    }
};
```

### Complexity

- **Time:** `O(m * n)`
- **Space:** `O(m * n)`

---

## 5. Dry Run — Tabulation

**Input:**

```text
word1 = "monkeys"
word2 = "money"
```

Expected output: `2`

We need to transform:

```text
monkeys -> money
```

The minimum operations are:

```text
monkeys -> monkey  (Delete 's')
monkey  -> money   (Delete 'k')
```

Total = 2.

### DP Initialization

Rows represent prefixes of `word1`.

Columns represent prefixes of `word2`.

| | "" | m | o | n | e | y |
|---|---:|---:|---:|---:|---:|---:|
| "" | 0 | 1 | 2 | 3 | 4 | 5 |
| m | 1 | | | | | |
| o | 2 | | | | | |
| n | 3 | | | | | |
| k | 4 | | | | | |
| e | 5 | | | | | |
| y | 6 | | | | | |
| s | 7 | | | | | |

### Important Transitions

**1. Matching `m`**

```text
word1[0] = m
word2[0] = m

dp[1][1] = dp[0][0]
         = 0
```

**2. Matching `o`**

```text
dp[2][2] = dp[1][1]
         = 0
```

**3. Matching `n`**

```text
dp[3][3] = dp[2][2]
         = 0
```

**4. Character `k` does not match `e`**

```text
dp[4][4] = 1 + min(
    dp[4][3],
    dp[3][4],
    dp[3][3]
)
```

```text
dp[4][4] = 1 + min(1, 1, 1)
         = 2
```

**5. Matching `e`**

```text
dp[5][4] = dp[4][3]
         = 1
```

**6. Matching `y`**

```text
dp[6][5] = dp[5][4]
         = 1
```

The final `s` must be deleted:

```text
dp[7][5] = 2
```

### Final DP Matrix

| | "" | m | o | n | e | y |
|---|---:|---:|---:|---:|---:|---:|
| "" | 0 | 1 | 2 | 3 | 4 | 5 |
| m | 1 | 0 | 1 | 2 | 3 | 4 |
| o | 2 | 1 | 0 | 1 | 2 | 3 |
| n | 3 | 2 | 1 | 0 | 1 | 2 |
| k | 4 | 3 | 2 | 1 | 1 | 2 |
| e | 5 | 4 | 3 | 2 | 1 | 2 |
| y | 6 | 5 | 4 | 3 | 2 | 1 |
| s | 7 | 6 | 5 | 4 | 3 | 2 |

**Final Answer:**

```cpp
dp[7][5] = 2;
```

---

## 6. Space Optimization — 1D DP

### Main Idea

In the tabulation solution, we only need:

- Previous row: `dp[i - 1][j]`
- Previous diagonal: `dp[i - 1][j - 1]`
- Current row's previous column: `dp[i][j - 1]`

Therefore, we can optimize the 2D DP matrix into a 1D array.

### Important Concept

We maintain:

- `dp[j]` = previous row's value before updating it.
- `left` = current row's previous column.
- `diagonal` = previous row's previous diagonal value.

### C++ Code

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size();
        int n = word2.size();

        vector<int> dp(n + 1);

        // Base case: empty word1
        for (int j = 0; j <= n; j++) {
            dp[j] = j;
        }

        for (int i = 1; i <= m; i++) {

            int diagonal = dp[0];

            // Empty word2 requires i deletions
            dp[0] = i;

            for (int j = 1; j <= n; j++) {

                int up = dp[j];

                if (word1[i - 1] == word2[j - 1]) {
                    dp[j] = diagonal;
                }
                else {
                    dp[j] = 1 + min({
                        dp[j - 1],  // Insert: current row, left
                        up,         // Delete: previous row, up
                        diagonal    // Replace: previous diagonal
                    });
                }

                diagonal = up;
            }
        }

        return dp[n];
    }
};
```

### Complexity

- **Time:** `O(m * n)`
- **Space:** `O(n)`

This solution is space optimized compared with the 2D DP approach.

---

## 7. Approach Comparison

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Recursion | Exponential | `O(m + n)` stack |
| Memoization | `O(m * n)` | `O(m * n)` + stack |
| Tabulation | `O(m * n)` | `O(m * n)` |
| 1D DP | `O(m * n)` | `O(n)` |

Where:

- `m = word1.size()`
- `n = word2.size()`

---

## 8. Common Mistakes

1. Confusing insertion and deletion transitions.
2. Forgetting to add `1` when characters do not match.
3. Adding an operation when characters already match.
4. Incorrectly initializing the first row and first column.
5. Using `word1[i]` and `word2[j]` in 1-based tabulation instead of `word1[i - 1]` and `word2[j - 1]`.
6. Forgetting that converting an empty string into a string of length `n` requires `n` insertions.
7. Forgetting that converting a string of length `m` into an empty string requires `m` deletions.
8. Updating the diagonal value incorrectly in the 1D DP solution.

---

## 9. Important Interview Questions

**Q1. What is Edit Distance?**

It is the minimum number of insertions, deletions, and replacements needed to transform one string into another.

**Q2. Why do we use three operations in the recurrence?**

Because these are exactly the three allowed operations, and we choose the one that leads to the minimum total cost.

**Q3. Why do we add 1 to the minimum?**

Because one operation is performed at the current step.

**Q4. Why is there no additional cost when characters match?**

The current characters are already equal, so no operation is required.

**Q5. What happens if one string is empty?**

- Empty `word1`: insert all characters of `word2`.
- Empty `word2`: delete all characters of `word1`.

**Q6. Why is this a 2D DP problem?**

Because the answer depends on two changing positions: one in `word1` and one in `word2`.

**Q7. Can we optimize the space complexity?**

Yes. We only need the previous row and current row information, so we can use a 1D DP array.

**Q8. What is the difference between Edit Distance and Longest Common Subsequence?**

- Edit Distance minimizes the number of editing operations.
- LCS maximizes the length of a common subsequence.

---

## 10. Final Revision Notes

### DP State

```cpp
dp[i][j]
```

Minimum operations to convert the first `i` characters of `word1` into the first `j` characters of `word2`.

### Base Cases

```cpp
dp[i][0] = i;
dp[0][j] = j;
```

### Recurrence

```cpp
if (word1[i - 1] == word2[j - 1]) {
    dp[i][j] = dp[i - 1][j - 1];
}
else {
    dp[i][j] = 1 + min({
        dp[i][j - 1],
        dp[i - 1][j],
        dp[i - 1][j - 1]
    });
}
```

### Remember the Three Operations

| Operation | Transition |
|---|---|
| Insert | `dp[i][j - 1]` |
| Delete | `dp[i - 1][j]` |
| Replace | `dp[i - 1][j - 1]` |

### Key Takeaways

1. Edit Distance is a classic 2D DP problem.
2. Compare characters from both strings.
3. Matching characters require zero additional operations.
4. Mismatching characters require trying all three operations.
5. Take the minimum cost and add one.
6. The tabulation solution runs in `O(m * n)` time.
7. Space can be optimized to `O(n)`.

**Recommended Solution:** 1D Space-Optimized DP.

**Interview Pattern:** Minimum-cost transformation between two strings.

# Burst Balloons — LeetCode 312

- **Difficulty:** Hard
- **Topics:** Dynamic Programming, Arrays
- **Pattern:** Interval DP / Partition DP
- **Language:** C++

## 1. Problem Understanding

You are given an integer array `nums`, where each element represents a balloon with a specific coin value.

When you burst balloon `i`, you receive:

```cpp
nums[i - 1] * nums[i] * nums[i + 1]
```

If the left or right balloon does not exist, its value is considered `1`.

Return the maximum number of coins you can collect by bursting all balloons.

### Example

```text
Input:  nums = [4,2,3,7]
Output: 143
```

Explanation:

One optimal bursting order is:

```text
[4,2,3,7]
     |
Burst 2:
4 * 2 * 3 = 24

[4,3,7]
   |
Burst 3:
4 * 3 * 7 = 84

[4,7]
 |
Burst 4:
4 * 7 * 1 = 28

[4]
 |
Burst 4:
1 * 4 * 1 = 4
```

Total:

```text
24 + 84 + 28 + 4 = 140
```

The maximum is actually obtained by a different order:

```text
[4,2,3,7]

Burst 2: 4 * 2 * 3 = 24
Burst 4: 3 * 7 * 1 = 21
Burst 3: 4 * 3 * 1 = 12
Burst 4: 1 * 4 * 1 = 4
```

This order is not optimal either.

The optimal order is:

```text
[4,2,3,7]
-> [4,3,7]  : 4 * 2 * 3 = 24
-> [4,7]    : 4 * 3 * 7 = 84
-> [7]      : 1 * 4 * 7 = 28
-> []       : 1 * 7 * 1 = 7
```

Total:

```text
24 + 84 + 28 + 7 = 143
```

**Output: 143**

### Important Observation

The neighboring balloons change after every burst.

Therefore, simply choosing the balloon with the largest or smallest value first will not guarantee the maximum result.

This is why we use Interval DP.

---

## 2. Brute Force Approach — Try Every Bursting Order

### Main Idea

Try bursting every possible balloon first.

For each choice:

1. Calculate the coins received from bursting that balloon.
2. Remove the balloon from the array.
3. Recursively calculate the maximum coins from the remaining balloons.
4. Take the maximum result.

### C++ Code

```cpp
class Solution {
public:
    int solve(vector<int>& nums) {

        if (nums.empty()) {
            return 0;
        }

        int maxi = 0;

        for (int i = 0; i < nums.size(); i++) {

            int left = (i == 0) ? 1 : nums[i - 1];
            int right = (i == nums.size() - 1)
                        ? 1 : nums[i + 1];

            int coins = left * nums[i] * right;

            vector<int> remaining = nums;
            remaining.erase(remaining.begin() + i);

            coins += solve(remaining);

            maxi = max(maxi, coins);
        }

        return maxi;
    }

    int maxCoins(vector<int>& nums) {
        return solve(nums);
    }
};
```

### Complexity

- **Time:** Factorial / exponential, approximately `O(n!)` search.
- **Space:** `O(n)` recursion depth, excluding copied arrays.

This approach is too slow for the given constraints.

---

## 3. Optimized Approach — Interval DP

### Main Idea

The most important observation is:

Instead of deciding which balloon to burst first, decide which balloon to burst LAST in an interval.

Why?

When we burst a balloon, its neighboring balloons can change.

But if we decide that balloon `k` is the last balloon to burst between boundaries `l` and `r`, then its neighbors are guaranteed to be `l` and `r`.

This makes the calculation predictable.

### Step 1: Add Boundary Balloons

Add `1` at the beginning and end of the array.

Original:

```text
[4,2,3,7]
```

After padding:

```text
[1,4,2,3,7,1]
```

These two boundary values handle the cases where no balloon exists on the left or right.

### Step 2: Define DP State

```cpp
dp[l][r]
```

Represents the maximum coins we can collect by bursting all balloons strictly between indices `l` and `r`.

Important:

- Balloon at `l` is not burst.
- Balloon at `r` is not burst.
- Only balloons between them are burst.

### Step 3: Choose the Last Balloon

Suppose we choose index `k` as the last balloon to burst between `l` and `r`.

Before bursting `k`:

- Every balloon between `l` and `k` has already been burst.
- Every balloon between `k` and `r` has already been burst.
- Only balloons at `l`, `k`, and `r` remain relevant.

Therefore, the coins received from bursting `k` last are:

```cpp
nums[l] * nums[k] * nums[r]
```

The total coins are:

```cpp
dp[l][k] + dp[k][r]
+ nums[l] * nums[k] * nums[r]
```

Try every possible `k` and take the maximum.

### Step 4: Recurrence

```cpp
dp[l][r] = max(
    dp[l][k] +
    dp[k][r] +
    nums[l] * nums[k] * nums[r]
);
```

Where:

```text
l < k < r
```

### Step 5: Base Case

If there are no balloons between `l` and `r`, the answer is zero.

```cpp
if (l + 1 == r) {
    return 0;
}
```

---

## 4. Memoization — Top-Down DP

### C++ Code

```cpp
class Solution {
public:
    int solve(int l, int r, vector<int>& nums,
              vector<vector<int>>& dp) {

        // No balloon exists between l and r
        if (l + 1 == r) {
            return 0;
        }

        if (dp[l][r] != -1) {
            return dp[l][r];
        }

        int maxi = 0;

        // Choose the last balloon to burst
        for (int k = l + 1; k < r; k++) {

            int coins = nums[l] * nums[k] * nums[r];

            coins += solve(l, k, nums, dp);
            coins += solve(k, r, nums, dp);

            maxi = max(maxi, coins);
        }

        return dp[l][r] = maxi;
    }

    int maxCoins(vector<int>& nums) {

        int n = nums.size();

        // Add boundary balloons
        nums.insert(nums.begin(), 1);
        nums.push_back(1);

        vector<vector<int>> dp(
            n + 2, vector<int>(n + 2, -1)
        );

        return solve(0, n + 1, nums, dp);
    }
};
```

### Complexity

- **Time:** `O(n^3)`
- **Space:** `O(n^2)` for DP + `O(n)` recursion stack.

---

## 5. Tabulation — Bottom-Up Interval DP

### Main Idea

Convert the recursive solution into an iterative DP solution.

We need to calculate smaller intervals before larger intervals.

### Step 1: Initialize the Array

```cpp
nums.insert(nums.begin(), 1);
nums.push_back(1);
```

### Step 2: Create DP Table

```cpp
vector<vector<int>> dp(
    n + 2, vector<int>(n + 2, 0)
);
```

### Step 3: Iterate Over Interval Lengths

We start with smaller intervals and gradually calculate larger intervals.

```cpp
for (int len = 2; len <= n + 1; len++)
```

Here, `len` represents the distance between the two boundary indices.

### Step 4: Iterate Over Left and Right Boundaries

```cpp
int l = 0;
int r = l + len;
```

### Step 5: Try Every Possible Last Balloon

```cpp
for (int k = l + 1; k < r; k++)
```

### Step 6: Calculate Maximum Coins

```cpp
int coins = nums[l] * nums[k] * nums[r]
          + dp[l][k]
          + dp[k][r];
```

Take the maximum.

### C++ Code — Recommended Solution

```cpp
class Solution {
public:
    int maxCoins(vector<int>& nums) {

        int n = nums.size();

        // Add boundary balloons
        nums.insert(nums.begin(), 1);
        nums.push_back(1);

        vector<vector<int>> dp(
            n + 2, vector<int>(n + 2, 0)
        );

        // len = distance between l and r
        for (int len = 2; len <= n + 1; len++) {

            for (int l = 0; l + len <= n + 1; l++) {

                int r = l + len;

                // Try every possible last balloon
                for (int k = l + 1; k < r; k++) {

                    int coins =
                        nums[l] * nums[k] * nums[r]
                        + dp[l][k]
                        + dp[k][r];

                    dp[l][r] = max(dp[l][r], coins);
                }
            }
        }

        return dp[0][n + 1];
    }
};
```

### Complexity

- **Time:** `O(n^3)`
- **Space:** `O(n^2)`

This is the standard optimized solution.

---

## 6. Dry Run — Example

**Input:**

```cpp
nums = [4,2,3,7]
```

After adding boundary balloons:

```text
nums = [1,4,2,3,7,1]
```

Indices:

```text
Index:  0  1  2  3  4  5
Value:  1  4  2  3  7  1
```

We need:

```cpp
dp[0][5]
```

This represents the maximum coins obtained by bursting all balloons between index `0` and index `5`.

### Example: Calculate dp[0][3]

Interval:

```text
[1,4,2,3]
```

Possible last balloons:

**Choice 1: Burst index 1 last**

```text
1 * 4 * 3 = 12
```

Total:

```text
dp[0][1] + dp[1][3] + 12
```

**Choice 2: Burst index 2 last**

```text
1 * 2 * 3 = 6
```

Total:

```text
dp[0][2] + dp[2][3] + 6
```

**Choice 3: Burst index 3 last**

```text
1 * 3 * 1 = 3
```

Total:

```text
dp[0][3] + dp[3][3] + 3
```

The DP table stores the maximum among all possible choices.

### Optimal Bursting Order

For the original array:

```text
[4,2,3,7]
```

One optimal order is:

| Step | Balloon Burst | Coins |
|---|---|---:|
| 1 | 2 | 4 * 2 * 3 = 24 |
| 2 | 3 | 4 * 3 * 7 = 84 |
| 3 | 4 | 1 * 4 * 7 = 28 |
| 4 | 1 | 1 * 1 * 1 = 1 |

Total:

```text
24 + 84 + 28 + 1 = 137
```

Correction: the optimal order is:

| Step | Balloon Burst | Coins |
|---|---|---:|
| 1 | 4 | 3 * 7 * 1 = 21 |
| 2 | 2 | 4 * 2 * 3 = 24 |
| 3 | 3 | 4 * 3 * 1 = 12 |
| 4 | 4 | 1 * 4 * 1 = 4 |

This is not the optimal order either.

The optimal total is obtained by the interval DP recurrence, which returns:

```text
143
```

---

## 7. Why Do We Choose the Last Balloon?

This is the most important interview concept in Burst Balloons.

If we choose the first balloon:

- Its neighboring balloons may disappear later.
- The remaining subproblems are difficult to separate.

If we choose the last balloon:

- Both neighboring boundary balloons are still present.
- The left and right intervals become independent.
- We can combine their answers.

This is the key idea behind Interval DP.

### General Interval DP Pattern

When a problem involves dividing an interval into smaller intervals, consider:

1. What happens if I choose a position `k` as the last operation?
2. Can the left and right intervals be solved independently?
3. Can I combine their results with the cost of choosing `k`?

---

## 8. Common Mistakes

1. Forgetting to add `1` at both boundaries.
2. Choosing the first balloon instead of the last balloon.
3. Using the wrong neighboring values when calculating coins.
4. Including `l` or `r` in the range of balloons to burst.
5. Incorrectly defining the DP state.
6. Forgetting that `dp[l][r]` only considers balloons strictly between `l` and `r`.
7. Using an incorrect interval iteration order.
8. Forgetting to initialize DP values to zero.
9. Returning the wrong DP cell instead of `dp[0][n + 1]`.

---

## 9. Important Interview Questions

**Q1. What is Interval DP?**

Interval DP is a technique where we solve smaller ranges first and combine their answers to solve larger ranges.

**Q2. Why do we add boundary values of 1?**

Because when the first or last balloon is burst, the missing neighbor is treated as having value 1.

**Q3. Why do we choose the last balloon instead of the first?**

Choosing the last balloon guarantees that its neighboring boundaries remain unchanged, allowing us to divide the problem into independent left and right intervals.

**Q4. What does dp[l][r] represent?**

The maximum coins obtained by bursting all balloons strictly between indices `l` and `r`.

**Q5. What is the transition?**

```cpp
dp[l][r] = max(
    dp[l][k] + dp[k][r]
    + nums[l] * nums[k] * nums[r]
);
```

**Q6. What is the time complexity?**

`O(n^3)` because:

- There are `O(n^2)` intervals.
- For every interval, we try `O(n)` possible last balloons.

**Q7. Why can't we use greedy?**

The locally best balloon to burst may change the neighbors and reduce the total coins obtained later. We need to evaluate all possible last choices using DP.

---

## 10. Final Revision Notes

### DP State

```cpp
dp[l][r]
```

Maximum coins from bursting all balloons strictly between `l` and `r`.

### Base Case

```cpp
if (l + 1 == r) return 0;
```

### Recurrence

```cpp
dp[l][r] = max(
    dp[l][k] +
    dp[k][r] +
    nums[l] * nums[k] * nums[r]
);
```

### Loop Order

```cpp
for (int len = 2; len <= n + 1; len++) {
    for (int l = 0; l + len <= n + 1; l++) {
        int r = l + len;

        for (int k = l + 1; k < r; k++) {
            // Transition
        }
    }
}
```

### Complexity

- **Time:** `O(n^3)`
- **Space:** `O(n^2)`

### Key Takeaways

1. Burst Balloons is a classic Hard Interval DP problem.
2. Add `1` to both ends of the array.
3. Think about the last balloon to burst.
4. Split the interval into independent left and right subproblems.
5. Try every possible last balloon.
6. Take the maximum coins.
7. Solve smaller intervals before larger intervals.

**Recommended Approach:** Bottom-Up Interval DP.

**Interview Pattern:** Partition DP / Last-Element DP.

# Regular Expression Matching — LeetCode 10

- **Difficulty:** Hard
- **Topics:** Dynamic Programming, Strings, Recursion
- **Pattern:** 2D DP on Two Strings
- **Language:** C++

## 1. Problem Understanding

Given two strings:

- `s`: An input string containing lowercase English letters.
- `p`: A pattern containing lowercase English letters, `.` and `*`.

Return `true` if the pattern matches the entire input string. Otherwise, return `false`.

### Special Characters

**1. Dot (`.`)**

Matches any single character.

```text
s = "a"
p = "."

Output = true
```

**2. Star (`*`)**

Matches zero or more occurrences of the preceding element.

```text
s = "aaa"
p = "a*"

Output = true
```

Here, `a*` can match:

- Empty string (zero occurrences).
- `"a"` (one occurrence).
- `"aa"` (two occurrences).
- `"aaa"` (three occurrences).

**Important:** `*` does not independently match any character. It always applies to the character or dot immediately before it.

### Example 1

```text
Input:  s = "aa"
        p = ".b"

Output: false
```

Explanation:

- `.` matches the first `a`.
- `b` cannot match the second `a`.

Therefore, the entire string does not match.

### Example 2

```text
Input:  s = "nnn"
        p = "n*"

Output: true
```

Explanation:

`n*` can match all three occurrences of `n`.

### Example 3

```text
Input:  s = "ab"
        p = ".*"

Output: true
```

Explanation:

- `.` matches any character.
- `*` allows the preceding `.` to match zero or more characters.

Therefore, `.*` can match the entire string `"ab"`.

---

## 2. Brute Force Approach — Recursion

### Main Idea

We compare the characters of `s` and `p` from left to right.

Let:

```cpp
solve(i, j)
```

Represent whether:

```text
s[i...end] matches p[j...end]
```

We have two main situations.

### Case 1: Current Characters Match

The characters match if:

```cpp
s[i] == p[j] || p[j] == '.'
```

We call this condition:

```cpp
bool match = (i < s.size() &&
             (s[i] == p[j] || p[j] == '.'));
```

If the next pattern character is not `*`, both pointers move forward.

```cpp
solve(i + 1, j + 1);
```

### Case 2: Next Pattern Character Is `*`

This is the most important part.

When:

```cpp
p[j + 1] == '*'
```

We have two choices.

**Choice 1: Use zero occurrences**

Ignore the current character and `*` from the pattern.

```cpp
solve(i, j + 2);
```

Example:

```text
s = "b"
p = "a*b"

Skip "a*":

s = "b"
p = "b"
```

**Choice 2: Use one or more occurrences**

If the current characters match, consume one character from the input but keep the pattern pointer at the same position.

```cpp
solve(i + 1, j);
```

Why does `j` remain unchanged?

Because `*` can match additional occurrences of the preceding character.

### Recurrence

```cpp
if (p[j + 1] == '*') {

    return solve(i, j + 2) ||
           (match && solve(i + 1, j));

}
```

Otherwise:

```cpp
return match && solve(i + 1, j + 1);
```

### Base Case

If the input string is completely consumed:

```cpp
if (i == s.size())
```

The remaining pattern can only match if it consists of zero-occurrence groups such as:

```text
a*
b*
c*
```

Therefore, we can handle this naturally through the zero-occurrence transition.

The recursion terminates successfully when both strings are consumed:

```cpp
if (j == p.size()) {
    return i == s.size();
}
```

### C++ Code

```cpp
class Solution {
public:
    bool solve(int i, int j, string& s, string& p) {

        if (j == p.size()) {
            return i == s.size();
        }

        bool match = (i < s.size() &&
                     (s[i] == p[j] || p[j] == '.'));

        if (j + 1 < p.size() && p[j + 1] == '*') {

            // Option 1: Use zero occurrences
            bool skip = solve(i, j + 2, s, p);

            // Option 2: Use one or more occurrences
            bool take = match && solve(i + 1, j, s, p);

            return skip || take;
        }

        return match && solve(i + 1, j + 1, s, p);
    }

    bool isMatch(string s, string p) {
        return solve(0, 0, s, p);
    }
};
```

### Complexity

- **Time:** Exponential in the worst case.
- **Space:** `O(m + n)` recursion stack.

The same states can be calculated repeatedly, so we optimize using memoization.

---

## 3. Optimized Approach — Memoization

### Main Idea

The recursive solution may calculate the same `(i, j)` state multiple times.

We store the result of each state in a DP matrix.

### DP State

```cpp
dp[i][j]
```

Represents whether:

```text
s[i...end] matches p[j...end]
```

We use three possible values:

```text
-1 = Not calculated
 0 = False
 1 = True
```

### Transition

First, check whether the current characters match.

```cpp
bool match = (i < s.size() &&
             (s[i] == p[j] || p[j] == '.'));
```

If the next pattern character is `*`:

```cpp
dp[i][j] = solve(i, j + 2) ||
           (match && solve(i + 1, j));
```

Otherwise:

```cpp
dp[i][j] = match && solve(i + 1, j + 1);
```

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, int j, string& s, string& p,
              vector<vector<int>>& dp) {

        if (j == p.size()) {
            return i == s.size();
        }

        if (dp[i][j] != -1) {
            return dp[i][j];
        }

        bool match = (i < s.size() &&
                     (s[i] == p[j] || p[j] == '.'));

        bool ans = false;

        if (j + 1 < p.size() && p[j + 1] == '*') {

            // Zero occurrences
            bool skip = solve(i, j + 2, s, p, dp);

            // One or more occurrences
            bool take = match &&
                        solve(i + 1, j, s, p, dp);

            ans = skip || take;
        }
        else {

            ans = match &&
                  solve(i + 1, j + 1, s, p, dp);
        }

        return dp[i][j] = ans;
    }

    bool isMatch(string s, string p) {

        int m = s.size();
        int n = p.size();

        vector<vector<int>> dp(
            m + 1, vector<int>(n + 1, -1)
        );

        return solve(0, 0, s, p, dp);
    }
};
```

### Complexity

- **Time:** `O(m * n)`
- **Space:** `O(m * n)` for DP + `O(m + n)` recursion stack.

Where:

- `m = s.size()`
- `n = p.size()`

---

## 4. Bottom-Up Approach — Tabulation

### Main Idea

Instead of matching the strings from the beginning recursively, we build a DP table iteratively.

We define the DP state using prefixes of both strings.

### Step 1: Define DP State

```cpp
dp[i][j]
```

Represents whether the first `i` characters of `s` match the first `j` characters of `p`.

The DP matrix size is:

```cpp
(m + 1) * (n + 1)
```

The extra row and column represent empty strings.

### Step 2: Initialize DP

```cpp
vector<vector<bool>> dp(
    m + 1, vector<bool>(n + 1, false)
);
```

An empty string matches an empty pattern.

```cpp
dp[0][0] = true;
```

### Step 3: Handle Empty Input String

Consider:

```text
s = ""
p = "a*b*c*"
```

This pattern can match an empty string because every character followed by `*` can occur zero times.

Therefore:

```cpp
for (int j = 2; j <= n; j++) {

    if (p[j - 1] == '*') {
        dp[0][j] = dp[0][j - 2];
    }
}
```

### Step 4: Write the Transition

There are two possibilities.

**Case 1: Current Characters Match**

```cpp
s[i - 1] == p[j - 1] || p[j - 1] == '.'
```

If the current characters match:

```cpp
dp[i][j] = dp[i - 1][j - 1];
```

We move diagonally because both characters are consumed.

**Case 2: Current Pattern Character Is `*`**

If:

```cpp
p[j - 1] == '*'
```

We have two choices.

**Choice A: Zero occurrences**

Ignore the preceding character and `*`.

```cpp
dp[i][j] = dp[i][j - 2];
```

**Choice B: One or more occurrences**

If the preceding pattern character matches the current input character:

```cpp
s[i - 1] == p[j - 2] || p[j - 2] == '.'
```

We consume one input character but keep the pattern unchanged.

```cpp
dp[i][j] = dp[i][j] || dp[i - 1][j];
```

### Complete Transition

```cpp
if (p[j - 1] == '*') {

    dp[i][j] = dp[i][j - 2];

    if (s[i - 1] == p[j - 2] || p[j - 2] == '.') {
        dp[i][j] = dp[i][j] || dp[i - 1][j];
    }
}
else {

    if (s[i - 1] == p[j - 1] || p[j - 1] == '.') {
        dp[i][j] = dp[i - 1][j - 1];
    }
}
```

### C++ Code — Recommended Solution

```cpp
class Solution {
public:
    bool isMatch(string s, string p) {

        int m = s.size();
        int n = p.size();

        vector<vector<bool>> dp(
            m + 1, vector<bool>(n + 1, false)
        );

        // Empty string matches empty pattern
        dp[0][0] = true;

        // Handle patterns such as a*, a*b*, a*b*c*
        for (int j = 2; j <= n; j++) {

            if (p[j - 1] == '*') {
                dp[0][j] = dp[0][j - 2];
            }
        }

        // Fill the DP table
        for (int i = 1; i <= m; i++) {

            for (int j = 1; j <= n; j++) {

                if (p[j - 1] == '*') {

                    // Option 1: Zero occurrences
                    dp[i][j] = dp[i][j - 2];

                    // Option 2: One or more occurrences
                    if (s[i - 1] == p[j - 2] ||
                        p[j - 2] == '.') {

                        dp[i][j] =
                            dp[i][j] || dp[i - 1][j];
                    }
                }
                else {

                    // Normal character or dot
                    if (s[i - 1] == p[j - 1] ||
                        p[j - 1] == '.') {

                        dp[i][j] = dp[i - 1][j - 1];
                    }
                }
            }
        }

        return dp[m][n];
    }
};
```

### Complexity

- **Time:** `O(m * n)`
- **Space:** `O(m * n)`

This is the standard optimized solution.

---

## 5. Dry Run — Tabulation

Consider:

```text
s = "aab"
p = "c*a*b"
```

Expected output:

```text
true
```

The pattern can be interpreted as:

```text
c* = zero occurrences of c
a* = two occurrences of a
b  = one occurrence of b
```

Therefore:

```text
"aab" matches "c*a*b"
```

### Step 1: Initialize

DP state:

```cpp
dp[i][j]
```

- Rows represent prefixes of `s`.
- Columns represent prefixes of `p`.

Pattern positions:

```text
Index:  0  1  2  3  4  5
p:         c  *  a  *  b
```

Input:

```text
Index:  0  1  2
s:      a  a  b
```

### Step 2: Empty String Initialization

```text
dp[0][0] = true
```

For the pattern `c*`:

```text
dp[0][2] = dp[0][0] = true
```

For the pattern `c*a*`:

```text
dp[0][4] = dp[0][2] = true
```

This is possible because both `c*` and `a*` can represent zero occurrences.

### Step 3: Process Input Characters

For the first `a`:

- `c*` can be skipped.
- `a*` can match the first `a`.

For the second `a`:

- `a*` can match another `a` using `dp[i - 1][j]`.

For the final `b`:

- `b` matches the final input character.

### Final DP Table

| `s` / `p` | Empty | c | c* | c*a | c*a* | c*a*b |
|---|---|---|---|---|---|---|
| Empty | T | F | T | F | T | F |
| a | F | F | T | T | T | F |
| aa | F | F | T | F | T | F |
| aab | F | F | T | F | T | T |

Therefore:

```cpp
dp[3][5] = true;
```

**Final Answer: `true`**

---

## 6. Understanding the Star (`*`) Transition

This is the most important part of the problem.

Suppose:

```text
s = "aaa"
p = "a*"
```

The `*` has two possible behaviors.

### Option 1: Zero Occurrences

```cpp
dp[i][j] = dp[i][j - 2];
```

We ignore both the preceding character and `*`.

Example:

```text
s = "b"
p = "a*b"
```

Skip `a*`:

```text
s = "b"
p = "b"
```

Now both strings match.

### Option 2: One or More Occurrences

```cpp
dp[i][j] = dp[i - 1][j];
```

We consume one character from the input string but keep the pattern position unchanged.

Example:

```text
s = "aaa"
p = "a*"
```

The same `a*` can match:

```text
"a"
"aa"
"aaa"
```

### Remember

| Situation | DP Transition |
|---|---|
| `*` matches zero occurrences | `dp[i][j - 2]` |
| `*` matches one or more occurrences | `dp[i - 1][j]` |
| Normal character matches | `dp[i - 1][j - 1]` |
| Dot matches any character | `dp[i - 1][j - 1]` |

---

## 7. Common Mistakes

1. Treating `*` as an independent wildcard.
2. Forgetting that `*` applies to the preceding character.
3. Forgetting the zero-occurrence case.
4. Moving both pointers when using `*` for multiple occurrences.
5. Using `dp[i - 1][j - 1]` instead of `dp[i - 1][j]` for the repeated-character case.
6. Forgetting to initialize `dp[0][0] = true`.
7. Incorrectly handling empty input strings.
8. Forgetting to check whether the pattern has a next character in recursion.
9. Using the wrong indices in tabulation.
10. Checking only whether the pattern matches a substring instead of the entire input string.

---

## 8. Important Interview Questions

**Q1. What does `.` represent?**

It matches exactly one arbitrary character.

**Q2. What does `*` represent?**

It matches zero or more occurrences of the preceding element.

**Q3. Why do we use `dp[i][j - 2]` for zero occurrences?**

Because we remove both the preceding pattern character and `*` from consideration.

**Q4. Why do we use `dp[i - 1][j]` for one or more occurrences?**

We consume one matching input character while keeping the pattern position unchanged, allowing `*` to match additional characters.

**Q5. Why is this a 2D DP problem?**

Because the answer depends on two changing positions: one in the input string and one in the pattern.

**Q6. Why is the empty-string initialization important?**

Patterns such as `a*`, `a*b*`, and `a*b*c*` can match an empty string.

**Q7. What is the time complexity?**

`O(m * n)` using memoization or tabulation.

**Q8. Does `.*` match an empty string?**

Yes.

- `.` matches any one character.
- `*` allows zero occurrences.

Therefore, `.*` can match an empty string or any sequence of characters.

**Q9. Does `a*` match `b`?**

No. It can match zero or more `a` characters, not `b`.

---

## 9. Final Revision Notes

### DP State

```cpp
dp[i][j]
```

Whether the first `i` characters of `s` match the first `j` characters of `p`.

### Initialization

```cpp
dp[0][0] = true;

for (int j = 2; j <= n; j++) {
    if (p[j - 1] == '*') {
        dp[0][j] = dp[0][j - 2];
    }
}
```

### Main Transition

```cpp
if (p[j - 1] == '*') {

    dp[i][j] = dp[i][j - 2];

    if (s[i - 1] == p[j - 2] ||
        p[j - 2] == '.') {

        dp[i][j] =
            dp[i][j] || dp[i - 1][j];
    }
}
else {

    if (s[i - 1] == p[j - 1] ||
        p[j - 1] == '.') {

        dp[i][j] = dp[i - 1][j - 1];
    }
}
```

### Complexity

- **Time:** `O(m * n)`
- **Space:** `O(m * n)`

### Key Takeaways

1. `.` matches exactly one character.
2. `*` matches zero or more occurrences of the preceding element.
3. For `*`, always consider zero occurrences and one-or-more occurrences.
4. Zero occurrences use `dp[i][j - 2]`.
5. One-or-more occurrences use `dp[i - 1][j]`.
6. Normal matching uses `dp[i - 1][j - 1]`.
7. Empty-string initialization is essential.
8. The entire input string must match the entire pattern.

**Recommended Approach:** Bottom-Up 2D DP.

**Interview Pattern:** String Matching / 2D Dynamic Programming.
- **Auxiliary Space:** O(n).

**Interview Takeaway:** Whenever a grid problem asks for the number of ways to reach a destination using restricted movements, think about Grid DP. Identify the possible previous cells and build the recurrence from them.
