# 28. Reverse Linked List

**NeetCode Pattern:** Linked List  
**Technique:** Iterative Reversal using Three Pointers  
**Difficulty:** Easy

## 1. Problem Understanding

You are given the head of a singly linked list.

Reverse the linked list and return the new head.

### Example 1

```text
Input:
1 → 2 → 3 → 4 → 5 → nullptr

Output:
5 → 4 → 3 → 2 → 1 → nullptr
```

### Example 2

```text
Input:
1 → 2 → nullptr

Output:
2 → 1 → nullptr
```

### Example 3

```text
Input:
nullptr

Output:
nullptr
```

---

## 2. Core Intuition

In a singly linked list, every node points to the next node.

Initially:

```text
1 → 2 → 3 → 4 → nullptr
```

We need to reverse all the arrows:

```text
1 ← 2 ← 3 ← 4
```

Final linked list:

```text
4 → 3 → 2 → 1 → nullptr
```

We cannot simply reverse the values because the problem asks us to reverse the linked list itself.

We must change the `next` pointers.

## 3. Three-Pointer Approach

We use three pointers:

| Pointer | Purpose |
|---|---|
| `prev` | Stores the previous node |
| `curr` | Points to the current node |
| `nextNode` | Temporarily stores the next node |

Initially:

```cpp
ListNode* prev = nullptr;
ListNode* curr = head;
```

Why is `prev = nullptr`?

Because after reversal, the original first node becomes the last node, and its next pointer must be `nullptr`.

### The four important steps

**Step 1: Save the next node**

```cpp
ListNode* nextNode = curr->next;
```

We save it because we are about to change `curr->next`.

**Step 2: Reverse the current pointer**

```cpp
curr->next = prev;
```

Instead of pointing forward, the current node now points backward.

**Step 3: Move `prev` forward**

```cpp
prev = curr;
```

The current node becomes the previous node for the next iteration.

**Step 4: Move `curr` forward**

```cpp
curr = nextNode;
```

Continue processing the remaining list.

---

## 4. Optimal C++ Solution (Iterative)

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {

        ListNode* prev = nullptr;
        ListNode* curr = head;

        while (curr != nullptr) {

            ListNode* nextNode = curr->next;

            curr->next = prev;

            prev = curr;
            curr = nextNode;
        }

        return prev;
    }
};
```

---

## 5. Dry Run

Input:

```text
1 → 2 → 3 → nullptr
```

Initially:

```text
prev = nullptr
curr = 1
```

### Iteration 1

Save the next node:

```cpp
nextNode = curr->next;  // 2
```

Reverse the pointer:

```cpp
curr->next = prev;
```

Now:

```text
1 → nullptr

2 → 3 → nullptr
```

Move pointers:

```text
prev = 1
curr = 2
```

### Iteration 2

Save:

```text
nextNode = 3
```

Reverse:

```text
2 → 1 → nullptr
```

Move pointers:

```text
prev = 2
curr = 3
```

### Iteration 3

Save:

```text
nextNode = nullptr
```

Reverse:

```text
3 → 2 → 1 → nullptr
```

Move pointers:

```text
prev = 3
curr = nullptr
```

The loop stops.

Return:

```cpp
return prev;
```

Final result:

```text
3 → 2 → 1 → nullptr
```

---

## 6. Why Must We Store `nextNode` First?

This is one of the most important linked-list concepts.

Consider:

```cpp
curr->next = prev;
curr = curr->next;
```

This is incorrect.

Why?

After reversing `curr->next`, it points backward to `prev`.

So `curr = curr->next` would move backward instead of moving to the next unprocessed node.

We would lose access to the remaining list.

Correct order:

```cpp
ListNode* nextNode = curr->next;
curr->next = prev;
prev = curr;
curr = nextNode;
```

**Remember:** Save the next node before changing the current node's pointer.

---

## 7. Why Return `prev` Instead of `head`?

Initially, `head` points to the first node.

After reversing, the original first node becomes the last node.

Example:

```text
Original:
1 → 2 → 3 → nullptr

Reversed:
3 → 2 → 1 → nullptr
```

The new head is node `3`, which is stored in `prev` when the loop finishes.

Therefore:

```cpp
return prev;
```

---

## 8. Alternative Approach: Recursive Reversal

We can also reverse a linked list using recursion.

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {

        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        ListNode* newHead = reverseList(head->next);

        head->next->next = head;
        head->next = nullptr;

        return newHead;
    }
};
```

### Understanding the recursive logic

Suppose:

```text
1 → 2 → 3 → nullptr
```

The recursive calls go until node `3`.

```cpp
reverseList(1)
    reverseList(2)
        reverseList(3)
```

Node `3` becomes the new head.

Now recursion starts returning.

At node `2`:

```cpp
head->next->next = head;
```

This makes node `3` point to node `2`.

At node `1`, the same operation makes node `2` point to node `1`.

Finally:

```cpp
head->next = nullptr;
```

This ensures the old forward pointer does not create a cycle.

Final result:

```text
3 → 2 → 1 → nullptr
```

### Complexity

| Approach | Time | Extra Space |
|---|---|---|
| Iterative | O(n) | O(1) |
| Recursive | O(n) | O(n) |

**Interview preference:** Iterative reversal is generally preferred because it uses constant extra space.

---

## 9. Complexity Analysis

Let `n` be the number of nodes.

**Time Complexity: O(n)**

Every node is visited exactly once.

**Space Complexity: O(1)**

The iterative approach uses only three pointers, regardless of the list length.

---

## 10. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Not storing `nextNode` | Remaining list can be lost |
| Initializing `prev = head` | Does not correctly terminate the reversed list |
| Returning `head` | Original head becomes the last node |
| Forgetting `curr->next = prev` | No pointer reversal happens |
| Moving `curr` before saving `nextNode` | Can lose access to remaining nodes |
| Forgetting the base case in recursion | Can cause invalid pointer access |

---

## 11. Edge Cases

| Input | Expected |
|---|---|
| `nullptr` | `nullptr` |
| `1 → nullptr` | `1 → nullptr` |
| `1 → 2 → nullptr` | `2 → 1 → nullptr` |
| `1 → 2 → 3 → 4 → nullptr` | `4 → 3 → 2 → 1 → nullptr` |

---

## 12. C++ Concepts to Remember

- `ListNode*`: Pointer to a linked-list node.
- `curr->next`: Accesses the next node through a pointer.
- `nullptr`: Represents an empty or null pointer.
- `ListNode* nextNode`: Temporarily preserves the next node.
- `head`: Points to the first node of the original list.
- `return prev`: Returns the new head after reversal.

---

## 13. Pattern Recognition for Interviews

Recognize Linked List Pointer Manipulation when:

- You need to reverse node connections.
- You need to rearrange nodes without creating a new list.
- You need to modify `next` pointers.
- The problem involves previous, current and next nodes.

Related problems:

- Reverse Linked List
- Reverse Linked List II
- Reorder List
- Palindrome Linked List
- Merge Two Sorted Lists

---

## 14. 30-Second Revision

1. Initialize `prev = nullptr`.
2. Initialize `curr = head`.
3. While `curr != nullptr`:
   - Save `curr->next` in `nextNode`.
   - Reverse the pointer: `curr->next = prev`.
   - Move `prev = curr`.
   - Move `curr = nextNode`.
4. Return `prev`.

**One-line interview explanation:**

# 29. Merge Two Sorted Lists

**NeetCode Pattern:** Linked List  
**Technique:** Two Pointers + Dummy Node  
**Difficulty:** Easy

## 1. Problem Understanding

You are given the heads of two sorted singly linked lists, `list1` and `list2`.

Merge them into one sorted linked list and return its head.

You must reuse the existing nodes rather than create a completely new list.

### Example 1

```text
Input:

list1 = 1 → 2 → 4
list2 = 1 → 3 → 4

Output:

1 → 1 → 2 → 3 → 4 → 4
```

### Example 2

```text
Input:

list1 = []
list2 = []

Output:

[]
```

### Example 3

```text
Input:

list1 = []
list2 = 0

Output:

0
```

---

## 2. Core Intuition

Both linked lists are already sorted.

We compare their current nodes and select the smaller one.

For example:

```text
list1: 1 → 2 → 4
list2: 1 → 3 → 4
```

Comparison:

```text
1 vs 1 → Select either one
2 vs 1 → Select 1
2 vs 3 → Select 2
4 vs 3 → Select 3
4 vs 4 → Select either one
```

Final merged list:

```text
1 → 1 → 2 → 3 → 4 → 4
```

We use two pointers to traverse the lists and a third pointer to build the result.

## 3. Understanding the Dummy Node

A dummy node is a temporary node placed before the actual merged list.

```cpp
ListNode dummy(0);
ListNode* tail = &dummy;
```

Initially:

```text
dummy → nullptr
```

Why use it?

Without a dummy node, we would need separate logic to determine the head of the merged list.

With a dummy node, every selected node can be attached using the same logic.

We maintain:

| Variable | Purpose |
|---|---|
| `dummy` | Temporary starting node |
| `tail` | Points to the last node of the merged list |
| `list1` | Traverses the first list |
| `list2` | Traverses the second list |

## 4. Optimal C++ Solution

```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {

        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (list1 != nullptr && list2 != nullptr) {

            if (list1->val <= list2->val) {

                tail->next = list1;
                list1 = list1->next;

            }
            else {

                tail->next = list2;
                list2 = list2->next;

            }

            tail = tail->next;
        }

        // Attach the remaining nodes
        if (list1 != nullptr) {
            tail->next = list1;
        }
        else {
            tail->next = list2;
        }

        return dummy.next;
    }
};
```

---

## 5. Dry Run

Input:

```text
list1 = 1 → 2 → 4
list2 = 1 → 3 → 4
```

Initially:

```text
dummy → nullptr

list1 → 1 → 2 → 4
list2 → 1 → 3 → 4

tail = dummy
```

### Iteration 1

Compare:

```text
list1->val = 1
list2->val = 1
```

