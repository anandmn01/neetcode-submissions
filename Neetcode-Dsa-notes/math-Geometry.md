# Rotate Image

- **Difficulty:** Medium
- **Problem:** LeetCode 48
- **Pattern:** Matrix Manipulation
- **Language:** C++
- **Key Techniques:** Transpose, Reverse, In-place Swapping

## 1. Problem Understanding

Given a square `n x n` matrix, rotate it by 90 degrees clockwise.

The rotation must happen **in-place**.

This means:
- We must modify the original matrix.
- We cannot allocate another 2D matrix for the rotated result.

### Example 1

**Input:**

```cpp
matrix = {
    {1, 2},
    {3, 4}
};
```

**Output:**

```cpp
{
    {3, 1},
    {4, 2}
}
```

### Visual Understanding

Before rotation:

```text
1  2
3  4
```

After 90-degree clockwise rotation:

```text
3  1
4  2
```

### Important Observation

After clockwise rotation:

- The first column becomes the first row.
- The second column becomes the second row.
- The order of elements in each column is reversed.

---

## 2. Approach 1 — Using an Extra Matrix

### Idea

Create another `n x n` matrix and place each element at its rotated position.

For an element at:

```cpp
matrix[i][j]
```

Its new position after 90-degree clockwise rotation is:

```cpp
rotated[j][n - 1 - i]
```

### C++ Code

```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {

        int n = matrix.size();

        vector<vector<int>> rotated(n, vector<int>(n));

        for (int i = 0; i < n; i++) {

            for (int j = 0; j < n; j++) {

                rotated[j][n - 1 - i] = matrix[i][j];
            }
        }

        matrix = rotated;
    }
};
```

### Complexity

- **Time:** O(n²)
- **Space:** O(n²)

### Limitation

This approach uses an additional matrix.

Therefore, it does not satisfy the problem's in-place requirement.

---

## 3. Approach 2 — Transpose + Reverse Each Row (Optimized)

### Core Idea

A 90-degree clockwise rotation can be performed using two operations:

1. Transpose the matrix.
2. Reverse every row.

Both operations can be performed in-place.

### Step 1: Transpose the Matrix

Transpose means converting rows into columns.

We swap:

```cpp
matrix[i][j]
```

with:

```cpp
matrix[j][i]
```

Example:

Before transpose:

```text
1  2  3
4  5  6
7  8  9
```

After transpose:

```text
1  4  7
2  5  8
3  6  9
```

### How to Transpose

```cpp
for (int i = 0; i < n; i++) {

    for (int j = i + 1; j < n; j++) {

        swap(matrix[i][j], matrix[j][i]);
    }
}
```

**Important:** Start the inner loop from `j = i + 1`.

Why?

Because:
- Elements on the diagonal do not need swapping.
- We only need to swap elements above the diagonal with their corresponding elements below it.
- Starting from `j = 0` would swap elements twice and restore the original matrix.

### Step 2: Reverse Every Row

After transposing:

```text
1  4  7
2  5  8
3  6  9
```

Reverse each row:

```text
7  4  1
8  5  2
9  6  3
```

This is the required 90-degree clockwise rotation.

C++:

```cpp
for (int i = 0; i < n; i++) {

    reverse(matrix[i].begin(), matrix[i].end());
}
```

---

## 4. Optimized C++ Solution

```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {

        int n = matrix.size();

        // Step 1: Transpose the matrix
        for (int i = 0; i < n; i++) {

            for (int j = i + 1; j < n; j++) {

                swap(matrix[i][j], matrix[j][i]);
            }
        }

        // Step 2: Reverse every row
        for (int i = 0; i < n; i++) {

            reverse(matrix[i].begin(), matrix[i].end());
        }
    }
};
```

---

## 5. Detailed Dry Run

Input:

```cpp
matrix = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
```

### Initial Matrix

```text
1  2  3
4  5  6
7  8  9
```

### Step 1: Transpose

We swap elements across the main diagonal.

#### First Swap

```cpp
swap(matrix[0][1], matrix[1][0]);
```

Before:

```text
1  2  3
4  5  6
7  8  9
```

After:

```text
1  4  3
2  5  6
7  8  9
```

#### Second Swap

```cpp
swap(matrix[0][2], matrix[2][0]);
```

After:

```text
1  4  7
2  5  6
3  8  9
```

#### Third Swap

```cpp
swap(matrix[1][2], matrix[2][1]);
```

After:

```text
1  4  7
2  5  8
3  6  9
```

Transpose completed.

### Step 2: Reverse Every Row

Before reversing:

```text
1  4  7
2  5  8
3  6  9
```

Reverse row 0:

```text
7  4  1
2  5  8
3  6  9
```

Reverse row 1:

```text
7  4  1
8  5  2
3  6  9
```

Reverse row 2:

```text
7  4  1
8  5  2
9  6  3
```

### Final Output

```text
7  4  1
8  5  2
9  6  3
```

---

## 6. Understanding the Index Mapping

For a clockwise 90-degree rotation:

```cpp
matrix[i][j] -> rotated[j][n - 1 - i]
```

Example:

For a 3 x 3 matrix:

```text
matrix[0][0] -> rotated[0][2]
matrix[0][1] -> rotated[1][2]
matrix[0][2] -> rotated[2][2]
```

The first row becomes the last column.

Similarly:

```text
matrix[1][0] -> rotated[0][1]
matrix[1][1] -> rotated[1][1]
matrix[1][2] -> rotated[2][1]
```

This mapping explains why transposing and reversing rows produces the correct rotation.

---

## 7. Approach 3 — Layer-by-Layer Four-Way Swapping

Another in-place approach is to rotate the matrix layer by layer.

### Core Idea

Each group of four elements is moved together:

- Top element moves to the right.
- Right element moves to the bottom.
- Bottom element moves to the left.
- Left element moves to the top.

Example:

```text
1  2  3
4  5  6
7  8  9
```

After rotation:

```text
7  4  1
8  5  2
9  6  3
```

### C++ Code

```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {

        int n = matrix.size();

        for (int layer = 0; layer < n / 2; layer++) {

            int first = layer;
            int last = n - 1 - layer;

            for (int i = first; i < last; i++) {

                int offset = i - first;

                int top = matrix[first][i];

                // Left -> Top
                matrix[first][i] = matrix[last - offset][first];

                // Bottom -> Left
                matrix[last - offset][first] =
                    matrix[last][last - offset];

                // Right -> Bottom
                matrix[last][last - offset] =
                    matrix[i][last];

                // Top -> Right
                matrix[i][last] = top;
            }
        }
    }
};
```

### Complexity

- **Time:** O(n²)
- **Space:** O(1)

This approach directly rotates four elements at a time without transposing.

---

## 8. Approach Comparison

| Approach | Time | Extra Space | In-place |
|---|---|---|---|
| Extra matrix | O(n²) | O(n²) | No |
| Transpose + Reverse | O(n²) | O(1) | Yes |
| Layer-by-layer swapping | O(n²) | O(1) | Yes |

### Recommended Approach

**Transpose + Reverse Each Row**

Reasons:
- Easy to understand.
- Easy to implement.
- Uses constant extra space.
- Commonly expected in interviews.

---

## 9. Complexity Analysis

Let `n` be the number of rows and columns.

### Time Complexity

Transpose:

```text
O(n²)
```

Reverse all rows:

```text
O(n²)
```

Total:

```text
O(n²) + O(n²) = O(n²)
```

### Space Complexity

We only use:
- Loop variables.
- Temporary values during swapping.

No additional matrix is created.

```text
O(1)
```

---

## 10. Common Mistakes

1. Forgetting that the rotation is clockwise.
2. Reversing columns instead of rows after transposing.
3. Starting the transpose inner loop from `j = 0`.
4. Creating another matrix despite the in-place requirement.
5. Using the wrong index mapping.
6. Forgetting that this transpose technique assumes a square matrix.
7. Confusing clockwise and anticlockwise rotation.

---

## 11. Important Interview Insights

### Q1. What is matrix transpose?

Transpose means converting rows into columns.

The element at `[i][j]` moves to `[j][i]`.

### Q2. Why do we start `j` from `i + 1`?

To avoid:
- Swapping diagonal elements.
- Swapping the same pair twice.

### Q3. Why reverse rows after transpose?

Transpose alone gives an anticlockwise rotation.

Reversing each row converts it into a clockwise rotation.

### Q4. How can we rotate a matrix in-place?

Use:
1. Transpose.
2. Reverse every row.

### Q5. What is the time complexity?

O(n²), because every matrix element is processed a constant number of times.

### Q6. What is the space complexity?

O(1), because no additional matrix is allocated.

### Q7. How would you rotate the matrix 90 degrees anticlockwise?

One way:

1. Transpose the matrix.
2. Reverse every column.

---

## 12. Pattern Recognition

| Problem Requirement | Technique |
|---|---|
| Convert rows into columns | Transpose |
| Reverse matrix rows | `reverse()` |
| Rotate clockwise 90 degrees | Transpose + Reverse Rows |
| Rotate anticlockwise 90 degrees | Transpose + Reverse Columns |
| Rotate four boundary elements | Layer-by-layer swapping |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Rotate Image | 48 | Matrix Transpose |
| Set Matrix Zeroes | 73 | In-place Matrix |
| Spiral Matrix | 54 | Matrix Traversal |
| Spiral Matrix II | 59 | Matrix Construction |
| Search a 2D Matrix | 74 | Matrix + Binary Search |
| Transpose Matrix | 867 | Matrix Manipulation |

---

## 13. Key Takeaways

Remember this simple formula:

```text
90-degree clockwise rotation:

1. Transpose the matrix.
2. Reverse each row.
```

### Final Recommended Solution

```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {

        int n = matrix.size();

        // Transpose
        for (int i = 0; i < n; i++) {

            for (int j = i + 1; j < n; j++) {

                swap(matrix[i][j], matrix[j][i]);
            }
        }

        // Reverse each row
        for (int i = 0; i < n; i++) {

            reverse(matrix[i].begin(), matrix[i].end());
        }
    }
};
```

### Final Complexity

- **Time:** O(n²)
- **Space:** O(1)

# Spiral Matrix

- **Difficulty:** Medium
- **Problem:** LeetCode 54
- **Pattern:** Matrix Traversal
- **Language:** C++
- **Key Technique:** Four Boundary Pointers

## 1. Problem Understanding

Given an `m x n` matrix, return all its elements in spiral order.

Spiral order means traversing the matrix clockwise:

1. Left to right across the top row.
2. Top to bottom down the right column.
3. Right to left across the bottom row.
4. Bottom to top up the left column.

Repeat these steps while moving inward until every element has been visited.

### Example 1

