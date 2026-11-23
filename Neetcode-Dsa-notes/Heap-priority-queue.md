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
# 1046. Last Stone Weight

- **Difficulty:** Easy
- **Pattern:** Heap / Priority Queue
- **Approach:** Max Heap
- **Language:** C++

## 1. Problem Understanding

We are given an array `stones`, where each element represents the weight of a stone.

At every step:

1. Pick the two heaviest stones, `x` and `y`.
2. If `x == y`, both stones are destroyed.
3. If `x != y`, the smaller stone is destroyed, and the larger stone gets the remaining weight `y - x`.
4. Continue until at most one stone remains.

Return the weight of the last remaining stone, or `0` if no stones remain.

### Example 1

**Input:**

```cpp
stones = {2, 3, 6, 2, 4};
```

**Output:**

```cpp
1
```

**Explanation:**

- Smash 6 and 4. Remaining weight = 2.
- Smash 3 and 2. Remaining weight = 1.
- Smash 2 and 2. Both are destroyed.
- Only stone with weight 1 remains.

### Example 2

**Input:**

```cpp
stones = {1, 2};
```

**Output:**

```cpp
1
```

## 2. Brute Force Approach — Sorting

### Idea

Since we need the two largest elements repeatedly, we can use sorting.

1. Sort the array in ascending order.
2. Extract the two largest elements.
3. Compare their weights.
4. Insert the difference if they are unequal.
5. Repeat until at most one element remains.

### C++ Code

```cpp
class Solution {
public:
    int lastStoneWeight(vector<int>& stones) {

        while (stones.size() > 1) {

            sort(stones.begin(), stones.end());

            int y = stones.back();
            stones.pop_back();

            int x = stones.back();
            stones.pop_back();

            if (x != y) {
                stones.push_back(y - x);
            }
        }

        return stones.empty() ? 0 : stones[0];
    }
};
```

### Complexity Analysis

- Sorting takes `O(n log n)` per iteration.
- There can be at most `O(n)` iterations.

**Time Complexity:** `O(n² log n)`

**Space Complexity:** `O(1)` auxiliary space apart from sorting overhead.

### Limitation

Sorting the entire array repeatedly is inefficient because we only need the two largest elements.

We can optimize this using a Max Heap.

## 3. Optimized Approach — Max Heap

### Core Concept: Priority Queue

A Priority Queue is a data structure that provides access to the highest-priority element.

In C++, a normal priority queue is a Max Heap by default.

```cpp
priority_queue<int> pq;
```

### Important Operations

| Operation | Description | Complexity |
|---|---|---|
| `pq.push(x)` | Insert an element | `O(log n)` |
| `pq.top()` | Access largest element | `O(1)` |
| `pq.pop()` | Remove largest element | `O(log n)` |
| `pq.empty()` | Check if empty | `O(1)` |
| `pq.size()` | Return number of elements | `O(1)` |

### Why Max Heap?

The problem repeatedly asks us to select the two heaviest stones.

A Max Heap keeps the largest element at the top, allowing us to retrieve it efficiently.

### Algorithm

1. Insert all stones into a Max Heap.
2. While the heap contains at least two stones:
   - Extract the largest stone `y`.
   - Extract the second-largest stone `x`.
   - If `x != y`, insert `y - x`.
   - Otherwise, both stones are destroyed.
3. Return the remaining element or `0`.

### Optimized C++ Solution

```cpp
class Solution {
public:
    int lastStoneWeight(vector<int>& stones) {

        priority_queue<int> pq;

        // Step 1: Insert all stones into Max Heap
        for (int stone : stones) {
            pq.push(stone);
        }

        // Step 2: Smash the two heaviest stones
        while (pq.size() > 1) {

            int y = pq.top();
            pq.pop();

            int x = pq.top();
            pq.pop();

            // Step 3: Insert the remaining weight
            if (x != y) {
                pq.push(y - x);
            }
        }

        // Step 4: Return the remaining stone
        return pq.empty() ? 0 : pq.top();
    }
};
```

## 4. Dry Run — Step by Step

**Input:**

```cpp
stones = {2, 3, 6, 2, 4};
```

Initially, the Max Heap contains:

```text
       6
      / \
     4   3
    / \
   2   2
```

### Step-by-step Execution

| Step | Two Largest Stones | Operation | Remaining Stones |
|---|---|---|---|
| 1 | 6, 4 | 6 - 4 = 2 | 3, 2, 2, 2 |
| 2 | 3, 2 | 3 - 2 = 1 | 2, 2, 1 |
| 3 | 2, 2 | Both destroyed | 1 |

**Final Output:**

```cpp
1
```

Note: The internal arrangement of a heap can differ, but its largest element is always available at the top.

## 5. Time and Space Complexity

Let `n` be the number of stones.

### Time Complexity

- Inserting all elements into the heap: `O(n log n)`.
- Each iteration performs at most two removals and one insertion.
- There can be at most `n - 1` iterations.
- Each heap operation takes `O(log n)`.

**Overall Time Complexity: `O(n log n)`**

### Space Complexity

The heap can contain at most `n` elements.

**Space Complexity: `O(n)`**

## 6. Important Interview Insights

### Q1. Why do we use a Max Heap instead of a Min Heap?

Because we repeatedly need to retrieve the two largest elements.

A Max Heap provides the largest element at the top in `O(1)` time.

### Q2. Why do we call `pop()` twice?

Because we need to remove both selected stones before processing their collision.

### Q3. Why do we insert only `y - x`?

The smaller stone is destroyed, while the larger stone survives with the difference in weight.

### Q4. What happens if both stones have equal weights?

Both stones are destroyed, so we do not insert anything back into the heap.

### Q5. What happens if the heap becomes empty?

Return `0` because no stones remain.

## 7. Common Mistakes

- Using a Min Heap instead of a Max Heap.
- Forgetting to remove both extracted stones.
- Inserting the difference even when both stones have equal weights.
- Using `x - y` instead of `y - x`.
- Forgetting to handle an empty heap before returning the result.

## 8. Key Takeaways

Whenever a problem repeatedly asks you to retrieve the largest or smallest elements, consider using a Heap / Priority Queue.

| Heap Type | Purpose |
|---|---|
| Max Heap | Largest element is available first |
| Min Heap | Smallest element is available first |

### Important C++ Operations

- `top()` → Access the highest-priority element.
- `pop()` → Remove the highest-priority element.
- `push()` → Insert a new element.

### Pattern Recognition

**Repeatedly extracting maximum or minimum elements → Heap is a strong candidate.**

### Connection With Other NeetCode Problems

- Last Stone Weight → Max Heap.
- Kth Largest Element in a Stream → Min Heap of size K.
- Top K Frequent Elements → Heap or Bucket Sort.
- Find Median from Data Stream → Two Heaps.

**Revision Point:** Understand why this problem requires a Max Heap instead of memorizing the implementation.

# 973. K Closest Points to Origin

- **Difficulty:** Medium
- **Pattern:** Heap / Priority Queue
- **Approach:** Sorting and Max Heap
- **Language:** C++

## 1. Problem Understanding

We are given:

- A 2D array `points`, where each point is represented as `[x, y]`.
- An integer `k`.

We need to return the `k` points closest to the origin `(0, 0)`.

The Euclidean distance between a point `(x, y)` and the origin is:

Distance = sqrt(x² + y²)

**Important:** We do not need to calculate the square root because comparing squared distances gives the same ordering.

We can simply use:

```cpp
distance = x * x + y * y;
```

### Example 1

**Input:**

```cpp
points = {{0, 2}, {2, 2}};
k = 1;
```

**Output:**

```cpp
{{0, 2}}
```

**Explanation:**

- Distance of `(0, 2)` = 0² + 2² = 4
- Distance of `(2, 2)` = 2² + 2² = 8

The closest point is `(0, 2)`.

### Example 2

**Input:**

```cpp
points = {{0, 2}, {2, 0}, {2, 2}};
k = 2;
```

**Output:**

```cpp
{{0, 2}, {2, 0}}
```

Both points have squared distance 4 from the origin.

The output can be returned in any order.