Since `1 <= 1`, select the first list's node.

```cpp
tail->next = list1;
list1 = list1->next;
tail = tail->next;
```

Merged list:

```text
dummy → 1
```

### Iteration 2

Compare:

```text
list1->val = 2
list2->val = 1
```

Select `list2`.

Merged list:

```text
dummy → 1 → 1
```

### Iteration 3

Compare:

```text
list1->val = 2
list2->val = 3
```

Select `list1`.

Merged list:

```text
dummy → 1 → 1 → 2
```

### Iteration 4

Compare:

```text
list1->val = 4
list2->val = 3
```

Select `list2`.

Merged list:

```text
dummy → 1 → 1 → 2 → 3
```

### Iteration 5

Compare:

```text
list1->val = 4
list2->val = 4
```

Select `list1`.

Merged list:

```text
dummy → 1 → 1 → 2 → 3 → 4
```

Now `list1` becomes `nullptr`.

The main loop stops.

### Attach the remaining nodes

```cpp
tail->next = list2;
```

Final list:

```text
dummy → 1 → 1 → 2 → 3 → 4 → 4
```

Return:

```cpp
return dummy.next;
```

Actual result:

```text
1 → 1 → 2 → 3 → 4 → 4
```

---

## 6. Understanding Important Conditions

### Condition 1: Why use `while` with both pointers?

```cpp
while (list1 != nullptr && list2 != nullptr)
```

We can compare values only when both lists have a current node.

Once either list becomes empty, the comparison stops.

### Condition 2: Why use `<=`?

```cpp
if (list1->val <= list2->val)
```

When values are equal, we can select either node.

Using `<=` simply selects the first list's node in case of equality.

### Condition 3: Why move `tail`?

```cpp
tail = tail->next;
```

After attaching a node, `tail` must move to the newly attached node so that the next selected node is appended after it.

### Condition 4: Why attach the remaining list?

```cpp
if (list1 != nullptr) {
    tail->next = list1;
}
else {
    tail->next = list2;
}
```

The remaining nodes are already sorted.

There is no need to process them individually.

We can attach the entire remaining list directly.

### Condition 5: Why return `dummy.next`?

`dummy` is only a temporary starting node.

The actual merged list begins at:

```cpp
dummy.next
```

We must not return `dummy`, because that would include the extra temporary node.

---

## 7. Complexity Analysis

Let:

- `m` = number of nodes in `list1`
- `n` = number of nodes in `list2`

**Time Complexity: O(m+n)**

Every node is visited or attached at most once.

**Space Complexity: O(1)** auxiliary space.

We reuse existing nodes and create only one dummy node. No new linked list is allocated.

---

## 8. Alternative Approach: Recursive Solution

We can also merge the lists recursively.

```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {

        if (list1 == nullptr) {
            return list2;
        }

        if (list2 == nullptr) {
            return list1;
        }

        if (list1->val <= list2->val) {

            list1->next = mergeTwoLists(list1->next, list2);
            return list1;

        }
        else {

            list2->next = mergeTwoLists(list1, list2->next);
            return list2;

        }
    }
};
```

### Recursive intuition

If the first list has the smaller value:

```cpp
list1->next = mergeTwoLists(list1->next, list2);
return list1;
```

We select `list1` as the first node and recursively merge the remaining lists.

The same logic applies when `list2` has the smaller value.

**Complexity:**

- Time: O(m+n)
- Auxiliary space: O(m+n) due to recursive call stack.

**Interview preference:** The iterative dummy-node approach is generally better because it uses O(1) auxiliary space.

---

## 9. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Returning `dummy` instead of `dummy.next` | Returns the extra dummy node |
| Forgetting `tail = tail->next` | New nodes may be attached incorrectly |
| Forgetting to advance `list1` or `list2` | Can cause an infinite loop |
| Comparing after one list becomes null | Causes invalid pointer access |
| Forgetting to attach the remaining list | Produces an incomplete result |
| Creating a new node for every value unnecessarily | Extra memory is not required |

---

## 10. Edge Cases

| `list1` | `list2` | Expected |
|---|---|---|
| `[]` | `[]` | `[]` |
| `[1]` | `[]` | `[1]` |
| `[]` | `[1]` | `[1]` |
| `[1,2,3]` | `[4,5,6]` | `[1,2,3,4,5,6]` |
| `[4,5,6]` | `[1,2,3]` | `[1,2,3,4,5,6]` |
| `[1,1]` | `[1,1]` | `[1,1,1,1]` |

---

## 11. C++ Concepts to Remember

- `ListNode*`: Pointer to a linked-list node.
- `ListNode dummy(0)`: Creates a temporary node on the stack.
- `&dummy`: Gets the address of the dummy node.
- `tail->next`: Connects one node to another.
- `dummy.next`: Accesses the actual head of the merged list.
- `nullptr`: Represents an empty pointer.
- Pointer reassignment: Allows us to reuse existing nodes without copying their values.

---

## 12. Pattern Recognition for Interviews

Recognize this pattern when:

- Two linked lists are already sorted.
- You need to combine them while maintaining sorted order.
- You need to manipulate node connections rather than copy values.

Related problems:

- Reverse Linked List
- Merge Two Sorted Lists
- Merge K Sorted Lists
- Reorder List
- Sort List

---

## 13. 30-Second Revision

1. Create a dummy node.
2. Set `tail = &dummy`.
3. Traverse while both lists are non-null.
4. Compare their current values.
5. Attach the smaller node to `tail->next`.
6. Advance the selected list pointer.
7. Move `tail` forward.
8. Attach whichever list remains.
9. Return `dummy.next`.

**One-line interview explanation:**

"I use two pointers to traverse the sorted linked lists and a dummy node to build the merged result. At each step, I attach the smaller node and advance its pointer. Once one list is exhausted, I attach the remaining list directly. This takes O(m+n) time and O(1) auxiliary space."

"I use three pointers: previous, current and next. I save the next node before reversing the current node's pointer, then move both pointers forward. Each node is processed once, giving O(n) time and O(1) extra space."

# 30. Linked List Cycle

**NeetCode Pattern:** Linked List  
**Technique:** Floyd's Cycle Detection (Slow & Fast Pointers)  
**Difficulty:** Easy

## 1. Problem Understanding

You are given the head of a singly linked list.

Determine whether the linked list contains a cycle.

A cycle exists when a node's `next` pointer points to a previously visited node, creating a loop.

Return:
- `true` if a cycle exists.
- `false` if there is no cycle.

### Example 1: Cycle exists

```text
Input:

3 → 2 → 0 → -4
    ↑         |
    └─────────┘

Output: true
```

The last node points back to the node containing `2`.

Therefore, the linked list never reaches `nullptr`.

### Example 2: No cycle

```text
Input:

1 → 2 → nullptr

Output: false
```

### Example 3: Self-cycle

```text
Input:

1
↑ ↓
└─┘

Output: true
```

A node can also point to itself.

---

## 2. Core Intuition

A simple approach would be to visit every node and remember which nodes we have already visited.

But we can solve this problem using only two pointers.

We maintain:

- `slow`: Moves one node at a time.
- `fast`: Moves two nodes at a time.

### What happens if there is no cycle?

```text
1 → 2 → 3 → 4 → nullptr
```

The fast pointer eventually reaches `nullptr`.

Therefore, no cycle exists.

### What happens if there is a cycle?

```text
1 → 2 → 3 → 4
    ↑       ↓
    └───────┘
```

Both pointers enter the cycle.

Since `fast` moves faster than `slow`, it eventually catches up with `slow`.

When:

```cpp
slow == fast
```

A cycle exists.

---

## 3. Floyd's Cycle Detection Algorithm

This algorithm uses two pointers.

| Pointer | Movement |
|---|---|
| `slow` | One node at a time |
| `fast` | Two nodes at a time |

Initialize:

```cpp
ListNode* slow = head;
ListNode* fast = head;
```

Move them:

```cpp
slow = slow->next;
fast = fast->next->next;
```

Check whether they meet:

```cpp
if (slow == fast) {
    return true;
}
```

If `fast` or `fast->next` becomes `nullptr`, there is no cycle.

---

## 4. Optimal C++ Solution

```cpp
class Solution {
public:
    bool hasCycle(ListNode* head) {

        ListNode* slow = head;
        ListNode* fast = head;

        while (fast != nullptr && fast->next != nullptr) {

            slow = slow->next;
            fast = fast->next->next;

            if (slow == fast) {
                return true;
            }
        }

        return false;
    }
};
```

---

## 5. Dry Run

Consider:

```text
1 → 2 → 3 → 4
    ↑       ↓
    └───────┘
```

The last node points back to node `2`.

Initially:

```text
slow = 1
fast = 1
```

| Iteration | Slow pointer | Fast pointer | Result |
|---|---:|---:|---|
| Initial | 1 | 1 | Not checked yet |
| 1 | 2 | 3 | Different |
| 2 | 3 | 2 | Different |
| 3 | 4 | 4 | Same |

At iteration 3:

```cpp
slow == fast
```

Therefore:

```cpp
return true;
```

A cycle exists.

---

## 6. Why Does the Fast Pointer Eventually Meet the Slow Pointer?

Imagine two runners on a circular track.

- Slow runner moves one step per turn.
- Fast runner moves two steps per turn.

The fast runner gains one step on the slow runner every turn.

Because the track is circular, the fast runner eventually catches the slow runner.

The same logic applies to a cyclic linked list.

**Important:** The fast pointer does not need to meet the slow pointer at the beginning of the cycle. It only needs to meet it somewhere inside the cycle.

---

## 7. Understanding the While Condition

```cpp
while (fast != nullptr && fast->next != nullptr)
```

Why check both?

Because we execute:

```cpp
fast = fast->next->next;
```

Before accessing `fast->next->next`, we must ensure:

1. `fast` is not `nullptr`.
2. `fast->next` is not `nullptr`.

Otherwise, we could access an invalid pointer and cause a runtime error.

