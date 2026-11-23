# 63. Subsets

## 1. Problem Understanding

You are given an integer array "nums" containing unique elements.

Return all possible subsets (the power set).

A subset can contain:

- No elements (empty subset).
- One element.
- Multiple elements.
- All elements.

**Example**

```cpp
nums = {1, 2, 3};
```

**Output:**

```text
[
  [],
  [1],
  [1, 2],
  [1, 2, 3],
  [1, 3],
  [2],
  [2, 3],
  [3]
]
```

For an array of size "n", the total number of subsets is:

\[
2^n
\]

For example, 3 elements produce (2³ = 8) subsets.

---

## 2. Core Intuition

For every element, we have two choices:

1. Include the element.
2. Do not include the element.

Let's take:

```cpp
nums = [1, 2, 3]
```

For element "1":

- Include "1".
- Skip "1".

For element "2", again:

- Include "2".
- Skip "2".

The same applies to element "3".

This creates a decision tree.

**Decision Tree**

```text
                         []
                       /    \
                  Include 1  Skip 1
                    /           \
                  [1]            []
                 /   \          /  \
           Include 2 Skip 2  Include 2 Skip 2
              /        \        /       \
          [1,2]       [1]     [2]        []
           / \         / \     / \       / \
         +3  -3       +3  -3  +3  -3   +3  -3
```

Every leaf represents one possible subset.

This is the foundation of the backtracking approach.

---

## 3. Complete C++ Solution

```cpp
class Solution {
private:

    void backtrack(
        int index,
        vector<int>& nums,
        vector<int>& current,
        vector<vector<int>>& result
    ) {

        // All elements have been considered.
        if (index == nums.size()) {
            result.push_back(current);
            return;
        }

        // Choice 1: Include the current element.
        current.push_back(nums[index]);

        backtrack(index + 1, nums, current, result);

        // Backtrack: Remove the last element.
        current.pop_back();

        // Choice 2: Skip the current element.
        backtrack(index + 1, nums, current, result);
    }

public:

    vector<vector<int>> subsets(vector<int>& nums) {

        vector<vector<int>> result;
        vector<int> current;

        backtrack(0, nums, current, result);

        return result;
    }
};
```

---

## 4. Understanding the Backtracking Function

Our helper function:

```cpp
void backtrack(
    int index,
    vector<int>& nums,
    vector<int>& current,
    vector<vector<int>>& result
)
```

Each parameter has a specific responsibility:

| Parameter | Meaning |
|---|---|
| "index" | Current element we are considering |
| "nums" | Original input array |
| "current" | Subset currently being constructed |
| "result" | Stores all completed subsets |

We start with:

```cpp
backtrack(0, nums, current, result);
```

This means we begin considering elements from index "0".

---

## 5. Understanding the Base Case

```cpp
if (index == nums.size()) {
    result.push_back(current);
    return;
}
```

When "index" reaches the array size, we have considered every element.

The current subset is now complete.

We copy it into the result:

```cpp
result.push_back(current);
```

Important: We use "push_back(current)" to save a copy of the current subset, not a reference to the temporary vector.

---

## 6. Understanding the Two Choices

This is the most important part of the problem.

**Choice 1: Include the current element**

```cpp
current.push_back(nums[index]);

backtrack(index + 1, nums, current, result);
```

Suppose:

```cpp
current = []
nums[index] = 1
```

After including:

```cpp
current = [1]
```

Now recursively process the next element.

**Backtracking: Remove the element**

```cpp
current.pop_back();
```

Why?

Because after exploring the branch that includes "1", we must restore the previous state before exploring the branch that skips "1".

Before:

```cpp
current = [1]
```

After:

```cpp
current = []
```

**Choice 2: Skip the current element**

```cpp
backtrack(index + 1, nums, current, result);
```

We simply move to the next element without including the current one.

This gives us both possibilities.

---

## 7. Complete Dry Run

**Input:**

```cpp
nums = [1, 2]
```

**Initial state:**

```cpp
current = []
result = []
```

**Step 1: Consider element 1**

Include "1":

```cpp
current = [1]
```

Now consider element "2".

**Step 2: Include element 2**

```cpp
current = [1, 2]
```

All elements processed.

Save:

```cpp
result = [[1, 2]]
```

**Step 3: Backtrack and skip element 2**

Remove "2":

```cpp
current = [1]
```

Save this subset:

```cpp
result = [[1, 2], [1]]
```

**Step 4: Backtrack and skip element 1**

Remove "1":

```cpp
current = []
```

Now include element "2":

```cpp
current = [2]
```

Save:

```cpp
result = [[1, 2], [1], [2]]
```

Finally, skip element "2":

```cpp
current = []
```

Save:

```cpp
result = [[1, 2], [1], [2], []]
```

**Final answer:**

```text
[
  [1, 2],
  [1],
  [2],
  []
]
```

The order of subsets does not matter.

---

## 8. Visualizing the Complete Decision Tree

For:

```cpp
nums = [1, 2]
```

```text
                         []
                       /    \
                    Take 1  Skip 1
                     /        \
                   [1]         []
                  /   \       /   \
              Take 2 Skip 2 Take 2 Skip 2
                /      \      /      \
             [1,2]    [1]    [2]      []
```

Each element creates two choices.

Therefore:

\[
2 \times 2 \times 2 = 2^n
\]

For "n = 2":

\[
2^2 = 4
\]

We get exactly four subsets.

---

## 9. Why Is This Called Backtracking?

Backtracking means:

1. Make a choice.
2. Explore the consequences.
3. Undo the choice.
4. Explore another possibility.

In this problem:

```cpp
current.push_back(nums[index]);  // Make choice

backtrack(...);                  // Explore

current.pop_back();              // Undo choice

backtrack(...);                  // Explore alternative
```

The "pop_back()" operation is what restores the current subset before exploring the other branch.

Think of it as:

Choose → Explore → Undo → Choose another option.

---

## 10. Complexity Analysis

Let "n" be the number of elements.

**Time Complexity**

There are:

\[
2^n
\]

possible subsets.

Copying each subset takes up to O(n) time.

Therefore:

\[
\boxed{O(n \times 2^n)}
\]

**Space Complexity**

- Recursion depth: O(n).
- Current subset: O(n).
- Output: O(n × 2ⁿ).

The auxiliary space excluding the output is O(n).

---

## 11. Common Interview Mistakes

1. Forgetting the empty subset: The empty subset is also valid.
2. Forgetting "pop_back()": This causes elements from one branch to remain in another branch.
3. Using the wrong base case: Stop when "index == nums.size()".
4. Not copying the current subset: Use "result.push_back(current)".
5. Moving the index incorrectly: Both choices must process "index + 1".
6. Thinking subsets must be in sorted order: The output order does not matter.

---

## 12. Alternative Approach: For Loop Backtracking

There is another common way to solve this problem.

```cpp
class Solution {
private:

    void backtrack(
        int start,
        vector<int>& nums,
        vector<int>& current,
        vector<vector<int>>& result
    ) {

        // Every current state is a valid subset.
        result.push_back(current);

        for (int i = start; i < nums.size(); i++) {

            current.push_back(nums[i]);

            backtrack(i + 1, nums, current, result);

            current.pop_back();
        }
    }

public:

    vector<vector<int>> subsets(vector<int>& nums) {

        vector<vector<int>> result;
        vector<int> current;

        backtrack(0, nums, current, result);

        return result;
    }
};
```

**Difference Between Both Approaches**

| Include/Skip approach | For-loop approach |
|---|---|
| Makes two explicit choices | Loops through available choices |
| Base case when all elements are processed | Saves current subset at every call |
| Easy to understand binary decisions | Common reusable combination pattern |
| Uses recursion with two branches | Uses recursion with a loop |

Both approaches are correct.

For learning the decision-making concept, the first approach is especially useful.

---

## 13. Pattern Recognition for Interviews

Think about backtracking when a problem asks you to generate:

- All possible subsets.
- All combinations.
- All possible selections.
- All possible arrangements.
- Every valid choice from a collection.

A useful question to ask yourself:

At each step, do I have multiple choices that I need to explore?

If yes, backtracking may be appropriate.

---

## 14. 30-Second Interview Revision

**Q1. What is a subset?**

A selection of zero or more elements from an array, without changing their relative order.

**Q2. How many subsets exist for n elements?**

(2^n).

**Q3. Why use backtracking?**

Because each element has two choices: include it or skip it.

**Q4. Why use "pop_back()"?**

To undo the previous choice before exploring another branch.

**Q5. What is the time complexity?**

O(n × 2ⁿ), because there are 2ⁿ subsets and copying each subset can take O(n).

**Q6. Is the empty subset valid?**

Yes. Every set has an empty subset.

**Final takeaway**

```text
SUBSETS = INCLUDE OR SKIP

For every element:
    1. Include it.
    2. Explore recursively.
    3. Remove it (backtrack).
    4. Skip it.
    5. Explore recursively.

When all elements are processed:
    Save the current subset.
```

# 64. Combination Sum

## 1. Problem Understanding

You are given:

- An array of distinct positive integers "nums".
- An integer "target".

Find all unique combinations where the chosen numbers add up to "target".

**Important rules:**

1. You can use the same number unlimited times.
2. The order of numbers in a combination does not matter.
3. Return all unique combinations.

**Example**

```cpp
nums = {2, 3, 6, 7};
target = 7;
```

**Output:**

```text
[
    [2, 2, 3],
    [7]
]
```

**Explanation:**

- "2 + 2 + 3 = 7"
- "7 = 7"

We cannot use combinations such as "[3, 2, 2]" separately because they contain the same numbers in a different order.

---

## 2. Core Intuition

We use Backtracking.

For every candidate, we have two choices:

1. Include the current number.
2. Skip the current number and move to the next candidate.

The important difference from the Subsets problem:

When we include a number, we stay at the same index because we can reuse it.

Consider:

```cpp
nums = [2, 3, 6, 7]
target = 7
```

We start with "2".

```cpp
current = [2]
remaining target = 5
```

We can select "2" again:

```cpp
current = [2, 2]
remaining target = 3
```

We can select "2" again, but that would make the sum exceed the target.

Instead, we explore the next candidate:

```cpp
current = [2, 2, 3]
remaining target = 0
```

We found a valid combination.

---

## 3. Complete C++ Solution

```cpp
class Solution {
private:

    void backtrack(
        int index,
        vector<int>& nums,
        int target,
        vector<int>& current,
        vector<vector<int>>& result
    ) {

        // A valid combination is found.
        if (target == 0) {
            result.push_back(current);
            return;
        }

        // No candidates left.
        if (index == nums.size()) {
            return;
        }

        // Choice 1: Include the current number.
        if (nums[index] <= target) {

            current.push_back(nums[index]);

            // Stay at the same index because reuse is allowed.
            backtrack(
                index,
                nums,
                target - nums[index],
                current,
                result
            );

            // Undo the choice.
            current.pop_back();
        }

        // Choice 2: Skip the current number.
        backtrack(
            index + 1,
            nums,
            target,
            current,
            result
        );
    }

public:

    vector<vector<int>> combinationSum(
        vector<int>& nums,
        int target
    ) {

        vector<vector<int>> result;
        vector<int> current;

        backtrack(0, nums, target, current, result);

        return result;
    }
};
```

---

## 4. Understanding the Parameters

```cpp
void backtrack(
    int index,
    vector<int>& nums,
    int target,
    vector<int>& current,
    vector<vector<int>>& result
)
```

Each parameter has a specific responsibility:

| Parameter | Meaning |
|---|---|
| "index" | Current candidate we are considering |
| "nums" | Original array |
| "target" | Remaining sum we need to achieve |
| "current" | Combination currently being constructed |
| "result" | Stores all valid combinations |

Notice that we use "target" as the remaining sum rather than repeatedly calculating the sum of "current".

This makes the implementation simpler.

---

## 5. Understanding the Base Cases

**Case 1: Target becomes zero**

```cpp
if (target == 0) {
    result.push_back(current);
    return;
}
```

This means the current combination has exactly reached the required sum.

Example:

```cpp
current = [2, 2, 3]
target = 0
```

We save the combination.

**Case 2: No candidates remain**

```cpp
if (index == nums.size()) {
    return;
}
```

We have no more numbers available to explore.

Therefore, this path cannot produce another combination.

---

## 6. The Most Important Concept: Same Index vs Next Index

Look at these two calls:

**Include the current number**

```cpp
backtrack(index, nums, target - nums[index], current, result);
```

We use the same "index".

Why?

Because the problem allows unlimited reuse of a candidate.

Example:

```cpp
nums = [2, 3, 6, 7]
```

We can select "2" repeatedly:

```text
[2]
[2, 2]
[2, 2, 2]
```

**Skip the current number**

```cpp
backtrack(index + 1, nums, target, current, result);
```

We move to the next index because we have decided not to use the current candidate anymore.

Remember this distinction:

| Decision | Next index |
|---|---|
| Include current number | "index" |
| Skip current number | "index + 1" |

This is the central logic of Combination Sum.

---

## 7. Complete Dry Run

**Input:**

```cpp
nums = [2, 3, 6, 7]
target = 7
```

**Initially:**

```cpp
index = 0
target = 7
current = []
```

**Step 1: Include 2**

```cpp
current = [2]
target = 5
index = 0
```

We stay at index "0".

**Step 2: Include 2 again**

```cpp
current = [2, 2]
target = 3
index = 0
```

Again, we stay at index "0".

**Step 3: Try including 2 again**

2 <= 3

So we can include it:

```cpp
current = [2, 2, 2]
target = 1
```

Now:

```text
2 > 1
```

We cannot include "2".

We skip it and move to index "1", where the candidate is "3".

But:

```text
3 > 1
```

We cannot include "3" either.

We continue skipping candidates until no candidates remain.

This path is invalid.

Backtracking removes the last selected numbers and explores other possibilities.

**Step 4: Find [2, 2, 3]**

After returning to:

```cpp
current = [2, 2]
target = 3
```

We skip candidate "2" and move to candidate "3".

Include "3":

```cpp
current = [2, 2, 3]
target = 0
```

Save:

```cpp
result = [[2, 2, 3]]
```

**Step 5: Explore the combination [7]**

After exploring the branches starting with "2", we backtrack and skip "2", "3" and "6".

We reach candidate "7":

```cpp
current = [7]
target = 0
```

