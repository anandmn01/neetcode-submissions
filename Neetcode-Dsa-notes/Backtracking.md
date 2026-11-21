63. Subsets

1. Problem Understanding

You are given an integer array "nums" containing unique elements.

Return all possible subsets (the power set).

A subset can contain:

- No elements (empty subset).
- One element.
- Multiple elements.
- All elements.

Example

nums = {1, 2, 3};

Output:

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

For an array of size "n", the total number of subsets is:

[
2^n
]

For example, 3 elements produce (2³ = 8) subsets.

---

2. Core Intuition

For every element, we have two choices:

1. Include the element.
2. Do not include the element.

Let's take:

nums = [1, 2, 3]

For element "1":

- Include "1".
- Skip "1".

For element "2", again:

- Include "2".
- Skip "2".

The same applies to element "3".

This creates a decision tree.

Decision Tree

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

Every leaf represents one possible subset.

This is the foundation of the backtracking approach.

---

3. Complete C++ Solution

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

---

4. Understanding the Backtracking Function

Our helper function:

void backtrack(
    int index,
    vector<int>& nums,
    vector<int>& current,
    vector<vector<int>>& result
)

Each parameter has a specific responsibility:

Parameter| Meaning
"index"| Current element we are considering
"nums"| Original input array
"current"| Subset currently being constructed
"result"| Stores all completed subsets

We start with:

backtrack(0, nums, current, result);

This means we begin considering elements from index "0".

---

5. Understanding the Base Case

if (index == nums.size()) {
    result.push_back(current);
    return;
}

When "index" reaches the array size, we have considered every element.

The current subset is now complete.

We copy it into the result:

result.push_back(current);

Important: We use "push_back(current)" to save a copy of the current subset, not a reference to the temporary vector.

---

6. Understanding the Two Choices

This is the most important part of the problem.

Choice 1: Include the current element

current.push_back(nums[index]);

backtrack(index + 1, nums, current, result);

Suppose:

current = []
nums[index] = 1

After including:

current = [1]

Now recursively process the next element.

Backtracking: Remove the element

current.pop_back();

Why?

Because after exploring the branch that includes "1", we must restore the previous state before exploring the branch that skips "1".

Before:

current = [1]

After:

current = []

Choice 2: Skip the current element

backtrack(index + 1, nums, current, result);

We simply move to the next element without including the current one.

This gives us both possibilities.

---

7. Complete Dry Run

Input:

nums = [1, 2]

Initial state:

current = []
result = []

Step 1: Consider element 1

Include "1":

current = [1]

Now consider element "2".

Step 2: Include element 2

current = [1, 2]

All elements processed.

Save:

result = [[1, 2]]

Step 3: Backtrack and skip element 2

Remove "2":

current = [1]

Save this subset:

result = [[1, 2], [1]]

Step 4: Backtrack and skip element 1

Remove "1":

current = []

Now include element "2":

current = [2]

Save:

result = [[1, 2], [1], [2]]

Finally, skip element "2":

current = []

Save:

result = [[1, 2], [1], [2], []]

Final answer:

[
  [1, 2],
  [1],
  [2],
  []
]

The order of subsets does not matter.

---

8. Visualizing the Complete Decision Tree

For:

nums = [1, 2]

                         []
                       /    \
                    Take 1  Skip 1
                     /        \
                   [1]         []
                  /   \       /   \
              Take 2 Skip 2 Take 2 Skip 2
                /      \      /      \
             [1,2]    [1]    [2]      []

Each element creates two choices.

Therefore:

[
2 \times 2 \times 2 = 2^n
]

For "n = 2":

[
2^2 = 4
]

We get exactly four subsets.

---

9. Why Is This Called Backtracking?

Backtracking means:

1. Make a choice.
2. Explore the consequences.
3. Undo the choice.
4. Explore another possibility.

In this problem:

current.push_back(nums[index]);  // Make choice

backtrack(...);                  // Explore

current.pop_back();              // Undo choice

backtrack(...);                  // Explore alternative

The "pop_back()" operation is what restores the current subset before exploring the other branch.

Think of it as:

Choose → Explore → Undo → Choose another option.