If either condition fails, the list has ended and there is no cycle.

---

## 8. Alternative Approach: Hash Set

We can also detect a cycle by storing visited node addresses.

```cpp
class Solution {
public:
    bool hasCycle(ListNode* head) {

        unordered_set<ListNode*> visited;

        while (head != nullptr) {

            if (visited.count(head)) {
                return true;
            }

            visited.insert(head);
            head = head->next;
        }

        return false;
    }
};
```

### Intuition

- Visit each node.
- Store its address in a Hash Set.
- If the same address appears again, a cycle exists.
- If we reach `nullptr`, no cycle exists.

### Complexity Comparison

| Approach | Time | Extra Space |
|---|---|---|
| Hash Set | O(n) | O(n) |
| Floyd's Algorithm | O(n) | O(1) |

**Interview preference:** Floyd's Cycle Detection because it uses constant extra space.

---

## 9. Complexity Analysis

Let `n` be the number of nodes.

**Time Complexity: O(n)**

- Without a cycle, the fast pointer reaches the end.
- With a cycle, the two pointers eventually meet.
- Each iteration performs a constant number of pointer operations.

**Space Complexity: O(1)**

Only two pointers are maintained.

---

## 10. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Moving both pointers one step | They may never catch each other |
| Checking only `fast != nullptr` | `fast->next` may still be null |
| Comparing node values instead of pointers | Different nodes can have identical values |
| Using `slow->val == fast->val` | Cycle detection requires comparing node identity |
| Forgetting the empty-list case | Can cause invalid pointer access |
| Returning false immediately when pointers are different | They may meet in a later iteration |

**Very important:**

```cpp
slow == fast
```

compares node addresses, not their stored values.

---

## 11. Edge Cases

| Linked List | Expected |
|---|---|
| `nullptr` | false |
| `1 → nullptr` | false |
| `1 → 2 → nullptr` | false |
| `1 → 1 (self-cycle)` | true |
| `1 → 2 → 3 → 2 ...` | true |
| `1 → 2 → 3 → 4 → nullptr` | false |

---

## 12. C++ Concepts to Remember

- `ListNode*`: Pointer to a linked-list node.
- `slow->next`: Moves one node forward.
- `fast->next->next`: Moves two nodes forward.
- `nullptr`: Represents the end of a linked list.
- `slow == fast`: Compares whether two pointers refer to the same node.
- `unordered_set<ListNode*>`: Stores visited node addresses.

---

## 13. Pattern Recognition for Interviews

Recognize **Slow and Fast Pointers** when:

- You need to detect a cycle in a linked list.
- You need to find the middle of a linked list.
- You need to determine whether a linked list has a circular structure.
- You need to solve a linked-list problem with O(1) extra space.

Related problems:

- Linked List Cycle
- Linked List Cycle II
- Middle of the Linked List
- Happy Number
- Reorder List

---

## 14. 30-Second Revision

1. Initialize `slow = head` and `fast = head`.
2. Run while `fast != nullptr && fast->next != nullptr`.
3. Move `slow` one step.
4. Move `fast` two steps.
5. If `slow == fast`, return `true`.
6. If the fast pointer reaches `nullptr`, return `false`.

**One-line interview explanation:**

"I use Floyd's Cycle Detection Algorithm with slow and fast pointers. The slow pointer moves one step while the fast pointer moves two steps. If a cycle exists, they eventually meet; otherwise, the fast pointer reaches nullptr. This takes O(n) time and O(1) extra space."

# 31. Reorder List

## 1. Problem Understanding

Rearrange a linked list in the following order:

- First node
- Last node
- Second node
- Second-last node
- Third node
- Third-last node
- Continue until all nodes are used.

**Example:**

```text
Input:
1 → 2 → 3 → 4 → 5

Output:
1 → 5 → 2 → 4 → 3
```

Another example:

```text
Input:
1 → 2 → 3 → 4

Output:
1 → 4 → 2 → 3
```

We must rearrange the existing nodes without changing their values.

## 2. Main Intuition

Think of the problem as three smaller problems:

1. Find the middle of the linked list.
2. Reverse the second half.
3. Merge the first and reversed second halves alternately.

This is an important linked-list pattern because it combines techniques used in other problems.

## 3. Visual Explanation

Consider:

```text
1 → 2 → 3 → 4 → 5
```

**Step 1: Find the middle**

Use slow and fast pointers.

```text
slow = 3
fast = 5

First half:  1 → 2 → 3
Second half: 4 → 5
```

**Step 2: Reverse the second half**

```text
Before:
4 → 5

After:
5 → 4
```

Now we have:

```text
First half:  1 → 2 → 3
Second half: 5 → 4
```

**Step 3: Merge alternately**

```text
Take 1, then 5
Take 2, then 4
Take 3
```

Final result:

```text
1 → 5 → 2 → 4 → 3
```

## 4. C++ Solution

```cpp
class Solution {
public:
    void reorderList(ListNode* head) {

        if (head == nullptr || head->next == nullptr) {
            return;
        }

        // Step 1: Find the middle
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast->next != nullptr &&
               fast->next->next != nullptr) {

            slow = slow->next;
            fast = fast->next->next;
        }

        // Step 2: Reverse the second half
        ListNode* second = slow->next;
        slow->next = nullptr;

        ListNode* prev = nullptr;
        ListNode* curr = second;

        while (curr != nullptr) {

            ListNode* nextNode = curr->next;

            curr->next = prev;

            prev = curr;
            curr = nextNode;
        }

        second = prev;

        // Step 3: Merge both halves alternately
        ListNode* first = head;

        while (second != nullptr) {

            ListNode* firstNext = first->next;
            ListNode* secondNext = second->next;

            first->next = second;
            second->next = firstNext;

            first = firstNext;
            second = secondNext;
        }
    }
};
```

## 5. Dry Run

Input:

```text
1 → 2 → 3 → 4 → 5
```

**After finding the middle:**

```text
slow = 3

1 → 2 → 3
4 → 5
```

**After reversing the second half:**

```text
1 → 2 → 3

5 → 4
```

**During merging:**

| Iteration | First node | Second node | Result |
|---|---:|---:|---|
| 1 | 1 | 5 | 1 → 5 |
| 2 | 2 | 4 | 1 → 5 → 2 → 4 |
| 3 | 3 | nullptr | No more merging needed |

Final linked list:

```text
1 → 5 → 2 → 4 → 3
```

## 6. Why Do We Set `slow->next = nullptr`?

This is very important.

Before splitting:

```text
1 → 2 → 3 → 4 → 5
```

After:

```cpp
ListNode* second = slow->next;
slow->next = nullptr;
```

We get two independent lists:

```text
First:  1 → 2 → 3 → nullptr

Second: 4 → 5 → nullptr
```

Without disconnecting the first half, merging can create unwanted links or cycles.

## 7. Complexity Analysis

Let `n` be the number of nodes.

| Operation | Complexity |
|---|---|
| Find middle | O(n) |
| Reverse second half | O(n) |
| Merge two halves | O(n) |
| **Total time** | **O(n)** |
| **Auxiliary space** | **O(1)** |

Why O(1) space?

We only use a fixed number of pointers. We do not create another linked list, array, or recursion stack.

## 8. Edge Cases

| Input | Expected output |
|---|---|
| `nullptr` | `nullptr` |
| `1` | `1` |
| `1 → 2` | `1 → 2` |
| `1 → 2 → 3` | `1 → 3 → 2` |
| `1 → 2 → 3 → 4` | `1 → 4 → 2 → 3` |

## 9. Common Interview Mistakes

1. Forgetting to disconnect the first half using `slow->next = nullptr`.
2. Reversing the entire list instead of only the second half.
3. Updating `first->next` before saving its original next node.
4. Updating `second->next` before saving its original next node.
5. Creating a new list instead of rearranging existing nodes.
6. Forgetting to handle `nullptr` and single-node lists.

**Important:** Always save `firstNext` and `secondNext` before changing pointers. Otherwise, you may lose access to the remaining nodes.

## 10. C++ Concepts to Remember

- `ListNode*`: Pointer to a linked-list node.
- `nullptr`: Represents an empty pointer.
- `slow` and `fast`: Two-pointer technique.
- `prev`, `curr`, `nextNode`: Used for reversing links.
- `firstNext`, `secondNext`: Preserve the remaining nodes during merging.

## 11. Pattern Recognition

Whenever a linked-list problem asks you to rearrange nodes from opposite ends, consider:

```text
Find Middle
     ↓
Reverse Second Half
     ↓
Merge Alternately
```

This problem combines three reusable linked-list techniques.

## 12. 30-Second Interview Revision

**Question:** How do you reorder a linked list in O(n) time and O(1) space?

**Answer:**

"I use three steps. First, I find the middle using slow and fast pointers. Second, I split the list and reverse its second half. Finally, I merge both halves alternately, taking one node from the first half and one from the reversed second half. This gives O(n) time complexity and O(1) auxiliary space."

**Remember the sequence:**

`Middle → Reverse → Merge`


# 32. Remove Nth Node From End of List

## 1. Problem Understanding

Given the head of a linked list and an integer `n`, remove the nth node from the end and return the updated head.

**Example 1:**

```text
Input:
head = 1 → 2 → 3 → 4 → 5
n = 2

Output:
1 → 2 → 3 → 5
```

The second node from the end is `4`, so we remove it.

**Example 2:**

```text
Input:
head = 1 → 2
n = 2

Output:
2
```

Here, the node to remove is the head itself.

## 2. Main Intuition

The challenge is that a singly linked list cannot move backwards.

We don't know the length of the list beforehand, so how can we find the nth node from the end efficiently?

**Use two pointers: `fast` and `slow`.**

Maintain a gap of exactly `n` nodes between them.

When `fast` reaches the end, `slow` will be positioned just before the node that needs to be removed.

## 3. Visual Explanation

Input:

```text
1 → 2 → 3 → 4 → 5
n = 2
```

**Step 1: Create a dummy node**