## 2. Brute Force Approach — Sorting

### Idea

We can calculate the distance of every point from the origin and sort all points based on their distances.

Steps:

1. Calculate the squared distance of every point.
2. Store the distance along with its corresponding point.
3. Sort all points in ascending order of distance.
4. Select the first `k` points.
5. Return those points.

### C++ Code

```cpp
class Solution {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {

        vector<pair<int, vector<int>>> arr;

        // Calculate squared distance
        for (auto &point : points) {

            int x = point[0];
            int y = point[1];

            int dist = x * x + y * y;

            arr.push_back({dist, point});
        }

        // Sort based on distance
        sort(arr.begin(), arr.end());

        vector<vector<int>> ans;

        // Select first k closest points
        for (int i = 0; i < k; i++) {
            ans.push_back(arr[i].second);
        }

        return ans;
    }
};
```

### Complexity Analysis

Let:
- `n` = Total number of points.
- `k` = Number of closest points required.

**Time Complexity:** `O(n log n)`

- Calculating distances: `O(n)`.
- Sorting all points: `O(n log n)`.
- Extracting k points: `O(k)`.

**Space Complexity:** `O(n)`

We store all points along with their squared distances.

### Limitation

We sort all `n` points even though we only need the closest `k` points.

Can we avoid sorting the entire array?

Yes! We can use a Max Heap of size `k`.

## 3. Optimized Approach — Max Heap of Size K

### Core Concept: Why Max Heap?

We need the `k` closest points.

Imagine we maintain a collection of only `k` closest points.

Whenever a new point comes:

1. Insert it into the heap.
2. If the heap size becomes greater than `k`, remove the point that is farthest from the origin.

Why remove the farthest point?

Because we only want to retain the `k` closest points.

A Max Heap is useful because it keeps the farthest point at the top.

This allows us to remove the farthest point efficiently.

### Important C++ Concept

```cpp
priority_queue<pair<int, vector<int>>> pq;
```

A normal C++ priority queue is a Max Heap.

For pairs, comparison happens using the first element first.

Here:

- `first` = Squared distance.
- `second` = Actual point.

Therefore, the point with the largest squared distance stays at the top.

### Algorithm

1. Create an empty Max Heap.
2. Traverse every point.
3. Calculate its squared distance.
4. Insert `{distance, point}` into the heap.
5. If heap size exceeds `k`, remove the top element.
6. After processing all points, the heap contains the `k` closest points.
7. Extract all points from the heap and return them.

### Optimized C++ Solution

```cpp
class Solution {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {

        // Max Heap: Largest distance stays at the top
        priority_queue<pair<int, vector<int>>> pq;

        for (auto &point : points) {

            int x = point[0];
            int y = point[1];

            // Calculate squared distance
            int dist = x * x + y * y;

            // Insert distance and corresponding point
            pq.push({dist, point});

            // Maintain heap size K
            if (pq.size() > k) {
                pq.pop();
            }
        }

        vector<vector<int>> ans;

        // Extract K closest points
        while (!pq.empty()) {

            ans.push_back(pq.top().second);
            pq.pop();
        }

        return ans;
    }
};
```

## 4. Dry Run — Step by Step

**Input:**

```cpp
points = {{0, 2}, {2, 0}, {2, 2}};
k = 2;
```

### Step 1: Process (0, 2)

Squared distance:

```cpp
0 * 0 + 2 * 2 = 4
```

Heap:

```text
Distance: 4
Point: (0, 2)
```

Heap size = 1.

No removal required.

### Step 2: Process (2, 0)

Squared distance:

```cpp
2 * 2 + 0 * 0 = 4
```

Heap:

```text
(4, (0, 2))
(4, (2, 0))
```

Heap size = 2.

No removal required.

### Step 3: Process (2, 2)

Squared distance:

```cpp
2 * 2 + 2 * 2 = 8
```

Heap:

```text
(8, (2, 2))
(4, (0, 2))
(4, (2, 0))
```

Heap size = 3.

Since `3 > k`, remove the top element.

The farthest point `(2, 2)` is removed.

### Final Heap

```text
(4, (0, 2))
(4, (2, 0))
```

Remaining points:

```cpp
{{0, 2}, {2, 0}}
```

**Final Output:**

```cpp
{{0, 2}, {2, 0}}
```

## 5. Time and Space Complexity

Let:
- `n` = Total number of points.
- `k` = Number of closest points required.

### Time Complexity

For every point:

- Calculate squared distance: `O(1)`.
- Insert into heap: `O(log k)` while maintaining a heap of size at most `k + 1`.
- Remove the top element when heap size exceeds `k`: `O(log k)`.

We process all `n` points.

Extracting the final `k` points also takes `O(k log k)`.

**Overall Time Complexity: `O(n log k)`**

### Space Complexity

The Max Heap stores at most `k` elements.

The answer vector stores `k` points.

**Auxiliary Space Complexity: `O(k)`**

**Output Space:** `O(k)`.

## 6. Important Interview Insights

### Q1. Why do we use a Max Heap instead of a Min Heap?

We need the `k` closest points.

Whenever a new point comes, we may need to remove the farthest point among the current candidates.

A Max Heap keeps the farthest point at the top, making removal efficient.

### Q2. Why do we maintain heap size K?

Because we only need the `k` closest points.

If the heap contains more than `k` points, the farthest point is removed.

This ensures that the heap always retains the best `k` candidates processed so far.

### Q3. Why do we calculate squared distance instead of Euclidean distance?

Euclidean distance:

```cpp
sqrt(x * x + y * y)
```

Squared distance:

```cpp
x * x + y * y
```

Square root does not change the relative ordering of non-negative distances.

Therefore, squared distance is sufficient and avoids unnecessary calculations.

### Q4. Why does a Max Heap help us find the K closest points?

Because the Max Heap gives us the farthest point among our current `k` candidates.

When a better candidate arrives, we can remove the farthest one.

### Q5. What is the difference between sorting and using a heap?

| Sorting | Max Heap |
|---|---|
| Sorts all n points | Maintains only k points |
| `O(n log n)` time | `O(n log k)` time |
| `O(n)` auxiliary space | `O(k)` auxiliary space |

## 7. Common Mistakes

- Using a Min Heap when maintaining the k closest points with this approach.
- Calculating `sqrt()` unnecessarily.
- Forgetting to remove an element when heap size exceeds `k`.
- Storing only distances and forgetting the corresponding coordinates.
- Returning the distance instead of the actual points.
- Forgetting that the answer can be returned in any order.

## 8. Key Takeaways

### Important Heap Pattern

When a problem asks for the Top K elements, think about using a Heap.

| Problem Requirement | Suitable Heap |
|---|---|
| K largest elements | Min Heap of size K |
| K smallest elements | Max Heap of size K |
| K closest points | Max Heap of size K |
| K farthest points | Min Heap of size K |

### Pattern Recognition

**When we need K best elements and want to discard the worst candidate whenever a better one arrives, use a heap of size K.**

### Connection With Other NeetCode Problems

- Last Stone Weight → Max Heap to repeatedly extract the two largest elements.
- K Closest Points to Origin → Max Heap of size K.
- Kth Largest Element in a Stream → Min Heap of size K.
- Top K Frequent Elements → Heap or Bucket Sort.
- Find Median from Data Stream → Two Heaps.

## 9. Interview Revision — Remember This

The most important concept in this problem is:

**K Closest Points = Max Heap of Size K**

Why?

Because the largest distance among the selected candidates must be removed whenever a closer point arrives.

Do not just memorize the code. Understand why we use a Max Heap instead of a Min Heap.

# 215. Kth Largest Element in an Array

- **Difficulty:** Medium
- **Pattern:** Heap / Priority Queue
- **Approaches:** Sorting, Min Heap, Quickselect
- **Language:** C++

## 1. Problem Understanding

We are given:

- An unsorted integer array `nums`.
- An integer `k`.

We need to return the Kth largest element in the array.

**Important:** Kth largest means the element at position K when the array is sorted in descending order.

It does not mean the Kth distinct element. Duplicate values are counted.

### Example 1

**Input:**