---

10. Complexity Analysis

Let "n" be the number of elements.

Time Complexity

There are:

[
2^n
]

possible subsets.

Copying each subset takes up to O(n) time.

Therefore:

[
\boxed{O(n \times 2^n)}
]

Space Complexity

- Recursion depth: O(n).
- Current subset: O(n).
- Output: O(n × 2ⁿ).

The auxiliary space excluding the output is O(n).

---

11. Common Interview Mistakes

1. Forgetting the empty subset: The empty subset is also valid.
2. Forgetting "pop_back()": This causes elements from one branch to remain in another branch.
3. Using the wrong base case: Stop when "index == nums.size()".
4. Not copying the current subset: Use "result.push_back(current)".
5. Moving the index incorrectly: Both choices must process "index + 1".
6. Thinking subsets must be in sorted order: The output order does not matter.

---

12. Alternative Approach: For Loop Backtracking

There is another common way to solve this problem.

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

Difference Between Both Approaches

Include/Skip approach| For-loop approach
Makes two explicit choices| Loops through available choices
Base case when all elements are processed| Saves current subset at every call
Easy to understand binary decisions| Common reusable combination pattern
Uses recursion with two branches| Uses recursion with a loop

Both approaches are correct.

For learning the decision-making concept, the first approach is especially useful.

---

13. Pattern Recognition for Interviews

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

14. 30-Second Interview Revision

Q1. What is a subset?

A selection of zero or more elements from an array, without changing their relative order.

Q2. How many subsets exist for n elements?

(2^n).

Q3. Why use backtracking?

Because each element has two choices: include it or skip it.

Q4. Why use "pop_back()"?

To undo the previous choice before exploring another branch.

Q5. What is the time complexity?

O(n × 2ⁿ), because there are 2ⁿ subsets and copying each subset can take O(n).

Q6. Is the empty subset valid?

Yes. Every set has an empty subset.

Final takeaway

SUBSETS = INCLUDE OR SKIP

For every element:
    1. Include it.
    2. Explore recursively.
    3. Remove it (backtrack).
    4. Skip it.
    5. Explore recursively.

When all elements are processed:
    Save the current subset.

64. Combination Sum

1. Problem Understanding

You are given:

- An array of distinct positive integers "nums".
- An integer "target".

Find all unique combinations where the chosen numbers add up to "target".

Important rules:

1. You can use the same number unlimited times.
2. The order of numbers in a combination does not matter.
3. Return all unique combinations.

Example

nums = {2, 3, 6, 7};
target = 7;

Output:

[
    [2, 2, 3],
    [7]
]

Explanation:

- "2 + 2 + 3 = 7"
- "7 = 7"

We cannot use combinations such as "[3, 2, 2]" separately because they contain the same numbers in a different order.

---

2. Core Intuition

We use Backtracking.

For every candidate, we have two choices:

1. Include the current number.
2. Skip the current number and move to the next candidate.

The important difference from the Subsets problem:

When we include a number, we stay at the same index because we can reuse it.

Consider:

nums = [2, 3, 6, 7]
target = 7

We start with "2".

current = [2]
remaining target = 5

We can select "2" again:

current = [2, 2]
remaining target = 3

We can select "2" again, but that would make the sum exceed the target.

Instead, we explore the next candidate:

current = [2, 2, 3]
remaining target = 0

We found a valid combination.

---

3. Complete C++ Solution

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

---

4. Understanding the Parameters

void backtrack(
    int index,
    vector<int>& nums,
    int target,
    vector<int>& current,
    vector<vector<int>>& result
)

Each parameter has a specific responsibility:

Parameter| Meaning
"index"| Current candidate we are considering
"nums"| Original array
"target"| Remaining sum we need to achieve
"current"| Combination currently being constructed
"result"| Stores all valid combinations

Notice that we use "target" as the remaining sum rather than repeatedly calculating the sum of "current".

This makes the implementation simpler.

---

5. Understanding the Base Cases

Case 1: Target becomes zero

if (target == 0) {
    result.push_back(current);
    return;
}

This means the current combination has exactly reached the required sum.

Example:

current = [2, 2, 3]
target = 0

