

# 45. Invert Binary Tree

## 1. Problem Understanding

Given the root of a binary tree, invert the tree and return its root.

Inverting means swapping the left and right child of **every node** in the tree.

### Example

**Input:**
```text
        4
       / \
      2   7
     / \ / \
    1  3 6  9
```

**Output:**
```text
        4
       / \
      7   2
     / \ / \
    9  6 3  1
```

Notice:
- Node `4`: swaps children `2` and `7`.
- Node `2`: swaps children `1` and `3`.
- Node `7`: swaps children `6` and `9`.

Every node follows the same operation.

---

## 2. Intuition — Think Like This

Imagine standing in front of a binary tree.

At every node:
1. Exchange the left child with the right child.
2. Visit the left subtree.
3. Visit the right subtree.
4. Continue until there are no more nodes.

The important observation is that **the same operation works for every node**. This is why recursion is a natural solution.

---

## 3. C++ Solution — Recursive DFS

```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {

        // Base case: empty tree
        if (root == nullptr) {
            return nullptr;
        }

        // Swap left and right children
        swap(root->left, root->right);

        // Invert left subtree
        invertTree(root->left);

        // Invert right subtree
        invertTree(root->right);

        return root;
    }
};
```

---

## 4. Understand the Code Step by Step

### Step 1: Base Case

```cpp
if (root == nullptr) {
    return nullptr;
}
```

If the current node is `nullptr`, there is nothing to invert.

This also handles an empty tree.

### Step 2: Swap Children

```cpp
swap(root->left, root->right);
```

Exchange the addresses stored in the left and right pointers.

For example:

Before:
```text
      4
     / \
    2   7
```

After:
```text
      4
     / \
    7   2
```

### Step 3: Recursively Invert Both Subtrees

```cpp
invertTree(root->left);
invertTree(root->right);
```

After swapping the current node, process both children.

Each recursive call performs exactly the same operation.

### Step 4: Return Root

```cpp
return root;
```

Return the original root pointer, which now points to the root of the inverted tree.

---

## 5. Recursion Dry Run

Input:
```text
        4
       / \
      2   7
     / \ / \
    1  3 6  9
```

The calls work like this:

| Current node | Operation |
|---|---|
| 4 | Swap 2 and 7 |
| 7 | Swap 6 and 9 |
| 9 | No children to swap |
| 6 | No children to swap |
| 2 | Swap 1 and 3 |
| 3 | No children to swap |
| 1 | No children to swap |

Final tree:
```text
        4
       / \
      7   2
     / \ / \
    9  6 3  1
```

The exact traversal order here is:

**Root → Left → Right**, after swapping the children.

This is a form of DFS (Depth-First Search).

---

## 6. Complexity Analysis

Let `n` be the total number of nodes.

| Complexity | Explanation |
|---|---|
| Time | O(n) |
| Auxiliary space | O(h) |

- **Time O(n):** Every node is visited exactly once.
- **Space O(h):** The recursion call stack can grow to the tree height `h`.
  - Balanced tree: O(log n)
  - Skewed tree: O(n)

The tree is modified in place, so no additional tree is created.

---

## 7. Important C++ Concepts

### `swap()`

```cpp
swap(root->left, root->right);
```

The `swap()` function exchanges two values.

Here, the values are pointers to `TreeNode` objects. Therefore, we exchange which node each child pointer refers to; we do not copy the actual nodes.

### `root->left` vs `root.left`

- `root->left` is used because `root` is a pointer.
- `root.left` would be used if `root` were an actual `TreeNode` object.

---

## 8. Alternative Approach — Iterative BFS

We can also invert the tree using a queue.

```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {

        if (root == nullptr) {
            return nullptr;
        }

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {

            TreeNode* node = q.front();
            q.pop();

            swap(node->left, node->right);

            if (node->left != nullptr) {
                q.push(node->left);
            }

            if (node->right != nullptr) {
                q.push(node->right);
            }
        }

        return root;
    }
};
```

This approach uses **BFS (Breadth-First Search)**, processing the tree level by level.

| Approach | Traversal | Auxiliary space |
|---|---|---|
| Recursive | DFS | O(h) |
| Iterative | BFS | O(w), where w is maximum tree width |

Both approaches take O(n) time.

For interviews, the recursive DFS solution is usually the simplest to explain.

---

## 9. Common Mistakes

1. **Forgetting the base case:** Always handle `root == nullptr`.
2. **Swapping only the root:** Every node must be inverted, not just the root.
3. **Not processing both subtrees:** Recursively visit both children.
4. **Returning the wrong pointer:** Return `root`, not `root->left` or `root->right`.
5. **Creating unnecessary new nodes:** This problem can be solved by swapping existing pointers.

---

## 10. Interview Questions

**Q1. Why do we use recursion here?**

Because the left and right subtrees are themselves binary trees, and the same inversion operation can be applied to each subtree.

**Q2. What is the base case?**

When `root == nullptr`, return `nullptr`.

**Q3. Does this solution create a new tree?**

No. It modifies the existing tree by swapping child pointers.

**Q4. Can we solve this without recursion?**

Yes, using BFS with a queue or DFS with an explicit stack.

**Q5. What is the worst-case recursion space complexity?**

O(n), when the tree is completely skewed.

---

## 11. Pattern Recognition

When you see a binary tree problem asking you to:
- Visit every node.
- Perform the same operation on every node.
- Process left and right subtrees independently.

Think about **Tree DFS + Recursion**.

Other problems where this pattern is useful include:
- Maximum Depth of Binary Tree
- Same Tree
- Diameter of Binary Tree
- Balanced Binary Tree

---

## 12. 30-Second Interview Revision

- **Pattern:** Binary Tree DFS.
- **Approach:** Recursively swap left and right children at every node.
- **Base case:** `root == nullptr`.
- **Time:** O(n).
- **Space:** O(h) recursion stack.
- **Key line:** `swap(root->left, root->right);`



# 46. Maximum Depth of Binary Tree

## 1. Problem Understanding

Given the root of a binary tree, find its maximum depth.

Maximum depth means the **number of nodes along the longest path from the root node down to the farthest leaf node**.

Example:

**Input:**
```text
        3
       / \
      9   20
          / \
         15  7
```

**Output:**
```text
3
```

Why?

The longest paths are:
- `3 → 20 → 15`
- `3 → 20 → 7`

Both contain 3 nodes.

---

## 2. Intuition — Think Like This

At every node, ask two questions:

1. What is the maximum depth of my left subtree?
2. What is the maximum depth of my right subtree?

Take whichever is greater and add `1` for the current node.

The formula is:

\[
Depth(root)=1+\max(Depth(left),Depth(right))
\]

If there is no node, its depth is `0`.

---

## 3. C++ Solution — Recursive DFS

```cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {

        // Base case
        if (root == nullptr) {
            return 0;
        }

        // Calculate left and right subtree depths
        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);

        // Current node + deeper subtree
        return 1 + max(leftDepth, rightDepth);
    }
};
```

---

## 4. Dry Run Step by Step

Consider:

```text
        3
       / \
      9   20
          / \
         15  7
```

The recursion first explores the left side, then the right side.

| Node | Left depth | Right depth | Returned depth |
|---|---:|---:|---:|
| 9 | 0 | 0 | 1 |
| 15 | 0 | 0 | 1 |
| 7 | 0 | 0 | 1 |
| 20 | 1 | 1 | 2 |
| 3 | 1 | 2 | 3 |

Final answer: **3**

### Visualize the recursion

```text
maxDepth(3)
    |
    +-- maxDepth(9)  = 1
    |
    +-- maxDepth(20) = 2
             |
             +-- maxDepth(15) = 1
             |
             +-- maxDepth(7)  = 1

Answer = 1 + max(1, 2) = 3
```

Notice that the function reaches the leaf nodes first. Their results are then returned to their parent nodes.

This is called **postorder-style DFS** because we calculate the children before returning the result for the current node.

---

## 5. Understand the Base Case

```cpp
if (root == nullptr) {
    return 0;
}
```

Why return `0`?

Because an empty tree has zero nodes and therefore zero depth.

For a leaf node:

```text
    9
```

Both children are `nullptr`.

Therefore:

```cpp
leftDepth = 0;
rightDepth = 0;

return 1 + max(0, 0);  // 1
```

A leaf node has a depth of `1`.

---

## 6. Complexity Analysis

Let `n` be the number of nodes and `h` be the height of the tree.

| Complexity | Explanation |
|---|---|
| Time | O(n) |
| Auxiliary space | O(h) |

- **Time O(n):** Every node is visited exactly once.
- **Space O(h):** Recursive calls occupy the call stack up to the tree height.
  - Balanced tree: O(log n)
  - Completely skewed tree: O(n)

---

## 7. Alternative Approach — Iterative BFS

We can also calculate maximum depth using a queue and level-order traversal.

```cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {

        if (root == nullptr) {
            return 0;
        }

        queue<TreeNode*> q;
        q.push(root);

        int depth = 0;

        while (!q.empty()) {

            int size = q.size();

            // Process one complete level
            for (int i = 0; i < size; i++) {

                TreeNode* node = q.front();
                q.pop();

                if (node->left != nullptr) {
                    q.push(node->left);
                }

                if (node->right != nullptr) {
                    q.push(node->right);
                }
            }

            depth++;
        }

        return depth;
    }
};
```

### How BFS works

```text
Level 1:          3
                  depth = 1

Level 2:        9   20
                  depth = 2

Level 3:           15  7
                     depth = 3
```

Every time we finish processing one level, we increase `depth`.

| Approach | Technique | Auxiliary space |
|---|---|---|
| Recursive | DFS | O(h) |
| Iterative | BFS | O(w), where w is maximum tree width |

Both take O(n) time.

---

## 8. Common Mistakes

1. Returning `1` for `nullptr` instead of `0`.
2. Forgetting to add `1` for the current node.
3. Using `min()` instead of `max()`.
4. Confusing the number of edges with the number of nodes.
5. Calculating only the left or right subtree instead of both.

**Important:** This problem counts nodes, not edges. A tree with only one node has maximum depth `1`.

---

## 9. Interview Questions

**Q1. Why do we use `max()`?**

Because we need the longest path from the root to any leaf.

**Q2. Why do we add `1`?**

To include the current node in the depth calculation.

**Q3. What is the base case?**

When `root == nullptr`, return `0`.

**Q4. Why is the time complexity O(n)?**

Every node is visited once.

**Q5. Can BFS solve this problem?**

Yes. Count how many levels exist using level-order traversal.

---

## 10. Pattern Recognition

When a binary tree problem asks you to:
- Find the height or depth.
- Calculate a result using left and right subtrees.
- Find the longest path from a node downward.

Think about **Binary Tree DFS + Recursion**.

Related problems:
- Invert Binary Tree
- Diameter of Binary Tree
- Balanced Binary Tree
- Same Tree

---

## 11. 30-Second Interview Revision

- **Pattern:** Binary Tree DFS.
- **Base case:** `root == nullptr` returns `0`.
- **Left depth:** `maxDepth(root->left)`.
- **Right depth:** `maxDepth(root->right)`.
- **Formula:** `1 + max(leftDepth, rightDepth)`.
- **Time:** O(n).
- **Space:** O(h).

**Remember:** Maximum depth = current node + maximum depth of its deeper subtree.



# 47. Diameter of Binary Tree

## 1. Problem Understanding

Given the root of a binary tree, return the diameter of the tree.

The **diameter** is the length of the longest path between any two nodes in the tree.

Important: The diameter is measured in **number of edges**, not number of nodes.

### Example

**Input:**
```text
        1
       / \
      2   3
     / \
    4   5
```

**Output:**
```text
3
```

Why?

The longest path is:

```text
    4
     \
      2
       \
        1
         \
          3
```

More accurately, the path is:

```text
4 → 2 → 1 → 3
```

It contains:
- 4 nodes
- 3 edges

Therefore, diameter = **3**.

---

## 2. Intuition — Think Like This

At every node, imagine a path that starts from the deepest node in its left subtree, reaches the current node, and continues to the deepest node in its right subtree.

The longest path passing through the current node is:

\[
DiameterAtNode = LeftHeight + RightHeight
\]

We need the maximum such value across all nodes.

So we need two things:
1. Calculate the height of every node.
2. Keep updating the maximum diameter.

**Important observation:** The diameter does not necessarily pass through the root. It can exist entirely inside the left or right subtree.

---

## 3. C++ Solution — DFS Recursion

```cpp
class Solution {
public:
    int diameter = 0;

    int height(TreeNode* root) {

        if (root == nullptr) {
            return 0;
        }

        int leftHeight = height(root->left);
        int rightHeight = height(root->right);

        // Update the maximum diameter
        diameter = max(diameter, leftHeight + rightHeight);

        // Return the height of the current subtree
        return 1 + max(leftHeight, rightHeight);
    }

    int diameterOfBinaryTree(TreeNode* root) {

        diameter = 0;
        height(root);

        return diameter;
    }
};
```

---

## 4. Understand the Code Step by Step

### Step 1: Maintain a diameter variable

```cpp
int diameter = 0;
```

This stores the maximum diameter found so far.

We initialize it to `0` because an empty tree or a single-node tree has zero edges.

### Step 2: Calculate left and right heights

```cpp
int leftHeight = height(root->left);
int rightHeight = height(root->right);
```

We recursively calculate the height of both subtrees.

Remember:

\[
Height = 1 + \max(LeftHeight, RightHeight)
\]

### Step 3: Update diameter

```cpp
diameter = max(diameter, leftHeight + rightHeight);
```

Why do we add the heights?

Because the longest path passing through the current node uses:
- The deepest path from the left subtree.
- The current node as the connecting point.
- The deepest path from the right subtree.

The number of edges is exactly `leftHeight + rightHeight`.

### Step 4: Return the current height

```cpp
return 1 + max(leftHeight, rightHeight);
```

This is important: the function returns height to its parent, not diameter.

The diameter is maintained separately.

### Step 5: Return the final answer

```cpp
height(root);
return diameter;
```

We calculate heights for the entire tree and update the diameter along the way.

---

## 5. Dry Run Step by Step

Consider:

```text
        1
       / \
      2   3
     / \
    4   5
```

The recursion reaches the leaf nodes first.

