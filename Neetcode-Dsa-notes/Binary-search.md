# 21. Binary Search

**NeetCode Pattern:** Binary Search  
**Technique:** Iterative Binary Search  
**Difficulty:** Easy

## 1. Problem Understanding

You are given an array of integers `nums`, sorted in ascending order, and an integer `target`.

Return the index of `target` if it exists in the array. Otherwise, return `-1`.

### Example 1

```text
Input:
nums = [-1,0,3,5,9,12]
target = 9

Output: 4
```

The target `9` is present at index `4`.

### Example 2

```text
Input:
nums = [-1,0,3,5,9,12]
target = 2

Output: -1
```

The target `2` does not exist.

---

## 2. Core Intuition

Binary Search works on a **sorted array**.

Instead of checking every element one by one, we check the middle element.

Based on the comparison, we eliminate half of the remaining search space.

We maintain three pointers:

| Variable | Purpose |
|---|---|
| `left` | Beginning of the search range |
| `right` | End of the search range |
| `mid` | Middle index of the search range |

The main idea:

- If `nums[mid] == target`, return `mid`.
- If `nums[mid] < target`, search the right half.
- If `nums[mid] > target`, search the left half.

## 3. Visual Understanding

Consider:

```text
nums = [-1, 0, 3, 5, 9, 12]
target = 9
```

Initial search range:

```text
[-1, 0, 3, 5, 9, 12]
 L           R
```

### Step 1

```cpp
mid = left + (right - left) / 2;
```

`mid = 2`, so `nums[mid] = 3`.

Since:

```text
3 < 9
```

The target must be on the right side.

Update:

```cpp
left = mid + 1;
```

### Step 2

New search range:

```text
[5, 9, 12]
 L     R
```

`mid = 4`, so `nums[mid] = 9`.

Target found!

Return index `4`.

---

## 4. Optimal C++ Solution

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {

        int left = 0;
        int right = nums.size() - 1;

        while (left <= right) {

            int mid = left + (right - left) / 2;

            if (nums[mid] == target) {
                return mid;
            }

            else if (nums[mid] < target) {
                left = mid + 1;
            }

            else {
                right = mid - 1;
            }
        }

        return -1;
    }
};
```

## 5. Understanding Important Conditions

### Condition 1: Why `while (left <= right)`?

```cpp
while (left <= right)
```

We continue searching while at least one index remains in the search range.

When:

```text
left > right
```

The search range is empty, which means the target does not exist.

### Condition 2: Why `left = mid + 1`?

```cpp
left = mid + 1;
```

If:

```text
nums[mid] < target
```

The target cannot be at `mid` or anywhere to its left because the array is sorted.

Therefore, eliminate that entire portion.

### Condition 3: Why `right = mid - 1`?

```cpp
right = mid - 1;
```

If:

```text
nums[mid] > target
```

The target cannot be at `mid` or anywhere to its right.

Therefore, search the left portion.

### Condition 4: Why calculate mid this way?

```cpp
int mid = left + (right - left) / 2;
```

This is safer than:

```cpp
int mid = (left + right) / 2;
```

Because `left + right` can overflow for sufficiently large integer indices.

---

## 6. Dry Run

Input:

```text
nums = [-1, 0, 3, 5, 9, 12]
target = 9
```

| Iteration | left | right | mid | nums[mid] | Action |
|---|---:|---:|---:|---:|---|
| 1 | 0 | 5 | 2 | 3 | Move left to 3 |
| 2 | 3 | 5 | 4 | 9 | Target found |

Output:

```text
4
```

### When target is absent

```text
nums = [-1, 0, 3, 5, 9, 12]
target = 2
```

| Iteration | left | right | mid | nums[mid] | Action |
|---|---:|---:|---:|---:|---|
| 1 | 0 | 5 | 2 | 3 | Move right to 1 |
| 2 | 0 | 1 | 0 | -1 | Move left to 1 |
| 3 | 1 | 1 | 1 | 0 | Move left to 2 |

Now `left = 2` and `right = 1`.

Since `left > right`, return `-1`.

---

## 7. Complexity Analysis

Let `n` be the number of elements.

**Time Complexity: O(log n)**

Each iteration eliminates approximately half of the remaining search space.

For example:

```text
16 → 8 → 4 → 2 → 1
```

**Space Complexity: O(1)**

We use only a few variables and do not create another data structure.

---

## 8. Alternative: Recursive Binary Search

Binary Search can also be implemented recursively.

```cpp
class Solution {
public:
    int binarySearch(vector<int>& nums, int left, int right, int target) {

        if (left > right) {
            return -1;
        }

        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            return mid;
        }

        if (nums[mid] < target) {
            return binarySearch(nums, mid + 1, right, target);
        }

        return binarySearch(nums, left, mid - 1, target);
    }

    int search(vector<int>& nums, int target) {
        return binarySearch(nums, 0, nums.size() - 1, target);
    }
};
```

- Time Complexity: O(log n)
- Space Complexity: O(log n) because of recursive call stack.

**Interview preference:** Use iterative binary search when possible because it uses O(1) extra space.

---

## 9. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Using `left < right` | Can skip the final remaining element |
| Updating `left = mid` | Can cause an infinite loop |
| Updating `right = mid` | Can cause an infinite loop |
| Forgetting `return -1` | Does not handle a missing target |
| Using `(left + right) / 2` | Can overflow for large indices |
| Applying ordinary binary search to an unsorted array | The half-elimination logic is not valid |

---

## 10. Edge Cases

| Input | Target | Expected |
|---|---:|---:|
| `[5]` | 5 | 0 |
| `[5]` | 3 | -1 |
| `[1,2,3,4,5]` | 1 | 0 |
| `[1,2,3,4,5]` | 5 | 4 |
| `[-5,-2,0,3,8]` | -2 | 1 |
| `[]` | 5 | -1 |

---

## 11. C++ Concepts to Remember

- `vector<int>&`: Passes the vector by reference, avoiding a copy.
- `nums.size()`: Returns the number of elements.
- `mid`: Represents the middle index.
- `left <= right`: Ensures the final remaining index is checked.
- `return -1`: Indicates the target is absent.
- Integer division: `/ 2` discards the fractional part.

---

## 12. Pattern Recognition for Interviews

Recognize Binary Search when:

- The array is sorted.
- You need to find a target efficiently.
- The problem allows eliminating half of the search space.
- You need to find a boundary or a position satisfying a monotonic condition.

**Important:** Binary Search is not limited to finding an exact target. It is also used for lower bounds, upper bounds, rotated arrays, and binary search on answers.

---

## 13. 30-Second Revision

1. Initialize `left = 0` and `right = n - 1`.
2. Run while `left <= right`.
3. Calculate `mid = left + (right-left)/2`.
4. If middle element equals target, return `mid`.
5. If middle element is smaller, move `left = mid + 1`.
6. Otherwise, move `right = mid - 1`.
7. If the loop ends, return `-1`.

**One-line interview explanation:**

"I use binary search because the array is sorted. I compare the target with the middle element and eliminate half of the remaining search space in every iteration, achieving O(log n) time and O(1) extra space."

# 22. Search a 2D Matrix

**NeetCode Pattern:** Binary Search  
**Technique:** Binary Search on a Flattened Matrix  
**Difficulty:** Medium

## 1. Problem Understanding

You are given an `m x n` matrix with the following properties:

1. Each row is sorted in ascending order.
2. The first integer of each row is greater than the last integer of the previous row.

Given an integer `target`, return `true` if it exists in the matrix. Otherwise, return `false`.

### Example 1

```text
Input:
matrix = [
  [1, 3, 5, 7],
  [10, 11, 16, 20],
  [23, 30, 34, 60]
]

