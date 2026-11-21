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

For example, 3 elements produce (2^3 = 8) subsets.

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