| Node | Left height | Right height | Diameter at node | Returned height |
|---|---:|---:|---:|---:|
| 4 | 0 | 0 | 0 | 1 |
| 5 | 0 | 0 | 0 | 1 |
| 2 | 1 | 1 | 2 | 2 |
| 3 | 0 | 0 | 0 | 1 |
| 1 | 2 | 1 | 3 | 3 |

Final diameter:

\[
\max(0,2,0,3)=3
\]

<text id="dr47" />

### Visual understanding

```text
        1
       / \
      2   3
     / \
    4   5

At node 2:
Left height  = 1
Right height = 1
Diameter     = 1 + 1 = 2

At node 1:
Left height  = 2
Right height = 1
Diameter     = 2 + 1 = 3
```

The maximum diameter is `3`.

---

## 6. Why Do We Return Height Instead of Diameter?

This is the most important concept in this problem.

Our recursive function has two responsibilities:

| Responsibility | How it is handled |
|---|---|
| Calculate height | Return value |
| Track maximum diameter | Update shared variable |

Why?

The parent node needs the height of its child subtree to calculate its own height and diameter.

The parent does not need the child's diameter to calculate its own height.

This is a common **Tree DFS pattern: return one value and update another shared result**.

---

## 7. Complexity Analysis

Let `n` be the total number of nodes and `h` be the tree height.

| Complexity | Explanation |
|---|---|
| Time | O(n) |
| Auxiliary space | O(h) |

- **Time O(n):** Every node is visited exactly once.
- **Space O(h):** Recursive call stack depends on tree height.
  - Balanced tree: O(log n)
  - Skewed tree: O(n)

We do not calculate the height repeatedly for every node. Each subtree's height is calculated once.

---

## 8. Common Mistakes

1. **Returning `leftHeight + rightHeight` from the helper:** That is the diameter through the current node, not the height needed by the parent.
2. **Forgetting to update the global diameter:** The answer must consider every node.
3. **Adding `1` to the diameter:** The problem counts edges, not nodes.
4. **Assuming the longest path passes through the root:** It may be completely inside a subtree.
5. **Recalculating heights separately:** This can lead to O(n²) time in a skewed tree.

---

## 9. Interview Questions

**Q1. Why is diameter calculated as left height + right height?**

Because the longest path passing through a node connects the deepest node on its left side to the deepest node on its right side. Those heights count the edges on both sides.

**Q2. Why don't we add 1 for the current node?**

Because diameter is measured in edges. The two subtree heights already count the edges connecting their deepest nodes to the current node.

**Q3. Can the diameter be smaller than the height?**

Yes. For a tree containing only one node, height is `1`, but diameter is `0`.

**Q4. Why do we use a shared variable?**

To maintain the maximum diameter found across all nodes while the recursive function returns subtree height.

**Q5. What is the time complexity?**

O(n), because each node is visited exactly once.

---

## 10. Pattern Recognition

When a binary tree problem asks you to:
- Calculate height at every node.
- Find the longest path.
- Combine information from left and right subtrees.
- Maintain a maximum or minimum result globally.

Think about:

**Postorder DFS + Shared Result Variable**

Related problems:
- Maximum Depth of Binary Tree
- Balanced Binary Tree
- Binary Tree Maximum Path Sum
- Lowest Common Ancestor of a Binary Tree

---

## 11. 30-Second Interview Revision

- **Pattern:** Binary Tree DFS / Postorder.
- **Helper returns:** Height of the subtree.
- **Left height:** `height(root->left)`.
- **Right height:** `height(root->right)`.
- **Diameter formula:** `leftHeight + rightHeight`.
- **Height formula:** `1 + max(leftHeight, rightHeight)`.
- **Time:** O(n).
- **Space:** O(h).

**Remember this distinction:**

- Height = `1 + max(left, right)`
- Diameter through current node = `left + right`

The helper returns height but updates diameter.



# 48. Balanced Binary Tree

## 1. Problem Understanding

Given the root of a binary tree, determine whether it is height-balanced.

A binary tree is height-balanced if:

> For every node, the difference between the heights of its left and right subtrees is no more than 1.

Mathematically:

\[
|LeftHeight-RightHeight|\leq 1
\]

This condition must be true for **every node**, not just the root.

### Example 1

**Input:**
```text
        3
       / \
      9   20
          / \
         15  7
```

**Output:**
```text
true
```

The height difference at every node is at most `1`.

### Example 2

**Input:**
```text
        1
       /
      2
     /
    3
   /
  4
```

**Output:**
```text
false
```

At node `2`, the left subtree has height `2`, and the right subtree has height `0`.

Difference = `2`, which is greater than `1`.

---

## 2. Intuition — Think Like This

At every node, ask three questions:

1. What is the height of my left subtree?
2. What is the height of my right subtree?
3. Is the difference between these heights greater than `1`?

If the difference is greater than `1`, the tree is not balanced.

But there is one more important condition:

**Both left and right subtrees must also be balanced.**

We can solve this using recursion.

---

## 3. C++ Solution — Optimized DFS

```cpp
class Solution {
public:
    int checkHeight(TreeNode* root) {

        // Base case: empty tree is balanced
        if (root == nullptr) {
            return 0;
        }

        // Calculate left subtree height
        int leftHeight = checkHeight(root->left);

        // If left subtree is unbalanced
        if (leftHeight == -1) {
            return -1;
        }

        // Calculate right subtree height
        int rightHeight = checkHeight(root->right);

        // If right subtree is unbalanced
        if (rightHeight == -1) {
            return -1;
        }

        // Check current node's balance condition
        if (abs(leftHeight - rightHeight) > 1) {
            return -1;
        }

        // Return current subtree height
        return 1 + max(leftHeight, rightHeight);
    }

    bool isBalanced(TreeNode* root) {
        return checkHeight(root) != -1;
    }
};
```

---

## 4. Understand the Code Step by Step

### Step 1: Base Case

```cpp
if (root == nullptr) {
    return 0;
}
```

An empty tree has height `0` and is considered balanced.

### Step 2: Calculate Left Height

```cpp
int leftHeight = checkHeight(root->left);
```

Recursively calculate the height of the left subtree.

### Step 3: Check Whether Left Subtree Is Balanced

```cpp
if (leftHeight == -1) {
    return -1;
}
```

We use `-1` as a special signal.

- Normal height: `0` or a positive number.
- `-1`: This subtree is unbalanced.

If the left subtree is already unbalanced, there is no need to process the right subtree.

### Step 4: Calculate Right Height

```cpp
int rightHeight = checkHeight(root->right);
```

Calculate the right subtree height.

Then check:

```cpp
if (rightHeight == -1) {
    return -1;
}
```

If the right subtree is unbalanced, propagate `-1` to the parent.

### Step 5: Check the Balance Condition

```cpp
if (abs(leftHeight - rightHeight) > 1) {
    return -1;
}
```

For a balanced node:

\[
|LeftHeight-RightHeight|\leq 1
\]

If the difference exceeds `1`, return `-1`.

### Step 6: Return the Height

```cpp
return 1 + max(leftHeight, rightHeight);
```

If both subtrees are balanced and the current node satisfies the condition, return its height.

### Step 7: Final Answer

```cpp
bool isBalanced(TreeNode* root) {
    return checkHeight(root) != -1;
}
```

- If the result is `-1`, return `false`.
- Otherwise, return `true`.

---

## 5. Dry Run Step by Step

Consider:

```text
        3
       / \
      9   20
          / \
         15  7
```

Recursion processes the leaf nodes first.

| Node | Left height | Right height | Difference | Returned value |
|---|---:|---:|---:|---:|
| 9 | 0 | 0 | 0 | 1 |
| 15 | 0 | 0 | 0 | 1 |
| 7 | 0 | 0 | 0 | 1 |
| 20 | 1 | 1 | 0 | 2 |
| 3 | 1 | 2 | 1 | 3 |

Every node satisfies the balance condition.

Final result:

```cpp
true
```

### What happens in an unbalanced tree?

```text
        1
       /
      2
     /
    3
```

| Node | Left height | Right height | Difference | Returned value |
|---|---:|---:|---:|---:|
| 3 | 0 | 0 | 0 | 1 |
| 2 | 1 | 0 | 1 | 2 |
| 1 | 2 | 0 | 2 | -1 |

At node `1`, the difference is `2`.

Therefore, `-1` is returned, and the final answer is `false`.

---

## 6. Why Do We Use -1?

This is the most important optimization in this problem.

We need to return two kinds of information:
- The height of a subtree.
- Whether that subtree is balanced.

Instead of creating separate functions or repeatedly checking the tree, we use one return value for both.

| Return value | Meaning |
|---|---|
| `0` | Empty subtree |
| Positive integer | Balanced subtree; value represents height |
| `-1` | Unbalanced subtree |

This technique avoids unnecessary repeated height calculations.

---

## 7. Complexity Analysis

Let `n` be the number of nodes and `h` be the height of the tree.

| Complexity | Explanation |
|---|---|
| Time | O(n) |
| Auxiliary space | O(h) |

- **Time O(n):** Each node is visited at most once.
- **Space O(h):** Recursion stack depends on tree height.
  - Balanced tree: O(log n)
  - Skewed tree: O(n)

---

## 8. Alternative Approach — Simple but Less Efficient

Another way is to create one function for calculating height and another for checking balance.

```cpp
class Solution {
public:
    int height(TreeNode* root) {

        if (root == nullptr) {
            return 0;
        }

        return 1 + max(height(root->left), height(root->right));
    }

    bool isBalanced(TreeNode* root) {

        if (root == nullptr) {
            return true;
        }

        int leftHeight = height(root->left);
        int rightHeight = height(root->right);

        if (abs(leftHeight - rightHeight) > 1) {
            return false;
        }

        return isBalanced(root->left) &&
               isBalanced(root->right);
    }
};
```

**Problem:** This approach may calculate the same subtree heights repeatedly.

- Worst-case time: O(n²), especially for a skewed tree.
- Auxiliary space: O(h).

For interviews, prefer the optimized `-1` approach.

---

## 9. Common Mistakes

1. Checking balance only at the root instead of every node.
2. Checking the height difference but forgetting to check whether the child subtrees are balanced.
3. Returning `false` or `true` from a helper that is supposed to return height.
4. Forgetting to propagate the `-1` signal.
5. Using the simple height approach without considering repeated calculations.

---

## 10. Interview Questions

**Q1. What is a height-balanced binary tree?**

A binary tree where the height difference between the left and right subtrees of every node is at most `1`.

**Q2. Why do we use `-1`?**

To indicate that a subtree is unbalanced while using the same function to return subtree height.

**Q3. Why is the optimized solution O(n)?**

Because every node is visited at most once, and its height is calculated during that visit.

**Q4. Why do we check the left and right subtree results before checking the current node?**

Because if either child subtree is already unbalanced, the entire tree containing that subtree cannot be balanced.

**Q5. Can a tree have a balanced root but still be unbalanced overall?**

Yes. A deeper node may have an unbalanced subtree even if the root's immediate left and right heights differ by at most `1`.

---

## 11. Pattern Recognition

When a binary tree problem asks you to:
- Calculate subtree heights.
- Check a condition at every node.
- Return information from child nodes to the parent.
- Stop early when a subtree violates a condition.

Think about:

**Postorder DFS + Information Propagation**

Related problems:
- Maximum Depth of Binary Tree
- Diameter of Binary Tree
- Binary Tree Maximum Path Sum
- Count Good Nodes in Binary Tree

---

## 12. 30-Second Interview Revision

- **Pattern:** Binary Tree DFS / Postorder.
- **Base case:** `nullptr` returns `0`.
- **Calculate:** Left and right subtree heights.
- **Condition:** `abs(leftHeight - rightHeight) > 1`.
- **Special signal:** `-1` means unbalanced.
- **Height formula:** `1 + max(leftHeight, rightHeight)`.
- **Time:** O(n).
- **Space:** O(h).

**Remember:** Return height when balanced; return `-1` when unbalanced.



# 49. Same Tree

## 1. Problem Understanding

Given the roots of two binary trees `p` and `q`, determine whether they are identical.

Two trees are considered the same when:

1. Both trees have the same structure.
2. Corresponding nodes have the same values.

### Example 1

**Input:**
```text
Tree p:       Tree q:

    1             1
   / \           / \
  2   3         2   3
```

**Output:** `true`

Both trees have the same structure and values.

### Example 2

**Input:**
```text
Tree p:       Tree q:

    1             1
   /               \
  2                 2
```

**Output:** `false`

The values are the same, but the structures are different.

### Example 3

**Input:**
```text
Tree p:       Tree q:

    1             1
   / \           / \
  2   1         1   2
```

**Output:** `false`

The structures are the same, but corresponding node values are different.

---

## 2. Intuition — Think Like This

Imagine comparing two trees side by side.

At every step, ask three questions:

1. Are both current nodes empty?
2. If both exist, do their values match?
3. Are their left children and right children also identical?

We can solve this by comparing corresponding nodes recursively.

**Important:** We must compare left with left and right with right. We cannot interchange them.

---

## 3. C++ Solution — Recursive DFS

```cpp
class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {

        // Case 1: Both nodes are empty
        if (p == nullptr && q == nullptr) {
            return true;
        }

        // Case 2: Only one node is empty
        if (p == nullptr || q == nullptr) {
            return false;
        }

        // Case 3: Values are different
        if (p->val != q->val) {
            return false;
        }

        // Compare left and right subtrees
        return isSameTree(p->left, q->left) &&
               isSameTree(p->right, q->right);
    }
};
```

---

## 4. Understand the Code Step by Step

### Step 1: Both Nodes Are Null

```cpp
if (p == nullptr && q == nullptr) {
    return true;
}
```

If both nodes are empty, they match.

For example:

```text
p: nullptr
q: nullptr
```

Both are empty, so return `true`.

### Step 2: Only One Node Is Null

```cpp
if (p == nullptr || q == nullptr) {
    return false;
}
```

If one tree has a node but the other doesn't, their structures are different.

Example:

```text
p:       q:

  1      1
 / 
2
```

When comparing the left children, one exists and the other is `nullptr`.

Therefore, return `false`.

### Step 3: Compare Node Values

```cpp
if (p->val != q->val) {
    return false;
}
```

