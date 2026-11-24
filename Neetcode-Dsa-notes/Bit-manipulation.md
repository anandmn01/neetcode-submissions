# Single Number

- **Difficulty:** Easy
- **Problem:** LeetCode 136
- **Pattern:** Bit Manipulation
- **Data Structure:** Bitwise XOR
- **Language:** C++

## 1. Problem Understanding

We are given a non-empty integer array `nums`.

- Every element appears exactly twice except one element.
- We need to find and return the element that appears only once.

### Examples

**Example 1:**

Input:
```cpp
nums = {3, 2, 3}
```

Output:
```cpp
2
```

Explanation:
- `3` appears twice.
- `2` appears once.
- Therefore, the answer is `2`.

**Example 2:**

Input:
```cpp
nums = {7, 6, 6, 7, 8}
```

Output:
```cpp
8
```

Explanation:
- `7` appears twice.
- `6` appears twice.
- `8` appears once.

### Constraints

```text
1 <= nums.length <= 10000
-10000 <= nums[i] <= 10000
```

**Important requirement:**

- Time Complexity: O(n)
- Extra Space Complexity: O(1)

## 2. Core Concept — Bitwise XOR

The optimal solution uses the XOR (`^`) operator.

Before solving the problem, understand these important XOR properties.

### XOR Properties

**Property 1: Same numbers produce zero**

```cpp
a ^ a = 0
```

Example:

```cpp
5 ^ 5 = 0
```

**Property 2: XOR with zero gives the same number**

```cpp
a ^ 0 = a
```

Example:

```cpp
7 ^ 0 = 7
```

**Property 3: XOR is commutative**

The order of operands does not matter.

```cpp
a ^ b = b ^ a
```

Example:

```cpp
2 ^ 3 = 3 ^ 2
```

**Property 4: XOR is associative**

We can change the grouping of operands.

```cpp
(a ^ b) ^ c = a ^ (b ^ c)
```

### Most Important Observation

If every number appears twice except one, all duplicate numbers cancel each other when XOR is applied.

For example:

```text
3 ^ 2 ^ 3
```

Rearrange:

```text
3 ^ 3 ^ 2
```

Cancel duplicates:

```text
0 ^ 2
```

Result:

```text
2
```

Therefore, XOR gives us the unique number.

## 3. Approach 1 — Brute Force

### Idea

For every element, count how many times it appears in the array.

If an element appears exactly once, return it.

