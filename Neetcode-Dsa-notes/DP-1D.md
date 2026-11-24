
# Climbing Stairs — LeetCode 70

- **Difficulty:** Easy
- **Pattern:** Dynamic Programming
- **Topics:** Recursion, Memoization, Tabulation, Space Optimization
- **Language:** C++

---

## 1. Problem Understanding

You are given an integer `n`, representing the number of steps required to reach the top of a staircase.

You can climb either:
- 1 step at a time.
- 2 steps at a time.

Return the total number of distinct ways to reach the top.

### Example 1

```text
Input: n = 2
Output: 2

Explanation:
1. 1 + 1 = 2
2. 2 = 2
```

### Example 2

```text
Input: n = 3
Output: 3

Explanation:
1. 1 + 1 + 1
2. 1 + 2
3. 2 + 1
```

### Key Observation

To reach step `n`, the last move must be either:

- A 1-step move from step `n-1`.
- A 2-step move from step `n-2`.

Therefore:

\[
f(n)=f(n-1)+f(n-2)
\]

This is the Fibonacci pattern.

---

## 2. Brute Force Approach — Recursion

### Idea

Use recursion to explore both possible choices at every step:

1. Climb one step.
2. Climb two steps.

Add the number of ways from both choices.

### C++ Code

```cpp
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) {
            return n;
        }

        return climbStairs(n - 1) +
               climbStairs(n - 2);
    }
};
```

### Dry Run

For `n = 4`:

```text
climbStairs(4)
= climbStairs(3) + climbStairs(2)

= [climbStairs(2) + climbStairs(1)] + 2

= [2 + 1] + 2

= 5
```

### Complexity

- **Time:** O(2^n) — repeated recursive calculations.
- **Auxiliary Space:** O(n) — maximum recursion stack depth.

### Limitation

The same subproblems are calculated multiple times.

For example, `climbStairs(3)` can be reached through different recursive branches.

We can optimize this using Dynamic Programming.

---

## 3. Optimized Approach 1 — Memoization (Top-Down DP)

### Idea

Memoization stores the result of every calculated subproblem.

Before calculating a state, check whether its result is already available.

Define:

```cpp
dp[i] = Number of ways to reach step i
```

Transition:

\[
dp[i]=dp[i-1]+dp[i-2]
\]

Base cases:

```cpp
dp[1] = 1;
dp[2] = 2;
```

### C++ Code

```cpp
class Solution {
public:
    int solve(int n, vector<int>& dp) {
        if (n <= 2) {
            return n;
        }

        if (dp[n] != -1) {
            return dp[n];
        }

        return dp[n] = solve(n - 1, dp) +
                       solve(n - 2, dp);
    }

    int climbStairs(int n) {
        vector<int> dp(n + 1, -1);

        return solve(n, dp);
    }
};
```

### Complexity

- **Time:** O(n) — each state is calculated once.
- **Auxiliary Space:** O(n) — DP array and recursion stack.

### Important Point

Memoization is called **Top-Down DP** because we start solving from the target state `n` and recursively move toward smaller states.

---

## 4. Optimized Approach 2 — Tabulation (Bottom-Up DP)

### Idea

Instead of recursion, calculate the answers from smaller states to larger states.

Define:

```cpp
dp[i] = Number of distinct ways to reach step i
```

Transition:

\[
dp[i]=dp[i-1]+dp[i-2]
\]

Base cases:

```cpp
dp[1] = 1;
dp[2] = 2;
```

### C++ Code

```cpp
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) {
            return n;
        }

        vector<int> dp(n + 1);

        dp[1] = 1;
        dp[2] = 2;

        for (int i = 3; i <= n; i++) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }

        return dp[n];
    }
};
```

### Dry Run

Input:

```text
n = 5
```

Initial values:

```text
dp[1] = 1
dp[2] = 2
```

Calculations:

| Step | Calculation | dp[i] |
|---|---|---:|
| 1 | Base case | 1 |
| 2 | Base case | 2 |
| 3 | 1 + 2 | 3 |
| 4 | 2 + 3 | 5 |
| 5 | 3 + 5 | 8 |

Final answer:

```text
Output: 8
```

### Complexity

- **Time:** O(n)
- **Auxiliary Space:** O(n) — DP array.

### Important Point

Tabulation is called **Bottom-Up DP** because we calculate smaller states first and build toward the final answer.

---

## 5. Best Approach — Space Optimized DP

### Observation

In the tabulation approach, we only need the previous two values:

```cpp
dp[i - 1]
dp[i - 2]
```

There is no need to store the entire DP array.

We can maintain two variables instead.

### C++ Code

```cpp
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) {
            return n;
        }

        int prev2 = 1;
        int prev1 = 2;

        for (int i = 3; i <= n; i++) {
            int curr = prev1 + prev2;

            prev2 = prev1;
            prev1 = curr;
        }

        return prev1;
    }
};
```

### Dry Run

Input:

```text
n = 5
```

Initial values:

```text
prev2 = 1
prev1 = 2
```

| Iteration | prev2 | prev1 | curr |
|---|---:|---:|---:|
| i = 3 | 1 | 2 | 3 |
| i = 4 | 2 | 3 | 5 |
| i = 5 | 3 | 5 | 8 |

Final answer:

```text
Output: 8
```

### Understanding Variable Updates

```cpp
int curr = prev1 + prev2;

prev2 = prev1;
prev1 = curr;
```

- `curr` stores the answer for the current step.
- `prev2` moves one position forward.
- `prev1` becomes the current answer.

**Important:** Calculate `curr` before updating the previous variables.

### Complexity

- **Time:** O(n)
- **Auxiliary Space:** O(1)

This is the recommended interview solution.

---

## 6. Comparison of All Approaches

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Recursion | O(2^n) | O(n) |
| Memoization | O(n) | O(n) |
| Tabulation | O(n) | O(n) |
| Space Optimized DP | O(n) | O(1) |

---

## 7. Important DP Concepts

### Q1. What is the DP state?

```cpp
dp[i] = Number of distinct ways to reach step i
```

### Q2. What is the recurrence relation?

\[
dp[i]=dp[i-1]+dp[i-2]
\]

### Q3. Why do we add the previous two states?

Because the last move must be either:

- One step from `i-1`.
- Two steps from `i-2`.

These are the only two possibilities.

### Q4. Why is space optimization possible?

The current state depends only on the previous two states.

Therefore, we do not need the entire DP array.

### Q5. What is the relationship with Fibonacci?

The Fibonacci sequence is:

```text
0, 1, 1, 2, 3, 5, 8, 13, ...
```

Climbing Stairs produces:

```text
1, 2, 3, 5, 8, 13, ...
```

The recurrence is identical, but the starting values are different.

Mathematically:

\[
f(n)=F(n+1)
\]

where `F` is the standard Fibonacci sequence.

---

## 8. Common Mistakes

1. Forgetting the base cases for `n = 1` and `n = 2`.
2. Allocating an array of size `n` instead of `n + 1` in tabulation.
3. Starting the tabulation loop from the wrong index.
4. Updating `prev2` before calculating `curr`.
5. Using plain recursion for large inputs, causing repeated calculations.
6. Confusing the number of steps with the number of possible moves.
7. Forgetting that different orders of moves represent different ways.

---

## 9. Interview Insights

### How to Identify This DP Pattern

Look for problems that ask:

- How many distinct ways can we reach a destination?
- What are the possible choices at each position?
- Can the answer be constructed from smaller subproblems?

### Problem-Solving Progression

A good interview explanation follows this sequence:

1. Start with recursive choices.
2. Identify overlapping subproblems.
3. Introduce memoization.
4. Convert recursion into tabulation.
5. Optimize space if only a few previous states are required.

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Climbing Stairs | 70 | 1D DP |
| Min Cost Climbing Stairs | 746 | 1D DP |
| House Robber | 198 | 1D DP |
| Fibonacci Number | 509 | 1D DP |
| N-th Tribonacci Number | 1137 | 1D DP |

---

## 10. Final Revision Notes

Remember these points:

1. There are two choices at each step: 1 or 2 steps.
2. The recurrence is `dp[i] = dp[i-1] + dp[i-2]`.
3. Base cases are `dp[1] = 1` and `dp[2] = 2`.
4. Memoization avoids repeated recursive calculations.
5. Tabulation calculates answers from smaller states to larger states.
6. Space optimization uses only two previous variables.

### Final Recommended Solution

- **Approach:** Space Optimized Dynamic Programming
- **Time Complexity:** O(n)
- **Auxiliary Space:** O(1)

  
# Min Cost Climbing Stairs — LeetCode 746

- **Difficulty:** Easy
- **Pattern:** Dynamic Programming
- **Topics:** Recursion, Memoization, Tabulation, Space Optimization
- **Language:** C++

---

## 1. Problem Understanding

You are given an integer array `cost`, where `cost[i]` represents the cost of stepping on the `i-th` floor.

After paying the cost of a floor, you can climb either:
- 1 step forward.
- 2 steps forward.

You can start from index `0` or index `1`.

Return the minimum cost required to reach the top of the staircase, which is just beyond the last index.

### Example 1

```text
Input: cost = [1,2,3]
Output: 2

Explanation:
Start at index 1.
Pay cost[1] = 2.
Take two steps to reach the top.

Total cost = 2.
```

### Example 2

```text
Input: cost = [1,2,1,2,1,1,1]
Output: 4
```

### Important Observation

Unlike Climbing Stairs, where we calculate the number of ways, here we calculate the minimum cost.

At every floor, we have two possible choices:

1. Move one step forward.
2. Move two steps forward.

We need to choose the path with the minimum total cost.

---

## 2. Brute Force Approach — Recursion

### Idea

Start from index `0` or index `1`.

At every index:
- Pay the current floor's cost.
- Recursively explore the next one-step and two-step moves.
- Return the minimum cost between both choices.

We can define:

```cpp
solve(i) = Minimum cost starting from index i
```

### Recurrence Relation

\[
solve(i)=cost[i]+\min(solve(i+1),solve(i+2))
\]

### Base Case

When we reach or pass the top of the staircase, no additional cost is required.

```cpp
if (i >= n) return 0;
```

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, vector<int>& cost) {
        int n = cost.size();

        if (i >= n) {
            return 0;
        }

        int oneStep = solve(i + 1, cost);
        int twoStep = solve(i + 2, cost);

        return cost[i] + min(oneStep, twoStep);
    }

    int minCostClimbingStairs(vector<int>& cost) {
        return min(solve(0, cost), solve(1, cost));
    }
};
```

### Dry Run

Input:

```text
cost = [1,2,3]
```

We can start at index `0` or index `1`.

Starting at index `0`:

```text
solve(0)
= 1 + min(solve(1), solve(2))

solve(1)
= 2 + min(solve(2), solve(3))

solve(2)
= 3 + min(solve(3), solve(4))

solve(3) = 0
solve(4) = 0
```

Therefore:

```text
solve(2) = 3
solve(1) = 2 + min(3, 0) = 2
solve(0) = 1 + min(2, 3) = 3
```

Starting at index `1`:

```text
solve(1) = 2
```

Final answer:

```text
min(3, 2) = 2
```

### Complexity

- **Time:** O(2^n) — repeated recursive calculations.
- **Auxiliary Space:** O(n) — recursion stack.

### Limitation

The same indexes are calculated multiple times.

We can avoid repeated calculations using Memoization.

---

## 3. Optimized Approach 1 — Memoization (Top-Down DP)

### Idea

Store the minimum cost calculated for each index.

Before calculating a state, check whether its answer is already stored.

Define:

```cpp
dp[i] = Minimum cost required to reach the top starting from index i
```

### Recurrence Relation

\[
dp[i]=cost[i]+\min(dp[i+1],dp[i+2])
\]

### Base Case

```cpp
if (i >= n) return 0;
```

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, vector<int>& cost, vector<int>& dp) {
        int n = cost.size();

        if (i >= n) {
            return 0;
        }

        if (dp[i] != -1) {
            return dp[i];
        }

        int oneStep = solve(i + 1, cost, dp);
        int twoStep = solve(i + 2, cost, dp);

        return dp[i] = cost[i] + min(oneStep, twoStep);
    }

    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size();

        vector<int> dp(n, -1);

        return min(solve(0, cost, dp),
                   solve(1, cost, dp));
    }
};
```

### Complexity

- **Time:** O(n) — each index is calculated once.
- **Auxiliary Space:** O(n) — DP array and recursion stack.

### Important Point

Memoization is called **Top-Down DP** because we start from the initial choices and recursively solve the remaining subproblems.

---

## 4. Optimized Approach 2 — Tabulation (Bottom-Up DP)

### Idea

Instead of recursion, calculate the minimum cost from the end of the array toward the beginning.

Define:

```cpp
dp[i] = Minimum cost to reach the top starting from index i
```

### Recurrence Relation

\[
dp[i]=cost[i]+\min(dp[i+1],dp[i+2])
\]

### Base Cases

We can reach the top from index `n` or beyond without paying any additional cost.

Therefore:

```cpp
dp[n] = 0;
dp[n + 1] = 0;
```

### C++ Code

```cpp
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size();

        vector<int> dp(n + 2, 0);

        for (int i = n - 1; i >= 0; i--) {
            dp[i] = cost[i] + min(dp[i + 1], dp[i + 2]);
        }

        return min(dp[0], dp[1]);
    }
};
```

### Dry Run

Input:

```text
cost = [1,2,3]
```

Initial DP array:

```text
dp = [0,0,0,0,0]
```

We calculate from right to left.

| Index | Calculation | dp[i] |
|---|---|---:|
| 2 | 3 + min(0, 0) | 3 |
| 1 | 2 + min(3, 0) | 2 |
| 0 | 1 + min(2, 3) | 3 |

Final DP array:

```text
dp = [3,2,3,0,0]
```

We can start at index `0` or index `1`.

```text
Answer = min(dp[0], dp[1])
       = min(3, 2)
       = 2
```

### Complexity

- **Time:** O(n)
- **Auxiliary Space:** O(n) — DP array.

### Important Point

Tabulation is called **Bottom-Up DP** because we calculate smaller subproblems first and build toward the initial state.

---

## 5. Best Approach — Space Optimized DP

### Observation

In tabulation, the current state only depends on the next two states:

```cpp
dp[i + 1]
dp[i + 2]
```

We do not need the complete DP array.

We can maintain only two variables.

### C++ Code

```cpp
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size();

        int next1 = 0;
        int next2 = 0;

        for (int i = n - 1; i >= 0; i--) {
            int curr = cost[i] + min(next1, next2);

            next2 = next1;
            next1 = curr;
        }

        return min(next1, next2);
    }
};
```

### Dry Run

Input:

```text
cost = [1,2,3]
```

Initial values:

```text
next1 = 0
next2 = 0
```

We iterate from index `2` to index `0`.

| Index | Calculation | curr |
|---|---|---:|
| 2 | 3 + min(0, 0) | 3 |
| 1 | 2 + min(3, 0) | 2 |
| 0 | 1 + min(2, 3) | 3 |

Variable updates:

```text
After index 2:
next1 = 3
next2 = 0

After index 1:
next1 = 2
next2 = 3

After index 0:
next1 = 3
next2 = 2
```

Final answer:

```text
min(next1, next2)
= min(3, 2)
= 2
```

### Understanding Variable Updates

```cpp
int curr = cost[i] + min(next1, next2);

next2 = next1;
next1 = curr;
```

- `next1` stores the minimum cost from the next index.
- `next2` stores the minimum cost from the index after that.
- `curr` stores the minimum cost from the current index.

**Important:** Calculate `curr` before updating the previous variables.

### Complexity

- **Time:** O(n)
- **Auxiliary Space:** O(1)

This is the recommended interview solution.

---

## 6. Comparison of All Approaches

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Recursion | O(2^n) | O(n) |
| Memoization | O(n) | O(n) |
| Tabulation | O(n) | O(n) |
| Space Optimized DP | O(n) | O(1) |

---

## 7. Important DP Concepts

### Q1. What is the DP state?

```cpp
dp[i] = Minimum cost to reach the top starting from index i
```