Save:

```cpp
result = [[2, 2, 3], [7]]
```

**Final output:**

```text
[
    [2, 2, 3],
    [7]
]
```

---

## 8. Visual Decision Tree

For:

```cpp
nums = [2, 3]
target = 7
```

A simplified view:

```text
                         []
                    target = 7
                     /       \
                Take 2       Skip 2
                  |              |
                [2]             []
              target=5       target=7
               /    \           |
           Take 2  Skip 2     Take 3
             |       |           |
           [2,2]   [2,3]        [3]
          target=3 target=2   target=4
             |       |           |
           Take 3  No valid   Take 3
             |       result       |
        [2,2,3]                 [3,3]
        target=0               target=1
```

Every time we include a number, we can choose it again.

Every time we skip a number, we move forward.

---

## 9. Why Don't We Get Duplicate Combinations?

Suppose:

```cpp
nums = [2, 3]
target = 7
```

We only explore candidates in their original order.

We can generate:

```text
[2, 2, 3]
```

But we never generate:

```text
[3, 2, 2]
```

Why?

Because once we move from candidate "2" to candidate "3", we never return to candidate "2" in that branch.

This prevents different orderings of the same combination from being generated.

---

## 10. Complexity Analysis

Let:

- "n" = number of candidates.
- "T" = target.
- "m" = smallest candidate.

The recursion can explore many combinations, so the time complexity is exponential in the worst case.

A commonly used upper-bound style estimate is:

\[
O(n^{T/m})
\]

This describes the branching search roughly; the exact number of recursive states depends on the candidate values.

Space complexity excluding the output:

\[
O(T/m)
\]

This represents the maximum recursion depth and current combination length, assuming all candidates are positive integers.

The output itself can also be large and must be counted separately.

---

## 11. Common Interview Mistakes

1. Using "index + 1" when including a number: This prevents unlimited reuse.
2. Using "index" when skipping a number: This can repeatedly reconsider the same candidate.
3. Forgetting "pop_back()": This contaminates other combinations.
4. Not stopping when "target == 0": This can generate unnecessary recursive calls.
5. Generating duplicate permutations: Process candidates in a fixed order.
6. Recalculating the current sum: Maintaining the remaining target is simpler.
7. Assuming the target must be reached with every candidate: It is valid to skip candidates.

---

## 12. Subsets vs Combination Sum

This comparison is especially important because both problems use backtracking.

| Feature | Subsets | Combination Sum |
|---|---|---|
| Include or skip | Yes | Yes |
| Reuse elements | No | Yes |
| Include recursive call | "index + 1" | "index" |
| Skip recursive call | "index + 1" | "index + 1" |
| Main stopping condition | All elements processed | Target becomes zero |
| Goal | Generate every subset | Find combinations matching target |

The most important difference is the recursive call after including an element.

---

## 13. Pattern Recognition for Interviews

Think about this pattern when a problem asks you to:

- Find combinations that add up to a target.
- Reuse elements multiple times.
- Generate unique combinations.
- Explore multiple choices recursively.

A useful question:

After choosing an element, am I allowed to choose it again?

- If yes, recursion often stays at the same index.
- If no, recursion generally moves to the next index.

---

## 14. 30-Second Interview Revision

**Q1. Why use backtracking?**

We need to explore different combinations of candidates that can reach the target.

**Q2. Can we reuse the same candidate?**

Yes, unlimited times.

**Q3. Why do we call "backtrack(index, ...)" after including a number?**

To allow that same number to be selected again.

**Q4. Why do we call "backtrack(index + 1, ...)" when skipping?**

Because we have decided not to use the current candidate in this branch.

**Q5. How do we avoid duplicate combinations?**

We process candidates in a fixed order and never move backward.

**Final takeaway**

```text
COMBINATION SUM

Include:
    current.push_back(nums[index])
    target -= nums[index]
    Recurse using SAME index
    current.pop_back()

Skip:
    Recurse using index + 1

Target == 0:
    Save the combination
```


# 65. Combination Sum II

## 1. Problem Understanding

You are given:

- An array "candidates" containing positive integers (possibly duplicates).
- An integer "target".

Find all unique combinations where the chosen numbers add up to "target".

**Important rules:**

1. Each candidate can be used at most once.
2. The input may contain duplicate values.
3. The answer must not contain duplicate combinations.

**Example**

```cpp
candidates = {10, 1, 2, 7, 6, 1, 5};
target = 8;
```

**Output:**

```text
[
    [1, 1, 6],
    [1, 2, 5],
    [1, 7],
    [2, 6]
]
```

Notice that the input contains two "1"s, so we can use both of them in "[1, 1, 6]".

However, we must not generate "[1, 7]" twice.

---

## 2. Core Intuition

We use Backtracking, just like Combination Sum.

But there are two important changes:

1. Sort the candidates.
2. After selecting a candidate, move to the next index because each element can be used only once.

Sorting also allows us to skip duplicate values at the same recursion level.

**Step 1: Sort the array**

Original:

```text
[10, 1, 2, 7, 6, 1, 5]
```

After sorting:

```text
[1, 1, 2, 5, 6, 7, 10]
```

**Step 2: Explore combinations**

For each candidate:

- Include it.
- Explore the remaining candidates.
- Remove it (backtrack).
- Try another candidate.

**Step 3: Skip duplicates**

If two adjacent candidates have the same value at the same recursion level, skip the second one.

This prevents generating the same combination through different choices of identical elements.

---

## 3. Complete C++ Solution

```cpp
class Solution {
private:

    void backtrack(
        int start,
        vector<int>& candidates,
        int target,
        vector<int>& current,
        vector<vector<int>>& result
    ) {

        // Found a valid combination.
        if (target == 0) {
            result.push_back(current);
            return;
        }

        for (int i = start; i < candidates.size(); i++) {

            // Skip duplicates at the same recursion level.
            if (i > start && candidates[i] == candidates[i - 1]) {
                continue;
            }

            // Candidates are sorted, so no later value can work.
            if (candidates[i] > target) {
                break;
            }

            // Choose the current candidate.
            current.push_back(candidates[i]);

            // Move to i + 1 because each element can be used once.
            backtrack(
                i + 1,
                candidates,
                target - candidates[i],
                current,
                result
            );

            // Undo the choice.
            current.pop_back();
        }
    }

public:

    vector<vector<int>> combinationSum2(
        vector<int>& candidates,
        int target
    ) {

        vector<vector<int>> result;
        vector<int> current;

        // Sort to group duplicate values together.
        sort(candidates.begin(), candidates.end());

        backtrack(0, candidates, target, current, result);

        return result;
    }
};
```

---

## 4. Understanding the Three Most Important Lines

**A. Sorting**

```cpp
sort(candidates.begin(), candidates.end());
```

Why?

Because duplicate values become adjacent.

Example:

```text
[10, 1, 2, 7, 6, 1, 5]
```

Becomes:

```text
[1, 1, 2, 5, 6, 7, 10]
```

Now we can easily identify duplicates and stop early when values exceed the remaining target.

**B. Skipping duplicates**

```cpp
if (i > start && candidates[i] == candidates[i - 1]) {
    continue;
}
```

This is the most important line in Combination Sum II.

Let's understand it.

Suppose:

```cpp
candidates = [1, 1, 2]
```

At the first recursion level:

```cpp
start = 0
```

- "i = 0": Select the first "1".
- "i = 1": The value is also "1".

Since:

```cpp
i > start
```

and:

```cpp
candidates[i] == candidates[i - 1]
```

we skip the second "1" at this level.

But does this mean we can never select two "1"s?

No!

When the first "1" is selected, recursion starts from index "1".

At that new level:

```cpp
start = 1
i = 1
```

Now "i > start" is false, so the second "1" can be selected.

This is exactly what we want.

Important distinction: We skip duplicate choices at the same recursion level, not duplicate values at every level.

**C. Moving to the next index**

```cpp
backtrack(i + 1, candidates, target - candidates[i], current, result);
```

Why "i + 1"?

Because every array element can be used only once.

Compare this with Combination Sum:

| Problem | Recursive call after choosing |
|---|---|
| Combination Sum | "backtrack(i, ...)" |
| Combination Sum II | "backtrack(i + 1, ...)" |

---

## 5. Understanding the Early Stopping Condition

```cpp
if (candidates[i] > target) {
    break;
}
```

Because the array is sorted, if the current candidate is greater than the remaining target, every candidate after it will also be too large.

Example:

```cpp
candidates = [1, 2, 5, 6, 7]
target = 4
```

When we reach "5":

```text
5 > 4
```

There is no reason to check "6" or "7".

We can stop the loop.

---

## 6. Complete Dry Run

**Input:**

```cpp
candidates = [1, 1, 2, 5, 6, 7, 10]
target = 8
```

**Initially:**

```cpp
current = []
target = 8
start = 0
```

**Combination 1: [1, 1, 6]**

Select the first "1":

```cpp
current = [1]
remaining target = 7
```

Select the second "1":

```cpp
current = [1, 1]
remaining target = 6
```

Select "2":

```cpp
current = [1, 1, 2]
remaining target = 4
```

No remaining candidate can complete the combination.

Backtrack and remove "2".

Select "5":

```cpp
current = [1, 1, 5]
remaining target = 1
```

This path also fails.

Backtrack and select "6" instead:

```cpp
current = [1, 1, 6]
remaining target = 0
```

Save:

```text
[1, 1, 6]
```

**Combination 2: [1, 2, 5]**

Backtrack to:

```cpp
current = [1]
remaining target = 7
```

Skip the second "1" at this recursion level.

Select "2":

```cpp
current = [1, 2]
remaining target = 5
```

Select "5":

```cpp
current = [1, 2, 5]
remaining target = 0
```

Save:

```text
[1, 2, 5]
```

**Combination 3: [1, 7]**

Backtrack and explore the next available candidate after "2".

Select "7":

```cpp
current = [1, 7]
remaining target = 0
```

Save:

```text
[1, 7]
```

**Combination 4: [2, 6]**

Backtrack to the root and skip duplicate "1" choices at that level.

Select "2":

```cpp
current = [2]
remaining target = 6
```

Select "6":

```cpp
current = [2, 6]
remaining target = 0
```

Save:

```text
[2, 6]
```

**Final result:**

```text
[
    [1, 1, 6],
    [1, 2, 5],
    [1, 7],
    [2, 6]
]
```

The order of combinations in the returned answer does not matter.

---

## 7. Visualizing the Recursion

A simplified decision tree:

```text
                     []
                  target = 8
                 /    |     \
               1      2      5 ...
              /       |
            [1]      [2]
          /    \       |
        1       2      6
        |       |      |
      [1,1]   [1,2]  [2,6]
        |       |      |
        6       5      0
        |       |
   [1,1,6]  [1,2,5]
```

Notice:

- The same array element is never reused.
- Duplicate choices at the same level are skipped.
- Every valid path is saved when the remaining target becomes zero.

---

## 8. Complexity Analysis

Let:

- "n" = number of candidates.
- "T" = target.
- "m" = smallest candidate.

**Time Complexity**

- Sorting: O(n log n).
- Backtracking: Exponential in the worst case because we explore combinations.
- A broad upper bound is O(2ⁿ × n), including copying combinations into the output.

The actual number of explored branches is often smaller because of duplicate skipping and early stopping.

**Space Complexity**

- Sorting: O(log n) auxiliary stack space on typical implementations.
- Recursion depth: O(n).
- Current combination: O(n).
- Output storage is additional and can be large.

Auxiliary space excluding output: O(n).

---

## 9. Common Interview Mistakes

1. Forgetting to sort: Duplicate skipping depends on adjacent equal values.
2. Using "i + 1" incorrectly: Use it in the recursive call because elements cannot be reused.
3. Using "i > 0" for duplicate skipping: It must be "i > start".
4. Skipping duplicates at every recursion level: This would incorrectly prevent using two equal values in one combination.
5. Forgetting "pop_back()": Other branches will contain unwanted elements.
6. Using "continue" instead of "break" when candidate exceeds target: Since the array is sorted, "break" avoids unnecessary checks.

---

## 10. Combination Sum vs Combination Sum II

This is one of the most important comparisons for interviews.

| Feature | Combination Sum | Combination Sum II |
|---|---|---|
| Reuse candidates | Unlimited | Once per index |
| Input duplicates | Distinct candidates | May contain duplicates |
| Sorting required | Not necessary | Yes, for this approach |
| Include recursive call | "i" | "i + 1" |
| Duplicate skipping | Not required | Required |
| Early break after exceeding target | Possible if sorted | Yes |

**Remember these two lines**

Combination Sum:

```cpp
backtrack(i, ...);
```

Combination Sum II:

```cpp
backtrack(i + 1, ...);
```

And the duplicate-skipping condition:

```cpp
if (i > start && candidates[i] == candidates[i - 1]) {
    continue;
}
```

---

## 11. Pattern Recognition for Interviews

Think about this pattern when a problem asks you to:

- Generate unique combinations.
- Reach a specific target sum.
- Use each input element at most once.
- Handle duplicate values in the input.

The combination of sorting, DFS and backtracking is a common solution pattern.

---

## 12. 30-Second Interview Revision

**Q1. Why sort the candidates?**

To group duplicate values together and enable early stopping.

**Q2. Why use "i + 1"?**

Each candidate index can be used only once.

**Q3. Why use "i > start" in duplicate skipping?**

To skip identical choices at the same recursion level while still allowing duplicate values to be selected at different levels.

**Q4. Can we select two equal numbers?**

Yes, if they are separate elements in the input array.

**Q5. Why use "break" when candidate exceeds target?**

The array is sorted, so all later candidates will also exceed the remaining target.

**Final takeaway**

```text
COMBINATION SUM II

1. Sort candidates.
2. Start backtracking.
3. Skip duplicates at the same level:
   if (i > start && candidates[i] == candidates[i-1])
       continue;

4. Include candidate.
5. Recurse using i + 1.
6. Backtrack using pop_back().
7. Save when target == 0.
```

# 66. Permutations

## 1. Problem Understanding

You are given an array `nums` containing unique integers.

Return all possible permutations of the array.

A permutation means an arrangement of all elements in a particular order.

**Important rules:**

1. Every element must be included exactly once.
2. The order of elements matters.
3. Return all possible arrangements.
4. The input contains unique integers.