target = 3

Output: true
```

### Example 2

```text
Input:
matrix = [
  [1, 3, 5, 7],
  [10, 11, 16, 20],
  [23, 30, 34, 60]
]

target = 13

Output: false
```

---

## 2. Core Intuition

Normally, a 2D matrix has rows and columns.

But because of the given conditions, we can imagine the matrix as a single sorted array.

Original matrix:

```text
[ 1,  3,  5,  7]
[10, 11, 16, 20]
[23, 30, 34, 60]
```

Imagine it as:

```text
[1, 3, 5, 7, 10, 11, 16, 20, 23, 30, 34, 60]
```

Now we can apply ordinary Binary Search.

**Important:** We do not actually create another array. We calculate the corresponding row and column from a one-dimensional index.

---

## 3. Converting 1D Index into 2D Coordinates

Suppose:

```cpp
int row = mid / n;
int col = mid % n;
```

Where `n` is the number of columns.

Why?

- Integer division by `n` gives the row.
- Modulo by `n` gives the column.

### Example

Suppose:

```text
matrix = [
  [1, 3, 5, 7],
  [10, 11, 16, 20],
  [23, 30, 34, 60]
]
```

There are 4 columns.

If:

```cpp
mid = 6;
```

Then:

```cpp
row = 6 / 4 = 1;
col = 6 % 4 = 2;
```

Therefore:

```cpp
matrix[1][2] = 16;
```

This is the key trick in this problem.

---

## 4. Optimal C++ Solution

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {

        int m = matrix.size();
        int n = matrix[0].size();

        int left = 0;
        int right = m * n - 1;

        while (left <= right) {

            int mid = left + (right - left) / 2;

            int row = mid / n;
            int col = mid % n;

            if (matrix[row][col] == target) {
                return true;
            }

            else if (matrix[row][col] < target) {
                left = mid + 1;
            }

            else {
                right = mid - 1;
            }
        }

        return false;
    }
};
```

This solution assumes the matrix has at least one row and one column, as in the usual problem constraints.

---

## 5. Dry Run

Input:

```text
matrix = [
  [1, 3, 5, 7],
  [10, 11, 16, 20],
  [23, 30, 34, 60]
]

target = 16
```

Matrix dimensions:

```text
m = 3
n = 4
```

Initial search range:

```cpp
left = 0;
right = 3 * 4 - 1;  // 11
```

| Iteration | left | right | mid | row | col | Value | Action |
|---|---:|---:|---:|---:|---:|---:|---|
| 1 | 0 | 11 | 5 | 1 | 1 | 11 | Move left to 6 |
| 2 | 6 | 11 | 8 | 2 | 0 | 23 | Move right to 7 |
| 3 | 6 | 7 | 6 | 1 | 2 | 16 | Found |

Output:

```text
true
```

---

## 6. Understanding the Important Conditions

### Condition 1: Total number of elements

```cpp
int right = m * n - 1;
```

If there are 3 rows and 4 columns:

```text
Total elements = 3 × 4 = 12
Valid indices = 0 to 11
```

### Condition 2: Convert index to row

```cpp
int row = mid / n;
```

For 4 columns:

- Indices 0–3 belong to row 0.
- Indices 4–7 belong to row 1.
- Indices 8–11 belong to row 2.

### Condition 3: Convert index to column

```cpp
int col = mid % n;
```

The modulo operation gives the position within the row.

### Condition 4: Binary Search updates

```cpp
if (matrix[row][col] < target) {
    left = mid + 1;
}
else {
    right = mid - 1;
}
```

The matrix behaves like one sorted array, so the usual binary search rules apply.

---

## 7. Complexity Analysis

Let:

- `m` = number of rows
- `n` = number of columns

**Time Complexity: O(log(m × n))**

We search across all `m × n` elements using binary search.

**Space Complexity: O(1)**

We do not create a flattened array. We only use a few variables.

---

## 8. Alternative Approach: Row Search + Binary Search

We can also identify the possible row first, then perform binary search inside that row.

For each row, check whether:

```cpp
matrix[i][0] <= target &&
target <= matrix[i][n - 1]
```

If the target falls within that row's range, binary search inside it.

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {

        int m = matrix.size();
        int n = matrix[0].size();

        for (int i = 0; i < m; i++) {

            if (target >= matrix[i][0] &&
                target <= matrix[i][n - 1]) {

                int left = 0;
                int right = n - 1;

                while (left <= right) {

                    int mid = left + (right - left) / 2;

                    if (matrix[i][mid] == target) {
                        return true;
                    }

                    else if (matrix[i][mid] < target) {
                        left = mid + 1;
                    }

                    else {
                        right = mid - 1;
                    }
                }

                return false;
            }
        }

        return false;
    }
};
```

- Time Complexity: O(m + log n) in the worst case.
- Space Complexity: O(1).

**Interview preference:** The flattened-index binary search is more efficient: O(log(m × n)).

---

## 9. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Using `mid / m` | Row calculation must divide by the number of columns |
| Using `mid % m` | Column calculation must use the number of columns |
| Setting `right = m * n` | Last valid index is `m*n - 1` |
| Creating a flattened array unnecessarily | Uses extra O(m*n) space |
| Applying this method to any sorted matrix | Both given ordering conditions are necessary |
| Forgetting the empty-matrix case in generalized code | Accessing `matrix[0]` would be invalid |

---

## 10. Edge Cases

| Matrix | Target | Expected |
|---|---:|---|
| `[[1]]` | 1 | true |
| `[[1]]` | 2 | false |
| `[[1,3,5]]` | 5 | true |
| `[[1],[3],[5]]` | 3 | true |
| `[[1,3],[5,7]]` | 4 | false |
| `[[1,3],[5,7]]` | 7 | true |

---

## 11. C++ Concepts to Remember

- `matrix.size()`: Number of rows.
- `matrix[0].size()`: Number of columns.
- `/`: Integer division.
- `%`: Modulo operator.
- `matrix[row][col]`: Access a 2D element.
- `vector<vector<int>>`: A 2D vector in C++.

---

## 12. Pattern Recognition for Interviews

Recognize this approach when:

- The matrix is sorted row-wise.
- The first element of each row is greater than the previous row's last element.
- You need to search for a target efficiently.

**Important distinction:**

This problem is different from **Search a 2D Matrix II**.

| Problem | Matrix property | Typical approach |
|---|---|---|
| Search a 2D Matrix | Each row and the entire row sequence are sorted | Binary Search |
| Search a 2D Matrix II | Rows and columns are independently sorted | Staircase Search |

Do not assume the flattened binary-search approach works for Search a 2D Matrix II.

---

## 13. 30-Second Revision

1. Calculate `m` and `n`.
2. Treat the matrix as a virtual sorted array.
3. Initialize `left = 0`, `right = m*n - 1`.
4. Calculate `mid`.
5. Convert it using `row = mid/n` and `col = mid%n`.
6. Compare `matrix[row][col]` with the target.
7. Update the binary search range.
8. Return `false` if the target is not found.

**One-line interview explanation:**

"Since the matrix is globally sorted across rows, I treat it as a virtual one-dimensional sorted array and apply binary search. I convert each middle index into row and column coordinates using division and modulo, achieving O(log(m*n)) time and O(1) extra space."

---

**Next problem:** Koko Eating Bananas.

# 23. Koko Eating Bananas

**NeetCode Pattern:** Binary Search  
**Technique:** Binary Search on Answer  
**Difficulty:** Medium

## 1. Problem Understanding

Koko has `n` piles of bananas. The `i-th` pile contains `piles[i]` bananas.

Koko can decide her eating speed `k` (bananas per hour).

Rules:

1. Koko chooses one pile to eat from each hour.
2. She eats `k` bananas from that pile.
3. If the pile has fewer than `k` bananas, she eats all of them.
4. She cannot eat from another pile during that same hour.

Given the integer array `piles` and the available hours `h`, return the minimum integer eating speed `k` that allows Koko to finish all bananas within `h` hours.

### Example 1

```text
Input:
piles = [3,6,7,11]
h = 8