```text
dummy → 1 → 2 → 3 → 4 → 5
```

Why dummy?

It makes deleting the head node easier because the dummy node acts as the previous node of the head.

Initialize:

```cpp
ListNode* dummy = new ListNode(0);
dummy->next = head;

ListNode* fast = dummy;
ListNode* slow = dummy;
```

**Step 2: Move fast pointer n steps ahead**

For `n = 2`:

```text
dummy → 1 → 2 → 3 → 4 → 5
  S            F
```

The fast pointer is now two nodes ahead of slow.

**Step 3: Move both pointers together**

Continue until `fast->next == nullptr`.

```text
dummy → 1 → 2 → 3 → 4 → 5
                  S         F
```

Now:
- `slow` points to node `3`.
- The next node, `4`, is the node to remove.

**Step 4: Delete the target node**

```cpp
ListNode* nodeToDelete = slow->next;
slow->next = slow->next->next;
delete nodeToDelete;
```

Result:

```text
1 → 2 → 3 → 5
```

## 4. C++ Solution — Optimal Approach

```cpp
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {

        ListNode* dummy = new ListNode(0);
        dummy->next = head;

        ListNode* fast = dummy;
        ListNode* slow = dummy;

        // Step 1: Move fast n steps ahead
        for (int i = 0; i < n; i++) {
            fast = fast->next;
        }

        // Step 2: Move both pointers together
        while (fast->next != nullptr) {
            fast = fast->next;
            slow = slow->next;
        }

        // Step 3: Remove the target node
        ListNode* nodeToDelete = slow->next;
        slow->next = slow->next->next;

        delete nodeToDelete;
        head = dummy->next;

        delete dummy;

        return head;
    }
};
```

## 5. Dry Run

Input:

```text
1 → 2 → 3 → 4 → 5
n = 2
```

Initially:

```text
fast = dummy
slow = dummy
```

After moving fast two steps:

```text
dummy → 1 → 2 → 3 → 4 → 5
  S            F
```

Move both pointers together:

| Iteration | Slow | Fast |
|---|---:|---:|
| 1 | 1 | 3 |
| 2 | 2 | 4 |
| 3 | 3 | 5 |

Now `fast->next == nullptr`.

Therefore:

```text
slow = 3
slow->next = 4
```

Delete node `4`:

```text
1 → 2 → 3 → 5
```

## 6. Why Do We Need a Dummy Node?

Consider:

```text
1 → 2 → 3
n = 3
```

We need to delete the head node (`1`).

Without a dummy node, there is no previous node to update.

With a dummy node:

```text
dummy → 1 → 2 → 3
```

Now `slow` can stop at `dummy`, and we can simply update:

```cpp
slow->next = slow->next->next;
```

This handles head deletion without a separate special case.

## 7. Complexity Analysis

Let `n` be the number of nodes in the linked list.

| Complexity | Value | Reason |
|---|---|---|
| Time | O(n) | Fast and slow pointers traverse the list |
| Auxiliary space | O(1) | Only a fixed number of pointers |

## 8. Edge Cases

| Input | n | Expected output |
|---|---:|---|
| `1 → 2 → 3 → 4 → 5` | 2 | `1 → 2 → 3 → 5` |
| `1 → 2` | 2 | `2` |
| `1` | 1 | `nullptr` |
| `1 → 2 → 3` | 1 | `1 → 2` |

## 9. Common Interview Mistakes

1. Forgetting the dummy node, making head deletion more complicated.
2. Moving `fast` only `n-1` steps instead of `n` when both pointers start at dummy.
3. Stopping when `fast == nullptr` instead of `fast->next == nullptr` in this particular implementation.
4. Deleting the node before saving its pointer.
5. Forgetting to return the updated head (`dummy->next`).
6. Forgetting to free dynamically allocated nodes when using raw pointers.

## 10. C++ Concepts to Remember

- `ListNode*`: Pointer to a linked-list node.
- `dummy->next`: Stores the actual head.
- `slow->next`: The node that needs to be removed.
- `slow->next->next`: The node after the target.
- `delete`: Frees a dynamically allocated node.

## 11. Pattern Recognition

Whenever a problem asks for the nth node from the end of a singly linked list, think:

```text
Two Pointers
     ↓
Create a gap of n nodes
     ↓
Move both pointers together
     ↓
Slow reaches node before target
     ↓
Delete target
```

**Important:** Both pointers start at the dummy node in this implementation.

## 12. 30-Second Interview Revision

**Question:** How can you remove the nth node from the end in one pass?

**Answer:**

"I use a dummy node and two pointers. Both start at the dummy node. I move the fast pointer n steps ahead, then move both pointers together until fast reaches the last node. Slow is now just before the target node. I update its next pointer to skip the target and return the updated head. The solution takes O(n) time and O(1) auxiliary space."

**Remember:**

`Dummy → Gap of n → Move Together → Delete`

# 33. Copy List with Random Pointer

## 1. Problem Understanding

Each node contains three things:

- `val`: Node's value.
- `next`: Pointer to the next node.
- `random`: Pointer to any node in the list or `nullptr`.

We need to create a **deep copy** of the entire linked list.

A deep copy means:
- Create new nodes instead of reusing original nodes.
- Copy every node's value.
- Preserve all `next` connections.
- Preserve all `random` connections.
- Do not modify the original list.

### Example

Original list:

```text
Node A          Node B          Node C
  7     →         13     →        11
  ↓                ↓              ↓
random            random         random
  C                A              nullptr
```

The copied list must have the same structure, but every node must be a new object.

## 2. Main Intuition — Hash Map

The biggest challenge is the `random` pointer.

For example:

```text
Original A.random = Original C
```

We need:

```text
Copied A.random = Copied C
```

We cannot directly assign the original random pointer to the copied node because that would connect the copied list back to the original list.

**Solution:** Maintain a hash map:

```text
Original Node → Copied Node
```

Example:

```text
Original A → Copy A
Original B → Copy B
Original C → Copy C
```

Now we can easily find the copied version of any original node.

## 3. Three-Step Approach

### Step 1: Create copies of all nodes

Traverse the original list and create a new node for every original node.

Store the relationship in an unordered map.

```cpp
mp[curr] = new Node(curr->val);
```

After this step:

```text
Original: A → B → C

Copies:
A'   B'   C'
```

### Step 2: Connect next pointers

For every original node, connect its copy to the copy of its original next node.

```cpp
mp[curr]->next = mp[curr->next];
```

### Step 3: Connect random pointers

Similarly, connect each copied node's random pointer to the copied version of the original random target.

```cpp
mp[curr]->random = mp[curr->random];
```

**Important:** Using `mp[nullptr]` with `unordered_map` creates a default null pointer entry if one does not already exist. Therefore, null connections remain null.

## 4. C++ Solution — Hash Map Approach

```cpp
class Solution {
public:
    Node* copyRandomList(Node* head) {

        if (head == nullptr) {
            return nullptr;
        }

        unordered_map<Node*, Node*> mp;

        // Step 1: Create a copy of every node
        Node* curr = head;

        while (curr != nullptr) {
            mp[curr] = new Node(curr->val);
            curr = curr->next;
        }

        // Step 2 and 3: Connect next and random pointers
        curr = head;

        while (curr != nullptr) {

            mp[curr]->next = mp[curr->next];

            mp[curr]->random = mp[curr->random];

            curr = curr->next;
        }

        return mp[head];
    }
};
```

## 5. Dry Run

Consider:

```text
Original list:

A(7) → B(13) → C(11)

A.random = C
B.random = A
C.random = B
```

### Step 1: Create copied nodes

Hash map:

| Original node | Copied node |
|---|---|
| A(7) | A'(7) |
| B(13) | B'(13) |
| C(11) | C'(11) |

At this point, copied nodes exist, but their connections are not established.

### Step 2: Connect next pointers

```text
A' → B' → C'
```

### Step 3: Connect random pointers

Original:

```text
A.random = C
B.random = A
C.random = B
```

Using the hash map:

```text
A'.random = C'
B'.random = A'
C'.random = B'
```

Final copied list:

```text
A'(7) → B'(13) → C'(11)
  ↘       ↙       ↙
   C'    A'      B'
```

The copied list now has the same structure as the original, but all nodes are independent objects.

## 6. Complexity Analysis

Let `n` be the number of nodes.

| Operation | Complexity |
|---|---|
| Create copied nodes | O(n) |
| Connect next pointers | O(n) |
| Connect random pointers | O(n) |
| **Total time** | **O(n)** |
| **Auxiliary space** | **O(n)** |

Why O(n) space?

We store one mapping for every original node in the hash map.

## 7. Alternative — O(1) Auxiliary Space

There is another interesting approach using three passes without a hash map.

### Pass 1: Insert copied nodes between original nodes

Before:

```text
A → B → C
```

After:

```text
A → A' → B → B' → C → C'
```

### Pass 2: Assign random pointers

The copied node is always immediately after its original node.

Therefore:

```cpp
curr->next->random =
    curr->random == nullptr
    ? nullptr
    : curr->random->next;
```

Why does this work?

If `curr->random` points to original node C, then `curr->random->next` points to copied node C'.

### Pass 3: Separate the original and copied lists

Restore the original list and extract the copied list.

```text
Original: A → B → C

Copied:   A' → B' → C'
```

This approach takes O(n) time and O(1) auxiliary space.

**Note:** The newly allocated copied nodes are still required; O(1) refers to auxiliary space, excluding the output list.

## 8. Common Interview Mistakes

1. Copying only the `next` pointer and forgetting `random`.
2. Assigning the original random pointer directly to the copied node.
3. Returning `head` instead of `mp[head]`.
4. Forgetting to handle an empty list.
5. Not understanding why every copied node must be a new object.
6. Forgetting that random pointers can point to any node, including themselves or `nullptr`.

## 9. C++ Concepts to Remember