We save the combination.

Case 2: No candidates remain

if (index == nums.size()) {
    return;
}

We have no more numbers available to explore.

Therefore, this path cannot produce another combination.

---

6. The Most Important Concept: Same Index vs Next Index

Look at these two calls:

Include the current number

backtrack(index, nums, target - nums[index], current, result);

We use the same "index".

Why?

Because the problem allows unlimited reuse of a candidate.

Example:

nums = [2, 3, 6, 7]

We can select "2" repeatedly:

[2]
[2, 2]
[2, 2, 2]

Skip the current number

backtrack(index + 1, nums, target, current, result);

We move to the next index because we have decided not to use the current candidate anymore.

Remember this distinction:

Decision| Next index
Include current number| "index"
Skip current number| "index + 1"

This is the central logic of Combination Sum.

---

7. Complete Dry Run

Input:

nums = [2, 3, 6, 7]
target = 7

Initially:

index = 0
target = 7
current = []

Step 1: Include 2

current = [2]
target = 5
index = 0

We stay at index "0".

Step 2: Include 2 again

current = [2, 2]
target = 3
index = 0

Again, we stay at index "0".

Step 3: Try including 2 again

2 <= 3

So we can include it:

current = [2, 2, 2]
target = 1

Now:

2 > 1

We cannot include "2".

We skip it and move to index "1", where the candidate is "3".

But:

3 > 1

We cannot include "3" either.

We continue skipping candidates until no candidates remain.

This path is invalid.

Backtracking removes the last selected numbers and explores other possibilities.

Step 4: Find [2, 2, 3]

After returning to:

current = [2, 2]
target = 3

We skip candidate "2" and move to candidate "3".

Include "3":

current = [2, 2, 3]
target = 0

Save:

result = [[2, 2, 3]]

Step 5: Explore the combination [7]

After exploring the branches starting with "2", we backtrack and skip "2", "3" and "6".

We reach candidate "7":

current = [7]
target = 0

Save:

result = [[2, 2, 3], [7]]

Final output:

[
    [2, 2, 3],
    [7]
]

---

8. Visual Decision Tree

For:

nums = [2, 3]
target = 7

A simplified view:

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

Every time we include a number, we can choose it again.

Every time we skip a number, we move forward.

---

9. Why Don't We Get Duplicate Combinations?

Suppose:

nums = [2, 3]
target = 7

We only explore candidates in their original order.

We can generate:

[2, 2, 3]

But we never generate:

[3, 2, 2]

Why?

Because once we move from candidate "2" to candidate "3", we never return to candidate "2" in that branch.

This prevents different orderings of the same combination from being generated.

---

10. Complexity Analysis

Let:

- "n" = number of candidates.
- "T" = target.
- "m" = smallest candidate.

The recursion can explore many combinations, so the time complexity is exponential in the worst case.

A commonly used upper-bound style estimate is:

[
O(n^{T/m})
]

This describes the branching search roughly; the exact number of recursive states depends on the candidate values.

Space complexity excluding the output:

[
O(T/m)
]

This represents the maximum recursion depth and current combination length, assuming all candidates are positive integers.

The output itself can also be large and must be counted separately.

---

11. Common Interview Mistakes

1. Using "index + 1" when including a number: This prevents unlimited reuse.
2. Using "index" when skipping a number: This can repeatedly reconsider the same candidate.
3. Forgetting "pop_back()": This contaminates other combinations.
4. Not stopping when "target == 0": This can generate unnecessary recursive calls.
5. Generating duplicate permutations: Process candidates in a fixed order.
6. Recalculating the current sum: Maintaining the remaining target is simpler.
7. Assuming the target must be reached with every candidate: It is valid to skip candidates.

---

12. Subsets vs Combination Sum

This comparison is especially important because both problems use backtracking.

Feature| Subsets| Combination Sum
Include or skip| Yes| Yes
Reuse elements| No| Yes
Include recursive call| "index + 1"| "index"
Skip recursive call| "index + 1"| "index + 1"
Main stopping condition| All elements processed| Target becomes zero
Goal| Generate every subset| Find combinations matching target