**Example 1**

```cpp
nums = [1, 2, 3];
```

**Output:**

```text
[
    [1, 2, 3],
    [1, 3, 2],
    [2, 1, 3],
    [2, 3, 1],
    [3, 1, 2],
    [3, 2, 1]
]
```

**Example 2**

```cpp
nums = [7];
```

**Output:**

```text
[
    [7]
]
```

For an array containing `n` unique elements, the total number of permutations is:

\[
n!
\]

For example, 3 elements produce:

\[
3! = 3 \times 2 \times 1 = 6
\]

---

## 2. Core Intuition

We use **Backtracking**.

Unlike Subsets, where every element has two choices (include or skip), here we must select exactly one element for every position.

Consider:

```cpp
nums = [1, 2, 3];
```

Initially:

```text
current = []
```

For the first position, we can choose:

- 1
- 2
- 3

Suppose we select `1`.

```text
current = [1]
```

Now we cannot select `1` again because every element must be used exactly once.

We can choose:

- 2
- 3

Suppose we select `2`.

```text
current = [1, 2]
```

Only `3` remains.

```text
current = [1, 2, 3]
```

We have completed one permutation.

After saving it, we backtrack and explore other possible arrangements.

**The central idea:**

1. Choose an unused element.
2. Add it to the current permutation.
3. Explore recursively.
4. Remove it (backtrack).
5. Try another unused element.

---

## 3. Complete C++ Solution

We use a `visited` array to track which elements have already been selected.

```cpp
class Solution {
private:

    void backtrack(
        vector<int>& nums,
        vector<bool>& visited,
        vector<int>& current,
        vector<vector<int>>& result
    ) {

        // A complete permutation is formed.
        if (current.size() == nums.size()) {
            result.push_back(current);
            return;
        }

        // Try every available element.
        for (int i = 0; i < nums.size(); i++) {

            // Skip elements that are already used.
            if (visited[i]) {
                continue;
            }

            // Choose the current element.
            current.push_back(nums[i]);
            visited[i] = true;

            // Explore the next position.
            backtrack(nums, visited, current, result);

            // Backtrack: undo the choice.
            current.pop_back();
            visited[i] = false;
        }
    }

public:

    vector<vector<int>> permute(vector<int>& nums) {

        vector<vector<int>> result;
        vector<int> current;

        vector<bool> visited(nums.size(), false);

        backtrack(nums, visited, current, result);

        return result;
    }
};
```

---

## 4. Understanding the Backtracking Function

Our helper function:

```cpp
void backtrack(
    vector<int>& nums,
    vector<bool>& visited,
    vector<int>& current,
    vector<vector<int>>& result
)
```

Each parameter has a specific responsibility:

| Parameter | Meaning |
|---|---|
| `nums` | Original input array |
| `visited` | Tracks which elements have already been used |
| `current` | Permutation currently being constructed |
| `result` | Stores all completed permutations |

We initialize:

```cpp
vector<bool> visited(nums.size(), false);
```

For example:

```cpp
nums = [1, 2, 3];
visited = [false, false, false];
```

Initially, no element has been selected.

When we select an element, its corresponding `visited` value becomes `true`.

---

## 5. Understanding the Base Case

```cpp
if (current.size() == nums.size()) {
    result.push_back(current);
    return;
}
```

When the size of `current` becomes equal to the size of `nums`, we have selected every element exactly once.

Example:

```cpp
nums = [1, 2, 3];
current = [1, 2, 3];
```

Both arrays have size 3.

Therefore, we have completed one permutation.

Save it:

```cpp
result.push_back(current);
```

Then return to explore other possible arrangements.

**Important:** We use `current.size()` because a permutation is complete when every position has been filled.

---

## 6. Understanding the Visited Array

This is one of the most important concepts in this problem.

```cpp
if (visited[i]) {
    continue;
}
```

Why do we need it?

Because each element can be used only once in a permutation.

Suppose:

```cpp
nums = [1, 2, 3];
```

We select `1`.

```cpp
current = [1];
visited = [true, false, false];
```

Now the loop checks every element again.

- `1` is already used, so skip it.
- `2` is available.
- `3` is available.

Therefore, we cannot select `1` again.

**After selecting an element:**

```cpp
current.push_back(nums[i]);
visited[i] = true;
```

**After returning from recursion:**

```cpp
current.pop_back();
visited[i] = false;
```

The second part restores the previous state so that the element can be used in another branch.

---

## 7. Understanding the Backtracking Process

Look at these operations:

```cpp
current.push_back(nums[i]);

visited[i] = true;

backtrack(nums, visited, current, result);

current.pop_back();

visited[i] = false;
```

Their responsibilities are:

| Operation | Purpose |
|---|---|
| `push_back()` | Add the selected element |
| `visited[i] = true` | Mark the element as used |
| `backtrack()` | Explore the next position |
| `pop_back()` | Remove the selected element |
| `visited[i] = false` | Make the element available again |

Think of it as:

**Choose → Mark → Explore → Undo → Try another choice.**

Both `pop_back()` and resetting `visited[i]` are necessary.

---

## 8. Complete Dry Run

**Input:**

```cpp
nums = [1, 2, 3];
```

Initially:

```cpp
current = [];
visited = [false, false, false];
result = [];
```

### Step 1: Select 1

Select the first element:

```cpp
current = [1];
visited = [true, false, false];
```

Now explore the remaining unused elements.

### Step 2: Select 2

```cpp
current = [1, 2];
visited = [true, true, false];
```

Only `3` remains available.

Select `3`:

```cpp
current = [1, 2, 3];
visited = [true, true, true];
```

All elements are selected.

Save:

```cpp
result = [[1, 2, 3]];
```

### Step 3: Backtrack from 3

Undo the selection of `3`:

```cpp
current = [1, 2];
visited = [true, true, false];
```

No more unused elements are available at this level.

Backtrack from `2`:

```cpp
current = [1];
visited = [true, false, false];
```

Now select `3` instead of `2`.

### Step 4: Select 3

```cpp
current = [1, 3];
visited = [true, false, true];
```

Now select the remaining element `2`:

```cpp
current = [1, 3, 2];
```

Save:

```cpp
result = [
    [1, 2, 3],
    [1, 3, 2]
];
```

### Step 5: Backtrack from 1

After exploring both possibilities starting with `1`, undo the selection of `1`.

```cpp
current = [];
visited = [false, false, false];
```

Now select `2` as the first element.

The remaining arrangements are:

```text
[2, 1, 3]
[2, 3, 1]
```

Next, select `3` as the first element.

The remaining arrangements are:

```text
[3, 1, 2]
[3, 2, 1]
```

**Final result:**

```text
[
    [1, 2, 3],
    [1, 3, 2],
    [2, 1, 3],
    [2, 3, 1],
    [3, 1, 2],
    [3, 2, 1]
]
```

We get exactly six permutations.

---

## 9. Visualizing the Decision Tree

For:

```cpp
nums = [1, 2, 3];
```

A simplified decision tree:

```text
                         []
                   /     |     \
                  1      2      3
                 / \    / \    / \
                2   3  1   3  1   2
                |   |  |   |  |   |
                3   2  3   1  2   1

              [1,2,3] [1,3,2]
              [2,1,3] [2,3,1]
              [3,1,2] [3,2,1]
```

At every level, we select one unused element.

The depth of the recursion tree is equal to the number of elements.

For three elements:

- First position: 3 choices.
- Second position: 2 choices.
- Third position: 1 choice.

Therefore:

\[
3 \times 2 \times 1 = 3! = 6
\]

---

## 10. Why Is the Time Complexity O(n × n!)?

Let `n` be the number of elements.

The number of permutations is:

\[
n!
\]

For every completed permutation, we copy up to `n` elements into the result.

Therefore, the time complexity is:

\[
\boxed{O(n \times n!)}
\]

**Space Complexity**

- Recursion depth: O(n).
- Current permutation: O(n).
- Visited array: O(n).
- Output: O(n × n!).

Auxiliary space excluding the output is:

\[
\boxed{O(n)}
\]

---

## 11. Common Interview Mistakes

1. Forgetting the visited array: This can cause the same element to be selected multiple times.
2. Forgetting `visited[i] = true`: The recursive call may select an already-used element.
3. Forgetting `visited[i] = false`: The element will remain unavailable in other branches.
4. Forgetting `pop_back()`: Elements from one permutation can remain in another.
5. Using the wrong base case: Check `current.size() == nums.size()`.
6. Confusing permutations with subsets: Every permutation must contain all elements.
7. Thinking the order does not matter: In permutations, order matters.

---

## 12. Subsets vs Combinations vs Permutations

These three problems are important backtracking patterns.

| Feature | Subsets | Combination Sum | Permutations |
|---|---|---|---|
| Main goal | Generate every subset | Reach a target sum | Generate every arrangement |
| Order matters | No | No | Yes |
| Can skip elements | Yes | Yes | No, every element must be used |
| Reuse elements | No | Unlimited | No |
| Main technique | Include/Skip | Include/Skip | Choose unused element |
| Tracking | Index | Index and remaining target | Visited array |

The most important difference:

- **Subsets:** Every element can be included or skipped.
- **Combination Sum:** Select candidates until the target becomes zero.
- **Permutations:** Every element must be selected exactly once, but in different orders.

---

## 13. Alternative Approach: Swapping

There is another common way to generate permutations without using a visited array.

Instead of tracking used elements, we fix one position at a time by swapping elements.

### Complete C++ Solution

```cpp
class Solution {
private:

    void backtrack(
        int index,
        vector<int>& nums,
        vector<vector<int>>& result
    ) {

        // All positions have been fixed.
        if (index == nums.size()) {
            result.push_back(nums);
            return;
        }

        for (int i = index; i < nums.size(); i++) {

            // Choose an element for the current position.
            swap(nums[index], nums[i]);

            // Fix the next position.
            backtrack(index + 1, nums, result);

            // Undo the swap.
            swap(nums[index], nums[i]);
        }
    }

public:

    vector<vector<int>> permute(vector<int>& nums) {

        vector<vector<int>> result;

        backtrack(0, nums, result);

        return result;
    }
};
```

### Understanding the Swapping Approach

Suppose:

```cpp
nums = [1, 2, 3];
```

Initially, `index = 0`.

We try every element at position `0`.

**First choice: Keep 1 at position 0**

```text
[1, 2, 3]
```

Move to index `1`.

We can arrange the remaining elements as:

```text
[1, 2, 3]
[1, 3, 2]
```

**Second choice: Put 2 at position 0**

Swap `1` and `2`:

```text
[2, 1, 3]
```

Arrange the remaining elements:

```text
[2, 1, 3]
[2, 3, 1]
```

**Third choice: Put 3 at position 0**

Swap `1` and `3`:

```text
[3, 2, 1]
```

Arrange the remaining elements:

```text
[3, 2, 1]
[3, 1, 2]
```

After every recursive call, we undo the swap to restore the previous arrangement.

### Important Operations

```cpp
swap(nums[index], nums[i]);

backtrack(index + 1, nums, result);

swap(nums[index], nums[i]);
```

The first swap makes a choice.

The recursive call explores that choice.

The second swap undoes the choice.

### Complexity

- Time Complexity: O(n × n!)
- Auxiliary Space: O(n) for recursion.
- Output Space: O(n × n!).

---

## 14. Comparing Both Approaches

| Feature | Visited Array | Swapping |
|---|---|---|
| Tracks used elements | Yes | No |
| Uses `visited[]` | Yes | No |
| Uses `swap()` | No | Yes |
| Modifies input array | No | Yes, temporarily |
| Backtracking operation | `pop_back()` and reset visited | Swap back |
| Auxiliary space | O(n) | O(n) |

Both approaches are correct.

The visited-array approach is often easier to understand initially because it explicitly tracks which elements are available.

The swapping approach is another useful technique to recognize in interviews.

---

## 15. Pattern Recognition for Interviews

Think about permutations when a problem asks you to generate:

- All possible arrangements.
- Every ordering of the given elements.
- Different sequences using all elements.
- Every possible ordering without reusing an element.

A useful question:

**Does the order of the selected elements matter?**

- If no, consider subsets or combinations.
- If yes, consider permutations.

Another useful question:

**Have I already used this element in the current arrangement?**

If yes, do not select it again.

---

## 16. 30-Second Interview Revision

**Q1. What is a permutation?**

An arrangement of all elements in a particular order.

**Q2. Does order matter in permutations?**

Yes. `[1, 2, 3]` and `[1, 3, 2]` are different permutations.

**Q3. Why do we use a visited array?**

To track which elements have already been selected in the current permutation.

**Q4. Why is the base case `current.size() == nums.size()`?**

Because a complete permutation must contain every input element exactly once.

**Q5. Why do we reset `visited[i] = false`?**

To make the element available for other recursive branches.

**Q6. What is the total number of permutations for n unique elements?**

\[
n!
\]

**Q7. What is the time complexity?**

\[
O(n \times n!)
\]

Because there are `n!` permutations and copying each permutation takes O(n) time.

**Final takeaway**

```text
PERMUTATIONS = CHOOSE UNUSED ELEMENT

For every position:
    1. Try every unused element.
    2. Add the element to current.
    3. Mark it as visited.
    4. Explore recursively.
    5. Remove the element.
    6. Mark it as unvisited.

When current.size() == nums.size():
    Save the permutation.
```
# 67. Subsets II

## 1. Problem Understanding

You are given an integer array `nums` that may contain duplicate elements.

Return all possible subsets (the power set).

**Important rules:**

1. The input array may contain duplicate values.
2. Return all possible subsets.
3. The solution must not contain duplicate subsets.
4. Every element can be selected at most once.

**Example 1**

```cpp
nums = [1, 2, 1];
```

**Output:**

```text
[
    [],
    [1],
    [1, 1],
    [1, 1, 2],
    [1, 2],
    [2]
]
```

**Example 2**

```cpp
nums = [7, 7];
```

**Output:**

```text
[
    [],
    [7],
    [7, 7]
]
```

Notice that `[7]` appears only once, even though the input contains two occurrences of `7`.

---

## 2. Core Intuition

We use **Backtracking**, just like the Subsets problem.

But there is one important difference:

In Subsets, all input elements are unique.

In Subsets II, duplicate values may exist, so we must avoid generating the same subset multiple times.

Consider:

```cpp
nums = [1, 2, 1];
```