- `unordered_map<Node*, Node*>`: Maps original node addresses to copied node addresses.
- `curr->val`: Accesses the current node's value.
- `mp[curr]`: Gets the copied version of an original node.
- `nullptr`: Represents an absent pointer.
- `new Node(curr->val)`: Dynamically creates a new node.

## 10. Pattern Recognition

When a linked list contains pointers that can refer to arbitrary nodes, think about using a hash map.

```text
Original Node
      ↓
Hash Map
      ↓
Copied Node
      ↓
Recreate Connections
```

This technique is useful when copying complex structures containing references between objects.

## 11. 30-Second Interview Revision

**Question:** How do you deep-copy a linked list with random pointers?

**Answer:**

"I use an unordered map to maintain the relationship between original nodes and their copies. In the first traversal, I create a copy of every node and store the mapping. In the second traversal, I connect the copied nodes' next and random pointers using that map. This ensures all connections point to copied nodes rather than original nodes. The solution takes O(n) time and O(n) auxiliary space."

**Remember:**

`Create Copies → Connect Next → Connect Random`

# 34. Add Two Numbers

## 1. Problem Understanding

You are given two non-empty linked lists representing two non-negative integers.

Each node contains one digit, and the digits are stored in **reverse order**.

Add the two numbers and return their sum as a linked list, also in reverse order.

### Example 1

```text
Input:

l1 = 2 → 4 → 3
l2 = 5 → 6 → 4

Output:

7 → 0 → 8
```

Why?

```text
l1 = 342
l2 = 465

342 + 465 = 807
```

The answer is stored in reverse order:

```text
7 → 0 → 8
```

### Example 2

```text
Input:

l1 = 0
l2 = 0

Output:

0
```

### Example 3

```text
Input:

l1 = 9 → 9 → 9 → 9 → 9 → 9 → 9
l2 = 9 → 9 → 9 → 9

Output:

8 → 9 → 9 → 9 → 0 → 0 → 0 → 1
```

## 2. Main Intuition

Think about how you perform addition manually.

For example:

```text
  342
+ 465
-----
  807
```

Start from the rightmost digit:

1. 2 + 5 = 7
2. 4 + 6 = 10 → write 0, carry 1
3. 3 + 4 + 1 = 8

The linked lists already store digits in reverse order, so we can process both lists from their heads.

We need three important things:

- `carry`: Stores the carry from the previous addition.
- `sum`: Stores the sum of the current digits.
- `dummy`: Helps build the resulting linked list easily.

## 3. Visual Explanation

Input:

```text
l1 = 2 → 4 → 3
l2 = 5 → 6 → 4
```

### Step 1: Initialize pointers

```cpp
ListNode* dummy = new ListNode(0);
ListNode* curr = dummy;

int carry = 0;
```

### Step 2: Add corresponding digits

**Iteration 1:**

```text
2 + 5 + 0 = 7

Digit = 7
Carry = 0
```

Result:

```text
7
```

**Iteration 2:**

```text
4 + 6 + 0 = 10

Digit = 0
Carry = 1
```

Result:

```text
7 → 0
```

**Iteration 3:**

```text
3 + 4 + 1 = 8

Digit = 8
Carry = 0
```

Result:

```text
7 → 0 → 8
```

### Step 3: Return the result

```text
7 → 0 → 8
```

## 4. C++ Solution — Optimal Approach

```cpp
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {

        ListNode* dummy = new ListNode(0);
        ListNode* curr = dummy;

        int carry = 0;

        while (l1 != nullptr || l2 != nullptr || carry != 0) {

            int sum = carry;

            if (l1 != nullptr) {
                sum += l1->val;
                l1 = l1->next;
            }

            if (l2 != nullptr) {
                sum += l2->val;
                l2 = l2->next;
            }

            carry = sum / 10;

            int digit = sum % 10;

            curr->next = new ListNode(digit);
            curr = curr->next;
        }

        return dummy->next;
    }
};
```

## 5. Understanding the Important Code

### A. Why do we use a dummy node?

```cpp
ListNode* dummy = new ListNode(0);
ListNode* curr = dummy;
```

The dummy node acts as the starting point of the result list.

Without it, we would need separate logic to handle the first node.

### B. Why this while condition?

```cpp
while (l1 != nullptr || l2 != nullptr || carry != 0)
```

We continue while:

- The first list still has digits.
- OR the second list still has digits.
- OR a carry is still remaining.

The carry condition is important.

For example:

```text
9 → 9
9 → 9
```

The result is:

```text
8 → 9 → 1
```

Even after both input lists finish, we must create the final node containing `1`.

### C. How do we calculate the digit and carry?

```cpp
carry = sum / 10;
int digit = sum % 10;
```

Example:

```text
sum = 15

carry = 15 / 10 = 1
digit = 15 % 10 = 5
```

So we insert `5` and carry `1` to the next iteration.

### D. Why do we return `dummy->next`?

```cpp
return dummy->next;
```

The dummy node is only a helper node.

The actual answer starts from the node after it.

## 6. Dry Run

Input:

```text
l1 = 2 → 4 → 3
l2 = 5 → 6 → 4
```

Initially:

```text
carry = 0
result = empty
```

| Iteration | l1 digit | l2 digit | Sum | Digit inserted | Carry |
|---|---:|---:|---:|---:|---:|
| 1 | 2 | 5 | 7 | 7 | 0 |
| 2 | 4 | 6 | 10 | 0 | 1 |
| 3 | 3 | 4 | 8 | 8 | 0 |

Final result:

```text
7 → 0 → 8
```

## 7. Complexity Analysis

Let:
- `m` = number of nodes in the first list.
- `n` = number of nodes in the second list.

| Complexity | Value | Reason |
|---|---|---|
| Time | O(max(m, n)) | Each input node is processed once |
| Auxiliary space | O(1) | Only a fixed number of pointers and variables |
| Output space | O(max(m, n) + 1) | New result nodes are created, including a possible final carry |

The output nodes are not counted as auxiliary space.

## 8. Edge Cases

| Input | Expected output |
|---|---|
| `2 → 4 → 3`, `5 → 6 → 4` | `7 → 0 → 8` |
| `0`, `0` | `0` |
| `9 → 9`, `1` | `0 → 0 → 1` |
| `1 → 8`, `0` | `1 → 8` |
| `9 → 9 → 9`, `9` | `8 → 0 → 0 → 1` |

## 9. Common Interview Mistakes

1. Forgetting that digits are stored in reverse order.
2. Forgetting to include the previous carry in the current sum.
3. Using `sum / 10` as the digit instead of the carry.
4. Using `sum % 10` as the carry instead of the digit.
5. Stopping when both lists end without checking whether carry remains.
6. Assuming both linked lists have the same length.
7. Returning `dummy` instead of `dummy->next`.

## 10. C++ Concepts to Remember

- `ListNode*`: Pointer to a linked-list node.
- `new ListNode(digit)`: Creates a new result node.
- `/`: Integer division, used to calculate carry.
- `%`: Modulo operator, used to extract the current digit.
- `nullptr`: Indicates that a list has ended.
- `dummy` node: Simplifies linked-list construction.

## 11. Pattern Recognition

Whenever a problem involves adding numbers represented by linked lists, think:

```text
Traverse Both Lists
       ↓
Add Current Digits + Carry
       ↓
digit = sum % 10
       ↓
carry = sum / 10
       ↓
Create Result Node
       ↓
Repeat Until Lists and Carry Finish
```

This is the standard carry-management pattern.

## 12. 30-Second Interview Revision

**Question:** How do you add two numbers represented by reverse-order linked lists?

**Answer:**

"I traverse both linked lists simultaneously and maintain a carry variable. At each step, I add the available digits and the previous carry. I calculate the current digit using modulo 10 and the next carry using integer division by 10. I create a new node for each digit and continue until both lists are exhausted and no carry remains. The time complexity is O(max(m,n))."

**Remember:**

`Sum = l1 digit + l2 digit + carry`

`Digit = Sum % 10`

`Carry = Sum / 10`

# 35. Find the Duplicate Number

## 1. Problem Understanding

You are given an integer array `nums` containing `n + 1` integers, where every integer is in the range `[1, n]`.

There is exactly one repeated number, but it may appear more than twice.

Find and return the duplicate number.

**Important constraints:**
- Do not modify the array.
- Use only constant extra space.
- Solve the problem in O(n) time if possible.

### Example 1

```text
Input:
nums = [1, 3, 4, 2, 2]

Output:
2
```

### Example 2

```text
Input:
nums = [3, 1, 3, 4, 2]

Output:
3
```

## 2. Main Intuition — Floyd's Cycle Detection

You might wonder:

How can an array problem be solved using linked-list cycle detection?

The trick is to treat each array value as the next index.

Consider:

```text
nums = [1, 3, 4, 2, 2]
Index:  0  1  2  3  4
```

We define the movement as:

```cpp
next = nums[current];
```

Starting from index `0`:

```text
0 → 1 → 3 → 2 → 4 → 2 → 4 → 2 ...
```

Notice that we have entered a cycle:

```text
2 → 4 → 2 → 4 → ...
```

The cycle starts at index `2`, which is also the duplicate number.

This is exactly the kind of cycle Floyd's algorithm detects.

## 3. Visual Explanation

Array:

```text
Index:  0  1  2  3  4
Value:  1  3  4  2  2
```

Convert it into a pointer structure:

```text
0 → 1 → 3 → 2
          ↑   ↓
          └── 4
```

More precisely, the cycle is:

```text
2 → 4 → 2
```

The duplicate number is the entrance to this cycle.

## 4. C++ Solution — Floyd's Cycle Detection

```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {

        // Step 1: Detect the cycle
        int slow = nums[0];
        int fast = nums[0];

        do {
            slow = nums[slow];
            fast = nums[nums[fast]];

        } while (slow != fast);

        // Step 2: Find the entrance of the cycle
        slow = nums[0];

        while (slow != fast) {
            slow = nums[slow];
            fast = nums[fast];
        }

        return slow;
    }
};
```

## 5. Understanding the Code

### Step 1: Initialize two pointers