### C++ Code

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {

        for (int i = 0; i < nums.size(); i++) {

            int count = 0;

            for (int j = 0; j < nums.size(); j++) {

                if (nums[i] == nums[j]) {
                    count++;
                }
            }

            if (count == 1) {
                return nums[i];
            }
        }

        return -1;
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(n²)
- **Space Complexity:** O(1)

### Limitation

For every element, we traverse the entire array again.

This makes the solution inefficient for large arrays.

It also does not satisfy the required O(n) time complexity.

## 4. Approach 2 — Hash Map

### Idea

Use an unordered map to store the frequency of every element.

1. Traverse the array.
2. Increase the frequency of each element.
3. Traverse the frequency map.
4. Return the element whose frequency is 1.

### C++ Code

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {

        unordered_map<int, int> freq;

        for (int num : nums) {
            freq[num]++;
        }

        for (auto& it : freq) {

            if (it.second == 1) {
                return it.first;
            }
        }

        return -1;
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(n) average
- **Space Complexity:** O(n)

### Limitation

The hash map requires additional memory.

The problem specifically asks us to use O(1) extra space.

Therefore, we need a better solution.

## 5. Approach 3 — Optimized Solution Using XOR

<mark>Best Approach</mark>

### Idea

Initialize a variable `result` with zero.

Traverse the entire array and XOR every element with `result`.

```cpp
result ^= num;
```

Since:

```cpp
a ^ a = 0
```

All duplicate elements cancel each other.

Since:

```cpp
a ^ 0 = a
```

The unique element remains.

### C++ Code

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {

        int result = 0;

        for (int num : nums) {

            result ^= num;
        }

        return result;
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(n)
- **Space Complexity:** O(1)

This satisfies both requirements of the problem.

## 6. Detailed Dry Run

**Input:**

```cpp
nums = {3, 2, 3}
```

Initialize:

```cpp
result = 0;
```

### Iteration 1

Current element:

```text
num = 3
```

Operation:

```cpp
result = 0 ^ 3;
```

Result:

```text
result = 3
```

### Iteration 2

Current element:

```text
num = 2
```

Operation:

```cpp
result = 3 ^ 2;
```

Result:

```text
result = 1
```

### Iteration 3

Current element:

```text
num = 3
```

Operation:

```cpp
result = 1 ^ 3;
```

Result:

```text
result = 2
```

### Dry Run Table

| Iteration | Current Number | Previous Result | New Result |
|---|---:|---:|---:|
| 1 | 3 | 0 | 3 |
| 2 | 2 | 3 | 1 |
| 3 | 3 | 1 | 2 |

Final result:

```cpp
return 2;
```

**Output:**

```text
2
```

### Another Way to Understand the Cancellation

```text
3 ^ 2 ^ 3

= 3 ^ 3 ^ 2

= 0 ^ 2

= 2
```

The duplicate `3` disappears, leaving only `2`.

## 7. Time and Space Complexity Comparison

| Approach | Time Complexity | Extra Space |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Hash Map | O(n) average | O(n) |
| XOR | O(n) | O(1) |

**Optimal choice:** XOR.

## 8. Important Interview Insights

### Q1. Why does XOR work in this problem?

Because XOR of two identical numbers is zero.

Every duplicate cancels out, leaving the unique element.

### Q2. Why do we initialize the result with zero?

Because zero is the identity element of XOR.

```cpp
a ^ 0 = a
```

Therefore, initializing with zero does not affect the final answer.

### Q3. Does XOR depend on the order of elements?

No.

XOR is both commutative and associative.

Therefore, we can rearrange the elements without changing the final result.

### Q4. Can XOR handle negative integers?

Yes.

C++ supports bitwise XOR on signed integer types, and the cancellation properties still hold for equal integer values.

### Q5. Why is the space complexity O(1)?

We only maintain one additional variable:

```cpp
int result = 0;
```

We do not create another array, map, or data structure.

### Q6. Can sorting solve this problem?

Yes.

After sorting, duplicate elements become adjacent, making it possible to identify the unique element.

However:

- Time Complexity: O(n log n)
- Auxiliary Space: Depends on the sorting implementation.

It is not as efficient as XOR.

## 9. Common Mistakes

1. Forgetting to initialize the XOR variable with zero.
2. Using addition or subtraction instead of XOR without a valid mathematical approach.
3. Using a hash map when the problem explicitly requires O(1) extra space.
4. Assuming XOR works for every frequency-based problem.
5. Confusing XOR (`^`) with exponentiation.

**Important:**

```cpp
^   // Bitwise XOR
```

It is not the power operator in C++.

## 10. Pattern Recognition — When Should You Use XOR?

Look for XOR when a problem contains:

- Every element appears twice except one.
- Duplicate values need to cancel each other.
- The problem requires O(n) time and O(1) extra space.
- You need to identify a unique number using bit manipulation.

### Related Problems

| Problem | LeetCode | Main Concept |
|---|---:|---|
| Single Number | 136 | XOR |
| Single Number II | 137 | Bit Manipulation |
| Single Number III | 260 | XOR + Bit Manipulation |
| Missing Number | 268 | XOR / Mathematics |
| Find the Difference | 389 | XOR |

## 11. Key Takeaways

Remember these four XOR properties:

```cpp
a ^ a = 0
a ^ 0 = a
a ^ b = b ^ a
(a ^ b) ^ c = a ^ (b ^ c)
```

### Final Optimized Code

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {

        int result = 0;

        for (int num : nums) {
            result ^= num;
        }

        return result;
    }
};
```

### Final Complexity

- **Time Complexity:** O(n)
- **Space Complexity:** O(1)

**Interview takeaway:**

Whenever every element appears twice except one and the problem requires constant extra space, immediately consider XOR.

The central idea is simple:
**Duplicate numbers cancel each other, and the unique number survives.**
# Number of 1 Bits

- **Difficulty:** Easy
- **Problem:** LeetCode 191
- **Pattern:** Bit Manipulation
- **Language:** C++
- **Key Technique:** Brian Kernighan's Algorithm

## 1. Problem Understanding

We are given an unsigned 32-bit integer `n`.

We need to count how many `1` bits are present in its binary representation.

This count is also known as the **Hamming Weight**.

### Example 1

**Input:**
```cpp
n = 23
```

Binary representation:

```text
23 = 10111
```

Number of set bits:

```text
1 0 1 1 1
^   ^ ^ ^
```

**Output:**
```text
4
```

### Example 2

**Input:**
```cpp
n = 2147483645
```

Binary representation contains 30 set bits.

**Output:**
```text
30
```

### Constraints

- `n` is a non-negative integer.
- `n` fits within 32 bits.
- The function accepts `uint32_t`.

## 2. Core Concepts — Bit Manipulation

Before solving this problem, understand these operators.

### 1. Bitwise AND (&)

Returns `1` only when both corresponding bits are `1`.

```text
1 & 1 = 1
1 & 0 = 0
0 & 1 = 0
0 & 0 = 0
```

Important observation:

```cpp
n & 1
```

Checks whether the last bit of `n` is set.

### 2. Right Shift (>>)

Moves all bits one position to the right.

```cpp
n = n >> 1;
```

Example:

```text
10110 >> 1 = 01011
```

This allows us to examine the binary representation one bit at a time.

### 3. Bitwise AND with n - 1

An important property:

```cpp
n & (n - 1)
```

Removes the rightmost set bit (rightmost `1`) from `n`.

Example:

```text
n     = 10110
n - 1 = 10101

AND   = 10100
```

The rightmost `1` has been removed.

This property is the foundation of the optimized solution.

## 3. Approach 1 — Brute Force Using Right Shift

### Idea

Check every bit of the integer.

1. Initialize `count = 0`.
2. Check the last bit using `n & 1`.
3. If it is `1`, increment the count.
4. Right-shift `n` by one position.
5. Repeat until all 32 bits have been processed.

### C++ Code

```cpp
class Solution {
public:
    int hammingWeight(uint32_t n) {

        int count = 0;

        for (int i = 0; i < 32; i++) {

            if (n & 1) {
                count++;
            }

            n = n >> 1;
        }

        return count;
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(1), because exactly 32 bits are processed.
- **Space Complexity:** O(1).

### Alternative Loop

We can also use:

```cpp
while (n > 0) {

    if (n & 1) {
        count++;
    }

    n >>= 1;
}
```

This processes only the bits needed to represent the number.

However, the fixed 32-iteration version makes the 32-bit processing explicit.

## 4. Approach 2 — Optimized Using Brian Kernighan's Algorithm

<mark>Recommended Approach</mark>

### Idea

Instead of checking every bit, remove one set bit during every iteration.

The key operation is:

```cpp
n = n & (n - 1);
```

This removes the rightmost set bit.

Therefore, the number of iterations equals the number of set bits.

### C++ Code

```cpp
class Solution {
public:
    int hammingWeight(uint32_t n) {

        int count = 0;

        while (n != 0) {

            n = n & (n - 1);

            count++;
        }

        return count;
    }
};
```

### Complexity Analysis

Let `k` represent the number of set bits.

- **Time Complexity:** O(k), where k is the number of set bits. Worst case O(32), which is O(1) for a fixed-width integer.
- **Space Complexity:** O(1).

### Why Is This Better?

The first approach checks all 32 bits.

Brian Kernighan's algorithm processes only the set bits.

For example:

```text
n = 10000000
```

There is only one set bit.

- Brute force: checks 32 positions.
- Brian Kernighan: performs only 1 iteration.

## 5. Detailed Dry Run — Brian Kernighan's Algorithm

**Input:**

```cpp
n = 23
```

Binary representation:

```text
23 = 10111
```

Initialize:

```cpp
count = 0;
```

### Iteration 1

Current number:

```text
n     = 10111
n - 1 = 10110
```

Apply AND:

```text
10111
10110
-----
10110
```

One set bit is removed.

```text
n = 10110
count = 1
```

### Iteration 2

```text
n     = 10110
n - 1 = 10101
```

Apply AND:

```text
10110
10101
-----
10100
```

```text
n = 10100
count = 2
```

### Iteration 3

```text
n     = 10100
n - 1 = 10011
```

Apply AND:

```text
10100
10011
-----
10000
```

```text
n = 10000
count = 3
```

### Iteration 4

```text
n     = 10000
n - 1 = 01111
```

Apply AND:

```text
10000
01111
-----
00000
```

```text
n = 00000
count = 4
```

Now `n == 0`, so the loop terminates.

**Final Output:**

```text
4
```

### Dry Run Table

| Iteration | Previous n | n - 1 | New n | Count |
|---|---|---|---|---|
| 1 | 10111 | 10110 | 10110 | 1 |
| 2 | 10110 | 10101 | 10100 | 2 |
| 3 | 10100 | 10011 | 10000 | 3 |
| 4 | 10000 | 01111 | 00000 | 4 |

## 6. Approach 3 — Using Built-in Function

C++ provides a built-in function to count set bits.

For an unsigned integer:

```cpp
__builtin_popcount(n)
```

### C++ Code

```cpp
class Solution {
public:
    int hammingWeight(uint32_t n) {

        return __builtin_popcount(n);
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(1) for a fixed 32-bit integer.
- **Space Complexity:** O(1).

**Important:**

- `__builtin_popcount()` counts set bits in an unsigned integer.
- `__builtin_popcountll()` is used for `unsigned long long`.

For interviews, understand and implement Brian Kernighan's algorithm rather than relying only on a built-in function.

## 7. Approach Comparison

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Check every bit | O(1) for 32 bits | O(1) |
| Brian Kernighan | O(k), k = set bits | O(1) |
| Built-in popcount | O(1) for 32 bits | O(1) |

**Recommended:** Brian Kernighan's Algorithm.

It demonstrates an important bit manipulation technique and avoids unnecessary bit checks.

## 8. Important Interview Insights

### Q1. What is a set bit?

A set bit is a bit whose value is `1` in binary representation.

Example:

```text
10110
```

There are three set bits.

### Q2. What does `n & (n - 1)` do?

It removes the rightmost set bit from the binary representation of `n`.

Example:

```text
n     = 1100
n - 1 = 1011

n & (n - 1) = 1000
```

### Q3. Why does Brian Kernighan's algorithm work?

Every iteration removes exactly one set bit.

When all set bits are removed, `n` becomes zero.

Therefore, the number of iterations equals the number of set bits.

### Q4. What is Hamming Weight?

Hamming Weight is the total number of set bits (`1`s) in the binary representation of a number.

### Q5. What is the difference between `n & 1` and `n & (n - 1)`?

| Expression | Purpose |
|---|---|
| `n & 1` | Checks whether the last bit is set |
| `n & (n - 1)` | Removes the rightmost set bit |

### Q6. What happens when n is zero?

No bits are set.

```cpp
n = 0;
```

The loop:

```cpp
while (n != 0)
```

does not execute.

The answer is `0`.

## 9. Common Mistakes

1. Confusing bitwise AND (`&`) with logical AND (`&&`).
2. Forgetting that `n & (n - 1)` removes one set bit.
3. Using `n >> 1` without checking the last bit first.
4. Using an incorrect integer type for the 32-bit input.
5. Assuming every bit manipulation problem requires checking all 32 positions.
6. Confusing the number of bits in an integer with the number of set bits.

## 10. Pattern Recognition — When Should You Use This Technique?

Look for Brian Kernighan's algorithm when:

- You need to count the number of set bits.
- You need to remove the rightmost set bit.
- You need to determine whether a number is a power of two.
- You need to count the number of operations required to clear all set bits.

### Related Problems

| Problem | LeetCode | Main Concept |
|---|---:|---|
| Number of 1 Bits | 191 | Brian Kernighan's Algorithm |
| Counting Bits | 338 | Dynamic Programming + Bit Manipulation |
| Reverse Bits | 190 | Bit Manipulation |
| Power of Two | 231 | `n & (n - 1)` |
| Single Number | 136 | XOR |
| Missing Number | 268 | XOR |

## 11. Key Takeaways

Remember these important expressions:

```cpp
n & 1
```

Checks whether the last bit is `1`.

```cpp
n >> 1
```

Shifts bits one position to the right.

```cpp
n & (n - 1)
```

Removes the rightmost set bit.

### Final Optimized Code

```cpp
class Solution {
public:
    int hammingWeight(uint32_t n) {

        int count = 0;

        while (n != 0) {

            n &= (n - 1);

            count++;
        }

        return count;
    }
};
```

### Final Complexity

- **Time Complexity:** O(k), where k is the number of set bits; worst case O(32) for a 32-bit integer.
- **Space Complexity:** O(1).

**Interview takeaway:**

Whenever you see a problem involving counting set bits or repeatedly removing the rightmost `1`, think about:

```cpp
n & (n - 1)
```

This single expression is one of the most useful tricks in bit manipulation.

# Counting Bits

- **Difficulty:** Easy
- **Problem:** LeetCode 338
- **Pattern:** Dynamic Programming + Bit Manipulation
- **Language:** C++
- **Key Technique:** `dp[i] = dp[i / 2] + (i % 2)`

## 1. Problem Understanding

We are given an integer `n`.

We need to calculate the number of set bits (`1`s) in the binary representation of every number from `0` to `n`.

Return an array `ans` where:

```cpp
ans[i] = number of set bits in i
```

### Example

**Input:**

```cpp
n = 4
```

Binary representations:

| Number | Binary | Number of 1s |
|---|---|---|
| 0 | 000 | 0 |
| 1 | 001 | 1 |
| 2 | 010 | 1 |
| 3 | 011 | 2 |
| 4 | 100 | 1 |

**Output:**

```cpp
[0, 1, 1, 2, 1]
```

### Constraints

```text
0 <= n <= 1000
```

## 2. Core Concept — Dynamic Programming

The important observation is that we do not need to count the set bits of every number independently.

We can use previously calculated answers.

Consider:

```text
Number 2 = 10
Number 3 = 11
Number 4 = 100
Number 5 = 101
Number 6 = 110
Number 7 = 111
```

Notice that:

- `2` is obtained by removing the last bit of `4`? No — instead, its parent after right shift is `1`.
- `3 >> 1 = 1`
- `4 >> 1 = 2`
- `5 >> 1 = 2`
- `6 >> 1 = 3`
- `7 >> 1 = 3`

When we right-shift a number by one position, its last bit is removed.

Therefore:

```cpp
dp[i] = dp[i >> 1] + (i & 1);
```

Here:

- `i >> 1` removes the last bit.
- `i & 1` checks whether the last bit was `1`.
- `dp[i >> 1]` already contains the count of set bits in the remaining bits.

This gives us our DP recurrence.

## 3. Approach 1 — Brute Force Using Bit Manipulation

### Idea

For every number from `0` to `n`:

1. Count its set bits.
2. Store the result in the answer array.
3. Repeat for the next number.

We can use Brian Kernighan's algorithm from LeetCode 191.

### C++ Code

```cpp
class Solution {
public:
    vector<int> countBits(int n) {

        vector<int> ans(n + 1, 0);

        for (int i = 0; i <= n; i++) {

            int num = i;
            int count = 0;

            while (num != 0) {

                num &= (num - 1);

                count++;
            }

            ans[i] = count;
        }

        return ans;
    }
};
```

### Complexity Analysis

Let `B` be the maximum number of bits needed to represent a number up to `n`.

- **Time Complexity:** O(n * B), because we count the set bits of each number independently.
- **Space Complexity:** O(n) for the answer array.

For a fixed-width integer, B is bounded.

### Limitation

We repeatedly calculate the number of set bits from scratch.

We can optimize this using Dynamic Programming.

## 4. Approach 2 — Optimized Dynamic Programming (Recommended)

### Idea

Use the recurrence:

```cpp
dp[i] = dp[i / 2] + (i % 2);
```

Equivalent bitwise expression:

```cpp
dp[i] = dp[i >> 1] + (i & 1);
```

Why does it work?

Dividing by 2 removes the last binary bit.

The remainder tells us whether that removed bit was `1` or `0`.

### C++ Code

```cpp
class Solution {
public:
    vector<int> countBits(int n) {

        vector<int> dp(n + 1, 0);

        for (int i = 1; i <= n; i++) {

            dp[i] = dp[i >> 1] + (i & 1);

        }

        return dp;
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(n), because we calculate each answer exactly once.
- **Space Complexity:** O(n), for the DP array.

This is the recommended solution.

## 5. Detailed Dry Run

**Input:**

```cpp
n = 5
```

Initialize:

```cpp
vector<int> dp(n + 1, 0);
```

Initially:

```text
dp = [0, 0, 0, 0, 0, 0]
```

Our recurrence:

```cpp
dp[i] = dp[i >> 1] + (i & 1);
```

### Iteration 1: i = 1

Binary:

```text
1 = 001
```

Calculation:

```cpp
dp[1] = dp[1 >> 1] + (1 & 1);

dp[1] = dp[0] + 1;

dp[1] = 0 + 1 = 1;
```

Array:

```text
[0, 1, 0, 0, 0, 0]
```

### Iteration 2: i = 2

Binary:

```text
2 = 010
```

Calculation:

```cpp
dp[2] = dp[2 >> 1] + (2 & 1);

dp[2] = dp[1] + 0;

dp[2] = 1;
```

Array:

```text
[0, 1, 1, 0, 0, 0]
```

### Iteration 3: i = 3

Binary:

```text
3 = 011
```

Calculation:

```cpp
dp[3] = dp[3 >> 1] + (3 & 1);

dp[3] = dp[1] + 1;

dp[3] = 1 + 1 = 2;
```

Array:

```text
[0, 1, 1, 2, 0, 0]
```

### Iteration 4: i = 4

Binary:

```text
4 = 100
```

Calculation:

```cpp
dp[4] = dp[4 >> 1] + (4 & 1);

dp[4] = dp[2] + 0;

dp[4] = 1;
```

Array:

```text
[0, 1, 1, 2, 1, 0]
```

### Iteration 5: i = 5

Binary:

```text
5 = 101
```

Calculation:

```cpp
dp[5] = dp[5 >> 1] + (5 & 1);

dp[5] = dp[2] + 1;

dp[5] = 1 + 1 = 2;
```

Final array:

```text
[0, 1, 1, 2, 1, 2]
```

### Dry Run Table

| i | Binary | i >> 1 | i & 1 | dp[i] |
|---|---|---|---|---|
| 0 | 000 | - | - | 0 |
| 1 | 001 | 0 | 1 | 1 |
| 2 | 010 | 1 | 0 | 1 |
| 3 | 011 | 1 | 1 | 2 |
| 4 | 100 | 2 | 0 | 1 |
| 5 | 101 | 2 | 1 | 2 |

## 6. Approach 3 — DP Using the Last Set Bit

Another useful recurrence is:

```cpp
dp[i] = dp[i & (i - 1)] + 1;
```

### Idea

From LeetCode 191, we know:

```cpp
i & (i - 1)
```

removes the rightmost set bit.

Therefore:

- Remove one set bit.
- The remaining number has an already calculated DP answer.
- Add one for the removed bit.

### C++ Code

```cpp
class Solution {
public:
    vector<int> countBits(int n) {

        vector<int> dp(n + 1, 0);

        for (int i = 1; i <= n; i++) {

            dp[i] = dp[i & (i - 1)] + 1;

        }

        return dp;
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(n).
- **Space Complexity:** O(n).

This is another valid DP solution.

## 7. Approach Comparison

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Brute Force + Brian Kernighan | O(n * B) | O(n) |
| DP using right shift | O(n) | O(n) |
| DP using last set bit | O(n) | O(n) |

**Recommended:** DP using right shift.

It is simple, efficient, and demonstrates the relationship between binary representation and Dynamic Programming.

## 8. Important Interview Insights

### Q1. Why do we use `n >> 1`?

Right shift removes the last binary bit.

Example:

```text
1101 >> 1 = 0110
```

### Q2. What does `n & 1` do?

It checks whether the last bit is set.

```text
1101 & 0001 = 0001
```

The result is `1`.

### Q3. Why is the recurrence `dp[i] = dp[i >> 1] + (i & 1)` correct?

Because:

- `i >> 1` contains all bits except the last bit.
- `dp[i >> 1]` gives the number of set bits in those remaining positions.
- `i & 1` contributes either zero or one.

### Q4. What is the difference between the two DP recurrences?

**Right-shift recurrence:**

```cpp
dp[i] = dp[i >> 1] + (i & 1);
```

Removes the last bit.

**Brian Kernighan recurrence:**

```cpp
dp[i] = dp[i & (i - 1)] + 1;
```

Removes the rightmost set bit.

Both calculate the number of set bits.

### Q5. Why is DP better than brute force?

DP reuses previously calculated answers instead of recounting every bit independently.

## 9. Common Mistakes

1. Initializing the DP array with size `n` instead of `n + 1`.
2. Forgetting that the answer must include the count for zero.
3. Starting the loop from zero unnecessarily when `dp[0]` is already initialized.
4. Confusing `i / 2` with `i >> 1` without understanding their binary relationship.
5. Forgetting that `i & 1` returns either zero or one.
6. Using an incorrect recurrence such as `dp[i] = dp[i - 1] + 1`.

## 10. Pattern Recognition

Use these observations when solving bit manipulation and DP problems.

| Observation | Technique |
|---|---|
| Need to count set bits | Brian Kernighan's Algorithm |
| Need answers for every number from 0 to n | Dynamic Programming |
| Remove the last binary bit | `n >> 1` |
| Check the last binary bit | `n & 1` |
| Remove the rightmost set bit | `n & (n - 1)` |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Number of 1 Bits | 191 | Brian Kernighan |
| Counting Bits | 338 | DP + Bit Manipulation |
| Reverse Bits | 190 | Bit Manipulation |
| Power of Two | 231 | Bit Manipulation |
| Single Number | 136 | XOR |

## 11. Key Takeaways

Remember these two important recurrences:

**Method 1 — Remove the last bit:**

```cpp
dp[i] = dp[i >> 1] + (i & 1);
```

**Method 2 — Remove the rightmost set bit:**

```cpp
dp[i] = dp[i & (i - 1)] + 1;
```

Both approaches have:

- Time Complexity: O(n)
- Space Complexity: O(n)

### Final Recommended Code

```cpp
class Solution {
public:
    vector<int> countBits(int n) {

        vector<int> dp(n + 1, 0);

        for (int i = 1; i <= n; i++) {

            dp[i] = dp[i >> 1] + (i & 1);

        }

        return dp;
    }
};
```

**Interview takeaway:**

This problem teaches an important DP principle:

Instead of solving every subproblem independently, use the answer to a smaller subproblem to calculate the current answer.

Here, the smaller subproblem is `dp[i >> 1]`.

# Reverse Bits

- **Difficulty:** Easy
- **Problem:** LeetCode 190
- **Pattern:** Bit Manipulation
- **Language:** C++
- **Key Technique:** Bit Extraction + Left Shift

## 1. Problem Understanding

We are given a 32-bit unsigned integer `n`.

We need to reverse all 32 bits of its binary representation and return the resulting unsigned integer.

**Important:** We must reverse exactly 32 bits, including leading zeros.

### Example

**Input:**

```cpp
n = 00000000000000000000000000010101
```

Binary representation:

```text
00000000000000000000000000010101
```

After reversing all 32 bits:

```text
10101000000000000000000000000000
```

**Output:**

```text
2818572288
```

### Key Observation

We need to:
1. Extract the last bit of the original number.
2. Add that bit to the result.
3. Shift the original number right to process its next bit.
4. Shift the result left to make room for the next bit.
5. Repeat exactly 32 times.

## 2. Approach 1 — Brute Force Using Binary String

### Idea

1. Convert the integer into a 32-bit binary string.
2. Reverse the string.
3. Convert the reversed binary string back into an unsigned integer.

This approach helps understand the problem but is not the preferred interview solution.

### C++ Code

```cpp
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {

        string binary = "";

        for (int i = 0; i < 32; i++) {

            binary += (n & 1) ? '1' : '0';

            n >>= 1;
        }

        uint32_t result = 0;

        for (int i = 0; i < 32; i++) {

            result = (result << 1) | (binary[i] - '0');
        }

        return result;
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(32) = O(1).
- **Space Complexity:** O(32) = O(1), because the string stores 32 bits.

## 3. Approach 2 — Optimized Bit Manipulation (Recommended)

### Core Idea

We can reverse the bits without using a string.

Use two variables:

```cpp
uint32_t result = 0;
```

The original number contains the bits we need to extract.

The result stores those bits in reverse order.

### Important Operations

**1. Extract the last bit**

```cpp
n & 1
```

This extracts the rightmost bit.

Example:

```text
n = 10101

n & 1 = 1
```

**2. Shift the original number right**

```cpp
n >>= 1;
```

Removes the last bit so that we can process the next bit.

**3. Shift the result left**

```cpp
result <<= 1;
```

Creates an empty position on the right side of the result.

**4. Add the extracted bit**

```cpp
result |= (n & 1);
```

Places the extracted bit in the rightmost position.

### C++ Code

```cpp
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {

        uint32_t result = 0;

        for (int i = 0; i < 32; i++) {

            result <<= 1;

            result |= (n & 1);

            n >>= 1;
        }

        return result;
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(32) = O(1), because we always process exactly 32 bits.
- **Space Complexity:** O(1), because we use only a constant number of variables.

This is the recommended solution.

## 4. Detailed Dry Run

**Input:**

```text
n = 00000000000000000000000000010101
```

For understanding, consider only the five meaningful bits:

```text
Original bits: 10101
```

We extract bits from right to left:

```text
1 → 0 → 1 → 0 → 1
```

The result is built from left to right:

```text
1 → 10 → 101 → 1010 → 10101
```

The actual algorithm executes 32 iterations, so the remaining 27 bits are also processed as zeros.

### Step-by-step Example

Initially:

```cpp
result = 0;
n = 21;
```

| Iteration | Extracted bit | Result after shift and OR |
|---|---|---|
| 1 | 1 | 1 |
| 2 | 0 | 10 |
| 3 | 1 | 101 |
| 4 | 0 | 1010 |
| 5 | 1 | 10101 |

After processing the remaining 27 zero bits, the final result is:

```text
10101000000000000000000000000000
```

In decimal:

```text
2818572288
```

**Output:**

```cpp
2818572288
```

## 5. Understanding the Algorithm Visually

Original number:

```text
Bit positions:

31                         0
|                          |
00000000000000000000000000010101
```

We extract bits starting from position 0:

```text
Position 0 → 1
Position 1 → 0
Position 2 → 1
Position 3 → 0
Position 4 → 1
```

These bits are placed into the result starting from its most significant side.

```text
Original: 00000000000000000000000000010101

Reversed: 10101000000000000000000000000000
```

This is why we use:
- Right shift on the original number.
- Left shift on the result.

## 6. Alternative Approach — Using Bit Positions

Another way is to explicitly check each bit position and place it at its reversed position.

### C++ Code

```cpp
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {

        uint32_t result = 0;

        for (int i = 0; i < 32; i++) {

            if (n & (1u << i)) {

                result |= (1u << (31 - i));
            }
        }

        return result;
    }
};
```

### Idea

If bit position `i` is set in the original number, set bit position `31 - i` in the result.

For example:

```text
Original bit position: 0
New bit position:      31
```

```text
Original bit position: 1
New bit position:      30
```

```text
Original bit position: 31
New bit position:      0
```

### Complexity Analysis

- **Time Complexity:** O(32) = O(1).
- **Space Complexity:** O(1).

## 7. Approach Comparison

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Binary String | O(32) | O(32) |
| Bit Extraction + Shifting | O(32) | O(1) |
| Bit Position Mapping | O(32) | O(1) |

**Recommended:** Bit Extraction + Shifting.

It is concise, efficient, and demonstrates a strong understanding of bit manipulation.

## 8. Important Interview Insights

### Q1. Why do we run the loop exactly 32 times?

Because the problem asks us to reverse all 32 bits.

Even if the number has only a few set bits, leading zeros must also be considered.

### Q2. Why do we use `n & 1`?

It extracts the rightmost bit.

```cpp
n & 1
```

Possible results:

```text
0 & 1 = 0
1 & 1 = 1
```

### Q3. Why do we shift the result left?

```cpp
result <<= 1;
```

It moves existing bits one position to the left, creating space for the next extracted bit.

### Q4. Why do we shift the original number right?

```cpp
n >>= 1;
```

It removes the bit that we have already processed.

### Q5. Why is `uint32_t` used?

The problem specifies a 32-bit unsigned integer.

Using `uint32_t` ensures that the value is treated as an unsigned 32-bit integer.

### Q6. Why must we process leading zeros?

Because reversing only the meaningful bits would produce an incorrect result.

For example:

```text
00000000000000000000000000010101
```

Must become:

```text
10101000000000000000000000000000
```

Not simply:

```text
10101
```

## 9. Common Mistakes

1. Running the loop only until `n` becomes zero.
   - This ignores the remaining leading zeros.
   - Always execute exactly 32 iterations.

2. Forgetting to shift the result before inserting the next bit.

3. Using `n >> 1` without assigning the result back to `n`.

4. Confusing left shift and right shift.
   - Original number: right shift.
   - Result: left shift.

5. Using signed integer types carelessly.
   - Prefer `uint32_t` for this problem.

6. Reversing only the set bits instead of all 32 bit positions.

## 10. Pattern Recognition

| Operation | Purpose |
|---|---|
| `n & 1` | Extract rightmost bit |
| `n >> 1` | Remove rightmost bit |
| `result << 1` | Create space for next bit |
| `result \| bit` | Insert extracted bit |
| `n & (n - 1)` | Remove rightmost set bit |
| `1u << i` | Create a bit mask at position i |

## 11. Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Number of 1 Bits | 191 | Bit Counting |
| Counting Bits | 338 | DP + Bit Manipulation |
| Reverse Bits | 190 | Bit Reversal |
| Single Number | 136 | XOR |
| Power of Two | 231 | Bit Manipulation |

## 12. Key Takeaways

Remember the following algorithm:

```cpp
uint32_t result = 0;

for (int i = 0; i < 32; i++) {

    result <<= 1;

    result |= (n & 1);

    n >>= 1;
}
```

### Final Recommended Code

```cpp
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {

        uint32_t result = 0;

        for (int i = 0; i < 32; i++) {

            result = (result << 1) | (n & 1);

            n >>= 1;
        }

        return result;
    }
};
```

**Final Complexity:**
- Time: O(1), exactly 32 iterations.
- Space: O(1).

**Interview takeaway:**

Bit reversal is about extracting bits from one direction and rebuilding the number in the opposite direction.

Remember:

- Extract from right to left.
- Build from left to right.
- Process all 32 bits.

  # Missing Number

- **Difficulty:** Easy
- **Problem:** LeetCode 268
- **Pattern:** Array + Math + Bit Manipulation
- **Language:** C++
- **Key Techniques:** Expected Sum, XOR

## 1. Problem Understanding

We are given an array `nums` containing `n` distinct integers.

The integers belong to the range:

```text
[0, n]
```

Exactly one number from this range is missing.

We need to find and return that missing number.

### Example 1

**Input:**

```cpp
nums = [1, 2, 3]
```

Array size:

```text
n = 3
```

Expected range:

```text
[0, 1, 2, 3]
```

Present numbers:

```text
1, 2, 3
```

Missing number:

```text
0
```

**Output:**

```cpp
0
```

### Example 2

**Input:**

```cpp
nums = [0, 2]
```

Expected range:

```text
[0, 1, 2]
```

Present numbers:

```text
0, 2
```

Missing number:

```text
1
```

**Output:**

```cpp
1
```

### Important Observations

1. All numbers are distinct.
2. The range always starts from `0`.
3. The range ends at `n`, where `n` is the array size.
4. Exactly one number is missing.

---

## 2. Approach 1 — Brute Force (Linear Search)

### Idea

Check every number from `0` to `n`.

For each number, search whether it exists in the array.

If a number is not present, return it.

### C++ Code

```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {

        int n = nums.size();

        for (int i = 0; i <= n; i++) {

            bool found = false;

            for (int j = 0; j < n; j++) {

                if (nums[j] == i) {

                    found = true;
                    break;
                }
            }

            if (!found) {
                return i;
            }
        }

        return -1;
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(n²), because for each number we potentially search the entire array.
- **Space Complexity:** O(1), because we use only a few variables.

### Limitation

We repeatedly search the array.

We can improve this using sorting, hashing, mathematics, or XOR.

---

## 3. Approach 2 — Sorting

### Idea

1. Sort the array.
2. Traverse it using its index.
3. If `nums[i] != i`, then `i` is the missing number.
4. If every element matches its index, the missing number is `n`.

### C++ Code

```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {

        sort(nums.begin(), nums.end());

        int n = nums.size();

        for (int i = 0; i < n; i++) {

            if (nums[i] != i) {
                return i;
            }
        }

        return n;
    }
};
```

### Dry Run

Input:

```cpp
nums = [3, 0, 1]
```

After sorting:

```text
[0, 1, 3]
```

| Index | nums[index] | Comparison | Result |
|---|---|---|---|
| 0 | 0 | 0 == 0 | Continue |
| 1 | 1 | 1 == 1 | Continue |
| 2 | 3 | 3 != 2 | Missing number = 2 |

Output:

```cpp
2
```

### Complexity Analysis

- **Time Complexity:** O(n log n) for sorting, followed by O(n) traversal.
- **Space Complexity:** Depends on the sorting implementation; the auxiliary space is generally O(log n) for `std::sort` due to recursion.

---

## 4. Approach 3 — Hash Set

### Idea

Store all array elements in an unordered set.

Then check every number from `0` to `n`.

The first number that is not present in the set is the missing number.

### C++ Code

```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {

        unordered_set<int> st(
            nums.begin(),
            nums.end()
        );

        int n = nums.size();

        for (int i = 0; i <= n; i++) {

            if (st.find(i) == st.end()) {
                return i;
            }
        }

        return -1;
    }
};
```

### Complexity Analysis

- **Time Complexity:** O(n) average.
- **Space Complexity:** O(n), for the unordered set.

### Limitation

Although efficient, this approach uses extra memory.

The follow-up asks whether we can solve it using O(1) extra space.

---

## 5. Approach 4 — Mathematical Sum (Optimized)

### Core Idea

We know the sum of the first `n` natural numbers starting from zero:

```text
Sum = n * (n + 1) / 2
```

The actual array contains all numbers from `0` to `n`, except one.

Therefore:

```text
Missing Number = Expected Sum - Actual Sum
```

### Formula

```cpp
expectedSum = n * (n + 1) / 2;

actualSum = sum of all elements in nums;

missingNumber = expectedSum - actualSum;
```

### C++ Code

```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {

        int n = nums.size();

        int expectedSum = n * (n + 1) / 2;

        int actualSum = 0;

        for (int num : nums) {

            actualSum += num;
        }

        return expectedSum - actualSum;
    }
};
```

### Detailed Dry Run

Input:

```cpp
nums = [3, 0, 1]
```

Array size:

```text
n = 3
```

**Step 1: Calculate expected sum**

```text
Expected Sum = n * (n + 1) / 2

             = 3 * 4 / 2

             = 6
```

The complete range is:

```text
0 + 1 + 2 + 3 = 6
```

**Step 2: Calculate actual sum**

```text
Actual Sum = 3 + 0 + 1

           = 4
```

**Step 3: Calculate missing number**

```text
Missing Number = Expected Sum - Actual Sum

               = 6 - 4

               = 2
```

**Output:**

```cpp
2
```

### Complexity Analysis

- **Time Complexity:** O(n), because we traverse the array once.
- **Space Complexity:** O(1), because we use only a constant number of variables.

### Important Note

For larger integer ranges, multiplication can overflow an `int`.

Using `long long` makes the arithmetic safer:

```cpp
long long expectedSum = 1LL * n * (n + 1) / 2;
```

---

## 6. Approach 5 — XOR (Optimized and Recommended)

### Core Idea

XOR has three important properties:

**Property 1: Same numbers cancel each other.**

```text
a ^ a = 0
```

**Property 2: XOR with zero returns the same number.**

```text
a ^ 0 = a
```

**Property 3: XOR is commutative and associative.**

```text
a ^ b = b ^ a

(a ^ b) ^ c = a ^ (b ^ c)
```

### Main Observation

The complete range contains:

```text
0, 1, 2, 3, ..., n
```

The array contains all these numbers except one.

If we XOR:

1. Every number from `0` to `n`.
2. Every element present in the array.

All numbers appearing twice cancel each other.

Only the missing number remains.

### C++ Code

```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {

        int n = nums.size();

        int result = n;

        for (int i = 0; i < n; i++) {

            result ^= i;

            result ^= nums[i];
        }

        return result;
    }
};
```

### Why do we initialize result with n?

The loop processes:

```text
0, 1, 2, ..., n-1
```

But the expected range also contains `n`.

Initializing:

```cpp
int result = n;
```

ensures that `n` is included in the XOR operation.

### Detailed Dry Run

Input:

```cpp
nums = [3, 0, 1]
```

Array size:

```text
n = 3
```

Initialize:

```cpp
result = 3;
```

Now process every index.

| Iteration | i | nums[i] | XOR operation | Result |
|---|---|---|---|---|
| Initial | - | - | 3 | 3 |
| 1 | 0 | 3 | 3 ^ 0 ^ 3 | 0 |
| 2 | 1 | 0 | 0 ^ 1 ^ 0 | 1 |
| 3 | 2 | 1 | 1 ^ 2 ^ 1 | 3 |

Final result:

```cpp
3
```

Wait — the table above must be evaluated carefully:

- Initial result = 3
- Iteration 1: `3 ^ 0 ^ 3 = 0`
- Iteration 2: `0 ^ 1 ^ 0 = 1`
- Iteration 3: `1 ^ 2 ^ 1 = 2`

Correct final result:

```cpp
2
```

### XOR Cancellation Explained

All numbers in the complete range:

```text
0 ^ 1 ^ 2 ^ 3
```

Array elements:

```text
3 ^ 0 ^ 1
```

Combine them:

```text
(0 ^ 1 ^ 2 ^ 3) ^ (3 ^ 0 ^ 1)
```

Rearrange:

```text
(0 ^ 0) ^ (1 ^ 1) ^ 2 ^ (3 ^ 3)
```

Cancel equal numbers:

```text
0 ^ 0 ^ 0 ^ 2
```

Final result:

```text
2
```

### Complexity Analysis

- **Time Complexity:** O(n), because we traverse the array once.
- **Space Complexity:** O(1), because we use only a constant number of variables.

This is the recommended solution when demonstrating XOR properties in an interview.

---

## 7. Approach Comparison

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Sorting | O(n log n) | Depends on sorting implementation |
| Hash Set | O(n) average | O(n) |
| Mathematical Sum | O(n) | O(1) |
| XOR | O(n) | O(1) |

### Which approach should you use?

- **Mathematical Sum:** Best when you recognize a missing value in a known numerical range.
- **XOR:** Best when you want to demonstrate bit manipulation and avoid sum overflow concerns.

Both optimized approaches satisfy the follow-up requirements.

---

## 8. Important Interview Insights

### Q1. Why does the mathematical approach work?

The expected sum includes every number from `0` to `n`.

The actual sum excludes exactly one number.

Subtracting the actual sum from the expected sum gives the missing number.

### Q2. Why does XOR work?

Every number except the missing number appears twice in the combined XOR operation.

Since:

```cpp
a ^ a = 0
```

All repeated numbers cancel, leaving only the missing number.

### Q3. Why do we initialize XOR with n?

Because our loop processes indices from `0` to `n-1`.

Initializing with `n` includes the final number in the expected range.

### Q4. Can we solve this without extra space?

Yes.

Both mathematical sum and XOR use O(1) extra space.

### Q5. What happens if zero is missing?

Example:

```cpp
nums = [1, 2, 3]
```

Expected range:

```text
[0, 1, 2, 3]
```

Both approaches correctly return:

```cpp
0
```

### Q6. What happens if n is missing?

Example:

```cpp
nums = [0, 1, 2]
```

Expected range:

```text
[0, 1, 2, 3]
```

Both approaches correctly return:

```cpp
3
```

---

## 9. Common Mistakes

1. Using the range `[0, n-1]` instead of `[0, n]`.
2. Forgetting that the array contains `n` elements but the expected range contains `n+1` values.
3. Initializing the XOR result to zero but forgetting to include `n`.
4. Using `a | a = 0` instead of the correct XOR property `a ^ a = 0`.
5. Using `int` for potentially large expected-sum calculations without considering overflow.
6. Assuming the missing number must be somewhere in the middle. It can be zero or `n`.

---

## 10. Pattern Recognition

| Problem Observation | Technique |
|---|---|
| One number missing from a known range | Expected Sum |
| Repeated values cancel out in pairs | XOR |
| Need O(1) extra space | Math or XOR |
| Need to detect duplicates or missing values | Hashing |
| Need to find a mismatch in sorted positions | Sorting |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Single Number | 136 | XOR |
| Missing Number | 268 | XOR + Math |
| Find the Duplicate Number | 287 | Cycle Detection / Binary Search |
| Set Mismatch | 645 | Hashing / Math / XOR |
| Single Number II | 137 | Bit Manipulation |

---

## 11. Key Takeaways

Remember these important properties:

**XOR properties:**

```cpp
a ^ a = 0;

a ^ 0 = a;
```

**Mathematical formula:**

```cpp
n * (n + 1) / 2
```

**Recommended XOR solution:**

```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {

        int n = nums.size();

        int result = n;

        for (int i = 0; i < n; i++) {

            result ^= i;

            result ^= nums[i];
        }

        return result;
    }
};
```

**Final Complexity:**

- Time: O(n)
- Space: O(1)

**Interview takeaway:**

Whenever a problem involves numbers appearing in pairs or a missing number from a known range, consider XOR.

The most important observation is:

```text
Equal numbers cancel each other using XOR.
```
# Sum of Two Integers

- **Difficulty:** Medium
- **Problem:** LeetCode 371
- **Pattern:** Bit Manipulation
- **Language:** C++
- **Key Techniques:** XOR, AND, Left Shift

## 1. Problem Understanding

Given two integers `a` and `b`, return their sum without using the `+` and `-` operators.

### Example 1

**Input:**

```cpp
a = 1, b = 1
```

**Output:**

```cpp
2
```

### Example 2

**Input:**

```cpp
a = 4, b = 7
```

**Output:**

```cpp
11
```

### Constraints

```text
-1000 <= a, b <= 1000
```

### Important Observation

Normal binary addition uses two operations:

1. Addition without carry.
2. Carry generation.

We can perform these operations using bitwise operators.

---

## 2. Understanding Binary Addition

Before solving the problem, understand these three operations:

| Operator | Meaning |
|---|---|
| XOR (`^`) | Addition without carry |
| AND (`&`) | Identifies positions where carry is generated |
| Left Shift (`<<`) | Moves carry to the next position |

### Binary Addition Truth Table

| A | B | Sum (XOR) | Carry (AND) |
|---|---|---|---|
| 0 | 0 | 0 | 0 |
| 0 | 1 | 1 | 0 |
| 1 | 0 | 1 | 0 |
| 1 | 1 | 0 | 1 |

Notice:

```text
1 + 1 = 10 (binary)
```

XOR produces `0`, while AND produces `1`.

The carry must move one position to the left.

Therefore:

```cpp
carry = (a & b) << 1;
```

---

## 3. Approach 1 — Brute Force

The direct solution would be:

```cpp
return a + b;
```

However, this is not allowed because the problem explicitly prohibits using the `+` operator.

We need to simulate addition using bitwise operations.

---

## 4. Approach 2 — Iterative Bit Manipulation (Optimized)

### Core Idea

We repeatedly calculate:

**Step 1: Calculate sum without carry**

```cpp
int sum = a ^ b;
```

XOR adds bits without generating carry.

**Step 2: Calculate carry**

```cpp
int carry = (a & b) << 1;
```

AND identifies positions where both bits are `1`.

Left shift moves the carry to the next bit position.

**Step 3: Update the numbers**

```cpp
a = sum;
b = carry;
```

Repeat until there is no carry.

When `b == 0`, no carry remains, so `a` contains the final answer.

### C++ Code

```cpp
class Solution {
public:
    int getSum(int a, int b) {

        while (b != 0) {

            int sum = a ^ b;

            int carry = (a & b) << 1;

            a = sum;

            b = carry;
        }

        return a;
    }
};
```

---

## 5. Detailed Dry Run

Input:

```cpp
a = 4;
b = 7;
```

### Step 1: Convert into Binary

```text
4 = 0100
7 = 0111
```

### Step 2: First Iteration

Calculate sum without carry:

```text
  0100
^ 0111
------
  0011
```

Therefore:

```cpp
sum = 3;
```

Calculate carry:

```text
  0100
& 0111
------
  0100
```

Shift left by one:

```text
0100 << 1 = 1000
```

Therefore:

```cpp
carry = 8;
```

Update:

```cpp
a = 3;
b = 8;
```

### Step 3: Second Iteration

Binary representation:

```text
a = 0011
b = 1000
```

Calculate sum:

```text
  0011
^ 1000
------
  1011
```

Therefore:

```cpp
sum = 11;
```

Calculate carry:

```text
  0011
& 1000
------
  0000
```

Therefore:

```cpp
carry = 0;
```

Update:

```cpp
a = 11;
b = 0;
```

### Step 4: Loop Termination

Condition:

```cpp
while (b != 0)
```

Since `b == 0`, the loop stops.

Return:

```cpp
11
```

### Iteration Summary

| Iteration | a | b | a XOR b | (a AND b) << 1 |
|---|---:|---:|---:|---:|
| Initial | 4 | 7 | 3 | 8 |
| 1 | 3 | 8 | 11 | 0 |
| 2 | 11 | 0 | - | - |

Final answer:

```cpp
11
```

---

## 6. Why Does This Algorithm Work?

Consider ordinary binary addition:

```text
  0100
+ 0111
------
  1011
```

The algorithm separates the operation into two parts.

### XOR: Addition Without Carry

```cpp
a ^ b
```

XOR gives the result of adding each pair of bits while ignoring carry.

### AND: Carry Generation

```cpp
a & b
```

A carry is generated only when both corresponding bits are `1`.

### Left Shift: Carry Position

```cpp
(a & b) << 1
```

The carry generated at one position must be added to the next higher position.

### Repeat

The carry generated in one iteration is added in the next iteration.

We continue until no carry remains.

---

## 7. Handling Negative Numbers

C++ signed integers are commonly represented using two's complement.

The same bitwise addition algorithm works for negative values using fixed-width integer representation.

Example:

```cpp
a = -2;
b = 3;
```

Expected result:

```cpp
1
```

The algorithm continues processing the bit patterns until the carry becomes zero.

### Important C++ Note

The simple signed implementation is the commonly used LeetCode solution.

For strictly portable fixed-width bitwise handling, unsigned integers are safer because left-shifting a negative signed integer is undefined behavior in C++.

---

## 8. Complexity Analysis

Let `W` represent the number of bits in the integer representation.

- **Time Complexity:** O(W) in the worst case, because carry propagation can require multiple iterations.
- **Space Complexity:** O(1), because only a fixed number of variables are used.

For a fixed-width 32-bit integer, the number of iterations is bounded by the integer width.

---

## 9. Alternative Approach — Recursive Bit Manipulation

The same logic can also be implemented recursively.

### C++ Code

```cpp
class Solution {
public:
    int getSum(int a, int b) {

        if (b == 0) {
            return a;
        }

        return getSum(a ^ b, (a & b) << 1);
    }
};
```

### Explanation

Each recursive call performs:

```cpp
a ^ b
```

to calculate the sum without carry, and:

```cpp
(a & b) << 1
```

to calculate the next carry.

The recursion stops when the carry becomes zero.

### Complexity

- **Time Complexity:** O(W) for fixed-width integers.
- **Space Complexity:** O(W) due to the recursive call stack.

The iterative approach is preferable because it avoids recursion overhead.

---

## 10. Approach Comparison

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Direct addition | O(1) | O(1) |
| Iterative XOR + AND | O(W) | O(1) |
| Recursive XOR + AND | O(W) | O(W) |

---

## 11. Important Interview Insights

### Q1. Why does XOR represent addition without carry?

Because XOR returns `1` when two bits differ and `0` when they are equal.

This matches binary addition when carry is ignored.

### Q2. Why do we use AND?

AND returns `1` only when both bits are `1`.

That is exactly when binary addition generates a carry.

### Q3. Why do we shift the carry left?

Because a carry generated at bit position `i` must be added at bit position `i + 1`.

### Q4. Why does the loop stop when b becomes zero?

In this algorithm, `b` stores the carry.

When `b == 0`, there is no remaining carry to process.

### Q5. Can this algorithm handle negative integers?

Yes, using fixed-width two's complement bit representations.

### Q6. Why is XOR alone insufficient?

XOR ignores carry.

For example:

```text
1 ^ 1 = 0
```

But:

```text
1 + 1 = 10
```

Therefore, we must separately calculate and propagate the carry.

---

## 12. Common Mistakes

1. Using XOR alone and forgetting carry generation.
2. Forgetting to shift the carry left.
3. Updating `a` before calculating the carry.
4. Using the wrong loop condition.
5. Not understanding why the algorithm works with negative numbers.
6. Ignoring signed left-shift limitations in strictly portable C++.

---

## 13. Pattern Recognition

| Problem Requirement | Technique |
|---|---|
| Add two numbers without `+` | XOR + AND |
| Addition without carry | XOR |
| Identify carry positions | AND |
| Move carry to next bit | Left Shift |
| Count set bits | Brian Kernighan's Algorithm |
| Find a unique number among pairs | XOR |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Single Number | 136 | XOR |
| Number of 1 Bits | 191 | Bit Manipulation |
| Counting Bits | 338 | Bit Manipulation + DP |
| Missing Number | 268 | XOR |
| Sum of Two Integers | 371 | XOR + AND |
| Reverse Bits | 190 | Bit Manipulation |

---

## 14. Key Takeaways

Remember these three operations:

```cpp
// Addition without carry
a ^ b

// Carry generation
a & b

// Move carry to the next bit
(a & b) << 1
```

### Final Recommended Solution

```cpp
class Solution {
public:
    int getSum(int a, int b) {

        while (b != 0) {

            int sum = a ^ b;

            int carry = (a & b) << 1;

            a = sum;
            b = carry;
        }

        return a;
    }
};
```

### Final Complexity

- Time: O(W), where W is the integer bit width.
- Space: O(1).

**Interview takeaway:**

Binary addition can be divided into two independent operations:

1. XOR calculates the sum without carry.
2. AND followed by left shift calculates the carry.

Repeating these operations until the carry becomes zero gives the final sum.

# Reverse Integer

- **Difficulty:** Medium
- **Problem:** LeetCode 7
- **Pattern:** Math / Digit Manipulation
- **Language:** C++
- **Key Techniques:** Modulo (`%`), Integer Division (`/`), Overflow Handling

## 1. Problem Understanding

Given a signed 32-bit integer `x`, reverse its digits and return the reversed integer.

Important conditions:

1. Preserve the sign of the original number.
2. If the reversed number goes outside the signed 32-bit integer range, return `0`.
3. Do not use integers outside the signed 32-bit range to solve the problem.

### Signed 32-bit Integer Range

```text
INT_MIN = -2147483648
INT_MAX =  2147483647
```

### Example 1

**Input:**

```cpp
x = 1234;
```

**Output:**

```cpp
4321
```

### Example 2

**Input:**

```cpp
x = -1234;
```

**Output:**

```cpp
-4321
```

### Example 3

**Input:**

```cpp
x = 1234236467;
```

**Output:**

```cpp
0
```

The reversed number exceeds the 32-bit signed integer range.

---

## 2. Understanding Digit Extraction

We can extract digits from a number using two operations.

### Modulo (%)

Returns the last digit.

```cpp
int digit = x % 10;
```

Example:

```text
1234 % 10 = 4
```

### Integer Division (/)

Removes the last digit.

```cpp
x = x / 10;
```

Example:

```text
1234 / 10 = 123
```

C++ integer division truncates toward zero.

For example:

```text
-1234 % 10 = -4
-1234 / 10 = -123
```

These operations allow us to reverse the number without converting it into a string.

---

## 3. Approach 1 — String Conversion

### Idea

1. Convert the integer into a string.
2. Reverse the digits while preserving the sign.
3. Convert the reversed string back into an integer.
4. Check whether the result is within the signed 32-bit range.

### Limitation

Converting an extremely large reversed string directly into an `int` can cause overflow.

We would need additional range checking during conversion.

This approach is less efficient in terms of extra space and does not teach the digit-manipulation pattern as directly.

---

## 4. Approach 2 — Mathematical Digit Reversal (Optimized)

### Core Idea

Use modulo and division to extract digits one by one.

The reversed number is calculated using:

```cpp
rev = rev * 10 + digit;
```

### Algorithm

**Step 1:** Initialize the reversed number.

```cpp
int rev = 0;
```

**Step 2:** Extract the last digit.

```cpp
int digit = x % 10;
```

**Step 3:** Remove the last digit.

```cpp
x /= 10;
```

**Step 4:** Check overflow before updating `rev`.

**Step 5:** Append the extracted digit.

```cpp
rev = rev * 10 + digit;
```

Repeat until `x` becomes zero.

---

## 5. C++ Optimized Solution

```cpp
class Solution {
public:
    int reverse(int x) {

        int rev = 0;

        while (x != 0) {

            int digit = x % 10;

            x /= 10;

            // Check positive overflow
            if (rev > INT_MAX / 10 ||
                (rev == INT_MAX / 10 && digit > 7)) {
                return 0;
            }

            // Check negative overflow
            if (rev < INT_MIN / 10 ||
                (rev == INT_MIN / 10 && digit < -8)) {
                return 0;
            }

            rev = rev * 10 + digit;
        }

        return rev;
    }
};
```

Include the following header if needed:

```cpp
#include <climits>
```

---

## 6. Understanding Overflow Handling

This is the most important part of the problem.

Before performing:

```cpp
rev = rev * 10 + digit;
```

we must ensure that the operation will not exceed the 32-bit integer range.

### Positive Overflow

Maximum signed 32-bit integer:

```text
2147483647
```

Before appending another digit, the current reversed number must satisfy:

```cpp
rev < INT_MAX / 10
```

Since:

```text
INT_MAX / 10 = 214748364
```

If:

```cpp
rev == 214748364
```

the next digit cannot be greater than `7`.

Therefore:

```cpp
if (rev > INT_MAX / 10 ||
    (rev == INT_MAX / 10 && digit > 7)) {
    return 0;
}
```

### Negative Overflow

Minimum signed 32-bit integer:

```text
-2147483648
```

Similarly:

```text
INT_MIN / 10 = -214748364
```

If:

```cpp
rev == -214748364
```

the next digit cannot be smaller than `-8`.

Therefore:

```cpp
if (rev < INT_MIN / 10 ||
    (rev == INT_MIN / 10 && digit < -8)) {
    return 0;
}
```

### Why Are the Last Digits 7 and -8?

Because the 32-bit signed range is asymmetric:

```text
INT_MAX =  2147483647
INT_MIN = -2147483648
```

The positive boundary ends in `7`, while the negative boundary ends in `8`.

---

## 7. Detailed Dry Run

### Example 1: Positive Number

Input:

```cpp
x = 1234;
```

Initial state:

```text
rev = 0
x = 1234
```

| Iteration | Digit (x % 10) | Remaining x | rev = rev * 10 + digit |
|---|---:|---:|---:|
| 1 | 4 | 123 | 4 |
| 2 | 3 | 12 | 43 |
| 3 | 2 | 1 | 432 |
| 4 | 1 | 0 | 4321 |

Final output:

```cpp
4321
```

### Example 2: Negative Number

Input:

```cpp
x = -1234;
```

C++ modulo preserves the sign of the dividend.

| Iteration | Digit (x % 10) | Remaining x | rev |
|---|---:|---:|---:|
| 1 | -4 | -123 | -4 |
| 2 | -3 | -12 | -43 |
| 3 | -2 | -1 | -432 |
| 4 | -1 | 0 | -4321 |

Final output:

```cpp
-4321
```

Notice that we do not need separate logic to reverse negative numbers.

### Example 3: Positive Overflow

Input:

```cpp
x = 1234236467;
```

The reversed number would be:

```text
7646324321
```

But:

```text
INT_MAX = 2147483647
```

The reversed value is outside the permitted range.

The overflow condition detects this before performing the unsafe multiplication.

Output:

```cpp
0
```

---

## 8. Complexity Analysis

Let `D` be the number of digits in the input number.

| Complexity | Value | Explanation |
|---|---|---|
| Time | O(log10(|x|)) | Each iteration removes one digit |
| Space | O(1) | Only a fixed number of variables are used |

For a 32-bit integer, there can be at most 10 decimal digits.

---

## 9. Common Mistakes

1. Forgetting to preserve the negative sign.
2. Using `x % 10` incorrectly for negative numbers.
3. Checking overflow after multiplication instead of before it.
4. Checking only positive overflow and ignoring negative overflow.
5. Using `long long` to store the reversed result when the problem specifically requires handling overflow within the 32-bit range.
6. Forgetting that `INT_MIN` and `INT_MAX` have different last digits.
7. Using floating-point operations for digit extraction unnecessarily.

---

## 10. Important Interview Insights

### Q1. Why use modulo 10?

It extracts the last digit of the number.

```cpp
digit = x % 10;
```

### Q2. Why divide by 10?

It removes the last digit.

```cpp
x /= 10;
```

### Q3. Why do we multiply the reversed number by 10?

Because we need to shift existing digits one position to the left before appending the next digit.

Example:

```text
rev = 43
digit = 2

43 * 10 + 2 = 432
```

### Q4. Why check overflow before multiplication?

Because performing an overflowing signed integer operation in C++ causes undefined behavior.

We must verify that the next operation is safe before executing it.

### Q5. Why are the overflow boundaries 7 and -8?

Because the signed 32-bit integer range is:

```text
-2147483648 to 2147483647
```

### Q6. Does the same algorithm work for negative numbers?

Yes. C++ modulo and division truncate toward zero, so extracted digits retain the negative sign.

---

## 11. Pattern Recognition

This problem belongs to the mathematical digit-manipulation pattern.

| Problem Requirement | Technique |
|---|---|
| Extract last digit | `% 10` |
| Remove last digit | `/ 10` |
| Reverse digits | `rev * 10 + digit` |
| Check integer boundaries | `INT_MIN`, `INT_MAX` |
| Count digits | Repeated division by 10 |
| Check palindrome number | Digit extraction and reversal |

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Palindrome Number | 9 | Digit Manipulation |
| Reverse Integer | 7 | Digit Manipulation + Overflow |
| String to Integer (atoi) | 8 | Parsing + Overflow |
| Sum of Two Integers | 371 | Bit Manipulation |

---

## 12. Key Takeaways

Remember these three operations:

```cpp
// Extract last digit
int digit = x % 10;

// Remove last digit
x /= 10;

// Append digit to reversed number
rev = rev * 10 + digit;
```

Most importantly:

**Always check integer overflow before multiplying the reversed number by 10.**

### Final Recommended Solution

```cpp
class Solution {
public:
    int reverse(int x) {

        int rev = 0;

        while (x != 0) {

            int digit = x % 10;
            x /= 10;

            if (rev > INT_MAX / 10 ||
                (rev == INT_MAX / 10 && digit > 7)) {
                return 0;
            }

            if (rev < INT_MIN / 10 ||
                (rev == INT_MIN / 10 && digit < -8)) {
                return 0;
            }

            rev = rev * 10 + digit;
        }

        return rev;
    }
};
```

### Interview Takeaway

The main learning is not just reversing digits. It is learning how to handle integer overflow safely before performing arithmetic operations.


**Duplicate numbers cancel each other, and the unique number survives.**