**Input:**

```cpp
matrix = {
    {1, 2},
    {3, 4}
};
```

**Output:**

```cpp
[1, 2, 4, 3]
```

### Visual Understanding

```text
1 → 2
↑   ↓
3 ← 4
```

### Example 2

**Input:**

```cpp
matrix = {
    {1,  2,  3},
    {4,  5,  6},
    {7,  8,  9}
};
```

**Output:**

```cpp
[1, 2, 3, 6, 9, 8, 7, 4, 5]
```

---

## 2. Approach 1 — Boundary Traversal (Optimal)

### Core Idea

Instead of visiting elements randomly, maintain four boundaries:

```cpp
int top = 0;
int bottom = m - 1;
int left = 0;
int right = n - 1;
```

These boundaries represent the remaining unvisited portion of the matrix.

Initially:

- `top` points to the first row.
- `bottom` points to the last row.
- `left` points to the first column.
- `right` points to the last column.

After completing each direction, shrink the corresponding boundary.

### Four Traversal Directions

**Step 1: Traverse Top Row (Left → Right)**

```cpp
for (int j = left; j <= right; j++) {
    ans.push_back(matrix[top][j]);
}

top++;
```

After processing the top row, move the top boundary downward.

**Step 2: Traverse Right Column (Top → Bottom)**

```cpp
for (int i = top; i <= bottom; i++) {
    ans.push_back(matrix[i][right]);
}

right--;
```

After processing the right column, move the right boundary leftward.

**Step 3: Traverse Bottom Row (Right → Left)**

Before traversing, check:

```cpp
if (top <= bottom)
```

Then:

```cpp
for (int j = right; j >= left; j--) {
    ans.push_back(matrix[bottom][j]);
}

bottom--;
```

After processing the bottom row, move the bottom boundary upward.

**Step 4: Traverse Left Column (Bottom → Top)**

Before traversing, check:

```cpp
if (left <= right)
```

Then:

```cpp
for (int i = bottom; i >= top; i--) {
    ans.push_back(matrix[i][left]);
}

left++;
```

After processing the left column, move the left boundary rightward.

---

## 3. Optimized C++ Solution

```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {

        int m = matrix.size();
        int n = matrix[0].size();

        int top = 0;
        int bottom = m - 1;
        int left = 0;
        int right = n - 1;

        vector<int> ans;

        while (top <= bottom && left <= right) {

            // 1. Left -> Right
            for (int j = left; j <= right; j++) {
                ans.push_back(matrix[top][j]);
            }
            top++;

            // 2. Top -> Bottom
            for (int i = top; i <= bottom; i++) {
                ans.push_back(matrix[i][right]);
            }
            right--;

            // 3. Right -> Left
            if (top <= bottom) {

                for (int j = right; j >= left; j--) {
                    ans.push_back(matrix[bottom][j]);
                }

                bottom--;
            }

            // 4. Bottom -> Top
            if (left <= right) {

                for (int i = bottom; i >= top; i--) {
                    ans.push_back(matrix[i][left]);
                }

                left++;
            }
        }

        return ans;
    }
};
```

---

## 4. Detailed Dry Run

Input:

```cpp
matrix = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
```

Initial boundaries:

```text
top    = 0
bottom = 2
left   = 0
right  = 2
```

### Initial Matrix

```text
1  2  3
4  5  6
7  8  9
```

### Step 1: Left → Right

Traverse the top row:

```text
1 → 2 → 3
```

Result:

```cpp
ans = [1, 2, 3]
```

Update:

```cpp
top++;
```

New boundaries:

```text
top = 1
```

### Step 2: Top → Bottom

Traverse the right column:

```text
6
↓
9
```

Result:

```cpp
ans = [1, 2, 3, 6, 9]
```

Update:

```cpp
right--;
```

New boundaries:

```text
right = 1
```

### Step 3: Right → Left

Traverse the bottom row:

```text
8 ← 7
```

Result:

```cpp
ans = [1, 2, 3, 6, 9, 8, 7]
```

Update:

```cpp
bottom--;
```

New boundaries:

```text
bottom = 1
```

### Step 4: Bottom → Top

Traverse the left column:

```text
↑
4
```

Result:

```cpp
ans = [1, 2, 3, 6, 9, 8, 7, 4]
```

Update:

```cpp
left++;
```

New boundaries:

```text
left = 1
```

### Step 5: Remaining Inner Matrix

The remaining boundaries are:

```text
top    = 1
bottom = 1
left   = 1
right  = 1
```

Only one element remains:

```text
5
```

Traverse the top row and add it.

Final result:

```cpp
[1, 2, 3, 6, 9, 8, 7, 4, 5]
```

---

## 5. Boundary Updates — Most Important Concept

| Traversal | Direction | Boundary Update |
|---|---|---|
| 1 | Left → Right | `top++` |
| 2 | Top → Bottom | `right--` |
| 3 | Right → Left | `bottom--` |
| 4 | Bottom → Top | `left++` |

Remember:

```text
TOP    moves DOWN
RIGHT  moves LEFT
BOTTOM moves UP
LEFT   moves RIGHT
```

The matrix gets smaller after every complete spiral layer.

---

## 6. Why Are the Two If Conditions Necessary?

These checks are very important:

```cpp
if (top <= bottom)
```

and

```cpp
if (left <= right)
```

### Reason

Consider a matrix with only one row:

```text
1  2  3  4
```

After traversing the top row, `top` becomes greater than `bottom`.

Without checking `top <= bottom`, the bottom-row traversal could visit the same elements again.

Similarly, consider a matrix with only one column:

```text
1
2
3
4
```

After traversing the right column, `left` and `right` can cross.

Without checking `left <= right`, the left-column traversal could duplicate elements.

These conditions prevent duplicate visits.

---

## 7. Edge Cases

### Case 1: Single Row

```text
1  2  3  4
```

Output:

```cpp
[1, 2, 3, 4]
```

### Case 2: Single Column

```text
1
2
3
4
```

Output:

```cpp
[1, 2, 3, 4]
```

### Case 3: Single Element

```text
5
```

Output:

```cpp
[5]
```

### Case 4: Rectangular Matrix

```text
1  2  3  4
5  6  7  8
```

Output:

```cpp
[1, 2, 3, 4, 8, 7, 6, 5]
```

The algorithm works for both square and rectangular matrices.

---

## 8. Complexity Analysis

Let:
- `m` = number of rows.
- `n` = number of columns.

### Time Complexity

Every element is visited exactly once.

Total number of elements:

```text
m * n
```

Therefore:

```text
O(m * n)
```

### Space Complexity

The algorithm uses four boundary variables and an output vector.

- Auxiliary space: O(1)
- Output space: O(m * n)

If output storage is excluded, the auxiliary space complexity is O(1).

---

## 9. Common Mistakes

1. Forgetting to increment `top` after traversing the top row.
2. Forgetting to decrement `right` after traversing the right column.
3. Forgetting to decrement `bottom` after traversing the bottom row.
4. Forgetting to increment `left` after traversing the left column.
5. Not checking `top <= bottom` before bottom-row traversal.
6. Not checking `left <= right` before left-column traversal.
7. Accidentally visiting elements twice.
8. Assuming the matrix is always square.
9. Confusing row indices with column indices.

---

## 10. Important Interview Insights

### Q1. Why do we use four boundaries?

They represent the remaining unvisited portion of the matrix.

Instead of tracking every visited element, we shrink the boundaries after each traversal.

### Q2. Why is the while condition?

```cpp
while (top <= bottom && left <= right)
```

It ensures that there are still unvisited rows and columns.

### Q3. Can this algorithm handle rectangular matrices?

Yes. We independently track the row and column boundaries.

### Q4. What prevents duplicate elements?

The boundary updates and the two conditional checks.

### Q5. What is the time complexity?

O(m × n), because each element is visited exactly once.

### Q6. What is the auxiliary space complexity?

O(1), excluding the output vector.

### Q7. Is this an in-place algorithm?

No. The original matrix is not modified, and an output vector is created.

---

## 11. Pattern Recognition

| Problem Requirement | Technique |
|---|---|
| Clockwise spiral traversal | Four boundaries |
| Move inward layer by layer | Boundary shrinking |
| Avoid duplicate visits | Boundary conditions |
| Traverse rectangular matrix | Independent row/column limits |
| Rotate a matrix | Transpose + Reverse |

### Related LeetCode Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Spiral Matrix | 54 | Boundary Traversal |
| Spiral Matrix II | 59 | Matrix Construction |
| Rotate Image | 48 | Transpose + Reverse |
| Set Matrix Zeroes | 73 | In-place Matrix |
| Transpose Matrix | 867 | Matrix Manipulation |

---

## 12. Key Takeaways

The entire solution revolves around four variables:

```cpp
int top = 0;
int bottom = m - 1;
int left = 0;
int right = n - 1;
```

Remember the clockwise traversal:

```text
1. Left   -> Right
2. Top    -> Bottom
3. Right  -> Left
4. Bottom -> Top
```

After every traversal, shrink the corresponding boundary.

### Final Complexity

- **Time:** O(m × n)
- **Auxiliary Space:** O(1)
- **Output Space:** O(m × n)

**Interview takeaway:**

Whenever a matrix traversal asks for spiral order, think about four boundaries and process the matrix layer by layer.

# Set Matrix Zeroes

- **Difficulty:** Medium
- **Problem:** LeetCode 73
- **Pattern:** Matrix Manipulation
- **Language:** C++
- **Key Technique:** In-place Matrix Marking

## 1. Problem Understanding

Given an `m x n` matrix of integers:

- If an element is `0`, set its entire row to `0`.
- Also set its entire column to `0`.
- Modify the original matrix in-place.

### Follow-up

Can you solve it using O(1) extra space?

### Example 1

**Input:**

```cpp
matrix = {
    {0, 1},
    {1, 0}
};
```

**Output:**

```cpp
{
    {0, 0},
    {0, 0}
};
```

**Explanation:**

- The zero at `(0,0)` makes row 0 and column 0 zero.
- The zero at `(1,1)` makes row 1 and column 1 zero.
- Therefore, the entire matrix becomes zero.

### Example 2

**Input:**

```cpp
matrix = {
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1}
};
```

**Output:**

```cpp
{
    {1, 0, 1},
    {0, 0, 0},
    {1, 0, 1}
};
```

---

## 2. Important Observation

Suppose the matrix is:

```text
1  2  3
4  0  6
7  8  9
```

The zero is at:

```text
row = 1
col = 1
```

Therefore:
- Entire row 1 becomes zero.
- Entire column 1 becomes zero.

Result:

```text
1  0  3
0  0  0
7  0  9
```

### Main Challenge

We must remember which rows and columns contain zeroes.