### Q2. What is the recurrence relation?

\[
dp[i]=cost[i]+\min(dp[i+1],dp[i+2])
\]

### Q3. Why do we use minimum?

Because we need the cheapest possible path to reach the top.

At every index, we compare the cost of:
- Taking one step.
- Taking two steps.

### Q4. Why do we return min(dp[0], dp[1])?

Because the problem allows us to start at either index `0` or index `1`.

### Q5. Why are the base cases zero?

When we reach the top, we have no more floors to pay for.

Therefore, the additional cost is zero.

### Q6. What is the difference between Climbing Stairs and Min Cost Climbing Stairs?

| Climbing Stairs | Min Cost Climbing Stairs |
|---|---|
| Counts distinct ways | Finds minimum cost |
| Uses addition | Uses minimum |
| dp[i] = dp[i-1] + dp[i-2] | dp[i] = cost[i] + min(dp[i+1], dp[i+2]) |
| Fibonacci pattern | Minimum-cost DP |

---

## 8. Common Mistakes

1. Forgetting that we can start from index `0` or index `1`.
2. Returning only the result from index `0`.
3. Forgetting to add `cost[i]` to the transition.
4. Using `max()` instead of `min()`.
5. Incorrectly handling the top of the staircase.
6. Allocating an insufficient DP array for `dp[n]` and `dp[n+1]`.
7. Updating `next1` and `next2` in the wrong order.
8. Confusing the number of ways with the minimum cost.

---

## 9. Interview Insights

### How to Identify This DP Pattern

Look for problems involving:

- Minimum cost to reach a destination.
- Multiple choices at every position.
- A decision between moving one or more steps.
- An optimal answer constructed from smaller subproblems.

### Problem-Solving Progression

A good interview explanation follows this sequence:

1. Define the recursive choices.
2. Write the recurrence relation.
3. Identify overlapping subproblems.
4. Apply memoization.
5. Convert recursion into tabulation.
6. Optimize space when only a few previous states are needed.

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Climbing Stairs | 70 | 1D DP |
| Min Cost Climbing Stairs | 746 | 1D DP |
| House Robber | 198 | 1D DP |
| House Robber II | 213 | Circular 1D DP |
| Fibonacci Number | 509 | 1D DP |

---

## 10. Final Revision Notes

Remember these points:

1. We can start from index `0` or index `1`.
2. Every floor has two possible moves: one step or two steps.
3. We need the minimum total cost.
4. The recurrence is `cost[i] + min(dp[i+1], dp[i+2])`.
5. The base cases are zero because reaching the top costs nothing.
6. Space optimization works because only two future states are required.

### Final Recommended Solution

- **Approach:** Space Optimized Dynamic Programming
- **Time Complexity:** O(n)
- **Auxiliary Space:** O(1)

**Interview takeaway:** This problem introduces minimum-cost 1D DP. The important skill is defining the DP state correctly and recognizing that the current answer depends on the minimum of two future states.


# House Robber — LeetCode 198

- **Difficulty:** Medium
- **Pattern:** Dynamic Programming (1D DP)
- **Topics:** Recursion, Memoization, Tabulation, Space Optimization
- **Language:** C++

---

## 1. Problem Understanding

You are given an integer array `nums`, where `nums[i]` represents the amount of money available in the `i-th` house.

The houses are arranged in a straight line.

You cannot rob two adjacent houses because doing so will trigger the security system.

Return the maximum amount of money you can rob without robbing adjacent houses.

### Example 1

```text
Input: nums = [1,1,3,3]
Output: 4

Explanation:
Rob house 0 and house 2.

Total = nums[0] + nums[2]
      = 1 + 3
      = 4
```

### Example 2

```text
Input: nums = [2,9,8,3,6]
Output: 15

Explanation:
Rob house 1 and house 4.

Total = 9 + 6 = 15
```

### Important Observation

At every house, we have two choices:

1. **Rob the current house:** We cannot rob the previous house, so we add the money from the house before the previous one.
2. **Skip the current house:** We keep the maximum money calculated up to the previous house.

We need to select the choice that gives us the maximum amount.

---

## 2. Brute Force Approach — Recursion

### Idea

For every house, we have two possibilities:

- Rob the current house and move two positions forward.
- Skip the current house and move one position forward.

Define:

```cpp
solve(i) = Maximum money that can be robbed starting from index i.
```

### Recurrence Relation

If we rob the current house:

```cpp
rob = nums[i] + solve(i + 2);
```

If we skip the current house:

```cpp
skip = solve(i + 1);
```

Therefore:

\[
solve(i)=\max(nums[i]+solve(i+2),solve(i+1))
\]

### Base Case

When we reach or pass the end of the array, no more money can be collected.

```cpp
if (i >= n) return 0;
```

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, vector<int>& nums) {
        int n = nums.size();

        if (i >= n) {
            return 0;
        }

        int rob = nums[i] + solve(i + 2, nums);

        int skip = solve(i + 1, nums);

        return max(rob, skip);
    }

    int rob(vector<int>& nums) {
        return solve(0, nums);
    }
};
```

### Dry Run

Input:

```text
nums = [2,9,8,3,6]
```

At index `0`, we have two choices:

```text
Rob house 0:
2 + solve(2)

Skip house 0:
solve(1)
```

The recursion explores both possibilities.

One optimal path:

```text
House 0: Skip
House 1: Rob  (9)
House 2: Skip
House 3: Skip
House 4: Rob  (6)
```

Total:

```text
9 + 6 = 15
```

Final Answer:

```text
15
```

### Complexity

- **Time:** O(2^n) — exponential recursive branching.
- **Auxiliary Space:** O(n) — recursion stack.

### Limitation

The same indexes are calculated multiple times.

We can avoid repeated calculations using Memoization.

---

## 3. Optimized Approach 1 — Memoization (Top-Down DP)

### Idea

Store the result of every index so that we do not calculate the same subproblem repeatedly.

Define:

```cpp
dp[i] = Maximum money that can be robbed starting from index i.
```

### Recurrence Relation

\[
dp[i]=\max(nums[i]+dp[i+2],dp[i+1])
\]

### Base Case

```cpp
if (i >= n) return 0;
```

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, vector<int>& nums, vector<int>& dp) {
        int n = nums.size();

        if (i >= n) {
            return 0;
        }

        if (dp[i] != -1) {
            return dp[i];
        }

        int rob = nums[i] + solve(i + 2, nums, dp);

        int skip = solve(i + 1, nums, dp);

        return dp[i] = max(rob, skip);
    }

    int rob(vector<int>& nums) {
        int n = nums.size();

        vector<int> dp(n, -1);

        return solve(0, nums, dp);
    }
};
```

### Complexity

- **Time:** O(n) — each index is calculated once.
- **Auxiliary Space:** O(n) — DP array and recursion stack.

### Important Point

Memoization is called **Top-Down DP** because we start from the initial state and recursively solve smaller subproblems.

---

## 4. Optimized Approach 2 — Tabulation (Bottom-Up DP)

### Idea

Instead of recursion, calculate the maximum money from the beginning of the array toward the end.

Define:

```cpp
dp[i] = Maximum money that can be robbed from houses 0 to i.
```

At every index, we have two choices:

**Choice 1: Rob the current house**

We cannot rob the previous house.

```cpp
rob = nums[i] + dp[i - 2];
```

**Choice 2: Skip the current house**

Keep the previous maximum.

```cpp
skip = dp[i - 1];
```

### Recurrence Relation

\[
dp[i]=\max(nums[i]+dp[i-2],dp[i-1])
\]

### Base Cases

For the first house:

```cpp
dp[0] = nums[0];
```

For the second house:

```cpp
dp[1] = max(nums[0], nums[1]);
```

### C++ Code

```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        vector<int> dp(n, 0);

        dp[0] = nums[0];

        dp[1] = max(nums[0], nums[1]);

        for (int i = 2; i < n; i++) {
            int rob = nums[i] + dp[i - 2];

            int skip = dp[i - 1];

            dp[i] = max(rob, skip);
        }

        return dp[n - 1];
    }
};
```

### Dry Run

Input:

```text
nums = [2,9,8,3,6]
```

Initialization:

```text
dp[0] = 2

dp[1] = max(2,9)
      = 9
```

Now iterate from index `2`.

| Index | Rob | Skip | dp[i] |
|---|---|---|---:|
| 0 | 2 | - | 2 |
| 1 | 9 | - | 9 |
| 2 | 8 + 2 = 10 | 9 | 10 |
| 3 | 3 + 9 = 12 | 10 | 12 |
| 4 | 6 + 10 = 16 | 12 | 16 |

Final DP array:

```text
dp = [2,9,10,12,16]
```

Answer:

```text
16
```

**Correction to the earlier example path:** For `[2,9,8,3,6]`, the optimal selection is houses 1, 3 and 4? These include adjacent houses 3 and 4, so that selection is invalid. The DP result is 16, obtained by robbing houses 1 and 4? That gives 9 + 6 = 15. The correct optimal selection is houses 0, 2 and 4:

2 + 8 + 6 = 16.
```

### Complexity

- **Time:** O(n)
- **Auxiliary Space:** O(n) — DP array.

---

## 5. Best Approach — Space Optimized DP

### Observation

In tabulation, the current state depends only on two previous states:

```cpp
dp[i - 1]
dp[i - 2]
```

We do not need to maintain the entire DP array.

We can maintain only two variables.

### C++ Code

```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();

        int prev1 = 0;
        int prev2 = 0;

        for (int i = 0; i < n; i++) {
            int rob = nums[i] + prev2;

            int skip = prev1;

            int curr = max(rob, skip);

            prev2 = prev1;
            prev1 = curr;
        }

        return prev1;
    }
};
```

### Dry Run

Input:

```text
nums = [2,9,8,3,6]
```

Initial values:

```text
prev1 = 0
prev2 = 0
```

| Index | nums[i] | Rob = nums[i] + prev2 | Skip = prev1 | curr |
|---|---:|---:|---:|---:|
| 0 | 2 | 2 | 0 | 2 |
| 1 | 9 | 9 | 2 | 9 |
| 2 | 8 | 10 | 9 | 10 |
| 3 | 3 | 12 | 10 | 12 |
| 4 | 6 | 16 | 12 | 16 |

Variable updates:

```text
After index 0:
prev1 = 2
prev2 = 0

After index 1:
prev1 = 9
prev2 = 2

After index 2:
prev1 = 10
prev2 = 9

After index 3:
prev1 = 12
prev2 = 10

After index 4:
prev1 = 16
prev2 = 12
```

Final Answer:

```text
16
```

### Understanding Variable Updates

```cpp
int curr = max(nums[i] + prev2, prev1);

prev2 = prev1;
prev1 = curr;
```

- `prev1` stores the maximum money up to the previous house.
- `prev2` stores the maximum money up to the house before the previous one.
- `curr` stores the maximum money up to the current house.

**Important:** Calculate `curr` before updating `prev1` and `prev2`.

### Complexity

- **Time:** O(n)
- **Auxiliary Space:** O(1)

This is the recommended interview solution.

---

## 6. Comparison of All Approaches

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Recursion | O(2^n) | O(n) |
| Memoization | O(n) | O(n) |
| Tabulation | O(n) | O(n) |
| Space Optimized DP | O(n) | O(1) |

---

## 7. Important DP Concepts

### Q1. What is the DP state?

```cpp
dp[i] = Maximum money that can be robbed from houses 0 to i.
```

### Q2. What is the recurrence relation?

\[
dp[i]=\max(nums[i]+dp[i-2],dp[i-1])
\]

### Q3. Why do we use max()?

Because we need to maximize the total money.

At every house, we compare:

- Rob the current house.
- Skip the current house.

### Q4. Why can't we rob adjacent houses?

The problem explicitly prohibits robbing two neighboring houses.

If we rob house `i`, the previous house `i-1` cannot be robbed.

### Q5. Why do we use dp[i-2] when robbing?

Because robbing house `i` means we must skip house `i-1`.

Therefore, the previous valid result is the maximum money up to index `i-2`.

### Q6. Why does space optimization work?

The current state depends only on the previous two DP states.

Therefore, maintaining two variables is sufficient.

---

## 8. Common Mistakes

1. Adding the previous house's value when robbing the current house.
2. Using `dp[i-1]` instead of `dp[i-2]` in the rob choice.
3. Forgetting to compare the rob and skip choices.
4. Incorrectly initializing the first two DP states.
5. Forgetting the single-element array case in the tabulation approach.
6. Updating `prev1` before calculating `curr`.
7. Confusing the maximum sum of non-adjacent elements with the sum of all elements.

---

## 9. Interview Insights

### How to Identify This DP Pattern

Look for problems where:

- You need to maximize or minimize a total.
- Choosing the current element prevents choosing its neighbor.
- Every position offers two choices.
- The answer depends on previously calculated states.

### Problem-Solving Progression

A good interview explanation follows this sequence:

1. Define the two choices: rob or skip.
2. Write the recursive recurrence.
3. Identify overlapping subproblems.
4. Apply memoization.
5. Convert recursion into tabulation.
6. Optimize space using two variables.

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Climbing Stairs | 70 | 1D DP |
| Min Cost Climbing Stairs | 746 | Minimum Cost DP |
| House Robber | 198 | Maximum Sum DP |
| House Robber II | 213 | Circular DP |
| Delete and Earn | 740 | DP / House Robber Pattern |

---

## 10. Final Revision Notes

Remember these points:

1. At every house, choose between robbing and skipping.
2. Robbing the current house means skipping the previous house.
3. The rob choice is `nums[i] + dp[i-2]`.
4. The skip choice is `dp[i-1]`.
5. Use `max()` because we want maximum money.
6. Only two previous DP states are required for the optimized solution.

### Final Recommended Solution

- **Approach:** Space Optimized Dynamic Programming
- **Time Complexity:** O(n)
- **Auxiliary Space:** O(1)

**Interview takeaway:** House Robber is a classic 1D DP problem. Whenever selecting an element prevents selecting its adjacent element, consider the two choices: take it or skip it.

# House Robber II — LeetCode 213

- **Difficulty:** Medium
- **Pattern:** Dynamic Programming (1D DP)
- **Topics:** Circular Array, Recursion, Memoization, Tabulation, Space Optimization
- **Language:** C++

---

## 1. Problem Understanding

You are given an integer array `nums`, where `nums[i]` represents the amount of money in the `i-th` house.

The houses are arranged in a circle.

You cannot rob two adjacent houses because doing so triggers the security system.

Since the houses are circular:
- The first house and the last house are also adjacent.
- You cannot rob both `nums[0]` and `nums[n-1]`.

Return the maximum amount of money you can rob without robbing adjacent houses.

### Example 1

```text
Input: nums = [3,4,3]
Output: 4

Explanation:
You cannot rob nums[0] + nums[2] = 3 + 3 = 6
because the first and last houses are adjacent.

The maximum amount is nums[1] = 4.
```

### Example 2

```text
Input: nums = [2,9,8,3,6]
Output: 15

Explanation:
The maximum amount is obtained by robbing houses
at index 1 and index 4.

Total = 9 + 6 = 15.
```

---

## 2. Important Observation — Circular Array

In House Robber I, houses are arranged in a straight line.

In House Robber II, houses are arranged in a circle.

The problem is that we cannot rob both the first and last houses.

### Key Insight

There are only two valid possibilities:

**Case 1: Exclude the last house**

Consider houses from index `0` to `n-2`.

```cpp
nums[0 ... n-2]
```

**Case 2: Exclude the first house**

Consider houses from index `1` to `n-1`.

```cpp
nums[1 ... n-1]
```

Now both cases become the original House Robber I problem.

Therefore:

\[
Answer = \max(Case1, Case2)
\]

This is the most important observation in this problem.

---

## 3. Brute Force Approach — Recursion

### Idea

We can divide the circular problem into two linear problems:

1. Exclude the last house and solve the remaining houses.
2. Exclude the first house and solve the remaining houses.

For each linear problem, use the recursive House Robber approach.

### Recursive Function

```cpp
solve(i, end)
```

Returns the maximum money that can be robbed from index `i` through index `end`.

### Recurrence Relation

Rob the current house:

```cpp
rob = nums[i] + solve(i + 2, end);
```

Skip the current house:

```cpp
skip = solve(i + 1, end);
```

Therefore:

\[
solve(i)=\max(nums[i]+solve(i+2),solve(i+1))
\]

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, int end, vector<int>& nums) {
        if (i > end) {
            return 0;
        }

        int rob = nums[i] + solve(i + 2, end, nums);

        int skip = solve(i + 1, end, nums);

        return max(rob, skip);
    }

    int rob(vector<int>& nums) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        int case1 = solve(0, n - 2, nums);

        int case2 = solve(1, n - 1, nums);

        return max(case1, case2);
    }
};
```