First, sort the array:

```text
[1, 1, 2]
```

Now duplicate values are adjacent.

We use a `for` loop to explore possible elements.

At every recursion level:

1. Choose an element.
2. Add it to the current subset.
3. Explore recursively.
4. Remove it (backtrack).
5. Try the next element.

**The important condition:**

```cpp
if (i > start && nums[i] == nums[i - 1]) {
    continue;
}
```

This skips duplicate choices at the same recursion level.

It does not prevent us from selecting two equal elements in the same subset.

---

## 3. Complete C++ Solution

```cpp
class Solution {
private:

    void backtrack(
        int start,
        vector<int>& nums,
        vector<int>& current,
        vector<vector<int>>& result
    ) {

        // Every current state is a valid subset.
        result.push_back(current);

        for (int i = start; i < nums.size(); i++) {

            // Skip duplicates at the same recursion level.
            if (i > start && nums[i] == nums[i - 1]) {
                continue;
            }

            // Choose the current element.
            current.push_back(nums[i]);

            // Move to the next index because each element is used once.
            backtrack(i + 1, nums, current, result);

            // Backtrack: remove the selected element.
            current.pop_back();
        }
    }

public:

    vector<vector<int>> subsetsWithDup(vector<int>& nums) {

        vector<vector<int>> result;
        vector<int> current;

        // Sort to group duplicate values together.
        sort(nums.begin(), nums.end());

        backtrack(0, nums, current, result);

        return result;
    }
};
```

---

## 4. Understanding the Parameters

Our helper function:

```cpp
void backtrack(
    int start,
    vector<int>& nums,
    vector<int>& current,
    vector<vector<int>>& result
)
```

Each parameter has a specific responsibility:

| Parameter | Meaning |
|---|---|
| `start` | Starting index for the current recursion level |
| `nums` | Original input array |
| `current` | Subset currently being constructed |
| `result` | Stores all unique subsets |

We start with:

```cpp
backtrack(0, nums, current, result);
```

This means we begin exploring elements from index `0`.

---

## 5. Why Do We Sort the Array?

```cpp
sort(nums.begin(), nums.end());
```

Sorting is important because it groups duplicate values together.

Example:

Original array:

```text
[1, 2, 1]
```

After sorting:

```text
[1, 1, 2]
```

Now duplicate values are adjacent.

This allows us to identify duplicate choices easily.

Without sorting, duplicate values might appear at different positions, making duplicate skipping difficult.

---

## 6. Understanding the Most Important Line

```cpp
if (i > start && nums[i] == nums[i - 1]) {
    continue;
}
```

This is the most important condition in Subsets II.

Let's understand it with an example.

Suppose:

```cpp
nums = [1, 1, 2];
```

At the first recursion level:

```cpp
start = 0;
```

The loop starts from index `0`.

**First iteration:**

```cpp
i = 0;
nums[i] = 1;
```

Since `i > start` is false, we select the first `1`.

**Second iteration:**

```cpp
i = 1;
nums[i] = 1;
```

Now:

```cpp
i > start
```

is true.

And:

```cpp
nums[i] == nums[i - 1]
```

is also true.

Therefore, we skip this duplicate choice at the current recursion level.

### But can we still select two 1s?

Yes!

When we select the first `1`, recursion starts from index `1`.

At that new recursion level:

```cpp
start = 1;
i = 1;
```

Now:

```cpp
i > start
```

is false.

Therefore, the second `1` can be selected.

This allows us to generate:

```text
[1, 1]
```

**Important distinction:**

We skip duplicate choices at the same recursion level, not duplicate values at every level.

---

## 7. Understanding the Backtracking Process

Look at these operations:

```cpp
current.push_back(nums[i]);

backtrack(i + 1, nums, current, result);

current.pop_back();
```

Their responsibilities:

| Operation | Purpose |
|---|---|
| `push_back()` | Add the selected element |
| `backtrack()` | Explore the next possible selections |
| `pop_back()` | Undo the previous choice |

Unlike Combination Sum, we use:

```cpp
backtrack(i + 1, ...);
```

Why?

Because each array element can be used only once.

Even if two elements have the same value, they are still separate positions in the input array.

---

## 8. Understanding the Base Case

Notice that we do not need a traditional base case like:

```cpp
if (start == nums.size())
```

Instead, we save the current subset at the beginning of every recursive call:

```cpp
result.push_back(current);
```

Why?

Because every current state represents a valid subset.

For example:

```text
[]
[1]
[1, 1]
[1, 1, 2]
[1, 2]
[2]
```

All these are valid subsets.

When the loop has no more available elements, the recursive call simply returns.

---

## 9. Complete Dry Run

**Input:**

```cpp
nums = [1, 2, 1];
```

After sorting:

```cpp
nums = [1, 1, 2];
```

Initially:

```cpp
current = [];
result = [];
start = 0;
```

### Step 1: Start with an empty subset

At the first recursive call:

```cpp
result = [[]];
```

### Step 2: Select the first 1

Select index `0`:

```cpp
current = [1];
```

Recurse from index `1`.

Save:

```cpp
result = [
    [],
    [1]
];
```

### Step 3: Select the second 1

At this level:

```cpp
start = 1;
i = 1;
```

Since `i > start` is false, we can select the second `1`.

```cpp
current = [1, 1];
```

Recurse from index `2`.

Save:

```cpp
result = [
    [],
    [1],
    [1, 1]
];
```

### Step 4: Select 2

Select index `2`:

```cpp
current = [1, 1, 2];
```

Recurse from index `3`.

Save:

```cpp
result = [
    [],
    [1],
    [1, 1],
    [1, 1, 2]
];
```

No more elements are available.

Backtrack and remove `2`.

### Step 5: Backtrack and skip the duplicate 1

Return to the recursion level where:

```cpp
current = [1];
start = 1;
```

The next loop iteration is:

```cpp
i = 2;
nums[i] = 2;
```

Select `2`:

```cpp
current = [1, 2];
```

Save:

```cpp
result = [
    [],
    [1],
    [1, 1],
    [1, 1, 2],
    [1, 2]
];
```

### Step 6: Backtrack to the root

Remove `2`, then remove the first `1`.

Now:

```cpp
current = [];
start = 0;
```

At the root level, the loop reaches index `1`.

Both values are equal:

```cpp
nums[1] == nums[0]
```

And:

```cpp
i > start
```

is true.

Therefore, we skip the second `1` at this level.

### Step 7: Select 2

Select the next available element:

```cpp
current = [2];
```

Save:

```cpp
result = [
    [],
    [1],
    [1, 1],
    [1, 1, 2],
    [1, 2],
    [2]
];
```

**Final output:**

```text
[
    [],
    [1],
    [1, 1],
    [1, 1, 2],
    [1, 2],
    [2]
]
```

We have generated all unique subsets without duplicates.

---

## 10. Visualizing the Recursion

For:

```cpp
nums = [1, 1, 2];
```

A simplified decision tree:

```text
                         []
                       /    \
                      1      2
                     /
                   [1]
                  /    \
                 1      2
                /        \
             [1,1]      [1,2]
                |
                2
                |
             [1,1,2]
```

Notice:

- At the root level, the second `1` is skipped.
- After selecting the first `1`, the second `1` is allowed.
- This prevents duplicate subsets while preserving valid combinations.

The total number of unique subsets for this input is:

```text
6
```

---

## 11. Complexity Analysis

Let `n` be the number of elements.

**Time Complexity**

- Sorting: O(n log n).
- Backtracking: O(n × 2ⁿ) in the worst case, including copying subsets into the output.

Therefore:

\[
\boxed{O(n \times 2^n)}
\]

Duplicate skipping can reduce the number of explored branches.

**Space Complexity**

- Recursion depth: O(n).
- Current subset: O(n).
- Sorting auxiliary space: implementation-dependent.

Auxiliary space excluding output is O(n).

The output can contain up to O(n × 2ⁿ) elements in total.

---

## 12. Common Interview Mistakes

1. Forgetting to sort: Duplicate skipping depends on adjacent equal values.
2. Using `i > 0` instead of `i > start`: This can incorrectly skip duplicates at deeper recursion levels.
3. Skipping duplicates at every recursion level: This prevents selecting multiple equal elements in one subset.
4. Forgetting `pop_back()`: Elements from one subset may remain in another.
5. Using `backtrack(i, ...)`: Each input index should be used at most once.
6. Saving the subset only at the end: Every current state is a valid subset and must be saved.
7. Confusing Subsets II with Combination Sum II: Both skip duplicates, but their goals and recursion structures are different.

---

## 13. Subsets vs Subsets II

This comparison is important for interviews.

| Feature | Subsets | Subsets II |
|---|---|---|
| Input elements | Unique | May contain duplicates |
| Goal | Generate all subsets | Generate unique subsets |
| Sorting required | No | Yes |
| Duplicate skipping | Not required | Required |
| Recursive call | `i + 1` | `i + 1` |
| Main technique | Backtracking | Backtracking + sorting |

The main difference is the duplicate-skipping condition.

**Subsets:**

```cpp
backtrack(i + 1, nums, current, result);
```

**Subsets II:**

```cpp
if (i > start && nums[i] == nums[i - 1]) {
    continue;
}

backtrack(i + 1, nums, current, result);
```

---

## 14. Subsets II vs Combination Sum II

Both problems contain duplicate input values, but their objectives are different.

| Feature | Subsets II | Combination Sum II |
|---|---|---|
| Goal | Generate all unique subsets | Find combinations matching target |
| Target required | No | Yes |
| Sorting | Yes | Yes |
| Duplicate skipping | Yes | Yes |
| Include recursion | `i + 1` | `i + 1` |
| Save condition | Every recursive call | `target == 0` |
| Early stopping | Not required | `nums[i] > target` can use `break` |

Both use the same duplicate-skipping condition:

```cpp
if (i > start && nums[i] == nums[i - 1]) {
    continue;
}
```

---

## 15. Pattern Recognition for Interviews

Think about this pattern when a problem asks you to:

- Generate all unique subsets.
- Handle duplicate values in an array.
- Avoid duplicate combinations or selections.
- Explore different subsets using backtracking.

A useful question:

**Can two different input positions generate the same subset?**

If yes:

1. Sort the input.
2. Use a loop-based backtracking approach.
3. Skip duplicate choices at the same recursion level.

---

## 16. 30-Second Interview Revision

**Q1. Why do we sort the array?**

To group duplicate values together so we can skip duplicate choices.

**Q2. What is the purpose of `i > start`?**

It ensures that we skip duplicates only at the same recursion level.

**Q3. Can we select two equal values?**

Yes, if they are separate elements in the input array.

**Q4. Why use `i + 1`?**

Because each array element can be selected only once.

**Q5. Why save `current` at every recursive call?**

Because every current state represents a valid subset.

**Q6. What is the time complexity?**

O(n × 2ⁿ), including copying all subsets into the output.

**Final takeaway**

```text
SUBSETS II = SUBSETS + DUPLICATE SKIPPING

1. Sort the input array.
2. Start backtracking.
3. Save current subset at every recursive call.
4. Loop through available elements.
5. Skip duplicates at the same level:

   if (i > start && nums[i] == nums[i - 1])
       continue;

6. Include the current element.
7. Recurse using i + 1.
8. Backtrack using pop_back().
```

# 68. Generate Parentheses

## 1. Problem Understanding

You are given an integer `n`, representing the number of pairs of parentheses.

Generate all combinations of well-formed (valid) parentheses.

**Important rules:**

1. Every opening parenthesis `(` must have a matching closing parenthesis `)`.
2. The total number of opening parentheses must be `n`.
3. The total number of closing parentheses must be `n`.
4. A closing parenthesis cannot appear before its matching opening parenthesis.

**Example 1**

```cpp
n = 3;
```

**Output:**

```text
[
    "((()))",
    "(()())",
    "(())()",
    "()(())",
    "()()()"
]
```

**Example 2**

```cpp
n = 1;
```

**Output:**

```text
[
    "()"
]
```

For `n` pairs, the number of valid combinations is given by the Catalan number:

\[
C_n = \frac{1}{n+1}\binom{2n}{n}
\]

For example:

- n = 1 → 1 combination
- n = 2 → 2 combinations
- n = 3 → 5 combinations
- n = 4 → 14 combinations

---

## 2. Core Intuition

We use **Backtracking**.

At every step, we have two possible choices:

1. Add an opening parenthesis `(`.
2. Add a closing parenthesis `)`.

But we cannot make these choices freely. We must follow two rules to ensure that the generated string is always valid.

### Rule 1: We can add an opening parenthesis if:

```cpp
open < n
```

This means we have not used all available opening parentheses.

### Rule 2: We can add a closing parenthesis if:

```cpp
close < open
```

This means we have more opening parentheses than closing parentheses.

Why?

Because every closing parenthesis must have an opening parenthesis before it.

**Example:**

For `n = 3`:

```text
(
((
(((
```

We can add `)` after opening parentheses have been added:

```text
((()
((())
((()))
```

A string such as:

```text
)(
```

is invalid because the closing parenthesis appears before the opening parenthesis.

**The central idea:**

- Add `(` while `open < n`.
- Add `)` while `close < open`.
- Save the string when its length becomes `2 * n`.

---

## 3. Complete C++ Solution

```cpp
class Solution {
private:

    void backtrack(
        int open,
        int close,
        int n,
        string& current,
        vector<string>& result
    ) {

        // A complete valid combination is formed.
        if (current.size() == 2 * n) {
            result.push_back(current);
            return;
        }

        // Choice 1: Add an opening parenthesis.
        if (open < n) {

            current.push_back('(');

            backtrack(
                open + 1,
                close,
                n,
                current,
                result
            );

            // Backtrack.
            current.pop_back();
        }

        // Choice 2: Add a closing parenthesis.
        if (close < open) {

            current.push_back(')');

            backtrack(
                open,
                close + 1,
                n,
                current,
                result
            );

            // Backtrack.
            current.pop_back();
        }
    }

public:

    vector<string> generateParenthesis(int n) {

        vector<string> result;
        string current;

        backtrack(0, 0, n, current, result);

        return result;
    }
};
```

---

## 4. Understanding the Parameters

Our helper function:

```cpp
void backtrack(
    int open,
    int close,
    int n,
    string& current,
    vector<string>& result
)
```