```cpp
nums = {2, 3, 1, 5, 4};
k = 2;
```

Sorted array in descending order:

```cpp
{5, 4, 3, 2, 1}
```

**Output:**

```cpp
4
```

The second largest element is `4`.

### Example 2

**Input:**

```cpp
nums = {3, 2, 3, 1, 2, 4, 5, 5, 6};
k = 4;
```

Sorted array in descending order:

```cpp
{6, 5, 5, 4, 3, 3, 2, 2, 1}
```

**Output:**

```cpp
4
```

The fourth largest element is `4`.

## 2. Brute Force Approach — Sorting

### Idea

The simplest approach is to sort the entire array in descending order.

After sorting, the element at index `k - 1` will be the Kth largest element.

### Algorithm

1. Sort the array in descending order.
2. Access the element at index `k - 1`.
3. Return that element.

### C++ Code

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {

        sort(nums.begin(), nums.end(), greater<int>());

        return nums[k - 1];
    }
};
```

### Complexity Analysis

- Sorting the array: `O(n log n)`.
- Accessing the Kth element: `O(1)`.

**Time Complexity:** `O(n log n)`

**Space Complexity:** `O(log n)` auxiliary space for typical `std::sort` implementations.

### Limitation

We sort the entire array even though we only need one element.

Can we avoid sorting the complete array?

Yes! We can use a Min Heap of size K.

## 3. Optimized Approach — Min Heap of Size K

### Core Concept: Why Min Heap?

We need to find the Kth largest element.

Instead of sorting the entire array, we maintain only the K largest elements using a Min Heap.

A Min Heap keeps the smallest element at the top.

This is useful because among the K largest elements, the smallest one will be the Kth largest element.

### Example

Consider:

```cpp
nums = {3, 2, 1, 5, 6, 4};
k = 2;
```

We need the 2nd largest element.

The two largest elements are:

```cpp
{5, 6}
```

If we maintain these two elements in a Min Heap:

```text
       5
      /
     6
```

The smallest element among them is `5`.

Therefore, the Kth largest element is `5`.

### Important C++ Concept

```cpp
priority_queue<int, vector<int>, greater<int>> pq;
```

This creates a Min Heap.

Compare:

| Declaration | Heap Type | Top Element |
|---|---|---|
| `priority_queue<int> pq` | Max Heap | Largest |
| `priority_queue<int, vector<int>, greater<int>> pq` | Min Heap | Smallest |

### Algorithm

1. Create an empty Min Heap.
2. Traverse every element in the array.
3. Insert the current element into the heap.
4. If heap size becomes greater than K, remove the smallest element.
5. Continue until all elements are processed.
6. The heap now contains the K largest elements.
7. The top element is the Kth largest element.
8. Return `pq.top()`.

### Optimized C++ Solution

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {

        // Create Min Heap
        priority_queue<int, vector<int>, greater<int>> pq;

        // Process every element
        for (int num : nums) {

            pq.push(num);

            // Maintain heap size K
            if (pq.size() > k) {
                pq.pop();
            }
        }

        // Top element is the Kth largest
        return pq.top();
    }
};
```

## 4. Dry Run — Step by Step

**Input:**

```cpp
nums = {3, 2, 1, 5, 6, 4};
k = 2;
```

We need the 2nd largest element.

We maintain a Min Heap of size 2.

### Step-by-step Execution

| Step | Current Element | Heap After Processing | Action |
|---|---|---|---|
| 1 | 3 | {3} | Insert 3 |
| 2 | 2 | {2, 3} | Insert 2 |
| 3 | 1 | {2, 3} | Insert 1, remove 1 |
| 4 | 5 | {3, 5} | Insert 5, remove 2 |
| 5 | 6 | {5, 6} | Insert 6, remove 3 |
| 6 | 4 | {5, 6} | Insert 4, remove 4 |

Final Min Heap:

```text
       5
      /
     6
```

The top element is `5`.

**Final Output:**

```cpp
5
```

### Understanding the Heap

After processing all elements, the heap contains:

```cpp
{5, 6}
```

These are the two largest elements in the original array.

Because we use a Min Heap, the smallest of these two elements stays at the top.

Therefore:

```cpp
pq.top() = 5;
```

## 5. Time and Space Complexity

Let:

- `n` = Number of elements.
- `k` = Required position.

### Time Complexity

For every element:

- Insert into Min Heap: `O(log k)`.
- Remove the smallest element if heap size exceeds K: `O(log k)`.

We process all `n` elements.

**Overall Time Complexity: `O(n log k)`**

### Space Complexity

The heap stores at most K elements.

**Space Complexity: `O(k)`**

## 6. Alternative Approach — Quickselect

Quickselect is another important approach for finding the Kth largest element without sorting the entire array.

### Core Concept

Quickselect is based on the Partition technique used in Quick Sort.

During partitioning:

1. Choose a pivot element.
2. Rearrange elements around the pivot.
3. Place the pivot in its correct sorted position.
4. Compare the pivot's index with the target index.
5. Continue searching only in the relevant partition.

For Kth largest, the target index in ascending sorted order is:

```cpp
n - k
```

### C++ Code

```cpp
class Solution {
public:

    int partition(vector<int>& nums, int left, int right) {

        int pivot = nums[right];

        int i = left;

        for (int j = left; j < right; j++) {

            if (nums[j] <= pivot) {
                swap(nums[i], nums[j]);
                i++;
            }
        }

        swap(nums[i], nums[right]);

        return i;
    }

    int quickSelect(vector<int>& nums, int left, int right, int target) {

        while (left <= right) {

            int pivotIndex = partition(nums, left, right);

            if (pivotIndex == target) {
                return nums[pivotIndex];
            }

            else if (pivotIndex < target) {
                left = pivotIndex + 1;
            }

            else {
                right = pivotIndex - 1;
            }
        }

        return -1;
    }

    int findKthLargest(vector<int>& nums, int k) {

        int n = nums.size();

        return quickSelect(nums, 0, n - 1, n - k);
    }
};
```

### Complexity Analysis

| Case | Time Complexity |
|---|---|
| Average | `O(n)` |
| Worst | `O(n²)` |

**Space Complexity:** `O(1)` auxiliary space for this iterative implementation.

### Why Quickselect?

Quickselect does not need to fully sort the array.

It eliminates the irrelevant partition after every partition operation.

It is useful when we need one Kth element rather than a fully sorted array.

## 7. Important Interview Insights

### Q1. Why do we use a Min Heap instead of a Max Heap?

We need the Kth largest element.

A Min Heap of size K maintains the K largest elements.

The smallest among those K elements is exactly the Kth largest element.

### Q2. Why do we remove the top element when heap size exceeds K?

The Min Heap contains the K largest elements seen so far.

If a new element makes the heap size greater than K, we remove the smallest element.

This keeps only the K largest elements.

### Q3. Why is the top element the Kth largest?

Because the heap contains exactly the K largest elements.

The smallest element among these K elements is the Kth largest in the original array.

### Q4. Does this approach work with duplicate elements?

Yes.

Duplicates are counted because the problem asks for the Kth largest element, not the Kth distinct element.

### Q5. What happens when K equals 1?

The Min Heap maintains only one element.

That element is the largest element in the array.

### Q6. What happens when K equals the array size?

The Min Heap contains all elements.

The smallest element at the top is the Kth largest element.

## 8. Common Mistakes

- Using a Max Heap instead of a Min Heap for the size-K approach.
- Forgetting to maintain the heap size at K.
- Returning the largest element instead of the Kth largest.
- Using index `k` instead of `k - 1` in the sorting approach.
- Ignoring duplicate elements.
- Confusing Kth largest with Kth distinct largest.

## 9. Comparison of Approaches

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Sorting | `O(n log n)` | `O(log n)` typical sorting stack |
| Min Heap of size K | `O(n log k)` | `O(k)` |
| Quickselect | `O(n)` average, `O(n²)` worst | `O(1)` auxiliary |

## 10. Key Takeaways

### Most Important Heap Pattern

**Kth Largest Element → Min Heap of Size K**

Why?

We need to retain the K largest elements.

The smallest among those K elements is the Kth largest element.