### Complexity

- **Time:** O(2^n) — exponential recursion.
- **Auxiliary Space:** O(n) — recursion stack.

### Limitation

Repeated recursive calculations make this approach inefficient.

We can optimize it using Memoization.

---

## 4. Optimized Approach 1 — Memoization (Top-Down DP)

### Idea

We use the same two cases:

- Case 1: Consider indices `0` to `n-2`.
- Case 2: Consider indices `1` to `n-1`.

For each case, store previously calculated results in a DP array.

### DP State

```cpp
dp[i] = Maximum money that can be robbed
        from index i to end.
```

### Recurrence Relation

\[
dp[i]=\max(nums[i]+dp[i+2],dp[i+1])
\]

### C++ Code

```cpp
class Solution {
public:
    int solve(int i, int end, vector<int>& nums, vector<int>& dp) {
        if (i > end) {
            return 0;
        }

        if (dp[i] != -1) {
            return dp[i];
        }

        int rob = nums[i] + solve(i + 2, end, nums, dp);

        int skip = solve(i + 1, end, nums, dp);

        return dp[i] = max(rob, skip);
    }

    int rob(vector<int>& nums) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        vector<int> dp1(n, -1);
        vector<int> dp2(n, -1);

        int case1 = solve(0, n - 2, nums, dp1);

        int case2 = solve(1, n - 1, nums, dp2);

        return max(case1, case2);
    }
};
```

### Why Two DP Arrays?

We solve two different ranges:

- First DP array stores results for the range `0` to `n-2`.
- Second DP array stores results for the range `1` to `n-1`.

Using separate arrays keeps the states of the two cases independent.

### Complexity

- **Time:** O(n) — each state is calculated once per case.
- **Auxiliary Space:** O(n) — DP arrays and recursion stack.

---

## 5. Optimized Approach 2 — Tabulation (Bottom-Up DP)

### Idea

We can convert House Robber I into an iterative DP solution.

For each case, calculate the maximum money from the beginning of the selected range to its end.

### DP State

```cpp
dp[i] = Maximum money that can be robbed
        from the beginning of the selected range to index i.
```

### Recurrence Relation

\[
dp[i]=\max(nums[i]+dp[i-2],dp[i-1])
\]

### C++ Code

```cpp
class Solution {
public:
    int solve(vector<int>& nums, int start, int end) {
        int len = end - start + 1;

        if (len == 1) {
            return nums[start];
        }

        vector<int> dp(len, 0);

        dp[0] = nums[start];

        dp[1] = max(nums[start], nums[start + 1]);

        for (int i = 2; i < len; i++) {
            int rob = nums[start + i] + dp[i - 2];

            int skip = dp[i - 1];

            dp[i] = max(rob, skip);
        }

        return dp[len - 1];
    }

    int rob(vector<int>& nums) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        int case1 = solve(nums, 0, n - 2);

        int case2 = solve(nums, 1, n - 1);

        return max(case1, case2);
    }
};
```

### Complexity

- **Time:** O(n) — two linear traversals.
- **Auxiliary Space:** O(n) — DP array.

---

## 6. Best Approach — Space Optimized DP

### Observation

In House Robber I, the current state depends only on two previous states.

We do not need the complete DP array.

We can maintain two variables:

```cpp
prev1 = Maximum money up to the previous house.

prev2 = Maximum money up to the house before previous.
```

### Helper Function

We create a helper function that solves the original linear House Robber problem for a given range.

```cpp
robLinear(nums, start, end)
```

### C++ Code — Recommended Solution

```cpp
class Solution {
public:
    int robLinear(vector<int>& nums, int start, int end) {
        int prev1 = 0;
        int prev2 = 0;

        for (int i = start; i <= end; i++) {
            int rob = nums[i] + prev2;

            int skip = prev1;

            int curr = max(rob, skip);

            prev2 = prev1;
            prev1 = curr;
        }

        return prev1;
    }

    int rob(vector<int>& nums) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        // Case 1: Exclude the last house.
        int case1 = robLinear(nums, 0, n - 2);

        // Case 2: Exclude the first house.
        int case2 = robLinear(nums, 1, n - 1);

        return max(case1, case2);
    }
};
```

### Dry Run

Input:

```text
nums = [2,9,8,3,6]
```

There are two cases.

### Case 1: Exclude the Last House

Selected range:

```text
[2,9,8,3]
```

Apply the linear House Robber algorithm.

| Index | Value | Rob | Skip | Maximum |
|---|---:|---:|---:|---:|
| 0 | 2 | 2 | 0 | 2 |
| 1 | 9 | 9 | 2 | 9 |
| 2 | 8 | 10 | 9 | 10 |
| 3 | 3 | 12 | 10 | 12 |

Result:

```text
case1 = 12
```

### Case 2: Exclude the First House

Selected range:

```text
[9,8,3,6]
```

Apply the linear House Robber algorithm.

| Index | Value | Rob | Skip | Maximum |
|---|---:|---:|---:|---:|
| 1 | 9 | 9 | 0 | 9 |
| 2 | 8 | 8 | 9 | 9 |
| 3 | 3 | 12 | 9 | 12 |
| 4 | 6 | 15 | 12 | 15 |

Result:

```text
case2 = 15
```

### Final Answer

```text
Answer = max(case1, case2)

       = max(12, 15)

       = 15
```

Therefore, the maximum amount is **15**.

### Complexity

- **Time:** O(n) — two linear traversals.
- **Auxiliary Space:** O(1) — only two variables are used.

---

## 7. Comparison of All Approaches

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Recursion | O(2^n) | O(n) |
| Memoization | O(n) | O(n) |
| Tabulation | O(n) | O(n) |
| Space Optimized DP | O(n) | O(1) |

---

## 8. Important DP Concepts

### Q1. What is the main difference between House Robber I and II?

| House Robber I | House Robber II |
|---|---|
| Houses are in a straight line. | Houses are in a circle. |
| First and last houses are not adjacent. | First and last houses are adjacent. |
| One linear DP calculation. | Two linear DP calculations. |
| LeetCode 198 | LeetCode 213 |

### Q2. Why do we divide the problem into two cases?

Because the first and last houses are adjacent.

We cannot rob both.

Therefore, every valid solution must exclude at least one of them.

We consider:

1. Exclude the last house.
2. Exclude the first house.

Taking the maximum of these two cases guarantees the best valid answer.

### Q3. Why do we handle n == 1 separately?

If there is only one house, we can simply rob it.

```cpp
if (n == 1) {
    return nums[0];
}
```

Without this check, the two ranges would not represent the correct circular problem.

### Q4. Why can we reuse the House Robber I algorithm?

Once we exclude either the first or last house, the remaining houses form a straight line.

Therefore, the original House Robber I solution works directly.

### Q5. Why do we use max()?

Because we want to maximize the total amount of money.

At each house, we compare robbing and skipping.

### Q6. What is the most important optimization?

Only two previous DP states are needed.

Therefore, we can replace the DP array with two variables and achieve O(1) auxiliary space.

---

## 9. Common Mistakes

1. Applying House Robber I directly to the entire circular array.
2. Forgetting that the first and last houses are adjacent.
3. Trying to rob both the first and last houses.
4. Forgetting to handle the single-house case.
5. Using incorrect start and end indexes.
6. Returning only Case 1 or Case 2 instead of taking their maximum.
7. Updating `prev1` and `prev2` in the wrong order.
8. Creating unnecessary DP arrays when space optimization is possible.

---

## 10. Interview Insights

### How to Identify This Pattern

Look for problems where:

- Elements are arranged in a circular structure.
- Adjacent elements cannot both be selected.
- You need to maximize a sum.
- The first and last elements have a special adjacency relationship.

### Problem-Solving Progression

A good interview explanation follows this sequence:

1. Identify the circular dependency.
2. Recognize that the first and last houses cannot both be robbed.
3. Divide the problem into two linear ranges.
4. Apply the House Robber I recurrence to each range.
5. Optimize each linear DP calculation using two variables.
6. Return the maximum result.

### Related Problems

| Problem | LeetCode | Pattern |
|---|---:|---|
| Climbing Stairs | 70 | 1D DP |
| Min Cost Climbing Stairs | 746 | Minimum Cost DP |
| House Robber | 198 | Linear DP |
| House Robber II | 213 | Circular DP |
| Delete and Earn | 740 | House Robber Pattern |

---

## 11. Final Revision Notes

Remember these points:

1. House Robber II is an extension of House Robber I.
2. The houses are arranged in a circle.
3. The first and last houses cannot both be robbed.
4. Exclude the last house and solve the range `[0, n-2]`.
5. Exclude the first house and solve the range `[1, n-1]`.
6. Apply the linear House Robber algorithm to both ranges.
7. Return the maximum of the two results.
8. Handle `n == 1` separately.

### Final Recommended Solution

- **Approach:** Two Cases + Space Optimized DP
- **Time Complexity:** O(n)
- **Auxiliary Space:** O(1)

**Interview takeaway:** When a linear DP problem becomes circular, check whether you can break the circular dependency into a small number of linear cases. Here, excluding the first or last house converts House Robber II into two instances of House Robber I.
# Longest Palindromic Substring (LeetCode 5)

- **Difficulty:** Medium
- **Topics:** String, Two Pointers, Dynamic Programming
- **Pattern:** Expand Around Center

## 1. Problem Understanding

Given a string `s`, return the longest substring that is a palindrome.

A palindrome reads the same forward and backward.

If multiple longest palindromic substrings exist, return any one.

### Examples

**Example 1**
```text
Input: s = "babad"
Output: "bab"
```
`"aba"` is also a valid answer.

**Example 2**
```text
Input: s = "cbbd"
Output: "bb"
```

## 2. Brute Force Approach

### Idea

1. Generate every possible substring.
2. Check whether each substring is a palindrome.
3. Store the longest palindrome found.

### C++ Code

```cpp
class Solution {
public:
    bool isPalindrome(string& s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }

    string longestPalindrome(string s) {
        int n = s.length();
        string ans = "";

        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                if (isPalindrome(s, i, j)) {
                    if (j - i + 1 > ans.length()) {
                        ans = s.substr(i, j - i + 1);
                    }
                }
            }
        }

        return ans;
    }
};
```

### Complexity

- **Time:** O(n³)
- **Auxiliary Space:** O(1), excluding the returned substring.

There are O(n²) possible substrings, and checking each one takes O(n).

## 3. Dynamic Programming Approach

### Idea

Use a 2D DP table to store whether a substring is a palindrome.

### DP State

`dp[i][j]` indicates whether the substring from index `i` to `j` is a palindrome.

### Base Cases

Every single character is a palindrome:

```cpp
dp[i][i] = true;
```

For two characters:

```cpp
dp[i][i + 1] = (s[i] == s[i + 1]);
```

### Recurrence Relation

A substring is a palindrome when its first and last characters match and its inner substring is also a palindrome.

```text
dp[i][j] = (s[i] == s[j]) && dp[i + 1][j - 1]
```

For a substring of length 2, matching endpoints are sufficient.

### C++ Code

```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.length();

        vector<vector<bool>> dp(n, vector<bool>(n, false));

        int start = 0;
        int maxLen = 1;

        for (int i = 0; i < n; i++) {
            dp[i][i] = true;
        }

        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len <= n; i++) {
                int j = i + len - 1;

                if (s[i] == s[j]) {
                    if (len == 2 || dp[i + 1][j - 1]) {
                        dp[i][j] = true;

                        if (len > maxLen) {
                            maxLen = len;
                            start = i;
                        }
                    }
                }
            }
        }

        return s.substr(start, maxLen);
    }
};
```

### Complexity

- **Time:** O(n²)
- **Auxiliary Space:** O(n²)

## 4. Optimized Approach: Expand Around Center

### Key Observation

Every palindrome has a center.

There are two types:

**Odd-length palindrome:** One middle character.

Example: `"racecar"`

```text
    c
   / \
  a   a
 /     \
r       r
```

Expand from:

```cpp
expand(s, i, i);
```

**Even-length palindrome:** Two middle characters.

Example: `"abba"`

```text
   b   b
  /     \
 a       a
```

Expand from:

```cpp
expand(s, i, i + 1);
```

Therefore, we check both possible centers for every index.

### Algorithm

1. Initialize `start = 0` and `maxLen = 1`.
2. Iterate through every index.
3. Expand around `(i, i)` for odd-length palindromes.
4. Expand around `(i, i + 1)` for even-length palindromes.
5. Continue while characters match and indexes are valid.
6. Update the longest palindrome.
7. Return `s.substr(start, maxLen)`.

### C++ Code

```cpp
class Solution {
public:
    void expand(string& s, int left, int right,
                int& start, int& maxLen) {

        while (left >= 0 &&
               right < s.length() &&
               s[left] == s[right]) {

            int len = right - left + 1;

            if (len > maxLen) {
                maxLen = len;
                start = left;
            }

            left--;
            right++;
        }
    }

    string longestPalindrome(string s) {
        int n = s.length();

        int start = 0;
        int maxLen = 1;

        for (int i = 0; i < n; i++) {
            // Odd-length palindrome
            expand(s, i, i, start, maxLen);

            // Even-length palindrome
            expand(s, i, i + 1, start, maxLen);
        }

        return s.substr(start, maxLen);
    }
};
```

### Complexity

- **Time:** O(n²)
- **Auxiliary Space:** O(1)

There are 2n possible centers, and each center can expand up to O(n) times.

## 5. Dry Run

Input:

```text
s = "babad"
```

Initially:

```text
start = 0
maxLen = 1
```

| Center | Palindrome Found | Maximum Length |
|---|---|---|
| 0 | "b" | 1 |
| 1 | "a", "bab" | 3 |
| 2 | "b", "aba" | 3 |
| 3 | "a" | 3 |
| 4 | "d" | 3 |

Final answer:

```text
"bab"
```

`"aba"` is also valid.

### Detailed Expansion of "bab"

Start with index 1:

```text
s = "babad"

left = 1
right = 1

Step 1:
s[1] = 'a'
Palindrome = "a"

Step 2:
left = 0
right = 2

s[0] == s[2]
'b' == 'b'

Palindrome = "bab"

Step 3:
left = -1
Expansion stops.
```

## 6. Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force | O(n³) | O(1) |
| Dynamic Programming | O(n²) | O(n²) |
| Expand Around Center | O(n²) | O(1) |

**Recommended:** Expand Around Center.

It provides O(n²) time complexity with O(1) auxiliary space.

## 7. Important Interview Questions

### Q1. Why do we check two centers?

Because palindromes can have odd or even lengths.

- Odd: `"aba"`
- Even: `"abba"`

Checking only `(i, i)` misses even-length palindromes.

### Q2. Why do we calculate `right - left + 1`?

Both indexes are inclusive.

For example:

```text
left = 1
right = 3

Length = 3 - 1 + 1 = 3
```

### Q3. Why use `s.substr(start, maxLen)`?

C++ `substr()` accepts a starting index and a length, not an ending index.

```cpp
s.substr(startingIndex, length);
```

### Q4. Why is the time complexity O(n²)?

There are approximately 2n possible centers.

Each center can expand up to O(n) times.

Therefore:

```text
O(2n * n) = O(n²)
```

### Q5. Can this problem be solved in O(n)?

Yes. Manacher's Algorithm solves it in O(n) time, but it is more complicated to implement.

### Q6. What if multiple longest palindromes exist?

The problem allows returning any valid longest palindrome.

