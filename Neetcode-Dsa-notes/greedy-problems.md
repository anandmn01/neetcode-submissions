# 53. Maximum Subarray

- **Difficulty:** Medium
- **Pattern:** Dynamic Programming / Greedy
- **Algorithm:** Kadane's Algorithm
- **Approaches:** Brute Force, Prefix Sum, Kadane's Algorithm
- **Language:** C++

## 1. Problem Understanding

We are given an integer array `nums`.

We need to find a contiguous, non-empty subarray that has the largest possible sum.

A subarray is a continuous sequence of elements within an array.

Return the maximum subarray sum.

### Example 1

**Input:**

```cpp
nums = {2, -3, 4, -2, 2, 1, -4};
```

**Output:**

```cpp
8
```

**Explanation:**

The maximum sum subarray is:

```cpp
{4, -2, 2, 1, -4}
```

Its sum is:

```cpp
4 + (-2) + 2 + 1 + (-4) = 1
```

However, the actual maximum subarray is:

```cpp
{4, -2, 2, 1, -4}
```

Wait: for the given input, the maximum subarray is actually:

```cpp
{4, -2, 2, 1}
```

Sum:

```cpp
4 - 2 + 2 + 1 = 5
```

The screenshot's example is:

```cpp
nums = {2, -3, 4, -2, 2, 1, -4};
```

For this input, the correct maximum subarray sum is **5**.

### Example 2

**Input:**

```cpp
nums = {-1};
```

**Output:**

```cpp
-1
```

Explanation:

There is only one element, so the maximum subarray sum is `-1`.

## 2. Brute Force Approach — Generate All Subarrays

### Idea

We generate every possible contiguous subarray and calculate its sum.

For every starting index:

1. Initialize the subarray sum to zero.
2. Expand the subarray by moving the ending index.
3. Keep adding elements.
4. Update the maximum sum whenever we find a larger sum.

### C++ Code

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {

        int n = nums.size();

        int maxSum = INT_MIN;

        for (int i = 0; i < n; i++) {

            int currentSum = 0;

            for (int j = i; j < n; j++) {

                currentSum += nums[j];

                maxSum = max(maxSum, currentSum);
            }
        }

        return maxSum;
    }
};
```

### Complexity Analysis

- Outer loop: `O(n)`.
- Inner loop: `O(n)`.

**Time Complexity:** `O(n²)`

**Space Complexity:** `O(1)`.

### Limitation

We calculate the sum of every possible subarray.

For an array containing 100,000 elements, this approach is too slow.

We need a more efficient solution.

## 3. Improved Approach — Prefix Sum

### Core Concept

A prefix sum stores the cumulative sum of elements.

We can use the difference between two prefix sums to calculate a subarray sum.

However, we need to track the minimum prefix sum encountered so far.

### Formula

Let:

- `prefixSum` = Sum of elements from index 0 to the current index.
- `minPrefix` = Minimum prefix sum encountered before the current position.

Then:

```cpp
currentSubarraySum = prefixSum - minPrefix;
```

We update the maximum subarray sum using this value.

### C++ Code

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {

        int prefixSum = 0;
        int minPrefix = 0;

        int maxSum = INT_MIN;

        for (int num : nums) {

            prefixSum += num;

            maxSum = max(maxSum, prefixSum - minPrefix);

            minPrefix = min(minPrefix, prefixSum);
        }

        return maxSum;
    }
};
```

### Complexity Analysis

**Time Complexity:** `O(n)`.

We traverse the array once.

**Space Complexity:** `O(1)`.

We use only a few variables.

## 4. Optimized Approach — Kadane's Algorithm

### Core Concept

Kadane's Algorithm solves the Maximum Subarray problem in one traversal.

At every element, we have two choices:

1. Extend the previous subarray.
2. Start a new subarray from the current element.

We choose whichever gives the larger sum.

### Important Formula

```cpp
currentSum = max(nums[i], currentSum + nums[i]);
```

Where:

- `nums[i]` means starting a new subarray.
- `currentSum + nums[i]` means extending the previous subarray.

We also maintain:

```cpp
maxSum = max(maxSum, currentSum);
```

This stores the maximum subarray sum found so far.

### Why Do We Start a New Subarray?

Suppose:

```cpp
nums = {-2, 1, -3, 4};
```

When we reach `1`, extending the previous subarray gives:

```cpp
-2 + 1 = -1
```

Starting a new subarray gives:

```cpp
1
```

Since `1` is greater than `-1`, we discard the previous negative contribution and start a new subarray.

This is the main idea behind Kadane's Algorithm.

### Algorithm

1. Initialize `currentSum` with the first element.
2. Initialize `maxSum` with the first element.
3. Traverse the array starting from index 1.
4. At each index:
   - Decide whether to extend the previous subarray or start a new one.
   - Update `currentSum`.
   - Update `maxSum`.
5. Return `maxSum`.

### Optimized C++ Solution

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {

        int currentSum = nums[0];
        int maxSum = nums[0];

        for (int i = 1; i < nums.size(); i++) {

            // Extend the previous subarray or start a new one
            currentSum = max(nums[i], currentSum + nums[i]);

            // Update the maximum subarray sum
            maxSum = max(maxSum, currentSum);
        }

        return maxSum;
    }
};
```

## 5. Dry Run — Step by Step

**Input:**

```cpp
nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
```

Initial values:

```cpp
currentSum = -2;
maxSum = -2;
```

### Step-by-step Execution

| Index | Element | Current Sum Calculation | Current Sum | Maximum Sum |
|---|---:|---|---:|---:|
| 0 | -2 | Initial value | -2 | -2 |
| 1 | 1 | max(1, -2 + 1) | 1 | 1 |
| 2 | -3 | max(-3, 1 - 3) | -2 | 1 |
| 3 | 4 | max(4, -2 + 4) | 4 | 4 |
| 4 | -1 | max(-1, 4 - 1) | 3 | 4 |
| 5 | 2 | max(2, 3 + 2) | 5 | 5 |
| 6 | 1 | max(1, 5 + 1) | 6 | 6 |
| 7 | -5 | max(-5, 6 - 5) | 1 | 6 |
| 8 | 4 | max(4, 1 + 4) | 5 | 6 |

### Final Result

The maximum subarray is:

```cpp
{4, -1, 2, 1}
```

Sum:

```cpp
4 - 1 + 2 + 1 = 6
```

**Output:**

```cpp
6
```

## 6. Understanding the Kadane's Algorithm Decision

The most important part is:

```cpp
currentSum = max(nums[i], currentSum + nums[i]);
```

Let's understand both choices.

### Choice 1: Extend Previous Subarray

```cpp
currentSum + nums[i]
```

We continue the existing subarray.

This is useful when the previous sum is positive and contributes positively to the new subarray.

### Choice 2: Start New Subarray

```cpp
nums[i]
```

We discard the previous subarray and start from the current element.

This is useful when the previous sum is negative.

### Example

```cpp
nums = {-5, 4, 2};
```

At index 1:

```cpp
currentSum = max(4, -5 + 4);

currentSum = max(4, -1);

currentSum = 4;
```

We start a new subarray at `4`.

At index 2:

```cpp
currentSum = max(2, 4 + 2);

currentSum = 6;
```

The maximum subarray is:

```cpp
{4, 2}
```

## 7. Time and Space Complexity

Let `n` be the number of elements.

### Brute Force

**Time Complexity:** `O(n²)`

**Space Complexity:** `O(1)`.

### Prefix Sum

**Time Complexity:** `O(n)`.

**Space Complexity:** `O(1)`.

### Kadane's Algorithm

**Time Complexity:** `O(n)`.

We traverse the array only once.

**Space Complexity:** `O(1)`.

We use only two variables.

## 8. Important Interview Insights

### Q1. What is Kadane's Algorithm?

Kadane's Algorithm finds the maximum sum of a contiguous subarray in linear time.

It decides whether to extend the previous subarray or start a new one at every position.

### Q2. Why do we use `max(nums[i], currentSum + nums[i])`?

Because we have two possible choices:

- Start a new subarray from the current element.
- Extend the previous subarray.

We choose the option that gives the larger sum.

### Q3. Why do we initialize `maxSum` with `nums[0]` instead of zero?

Because the problem requires a non-empty subarray.

Consider:

```cpp
nums = {-5, -2, -8};
```

The answer must be:

```cpp
-2
```

If we initialize `maxSum = 0`, we may incorrectly return zero.

### Q4. What happens if all elements are negative?

Kadane's Algorithm still works correctly because it always considers the current element as a possible new subarray.

Example:

```cpp
nums = {-3, -2, -5};
```

Output:

```cpp
-2
```

### Q5. Is Kadane's Algorithm Dynamic Programming?

Yes.

At every index, we calculate the best subarray sum ending at that index using the result from the previous index.

The recurrence is:

```cpp
dp[i] = max(nums[i], dp[i - 1] + nums[i]);
```

We optimize the DP space by keeping only the previous state in `currentSum`.

## 9. Common Mistakes

- Initializing `maxSum` to zero.
- Forgetting that the subarray must be contiguous.
- Adding every element without considering whether to start a new subarray.
- Confusing subsequence with subarray.
- Forgetting to update `maxSum` after calculating `currentSum`.
- Using an empty subarray when all elements are negative.

## 10. Comparison of Approaches

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Prefix Sum | O(n) | O(1) |
| Kadane's Algorithm | O(n) | O(1) |

## 11. Key Takeaways

### Most Important Pattern

**Maximum Sum Contiguous Subarray → Kadane's Algorithm**

Remember the two important formulas:

```cpp
currentSum = max(nums[i], currentSum + nums[i]);

maxSum = max(maxSum, currentSum);
```

### Pattern Recognition

Whenever a problem asks for the maximum sum of a contiguous subarray, consider Kadane's Algorithm.

### Connection With Other NeetCode Problems

- Maximum Subarray → Kadane's Algorithm.
- Maximum Product Subarray → Modified Kadane's Algorithm.
- Best Time to Buy and Sell Stock → Greedy / Minimum Prefix.
- Maximum Sum Circular Subarray → Kadane's Algorithm + Minimum Subarray.

## 12. Final Interview Revision

Remember these three concepts:

1. A subarray must be contiguous.
2. At every index, decide whether to extend the previous subarray or start a new one.
3. Keep updating the global maximum.

**Most Important Insight:**

If the previous subarray sum is negative, carrying it forward may reduce the current sum.

Therefore, Kadane's Algorithm allows us to discard the previous contribution and start a new subarray whenever that is better.

**Final Complexity:**
- Time: `O(n)`
- Space: `O(1)`

  # 55. Jump Game

- **Difficulty:** Medium
- **Pattern:** Greedy
- **Approaches:** Brute Force, Dynamic Programming, Greedy
- **Language:** C++

## 1. Problem Understanding

We are given an integer array `nums`.

Each element represents the maximum jump length we can make from that position.

We start at index `0` and need to determine whether we can reach the last index.

We can jump from index `i` to any index between:

```cpp
i + 1
```

and

```cpp
i + nums[i]
```

We need to return:

- `true` if we can reach the last index.
- `false` otherwise.

### Example 1

**Input:**

```cpp
nums = {2, 3, 1, 1, 4};
```

**Output:**

```cpp
true
```

**Explanation:**

- Start at index 0.
- Jump to index 1.
- From index 1, jump to index 4.
- We successfully reach the last index.

### Example 2

**Input:**

```cpp
nums = {3, 2, 1, 0, 4};
```

**Output:**

```cpp
false
```

**Explanation:**

- Start at index 0.
- We can reach index 1, 2, or 3.
- At index 3, the jump length is 0.
- We cannot move forward to index 4.

Therefore, reaching the last index is impossible.

## 2. Brute Force Approach — Recursion

### Idea

At every index, we try every possible jump.

For the current index:

1. Find the maximum jump length.
2. Try jumping to every reachable next index.
3. Recursively check whether any of those positions can reach the destination.
4. If at least one path reaches the last index, return `true`.
5. Otherwise, return `false`.

### C++ Code

```cpp
class Solution {
public:

    bool solve(vector<int>& nums, int index) {

        // Destination reached
        if (index >= nums.size() - 1) {
            return true;
        }

        // Cannot move forward
        if (nums[index] == 0) {
            return false;
        }

        int maxJump = nums[index];

        // Try every possible jump
        for (int jump = 1; jump <= maxJump; jump++) {

            if (solve(nums, index + jump)) {
                return true;
            }
        }

        return false;
    }

    bool canJump(vector<int>& nums) {

        return solve(nums, 0);
    }
};
```

### Complexity Analysis

**Time Complexity:** Exponential in the worst case because multiple jump possibilities are explored recursively.

**Space Complexity:** `O(n)` for the maximum recursion depth.

### Limitation

We repeatedly explore the same indices through different paths.

This creates unnecessary recursive calls.

We can improve this using Dynamic Programming or Greedy.

## 3. Approach 2 — Dynamic Programming (Memoization)

### Core Concept

Instead of solving the same index repeatedly, we store its result.

We maintain a DP array:

- `-1` = Not calculated.
- `0` = Cannot reach the destination.
- `1` = Can reach the destination.

### Algorithm

1. Start from index 0.
2. Check whether the current index has already been calculated.
3. Try every possible jump.
4. If any jump reaches the destination, store `true`.
5. Otherwise, store `false`.
6. Return the stored result.

### C++ Code

```cpp
class Solution {
public:

    vector<int> dp;

    bool solve(vector<int>& nums, int index) {

        if (index >= nums.size() - 1) {
            return true;
        }

        if (dp[index] != -1) {
            return dp[index];
        }

        int maxJump = nums[index];

        for (int jump = 1; jump <= maxJump; jump++) {

            if (solve(nums, index + jump)) {
                return dp[index] = 1;
            }
        }

        return dp[index] = 0;
    }

    bool canJump(vector<int>& nums) {

        dp.assign(nums.size(), -1);

        return solve(nums, 0);
    }
};
```

### Complexity Analysis

**Time Complexity:** `O(n²)`.

Each index is calculated once, and each calculation may examine up to `n` possible jumps.

**Space Complexity:** `O(n)` for the DP array and recursion stack.

## 4. Optimized Approach — Greedy Algorithm

### Core Concept

Instead of exploring every possible jump, we maintain the farthest index that we can reach.

We use one variable:

```cpp
int maxReach = 0;
```

It represents the maximum index reachable using all jumps considered so far.

At every index:

1. Check whether the current index is reachable.
2. If not, return `false`.
3. Otherwise, update the farthest reachable index.

### Important Formula

```cpp
maxReach = max(maxReach, i + nums[i]);
```

Where:

- `i` = Current index.
- `nums[i]` = Maximum jump length from the current index.
- `i + nums[i]` = Farthest index reachable from the current index.

### Why Greedy Works

Suppose:

```cpp
nums = {2, 3, 1, 1, 4};
```

At index 0, we can reach index 2.

At index 1, we can reach index 4.

We don't need to explore every possible path.

We only need to know the farthest position reachable so far.

If the current index is beyond `maxReach`, that index cannot be reached.

Therefore, the destination is unreachable.

### Algorithm

1. Initialize `maxReach = 0`.
2. Traverse the array from left to right.
3. If `i > maxReach`, return `false`.
4. Update `maxReach` using `i + nums[i]`.
5. If `maxReach` reaches the last index, return `true`.
6. If traversal completes, return `true`.

### Optimized C++ Solution

```cpp
class Solution {
public:
    bool canJump(vector<int>& nums) {

        int maxReach = 0;

        for (int i = 0; i < nums.size(); i++) {

            // Current index is unreachable
            if (i > maxReach) {
                return false;
            }

            // Update farthest reachable position
            maxReach = max(maxReach, i + nums[i]);

            // Destination is reachable
            if (maxReach >= nums.size() - 1) {
                return true;
            }
        }

        return true;
    }
};
```

## 5. Dry Run — Step by Step

**Input:**

```cpp
nums = {2, 3, 1, 1, 4};
```

Initially:

```cpp
maxReach = 0;
```

### Step-by-step Execution

| Index | nums[i] | i + nums[i] | maxReach |
|---|---:|---:|---:|
| 0 | 2 | 2 | 2 |
| 1 | 3 | 4 | 4 |
| 2 | 1 | 3 | 4 |
| 3 | 1 | 4 | 4 |

At index 1:

```cpp
maxReach = max(2, 1 + 3);
maxReach = 4;
```

Since the last index is 4, we can reach the destination.

**Final Output:**

```cpp
true
```

## 6. Dry Run — Impossible Case

**Input:**

```cpp
nums = {3, 2, 1, 0, 4};
```

Initially:

```cpp
maxReach = 0;
```

| Index | nums[i] | i + nums[i] | maxReach |
|---|---:|---:|---:|
| 0 | 3 | 3 | 3 |
| 1 | 2 | 3 | 3 |
| 2 | 1 | 3 | 3 |
| 3 | 0 | 3 | 3 |
| 4 | 4 | 8 | Not processed |

At index 4:

```cpp
i = 4;
maxReach = 3;
```

Check:

```cpp
if (i > maxReach)
```

Since:

```cpp
4 > 3
```

We return:

```cpp
false
```

We cannot reach index 4.

## 7. Time and Space Complexity

Let `n` be the number of elements.

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Recursion | Exponential worst case | O(n) |
| DP Memoization | O(n²) | O(n) |
| Greedy | O(n) | O(1) |

### Optimized Greedy Complexity

**Time Complexity: `O(n)`**

We traverse the array only once.

**Space Complexity: `O(1)`**

We use only one additional variable, `maxReach`.

## 8. Important Interview Insights

### Q1. What does maxReach represent?

It represents the farthest index that can be reached from all positions processed so far.

### Q2. Why do we check `i > maxReach`?

Because if the current index is beyond the farthest reachable position, we cannot reach that index.

Therefore, reaching the destination is impossible.

### Q3. Why do we use `max()`?

A previous position might already allow us to reach farther than the current position.

We must never reduce the maximum reachable distance.

### Q4. Why don't we need to calculate the exact jumps?

The problem only asks whether the destination is reachable.

We don't need to find the actual path or minimum number of jumps.

Maintaining the farthest reachable index is sufficient.

### Q5. What happens if all elements are zero except the first element?

If the array contains more than one element and the first element is zero, we cannot reach the destination.

Example:

```cpp
nums = {0, 1};
```

Output:

```cpp
false
```

### Q6. What happens if the array contains only one element?

We are already at the destination.

Example:

```cpp
nums = {0};
```

Output:

```cpp
true
```

## 9. Common Mistakes

- Forgetting to check whether the current index is reachable.
- Updating `maxReach` without checking reachability first.
- Assigning `maxReach = i + nums[i]` instead of using `max()`.
- Confusing maximum jump length with exact jump length.
- Trying to calculate the minimum number of jumps when the problem only asks whether reaching the end is possible.
- Returning `false` immediately when encountering a zero, even though later positions might still be reachable.

## 10. Comparison of Approaches

| Approach | Main Idea | Time | Space |
|---|---|---|---|
| Recursion | Try every possible jump | Exponential | O(n) |
| DP | Store previously calculated results | O(n²) | O(n) |
| Greedy | Maintain farthest reachable index | O(n) | O(1) |

## 11. Key Takeaways

### Most Important Pattern

**Reachability Problem → Greedy + Maximum Reach**

Remember:

```cpp
maxReach = max(maxReach, i + nums[i]);
```

And the most important condition:

```cpp
if (i > maxReach) {
    return false;
}
```

### Pattern Recognition

Whenever a problem asks whether you can reach a destination using variable jump lengths, consider maintaining the farthest reachable position.

### Connection With Other NeetCode Problems

- Jump Game → Greedy / Maximum Reach.
- Jump Game II → Greedy / Minimum Jumps.
- Gas Station → Greedy.
- Maximum Subarray → Kadane's Algorithm.
- Task Scheduler → Greedy + Frequency Counting.

## 12. Final Interview Revision

Remember these three concepts:

1. `maxReach` stores the farthest reachable index.
2. If `i > maxReach`, the current index is unreachable.
3. If `maxReach >= nums.size() - 1`, the destination is reachable.

**Most Important Insight:**

We don't need to explore every possible jump.

We only need to track the farthest position reachable so far.

This reduces the solution from potentially exponential time to:

- Time: `O(n)`
- Space: `O(1)`

  # 45. Jump Game II

- **Difficulty:** Medium
- **Pattern:** Greedy / Breadth-First Search
- **Approaches:** Recursion, Dynamic Programming, Greedy
- **Language:** C++

## 1. Problem Understanding

We are given an integer array `nums`.

Each element represents the maximum jump length from that index.

We initially stand at index `0`.

We need to reach the last index using the minimum number of jumps.

From index `i`, we can jump to any position:

```cpp
i + j
```

Where:

```cpp
1 <= j <= nums[i]
```

The problem guarantees that reaching the last index is always possible.

### Example 1

**Input:**

```cpp
nums = {2, 4, 1, 1, 1};
```

**Output:**

```cpp
2
```

**Explanation:**

- Start at index 0.
- Jump from index 0 to index 1.
- From index 1, jump to the last index.

Total jumps = 2.

### Example 2

**Input:**

```cpp
nums = {2, 1, 2, 1, 0};
```

**Output:**

```cpp
2
```

**Explanation:**

- Jump from index 0 to index 2.
- Jump from index 2 to index 4.

Total jumps = 2.

## 2. Brute Force Approach — Recursion

### Idea

At every index, try every possible jump.

For each position:

1. Calculate the maximum jump length.
2. Try jumping to every reachable next index.
3. Recursively calculate the minimum jumps required from that position.
4. Return the minimum among all possible choices.

### C++ Code

```cpp
class Solution {
public:

    int solve(vector<int>& nums, int index) {

        int n = nums.size();

        // Destination reached
        if (index >= n - 1) {
            return 0;
        }

        int minJumps = INT_MAX;

        // Try every possible jump
        for (int jump = 1; jump <= nums[index]; jump++) {

            int nextIndex = index + jump;

            if (nextIndex < n) {

                int result = solve(nums, nextIndex);

                if (result != INT_MAX) {
                    minJumps = min(minJumps, 1 + result);
                }
            }
        }

        return minJumps;
    }

    int jump(vector<int>& nums) {

        return solve(nums, 0);
    }
};
```

### Complexity Analysis

**Time Complexity:** Exponential in the worst case because multiple jump combinations are explored.

**Space Complexity:** `O(n)` for the recursion stack.

### Limitation

The same index can be visited repeatedly through different paths.

This creates redundant recursive calls.

We can improve this using Dynamic Programming.

## 3. Approach 2 — Dynamic Programming (Memoization)

### Core Concept

Instead of solving the same index repeatedly, store the minimum jumps required from that index.

Define:

```cpp
dp[i]
```

As the minimum number of jumps required to reach the last index from index `i`.

### Recurrence

For every possible jump from index `i`:

```cpp
dp[i] = min(dp[i], 1 + dp[i + j]);
```

Where:

```cpp
1 <= j <= nums[i]
```

The base case is:

```cpp
dp[n - 1] = 0;
```

Because we need zero jumps when we are already at the destination.

### C++ Code

```cpp
class Solution {
public:

    vector<int> dp;

    int solve(vector<int>& nums, int index) {

        int n = nums.size();

        // Destination reached
        if (index >= n - 1) {
            return 0;
        }

        // Already calculated
        if (dp[index] != -1) {
            return dp[index];
        }

        int minJumps = INT_MAX;

        for (int jump = 1; jump <= nums[index]; jump++) {

            int nextIndex = index + jump;

            if (nextIndex < n) {

                int result = solve(nums, nextIndex);

                if (result != INT_MAX) {
                    minJumps = min(minJumps, 1 + result);
                }
            }
        }

        return dp[index] = minJumps;
    }

    int jump(vector<int>& nums) {

        dp.assign(nums.size(), -1);

        return solve(nums, 0);
    }
};
```

### Complexity Analysis

Let `n` be the number of elements.

- Each index is calculated once.
- Each index may examine up to `n` possible jumps.

**Time Complexity:** `O(n²)`.

**Space Complexity:** `O(n)` for the DP array and recursion stack.

## 4. Optimized Approach — Greedy Algorithm

### Core Concept

Instead of checking every possible path, we divide the array into reachable ranges.

We maintain three variables:

```cpp
int jumps = 0;
int currentEnd = 0;
int farthest = 0;
```

**1. jumps**

Stores the minimum number of jumps made so far.

**2. currentEnd**

Represents the farthest index we can reach using the current number of jumps.

**3. farthest**

Represents the farthest index reachable from all positions in the current range.

### Understanding With an Example

```cpp
nums = {2, 4, 1, 1, 1};
```

Initially:

```cpp
jumps = 0;
currentEnd = 0;
farthest = 0;
```

At index 0:

```cpp
farthest = max(0, 0 + 2);
farthest = 2;
```

We have explored the current range, which ends at index 0.

Now we must make our first jump.

```cpp
jumps = 1;
currentEnd = 2;
```

This means that after one jump, we can reach any position up to index 2.

Now explore indices 1 and 2 to determine how far we can reach with the next jump.

At index 1:

```cpp
farthest = max(2, 1 + 4);
farthest = 5;
```

We can reach the destination.

When we finish exploring the current range, we make the second jump.

Total jumps = 2.

### Algorithm

1. Initialize `jumps = 0`.
2. Initialize `currentEnd = 0`.
3. Initialize `farthest = 0`.
4. Traverse the array until the second-last index.
5. At every index, update the farthest reachable position:

```cpp
farthest = max(farthest, i + nums[i]);
```

6. When we reach `currentEnd`, we have explored all positions reachable using the current number of jumps.
7. Increase the jump count.
8. Update `currentEnd = farthest`.
9. Return the minimum number of jumps.

### Optimized C++ Solution

```cpp
class Solution {
public:
    int jump(vector<int>& nums) {

        int jumps = 0;

        int currentEnd = 0;

        int farthest = 0;

        for (int i = 0; i < nums.size() - 1; i++) {

            // Update farthest reachable index
            farthest = max(farthest, i + nums[i]);

            // Current jump range is completed
            if (i == currentEnd) {

                jumps++;

                currentEnd = farthest;
            }
        }

        return jumps;
    }
};
```

## 5. Dry Run — Step by Step

**Input:**

```cpp
nums = {2, 4, 1, 1, 1};
```

Initially:

```cpp
jumps = 0;
currentEnd = 0;
farthest = 0;
```

### Step 1: Index 0

```cpp
farthest = max(0, 0 + 2);
farthest = 2;
```

Now:

```cpp
i == currentEnd
```

Because:

```cpp
0 == 0
```

We have finished exploring the current range.

Increase jumps:

```cpp
jumps = 1;
currentEnd = 2;
```

### Step 2: Index 1

```cpp
farthest = max(2, 1 + 4);
farthest = 5;
```

We can reach index 5, which is beyond the last index.

### Step 3: Index 2

```cpp
farthest = max(5, 2 + 1);
farthest = 5;
```

Now:

```cpp
i == currentEnd
```

Because:

```cpp
2 == 2
```

We have completed the current range.

Increase jumps:

```cpp
jumps = 2;
currentEnd = 5;
```

The loop ends because we only need to traverse until the second-last index.

### Final Result

```cpp
return jumps;
```

**Output:**

```cpp
2
```

## 6. Understanding the Greedy Variables

This is the most important part of the problem.

### currentEnd

It represents the boundary of the current jump range.

When we reach this boundary, we must increase the jump count.

### farthest

It tracks the maximum position we can reach by considering all positions in the current range.

### jumps

It counts how many times we have expanded our reachable range.

### Important Condition

```cpp
if (i == currentEnd)
```

This condition tells us that we have explored all positions reachable using the current number of jumps.

Therefore, we must make another jump.

## 7. Connection With BFS

Jump Game II can also be understood as a BFS problem.

Think of every reachable index as a node.

From index `i`, we can reach all indices between:

```cpp
i + 1
```

and:

```cpp
i + nums[i]
```

BFS explores nodes level by level.

- Level 0: Starting index.
- Level 1: All positions reachable in one jump.
- Level 2: All positions reachable in two jumps.
- Level 3: All positions reachable in three jumps.

The first level that reaches the last index gives the minimum number of jumps.

The Greedy approach simulates these BFS levels without creating an explicit queue.

## 8. Time and Space Complexity

Let `n` be the number of elements.

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Recursion | Exponential | O(n) |
| DP Memoization | O(n²) | O(n) |
| Greedy | O(n) | O(1) |

### Greedy Complexity

**Time Complexity: `O(n)`**

We traverse the array only once.

**Space Complexity: `O(1)`**

We use only three variables:

- `jumps`
- `currentEnd`
- `farthest`

## 9. Important Interview Insights

### Q1. What is the difference between Jump Game and Jump Game II?

**Jump Game:**

We only need to determine whether the last index is reachable.

We maintain:

```cpp
maxReach
```

**Jump Game II:**

We need the minimum number of jumps.

We maintain:

```cpp
jumps
currentEnd
farthest
```

### Q2. Why do we stop the loop at `n - 1`?

Because once we reach the last index, no additional jump is required.

Therefore:

```cpp
for (int i = 0; i < n - 1; i++)
```

is sufficient.

### Q3. Why do we update farthest at every index?

Because different positions within the current range may offer different jump lengths.

We need the maximum reachable position among all of them.

### Q4. Why do we increase jumps when `i == currentEnd`?

Because we have finished exploring all positions reachable with the current number of jumps.

To move beyond this range, another jump is required.

### Q5. Why is Greedy optimal?

Each range represents all positions reachable with a particular number of jumps.

By extending the range as far as possible, we cover every position reachable using the current number of jumps.

This is equivalent to exploring BFS levels.

### Q6. Do we need a queue for BFS?

No.

We can simulate the BFS levels using:

- `currentEnd`
- `farthest`
- `jumps`

This avoids the additional queue.

## 10. Common Mistakes

- Increasing `jumps` at every index instead of at the end of a reachable range.
- Updating `currentEnd` using `i + nums[i]` instead of `farthest`.
- Forgetting to maintain the maximum reachable position.
- Traversing the entire array instead of stopping at the second-last index.
- Confusing the logic of Jump Game with Jump Game II.
- Thinking that the maximum jump length must always be selected immediately.
- Forgetting that we need the minimum number of jumps, not the maximum jump distance.

## 11. Comparison With Jump Game

| Concept | Jump Game | Jump Game II |
|---|---|---|
| LeetCode | 55 | 45 |
| Goal | Check reachability | Find minimum jumps |
| Main variable | maxReach | currentEnd, farthest |
| Pattern | Greedy | Greedy / BFS |
| Time | O(n) | O(n) |
| Space | O(1) | O(1) |

## 12. Key Takeaways

### Most Important Pattern

**Minimum jumps to reach destination → Greedy / BFS Range Expansion**

Remember:

```cpp
farthest = max(farthest, i + nums[i]);
```

And:

```cpp
if (i == currentEnd) {

    jumps++;

    currentEnd = farthest;
}
```

### Connection With Other NeetCode Problems

- Jump Game → Greedy / Maximum Reach.
- Jump Game II → Greedy / BFS Range Expansion.
- Gas Station → Greedy.
- Maximum Subarray → Kadane's Algorithm.
- Task Scheduler → Greedy + Frequency Counting.

## 13. Final Interview Revision

Remember these three concepts:

1. `farthest` tracks the maximum reachable position.
2. `currentEnd` represents the boundary of the current jump range.
3. Whenever `i == currentEnd`, increase the jump count and extend the range.

**Most Important Insight:**

Jump Game II is essentially BFS without an explicit queue.

Each jump represents one BFS level, and Greedy helps us calculate the farthest boundary of that level.

**Final Complexity:**

- Time: `O(n)`
- Space: `O(1)`

  # 134. Gas Station

- **Difficulty:** Medium
- **Pattern:** Greedy
- **Approaches:** Brute Force, Greedy
- **Language:** C++

## 1. Problem Understanding

We are given two integer arrays:

- `gas[i]` = Amount of gas available at station `i`.
- `cost[i]` = Amount of gas required to travel from station `i` to the next station.

There are `n` gas stations arranged in a circular route.

We start at one station with an empty fuel tank.

Our task is to find the starting station index from which we can complete the entire circular journey.

Return:

- The starting station index if completing the circuit is possible.
- `-1` if completing the circuit is impossible.

### Important Rules

1. The car starts with an empty tank.
2. The fuel tank has unlimited capacity.
3. We collect gas when we arrive at a station.
4. We spend fuel to travel to the next station.
5. The last station connects back to the first station.
6. At most one valid starting station exists.

### Example 1

**Input:**

```cpp
gas = {1, 2, 3, 4};
cost = {2, 2, 4, 1};
```

**Output:**

```cpp
3
```

**Explanation:**

Start at station 3.

Initial fuel:

```cpp
tank = 0;
```

At station 3:

```cpp
tank = 0 + 4 - 1;
tank = 3;
```

Travel to station 0.

At station 0:

```cpp
tank = 3 + 1 - 2;
tank = 2;
```

Travel to station 1.

At station 1:

```cpp
tank = 2 + 2 - 2;
tank = 2;
```

Travel to station 2.

At station 2:

```cpp
tank = 2 + 3 - 4;
tank = 1;
```

We successfully return to station 3.

Therefore:

```cpp
Output = 3;
```

## 2. Core Concept — Net Gas

Before understanding the approaches, calculate the net gas available at every station.

The formula is:

```cpp
netGas = gas[i] - cost[i];
```

There are two possibilities:

**Case 1: Positive Net Gas**

```cpp
gas[i] > cost[i]
```

We gain fuel after travelling to the next station.

**Case 2: Negative Net Gas**

```cpp
gas[i] < cost[i]
```

We lose fuel after travelling to the next station.

Example:

```cpp
gas  = {1, 2, 3, 4};
cost = {2, 2, 4, 1};
```

Net gas:

| Station | Gas | Cost | Net Gas |
|---|---:|---:|---:|
| 0 | 1 | 2 | -1 |
| 1 | 2 | 2 | 0 |
| 2 | 3 | 4 | -1 |
| 3 | 4 | 1 | 3 |

Net gas array:

```cpp
{-1, 0, -1, 3}
```

Total net gas:

```cpp
-1 + 0 - 1 + 3 = 1;
```

Since the total net gas is positive, completing the circuit is possible.

## 3. Brute Force Approach — Try Every Starting Station

### Idea

We try every station as a possible starting point.

For each starting station:

1. Start with an empty fuel tank.
2. Travel through the stations one by one.
3. Add the gas available at the current station.
4. Subtract the cost of travelling to the next station.
5. If fuel becomes negative, this starting point is invalid.
6. If we complete all `n` stations, return the starting index.

If no starting station works, return `-1`.

### C++ Code

```cpp
class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {

        int n = gas.size();

        for (int start = 0; start < n; start++) {

            int tank = 0;
            bool possible = true;

            for (int i = 0; i < n; i++) {

                int station = (start + i) % n;

                tank += gas[station] - cost[station];

                if (tank < 0) {
                    possible = false;
                    break;
                }
            }

            if (possible) {
                return start;
            }
        }

        return -1;
    }
};
```

### Complexity Analysis

Let `n` be the number of stations.

**Time Complexity:** `O(n²)`.

For every starting station, we may traverse all `n` stations.

**Space Complexity:** `O(1)`.

We use only a few variables.

### Limitation

We repeatedly simulate the same stations from different starting points.

We can optimize this using Greedy.

## 4. Optimized Approach — Greedy Algorithm

### Core Concept

We maintain two variables:

```cpp
int totalGas = 0;
int currentGas = 0;
```

**1. totalGas**

Stores the total net gas available across all stations.

**2. currentGas**

Stores the net gas accumulated from the current candidate starting station.

We also maintain:

```cpp
int start = 0;
```

This represents our current candidate starting station.

### Important Observation 1 — Total Gas Condition

If the total gas available is less than the total cost, completing the circuit is impossible.

Mathematically:

```cpp
sum(gas) < sum(cost)
```

Therefore:

```cpp
totalGas < 0
```

means we must return:

```cpp
-1;
```

Why?

Because no matter where we start, the car cannot generate enough fuel to cover the complete journey.

### Important Observation 2 — When Do We Reset the Starting Point?

Suppose we start at station `start`.

While travelling, we maintain:

```cpp
currentGas += gas[i] - cost[i];
```

If:

```cpp
currentGas < 0
```

we cannot reach the next station.

Therefore, the current starting point is invalid.

But there is an important greedy observation:

**None of the stations between the current starting point and the failed station can be a valid starting point.**

We can safely move the candidate starting point to:

```cpp
start = i + 1;
```

Then reset:

```cpp
currentGas = 0;
```

This is the key optimization.

### Why Can We Skip the Intermediate Stations?

Suppose we start at station 0 and fail at station 3.

That means the total net gas from station 0 through station 3 is negative.

If we had started at station 1, station 2, or station 3, the remaining segment would still have a negative total contribution because we removed an earlier prefix whose contribution was non-negative before the first failure.

Therefore, none of those stations can be a valid starting point.

We can skip them all.

## 5. Optimized C++ Solution

### Algorithm

1. Initialize `totalGas = 0`.
2. Initialize `currentGas = 0`.
3. Initialize `start = 0`.
4. Traverse all stations.
5. At each station:
   - Calculate net gas.
   - Add it to `totalGas`.
   - Add it to `currentGas`.
6. If `currentGas` becomes negative:
   - Set `start = i + 1`.
   - Reset `currentGas = 0`.
7. After traversing the array:
   - If `totalGas < 0`, return `-1`.
   - Otherwise, return `start`.

### C++ Code

```cpp
class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {

        int totalGas = 0;
        int currentGas = 0;

        int start = 0;

        for (int i = 0; i < gas.size(); i++) {

            int netGas = gas[i] - cost[i];

            totalGas += netGas;

            currentGas += netGas;

            // Current starting point is invalid
            if (currentGas < 0) {

                start = i + 1;

                currentGas = 0;
            }
        }

        // Total gas is insufficient
        if (totalGas < 0) {
            return -1;
        }

        return start;
    }
};
```

## 6. Dry Run — Step by Step

**Input:**

```cpp
gas  = {1, 2, 3, 4};
cost = {2, 2, 4, 1};
```

Initially:

```cpp
totalGas = 0;
currentGas = 0;
start = 0;
```

### Step 1: Index 0

```cpp
netGas = gas[0] - cost[0];

netGas = 1 - 2;

netGas = -1;
```

Update:

```cpp
totalGas = -1;
currentGas = -1;
```

Since:

```cpp
currentGas < 0
```

Reset:

```cpp
start = 1;
currentGas = 0;
```

### Step 2: Index 1

```cpp
netGas = 2 - 2;

netGas = 0;
```

Update:

```cpp
totalGas = -1;
currentGas = 0;
```

No reset is required.

### Step 3: Index 2

```cpp
netGas = 3 - 4;

netGas = -1;
```

Update:

```cpp
totalGas = -2;
currentGas = -1;
```

Since:

```cpp
currentGas < 0
```

Reset:

```cpp
start = 3;
currentGas = 0;
```

### Step 4: Index 3

```cpp
netGas = 4 - 1;

netGas = 3;
```

Update:

```cpp
totalGas = 1;
currentGas = 3;
```

No reset is required.

### Final Check

```cpp
totalGas = 1;
```

Since:

```cpp
totalGas >= 0
```

A valid starting station exists.

Return:

```cpp
start = 3;
```

**Final Output:**

```cpp
3
```

## 7. Dry Run — Impossible Case

**Input:**

```cpp
gas  = {2, 3, 4};
cost = {3, 4, 3};
```

Net gas:

```cpp
{-1, -1, 1}
```

Total gas:

```cpp
totalGas = -1 - 1 + 1;

totalGas = -1;
```

Since:

```cpp
totalGas < 0
```

We immediately know that completing the circuit is impossible.

**Output:**

```cpp
-1
```

## 8. Important Interview Insights

### Q1. Why do we calculate totalGas?

It tells us whether completing the entire circuit is possible at all.

If total gas is less than total cost, no starting station can work.

### Q2. Why do we reset currentGas when it becomes negative?

Because the current candidate starting station cannot complete the journey.

We discard that candidate and start checking from the next station.

### Q3. Why do we set start = i + 1?

Because none of the stations between the previous candidate start and the current failed station can be a valid starting point.

This is the main Greedy observation.

### Q4. Why don't we need to simulate the circular route?

The total gas condition and greedy reset logic are sufficient to identify a valid starting station.

We can solve the problem using one traversal.

### Q5. What if totalGas is exactly zero?

A valid solution can still exist.

For example:

```cpp
gas  = {1, 2, 3};
cost = {2, 2, 2};
```

Total gas:

```cpp
6
```

Total cost:

```cpp
6
```

A valid starting station exists.

Therefore, the failure condition must be:

```cpp
totalGas < 0
```

not:

```cpp
totalGas <= 0
```

### Q6. Why is there at most one valid starting station?

The problem guarantees that at most one solution exists.

The greedy approach identifies that candidate when a solution exists.

## 9. Common Mistakes

- Checking only whether gas at each station is greater than its cost.
- Forgetting to check the total gas condition.
- Resetting `totalGas` instead of `currentGas`.
- Forgetting to update `start = i + 1`.
- Using `totalGas <= 0` instead of `totalGas < 0`.
- Trying to simulate every circular route unnecessarily.
- Confusing the fuel available at a station with the net fuel after travelling to the next station.

## 10. Comparison of Approaches

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Greedy | O(n) | O(1) |

## 11. Key Takeaways

### Most Important Pattern

**Circular Route + Fuel Balance → Greedy**

Remember these three concepts:

1. Calculate the net gas at every station.
2. If total gas is less than total cost, return `-1`.
3. If current gas becomes negative, discard the current candidate and start from the next station.

### Important Code

```cpp
currentGas += gas[i] - cost[i];