If both nodes exist but their values are different, the trees are not identical.

### Step 4: Compare Both Subtrees

```cpp
return isSameTree(p->left, q->left) &&
       isSameTree(p->right, q->right);
```

This is the main recursive logic.

- Compare left subtree of `p` with left subtree of `q`.
- Compare right subtree of `p` with right subtree of `q`.

We use `&&` because **both comparisons must be true**.

---

## 5. Dry Run Step by Step

Consider:

```text
Tree p:          Tree q:

      1                1
     / \              / \
    2   3            2   3
   /                  /
  4                  4
```

The recursive comparisons are:

| Comparison | Result |
|---|---|
| `p(1)` vs `q(1)` | Values match |
| `p(2)` vs `q(2)` | Values match |
| `p(4)` vs `q(4)` | Values match |
| `nullptr` vs `nullptr` | Both empty → true |
| `nullptr` vs `nullptr` | Both empty → true |
| `p(3)` vs `q(3)` | Values match |
| `nullptr` vs `nullptr` | Both empty → true |
| `nullptr` vs `nullptr` | Both empty → true |

Every comparison succeeds.

Final result:

```cpp
true
```

### What if the values differ?

```text
Tree p:          Tree q:

      1                1
     / \              / \
    2   3            2   4
```

When comparing the right children:

```cpp
p->val = 3;
q->val = 4;
```

Since `3 != 4`, the function immediately returns `false`.

---

## 6. Why Do We Use && Instead of ||?

```cpp
return isSameTree(p->left, q->left) &&
       isSameTree(p->right, q->right);
```

We use logical AND because both subtrees must match.

| Left comparison | Right comparison | Final result |
|---|---|---|
| true | true | true |
| true | false | false |
| false | true | false |
| false | false | false |

Using `||` would be incorrect because it could return `true` even when one subtree is different.

Also, `&&` uses short-circuit evaluation: if the left comparison is false, the right comparison is not executed.

---

## 7. Complexity Analysis

Let:
- `n` = number of nodes in tree `p`.
- `m` = number of nodes in tree `q`.

| Complexity | Explanation |
|---|---|
| Time | O(min(n, m)) in the worst case up to the first mismatch; O(n) when both trees have n nodes and match |
| Auxiliary space | O(min(h1, h2)) for recursive calls |

- **Time:** Each corresponding pair of nodes is compared at most once. A mismatch can stop the comparison early.
- **Space:** Recursion stack depends on the height of the trees being compared.

---

## 8. Common Mistakes

1. Checking only node values and ignoring tree structure.
2. Comparing left child of one tree with right child of the other.
3. Forgetting to handle `nullptr`.
4. Using `||` instead of `&&`.
5. Accessing `p->val` or `q->val` before checking whether the pointers are null.

---

## 9. Interview Questions

**Q1. When are two binary trees considered identical?**

When their structures are identical and all corresponding node values are equal.

**Q2. Why do we check null pointers first?**

To safely handle missing nodes and prevent accessing values through null pointers.

**Q3. Why do we use recursion?**

Because comparing two trees requires comparing their corresponding left and right subtrees using the same logic.

**Q4. What happens if both roots are null?**

Return `true`, because both trees are empty and identical.

**Q5. Why do we use `&&`?**

Both left and right subtree comparisons must succeed.

---

## 10. Pattern Recognition

When a binary tree problem asks you to:
- Compare two trees.
- Validate matching structures.
- Compare corresponding node values.
- Apply the same operation to two subtrees.

Think about:

**Binary Tree DFS + Simultaneous Recursion**

Related problems:
- Invert Binary Tree
- Subtree of Another Tree
- Symmetric Tree
- Same Tree

---

## 11. 30-Second Interview Revision

- **Pattern:** Binary Tree DFS.
- Both null → `true`.
- Only one null → `false`.
- Different values → `false`.
- Compare left with left.
- Compare right with right.
- Both comparisons must be true (`&&`).
- **Time:** O(n) when both trees match and each has n nodes.
- **Space:** O(h) for recursion.

**Remember:** Same Tree = Same Structure + Same Values.



# 50. Subtree of Another Tree

## 1. Problem Understanding

Given two binary trees, `root` and `subRoot`, determine whether `subRoot` is a subtree of `root`.

A subtree consists of:
- A node in the main tree.
- All descendants of that node.
- The same structure and node values as the candidate subtree.

### Example 1

**Input:**
```text
root:

        3
       / \
      4   5
     / \
    1   2

subRoot:

      4
     / \
    1   2
```

**Output:** `true`

The subtree rooted at node `4` matches `subRoot` exactly.

### Example 2

**Input:**
```text
root:

        3
       / \
      4   5
     / \
    1   2
       /
      0

subRoot:

      4
     / \
    1   2
```

**Output:** `false`

The values `4`, `1`, and `2` match, but the structures differ.

The extra node `0` means the subtree rooted at `4` is not identical to `subRoot`.

---

## 2. Intuition — Think Like This

We have two trees:

- Main tree (`root`).
- Target tree (`subRoot`).

We need to find whether the target tree exists anywhere inside the main tree.

Think of the solution in two steps:

1. Visit every node in the main tree.
2. At each node, check whether the tree starting from that node is exactly the same as `subRoot`.

If any node matches, return `true`.

Otherwise, return `false`.

**Important connection:** We already solved the exact tree-comparison logic in Problem 49 — Same Tree.

We can reuse it here.

---

## 3. C++ Solution — DFS + Same Tree

```cpp
class Solution {
public:

    // Check whether two trees are identical
    bool isSameTree(TreeNode* p, TreeNode* q) {

        if (p == nullptr && q == nullptr) {
            return true;
        }

        if (p == nullptr || q == nullptr) {
            return false;
        }

        if (p->val != q->val) {
            return false;
        }

        return isSameTree(p->left, q->left) &&
               isSameTree(p->right, q->right);
    }

    bool isSubtree(TreeNode* root, TreeNode* subRoot) {

        // Empty target tree is a subtree
        if (subRoot == nullptr) {
            return true;
        }

        // Main tree is empty, but target is not
        if (root == nullptr) {
            return false;
        }

        // Check whether current tree matches target
        if (isSameTree(root, subRoot)) {
            return true;
        }

        // Search in left and right subtrees
        return isSubtree(root->left, subRoot) ||
               isSubtree(root->right, subRoot);
    }
};
```

---

## 4. Understand the Code Step by Step

### Step 1: Reuse Same Tree Logic

```cpp
bool isSameTree(TreeNode* p, TreeNode* q)
```

This helper checks:
- Both nodes are null → `true`.
- Only one is null → `false`.
- Values differ → `false`.
- Otherwise, compare both left and right subtrees.

This is exactly Problem 49.

### Step 2: Handle Empty Target Tree

```cpp
if (subRoot == nullptr) {
    return true;
}
```

An empty tree is considered a subtree of any tree.

### Step 3: Handle Empty Main Tree

```cpp
if (root == nullptr) {
    return false;
}
```

If the main tree is empty but the target is not, there is no possible match.

### Step 4: Check the Current Node

```cpp
if (isSameTree(root, subRoot)) {
    return true;
}
```

This is the most important line.

At the current node, compare the entire subtree rooted there with `subRoot`.

If both trees are identical, we found the answer.

### Step 5: Search Left and Right

```cpp
return isSubtree(root->left, subRoot) ||
       isSubtree(root->right, subRoot);
```

If the current subtree doesn't match, search:
- Left subtree.
- Right subtree.

We use `||` because finding a match in either side is enough.

---

## 5. Dry Run Step by Step

Consider:

```text
Main tree:

        3
       / \
      4   5
     / \
    1   2

Target:

      4
     / \
    1   2
```

### Step 1: Compare at node 3

```cpp
isSameTree(node3, subRoot)
```

Values:

```text
3 != 4
```

So this comparison returns `false`.

### Step 2: Search the left subtree

```cpp
isSubtree(node3->left, subRoot)
```

Now the current node is `4`.

### Step 3: Compare at node 4

```cpp
isSameTree(node4, subRoot)
```

Comparison:

```text
Main subtree:       Target:

      4                4
     / \              / \
    1   2            1   2
```

- Root values match.
- Left children match.
- Right children match.
- Their corresponding null children match.

Therefore:

```cpp
isSameTree(node4, subRoot) == true
```

### Step 4: Return the answer

Since we found a match:

```cpp
isSubtree(root, subRoot) == true
```

We don't need to search the remaining nodes.

---

## 6. Why Do We Use OR (`||`) Here?

```cpp
return isSubtree(root->left, subRoot) ||
       isSubtree(root->right, subRoot);
```

Because we only need one matching subtree.

| Left subtree | Right subtree | Final answer |
|---|---|---|
| true | true | true |
| true | false | true |
| false | true | true |
| false | false | false |

Compare this with Problem 49:

- Same Tree uses `&&` because both sides must match.
- Subtree uses `||` because either side can contain the target.

This distinction is important for interviews.

---

## 7. Complexity Analysis

Let:
- `n` = number of nodes in the main tree.
- `m` = number of nodes in `subRoot`.
- `h` = height of the main tree.

| Complexity | Explanation |
|---|---|
| Time | O(n × m) worst case |
| Auxiliary space | O(h + h₂) |

**Time O(n × m):**

- We may visit every node in the main tree.
- At each node, `isSameTree()` may compare up to `m` nodes.

Therefore, worst-case time complexity is O(n × m).

**Space:**

- `isSubtree()` uses recursion up to the height of the main tree.
- `isSameTree()` uses recursion up to the height of the target tree.

So auxiliary space is O(h + h₂), where `h₂` is the target tree height.

---

## 8. Common Mistakes

1. Checking only whether the target root value exists in the main tree.
2. Comparing values without comparing the complete structure.
3. Forgetting to search both left and right subtrees.
4. Using `&&` instead of `||` when searching for the target.
5. Forgetting the empty `subRoot` case.
6. Returning true immediately after finding a matching root value without verifying its descendants.

---

## 9. Interview Questions

**Q1. What is the difference between Same Tree and Subtree of Another Tree?**

Same Tree compares two trees from their roots. Subtree searches for an identical tree at any node in another tree.

**Q2. Why do we reuse `isSameTree()`?**

Because once we find a possible matching root node, we need to verify that the complete subtree has the same structure and values.

**Q3. Why do we use OR in the recursive calls?**

Because the target subtree can exist in either the left or right subtree.

**Q4. What happens if `subRoot` is null?**

Return `true`.

**Q5. What is the worst-case time complexity?**

O(n × m), because we may compare the target tree against a subtree rooted at every node of the main tree.

---

## 10. Pattern Recognition

When a binary tree problem asks you to:
- Find one tree inside another tree.
- Compare matching structures.
- Search every possible starting node.
- Reuse a tree-comparison operation.

Think about:

**DFS + Tree Matching**

Related problems:
- Same Tree
- Invert Binary Tree
- Symmetric Tree
- Binary Tree Maximum Path Sum

---

## 11. 30-Second Interview Revision

- **Pattern:** Binary Tree DFS + Same Tree.
- First, check whether the current subtree matches `subRoot`.
- If it matches, return `true`.
- Otherwise, search left and right.
- Use `||` because either side can contain the target.
- Reuse `isSameTree()` from Problem 49.
- **Time:** O(n × m) worst case.
- **Space:** O(h + h₂).

**Remember:**

Same Tree = Compare two trees.

Subtree = Search the main tree and repeatedly apply Same Tree comparison.



# 51. Lowest Common Ancestor of a Binary Search Tree

## 1. Problem Understanding

Given the root of a Binary Search Tree (BST) and two nodes `p` and `q`, find their Lowest Common Ancestor.

The **Lowest Common Ancestor (LCA)** is the lowest node in the tree that has both `p` and `q` as descendants. A node can be considered a descendant of itself.

### Example 1

**Input:**
```text
        6
       / \
      2   8
     / \ / \
    0  4 7  9
      / \
     3   5

p = 2
q = 8
```

**Output:**
```text
6
```

Why?

- Node `2` is on the left side of `6`.
- Node `8` is on the right side of `6`.
- Therefore, `6` is their lowest common ancestor.

### Example 2

**Input:**
```text
        6
       /
      2
     / \
    0   4
       / \
      3   5

p = 2
q = 4
```

**Output:**
```text
2
```

Why?

Node `2` itself is one of the given nodes, and it is an ancestor of node `4`.

---

## 2. First Understand the BST Property

A Binary Search Tree follows this rule:

```text
        6
       / \
      2   8
     / \ / \
    0  4 7  9
      / \
     3   5
```

For every node:

- Left subtree contains smaller values.
- Right subtree contains greater values.

Mathematically:

\[
Left < Root < Right
\]

This property makes the LCA problem easier because we don't need to search every node.

---

## 3. Intuition — Think Like This

Suppose:

```text
        6
       / \
      2   8
     / \
    0   4
```

We want LCA of:

```cpp
p = 2;
q = 4;
```

Start at root `6`.

### Case 1: Both nodes are smaller than current node

```cpp
p->val < root->val &&
q->val < root->val
```

Both nodes must be in the left subtree.

Move left.

### Case 2: Both nodes are greater than current node

```cpp
p->val > root->val &&
q->val > root->val
```

Both nodes must be in the right subtree.

Move right.

### Case 3: Nodes are on different sides

One node is smaller and the other is greater than the current node.

Or the current node is itself `p` or `q`.

The current node is the LCA.

**This is called the split point.**

---

## 4. C++ Solution — Recursive DFS

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(
        TreeNode* root,
        TreeNode* p,
        TreeNode* q
    ) {

        // Both nodes are in the left subtree
        if (p->val < root->val && q->val < root->val) {
            return lowestCommonAncestor(root->left, p, q);
        }

        // Both nodes are in the right subtree
        if (p->val > root->val && q->val > root->val) {
            return lowestCommonAncestor(root->right, p, q);
        }

        // Split point: current node is the LCA
        return root;
    }
};
```

---

## 5. Understand the Code Step by Step

### Step 1: Check Whether Both Nodes Are Smaller

```cpp
if (p->val < root->val && q->val < root->val)
```

If both values are smaller than the current root, the LCA must be somewhere in the left subtree.

```cpp
return lowestCommonAncestor(root->left, p, q);
```

### Step 2: Check Whether Both Nodes Are Greater

```cpp
if (p->val > root->val && q->val > root->val)
```

If both values are greater, the LCA must be somewhere in the right subtree.

```cpp
return lowestCommonAncestor(root->right, p, q);
```

### Step 3: Otherwise, Return Current Node

```cpp
return root;
```

This happens when:

- `p` is on the left and `q` is on the right.
- `p` is on the right and `q` is on the left.
- `root` is equal to `p`.
- `root` is equal to `q`.

In all these cases, the current node is the LCA.

---

## 6. Dry Run Step by Step

Consider:

```text
        6
       / \
      2   8
     / \ / \
    0  4 7  9
      / \
     3   5