Using `len > maxLen` preserves the first maximum found.

## 8. Common Mistakes

1. Checking only odd-length palindromes.
2. Forgetting boundary conditions during expansion.
3. Using `right - left` instead of `right - left + 1`.
4. Updating the answer before confirming characters match.
5. Confusing substring length with ending index.
6. Returning a subsequence instead of a contiguous substring.
7. Passing the ending index instead of the length to `substr()`.

## 9. Related Problems

| Problem | LeetCode | Pattern |
|---|---|---|
| Valid Palindrome | 125 | Two Pointers |
| Valid Palindrome II | 680 | Two Pointers |
| Longest Palindromic Substring | 5 | Expand Around Center |
| Palindromic Substrings | 647 | Expand Around Center |
| Longest Palindromic Subsequence | 516 | Dynamic Programming |

## 10. Final Revision

1. Every palindrome has a center.
2. Odd-length center: `(i, i)`.
3. Even-length center: `(i, i + 1)`.
4. Expand while characters match.
5. Update `start` and `maxLen`.
6. Return `s.substr(start, maxLen)`.

**Final Recommended Approach:** Expand Around Center

- **Time Complexity:** O(n²)
- **Auxiliary Space:** O(1)

**Interview Takeaway:** Whenever a problem asks for the longest palindromic substring, first consider expanding around every possible center.
# Palindromic Substrings (LeetCode 647)

- **Difficulty:** Medium
- **Topics:** String, Two Pointers, Dynamic Programming
- **Pattern:** Expand Around Center

## 1. Problem Understanding

Given a string `s`, return the total number of substrings that are palindromes.

A palindrome is a string that reads the same forward and backward.

**Important:** Different substrings are counted separately even if their contents are identical.

### Examples

**Example 1**

```text
Input: s = "abc"
Output: 3

Explanation:
"a", "b", "c"
```

**Example 2**

```text
Input: s = "aaa"
Output: 6

Explanation:
"a", "a", "a", "aa", "aa", "aaa"
```

## 2. Brute Force Approach

### Idea

1. Generate every possible substring.
2. Check whether each substring is a palindrome.
3. If it is a palindrome, increment the count.
4. Return the total count.

### C++ Code

```cpp
class Solution {
public:
    bool isPalindrome(string& s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) {
                return false;
            }

            left++;
            right--;
        }

        return true;
    }

    int countSubstrings(string s) {
        int n = s.length();
        int count = 0;

        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {

                if (isPalindrome(s, i, j)) {
                    count++;
                }
            }
        }

        return count;
    }
};
```

### Complexity Analysis

- **Time:** O(n³)
  - O(n²) possible substrings.
  - O(n) to check each substring.
- **Auxiliary Space:** O(1)

### Limitation

We repeatedly check overlapping substrings, which makes the solution inefficient.

We can optimize this using Expand Around Center.

## 3. Optimized Approach: Expand Around Center

### Key Observation

Every palindrome has a center.

There are two types of palindromes:

**1. Odd-length palindrome**

Example: `"aba"`

```text
    a
   / \
  b   b
```

The center is one character.

We expand from:

```cpp
expand(s, i, i);
```

**2. Even-length palindrome**

Example: `"abba"`

```text
   b   b
  /     \
 a       a
```

The center consists of two characters.

We expand from:

```cpp
expand(s, i, i + 1);
```

Therefore, for every index, we check both odd and even centers.

### Algorithm

1. Initialize `count = 0`.
2. Iterate through every index of the string.
3. Expand around `(i, i)` for odd-length palindromes.
4. Expand around `(i, i + 1)` for even-length palindromes.
5. While the indexes are valid and both characters match:
   - Increment the count.
   - Move the left pointer backward.
   - Move the right pointer forward.
6. Return the total count.

### C++ Code

```cpp
class Solution {
public:
    void expand(string& s, int left, int right, int& count) {

        while (left >= 0 &&
               right < s.length() &&
               s[left] == s[right]) {

            count++;

            left--;
            right++;
        }
    }

    int countSubstrings(string s) {
        int n = s.length();
        int count = 0;

        for (int i = 0; i < n; i++) {

            // Odd-length palindromes
            expand(s, i, i, count);

            // Even-length palindromes
            expand(s, i, i + 1, count);
        }

        return count;
    }
};
```

### Complexity Analysis

- **Time:** O(n²)
- **Auxiliary Space:** O(1)

There are approximately 2n possible centers, and each center can expand up to O(n) times.

## 4. Dry Run

Input:

```text
s = "aaa"
```

Initially:

```text
count = 0
```

We check odd and even centers.

| Center | Palindromes Found | Count |
|---|---|---|
| 0 | "a", "aa" | 2 |
| 1 | "a", "aa", "aaa" | 5 |
| 2 | "a" | 6 |

Final output:

```text
6
```

### Detailed Expansion

For `s = "aaa"`:

**Center at index 0 — Odd**

```text
left = 0, right = 0

s[0] == s[0]
Palindrome = "a"
count = 1

Move outward:
left = -1
Stop.
```

**Center at index 0 — Even**

```text
left = 0, right = 1

s[0] == s[1]
Palindrome = "aa"
count = 2

Move outward:
left = -1
Stop.
```

**Center at index 1 — Odd**

```text
left = 1, right = 1

Palindrome = "a"
count = 3

Expand:
left = 0, right = 2

s[0] == s[2]
Palindrome = "aaa"
count = 4

Move outward:
left = -1
Stop.
```

**Center at index 1 — Even**

```text
left = 1, right = 2

Palindrome = "aa"
count = 5
```

**Center at index 2 — Odd**

```text
left = 2, right = 2

Palindrome = "a"
count = 6
```

Final answer:

```text
6
```

## 5. Dynamic Programming Approach

### Idea

Use a 2D DP table to remember whether a substring is a palindrome.

### DP State

```cpp
dp[i][j]
```

Represents whether the substring from index `i` to `j` is a palindrome.

### Base Cases

Every single character is a palindrome:

```cpp
dp[i][i] = true;
```

For two characters:

```cpp
dp[i][i + 1] = (s[i] == s[i + 1]);
```

### Recurrence Relation

A substring is a palindrome when:

1. Its first and last characters are equal.
2. Its inner substring is also a palindrome.

```text
dp[i][j] = (s[i] == s[j]) && dp[i + 1][j - 1]
```

For a substring of length 2, matching characters are sufficient.

### C++ Code

```cpp
class Solution {
public:
    int countSubstrings(string s) {
        int n = s.length();

        vector<vector<bool>> dp(n, vector<bool>(n, false));

        int count = 0;

        for (int len = 1; len <= n; len++) {

            for (int i = 0; i + len <= n; i++) {

                int j = i + len - 1;

                if (s[i] == s[j]) {

                    if (len <= 2 || dp[i + 1][j - 1]) {

                        dp[i][j] = true;
                        count++;
                    }
                }
            }
        }

        return count;
    }
};
```

### Complexity Analysis

- **Time:** O(n²)
- **Auxiliary Space:** O(n²)

## 6. Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force | O(n³) | O(1) |
| Dynamic Programming | O(n²) | O(n²) |
| Expand Around Center | O(n²) | O(1) |

**Recommended Approach:** Expand Around Center.

It provides O(n²) time complexity with O(1) auxiliary space.

## 7. Important Interview Questions

### Q1. Why do we check two centers?

Because palindromes can have odd or even lengths.

- Odd: `"aba"`
- Even: `"abba"`

Checking only `(i, i)` misses even-length palindromes.

### Q2. Why do we increment count inside the while loop?

Every successful expansion represents one new palindromic substring.

For example:

```text
s = "aaa"

Center at index 1:

"a"
"aaa"
```

Each valid expansion contributes one palindrome.

### Q3. Why do duplicate substrings count separately?

The problem counts substrings based on their positions, not unique string values.

For `"aaa"`:

```text
Index 0: "a"
Index 1: "a"
Index 2: "a"
```

These are three different substrings.

### Q4. Why is the time complexity O(n²)?

There are approximately 2n possible centers.

Each center can expand up to O(n) times.

Therefore:

```text
O(2n * n) = O(n²)
```

### Q5. What is the difference between LeetCode 5 and LeetCode 647?

| Problem | Requirement |
|---|---|
| Longest Palindromic Substring (5) | Find the longest palindrome |
| Palindromic Substrings (647) | Count all palindromic substrings |

Both problems use the same Expand Around Center technique.

## 8. Common Mistakes

1. Checking only odd-length palindromes.
2. Forgetting to check even-length palindromes.
3. Incrementing count only once per center instead of once per valid expansion.
4. Forgetting boundary conditions.
5. Counting only unique palindrome values.
6. Moving pointers in the wrong direction.
7. Using `left <= right` without checking whether the characters match.

## 9. Related Problems

| Problem | LeetCode | Pattern |
|---|---|---|
| Longest Palindromic Substring | 5 | Expand Around Center |
| Palindromic Substrings | 647 | Expand Around Center |
| Valid Palindrome | 125 | Two Pointers |
| Valid Palindrome II | 680 | Two Pointers |
| Longest Palindromic Subsequence | 516 | Dynamic Programming |

## 10. Final Revision Notes

Remember:

1. Every palindrome has a center.
2. Odd-length center: `(i, i)`.
3. Even-length center: `(i, i + 1)`.
4. Expand while characters match.
5. Increment count for every successful expansion.
6. Return the total count.

**Final Recommended Approach:** Expand Around Center

- **Time Complexity:** O(n²)
- **Auxiliary Space:** O(1)

**Interview Takeaway:** LeetCode 5 and LeetCode 647 share the same palindrome expansion pattern. The difference is that one tracks the maximum length while the other counts every valid expansion.
# Decode Ways (LeetCode 91)

- **Difficulty:** Medium
- **Topics:** String, Dynamic Programming
- **Pattern:** 1D Dynamic Programming
- **Related Pattern:** Climbing Stairs

## 1. Problem Understanding

A string containing digits can be decoded into letters using the following mapping:

```text
'A' -> "1"
'B' -> "2"
'C' -> "3"
...
'Z' -> "26"
```

Given a string `s` containing only digits, return the number of ways to decode it.

### Important Rules

1. A single digit from `1` to `9` can be decoded into a letter.
2. Two consecutive digits can be decoded only if their value is between `10` and `26`.
3. A number with a leading zero is invalid.
4. The digit `0` cannot be decoded independently.

### Examples

**Example 1**

```text
Input: s = "12"
Output: 2

Explanation:
1. "1" + "2" = "AB"
2. "12" = "L"
```

**Example 2**

```text
Input: s = "226"
Output: 3

Explanation:
1. "2" + "2" + "6" = "BBF"
2. "22" + "6" = "VF"
3. "2" + "26" = "BZ"
```

**Example 3**

```text
Input: s = "06"
Output: 0

Explanation:
"06" is invalid because numbers cannot have a leading zero.
```

## 2. Brute Force Approach — Recursion

### Idea

At every index, we have at most two choices:

- Take one digit if it is not zero.
- Take two digits if their value is between 10 and 26.

We recursively explore both possibilities and count all valid decoding paths.

### C++ Code

```cpp
class Solution {
public:
    int solve(string& s, int index) {

        // Successfully decoded the entire string
        if (index == s.length()) {
            return 1;
        }

        // A single zero cannot be decoded
        if (s[index] == '0') {
            return 0;
        }

        // Choice 1: Take one digit
        int ways = solve(s, index + 1);

        // Choice 2: Take two digits
        if (index + 1 < s.length()) {

            int num = stoi(s.substr(index, 2));

            if (num >= 10 && num <= 26) {
                ways += solve(s, index + 2);
            }
        }

        return ways;
    }

    int numDecodings(string s) {
        return solve(s, 0);
    }
};
```

### Complexity Analysis

- **Time:** O(2^n) in the worst case.
- **Auxiliary Space:** O(n) recursion stack.

### Limitation

The same indexes can be calculated repeatedly.

For example, different recursive paths may reach the same index and solve the same subproblem again.

We can eliminate this repeated work using Dynamic Programming.

## 3. Optimized Approach — Top-Down DP (Memoization)

### Key Observation

The number of ways to decode the remaining string depends only on the current index.

Therefore, we can store the result for every index and reuse it.

### DP State

```cpp
dp[i]
```

Represents the number of ways to decode the string starting from index `i`.

### Base Cases

**1. Successfully decoded the complete string**

```cpp
if (i == n) return 1;
```

There is exactly one way to complete an already-decoded string: do nothing more.

**2. Current digit is zero**

```cpp
if (s[i] == '0') return 0;
```

A standalone zero cannot be decoded.

### Recurrence Relation

We have two possible choices.

**Choice 1: Take one digit**

If `s[i] != '0'`:

```text
dp[i] += dp[i + 1]
```

**Choice 2: Take two digits**

If the next two digits form a number between 10 and 26:

```text
dp[i] += dp[i + 2]
```

Therefore:

```text
dp[i] = dp[i + 1] + dp[i + 2]
```

This recurrence applies only when the corresponding one-digit and two-digit choices are valid.

### C++ Code

```cpp
class Solution {
public:
    int solve(string& s, int i, vector<int>& dp) {

        int n = s.length();

        if (i == n) {
            return 1;
        }

        if (s[i] == '0') {
            return 0;
        }

        if (dp[i] != -1) {
            return dp[i];
        }

        // Choice 1: Decode one digit
        int ways = solve(s, i + 1, dp);

        // Choice 2: Decode two digits
        if (i + 1 < n) {

            int num = (s[i] - '0') * 10 + (s[i + 1] - '0');

            if (num >= 10 && num <= 26) {
                ways += solve(s, i + 2, dp);
            }
        }

        return dp[i] = ways;
    }

    int numDecodings(string s) {

        int n = s.length();

        vector<int> dp(n, -1);

        return solve(s, 0, dp);
    }
};
```

### Complexity Analysis

- **Time:** O(n)
- **Auxiliary Space:** O(n) for the DP array and recursion stack.

Every index is calculated only once.

## 4. Bottom-Up DP — Tabulation

### Idea

Instead of solving the problem recursively, we calculate the number of decoding ways from right to left.

We use the same DP state:

```cpp
dp[i] = Number of ways to decode s starting at index i.
```

### Base Case

```cpp
dp[n] = 1;
```

When we reach the end of the string, we have successfully decoded it.

### Transition

For every index `i`:

**1. Single-digit choice**

If `s[i] != '0'`:

```cpp
dp[i] += dp[i + 1];
```

**2. Two-digit choice**

If the two-digit number is between 10 and 26:

```cpp
dp[i] += dp[i + 2];
```

### C++ Code

```cpp
class Solution {
public:
    int numDecodings(string s) {

        int n = s.length();

        vector<int> dp(n + 1, 0);

        dp[n] = 1;

        for (int i = n - 1; i >= 0; i--) {

            if (s[i] == '0') {
                dp[i] = 0;
            }
            else {

                // Choice 1: Take one digit
                dp[i] = dp[i + 1];

                // Choice 2: Take two digits
                if (i + 1 < n) {

                    int num = (s[i] - '0') * 10 +
                              (s[i + 1] - '0');

                    if (num >= 10 && num <= 26) {
                        dp[i] += dp[i + 2];
                    }
                }
            }
        }

        return dp[0];
    }
};
```

### Complexity Analysis

- **Time:** O(n)
- **Auxiliary Space:** O(n)

## 5. Dry Run

Input:

```text
s = "226"
```

We create a DP array of size `n + 1`.

Initially:

```text
dp[4] = 1
```

We calculate from right to left.

### Step 1: Index 2

Current digit:

```text
s[2] = '6'
```

It can be decoded as one digit.

```text
dp[3] = 1
```

Therefore:

```text
dp[2] = dp[3] = 1
```

### Step 2: Index 1

Current digit:

```text
s[1] = '2'
```

**Choice 1: Take one digit**

```text
2 + 6 = "26"
```

The remaining decoding ways are:

```text
dp[2] = 1
```

**Choice 2: Take two digits**

```text
26
```

This is valid because 26 is within the range 10 to 26.

The remaining decoding ways are:

```text
dp[3] = 1
```