Output:
4
```

At speed `k = 4`:

| Pile | Hours required |
|---:|---:|
| 3 | 1 |
| 6 | 2 |
| 7 | 2 |
| 11 | 3 |
| **Total** | **8** |

Koko finishes in exactly 8 hours.

### Example 2

```text
Input:
piles = [30,11,23,4,20]
h = 5

Output:
30
```

### Example 3

```text
Input:
piles = [30,11,23,4,20]
h = 6

Output:
23
```

---

## 2. Core Intuition

We need to find the **minimum possible eating speed**.

Think about how speed affects the time:

- If Koko eats slowly, she needs more hours.
- If Koko eats quickly, she needs fewer hours.
- Once a speed is sufficient, every higher speed is also sufficient.

This creates a monotonic pattern:

```text
Speed:       1   2   3   4   5   6   7   8 ...
Can finish:  No  No  No  Yes Yes Yes Yes Yes
```

We need the first `Yes`.

This is exactly where **Binary Search on Answer** is useful.

## 3. Search Space: Minimum and Maximum Speed

What is the minimum possible speed?

```cpp
left = 1;
```

Koko must eat at least one banana per hour.

What is the maximum necessary speed?

```cpp
right = *max_element(piles.begin(), piles.end());
```

If Koko eats at the size of the largest pile, she can finish each pile in one hour.

Therefore:

```text
left = 1
right = maximum pile size
```

## 4. Calculate Hours Required for a Speed

Suppose:

```text
piles = [3,6,7,11]
k = 4
```

Hours for each pile:

```text
3 / 4  → 1 hour
6 / 4  → 2 hours
7 / 4  → 2 hours
11 / 4 → 3 hours
```

Total:

```text
1 + 2 + 2 + 3 = 8 hours
```

We need to calculate the ceiling division:

\[
\text{Hours} = \left\lceil \frac{\text{pile}}{k} \right\rceil
\]

In C++, use:

```cpp
hours += (pile + k - 1) / k;
```

This calculates ceiling division using integer arithmetic.

For example:

```text
pile = 7, k = 4

(7 + 4 - 1) / 4
= 10 / 4
= 2
```

## 5. Optimal C++ Solution

```cpp
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {

        int left = 1;
        int right = *max_element(piles.begin(), piles.end());

        while (left <= right) {

            int mid = left + (right - left) / 2;

            long long hours = 0;

            // Calculate total hours at speed mid
            for (int pile : piles) {
                hours += (pile + mid - 1) / mid;
            }

            if (hours <= h) {
                // Speed is sufficient; try a smaller speed
                right = mid - 1;
            }
            else {
                // Speed is too slow; increase it
                left = mid + 1;
            }
        }

        return left;
    }
};
```

**Why return `left`?**

Binary search is finding the first speed that satisfies the condition.

When the loop ends:

- All speeds smaller than `left` are insufficient.
- `left` is the minimum sufficient speed.

Therefore, return `left`.

## 6. Dry Run

Input:

```text
piles = [3,6,7,11]
h = 8
```

Initial search range:

```text
left = 1
right = 11
```

| Iteration | left | right | mid (speed) | Hours required | Action |
|---|---:|---:|---:|---:|---|
| 1 | 1 | 11 | 6 | 6 | Try smaller speed |
| 2 | 1 | 5 | 3 | 10 | Speed too slow |
| 3 | 4 | 5 | 4 | 8 | Try smaller speed |
| 4 | 4 | 3 | — | — | Stop |

Final answer:

```text
4
```

Why?

- Speed 3 needs 10 hours, which exceeds the limit.
- Speed 4 needs exactly 8 hours.
- Therefore, 4 is the minimum valid speed.

## 7. Understanding the Binary Search Conditions

### Condition 1: If hours are within the limit

```cpp
if (hours <= h) {
    right = mid - 1;
}
```

The current speed works.

But we want the minimum speed, so we search for a smaller one.

### Condition 2: If hours exceed the limit

```cpp
else {
    left = mid + 1;
}
```

The current speed is too slow.

We must increase the speed.

### Condition 3: Why not return `mid` immediately?

Because finding one valid speed does not guarantee that it is the minimum.

For example:

```text
Speed 4 → Valid
Speed 5 → Valid
Speed 6 → Valid
```

We must continue searching to find the first valid speed.

---

## 8. Complexity Analysis

Let:

- `n` = number of piles
- `M` = maximum pile size

**Time Complexity: O(n log M)**

- Binary search performs O(log M) iterations.
- Each iteration calculates the required hours by scanning all `n` piles.

**Space Complexity: O(1)**

Only a few variables are used.

---

## 9. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Using `pile / k` directly | Integer division rounds down, but we need ceiling division |
| Using `right = piles.size()` | Search range is based on eating speed, not number of piles |
| Returning `mid` immediately when valid | May not be the minimum valid speed |
| Using `right = mid` with `left <= right` | Can cause an infinite loop |
| Forgetting to increase speed when hours exceed `h` | Moves in the wrong direction |
| Using `int` for total hours without considering overflow | A large sum can exceed the integer range |

**Important C++ detail:** The code uses `long long hours` to safely accumulate the total required hours.

---

## 10. Edge Cases

| `piles` | `h` | Expected |
|---|---:|---:|
| `[3]` | 2 | 2 |
| `[3,6,7,11]` | 8 | 4 |
| `[30,11,23,4,20]` | 5 | 30 |
| `[30,11,23,4,20]` | 6 | 23 |
| `[1,1,1,1]` | 4 | 1 |
| `[1000000000]` | 2 | 500000000 |

---

## 11. C++ Concepts to Remember

- `max_element()`: Finds the largest element in a range.
- `*max_element(...)`: Dereferences the iterator to get the maximum value.
- `long long`: Stores larger integer values.
- Ceiling division: `(a + b - 1) / b` for positive integers.
- Binary Search on Answer: Searches for the smallest or largest feasible answer.

---

## 12. Pattern Recognition for Interviews

Recognize **Binary Search on Answer** when:

- The question asks for a minimum or maximum feasible value.
- You can test whether a particular answer is valid.
- If one answer is valid, all larger or smaller answers are also valid.
- The feasibility condition is monotonic.

Examples of similar problems:

- Koko Eating Bananas
- Capacity To Ship Packages Within D Days
- Minimum Number of Days to Make m Bouquets
- Split Array Largest Sum

---

## 13. 30-Second Revision

1. Search speed from `1` to the maximum pile.
2. Calculate `mid`.
3. For each pile, calculate hours using `(pile + mid - 1) / mid`.
4. Add all hours.
5. If `hours <= h`, speed works; search smaller.
6. If `hours > h`, speed is too slow; search larger.
7. Return `left`, the minimum valid speed.

**One-line interview explanation:**

"I use binary search on the eating speed because feasibility is monotonic. For each candidate speed, I calculate the total hours using ceiling division. If Koko can finish within `h` hours, I try a smaller speed; otherwise, I increase it. This gives O(n log M) time."

---

## 14. Important Difference: Ordinary Binary Search vs Binary Search on Answer

| Ordinary Binary Search | Binary Search on Answer |
|---|---|
| Searches for a target in sorted data | Searches for an optimal feasible value |
| Compares `nums[mid]` with target | Checks whether `mid` satisfies a condition |
| Returns target index or `-1` | Returns minimum/maximum valid answer |
| Example: Binary Search | Example: Koko Eating Bananas |

**Remember:** In Koko, we are not searching for a number inside `piles`. We are searching for the minimum eating speed.

# 24. Find Minimum in Rotated Sorted Array

**NeetCode Pattern:** Binary Search  
**Technique:** Modified Binary Search  
**Difficulty:** Medium

## 1. Problem Understanding

You are given an array `nums` that was originally sorted in ascending order, but it has been rotated at an unknown pivot.

Find and return the minimum element.

All elements are unique.

### Example 1

```text
Input:
nums = [3,4,5,1,2]

