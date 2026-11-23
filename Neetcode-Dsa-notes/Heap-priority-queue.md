# 73. Kth Largest Element in a Stream

## 1. Problem Understanding

You are given:

- An integer `k`.
- An initial array `nums`.
- A continuous stream of new integers.

Design a class `KthLargest` with two operations:

1. **Constructor:** Initialize the object with `k` and the initial numbers.
2. **add(val):** Add a new integer to the stream and return the kth largest element.

**Important rules:**

1. The stream is not necessarily sorted.
2. Duplicate values are allowed.
3. The kth largest element must be calculated after every insertion.
4. The stream always contains at least `k` elements when `add()` is called.

**Example**

```cpp
k = 3;
nums = [4, 5, 8, 2];
```

Operations:

```cpp
add(3); // returns 4
add(5); // returns 5
add(10); // returns 5
add(9); // returns 8
add(4); // returns 8
```

**Output:**

```text
[null, 4, 5, 5, 8, 8]
```

Explanation:

We need to return the third largest element after each insertion.

---

## 2. Core Intuition

We use a **Min Heap (Priority Queue)**.

The main idea is to maintain only the K largest elements from the entire stream.

Why?

Because we only need the kth largest element.

We do not need to store or sort every element to find it.

Consider:

```cpp
k = 3;
nums = [4, 5, 8, 2];
```

The three largest elements are:

```text
8, 5, 4
```

If we maintain these three elements in a Min Heap:

```text
       4
      / \
     8   5
```

The smallest element among these three is `4`.

Therefore:

```cpp
minHeap.top() == 4
```

This is the third largest element in the complete stream.

**The central idea:**

1. Insert every number into a Min Heap.
2. If the heap size becomes greater than `k`, remove the smallest element.
3. Keep only the K largest elements.
4. The heap's top element is always the kth largest.

---

## 3. Complete C++ Solution

```cpp
class KthLargest {
private:

    int k;

    // Min Heap.
    priority_queue<int, vector<int>, greater<int>> minHeap;

public:

    KthLargest(int k, vector<int>& nums) {

        this->k = k;

        // Insert all initial numbers.
        for (int num : nums) {

            minHeap.push(num);

            // Maintain at most k elements.
            if (minHeap.size() > k) {
                minHeap.pop();
            }
        }
    }

    int add(int val) {

        // Insert the new value.
        minHeap.push(val);

        // Remove the smallest if size exceeds k.
        if (minHeap.size() > k) {
            minHeap.pop();
        }

        // The top is the kth largest element.
        return minHeap.top();
    }
};
```

---

## 4. Understanding the Min Heap

A Min Heap is a data structure where:

- The smallest element is always at the top.
- Insertion takes O(log n).
- Removing the smallest element takes O(log n).
- Accessing the smallest element takes O(1).

In C++, we create a Min Heap using:

```cpp
priority_queue<int, vector<int>, greater<int>> minHeap;
```

By default, C++ `priority_queue` creates a Max Heap.

But here we use:

```cpp
greater<int>
```

to create a Min Heap.

Example:

```text
Elements: 4, 5, 8

Min Heap:

       4
      / \
     5   8
```

The smallest element is always available through:

```cpp
minHeap.top();
```

---

## 5. Why Do We Maintain Only K Elements?

This is the most important concept in the problem.

Suppose:

```cpp
k = 3;

Stream = [4, 5, 8, 2, 3, 5, 10]
```

We only need the three largest elements.

After processing the stream, the three largest values are:

```text
10, 8, 5
```

We do not need smaller values such as:

```text
2, 3, 4
```

They cannot become the third largest while these three larger values remain in the stream.

Therefore, we remove the smallest element whenever the heap size exceeds `k`.

This ensures that the heap always contains the K largest elements seen so far.

---

## 6. Understanding the Most Important Lines

### A. Insert the new element

```cpp
minHeap.push(val);
```

Every new value must be considered because it might become one of the K largest elements.

### B. Maintain heap size

```cpp
if (minHeap.size() > k) {
    minHeap.pop();
}
```

Why?

Because we only want to maintain the K largest elements.

If the heap contains `k + 1` elements, we remove the smallest.

### C. Return the kth largest element

```cpp
return minHeap.top();
```

After maintaining exactly `k` elements, the smallest among them is the kth largest element in the complete stream.

---

## 7. Complete Dry Run

**Input:**

```cpp
k = 3;
nums = [4, 5, 8, 2];
```

Initially:

```text
minHeap = []
```

### Step 1: Insert 4

```cpp
minHeap.push(4);
```

Heap:

```text
[4]
```

Size is not greater than `k`.

### Step 2: Insert 5

```cpp
minHeap.push(5);
```

Heap:

```text
[4, 5]
```

Size is not greater than `k`.

### Step 3: Insert 8

```cpp
minHeap.push(8);
```

Heap:

```text
       4
      / \
     5   8
```

Size is exactly `3`.

The kth largest element is:

```cpp
minHeap.top() = 4
```

### Step 4: Insert 2

```cpp
minHeap.push(2);
```

Now the heap contains four elements.

The size exceeds `k`.

Remove the smallest element:

```cpp
minHeap.pop();
```

The value `2` is removed.

Heap:

```text
       4
      / \
     5   8
```

Return:

```text
4
```

### Step 5: add(3)

Insert `3`.

The heap temporarily contains:

```text
3, 4, 5, 8
```

Remove the smallest element `3`.

Remaining elements:

```text
4, 5, 8
```

Return:

```text
4
```

### Step 6: add(5)

Insert `5`.

The heap temporarily contains:

```text
4, 5, 5, 8
```

Remove the smallest element `4`.

Remaining elements:

```text
5, 5, 8
```

Return:

```text
5
```

### Step 7: add(10)

Insert `10`.

The heap temporarily contains:

```text
5, 5, 8, 10
```

Remove the smallest element `5`.

Remaining elements:

```text
5, 8, 10
```

Return:

```text
5
```

### Step 8: add(9)

Insert `9`.

The heap temporarily contains:

```text
5, 8, 9, 10
```

Remove the smallest element `5`.

Remaining elements:

```text
8, 9, 10
```

Return:

```text
8
```

### Step 9: add(4)

Insert `4`.

The heap temporarily contains:

```text
4, 8, 9, 10
```

Remove the smallest element `4`.

Remaining elements:

```text
8, 9, 10
```

Return:

```text
8
```

**Final output:**

```text
[null, 4, 5, 5, 8, 8]
```

---

## 8. Visualizing the Min Heap

For:

```cpp
k = 3;
```

Imagine the stream contains:

```text
[4, 5, 8, 2, 3, 5, 10]
```

We maintain only the three largest elements.

```text
After processing 4, 5, 8:

       4
      / \
     5   8

After adding 2:

       4
      / \
     5   8

2 is removed because it is the smallest.

After adding 10:

       8
      / \
     10  9

The heap contains the three largest values.
```

The important observation:

```cpp
minHeap.top()
```

always gives the kth largest element once the heap contains at least `k` elements.

---

## 9. Why Does This Approach Work?

Suppose the heap contains exactly `k` elements.

These are the K largest elements seen so far.

Now a new value arrives.

There are two possibilities:

**Case 1: The new value is smaller than the heap's top.**

It cannot be among the K largest elements.

After insertion, it becomes the smallest element and gets removed.

**Case 2: The new value is greater than the heap's top.**

It may belong to the K largest elements.

After insertion, the previous smallest element is removed.

Therefore, the heap continues to contain the K largest elements.

Since the heap is a Min Heap, its top is the smallest among those K largest elements.

That is exactly the kth largest element.

---

## 10. Complexity Analysis

Let:

- `n` = number of initial elements.
- `k` = required position.

### Constructor

We process each initial element.

Each insertion and possible removal takes O(log k).

Time complexity:

\[
\boxed{O(n \log k)}
\]

### add() Operation

Each call performs:

- One heap insertion: O(log k).
- At most one heap removal: O(log k).
- One top operation: O(1).

Therefore:

\[
\boxed{O(\log k)}
\]

### Space Complexity

We maintain at most `k` elements.

\[
\boxed{O(k)}
\]

---

## 11. Common Interview Mistakes

1. Using a Max Heap instead of a Min Heap: We need the smallest among the K largest elements.
2. Keeping every stream element: This uses unnecessary memory.
3. Forgetting to remove the smallest element: The heap must never exceed size `k`.
4. Returning `minHeap.top()` before maintaining the heap size: The heap must contain exactly `k` elements.
5. Ignoring duplicate values: Duplicates count as separate elements when determining the kth largest.
6. Sorting the entire stream after every insertion: This is unnecessary.
7. Confusing kth largest with kth smallest: The Min Heap is used here to track the K largest elements.

---

## 12. Min Heap vs Max Heap

This comparison is important for interviews.

| Feature | Min Heap | Max Heap |
|---|---|---|
| Top element | Smallest | Largest |
| C++ declaration | `greater<int>` | Default `priority_queue<int>` |
| Main use here | Maintain K largest elements | Not the preferred approach |
| Heap size | K | Could require storing more elements |

**Remember:**

- To find the kth largest element, maintain a Min Heap of size `k`.
- To find the kth smallest element, maintain a Max Heap of size `k`.

---

## 13. Pattern Recognition for Interviews

Think about this pattern when a problem asks you to:

- Find the kth largest element.
- Process numbers arriving continuously.
- Maintain the top K elements.
- Avoid sorting the complete collection repeatedly.
- Efficiently handle new incoming values.

A useful question:

**Do I need to maintain only the K largest elements from a changing stream?**

If yes, a Min Heap of size K is a strong approach.

---

## 14. 30-Second Interview Revision

**Q1. Why use a Min Heap?**

Because we need the smallest element among the K largest elements.

**Q2. Why maintain only K elements?**

Because smaller elements outside the K largest cannot affect the kth largest result.

**Q3. Why remove the heap's top when size exceeds K?**

The top is the smallest element, so removing it keeps only the K largest elements.

**Q4. What does `minHeap.top()` return?**

The kth largest element once the heap contains K elements.

**Q5. What is the time complexity of `add()`?**

O(log k).

**Q6. What is the space complexity?**

O(k).

**Q7. Do duplicates count?**

Yes. Every occurrence is counted separately.

**Final takeaway**

```text
KTH LARGEST IN A STREAM = MIN HEAP OF SIZE K

1. Create a Min Heap.
2. Insert initial numbers.
3. If heap size > k:
       Remove the smallest element.

For every add(val):

1. Insert val.
2. If heap size > k:
       Remove the smallest element.
3. Return minHeap.top().
```