if (currentGas < 0) {

    start = i + 1;

    currentGas = 0;
}
```

And:

```cpp
if (totalGas < 0) {
    return -1;
}
```

### Connection With Other NeetCode Problems

- Maximum Subarray → Kadane's Algorithm.
- Jump Game → Greedy / Maximum Reach.
- Jump Game II → Greedy / BFS Range Expansion.
- Gas Station → Greedy / Prefix Sum / Circular Route.
- Task Scheduler → Greedy + Frequency Counting.

## 12. Final Interview Revision

Remember:

- `totalGas` checks whether a complete circuit is possible.
- `currentGas` checks whether the current candidate can reach the next station.
- `start` stores the candidate starting station.

**Most Important Insight:**

Whenever the accumulated gas becomes negative, the current starting point and all stations between it and the failure point can be eliminated.

This allows us to find the answer in one traversal.

**Final Complexity:**

- Time: `O(n)`
- Space: `O(1)`

  # 846. Hand of Straights

- **Difficulty:** Medium
- **Pattern:** Greedy / Hash Map / Sorting
- **Data Structures:** Ordered Map
- **Approaches:** Brute Force, Greedy with Ordered Map
- **Language:** C++

## 1. Problem Understanding

We are given:

- An integer array `hand`, representing cards.
- An integer `groupSize`, representing the required size of each group.

We need to rearrange the cards into groups such that:

1. Every group contains exactly `groupSize` cards.
2. Cards in each group have consecutive values, increasing by 1.
3. Every card must belong to exactly one group.

Return:

- `true` if we can form all groups.
- `false` otherwise.

### Important Observation

The total number of cards must be divisible by `groupSize`.

If:

```cpp
hand.size() % groupSize != 0
```

then forming equal-sized groups is impossible.

### Example 1

**Input:**

```cpp
hand = {1, 2, 4, 2, 3, 5, 3, 4};
groupSize = 4;
```

**Output:**

```cpp
true
```

**Explanation:**

We can form these groups:

```text
Group 1: [1, 2, 3, 4]
Group 2: [2, 3, 4, 5]
```

Every group contains four consecutive cards.

Therefore, the answer is `true`.

### Example 2

**Input:**

```cpp
hand = {1, 2, 3, 3, 4, 5, 6, 7};
groupSize = 4;
```

**Output:**

```cpp
false
```

**Explanation:**

The closest possible arrangement is:

```text
Group 1: [1, 2, 3, 4]
Group 2: [3, 5, 6, 7]
```

The second group is not consecutive because card 4 is missing.

Therefore, the answer is `false`.

## 2. Core Concept — Greedy

### Why Greedy?

We should always start forming a group using the smallest available card.

Suppose the available cards are:

```cpp
hand = {1, 2, 2, 3, 3, 4, 5};
```

And:

```cpp
groupSize = 3;
```

The smallest available card is 1.

We must form a group:

```text
[1, 2, 3]
```

After using these cards, the remaining cards are:

```text
[2, 3, 4, 5]
```

Now the smallest available card is 2.

We form:

```text
[2, 3, 4]
```

Remaining:

```text
[5]
```

We cannot form another group of size 3.

Therefore, the answer is `false`.

### Important Greedy Rule

Always start a new group from the smallest card that still has a positive frequency.

Why?

Because every smaller card must be used somewhere, and it cannot be placed after a larger card in a consecutive increasing group.

Starting with the smallest card prevents us from skipping a card that must be used.

## 3. Approach 1 — Brute Force

### Idea

We can sort the cards and try to construct consecutive groups.

For every group:

1. Find the smallest unused card.
2. Try to find the next `groupSize - 1` consecutive cards.
3. Mark the selected cards as used.
4. Repeat until all cards are processed.

A straightforward implementation uses sorting and a visited array.

### C++ Code

```cpp
class Solution {
public:
    bool isNStraightHand(vector<int>& hand, int groupSize) {

        int n = hand.size();

        if (n % groupSize != 0) {
            return false;
        }

        sort(hand.begin(), hand.end());

        vector<bool> used(n, false);

        for (int i = 0; i < n; i++) {

            if (used[i]) {
                continue;
            }

            int current = hand[i];

            used[i] = true;

            for (int next = 1; next < groupSize; next++) {

                bool found = false;

                for (int j = i + 1; j < n; j++) {

                    if (!used[j] && hand[j] == current + next) {

                        used[j] = true;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    return false;
                }
            }
        }

        return true;
    }
};
```

### Complexity Analysis

Let `n` be the number of cards.

- Sorting: `O(n log n)`.
- Searching for each required card: potentially `O(n)` per card.
- Across all groups, the nested search can take `O(n²)`.

**Time Complexity:** `O(n²)`.

**Space Complexity:** `O(n)` for the visited array, excluding sorting implementation details.

### Limitation

We repeatedly scan the array to find the next required card.

We can improve this using a frequency map.

## 4. Optimized Approach — Greedy + Ordered Map

### Core Concept

We use:

```cpp
map<int, int> freq;
```

This stores:

```text
Card Value -> Frequency
```

Example:

```cpp
hand = {1, 2, 2, 3, 3, 4};
```

Frequency map:

| Card | Frequency |
|---|---:|
| 1 | 1 |
| 2 | 2 |
| 3 | 2 |
| 4 | 1 |

Why use `map` instead of `unordered_map`?

Because `map` automatically keeps keys sorted in ascending order.

Therefore:

```cpp
freq.begin()->first
```

always gives us the smallest available card.

### Algorithm

1. Check whether the number of cards is divisible by `groupSize`.
2. Count the frequency of every card using an ordered map.
3. While the map is not empty:
   - Get the smallest available card.
   - Get its frequency.
   - Try to form a group starting from that card.
4. For every required consecutive card:
   - Check whether it exists.
   - Check whether its frequency is sufficient.
   - Decrease its frequency by the starting card's frequency.
   - Remove it from the map if its frequency becomes zero.
5. If any required card is missing or has insufficient frequency, return `false`.
6. If all cards are used successfully, return `true`.

## 5. Optimized C++ Solution

```cpp
class Solution {
public:
    bool isNStraightHand(vector<int>& hand, int groupSize) {

        int n = hand.size();

        // Total cards must be divisible by groupSize
        if (n % groupSize != 0) {
            return false;
        }

        // Store card frequencies in sorted order
        map<int, int> freq;

        for (int card : hand) {
            freq[card]++;
        }

        // Process cards until the map becomes empty
        while (!freq.empty()) {

            // Smallest available card
            int start = freq.begin()->first;

            // Number of groups starting with this card
            int count = freq.begin()->second;

            // Form consecutive groups
            for (int i = 0; i < groupSize; i++) {

                int card = start + i;

                // Required card must exist
                if (freq.find(card) == freq.end()) {
                    return false;
                }

                // Required card must have enough frequency
                if (freq[card] < count) {
                    return false;
                }

                // Consume the required cards
                freq[card] -= count;

                // Remove card when frequency becomes zero
                if (freq[card] == 0) {
                    freq.erase(card);
                }
            }
        }

        return true;
    }
};
```

## 6. Understanding Why We Subtract count

Consider:

```cpp
hand = {1, 2, 2, 3, 3, 4};
groupSize = 3;
```

Frequency map:

```text
1 -> 1
2 -> 2
3 -> 2
4 -> 1
```

The smallest card is 1.

Its frequency is:

```cpp
count = 1;
```

We must form one group:

```text
[1, 2, 3]
```

Update frequencies:

```text
1 -> 0
2 -> 1
3 -> 1
4 -> 1
```

Remove card 1 because its frequency is zero.

Now the smallest available card is 2.

Its frequency is:

```cpp
count = 1;
```

We form:

```text
[2, 3, 4]
```

All remaining frequencies become zero.

The map becomes empty.

Therefore:

```cpp
return true;
```

### Why Do We Subtract the Entire Frequency?

Suppose the smallest card has frequency 3.

For example:

```text
1 -> 3
2 -> 3
3 -> 3
```

We need three groups:

```text
[1, 2, 3]
[1, 2, 3]
[1, 2, 3]
```

Therefore, we must consume three copies of every required card.

That is why we write:

```cpp
freq[card] -= count;
```

Instead of:

```cpp
freq[card]--;
```

## 7. Dry Run — Step by Step

**Input:**

```cpp
hand = {1, 2, 4, 2, 3, 5, 3, 4};

groupSize = 4;
```

### Step 1: Frequency Map

After counting:

```text
1 -> 1
2 -> 2
3 -> 2
4 -> 2
5 -> 1
```

### Step 2: First Group

Smallest available card:

```cpp
start = 1;
```

Its frequency:

```cpp
count = 1;
```

Required group:

```text
[1, 2, 3, 4]
```

Update frequencies:

| Card | Before | After |
|---|---:|---:|
| 1 | 1 | 0 |
| 2 | 2 | 1 |
| 3 | 2 | 1 |
| 4 | 2 | 1 |
| 5 | 1 | 1 |

Remove card 1.

Remaining map:

```text
2 -> 1
3 -> 1
4 -> 1
5 -> 1
```

### Step 3: Second Group

Smallest available card:

```cpp
start = 2;
```

Its frequency:

```cpp
count = 1;
```

Required group:

```text
[2, 3, 4, 5]
```

Update frequencies:

| Card | Before | After |
|---|---:|---:|
| 2 | 1 | 0 |
| 3 | 1 | 0 |
| 4 | 1 | 0 |
| 5 | 1 | 0 |

All cards are removed from the map.

### Final Result

```cpp
return true;
```

**Output:**

```cpp
true
```

## 8. Dry Run — Impossible Case

**Input:**

```cpp
hand = {1, 2, 3, 3, 4, 5, 6, 7};

groupSize = 4;
```

Frequency map:

```text
1 -> 1
2 -> 1
3 -> 2
4 -> 1
5 -> 1
6 -> 1
7 -> 1
```

Start with the smallest card:

```cpp
start = 1;
```

First group:

```text
[1, 2, 3, 4]
```

Remaining:

```text
3 -> 1
5 -> 1
6 -> 1
7 -> 1
```

Now the smallest available card is 3.

We need:

```text
[3, 4, 5, 6]
```

But card 4 is no longer available.

Therefore:

```cpp
return false;
```

**Output:**

```cpp
false
```

## 9. Time and Space Complexity

Let:

- `n` = Number of cards.
- `k` = `groupSize`.

### Frequency Counting

Inserting `n` cards into an ordered map:

```cpp
O(n log n)
```

### Group Formation

Every card is consumed exactly once.

For every required card, we perform map lookup, frequency update and possibly deletion.

Each map operation takes:

```cpp
O(log n)
```

Across all cards, this takes:

```cpp
O(n log n)
```

### Overall Complexity

**Time Complexity: `O(n log n)`**

**Space Complexity: `O(n)`**

The map can store up to `n` distinct card values.

## 10. Important Interview Insights

### Q1. Why do we always start with the smallest available card?

Because every card must belong to a consecutive increasing group.

The smallest available card cannot be placed after a larger card.

Therefore, it must be the starting point of its group.

### Q2. Why do we use an ordered map?

An ordered map automatically sorts card values.

We can always retrieve the smallest available card using:

```cpp
freq.begin()->first;
```

### Q3. Why do we subtract `count` instead of 1?

If the smallest card appears multiple times, we must create multiple groups starting with that card.

Every consecutive card must have enough copies to complete all those groups.

### Q4. Why do we erase cards when their frequency becomes zero?

Because those cards have been completely used.

Removing them ensures that `freq.begin()` always returns the smallest card that is still available.

### Q5. Why do we check divisibility first?

Every group must contain exactly `groupSize` cards.

If the total number of cards cannot be divided equally into groups, the answer must be `false`.

### Q6. Can we use unordered_map?

Yes, but an `unordered_map` does not maintain sorted keys.

We would need an additional Min Heap or sorting step to find the smallest available card efficiently.

An ordered map is simpler for this problem.

## 11. Common Mistakes

- Forgetting the divisibility check.
- Starting groups from arbitrary cards instead of the smallest available card.
- Decreasing frequency by 1 instead of by `count`.
- Forgetting to remove cards whose frequency becomes zero.
- Assuming that having enough total cards guarantees consecutive groups.
- Using `unordered_map` and assuming its keys are sorted.
- Checking only whether a required card exists without checking its frequency.

## 12. Comparison of Approaches

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Brute Force | O(n²) | O(n) |
| Greedy + Ordered Map | O(n log n) | O(n) |

## 13. Key Takeaways

### Most Important Pattern

**Consecutive Groups + Smallest Available Element → Greedy + Ordered Map**

Remember:

1. Count the frequency of every card.
2. Always start with the smallest available card.
3. Consume the required consecutive cards.
4. Remove cards when their frequency becomes zero.

### Important Code

```cpp
int start = freq.begin()->first;

int count = freq.begin()->second;
```

And:

```cpp
freq[card] -= count;
```

### Connection With Other NeetCode Problems

- Task Scheduler → Greedy + Frequency Counting.
- Hand of Straights → Greedy + Ordered Map.
- Merge Triplets to Form Target Triplet → Greedy.
- Gas Station → Greedy + Running Sum.
- Jump Game II → Greedy + Range Expansion.

## 14. Final Interview Revision

Remember these three concepts:

1. The smallest available card must start the next group.
2. Its frequency determines how many groups must be formed.
3. Every consecutive card must have at least that frequency.

**Most Important Insight:**

Instead of trying to arrange cards in every possible way, always consume the smallest available card and form the required consecutive groups.

This greedy strategy avoids unnecessary combinations.

**Final Complexity:**

- Time: `O(n log n)`
- Space: `O(n)`

  # 1899. Merge Triplets to Form Target Triplet

- **Difficulty:** Medium
- **Pattern:** Greedy
- **Data Structures:** Arrays / Vectors
- **Approaches:** Brute Force, Greedy
- **Language:** C++

## 1. Problem Understanding

We are given:

- A 2D array `triplets`, where each element contains three integers: `[a, b, c]`.
- A target array containing three integers: `[x, y, z]`.

We can choose two different triplets and merge them using the following operation:

```cpp
triplets[j] = {
    max(triplets[i][0], triplets[j][0]),
    max(triplets[i][1], triplets[j][1]),
    max(triplets[i][2], triplets[j][2])
};
```

In other words, we take the maximum value at each corresponding position.

We can perform this operation zero or more times.

Our task is to determine whether we can obtain the target triplet as an element of the array.

Return:

- `true` if the target triplet can be formed.
- `false` otherwise.

### Example 1

**Input:**

```cpp
triplets = {{1, 2, 3}, {7, 1, 1}};
target = {7, 2, 3};
```

**Output:**

```cpp
true
```

**Explanation:**

Merge:

```text
[1, 2, 3]
[7, 1, 1]
```

Taking the maximum at every position:

```text
[max(1, 7), max(2, 1), max(3, 1)]
```

Result:

```text
[7, 2, 3]
```

This matches the target.

### Example 2

**Input:**

```cpp
triplets = {{2, 5, 3}, {1, 8, 4}, {1, 7, 5}};
target = {2, 7, 5};
```

**Output:**

```cpp
true
```

**Explanation:**

The second triplet:

```text
[1, 8, 4]
```

must be ignored because its second value `8` is greater than the target value `7`.

The other two triplets are valid:

```text
[2, 5, 3]
[1, 7, 5]
```

Merge them:

```text
[max(2, 1), max(5, 7), max(3, 5)]
```

Result:

```text
[2, 7, 5]
```

This matches the target.

## 2. Core Concept — Greedy

### Important Observation 1: Values Cannot Decrease

The merge operation always takes the maximum value.

For example:

```text
max(3, 7) = 7
```

Once a value becomes 7, merging another triplet cannot reduce it to 5.

This gives us an important rule:

**If any value in a triplet is greater than the corresponding target value, that triplet cannot be used to form the target.**

### Important Observation 2: Ignore Invalid Triplets

Suppose:

```cpp
target = {2, 7, 5};
```

Consider these triplets:

```text
[2, 5, 3]  -> Valid
[1, 8, 4]  -> Invalid
[1, 7, 5]  -> Valid
[3, 2, 4]  -> Invalid
```

Why are the invalid triplets ignored?

Because:

```text
8 > 7
```

and:

```text
3 > 2
```

The maximum operation cannot reduce these values.

Therefore, including either invalid triplet would make it impossible to obtain the exact target.

### Important Observation 3: We Only Need to Track Three Maximum Values

After filtering invalid triplets, we need to check whether we can obtain:

```cpp
target[0]
target[1]
target[2]
```

We can maintain three boolean variables:

```cpp
bool first = false;
bool second = false;
bool third = false;
```

Each variable tells us whether the corresponding target value has been found in a valid triplet.

If all three become `true`, the target can be formed.

## 3. Brute Force Approach — Try Combinations

### Idea

We can consider different subsets of triplets.

For each subset:

1. Merge the selected triplets using the maximum operation.
2. Calculate the resulting triplet.
3. Compare it with the target.
4. If any subset produces the target, return `true`.

However, with `n` triplets, there can be up to:

```cpp
2^n
```

possible subsets.

This approach is inefficient and unnecessary.

### Complexity Analysis

**Time Complexity:** Exponential, approximately `O(2^n)` subset exploration.

**Space Complexity:** `O(n)` for storing or tracking a selected subset, depending on implementation.

### Limitation

We do not need to explore every possible combination.

The maximum operation gives us a much simpler Greedy solution.

## 4. Optimized Approach — Greedy

### Core Concept

We process every triplet and ignore those that contain a value greater than the corresponding target value.

For every valid triplet, we check whether it contributes any target value.

We maintain:

```cpp
bool first = false;
bool second = false;
bool third = false;
```

### Algorithm

1. Initialize all three boolean variables to `false`.
2. Traverse every triplet.
3. Check whether any component exceeds the corresponding target component.
4. If the triplet is invalid, skip it.
5. Otherwise:
   - Check whether its first value matches `target[0]`.
   - Check whether its second value matches `target[1]`.
   - Check whether its third value matches `target[2]`.
6. If all three values have been found, return `true`.
7. If traversal finishes without finding all three, return `false`.

## 5. Optimized C++ Solution

```cpp
class Solution {
public:
    bool mergeTriplets(vector<vector<int>>& triplets, vector<int>& target) {

        bool first = false;
        bool second = false;
        bool third = false;

        for (auto& t : triplets) {

            // Ignore triplets that exceed the target
            if (t[0] > target[0] ||
                t[1] > target[1] ||
                t[2] > target[2]) {

                continue;
            }

            // Check which target values are available
            if (t[0] == target[0]) {
                first = true;
            }

            if (t[1] == target[1]) {
                second = true;
            }

            if (t[2] == target[2]) {
                third = true;
            }
        }

        return first && second && third;
    }
};
```

## 6. Understanding the Code

### Step 1: Initialize Boolean Variables

```cpp
bool first = false;
bool second = false;
bool third = false;
```

These variables track whether we have found each target component in a valid triplet.

### Step 2: Traverse All Triplets

```cpp
for (auto& t : triplets)
```

We process each triplet one by one.

### Step 3: Filter Invalid Triplets

```cpp
if (t[0] > target[0] ||
    t[1] > target[1] ||
    t[2] > target[2]) {

    continue;
}
```

If even one value exceeds its corresponding target value, we skip the entire triplet.

### Step 4: Find Matching Components

```cpp
if (t[0] == target[0]) {
    first = true;
}
```

We repeat the same check for the second and third components.

Notice that the three conditions are independent.

Different valid triplets can provide different target components.

### Step 5: Check All Three Conditions

```cpp
return first && second && third;
```

All three components must be available.

If even one is missing, the target cannot be formed.

## 7. Dry Run — Step by Step

**Input:**

```cpp
triplets = {
    {1, 2, 3},
    {7, 1, 1}
};

target = {7, 2, 3};
```

Initially:

```cpp
first = false;
second = false;
third = false;
```

### Step 1: Process First Triplet

```cpp
t = {1, 2, 3};
```

Compare with target:

```text
[1, 2, 3]
[7, 2, 3]
```

Check whether any value exceeds the target:

```cpp
1 > 7  // false
2 > 2  // false
3 > 3  // false
```

The triplet is valid.

Now check matching components:

```cpp
1 == 7  // false
2 == 2  // true
3 == 3  // true
```

Update:

```cpp
first = false;
second = true;
third = true;
```

### Step 2: Process Second Triplet

```cpp
t = {7, 1, 1};
```

Compare with target:

```text
[7, 1, 1]
[7, 2, 3]
```

The triplet does not exceed any target component.

It is valid.

Check matching components:

```cpp
7 == 7  // true
1 == 2  // false
1 == 3  // false
```

Update:

```cpp
first = true;
second = true;
third = true;
```

### Step 3: Final Check

```cpp
return first && second && third;
```

Since all three variables are `true`:

```cpp
true && true && true
```

**Final Output:**

```cpp
true
```

## 8. Dry Run — Invalid Triplet

**Input:**

```cpp
triplets = {
    {2, 5, 3},
    {1, 8, 4},
    {1, 7, 5}
};

target = {2, 7, 5};
```

### Step 1: First Triplet

```cpp
{2, 5, 3}
```

It does not exceed the target.

It provides:

```cpp
first = true;
```

### Step 2: Second Triplet

```cpp
{1, 8, 4}
```

Check:

```cpp
8 > 7
```

This triplet is invalid.

We skip it using:

```cpp
continue;
```

### Step 3: Third Triplet

```cpp
{1, 7, 5}
```

It is valid.

It provides:

```cpp
second = true;
third = true;
```

### Final State

```cpp
first = true;
second = true;
third = true;
```

**Output:**

```cpp
true
```

The target can be formed by merging:

```text
[2, 5, 3]
[1, 7, 5]
```

Result:

```text
[2, 7, 5]
```

## 9. Time and Space Complexity

Let:

- `n` = Number of triplets.
- Each triplet contains exactly three values.

### Time Complexity

We traverse the triplets once.

For each triplet, we perform a constant number of comparisons.

Therefore:

**Time Complexity: `O(n)`**

### Space Complexity

We use only three boolean variables.

Therefore:

**Space Complexity: `O(1)`**

## 10. Important Interview Insights

### Q1. Why do we ignore triplets containing values greater than the target?

Because the merge operation uses `max()`.

Values can only stay the same or increase. They can never decrease.

Therefore, a value greater than the target can never be corrected.

### Q2. Do all three target values need to come from the same triplet?

No.

Different valid triplets can contribute different target components.

For example:

```text
[7, 1, 1]
[1, 2, 3]
```

Together, they can form:

```text
[7, 2, 3]
```

### Q3. Why do we use three independent boolean variables?

Because each target component can be obtained from a different valid triplet.

We only need to know whether every required component has been found.

### Q4. Why do we use `continue`?

If a triplet exceeds any target component, it cannot participate in a valid merge.

`continue` skips the remaining checks for that triplet and moves to the next one.

### Q5. Why don't we actually perform the merge operation?

Because the maximum operation is associative and can combine information from multiple valid triplets.

We only need to verify that all three target components can be obtained without exceeding the target.

### Q6. What if all three target values are found in one triplet?

Then that triplet already equals the target.

We can return `true`.

### Q7. Can a triplet with a smaller value still be useful?

Yes.

For example:

```text
target = [7, 2, 3]
triplet = [1, 2, 3]
```

It is useful because it contributes the second and third target values.

The first value can come from another valid triplet.

## 11. Common Mistakes

- Using triplets that contain values greater than the target.
- Assuming all three target values must come from one triplet.
- Using `&&` instead of `||` in the invalid-triplet condition.
- Forgetting that different triplets can contribute different components.
- Comparing the entire triplet with the target instead of checking individual components.
- Performing unnecessary sorting or nested loops.
- Returning `true` after finding only one or two target components.

## 12. Comparison of Approaches

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Brute Force | Exponential | Depends on subset tracking |
| Greedy | O(n) | O(1) |

## 13. Key Takeaways

### Most Important Pattern

**Merge Using Maximum + Exact Target → Greedy Filtering**

Remember these three steps:

1. Ignore every triplet that exceeds any target component.
2. Track whether each target component is available in the remaining triplets.
3. Return `true` only when all three components are found.

### Important Code

Invalid triplet condition:

```cpp
if (t[0] > target[0] ||
    t[1] > target[1] ||
    t[2] > target[2]) {

    continue;
}
```

Final condition:

```cpp
return first && second && third;
```

### Connection With Other NeetCode Problems

- Hand of Straights → Greedy + Ordered Map.
- Gas Station → Greedy + Running Sum.
- Jump Game → Greedy + Maximum Reach.
- Merge Triplets → Greedy + Filtering.
- Partition Labels → Greedy + Last Occurrence.

## 14. Final Interview Revision

Remember:

- The merge operation uses the maximum value at each position.
- Values cannot decrease after merging.
- Any triplet exceeding the target must be ignored.
- The three target components can come from different valid triplets.

**Most Important Insight:**

We do not need to simulate the merge operations.

We only need to identify valid triplets and check whether they collectively provide all three target values.

**Final Complexity:**

- Time: `O(n)`
- Space: `O(1)`

  # 763. Partition Labels

- **Difficulty:** Medium
- **Pattern:** Greedy
- **Data Structures:** Array / Hashing
- **Approaches:** Brute Force, Greedy
- **Language:** C++

## 1. Problem Understanding

We are given a string `s` containing lowercase English letters.

We need to split the string into as many substrings as possible while satisfying this condition:

**Each character must appear in at most one substring.**

In other words, if a character appears in one partition, all its occurrences must belong to that same partition.

Return a vector containing the lengths of all partitions in their original order.

### Example 1

**Input:**

```cpp
s = "xyxxyzbzb...";
```

The screenshot shows the following output:

```cpp
[5, 5, 1, 1, 1]
```

### Example 2

**Input:**

```cpp
s = "abcabc";
```

**Output:**

```cpp
[6]
```

**Explanation:**

Characters `a`, `b`, and `c` all appear again later in the string.

Therefore, we cannot split the string into smaller valid partitions.

The entire string becomes one partition:

```text
"abcabc"
```

Its length is 6.

## 2. Core Concept — Greedy + Last Occurrence

### Important Observation

A character must belong to exactly one partition.

Therefore, whenever we include a character in a partition, we must include all its future occurrences before ending that partition.

For example:

```cpp
s = "abac";
```

Character positions:

```text
Index:      0  1  2  3
Character:  a  b  a  c
```

Last occurrences:

```text
a -> 2
b -> 1
c -> 3
```

We cannot end the first partition at index 0 because character `a` appears again at index 2.

Therefore, the first partition must extend at least to index 2.

We then encounter `c` at index 3, so the complete partition becomes:

```text
"abac"
```

Output:

```cpp
[4]
```

### Main Greedy Idea

We maintain two variables:

```cpp
int start = 0;
int end = 0;
```

- `start` represents the beginning of the current partition.
- `end` represents the farthest last occurrence that the current partition must include.

For every character:

1. Find its last occurrence.
2. Update `end` if necessary.
3. If the current index reaches `end`, the partition is complete.
4. Calculate its length and start a new partition.

## 3. Approach 1 — Brute Force

### Idea

We can try different ways to divide the string into substrings.

For every possible partition:

1. Select a position where the current substring ends.
2. Check whether every character in that substring appears only inside that substring.
3. If the partition is valid, recursively process the remaining string.
4. Compare the number of valid partitions.
5. Choose the division that produces the maximum number of partitions.

### Limitation

There are many possible ways to divide a string.

For a string of length `n`, there are:

```cpp
2^(n - 1)
```

possible ways to place partition boundaries.

Checking all possibilities is inefficient.

We can solve the problem in one traversal using Greedy.

## 4. Optimized Approach — Greedy + Last Occurrence Array

### Step 1: Store the Last Occurrence

Create an array:

```cpp
vector<int> last(26, 0);
```

Since the string contains only lowercase English letters, we need 26 positions.

For every character, store its last index.

```cpp
last[s[i] - 'a'] = i;
```

For example:

```cpp
s = "abac";
```

Last occurrence information:

| Character | Last Index |
|---|---:|
| a | 2 |
| b | 1 |
| c | 3 |

### Step 2: Traverse the String

Initialize:

```cpp
int start = 0;
int end = 0;
```

Traverse the string.

At every index:

```cpp
end = max(end, last[s[i] - 'a']);
```

This ensures that the current partition includes the last occurrence of every character encountered so far.

### Step 3: Identify Partition Boundaries

When:

```cpp
i == end
```

we know that every character in the current partition has had all its occurrences included.

Therefore, we can safely close the partition.

Calculate its length:

```cpp
end - start + 1
```

Add it to the answer:

```cpp
result.push_back(end - start + 1);
```

Move the starting position:

```cpp
start = i + 1;
```

## 5. Optimized C++ Solution

```cpp
class Solution {
public:
    vector<int> partitionLabels(string s) {

        vector<int> last(26, 0);

        // Store the last occurrence of each character
        for (int i = 0; i < s.size(); i++) {
            last[s[i] - 'a'] = i;
        }

        vector<int> result;

        int start = 0;
        int end = 0;

        // Traverse the string
        for (int i = 0; i < s.size(); i++) {

            // Extend the current partition
            end = max(end, last[s[i] - 'a']);

            // Current partition is complete
            if (i == end) {

                result.push_back(end - start + 1);

                start = i + 1;
            }
        }

        return result;
    }
};
```

## 6. Understanding the Code

### Last Occurrence Array

```cpp
vector<int> last(26, 0);
```

Creates an array to store the last index of every lowercase English letter.

### Character-to-Index Mapping

```cpp
last[s[i] - 'a'] = i;
```

We convert a character into an array index.

For example:

```cpp
'a' - 'a' = 0
'b' - 'a' = 1
'c' - 'a' = 2
```

This allows us to access the corresponding last occurrence in constant time.

### Update the Partition Boundary

```cpp
end = max(end, last[s[i] - 'a']);
```

We update the partition boundary using the last occurrence of the current character.

We use `max()` because a previously encountered character may require the partition to extend farther.

### Check Whether the Partition Is Complete

```cpp
if (i == end)
```

If the current index reaches the required boundary, every character in the current partition has been completely included.

We can safely close the partition.

### Calculate Partition Length

```cpp
result.push_back(end - start + 1);
```

The `+1` is necessary because both the starting and ending indices are included.

## 7. Dry Run — Step by Step

**Input:**

```cpp
s = "ababcbacadefegdehijhklij";
```

### Step 1: Find Last Occurrences

| Character | Last Occurrence |
|---|---:|
| a | 8 |
| b | 5 |
| c | 7 |
| d | 14 |
| e | 15 |
| f | 11 |
| g | 13 |
| h | 19 |
| i | 22 |
| j | 23 |
| k | 20 |
| l | 21 |

### Step 2: Initialize Variables

```cpp
start = 0;
end = 0;
```

### Step 3: Process the String

The current partition boundary keeps expanding whenever we encounter a character whose last occurrence is farther away.

| Index | Character | Last Occurrence | Updated end | Partition Completed? |
|---:|:---:|---:|---:|:---:|
| 0 | a | 8 | 8 | No |
| 1 | b | 5 | 8 | No |
| 2 | a | 8 | 8 | No |
| 3 | b | 5 | 8 | No |
| 4 | c | 7 | 8 | No |
| 5 | b | 5 | 8 | No |
| 6 | a | 8 | 8 | No |
| 7 | c | 7 | 8 | No |
| 8 | a | 8 | 8 | Yes |

At index 8:

```cpp
i == end;
```

We complete the first partition.

Length:

```cpp
8 - 0 + 1 = 9;
```

Result:

```cpp
[9]
```

Update:

```cpp
start = 9;
```

### Step 4: Second Partition

Continue traversing from index 9.

The partition boundary expands to index 15.

At index 15:

```cpp
i == end;
```

The second partition is complete.

Length:

```cpp
15 - 9 + 1 = 7;
```

Result:

```cpp
[9, 7]
```

Update:

```cpp
start = 16;
```

### Step 5: Third Partition

Continue traversing from index 16.

The partition boundary expands to index 23.

At index 23:

```cpp
i == end;
```

The third partition is complete.

Length:

```cpp
23 - 16 + 1 = 8;
```

Final result:

```cpp
[9, 7, 8]
```

**Output:**

```cpp
[9, 7, 8]
```

## 8. Why Does Greedy Work?

The most important observation is:

**A partition cannot end before the last occurrence of any character inside it.**

Suppose the current partition starts at index 0.

If the first character appears again at index 8, the partition must extend at least to index 8.

While traversing these positions, we may discover another character whose last occurrence is even farther away.

Therefore, we continuously extend the boundary.

When:

```cpp
i == end
```

we know that no character inside the partition appears after this index.

We can safely close it.

This produces the maximum possible number of valid partitions because we close each partition at the earliest valid position.

## 9. Time and Space Complexity

Let `n` be the length of the string.

### Time Complexity

We traverse the string twice:

1. First traversal: calculate last occurrences.
2. Second traversal: create partitions.

Each traversal takes `O(n)`.

Therefore:

**Time Complexity: O(n)**

### Space Complexity

We use:

- A fixed-size array of 26 elements.
- A result vector containing at most `n` partition lengths.

The auxiliary space excluding the output is:

**O(1)**

Including the output vector:

**O(n)**

## 10. Important Interview Insights

### Q1. Why do we store the last occurrence instead of the first occurrence?

Because we must include every occurrence of a character in the same partition.

The last occurrence tells us the minimum position up to which the current partition must extend.

### Q2. Why do we use `max()` to update end?

Different characters may require different partition boundaries.

We must include the farthest last occurrence among all characters encountered so far.

### Q3. Why do we check `i == end`?

It means we have reached the farthest required position for all characters in the current partition.

No character in this partition appears later.

Therefore, we can safely close it.

### Q4. Why do we use `end - start + 1`?

Because string indices are zero-based and both endpoints are included.

For example:

```cpp
start = 0;
end = 4;
```

Length:

```cpp
4 - 0 + 1 = 5;
```

### Q5. Can the same character appear in multiple partitions?

No.

Every occurrence of a character must belong to the same partition.

### Q6. Why is this Greedy?

We always close the current partition at the earliest valid position.

Waiting longer would not help create more partitions.

### Q7. Why is the last occurrence array of size 26?

The problem guarantees that the string contains lowercase English letters only.

There are exactly 26 lowercase English letters.

## 11. Common Mistakes

- Storing the first occurrence instead of the last occurrence.
- Updating `end` using assignment instead of `max()`.
- Closing a partition before reaching its required last occurrence.
- Forgetting to reset `start` after completing a partition.
- Calculating the partition length without adding 1.
- Assuming that every character can be separated into its own partition.
- Using nested loops unnecessarily.

## 12. Comparison of Approaches

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Brute Force | Exponential | Depends on recursion |
| Greedy | O(n) | O(1) auxiliary space |

## 13. Key Takeaways

### Most Important Pattern

**String Partitioning + Character Last Occurrence → Greedy**

Remember these three steps:

1. Store the last occurrence of every character.
2. Extend the current partition boundary using the maximum last occurrence.
3. When the current index reaches the boundary, close the partition.

### Important Code

Last occurrence:

```cpp
last[s[i] - 'a'] = i;
```

Extend partition:

```cpp
end = max(end, last[s[i] - 'a']);
```

Complete partition:

```cpp
if (i == end) {

    result.push_back(end - start + 1);

    start = i + 1;
}
```

### Connection With Other NeetCode Problems

- Merge Triplets to Form Target → Greedy Filtering.
- Hand of Straights → Greedy + Ordered Map.
- Gas Station → Greedy + Running Sum.
- Jump Game → Greedy + Maximum Reach.
- Partition Labels → Greedy + Last Occurrence.

## 14. Final Interview Revision

Remember:

- Every character must belong to exactly one partition.
- Store the last occurrence of every character.
- `end` tracks the farthest required boundary.
- When `i == end`, the current partition is complete.

**Most Important Insight:**

Do not close a partition until every character inside it has reached its last occurrence.

By expanding the boundary greedily and closing it at the earliest valid position, we obtain the maximum number of partitions.

**Final Complexity:**

- Time: `O(n)`
- Auxiliary Space: `O(1)`
- Output Space: `O(n)`

  # 678. Valid Parenthesis String

- **Difficulty:** Medium
- **Pattern:** Greedy / String
- **Approaches:** Recursion, Dynamic Programming, Greedy
- **Language:** C++

## 1. Problem Understanding

We are given a string `s` containing three types of characters:

- `(` — Opening parenthesis.
- `)` — Closing parenthesis.
- `*` — Wildcard character.

The wildcard `*` can represent:

1. An opening parenthesis `(`.
2. A closing parenthesis `)`.
3. An empty string `""`.

We need to determine whether the string can be converted into a valid parenthesis string.

A valid parenthesis string must satisfy:

1. Every opening parenthesis must have a corresponding closing parenthesis.
2. Every closing parenthesis must have a corresponding opening parenthesis.
3. Opening parentheses must appear before their corresponding closing parentheses.

Return:

- `true` if the string can be made valid.
- `false` otherwise.

### Example 1

**Input:**

```cpp
s = "((*))";
```

**Output:**

```cpp
true
```

**Explanation:**

We can treat `*` as an empty string.

The resulting string is:

```text
(())
```

This is a valid parenthesis string.

### Example 2

**Input:**

```cpp
s = "(*))";
```

**Output:**

```cpp
true
```

**Explanation:**

We can replace `*` with an opening parenthesis.

The resulting string is:

```text
(())
```

This is valid.

### Example 3

**Input:**

```cpp
s = "(((*)";
```

**Output:**

```cpp
false
```

**Explanation:**

There are too many unmatched opening parentheses.

Even if `*` is treated as a closing parenthesis, one opening parenthesis remains unmatched.

Therefore, the string cannot be made valid.

## 2. Core Concept — Greedy

### Important Observation

In a normal Valid Parentheses problem, we maintain a balance counter.

```cpp
balance++;
```

For an opening parenthesis.

```cpp
balance--;
```

For a closing parenthesis.

But here, `*` introduces uncertainty because it can increase the balance, decrease it, or leave it unchanged.

Instead of maintaining one balance, we maintain a range of possible balances.

We use two variables:

```cpp
int low = 0;
int high = 0;
```

### Meaning of low

`low` represents the minimum possible number of unmatched opening parentheses.

### Meaning of high

`high` represents the maximum possible number of unmatched opening parentheses.

The actual balance can be any valid value between:

```cpp
low and high
```

This is the key Greedy idea.

## 3. Understanding the Three Characters

### Case 1: Opening Parenthesis `(`

An opening parenthesis increases the balance by 1.

Therefore:

```cpp
low++;
high++;
```

### Case 2: Closing Parenthesis `)`

A closing parenthesis decreases the balance by 1.

Therefore:

```cpp
low--;
high--;
```

However, `low` cannot become negative because we can never have fewer than zero unmatched opening parentheses.

So we use:

```cpp
low = max(0, low - 1);
high--;
```

### Case 3: Wildcard `*`

A wildcard has three possibilities.

**Option 1: Treat it as `(`**

Balance increases by 1.

**Option 2: Treat it as `)`**

Balance decreases by 1.

**Option 3: Treat it as empty**

Balance remains unchanged.

Therefore, the possible balance range expands:

```cpp
low--;
high++;
```

But the minimum balance cannot be negative:

```cpp
low = max(0, low - 1);
high++;
```

## 4. Important Invalid Condition

After processing every character, check:

```cpp
if (high < 0) {
    return false;
}
```

Why?

`high` represents the maximum possible balance.

If even the maximum possible balance becomes negative, there is no way to match the closing parentheses.

Therefore, the string is invalid.

At the end, we check:

```cpp
return low == 0;
```

Why?

If `low == 0`, at least one valid interpretation exists in which all opening parentheses are matched.

If `low > 0`, unmatched opening parentheses remain in every possible interpretation.

## 5. Approach 1 — Brute Force Recursion

### Idea

For every `*`, we try all three possible interpretations:

1. Treat `*` as `(`.
2. Treat `*` as `)`.
3. Treat `*` as an empty string.

For normal parentheses:

- `(` increases the balance.
- `)` decreases the balance.

We recursively explore every possibility.

### C++ Code

```cpp
class Solution {
public:

    bool solve(string& s, int index, int balance) {

        // More closing parentheses than opening parentheses
        if (balance < 0) {
            return false;
        }

        // All characters processed
        if (index == s.size()) {
            return balance == 0;
        }

        // Opening parenthesis
        if (s[index] == '(') {
            return solve(s, index + 1, balance + 1);
        }

        // Closing parenthesis
        if (s[index] == ')') {
            return solve(s, index + 1, balance - 1);
        }

        // Wildcard: try all three possibilities

        // Treat '*' as '('
        bool open = solve(s, index + 1, balance + 1);

        // Treat '*' as ')'
        bool close = solve(s, index + 1, balance - 1);

        // Treat '*' as empty
        bool empty = solve(s, index + 1, balance);

        return open || close || empty;
    }

    bool checkValidString(string s) {

        return solve(s, 0, 0);
    }
};
```

### Complexity Analysis

**Time Complexity:** `O(3^n)` in the worst case because every wildcard can generate three recursive branches.

**Space Complexity:** `O(n)` for the recursion stack.

### Limitation

The same index and balance combination can be calculated repeatedly.

We can optimize this using Dynamic Programming.

## 6. Approach 2 — Dynamic Programming (Memoization)

### Core Concept

Instead of solving the same state repeatedly, we store its result.

Define:

```cpp
dp[index][balance]
```

Where:

- `index` represents the current position in the string.
- `balance` represents the number of unmatched opening parentheses.

Each state tells us whether the remaining string can be made valid.

### Algorithm

1. Start at index 0 with balance 0.
2. For `(`, increase balance.
3. For `)`, decrease balance.
4. For `*`, explore all three possibilities.
5. Store previously calculated results.
6. At the end, return true only if balance is zero.

### C++ Code

```cpp
class Solution {
public:

    int dp[101][101];

    bool solve(string& s, int index, int balance) {

        if (balance < 0) {
            return false;
        }

        if (index == s.size()) {
            return balance == 0;
        }

        if (dp[index][balance] != -1) {
            return dp[index][balance];
        }

        bool result = false;

        if (s[index] == '(') {

            result = solve(s, index + 1, balance + 1);

        }
        else if (s[index] == ')') {

            result = solve(s, index + 1, balance - 1);

        }
        else {

            result =
                solve(s, index + 1, balance + 1) ||
                solve(s, index + 1, balance - 1) ||
                solve(s, index + 1, balance);
        }

        return dp[index][balance] = result;
    }

    bool checkValidString(string s) {

        memset(dp, -1, sizeof(dp));

        return solve(s, 0, 0);
    }
};
```

### Complexity Analysis

Let `n` be the length of the string.

- There are at most `n` indices.
- The balance ranges from 0 to `n`.
- Each state explores at most three transitions.

**Time Complexity:** `O(n²)`.

**Space Complexity:** `O(n²)` for the DP table, plus `O(n)` recursion stack.

## 7. Optimized Approach — Greedy (Two Counters)

### Core Concept

Instead of exploring all possible interpretations of `*`, we maintain the minimum and maximum possible balance.

```cpp
int low = 0;
int high = 0;
```

At every character, update the possible balance range.

### Algorithm

1. Initialize `low = 0` and `high = 0`.
2. Traverse the string.
3. If the character is `(`:
   - Increment `low`.
   - Increment `high`.
4. If the character is `)`:
   - Decrement `low`.
   - Decrement `high`.
5. If the character is `*`:
   - Decrement `low`.
   - Increment `high`.
6. Ensure `low` never becomes negative.
7. If `high` becomes negative, return `false`.
8. After processing the entire string, return whether `low == 0`.

## 8. Optimized C++ Solution

```cpp
class Solution {
public:
    bool checkValidString(string s) {

        int low = 0;
        int high = 0;

        for (char ch : s) {

            if (ch == '(') {

                low++;
                high++;

            }
            else if (ch == ')') {

                low--;
                high--;

            }
            else {

                // '*' can be '(', ')' or empty
                low--;
                high++;
            }

            // Minimum possible balance cannot be negative
            low = max(0, low);

            // Even maximum possible balance is negative
            if (high < 0) {
                return false;
            }
        }

        // At least one interpretation must have balance zero
        return low == 0;
    }
};
```

## 9. Dry Run — Step by Step

**Input:**

```cpp
s = "((*))";
```

Initially:

```cpp
low = 0;
high = 0;
```

### Step 1: Character `(`

An opening parenthesis increases both balances.

```cpp
low = 1;
high = 1;
```

### Step 2: Character `(`

Again, increase both.

```cpp
low = 2;
high = 2;
```

### Step 3: Character `*`

The wildcard can represent:

- `(`, giving balance 3.
- `)`, giving balance 1.
- Empty, keeping balance 2.

Therefore:

```cpp
low = 1;
high = 3;
```

### Step 4: Character `)`

Decrease both:

```cpp
low = 0;
high = 2;
```

### Step 5: Character `)`

Decrease both:

```cpp
low = -1;
high = 1;
```

Clamp the minimum:

```cpp
low = max(0, -1);

low = 0;
```

### Final State

```cpp
low = 0;
high = 1;
```

Since:

```cpp
low == 0
```

At least one valid interpretation exists.

**Output:**

```cpp
true
```

## 10. Dry Run — Invalid Case

**Input:**

```cpp
s = "(((*)";
```

Initially:

```cpp
low = 0;
high = 0;
```

| Character | low before clamp | high | low after clamp |
|---|---:|---:|---:|
| `(` | 1 | 1 | 1 |
| `(` | 2 | 2 | 2 |
| `(` | 3 | 3 | 3 |
| `*` | 2 | 4 | 2 |
| `)` | 1 | 3 | 1 |

At the end:

```cpp
low = 1;
high = 3;
```

Since `low` is not zero, at least one opening parenthesis remains unmatched in every possible interpretation.

**Output:**

```cpp
false
```

## 11. Why Does Greedy Work?

The wildcard character creates multiple possible balances.

Instead of checking each possibility individually, we maintain the range of possible balances.

- `low` tracks the smallest possible unmatched opening count.
- `high` tracks the largest possible unmatched opening count.

Whenever a wildcard appears, it can either increase, decrease, or preserve the balance.

Therefore, we expand the range.

Whenever a closing parenthesis appears, every possible balance decreases.

If even the maximum balance becomes negative, no valid interpretation is possible.

At the end, if the minimum possible balance is zero, we can choose an interpretation that leaves no unmatched opening parentheses.

This allows us to solve the problem in one traversal.

## 12. Time and Space Complexity

Let `n` be the length of the string.

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Recursion | O(3^n) | O(n) |
| DP Memoization | O(n²) | O(n²) |
| Greedy | O(n) | O(1) |

### Optimized Greedy Complexity

**Time Complexity: O(n)**

We traverse the string exactly once.

**Space Complexity: O(1)**

We use only two integer variables:

- `low`
- `high`

## 13. Important Interview Insights

### Q1. Why do we need two counters instead of one?

Because `*` has three possible interpretations.

One counter cannot represent all possible balances.

Two counters represent the minimum and maximum possible balances.

### Q2. What does low represent?

The minimum possible number of unmatched opening parentheses after processing the current prefix.

### Q3. What does high represent?

The maximum possible number of unmatched opening parentheses after processing the current prefix.

### Q4. Why do we use `low = max(0, low)`?

A valid parenthesis string cannot have more closing parentheses than opening parentheses at any point.

A negative balance is invalid.

Therefore, we clamp the minimum possible balance to zero.

### Q5. Why do we return false when `high < 0`?

Because even the maximum possible balance is negative.

There is no possible interpretation that can make the current prefix valid.

### Q6. Why do we return `low == 0`?

Because `low` is the minimum possible unmatched opening count.

If it is zero, there exists an interpretation in which all opening parentheses can be matched.

### Q7. Can `*` represent an empty string?

Yes.

This is important because sometimes the best interpretation is to ignore the wildcard entirely.

### Q8. Why is this a Greedy problem?

We do not need to choose the exact interpretation of every wildcard immediately.

Instead, we maintain the range of all possible valid balances and eliminate impossible states.

## 14. Common Mistakes

- Treating `*` as only an opening or closing parenthesis.
- Forgetting that `*` can also represent an empty string.
- Using only one balance counter.
- Forgetting to clamp `low` to zero.
- Checking `low < 0` instead of checking whether `high < 0`.
- Returning true just because `high >= 0` at the end.
- Forgetting to verify that `low == 0` after processing the complete string.
- Confusing the minimum possible balance with the maximum possible balance.

## 15. Comparison of Approaches

| Approach | Main Idea | Time | Space |
|---|---|---|---|
| Recursion | Try all three wildcard choices | O(3^n) | O(n) |
| DP | Cache index and balance states | O(n²) | O(n²) |
| Greedy | Maintain minimum and maximum balance | O(n) | O(1) |

## 16. Key Takeaways

### Most Important Pattern

**Wildcard Parentheses + Multiple Possibilities → Greedy with Balance Range**

Remember these three rules:

1. `(` increases both `low` and `high`.
2. `)` decreases both `low` and `high`.
3. `*` decreases `low` and increases `high`.

Important code:

```cpp
low = max(0, low);

if (high < 0) {
    return false;
}
```

Final condition:

```cpp
return low == 0;
```

### Connection With Other NeetCode Problems

- Valid Parentheses → Stack.
- Generate Parentheses → Backtracking.
- Minimum Add to Make Parentheses Valid → Greedy / Balance.
- Valid Parenthesis String → Greedy / Balance Range.
- Longest Valid Parentheses → Stack / Dynamic Programming.

## 17. Final Interview Revision

Remember:

- `low` = minimum possible unmatched opening parentheses.
- `high` = maximum possible unmatched opening parentheses.
- `*` can represent opening, closing, or empty.
- If `high < 0`, return false immediately.
- If `low == 0` at the end, return true.

**Most Important Insight:**

Instead of trying every possible interpretation of `*`, maintain the minimum and maximum possible balance.

This converts an exponential search into a single-pass Greedy solution.

**Final Complexity:**

- Time: `O(n)`
- Space: `O(1)`

- Time: `O(n)`
- Space: `O(1)`