Output: 1
```

The original sorted array was:

```text
[1,2,3,4,5]
```

After rotation:

```text
[3,4,5,1,2]
```

Minimum element = `1`.

### Example 2

```text
Input:
nums = [4,5,6,7,0,1,2]

Output: 0
```

### Example 3

```text
Input:
nums = [11,13,15,17]

Output: 11
```

The array may not have been rotated at all.

---

## 2. Core Intuition

In a normal sorted array, the minimum is always at index `0`.

But after rotation, the array has two sorted portions.

Example:

```text
[4,5,6,7,0,1,2]
       ↑
   Rotation point
```

The two portions are:

```text
[4,5,6,7] and [0,1,2]
```

The minimum is the first element of the second portion.

We need to find this element efficiently using Binary Search.

## 3. Main Observation

Compare the middle element with the rightmost element.

```cpp
if (nums[mid] > nums[right])
```

This means the minimum must be in the right half, excluding `mid`.

Why?

Example:

```text
[4,5,6,7,0,1,2]
 L     M       R
```

Here:

```text
nums[mid] = 7
nums[right] = 2
```

Since `7 > 2`, the rotation point and minimum are to the right of `mid`.

Update:

```cpp
left = mid + 1;
```

### Case 2: Middle element is smaller than or equal to right

```cpp
else {
    right = mid;
}
```

The minimum is at `mid` or somewhere to its left.

Example:

```text
[4,5,6,7,0,1,2]
       L M   R
```

If `nums[mid] = 0` and `nums[right] = 2`, the minimum could be `mid` itself.

Therefore, we cannot eliminate `mid`.

Update:

```cpp
right = mid;
```

**Important:** We use `right = mid`, not `right = mid - 1`, because `mid` might be the minimum.

---

## 4. Optimal C++ Solution

```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {

        int left = 0;
        int right = nums.size() - 1;

        while (left < right) {

            int mid = left + (right - left) / 2;

            if (nums[mid] > nums[right]) {

                // Minimum is in the right half
                left = mid + 1;
            }

            else {

                // Minimum is at mid or in the left half
                right = mid;
            }
        }

        return nums[left];
    }
};
```

---

## 5. Dry Run

Input:

```text
nums = [4,5,6,7,0,1,2]
```

Initial values:

```text
left = 0
right = 6
```

| Iteration | left | right | mid | nums[mid] | nums[right] | Action |
|---|---:|---:|---:|---:|---:|---|
| 1 | 0 | 6 | 3 | 7 | 2 | `left = mid + 1` |
| 2 | 4 | 6 | 5 | 1 | 2 | `right = mid` |
| 3 | 4 | 5 | 4 | 0 | 1 | `right = mid` |

Now:

```text
left = 4
right = 4
```

The loop stops because `left == right`.

Return:

```cpp
nums[4] = 0;
```

Answer: `0`.

---

## 6. Why Do We Use `while (left < right)`?

```cpp
while (left < right)
```

We stop when both pointers point to the same index.

At that point, only one possible minimum remains.

Unlike ordinary Binary Search, we do not use `left <= right` here because we are narrowing the search to one candidate rather than checking for an exact target.

## 7. Why Is `right = mid` Important?

Consider:

```text
nums = [3,4,5,1,2]
```

Suppose `mid` points to `1`.

Since:

```text
nums[mid] < nums[right]
```

The minimum might be exactly at `mid`.

If we use:

```cpp
right = mid - 1;
```

We could accidentally discard the answer.

Therefore:

```cpp
right = mid;
```

keeps the middle element in the search range.

---

## 8. Complexity Analysis

Let `n` be the number of elements.

**Time Complexity: O(log n)**

Each iteration eliminates approximately half of the search space.

**Space Complexity: O(1)**

Only three variables are used.

---

## 9. Alternative Approach: Find the Rotation Point

Another way is to compare the middle element with the previous element and identify the point where the ascending order breaks.

However, handling boundaries requires extra conditions.

The previous solution is cleaner and is the recommended interview approach.

---

## 10. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Comparing `nums[mid]` only with `nums[left]` without handling boundaries | Can make the logic harder or incorrect |
| Using `right = mid - 1` | May discard the minimum |
| Using `left = mid` | Can cause an infinite loop |
| Using `while (left <= right)` with these exact updates | Does not follow the one-candidate stopping logic |
| Returning `nums[0]` | The array may be rotated |
| Forgetting the unrotated case | The minimum can be at index 0 |

---

## 11. Edge Cases

| Input | Expected |
|---|---:|
| `[1]` | 1 |
| `[2,1]` | 1 |
| `[3,4,5,1,2]` | 1 |
| `[4,5,6,7,0,1,2]` | 0 |
| `[11,13,15,17]` | 11 |
| `[2,3,4,5,1]` | 1 |

---

## 12. C++ Concepts to Remember

- `vector<int>&`: Passes the vector by reference.
- `nums.size() - 1`: Last valid index for a nonempty vector.
- `mid = left + (right-left)/2`: Safe midpoint calculation.
- `left < right`: Continue until one candidate remains.
- `right = mid`: Retain the middle element as a possible answer.

---

## 13. Pattern Recognition for Interviews

Recognize modified Binary Search when:

- The array was originally sorted.
- The array has been rotated.
- You need to find a minimum or a particular element.
- The ordering can help you eliminate half of the search space.

Related problems:

- Search in Rotated Sorted Array
- Find Minimum in Rotated Sorted Array
- Search a 2D Matrix
- Binary Search

---

## 14. 30-Second Revision

1. Initialize `left = 0`, `right = n-1`.
2. Run while `left < right`.
3. Calculate `mid`.
4. If `nums[mid] > nums[right]`, minimum is to the right: `left = mid + 1`.
5. Otherwise, minimum is at `mid` or to the left: `right = mid`.
6. When both pointers meet, return `nums[left]`.

**One-line interview explanation:**

"I use modified binary search. By comparing the middle element with the rightmost element, I determine which half contains the minimum. I repeatedly eliminate the other half until one candidate remains, achieving O(log n) time and O(1) space."

# 25. Search in Rotated Sorted Array

**NeetCode Pattern:** Binary Search  
**Technique:** Modified Binary Search  
**Difficulty:** Medium

## 1. Problem Understanding

You are given an integer array `nums` sorted in ascending order, but it has been rotated at an unknown pivot.

All elements are unique.

Given an integer `target`, return its index if it exists in the array. Otherwise, return `-1`.

### Example 1

```text
Input:
nums = [4,5,6,7,0,1,2]
target = 0