Each parameter has a specific responsibility:

| Parameter | Meaning |
|---|---|
| `open` | Number of opening parentheses used |
| `close` | Number of closing parentheses used |
| `n` | Required number of pairs |
| `current` | Parentheses string currently being constructed |
| `result` | Stores all valid combinations |

We start with:

```cpp
backtrack(0, 0, n, current, result);
```

Initially:

```text
open = 0
close = 0
current = ""
```

No parentheses have been selected yet.

---

## 5. Understanding the Two Conditions

These are the most important conditions in this problem.

### A. Opening Parenthesis Condition

```cpp
if (open < n)
```

Why?

Because we can use only `n` opening parentheses.

For example, when:

```text
n = 3
open = 3
```

We cannot add another `(`.

Otherwise, we would have more than three opening parentheses.

### B. Closing Parenthesis Condition

```cpp
if (close < open)
```

Why?

Because a closing parenthesis must have a corresponding opening parenthesis before it.

Example:

```text
current = "((("
open = 3
close = 0
```

We can safely add `)`.

After adding one:

```text
current = "((()"
open = 3
close = 1
```

We can add another `)` because:

```text
1 < 3
```

But consider:

```text
current = "())"
open = 2
close = 2
```

We cannot add another `)` because:

```text
2 < 2
```

is false.

This prevents invalid parentheses combinations.

---

## 6. Understanding the Base Case

```cpp
if (current.size() == 2 * n) {
    result.push_back(current);
    return;
}
```

Why `2 * n`?

Because every valid combination contains:

- `n` opening parentheses.
- `n` closing parentheses.

Therefore, the total string length is:

\[
n+n=2n
\]

For example, when:

```text
n = 3
```

The required string length is:

```text
2 * 3 = 6
```

When the string reaches length 6, we have completed one valid combination.

Save it:

```cpp
result.push_back(current);
```

---

## 7. Understanding Backtracking

Look at these operations:

```cpp
current.push_back('(');

backtrack(open + 1, close, n, current, result);

current.pop_back();
```

And:

```cpp
current.push_back(')');

backtrack(open, close + 1, n, current, result);

current.pop_back();
```

Their responsibilities:

| Operation | Purpose |
|---|---|
| `push_back('(')` | Add an opening parenthesis |
| `open + 1` | Increase opening count |
| `push_back(')')` | Add a closing parenthesis |
| `close + 1` | Increase closing count |
| `pop_back()` | Undo the previous choice |

Think of it as:

**Choose → Explore → Undo → Try another choice.**

Notice that we do not need a separate visited array because we are building a string and tracking the counts of opening and closing parentheses.

---

## 8. Complete Dry Run

**Input:**

```cpp
n = 2;
```

Initially:

```text
open = 0
close = 0
current = ""
result = []
```

### Step 1: Add an opening parenthesis

Since:

```cpp
open < n
```

we can add `(`.

```text
current = "("
open = 1
close = 0
```

### Step 2: Add another opening parenthesis

Again:

```cpp
open < n
```

is true.

Add another `(`.

```text
current = "(("
open = 2
close = 0
```

We cannot add another opening parenthesis because `open == n`.

But we can add a closing parenthesis.

### Step 3: Add a closing parenthesis

Since:

```cpp
close < open
```

we can add `)`.

```text
current = "(()"
open = 2
close = 1
```

Add another closing parenthesis:

```text
current = "(())"
open = 2
close = 2
```

The string length is now:

```text
2 * n = 4
```

Save:

```text
result = ["(())"]
```

### Step 4: Backtrack

Undo the last closing parenthesis.

```text
current = "(()"
```

There are no more valid choices in this branch.

Backtrack again and explore another possibility.

### Step 5: Explore the second combination

Return to:

```text
current = "("
open = 1
close = 0
```

Now add a closing parenthesis:

```text
current = "()"
open = 1
close = 1
```

We can add another opening parenthesis:

```text
current = "()("
open = 2
close = 1
```

Add a closing parenthesis:

```text
current = "()()"
open = 2
close = 2
```

Save:

```text
result = [
    "(())",
    "()()"
]
```

**Final output:**

```text
[
    "(())",
    "()()"
]
```

There are exactly two valid combinations for `n = 2`.

---

## 9. Visualizing the Decision Tree

For:

```cpp
n = 2;
```

A simplified decision tree:

```text
                         ""
                         |
                         (
                      /     \
                    ((      ()
                    |        |
                   (()      ()(
                    |        |
                  (())      ()()
```

The invalid branches are never explored.

For example:

```text
)(
```

is never generated because a closing parenthesis cannot be added when:

```cpp
close == open
```

This is called **pruning**.

Instead of generating invalid strings and checking them afterward, we avoid invalid choices from the beginning.

---

## 10. Why Is This Called Pruning?

Pruning means avoiding branches that cannot produce a valid answer.

Consider:

```text
current = ")"
```

This is already invalid.

There is no reason to continue exploring this branch.

Our condition:

```cpp
if (close < open)
```

prevents us from creating such a branch.

Similarly:

```cpp
if (open < n)
```

prevents us from using too many opening parentheses.

This makes the backtracking solution efficient because it explores only valid prefixes.

---

## 11. Complexity Analysis

Let `n` be the number of pairs.

The number of valid combinations is the nth Catalan number:

\[
C_n = \frac{1}{n+1}\binom{2n}{n}
\]

Each valid string has length `2n`.

Therefore, copying all valid strings into the output takes:

\[
O(n \times C_n)
\]

**Time Complexity:**

\[
\boxed{O(n \times C_n)}
\]

**Space Complexity:**

- Recursion depth: O(n).
- Current string: O(n).
- Output: O(n × Cₙ).

Auxiliary space excluding output:

\[
\boxed{O(n)}
\]

---

## 12. Common Interview Mistakes

1. Allowing more than `n` opening parentheses: Always check `open < n`.
2. Adding a closing parenthesis without an opening one: Check `close < open`.
3. Using `close <= open`: This can generate invalid combinations.
4. Using the wrong base case: Check `current.size() == 2 * n`.
5. Forgetting `pop_back()`: This contaminates other recursive branches.
6. Generating every possible string first: This creates unnecessary invalid combinations.
7. Confusing the number of pairs with the total string length: The length must be `2 * n`.

---

## 13. Subsets vs Permutations vs Generate Parentheses

These are three important backtracking patterns.

| Feature | Subsets | Permutations | Generate Parentheses |
|---|---|---|---|
| Main goal | Generate every subset | Generate every arrangement | Generate valid parentheses |
| Main choices | Include or skip | Choose unused element | Add `(` or `)` |
| Tracking | Index | Visited array | Open and close counts |
| Reuse elements | No | No | Limited by `n` |
| Validity condition | All subsets are valid | All elements used once | `close < open` |
| Base case | All elements processed | All elements selected | String length = `2 * n` |

The main difference is that Generate Parentheses uses constraints to prevent invalid choices.

---

## 14. Pattern Recognition for Interviews

Think about this pattern when a problem asks you to:

- Generate valid parentheses combinations.
- Generate balanced brackets.
- Construct valid sequences under restrictions.
- Explore choices while maintaining a validity condition.

A useful question:

**Can I determine whether a choice is valid before exploring it?**

If yes, use backtracking with pruning.

For Generate Parentheses:

- Add `(` if `open < n`.
- Add `)` if `close < open`.

---

## 15. 30-Second Interview Revision

**Q1. Why use backtracking?**

Because we need to generate all valid parentheses combinations by exploring different choices.

**Q2. When can we add `(`?**

When:

```cpp
open < n
```

**Q3. When can we add `)`?**

When:

```cpp
close < open
```

**Q4. Why is the base case `current.size() == 2 * n`?**

Because every valid combination contains exactly `n` opening and `n` closing parentheses.

**Q5. What is pruning?**

Avoiding invalid recursive branches before exploring them.

**Q6. How many valid combinations exist?**

The nth Catalan number:

\[
C_n = \frac{1}{n+1}\binom{2n}{n}
\]

**Q7. What is the time complexity?**

\[
O(n \times C_n)
\]

Because there are `Cₙ` valid combinations and each has length `2n`.

**Final takeaway**

```text
GENERATE PARENTHESES = VALID CHOICES ONLY

Opening:
    if (open < n)
        Add '('
        open + 1
        Explore
        Remove '('

Closing:
    if (close < open)
        Add ')'
        close + 1
        Explore
        Remove ')'

When current.size() == 2 * n:
    Save the valid combination.
```

# 69. Word Search

## 1. Problem Understanding

You are given:

- A 2D character matrix `board`.
- A string `word`.

Return `true` if the word exists in the board.

Otherwise, return `false`.

**Important rules:**

1. The word can be constructed from letters of sequentially adjacent cells.
2. Adjacent cells can be horizontal or vertical.
3. Diagonal movement is not allowed.
4. The same cell cannot be used more than once in a single word search.

**Example 1**

```cpp
board = {
    {'A', 'B', 'C', 'E'},
    {'S', 'F', 'C', 'S'},
    {'A', 'D', 'E', 'E'}
};

word = "ABCCED";
```

**Output:**

```text
true
```

Explanation:

The word can be formed by following adjacent cells:

```text
A → B → C → C → E → D
```

**Example 2**

```cpp
word = "SEE";
```

**Output:**

```text
true
```

**Example 3**

```cpp
word = "ABCB";
```

**Output:**

```text
false
```

The same cell cannot be used twice.

---

## 2. Core Intuition

We use **Depth First Search (DFS) + Backtracking**.

The idea is to search for the word starting from every cell in the board.

For each cell:

1. Check whether its character matches the current character of the word.
2. If it matches, explore its four neighboring cells.
3. Continue until the complete word is found.
4. If a path fails, backtrack and try another direction.

Consider:

```text
board =

A  B  C  E
S  F  C  S
A  D  E  E

word = "ABCCED"
```

We start from `A`.

Then we search:

```text
A → B → C → C → E → D
```

If all characters match in a valid path, return `true`.

**The central idea:**

- Try every possible starting cell.
- Explore four directions.
- Never reuse a cell in the current path.
- Restore the board when backtracking.

---

## 3. Complete C++ Solution

```cpp
class Solution {
private:

    bool dfs(
        int r,
        int c,
        int index,
        vector<vector<char>>& board,
        string& word
    ) {

        // Entire word has been found.
        if (index == word.size()) {
            return true;
        }

        // Check boundaries.
        if (
            r < 0 || c < 0 ||
            r >= board.size() ||
            c >= board[0].size()
        ) {
            return false;
        }

        // Current character does not match.
        if (board[r][c] != word[index]) {
            return false;
        }

        // Temporarily mark the cell as visited.
        char temp = board[r][c];
        board[r][c] = '#';

        // Explore four directions.
        bool found =
            dfs(r + 1, c, index + 1, board, word) ||
            dfs(r - 1, c, index + 1, board, word) ||
            dfs(r, c + 1, index + 1, board, word) ||
            dfs(r, c - 1, index + 1, board, word);

        // Restore the original character.
        board[r][c] = temp;

        return found;
    }

public:

    bool exist(vector<vector<char>>& board, string word) {

        int rows = board.size();
        int cols = board[0].size();

        // Try every cell as a starting point.
        for (int r = 0; r < rows; r++) {

            for (int c = 0; c < cols; c++) {

                if (dfs(r, c, 0, board, word)) {
                    return true;
                }
            }
        }

        return false;
    }
};
```

---

## 4. Understanding the DFS Function

Our helper function:

```cpp
bool dfs(
    int r,
    int c,
    int index,
    vector<vector<char>>& board,
    string& word
)
```

Each parameter has a specific responsibility:

| Parameter | Meaning |
|---|---|
| `r` | Current row |
| `c` | Current column |
| `index` | Current character position in the word |
| `board` | Original character matrix |
| `word` | Word we are searching for |

We start searching with:

```cpp
dfs(r, c, 0, board, word);
```

This means:

- Start from cell `(r, c)`.
- Match the first character of the word.
- Continue with the next character after a successful match.

---

## 5. Understanding the Base Cases

There are three important checks.

### A. Entire word has been found

```cpp
if (index == word.size()) {
    return true;
}
```

If `index` reaches the length of the word, all characters have been matched.

Therefore, return `true`.

### B. Check board boundaries

```cpp
if (
    r < 0 || c < 0 ||
    r >= board.size() ||
    c >= board[0].size()
) {
    return false;
}
```

We cannot access cells outside the board.

For example, if we are at the first row and move upward:

```text
r = -1
```

This position is invalid.

Therefore, return `false`.

### C. Character does not match

```cpp
if (board[r][c] != word[index]) {
    return false;
}
```

The current board character must match the current word character.

Otherwise, this path cannot produce the required word.

---

## 6. Understanding the Four Directions

From every matching cell, we can explore four directions:

```cpp
dfs(r + 1, c, index + 1, board, word); // Down

dfs(r - 1, c, index + 1, board, word); // Up

dfs(r, c + 1, index + 1, board, word); // Right

dfs(r, c - 1, index + 1, board, word); // Left
```

Notice that we use:

```cpp
index + 1
```

Why?

Because we have already matched the current character.

Now we need to find the next character in the word.

**Important:** Diagonal movement is not allowed.

---

## 7. Understanding the Visited Cell Concept

This is one of the most important parts of Word Search.

The problem says:

**We cannot use the same cell more than once in a single path.**

We temporarily mark the current cell:

```cpp
char temp = board[r][c];

board[r][c] = '#';
```

Why use `#`?

Because the board contains uppercase English letters, so `#` can act as a temporary visited marker.

If DFS reaches this cell again, it will not match the required character.

After exploring all directions, restore the original character:

```cpp
board[r][c] = temp;
```

This is backtracking.

**Choose → Mark visited → Explore → Restore.**

---

## 8. Complete Dry Run

**Input:**

```text
board =

A  B  C  E
S  F  C  S
A  D  E  E

word = "ABCCED"
```

Initially:

```text
index = 0
word[0] = 'A'
```

### Step 1: Find A

We start at:

```text
(0, 0) = A
```

It matches `word[0]`.

Mark it visited:

```text
#  B  C  E
S  F  C  S
A  D  E  E
```

Now search for `B`.

### Step 2: Move Right to B

```text
(0, 1) = B
```

It matches `word[1]`.

Mark it visited:

```text
#  #  C  E
S  F  C  S
A  D  E  E
```

Now search for `C`.

### Step 3: Move Right to C

```text
(0, 2) = C
```

It matches `word[2]`.

Mark it visited:

```text
#  #  #  E
S  F  C  S
A  D  E  E
```

Now search for the next `C`.

### Step 4: Move Down to C

```text
(1, 2) = C
```

It matches `word[3]`.

Mark it visited:

```text
#  #  #  E
S  F  #  S
A  D  E  E
```

Now search for `E`.

### Step 5: Move Down to E

```text
(2, 2) = E
```

It matches `word[4]`.

Mark it visited:

```text
#  #  #  E
S  F  #  S
A  D  #  E
```

Now search for `D`.

### Step 6: Move Left to D

```text
(2, 1) = D
```

It matches `word[5]`.

We have matched every character.

```text
A → B → C → C → E → D
```

The word is found.

Return:

```cpp
true;
```

The recursive calls restore the marked cells while returning.

---

## 9. Visualizing the Search Path

For:

```text
A  B  C  E
S  F  C  S
A  D  E  E
```

Word:

```text
ABCCED
```

The successful path is:

```text
A → B → C
        ↓
        C
        ↓
        E ← D
```

Coordinates:

| Character | Row | Column |
|---|---:|---:|
| A | 0 | 0 |
| B | 0 | 1 |
| C | 0 | 2 |
| C | 1 | 2 |
| E | 2 | 2 |
| D | 2 | 1 |

Every movement is horizontal or vertical.

No cell is visited twice.

---

## 10. Why Do We Need Backtracking?

Suppose one direction does not contain the next required character.

We must undo the current choice and try another direction.

For example:

```cpp
board[r][c] = '#';
```

Temporarily blocks the current cell.

After searching:

```cpp
board[r][c] = temp;
```

Restores it.

Without restoration, another search path might incorrectly treat the cell as already visited.

This is why backtracking is essential.

---

## 11. Complexity Analysis

Let:

- `m` = number of rows.
- `n` = number of columns.
- `L` = length of the word.

**Time Complexity**

We can start DFS from every cell.

There are `m × n` possible starting cells.

From each starting cell, we can explore up to four directions initially. After moving to a neighboring cell, we cannot immediately return to the previous cell because it is marked visited.

A commonly used upper bound is:

\[
\boxed{O(m \times n \times 3^L)}
\]

The first move has up to four choices, and subsequent moves have at most three new directions because the previous cell cannot be reused.

**Space Complexity**

- Recursion depth: O(L).
- No separate visited matrix is required.

Auxiliary space:

\[
\boxed{O(L)}
\]

---

## 12. Common Interview Mistakes

1. Forgetting boundary checks: This can access invalid board positions.
2. Forgetting to check the current character: Only matching cells should continue.
3. Allowing diagonal movement: Only four directions are permitted.
4. Forgetting to mark a cell visited: The same cell could be reused.
5. Forgetting to restore the original character: Other paths may be affected.
6. Using `index` instead of `index + 1`: The next recursive call must match the next character.
7. Starting DFS from only one cell: The word may begin anywhere in the board.
8. Returning `true` before matching the complete word: Every character must be matched.

---

## 13. Backtracking vs Word Search

This problem is different from ordinary backtracking problems such as Subsets and Permutations.

| Feature | Subsets | Permutations | Word Search |
|---|---|---|---|
| Main goal | Generate subsets | Generate arrangements | Find a word in a board |
| Choices | Include or skip | Choose unused element | Explore four directions |
| Tracking | Index | Visited array | Mark board cell |
| Completion condition | All elements processed | All elements selected | Entire word matched |
| Backtracking | `pop_back()` | `pop_back()` and reset visited | Restore board character |

The important difference is that Word Search uses a 2D grid and explores neighboring cells.

---

## 14. Pattern Recognition for Interviews

Think about DFS + Backtracking when a problem asks you to:

- Search for a word in a matrix.
- Explore connected neighboring cells.
- Find a path through a grid.
- Prevent revisiting the same cell.
- Explore multiple directions from each position.

A useful question:

**Am I searching for a path through a grid where each cell can be used only once?**

If yes, DFS + Backtracking is a strong approach.

---

## 15. 30-Second Interview Revision

**Q1. Why use DFS?**

To explore possible paths through the board.

**Q2. Why use backtracking?**

To restore the board after exploring a path so other paths can use those cells.

**Q3. How many directions can we explore?**

Four: up, down, left and right.

**Q4. Why mark a cell with `#`?**

To prevent the same cell from being reused in the current path.

**Q5. Why restore the original character?**

Because other search paths may need to use that cell.

**Q6. What is the base case?**

When `index == word.size()`, the complete word has been found.

**Q7. What is the time complexity?**

O(m × n × 3ᴸ) as a commonly used upper bound.

**Final takeaway**

```text
WORD SEARCH = DFS + BACKTRACKING

1. Try every cell as a starting point.
2. Check boundaries.
3. Match the current character.
4. Mark the cell as visited.
5. Explore four directions.
6. Restore the original character.
7. Return true when the complete word is found.
```
# 70. Palindrome Partitioning

## 1. Problem Understanding

You are given a string `s`.

Partition the string into substrings such that every substring is a palindrome.

Return all possible palindrome partitions.

**Important rules:**

1. Every character must belong to exactly one substring.
2. Every substring must be a palindrome.
3. The original order of characters must be maintained.
4. Return all possible valid partitions.

A palindrome is a string that reads the same forward and backward.

Examples:

```text
"aba"  → Palindrome
"aa"   → Palindrome
"abc"  → Not a palindrome
"racecar" → Palindrome
```

**Example 1**

```cpp
s = "aab";
```

**Output:**

```text
[
    ["a", "a", "b"],
    ["aa", "b"]
]
```

Explanation:

There are two valid ways to partition `"aab"`:

- `"a" + "a" + "b"`
- `"aa" + "b"`

Every substring in both partitions is a palindrome.

**Example 2**

```cpp
s = "a";
```

**Output:**

```text
[
    ["a"]
]
```

---

## 2. Core Intuition

We use **Backtracking + Palindrome Checking**.

The idea is to try every possible substring starting from the current index.

For each substring:

1. Check whether it is a palindrome.
2. If it is a palindrome, add it to the current partition.
3. Recursively process the remaining string.
4. Remove the selected substring (backtrack).
5. Try another possible substring.

Consider:

```cpp
s = "aab";
```

Starting from index `0`, we can try:

```text
"a"
"aa"
"aab"
```

Check each substring:

| Substring | Palindrome? |
|---|---|
| `"a"` | Yes |
| `"aa"` | Yes |
| `"aab"` | No |

We explore only valid palindrome substrings.

For example:

**First choice:**

```text
current = ["a"]
```

Now process the remaining string `"ab"`.

Choose `"a"`:

```text
current = ["a", "a"]
```

Then choose `"b"`:

```text
current = ["a", "a", "b"]
```

The entire string has been partitioned.

Save this answer.

**Second choice:**

Backtrack and select `"aa"`:

```text
current = ["aa"]
```

Then select `"b"`:

```text
current = ["aa", "b"]
```

Save this answer too.

**The central idea:**

Choose a palindrome → Explore the remaining string → Undo the choice → Try another palindrome.

---

## 3. Complete C++ Solution

```cpp
class Solution {
private:

    // Check whether a substring is a palindrome.
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

    void backtrack(
        int start,
        string& s,
        vector<string>& current,
        vector<vector<string>>& result
    ) {

        // Entire string has been partitioned.
        if (start == s.size()) {
            result.push_back(current);
            return;
        }

        // Try every possible ending position.
        for (int end = start; end < s.size(); end++) {

            // Consider only palindrome substrings.
            if (isPalindrome(s, start, end)) {

                // Choose the palindrome.
                current.push_back(s.substr(start, end - start + 1));

                // Explore the remaining string.
                backtrack(end + 1, s, current, result);

                // Backtrack: remove the selected substring.
                current.pop_back();
            }
        }
    }

public:

    vector<vector<string>> partition(string s) {

        vector<vector<string>> result;
        vector<string> current;

        backtrack(0, s, current, result);

        return result;
    }
};
```

---

## 4. Understanding the Parameters

Our helper function:

```cpp
void backtrack(
    int start,
    string& s,
    vector<string>& current,
    vector<vector<string>>& result
)
```

Each parameter has a specific responsibility:

| Parameter | Meaning |
|---|---|
| `start` | Starting index of the remaining string |
| `s` | Original input string |
| `current` | Palindrome partition currently being constructed |
| `result` | Stores all valid partitions |

We start with:

```cpp
backtrack(0, s, current, result);
```

This means we begin partitioning from index `0`.

For example:

```text
s = "aab"

Index:  0  1  2
       [a][a][b]
```

Initially:

```text
start = 0
current = []
```

---

## 5. Understanding the isPalindrome() Function

This function checks whether a substring is a palindrome.

```cpp
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
```

We use the **Two Pointers** technique.

- `left` starts at the beginning of the substring.
- `right` starts at the end.
- Compare both characters.
- Move `left` forward.
- Move `right` backward.

If any two characters do not match, the substring is not a palindrome.

### Example: "aba"

```text
s = "aba"

       a  b  a
       ↑     ↑
      left  right
```

First comparison:

```text
s[left] == s[right]

'a' == 'a'
```

Move both pointers:

```text
       a  b  a
          ↑
       left/right
```

The pointers meet.

All comparisons matched, so return:

```cpp
true;
```

### Example: "abc"

```text
       a  b  c
       ↑     ↑
      left  right
```

Comparison:

```text
'a' != 'c'
```

Therefore, return:

```cpp
false;
```

---

## 6. Understanding the for Loop

This is the main part of the backtracking logic.

```cpp
for (int end = start; end < s.size(); end++)
```

Why do we use this loop?

Because we want to try every possible substring starting from `start`.

For:

```cpp
s = "aab";
```

Initially:

```text
start = 0
```

The loop tries:

| start | end | Substring |
|---|---|---|
| 0 | 0 | `"a"` |
| 0 | 1 | `"aa"` |
| 0 | 2 | `"aab"` |

Each substring is checked using:

```cpp
isPalindrome(s, start, end)
```

If it is a palindrome, we select it.

---

## 7. Understanding the Most Important Recursive Call

```cpp
backtrack(end + 1, s, current, result);
```

Why `end + 1`?

Because we have already selected the substring from `start` to `end`.

The next recursive call must process the remaining string.

Example:

```text
s = "aab"
```

Suppose we select:

```text
"aa"
```

Its indices are:

```text
0  1
a  a
```

The next unprocessed character is at index `2`.

Therefore:

```cpp
backtrack(2, s, current, result);
```

This ensures that:

- No character is skipped.
- No character is used twice.
- The original order is maintained.

**Remember:**

```cpp
backtrack(end + 1, ...);
```

We move forward because the selected substring has already been processed.

---

## 8. Understanding the Base Case

```cpp
if (start == s.size()) {
    result.push_back(current);
    return;
}
```

When `start` reaches the length of the string, the entire string has been partitioned.

Example:

```text
s = "aab"
```

Suppose:

```text
current = ["aa", "b"]
start = 3
```

Since:

```cpp
s.size() == 3
```

the complete string has been processed.

Save:

```cpp
result.push_back(current);
```

**Important:** We save the partition only when the entire string has been processed.

Unlike Subsets, we do not save every intermediate state because a partial partition does not necessarily cover the complete string.

---

## 9. Understanding Backtracking

Look at these operations:

```cpp
current.push_back(s.substr(start, end - start + 1));

backtrack(end + 1, s, current, result);

current.pop_back();
```

Their responsibilities:

| Operation | Purpose |
|---|---|
| `substr()` | Extract the selected substring |
| `push_back()` | Add the palindrome to the current partition |
| `backtrack()` | Process the remaining string |
| `pop_back()` | Remove the selected substring |

Think of it as:

**Choose → Explore → Undo → Try another partition.**

The `pop_back()` operation restores the previous partition before trying another substring.

---

## 10. Complete Dry Run

**Input:**

```cpp
s = "aab";
```

Initially:

```text
start = 0
current = []
result = []
```

### Step 1: Select "a"

The first possible substring is:

```text
s[0...0] = "a"
```

It is a palindrome.

Select it:

```text
current = ["a"]
```

Recurse from:

```text
start = 1
```

### Step 2: Select another "a"

At index `1`, we try:

```text
s[1...1] = "a"
```

It is a palindrome.

Select it:

```text
current = ["a", "a"]
```

Recurse from:

```text
start = 2
```

### Step 3: Select "b"

At index `2`:

```text
s[2...2] = "b"
```

It is a palindrome.

Select it:

```text
current = ["a", "a", "b"]
```

Now:

```text
start = 3
s.size() = 3
```

The base case is reached.

Save:

```text
result = [
    ["a", "a", "b"]
]
```

### Step 4: Backtrack

Remove `"b"`:

```text
current = ["a", "a"]
```

No more substrings are available from index `2`.

Backtrack again and remove the second `"a"`:

```text
current = ["a"]
```

### Step 5: Try "ab"

From index `1`, the next possible substring is:

```text
"ab"
```

But:

```text
'a' != 'b'
```

It is not a palindrome.

Skip this choice.

### Step 6: Backtrack to the root

Remove the first `"a"`:

```text
current = []
```

Now try another substring starting at index `0`.

### Step 7: Select "aa"

The substring is:

```text
s[0...1] = "aa"
```

It is a palindrome.

Select it:

```text
current = ["aa"]
```

Recurse from index `2`.

### Step 8: Select "b"

```text
current = ["aa", "b"]
```

The entire string has been partitioned.

Save:

```text
result = [
    ["a", "a", "b"],
    ["aa", "b"]
]
```

The next possible substring `"aab"` is not a palindrome.

**Final output:**

```text
[
    ["a", "a", "b"],
    ["aa", "b"]
]
```

---

## 11. Visualizing the Recursion Tree

For:

```cpp
s = "aab";
```

A simplified decision tree:

```text
                         []
                    /         \
                  "a"          "aa"
                   |             |
                ["a"]         ["aa"]
               /     \           |
             "a"     "ab"       "b"
              |       X           |
          ["a","a"]             ["aa","b"]
              |
             "b"
              |
        ["a","a","b"]
```

Here:

- `"a"` is a valid palindrome.
- `"aa"` is a valid palindrome.
- `"ab"` is not a palindrome.
- `"aab"` is not a palindrome.
- `"b"` is a valid palindrome.

Only valid palindrome choices are explored.

---

## 12. Complexity Analysis

Let `n` be the length of the string.

**Time Complexity**

There are up to:

\[
2^{n-1}
\]

possible ways to place partition boundaries between `n` characters.

For each possible partition path, palindrome checking and substring construction can take up to O(n) work per choice.

A conservative upper bound for this implementation is:

\[
\boxed{O(n^2 \times 2^n)}
\]

The number of actual partitions depends on the input string.

**Space Complexity**

- Recursion depth: O(n).
- Current partition: O(n) in total character content.
- Temporary substring creation: up to O(n).

Auxiliary space excluding the output:

\[
\boxed{O(n)}
\]

The output can be exponential because many valid partitions may exist.

---

## 13. Common Interview Mistakes

1. Forgetting to check whether a substring is a palindrome.
2. Using `start + 1` instead of `end + 1` in the recursive call.
3. Forgetting `pop_back()`: This contaminates other partitions.
4. Saving the result before the entire string is processed.
5. Using the wrong substring length: Use `end - start + 1`.
6. Skipping invalid substrings incorrectly: Continue trying longer substrings from the same starting position.
7. Forgetting that every character must be included.
8. Confusing palindrome checking with partition generation: One checks a substring; the other explores possible partitions.

---

## 14. Subsets vs Combination Sum vs Palindrome Partitioning

These problems all use backtracking, but their selection rules differ.

| Feature | Subsets | Combination Sum | Palindrome Partitioning |
|---|---|---|---|
| Main goal | Generate subsets | Reach a target sum | Partition a string |
| Choices | Include or skip | Include or skip candidate | Choose a substring |
| Validity condition | Every subset is valid | Remaining target reaches zero | Selected substring is palindrome |
| Recursive call | `index + 1` | `index` or `index + 1` | `end + 1` |
| Base case | All elements processed | Target equals zero | Entire string processed |
| Backtracking | `pop_back()` | `pop_back()` | `pop_back()` |

The important difference is that Palindrome Partitioning must validate every selected substring before exploring it.

---

## 15. Pattern Recognition for Interviews

Think about this pattern when a problem asks you to:

- Split a string into smaller parts.
- Generate all possible partitions.
- Ensure every selected substring satisfies a condition.
- Explore different ways to divide a string.

A useful question:

**Can I try every possible ending position for the current substring and validate it before making a recursive call?**

If yes, loop-based backtracking is a strong approach.

For Palindrome Partitioning:

1. Start from an index.
2. Try every possible ending position.
3. Check whether the substring is a palindrome.
4. Select valid substrings.
5. Recursively process the remaining string.

---

## 16. 30-Second Interview Revision

**Q1. Why use backtracking?**

Because we need to explore all possible ways to partition the string.

**Q2. How do we check whether a substring is a palindrome?**

Use two pointers: one from the beginning and one from the end.

**Q3. Why use `end + 1` in the recursive call?**

Because the substring from `start` to `end` has already been selected.

**Q4. When do we save a partition?**

When `start == s.size()`.

**Q5. Why use `pop_back()`?**

To remove the selected substring before exploring another partition.

**Q6. What is the time complexity?**

A conservative upper bound for this implementation is:

\[
O(n^2 \times 2^n)
\]

**Final takeaway**

```text
PALINDROME PARTITIONING = BACKTRACKING + TWO POINTERS

1. Start from the current index.
2. Try every possible ending position.
3. Check whether the substring is a palindrome.
4. If valid, add it to current.
5. Recurse from end + 1.
6. Backtrack using pop_back().
7. When start == s.size():
       Save the partition.
```
# 71. Letter Combinations of a Phone Number

## 1. Problem Understanding

You are given a string `digits` containing digits from `2` to `9`.

Return all possible letter combinations that the number could represent using the letters on a traditional phone keypad.

**Important rules:**

1. Each digit represents multiple letters.
2. Select exactly one letter for each digit.
3. Maintain the original order of the digits.
4. Return all possible combinations.
5. If the input string is empty, return an empty vector.

### Phone Keypad Mapping

```text
  2       3       4
 abc     def     ghi

  5       6       7
 jkl     mno     pqrs

  8       9
 tuv     wxyz
```

**Example 1**

```cpp
digits = "23";
```

**Output:**

```text
[
    "ad", "ae", "af",
    "bd", "be", "bf",
    "cd", "ce", "cf"
]
```

Explanation:

- Digit `2` represents `a`, `b`, `c`.
- Digit `3` represents `d`, `e`, `f`.

We select one letter from each digit.

**Example 2**

```cpp
digits = "";
```

**Output:**

```text
[]
```

**Example 3**

```cpp
digits = "2";
```

**Output:**

```text
["a", "b", "c"]
```

---

## 2. Core Intuition

We use **Backtracking**.

For every digit, we have multiple possible letters.

Our task is to select one letter from each digit and generate every possible combination.

Consider:

```cpp
digits = "23";
```

Mapping:

```text
2 → abc
3 → def
```

First, choose a letter from digit `2`.

Suppose we choose:

```text
a
```

Now move to digit `3`.

We can choose:

```text
d → "ad"
e → "ae"
f → "af"
```

After exploring all possibilities starting with `a`, backtrack and try `b`.

```text
bd
be
bf
```

Then try `c`.

```text
cd
ce
cf
```

Final combinations:

```text
ad, ae, af
bd, be, bf
cd, ce, cf
```

**The central idea:**

1. Take the current digit.
2. Find its corresponding letters.
3. Choose one letter.
4. Recursively process the next digit.
5. Remove the selected letter.
6. Try another letter.

---

## 3. Complete C++ Solution

```cpp
class Solution {
private:

    void backtrack(
        int index,
        string& digits,
        vector<string>& mapping,
        string& current,
        vector<string>& result
    ) {

        // All digits have been processed.
        if (index == digits.size()) {
            result.push_back(current);
            return;
        }

        // Get letters corresponding to the current digit.
        string letters = mapping[digits[index] - '0'];

        // Try every possible letter.
        for (char ch : letters) {

            // Choose a letter.
            current.push_back(ch);

            // Process the next digit.
            backtrack(
                index + 1,
                digits,
                mapping,
                current,
                result
            );

            // Backtrack.
            current.pop_back();
        }
    }

public:

    vector<string> letterCombinations(string digits) {

        vector<string> result;

        // Handle empty input.
        if (digits.empty()) {
            return result;
        }

        // Phone keypad mapping.
        vector<string> mapping = {
            "",     // 0
            "",     // 1
            "abc",  // 2
            "def",  // 3
            "ghi",  // 4
            "jkl",  // 5
            "mno",  // 6
            "pqrs", // 7
            "tuv",  // 8
            "wxyz"  // 9
        };

        string current;

        backtrack(0, digits, mapping, current, result);

        return result;
    }
};
```

---

## 4. Understanding the Parameters

Our helper function:

```cpp
void backtrack(
    int index,
    string& digits,
    vector<string>& mapping,
    string& current,
    vector<string>& result
)
```

Each parameter has a specific responsibility:

| Parameter | Meaning |
|---|---|
| `index` | Current digit position |
| `digits` | Original input digits |
| `mapping` | Phone keypad digit-to-letter mapping |
| `current` | Combination currently being constructed |
| `result` | Stores all completed combinations |

We start with:

```cpp
backtrack(0, digits, mapping, current, result);
```

This means we start processing digits from index `0`.

For example:

```text
digits = "23"

Index:    0   1
Digit:    2   3
Letters: abc def
```

---

## 5. Understanding the Phone Keypad Mapping

We create:

```cpp
vector<string> mapping = {
    "",
    "",
    "abc",
    "def",
    "ghi",
    "jkl",
    "mno",
    "pqrs",
    "tuv",
    "wxyz"
};
```

Each index represents a digit.

For example:

```cpp
mapping[2] = "abc";
mapping[3] = "def";
mapping[7] = "pqrs";
```

### Why use `digits[index] - '0'`?

Look at:

```cpp
string letters = mapping[digits[index] - '0'];
```

The input is a string, so each digit is stored as a character.

For example:

```cpp
digits[index] = '2';
```

But vector indexing requires an integer.

Therefore:

```cpp
'2' - '0' = 2
```

Now we can access:

```cpp
mapping[2]
```

Which gives:

```text
abc
```

This is a common C++ technique for converting a numeric character into an integer.

---

## 6. Understanding the for Loop

```cpp
for (char ch : letters)
```

This loop explores every possible letter for the current digit.

Suppose:

```cpp
letters = "abc";
```

The loop executes three times:

```text
ch = 'a'
ch = 'b'
ch = 'c'
```

For each letter:

```cpp
current.push_back(ch);
```

We add the selected letter to our current combination.

Then:

```cpp
backtrack(index + 1, digits, mapping, current, result);
```

We move to the next digit.

After recursion finishes:

```cpp
current.pop_back();
```

We remove the selected letter and try the next possibility.

---

## 7. Understanding the Recursive Call

```cpp
backtrack(index + 1, digits, mapping, current, result);
```

Why do we use `index + 1`?

Because we must select exactly one letter from each digit.

After choosing a letter for the current digit, we move to the next digit.

Example:

```text
digits = "23"
```

Initially:

```text
index = 0
```

Choose `a` from digit `2`.

Now:

```text
current = "a"
index = 1
```

Next, choose a letter from digit `3`.

If we choose `d`:

```text
current = "ad"
index = 2
```

Now every digit has been processed.

Save `"ad"`.

---

## 8. Understanding the Base Case

```cpp
if (index == digits.size()) {
    result.push_back(current);
    return;
}
```

When `index` reaches the length of the input, every digit has been processed.

The current combination is complete.

For example:

```text
digits = "23"

current = "ad"
index = 2
digits.size() = 2
```

Since:

```cpp
index == digits.size()
```

we save:

```cpp
result.push_back(current);
```

This produces one complete letter combination.

---

## 9. Complete Dry Run

**Input:**

```cpp
digits = "23";
```

Mapping:

```text
2 → abc
3 → def
```

Initially:

```text
index = 0
current = ""
result = []
```

### Step 1: Select 'a'

At index `0`:

```text
letters = "abc"
```

Select:

```text
current = "a"
```

Move to index `1`.

### Step 2: Select 'd'

At index `1`:

```text
letters = "def"
```

Select:

```text
current = "ad"
```

Now:

```text
index = 2
```

All digits have been processed.

Save:

```text
result = ["ad"]
```

### Step 3: Backtrack and select 'e'

Remove `d`:

```text
current = "a"
```

Select `e`:

```text
current = "ae"
```

Save:

```text
result = ["ad", "ae"]
```

### Step 4: Backtrack and select 'f'

Remove `e` and select `f`:

```text
current = "af"
```

Save:

```text
result = ["ad", "ae", "af"]
```

### Step 5: Backtrack and select 'b'

After exploring all letters starting with `a`, remove `a`.

Now choose `b`:

```text
current = "b"
```

Explore digit `3`:

```text
"bd"
"be"
"bf"
```

Result:

```text
[
    "ad", "ae", "af",
    "bd", "be", "bf"
]
```

### Step 6: Backtrack and select 'c'

Now choose `c`:

```text
current = "c"
```

Explore digit `3`:

```text
"cd"
"ce"
"cf"
```

**Final output:**

```text
[
    "ad", "ae", "af",
    "bd", "be", "bf",
    "cd", "ce", "cf"
]
```

Total combinations:

```text
3 × 3 = 9
```

---

## 10. Visualizing the Recursion Tree

For:

```cpp
digits = "23";
```

A simplified decision tree:

```text
                         ""
                    /     |     \
                   a      b      c
                 / | \   / | \  / | \
                d  e  f d  e  f d  e  f
                |  |  | |  |  | |  |  |
               ad ae af bd be bf cd ce cf
```

Every level represents one digit.

- Level 0: Choose a letter for digit `2`.
- Level 1: Choose a letter for digit `3`.
- Leaf nodes: Completed combinations.

Each path from the root to a leaf produces one answer.

---

## 11. Why Do We Need Backtracking?

Consider:

```cpp
current.push_back(ch);

backtrack(index + 1, digits, mapping, current, result);

current.pop_back();
```

Suppose we choose:

```text
a
```

We explore:

```text
ad
ae
af
```

After completing these branches, we must remove `a` before trying `b`.

Otherwise, the next combinations would incorrectly contain previous selections.

Backtracking restores the previous state.

**Choose → Explore → Undo → Choose another letter.**

---

## 12. Complexity Analysis

Let:

- `n` = number of digits.
- `k` = maximum number of letters mapped to one digit.

For digits `2` to `9`, the maximum number of letters is `4`.

### Time Complexity

In the worst case, every digit maps to four letters.

Therefore, the number of combinations is:

\[
4^n
\]

Each combination contains `n` characters.

Copying each combination into the result takes O(n).

Therefore:

\[
\boxed{O(n \times 4^n)}
\]

### Space Complexity

- Recursion depth: O(n).
- Current combination: O(n).
- Mapping: O(1), because the keypad mapping has a fixed size.
- Output: O(n × 4ⁿ) in the worst case.

Auxiliary space excluding output:

\[
\boxed{O(n)}
\]

---

## 13. Common Interview Mistakes

1. Forgetting to handle empty input: Return an empty vector when `digits.empty()`.
2. Incorrect keypad mapping: Make sure digits `7` and `9` have four letters.
3. Forgetting `index + 1`: Each recursive call must process the next digit.
4. Forgetting `pop_back()`: This contaminates other combinations.
5. Using the digit character directly as an integer index: Convert using `digits[index] - '0'`.
6. Saving incomplete combinations: Save only when all digits have been processed.
7. Choosing multiple letters from the same digit: Exactly one letter must be selected for each digit.

---

## 14. Subsets vs Generate Parentheses vs Letter Combinations

These problems use the same backtracking foundation but have different choices.

| Feature | Subsets | Generate Parentheses | Letter Combinations |
|---|---|---|---|
| Main goal | Generate subsets | Generate valid parentheses | Generate letter combinations |
| Choices | Include or skip | Add `(` or `)` | Choose mapped letter |
| Tracking | Index | Open and close counts | Digit index |
| Validity condition | Every subset is valid | `close < open` | One letter per digit |
| Base case | All elements processed | String length = `2n` | All digits processed |
| Backtracking | `pop_back()` | `pop_back()` | `pop_back()` |