There are three approaches:

1. Brute Force — use a separate matrix.
2. Better — use separate row and column arrays.
3. Optimal — use the first row and first column as markers.

---

## 3. Approach 1 — Brute Force

### Idea

Create a copy of the original matrix.

Traverse the original matrix:
- Whenever a zero is found, mark the corresponding row and column as zero in the copied matrix.

The original matrix is used for detecting zeroes, so newly inserted zeroes do not affect further detection.

### C++ Code

```cpp
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {

        int m = matrix.size();
        int n = matrix[0].size();

        vector<vector<int>> temp = matrix;

        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (matrix[i][j] == 0) {

                    // Mark entire row
                    for (int col = 0; col < n; col++) {
                        temp[i][col] = 0;
                    }

                    // Mark entire column
                    for (int row = 0; row < m; row++) {
                        temp[row][j] = 0;
                    }
                }
            }
        }

        matrix = temp;
    }
};
```

### Complexity

- Time: O(m × n × (m + n))
- Space: O(m × n)

This approach is easy to understand but uses an additional matrix.

---

## 4. Approach 2 — Better Solution

### Idea

Instead of creating another matrix, maintain two arrays:

```cpp
vector<int> rows(m, 0);
vector<int> cols(n, 0);
```

These arrays remember which rows and columns contain zeroes.

### Step 1: Identify zeroes

Traverse the matrix.

Whenever:

```cpp
matrix[i][j] == 0
```

Mark:

```cpp
rows[i] = 1;
cols[j] = 1;
```

### Step 2: Update the matrix

Traverse the matrix again.

If either condition is true:

```cpp
rows[i] == 1 || cols[j] == 1
```

Set:

```cpp
matrix[i][j] = 0;
```

### C++ Code

```cpp
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {

        int m = matrix.size();
        int n = matrix[0].size();

        vector<int> rows(m, 0);
        vector<int> cols(n, 0);

        // Step 1: Mark rows and columns
        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (matrix[i][j] == 0) {
                    rows[i] = 1;
                    cols[j] = 1;
                }
            }
        }

        // Step 2: Update matrix
        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (rows[i] == 1 || cols[j] == 1) {
                    matrix[i][j] = 0;
                }
            }
        }
    }
};
```

### Dry Run

Input:

```text
1  2  3
4  0  6
7  8  9
```

Initially:

```cpp
rows = [0, 0, 0];
cols = [0, 0, 0];
```

The zero is found at:

```text
matrix[1][1] = 0
```

Mark:

```cpp
rows[1] = 1;
cols[1] = 1;
```

Updated arrays:

```cpp
rows = [0, 1, 0];
cols = [0, 1, 0];
```

Now update every cell where its row or column is marked.

Final matrix:

```text
1  0  3
0  0  0
7  0  9
```

### Complexity

- Time: O(m × n)
- Space: O(m + n)

This is better than brute force, but it does not satisfy the O(1) extra-space follow-up.

---

## 5. Approach 3 — Optimal O(1) Auxiliary Space

### Core Idea

Use the first row and first column of the original matrix as marker arrays.

Instead of creating:

```cpp
vector<int> rows(m);
vector<int> cols(n);
```

We use:

```cpp
matrix[i][0]
matrix[0][j]
```

These cells indicate whether the corresponding row or column needs to become zero.

### Important Problem

The first row and first column are both used as markers.

But what if the original first row or first column already contains zero?

We need two separate variables to remember that information:

```cpp
bool firstRowZero = false;
bool firstColZero = false;
```

### Step 1: Check First Row

```cpp
for (int j = 0; j < n; j++) {

    if (matrix[0][j] == 0) {
        firstRowZero = true;
        break;
    }
}
```

### Step 2: Check First Column

```cpp
for (int i = 0; i < m; i++) {

    if (matrix[i][0] == 0) {
        firstColZero = true;
        break;
    }
}
```

### Step 3: Mark Rows and Columns

Traverse the remaining matrix, excluding the first row and first column.

```cpp
for (int i = 1; i < m; i++) {

    for (int j = 1; j < n; j++) {

        if (matrix[i][j] == 0) {

            matrix[i][0] = 0;
            matrix[0][j] = 0;
        }
    }
}
```

Whenever a zero is found:

- Set the first cell of that row to zero.
- Set the first cell of that column to zero.

### Step 4: Update Inner Matrix

```cpp
for (int i = 1; i < m; i++) {

    for (int j = 1; j < n; j++) {

        if (matrix[i][0] == 0 ||
            matrix[0][j] == 0) {

            matrix[i][j] = 0;
        }
    }
}
```

### Step 5: Update First Row

If the original first row contained zero:

```cpp
if (firstRowZero) {

    for (int j = 0; j < n; j++) {
        matrix[0][j] = 0;
    }
}
```

### Step 6: Update First Column

If the original first column contained zero:

```cpp
if (firstColZero) {

    for (int i = 0; i < m; i++) {
        matrix[i][0] = 0;
    }
}
```

---

## 6. Final Optimized C++ Solution

```cpp
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {

        int m = matrix.size();
        int n = matrix[0].size();

        bool firstRowZero = false;
        bool firstColZero = false;

        // Step 1: Check first row
        for (int j = 0; j < n; j++) {

            if (matrix[0][j] == 0) {
                firstRowZero = true;
                break;
            }
        }

        // Step 2: Check first column
        for (int i = 0; i < m; i++) {

            if (matrix[i][0] == 0) {
                firstColZero = true;
                break;
            }
        }

        // Step 3: Mark zeroes using first row and column
        for (int i = 1; i < m; i++) {

            for (int j = 1; j < n; j++) {

                if (matrix[i][j] == 0) {

                    matrix[i][0] = 0;
                    matrix[0][j] = 0;
                }
            }
        }

        // Step 4: Update inner matrix
        for (int i = 1; i < m; i++) {

            for (int j = 1; j < n; j++) {

                if (matrix[i][0] == 0 ||
                    matrix[0][j] == 0) {

                    matrix[i][j] = 0;
                }
            }
        }

        // Step 5: Update first row
        if (firstRowZero) {

            for (int j = 0; j < n; j++) {
                matrix[0][j] = 0;
            }
        }

        // Step 6: Update first column
        if (firstColZero) {

            for (int i = 0; i < m; i++) {
                matrix[i][0] = 0;
            }
        }
    }
};
```

---

## 7. Detailed Dry Run — Optimal Approach

Input:

```text
1  2  3
4  0  6
7  8  9
```

### Initial State

```cpp
firstRowZero = false;
firstColZero = false;
```

The first row:

```text
1  2  3
```

Contains no zero.

The first column:

```text
1
4
7
```

Contains no zero.

Therefore:

```cpp
firstRowZero = false;
firstColZero = false;
```

### Step 1: Mark the Zero

The zero is found at:

```text
matrix[1][1] = 0
```

Mark its row and column:

```cpp
matrix[1][0] = 0;
matrix[0][1] = 0;
```

The matrix becomes:

```text
1  0  3
0  0  6
7  8  9
```

Important:

- `matrix[1][0] = 0` indicates row 1 must become zero.
- `matrix[0][1] = 0` indicates column 1 must become zero.

### Step 2: Update Inner Matrix

Check each inner cell.

| Cell | Row Marker | Column Marker | Action |
|---|---|---|---|
| `(1,1)` | 0 | 0 | Set 0 |
| `(1,2)` | 0 | 3 | Set 0 |
| `(2,1)` | 7 | 0 | Set 0 |
| `(2,2)` | 7 | 3 | Keep 9 |

Matrix:

```text
1  0  3
0  0  0
7  0  9
```

### Step 3: Update First Row and Column

Both flags are false, so neither needs additional modification.

Final answer:

```text
1  0  3
0  0  0
7  0  9
```

---

## 8. Why Do We Need Two Boolean Variables?

Consider:

```text
0  1  2
3  4  5
6  7  8
```

The first row already contains zero.

If we use the first row as a marker without remembering its original state, we cannot distinguish between:

- A zero that existed in the original first row.
- A zero inserted later as a marker.

Therefore:

```cpp
bool firstRowZero;
bool firstColZero;
```

preserve the original information.

This is the most important edge-case handling in the optimal approach.

---

## 9. Complexity Analysis

Let:
- `m` = number of rows.
- `n` = number of columns.

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force | O(m × n × (m + n)) | O(m × n) |
| Better | O(m × n) | O(m + n) |
| Optimal | O(m × n) | O(1) |

### Optimal Complexity

**Time: O(m × n)**

We perform a constant number of matrix traversals.

**Auxiliary Space: O(1)**

We only use two boolean variables and a few integer variables.

No additional arrays or matrices are created.

---

## 10. Common Mistakes

1. Updating zeroes immediately while still detecting original zeroes.
2. Forgetting to check the original first row.
3. Forgetting to check the original first column.
4. Not excluding the first row and first column during marker creation.
5. Updating the first row and column before using their markers.
6. Using `&&` instead of `||` while checking row and column markers.
7. Confusing `matrix[i][0]` with `matrix[0][j]`.
8. Forgetting that the matrix must be modified in-place.

### Most Important Rule

Use this condition:

```cpp
if (matrix[i][0] == 0 ||
    matrix[0][j] == 0)
```

A cell becomes zero if either its row OR its column has been marked.

---

## 11. Interview Insights

### Q1. Why can't we simply set the row and column to zero immediately?

Because newly inserted zeroes could be mistaken for original zeroes, causing unnecessary rows and columns to become zero.

### Q2. Why do we use the first row and first column?

They act as marker arrays, avoiding the need for additional O(m + n) storage.

### Q3. Why are two boolean variables needed?

Because the first row and first column are also used as markers. Their original zero status must be preserved.

### Q4. What is the optimal time complexity?

O(m × n).

### Q5. What is the optimal auxiliary space complexity?

O(1).

### Q6. Why do we use OR (`||`) instead of AND (`&&`)?

A cell must become zero if its row OR its column contains an original zero.

### Q7. Is the solution in-place?

Yes. It modifies the original matrix without creating another matrix or marker array.

---

## 12. Pattern Recognition

| Requirement | Technique |
|---|---|
| Identify rows and columns containing zeroes | Matrix marking |
| Reduce extra space | First row and column as markers |
| Preserve original first-row state | Boolean flag |
| Preserve original first-column state | Boolean flag |
| Modify original matrix | In-place algorithm |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Set Matrix Zeroes | 73 | In-place Matrix Marking |
| Spiral Matrix | 54 | Boundary Traversal |
| Rotate Image | 48 | Transpose + Reverse |
| Search a 2D Matrix | 74 | Binary Search |
| Search a 2D Matrix II | 240 | Matrix Traversal |