Output: 4
```

### Example 2

```text
Input:
nums = [4,5,6,7,0,1,2]
target = 3

Output: -1
```

### Example 3

```text
Input:
nums = [1]
target = 0

Output: -1
```

---

## 2. Core Intuition

In an ordinary sorted array, binary search is straightforward.

But after rotation, the array looks like this:

```text
[4, 5, 6, 7, 0, 1, 2]
```

The array consists of two sorted portions:

```text
[4, 5, 6, 7]  [0, 1, 2]
```

Even after rotation, **at least one half of the current search range is always sorted**.

Our strategy:

1. Calculate `mid`.
2. Check whether the left half or right half is sorted.
3. Determine whether the target belongs to that sorted half.
4. Eliminate the other half.

This allows us to maintain O(log n) time complexity.

---

## 3. Identify the Sorted Half

Suppose:

```text
nums = [4,5,6,7,0,1,2]
```

Initially:

```text
left = 0
right = 6
mid = 3

nums[mid] = 7
```

Compare:

```cpp
if (nums[left] <= nums[mid])
```

Since:

```text
4 <= 7
```

The left half is sorted:

```text
[4, 5, 6, 7]
```

Otherwise, the right half must be sorted.

### Case 1: Left half is sorted

```cpp
if (nums[left] <= nums[mid])
```

Now check whether the target lies within the sorted left half:

```cpp
if (nums[left] <= target && target < nums[mid])
```

If true:

```cpp
right = mid - 1;
```

Otherwise:

```cpp
left = mid + 1;
```

### Case 2: Right half is sorted

If the left half is not sorted, the right half must be sorted.

Check:

```cpp
if (nums[mid] < target && target <= nums[right])
```

If true:

```cpp
left = mid + 1;
```

Otherwise:

```cpp
right = mid - 1;
```

---

## 4. Optimal C++ Solution

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {

        int left = 0;
        int right = nums.size() - 1;

        while (left <= right) {

            int mid = left + (right - left) / 2;

            // Target found
            if (nums[mid] == target) {
                return mid;
            }

            // Left half is sorted
            if (nums[left] <= nums[mid]) {

                // Target belongs to the sorted left half
                if (nums[left] <= target && target < nums[mid]) {
                    right = mid - 1;
                }
                else {
                    left = mid + 1;
                }
            }

            // Right half is sorted
            else {

                // Target belongs to the sorted right half
                if (nums[mid] < target && target <= nums[right]) {
                    left = mid + 1;
                }
                else {
                    right = mid - 1;
                }
            }
        }

        return -1;
    }
};
```

---

## 5. Dry Run

Input:

```text
nums = [4,5,6,7,0,1,2]
target = 0
```

Initial values:

```text
left = 0
right = 6
```

| Iteration | left | right | mid | nums[mid] | Decision |
|---|---:|---:|---:|---:|---|
| 1 | 0 | 6 | 3 | 7 | Left half sorted; target not in it |
| 2 | 4 | 6 | 5 | 1 | Right half sorted; target not in it |
| 3 | 4 | 4 | 4 | 0 | Target found |

### Iteration 1

```text
[4,5,6,7,0,1,2]
 L     M       R
```

Left half is sorted:

```text
4 <= 7
```

But target `0` is not between `4` and `7`.

Therefore:

```cpp
left = mid + 1;  // left = 4
```

### Iteration 2

```text
[4,5,6,7,0,1,2]
             L M R
```

The current right half `[0,1,2]` is sorted.

Target `0` is not greater than `nums[mid] = 1`.

Therefore:

```cpp
right = mid - 1;  // right = 4
```

### Iteration 3

```text
left = 4
right = 4
mid = 4

nums[4] = 0
```

Target found.

Output:

```text
4
```

---

## 6. Understanding the Important Conditions

### Condition 1: Why `nums[left] <= nums[mid]`?

```cpp
if (nums[left] <= nums[mid])
```

This identifies whether the left portion is sorted.

We use `<=` because the left and middle pointers can sometimes refer to the same element.

### Condition 2: Why `nums[left] <= target`?

```cpp
nums[left] <= target && target < nums[mid]
```

The target must lie within the sorted left half.

If it does not, we eliminate that half.

### Condition 3: Why `target < nums[mid]` instead of `<=`?

Because we already checked:

```cpp
if (nums[mid] == target)
```

So the target cannot equal `nums[mid]` at this point.

### Condition 4: Why use `nums[mid] < target` in the right half?

```cpp
nums[mid] < target && target <= nums[right]
```

This checks whether the target belongs to the sorted right portion.

---

## 7. Complexity Analysis

Let `n` be the number of elements.

**Time Complexity: O(log n)**

Each iteration eliminates approximately half of the remaining search space.

**Space Complexity: O(1)**

Only a few variables are used.

---

## 8. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Applying ordinary binary search directly | The entire array is not necessarily sorted |
| Forgetting to check `nums[mid] == target` first | May miss the target at the middle |
| Using `nums[left] < nums[mid]` | Can fail when `left == mid` |
| Using incorrect range boundaries | Can eliminate a valid target |
| Forgetting `return -1` | Does not handle an absent target |
| Updating `left = mid` | Can cause an infinite loop |

---

## 9. Edge Cases

| Input | Target | Expected |
|---|---:|---:|
| `[1]` | 1 | 0 |
| `[1]` | 0 | -1 |
| `[3,1]` | 1 | 1 |
| `[1,3]` | 3 | 1 |
| `[4,5,6,7,0,1,2]` | 0 | 4 |
| `[4,5,6,7,0,1,2]` | 3 | -1 |
| `[1,2,3,4,5]` | 4 | 3 |

---

## 10. C++ Concepts to Remember

- `vector<int>&`: Passes the array by reference.
- `nums.size() - 1`: Last valid index.
- `mid = left + (right-left)/2`: Safe midpoint calculation.
- `<=` and `<`: Important for handling inclusive and exclusive boundaries.
- `return -1`: Indicates the target was not found.

---

## 11. Pattern Recognition for Interviews

Recognize Modified Binary Search when:

- The array was originally sorted.
- The array has been rotated.
- You need to find a target efficiently.
- At least one half of the current search range remains sorted.