The main difference is the number of choices available at each step.

---

## 15. Pattern Recognition for Interviews

Think about this pattern when a problem asks you to:

- Generate all possible combinations from multiple groups.
- Select exactly one element from each group.
- Generate combinations using a mapping.
- Explore multiple choices at every position.

A useful question:

**Does each position have multiple possible choices, and must I select exactly one from each position?**

If yes, DFS + Backtracking is a strong approach.

For Letter Combinations:

1. Map each digit to its letters.
2. Start from the first digit.
3. Try every mapped letter.
4. Move to the next digit.
5. Save when all digits are processed.

---

## 16. 30-Second Interview Revision

**Q1. Why use backtracking?**

Because we need to generate all possible letter combinations by exploring every mapped letter.

**Q2. Why use a mapping vector?**

To store the letters corresponding to each digit.

**Q3. Why use `digits[index] - '0'`?**

To convert a numeric character into an integer index.

**Q4. Why use `index + 1`?**

Because we select exactly one letter from the current digit before processing the next digit.

**Q5. Why use `pop_back()`?**

To undo the selected letter before exploring another possibility.

**Q6. What is the base case?**

When `index == digits.size()`, save the complete combination.

**Q7. What is the time complexity?**

O(n × 4ⁿ) in the worst case.

**Final takeaway**

```text
LETTER COMBINATIONS = MAPPING + BACKTRACKING

1. Create the phone keypad mapping.
2. Start from digit index 0.
3. Get letters for the current digit.
4. Loop through every available letter.
5. Add the selected letter.
6. Recurse using index + 1.
7. Backtrack using pop_back().
8. When all digits are processed:
       Save the combination.
```
# 72. N-Queens

## 1. Problem Understanding

You are given an integer `n`.

Place `n` queens on an `n × n` chessboard such that no two queens can attack each other.

Return all distinct solutions.

**Important rules:**

1. A queen can attack horizontally.
2. A queen can attack vertically.
3. A queen can attack diagonally.
4. No two queens can attack each other.
5. Each solution must contain exactly `n` queens.

The board uses:

- `Q` to represent a queen.
- `.` to represent an empty cell.

**Example 1**

```cpp
n = 4;
```

**Output:**

```text
[
    [".Q..", "...Q", "Q...", "..Q."],
    ["..Q.", "Q...", "...Q", ".Q.."]
]
```

There are two valid solutions for the 4-Queens problem.

**Example 2**

```cpp
n = 1;
```

**Output:**

```text
[
    ["Q"]
]
```

---

## 2. Core Intuition

We use **Backtracking**.

The idea is to place queens row by row.

For each row:

1. Try placing a queen in every column.
2. Check whether the position is safe.
3. If safe, place the queen.
4. Move to the next row.
5. If the next row cannot produce a solution, remove the queen and try another column.

Why place one queen per row?

Because two queens in the same row would attack each other.

Therefore, we can simplify the problem:

- Place exactly one queen in each row.
- Make sure no two queens share a column.
- Make sure no two queens share a diagonal.

**The central idea:**

Choose a safe position → Explore the next row → Undo the choice → Try another position.

---

## 3. Complete C++ Solution

```cpp
class Solution {
private:

    bool isSafe(
        int row,
        int col,
        vector<string>& board,
        int n
    ) {

        // Check the same column in previous rows.
        for (int i = 0; i < row; i++) {

            if (board[i][col] == 'Q') {
                return false;
            }
        }

        // Check upper-left diagonal.
        int i = row - 1;
        int j = col - 1;

        while (i >= 0 && j >= 0) {

            if (board[i][j] == 'Q') {
                return false;
            }

            i--;
            j--;
        }

        // Check upper-right diagonal.
        i = row - 1;
        j = col + 1;

        while (i >= 0 && j < n) {

            if (board[i][j] == 'Q') {
                return false;
            }

            i--;
            j++;
        }

        return true;
    }

    void backtrack(
        int row,
        vector<string>& board,
        vector<vector<string>>& result,
        int n
    ) {

        // All n queens have been placed.
        if (row == n) {
            result.push_back(board);
            return;
        }

        // Try every column in the current row.
        for (int col = 0; col < n; col++) {

            if (isSafe(row, col, board, n)) {

                // Choose: Place the queen.
                board[row][col] = 'Q';

                // Explore the next row.
                backtrack(row + 1, board, result, n);

                // Backtrack: Remove the queen.
                board[row][col] = '.';
            }
        }
    }

public:

    vector<vector<string>> solveNQueens(int n) {

        vector<vector<string>> result;

        // Initially, every cell is empty.
        vector<string> board(n, string(n, '.'));

        backtrack(0, board, result, n);

        return result;
    }
};
```

---

## 4. Understanding the Board Representation

We use:

```cpp
vector<string> board(n, string(n, '.'));
```

This creates an `n × n` chessboard filled with dots.

For `n = 4`:

```text
....
....
....
....
```

Each string represents one row.

For example:

```cpp
board[0][1] = 'Q';
```

Places a queen in row `0`, column `1`.

The board becomes:

```text
.Q..
....
....
....
```

We modify the board directly during backtracking.

---

## 5. Understanding the Parameters

Our helper function:

```cpp
void backtrack(
    int row,
    vector<string>& board,
    vector<vector<string>>& result,
    int n
)
```

Each parameter has a specific responsibility:

| Parameter | Meaning |
|---|---|
| `row` | Current row where we need to place a queen |
| `board` | Current chessboard configuration |
| `result` | Stores all valid board arrangements |
| `n` | Size of the chessboard |

We start with:

```cpp
backtrack(0, board, result, n);
```

This means we start placing queens from row `0`.

---

## 6. Understanding the isSafe() Function

This is one of the most important parts of N-Queens.

Before placing a queen, we must check whether another queen can attack that position.

Because we place queens row by row from top to bottom, we only need to check previous rows.

There are three checks.

### A. Check the Same Column

```cpp
for (int i = 0; i < row; i++) {

    if (board[i][col] == 'Q') {
        return false;
    }
}
```

Why?

Because two queens cannot share the same column.

Example:

```text
.Q..
.Q..
....
....
```

The second queen cannot be placed in column `1`.

The first queen already occupies that column.

### B. Check the Upper-Left Diagonal

```cpp
int i = row - 1;
int j = col - 1;

while (i >= 0 && j >= 0) {

    if (board[i][j] == 'Q') {
        return false;
    }

    i--;
    j--;
}
```

We move:

- One row upward.
- One column to the left.

Example:

```text
Q...
....
..X.
....
```

If we want to place a queen at `X`, we must check the upper-left diagonal.

If a queen exists there, the position is unsafe.

### C. Check the Upper-Right Diagonal

```cpp
i = row - 1;
j = col + 1;

while (i >= 0 && j < n) {

    if (board[i][j] == 'Q') {
        return false;
    }

    i--;
    j++;
}
```

We move:

- One row upward.
- One column to the right.

If a queen exists on this diagonal, the position is unsafe.

### Why Don't We Check Lower Rows?

Because we place queens from top to bottom.

When placing a queen in the current row, all lower rows are still empty.

Therefore, checking previous rows is sufficient.

---

## 7. Understanding the Recursive Call

```cpp
backtrack(row + 1, board, result, n);
```

Why `row + 1`?

Because we place exactly one queen in each row.

After successfully placing a queen in the current row, we move to the next row.

Example:

```text
Row 0 → Place first queen.
Row 1 → Place second queen.
Row 2 → Place third queen.
Row 3 → Place fourth queen.
```

Once all rows have a queen, we have a complete solution.

---

## 8. Understanding the Base Case

```cpp
if (row == n) {
    result.push_back(board);
    return;
}
```

Why?

Because the valid row indices are:

```text
0, 1, 2, ..., n - 1
```

When `row == n`, all rows have been processed.

Since we place one queen in every row, we have successfully placed all `n` queens.

Save the current board:

```cpp
result.push_back(board);
```

**Important:** We save a copy of the board because other recursive branches will modify the original board.

---

## 9. Understanding Backtracking

Look at these operations:

```cpp
board[row][col] = 'Q';

backtrack(row + 1, board, result, n);

board[row][col] = '.';
```

Their responsibilities:

| Operation | Purpose |
|---|---|
| `board[row][col] = 'Q'` | Place a queen |
| `backtrack(row + 1, ...)` | Explore the next row |
| `board[row][col] = '.'` | Remove the queen |

Why remove the queen?

Suppose we place a queen in column `0`.

We explore all possibilities below that position.

If no valid complete solution is possible, we must remove that queen and try column `1`.

This restores the previous board state.

**Choose → Explore → Undo → Try another position.**

---

## 10. Complete Dry Run

**Input:**

```cpp
n = 4;
```

Initially:

```text
....
....
....
....
```

We start from row `0`.

### Step 1: Place the first queen

Try column `0`.

Position `(0, 0)` is safe.

```text
Q...
....
....
....
```

Move to row `1`.

### Step 2: Try placing the second queen

Check column `0`.

It is already occupied.

Try column `1`.

It is on the upper-left diagonal of the first queen.

Unsafe.

Try column `2`.

It is safe.

Place the queen:

```text
Q...
..Q.
....
....
```

Move to row `2`.

### Step 3: Try placing the third queen

Check all columns.

- Column `0`: Occupied.
- Column `1`: Diagonal conflict.
- Column `2`: Occupied.
- Column `3`: Diagonal conflict.

No safe position exists.

Therefore, backtrack.

Remove the queen from `(1, 2)`:

```text
Q...
....
....
....
```

Try another position in row `1`.

Column `3` is safe.

```text
Q...
...Q
....
....
```

Move to row `2`.

Again, explore all possible columns.

If no valid placement exists, backtrack to row `0` and remove the first queen.

### Step 4: Try column `1` in row `0`

Place the queen:

```text
.Q..
....
....
....
```

Continue exploring the next rows.

One valid arrangement is:

```text
.Q..
...Q
Q...
..Q.
```

All four queens are safe.

Save this solution.

### Step 5: Explore remaining branches

Continue backtracking and trying other safe positions.

The second valid arrangement is:

```text
..Q.
Q...
...Q
.Q..
```

Save this solution too.

**Final output:**

```text
[
    [".Q..", "...Q", "Q...", "..Q."],
    ["..Q.", "Q...", "...Q", ".Q.."]
]
```

There are exactly two solutions for `n = 4`.

---

## 11. Visualizing the Two Valid Solutions

### Solution 1

```text
. Q . .
. . . Q
Q . . .
. . Q .
```

### Solution 2

```text
. . Q .
Q . . .
. . . Q
. Q . .
```

In both arrangements:

- Every row contains one queen.
- Every column contains one queen.
- No two queens share a diagonal.

---

## 12. Complexity Analysis

Let `n` be the size of the chessboard.

### Time Complexity

We place one queen per row.

For each row, we may try up to `n` columns.

The number of possible arrangements is bounded by the permutations of columns, giving an O(n!) search bound.

In the given implementation, each safety check takes O(n) time.

Therefore, a conservative upper bound is:

\[
\boxed{O(n \times n!)}
\]

### Space Complexity

- Chessboard: O(n²).
- Recursion depth: O(n).
- Temporary variables: O(1).

Auxiliary space excluding the output:

\[
\boxed{O(n^2)}
\]

The output requires additional space for all valid board arrangements.

---

## 13. Common Interview Mistakes

1. Checking only the column: Diagonal attacks must also be checked.
2. Forgetting upper-left diagonal checking.
3. Forgetting upper-right diagonal checking.
4. Checking lower rows unnecessarily: They are empty because we move top to bottom.
5. Forgetting to restore the board: Other recursive branches will be affected.
6. Using `row + 1` incorrectly: We must move to the next row after placing a queen.
7. Saving the result before all queens are placed.
8. Forgetting that every row must contain exactly one queen.

---

## 14. Subsets vs Generate Parentheses vs N-Queens

These problems use backtracking but have different constraints.

| Feature | Subsets | Generate Parentheses | N-Queens |
|---|---|---|---|
| Main goal | Generate subsets | Generate valid parentheses | Place non-attacking queens |
| Choices | Include or skip | Add `(` or `)` | Choose a column |
| Tracking | Index | Open and close counts | Current row and board |
| Validity condition | Every subset is valid | `close < open` | Column and diagonal checks |
| Base case | All elements processed | String length = `2n` | `row == n` |
| Backtracking | `pop_back()` | `pop_back()` | Restore `.` |

The main difference is that N-Queens must validate whether a position is safe before exploring it.

---

## 15. Pattern Recognition for Interviews

Think about this pattern when a problem asks you to:

- Place objects on a board.
- Follow restrictions between selected positions.
- Generate all valid board arrangements.
- Explore multiple positions and undo invalid choices.

A useful question:

**Can I build the solution one row at a time and validate each choice before moving forward?**

If yes, backtracking is a strong approach.

For N-Queens:

1. Start from row `0`.
2. Try every column.
3. Check whether the position is safe.
4. Place the queen.
5. Move to the next row.
6. Backtrack if the branch fails.
7. Save when all rows are completed.

---

## 16. 30-Second Interview Revision

**Q1. Why use backtracking?**

Because we need to explore different queen placements and remove placements that cannot lead to a valid solution.

**Q2. Why place one queen per row?**

Because two queens in the same row would attack each other.

**Q3. What three conditions must we check?**

- Same column.
- Upper-left diagonal.
- Upper-right diagonal.

**Q4. Why don't we check lower rows?**

They are empty because we place queens from top to bottom.

**Q5. Why restore the board using `.`?**

To undo the previous placement before exploring another column.

**Q6. What is the base case?**

When `row == n`, all queens have been placed.

**Q7. What is the time complexity?**

A conservative upper bound for this implementation is O(n × n!).

**Final takeaway**

```text
N-QUEENS = BACKTRACKING + SAFETY CHECK

1. Start from row 0.
2. Try every column.
3. Check:
       Same column
       Upper-left diagonal
       Upper-right diagonal
4. If safe:
       Place 'Q'.
       Recurse using row + 1.
       Restore '.'.
5. When row == n:
       Save the board.
```