p = 2
q = 8
```

| Step | Current root | p | q | Decision |
|---|---:|---:|---:|---|
| 1 | 6 | 2 | 8 | Different sides |
| 2 | 6 | — | — | Return 6 |

Final answer:

```cpp
return root;  // Node 6
```

### Another Dry Run

```text
        6
       / \
      2   8
     / \
    0   4
       / \
      3   5

p = 2
q = 4
```

| Step | Current root | p | q | Decision |
|---|---:|---:|---:|---|
| 1 | 6 | 2 | 4 | Both smaller → go left |
| 2 | 2 | 2 | 4 | Current node equals p → return 2 |

Final answer: `2`.

---

## 7. Iterative Approach — Without Recursion

We can also solve this problem using a loop.

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(
        TreeNode* root,
        TreeNode* p,
        TreeNode* q
    ) {

        while (root != nullptr) {

            if (p->val < root->val && q->val < root->val) {
                root = root->left;
            }
            else if (p->val > root->val && q->val > root->val) {
                root = root->right;
            }
            else {
                return root;
            }
        }

        return nullptr;
    }
};
```

The logic is exactly the same.

The difference:

- Recursive approach uses function calls.
- Iterative approach uses a `while` loop.

For interviews, both are good. The iterative version uses O(1) auxiliary space.

---

## 8. Complexity Analysis

Let `h` be the height of the BST.

| Complexity | Recursive | Iterative |
|---|---|---|
| Time | O(h) | O(h) |
| Auxiliary space | O(h) | O(1) |

- Balanced BST: O(log n) time.
- Skewed BST: O(n) time.
- Recursive stack: O(h).
- Iterative approach: O(1) auxiliary space.

---

## 9. Common Mistakes

1. Treating this as a normal binary tree problem and ignoring the BST property.
2. Searching both subtrees unnecessarily.
3. Forgetting that a node can be its own ancestor.
4. Returning a node when both target values are on the same side instead of continuing the search.
5. Confusing LCA with the root node. The LCA can be anywhere in the tree.

---

## 10. Interview Questions

**Q1. What is the Lowest Common Ancestor?**

The lowest node that has both given nodes as descendants, allowing a node to be its own descendant.

**Q2. Why is BST property useful here?**

It tells us which subtree can contain both nodes, allowing us to discard the other subtree.

**Q3. When do we return the current root?**

When the nodes lie on different sides or the current root equals one of the target nodes.

**Q4. What is the time complexity?**

O(h), where h is the height of the BST.

**Q5. Can we solve it without recursion?**

Yes, using an iterative `while` loop with O(1) auxiliary space.

**Q6. Is this solution valid for a normal binary tree?**

No. It relies on the BST ordering property. A normal binary tree needs a different algorithm.

---

## 11. Pattern Recognition

When a problem mentions:

- Binary Search Tree.
- Finding an ancestor.
- Two target nodes.
- Searching based on smaller or greater values.

Think about:

**BST Property + DFS**

Related problems:
- Search in a Binary Search Tree
- Insert into a Binary Search Tree
- Validate Binary Search Tree
- Lowest Common Ancestor of a Binary Tree

---

## 12. 30-Second Interview Revision

- **Pattern:** BST + DFS.
- Both values smaller → go left.
- Both values greater → go right.
- Otherwise → current root is LCA.
- **Recursive time:** O(h).
- **Recursive space:** O(h).
- **Iterative space:** O(1).

**Remember:**

The LCA is the first node where the two target nodes split into different directions, or where one target is the current node.



# 52. Binary Tree Level Order Traversal

## 1. Problem Understanding

Given the root of a binary tree, return its level-order traversal.

Level-order traversal means visiting nodes **from left to right, level by level**.

### Example

**Input:**
```text
        3
       / \
      9   20
          / \
         15  7
```

**Output:**
```cpp
{{3}, {9, 20}, {15, 7}}
```

Notice:
- Level 1: `[3]`
- Level 2: `[9, 20]`
- Level 3: `[15, 7]`

We need to return a 2D vector where each inner vector represents one level.

---

## 2. Intuition — Think Like This

Imagine visiting a tree level by level.

Instead of going deep into the left subtree first, we process all nodes at the current level before moving down.

For this, we use a **Queue (FIFO)**.

FIFO means:

**First In, First Out.**

The first node inserted into the queue is the first node removed.

### Visual understanding

<escape>
        3              Level 1
       / \
      9   20           Level 2
          / \
         15  7          Level 3
</escape>

The queue helps us remember which nodes need to be processed next.

---

## 3. C++ Solution — BFS Using Queue

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {

        vector<vector<int>> result;

        // Handle empty tree
        if (root == nullptr) {
            return result;
        }

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {

            // Number of nodes at current level
            int size = q.size();

            vector<int> level;

            // Process all nodes of this level
            for (int i = 0; i < size; i++) {

                TreeNode* node = q.front();
                q.pop();

                level.push_back(node->val);

                // Add left child
                if (node->left != nullptr) {
                    q.push(node->left);
                }

                // Add right child
                if (node->right != nullptr) {
                    q.push(node->right);
                }
            }

            // Store current level
            result.push_back(level);
        }

        return result;
    }
};
```

---

## 4. Understand the Code Step by Step

### Step 1: Create the Result

```cpp
vector<vector<int>> result;
```

Why 2D vector?

Because we need to store multiple levels.

Example:
```cpp
{
    {3},
    {9, 20},
    {15, 7}
}
```

### Step 2: Handle Empty Tree

```cpp
if (root == nullptr) {
    return result;
}
```

If the tree is empty, return an empty 2D vector.

### Step 3: Create a Queue

```cpp
queue<TreeNode*> q;
q.push(root);
```

We insert the root node first.

Initially:

```text
Queue: [3]
```

### Step 4: Process While Queue Is Not Empty

```cpp
while (!q.empty())
```

Continue until every node has been processed.

### Step 5: Capture Current Level Size

```cpp
int size = q.size();
```

This is the most important line in this problem.

It tells us how many nodes belong to the current level.

We save the size before processing because processing nodes adds their children to the queue.

**Why is this important?**

We must not accidentally process the next level while collecting the current level.

### Step 6: Process All Nodes at Current Level

```cpp
for (int i = 0; i < size; i++)
```

Run exactly `size` times.

For every node:

```cpp
TreeNode* node = q.front();
q.pop();

level.push_back(node->val);
```

- Get the front node.
- Remove it from the queue.
- Add its value to the current level.

### Step 7: Add Children to Queue

```cpp
if (node->left != nullptr) {
    q.push(node->left);
}

if (node->right != nullptr) {
    q.push(node->right);
}
```

Insert the left child first and then the right child.

This ensures left-to-right traversal.

### Step 8: Store the Level

```cpp
result.push_back(level);
```

Once all nodes at the current level have been processed, store that level in the final result.

---

## 5. Complete Dry Run

Input:

```text
        3
       / \
      9   20
          / \
         15  7
```

### Initial State

```text
Queue: [3]
Result: []
```

### Iteration 1 — Level 1

Current queue size:

```cpp
size = 1;
```

Process node `3`.

- Add `3` to level.
- Add children `9` and `20` to queue.

```text
Queue: [9, 20]
Level: [3]
Result: [[3]]
```

### Iteration 2 — Level 2

Current queue size:

```cpp
size = 2;
```

Process node `9`:
- Add `9` to level.
- No children.

Process node `20`:
- Add `20` to level.
- Add children `15` and `7`.

```text
Queue: [15, 7]
Level: [9, 20]
Result: [[3], [9, 20]]
```

### Iteration 3 — Level 3

Current queue size:

```cpp
size = 2;
```

Process node `15`:
- Add `15` to level.

Process node `7`:
- Add `7` to level.

```text
Queue: []
Level: [15, 7]
Result: [[3], [9, 20], [15, 7]]
```

Queue is empty, so the loop terminates.

**Final Answer:**

```cpp
{{3}, {9, 20}, {15, 7}}
```

---

## 6. Why Do We Store `int size = q.size()`?

This is the most important interview concept in level-order traversal.

Suppose:

```text
        3
       / \
      9   20
```

Initially:

```text
Queue = [3]
```

We save:

```cpp
int size = q.size(); // 1
```

After processing `3`, we insert `9` and `20`.

Now:

```text
Queue = [9, 20]
```

But the current level must contain only `[3]`.

The saved `size = 1` ensures that we process exactly one node in this iteration.

The next iteration processes `[9, 20]`.

**Remember:** Always capture the queue size before processing a level.

---

## 7. Complexity Analysis

Let `n` be the total number of nodes and `w` be the maximum width of the tree.

| Complexity | Explanation |
|---|---|
| Time | O(n) |
| Auxiliary space | O(w) for the queue |
| Output space | O(n) |

- **Time O(n):** Every node enters and leaves the queue exactly once.
- **Queue space O(w):** At most one level's nodes are waiting in the queue.
- **Output space O(n):** We store every node's value in the result.

---

## 8. DFS vs BFS

This is an important difference to understand.

| DFS | BFS |
|---|---|
| Depth First Search | Breadth First Search |
| Goes deep into a branch | Visits level by level |
| Uses recursion or stack | Usually uses queue |
| Useful for height, diameter and path problems | Useful for level-order traversal and shortest paths in unweighted graphs |

Examples from our previous problems:

| Problem | Pattern |
|---|---|
| #45 Invert Binary Tree | DFS |
| #46 Maximum Depth | DFS |
| #47 Diameter | DFS |
| #48 Balanced Binary Tree | DFS |
| #49 Same Tree | DFS |
| #50 Subtree of Another Tree | DFS |
| #51 LCA of BST | BST + DFS |
| #52 Level Order Traversal | BFS |

---

## 9. Common Mistakes

1. Forgetting to check whether `root` is null.
2. Using a stack instead of a queue.
3. Forgetting to capture `q.size()` before processing a level.
4. Adding children in the wrong order when left-to-right traversal is required.
5. Creating a new `level` vector outside the while loop, causing values from different levels to mix.
6. Forgetting to push the completed level into `result`.

---

## 10. Interview Questions

**Q1. Why do we use a queue instead of a stack?**

A queue follows FIFO order, allowing us to process nodes level by level.

**Q2. What does `q.size()` represent?**

When captured at the beginning of an iteration, it represents the number of nodes at the current level.

**Q3. Why do we use a 2D vector?**

Because each level needs its own separate collection of node values.

**Q4. What is BFS?**

Breadth-First Search is a traversal technique that explores nodes level by level.

**Q5. What is the time complexity?**

O(n), because every node is processed once.

**Q6. Can we solve this using DFS?**

Yes, by passing the current level as an argument and storing values in the corresponding result vector. However, BFS with a queue is the most natural approach here.

---

## 11. Pattern Recognition

When a binary tree problem asks you to:
- Traverse nodes level by level.
- Group nodes according to their depth.
- Process all nodes at one level before moving to the next.

Think about:

**BFS + Queue + Level Size**

Related problems:
- Binary Tree Zigzag Level Order Traversal
- Binary Tree Right Side View
- Average of Levels in Binary Tree
- Minimum Depth of Binary Tree
- Binary Tree Level Order Traversal II

---

## 12. 30-Second Interview Revision

- **Pattern:** BFS.
- **Data structure:** `queue<TreeNode*>`.
- Insert root into queue.
- Capture `int size = q.size()`.
- Process exactly `size` nodes.
- Add their children to the queue.
- Store the current level in the result.
- Repeat until queue is empty.
- **Time:** O(n).
- **Queue space:** O(w).

**Remember these three lines:**

```cpp
int size = q.size();

for (int i = 0; i < size; i++) {
    // Process current level
}

result.push_back(level);
```

These lines form the foundation of most binary-tree level-order traversal problems.



# 53. Binary Tree Right Side View

## 1. Problem Understanding

Given the root of a binary tree, return the values of the nodes that are visible when looking at the tree from the right side.

In simple words:

**Return the rightmost node at every level.**

### Example

**Input:**
```text
        1
       / \
      2   3
       \   \
        5   4
```

**Output:**
```cpp
{1, 3, 4}
```

Why?

- Level 1: `1`
- Level 2: `3` (rightmost)
- Level 3: `4` (rightmost)

### Visual Understanding

Imagine standing on the right side of this tree:

```text
        1  <--- Visible
       / \
      2   3  <--- Visible
       \   \
        5   4  <--- Visible
```

The nodes visible from the right are:

```text
1
3
4
```

---

## 2. Intuition — Think Like This

Remember Problem 52: Binary Tree Level Order Traversal.

There, we collected every node level by level.

Here, the logic is almost the same.

The only difference:

**Instead of storing every node at a level, store only the last node.**

We use:
- Queue for BFS.
- `size` to identify the number of nodes at the current level.
- `i == size - 1` to identify the last node.

---

## 3. C++ Solution — BFS Using Queue

```cpp
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {

        vector<int> result;

        if (root == nullptr) {
            return result;
        }

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {

            int size = q.size();

            for (int i = 0; i < size; i++) {

                TreeNode* node = q.front();
                q.pop();

                // Last node of current level
                if (i == size - 1) {
                    result.push_back(node->val);
                }

                // Add left child
                if (node->left != nullptr) {
                    q.push(node->left);
                }

                // Add right child
                if (node->right != nullptr) {
                    q.push(node->right);
                }
            }
        }

        return result;
    }
};
```

---

## 4. Understand the Code Step by Step

### Step 1: Create Result Vector

```cpp
vector<int> result;
```

We only need one integer per level.

### Step 2: Handle Empty Tree

```cpp
if (root == nullptr) {
    return result;
}
```

An empty tree has no visible nodes.

### Step 3: Create Queue

```cpp
queue<TreeNode*> q;
q.push(root);
```

Insert the root node.

### Step 4: Capture Current Level Size

```cpp
int size = q.size();
```

This tells us how many nodes belong to the current level.

Remember this from Problem 52.

### Step 5: Process Current Level

```cpp
for (int i = 0; i < size; i++)
```

Process exactly the nodes belonging to this level.

### Step 6: Select the Last Node

```cpp
if (i == size - 1) {
    result.push_back(node->val);
}
```

This is the most important condition.

For example, if the current level contains:

```text
[2, 3, 4]
```

Then:

```text
size = 3