---

## 13. Key Takeaways

Remember these three approaches:

1. Brute Force: Copy the matrix.
2. Better: Use separate row and column arrays.
3. Optimal: Use the first row and first column as markers.

### Optimal Solution Memory Trick

```cpp
// 1. Check first row and column.

// 2. Mark rows and columns.
matrix[i][0] = 0;
matrix[0][j] = 0;

// 3. Update inner matrix.
if (matrix[i][0] == 0 || matrix[0][j] == 0) {
    matrix[i][j] = 0;
}

// 4. Update first row and column using saved flags.
```

### Final Complexity

- **Time:** O(m × n)
- **Auxiliary Space:** O(1)
- **Technique:** In-place Matrix Marking

**Interview takeaway:** Whenever a matrix problem asks you to modify rows and columns based on cell values while using constant extra space, consider whether the first row and first column can store the required markers.

# Non-Cyclical Number (Happy Number)

- **Difficulty:** Easy
- **Problem:** LeetCode 202
- **Pattern:** Cycle Detection
- **Language:** C++
- **Key Techniques:** Hash Set, Fast & Slow Pointers

## 1. Problem Understanding

A non-cyclical number is an integer defined by the following algorithm:

1. Take a positive integer.
2. Replace it with the sum of the squares of its digits.
3. Repeat this process until:
   - The number becomes `1`, or
   - The process enters an infinite cycle that does not include `1`.

If the process eventually reaches `1`, return `true`.

Otherwise, return `false`.

### Example 1

**Input:**

```cpp
n = 100
```

**Output:**

```cpp
true
```

**Explanation:**

```text
100
= 1² + 0² + 0²
= 1
```

The process reaches `1`, so it is a non-cyclical number.

### Example 2

**Input:**

```cpp
n = 101
```

**Output:**

```cpp
false
```

**Explanation:**

```text
101
= 1² + 0² + 1²
= 2

2
= 2²
= 4

4
= 4²
= 16

16
= 1² + 6²
= 37

37
= 3² + 7²
= 58

58
= 5² + 8²
= 89

89
= 8² + 9²
= 145

145
= 1² + 4² + 5²
= 42

42
= 4² + 2²
= 20

20
= 2² + 0²
= 4
```

Now `4` appears again.

Therefore, the process has entered a cycle:

```text
4 → 16 → 37 → 58 → 89 → 145 → 42 → 20 → 4
```

It will never reach `1`.

Return `false`.

---

## 2. Important Observation

Every number is repeatedly transformed using:

```text
sum of squares of digits
```

For example:

```text
n = 19

19 → 1² + 9²
   → 1 + 81
   → 82

82 → 8² + 2²
   → 64 + 4
   → 68

68 → 6² + 8²
   → 36 + 64
   → 100

100 → 1² + 0² + 0²
    → 1
```

Final result:

```cpp
true
```

### Main Challenge

How do we know whether the transformation will:

- Eventually reach `1`, or
- Continue infinitely in a cycle?

We need a cycle detection technique.

There are two important approaches:

1. Hash Set — remember previously visited numbers.
2. Floyd's Cycle Detection — use slow and fast pointers.

---

## 3. Helper Function — Sum of Squares of Digits

Before solving the problem, understand how to calculate the sum of squared digits.

### C++ Code

```cpp
int getNext(int n) {

    int sum = 0;

    while (n > 0) {

        int digit = n % 10;

        sum += digit * digit;

        n = n / 10;
    }

    return sum;
}
```

### Dry Run

Suppose:

```cpp
n = 82;
```

| Iteration | Digit (`n % 10`) | Square | Sum |
|---|---:|---:|---:|
| 1 | 2 | 4 | 4 |
| 2 | 8 | 64 | 68 |

Final result:

```cpp
68
```

### Important Operators

```cpp
n % 10
```

Extracts the last digit.

```cpp
n / 10
```

Removes the last digit.

```cpp
digit * digit
```

Calculates the square of the digit.

---

## 4. Approach 1 — Brute Force / Hash Set

### Idea

Use an unordered set to store numbers that have already appeared.

Whenever we calculate the next number:

- If it is `1`, return `true`.
- If it has already appeared, a cycle exists. Return `false`.
- Otherwise, store it and continue.

### C++ Code

```cpp
class Solution {
public:

    int getNext(int n) {

        int sum = 0;

        while (n > 0) {

            int digit = n % 10;

            sum += digit * digit;

            n /= 10;
        }

        return sum;
    }

    bool isHappy(int n) {

        unordered_set<int> seen;

        while (n != 1) {

            if (seen.find(n) != seen.end()) {
                return false;
            }

            seen.insert(n);

            n = getNext(n);
        }

        return true;
    }
};
```

### Dry Run

Input:

```cpp
n = 19;
```

Initially:

```cpp
seen = {};
```

| Iteration | Current Number | Next Number | Seen Set |
|---|---:|---:|---|
| 1 | 19 | 82 | {19} |
| 2 | 82 | 68 | {19, 82} |
| 3 | 68 | 100 | {19, 82, 68} |
| 4 | 100 | 1 | {19, 82, 68, 100} |

Now:

```cpp
n == 1
```

Return:

```cpp
true
```

### Complexity Analysis

Let `k` be the number of distinct transformed values visited.

- **Time:** O(k × log n) for the digit calculations, with expected O(1) hash operations per visited value.
- **Space:** O(k) for the hash set.

This is a straightforward and easy-to-understand solution.

---

## 5. Approach 2 — Optimal Solution (Floyd's Cycle Detection)

### Core Idea

Instead of storing every previously visited number in a hash set, use two pointers:

```cpp
slow
fast
```

Both pointers follow the same transformation function, but at different speeds.

- `slow` moves one step at a time.
- `fast` moves two steps at a time.

If the sequence reaches `1`, the number is happy.

If the sequence enters a cycle, the slow and fast pointers will eventually meet.

### Why Does This Work?

Consider a linked list containing a cycle.

A slow pointer moves one node at a time.

A fast pointer moves two nodes at a time.

When both pointers enter the cycle, the fast pointer gradually catches up with the slow pointer.

The same principle works here because every number produces exactly one next number.

### C++ Code

```cpp
class Solution {
public:

    int getNext(int n) {

        int sum = 0;

        while (n > 0) {

            int digit = n % 10;

            sum += digit * digit;

            n /= 10;
        }

        return sum;
    }

    bool isHappy(int n) {

        int slow = n;
        int fast = getNext(n);

        while (fast != 1 && slow != fast) {

            slow = getNext(slow);

            fast = getNext(getNext(fast));
        }

        return fast == 1;
    }
};
```

---

## 6. Detailed Dry Run — Floyd's Cycle Detection

Input:

```cpp
n = 19;
```

Initially:

```cpp
slow = 19;
fast = getNext(19) = 82;
```

Now move:

```cpp
slow = getNext(slow);
fast = getNext(getNext(fast));
```

| Iteration | Slow Pointer | Fast Pointer |
|---|---:|---:|
| Initial | 19 | 82 |
| 1 | 82 | 100 |
| 2 | 68 | 1 |

The fast pointer reaches `1`.

Therefore:

```cpp
return true;
```

### Cycle Example

Input:

```cpp
n = 2;
```

The transformation sequence is:

```text
2 → 4 → 16 → 37 → 58 → 89 → 145 → 42 → 20 → 4
```

The sequence enters a cycle.

Pointer movement:

| Iteration | Slow | Fast |
|---|---:|---:|
| Initial | 2 | 4 |
| 1 | 4 | 16 |
| 2 | 16 | 58 |
| 3 | 37 | 145 |
| 4 | 58 | 20 |
| 5 | 89 | 16 |
| 6 | 145 | 4 |
| 7 | 42 | 58 |
| 8 | 20 | 4 |
| 9 | 4 | 58 |

The pointers eventually meet as they continue moving around the cycle.

When:

```cpp
slow == fast
```

and the value is not `1`, a cycle has been detected.

Return:

```cpp
false;
```

---

## 7. Why Is Floyd's Algorithm Optimal?

The hash set stores every previously visited number.

Floyd's algorithm avoids that extra storage.

It only maintains:

```cpp
int slow;
int fast;
```

Therefore, its auxiliary space complexity is constant.

### Complexity

Let `k` represent the number of transformation steps before reaching `1` or detecting a cycle.

Each transformation requires processing the digits of a number.

- **Time:** O(k × log n) as a simple upper-bound description for the visited transformations.
- **Auxiliary Space:** O(1).

The exact number of iterations depends on the transformation sequence.

---

## 8. Hash Set vs Floyd's Cycle Detection

| Feature | Hash Set | Floyd's Algorithm |
|---|---|---|
| Technique | Visited tracking | Slow and fast pointers |
| Cycle detection | Repeated value | Pointer collision |
| Extra Space | O(k) | O(1) |
| Implementation | Easier | Slightly more involved |
| Interview Value | Good | Excellent |

### Which Approach Should You Prefer?

- Use Hash Set when you want a simple and readable solution.
- Use Floyd's algorithm when constant extra space is required.

Both approaches are valid.

---

## 9. Common Mistakes

1. Forgetting to square each digit.
2. Using `n % 10` without removing the last digit.
3. Forgetting to update `n /= 10`.
4. Not checking whether the number has reached `1`.
5. In the hash set approach, checking for a repeated number after inserting it.
6. In Floyd's algorithm, moving the fast pointer only one step.
7. Returning `false` immediately when `slow == fast` without checking whether the meeting value is `1`.
8. Confusing the original number with its transformed value.

### Important Floyd's Algorithm Condition

```cpp
while (fast != 1 && slow != fast)
```

This loop stops when:

- `fast == 1`, meaning a happy number was found.
- `slow == fast`, meaning a cycle was detected.

Final return:

```cpp
return fast == 1;
```

---

## 10. Interview Insights

### Q1. What is a happy number?

A number that eventually becomes `1` after repeatedly replacing it with the sum of the squares of its digits.

### Q2. How can we detect an infinite cycle?

Use a hash set to track visited numbers or Floyd's slow and fast pointer algorithm.

### Q3. Why does Floyd's algorithm work here?

Every number has exactly one next transformed number. This creates a deterministic sequence in which a repeated value forms a cycle.

### Q4. Why does the fast pointer move twice?

It helps detect a cycle by catching up with the slow pointer.

### Q5. What is the advantage of Floyd's algorithm?

It detects cycles using O(1) auxiliary space.

### Q6. What happens if the number is not happy?

The transformation eventually enters a cycle that does not contain `1`.

### Q7. What is the time complexity of calculating the next number?

O(log n), because we process each digit once.

---

## 11. Pattern Recognition