### Heap Selection Cheat Sheet

| Problem Requirement | Suitable Heap |
|---|---|
| K largest elements | Min Heap of size K |
| K smallest elements | Max Heap of size K |
| Kth largest element | Min Heap of size K |
| Kth smallest element | Max Heap of size K |
| K closest points | Max Heap of size K |
| Last Stone Weight | Max Heap |

### Connection With Other NeetCode Problems

- Last Stone Weight → Max Heap.
- K Closest Points to Origin → Max Heap of size K.
- Kth Largest Element in an Array → Min Heap of size K.
- Kth Largest Element in a Stream → Min Heap of size K.
- Top K Frequent Elements → Heap or Bucket Sort.
- Find Median from Data Stream → Two Heaps.

### Final Interview Revision

Remember these two rules:

1. **K Largest → Min Heap of size K**
2. **K Smallest → Max Heap of size K**

Do not memorize only the code. Understand why the opposite heap is used.

# 621. Task Scheduler

- **Difficulty:** Medium
- **Pattern:** Greedy / Heap / Frequency Counting
- **Approaches:** Max Heap Simulation, Mathematical Greedy Formula
- **Language:** C++

## 1. Problem Understanding

We are given:

- An array `tasks`, where each element represents a CPU task (`A` to `Z`).
- An integer `n`, representing the cooldown period.

Rules:

1. Each CPU cycle can execute only one task.
2. Tasks can be executed in any order.
3. Two identical tasks must have at least `n` CPU cycles between their executions.
4. If no task can be executed during a cycle, the CPU remains idle.

We need to return the minimum number of CPU cycles required to complete all tasks.

### Example 1

**Input:**

```cpp
tasks = {'X', 'X', 'Y', 'Y'};
n = 2;
```

**One possible schedule:**

```text
X -> Y -> Idle -> X -> Y
```

**Output:**

```cpp
5
```

Explanation:

- Execute X.
- Execute Y.
- Both X and Y are in cooldown.
- Execute Idle.
- Execute X.
- Execute Y.

Total CPU cycles = 5.

### Example 2

**Input:**

```cpp
tasks = {'A', 'A', 'A', 'B', 'B', 'C'};
n = 3;
```

**One possible schedule:**

```text
A -> B -> C -> Idle -> A -> B -> Idle -> Idle -> A
```

**Output:**

```cpp
9
```

Explanation:

- A is the most frequent task and appears 3 times.
- The cooldown period is 3.
- We need to maintain at least 3 cycles between consecutive A executions.
- Idle cycles may be required when no other task is available.

Total CPU cycles = 9.

## 2. Core Concept — Frequency Counting

Before understanding the optimized solution, we need to understand task frequencies.

We have only 26 possible task characters (`A` to `Z`).

We can use a frequency array to count how many times each task appears.

### C++ Code

```cpp
vector<int> freq(26, 0);

for (char task : tasks) {
    freq[task - 'A']++;
}
```

For example:

```cpp
tasks = {'A', 'A', 'A', 'B', 'B', 'C'};
```

Frequencies:

| Task | Frequency |
|---|---|
| A | 3 |
| B | 2 |
| C | 1 |

Maximum frequency = 3.

The most frequent task is important because its repeated executions create the strongest cooldown constraint.

## 3. Approach 1 — Max Heap Simulation

### Core Concept

A Max Heap allows us to execute the task with the highest remaining frequency first.

Why?

Because the most frequent tasks are the most difficult to schedule.

We should prioritize them to avoid leaving too many idle CPU cycles toward the end.

We also need a cooldown queue to temporarily store tasks that cannot be executed yet.

### Data Structures

We use:

**1. Max Heap**

Stores the remaining frequencies of tasks.

```cpp
priority_queue<int> maxHeap;
```

**2. Cooldown Queue**

Stores:

- Remaining frequency of a task.
- Time when that task becomes available again.

```cpp
queue<pair<int, int>> cooldown;
```

### Algorithm

1. Count the frequency of every task.
2. Insert all non-zero frequencies into a Max Heap.
3. Maintain a variable `time` to track CPU cycles.
4. At every cycle:
   - Check whether a task in the cooldown queue is available again.
   - If available, insert it back into the Max Heap.
   - If the Max Heap is not empty, execute its most frequent task.
   - Decrease its remaining frequency.
   - If it still has remaining executions, put it into the cooldown queue.
   - If no task is available, the CPU remains idle.
5. Continue until both the heap and cooldown queue are empty.
6. Return the total time.

### C++ Code

```cpp
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {

        vector<int> freq(26, 0);

        // Count task frequencies
        for (char task : tasks) {
            freq[task - 'A']++;
        }

        // Max Heap
        priority_queue<int> maxHeap;

        for (int count : freq) {
            if (count > 0) {
                maxHeap.push(count);
            }
        }

        // {remaining frequency, available time}
        queue<pair<int, int>> cooldown;

        int time = 0;

        while (!maxHeap.empty() || !cooldown.empty()) {

            time++;

            // Check whether a task has completed cooldown
            if (!cooldown.empty() &&
                cooldown.front().second <= time) {

                maxHeap.push(cooldown.front().first);
                cooldown.pop();
            }

            // Execute the task with highest frequency
            if (!maxHeap.empty()) {

                int count = maxHeap.top();
                maxHeap.pop();

                count--;

                // If task still has remaining executions
                if (count > 0) {

                    cooldown.push({
                        count,
                        time + n + 1
                    });
                }
            }
        }

        return time;
    }
};
```

### Understanding the Cooldown Calculation

Suppose:

```cpp
n = 2;
```

If task A executes at time 1:

```text
Time 1: A
Time 2: B
Time 3: Idle
Time 4: A
```

The next A can execute at time:

```cpp
1 + 2 + 1 = 4;
```

Therefore:

```cpp
availableTime = time + n + 1;
```

The `+1` accounts for the current execution cycle.

### Complexity Analysis

Let:

- `T` = Total number of tasks.
- `K` = Number of distinct task types (maximum 26).

**Time Complexity:** `O(T + K log K)` for frequency counting and heap operations, plus the simulated CPU cycles.

The simulation takes at most `O(T * (n + 1))` cycles in a loose upper bound.

**Space Complexity:** `O(K)` for the heap, frequency array and cooldown queue.

## 4. Approach 2 — Optimized Mathematical Greedy Formula

This is the most important approach for interviews.

Instead of simulating every CPU cycle, we can directly calculate the minimum number of intervals.

### Core Observation

The most frequent task determines the minimum schedule length.

Suppose:

```cpp
tasks = {'A', 'A', 'A', 'B', 'B', 'C'};
n = 2;
```

Frequencies:

```text
A = 3
B = 2
C = 1
```

The maximum frequency is 3.

We can arrange the most frequent task first:

```text
A -> _ -> _ -> A -> _ -> _ -> A
```

Here:

- A appears 3 times.
- There are 2 gaps between its executions.
- Each gap needs at least 2 positions.

Therefore, the number of positions required is:

```cpp
(maxFreq - 1) * (n + 1) + 1
```

But there may be multiple tasks having the same maximum frequency.

For example:

```cpp
tasks = {'A', 'A', 'B', 'B'};
n = 2;
```

Both A and B have frequency 2.

We must account for both of them in the final group.

### Mathematical Formula

Let:

- `maxFreq` = Maximum frequency of any task.
- `countMax` = Number of tasks having that maximum frequency.
- `T` = Total number of tasks.

The minimum number of CPU cycles is:

```cpp
max(
    T,
    (maxFreq - 1) * (n + 1) + countMax
);
```

### Why This Formula Works

Consider:

```text
A -> _ -> _ -> A
```

If:

```cpp
maxFreq = 2;
n = 2;
```

There are:

- `maxFreq - 1` complete gaps.
- Each gap contains `n + 1` positions, including the task at its beginning.
- The final group contains all tasks with maximum frequency.

Therefore:

```cpp
(maxFreq - 1) * (n + 1) + countMax
```

However, if there are enough different tasks to fill every cooldown position, no idle cycles are needed.

In that situation, the total number of tasks itself determines the answer.

That's why we take the maximum of:

1. Total number of tasks.
2. The calculated cooldown-based schedule length.

### Optimized C++ Solution

```cpp
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {

        vector<int> freq(26, 0);

        // Step 1: Count frequencies
        for (char task : tasks) {
            freq[task - 'A']++;
        }

        // Step 2: Find maximum frequency
        int maxFreq = 0;

        for (int count : freq) {
            maxFreq = max(maxFreq, count);
        }

        // Step 3: Count tasks having maximum frequency
        int countMax = 0;

        for (int count : freq) {
            if (count == maxFreq) {
                countMax++;
            }
        }

        // Step 4: Calculate minimum intervals
        int intervals =
            (maxFreq - 1) * (n + 1) + countMax;

        // Step 5: Return the optimal answer
        return max((int)tasks.size(), intervals);
    }
};
```

## 5. Dry Run — Mathematical Approach

**Input:**

```cpp
tasks = {'A', 'A', 'A', 'B', 'B', 'C'};
n = 3;
```

### Step 1: Calculate Frequencies

| Task | Frequency |
|---|---|
| A | 3 |
| B | 2 |
| C | 1 |

### Step 2: Find Maximum Frequency

```cpp
maxFreq = 3;
```

### Step 3: Count Tasks With Maximum Frequency

Only A has frequency 3.

```cpp
countMax = 1;
```

### Step 4: Apply Formula

```cpp
intervals = (maxFreq - 1) * (n + 1) + countMax;
```

Substitute values:

```cpp
intervals = (3 - 1) * (3 + 1) + 1;

intervals = 2 * 4 + 1;

intervals = 9;
```

### Step 5: Compare With Total Tasks

```cpp
tasks.size() = 6;
intervals = 9;
```

Therefore:

```cpp
answer = max(6, 9);
answer = 9;
```

**Final Output:**

```cpp
9
```

## 6. Another Dry Run — No Idle Cycles Required

**Input:**

```cpp
tasks = {'A', 'A', 'B', 'B', 'C', 'C'};
n = 1;
```

Frequencies:

```text
A = 2
B = 2
C = 2
```

Therefore:

```cpp
maxFreq = 2;
countMax = 3;
```

Apply formula:

```cpp
intervals = (2 - 1) * (1 + 1) + 3;

intervals = 2 + 3;

intervals = 5;
```

Total tasks:

```cpp
tasks.size() = 6;
```

Final answer:

```cpp
max(6, 5) = 6;
```

One valid schedule:

```text
A -> B -> C -> A -> B -> C
```

No idle cycles are required.

## 7. Time and Space Complexity

Let:

- `T` = Total number of tasks.
- `K` = Number of possible task types (26).

### Time Complexity

Frequency counting takes:

```cpp
O(T)
```

Finding the maximum frequency takes:

```cpp
O(K)
```

Counting tasks with maximum frequency takes:

```cpp
O(K)
```

Since K = 26:

**Overall Time Complexity: `O(T)`**

### Space Complexity

We use a frequency array of fixed size 26.

**Space Complexity: `O(1)`**

The space is constant because the number of possible task types is fixed.

## 8. Important Interview Insights

### Q1. Why does the most frequent task determine the schedule?

Because it creates the largest number of repeated executions that must respect the cooldown period.

Other tasks can fill the gaps, but they cannot reduce the required spacing between identical tasks.

### Q2. Why do we use `maxFreq - 1`?

If a task appears 3 times:

```text
A -> _ -> _ -> A -> _ -> _ -> A
```

There are only 2 gaps between the 3 executions.

Therefore:

```cpp
maxFreq - 1
```

### Q3. Why do we use `n + 1`?

Each complete group contains:

- One execution of the most frequent task.
- `n` positions before that task can execute again.

Therefore, the group length is:

```cpp
n + 1
```

### Q4. Why do we count `countMax`?

Sometimes multiple tasks have the same maximum frequency.

For example:

```text
A = 3
B = 3
C = 2
```

Both A and B must be included in the final group.

Therefore, we add the number of tasks having maximum frequency.

### Q5. Why do we take the maximum with total task count?

If enough different tasks exist, they can fill the cooldown gaps.

In that case, no idle cycles are needed.

The minimum schedule length cannot be smaller than the total number of tasks.

### Q6. Which approach is better in an interview?

Both approaches are useful.

- Max Heap simulation demonstrates practical scheduling and priority queue understanding.
- Mathematical formula is more efficient and avoids simulating CPU cycles.

For this problem, the mathematical formula is the optimal solution.

## 9. Common Mistakes

- Forgetting that identical tasks need at least `n` cycles between executions.
- Using `maxFreq` instead of `maxFreq - 1`.
- Forgetting to add 1 to the cooldown period.
- Forgetting to count multiple tasks having maximum frequency.
- Returning only the formula without comparing it with the total task count.
- Confusing idle cycles with actual task executions.
- In heap simulation, calculating the next available time incorrectly.

## 10. Comparison of Approaches

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Max Heap Simulation | Depends on simulated cycles; upper bound `O(T * (n + 1))` | `O(K)` |
| Mathematical Greedy Formula | `O(T)` | `O(1)` |

## 11. Key Takeaways

### Important Pattern

**Task Scheduling + Cooldown → Greedy + Frequency Counting + Heap**

The most frequent task determines the minimum schedule length.

### Formula to Remember

```cpp
max(
    tasks.size(),
    (maxFreq - 1) * (n + 1) + countMax
);
```

### Connection With Other NeetCode Problems

- Last Stone Weight → Max Heap.
- K Closest Points to Origin → Max Heap of size K.
- Kth Largest Element in an Array → Min Heap of size K.
- Task Scheduler → Greedy + Frequency Counting + Heap.
- Find Median from Data Stream → Two Heaps.

### Final Interview Revision

Remember these three concepts:

1. Count the frequency of every task.
2. The maximum frequency determines the cooldown-based schedule.
3. Other tasks can fill the gaps; otherwise, idle cycles are required.

**Most Important:** Understand why the formula works instead of memorizing it.

# 355. Design Twitter

- **Difficulty:** Medium
- **Pattern:** Design / Hash Map / Heap
- **Data Structures:** Hash Map, Hash Set, Vector, Max Heap
- **Language:** C++

## 1. Problem Understanding

We need to design a simplified Twitter system that supports four operations:

1. `postTweet(userId, tweetId)` — A user publishes a new tweet.
2. `getNewsFeed(userId)` — Returns the 10 most recent tweets from the user and everyone they follow.
3. `follow(followerId, followeeId)` — One user follows another user.
4. `unfollow(followerId, followeeId)` — One user unfollows another user.

### Important Rules

- Every tweet has a unique ID.
- Tweets must appear from most recent to least recent.
- A user's own tweets must appear in their news feed.
- A user cannot follow themselves.
- A news feed contains at most 10 tweets.
- The feed should reflect the current follow relationships.

## 2. Understanding the Data Structures

We need multiple data structures because each operation has a different responsibility.

### 1. Hash Map — Store Tweets

```cpp
unordered_map<int, vector<pair<int, int>>> tweets;
```

Structure:

```text
User ID -> List of tweets
```

Each tweet stores:

```cpp
{timestamp, tweetId}
```

Example:

```text
User 1 -> {(1, 10), (4, 20)}
User 2 -> {(2, 30), (3, 40)}
```

Here:

- User 1 posted tweet 10 at timestamp 1.
- User 1 posted tweet 20 at timestamp 4.
- User 2 posted tweet 30 at timestamp 2.
- User 2 posted tweet 40 at timestamp 3.

### 2. Hash Map + Hash Set — Store Follow Relationships

```cpp
unordered_map<int, unordered_set<int>> following;
```

Structure:

```text
Follower ID -> Set of users they follow
```

Example:

```text
User 1 -> {2, 3}
User 2 -> {3}
User 3 -> {}
```

Meaning:

- User 1 follows users 2 and 3.
- User 2 follows user 3.
- User 3 follows nobody.

Why use `unordered_set`?

Because:

- Insertion is average `O(1)`.
- Deletion is average `O(1)`.
- Duplicate follow relationships are automatically prevented.