The most important difference is the recursive call after including an element.

---

13. Pattern Recognition for Interviews

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

14. 30-Second Interview Revision

Q1. Why use backtracking?

We need to explore different combinations of candidates that can reach the target.

Q2. Can we reuse the same candidate?

Yes, unlimited times.

Q3. Why do we call "backtrack(index, ...)" after including a number?

To allow that same number to be selected again.

Q4. Why do we call "backtrack(index + 1, ...)" when skipping?

Because we have decided not to use the current candidate in this branch.

Q5. How do we avoid duplicate combinations?

We process candidates in a fixed order and never move backward.

Final takeaway

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
65. Combination Sum II

1. Problem Understanding

You are given:

- An array "candidates" containing positive integers (possibly duplicates).
- An integer "target".

Find all unique combinations where the chosen numbers add up to "target".

Important rules:

1. Each candidate can be used at most once.
2. The input may contain duplicate values.
3. The answer must not contain duplicate combinations.

Example

candidates = {10, 1, 2, 7, 6, 1, 5};
target = 8;

Output:

[
    [1, 1, 6],
    [1, 2, 5],
    [1, 7],
    [2, 6]
]

Notice that the input contains two "1"s, so we can use both of them in "[1, 1, 6]".

However, we must not generate "[1, 7]" twice.

---

2. Core Intuition

We use Backtracking, just like Combination Sum.

But there are two important changes:

1. Sort the candidates.
2. After selecting a candidate, move to the next index because each element can be used only once.

Sorting also allows us to skip duplicate values at the same recursion level.

Step 1: Sort the array

Original:

[10, 1, 2, 7, 6, 1, 5]

After sorting:

[1, 1, 2, 5, 6, 7, 10]

Step 2: Explore combinations

For each candidate:

- Include it.
- Explore the remaining candidates.
- Remove it (backtrack).
- Try another candidate.

Step 3: Skip duplicates

If two adjacent candidates have the same value at the same recursion level, skip the second one.

This prevents generating the same combination through different choices of identical elements.

---

3. Complete C++ Solution

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

---

4. Understanding the Three Most Important Lines

A. Sorting

sort(candidates.begin(), candidates.end());

Why?

Because duplicate values become adjacent.

Example:

[10, 1, 2, 7, 6, 1, 5]

Becomes:

[1, 1, 2, 5, 6, 7, 10]

Now we can easily identify duplicates and stop early when values exceed the remaining target.

B. Skipping duplicates

if (i > start && candidates[i] == candidates[i - 1]) {
    continue;
}

This is the most important line in Combination Sum II.

Let's understand it.

Suppose:

candidates = [1, 1, 2]

At the first recursion level:

start = 0

- "i = 0": Select the first "1".
- "i = 1": The value is also "1".

Since:

i > start

and:

candidates[i] == candidates[i - 1]

we skip the second "1" at this level.

But does this mean we can never select two "1"s?

No!

When the first "1" is selected, recursion starts from index "1".

At that new level:

start = 1
i = 1

Now "i > start" is false, so the second "1" can be selected.

This is exactly what we want.

Important distinction: We skip duplicate choices at the same recursion level, not duplicate values at every level.

C. Moving to the next index

backtrack(i + 1, candidates, target - candidates[i], current, result);

Why "i + 1"?

Because every array element can be used only once.

Compare this with Combination Sum:

Problem| Recursive call after choosing
Combination Sum| "backtrack(i, ...)"
Combination Sum II| "backtrack(i + 1, ...)"

---

5. Understanding the Early Stopping Condition

if (candidates[i] > target) {
    break;
}

Because the array is sorted, if the current candidate is greater than the remaining target, every candidate after it will also be too large.

Example:

candidates = [1, 2, 5, 6, 7]
target = 4

When we reach "5":

5 > 4

There is no reason to check "6" or "7".

We can stop the loop.

---

6. Complete Dry Run

Input:

candidates = [1, 1, 2, 5, 6, 7, 10]
target = 8

Initially:

current = []
target = 8
start = 0

Combination 1: [1, 1, 6]

Select the first "1":

current = [1]
remaining target = 7

Select the second "1":