| Problem Requirement | Technique |
|---|---|
| Remember previously visited states | Hash Set |
| Detect repeated values | Cycle Detection |
| Avoid extra storage | Floyd's Algorithm |
| Process individual digits | Modulo and Division |
| Move at different speeds | Slow and Fast Pointers |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Happy Number | 202 | Cycle Detection |
| Linked List Cycle | 141 | Fast & Slow Pointers |
| Linked List Cycle II | 142 | Cycle Detection |
| Find the Duplicate Number | 287 | Floyd's Algorithm |
| Middle of the Linked List | 876 | Fast & Slow Pointers |

---

## 12. Key Takeaways

Remember these three concepts:

1. **Digit Extraction**
   - `n % 10` extracts the last digit.
   - `n / 10` removes the last digit.

2. **Hash Set**
   - Stores previously visited numbers.
   - Detects repetition.

3. **Floyd's Cycle Detection**
   - Slow pointer moves one step.
   - Fast pointer moves two steps.
   - If they meet before reaching `1`, a cycle exists.

### Optimal Solution Memory Trick

```cpp
int slow = n;
int fast = getNext(n);

while (fast != 1 && slow != fast) {

    slow = getNext(slow);

    fast = getNext(getNext(fast));
}

return fast == 1;
```

### Final Complexity

- **Time:** Depends on the number of transformations and digits processed.
- **Auxiliary Space:** O(1)
- **Technique:** Floyd's Cycle Detection

**Interview takeaway:** Whenever a problem repeatedly transforms a value into another value and asks whether the process enters an infinite loop, consider Hash Set or Floyd's Cycle Detection.

# Plus One

- **Difficulty:** Easy
- **Problem:** LeetCode 66
- **Pattern:** Array / Simulation
- **Language:** C++
- **Key Technique:** Carry Propagation

## 1. Problem Understanding

You are given an integer array `digits`, where each element represents one digit of a large integer.

The digits are arranged from most significant to least significant.

Your task is to increment the represented number by `1` and return the resulting digit array.

### Example 1

**Input:**

```cpp
digits = [1,2,3,4]
```

**Output:**

```cpp
[1,2,3,5]
```

**Explanation:**

```text
1234 + 1 = 1235
```

### Example 2

**Input:**

```cpp
digits = [9,9,9]
```

**Output:**

```cpp
[1,0,0,0]
```

**Explanation:**

```text
999 + 1 = 1000
```

### Important Observation

The array stores digits from left to right:

```text
Index:   0  1  2  3
Digits:  1  2  3  4
```

The rightmost digit is the least significant digit.

Therefore, addition must begin from the right side.

---

## 2. Approach 1 — Brute Force (Integer Conversion)

### Idea

1. Convert the digit array into an integer.
2. Add `1`.
3. Convert the result back into an array of digits.

### Why Is This Approach Not Recommended?

The problem allows a large number of digits.

A normal C++ integer or even `long long` cannot safely store a number containing up to 100 digits.

Therefore, converting the entire array into a numeric type can cause integer overflow.

### Interview Insight

Avoid converting the complete digit array into an integer.

Instead, perform addition directly on the array using carry propagation.

---

## 3. Approach 2 — Optimal Solution (Carry Propagation)

### Core Idea

Start from the last index and add `1`.

There are three possible situations:

**Case 1: Current digit is less than 9**

Simply increment the digit and return.

Example:

```text
[1,2,3,4]

Last digit = 4

4 + 1 = 5

Result = [1,2,3,5]
```

**Case 2: Current digit is 9**

Adding `1` produces `10`.

Therefore:

- Change the current digit to `0`.
- Carry `1` to the previous digit.

Example:

```text
[1,2,9]

9 + 1 = 10

Change 9 to 0.

Carry 1 to previous digit.

Result = [1,3,0]
```

**Case 3: Every digit is 9**

All digits become `0`, and a new leading `1` must be inserted.

Example:

```text
[9,9,9]

999 + 1 = 1000

Result = [1,0,0,0]
```

### C++ Code

```cpp
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {

        int n = digits.size();

        for (int i = n - 1; i >= 0; i--) {

            if (digits[i] < 9) {

                digits[i]++;

                return digits;
            }

            digits[i] = 0;
        }

        digits.insert(digits.begin(), 1);

        return digits;
    }
};
```

---

## 4. Detailed Dry Run

### Example 1

Input:

```cpp
digits = [1,2,3,4]
```

Initial array:

```text
[1,2,3,4]
```

We start from the last index.

| Index | Digit | Condition | Action |
|---|---:|---|---|
| 3 | 4 | 4 < 9 | Increment to 5 |

The digit is less than `9`, so no further carry is needed.

Final result:

```cpp
[1,2,3,5]
```

Return immediately.

### Example 2

Input:

```cpp
digits = [1,2,9]
```

Initial array:

```text
[1,2,9]
```

| Index | Digit | Condition | Action |
|---|---:|---|---|
| 2 | 9 | Digit is 9 | Change to 0 |
| 1 | 2 | 2 < 9 | Increment to 3 |

Final result:

```cpp
[1,3,0]
```

### Example 3 — All Digits Are 9

Input:

```cpp
digits = [9,9,9]
```

Initial array:

```text
[9,9,9]
```

| Index | Digit | Action | Array |
|---|---:|---|---|
| 2 | 9 | Change to 0 | [9,9,0] |
| 1 | 9 | Change to 0 | [9,0,0] |
| 0 | 9 | Change to 0 | [0,0,0] |

The loop finishes without finding a digit smaller than `9`.

This means a carry remains.

Insert `1` at the beginning:

```cpp
digits.insert(digits.begin(), 1);
```

Final result:

```cpp
[1,0,0,0]
```

---

## 5. Understanding the Code

### Step 1: Start from the last index

```cpp
for (int i = n - 1; i >= 0; i--)
```

Why?

Because addition begins from the least significant digit.

### Step 2: Check whether the digit is less than 9

```cpp
if (digits[i] < 9)
```

If the digit is less than `9`, adding `1` does not produce another carry.

### Step 3: Increment and return

```cpp
digits[i]++;

return digits;
```

Once the carry is handled, the remaining digits do not change.

Therefore, we can return immediately.

### Step 4: Handle digit 9

```cpp
digits[i] = 0;
```

When:

```text
9 + 1 = 10
```

The current position becomes `0`, and the carry moves to the previous position.

### Step 5: Handle an additional leading digit

```cpp
digits.insert(digits.begin(), 1);
```

This happens only when all digits were `9`.

---

## 6. Complexity Analysis

Let `n` be the number of digits.

### Time Complexity

**O(n)**

In the worst case, every digit is `9`.

We visit all digits and then insert a new leading digit.

### Space Complexity

**O(1) auxiliary space** for the normal in-place carry processing.

The insertion of a new leading digit may require O(n) additional storage internally because the vector can reallocate.

The returned vector itself contains O(n) digits.

---

## 7. Alternative Approach — Explicit Carry Variable

We can also write the solution using an explicit carry variable.

### C++ Code

```cpp
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {

        int carry = 1;

        for (int i = digits.size() - 1; i >= 0; i--) {

            int sum = digits[i] + carry;

            digits[i] = sum % 10;

            carry = sum / 10;

            if (carry == 0) {
                return digits;
            }
        }

        if (carry) {
            digits.insert(digits.begin(), carry);
        }

        return digits;
    }
};
```

### How It Works

For every digit:

```cpp
sum = digits[i] + carry;
```

Extract the resulting digit:

```cpp
digits[i] = sum % 10;
```

Calculate the carry:

```cpp
carry = sum / 10;
```

For example:

```text
9 + 1 = 10

Digit = 10 % 10 = 0
Carry = 10 / 10 = 1
```

If carry becomes `0`, we can stop.

If carry remains after processing all digits, insert it at the beginning.

This approach is useful because it follows the general arithmetic addition algorithm.

---

## 8. Comparison of Approaches

| Approach | Time | Auxiliary Space | Recommended |
|---|---|---|---|
| Integer Conversion | Depends on numeric representation | Depends | No |
| Direct Carry Propagation | O(n) | O(1) | Yes |
| Explicit Carry Variable | O(n) | O(1) | Yes |

---

## 9. Common Mistakes

1. Starting the loop from index `0` instead of the last index.
2. Forgetting that digits are stored from most significant to least significant.
3. Incrementing `9` directly without handling the carry.
4. Forgetting to change a `9` to `0`.
5. Forgetting the all-9s case.
6. Forgetting to insert a new leading `1`.
7. Converting the entire array into an integer and causing overflow.
8. Continuing to process digits even after the carry becomes zero.

---

## 10. Interview Insights

### Q1. Why do we process digits from right to left?

Because the rightmost digit is the least significant digit, and standard addition starts from that position.

### Q2. What happens when the current digit is 9?

It becomes `0`, and a carry is propagated to the previous digit.

### Q3. When do we insert a new digit?

When all existing digits are `9` and the carry remains after processing the entire array.

### Q4. Why can we return immediately when a digit is less than 9?

Because incrementing that digit resolves the carry. All digits to its left remain unchanged.

### Q5. Why should we avoid integer conversion?

The input can contain up to 100 digits, exceeding the capacity of standard integer types.

### Q6. What is the time complexity?

O(n), because in the worst case we process every digit.

---

## 11. Pattern Recognition

| Problem Requirement | Technique |
|---|---|
| Add one to a digit array | Carry Propagation |
| Process digits from right to left | Reverse Traversal |
| Handle digit overflow | Modulo and Division |
| Handle all digits equal to 9 | Insert Leading Digit |
| Avoid integer overflow | Direct Array Manipulation |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Plus One | 66 | Carry Propagation |
| Add Binary | 67 | Binary Addition |
| Add Strings | 415 | Digit-by-Digit Addition |
| Multiply Strings | 43 | Manual Arithmetic |
| Add Two Numbers | 2 | Linked List Addition |

---

## 12. Key Takeaways

Remember these three rules:

1. Start from the last index.
2. If the digit is less than `9`, increment and return.
3. If every digit is `9`, convert them to zero and insert `1` at the beginning.

### Optimal Code to Remember

```cpp
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {

        for (int i = digits.size() - 1; i >= 0; i--) {

            if (digits[i] < 9) {
                digits[i]++;
                return digits;
            }

            digits[i] = 0;
        }

        digits.insert(digits.begin(), 1);

        return digits;
    }
};
```

### Final Complexity

- **Time:** O(n)
- **Auxiliary Space:** O(1) for carry processing
- **Pattern:** Array + Carry Propagation

**Interview takeaway:** Whenever a problem asks you to add or subtract numbers represented as digit arrays, think about processing digits from right to left and propagating carry or borrow instead of converting the entire number into an integer.