### Difference Between the Previous Two Problems

| Problem | Objective | Key logic |
|---|---|---|
| Find Minimum in Rotated Sorted Array | Find the smallest value | Compare `nums[mid]` with `nums[right]` |
| Search in Rotated Sorted Array | Find a target index | Identify the sorted half and check target range |

---

## 12. 30-Second Revision

1. Initialize `left = 0`, `right = n-1`.
2. Calculate `mid`.
3. If `nums[mid] == target`, return `mid`.
4. Check whether the left half is sorted using `nums[left] <= nums[mid]`.
5. If sorted, check whether the target belongs to that half.
6. Otherwise, the right half is sorted; check whether the target belongs there.
7. Eliminate the other half.
8. Return `-1` if the search ends.

**One-line interview explanation:**

"I use modified binary search. In every iteration, at least one half is sorted. I identify that half, check whether the target belongs to its range, and eliminate the other half. This gives O(log n) time and O(1) extra space."

# 26. Time Based Key-Value Store

**NeetCode Pattern:** Binary Search  
**Technique:** Hash Map + Binary Search  
**Difficulty:** Medium

## 1. Problem Understanding

Design a data structure that stores multiple values for the same key, each associated with a timestamp.

Implement three functions:

- `TimeMap()` – Initializes the data structure.
- `set(key, value, timestamp)` – Stores the key-value pair with its timestamp.
- `get(key, timestamp)` – Returns the value associated with the largest timestamp that is less than or equal to the requested timestamp.

If no suitable timestamp exists, return an empty string.

### Example

```text
TimeMap timeMap;

timeMap.set("foo", "bar", 1);

timeMap.get("foo", 1);  // "bar"
timeMap.get("foo", 3);  // "bar"

timeMap.set("foo", "bar2", 4);

timeMap.get("foo", 4);  // "bar2"
timeMap.get("foo", 5);  // "bar2"
```

### Important Observation

For the key `"foo"`, we have:

```text
Timestamp    Value
1            bar
4            bar2
```

If someone asks for the value at timestamp `3`, there is no exact timestamp `3`.

We return `"bar"` because timestamp `1` is the largest timestamp that does not exceed `3`.

If someone asks for timestamp `5`, we return `"bar2"` because timestamp `4` is the closest previous timestamp.

---

## 2. Core Intuition

We need two data structures working together:

1. **Hash Map:** Quickly find the timestamps and values associated with a key.
2. **Binary Search:** Find the closest timestamp that is less than or equal to the requested timestamp.

We can maintain:

```cpp
unordered_map<string, vector<pair<int, string>>> mp;
```

Understand this structure:

```text
Key: "foo"

Value:
[
  {1, "bar"},
  {4, "bar2"},
  {8, "bar3"}
]
```

Each pair contains:

```text
{timestamp, value}
```

Since timestamps for a given key are provided in strictly increasing order, the vector is already sorted.

Therefore, Binary Search can efficiently find the required timestamp.

---

## 3. Optimal C++ Solution

```cpp
class TimeMap {
public:

    unordered_map<string, vector<pair<int, string>>> mp;

    TimeMap() {
        
    }

    void set(string key, string value, int timestamp) {

        mp[key].push_back({timestamp, value});
    }

    string get(string key, int timestamp) {

        if (mp.find(key) == mp.end()) {
            return "";
        }

        auto &values = mp[key];

        int left = 0;
        int right = values.size() - 1;

        string ans = "";

        while (left <= right) {

            int mid = left + (right - left) / 2;

            if (values[mid].first <= timestamp) {

                ans = values[mid].second;

                // Search for a later valid timestamp
                left = mid + 1;
            }

            else {

                // Timestamp is too large
                right = mid - 1;
            }
        }

        return ans;
    }
};
```

---

## 4. Understanding the `set()` Function

```cpp
void set(string key, string value, int timestamp) {
    mp[key].push_back({timestamp, value});
}
```

Suppose we call:

```cpp
set("foo", "bar", 1);
set("foo", "bar2", 4);
set("foo", "bar3", 8);
```

The Hash Map becomes:

```text
mp["foo"] = {
    {1, "bar"},
    {4, "bar2"},
    {8, "bar3"}
}
```

### Why use `push_back()`?

Because timestamps for each key are guaranteed to arrive in strictly increasing order.

Therefore, we can append new entries directly without sorting.

---

## 5. Understanding the `get()` Function

Suppose we call:

```cpp
get("foo", 6);
```

Stored values:

```text
Timestamp:  1       4       8
Value:      bar     bar2    bar3
```

We need the largest timestamp that satisfies:

```cpp
storedTimestamp <= 6
```

The answer is:

```text
Timestamp = 4
Value = "bar2"
```

### Why use Binary Search?

Because the timestamps are sorted.

We can eliminate half of the search range in each iteration.

### Important condition

```cpp
if (values[mid].first <= timestamp)
```

If the stored timestamp is valid:

1. Save its value as the current answer.
2. Search the right side for a potentially later valid timestamp.

```cpp
ans = values[mid].second;
left = mid + 1;
```

Otherwise, search the left side:

```cpp
right = mid - 1;
```

This ensures we find the largest valid timestamp, not just any valid timestamp.

---

## 6. Dry Run

Consider:

```cpp
set("foo", "bar", 1);
set("foo", "bar2", 4);
set("foo", "bar3", 8);

get("foo", 6);
```

Stored data:

```text
Index       0       1        2
Timestamp   1       4        8
Value       bar     bar2     bar3
```

We want:

```text
timestamp = 6
```

Initial values:

```text
left = 0
right = 2
ans = ""
```

| Iteration | left | right | mid | Stored timestamp | Action |
|---|---:|---:|---:|---:|---|
| 1 | 0 | 2 | 1 | 4 | Valid; save `"bar2"` and search right |
| 2 | 2 | 2 | 2 | 8 | Too large; search left |
| Stop | 2 | 1 | — | — | Return `"bar2"` |

Final output:

```text
"bar2"
```

---

## 7. Why Do We Search Right After Finding a Valid Timestamp?

This is the most important part of the problem.

Suppose:

```text
timestamps = [1, 4, 8, 10]
target = 9
```

Both timestamps `1`, `4`, and `8` are valid.

But we need the largest valid timestamp, which is `8`.

If we immediately return after finding `4`, the answer will be incorrect.

Therefore:

```cpp
ans = values[mid].second;
left = mid + 1;
```

We save the current candidate and continue searching for a better one.

---

## 8. Complexity Analysis

Let:

- `N` = total number of stored entries.
- `K` = number of entries for a particular key.

| Operation | Complexity |
|---|---|
| `TimeMap()` | O(1) |
| `set()` | O(1) average |
| `get()` | O(log K) |

**Space Complexity: O(N)**

We store every key, timestamp, and value.

The Hash Map lookup is O(1) average, and Binary Search takes O(log K).

---

## 9. Alternative: Lower Bound

C++ provides `lower_bound()`, which can simplify the Binary Search.

We need the first timestamp strictly greater than the requested timestamp.

The previous entry, if one exists, is our answer.