### 3. Timestamp — Maintain Tweet Order

```cpp
int timestamp = 0;
```

Whenever a user posts a tweet:

```cpp
tweets[userId].push_back({timestamp++, tweetId});
```

Every new tweet receives a larger timestamp.

Therefore, a larger timestamp means a more recent tweet.

### 4. Max Heap — Retrieve Recent Tweets

```cpp
priority_queue<tuple<int, int, int>> pq;
```

Each heap entry contains:

```cpp
{timestamp, userId, index}
```

Where:

- `timestamp` = Time when the tweet was posted.
- `userId` = User who posted the tweet.
- `index` = Position of the tweet in that user's tweet vector.

The Max Heap keeps the most recent tweet at the top.

## 3. Brute Force Approach — Collect and Sort

### Idea

The simplest way to generate a news feed is:

1. Get all users followed by the current user.
2. Include the current user's own tweets.
3. Collect all their tweets.
4. Sort all collected tweets by timestamp in descending order.
5. Return the first 10 tweet IDs.

### C++ Code

```cpp
class Twitter {
private:

    int timestamp = 0;

    unordered_map<int, vector<pair<int, int>>> tweets;

    unordered_map<int, unordered_set<int>> following;

public:

    Twitter() {
    }

    void postTweet(int userId, int tweetId) {

        tweets[userId].push_back({timestamp++, tweetId});
    }

    vector<int> getNewsFeed(int userId) {

        vector<pair<int, int>> allTweets;

        // Include the user's own tweets
        for (auto tweet : tweets[userId]) {
            allTweets.push_back(tweet);
        }

        // Include tweets from followed users
        for (int followee : following[userId]) {

            for (auto tweet : tweets[followee]) {
                allTweets.push_back(tweet);
            }
        }

        // Sort by timestamp in descending order
        sort(allTweets.rbegin(), allTweets.rend());

        vector<int> ans;

        // Return at most 10 recent tweets
        for (int i = 0; i < min(10, (int)allTweets.size()); i++) {

            ans.push_back(allTweets[i].second);
        }

        return ans;
    }

    void follow(int followerId, int followeeId) {

        following[followerId].insert(followeeId);
    }

    void unfollow(int followerId, int followeeId) {

        following[followerId].erase(followeeId);
    }
};
```

### Complexity Analysis

Let:

- `F` = Number of users followed by the current user.
- `M` = Total number of tweets belonging to the user and their followed users.

**postTweet():**
- Time: `O(1)` average.
- Space: `O(1)` per inserted tweet.

**follow():**
- Time: `O(1)` average.

**unfollow():**
- Time: `O(1)` average.

**getNewsFeed():**
- Collecting tweets: `O(M)`.
- Sorting tweets: `O(M log M)`.
- Extracting 10 tweets: `O(10)`.

Overall:

- Time: `O(M log M)`.
- Auxiliary Space: `O(M)`.

### Limitation

We collect and sort every tweet even though we only need the 10 most recent tweets.

Can we avoid sorting all tweets?

Yes! We can use a Max Heap.

## 4. Optimized Approach — Max Heap (K-Way Merge)

### Core Concept

Each user has their own list of tweets.

Because tweets are stored in chronological order, each user's tweet list is already sorted by timestamp.

For example:

```text
User 1: 10 -> 20 -> 30
User 2: 40 -> 50
User 3: 60 -> 70
```

We need to merge these multiple sorted lists and retrieve only the 10 most recent tweets.

This is similar to the K-Way Merge pattern.

Instead of collecting every tweet, we maintain only the latest available tweet from each user in a Max Heap.

### Why Max Heap?

We need the most recent tweet globally.

The timestamp tells us which tweet is newer.

A Max Heap gives us the largest timestamp at the top.

Therefore, we can always extract the most recent available tweet.

### Important Observation

We don't need to insert all tweets of every user into the heap.

We only insert the latest tweet from each user initially.

Whenever we remove one tweet from the heap, we insert that user's previous tweet.

This process continues until:

- We collect 10 tweets, or
- The heap becomes empty.

### Algorithm

1. Create an empty Max Heap.
2. Get all users followed by the current user.
3. Include the current user as well.
4. For each user:
   - Check whether they have posted any tweets.
   - If yes, insert their latest tweet into the heap.
5. While the heap is not empty and fewer than 10 tweets have been collected:
   - Extract the most recent tweet.
   - Add its tweet ID to the answer.
   - Move to the previous tweet of the same user.
   - Insert that previous tweet into the heap if it exists.
6. Return the answer.

## 5. Optimized C++ Solution

```cpp
class Twitter {
private:

    int timestamp = 0;

    // userId -> {timestamp, tweetId}
    unordered_map<int, vector<pair<int, int>>> tweets;

    // followerId -> set of followees
    unordered_map<int, unordered_set<int>> following;

public:

    Twitter() {
    }

    // Publish a tweet
    void postTweet(int userId, int tweetId) {

        tweets[userId].push_back({timestamp++, tweetId});
    }

    // Get 10 most recent tweets
    vector<int> getNewsFeed(int userId) {

        vector<int> ans;

        // Max Heap:
        // {timestamp, userId, index}
        priority_queue<tuple<int, int, int>> pq;

        // Include the user's own tweets
        unordered_set<int> users = following[userId];

        users.insert(userId);

        // Insert the latest tweet of every relevant user
        for (int user : users) {

            if (!tweets[user].empty()) {

                int index = tweets[user].size() - 1;

                int time = tweets[user][index].first;

                pq.push({time, user, index});
            }
        }

        // Extract at most 10 most recent tweets
        while (!pq.empty() && ans.size() < 10) {

            auto [time, user, index] = pq.top();

            pq.pop();

            // Add tweet ID to answer
            ans.push_back(tweets[user][index].second);

            // Move to the previous tweet of the same user
            if (index > 0) {

                int prevIndex = index - 1;

                int prevTime = tweets[user][prevIndex].first;

                pq.push({prevTime, user, prevIndex});
            }
        }

        return ans;
    }

    // Follow another user
    void follow(int followerId, int followeeId) {

        following[followerId].insert(followeeId);
    }

    // Unfollow a user
    void unfollow(int followerId, int followeeId) {

        following[followerId].erase(followeeId);
    }
};
```

## 6. Dry Run — Step by Step

Consider the following operations:

```cpp
Twitter twitter;

twitter.postTweet(1, 10);
twitter.postTweet(1, 20);

twitter.postTweet(2, 30);
twitter.postTweet(2, 40);

twitter.follow(1, 2);

twitter.getNewsFeed(1);
```

### Step 1: Post Tweets

The timestamp increases with every tweet.

| User | Tweet ID | Timestamp |
|---|---|---|
| 1 | 10 | 0 |
| 1 | 20 | 1 |
| 2 | 30 | 2 |
| 2 | 40 | 3 |

Stored tweets:

```text
User 1:
(0, 10)
(1, 20)

User 2:
(2, 30)
(3, 40)
```

### Step 2: Follow Relationship

```cpp
twitter.follow(1, 2);
```

User 1 follows user 2.

Therefore, User 1's news feed should contain tweets from:

```text
User 1 + User 2
```

### Step 3: Initialize Max Heap

Insert the latest tweet from each user.

For User 1:

```text
Timestamp = 1
Tweet ID = 20
```

For User 2:

```text
Timestamp = 3
Tweet ID = 40
```

Initial Max Heap:

```text
(3, User 2, Index 1)
(1, User 1, Index 1)
```

### Step 4: Extract Most Recent Tweet

The heap top is:

```text
(3, User 2, Index 1)
```

Tweet ID = 40.

Add it to the answer.

Now move to User 2's previous tweet:

```text
(2, User 2, Index 0)
```

Insert it into the heap.

### Step 5: Extract Next Tweet

The heap top is now:

```text
(2, User 2, Index 0)
```

Tweet ID = 30.

Add it to the answer.

### Step 6: Extract Next Tweet

The remaining latest tweet is:

```text
(1, User 1, Index 1)
```

Tweet ID = 20.

Add it to the answer.

Insert User 1's previous tweet:

```text
(0, User 1, Index 0)
```

### Step 7: Extract Final Tweet

Tweet ID = 10.

Add it to the answer.

The heap is now empty.

### Final News Feed

```cpp
{40, 30, 20, 10}
```

The tweets are returned from most recent to least recent.

## 7. Time and Space Complexity

Let:

- `F` = Number of followed users.
- `K` = Maximum number of tweets returned (10).
- `T` = Total number of tweets stored in the system.

### postTweet()

A new tweet is appended to the user's vector.

**Time Complexity:** `O(1)` amortized.

### follow()

Insert the followee into an unordered set.

**Time Complexity:** `O(1)` average.

### unfollow()

Remove the followee from an unordered set.

**Time Complexity:** `O(1)` average.

### getNewsFeed()

**Step 1:** Initialize the heap with the latest tweet from each followed user and the current user.

Time: `O(F)` expected.

**Step 2:** Extract at most 10 tweets.

Each extraction and insertion takes `O(log F)`.

Time: `O(K log F)`.

**Overall Time Complexity:**

`O(F + K log F)`

Since K is fixed at 10:

`O(F)` for the feed operation, with the heap processing bounded by 10 extractions.

### Space Complexity

- Follow relationships: `O(F)` per user's follow set.
- Tweet storage: `O(T)` overall.
- Heap: `O(F)`.
- Answer: `O(10)`.

The additional space used by `getNewsFeed()` is:

**O(F)**.

## 8. Important Interview Insights

### Q1. Why do we use a timestamp?

A timestamp helps us identify which tweet is more recent.

A larger timestamp means a newer tweet.

### Q2. Why do we use a Max Heap?

Because we need to retrieve the most recent tweet among multiple users.

The Max Heap keeps the largest timestamp at the top.

### Q3. Why don't we insert all tweets into the heap?

Because we only need the 10 most recent tweets.

Initially inserting only the latest tweet from each user reduces unnecessary heap operations.

### Q4. Why do we insert the previous tweet after extracting one?

Each user's tweet list is already sorted by timestamp.

After extracting the latest tweet of a user, the next possible tweet from that user is the previous one.

This allows us to merge multiple sorted lists efficiently.

### Q5. Why do we include the current user in the news feed?

A user's own tweets must always appear in their news feed, even if they follow nobody.

### Q6. Why use an unordered_set for follow relationships?

It prevents duplicate follow relationships and supports efficient insertion and deletion.

### Q7. Why do we use tuple<int, int, int>?

Because every heap entry needs three pieces of information:

- Timestamp.
- User ID.
- Tweet index.

The timestamp is the primary value used to determine recency.

## 9. Common Mistakes

- Forgetting to include the user's own tweets.
- Returning tweets in oldest-to-newest order.
- Forgetting to maintain a global timestamp.
- Inserting every tweet into the heap instead of only the latest tweet initially.
- Forgetting to insert the previous tweet after extracting one.
- Returning more than 10 tweets.
- Not removing follow relationships correctly.
- Using a Min Heap instead of a Max Heap.
- Confusing tweet ID with timestamp.

## 10. Comparison of Approaches

| Approach | getNewsFeed() Time | Additional Space |
|---|---|---|
| Collect and Sort | `O(M log M)` | `O(M)` |
| Max Heap / K-Way Merge | `O(F + 10 log F)` | `O(F)` |

Where:

- `M` = Total tweets belonging to the user and followed users.
- `F` = Number of followed users.

## 11. Key Takeaways

### Important Patterns

**Multiple sorted lists + retrieve globally largest elements → Max Heap / K-Way Merge.**

**User relationships → Hash Map + Hash Set.**

**Chronological ordering → Timestamp.**

### Connection With Other NeetCode Problems

- Last Stone Weight → Max Heap.
- K Closest Points to Origin → Max Heap of size K.
- Kth Largest Element in an Array → Min Heap of size K.
- Task Scheduler → Greedy + Frequency Counting.
- Design Twitter → Hash Map + Hash Set + Max Heap.
- Merge K Sorted Lists → Heap / K-Way Merge.

## 12. Final Interview Revision

Remember these four things:

1. Store tweets separately for each user.
2. Use a Hash Set to maintain follow relationships.
3. Use a global timestamp to track tweet recency.
4. Use a Max Heap to merge recent tweets from all relevant users.

**Most Important Concept:**

We don't need to sort all tweets.

We only need the latest tweet from each relevant user initially. After extracting a tweet, we insert that user's previous tweet.

This is the K-Way Merge pattern using a Max Heap.

# 295. Find Median from Data Stream

- **Difficulty:** Hard
- **Pattern:** Heap / Priority Queue / Two Heaps
- **Approaches:** Sorting, Two Heaps
- **Language:** C++

## 1. Problem Understanding

We need to design a `MedianFinder` class that supports two operations:

1. `addNum(int num)` — Adds an integer to the data structure.
2. `findMedian()` — Returns the median of all numbers added so far.

### What Is Median?

The median is the middle element of a sorted array.

There are two cases:

**Case 1: Odd Number of Elements**

The median is the middle element.

Example:

```cpp
arr = {1, 2, 3};
```

Sorted array:

```text
1  2  3
```

Median = `2`

**Case 2: Even Number of Elements**

The median is the average of the two middle elements.

Example:

```cpp
arr = {1, 2, 3, 4};
```

Sorted array:

```text
1  2  3  4
```

Median:

```cpp
(2 + 3) / 2.0 = 2.5
```

## 2. Brute Force Approach — Sorting

### Idea

We can store all incoming numbers in a vector.

Whenever `findMedian()` is called:

1. Sort the vector.
2. Find the middle element if the size is odd.
3. Find the average of the two middle elements if the size is even.
4. Return the median.

### C++ Code

```cpp
class MedianFinder {
private:
    vector<int> nums;

public:

    MedianFinder() {
    }

    void addNum(int num) {
        nums.push_back(num);
    }

    double findMedian() {

        sort(nums.begin(), nums.end());

        int n = nums.size();

        if (n % 2 == 1) {
            return nums[n / 2];
        }

        return ((double)nums[n / 2 - 1] + nums[n / 2]) / 2.0;
    }
};
```

### Complexity Analysis

Let `n` be the number of elements currently stored.

**addNum():**
- Insert into vector: `O(1)` amortized.

**findMedian():**
- Sorting: `O(n log n)`.
- Finding the middle element: `O(1)`.

**Time Complexity:**
- `addNum()` = `O(1)` amortized.
- `findMedian()` = `O(n log n)`.

**Space Complexity:** `O(n)`.

### Limitation

Every time we call `findMedian()`, we sort the entire array again.

This is inefficient because we only need the middle element, not the complete sorted array.

Can we maintain the middle elements efficiently?

Yes! We can use Two Heaps.

## 3. Optimized Approach — Two Heaps

### Core Concept

We divide all incoming numbers into two halves:

1. Smaller Half
2. Larger Half

We use two different heaps to maintain these halves.

**Heap 1: Max Heap**

Stores the smaller half of the numbers.

```cpp
priority_queue<int> maxHeap;
```

The largest element of the smaller half stays at the top.

**Heap 2: Min Heap**

Stores the larger half of the numbers.

```cpp
priority_queue<int, vector<int>, greater<int>> minHeap;
```

The smallest element of the larger half stays at the top.

### Visual Understanding

Consider:

```cpp
nums = {1, 2, 3, 4, 5};
```

We divide the sorted numbers into two halves:

```text
Smaller Half       Larger Half

   1                  4
   2                  5
   3
```

Max Heap:

```text
       3
      / \
     2   1
```

Min Heap:

```text
       4
      /
     5
```

The two important elements are:

```cpp
maxHeap.top() = 3;
minHeap.top() = 4;
```

Since there are 5 elements, the median is:

```cpp
3
```

### Two Important Rules

**Rule 1: Heap Size**

The size difference between the two heaps must never exceed 1.

```text
abs(maxHeap.size() - minHeap.size()) <= 1
```

**Rule 2: Heap Ordering**

Every element in the smaller half must be less than or equal to every element in the larger half.

Therefore:

```cpp
maxHeap.top() <= minHeap.top();
```

Whenever both heaps contain elements.

### How Do We Find the Median?

There are two possibilities.

**Case 1: Odd Number of Elements**

One heap contains one extra element.

The median is the top element of that larger heap.

**Case 2: Even Number of Elements**

Both heaps contain an equal number of elements.

The median is the average of their top elements.

```cpp
(maxHeap.top() + minHeap.top()) / 2.0
```

## 4. Optimized C++ Solution — Two Heaps

### Algorithm

**addNum(num):**

1. Insert the incoming number into the Max Heap.
2. Move the largest element of the Max Heap into the Min Heap.
3. If the Min Heap becomes larger than the Max Heap, move its smallest element back to the Max Heap.

This maintains:

- The size difference between heaps is at most 1.
- Every element in the Max Heap is less than or equal to every element in the Min Heap.

**findMedian():**

1. If the Max Heap is larger, return its top element.
2. If the Min Heap is larger, return its top element.
3. If both heaps have equal sizes, return the average of their top elements.

### C++ Code

```cpp
class MedianFinder {
private:

    // Stores the smaller half
    priority_queue<int> maxHeap;

    // Stores the larger half
    priority_queue<int, vector<int>, greater<int>> minHeap;

public:

    MedianFinder() {
    }

    void addNum(int num) {

        // Step 1: Insert into Max Heap
        maxHeap.push(num);

        // Step 2: Move largest element of smaller half
        // into the larger half
        minHeap.push(maxHeap.top());
        maxHeap.pop();

        // Step 3: Balance the heap sizes
        if (minHeap.size() > maxHeap.size()) {

            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    double findMedian() {

        // Odd number of elements
        if (maxHeap.size() > minHeap.size()) {
            return maxHeap.top();
        }

        // Even number of elements
        return ((double)maxHeap.top() + minHeap.top()) / 2.0;
    }
};
```

## 5. Dry Run — Step by Step

Consider the following operations:

```cpp
MedianFinder mf;

mf.addNum(1);
mf.addNum(2);
mf.findMedian();

mf.addNum(3);
mf.findMedian();
```

### Step 1: addNum(1)

Insert 1 into Max Heap.

Max Heap:

```text
1
```

Min Heap:

```text
Empty
```

Sizes:

```text
Max Heap = 1
Min Heap = 0
```

### Step 2: addNum(2)

Insert 2 into Max Heap.

Max Heap:

```text
   2
  /
 1
```

Move the largest element (2) into Min Heap.

Max Heap:

```text
1
```

Min Heap:

```text
2
```

Both heaps have equal sizes.

### Step 3: findMedian()

The total number of elements is 2.

Therefore, take the average of both heap tops.

```cpp
(1 + 2) / 2.0 = 1.5
```

Output:

```cpp
1.5
```

### Step 4: addNum(3)

Insert 3 into Max Heap.

Max Heap:

```text
   3
  /
 1
```

Move the largest element (3) into Min Heap.

Min Heap:

```text
   3
  /
 2
```

Now Min Heap is larger.

Move its smallest element (2) back into Max Heap.

Final heaps:

Max Heap:

```text
1
```

Min Heap:

```text
2
```

Wait: the remaining values must be distributed correctly. After this operation, the actual heaps are:

Max Heap:

```text
   2
  /
 1
```

Min Heap:

```text
3
```

Sizes:

```text
Max Heap = 2
Min Heap = 1
```

### Step 5: findMedian()

The Max Heap contains one extra element.

Therefore, the median is:

```cpp
maxHeap.top() = 2;
```

Output:

```cpp
2.0
```

### Final Results

| Operation | Result |
|---|---|
| `addNum(1)` | null |
| `addNum(2)` | null |
| `findMedian()` | 1.5 |
| `addNum(3)` | null |
| `findMedian()` | 2.0 |

## 6. Another Dry Run — Even Number of Elements

Input:

```cpp
addNum(5);
addNum(15);
addNum(1);
addNum(3);
```

Numbers in sorted order:

```text
1  3  5  15
```

Max Heap contains the smaller half:

```text
   3
  /
 1
```

Min Heap contains the larger half:

```text
   5
  /
15
```

Both heaps have equal sizes.

Therefore:

```cpp
median = (3 + 5) / 2.0;
```

Output:

```cpp
4.0
```

## 7. Time and Space Complexity

Let `n` be the total number of inserted elements.

### addNum()

- Insert into Max Heap: `O(log n)`.
- Move an element between heaps: `O(log n)`.
- At most one balancing operation: `O(log n)`.

**Time Complexity: `O(log n)`**

### findMedian()

We only access the top elements of the heaps.

```cpp
maxHeap.top();
minHeap.top();
```

Both operations take constant time.

**Time Complexity: `O(1)`**

### Space Complexity

Both heaps together store all inserted elements.

**Space Complexity: `O(n)`**

## 8. Important Interview Insights

### Q1. Why do we use two heaps instead of one?

Because we need efficient access to the two middle elements.

A Max Heap maintains the smaller half, while a Min Heap maintains the larger half.

Their top elements represent the middle boundary of the sorted data.

### Q2. Why is the Max Heap used for the smaller half?

We need quick access to the largest element of the smaller half.

That element can be one of the middle elements.

### Q3. Why is the Min Heap used for the larger half?

We need quick access to the smallest element of the larger half.

That element can also be one of the middle elements.

### Q4. Why must the heap sizes differ by at most one?

Because the median is determined by the middle element or the two middle elements.

Keeping the two halves balanced allows us to retrieve the median directly from their tops.

### Q5. Why do we compare heap sizes in findMedian()?

- If Max Heap has one extra element, its top is the median.
- If Min Heap has one extra element, its top is the median.
- If both have equal sizes, the average of both tops is the median.

### Q6. Why do we use `/ 2.0` instead of `/ 2`?

Because integer division can discard the decimal portion.

Example:

```cpp
(1 + 2) / 2 = 1
```

But:

```cpp
(1 + 2) / 2.0 = 1.5
```

The problem requires returning a `double`.

### Q7. What if duplicate numbers arrive?

The two-heap approach handles duplicates correctly.

There is no need to remove duplicates because every incoming number must be considered.

## 9. Common Mistakes

- Using two Max Heaps instead of one Max Heap and one Min Heap.
- Forgetting to balance the heap sizes.
- Allowing one heap to contain more than one extra element.
- Forgetting the ordering relationship between the two halves.
- Returning only one heap's top when both heaps have equal sizes.
- Using integer division instead of floating-point division.
- Assuming the heaps contain the same number of elements after every insertion.

## 10. Comparison of Approaches

| Approach | addNum() | findMedian() | Space |
|---|---|---|---|
| Sorting each time | O(1) amortized | O(n log n) | O(n) |
| Two Heaps | O(log n) | O(1) | O(n) |

## 11. Key Takeaways

### Most Important Pattern

**Find Median from Data Stream → Two Heaps**

Remember:

- Max Heap → Smaller half.
- Min Heap → Larger half.
- Max Heap top → Largest element of smaller half.
- Min Heap top → Smallest element of larger half.

### Median Formula

For odd number of elements:

```cpp
median = top of the larger heap;
```

For even number of elements:

```cpp
median = (maxHeap.top() + minHeap.top()) / 2.0;
```

### Connection With Other NeetCode Problems

- Last Stone Weight → Max Heap.
- K Closest Points to Origin → Max Heap of size K.
- Kth Largest Element in an Array → Min Heap of size K.
- Task Scheduler → Greedy + Frequency Counting.
- Design Twitter → Hash Map + Hash Set + Max Heap.
- Find Median from Data Stream → Two Heaps.

## 12. Final Interview Revision

Remember these three rules:

1. Max Heap stores the smaller half.
2. Min Heap stores the larger half.
3. The difference between heap sizes must never exceed one.

**Most Important Insight:**

Instead of sorting the complete data stream every time, maintain two balanced heaps.

Their top elements give us direct access to the middle of the data.

This reduces:

- addNum() from repeated sorting to `O(log n)`.
- findMedian() from `O(n log n)` to `O(1)`.