# Pow(x, n)

- **Difficulty:** Medium
- **Problem:** LeetCode 50
- **Pattern:** Binary Exponentiation
- **Language:** C++
- **Key Techniques:** Iteration, Recursion, Bit Manipulation

## 1. Problem Understanding

Given:
- A floating-point number `x`.
- An integer `n`.

Calculate:

```text
x^n
```

You cannot use built-in power functions such as `pow()`.

### Example 1

**Input:**

```cpp
x = 2.00000, n = 5
```

**Output:**

```cpp
32.00000
```

**Explanation:**

```text
2^5 = 2 × 2 × 2 × 2 × 2 = 32
```

### Example 2

**Input:**

```cpp
x = 1.10000, n = 10
```

**Output:**

```cpp
2.59374
```

**Explanation:**

```text
1.1^10 = 2.593742...
```

### Important Cases

**Positive exponent:**

```text
2^5 = 32
```

**Zero exponent:**

```text
2^0 = 1
```

**Negative exponent:**

```text
2^-2 = 1 / (2^2) = 0.25
```

### Main Challenge

A straightforward solution takes O(n) time.

We need a faster approach that handles:
- Positive exponents.
- Negative exponents.
- Zero exponent.
- The minimum 32-bit integer (`INT_MIN`).

---

## 2. Approach 1 — Brute Force

### Idea

Multiply `x` by itself `n` times.

For example:

```text
x = 2
n = 5

result = 1

result = 1 × 2 = 2
result = 2 × 2 = 4
result = 4 × 2 = 8
result = 8 × 2 = 16
result = 16 × 2 = 32
```

### C++ Code

```cpp
class Solution {
public:
    double myPow(double x, int n) {

        double result = 1.0;

        long long power = n;

        if (power < 0) {
            x = 1 / x;
            power = -power;
        }

        for (long long i = 0; i < power; i++) {
            result *= x;
        }

        return result;
    }
};
```

### Complexity Analysis

- **Time:** O(|n|)
- **Space:** O(1)

### Why Is It Not Optimal?

If the exponent is very large, the loop performs too many multiplications.

We can reduce the number of operations using Binary Exponentiation.

---

## 3. Approach 2 — Optimal Solution (Binary Exponentiation)

### Core Idea

Binary Exponentiation uses the following mathematical properties:

If the exponent is even:

```text
x^n = (x × x)^(n/2)
```

If the exponent is odd:

```text
x^n = x × x^(n-1)
```

Instead of multiplying the same number repeatedly, we square the base and divide the exponent by 2.

### Example

Calculate:

```text
2^10
```

Using binary exponentiation:

```text
2^10
= (2 × 2)^5
= 4^5

= 4 × 4^4
= 4 × (4 × 4)^2
= 4 × 16^2

= 4 × 16 × 16
= 1024
```

We need far fewer multiplications than the brute-force approach.

### Important Observation

At each iteration:

1. If the exponent is odd, multiply the result by the current base.
2. Square the base.
3. Divide the exponent by 2.

Continue until the exponent becomes zero.

---

## 4. Optimal C++ Code — Iterative Binary Exponentiation

```cpp
class Solution {
public:
    double myPow(double x, int n) {

        long long power = n;

        if (power < 0) {
            x = 1 / x;
            power = -power;
        }

        double result = 1.0;

        while (power > 0) {

            if (power % 2 == 1) {
                result *= x;
            }

            x *= x;

            power /= 2;
        }

        return result;
    }
};
```

---

## 5. Detailed Dry Run

### Example 1

Input:

```cpp
x = 2
n = 10
```

Initially:

```cpp
power = 10
result = 1.0
x = 2
```

| Iteration | Power | Base (x) | Power Odd? | Result |
|---|---:|---:|---|---:|
| 1 | 10 | 2 | No | 1 |
| 2 | 5 | 4 | Yes | 4 |
| 3 | 2 | 16 | No | 4 |
| 4 | 1 | 256 | Yes | 1024 |

Explanation:

**Iteration 1:**

```text
power = 10

10 is even.

result = 1
x = 2 × 2 = 4
power = 10 / 2 = 5
```

**Iteration 2:**

```text
power = 5

5 is odd.

result = 1 × 4 = 4
x = 4 × 4 = 16
power = 5 / 2 = 2
```

**Iteration 3:**

```text
power = 2

2 is even.

result = 4
x = 16 × 16 = 256
power = 2 / 2 = 1
```

**Iteration 4:**

```text
power = 1

1 is odd.

result = 4 × 256 = 1024
power = 1 / 2 = 0
```

Loop terminates.

Final result:

```cpp
1024
```

### Example 2 — Negative Exponent

Input:

```cpp
x = 2
n = -3
```

First handle the negative exponent:

```cpp
x = 1 / x;
power = -power;
```

Now:

```text
x = 0.5
power = 3
```

Dry run:

| Iteration | Power | Base | Odd? | Result |
|---|---:|---:|---|---:|
| 1 | 3 | 0.5 | Yes | 0.5 |
| 2 | 1 | 0.25 | Yes | 0.125 |

Final result:

```text
0.125
```

Mathematically:

```text
2^-3 = 1 / 2^3
     = 1 / 8
     = 0.125
```

---

## 6. Understanding Each Important Line

### Step 1: Convert exponent to long long

```cpp
long long power = n;
```

Why?

Because the minimum 32-bit integer is:

```cpp
INT_MIN = -2147483648
```

Its positive magnitude is:

```text
2147483648
```

This cannot be represented by a signed 32-bit integer.

Therefore, we convert `n` into `long long` before negating it.

### Step 2: Handle negative exponent

```cpp
if (power < 0) {
    x = 1 / x;
    power = -power;
}
```

Mathematical property:

```text
x^-n = 1 / x^n
```

Example:

```text
2^-3 = 1 / 2^3
```

After this step, the exponent is always non-negative.

### Step 3: Initialize result

```cpp
double result = 1.0;
```

We start with `1` because multiplying any number by `1` does not change its value.

### Step 4: Check whether exponent is odd

```cpp
if (power % 2 == 1) {
    result *= x;
}
```

For odd exponents, we must include the current base in the result.

### Step 5: Square the base

```cpp
x *= x;
```

This is the most important operation.

Example:

```text
2 → 4 → 16 → 256 → 65536
```

Each step squares the previous base.

### Step 6: Divide exponent by 2

```cpp
power /= 2;
```

The exponent becomes smaller by half in each iteration.

This is why the algorithm is logarithmic.

---

## 7. Approach 3 — Recursive Binary Exponentiation

### Idea

Use the same mathematical property recursively.

```text
x^n = (x × x)^(n/2)       if n is even

x^n = x × x^(n-1)        if n is odd
```

### C++ Code

```cpp
class Solution {
public:

    double fastPow(double x, long long n) {

        if (n == 0) {
            return 1.0;
        }

        double half = fastPow(x, n / 2);

        if (n % 2 == 0) {
            return half * half;
        }

        return x * half * half;
    }

    double myPow(double x, int n) {

        long long power = n;

        if (power < 0) {
            x = 1 / x;
            power = -power;
        }

        return fastPow(x, power);
    }
};
```

### Recursive Dry Run

Calculate:

```text
2^5
```

Recursive calls:

```text
fastPow(2, 5)
    fastPow(2, 2)
        fastPow(2, 1)
            fastPow(2, 0)
```

Unwinding:

```text
fastPow(2, 0) = 1

fastPow(2, 1)
= 2 × 1 × 1
= 2

fastPow(2, 2)
= 2 × 2
= 4

fastPow(2, 5)
= 2 × 4 × 4
= 32
```

### Complexity

- **Time:** O(log |n|)
- **Space:** O(log |n|) due to the recursion stack.

---

## 8. Comparison of Approaches

| Approach | Time | Auxiliary Space |
|---|---|---|
| Brute Force | O(|n|) | O(1) |
| Iterative Binary Exponentiation | O(log |n|) | O(1) |
| Recursive Binary Exponentiation | O(log |n|) | O(log |n|) |

### Best Approach

Prefer iterative Binary Exponentiation in interviews because:

- It is efficient.
- It uses constant auxiliary space.
- It avoids recursion overhead.
- It handles negative exponents safely.

---

## 9. Common Mistakes

1. Using `n` directly when it is negative.
2. Negating `INT_MIN` while still using an `int`.
3. Forgetting to take the reciprocal for negative exponents.
4. Forgetting to multiply the result when the exponent is odd.
5. Forgetting to square the base.
6. Dividing the exponent incorrectly.
7. Initializing result to `0` instead of `1`.
8. Using an O(n) loop instead of binary exponentiation.

### Most Important Edge Case

```cpp
x = 2.0
n = -2147483648
```

Do not write:

```cpp
int power = abs(n);
```

This can overflow because the positive magnitude of `INT_MIN` cannot fit in a signed 32-bit integer.

Correct:

```cpp
long long power = n;

if (power < 0) {
    x = 1 / x;
    power = -power;
}
```

---

## 10. Interview Insights

### Q1. What is Binary Exponentiation?

An algorithm that calculates powers by repeatedly squaring the base and dividing the exponent by two.

### Q2. Why is it faster than brute force?

Brute force performs O(n) multiplications, while binary exponentiation performs O(log n) iterations.

### Q3. How do you handle negative exponents?

Use:

```text
x^-n = 1 / x^n
```

### Q4. Why do we check whether the exponent is odd?

Because an odd exponent leaves one extra factor of the current base.

### Q5. Why do we square the base?

It allows us to combine pairs of powers efficiently.

### Q6. Why use long long for the exponent?

To safely handle `INT_MIN` when converting a negative exponent into its positive magnitude.

### Q7. What is the space complexity of iterative binary exponentiation?

O(1) auxiliary space.

---

## 11. Pattern Recognition

| Problem Requirement | Technique |
|---|---|
| Calculate x raised to n | Binary Exponentiation |
| Reduce operations by half | Divide and Conquer |
| Handle odd exponent | Conditional multiplication |
| Handle negative exponent | Reciprocal |
| Work with exponent bits | Bit Manipulation |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Pow(x, n) | 50 | Binary Exponentiation |
| Sqrt(x) | 69 | Binary Search |
| Super Pow | 372 | Modular Exponentiation |
| Count Good Numbers | 1922 | Fast Power |
| Matrix Power | — | Binary Exponentiation |

---

## 12. Key Takeaways

Remember these four rules:

1. Convert the exponent to `long long`.
2. If the exponent is negative, take the reciprocal.
3. If the exponent is odd, multiply the result by the current base.
4. Square the base and divide the exponent by two.

### Optimal Code to Remember