Therefore:

```text
dp[1] = dp[2] + dp[3]

dp[1] = 1 + 1 = 2
```

### Step 3: Index 0

Current digit:

```text
s[0] = '2'
```

**Choice 1: Take one digit**

```text
2
```

Remaining ways:

```text
dp[1] = 2
```

**Choice 2: Take two digits**

```text
22
```

This is valid.

Remaining ways:

```text
dp[2] = 1
```

Therefore:

```text
dp[0] = dp[1] + dp[2]

dp[0] = 2 + 1 = 3
```

### Final DP Array

```text
Index:  0  1  2  3
String: 2  2  6  END

DP:     3  2  1  1
```

Final answer:

```text
Output: 3
```

## 6. Space Optimized DP — O(1)

### Key Observation

In tabulation, we only need:

```text
dp[i + 1]
dp[i + 2]
```

We do not need the entire DP array.

Therefore, we can maintain two variables:

- `next1` = dp[i + 1]
- `next2` = dp[i + 2]

### C++ Code

```cpp
class Solution {
public:
    int numDecodings(string s) {

        int n = s.length();

        int next1 = 1;  // dp[n]
        int next2 = 0;  // dp[n + 1], initially unused

        for (int i = n - 1; i >= 0; i--) {

            int curr = 0;

            if (s[i] != '0') {

                // Choice 1: Take one digit
                curr = next1;

                // Choice 2: Take two digits
                if (i + 1 < n) {

                    int num = (s[i] - '0') * 10 +
                              (s[i + 1] - '0');

                    if (num >= 10 && num <= 26) {
                        curr += next2;
                    }
                }
            }

            next2 = next1;
            next1 = curr;
        }

        return next1;
    }
};
```

### Complexity Analysis

- **Time:** O(n)
- **Auxiliary Space:** O(1)

**Recommended interview solution:** Bottom-Up DP or Space Optimized DP.

## 7. Important Edge Cases

| Input | Output | Reason |
|---|---:|---|
| `"12"` | 2 | 1+2 or 12 |
| `"226"` | 3 | Three valid groupings |
| `"0"` | 0 | Zero cannot be decoded |
| `"06"` | 0 | Leading zero is invalid |
| `"10"` | 1 | Only 10 is valid |
| `"20"` | 1 | Only 20 is valid |
| `"27"` | 1 | 27 is invalid as a pair |
| `"111"` | 3 | Three valid groupings |
| `"1111"` | 5 | Five valid groupings |
| `"101"` | 1 | Only 10+1 is valid |
| `"100"` | 0 | Trailing zero cannot be decoded |

## 8. Approach Comparison

| Approach | Time | Auxiliary Space |
|---|---|---|
| Recursion | O(2^n) | O(n) |
| Memoization | O(n) | O(n) |
| Tabulation | O(n) | O(n) |
| Space Optimized DP | O(n) | O(1) |

## 9. Common Mistakes

1. Treating `0` as a valid standalone character.
2. Accepting two-digit values greater than 26.
3. Accepting numbers with leading zero, such as `06`.
4. Forgetting the base case `dp[n] = 1`.
5. Using `dp[i + 2]` without checking whether `i + 1` exists.
6. Confusing the number of decoding paths with the number of unique decoded strings.
7. Updating space-optimized variables in the wrong order.

## 10. Important Interview Insights

### Q1. Why is `dp[n] = 1` instead of 0?

Reaching the end means we have successfully decoded the entire string.

There is one valid way to complete the decoding: stop.

### Q2. Why can't zero be decoded independently?

The mapping starts from 1:

```text
A = 1
...
Z = 26
```

There is no mapping for 0.

### Q3. Why is 10 valid but 06 invalid?

```text
10 -> J
```

But:

```text
06 -> Invalid
```

A two-digit number must not have a leading zero.

### Q4. Why is Decode Ways similar to Climbing Stairs?

Both problems use choices that advance the current index by one or two positions.

**Climbing Stairs:**

```text
dp[i] = dp[i + 1] + dp[i + 2]
```

**Decode Ways:**

```text
dp[i] = dp[i + 1] + dp[i + 2]
```

However, Decode Ways has additional validity conditions:

- A single digit must not be zero.
- A pair must be between 10 and 26.

### Q5. When should we use memoization instead of tabulation?

- Memoization is useful when the recursive choices are easier to understand.
- Tabulation avoids recursion overhead.
- Space optimization is possible when only a fixed number of previous states are required.

## 11. Related Problems

| Problem | LeetCode | Pattern |
|---|---|---|
| Climbing Stairs | 70 | 1D DP |
| Min Cost Climbing Stairs | 746 | 1D DP |
| Decode Ways | 91 | 1D DP |
| House Robber | 198 | 1D DP |
| House Robber II | 213 | Circular 1D DP |

## 12. Final Revision Notes

Remember these points:

1. Every index represents a subproblem.
2. A single digit is valid only from 1 to 9.
3. A two-digit number is valid only from 10 to 26.
4. A zero cannot be decoded independently.
5. Base case: `dp[n] = 1`.
6. Transition: add the valid one-digit and two-digit choices.
7. Only the next two DP states are needed.

**Best Approach:** Space Optimized Dynamic Programming

- **Time Complexity:** O(n)
- **Auxiliary Space:** O(1)

**Interview Takeaway:** Decode Ways follows the Climbing Stairs DP pattern, but every transition must satisfy the decoding rules.

# Coin Change (LeetCode 322)

- **Difficulty:** Medium
- **Topics:** Array, Dynamic Programming, Breadth-First Search
- **Pattern:** Unbounded Knapsack
- **Goal:** Find the minimum number of coins required to make the target amount.

## 1. Problem Understanding

You are given:

- An integer array `coins`, representing available coin denominations.
- An integer `amount`, representing the target amount.

Return the **minimum number of coins** required to make exactly the target amount.

If the amount cannot be formed using the available coins, return `-1`.

### Important Conditions

1. You can use unlimited coins of each denomination.
2. You do not need to use every available denomination.
3. You must form the exact target amount.
4. Return the minimum number of coins, not the number of possible combinations.

### Examples

**Example 1**

```text
Input: coins = [1,5,10], amount = 12

Output: 3

Explanation:
12 = 10 + 1 + 1

Minimum coins required = 3
```

**Example 2**

```text
Input: coins = [2], amount = 3

Output: -1

Explanation:
We cannot form amount 3 using only coins of denomination 2.
```

**Example 3**

```text
Input: coins = [1], amount = 0

Output: 0

Explanation:
No coins are required to form amount 0.
```

## 2. Brute Force Approach — Recursion

### Idea

At every step, try selecting each available coin.

After selecting a coin:

```text
Remaining Amount = amount - coin
```

We recursively calculate the minimum coins needed for the remaining amount.

We take the minimum among all possible choices.

### Base Cases

**Case 1: Amount becomes zero**

```cpp
if (amount == 0) return 0;
```

No additional coins are required.

**Case 2: Amount becomes negative**

```cpp
if (amount < 0) return INT_MAX;
```

This combination is invalid.

### C++ Code

```cpp
class Solution {
public:
    int solve(vector<int>& coins, int amount) {

        if (amount == 0) {
            return 0;
        }

        if (amount < 0) {
            return INT_MAX;
        }

        int mini = INT_MAX;

        for (int coin : coins) {

            int result = solve(coins, amount - coin);

            if (result != INT_MAX) {
                mini = min(mini, 1 + result);
            }
        }

        return mini;
    }

    int coinChange(vector<int>& coins, int amount) {

        int ans = solve(coins, amount);

        return ans == INT_MAX ? -1 : ans;
    }
};
```

### Complexity Analysis

Let `C` be the number of coin denominations and `A` be the target amount.

- **Time:** Exponential in the worst case.
- **Auxiliary Space:** O(A / minimum coin value) recursion depth.

### Limitation

The same remaining amounts are calculated repeatedly.

For example, multiple recursive paths may reach the same remaining amount.

We can avoid repeated calculations using Dynamic Programming.

## 3. Optimized Approach — Top-Down DP (Memoization)

### Key Observation

The minimum coins required depend only on the remaining amount.

If we have already calculated the answer for a particular amount, we can reuse it.

### DP State

```cpp
dp[amount]
```

Represents the minimum number of coins required to form that amount.

### Base Cases

```cpp
if (amount == 0) return 0;
```

No coins are needed.

```cpp
if (amount < 0) return INT_MAX;
```

An invalid amount cannot be formed.

### Recurrence Relation

For every available coin:

```text
result = solve(amount - coin)
```

If the remaining amount is achievable:

```text
mini = min(mini, 1 + result)
```

The `+1` represents the coin we just selected.

### C++ Code

```cpp
class Solution {
public:
    int solve(vector<int>& coins, int amount,
              vector<int>& dp) {

        if (amount == 0) {
            return 0;
        }

        if (amount < 0) {
            return INT_MAX;
        }

        if (dp[amount] != -1) {
            return dp[amount];
        }

        int mini = INT_MAX;

        for (int coin : coins) {

            int result = solve(coins, amount - coin, dp);

            if (result != INT_MAX) {
                mini = min(mini, 1 + result);
            }
        }

        return dp[amount] = mini;
    }

    int coinChange(vector<int>& coins, int amount) {

        vector<int> dp(amount + 1, -1);

        int ans = solve(coins, amount, dp);

        return ans == INT_MAX ? -1 : ans;
    }
};
```

### Complexity Analysis

- **Time:** O(C × A)
- **Auxiliary Space:** O(A) for the memoization array and recursion stack.

Every remaining amount is solved at most once, and each state checks all available coins.

## 4. Bottom-Up DP — Tabulation

### Idea

Instead of solving recursively, we calculate the minimum number of coins required for every amount from `0` to the target.

### DP State

```cpp
dp[i]
```

Represents the minimum number of coins required to make amount `i`.

### Initialization

Initially, assume every amount is impossible to form.

```cpp
vector<int> dp(amount + 1, INT_MAX);

dp[0] = 0;
```

Why?

- Amount `0` requires zero coins.
- Every other amount is initially considered unreachable.

### Transition

For every amount, try every available coin.

If the coin can be used:

```text
i >= coin
```

Then:

```text
dp[i] = min(dp[i], 1 + dp[i - coin])
```

We must also ensure that `dp[i - coin]` is reachable before adding one.

### C++ Code

```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {

        vector<int> dp(amount + 1, INT_MAX);

        dp[0] = 0;

        for (int i = 1; i <= amount; i++) {

            for (int coin : coins) {

                if (i >= coin && dp[i - coin] != INT_MAX) {

                    dp[i] = min(dp[i], 1 + dp[i - coin]);
                }
            }
        }

        return dp[amount] == INT_MAX ? -1 : dp[amount];
    }
};
```

### Complexity Analysis

- **Time:** O(C × A)
- **Auxiliary Space:** O(A)

Where:
- `C` = number of coin denominations.
- `A` = target amount.

## 5. Dry Run

Input:

```text
coins = [1, 5, 10]
amount = 12
```

We create a DP array of size `amount + 1`.

Initially:

```text
dp = [0, INF, INF, INF, INF, INF, INF,
      INF, INF, INF, INF, INF, INF]
```

Here, `INF` represents an amount that has not yet been formed.

### Step 1: Amount = 1

Available coins: `[1, 5, 10]`

Only coin `1` can be used.

```text
dp[1] = 1 + dp[0]
      = 1
```

### Step 2: Amount = 2

Use coin `1`.

```text
dp[2] = 1 + dp[1]
      = 2
```

### Step 3: Amount = 5

Possible choices:

Using coin `1`:

```text
1 + dp[4] = 5
```

Using coin `5`:

```text
1 + dp[0] = 1
```

Minimum:

```text
dp[5] = 1
```

### Step 4: Amount = 10

Possible choices:

Using coin `1`:

```text
1 + dp[9] = 6
```

Using coin `5`:

```text
1 + dp[5] = 2
```

Using coin `10`:

```text
1 + dp[0] = 1
```

Minimum:

```text
dp[10] = 1
```

### Step 5: Amount = 12

Possible choices:

Using coin `1`:

```text
1 + dp[11] = 3
```

Using coin `5`:

```text
1 + dp[7] = 3
```

Using coin `10`:

```text
1 + dp[2] = 3
```

Minimum:

```text
dp[12] = 3
```

### Final DP Array

```text
Amount: 0  1  2  3  4  5  6  7  8  9  10 11 12
DP:     0  1  2  3  4  1  2  3  4  5   1  2  3
```

Final answer:

```text
Output: 3
```

One optimal combination:

```text
10 + 1 + 1 = 12
```

## 6. Important Concept — Why Is This Unbounded Knapsack?

Coin Change is a variation of the **Unbounded Knapsack** problem.

In this problem:

- Each coin represents an item.
- Each coin has a value equal to its denomination.
- We can select the same coin unlimited times.
- Our objective is to minimize the number of selected coins.

### 0/1 Knapsack vs Unbounded Knapsack

| Feature | 0/1 Knapsack | Unbounded Knapsack |
|---|---|---|
| Item usage | At most once | Unlimited times |
| Reuse item | No | Yes |
| Typical DP transition | Previous item state | Current item can be reused |
| Example | Subset Sum | Coin Change |

### Why Can We Reuse the Same Coin?

Consider:

```text
coins = [1, 5, 10]
amount = 12
```

We can select coin `1` repeatedly:

```text
10 + 1 + 1 = 12
```

This is allowed because the problem provides an unlimited number of each denomination.

## 7. Alternative — Space Optimized DP (1D Array)

The tabulation solution already uses a one-dimensional DP array.

We cannot generally reduce it to O(1) auxiliary space because we need results for multiple amounts.

However, we can use a different loop arrangement that makes the unbounded-choice pattern more explicit.

### C++ Code

```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {

        vector<int> dp(amount + 1, amount + 1);

        dp[0] = 0;

        for (int coin : coins) {

            for (int i = coin; i <= amount; i++) {

                dp[i] = min(dp[i], 1 + dp[i - coin]);
            }
        }

        return dp[amount] > amount ? -1 : dp[amount];
    }
};
```

### Complexity Analysis

- **Time:** O(C × A)
- **Auxiliary Space:** O(A)

**Important:** The inner loop runs forward because the same coin can be reused unlimited times.

This loop arrangement is also used in the unbounded knapsack pattern.

## 8. Common Mistakes

1. Forgetting that unlimited coins are available.
2. Returning the number of combinations instead of the minimum number of coins.
3. Forgetting to initialize `dp[0] = 0`.
4. Adding one to `INT_MAX`, which can cause integer overflow.
5. Forgetting to return `-1` when the target amount is unreachable.
6. Using a backward inner loop when implementing the unbounded knapsack version.
7. Confusing Coin Change (minimum coins) with Coin Change II (number of combinations).

## 9. Coin Change vs Coin Change II

These two problems look similar but have different objectives.

| Feature | Coin Change (322) | Coin Change II (518) |
|---|---|---|
| Objective | Minimum number of coins | Number of combinations |
| DP operation | `min()` | Addition |
| Impossible amount | Return `-1` | Return `0` |
| Pattern | Unbounded Knapsack | Unbounded Knapsack |

### Remember

**Coin Change:**

```cpp
dp[i] = min(dp[i], 1 + dp[i - coin]);
```

**Coin Change II:**

```cpp
dp[i] += dp[i - coin];
```

## 10. Important Interview Insights

### Q1. Why do we initialize DP with INT_MAX?

Because we are looking for the minimum number of coins.

Initially, every amount is treated as unreachable. A valid transition replaces that value with a smaller achievable count.

### Q2. Why do we use `dp[0] = 0`?

Zero coins are needed to form an amount of zero.

This is the foundation for building all other DP states.

### Q3. Why do we check `dp[i - coin] != INT_MAX`?

To avoid adding one to an unreachable state and prevent integer overflow.

### Q4. Why is this an unbounded knapsack problem?

Because we can select each coin denomination unlimited times.

### Q5. Can a greedy approach solve Coin Change?

Not always.

For example:

```text
coins = [1, 3, 4]
amount = 6
```

A greedy strategy chooses:

```text
4 + 1 + 1 = 6
```

Total coins = 3.

But the optimal solution is:

```text
3 + 3 = 6
```

Total coins = 2.

Therefore, a general greedy strategy does not guarantee the minimum number of coins.

## 11. Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Recursion | Exponential | O(A / minCoin) |
| Memoization | O(C × A) | O(A) |
| Tabulation | O(C × A) | O(A) |
| 1D DP | O(C × A) | O(A) |

## 12. Final Revision Notes

Remember these points:

1. Coin Change is an Unbounded Knapsack problem.
2. Each coin can be used unlimited times.
3. `dp[i]` represents the minimum coins needed to form amount `i`.
4. Initialize `dp[0] = 0`.
5. Use `INT_MAX` to represent unreachable amounts.
6. Transition:

   ```cpp
   dp[i] = min(dp[i], 1 + dp[i - coin]);
   ```

7. Check that the previous state is reachable before adding one.
8. Return `-1` if the target amount is unreachable.
9. A greedy approach is not always optimal.

**Best Approach:** Bottom-Up Dynamic Programming

- **Time Complexity:** O(C × A)
- **Auxiliary Space:** O(A)

**Interview Takeaway:** Whenever an item can be selected unlimited times, consider the Unbounded Knapsack pattern.

# Maximum Product Subarray (LeetCode 152)

- **Difficulty:** Medium
- **Topics:** Array, Dynamic Programming
- **Pattern:** Kadane's Algorithm Variation
- **Goal:** Find a contiguous subarray with the maximum product.

## 1. Problem Understanding

Given an integer array `nums`, find a non-empty contiguous subarray whose product is maximum.

Return the maximum product.

### Important Conditions

1. A subarray must contain consecutive elements.
2. The subarray must be non-empty.
3. The array can contain positive numbers, negative numbers and zero.
4. A single element is also a valid subarray.
5. The answer fits in a 32-bit integer.

### Examples

**Example 1**

```text
Input: nums = [2,4,-3,5]

Output: 8

Explanation:
The subarray [2,4] has the maximum product.

2 * 4 = 8
```

**Example 2**

```text
Input: nums = [-3,0,-2]

Output: 0

Explanation:
The maximum product is 0.
```

**Example 3**

```text
Input: nums = [-2,3,-4]

Output: 24

Explanation:
The subarray [-2,3,-4] has product:

-2 * 3 * -4 = 24
```

## 2. Brute Force Approach — Generate All Subarrays

### Idea

Generate every possible contiguous subarray and calculate its product.

For every starting index `i`:

1. Initialize the product as 1.
2. Extend the subarray using index `j`.
3. Multiply the current product by `nums[j]`.
4. Update the maximum product.

### C++ Code

```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {

        int n = nums.size();
        int ans = INT_MIN;

        for (int i = 0; i < n; i++) {

            int product = 1;

            for (int j = i; j < n; j++) {

                product *= nums[j];

                ans = max(ans, product);
            }
        }

        return ans;
    }
};
```

### Complexity Analysis

- **Time:** O(n²)
- **Auxiliary Space:** O(1)

### Limitation

We calculate the product of many overlapping subarrays repeatedly.

We can optimize this using the observation that negative numbers can change the maximum and minimum product.

## 3. Why Do We Need Both Maximum and Minimum Products?

This is the most important concept in this problem.

In normal Kadane's Algorithm, we track the maximum sum ending at the current index.

But multiplication behaves differently because of negative numbers.

### Key Observation

Consider:

```text
nums = [-2, 3, -4]
```

After processing the first two elements:

```text
-2 * 3 = -6
```

The current maximum product is 3, while the minimum product is -6.

Now multiply both by -4:

```text
3 * -4 = -12

-6 * -4 = 24
```

Notice:

- The previous maximum becomes negative.
- The previous minimum becomes the new maximum.

Therefore, tracking only the maximum product is insufficient.

### We Must Track Two Values

1. `maxProduct`: Maximum product of a subarray ending at the current index.
2. `minProduct`: Minimum product of a subarray ending at the current index.

Why minimum?

Because a negative number multiplied by another negative number can become a large positive number.

## 4. Optimized Approach — Dynamic Programming

### Key Observation

At every index, there are three possibilities:

1. Start a new subarray from the current element.
2. Extend the previous maximum product.
3. Extend the previous minimum product.

For the current number `num`, calculate:

```text
currentMax = max(num, num * previousMax, num * previousMin)

currentMin = min(num, num * previousMax, num * previousMin)
```

We use the previous maximum and minimum values for both calculations.

### DP State

```cpp
maxProduct
```

Maximum product of a subarray ending at the current index.

```cpp
minProduct
```

Minimum product of a subarray ending at the current index.

```cpp
ans
```

Maximum product found anywhere in the array.

### Initialization

Initialize all three variables using the first element:

```cpp
int maxProduct = nums[0];
int minProduct = nums[0];
int ans = nums[0];
```

This ensures that a single-element subarray is considered.

### C++ Code

```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {

        int maxProduct = nums[0];
        int minProduct = nums[0];

        int ans = nums[0];

        for (int i = 1; i < nums.size(); i++) {

            int num = nums[i];

            int tempMax = max({
                num,
                num * maxProduct,
                num * minProduct
            });

            int tempMin = min({
                num,
                num * maxProduct,
                num * minProduct
            });

            maxProduct = tempMax;
            minProduct = tempMin;

            ans = max(ans, maxProduct);
        }

        return ans;
    }
};
```

### Complexity Analysis

- **Time:** O(n)
- **Auxiliary Space:** O(1)

Each element is processed exactly once.

## 5. Dry Run

Input:

```text
nums = [-2, 3, -4]
```

### Initialization

```text
maxProduct = -2
minProduct = -2
ans = -2
```

### Step 1: Index 1

Current element:

```text
num = 3
```

Calculate the three possibilities:

```text
1. Start new subarray:

   3

2. Extend previous maximum:

   3 * -2 = -6

3. Extend previous minimum:

   3 * -2 = -6
```

Therefore:

```text
maxProduct = 3
minProduct = -6
ans = 3
```

### Step 2: Index 2

Current element:

```text
num = -4
```

Calculate the three possibilities:

```text
1. Start new subarray:

   -4

2. Extend previous maximum:

   -4 * 3 = -12

3. Extend previous minimum:

   -4 * -6 = 24
```

Therefore:

```text
maxProduct = 24
minProduct = -12
ans = 24
```

### Final Result

| Index | Number | Maximum Product | Minimum Product | Overall Answer |
|---|---:|---:|---:|---:|
| 0 | -2 | -2 | -2 | -2 |
| 1 | 3 | 3 | -6 | 3 |
| 2 | -4 | 24 | -12 | 24 |

```text
Output: 24
```

The maximum product comes from:

```text
[-2, 3, -4]

(-2) * 3 * (-4) = 24
```

## 6. Understanding the Role of Zero

Zero is important because multiplying any product by zero resets it to zero.

Consider:

```text
nums = [-2, 0, -3]
```

At zero:

```text
maxProduct = 0
minProduct = 0
```

The next element can start a new subarray.

The optimized recurrence handles zero automatically because the current element itself is one of the three candidates.

### Example

```text
nums = [2, 3, 0, 4, 5]
```

Products:

```text
[2, 3]       = 6
[0]          = 0
[4, 5]       = 20
```

Answer:

```text
20
```

## 7. Alternative — Sign-Based Approach

Another way to understand this problem is to observe that:

- An even number of negative values can produce a positive product.
- An odd number of negative values produces a negative product.
- Zero separates independent subarrays.

A prefix and suffix traversal can also solve the problem.

### Idea

1. Traverse from left to right and maintain a running product.
2. Traverse from right to left and maintain another running product.
3. Reset the running product to 1 whenever it becomes zero.
4. Track the maximum product encountered.

### C++ Code

```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {

        int n = nums.size();

        int prefix = 1;
        int suffix = 1;

        int ans = INT_MIN;

        for (int i = 0; i < n; i++) {

            if (prefix == 0) {
                prefix = 1;
            }

            if (suffix == 0) {
                suffix = 1;
            }

            prefix *= nums[i];
            suffix *= nums[n - 1 - i];

            ans = max(ans, max(prefix, suffix));
        }

        return ans;
    }
};
```

### Complexity Analysis

- **Time:** O(n)
- **Auxiliary Space:** O(1)

### When Is This Useful?

This approach is useful when thinking in terms of negative-number parity and zero-separated segments.

For interviews, the maximum/minimum DP approach is particularly useful because it clearly explains the state transitions.

## 8. Common Mistakes

1. Tracking only the maximum product.
2. Forgetting that a negative number can turn the previous minimum into the new maximum.
3. Forgetting to consider the current element as a new subarray.
4. Initializing the answer with zero, which fails when every element is negative.
5. Updating `maxProduct` before calculating `minProduct` and accidentally using the updated value.
6. Ignoring zeros.
7. Confusing a subarray with a subsequence.

## 9. Important Interview Insights

### Q1. Why do we track both maximum and minimum products?

Because multiplying by a negative number reverses the sign.

The previous minimum can become the new maximum.

### Q2. Why do we consider the current element separately?

A new subarray can begin at any index.

For example:

```text
nums = [0, 2, 3]
```

After zero, the best subarray starts again at 2.

### Q3. Why is this different from normal Kadane's Algorithm?

Normal Kadane's Algorithm tracks the maximum sum ending at each index.

Maximum Product Subarray must track both maximum and minimum products because negative multiplication changes their roles.

### Q4. Why do we initialize using nums[0]?

Because the subarray must be non-empty.

Initializing with zero can give an incorrect result for an array containing only negative numbers.

### Q5. What is the difference between a subarray and a subsequence?

**Subarray:** Elements must be contiguous.

```text
[2, 3] is a subarray of [1, 2, 3, 4].
```

**Subsequence:** Elements do not need to be contiguous.

```text
[1, 3, 4] is a subsequence of [1, 2, 3, 4].
```

## 10. Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Maximum/Minimum DP | O(n) | O(1) |
| Prefix/Suffix | O(n) | O(1) |

## 11. Final Revision Notes

Remember these points:

1. A subarray must be contiguous.
2. Negative numbers can change the maximum into the minimum and vice versa.
3. Track both `maxProduct` and `minProduct`.
4. Consider starting a new subarray at every index.
5. Zero automatically resets the product.
6. Initialize using the first array element.
7. Save previous maximum and minimum values before updating either state.

**Best Approach:** Dynamic Programming with Maximum and Minimum Product Tracking.

- **Time Complexity:** O(n)
- **Auxiliary Space:** O(1)

**Interview Takeaway:** Whenever multiplication and negative numbers are involved, consider tracking both the maximum and minimum states.

# Word Break (LeetCode 139)

- **Difficulty:** Medium
- **Topics:** Array, Hash Table, String, Dynamic Programming, Trie
- **Pattern:** 1D Dynamic Programming
- **Goal:** Determine whether a string can be segmented into dictionary words.

## 1. Problem Understanding

Given a string `s` and a dictionary of strings `wordDict`, return `true` if `s` can be segmented into a sequence of one or more dictionary words.

Otherwise, return `false`.

### Important Conditions

1. Every part of the string must exist in the dictionary.
2. Dictionary words can be reused unlimited times.
3. We do not need to use every word in the dictionary.
4. The complete string must be segmented without leaving any characters.

### Examples

**Example 1**

```text
Input:
s = "neetcode"
wordDict = ["neet", "code"]

Output: true

Explanation:
"neetcode" = "neet" + "code"
```

**Example 2**

```text
Input:
s = "applepenapple"
wordDict = ["apple", "pen", "ape"]

Output: true

Explanation:
"applepenapple" = "apple" + "pen" + "apple"

The word "apple" is reused.
```

**Example 3**

```text
Input:
s = "catsandog"
wordDict = ["cats", "dog", "sand", "and", "cat"]

Output: false
```

## 2. Brute Force Approach — Recursion

### Idea

Try every possible prefix of the string.

If the prefix exists in the dictionary:

1. Consider that prefix as one valid word.
2. Recursively check whether the remaining substring can also be segmented.
3. If any path successfully reaches the end of the string, return true.

### C++ Code

```cpp
class Solution {
public:

    unordered_set<string> dict;

    bool solve(int start, string& s) {

        if (start == s.size()) {
            return true;
        }

        for (int end = start + 1; end <= s.size(); end++) {

            string word = s.substr(start, end - start);

            if (dict.count(word)) {

                if (solve(end, s)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool wordBreak(string s, vector<string>& wordDict) {

        for (string word : wordDict) {
            dict.insert(word);
        }

        return solve(0, s);
    }
};
```

### Complexity Analysis

Let `n` be the length of the string.

- **Time:** Exponential in the worst case (up to O(2^n) recursive partitions, excluding substring/hash costs).
- **Auxiliary Space:** O(n) recursion depth, excluding dictionary storage.

### Limitation

The same substring positions may be evaluated repeatedly.

For example, multiple recursive paths can reach the same starting index.

We can avoid this repeated work using Dynamic Programming.

## 3. Optimized Approach — Dynamic Programming

### Key Observation

Instead of checking every possible segmentation repeatedly, store whether each prefix of the string can be segmented successfully.

We use a DP array to remember previously calculated results.

### DP State

```cpp
dp[i]
```

`dp[i] = true` means the first `i` characters of the string can be segmented into valid dictionary words.

### Initialization

```cpp
dp[0] = true;
```

Why?

An empty string can be segmented successfully because no characters need to be processed.

This also allows us to start checking words from index 0.

### Transition

For every position `i`, check previous positions `j`.

If:

```text
dp[j] == true
```

and:

```text
s.substr(j, i - j) exists in wordDict
```

then:

```text
dp[i] = true
```

In simple terms:

If the prefix ending at `j` is valid, and the substring from `j` to `i - 1` is a dictionary word, then the prefix ending at `i - 1` is also valid.

### DP Formula

```text
dp[i] = true

if there exists j < i such that:

dp[j] == true

AND

s[j ... i-1] exists in wordDict
```

## 4. Optimized C++ Code

```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {

        unordered_set<string> dict(
            wordDict.begin(),
            wordDict.end()
        );

        int n = s.size();

        vector<bool> dp(n + 1, false);

        dp[0] = true;

        for (int i = 1; i <= n; i++) {

            for (int j = 0; j < i; j++) {

                if (dp[j] &&
                    dict.count(s.substr(j, i - j))) {

                    dp[i] = true;
                    break;
                }
            }
        }

        return dp[n];
    }
};
```

### Complexity Analysis

Let `n` be the length of the string.

- **Time:** O(n³) worst case with substring construction and hashing.
- **Auxiliary Space:** O(n) for the DP array, excluding the dictionary.

The algorithm checks O(n²) pairs of indices. Constructing and hashing substrings can take O(n) time in the worst case.

## 5. Dry Run

Input:

```text
s = "neetcode"

wordDict = ["neet", "code"]
```

String indexes:

```text
Index:  0 1 2 3 4 5 6 7
String: n e e t c o d e
```

String length:

```text
n = 8
```

### Initialization

```text
dp = [true, false, false, false,
      false, false, false, false, false]
```

`dp[0] = true` because the empty prefix is valid.

### Step 1: i = 1

Check possible substrings ending at index 0:

```text
"n"
```

Not present in the dictionary.

```text
dp[1] = false
```

### Step 2: i = 2

Possible substrings:

```text
"ne"
"e"
```

None is a valid dictionary word.

```text
dp[2] = false
```

### Step 3: i = 3

Possible substrings:

```text
"nee"
"ee"
"e"
```

None is a valid dictionary word.

```text
dp[3] = false
```

### Step 4: i = 4

Check:

```text
s.substr(0, 4) = "neet"
```

Conditions:

```text
dp[0] = true
"neet" exists in wordDict
```

Both conditions are satisfied.

Therefore:

```text
dp[4] = true
```

### Step 5: i = 5, 6, 7

No valid segmentation can be formed for these prefixes.

```text
dp[5] = false
dp[6] = false
dp[7] = false
```

### Step 6: i = 8

Check the substring:

```text
s.substr(4, 4) = "code"
```

Conditions:

```text
dp[4] = true
"code" exists in wordDict
```

Both conditions are satisfied.

Therefore:

```text
dp[8] = true
```

### Final DP Array

```text
Index:  0     1      2      3      4      5      6      7      8

dp:    true  false  false  false  true   false  false  false  true
```

Final answer:

```text
dp[8] = true

Output: true
```

Valid segmentation:

```text
"neet" + "code"
```

## 6. Optimization — Limit the Search Using Maximum Word Length

The previous DP solution checks every possible previous index.

But suppose the longest dictionary word has length 5.

There is no need to check substrings longer than 5 characters.

We can calculate:

```cpp
maxLen = maximum length of words in wordDict
```

Then limit the inner loop:

```cpp
for (int j = max(0, i - maxLen); j < i; j++)
```

### Improved C++ Code

```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {

        unordered_set<string> dict(
            wordDict.begin(),
            wordDict.end()
        );

        int n = s.size();

        int maxLen = 0;

        for (string word : wordDict) {
            maxLen = max(maxLen, (int)word.size());
        }

        vector<bool> dp(n + 1, false);

        dp[0] = true;

        for (int i = 1; i <= n; i++) {

            for (int j = max(0, i - maxLen); j < i; j++) {

                if (dp[j] &&
                    dict.count(s.substr(j, i - j))) {

                    dp[i] = true;
                    break;
                }
            }
        }

        return dp[n];
    }
};
```

### Complexity Analysis

Let:

- `n` = length of the string.
- `L` = maximum word length in the dictionary.

- **Time:** O(n × L²) worst case with substring construction and hashing.
- **Auxiliary Space:** O(n), excluding dictionary storage.

This optimization is useful when the maximum dictionary word length is much smaller than the input string length.

## 7. Common Mistakes

1. Forgetting to initialize `dp[0] = true`.
2. Returning `dp[n - 1]` instead of `dp[n]`.
3. Checking only whether a substring exists in the dictionary without verifying `dp[j]`.
4. Assuming each dictionary word can only be used once.
5. Forgetting that the entire string must be segmented.
6. Using `j <= i` and accidentally generating an empty substring.
7. Confusing the substring boundaries: `substr(j, i - j)` uses a starting index and a length.
8. Repeating recursive calculations without memoization.

## 8. Important Interview Insights

### Q1. Why do we use dp[n + 1] instead of dp[n]?

Because we need to represent all prefixes, including the empty prefix.

For a string of length `n`, there are `n + 1` prefix positions.

```text
dp[0] = empty prefix
dp[1] = first character
...
dp[n] = complete string
```

### Q2. Why is dp[0] initialized to true?

It represents a valid starting point.

If the first substring is a dictionary word, we can extend the valid empty prefix to form a valid non-empty prefix.

### Q3. Why do we use unordered_set?

It provides average O(1) lookup for dictionary words.

This avoids searching through the entire dictionary for every substring.

### Q4. Can a dictionary word be reused?

Yes.

For example:

```text
s = "applepenapple"

wordDict = ["apple", "pen"]
```

The word `"apple"` is used twice.

### Q5. What is the difference between Word Break and Word Break II?

**Word Break:**

Return only whether segmentation is possible.

```text
true / false
```

**Word Break II:**

Return all possible valid sentences formed by segmenting the string.

### Q6. Why is this Dynamic Programming?

Because the problem has:

1. Overlapping subproblems: Multiple paths can reach the same prefix.
2. Optimal substructure: A valid segmentation of a larger prefix depends on a valid segmentation of a smaller prefix.

## 9. Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force Recursion | Exponential | O(n) stack |
| DP + Hash Set | O(n³) | O(n) |
| DP + Maximum Word Length | O(n × L²) | O(n) |

*Complexities exclude dictionary storage. The DP time bounds account for substring construction and hashing.*

## 10. Final Revision Notes

Remember these points:

1. Use `dp[i]` to represent whether the first `i` characters can be segmented.
2. Initialize `dp[0] = true`.
3. Check previous valid positions using `dp[j]`.
4. Check whether `s.substr(j, i - j)` exists in the dictionary.
5. Use an `unordered_set` for efficient average lookup.
6. Dictionary words can be reused.
7. The final answer is `dp[n]`.
8. Maximum word length can reduce unnecessary substring checks.

**Best Approach:** 1D Dynamic Programming + Hash Set.

- **Time Complexity:** O(n³) for the standard implementation.
- **Auxiliary Space:** O(n).

**Interview Takeaway:** When a string must be divided into valid dictionary words, think about whether each prefix can be formed using an earlier valid prefix.

# Longest Increasing Subsequence (LIS) — LeetCode 300

- **Difficulty:** Medium
- **Topics:** Array, Binary Search, Dynamic Programming
- **Pattern:** 1D DP / Patience Sorting
- **Goal:** Find the length of the longest strictly increasing subsequence.

## 1. Problem Understanding

Given an integer array `nums`, return the length of the longest strictly increasing subsequence.

A **subsequence** is a sequence that can be obtained from an array by deleting zero or more elements without changing the relative order of the remaining elements.

### Important Conditions

1. The elements must be in strictly increasing order.
2. Elements do not need to be adjacent.
3. The relative order of elements must remain unchanged.
4. Duplicate values cannot extend a strictly increasing subsequence.

### Example 1

```text
Input:
nums = [9,1,4,2,3,3,7]

Output:
4

Explanation:
The longest increasing subsequence is [1,2,3,7].

Length = 4
```

### Example 2

```text
Input:
nums = [0,3,1,3,2,3]

Output:
4

Explanation:
One longest increasing subsequence is [0,1,2,3].

Length = 4
```

## 2. Brute Force Approach — Recursion

### Idea

At every index, we have two choices:

1. Include the current element if it is greater than the previously selected element.
2. Skip the current element.

We recursively explore both possibilities and return the maximum subsequence length.

### C++ Code

```cpp
class Solution {
public:

    int solve(int index, int prev, vector<int>& nums) {

        if (index == nums.size()) {
            return 0;
        }

        // Option 1: Skip current element
        int notTake = solve(index + 1, prev, nums);

        // Option 2: Take current element
        int take = 0;

        if (prev == -1 || nums[index] > nums[prev]) {

            take = 1 + solve(index + 1, index, nums);
        }

        return max(take, notTake);
    }

    int lengthOfLIS(vector<int>& nums) {

        return solve(0, -1, nums);
    }
};
```

### Complexity Analysis

- **Time:** O(2^n), since each element can be taken or skipped.
- **Auxiliary Space:** O(n), due to recursion depth.

### Limitation

The same subproblems are calculated repeatedly.

We can optimize this using memoization.

## 3. Better Approach — DP with Memoization

### Idea

The recursive solution has two changing parameters:

- `index`: Current position.
- `prev`: Index of the previously selected element.

We store the results of these states so that repeated calculations are avoided.

### C++ Code

```cpp
class Solution {
public:

    int solve(int index, int prev, vector<int>& nums,
              vector<vector<int>>& dp) {

        if (index == nums.size()) {
            return 0;
        }

        if (dp[index][prev + 1] != -1) {
            return dp[index][prev + 1];
        }

        int notTake = solve(index + 1, prev, nums, dp);

        int take = 0;

        if (prev == -1 || nums[index] > nums[prev]) {

            take = 1 + solve(index + 1, index, nums, dp);
        }

        return dp[index][prev + 1] = max(take, notTake);
    }

    int lengthOfLIS(vector<int>& nums) {

        int n = nums.size();

        vector<vector<int>> dp(
            n, vector<int>(n + 1, -1)
        );

        return solve(0, -1, nums, dp);
    }
};
```

### Why `prev + 1`?

The previous index can be `-1`, representing that no element has been selected yet.

Since array indexes cannot be negative, we shift the previous index by one.

```text
prev = -1  -> dp column 0
prev = 0   -> dp column 1
prev = 1   -> dp column 2
```

### Complexity Analysis

- **Time:** O(n²), because there are O(n²) possible states.
- **Auxiliary Space:** O(n²) for the DP table, plus O(n) recursion stack.

## 4. Optimized DP Approach — Tabulation (O(n²))

### Key Observation

Instead of exploring take and skip choices recursively, we can calculate the LIS ending at every index.

### DP State

```cpp
dp[i]
```

`dp[i]` represents the length of the longest strictly increasing subsequence ending at index `i`.

### Initialization

Initially, every element forms an increasing subsequence of length 1.

```cpp
vector<int> dp(n, 1);
```

### Transition

For every index `i`, check all previous indexes `j`.

If:

```text
nums[i] > nums[j]
```

then we can extend the increasing subsequence ending at `j`.

Therefore:

```text
dp[i] = max(dp[i], dp[j] + 1)
```

### DP Formula

\[
dp[i] = 1 + \max(dp[j])
\]

where:

\[
0 \leq j < i \quad \text{and} \quad nums[j] < nums[i]
\]

If no previous element satisfies the condition, `dp[i] = 1`.

The final answer is:

```text
max(dp[i])
```

## 5. C++ Code — O(n²) DP

```cpp
class Solution {
public:

    int lengthOfLIS(vector<int>& nums) {

        int n = nums.size();

        vector<int> dp(n, 1);

        int ans = 1;

        for (int i = 0; i < n; i++) {

            for (int j = 0; j < i; j++) {

                if (nums[i] > nums[j]) {

                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }

            ans = max(ans, dp[i]);
        }

        return ans;
    }
};
```

### Complexity Analysis

- **Time:** O(n²), using two nested loops.
- **Auxiliary Space:** O(n), for the DP array.

## 6. Dry Run — O(n²) DP

Input:

```text
nums = [9,1,4,2,3,3,7]
```

Initially:

```text
dp = [1,1,1,1,1,1,1]
```

Every element starts with a subsequence length of 1.

### Iteration 1: i = 0

```text
nums[0] = 9
```

No previous elements.

```text
dp[0] = 1
```

### Iteration 2: i = 1

```text
nums[1] = 1
```

Previous element:

```text
9 < 1  -> false
```

Therefore:

```text
dp[1] = 1
```

### Iteration 3: i = 2

```text
nums[2] = 4
```

Previous elements:

```text
9 < 4  -> false
1 < 4  -> true
```

Therefore:

```text
dp[2] = dp[1] + 1
      = 2
```

### Iteration 4: i = 3

```text
nums[3] = 2
```

Previous elements:

```text
9 < 2  -> false
1 < 2  -> true
4 < 2  -> false
```

Therefore:

```text
dp[3] = 2
```

### Iteration 5: i = 4

```text
nums[4] = 3
```

Previous elements:

```text
9 < 3  -> false
1 < 3  -> true
4 < 3  -> false
2 < 3  -> true
```

The best previous subsequence has length 2.

```text
dp[4] = 3
```

### Iteration 6: i = 5

```text
nums[5] = 3
```

Previous elements with smaller values:

```text
1 < 3  -> true
2 < 3  -> true
```

The best previous subsequence has length 2.

```text
dp[5] = 3
```

Notice that the previous value 3 cannot extend the subsequence because the sequence must be strictly increasing.

### Iteration 7: i = 6

```text
nums[6] = 7
```

Previous elements with smaller values include:

```text
1, 4, 2, 3, 3
```

The maximum previous DP value is 3.

```text
dp[6] = 4
```

### Final DP Array

```text
nums = [9,1,4,2,3,3,7]

dp   = [1,1,2,2,3,3,4]
```

Final answer:

```text
max(dp) = 4
```

One valid longest increasing subsequence:

```text
[1,2,3,7]
```

## 7. Most Optimized Approach — Binary Search (O(n log n))

### Core Concept: Patience Sorting

We maintain an array called `tails`.

`tails[i]` represents the smallest possible ending value of an increasing subsequence of length `i + 1` found so far.

The objective is to maintain the smallest possible tail values.

Why?

Smaller ending values give us more opportunities to extend a subsequence with future elements.

### Important Operations

For every number `num` in the input:

1. Find the first element in `tails` that is greater than or equal to `num`.
2. If no such element exists, append `num`.
3. Otherwise, replace that element with `num`.

We use:

```cpp
lower_bound(tails.begin(), tails.end(), num);
```

`lower_bound` returns the first position where the value is greater than or equal to `num`.

### Example

```text
nums = [9,1,4,2,3,3,7]
```

Initially:

```text
tails = []
```

Process each number:

| Number | Operation | tails |
|---|---|---|
| 9 | Insert | [9] |
| 1 | Replace 9 | [1] |
| 4 | Insert | [1,4] |
| 2 | Replace 4 | [1,2] |
| 3 | Insert | [1,2,3] |
| 3 | Replace existing 3 | [1,2,3] |
| 7 | Insert | [1,2,3,7] |

Final:

```text
tails = [1,2,3,7]
```

Answer:

```text
tails.size() = 4
```

## 8. C++ Code — Binary Search

```cpp
class Solution {
public:

    int lengthOfLIS(vector<int>& nums) {

        vector<int> tails;

        for (int num : nums) {

            auto it = lower_bound(
                tails.begin(),
                tails.end(),
                num
            );

            if (it == tails.end()) {

                tails.push_back(num);

            } else {

                *it = num;
            }
        }

        return tails.size();
    }
};
```

### Complexity Analysis

- **Time:** O(n log n).
  - We process every element once.
  - Each `lower_bound` takes O(log n).
- **Auxiliary Space:** O(n), for the `tails` array.

## 9. Important Concept — Does tails Store the Actual LIS?

Not necessarily.

The `tails` array helps calculate the length of the LIS, but it is not always an actual subsequence from the original input.

Example:

```text
nums = [2,6,3,4,1,5]
```

The algorithm may maintain:

```text
tails = [1,3,4,5]
```

But this particular array is not necessarily the subsequence represented by the selected indexes in the original input.

The important point:

**The size of tails gives the length of the LIS.**

If we need to reconstruct the actual LIS, we need additional arrays to track parent indexes and the index representing each subsequence length.

## 10. Why lower_bound Instead of upper_bound?

The problem asks for a strictly increasing subsequence.

That means:

```text
previous element < current element
```

We use:

```cpp
lower_bound()
```

It finds the first element greater than or equal to the current number.

This ensures duplicate values replace an existing tail rather than increasing the LIS length.

### Difference

| Function | Finds |
|---|---|
| lower_bound | First element >= target |
| upper_bound | First element > target |

For strictly increasing LIS, use `lower_bound`.

For a non-decreasing subsequence, the binary search approach uses `upper_bound`.

## 11. Common Mistakes

1. Using `>=` instead of `>` in the O(n²) DP condition.
2. Using `upper_bound` instead of `lower_bound` for strictly increasing LIS.
3. Assuming the LIS must contain adjacent elements.
4. Assuming the final `tails` array always represents the actual LIS.
5. Forgetting that every individual element creates a subsequence of length 1.
6. Returning `dp[n - 1]` in the O(n²) approach instead of the maximum DP value.
7. Confusing subsequence with subarray.

## 12. Approach Comparison

| Approach | Time | Auxiliary Space |
|---|---|---|
| Brute Force Recursion | O(2^n) | O(n) stack |
| Memoization | O(n²) | O(n²) + recursion stack |
| Tabulation DP | O(n²) | O(n) |
| Binary Search | O(n log n) | O(n) |

## 13. Important Interview Questions

### Q1. What is the difference between a subsequence and a subarray?

**Subarray:**
- Must contain contiguous elements.
- Cannot skip elements.

**Subsequence:**
- Does not need contiguous elements.
- Can skip elements.
- Must preserve relative order.

### Q2. Why do we initialize every DP value to 1?

Every individual element forms an increasing subsequence of length 1.

### Q3. Why do we use lower_bound?

Because the subsequence must be strictly increasing.

It replaces the first element greater than or equal to the current number.

### Q4. Why is the binary search approach O(n log n)?

We process `n` elements, and for every element, binary search takes O(log n).

Therefore:

```text
O(n) * O(log n) = O(n log n)
```

### Q5. Can we reconstruct the actual LIS?

Yes.

We need additional tracking arrays to store predecessor indexes and the indexes representing each LIS length.