```cpp
class TimeMap {
public:
    unordered_map<string, vector<pair<int, string>>> mp;

    TimeMap() {}

    void set(string key, string value, int timestamp) {
        mp[key].push_back({timestamp, value});
    }

    string get(string key, int timestamp) {

        if (mp.find(key) == mp.end()) {
            return "";
        }

        auto &values = mp[key];

        auto it = upper_bound(
            values.begin(),
            values.end(),
            timestamp,
            [](int t, const pair<int, string>& p) {
                return t < p.first;
            }
        );

        if (it == values.begin()) {
            return "";
        }

        return prev(it)->second;
    }
};
```

**Important STL concept:**

- `lower_bound`: Finds the first element not less than the target.
- `upper_bound`: Finds the first element strictly greater than the target.
- `prev(it)`: Moves one iterator backward.

The manual Binary Search implementation is useful to understand the actual interview logic.

---

## 10. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Using only `unordered_map<string,string>` | Cannot store multiple timestamped values |
| Returning the exact timestamp only | The problem asks for the closest previous timestamp |
| Returning immediately when a valid timestamp is found | A later valid timestamp may exist |
| Searching for the smallest valid timestamp | We need the largest valid timestamp |
| Forgetting the missing-key check | Could access a nonexistent key |
| Sorting after every insertion | Unnecessary because timestamps arrive in increasing order |

---

## 11. Edge Cases

| Operations | Expected result |
|---|---|
| `get("unknown", 5)` | `""` |
| `set("foo","bar",1)`, `get("foo",1)` | `"bar"` |
| `set("foo","bar",1)`, `get("foo",0)` | `""` |
| `set("foo","bar",1)`, `get("foo",10)` | `"bar"` |
| `set("foo","bar",1)`, `set("foo","bar2",5)`, `get("foo",3)` | `"bar"` |
| Same entries, `get("foo",5)` | `"bar2"` |

---

## 12. C++ Concepts to Remember

- `unordered_map<string, vector<pair<int,string>>>`: Stores multiple timestamped values for each key.
- `pair<int,string>`: Stores timestamp and value together.
- `.first`: Accesses the timestamp.
- `.second`: Accesses the value.
- `push_back({timestamp, value})`: Appends a pair to the vector.
- `auto &values`: Creates a reference to the stored vector, avoiding a copy.
- `upper_bound()`: Finds the first timestamp greater than the requested timestamp.

---

## 13. Pattern Recognition for Interviews

Recognize **Hash Map + Binary Search** when:

- Multiple values are stored against the same key.
- Each key has sorted timestamps.
- You need the closest previous value.
- You need the largest timestamp less than or equal to a target.

Related concepts:

- Binary Search
- Upper Bound / Lower Bound
- Hash Map
- Sorted Vectors
- Time-based lookup

---

## 14. 30-Second Revision

1. Create `unordered_map<string, vector<pair<int,string>>>`.
2. In `set()`, append `{timestamp, value}` to the key's vector.
3. In `get()`, first check whether the key exists.
4. Binary search its timestamp vector.
5. If `values[mid].first <= timestamp`, save the value and search right.
6. Otherwise, search left.
7. Return the last valid value or an empty string.

**One-line interview explanation:**

"I use a Hash Map to store a sorted vector of timestamp-value pairs for each key. Since timestamps arrive in increasing order, I can append in O(1) average time and use Binary Search to find the largest timestamp not exceeding the requested timestamp in O(log K) time."

# 27. Median of Two Sorted Arrays

**NeetCode Pattern:** Binary Search  
**Technique:** Binary Search on Partition  
**Difficulty:** Hard

## 1. Problem Understanding

You are given two sorted integer arrays:

```cpp
vector<int> nums1;
vector<int> nums2;
```

Find the median of the two sorted arrays.

The overall time complexity must be:

```text
O(log(m + n))
```

Where:
- `m` = length of `nums1`
- `n` = length of `nums2`

### What is a Median?

The median is the middle value of a sorted collection.

**Odd number of elements:**

```text
[1, 3, 5]

Median = 3
```

**Even number of elements:**

```text
[1, 3, 5, 7]

Median = (3 + 5) / 2 = 4
```

### Example 1

```text
Input:
nums1 = [1,3]
nums2 = [2]

Output: 2.0
```

Combined sorted array:

```text
[1, 2, 3]
```

Median = `2`.

### Example 2

```text
Input:
nums1 = [1,2]
nums2 = [3,4]

Output: 2.5
```

Combined sorted array:

```text
[1, 2, 3, 4]
```

Median:

```text
(2 + 3) / 2 = 2.5
```

---

## 2. Core Intuition: Binary Search on Partition

A straightforward solution would merge both arrays and find the middle element.

But merging takes O(m+n) time, which does not satisfy the required logarithmic complexity.

Instead, we divide both arrays into two parts.

Our goal is:

- All elements on the left side must be smaller than or equal to all elements on the right side.
- The left side must contain half of the total elements (or one extra element when the total is odd).

Visualize:

```text
nums1: [1, 3 | 8, 9]
nums2: [2, 4 | 5, 7]

Left partition:  [1, 3, 2, 4]
Right partition: [8, 9, 5, 7]
```

This partition is not correct because `4 > 5` is fine, but the left maximum is `4` and right minimum is `5`; actually it is valid.

For this example, the combined sorted order is:

```text
[1, 2, 3, 4 | 5, 7, 8, 9]
```

The left half contains the four smallest elements.

The median is:

```text
(4 + 5) / 2 = 4.5
```

**Important:** We don't physically merge the arrays. We only find the partition positions.

---

## 3. The Four Important Boundary Values

Suppose we partition the two arrays:

```text
nums1: [1, 3 | 8, 9]
nums2: [2, 4 | 5, 7]
```

We only need four values:

```text
nums1: [1, 3 | 8, 9]
          A   B

nums2: [2, 4 | 5, 7]
          C   D
```

| Variable | Meaning |
|---|---|
| `left1` | Largest element on the left side of `nums1` |
| `right1` | Smallest element on the right side of `nums1` |
| `left2` | Largest element on the left side of `nums2` |
| `right2` | Smallest element on the right side of `nums2` |

The partition is valid when:

```cpp
left1 <= right2 && left2 <= right1
```

In simple words:

- The left part of array 1 must not contain an element larger than the right part of array 2.
- The left part of array 2 must not contain an element larger than the right part of array 1.

---

## 4. Why Binary Search Only on the Smaller Array?

We can choose how many elements to take from the first array.

Let:

```cpp
int m = nums1.size();
int n = nums2.size();
```

We always make `nums1` the smaller array:

```cpp
if (m > n) {
    return findMedianSortedArrays(nums2, nums1);
}
```

Why?

1. Binary search runs on `nums1`.
2. Searching the smaller array minimizes the search range.
3. It also helps ensure the partition can be constructed safely.

This gives the required O(log(min(m,n))) complexity.

---

## 5. Calculate the Partition

The total number of elements is:

```cpp
int total = m + n;
```

The number of elements required on the left is:

```cpp
int half = (total + 1) / 2;
```

Why add 1?

For an odd total, the left side should contain one more element than the right side.

Examples:

| Total elements | Left partition | Right partition |
|---:|---:|---:|
| 5 | 3 | 2 |
| 6 | 3 | 3 |
| 7 | 4 | 3 |
| 8 | 4 | 4 |

Let `i` be the number of elements taken from `nums1`.