```cpp
class Solution {
public:
    double myPow(double x, int n) {

        long long power = n;

        if (power < 0) {
            x = 1 / x;
            power = -power;
        }

        double result = 1.0;

        while (power > 0) {

            if (power % 2 == 1) {
                result *= x;
            }

            x *= x;
            power /= 2;
        }

        return result;
    }
};
```

### Final Complexity

- **Time:** O(log |n|)
- **Auxiliary Space:** O(1)
- **Pattern:** Binary Exponentiation

**Interview takeaway:** Whenever a problem asks you to calculate a large power efficiently, think about Binary Exponentiation instead of repeated multiplication.

# Multiply Strings

- **Difficulty:** Medium
- **Problem:** LeetCode 43
- **Pattern:** String Manipulation / Math / Simulation
- **Language:** C++
- **Key Techniques:** Nested Loops, Carry Handling, Array Indexing

## 1. Problem Understanding

Given two strings `num1` and `num2` representing non-negative integers, return their product as a string.

### Important Restrictions

- You cannot convert the input strings directly into integers using built-in conversion functions.
- You cannot use built-in multiplication of the complete numbers.
- The result must also be returned as a string.
- Neither input contains leading zeros unless the number itself is `"0"`.

### Example 1

**Input:**

```cpp
num1 = "3", num2 = "4"
```

**Output:**

```cpp
"12"
```

### Example 2

**Input:**

```cpp
num1 = "111", num2 = "222"
```

**Output:**

```cpp
"24642"
```

**Explanation:**

```text
    111
  × 222
  -----
    222
   2220
  22200
  -----
  24642
```

### Main Challenge

How can we multiply two large numbers without converting them into integers?

**Answer:** Simulate the multiplication process that we perform manually.

---

## 2. Approach 1 — Brute Force / Manual Multiplication

### Idea

Multiply each digit of the first number with each digit of the second number, maintaining carry values.

For example:

```text
num1 = "12"
num2 = "34"
```

We can calculate:

```text
12 × 34

12 × 4  = 48
12 × 30 = 360

48 + 360 = 408
```

We can implement this using digit multiplication and addition.

However, repeatedly building partial product strings and adding them is more complicated.

A better implementation uses a single result array and directly updates the appropriate digit positions.

---

## 3. Approach 2 — Optimal Solution (Digit-by-Digit Multiplication)

### Core Idea

For two numbers:

```text
num1 = "123"
num2 = "45"
```

The maximum possible result length is:

```text
m + n
```

Where:
- `m` = length of `num1`
- `n` = length of `num2`

Therefore, create an array of size:

```cpp
m + n
```

Initially, every position contains zero.

```text
num1 = 123
num2 =  45

result = [0, 0, 0, 0, 0]
```

### Important Index Formula

When multiplying:

```text
num1[i] × num2[j]
```

The corresponding result positions are:

```cpp
result[i + j]
result[i + j + 1]
```

Why?

Because multiplying two digits contributes to the corresponding decimal place.

For example:

```text
      123
    ×  45
    -----
      615
     4920
    -----
     5535
```

The result array allows us to combine these partial products without creating separate strings.

---

## 4. Optimal C++ Code

```cpp
class Solution {
public:
    string multiply(string num1, string num2) {

        if (num1 == "0" || num2 == "0") {
            return "0";
        }

        int m = num1.size();
        int n = num2.size();

        vector<int> result(m + n, 0);

        for (int i = m - 1; i >= 0; i--) {

            for (int j = n - 1; j >= 0; j--) {

                int digit1 = num1[i] - '0';
                int digit2 = num2[j] - '0';

                int product = digit1 * digit2;

                int sum = product + result[i + j + 1];

                result[i + j + 1] = sum % 10;

                result[i + j] += sum / 10;
            }
        }

        string answer = "";

        int i = 0;

        while (i < result.size() && result[i] == 0) {
            i++;
        }

        while (i < result.size()) {
            answer += to_string(result[i]);
            i++;
        }

        return answer;
    }
};
```

---

## 5. Understanding the Code Step by Step

### Step 1: Handle Zero

```cpp
if (num1 == "0" || num2 == "0") {
    return "0";
}
```

If either number is zero, the product is zero.

### Step 2: Get String Lengths

```cpp
int m = num1.size();
int n = num2.size();
```

For example:

```text
num1 = "123" → m = 3
num2 = "45"  → n = 2
```

### Step 3: Create Result Array

```cpp
vector<int> result(m + n, 0);
```

Maximum possible result length is `m + n`.

For our example:

```text
m + n = 3 + 2 = 5

result = [0, 0, 0, 0, 0]
```

### Step 4: Traverse Both Strings Backwards

```cpp
for (int i = m - 1; i >= 0; i--) {
    for (int j = n - 1; j >= 0; j--) {
```

Why backwards?

Because multiplication starts from the rightmost digit, just like normal mathematical multiplication.

### Step 5: Convert Characters Into Digits

```cpp
int digit1 = num1[i] - '0';
int digit2 = num2[j] - '0';
```

For example:

```text
'3' - '0' = 3
'5' - '0' = 5
```

This converts individual characters into digits. It does not convert the entire string into an integer.

### Step 6: Multiply the Digits

```cpp
int product = digit1 * digit2;
```

For example:

```text
3 × 5 = 15
```

### Step 7: Add Existing Result Digit

```cpp
int sum = product + result[i + j + 1];
```

The result position might already contain a digit from an earlier multiplication.

Therefore, we must include it.

### Step 8: Store the Current Digit

```cpp
result[i + j + 1] = sum % 10;
```

The remainder gives the current digit.

Example:

```text
sum = 15

15 % 10 = 5
```

### Step 9: Store Carry

```cpp
result[i + j] += sum / 10;
```

Integer division gives the carry.

Example:

```text
15 / 10 = 1
```

The carry goes into the previous position.

### Step 10: Remove Leading Zeros

```cpp
while (i < result.size() && result[i] == 0) {
    i++;
}
```

The result array may contain leading zeros.

We skip them before constructing the answer.

### Step 11: Build the Answer String

```cpp
while (i < result.size()) {
    answer += to_string(result[i]);
    i++;
}
```

Convert each result digit into a character representation and append it to the answer.

---

## 6. Detailed Dry Run

Input:

```cpp
num1 = "12"
num2 = "34"
```

Initial result:

```text
result = [0, 0, 0, 0]
```

We use:

```text
m = 2
n = 2
```

### Iteration 1

```text
i = 1
j = 1

digit1 = 2
digit2 = 4

product = 2 × 4 = 8

sum = 8 + result[2]
    = 8 + 0
    = 8
```

Update:

```cpp
result[2] = 8 % 10;
result[1] += 8 / 10;
```

Result:

```text
[0, 0, 8, 0]
```

### Iteration 2

```text
i = 1
j = 0

digit1 = 2
digit2 = 3

product = 2 × 3 = 6

sum = 6 + result[1]
    = 6 + 0
    = 6
```

Update:

```text
result[2] = 6 % 10 = 6
result[1] += 6 / 10 = 0
```

Result:

```text
[0, 6, 8, 0]
```

### Iteration 3

```text
i = 0
j = 1

digit1 = 1
digit2 = 4

product = 1 × 4 = 4

sum = 4 + result[2]
    = 4 + 8
    = 12
```

Update:

```text
result[2] = 12 % 10 = 2
result[1] += 12 / 10 = 1
```

Result:

```text
[0, 7, 2, 0]
```

### Iteration 4

```text
i = 0
j = 0

digit1 = 1
digit2 = 3

product = 1 × 3 = 3

sum = 3 + result[1]
    = 3 + 7
    = 10
```

Update:

```text
result[1] = 10 % 10 = 0
result[0] += 10 / 10 = 1
```

Result:

```text
[1, 0, 2, 0]
```

Final answer:

```text
"1020"
```

Therefore:

```text
12 × 34 = 408
```

**Correction to the dry-run indexing above:** The standard implementation must add carry to the preceding position after accounting for the existing result digit. The code in Section 4 uses `result[i + j] += sum / 10`, which is valid for this implementation. The correct final result for `12 × 34` is `[0, 4, 0, 8]`, giving `"408"`.

---

## 7. Complexity Analysis

Let:
- `m` = length of `num1`
- `n` = length of `num2`

### Time Complexity

The outer loop executes `m` times.

The inner loop executes `n` times.

Therefore:

```text
O(m × n)
```

Building the answer takes O(m + n).

Overall:

```text
O(m × n)
```

### Space Complexity

The result array requires:

```text
O(m + n)
```

Auxiliary space:

```text
O(m + n)
```

---

## 8. Common Mistakes

1. Trying to convert the complete strings into `int` or `long long`.
2. Creating a result array of incorrect size.
3. Traversing from left to right instead of right to left.
4. Forgetting to add the existing result digit.
5. Storing the complete product instead of only the last digit.
6. Forgetting to propagate carry.
7. Returning leading zeros.
8. Forgetting the zero-input edge case.
9. Using incorrect result indices.

### Most Important Formula

For multiplying digits at positions `i` and `j`:

```cpp
int sum = (num1[i] - '0') *
          (num2[j] - '0') +
          result[i + j + 1];

result[i + j + 1] = sum % 10;

result[i + j] += sum / 10;
```

Remember this indexing pattern.

---

## 9. Important Interview Insights

### Q1. Why is the result array size `m + n`?

The product of an m-digit number and an n-digit number can have at most `m + n` digits.

### Q2. Why do we traverse backwards?

Because multiplication starts from the least significant digit.

### Q3. Why use `% 10`?

To extract the last digit of the current sum.

### Q4. Why use `/ 10`?

To extract the carry.

### Q5. Why subtract `'0'`?

To convert an individual numeric character into its integer digit.

### Q6. Can this problem be solved without converting strings into integers?

Yes. We multiply individual digits and store the result in an integer array.

### Q7. What is the time complexity?

O(m × n).

### Q8. Why remove leading zeros?

The result must be returned in its normal decimal representation.

---

## 10. Pattern Recognition

| Problem Requirement | Technique |
|---|---|
| Multiply large numbers represented as strings | Digit simulation |
| Handle carry | Division and modulo |
| Avoid integer overflow | Character-by-character processing |
| Store partial products | Result array |
| Remove unnecessary leading zeros | String processing |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Add Strings | 415 | Carry Simulation |
| Multiply Strings | 43 | Digit Multiplication |
| Plus One | 66 | Carry Propagation |
| Add Binary | 67 | Binary Addition |
| String to Integer (atoi) | 8 | String Parsing |

---

## 11. Key Takeaways

Remember these points:

1. Never convert the complete strings into integers.
2. Create a result array of size `m + n`.
3. Traverse both strings from right to left.
4. Multiply individual digits.
5. Use `% 10` for the current digit.
6. Use `/ 10` for carry.
7. Remove leading zeros before returning the answer.

### Optimal Complexity

- **Time:** O(m × n)
- **Space:** O(m + n)
- **Pattern:** String Manipulation + Mathematical Simulation

**Interview takeaway:** When a problem involves arithmetic on very large numbers represented as strings, think about simulating the manual arithmetic process using digit arrays and carry propagation.


# Detect Squares

- **LeetCode:** 2013
- **Difficulty:** Medium
- **Pattern:** Hash Map + Geometry + Frequency Counting
- **Language:** C++

---

## 1. Problem Understanding

You are given a stream of 2D points represented by coordinates `(x, y)`.

Implement a `CountSquares` class with three operations:

1. `CountSquares()` – Initializes the object.
2. `add(point)` – Adds a new point to the data structure.
3. `count(point)` – Counts the number of ways to choose three previously added points such that, together with the query point, they form an axis-aligned square.

### Important Conditions

- Squares must have sides parallel to the X-axis and Y-axis.
- Duplicate points are allowed.
- Duplicate points must be counted as separate points.
- The query point is not automatically added to the data structure.

### Example

Query point:

`[1, 1]`

Previously added points:

`[1, 2]`, `[2, 1]`, `[2, 2]`

These four points form a square:

```text
(1,2) -------- (2,2)
  |               |
  |               |
(1,1) -------- (2,1)
```

Output: `1`

---

## 2. Brute Force Approach

### Idea

Store all previously added points in a vector.

For every query:

1. Select the first stored point.
2. Select the second stored point.
3. Select the third stored point.
4. Check whether these three points and the query point form a valid axis-aligned square.
5. Count every valid combination.

### Square Validation

For four points to form an axis-aligned square:

- They must have exactly two distinct X-coordinates.
- They must have exactly two distinct Y-coordinates.
- All four coordinate combinations must exist.
- Horizontal and vertical side lengths must be equal.

### Complexity

If there are `N` stored points:

- **Time:** O(N³) combinations per query, assuming constant-time validation.
- **Space:** O(N) to store the points.

This approach is inefficient for large inputs.

---

## 3. Optimal Approach – Hash Map + Diagonal Checking

### Core Observation

In an axis-aligned square, diagonally opposite corners have:

- Different X-coordinates.
- Different Y-coordinates.
- Equal absolute horizontal and vertical distances.

Suppose:

```text
Query point = (qx, qy)
Opposite point = (x, y)
```

They can be diagonal corners of a square when:

```cpp
x != qx
y != qy
abs(x - qx) == abs(y - qy)
```

The other two corners must be:

```text
(x, qy)
(qx, y)
```

### Example

Query point: `(1, 1)`

Opposite point: `(3, 3)`

Other two corners:

```text
(3, 1)
(1, 3)
```

The four points form an axis-aligned square with side length 2.

### Hash Map Structure

Use a nested unordered map:

```cpp
unordered_map<int, unordered_map<int, int>> points;
```

Meaning:

```cpp
points[x][y] = frequency of coordinate (x, y);
```

For example, if `(2, 2)` is added three times:

```cpp
points[2][2] = 3;
```

This is important because duplicate points must be counted separately.

---

## 4. Optimal C++ Code

```cpp
class CountSquares {
private:
    unordered_map<int, unordered_map<int, int>> points;

public:
    CountSquares() {

    }

    void add(vector<int> point) {
        int x = point[0];
        int y = point[1];

        points[x][y]++;
    }

    int count(vector<int> point) {
        int qx = point[0];
        int qy = point[1];

        int ans = 0;

        // Try every unique stored coordinate as the opposite corner.
        for (const auto& [x, ys] : points) {
            for (const auto& [y, freq] : ys) {

                // Opposite corners cannot share X or Y coordinates.
                if (x == qx || y == qy) {
                    continue;
                }

                // Horizontal and vertical distances must be equal.
                if (abs(x - qx) != abs(y - qy)) {
                    continue;
                }

                // Find the remaining two corners.
                auto it1 = points.find(x);
                auto it2 = points.find(qx);

                if (it1 == points.end() || it2 == points.end()) {
                    continue;
                }

                auto p1 = it1->second.find(qy);
                auto p2 = it2->second.find(y);

                if (p1 != it1->second.end() &&
                    p2 != it2->second.end()) {

                    ans += freq * p1->second * p2->second;
                }
            }
        }

        return ans;
    }
};
```

---

## 5. Understanding the Code Step by Step

### Step 1: Store Coordinate Frequencies

```cpp
points[x][y]++;
```

Every time a point is added, increase its frequency.

Example:

```text
add([2,2])
add([2,2])
add([2,2])
```

Map:

```text
points[2][2] = 3
```

### Step 2: Get Query Coordinates

```cpp
int qx = point[0];
int qy = point[1];
```

For query `[1,1]`:

```text
qx = 1
qy = 1
```

### Step 3: Iterate Over Stored Points

```cpp
for (const auto& [x, ys] : points) {
    for (const auto& [y, freq] : ys) {
```

Consider each unique stored coordinate as a possible diagonally opposite corner.

### Step 4: Reject Invalid Diagonal Candidates

```cpp
if (x == qx || y == qy) {
    continue;
}
```

The opposite corner must differ in both coordinates.

### Step 5: Check Equal Distances

```cpp
if (abs(x - qx) != abs(y - qy)) {
    continue;
}
```

This ensures the diagonal corners can belong to an axis-aligned square.

### Step 6: Find the Other Two Corners

The missing coordinates are:

```text
(x, qy)
(qx, y)
```

Use `find()` to retrieve their frequencies.

### Step 7: Multiply Frequencies

```cpp
ans += freq * p1->second * p2->second;
```

Every combination of the three stored corners creates a separate valid square.

---

## 6. Detailed Dry Run

Operations:

```cpp
add([1,1]);
add([2,2]);
add([2,1]);
add([1,2]);
```

### Step 1: Build the Frequency Map

| Coordinate | Frequency |
|---|---:|
| (1,1) | 1 |
| (2,2) | 1 |
| (2,1) | 1 |
| (1,2) | 1 |

### Step 2: Execute Query

```cpp
count([1,1]);
```

Therefore:

```text
qx = 1
qy = 1
ans = 0
```

### Iteration 1: Candidate `(1,1)`

`x == qx`

Skip because it cannot be the opposite corner.

### Iteration 2: Candidate `(2,2)`

```text
abs(x - qx) = abs(2 - 1) = 1
abs(y - qy) = abs(2 - 1) = 1
```

Both distances are equal.

Find the remaining corners:

```text
(2,1) → frequency 1
(1,2) → frequency 1
```

Calculate:

```cpp
ans += 1 * 1 * 1;
```

Now:

```text
ans = 1
```

### Iteration 3: Candidate `(2,1)`

`y == qy`

Skip.

### Iteration 4: Candidate `(1,2)`

`x == qx`

Skip.

### Final Output

```text
count([1,1]) = 1
```

---

## 7. Why Do We Multiply Frequencies?

Suppose the three required stored corners have these frequencies:

```text
Opposite corner: 2
Second corner:   3
Third corner:    4
```

The total number of combinations is:

```text
2 * 3 * 4 = 24
```

Each occurrence of a coordinate represents a separate point.

Therefore:

```cpp
ans += freq1 * freq2 * freq3;
```

This is the most important counting concept in this problem.

---

## 8. Complexity Analysis

Let `U` represent the number of unique coordinates stored.

| Operation | Time Complexity |
|---|---|
| Constructor | O(1) |
| add() | O(1) average |
| count() | O(U) average |
| Space | O(U) |

### Explanation

**add():**

One frequency is incremented in the nested hash map.

Average complexity: O(1).

**count():**

We iterate through every unique stored coordinate. For each candidate, we perform a constant number of average O(1) hash map lookups.

Therefore:

```text
Time = O(U) average
```

**Space:**

We store frequencies for all unique coordinates.

```text
Space = O(U)
```

---

## 9. Common Mistakes

1. Using `set` instead of a frequency map and losing duplicate information.
2. Forgetting to multiply the frequencies of all three stored corners.
3. Checking only whether coordinates are different without checking equal distances.
4. Counting squares with diagonal sides instead of axis-aligned sides.
5. Accidentally adding the query point inside `count()`.
6. Using `operator[]` to access missing coordinates while iterating over the same unordered map.
7. Assuming each unique coordinate represents only one point.

---

## 10. Important Interview Questions

### Q1. Why do we use a nested Hash Map?

To store the frequency of every coordinate pair and retrieve the other corners efficiently.

### Q2. Why check equal horizontal and vertical distances?

Because the diagonally opposite corners of an axis-aligned square must differ equally in X and Y coordinates.

### Q3. Why multiply frequencies?

Because duplicate points are separate choices, and the choices for the three required corners are independent.

### Q4. What is the time complexity of count()?

O(U) average, where U is the number of unique stored coordinates.

### Q5. Why not use a normal unordered set?

A set stores only whether a coordinate exists. It does not preserve how many duplicate points were added.

### Q6. Can the opposite corner be above or below the query point?

Yes. We iterate over all stored points, so both orientations are considered.

### Q7. Can one square be counted twice?

No. For a fixed query corner, each valid square has exactly one diagonally opposite corner. Each unique opposite coordinate is visited once.

---

## 11. Pattern Recognition

| Requirement | Technique |
|---|---|
| Store point coordinates | Hash Map |
| Preserve duplicate points | Frequency Counting |
| Identify opposite corners | Geometry |
| Find other two corners | Coordinate Calculation |
| Count combinations | Frequency Multiplication |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Detect Squares | 2013 | Hash Map + Geometry |
| Valid Square | 593 | Geometry + Distance |
| Number of Boomerangs | 447 | Hash Map + Counting |
| Max Points on a Line | 149 | Geometry + Hash Map |

---

## 12. Final Revision Notes

Remember these points:

1. Store each coordinate's frequency using a nested Hash Map.
2. Treat every stored point as a possible opposite corner.
3. Check that both coordinates differ.
4. Check that horizontal and vertical distances are equal.
5. Calculate the other two corners.
6. Multiply the frequencies of all three stored corners.
7. Use `find()` for safe lookups while iterating.

### Optimal Complexity

- **add():** O(1) average
- **count():** O(U) average
- **Space:** O(U)

**Interview takeaway:** This problem combines geometry with frequency counting. The key observation is that the opposite corners of an axis-aligned square have equal horizontal and vertical distances.

**Interview takeaway:**

Whenever you see a square matrix rotation problem, first think about transpose, row reversal, column reversal, and layer-by-layer swapping.