### Q6. Is this problem a Dynamic Programming problem even when using binary search?

Yes. The O(n²) solution explicitly uses DP states.

The optimized tails approach uses a different technique to maintain the best possible ending values and calculate the same LIS length efficiently.

## 14. Final Revision Notes

Remember these patterns:

**O(n²) DP:**

```cpp
if (nums[i] > nums[j]) {
    dp[i] = max(dp[i], dp[j] + 1);
}
```

- `dp[i]` = LIS ending at index `i`.
- Final answer = maximum value in `dp`.

**O(n log n) Binary Search:**

```cpp
auto it = lower_bound(
    tails.begin(),
    tails.end(),
    num
);

if (it == tails.end()) {
    tails.push_back(num);
} else {
    *it = num;
}
```

- `tails` maintains the smallest possible tail for each subsequence length.
- Final answer = `tails.size()`.

**Recommended Interview Strategy:**

1. Explain the O(n²) DP approach first.
2. Explain how repeated comparisons can be optimized.
3. Introduce the `tails` array.
4. Explain why `lower_bound` is used.
5. Finish with O(n log n) complexity.

**Best Approach:** Binary Search + Tails Array.

- **Time Complexity:** O(n log n).
- **Auxiliary Space:** O(n).

  # Partition Equal Subset Sum — LeetCode 416

- **Difficulty:** Medium
- **Topics:** Array, Dynamic Programming
- **Pattern:** 0/1 Knapsack / Subset Sum
- **Goal:** Determine whether an array can be divided into two subsets having equal sums.

## 1. Problem Understanding

Given an array of positive integers `nums`, determine whether it can be partitioned into two subsets such that both subsets have the same sum.

Return:

- `true` if an equal partition is possible.
- `false` otherwise.

### Important Conditions

1. Every element must belong to exactly one of the two subsets.
2. Both subsets must have equal sums.
3. We cannot use an element more than once.
4. The array contains positive integers.

### Example 1

```text
Input:
nums = [1,2,3,4]

Output:
true

Explanation:

Subset 1 = [1,4]
Subset 2 = [2,3]

Sum of both subsets = 5
```

### Example 2

```text
Input:
nums = [1,2,3,4,5]

Output:
false

Explanation:

Total sum = 15

15 cannot be divided equally into two integer subset sums.
```

## 2. Key Observation — Convert the Problem into Subset Sum

Instead of finding two subsets directly, calculate the total sum.

```cpp
int total = accumulate(nums.begin(), nums.end(), 0);
```

For an equal partition:

```text
sum(subset1) = sum(subset2)
```

Therefore:

```text
total = sum(subset1) + sum(subset2)
```

Both subsets must have:

```text
target = total / 2
```

### Important Edge Case

If the total sum is odd, equal partitioning is impossible.

```cpp
if (total % 2 != 0) {
    return false;
}
```

Otherwise, our problem becomes:

**Can we find a subset whose sum equals `total / 2`?**

This is the classic Subset Sum problem.

## 3. Brute Force Approach — Recursion

### Idea

At every index, we have two choices:

1. Include the current element in the subset.
2. Exclude the current element.

We recursively explore both choices until we either:

- Find the required target sum.
- Run out of elements.

### C++ Code

```cpp
class Solution {
public:

    bool solve(int index, int target, vector<int>& nums) {

        if (target == 0) {
            return true;
        }

        if (index == nums.size() || target < 0) {
            return false;
        }

        // Include current element
        bool take = solve(
            index + 1,
            target - nums[index],
            nums
        );

        // Exclude current element
        bool notTake = solve(
            index + 1,
            target,
            nums
        );

        return take || notTake;
    }

    bool canPartition(vector<int>& nums) {

        int total = accumulate(
            nums.begin(),
            nums.end(),
            0
        );

        if (total % 2 != 0) {
            return false;
        }

        int target = total / 2;

        return solve(0, target, nums);
    }
};
```

### Complexity Analysis

- **Time:** O(2^n), because every element has two choices.
- **Auxiliary Space:** O(n), due to recursion depth.

### Limitation

The same index and remaining target combinations may be calculated repeatedly.

We can avoid this using memoization.

## 4. Better Approach — DP with Memoization

### Idea

The recursive function depends on two changing parameters:

- `index`: Current array position.
- `target`: Remaining sum required.

We store the result of each state to avoid repeated calculations.

### DP State

```cpp
dp[index][target]
```

It represents whether we can form the required target using elements from the current index onward.

### C++ Code

```cpp
class Solution {
public:

    bool solve(int index, int target,
               vector<int>& nums,
               vector<vector<int>>& dp) {

        if (target == 0) {
            return true;
        }

        if (index == nums.size() || target < 0) {
            return false;
        }

        if (dp[index][target] != -1) {
            return dp[index][target];
        }

        // Include current element
        bool take = false;

        if (nums[index] <= target) {

            take = solve(
                index + 1,
                target - nums[index],
                nums,
                dp
            );
        }

        // Exclude current element
        bool notTake = solve(
            index + 1,
            target,
            nums,
            dp
        );

        return dp[index][target] = take || notTake;
    }

    bool canPartition(vector<int>& nums) {

        int total = accumulate(
            nums.begin(),
            nums.end(),
            0
        );

        if (total % 2 != 0) {
            return false;
        }

        int target = total / 2;
        int n = nums.size();

        vector<vector<int>> dp(
            n,
            vector<int>(target + 1, -1)
        );

        return solve(0, target, nums, dp);
    }
};
```

### Complexity Analysis

- **Time:** O(n × target), because each index-target state is calculated once.
- **Auxiliary Space:** O(n × target) for the DP table, plus O(n) recursion stack.

## 5. Optimized Approach — 2D DP Tabulation

### Key Observation

We can eliminate recursion and solve the problem using an iterative DP table.

### DP State

```cpp
dp[i][j]
```

It represents whether a subset with sum `j` can be formed using the first `i` elements.

### Base Cases

**Case 1: Target sum is zero**

An empty subset can always form sum zero.

```text
dp[i][0] = true
```

**Case 2: No elements available**

A positive target cannot be formed without elements.

```text
dp[0][j] = false, when j > 0
```

### Transition

For every element, we have two choices.

**Option 1: Do not include the current element**

```text
dp[i][j] = dp[i-1][j]
```

**Option 2: Include the current element**

This is possible only when:

```text
nums[i-1] <= j
```

Then:

```text
dp[i][j] = dp[i-1][j - nums[i-1]]
```

### Final Formula

```text
dp[i][j] =
    dp[i-1][j] OR dp[i-1][j - nums[i-1]]
```

When the current element is not greater than the target.

Otherwise:

```text
dp[i][j] = dp[i-1][j]
```

The final answer is:

```cpp
dp[n][target]
```

## 6. C++ Code — 2D DP

```cpp
class Solution {
public:

    bool canPartition(vector<int>& nums) {

        int total = accumulate(
            nums.begin(),
            nums.end(),
            0
        );

        if (total % 2 != 0) {
            return false;
        }

        int target = total / 2;
        int n = nums.size();

        vector<vector<bool>> dp(
            n + 1,
            vector<bool>(target + 1, false)
        );

        // Target zero is always possible
        for (int i = 0; i <= n; i++) {
            dp[i][0] = true;
        }

        for (int i = 1; i <= n; i++) {

            for (int j = 1; j <= target; j++) {

                // Do not take current element
                dp[i][j] = dp[i - 1][j];

                // Take current element
                if (nums[i - 1] <= j) {

                    dp[i][j] =
                        dp[i][j] ||
                        dp[i - 1][j - nums[i - 1]];
                }
            }
        }

        return dp[n][target];
    }
};
```

### Complexity Analysis

- **Time:** O(n × target).
- **Auxiliary Space:** O(n × target).

## 7. Dry Run — 2D DP

Input:

```text
nums = [1,2,3,4]
```

### Step 1: Calculate Total Sum

```text
total = 1 + 2 + 3 + 4

total = 10
```

Since the total is even:

```text
target = 10 / 2 = 5
```

Now we need to determine whether a subset with sum 5 exists.

### Step 2: Initialize DP

We have:

```text
n = 4
target = 5
```

DP dimensions:

```text
dp[5][6]
```

Initially, all values are false.

Set the first column to true because sum zero is always possible.

### Step 3: Process Elements

For every element, update the possible subset sums.

After processing `1`:

```text
Possible sums: {0,1}
```

After processing `2`:

```text
Possible sums: {0,1,2,3}
```

After processing `3`:

```text
Possible sums: {0,1,2,3,4,5,6}
```

After processing `4`:

```text
Possible sums: {0,1,2,3,4,5,6,7,8,9,10}
```

The target sum 5 is possible.

### Final DP Table

Rows represent how many elements have been processed.

Columns represent target sums.

```text
          Target Sum
        0  1  2  3  4  5
-------------------------
0       T  F  F  F  F  F
1       T  T  F  F  F  F
2       T  T  T  T  F  F
3       T  T  T  T  T  T
4       T  T  T  T  T  T
```

Therefore:

```text
dp[4][5] = true
```

One valid subset is:

```text
[1,4] = 5
```

The remaining elements form:

```text
[2,3] = 5
```

Final answer:

```text
Output: true
```

## 8. Most Optimized Approach — 1D DP (Space Optimization)

### Key Observation

In the 2D DP solution, we only use the previous row:

```cpp
dp[i - 1][...]
```

We do not need to maintain the entire 2D table.

We can optimize the space by using a single 1D array.

### DP State

```cpp
dp[j]
```

It represents whether a subset with sum `j` can be formed using the elements processed so far.

### Initialization

```cpp
vector<bool> dp(target + 1, false);

dp[0] = true;
```

Sum zero is always possible.

### Transition

For every number:

```cpp
for (int j = target; j >= num; j--)
```

Update:

```cpp
dp[j] = dp[j] || dp[j - num];
```

If the previous sum `j - num` is possible, adding the current number makes sum `j` possible.

### Why Do We Iterate Backwards?

This is the most important concept in this problem.

We iterate from `target` down to `num` to ensure that every array element is used at most once.

If we iterate forwards, the current element could be reused multiple times during the same iteration.

That would turn the solution into an **Unbounded Knapsack** style update.

For this problem, we need **0/1 Knapsack**, where each element can be selected only once.

## 9. C++ Code — 1D DP (Recommended)

```cpp
class Solution {
public:

    bool canPartition(vector<int>& nums) {

        int total = accumulate(
            nums.begin(),
            nums.end(),
            0
        );

        if (total % 2 != 0) {
            return false;
        }

        int target = total / 2;

        vector<bool> dp(target + 1, false);

        dp[0] = true;

        for (int num : nums) {

            for (int j = target; j >= num; j--) {

                dp[j] = dp[j] || dp[j - num];
            }
        }

        return dp[target];
    }
};
```

### Complexity Analysis

Let:

- `n` = number of elements.
- `target` = total sum divided by 2.

- **Time:** O(n × target).
- **Auxiliary Space:** O(target).

This is the recommended solution because it achieves the same DP time complexity while reducing space from O(n × target) to O(target).

## 10. Dry Run — 1D DP

Input:

```text
nums = [1,2,3,4]
```

Total:

```text
total = 10
target = 5
```

Initial DP:

```text
dp = [T,F,F,F,F,F]
```

Index represents the target sum.

### Step 1: num = 1

Update backwards from 5 to 1.

```text
dp = [T,T,F,F,F,F]
```

Possible sums:

```text
0, 1
```

### Step 2: num = 2

Update backwards from 5 to 2.

```text
dp = [T,T,T,T,F,F]
```

Possible sums:

```text
0, 1, 2, 3
```

### Step 3: num = 3

Update backwards from 5 to 3.

```text
dp = [T,T,T,T,T,T]
```

Possible sums:

```text
0, 1, 2, 3, 4, 5
```

Target 5 is now achievable.

### Step 4: num = 4

The target is already achievable.

The final DP array remains:

```text
dp = [T,T,T,T,T,T]
```

Final answer:

```text
dp[5] = true
```

Therefore:

```text
Output: true
```

## 11. Important Concept — Why Backward Iteration Matters

Consider:

```text
nums = [2]
target = 4
```

We have only one element, 2.

It is impossible to form sum 4 because we cannot use the same element twice.

### Incorrect Forward Iteration

```cpp
for (int j = num; j <= target; j++) {
    dp[j] = dp[j] || dp[j - num];
}
```

After processing 2:

```text
dp[2] = true
```

The same iteration then checks:

```text
dp[4] = dp[4] || dp[2]
```

Now:

```text
dp[4] = true
```

This incorrectly uses the same element twice.

### Correct Backward Iteration

```cpp
for (int j = target; j >= num; j--) {
    dp[j] = dp[j] || dp[j - num];
}
```

When processing sum 4, `dp[2]` has not yet been updated for this element.

Therefore, the element cannot be reused.

**Interview takeaway:**

- Backward iteration = 0/1 Knapsack (use each element at most once).
- Forward iteration = Unbounded Knapsack style (reuse is possible).

## 12. Common Mistakes

1. Forgetting to check whether the total sum is odd.
2. Using `total` as the target instead of `total / 2`.
3. Initializing `dp[0]` to false.
4. Iterating forward in the 1D DP solution.
5. Forgetting that each element can only be used once.
6. Confusing Subset Sum with Unbounded Knapsack.
7. Returning the wrong DP index instead of `dp[target]`.
8. Forgetting that an empty subset can form sum zero.

## 13. Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force Recursion | O(2^n) | O(n) stack |
| Memoization | O(n × target) | O(n × target) + stack |
| 2D DP | O(n × target) | O(n × target) |
| 1D DP | O(n × target) | O(target) |

## 14. Important Interview Questions

### Q1. Why do we check whether the total sum is odd?

An odd number cannot be divided into two equal integer sums.

Therefore, an equal partition is impossible.

### Q2. Why is this a 0/1 Knapsack problem?

Because each element has two choices:

- Include it.
- Exclude it.

Every element can be used at most once.

### Q3. Why do we use `dp[0] = true`?

Because an empty subset can always produce sum zero.

It also provides the starting point for building larger subset sums.

### Q4. Why do we iterate backwards in 1D DP?

To prevent using the same element multiple times during one iteration.

### Q5. What is the difference between 0/1 Knapsack and Unbounded Knapsack?

**0/1 Knapsack:**

Each item can be selected at most once.

**Unbounded Knapsack:**

Each item can be selected unlimited times.

### Q6. Can this problem be solved using recursion?

Yes.

At every index, recursively explore taking or not taking the element. Memoization and tabulation improve the performance.

### Q7. What happens if the target is zero?

The answer is true because an empty subset has sum zero.

## 15. Final Revision Notes

### Key Mathematical Reduction

```text
total = sum(nums)

If total % 2 != 0:
    return false

target = total / 2
```

### 2D DP Transition

```cpp
dp[i][j] = dp[i - 1][j];

if (nums[i - 1] <= j) {

    dp[i][j] =
        dp[i][j] ||
        dp[i - 1][j - nums[i - 1]];
}
```

### 1D DP Transition

```cpp
for (int num : nums) {

    for (int j = target; j >= num; j--) {

        dp[j] = dp[j] || dp[j - num];
    }
}
```

### Most Important Points

1. Equal partition means finding a subset with sum `total / 2`.
2. Odd total sum means immediate false.
3. This is a Subset Sum / 0/1 Knapsack problem.
4. `dp[j]` tells whether sum `j` is achievable.
5. Initialize `dp[0] = true`.
6. Iterate backwards to avoid reusing elements.
7. Return `dp[target]`.

**Best Approach:** 1D DP (0/1 Knapsack).

- **Time Complexity:** O(n × target).
- **Auxiliary Space:** O(target).

**Interview Takeaway:** Whenever a problem asks whether some elements can form a particular target sum, think about Subset Sum DP. If every element can be used only once, use backward iteration in the 1D DP solution.

**Interview takeaway:** Climbing Stairs is a fundamental 1D Dynamic Programming problem. Learn to derive the recurrence first, then move from recursion to memoization, tabulation, and finally space optimization.