```cpp
int slow = nums[0];
int fast = nums[0];
```

Both pointers start at the first value.

### Step 2: Move pointers at different speeds

```cpp
slow = nums[slow];
fast = nums[nums[fast]];
```

- Slow moves one step.
- Fast moves two steps.

This guarantees that they eventually meet inside the cycle.

We use a `do-while` loop because both pointers start at the same position, but we need them to move before checking for a meeting.

### Step 3: Find the cycle entrance

```cpp
slow = nums[0];
```

Move slow back to the starting position.

Keep fast at the meeting position.

Now move both one step at a time:

```cpp
slow = nums[slow];
fast = nums[fast];
```

They will meet at the entrance of the cycle.

That entrance gives us the duplicate number.

## 6. Dry Run

Input:

```text
nums = [1, 3, 4, 2, 2]
```

### Phase 1: Detect the cycle

Initially:

```text
slow = nums[0] = 1
fast = nums[0] = 1
```

| Iteration | Slow movement | Fast movement | Slow | Fast |
|---|---|---|---:|---:|
| 1 | nums[1] | nums[nums[1]] | 3 | 2 |
| 2 | nums[3] | nums[nums[2]] | 2 | 4 |
| 3 | nums[2] | nums[nums[4]] | 4 | 4 |

They meet at index/value `4`.

### Phase 2: Find cycle entrance

Reset:

```text
slow = nums[0] = 1
fast = 4
```

Move both one step at a time:

| Iteration | Slow | Fast |
|---|---:|---:|
| Start | 1 | 4 |
| 1 | 3 | 2 |
| 2 | 2 | 4 |
| 3 | 4 | 2 |
| 4 | 2 | 4 |

Correction: The table above would not describe the actual pointer sequence because the pointers are reset and then moved one step each from their current positions. The actual progression is:

```text
Start: slow = 1, fast = 4
Step 1: slow = 3, fast = 2
Step 2: slow = 2, fast = 4
Step 3: slow = 4, fast = 2
Step 4: slow = 2, fast = 4
```

This reveals an important distinction: the meeting position from Phase 1 must be retained, and the standard Floyd algorithm's reset-and-walk guarantee applies when the initial traversal is modeled from the correct cycle start. For this array, the conventional algorithm actually meets at `4`, then resetting one pointer to `nums[0]` and moving both gives:

```text
slow: 1 → 3 → 2
fast: 4 → 2 → 4
```

They meet at `2` after the next move from `3` and `4` respectively:

```text
slow = nums[3] = 2
fast = nums[4] = 2
```

Duplicate number:

```text
2
```

## 7. Complexity Analysis

Let `n` be the number of elements minus one.

| Complexity | Value |
|---|---|
| Time | O(n) |
| Auxiliary space | O(1) |

Why O(1) space?

We only use two pointers and a fixed number of variables. We do not create a set, map, or another array.

## 8. Alternative Approaches

### Approach 1: Sorting

Sort the array and compare adjacent elements.

- Time: O(n log n)
- Extra space: Depends on sorting implementation.
- Violates the constraint because the array is modified.

### Approach 2: Hash Set

Store visited values in an unordered set.

```cpp
int findDuplicate(vector<int>& nums) {

    unordered_set<int> seen;

    for (int num : nums) {

        if (seen.count(num)) {
            return num;
        }

        seen.insert(num);
    }

    return -1;
}
```

- Average time: O(n)
- Extra space: O(n)
- Does not satisfy the constant-space requirement.

### Approach 3: Floyd's Cycle Detection

- Time: O(n)
- Auxiliary space: O(1)
- Does not modify the array.
- Meets the problem's constraints.

**Best interview answer: Floyd's Cycle Detection.**

## 9. Common Interview Mistakes

1. Using `nums[slow]` and `nums[fast]` incorrectly.
2. Moving fast only one step instead of two in Phase 1.
3. Forgetting to reset slow to `nums[0]` after detecting the cycle.
4. Returning the first meeting point instead of the cycle entrance.
5. Using a hash set despite the O(1) space requirement.
6. Modifying the original array when the problem prohibits it.

## 10. C++ Concepts to Remember

- `nums[index]`: Uses the value at an index as the next index.
- `slow = nums[slow]`: Moves one step.
- `fast = nums[nums[fast]]`: Moves two steps.
- `do-while`: Executes at least once before checking the condition.

## 11. Pattern Recognition

When an array has:
- `n + 1` elements,
- values restricted to `[1, n]`,
- exactly one repeated number,
- and a constant-space requirement,

think about Floyd's Cycle Detection.

```text
Array Values
     ↓
Treat Values as Pointers
     ↓
Detect Cycle
     ↓
Reset One Pointer
     ↓
Find Cycle Entrance
     ↓
Return Duplicate
```

## 12. 30-Second Interview Revision

**Question:** How do you find a duplicate number in O(n) time and O(1) space without modifying the array?

**Answer:**

"I treat each array value as a pointer to another index. This creates a linked-list-like structure with a cycle because one number is repeated. I use Floyd's slow and fast pointers to detect the cycle. After they meet, I reset one pointer to the starting position and move both pointers one step at a time. Their next meeting point is the duplicate number."

**Remember:**

`Treat as Linked List → Detect Cycle → Find Cycle Entrance`

# 36. LRU Cache

## 1. Problem Understanding

Design a data structure that follows the **LRU (Least Recently Used)** policy.

LRU means:

When the cache reaches its capacity and we need to insert a new item, remove the item that has not been used for the longest time.

We need to implement three functions:

- `LRUCache(int capacity)`: Initialize the cache with a fixed capacity.
- `get(int key)`: Return the value if the key exists; otherwise return `-1`. Accessing a key makes it recently used.
- `put(int key, int value)`: Insert or update a key-value pair. If capacity is exceeded, remove the least recently used item.

Both `get()` and `put()` must run in O(1) average time.

## 2. Main Intuition

We need two data structures working together.

### A. Hash Map

```cpp
unordered_map<int, Node*> mp;
```

It stores:

```text
Key → Address of Node
```

Why?

A hash map allows us to find a node in O(1) average time.

### B. Doubly Linked List

It maintains the usage order.

```text
Least Recently Used             Most Recently Used
       LRU                              MRU
        ↓                                ↓
       Node A ⇄ Node B ⇄ Node C
```

Why doubly linked list?

- We can remove any node in O(1) time.
- We can insert a node at the front in O(1) time.
- We can access both previous and next nodes.

**Important rule:**

- Front = Most Recently Used (MRU).
- Back = Least Recently Used (LRU).

## 3. Visual Understanding

Suppose:

```text
Capacity = 2
```

Initially:

```text
Cache = Empty
```

### Operation 1

```cpp
put(1, 10);
```

Cache:

```text
MRU                    LRU
 1
```

### Operation 2

```cpp
put(2, 20);
```

Cache:

```text
MRU                    LRU
 2 ⇄ 1
```

### Operation 3

```cpp
get(1);
```

Key `1` was accessed, so it becomes the most recently used.

```text
MRU                    LRU
 1 ⇄ 2
```

### Operation 4

```cpp
put(3, 30);
```

Capacity is already full.

The least recently used key is `2`, so remove it.

```text
Before:
1 ⇄ 2

After:
3 ⇄ 1
```

Key `2` is evicted.

## 4. Complete C++ Solution

We use:
- A custom doubly linked list node.
- A hash map.
- Two dummy nodes: `head` and `tail`.

```cpp
class LRUCache {

private:

    struct Node {
        int key;
        int value;

        Node* prev;
        Node* next;

        Node(int k, int v) {
            key = k;
            value = v;
            prev = nullptr;
            next = nullptr;
        }
    };

    int capacity;

    unordered_map<int, Node*> mp;

    Node* head;
    Node* tail;

    // Remove a node from the linked list
    void removeNode(Node* node) {

        Node* prevNode = node->prev;
        Node* nextNode = node->next;

        prevNode->next = nextNode;
        nextNode->prev = prevNode;
    }

    // Insert a node immediately after head
    void insertAtFront(Node* node) {

        Node* nextNode = head->next;

        head->next = node;
        node->prev = head;

        node->next = nextNode;
        nextNode->prev = node;
    }

    // Move an existing node to the front
    void moveToFront(Node* node) {

        removeNode(node);
        insertAtFront(node);
    }

public:

    LRUCache(int capacity) {

        this->capacity = capacity;

        head = new Node(0, 0);
        tail = new Node(0, 0);

        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {

        if (mp.find(key) == mp.end()) {
            return -1;
        }

        Node* node = mp[key];

        // Accessed node becomes recently used
        moveToFront(node);

        return node->value;
    }

    void put(int key, int value) {

        // If key already exists, update its value
        if (mp.find(key) != mp.end()) {

            Node* node = mp[key];

            node->value = value;

            moveToFront(node);

            return;
        }

        // Create a new node
        Node* newNode = new Node(key, value);

        mp[key] = newNode;

        insertAtFront(newNode);

        // If capacity is exceeded, remove LRU node
        if (mp.size() > capacity) {

            Node* lruNode = tail->prev;

            removeNode(lruNode);

            mp.erase(lruNode->key);

            delete lruNode;
        }
    }

    ~LRUCache() {

        Node* curr = head;

        while (curr != nullptr) {

            Node* nextNode = curr->next;

            delete curr;

            curr = nextNode;
        }
    }
};
```

## 5. Understanding the Helper Functions

### A. removeNode()

```cpp
void removeNode(Node* node) {

    Node* prevNode = node->prev;
    Node* nextNode = node->next;

    prevNode->next = nextNode;
    nextNode->prev = prevNode;
}
```

It disconnects a node from the doubly linked list.

Example:

```text
A ⇄ B ⇄ C
```

Remove B:

```text
A ⇄ C
```

No traversal is needed because every node knows its previous and next nodes.

### B. insertAtFront()

```cpp
void insertAtFront(Node* node) {

    Node* nextNode = head->next;

    head->next = node;
    node->prev = head;

    node->next = nextNode;
    nextNode->prev = node;
}
```