i = 0 → Node 2
i = 1 → Node 3
i = 2 → Node 4  ← Last node
```

We store only `4`.

### Step 7: Add Children

```cpp
if (node->left != nullptr) {
    q.push(node->left);
}

if (node->right != nullptr) {
    q.push(node->right);
}
```

Add children to the queue so the next level can be processed.

---

## 5. Complete Dry Run

Input:

```text
        1
       / \
      2   3
       \   \
        5   4
```

### Initial State

```text
Queue: [1]
Result: []
```

### Iteration 1 — Level 1

```text
size = 1

i = 0 → Node 1
```

Since `i == size - 1`, store `1`.

Add children `2` and `3`.

```text
Queue: [2, 3]
Result: [1]
```

### Iteration 2 — Level 2

```text
size = 2
```

Process node `2`:

```text
i = 0
```

Not the last node. Don't store it.

Add child `5`.

Process node `3`:

```text
i = 1
```

Since `1 == 2 - 1`, store `3`.

Add child `4`.

```text
Queue: [5, 4]
Result: [1, 3]
```

### Iteration 3 — Level 3

```text
size = 2
```

Process node `5`:
- `i = 0`
- Don't store.

Process node `4`:
- `i = 1`
- Store `4`.

```text
Queue: []
Result: [1, 3, 4]
```

Final Answer:

```cpp
{1, 3, 4}
```

---

## 6. Important Edge Case — Missing Right Child

Consider:

```text
        1
       /
      2
     /
    3
```

What is visible from the right side?

```text
1
2
3
```

Output:

```cpp
{1, 2, 3}
```

**Important:** The right-side view does not mean we always choose the actual right child.

It means we choose the rightmost node that exists at each level.

Even if a node has no right child, its left child might still be visible.

---

## 7. Alternative Approach — DFS

We can also solve this using DFS.

The idea is to visit the right subtree before the left subtree.

The first node visited at each depth will be the rightmost visible node.

```cpp
class Solution {
public:
    vector<int> result;

    void dfs(TreeNode* root, int depth) {

        if (root == nullptr) {
            return;
        }

        // First node visited at this depth
        if (depth == result.size()) {
            result.push_back(root->val);
        }

        // Visit right side first
        dfs(root->right, depth + 1);

        // Then visit left side
        dfs(root->left, depth + 1);
    }

    vector<int> rightSideView(TreeNode* root) {

        result.clear();
        dfs(root, 0);

        return result;
    }
};
```

### Why does DFS work?

Because we visit the right subtree first.

For every level, the first node encountered is the rightmost available node.

| Approach | Technique | Auxiliary space |
|---|---|---|
| BFS | Queue | O(w) |
| DFS | Recursion | O(h) |

Both take O(n) time.

For interviews, BFS is especially easy to explain because it directly follows the level-order traversal pattern.

---

## 8. Complexity Analysis

Let:
- `n` = number of nodes.
- `w` = maximum width of the tree.
- `h` = height of the tree.

| Complexity | BFS |
|---|---|
| Time | O(n) |
| Auxiliary space | O(w) |
| Output space | O(h) |

Every node is processed exactly once.

---

## 9. Common Mistakes

1. Always selecting `node->right` instead of the rightmost node at each level.
2. Forgetting that a left child can be visible when the right child is missing.
3. Using `i == size` instead of `i == size - 1`.
4. Forgetting to capture the queue size before processing a level.
5. Processing the entire queue without separating levels.

---

## 10. Interview Questions

**Q1. What is the right-side view of a binary tree?**

The nodes visible when looking at the tree from the right side, which means the rightmost node at each level.

**Q2. Why do we use BFS?**

Because BFS naturally processes nodes level by level.

**Q3. Why do we check `i == size - 1`?**

To identify the last node of the current level.

**Q4. Can DFS solve this problem?**

Yes. Visit the right subtree before the left subtree and store the first node encountered at each depth.

**Q5. What if the right child is missing?**

The rightmost existing node at that level is still visible, even if it is a left child.

---

## 11. Pattern Recognition

When a binary tree problem asks you to:
- Find visible nodes from a particular side.
- Select one node from every level.
- Return the leftmost or rightmost node.

Think about:

**BFS + Level Order Traversal + Position Selection**

Related problems:
- Binary Tree Level Order Traversal
- Binary Tree Zigzag Level Order Traversal
- Average of Levels in Binary Tree
- Find Largest Value in Each Tree Row

---

## 12. 30-Second Interview Revision

- **Pattern:** BFS.
- **Data structure:** Queue.
- Capture `size = q.size()`.
- Process all nodes at the current level.
- Store only when `i == size - 1`.
- Add children for the next level.
- **Time:** O(n).
- **Auxiliary space:** O(w).

**Remember this line:**

```cpp
if (i == size - 1) {
    result.push_back(node->val);
}
```

Problem 52 stores every node at each level. Problem 53 stores only the last node at each level.



# 54. Count Good Nodes in Binary Tree

## 1. Problem Understanding

Given the root of a binary tree, count the number of good nodes.

A node `X` is called a **good node** if, on the path from the root to `X`, there are no nodes with a value greater than `X`.

In simple words:

A node is good if its value is greater than or equal to the maximum value encountered before reaching it.

### Example

**Input:**
```text
        3
       / \
      1   4
     /   / \
    3   1   5
```

**Output:**
```text
4
```

Let's identify the good nodes:

| Node | Path from root | Good? | Reason |
|---|---|---|---|
| 3 | 3 | Yes | Root is always good |
| 1 | 3 → 1 | No | 3 is greater |
| 4 | 3 → 4 | Yes | 4 is the new maximum |
| 3 | 3 → 1 → 3 | Yes | Equal to maximum 3 |
| 1 | 3 → 4 → 1 | No | 4 is greater |
| 5 | 3 → 4 → 5 | Yes | 5 is the new maximum |

Good nodes are:

```text
3, 4, 3, 5
```

Answer = `4`.

---

## 2. Intuition — Think Like This

Imagine walking from the root to every node.

While walking, keep track of the **maximum value seen so far**.

At each node:

1. Compare its value with the maximum value seen so far.
2. If its value is greater than or equal to that maximum, count it as good.
3. Update the maximum if necessary.
4. Continue to its left and right children.

The important thing is that each node needs information about its ancestors.

We can pass that information through recursion.

---

## 3. C++ Solution — DFS Recursion

```cpp
class Solution {
public:
    int dfs(TreeNode* root, int maxVal) {

        if (root == nullptr) {
            return 0;
        }

        int count = 0;

        // Check whether current node is good
        if (root->val >= maxVal) {
            count = 1;
        }

        // Update maximum value along the path
        maxVal = max(maxVal, root->val);

        // Explore left and right subtrees
        count += dfs(root->left, maxVal);
        count += dfs(root->right, maxVal);

        return count;
    }

    int goodNodes(TreeNode* root) {

        if (root == nullptr) {
            return 0;
        }

        return dfs(root, root->val);
    }
};
```

---

## 4. Understand the Code Step by Step

### Step 1: Create DFS Function

```cpp
int dfs(TreeNode* root, int maxVal)
```

We pass two things:

- `root`: Current node.
- `maxVal`: Maximum value encountered on the path from the original root to the current node.

This is the key concept in this problem.

### Step 2: Base Case

```cpp
if (root == nullptr) {
    return 0;
}
```

If there is no node, there is nothing to count.

### Step 3: Check Whether Current Node Is Good

```cpp
if (root->val >= maxVal) {
    count = 1;
}
```

If the current node value is greater than or equal to the maximum value seen so far, it is a good node.

Notice the use of `>=`, not just `>`.

Equal values also qualify.

### Step 4: Update Maximum Value

```cpp
maxVal = max(maxVal, root->val);
```

Update the maximum before exploring the children.

This ensures that the children know the maximum value encountered along their path.

### Step 5: Explore Both Subtrees

```cpp
count += dfs(root->left, maxVal);
count += dfs(root->right, maxVal);
```

Recursively count good nodes in the left and right subtrees.

Add both results to the current count.

### Step 6: Return the Answer

```cpp
return count;
```

The helper returns the total number of good nodes in the current subtree.

---

## 5. Complete Dry Run

Consider:

```text
        3
       / \
      1   4
     /   / \
    3   1   5
```

We start with:

```cpp
dfs(root, root->val);
```

Therefore:

```text
maxVal = 3
```

### Step-by-step execution

| Current node | Maximum before checking | Good node? | Updated maximum |
|---|---:|---|---:|
| 3 | 3 | Yes | 3 |
| 1 | 3 | No | 3 |
| 3 | 3 | Yes | 3 |
| 4 | 3 | Yes | 4 |
| 1 | 4 | No | 4 |
| 5 | 4 | Yes | 5 |

Total good nodes:

```text
3 + 3 + 4 + 5
```

Count = `4`.

### Visualize the DFS

```text
                  3 (Good)
                 / \
          1 (Not Good)  4 (Good)
              /          / \
        3 (Good)   1 (Not Good)  5 (Good)
```

Final answer:

```cpp
return 4;
```

---

## 6. Important Concept — Why Pass Maximum Value?

Suppose:

```text
        5
       /
      3
     /
    4
```

At node `4`, the path is:

```text
5 → 3 → 4
```

Although `4` is greater than its parent `3`, it is not a good node because an ancestor (`5`) is greater.

Therefore, we must track the maximum value from the entire root-to-current-node path, not just the parent.

This is why we pass `maxVal` through recursion.

---

## 7. Important C++ Concept — Pass by Value

```cpp
dfs(root->left, maxVal);
dfs(root->right, maxVal);
```

Here, `maxVal` is passed by value.

Each recursive call gets its own copy.

This is useful because the maximum value on one branch should not incorrectly affect the other branch.

For example:

```text
        3
       / \
      1   4
     /     \
    2       5
```

- Left branch maximum evolves: `3 → 3 → 3`.
- Right branch maximum evolves: `3 → 4 → 5`.

Each path maintains its own maximum.

---

## 8. Complexity Analysis

Let `n` be the total number of nodes and `h` be the height of the tree.

| Complexity | Explanation |
|---|---|
| Time | O(n) |
| Auxiliary space | O(h) |

- **Time O(n):** Every node is visited exactly once.
- **Space O(h):** Recursive call stack depends on the tree height.
  - Balanced tree: O(log n)
  - Skewed tree: O(n)

---

## 9. Common Mistakes

1. Comparing only with the parent instead of the maximum value along the entire path.
2. Using `>` instead of `>=`.
3. Forgetting to update `maxVal` before exploring children.
4. Using one shared maximum across all branches instead of passing it separately.
5. Forgetting to add the results from both left and right subtrees.
6. Forgetting the empty-tree case.

---

## 10. Interview Questions

**Q1. What makes a node good?**

Its value must be greater than or equal to every value on the path from the root to that node.

**Q2. Why do we pass `maxVal`?**

To remember the largest value encountered along the current root-to-node path.

**Q3. Why do we use `>=`?**

Because a node equal to the maximum value seen so far is also considered good.

**Q4. Why is `maxVal` passed by value?**

Each branch needs its own path-specific maximum. Changes in one branch should not affect another.

**Q5. What is the time complexity?**

O(n), because every node is visited exactly once.

---

## 11. Pattern Recognition

When a binary tree problem asks you to:
- Track information from the root to the current node.
- Compare a node with its ancestors.
- Maintain a maximum or minimum along a path.
- Count or validate nodes based on path information.

Think about:

**DFS + Path Tracking**

Related problems:
- Path Sum
- Path Sum II
- Binary Tree Maximum Path Sum
- Validate Binary Search Tree

---

## 12. 30-Second Interview Revision

- **Pattern:** DFS + Path Tracking.
- **Extra parameter:** `maxVal`.
- Check: `root->val >= maxVal`.
- Update: `maxVal = max(maxVal, root->val)`.
- Explore left and right subtrees.
- Add their counts.
- **Time:** O(n).
- **Space:** O(h).

**Remember:** A good node is compared against the maximum value on its entire path from the root, not just its parent.



# 55. Validate Binary Search Tree

## 1. Problem Understanding

Given the root of a binary tree, determine whether it is a valid Binary Search Tree (BST).

A valid BST follows these rules:

1. Every node in the left subtree must have a value smaller than the current node.
2. Every node in the right subtree must have a value greater than the current node.
3. Both left and right subtrees must also be valid BSTs.

### Example 1

**Input:**
```text
        2
       / \
      1   3
```

**Output:**
```cpp
true
```

This is a valid BST because:

- `1 < 2`
- `3 > 2`

### Example 2

**Input:**
```text
        5
       / \
      1   4
         / \
        3   6
```

**Output:**
```cpp
false
```

Why?

Node `3` is smaller than `4`, which looks correct locally.

But node `3` is inside the right subtree of `5`.

Therefore, it must be greater than `5`.

Since `3 < 5`, the tree is invalid.

---

## 2. Intuition — Think Like This

A common mistake is checking only the immediate children.

For example:

```text
        5
       / \
      3   8
         /
        4
```

At first glance:

- `3 < 5` — correct.
- `8 > 5` — correct.
- `4 < 8` — correct.

But the tree is invalid because `4` is in the right subtree of `5`, so it must be greater than `5`.

**The important observation: Every node must satisfy all ancestor restrictions, not just its parent's restriction.**

We solve this by passing a minimum and maximum allowed value to each node.

---

## 3. C++ Solution — DFS with Range Validation

```cpp
class Solution {
public:

    bool validate(TreeNode* root, long long minVal, long long maxVal) {

        // Empty subtree is valid
        if (root == nullptr) {
            return true;
        }

        // Current value must be within the allowed range
        if (root->val <= minVal || root->val >= maxVal) {
            return false;
        }

        // Validate left and right subtrees
        return validate(root->left, minVal, root->val) &&
               validate(root->right, root->val, maxVal);
    }

    bool isValidBST(TreeNode* root) {

        return validate(root, LLONG_MIN, LLONG_MAX);
    }
};
```

---

## 4. Understand the Code Step by Step

### Step 1: Create the Helper Function

```cpp
bool validate(TreeNode* root, long long minVal, long long maxVal)
```

We pass three things:

- `root`: Current node.
- `minVal`: Minimum allowed value.
- `maxVal`: Maximum allowed value.

Every node must satisfy:

\[
minVal < root->val < maxVal
\]

### Step 2: Base Case

```cpp
if (root == nullptr) {
    return true;
}
```

An empty subtree is always a valid BST.

### Step 3: Check the Current Node

```cpp
if (root->val <= minVal || root->val >= maxVal) {
    return false;
}
```

If the current value is outside its allowed range, the BST is invalid.

Notice that we use strict inequalities.

Duplicate values are not allowed in a valid BST.

### Step 4: Validate the Left Subtree

```cpp
validate(root->left, minVal, root->val)
```

Every node in the left subtree must be:

- Greater than the inherited minimum.
- Smaller than the current root value.

Therefore, the allowed range becomes:

\[
(minVal, root->val)
\]

### Step 5: Validate the Right Subtree

```cpp
validate(root->right, root->val, maxVal)
```

Every node in the right subtree must be:

- Greater than the current root value.
- Smaller than the inherited maximum.

Therefore, the allowed range becomes:

\[
(root->val, maxVal)
\]

### Step 6: Both Subtrees Must Be Valid

```cpp
return validate(root->left, minVal, root->val) &&
       validate(root->right, root->val, maxVal);
```

We use `&&` because both subtrees must satisfy BST rules.

---

## 5. Dry Run Step by Step

Consider this valid BST:

```text
        5
       / \
      3   8
     / \ / \
    2  4 7  9
```

Initially:

```cpp
validate(root, LLONG_MIN, LLONG_MAX);
```

The allowed range is initially unlimited.

| Node | Minimum | Maximum | Valid? |
|---|---:|---:|---|
| 5 | -∞ | +∞ | Yes |
| 3 | -∞ | 5 | Yes |
| 2 | -∞ | 3 | Yes |
| 4 | 3 | 5 | Yes |
| 8 | 5 | +∞ | Yes |
| 7 | 5 | 8 | Yes |
| 9 | 8 | +∞ | Yes |

Every node satisfies its allowed range.

**Final answer: `true`**

### Now consider an invalid BST

```text
        5
       / \
      1   8
         /
        4
```

Let's trace the important nodes:

| Node | Minimum | Maximum | Valid? |
|---|---:|---:|---|
| 5 | -∞ | +∞ | Yes |
| 1 | -∞ | 5 | Yes |
| 8 | 5 | +∞ | Yes |
| 4 | 5 | 8 | No |

At node `4`:

\[
5 < 4 < 8
\]

This condition is false.

Therefore, the entire tree is invalid.

**Final answer: `false`**

---

## 6. Why Do We Use LLONG_MIN and LLONG_MAX?

```cpp
return validate(root, LLONG_MIN, LLONG_MAX);
```

Initially, we don't have any restrictions on the root node.

So we use the largest possible range for a `long long` integer.

```cpp
LLONG_MIN
LLONG_MAX
```

Why not simply use `INT_MIN` and `INT_MAX`?

Because a node could contain the value `INT_MIN` or `INT_MAX`.

Since the comparison is strict, using those same values as initial boundaries would incorrectly reject such a node.

Using `long long` boundaries safely handles the full range of C++ `int` values.

---

## 7. Alternative Approach — Inorder Traversal

There is another useful way to validate a BST.

**Important property: Inorder traversal of a valid BST produces values in strictly increasing order.**

Inorder traversal follows:

```text
Left → Root → Right
```

For example:

```text
        5
       / \
      3   8
     / \ / \
    2  4 7  9
```

Inorder result:

```text
2 → 3 → 4 → 5 → 7 → 8 → 9
```

The values are strictly increasing.

### C++ Code

```cpp
class Solution {
public:
    TreeNode* prev = nullptr;

    bool isValidBST(TreeNode* root) {

        if (root == nullptr) {
            return true;
        }

        // Check left subtree
        if (!isValidBST(root->left)) {
            return false;
        }

        // Current value must be greater than previous
        if (prev != nullptr && root->val <= prev->val) {
            return false;
        }

        prev = root;

        // Check right subtree
        return isValidBST(root->right);
    }
};
```

This approach maintains the previously visited node during inorder traversal.

**Note:** If using a class member like `prev`, reset it before starting a new traversal if the same object can be reused for multiple calls. The range-validation solution avoids this issue.

---

## 8. Complexity Analysis

Let `n` be the total number of nodes and `h` be the tree height.

| Complexity | Range DFS |
|---|---|
| Time | O(n) |
| Auxiliary space | O(h) |

- **Time O(n):** Every node is visited at most once.
- **Space O(h):** Recursive call stack depends on tree height.
  - Balanced tree: O(log n)
  - Skewed tree: O(n)

The inorder solution also takes O(n) time and O(h) auxiliary space.

---

## 9. Common Mistakes

1. Checking only the immediate left and right children.
2. Forgetting to pass ancestor restrictions to descendants.
3. Using `<=` or `>=` incorrectly in the range check.
4. Allowing duplicate values.
5. Using `INT_MIN` and `INT_MAX` as strict initial boundaries.
6. Forgetting that both subtrees must be valid.

---

## 10. Interview Questions

**Q1. Why isn't checking only the immediate children enough?**

Because a node must satisfy restrictions imposed by every ancestor, not just its parent.

**Q2. Why do we use minimum and maximum boundaries?**

To carry all ancestor restrictions down through the tree.

**Q3. Are duplicate values allowed in a valid BST?**

No. The condition is strictly increasing or decreasing, so duplicate values are invalid.

**Q4. Why do we use `long long` boundaries?**

To safely handle nodes containing `INT_MIN` or `INT_MAX`.

**Q5. How can inorder traversal validate a BST?**

A valid BST produces a strictly increasing sequence during inorder traversal.

**Q6. What is the time complexity?**

O(n), because each node is visited at most once.

---

## 11. Pattern Recognition

When a BST problem asks you to:
- Validate whether a tree follows BST rules.
- Check ancestor restrictions.
- Maintain a minimum and maximum allowed range.
- Ensure all nodes satisfy ordering constraints.

Think about:

**DFS + Range Validation**

Related problems:
- Search in a Binary Search Tree
- Insert into a Binary Search Tree
- Lowest Common Ancestor of a BST
- Kth Smallest Element in a BST

---

## 12. 30-Second Interview Revision

- **Pattern:** DFS + Range Validation.
- Initial range: `LLONG_MIN` to `LLONG_MAX`.
- Every node must satisfy: `minVal < root->val < maxVal`.
- Left subtree: `(minVal, root->val)`.
- Right subtree: `(root->val, maxVal)`.
- Both subtrees must be valid (`&&`).
- **Time:** O(n).
- **Space:** O(h).

**Remember:** A valid BST is not just about parent-child comparisons. Every node must satisfy the restrictions of all its ancestors.



# 56. Kth Smallest Element in a BST

## 1. Problem Understanding

Given the root of a Binary Search Tree (BST) and an integer `k`, return the kth smallest value in the tree.

### Example

**Input:**
```text
        5
       / \
      3   6
     / \
    2   4
   /
  1

k = 3
```

**Output:**
```cpp
3
```

Why?

The BST values in ascending order are:

```text
1 → 2 → 3 → 4 → 5 → 6
```

The 3rd smallest element is `3`.

---

## 2. First Understand the BST Property

A Binary Search Tree follows:

- Left subtree contains smaller values.
- Right subtree contains greater values.

Example:

```text
        5
       / \
      3   7
     / \ / \
    2  4 6  8
```

Now perform **Inorder Traversal**:

```text
Left → Root → Right
```

Result:

```text
2 → 3 → 4 → 5 → 6 → 7 → 8
```

Notice that the values are automatically sorted in ascending order.

**This is the key observation behind the solution.**

---

## 3. Intuition — Think Like This

We don't need to store all BST values in an array and then sort them.

Instead:

1. Traverse the left subtree first.
2. Visit the current node.
3. Decrease `k` by one.
4. When `k` becomes zero, we have found the kth smallest element.
5. Otherwise, continue into the right subtree.

We can use recursion to implement this.

---

## 4. C++ Solution — Recursive Inorder Traversal

```cpp
class Solution {
public:
    int count = 0;
    int answer = 0;

    void inorder(TreeNode* root, int k) {

        if (root == nullptr) {
            return;
        }

        // Visit left subtree
        inorder(root->left, k);

        // Visit current node
        count++;

        if (count == k) {
            answer = root->val;
            return;
        }

        // Visit right subtree
        inorder(root->right, k);
    }

    int kthSmallest(TreeNode* root, int k) {

        count = 0;
        answer = 0;

        inorder(root, k);

        return answer;
    }
};
```

**Important:** This version is correct, but it can continue traversing some unnecessary nodes after finding the answer. We can improve it by stopping the recursion early.

### Optimized Recursive Solution

```cpp
class Solution {
public:
    int count = 0;
    int answer = 0;

    bool inorder(TreeNode* root, int k) {

        if (root == nullptr) {
            return false;
        }

        // Visit left subtree
        if (inorder(root->left, k)) {
            return true;
        }

        // Visit current node
        count++;

        if (count == k) {
            answer = root->val;
            return true;
        }

        // Visit right subtree
        return inorder(root->right, k);
    }

    int kthSmallest(TreeNode* root, int k) {

        count = 0;
        answer = 0;

        inorder(root, k);

        return answer;
    }
};
```

For interviews, understand the optimized version because it stops as soon as the kth element is found.

---

## 5. Understand the Optimized Code Step by Step

### Step 1: Maintain a Counter

```cpp
int count = 0;
```

This tracks how many nodes we have visited during inorder traversal.

### Step 2: Traverse the Left Subtree

```cpp
if (inorder(root->left, k)) {
    return true;
}
```

Inorder always visits the left subtree first.

This ensures smaller values are processed before the current node.

### Step 3: Process Current Node

```cpp
count++;
```

Every time we visit a node, increase the counter.

For example:

```text
Visited values: 1 → 2 → 3

Counter:        1 → 2 → 3
```

### Step 4: Check Whether We Found kth Element

```cpp
if (count == k) {
    answer = root->val;
    return true;
}
```

If the counter equals `k`, we found the answer.

Return `true` to indicate that the answer has been found.

### Step 5: Stop Unnecessary Recursion

```cpp
return inorder(root->right, k);
```

If the answer was not found in the left subtree or at the current node, continue into the right subtree.

The `true` return value propagates upward, stopping further traversal.

---

## 6. Complete Dry Run

Consider:

```text
        5
       / \
      3   6
     / \
    2   4
   /
  1

k = 3
```

Inorder traversal visits:

```text
1 → 2 → 3 → 4 → 5 → 6
```

Let's track the counter.

| Step | Current node | Counter | Is counter == k? |
|---|---:|---:|---|
| 1 | 1 | 1 | No |
| 2 | 2 | 2 | No |
| 3 | 3 | 3 | Yes |

At node `3`:

```cpp
count == k
```

Therefore:

```cpp
answer = 3;
return true;
```

The recursive calls stop.

**Final answer: `3`**

---

## 7. Alternative Approach — Iterative Inorder Using Stack

We can also solve this problem without recursion by using an explicit stack.

```cpp
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {

        stack<TreeNode*> st;

        while (root != nullptr || !st.empty()) {

            // Go as far left as possible
            while (root != nullptr) {
                st.push(root);
                root = root->left;
            }

            // Visit the top node
            root = st.top();
            st.pop();

            k--;

            if (k == 0) {
                return root->val;
            }

            // Move to right subtree
            root = root->right;
        }

        return -1;
    }
};
```

### How it works

Instead of recursive calls, we explicitly store nodes in a stack.

The stack follows:

**LIFO — Last In, First Out.**

The algorithm:
1. Push nodes while moving left.
2. Pop and visit the next smallest node.
3. Decrease `k`.
4. If `k == 0`, return the current value.
5. Otherwise, move to the right subtree.

This also stops immediately when the kth element is found.

---

## 8. Complexity Analysis

Let:
- `n` = total number of nodes.
- `h` = height of the BST.

| Approach | Time | Auxiliary Space |
|---|---|---|
| Recursive inorder | O(n) worst case | O(h) |
| Iterative inorder | O(n) worst case | O(h) |

Both approaches stop early when the kth element is found.

More precisely, they visit only the nodes required to reach the kth element, rather than necessarily visiting all `n` nodes.

---

## 9. Common Mistakes

1. Using preorder or postorder instead of inorder traversal.
2. Forgetting that BST inorder traversal produces ascending order.
3. Returning the kth node by level rather than sorted order.
4. Forgetting to increment the counter when visiting a node.
5. Not stopping recursion after finding the answer.
6. Using a global counter without resetting it before a new call.

---

## 10. Interview Questions

**Q1. Why does inorder traversal work for a BST?**

Because it visits the left subtree, root, and right subtree, producing values in ascending order.

**Q2. Why do we maintain a counter?**

To track how many nodes have been visited and identify the kth smallest node.

**Q3. Can we solve this without recursion?**

Yes, using an explicit stack for iterative inorder traversal.

**Q4. What is the time complexity?**

O(n) worst case, although early stopping can reduce the number of visited nodes.

**Q5. What is the space complexity?**

O(h), where h is the height of the tree.

**Q6. Can we use an array and sort it?**

Yes, but it is unnecessary because the BST already provides sorted order through inorder traversal.

---

## 11. Pattern Recognition

When a BST problem asks you to:
- Find the kth smallest element.
- Find elements in ascending order.
- Retrieve sorted values from a BST.

Think about:

**BST + Inorder Traversal**

Related problems:
- Validate Binary Search Tree
- Lowest Common Ancestor of a BST
- Convert Sorted Array to Binary Search Tree
- Kth Largest Element in a BST

---

## 12. 30-Second Interview Revision

- **Pattern:** BST + Inorder DFS.
- **Traversal:** Left → Root → Right.
- Inorder produces ascending values.
- Maintain a counter.
- Increment counter when visiting each node.
- When `count == k`, return the current node's value.
- Stop further recursion after finding the answer.
- **Time:** O(n) worst case.
- **Space:** O(h).

**Remember:**

For a BST, the kth node visited during inorder traversal is the kth smallest element.



# 57. Construct Binary Tree from Preorder and Inorder Traversal

## 1. Problem Understanding

You are given two arrays:

- `preorder`: Traversal in Root → Left → Right order.
- `inorder`: Traversal in Left → Root → Right order.

Your task is to reconstruct the original binary tree and return its root.

### Example

**Input:**
```cpp
preorder = {3, 9, 20, 15, 7};
inorder  = {9, 3, 15, 20, 7};
```

**Output:**
```text
        3
       / \
      9   20
         / \
        15  7