current = [1, 1]
remaining target = 6

Select "2":

current = [1, 1, 2]
remaining target = 4

No remaining candidate can complete the combination.

Backtrack and remove "2".

Select "5":

current = [1, 1, 5]
remaining target = 1

This path also fails.

Backtrack and select "6" instead:

current = [1, 1, 6]
remaining target = 0

Save:

[1, 1, 6]

Combination 2: [1, 2, 5]

Backtrack to:

current = [1]
remaining target = 7

Skip the second "1" at this recursion level.

Select "2":

current = [1, 2]
remaining target = 5

Select "5":

current = [1, 2, 5]
remaining target = 0

Save:

[1, 2, 5]

Combination 3: [1, 7]

Backtrack and explore the next available candidate after "2".

Select "7":

current = [1, 7]
remaining target = 0

Save:

[1, 7]

Combination 4: [2, 6]

Backtrack to the root and skip duplicate "1" choices at that level.

Select "2":

current = [2]
remaining target = 6

Select "6":

current = [2, 6]
remaining target = 0

Save:

[2, 6]

Final result:

[
    [1, 1, 6],
    [1, 2, 5],
    [1, 7],
    [2, 6]
]

The order of combinations in the returned answer does not matter.

---

7. Visualizing the Recursion

A simplified decision tree:

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

Notice:

- The same array element is never reused.
- Duplicate choices at the same level are skipped.
- Every valid path is saved when the remaining target becomes zero.

---

8. Complexity Analysis

Let:

- "n" = number of candidates.
- "T" = target.
- "m" = smallest candidate.

Time Complexity

- Sorting: O(n log n).
- Backtracking: Exponential in the worst case because we explore combinations.
- A broad upper bound is O(2ⁿ × n), including copying combinations into the output.

The actual number of explored branches is often smaller because of duplicate skipping and early stopping.

Space Complexity

- Sorting: O(log n) auxiliary stack space on typical implementations.
- Recursion depth: O(n).
- Current combination: O(n).
- Output storage is additional and can be large.

Auxiliary space excluding output: O(n).

---

9. Common Interview Mistakes

1. Forgetting to sort: Duplicate skipping depends on adjacent equal values.
2. Using "i + 1" incorrectly: Use it in the recursive call because elements cannot be reused.
3. Using "i > 0" for duplicate skipping: It must be "i > start".
4. Skipping duplicates at every recursion level: This would incorrectly prevent using two equal values in one combination.
5. Forgetting "pop_back()": Other branches will contain unwanted elements.
6. Using "continue" instead of "break" when candidate exceeds target: Since the array is sorted, "break" avoids unnecessary checks.

---

10. Combination Sum vs Combination Sum II

This is one of the most important comparisons for interviews.

Feature| Combination Sum| Combination Sum II
Reuse candidates| Unlimited| Once per index
Input duplicates| Distinct candidates| May contain duplicates
Sorting required| Not necessary| Yes, for this approach
Include recursive call| "i"| "i + 1"
Duplicate skipping| Not required| Required
Early break after exceeding target| Possible if sorted| Yes

Remember these two lines

Combination Sum:

backtrack(i, ...);

Combination Sum II:

backtrack(i + 1, ...);

And the duplicate-skipping condition:

if (i > start && candidates[i] == candidates[i - 1]) {
    continue;
}

---

11. Pattern Recognition for Interviews

Think about this pattern when a problem asks you to:

- Generate unique combinations.
- Reach a specific target sum.
- Use each input element at most once.
- Handle duplicate values in the input.

The combination of sorting, DFS and backtracking is a common solution pattern.

---

12. 30-Second Interview Revision

Q1. Why sort the candidates?

To group duplicate values together and enable early stopping.

Q2. Why use "i + 1"?

Each candidate index can be used only once.

Q3. Why use "i > start" in duplicate skipping?

To skip identical choices at the same recursion level while still allowing duplicate values to be selected at different levels.

Q4. Can we select two equal numbers?

Yes, if they are separate elements in the input array.

Q5. Why use "break" when candidate exceeds target?

The array is sorted, so all later candidates will also exceed the remaining target.

Final takeaway

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