It inserts a node immediately after the dummy head.

This makes it the most recently used node.

### C. moveToFront()

```cpp
void moveToFront(Node* node) {

    removeNode(node);
    insertAtFront(node);
}
```

Whenever a key is accessed or updated, move its node to the front.

### D. Why do we use dummy head and tail?

Our list always looks like this:

```text
head ⇄ Node A ⇄ Node B ⇄ tail
```

Even when the cache is empty:

```text
head ⇄ tail
```

Dummy nodes simplify insertion and deletion because we do not need separate logic for the first and last actual nodes.

## 6. Dry Run

Consider:

```text
LRUCache cache(2);
```

| Operation | Result | Cache order (MRU → LRU) |
|---|---|---|
| `put(1, 10)` | Insert | 1 |
| `put(2, 20)` | Insert | 2 → 1 |
| `get(1)` | Returns 10 | 1 → 2 |
| `put(3, 30)` | Evicts key 2 | 3 → 1 |
| `get(2)` | Returns -1 | 3 → 1 |
| `put(4, 40)` | Evicts key 1 | 4 → 3 |
| `get(1)` | Returns -1 | 4 → 3 |
| `get(3)` | Returns 30 | 3 → 4 |
| `get(4)` | Returns 40 | 4 → 3 |

The important observation:

Every successful `get()` and every `put()` of an existing key changes the usage order.

## 7. Complexity Analysis

| Operation | Time Complexity |
|---|---|
| `get()` | O(1) average |
| `put()` | O(1) average |
| Remove node | O(1) |
| Insert at front | O(1) |
| Hash map lookup | O(1) average |

**Space complexity: O(capacity)**

We store at most `capacity` actual cache nodes and their corresponding hash-map entries, plus two dummy nodes.

## 8. Common Interview Mistakes

1. Using a singly linked list, which makes deletion of an arbitrary node difficult.
2. Forgetting to move a successfully accessed node to the front.
3. Forgetting to move an updated key to the front.
4. Removing the wrong node when capacity is exceeded.
5. Erasing the node from the linked list but forgetting to erase its hash-map entry.
6. Forgetting to delete an evicted node when using raw pointers.
7. Confusing the front and back positions of MRU and LRU.

## 9. C++ Concepts to Remember

- `unordered_map<int, Node*>`: Maps keys to node addresses.
- `struct Node`: Custom doubly linked list node.
- `Node*`: Pointer to a node.
- `this->capacity`: Refers to the current object's capacity member.
- `mp.erase(key)`: Removes a key from the hash map.
- `delete node`: Frees dynamically allocated memory.
- Destructor `~LRUCache()`: Frees the remaining nodes when the cache object is destroyed.

## 10. Pattern Recognition

Whenever a question requires:

- Fast key lookup.
- Fast insertion and deletion.
- Maintaining usage order.
- Removing the least recently used element.

Think:

```text
Hash Map + Doubly Linked List
```

Responsibilities:

```text
Hash Map
   ↓
Find node quickly

Doubly Linked List
   ↓
Maintain usage order
   ↓
Insert and delete quickly
```

## 11. 30-Second Interview Revision

**Question:** How do you implement an LRU Cache with O(1) get and put?

**Answer:**

"I combine an unordered map and a doubly linked list. The hash map stores each key and its corresponding node address, allowing O(1) average lookup. The doubly linked list maintains usage order, with the most recently used node at the front and the least recently used node at the back. Whenever a key is accessed or updated, I move its node to the front. When capacity is exceeded, I remove the node before the dummy tail and erase its key from the map."

**Remember:**

`Hash Map = Fast Lookup`

`Doubly Linked List = Usage Order`

`Front = MRU`

`Back = LRU`

# 37. Merge K Sorted Lists

## 1. Problem Understanding

You are given an array of `k` linked lists, where each linked list is sorted in ascending order.

Merge all the linked lists into one sorted linked list and return its head.

### Example

```text
Input:

lists = [
    1 → 4 → 5,
    1 → 3 → 4,
    2 → 6
]

Output:

1 → 1 → 2 → 3 → 4 → 4 → 5 → 6
```

The goal is to combine all nodes while maintaining ascending order.

## 2. Main Intuition

We already know how to merge two sorted linked lists.

But what if we have 10, 100, or 1000 sorted linked lists?

We need an efficient way to repeatedly find the smallest available node.

**Solution: Min Heap (Priority Queue).**

A min heap always gives us the smallest element.

We can use it like this:

1. Insert the first node of every non-empty list into the min heap.
2. Extract the smallest node.
3. Attach it to our result list.
4. Insert the extracted node's next node into the heap, if it exists.
5. Repeat until the heap becomes empty.

## 3. Visual Explanation

Input:

```text
List 1: 1 → 4 → 5
List 2: 1 → 3 → 4
List 3: 2 → 6
```

### Step 1: Insert the first node of every list

```text
Min Heap:

    1
   / \
  1   2
```

The heap contains the current smallest available node from each list.

### Step 2: Extract the smallest node

The smallest node is `1` from List 1.

Attach it to the result:

```text
Result: 1
```

Insert its next node (`4`) into the heap.

### Step 3: Continue extracting

The heap now contains:

```text
1, 2, 4
```

Extract `1`, attach it, and insert its next node (`3`).

```text
Result: 1 → 1
```

Continue the same process:

```text
Extract 2 → Result: 1 → 1 → 2
Extract 3 → Result: 1 → 1 → 2 → 3
Extract 4 → Result: 1 → 1 → 2 → 3 → 4
```

Keep going until the heap is empty.

Final result:

```text
1 → 1 → 2 → 3 → 4 → 4 → 5 → 6
```

## 4. C++ Solution — Min Heap Approach

```cpp
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {

        // Min heap to store node pointers
        auto cmp = [](ListNode* a, ListNode* b) {
            return a->val > b->val;
        };

        priority_queue<
            ListNode*,
            vector<ListNode*>,
            decltype(cmp)
        > pq(cmp);

        // Step 1: Insert first node of every non-empty list
        for (ListNode* node : lists) {

            if (node != nullptr) {
                pq.push(node);
            }
        }

        // Dummy node for the result list
        ListNode dummy(0);
        ListNode* tail = &dummy;

        // Step 2: Process the heap
        while (!pq.empty()) {

            // Get the smallest node
            ListNode* smallest = pq.top();
            pq.pop();

            // Attach it to the result
            tail->next = smallest;
            tail = tail->next;

            // Insert its next node into the heap
            if (smallest->next != nullptr) {
                pq.push(smallest->next);
            }
        }

        tail->next = nullptr;

        return dummy.next;
    }
};
```

## 5. Understanding the Important Code

### A. Why use a Min Heap?

```cpp
priority_queue<
    ListNode*,
    vector<ListNode*>,
    decltype(cmp)
> pq(cmp);
```

C++ `priority_queue` is a max heap by default.

We want a min heap, so we provide a custom comparator:

```cpp
auto cmp = [](ListNode* a, ListNode* b) {
    return a->val > b->val;
};
```

This makes the node with the smallest `val` appear at the top.

### B. Why insert only the first node of every list?

```cpp
for (ListNode* node : lists) {
    if (node != nullptr) {
        pq.push(node);
    }
}
```

Each linked list is already sorted.

Therefore, its first node is the smallest remaining node in that list.

We don't need to insert every node immediately. We insert the next node only after extracting the current node.

This keeps the heap size at most `k`.

### C. Why use a dummy node?

```cpp
ListNode dummy(0);
ListNode* tail = &dummy;
```

The dummy node simplifies building the result.

Whenever we extract a node:

```cpp
tail->next = smallest;
tail = tail->next;
```

We attach it to the end of the result.

### D. Why insert the next node?

```cpp
if (smallest->next != nullptr) {
    pq.push(smallest->next);
}
```

After removing a node from a sorted list, its next node becomes the smallest remaining candidate from that particular list.

We add it to the heap so it can compete with nodes from other lists.

## 6. Dry Run

Input:

```text
List 1: 1 → 4 → 5
List 2: 1 → 3 → 4
List 3: 2 → 6
```

Initially, the heap contains:

```text
[1, 1, 2]
```

| Step | Extracted node | Newly inserted node | Result |
|---|---:|---:|---|
| 1 | 1 | 4 | 1 |
| 2 | 1 | 3 | 1 → 1 |
| 3 | 2 | 6 | 1 → 1 → 2 |
| 4 | 3 | 4 | 1 → 1 → 2 → 3 |
| 5 | 4 | 5 | 1 → 1 → 2 → 3 → 4 |
| 6 | 4 | None | 1 → 1 → 2 → 3 → 4 → 4 |
| 7 | 5 | None | 1 → 1 → 2 → 3 → 4 → 4 → 5 |
| 8 | 6 | None | 1 → 1 → 2 → 3 → 4 → 4 → 5 → 6 |

Final output:

```text
1 → 1 → 2 → 3 → 4 → 4 → 5 → 6
```

## 7. Complexity Analysis

Let:

- `k` = Number of linked lists.
- `N` = Total number of nodes across all lists.

| Operation | Complexity |
|---|---|
| Insert initial nodes | O(k log k) |
| Extract each node | O(log k) |
| Insert next nodes | O(log k) |
| Process all N nodes | O(N log k) |
| Auxiliary space | O(k) |

**Final complexity:**

- Time: O(N log k)
- Auxiliary space: O(k)

Why O(k) space?

At any moment, the heap contains at most one candidate node from each linked list.

The result reuses existing nodes, so we don't create a separate list of N new nodes.

## 8. Alternative Approach — Divide and Conquer

Another way is to repeatedly merge pairs of sorted linked lists.

For example:

```text
Initially:

L1   L2   L3   L4

Round 1:

Merge(L1, L2)   Merge(L3, L4)

Round 2:

Merge(previous result 1, previous result 2)
```

Complexity:

- Time: O(N log k)
- Auxiliary space: O(1) iterative merging, excluding recursion or bookkeeping overhead.