```

Important: All node values are unique, as guaranteed by the problem.

---

## 2. First Understand Preorder and Inorder

Before solving this problem, understand what information each traversal provides.

### Preorder: Root → Left → Right

```text
        3
       / \
      9   20
         / \
        15  7
```

Preorder:
```text
3 → 9 → 20 → 15 → 7
```

The first element is always the root.

### Inorder: Left → Root → Right

For the same tree:

```text
9 → 3 → 15 → 20 → 7
```

The root divides the inorder array into two parts:

```text
Left subtree | Root | Right subtree

     9       |  3   |  15, 20, 7
```

This is the key observation.

**Preorder identifies the root. Inorder identifies the left and right subtrees.**

---

## 3. Intuition — Think Like This

We can reconstruct the tree using three steps.

1. Take the next element from preorder. This is the current root.
2. Find that root in inorder.
3. Everything before it belongs to the left subtree, and everything after it belongs to the right subtree.

Repeat the same process recursively.

### Visual Example

```text
Preorder = [3, 9, 20, 15, 7]
Inorder  = [9, 3, 15, 20, 7]
```

First root:

```text
3
```

Inorder divides into:

```text
[9] | 3 | [15, 20, 7]
```

So:

```text
        3
       / \
      9   ?
```

Now construct the right subtree using the remaining preorder elements.

Next root:

```text
20
```

Its inorder section is:

```text
[15] | 20 | [7]
```

Final tree:

```text
        3
       / \
      9   20
         / \
        15  7
```

---

## 4. C++ Solution — Optimized Recursion + Hash Map

```cpp
class Solution {
public:
    unordered_map<int, int> inorderMap;
    int preorderIndex = 0;

    TreeNode* build(
        vector<int>& preorder,
        int left,
        int right
    ) {

        // No elements in this subtree
        if (left > right) {
            return nullptr;
        }

        // First available preorder element is the root
        int rootValue = preorder[preorderIndex++];

        TreeNode* root = new TreeNode(rootValue);

        // Find root position in inorder
        int mid = inorderMap[rootValue];

        // Construct left subtree
        root->left = build(preorder, left, mid - 1);

        // Construct right subtree
        root->right = build(preorder, mid + 1, right);

        return root;
    }

    TreeNode* buildTree(
        vector<int>& preorder,
        vector<int>& inorder
    ) {

        preorderIndex = 0;
        inorderMap.clear();

        // Store inorder values and their indices
        for (int i = 0; i < inorder.size(); i++) {
            inorderMap[inorder[i]] = i;
        }

        return build(preorder, 0, inorder.size() - 1);
    }
};
```

---

## 5. Understand the Code Step by Step

### Step 1: Create a Hash Map

```cpp
unordered_map<int, int> inorderMap;
```

We store each inorder value and its index.

For example:

```cpp
inorder = {9, 3, 15, 20, 7};
```

Hash map:

| Value | Index |
|---|---:|
| 9 | 0 |
| 3 | 1 |
| 15 | 2 |
| 20 | 3 |
| 7 | 4 |

Why?

Because we need to quickly find the root's position in inorder.

Hash map lookup takes O(1) average time.

### Step 2: Maintain Preorder Index

```cpp
int preorderIndex = 0;
```

This tracks which element of preorder should become the next root.

Initially:

```text
preorderIndex = 0
```

So the first root is:

```cpp
preorder[0] = 3;
```

### Step 3: Define Recursive Function

```cpp
TreeNode* build(
    vector<int>& preorder,
    int left,
    int right
)
```

The parameters mean:

- `preorder`: Gives us the next root.
- `left`: Starting index of the current inorder section.
- `right`: Ending index of the current inorder section.

We use these boundaries to identify the current subtree.

### Step 4: Base Case

```cpp
if (left > right) {
    return nullptr;
}
```

If the inorder section is empty, there is no node to construct.

Return `nullptr`.

### Step 5: Identify the Root

```cpp
int rootValue = preorder[preorderIndex++];
```

Take the next preorder element and move the index forward.

For example:

```text
Preorder = [3, 9, 20, 15, 7]

First call:
rootValue = 3
preorderIndex = 1
```

### Step 6: Create the Root Node

```cpp
TreeNode* root = new TreeNode(rootValue);
```

Create a new tree node.

### Step 7: Find Root in Inorder

```cpp
int mid = inorderMap[rootValue];
```

For root `3`:

```text
Inorder = [9, 3, 15, 20, 7]

Index of 3 = 1
```

Therefore:

```text
mid = 1
```

The left subtree occupies indices `0` to `0`.

The right subtree occupies indices `2` to `4`.

### Step 8: Build Left Subtree

```cpp
root->left = build(preorder, left, mid - 1);
```

For root `3`:

```text
build(preorder, 0, 0)
```

This creates the left subtree containing node `9`.

### Step 9: Build Right Subtree

```cpp
root->right = build(preorder, mid + 1, right);
```

For root `3`:

```text
build(preorder, 2, 4)
```

This creates the right subtree containing `20`, `15`, and `7`.

### Step 10: Return Root

```cpp
return root;
```

Once both subtrees are constructed, return the current root.

---

## 6. Complete Dry Run

Input:

```cpp
preorder = {3, 9, 20, 15, 7};
inorder  = {9, 3, 15, 20, 7};
```

Initial state:

```text
preorderIndex = 0

inorderMap:
9  → 0
3  → 1
15 → 2
20 → 3
7  → 4
```

### Call 1 — Construct Root 3

```text
preorder[0] = 3
mid = 1
```

Inorder section:

```text
[9] | 3 | [15, 20, 7]
```

Create:

```text
    3
```

Now recursively construct its left and right children.

### Call 2 — Construct Left Child 9

Current inorder range:

```text
left = 0
right = 0
```

Next preorder value:

```text
9
```

Create:

```text
    3
   /
  9
```

Its left and right ranges are empty, so both children are `nullptr`.

### Call 3 — Construct Right Child 20

Current inorder range:

```text
left = 2
right = 4
```

Next preorder value:

```text
20
```

Its inorder position is `3`.

Inorder section:

```text
[15] | 20 | [7]
```

Create:

```text
    3
   / \
  9   20
```

### Call 4 — Construct Left Child 15

Current inorder range:

```text
left = 2
right = 2
```

Next preorder value:

```text
15
```

Create node `15`.

### Call 5 — Construct Right Child 7

Current inorder range:

```text
left = 4
right = 4
```

Next preorder value:

```text
7
```

Create node `7`.

### Final Tree

```text
        3
       / \
      9   20
         / \
        15  7
```

All nodes are constructed.

---

## 7. Why Do We Construct Left Before Right?

Remember preorder:

```text
Root → Left → Right
```

The preorder array is:

```text
[3, 9, 20, 15, 7]
```

After selecting root `3`, the next element `9` belongs to its left subtree.

If we construct the right subtree before the left subtree, we would consume preorder elements in the wrong order.

Therefore, we must follow:

```cpp
root->left = build(...);
root->right = build(...);
```

**The recursive construction order must match preorder traversal.**

---

## 8. Why Do We Use a Hash Map?

Without a hash map, we could search for every root inside the inorder array using a loop.

That search could take O(n) for each node.

In the worst case, total time could become O(n²).

With an unordered map:

```cpp
unordered_map<int, int> inorderMap;
```

We can find the root index in O(1) average time.

This improves the overall solution to O(n) average time.

---

## 9. Complexity Analysis

Let `n` be the number of nodes.

| Complexity | Explanation |
|---|---|
| Time | O(n) average |
| Auxiliary space | O(n) |

**Time O(n):**
- Building the hash map: O(n) average.
- Constructing every node: O(n).
- Each hash map lookup: O(1) average.

**Space O(n):**
- Hash map: O(n).
- Recursion stack: O(h), where h is tree height.
- Constructed tree: O(n) output space, not counted as auxiliary space.

Worst-case recursion depth can be O(n) for a skewed tree.

---

## 10. Common Mistakes

1. Assuming the first inorder element is always the root.
2. Forgetting that the first preorder element is the root.
3. Constructing the right subtree before the left subtree.
4. Incorrectly calculating inorder boundaries.
5. Forgetting to increment `preorderIndex`.
6. Searching the entire inorder array repeatedly.
7. Forgetting the base case `left > right`.
8. Not handling unique node values as required by the problem.

---

## 11. Interview Questions

**Q1. Why do we need both preorder and inorder?**

Preorder identifies the root, while inorder identifies the left and right subtree boundaries.

**Q2. Why is the first preorder element the root?**

Because preorder follows Root → Left → Right.

**Q3. Why do we use a hash map?**

To find the root's position in inorder in O(1) average time.

**Q4. Why do we construct the left subtree first?**

Because preorder visits the left subtree before the right subtree.

**Q5. What is the base case?**

When `left > right`, the current subtree is empty, so return `nullptr`.

**Q6. Can we construct a tree using preorder and postorder alone?**

Not uniquely in the general case. A single-child node can be placed on either the left or right without changing those two traversals.

---

## 12. Pattern Recognition

When a binary tree problem provides:
- Preorder and inorder arrays.
- Traversal arrays and asks to reconstruct a tree.
- Root information and subtree boundaries.

Think about:

**Tree Construction + DFS + Hash Map**

Related problems:
- Construct Binary Tree from Inorder and Postorder Traversal
- Serialize and Deserialize Binary Tree
- Maximum Depth of Binary Tree
- Convert Sorted Array to Binary Search Tree

---

## 13. 30-Second Interview Revision

- **Pattern:** DFS + Recursion + Hash Map.
- Preorder gives the root.
- Inorder divides left and right subtrees.
- Maintain a `preorderIndex`.
- Use a hash map to find root positions quickly.
- Construct left subtree first, then right subtree.
- Base case: `left > right`.
- **Time:** O(n) average.
- **Auxiliary space:** O(n).

**Remember this formula:**

```cpp
int mid = inorderMap[rootValue];

root->left = build(preorder, left, mid - 1);
root->right = build(preorder, mid + 1, right);
```

Preorder tells us WHAT the root is. Inorder tells us WHERE its left and right subtrees are.



# 58. Binary Tree Maximum Path Sum

## 1. Problem Understanding

Given the root of a binary tree, return the maximum path sum of any non-empty path.

A path is a sequence of nodes connected by edges. It:
- Can start and end at any nodes.
- Must follow connected parent-child edges.
- Cannot visit the same node more than once.

**Important:** The path does not need to pass through the root.

### Example 1

**Input:**
```text
        1
       / \
      2   3
```

**Output:**
```cpp
6
```

Maximum path:

```text
2 → 1 → 3
```

Sum:

\[
2+1+3=6
\]

### Example 2

**Input:**
```text
       -10
       / \
      9   20
         / \
        15  7
```

**Output:**
```cpp
42
```

Maximum path:

```text
15 → 20 → 7
```

Sum:

\[
15+20+7=42
\]

Notice that the maximum path does not pass through the root `-10`.

---

## 2. Intuition — Think Like This

At every node, imagine two possible situations.

**Situation 1: We want to extend a path to the parent.**

The parent can continue through only one side of the current node.

Therefore, we can choose:

- Current node alone.
- Current node + left path.
- Current node + right path.

Formula:

\[
Gain = NodeValue+\max(0,LeftGain,RightGain)
\]

**Situation 2: We want to calculate the maximum path passing through the current node.**

Now we can use both left and right paths.

Formula:

\[
CurrentPath = NodeValue+LeftGain+RightGain
\]

We update the global maximum using this value.

**This is the most important distinction in this problem:**

- Return only one branch to the parent.
- Use both branches to update the global answer.

---

## 3. C++ Solution — Optimized DFS

```cpp
class Solution {
public:
    int maxSum = INT_MIN;

    int dfs(TreeNode* root) {

        // Base case
        if (root == nullptr) {
            return 0;
        }

        // Calculate left and right maximum gains
        int leftGain = max(0, dfs(root->left));
        int rightGain = max(0, dfs(root->right));

        // Maximum path passing through current node
        int currentPath = root->val + leftGain + rightGain;

        // Update global maximum
        maxSum = max(maxSum, currentPath);

        // Return one branch to parent
        return root->val + max(leftGain, rightGain);
    }

    int maxPathSum(TreeNode* root) {

        maxSum = INT_MIN;

        dfs(root);

        return maxSum;
    }
};
```

---

## 4. Understand the Code Step by Step

### Step 1: Initialize Global Maximum

```cpp
int maxSum = INT_MIN;
```

We maintain the maximum path sum found anywhere in the tree.

Why `INT_MIN`?

Because the tree may contain only negative values.

For example:

```text
    -3
```

The answer must be `-3`, not `0`.

### Step 2: Base Case

```cpp
if (root == nullptr) {
    return 0;
}
```

An empty subtree contributes zero to the path.

### Step 3: Calculate Left and Right Gains

```cpp
int leftGain = max(0, dfs(root->left));
int rightGain = max(0, dfs(root->right));
```

This is extremely important.

If a subtree has a negative path sum, we ignore it because including it would reduce our total.

Example:

```text
       5
      / \
    -3   4
