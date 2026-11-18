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