This approach is also efficient and is a useful interview alternative.

## 9. Common Interview Mistakes

1. Inserting all nodes into the heap immediately instead of inserting one candidate per list.
2. Using the default max heap instead of a min heap.
3. Forgetting to check for `nullptr` before inserting a node.
4. Forgetting to insert the extracted node's next node.
5. Losing the result list by not maintaining a separate `tail` pointer.
6. Forgetting to terminate the result with `tail->next = nullptr`.

## 10. C++ Concepts to Remember

- `priority_queue`: Implements a heap.
- `vector<ListNode*>`: Stores node pointers in the heap.
- `decltype(cmp)`: Obtains the comparator's type.
- Lambda comparator: Defines how heap elements are ordered.
- `pq.top()`: Returns the smallest node with our comparator.
- `pq.pop()`: Removes the top node.
- `pq.push()`: Inserts a node.
- `ListNode dummy(0)`: Creates a stack-allocated dummy node.

## 11. Pattern Recognition

Whenever a problem asks you to combine multiple sorted sequences and repeatedly select the smallest available element, think:

**Min Heap / Priority Queue**

```text
K Sorted Lists
      ↓
Insert First Node of Each List
      ↓
Extract Smallest
      ↓
Attach to Result
      ↓
Insert Its Next Node
      ↓
Repeat Until Heap Empty
```

## 12. 30-Second Interview Revision

**Question:** How do you merge K sorted linked lists efficiently?

**Answer:**

"I use a min heap to maintain the smallest available node from each linked list. Initially, I insert the head of every non-empty list. I repeatedly extract the smallest node, attach it to the result, and insert its next node into the heap. Since the heap contains at most K nodes, each extraction or insertion costs O(log K). Processing N total nodes gives O(N log K) time and O(K) auxiliary space."

**Remember:**

`K Lists → Min Heap → Extract Minimum → Insert Next → Repeat`

# 38. Reverse Nodes in K-Group

## 1. Problem Understanding

Given the head of a linked list and an integer `k`, reverse the nodes of the linked list `k` at a time.

If the remaining nodes are fewer than `k`, leave them in their original order.

**Important:** We must change the node links, not the node values.

### Example 1

```text
Input:
head = 1 → 2 → 3 → 4 → 5
k = 2

Output:
2 → 1 → 4 → 3 → 5
```

Explanation:

```text
Group 1: 1 → 2 becomes 2 → 1
Group 2: 3 → 4 becomes 4 → 3
Remaining: 5 stays unchanged
```

### Example 2

```text
Input:
head = 1 → 2 → 3 → 4 → 5
k = 3

Output:
3 → 2 → 1 → 4 → 5
```

The last two nodes are fewer than `k`, so they remain unchanged.

## 2. Main Intuition

We already know how to reverse an entire linked list.

Now we need to apply that technique repeatedly to groups of `k` nodes.

For every group:

1. Check whether at least `k` nodes are available.
2. Identify the node immediately after the group.
3. Reverse exactly `k` nodes.
4. Connect the reversed group to the previous part.
5. Move to the next group.

The most important part is maintaining the connections between groups.

## 3. Visual Explanation

Input:

```text
1 → 2 → 3 → 4 → 5 → 6
k = 2
```

**Step 1: First group**

```text
1 → 2
```

Reverse:

```text
2 → 1
```

**Step 2: Second group**

```text
3 → 4
```

Reverse:

```text
4 → 3
```

Connect both groups:

```text
2 → 1 → 4 → 3
```

**Step 3: Third group**

```text
5 → 6
```

Reverse:

```text
6 → 5
```

Final result:

```text
2 → 1 → 4 → 3 → 6 → 5
```

For an incomplete final group, do not reverse it.

## 4. C++ Solution — Iterative Approach

We use a dummy node to simplify handling the head and a pointer called `groupPrev` to track the node before the current group.

```cpp
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {

        if (head == nullptr || k == 1) {
            return head;
        }

        ListNode dummy(0);
        dummy.next = head;

        ListNode* groupPrev = &dummy;

        while (true) {

            // Step 1: Find the kth node
            ListNode* kth = groupPrev;

            for (int i = 0; i < k && kth != nullptr; i++) {
                kth = kth->next;
            }

            // Not enough nodes remaining
            if (kth == nullptr) {
                break;
            }

            // Step 2: Save the next group's starting node
            ListNode* groupNext = kth->next;

            // Step 3: Reverse the current group
            ListNode* prev = groupNext;
            ListNode* curr = groupPrev->next;

            while (curr != groupNext) {

                ListNode* nextNode = curr->next;

                curr->next = prev;

                prev = curr;
                curr = nextNode;
            }

            // Step 4: Reconnect the reversed group
            ListNode* oldGroupStart = groupPrev->next;

            groupPrev->next = kth;

            groupPrev = oldGroupStart;
        }

        return dummy.next;
    }
};
```

## 5. Understanding the Important Pointers

There are five important pointers.

| Pointer | Responsibility |
|---|---|
| `groupPrev` | Node before the current group |
| `kth` | Last node of the current group |
| `groupNext` | First node after the current group |
| `prev` | Previous node during reversal |
| `curr` | Current node being reversed |

### A. Find the kth node

```cpp
ListNode* kth = groupPrev;

for (int i = 0; i < k && kth != nullptr; i++) {
    kth = kth->next;
}
```

This identifies the last node in the current group.

If `kth` becomes `nullptr`, fewer than `k` nodes remain.

### B. Save the next group

```cpp
ListNode* groupNext = kth->next;
```

We must save this pointer before reversing because we need to connect the reversed group to the remaining list.

### C. Reverse the current group

```cpp
ListNode* prev = groupNext;
ListNode* curr = groupPrev->next;
```

Why does `prev` start at `groupNext` instead of `nullptr`?

Because the reversed group's last node must point to the next group.

For example:

```text
Before:
1 → 2 → 3 → 4

Current group: 1 → 2
Next group: 3 → 4
```

After reversing:

```text
2 → 1 → 3 → 4
```

Starting `prev` at `groupNext` automatically creates the connection.

### D. Reconnect the group

```cpp
ListNode* oldGroupStart = groupPrev->next;

groupPrev->next = kth;

groupPrev = oldGroupStart;
```

After reversal:

- `kth` becomes the new first node.
- `oldGroupStart` becomes the last node.
- The next group begins after `oldGroupStart`.

Therefore, the old starting node becomes the previous group's tail.

## 6. Dry Run

Input:

```text
1 → 2 → 3 → 4 → 5
k = 2
```

Initially:

```text
dummy → 1 → 2 → 3 → 4 → 5
  ↑
groupPrev
```

### Iteration 1

Find `kth`:

```text
kth = 2
groupNext = 3
```

Reverse:

```text
2 → 1 → 3 → 4 → 5
```

Reconnect:

```text
dummy → 2 → 1 → 3 → 4 → 5
              ↑
           groupPrev
```

### Iteration 2

Find `kth`:

```text
kth = 4
groupNext = 5
```

Reverse:

```text
4 → 3 → 5
```

Reconnect:

```text
dummy → 2 → 1 → 4 → 3 → 5
                        ↑
                     groupPrev
```

### Iteration 3

Only one node remains:

```text
5
```

We cannot find two nodes, so we stop without reversing it.

Final result:

```text
2 → 1 → 4 → 3 → 5
```

## 7. Complexity Analysis

Let `n` be the total number of nodes.

| Complexity | Value | Explanation |
|---|---|---|
| Time | O(n) | Each node is visited a constant number of times |
| Auxiliary space | O(1) | Only a fixed number of pointers are used |

We do not create new linked-list nodes for the groups.

## 8. Edge Cases

| Input | k | Expected output |
|---|---:|---|
| `1 → 2 → 3 → 4 → 5` | 2 | `2 → 1 → 4 → 3 → 5` |
| `1 → 2 → 3 → 4 → 5` | 3 | `3 → 2 → 1 → 4 → 5` |
| `1 → 2 → 3` | 3 | `3 → 2 → 1` |
| `1 → 2` | 3 | `1 → 2` |
| `1` | 1 | `1` |
| `nullptr` | 2 | `nullptr` |

## 9. Common Interview Mistakes

1. Reversing the entire linked list instead of only `k` nodes.
2. Forgetting to check whether `k` nodes are available.
3. Forgetting to save `groupNext` before reversing.
4. Starting `prev` at `nullptr`, which can break the connection to the next group.
5. Forgetting to reconnect the previous group to the reversed group.
6. Moving `groupPrev` to the new group's first node instead of its last node.
7. Reversing the incomplete final group.

## 10. C++ Concepts to Remember

- `ListNode*`: Pointer to a linked-list node.
- `ListNode dummy(0)`: Stack-allocated dummy node.
- `groupPrev`: Tracks the previous group's ending node.
- `groupNext`: Preserves the remaining list.
- `curr->next = prev`: Reverses a pointer.
- `while (curr != groupNext)`: Restricts reversal to the current group.

## 11. Pattern Recognition

Whenever a problem asks you to reverse nodes in fixed-size groups, think:

```text
Find Kth Node
     ↓
Check Group Size
     ↓
Save Next Group
     ↓
Reverse K Nodes
     ↓
Reconnect Groups
     ↓
Move to Next Group
```

This problem is an extension of Reverse Linked List, but requires careful pointer management between groups.

## 12. 30-Second Interview Revision

**Question:** How do you reverse a linked list in groups of K nodes?

**Answer:**

"I use an iterative approach with a dummy node and a groupPrev pointer. First, I check whether a complete group of K nodes exists. Then I save the node after that group, reverse exactly K nodes, and reconnect the reversed group to the remaining list. I move groupPrev to the old starting node, which becomes the tail of the reversed group. If fewer than K nodes remain, I leave them unchanged. The solution takes O(n) time and O(1) auxiliary space."

**Remember:**

`Find K → Save Next → Reverse K → Reconnect → Repeat`