```

At node `5`:

```text
Left gain  = max(0, -3) = 0
Right gain = max(0, 4)  = 4
```

We ignore the negative contribution.

### Step 4: Calculate Path Through Current Node

```cpp
int currentPath = root->val + leftGain + rightGain;
```

Here, we can use both branches.

Example:

```text
       5
      / \
     3   4
```

Path through `5`:

\[
3+5+4=12
\]

### Step 5: Update Global Maximum

```cpp
maxSum = max(maxSum, currentPath);
```

Compare the current path sum with the best answer found so far.

### Step 6: Return Only One Branch

```cpp
return root->val + max(leftGain, rightGain);
```

Why only one branch?

Because the parent can connect to the current node through only one child path without creating a fork.

If we returned both branches, the parent could accidentally combine three branches into one invalid path.

---

## 5. Complete Dry Run

Consider:

```text
       -10
       / \
      9   20
         / \
        15  7
```

Initially:

```cpp
maxSum = INT_MIN;
```

DFS processes children before their parent.

### Node 9

```text
Left gain  = 0
Right gain = 0
```

Current path:

\[
9+0+0=9
\]

```text
maxSum = 9
```

Return:

```text
9
```

### Node 15

```text
Left gain  = 0
Right gain = 0
```

Current path:

\[
15+0+0=15
\]

```text
maxSum = 15
```

Return:

```text
15
```

### Node 7

Current path:

\[
7+0+0=7
\]

```text
maxSum = 15
```

Return:

```text
7
```

### Node 20

Left gain = `15`

Right gain = `7`

Current path:

\[
20+15+7=42
\]

```text
maxSum = 42
```

Return to parent:

\[
20+\max(15,7)=35
\]

Notice that we return `35`, not `42`, because the parent can use only one branch.

### Node -10

Left gain = `9`

Right gain = `35`

Current path:

\[
-10+9+35=34
\]

```text
maxSum = max(42, 34) = 42
```

Return:

\[
-10+\max(9,35)=25
\]

Final answer:

```cpp
return 42;
```

### Dry Run Summary

| Node | Left gain | Right gain | Current path | Global maximum |
|---|---:|---:|---:|---:|
| 9 | 0 | 0 | 9 | 9 |
| 15 | 0 | 0 | 15 | 15 |
| 7 | 0 | 0 | 7 | 15 |
| 20 | 15 | 7 | 42 | 42 |
| -10 | 9 | 35 | 34 | 42 |

---

## 6. Why Do We Use max(0, gain)?

Consider:

```text
       5
      / \
    -10   4
```

If we include the negative left subtree:

\[
5+(-10)+4=-1
\]

But if we ignore it:

\[
5+4=9
\]

Therefore:

```cpp
max(0, dfs(root->left))
```

means:

- Positive gain → use it.
- Negative gain → ignore it.
- Zero gain → no contribution.

**Important:** We still initialize `maxSum` to `INT_MIN`, not zero, so an all-negative tree returns its largest node value.

---

## 7. Important Concept — Why Return Only One Branch?

Consider:

```text
        10
       /  \
      5    8
```

At node `10`:

Maximum path through this node:

\[
5+10+8=23
\]

But suppose the parent of `10` is `20`.

If we returned `23`, the parent might try to add `20`:

\[
20+23=43
\]

That would represent a path:

```text
5 → 10 → 8
          \
           20
```

This is invalid because the path branches at node `10`.

The correct return is:

\[
10+\max(5,8)=18
\]

The parent can extend only one side.

---

## 8. Special Case — All Negative Values

Input:

```text
       -3
       / \
     -5  -2
```

At each node, negative child gains are ignored.

| Node | Left gain | Right gain | Current path |
|---|---:|---:|---:|
| -5 | 0 | 0 | -5 |
| -2 | 0 | 0 | -2 |
| -3 | 0 | 0 | -3 |

Global maximum:

\[
\max(-5,-2,-3)=-2
\]

Answer:

```cpp
-2
```

This is why we must not initialize `maxSum` to `0`.

---

## 9. Complexity Analysis

Let `n` be the total number of nodes and `h` be the height of the tree.

| Complexity | Explanation |
|---|---|
| Time | O(n) |
| Auxiliary space | O(h) |

- **Time O(n):** Every node is visited exactly once.
- **Space O(h):** Recursive call stack depends on tree height.
  - Balanced tree: O(log n)
  - Skewed tree: O(n)

---

## 10. Common Mistakes

1. Returning both left and right gains to the parent.
2. Forgetting to ignore negative subtree contributions.
3. Initializing the global answer to `0`, which fails for all-negative trees.
4. Updating the global answer using only one branch.
5. Confusing the value returned by DFS with the maximum path through the current node.
6. Forgetting to reset `maxSum` before starting a new calculation.

---

## 11. Interview Questions

**Q1. Why do we use a global maximum?**

Because the maximum path can exist anywhere in the tree, not necessarily through the root.

**Q2. Why do we use `max(0, dfs(...))`?**

To ignore negative subtree contributions that would reduce the path sum.

**Q3. Why can we return only one branch?**

Because the parent can extend the path through only one child without creating a branching path.

**Q4. Why do we add both left and right gains when updating the answer?**

Because a valid path passing through the current node can connect one branch from the left and one from the right.

**Q5. Why initialize the answer to `INT_MIN`?**

To handle trees where all node values are negative.

**Q6. What is the time complexity?**

O(n), because every node is visited once.

---

## 12. Pattern Recognition

When a binary tree problem asks you to:

- Find a maximum or minimum path.
- Combine information from left and right subtrees.
- Return a result to the parent.
- Maintain a global answer.
- Ignore negative contributions when they are harmful.

Think about:

**Postorder DFS + Global Maximum + One-Branch Return**

Related problems:
- Diameter of Binary Tree
- Maximum Depth of Binary Tree
- Balanced Binary Tree
- Binary Tree Maximum Path Sum

---

## 13. 30-Second Interview Revision

- **Pattern:** Postorder DFS.
- Calculate left and right gains.
- Ignore negative gains using `max(0, gain)`.
- Current path = `node + leftGain + rightGain`.
- Update global maximum.
- Return only `node + max(leftGain, rightGain)`.
- Initialize answer with `INT_MIN`.
- **Time:** O(n).
- **Space:** O(h).

**Remember these two formulas:**

```cpp
// Update global answer using both branches
maxSum = max(maxSum, root->val + leftGain + rightGain);

// Return only one branch to parent
return root->val + max(leftGain, rightGain);
```

The entire problem revolves around understanding why these two calculations are different.



# 59. Serialize and Deserialize Binary Tree

## 1. Problem Understanding

We need to implement two functions:

- `serialize(root)`: Convert a binary tree into a single string.
- `deserialize(data)`: Reconstruct the original binary tree from that string.

The reconstructed tree must have exactly the same structure and values as the original tree.

### Example

Original tree:

```text
        1
       / \
      2   3
         / \
        4   5
```

Serialized string using preorder traversal:

```text
1,2,#,#,3,4,#,#,5,#,#
```

Here:
- Numbers represent node values.
- `#` represents a null child.

Why do we need `#`?

Because without null markers, we cannot reliably distinguish between different tree structures.

---

## 2. Core Intuition

We will use **DFS Preorder Traversal**.

Preorder means:

```text
Root → Left → Right
```

For every node:

1. Store its value.
2. Serialize its left subtree.
3. Serialize its right subtree.
4. If the node is null, store `#`.

For deserialization, we read the serialized values in the same order and reconstruct the tree recursively.

The important idea is that **serialization and deserialization must follow the same traversal order**.

---

## 3. Visual Understanding

Consider this tree:

```text
        1
       / \
      2   3
         / \
        4   5
```

Preorder traversal:

```text
1 → 2 → null → null → 3 → 4 → null → null → 5 → null → null
```

Serialized output:

```text
1,2,#,#,3,4,#,#,5,#,#
```

Notice how every missing child is represented by `#`.

This makes the tree structure unambiguous.

---

## 4. Complete C++ Solution

```cpp
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {

        if (root == nullptr) {
            return "#,";
        }

        string result = to_string(root->val) + ",";

        result += serialize(root->left);
        result += serialize(root->right);

        return result;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {

        stringstream ss(data);
        return buildTree(ss);
    }

private:

    TreeNode* buildTree(stringstream& ss) {

        string value;
        getline(ss, value, ',');

        // Null marker means no node exists here.
        if (value == "#") {
            return nullptr;
        }

        // Create the current node.
        TreeNode* root = new TreeNode(stoi(value));

        // Reconstruct left subtree first.
        root->left = buildTree(ss);

        // Then reconstruct right subtree.
        root->right = buildTree(ss);

        return root;
    }
};
```

---

## 5. Understanding serialize() Step by Step

Code:

```cpp
if (root == nullptr) {
    return "#,";
}

string result = to_string(root->val) + ",";

result += serialize(root->left);
result += serialize(root->right);

return result;
```

Let's understand what happens:

For node `1`:

```text
1
```

First, store its value:

```text
"1,"
```

Then serialize its left subtree:

```text
2,#,#,
```

Then serialize its right subtree:

```text
3,4,#,#,5,#,#,
```

Final result:

```text
1,2,#,#,3,4,#,#,5,#,#
```

**Important:** Every null pointer returns `"#,"`. This is what preserves the tree's structure.

---

## 6. Understanding deserialize() Step by Step

We use:

```cpp
stringstream ss(data);
```

A stringstream lets us read the serialized string token by token.

For example:

```text
1,2,#,#,3,4,#,#,5,#,#
```

We read each token using:

```cpp
getline(ss, value, ',');
```

The comma acts as the delimiter.

### Reconstruction process

| Token | Action |
|---|---|
| `1` | Create node 1 |
| `2` | Create node 2 as left child of 1 |
| `#` | Node 2's left child is null |
| `#` | Node 2's right child is null |
| `3` | Create node 3 as right child of 1 |
| `4` | Create node 4 as left child of 3 |
| `#` | Node 4's left child is null |
| `#` | Node 4's right child is null |
| `5` | Create node 5 as right child of 3 |
| `#` | Node 5's left child is null |
| `#` | Node 5's right child is null |

Reconstructed tree:

```text
        1
       / \
      2   3
         / \
        4   5
```

The original tree is restored.

---

## 7. Dry Run: Recursive Deserialization

Input:

```text
1,2,#,#,3,4,#,#,5,#,#
```

The recursive calls work like this:

```text
buildTree()
    |
    |-- Read 1 → Create node 1
    |
    |-- Build left of 1
    |      |
    |      |-- Read 2 → Create node 2
    |      |-- Read # → left = nullptr
    |      |-- Read # → right = nullptr
    |
    |-- Build right of 1
           |
           |-- Read 3 → Create node 3
           |-- Build left of 3
           |      |
           |      |-- Read 4 → Create node 4
           |      |-- Read # → left = nullptr
           |      |-- Read # → right = nullptr
           |
           |-- Build right of 3
                  |
                  |-- Read 5 → Create node 5
                  |-- Read # → left = nullptr
                  |-- Read # → right = nullptr
```

One important detail: the stringstream is shared across all recursive calls. Every call consumes the next token, so we do not need to manually track an index.

---

## 8. Edge Cases

### Case 1: Empty tree

Input:

```text
root = nullptr
```

Serialization:

```text
#,
```

Deserialization:

```cpp
return nullptr;
```

### Case 2: Single node

Tree:

```text
    10
```

Serialization:

```text
10,#,#,
```

### Case 3: Only left child

Tree:

```text
    1
   /
  2
```

Serialization:

```text
1,2,#,#,#,
```

### Case 4: Only right child

Tree:

```text
  1
   \
    2
```

Serialization:

```text
1,#,2,#,#,
```

The null markers distinguish the left-only and right-only trees.

---

## 9. Complexity Analysis

Let `n` be the number of nodes in the tree.

| Operation | Time Complexity | Auxiliary Space |
|---|---|---|
| serialize | O(n) | O(h) recursion stack |
| deserialize | O(n) | O(h) recursion stack |

- `h` is the height of the tree.
- For a balanced tree, height is O(log n).
- For a skewed tree, height is O(n).
- The serialized string itself requires O(n) space, excluding the recursion stack.

---

## 10. Common Interview Mistakes

1. **Not storing null markers:** Without them, the original tree structure cannot always be reconstructed.
2. **Using different traversal orders:** If serialization uses preorder, deserialization must consume preorder.
3. **Not sharing the stringstream:** Each recursive call must read from the same stream.
4. **Forgetting to build the right subtree:** After reconstructing the left subtree, reconstruct the right subtree.
5. **Incorrect token parsing:** Use a delimiter such as a comma to separate values.
6. **Not handling an empty tree:** `nullptr` must serialize and deserialize correctly.

---

## 11. C++ Concepts to Remember

| Concept | Purpose |
|---|---|
| `to_string()` | Converts an integer into a string |
| `stringstream` | Reads data from a string |
| `getline()` | Reads one token until a delimiter |
| `stoi()` | Converts a string into an integer |
| Recursion | Processes nested left and right subtrees |
| `nullptr` | Represents a missing child |

---

## 12. Pattern Recognition for Interviews

Whenever you see a problem involving:

- Converting a tree into a string.
- Reconstructing a tree from encoded data.
- Preserving the exact tree structure.
- Representing missing children.

Think about:

**Tree Traversal + Null Markers + Recursion**

Preorder DFS is a simple and effective solution.

---

## 13. 30-Second Interview Revision

**Q: Which traversal did you use?**

Preorder DFS: Root → Left → Right.

**Q: Why do you store `#`?**

To represent null children and preserve the exact tree structure.

**Q: How do you deserialize?**

Read tokens sequentially using a shared stringstream. Create a node for a number, recursively construct its left and right children, and return null for `#`.

**Q: Why is the solution O(n)?**

Every node and null marker is processed a constant number of times.

**Q: What is the most important condition?**

Serialization and deserialization must use the same traversal order.

### Final takeaway

```text
SERIALIZE:
Root → Left → Right
Store values and # for nulls.

DESERIALIZE:
Read next token.
If # → return nullptr.
Otherwise → create node,
             build left,
             build right.
```


**Remember:** Invert the current node, then invert both of its subtrees.