Then the number taken from `nums2` is:

```cpp
int j = half - i;
```

Now we have both partition positions.

---

## 6. Optimal C++ Solution

```cpp
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1,
                                  vector<int>& nums2) {

        int m = nums1.size();
        int n = nums2.size();

        // Always binary search on the smaller array
        if (m > n) {
            return findMedianSortedArrays(nums2, nums1);
        }

        int left = 0;
        int right = m;

        int half = (m + n + 1) / 2;

        while (left <= right) {

            int i = left + (right - left) / 2;
            int j = half - i;

            // Boundary values around the partitions
            int left1 = (i == 0) ? INT_MIN : nums1[i - 1];
            int right1 = (i == m) ? INT_MAX : nums1[i];

            int left2 = (j == 0) ? INT_MIN : nums2[j - 1];
            int right2 = (j == n) ? INT_MAX : nums2[j];

            // Correct partition found
            if (left1 <= right2 && left2 <= right1) {

                // Odd total number of elements
                if ((m + n) % 2 == 1) {
                    return max(left1, left2);
                }

                // Even total number of elements
                return (max(left1, left2) +
                        min(right1, right2)) / 2.0;
            }

            // Too many elements taken from nums1
            else if (left1 > right2) {
                right = i - 1;
            }

            // Too few elements taken from nums1
            else {
                left = i + 1;
            }
        }

        return 0.0;
    }
};
```

---

## 7. Understanding the Important Conditions

### Condition 1: Correct partition

```cpp
if (left1 <= right2 && left2 <= right1)
```

Both conditions must be true.

This guarantees that every element in the left partition is less than or equal to every element in the right partition.

### Condition 2: Too many elements from nums1

```cpp
else if (left1 > right2) {
    right = i - 1;
}
```

The left side of `nums1` contains an element that is too large compared with the right side of `nums2`.

We need to take fewer elements from `nums1`.

### Condition 3: Too few elements from nums1

```cpp
else {
    left = i + 1;
}
```

The partition needs more elements from `nums1`.

So we move the binary search to the right.

### Condition 4: Why use INT_MIN and INT_MAX?

```cpp
int left1 = (i == 0) ? INT_MIN : nums1[i - 1];
int right1 = (i == m) ? INT_MAX : nums1[i];
```

Sometimes a partition is at the very beginning or end of an array.

For example:

```text
nums1 = [1, 3]
```

If we take zero elements from `nums1`, there is no actual left-side element.

Using `INT_MIN` makes the boundary comparison work without accessing an invalid index.

Similarly, `INT_MAX` represents a missing right-side element.

These are boundary placeholders, not actual array values.

---

## 8. Dry Run

Input:

```text
nums1 = [1, 3]
nums2 = [2]
```

Since `nums1` is larger, swap the arrays:

```text
nums1 = [2]
nums2 = [1, 3]

m = 1
n = 2
```

Calculate:

```cpp
half = (1 + 2 + 1) / 2 = 2;
```

Initial search:

```text
left = 0
right = 1
```

### Iteration 1

```text
i = 0
j = 2
```

Boundary values:

```text
nums1 = [ | 2]
nums2 = [1, 3 | ]

left1  = INT_MIN
right1 = 2

left2  = 3
right2 = INT_MAX
```

Check:

```cpp
left1 <= right2  // true
left2 <= right1  // 3 <= 2 → false
```

The partition is invalid.

We have taken too few elements from `nums1`.

```cpp
left = i + 1; // 1
```

### Iteration 2

```text
i = 1
j = 1
```

Boundary values:

```text
nums1 = [2 | ]
nums2 = [1 | 3]

left1  = 2
right1 = INT_MAX

left2  = 1
right2 = 3
```

Check:

```cpp
2 <= 3  // true
1 <= INT_MAX  // true
```

Correct partition found!

Total elements = 3, which is odd.

Median:

```cpp
max(left1, left2)
= max(2, 1)
= 2
```

Output:

```text
2.0
```

---

## 9. Complexity Analysis

Let:

- `m` = length of `nums1`
- `n` = length of `nums2`

**Time Complexity: O(log(min(m,n)))**

We perform binary search only on the smaller array.

**Space Complexity: O(1)** auxiliary space.

We use only a fixed number of variables. The recursive swap call, when needed, adds only constant stack depth.

---

## 10. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Merging both arrays | Takes O(m+n), not the required logarithmic time |
| Binary searching the larger array | Can cause invalid partition positions |
| Forgetting `+1` in `(m+n+1)/2` | Makes odd-sized partition handling harder |
| Using the wrong partition condition | Can accept an invalid partition |
| Accessing `nums1[i]` when `i == m` | Causes out-of-bounds access |
| Accessing `nums1[i-1]` when `i == 0` | Causes out-of-bounds access |
| Dividing by `2` instead of `2.0` | Can cause integer division and lose the decimal part |
| Returning `left1` for an odd total | The median is the maximum of both left boundaries |

---

## 11. Edge Cases

| `nums1` | `nums2` | Expected |
|---|---|---:|
| `[]` | `[1]` | 1.0 |
| `[1]` | `[]` | 1.0 |
| `[1,3]` | `[2]` | 2.0 |
| `[1,2]` | `[3,4]` | 2.5 |
| `[0,0]` | `[0,0]` | 0.0 |
| `[2]` | `[1,3,4]` | 2.5 |

---

## 12. C++ Concepts to Remember

- `INT_MIN`: Smallest representable `int`.
- `INT_MAX`: Largest representable `int`.
- `max(a,b)`: Returns the larger value.
- `min(a,b)`: Returns the smaller value.
- Ternary operator: `condition ? value1 : value2`.
- `2.0`: Forces floating-point division.
- Recursive function call: Used here to ensure binary search operates on the smaller array.

---

## 13. Pattern Recognition for Interviews

Recognize **Binary Search on Partition** when:

- Two arrays are already sorted.
- You need a median or middle element.
- Merging is too expensive.
- You can divide both arrays into left and right partitions.
- You can check partition validity using boundary elements.

This is a more advanced form of binary search because we search for a valid partition rather than a target value.

---

## 14. 30-Second Revision

1. Make `nums1` the smaller array.
2. Calculate `half = (m+n+1)/2`.
3. Binary search the partition index `i` in `nums1`.
4. Calculate `j = half-i` for `nums2`.
5. Identify the four boundary values.
6. Check `left1 <= right2` and `left2 <= right1`.
7. If valid:
   - Odd total: return `max(left1,left2)`.
   - Even total: return `(max(left1,left2)+min(right1,right2))/2.0`.
8. If `left1 > right2`, move left; otherwise, move right.

**One-line interview explanation:**

"I use binary search on the smaller array to find a partition where the left-side elements of both arrays are smaller than or equal to the right-side elements. Once the partition is valid, the median can be calculated from the four boundary elements in O(log(min(m,n))) time."

---

## 15. Important Concept to Master

Don't memorize the entire implementation blindly.

Make sure you understand these three things:

1. Why do we divide the combined arrays into equal-sized left and right partitions?
2. Why do we need only four boundary values to validate the partition?
3. Why does comparing `left1` with `right2` tell us which direction to move?

If you can explain these three points, you understand the core of this hard problem.

---

**Next problem:** Search a 2D Matrix.
