# 57. Insert Interval

- **Difficulty:** Medium
- **Pattern:** Intervals / Greedy
- **Data Structures:** Vector
- **Approaches:** Brute Force, Optimized
- **Language:** C++

## 1. Problem Understanding

We are given:

- A vector of non-overlapping intervals called `intervals`.
- The intervals are already sorted in ascending order by their starting times.
- Another interval called `newInterval`.

Each interval is represented as:

```cpp
[start, end]
```

Our task is to insert `newInterval` into the existing intervals while maintaining:

1. Sorted order based on starting time.
2. No overlapping intervals.

If the new interval overlaps with existing intervals, we must merge them.

Return the updated list of intervals.

### Important Note

Two intervals overlap if they have at least one common point.

For example:

```text
[1, 2] and [2, 3] -> Overlapping
[1, 2] and [3, 4] -> Non-overlapping
```

This means equality at the boundary counts as overlap.

### Example 1

**Input:**

```cpp
intervals = {{1, 3}, {4, 6}};
newInterval = {2, 5};
```

**Output:**

```cpp
{{1, 6}}
```

**Explanation:**

The new interval `[2,5]` overlaps with:

```text
[1,3]
[4,6]
```

Merge all three intervals:

```text
[1, 6]
```

### Example 2

**Input:**

```cpp
intervals = {{1, 2}, {3, 5}, {6, 7}, {8, 10}, {12, 16}};
newInterval = {4, 8};
```

**Output:**

```cpp
{{1, 2}, {3, 10}, {12, 16}}
```

**Explanation:**

The new interval `[4,8]` overlaps with:

```text
[3,5]
[6,7]
[8,10]
```

After merging:

```text
[3,10]
```

The final result is:

```text
[1,2], [3,10], [12,16]
```

## 2. Core Concept — Three Phases

The most important observation is that the original intervals are already sorted and non-overlapping.

Therefore, we do not need to sort them again.

We can process them in three phases.

### Phase 1: Intervals Before the New Interval

If:

```cpp
intervals[i][1] < newInterval[0]
```

The current interval ends before the new interval starts.

It cannot overlap with the new interval.

So we directly add it to the result.

### Phase 2: Overlapping Intervals

Two intervals overlap when:

```cpp
intervals[i][0] <= newInterval[1] &&
intervals[i][1] >= newInterval[0]
```

When intervals overlap, merge them by updating:

```cpp
newInterval[0] = min(newInterval[0], intervals[i][0]);

newInterval[1] = max(newInterval[1], intervals[i][1]);
```

This expands the new interval to include the overlapping interval.

Continue merging while the next interval overlaps.

### Phase 3: Intervals After the New Interval

If:

```cpp
intervals[i][0] > newInterval[1]
```

The current interval starts after the new interval ends.

Since the original intervals are sorted, all remaining intervals will also be after the merged interval.

Therefore, we can add the new interval and then append all remaining intervals.

## 3. Approach 1 — Brute Force

### Idea

We can use the general Merge Intervals technique.

1. Add `newInterval` to the existing intervals.
2. Sort all intervals by their starting positions.
3. Traverse the sorted intervals.
4. If the current interval overlaps with the previous interval, merge them.
5. Otherwise, add the current interval as a new entry.

### C++ Code

```cpp
class Solution {
public:
    vector<vector<int>> insert(
        vector<vector<int>>& intervals,
        vector<int>& newInterval
    ) {

        intervals.push_back(newInterval);

        sort(intervals.begin(), intervals.end());

        vector<vector<int>> result;

        for (auto& interval : intervals) {

            if (result.empty() ||
                result.back()[1] < interval[0]) {

                result.push_back(interval);
            }
            else {

                result.back()[1] =
                    max(result.back()[1], interval[1]);
            }
        }

        return result;
    }
};
```

### Complexity Analysis

Let `n` be the number of existing intervals.

**Time Complexity: O(n log n)**

- Sorting: `O(n log n)`.
- Merging: `O(n)`.

**Space Complexity: O(n)** for the resulting intervals, excluding sorting implementation details.

### Limitation

The input is already sorted.

Sorting everything again is unnecessary.

We can solve the problem in a single traversal.

## 4. Optimized Approach — Three Phases

### Step 1: Initialize the Result

```cpp
vector<vector<int>> result;
int i = 0;
int n = intervals.size();
```

We use `i` to traverse the existing intervals.

### Step 2: Add Intervals Before the New Interval

```cpp
while (i < n && intervals[i][1] < newInterval[0]) {

    result.push_back(intervals[i]);

    i++;
}
```

These intervals do not overlap with the new interval.

We can safely add them directly.

### Step 3: Merge Overlapping Intervals

```cpp
while (i < n && intervals[i][0] <= newInterval[1]) {

    newInterval[0] =
        min(newInterval[0], intervals[i][0]);

    newInterval[1] =
        max(newInterval[1], intervals[i][1]);

    i++;
}
```

We merge every interval that overlaps with the expanding `newInterval`.

Notice that the loop condition uses the updated `newInterval[1]`.

This is important because merging an interval may extend the ending boundary and cause another interval to overlap.

### Step 4: Add the Merged New Interval

```cpp
result.push_back(newInterval);
```

After processing all overlapping intervals, the new interval is ready.

### Step 5: Add Remaining Intervals

```cpp
while (i < n) {

    result.push_back(intervals[i]);

    i++;
}
```

All remaining intervals are after the merged interval.

We can directly append them.

## 5. Optimized C++ Solution

```cpp
class Solution {
public:
    vector<vector<int>> insert(
        vector<vector<int>>& intervals,
        vector<int>& newInterval
    ) {

        vector<vector<int>> result;

        int i = 0;
        int n = intervals.size();

        // Phase 1: Add intervals before newInterval
        while (i < n && intervals[i][1] < newInterval[0]) {

            result.push_back(intervals[i]);

            i++;
        }

        // Phase 2: Merge overlapping intervals
        while (i < n && intervals[i][0] <= newInterval[1]) {

            newInterval[0] =
                min(newInterval[0], intervals[i][0]);

            newInterval[1] =
                max(newInterval[1], intervals[i][1]);

            i++;
        }

        // Add the merged interval
        result.push_back(newInterval);

        // Phase 3: Add remaining intervals
        while (i < n) {

            result.push_back(intervals[i]);

            i++;
        }

        return result;
    }
};
```

## 6. Understanding the Code

### Why do we use this condition in Phase 1?

```cpp
intervals[i][1] < newInterval[0]
```

The current interval ends before the new interval starts.

Example:

```text
Current interval: [1, 2]
New interval:     [4, 7]
```

Since `2 < 4`, there is no overlap.

We add `[1,2]` directly.

### Why do we use this condition in Phase 2?

```cpp
intervals[i][0] <= newInterval[1]
```

We already know that the current interval does not belong to Phase 1.

Therefore, if its starting point is less than or equal to the new interval's ending point, it overlaps.

Example:

```text
Current interval: [4, 6]
New interval:     [2, 5]
```

Since `4 <= 5`, they overlap.

Merge:

```text
[2, 6]
```

### Why do we update both boundaries?

```cpp
newInterval[0] = min(newInterval[0], intervals[i][0]);

newInterval[1] = max(newInterval[1], intervals[i][1]);
```

The merged interval must cover the complete range of both intervals.

The minimum starting point becomes the new start.

The maximum ending point becomes the new end.

### Why can we directly append the remaining intervals?

Because the original intervals are sorted by starting position.

Once an interval starts after the merged interval ends, every subsequent interval will also start after it.

Therefore, no further merging is required.

## 7. Dry Run — Step by Step

**Input:**

```cpp
intervals = {
    {1, 3},
    {4, 6}
};

newInterval = {2, 5};
```

Initially:

```cpp
i = 0;
n = 2;

result = {};
newInterval = {2, 5};
```

### Phase 1: Check Intervals Before New Interval

Current interval:

```text
[1, 3]
```

Condition:

```cpp
3 < 2
```

Result:

```text
false
```

Therefore, Phase 1 stops immediately.

### Phase 2: Merge Overlapping Intervals

Current interval:

```text
[1, 3]
```

Condition:

```cpp
1 <= 5
```

Result:

```text
true
```

Merge:

```cpp
newInterval[0] = min(2, 1) = 1;

newInterval[1] = max(5, 3) = 5;
```

Updated interval:

```text
[1, 5]
```

Move to the next interval.

Current interval:

```text
[4, 6]
```

Check:

```cpp
4 <= 5
```

Result:

```text
true
```

Merge:

```cpp
newInterval[0] = min(1, 4) = 1;

newInterval[1] = max(5, 6) = 6;
```

Updated interval:

```text
[1, 6]
```

Move to the next position.

Now:

```cpp
i = 2;
```

Since `i == n`, there are no more intervals to merge.

### Phase 3: Add the Merged Interval

```cpp
result.push_back(newInterval);
```

Result:

```cpp
{{1, 6}}
```

There are no remaining intervals.

**Final Output:**

```cpp
{{1, 6}}
```

## 8. Dry Run — All Three Phases

**Input:**

```cpp
intervals = {
    {1, 2},
    {3, 5},
    {6, 7},
    {8, 10},
    {12, 16}
};

newInterval = {4, 8};
```

### Phase 1: Add Intervals Before New Interval

Check:

```text
[1, 2]
```

Since:

```cpp
2 < 4
```

Add it.

```cpp
result = {{1, 2}};
```

Next interval:

```text
[3, 5]
```

Since:

```cpp
5 < 4
```

is false, Phase 1 stops.

### Phase 2: Merge Overlapping Intervals

Current interval:

```text
[3, 5]
```

It overlaps with `[4,8]`.

Merge:

```text
[3, 8]
```

Next interval:

```text
[6, 7]
```

It overlaps with `[3,8]`.

Merge:

```text
[3, 8]
```

Next interval:

```text
[8, 10]
```

Since:

```cpp
8 <= 8
```

it also overlaps.

Merge:

```text
[3, 10]
```

Next interval:

```text
[12, 16]
```

Since:

```cpp
12 <= 10
```

is false, merging stops.

Add the merged interval:

```cpp
result = {
    {1, 2},
    {3, 10}
};
```

### Phase 3: Add Remaining Intervals

Add:

```text
[12, 16]
```

Final result:

```cpp
{
    {1, 2},
    {3, 10},
    {12, 16}
}
```

**Final Output:**

```cpp
{{1, 2}, {3, 10}, {12, 16}}
```

## 9. Time and Space Complexity

Let `n` be the number of existing intervals.

### Time Complexity

Every interval is visited at most once.

We use three sequential loops, but the index `i` only moves forward.

Therefore:

**Time Complexity: O(n)**

### Space Complexity

We create a result vector that can contain up to `n + 1` intervals.

Therefore:

**Space Complexity: O(n)**

The algorithm uses only `O(1)` auxiliary variables, excluding the result vector.

## 10. Important Interview Insights

### Q1. Why don't we sort the intervals?

The input is already sorted by starting time.

We can use this property to process the intervals in one pass.

### Q2. Why do we use three separate loops?

Each loop handles a different category:

1. Intervals before the new interval.
2. Overlapping intervals.
3. Intervals after the new interval.

This makes the logic easy to understand and avoids unnecessary comparisons.

### Q3. Why is the overlapping condition `<=` instead of `<`?

Because intervals that share an endpoint are considered overlapping.

For example:

```text
[1, 2] and [2, 3]
```

They share the point `2`.

Therefore, they must be merged.

### Q4. Why do we update `newInterval` while merging?

The merged interval may expand in either direction.

We must use the minimum start and maximum end to cover every overlapping interval.

### Q5. Why does the second loop use the updated ending boundary?

Because merging one interval can extend the new interval and cause another interval to overlap.

We must keep checking until no more overlaps exist.

### Q6. What happens if the new interval does not overlap with any existing interval?

It is simply inserted at the correct sorted position.

### Q7. What happens if the input vector is empty?

The first and second loops are skipped.

The new interval is added directly to the result.

### Q8. What if the new interval overlaps with every existing interval?

All intervals are merged into one interval.

### Q9. Can we modify the input directly?

Yes, but creating a separate result vector makes the three-phase logic simpler and easier to maintain.

## 11. Common Mistakes

- Sorting the intervals unnecessarily.
- Using `<` instead of `<=` in the overlap condition.
- Forgetting to update both the starting and ending boundaries.
- Failing to increment `i` after processing an interval.
- Adding the new interval before merging all overlapping intervals.
- Forgetting to append the remaining intervals.
- Assuming that merging stops after the first overlap.
- Forgetting that the updated new interval can overlap with additional intervals.

## 12. Comparison of Approaches

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Brute Force (Sort + Merge) | O(n log n) | O(n) |
| Optimized Three-Phase | O(n) | O(n) |

## 13. Key Takeaways

### Most Important Pattern

**Sorted Intervals + Insert New Interval → Three-Phase Greedy**

Remember these three phases:

1. Add intervals that end before the new interval starts.
2. Merge every interval that overlaps with the expanding new interval.
3. Add the merged interval and append all remaining intervals.

### Important Code

Phase 1:

```cpp
while (i < n && intervals[i][1] < newInterval[0]) {
    result.push_back(intervals[i]);
    i++;
}
```

Phase 2:

```cpp
while (i < n && intervals[i][0] <= newInterval[1]) {

    newInterval[0] = min(newInterval[0], intervals[i][0]);

    newInterval[1] = max(newInterval[1], intervals[i][1]);

    i++;
}
```

Phase 3:

```cpp
result.push_back(newInterval);

while (i < n) {
    result.push_back(intervals[i]);
    i++;
}
```

### Connection With Other NeetCode Problems

- Merge Intervals → Sorting + Greedy.
- Insert Interval → Three-Phase Greedy.
- Non-overlapping Intervals → Greedy + Sorting.
- Meeting Rooms → Interval Sorting.
- Meeting Rooms II → Intervals + Min Heap.

## 14. Final Interview Revision

Remember:

- Input intervals are already sorted.
- No need to sort again.
- First add intervals that end before the new interval starts.
- Then merge overlapping intervals.
- Finally, add the merged interval and all remaining intervals.
- Equal endpoints count as overlapping.

**Most Important Insight:**

Because the intervals are already sorted, we can divide the problem into three phases and process every interval only once.

**Final Complexity:**
- Time: `O(n)`
- Auxiliary Space: `O(1)`
- Output Space: `O(n)`

  # 56. Merge Intervals

- **Difficulty:** Medium
- **Pattern:** Intervals / Greedy
- **Data Structures:** Vector
- **Approaches:** Brute Force, Sorting + Greedy
- **Language:** C++

## 1. Problem Understanding

We are given an array of intervals.

Each interval is represented as:

```cpp
[start, end]
```

Our task is to merge all overlapping intervals and return an array of non-overlapping intervals that cover all the intervals in the input.

We may return the answer in any order.

### Important Note

Two intervals overlap if they have at least one common point.

For example:

```text
[1, 2] and [3, 4] -> Non-overlapping

[1, 2] and [2, 3] -> Overlapping
```

Intervals sharing an endpoint are considered overlapping.

### Example 1

**Input:**

```cpp
intervals = {{1, 3}, {1, 5}, {6, 7}};
```

**Output:**

```cpp
{{1, 5}, {6, 7}};
```

**Explanation:**

The intervals:

```text
[1, 3]
[1, 5]
```

overlap with each other.

Merge them:

```text
[1, 5]
```

The interval `[6,7]` does not overlap with `[1,5]`.

Final result:

```text
[1, 5], [6, 7]
```

### Example 2

**Input:**

```cpp
intervals = {{1, 2}, {2, 3}};
```

**Output:**

```cpp
{{1, 3}};
```

**Explanation:**

Both intervals share the endpoint `2`.

Therefore, they overlap and must be merged.

Result:

```text
[1, 3]
```

## 2. Core Concept — Sorting + Greedy

### Important Observation

The intervals may be given in any order.

For example:

```text
[8, 10]
[1, 3]
[2, 6]
[15, 18]
```

It is difficult to identify overlapping intervals directly without organizing them.

Therefore, we first sort intervals based on their starting positions.

After sorting:

```text
[1, 3]
[2, 6]
[8, 10]
[15, 18]
```

Now overlapping intervals become easier to identify.

### Main Greedy Idea

After sorting, compare the current interval with the last interval in the result.

There are two possibilities:

**Case 1: No Overlap**

If:

```cpp
result.back()[1] < interval[0]
```

The current interval starts after the previous interval ends.

Therefore, add it as a new interval.

**Case 2: Overlap**

If:

```cpp
result.back()[1] >= interval[0]
```

The current interval overlaps with the previous interval.

Merge them by updating:

```cpp
result.back()[1] = max(result.back()[1], interval[1]);
```

We keep the starting position unchanged because the intervals are sorted.

## 3. Approach 1 — Brute Force

### Idea

We can repeatedly compare pairs of intervals.

1. Select an interval.
2. Compare it with the remaining intervals.
3. If two intervals overlap, merge them.
4. Remove the merged interval.
5. Continue until no more overlapping intervals remain.

This approach can require repeated comparisons and modifications.

### Limitation

Repeatedly checking all interval pairs can take quadratic time or worse depending on the implementation.

We can solve the problem more efficiently by sorting the intervals first.

## 4. Optimized Approach — Sorting + Greedy

### Step 1: Sort the Intervals

```cpp
sort(intervals.begin(), intervals.end());
```

For a vector of intervals, this sorts them lexicographically:

1. First by starting position.
2. If starting positions are equal, by ending position.

### Step 2: Initialize the Result

```cpp
vector<vector<int>> result;
```

We will store the merged intervals here.

### Step 3: Traverse Every Interval

```cpp
for (auto& interval : intervals)
```

Process each interval one by one.

### Step 4: Check Whether the Result Is Empty

```cpp
if (result.empty())
```

If the result is empty, add the current interval.

### Step 5: Check for Non-Overlapping Intervals

```cpp
else if (result.back()[1] < interval[0])
```

If the previous interval ends before the current interval starts, there is no overlap.

Add the current interval as a new interval.

### Step 6: Merge Overlapping Intervals

```cpp
else {
    result.back()[1] =
        max(result.back()[1], interval[1]);
}
```

If the intervals overlap, extend the ending position of the last interval in the result.

## 5. Optimized C++ Solution

```cpp
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {

        // Sort intervals by starting position
        sort(intervals.begin(), intervals.end());

        vector<vector<int>> result;

        for (auto& interval : intervals) {

            // First interval or no overlap
            if (result.empty() ||
                result.back()[1] < interval[0]) {

                result.push_back(interval);
            }

            // Overlapping intervals
            else {

                result.back()[1] =
                    max(result.back()[1], interval[1]);
            }
        }

        return result;
    }
};
```

## 6. Understanding the Code

### Sorting

```cpp
sort(intervals.begin(), intervals.end());
```

Sorting ensures that intervals are processed in ascending order of their starting positions.

For example:

Before sorting:

```text
[8, 10]
[1, 3]
[2, 6]
```

After sorting:

```text
[1, 3]
[2, 6]
[8, 10]
```

### Checking Whether the Result Is Empty

```cpp
if (result.empty())
```

The first interval must always be added because there is no previous interval to compare it with.

### Checking for No Overlap

```cpp
result.back()[1] < interval[0]
```

Here:

- `result.back()[1]` is the ending position of the last merged interval.
- `interval[0]` is the starting position of the current interval.

If the previous interval ends before the current interval starts, they do not overlap.

### Merging Intervals

```cpp
result.back()[1] =
    max(result.back()[1], interval[1]);
```

We update the ending position to the maximum of both ending positions.

Why?

Because the merged interval must cover both intervals completely.

The starting position remains unchanged because sorting guarantees that the current interval starts at or after the previous interval.

## 7. Dry Run — Step by Step

**Input:**

```cpp
intervals = {
    {1, 3},
    {1, 5},
    {6, 7}
};
```

### Step 1: Sort the Intervals

Sorted input:

```text
[1, 3]
[1, 5]
[6, 7]
```

### Step 2: Initialize Result

```cpp
result = {};
```

### Step 3: Process First Interval

Current interval:

```text
[1, 3]
```

The result is empty.

Therefore, add it:

```cpp
result = {{1, 3}};
```

### Step 4: Process Second Interval

Current interval:

```text
[1, 5]
```

Previous merged interval:

```text
[1, 3]
```

Check:

```cpp
3 < 1
```

Result:

```text
false
```

Therefore, the intervals overlap.

Merge:

```cpp
result.back()[1] = max(3, 5);
```

Updated result:

```cpp
result = {{1, 5}};
```

### Step 5: Process Third Interval

Current interval:

```text
[6, 7]
```

Previous merged interval:

```text
[1, 5]
```

Check:

```cpp
5 < 6
```

Result:

```text
true
```

There is no overlap.

Add the current interval:

```cpp
result = {
    {1, 5},
    {6, 7}
};
```

**Final Output:**

```cpp
{{1, 5}, {6, 7}}
```

## 8. Dry Run — Multiple Overlapping Intervals

**Input:**

```cpp
intervals = {
    {1, 3},
    {2, 6},
    {8, 10},
    {15, 18}
};
```

### Step 1: Sort

The intervals are already sorted:

```text
[1, 3]
[2, 6]
[8, 10]
[15, 18]
```

### Step 2: Process [1, 3]

Result is empty.

Add:

```text
[1, 3]
```

### Step 3: Process [2, 6]

Check:

```cpp
3 < 2
```

False.

They overlap.

Merge:

```text
[1, 6]
```

### Step 4: Process [8, 10]

Check:

```cpp
6 < 8
```

True.

No overlap.

Add:

```text
[8, 10]
```

### Step 5: Process [15, 18]

Check:

```cpp
10 < 15
```

True.

No overlap.

Add:

```text
[15, 18]
```

**Final Result:**

```cpp
{
    {1, 6},
    {8, 10},
    {15, 18}
};
```

## 9. Why Does Greedy Work?

After sorting, every interval starts at or after the starting position of the previous interval.

Therefore, we only need to compare the current interval with the last merged interval.

If the current interval starts after the last merged interval ends, no overlap exists.

Otherwise, the intervals overlap and can be merged.

We do not need to compare the current interval with every previous interval because all earlier intervals have already been merged into the result.

This makes the algorithm efficient.

## 10. Time and Space Complexity

Let `n` be the number of intervals.

### Time Complexity

Sorting:

```text
O(n log n)
```

Traversing and merging:

```text
O(n)
```

Overall:

**Time Complexity: O(n log n)**

### Space Complexity

The result vector can contain up to `n` intervals.

Therefore:

**Output Space: O(n)**

The algorithm uses only constant extra variables apart from sorting implementation details.

**Auxiliary Space:** Depends on the sorting implementation; typically `O(log n)` stack space for `std::sort`.

## 11. Important Interview Insights

### Q1. Why do we sort the intervals first?

Because intervals may be provided in any order.

Sorting makes overlapping intervals easier to identify by processing them from left to right.

### Q2. Why do we compare only with `result.back()`?

Because all previously processed overlapping intervals have already been merged.

The last interval in the result represents the current merged range.

### Q3. Why do we use `<` instead of `<=` in the non-overlap condition?

Because intervals sharing an endpoint are considered overlapping.

Example:

```text
[1, 2]
[2, 3]
```

They share the point `2`.

Therefore, they must be merged into:

```text
[1, 3]
```

### Q4. Why do we use `max()` while merging?

The merged interval must extend to the farthest ending position.

For example:

```text
[1, 6]
[2, 4]
```

The merged interval remains:

```text
[1, 6]
```

Using `max()` prevents the ending position from shrinking.

### Q5. Why don't we update the starting position?

Because the intervals are sorted by starting position.

The current interval cannot start before the previous merged interval.

Therefore, the previous starting position is already the correct starting position.

### Q6. What happens if all intervals overlap?

All intervals are merged into one interval.

### Q7. What happens if no intervals overlap?

Every interval is added separately to the result.

### Q8. Is this problem similar to Insert Interval?

Yes.

Both problems use interval merging.

The difference is:

- Merge Intervals: Input can be unsorted, so we sort first.
- Insert Interval: Input is already sorted and non-overlapping, so sorting is unnecessary.

## 12. Common Mistakes

- Forgetting to sort the intervals.
- Using `<=` instead of `<` in the non-overlap condition.
- Comparing with the original previous interval instead of `result.back()`.
- Updating the ending position using `min()` instead of `max()`.
- Forgetting to handle an empty result vector.
- Assuming that every overlapping interval can be merged only once.
- Updating the starting position unnecessarily.
- Using nested loops when a single traversal is sufficient after sorting.

## 13. Comparison of Approaches

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Brute Force Pairwise Merging | O(n²) or worse, depending on implementation | Depends on implementation |
| Sorting + Greedy | O(n log n) | O(n) output space |

## 14. Key Takeaways

### Most Important Pattern

**Unsorted Intervals + Merge Overlaps → Sorting + Greedy**

Remember these three steps:

1. Sort intervals by starting position.
2. Compare each interval with the last interval in the result.
3. Merge if overlapping; otherwise, add a new interval.

### Important Code

Sorting:

```cpp
sort(intervals.begin(), intervals.end());
```

No-overlap condition:

```cpp
result.back()[1] < interval[0]
```

Merge condition:

```cpp
result.back()[1] =
    max(result.back()[1], interval[1]);
```

### Connection With Other NeetCode Problems

- Insert Interval → Three-Phase Greedy.
- Merge Intervals → Sorting + Greedy.
- Non-overlapping Intervals → Greedy + Sorting.
- Meeting Rooms → Interval Sorting.
- Meeting Rooms II → Intervals + Min Heap.

## 15. Final Interview Revision

Remember:

- Sort intervals first.
- The result stores already merged intervals.
- Compare the current interval with `result.back()`.
- If the previous end is smaller than the current start, add a new interval.
- Otherwise, merge by taking the maximum ending position.
- Equal endpoints count as overlapping.

**Most Important Insight:**

After sorting, we only need to compare each interval with the last merged interval.

This converts the problem into one linear traversal after sorting.

**Final Complexity:**

- Time: `O(n log n)`
- Auxiliary Space: Typically `O(log n)` for sorting.
- Output Space: `O(n)`

  # 435. Non-overlapping Intervals

- **Difficulty:** Medium
- **Pattern:** Greedy + Sorting
- **Data Structures:** Vector
- **Language:** C++
- **Key Concept:** Activity Selection / Interval Scheduling

## 1. Problem Understanding

We are given an array of intervals.

Each interval is represented as:

```cpp
[start, end]
```

Our task is to return the minimum number of intervals that must be removed so that the remaining intervals do not overlap.

### Important Note

Intervals are considered non-overlapping even if they share a common endpoint.

For example:

```text
[1, 2] and [2, 3] -> Non-overlapping

[1, 3] and [2, 4] -> Overlapping
```

This means an interval can start exactly when the previous interval ends.

### Example 1

**Input:**

```cpp
intervals = {{1, 2}, {2, 4}, {1, 4}};
```

**Output:**

```cpp
1
```

**Explanation:**

We need to remove only one interval.

Remove:

```text
[1, 4]
```

The remaining intervals are:

```text
[1, 2]
[2, 4]
```

These intervals do not overlap.

Therefore, the minimum number of removals is `1`.

### Example 2

**Input:**

```cpp
intervals = {{1, 2}, {2, 4}};
```

**Output:**

```cpp
0
```

**Explanation:**

Both intervals are already non-overlapping because they share only an endpoint.

No removal is required.

## 2. Core Concept — Greedy Algorithm

The most important observation is:

**To maximize the number of non-overlapping intervals, always select the interval that finishes earliest.**

Why?

An interval that finishes earlier leaves more room for upcoming intervals.

This is the same greedy strategy used in the Activity Selection Problem.

### Greedy Strategy

1. Sort intervals by their ending position.
2. Select the first interval.
3. Track its ending position.
4. Traverse the remaining intervals.
5. If the current interval starts at or after the previous selected interval's end, select it.
6. Otherwise, remove it.
7. Return the total number of removed intervals.

### Important Formula

If:

```cpp
currentStart >= previousEnd
```

The current interval does not overlap with the previous selected interval.

Otherwise:

```cpp
currentStart < previousEnd
```

The intervals overlap, so we remove one.

## 3. Approach 1 — Brute Force

### Idea

We can try different combinations of intervals and find the maximum number of intervals that can be kept without overlapping.

Suppose there are `n` intervals.

For every interval, we have two choices:

- Keep the interval.
- Remove the interval.

We explore possible combinations and check whether the selected intervals are non-overlapping.

Finally:

```cpp
minimumRemovals = totalIntervals - maximumIntervalsKept;
```

### Limitation

There can be up to `2^n` possible subsets.

Therefore, this approach is exponential and inefficient.

We can optimize it using Greedy + Sorting.

## 4. Optimized Approach — Sorting by Ending Time + Greedy

### Step 1: Sort by Ending Position

```cpp
sort(intervals.begin(), intervals.end(),
     [](vector<int>& a, vector<int>& b) {
         return a[1] < b[1];
     });
```

Why sort by ending position?

We want to select intervals that finish as early as possible.

This leaves maximum available space for future intervals.

### Step 2: Initialize Variables

```cpp
int removals = 0;
int prevEnd = intervals[0][1];
```

- `removals` stores the number of intervals removed.
- `prevEnd` stores the ending position of the last selected interval.

We initially select the first interval because it has the earliest ending position.

### Step 3: Traverse Remaining Intervals

```cpp
for (int i = 1; i < intervals.size(); i++)
```

Check whether the current interval overlaps with the previously selected interval.

### Step 4: Check for Overlap

```cpp
if (intervals[i][0] < prevEnd)
```

If the current interval starts before the previous selected interval ends, they overlap.

Therefore, remove the current interval.

```cpp
removals++;
```

We do not update `prevEnd` because the previously selected interval ends earlier and is the better greedy choice.

### Step 5: Select Non-Overlapping Interval

If the current interval starts at or after `prevEnd`, we can keep it.

```cpp
else {
    prevEnd = intervals[i][1];
}
```

Update the ending position to the ending position of the newly selected interval.

## 5. Optimized C++ Solution

```cpp
class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {

        // Sort intervals by ending position
        sort(intervals.begin(), intervals.end(),
             [](vector<int>& a, vector<int>& b) {
                 return a[1] < b[1];
             });

        int removals = 0;

        // Select the first interval
        int prevEnd = intervals[0][1];

        // Traverse remaining intervals
        for (int i = 1; i < intervals.size(); i++) {

            // Overlapping interval
            if (intervals[i][0] < prevEnd) {
                removals++;
            }

            // Non-overlapping interval
            else {
                prevEnd = intervals[i][1];
            }
        }

        return removals;
    }
};
```

## 6. Understanding the Greedy Decision

Consider:

```text
[1, 4]
[2, 3]
[3, 5]
```

If we select `[1,4]` first, we cannot select either of the other two intervals.

But if we select `[2,3]` first:

- `[2,3]` is selected.
- `[3,5]` can also be selected.

Therefore, selecting the interval with the earliest ending position gives us a better opportunity to keep more intervals.

This is why we sort by ending time.

## 7. Dry Run — Step by Step

**Input:**

```cpp
intervals = {
    {1, 2},
    {2, 4},
    {1, 4}
};
```

### Step 1: Sort by Ending Position

Sorted intervals:

```text
[1, 2]
[2, 4]
[1, 4]
```

### Step 2: Initialize

```cpp
removals = 0;
prevEnd = 2;
```

We select the first interval:

```text
[1, 2]
```

### Step 3: Process [2, 4]

Current interval:

```text
[2, 4]
```

Check:

```cpp
2 < 2
```

Result:

```text
false
```

Therefore, the interval does not overlap.

Select it and update:

```cpp
prevEnd = 4;
```

### Step 4: Process [1, 4]

Current interval:

```text
[1, 4]
```

Check:

```cpp
1 < 4
```

Result:

```text
true
```

The interval overlaps with the previously selected interval.

Remove it:

```cpp
removals++;
```

Updated value:

```cpp
removals = 1;
```

We do not update `prevEnd`.

### Final Result

```cpp
return 1;
```

**Output:**

```text
1
```

## 8. Dry Run — Another Example

**Input:**

```cpp
intervals = {
    {1, 3},
    {2, 4},
    {3, 5},
    {4, 6}
};
```

### Step 1: Sort by Ending Position

```text
[1, 3]
[2, 4]
[3, 5]
[4, 6]
```

### Step 2: Initialize

```cpp
removals = 0;
prevEnd = 3;
```

Select `[1,3]`.

### Step 3: Process [2,4]

Check:

```cpp
2 < 3
```

True.

Remove `[2,4]`.

```cpp
removals = 1;
```

`prevEnd` remains `3`.

### Step 4: Process [3,5]

Check:

```cpp
3 < 3
```

False.

Select `[3,5]`.

```cpp
prevEnd = 5;
```

### Step 5: Process [4,6]

Check:

```cpp
4 < 5
```

True.

Remove `[4,6]`.

```cpp
removals = 2;
```

### Final Result

```cpp
2
```

We removed two intervals and kept:

```text
[1, 3]
[3, 5]
```

These intervals do not overlap.

## 9. Alternative Optimized Approach — Sorting by Starting Time

We can also solve this problem by sorting intervals based on their starting positions.

### Idea

1. Sort intervals by starting position.
2. Track the ending position of the previous selected interval.
3. If intervals overlap, remove the interval that ends later.
4. Otherwise, select the current interval.

### C++ Code

```cpp
class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {

        sort(intervals.begin(), intervals.end());

        int removals = 0;
        int prevEnd = intervals[0][1];

        for (int i = 1; i < intervals.size(); i++) {

            // Overlapping intervals
            if (intervals[i][0] < prevEnd) {

                removals++;

                // Keep the interval that ends earlier
                prevEnd = min(prevEnd, intervals[i][1]);
            }

            // Non-overlapping interval
            else {
                prevEnd = intervals[i][1];
            }
        }

        return removals;
    }
};
```

### Why Does This Work?

When two intervals overlap, we should keep the one that finishes earlier.

For example:

```text
[1, 5]
[2, 3]
```

Both intervals overlap.

Keeping `[2,3]` is better because it finishes earlier and leaves more room for future intervals.

Therefore:

```cpp
prevEnd = min(prevEnd, intervals[i][1]);
```

This ensures that we retain the interval with the smaller ending position.

### Complexity

- Time: `O(n log n)`
- Auxiliary Space: Typically `O(log n)` for sorting.
- Output Space: `O(1)` apart from sorting-related space.

## 10. Why Does Greedy Work?

Our objective is to remove the minimum number of intervals.

This is equivalent to keeping the maximum number of non-overlapping intervals.

The earliest-ending interval is always a good greedy choice because it leaves the maximum remaining space for future intervals.

After selecting it, we repeat the same process with the remaining intervals.

This is the classic Activity Selection strategy.

## 11. Time and Space Complexity

Let `n` be the number of intervals.

### Time Complexity

Sorting:

```text
O(n log n)
```

Traversing the intervals:

```text
O(n)
```

Overall:

**Time Complexity: O(n log n)**

### Space Complexity

The algorithm does not create another result vector.

It only uses a few variables.

The sorting implementation may require additional stack space.

**Auxiliary Space:** Typically `O(log n)` for `std::sort`.

**Output Space:** `O(1)` because the function returns only an integer.

## 12. Important Interview Insights

### Q1. Why do we sort by ending time?

Because selecting the interval that finishes earliest leaves maximum space for future intervals.

### Q2. Why do we remove the current interval when overlap occurs?

Because intervals are sorted by ending time.

The previously selected interval ends earlier or at the same time, making it the better choice.

### Q3. Why don't we update `prevEnd` when removing an interval?

Because we are keeping the previous interval.

Its ending position is still the correct reference for checking future intervals.

### Q4. Why is this an Activity Selection Problem?

Both problems aim to select the maximum number of mutually compatible intervals.

The minimum removals are simply:

```cpp
minimumRemovals = n - maximumIntervalsKept;
```

### Q5. Why do we use `<` instead of `<=`?

Because intervals sharing an endpoint are considered non-overlapping.

For example:

```text
[1, 2]
[2, 3]
```

They can both be selected.

Therefore, the overlap condition is:

```cpp
currentStart < prevEnd
```

### Q6. Can we solve this by sorting according to starting time?

Yes.

When intervals overlap, retain the interval with the smaller ending position.

This can be implemented using:

```cpp
prevEnd = min(prevEnd, intervals[i][1]);
```

### Q7. What is the relationship between removals and selected intervals?

```cpp
removals = totalIntervals - selectedIntervals;
```

## 13. Common Mistakes

- Sorting by starting position but using the earliest-ending greedy logic without adjusting the implementation.
- Updating `prevEnd` when removing an interval in the ending-time approach.
- Using `<=` instead of `<` in the overlap condition.
- Removing the interval that ends earlier instead of the one that ends later.
- Forgetting that equal endpoints are allowed.
- Using nested loops unnecessarily.
- Forgetting to handle the possibility of an empty input if adapting the solution beyond the given problem constraints.

## 14. Comparison With Other Interval Problems

| Problem | Main Objective | Strategy |
|---|---|---|
| Merge Intervals | Merge all overlapping intervals | Sort by start + Greedy |
| Insert Interval | Insert and merge a new interval | Three-phase Greedy |
| Non-overlapping Intervals | Remove minimum intervals | Sort by end + Greedy |
| Meeting Rooms | Check whether meetings overlap | Sorting |
| Meeting Rooms II | Find minimum rooms required | Sorting + Min Heap |

## 15. Key Takeaways

### Most Important Pattern

**Minimum Interval Removals → Maximum Compatible Intervals → Sort by Ending Time + Greedy**

Remember:

1. Sort intervals by ending position.
2. Select the first interval.
3. Track its ending position using `prevEnd`.
4. If `currentStart < prevEnd`, remove the current interval.
5. Otherwise, select it and update `prevEnd`.

### Most Important Code

Sorting by ending position:

```cpp
sort(intervals.begin(), intervals.end(),
     [](vector<int>& a, vector<int>& b) {
         return a[1] < b[1];
     });
```

Overlap condition:

```cpp
if (intervals[i][0] < prevEnd)
```

Removal:

```cpp
removals++;
```

Selecting a non-overlapping interval:

```cpp
prevEnd = intervals[i][1];
```

### Final Interview Revision

**Core Insight:**

When choosing between overlapping intervals, prefer the interval that finishes earlier.

This leaves more room for future intervals and maximizes the number of intervals we can keep.

**Final Complexity:**

- Time: `O(n log n)`
- Auxiliary Space: Typically `O(log n)` for sorting.
- Output Space: `O(1)`

  # 252. Meeting Rooms

- **Difficulty:** Easy
- **Pattern:** Intervals + Sorting
- **Data Structures:** Vector
- **Language:** C++
- **Core Concept:** Detect Overlapping Intervals

## 1. Problem Understanding

We are given an array of meeting intervals.

Each interval contains:

```cpp
[start, end]
```

Where:

- `start` represents the meeting's starting time.
- `end` represents the meeting's ending time.

Our task is to determine whether a person can attend all meetings without any scheduling conflicts.

The intervals may be provided in any order.

### Important Note

Two meetings do not conflict if the first meeting ends exactly when the second meeting starts.

For example:

```text
[0, 8] and [8, 10] -> No conflict
```

But:

```text
[0, 10] and [5, 15] -> Conflict
```

### Example 1

**Input:**

```cpp
intervals = {
    {0, 30},
    {5, 10},
    {15, 20}
};
```

**Output:**

```cpp
false
```

**Explanation:**

The meeting `[0,30]` overlaps with:

- `[5,10]`
- `[15,20]`

A person cannot attend all three meetings.

Therefore, return `false`.

### Example 2

**Input:**

```cpp
intervals = {
    {5, 8},
    {9, 15}
};
```

**Output:**

```cpp
true
```

**Explanation:**

The first meeting ends at `8`, and the second meeting starts at `9`.

There is no overlap.

Therefore, return `true`.

## 2. Core Concept — Sorting + Greedy

### Important Observation

The intervals can be provided in any order.

For example:

```text
[15, 20]
[0, 30]
[5, 10]
```

Checking conflicts directly becomes difficult.

Therefore, first sort the intervals by their starting positions.

After sorting:

```text
[0, 30]
[5, 10]
[15, 20]
```

Now we only need to compare adjacent intervals.

### Main Idea

For every pair of consecutive intervals, check:

```cpp
currentStart < previousEnd
```

If this condition is true, the meetings overlap.

Therefore, return `false`.

If no overlapping pair exists, return `true`.

## 3. Approach — Sorting + Checking Adjacent Intervals

### Step 1: Handle Empty Input

If there are no meetings, there cannot be any scheduling conflict.

Return `true`.

### Step 2: Sort Intervals by Starting Time

```cpp
sort(intervals.begin(), intervals.end(),
     [](Interval& a, Interval& b) {
         return a.start < b.start;
     });
```

Sorting ensures that meetings are arranged in chronological order.

### Step 3: Traverse the Intervals

Start from the second interval:

```cpp
for (int i = 1; i < intervals.size(); i++)
```

We compare each meeting with the previous meeting.

### Step 4: Check for Overlap

```cpp
if (intervals[i].start < intervals[i - 1].end)
```

If the current meeting starts before the previous meeting ends, there is a scheduling conflict.

Return:

```cpp
false;
```

### Step 5: Return True

If the loop finishes without finding any overlap, all meetings can be attended.

Return:

```cpp
true;
```

## 4. Optimized C++ Solution

```cpp
/**
 * Definition of Interval:
 *
 * class Interval {
 * public:
 *     int start, end;
 *
 *     Interval(int start, int end) {
 *         this->start = start;
 *         this->end = end;
 *     }
 * };
 */

class Solution {
public:
    bool canAttendMeetings(vector<Interval>& intervals) {

        // Sort meetings by starting time
        sort(intervals.begin(), intervals.end(),
             [](Interval& a, Interval& b) {
                 return a.start < b.start;
             });

        // Check adjacent meetings
        for (int i = 1; i < intervals.size(); i++) {

            // Overlapping meetings
            if (intervals[i].start < intervals[i - 1].end) {
                return false;
            }
        }

        // No scheduling conflicts
        return true;
    }
};
```

## 5. Understanding the Code

### Sorting

```cpp
sort(intervals.begin(), intervals.end(),
     [](Interval& a, Interval& b) {
         return a.start < b.start;
     });
```

We sort the intervals using their starting times.

For example:

Before sorting:

```text
[15, 20]
[0, 30]
[5, 10]
```

After sorting:

```text
[0, 30]
[5, 10]
[15, 20]
```

### Why Start From Index 1?

```cpp
for (int i = 1; i < intervals.size(); i++)
```

We compare the current interval with the previous interval.

For the first interval, there is no previous meeting.

Therefore, traversal starts from index `1`.

### Overlap Condition

```cpp
intervals[i].start < intervals[i - 1].end
```

Here:

- `intervals[i].start` is the current meeting's starting time.
- `intervals[i - 1].end` is the previous meeting's ending time.

If the current meeting starts before the previous meeting ends, they overlap.

### Early Return

```cpp
return false;
```

As soon as we find one conflict, we know that the person cannot attend all meetings.

There is no need to check the remaining intervals.

## 6. Dry Run — Step by Step

**Input:**

```cpp
intervals = {
    {0, 30},
    {5, 10},
    {15, 20}
};
```

### Step 1: Sort the Intervals

The intervals are already sorted:

```text
[0, 30]
[5, 10]
[15, 20]
```

### Step 2: Initialize Traversal

```cpp
i = 1;
```

### Step 3: Compare First and Second Meetings

Previous meeting:

```text
[0, 30]
```

Current meeting:

```text
[5, 10]
```

Check:

```cpp
5 < 30
```

Result:

```text
true
```

There is an overlap.

Therefore:

```cpp
return false;
```

We do not need to process the remaining interval.

**Final Output:**

```cpp
false
```

## 7. Dry Run — No Overlapping Meetings

**Input:**

```cpp
intervals = {
    {5, 8},
    {9, 15},
    {16, 20}
};
```

### Step 1: Sort

```text
[5, 8]
[9, 15]
[16, 20]
```

### Step 2: Compare [5, 8] and [9, 15]

Check:

```cpp
9 < 8
```

False.

No overlap.

### Step 3: Compare [9, 15] and [16, 20]

Check:

```cpp
16 < 15
```

False.

No overlap.

### Step 4: Finish Traversal

No overlapping intervals were found.

**Final Output:**

```cpp
true
```

## 8. Time and Space Complexity

Let `n` be the number of meetings.

### Time Complexity

Sorting:

```text
O(n log n)
```

Checking adjacent intervals:

```text
O(n)
```

Overall:

**Time Complexity: O(n log n)**

### Space Complexity

The algorithm does not create another vector.

It only uses a few variables apart from sorting.

**Auxiliary Space:** Typically `O(log n)` for `std::sort`.

**Output Space:** `O(1)`.

## 9. Important Interview Insights

### Q1. Why do we sort intervals first?

Because intervals may be given in any order.

Sorting arranges meetings chronologically, making it possible to check adjacent meetings.

### Q2. Why do we compare only adjacent intervals?

After sorting by starting time, if a meeting overlaps with any earlier meeting, it must also overlap with the immediately preceding interval.

Therefore, checking adjacent intervals is sufficient to detect whether any conflict exists.

### Q3. Why do we use `<` instead of `<=`?

Because meetings that share an endpoint do not conflict.

Example:

```text
[0, 8]
[8, 10]
```

The first meeting finishes exactly when the second begins.

Therefore:

```cpp
8 < 8
```

is false, correctly indicating no conflict.

### Q4. Why can we return false immediately?

We only need to determine whether at least one conflict exists.

Once a conflict is found, the answer is definitely false.

### Q5. Can we solve this without sorting?

A brute-force approach can compare every pair of meetings.

However, it takes `O(n²)` time.

Sorting provides a more efficient solution.

### Q6. Is this the same as Merge Intervals?

No.

Both problems involve sorting and intervals, but their objectives are different.

- Meeting Rooms: Detect whether any overlap exists.
- Merge Intervals: Combine overlapping intervals.

## 10. Common Mistakes

- Forgetting to sort intervals before checking conflicts.
- Sorting by ending time instead of starting time.
- Using `<=` instead of `<` in the overlap condition.
- Comparing the current interval with the wrong interval.
- Starting traversal from index `0` and accessing `i - 1`.
- Checking every pair unnecessarily.
- Forgetting that meetings sharing an endpoint are allowed.

## 11. Comparison With Other Interval Problems

| Problem | Objective | Main Strategy |
|---|---|---|
| Merge Intervals | Merge overlapping intervals | Sort by start + Greedy |
| Insert Interval | Insert and merge a new interval | Three-phase Greedy |
| Non-overlapping Intervals | Minimize removals | Sort by end + Greedy |
| Meeting Rooms | Detect scheduling conflicts | Sort by start + Adjacent Check |
| Meeting Rooms II | Find minimum meeting rooms | Min Heap + Sorting |

## 12. Key Takeaways

### Most Important Pattern

**Meeting Conflict Detection → Sort by Start Time + Compare Adjacent Intervals**

Remember these steps:

1. Sort meetings by starting time.
2. Start traversal from index `1`.
3. Compare current start with previous end.
4. If `currentStart < previousEnd`, return `false`.
5. If no conflict exists, return `true`.

### Most Important Code

Sorting:

```cpp
sort(intervals.begin(), intervals.end(),
     [](Interval& a, Interval& b) {
         return a.start < b.start;
     });
```

Overlap condition:

```cpp
if (intervals[i].start < intervals[i - 1].end)
```

Final result:

```cpp
return true;
```

### Final Interview Revision

**Core Insight:**

After sorting meetings by starting time, we only need to check whether any meeting starts before the previous meeting ends.

If it does, a scheduling conflict exists.

**Final Complexity:**

- Time: `O(n log n)`
- Auxiliary Space: Typically `O(log n)` for sorting.
- Output Space: `O(1)`

  # 253. Meeting Rooms II

- **Difficulty:** Medium
- **Pattern:** Intervals + Sorting + Min Heap
- **Data Structures:** Vector, Priority Queue
- **Language:** C++
- **Core Concept:** Minimum Resources Required for Overlapping Intervals

## 1. Problem Understanding

We are given an array of meeting intervals.

Each interval contains:

```cpp
[start, end]
```

Where:

- `start` represents the meeting's starting time.
- `end` represents the meeting's ending time.

Our task is to find the minimum number of meeting rooms required to schedule all meetings without conflicts.

### Important Note

Two meetings do not conflict if one meeting ends exactly when another meeting starts.

For example:

```text
[0, 8] and [8, 10] -> No conflict
```

But:

```text
[0, 10] and [5, 15] -> Conflict
```

If multiple meetings happen simultaneously, each meeting requires a separate room.

### Example 1

**Input:**

```cpp
intervals = {
    {0, 40},
    {5, 10},
    {15, 20}
};
```

**Output:**

```cpp
2
```

**Explanation:**

We need two meeting rooms.

Room 1:

```text
[0, 40]
```

Room 2:

```text
[5, 10]
[15, 20]
```

The meetings `[5,10]` and `[15,20]` can use the same room because the first meeting finishes before the second starts.

Therefore, the minimum number of rooms required is `2`.

### Example 2

**Input:**

```cpp
intervals = {
    {4, 9}
};
```

**Output:**

```cpp
1
```

Only one meeting exists, so only one room is required.

## 2. Core Concept — Sorting + Min Heap

### Important Observation

The minimum number of meeting rooms required is equal to the maximum number of meetings happening simultaneously.

We need to efficiently track when the currently running meetings finish.

A Min Heap is useful because it always gives us the earliest ending meeting.

### Why Min Heap?

A Min Heap stores meeting ending times.

The smallest ending time is always available at the top:

```cpp
minHeap.top()
```

This helps us determine whether a room has become free.

### Main Greedy Idea

1. Sort all meetings by their starting times.
2. Create a Min Heap to store ending times.
3. Process meetings one by one.
4. Check the earliest ending meeting.
5. If that meeting has finished, reuse its room.
6. Otherwise, allocate another room.
7. The maximum heap size represents the minimum number of rooms required.

## 3. Approach 1 — Brute Force

### Idea

We can process meetings one by one and assign each meeting to an available room.

For every meeting:

1. Check all existing rooms.
2. Find a room where the previous meeting has finished.
3. If a room is available, reuse it.
4. Otherwise, create a new room.

### Limitation

Checking all existing rooms for every meeting can require quadratic time.

In the worst case:

**Time Complexity: O(n²)**

We can improve this by using a Min Heap to efficiently find the room that becomes available earliest.

## 4. Optimized Approach — Sorting + Min Heap

### Step 1: Sort Meetings by Starting Time

```cpp
sort(intervals.begin(), intervals.end(),
     [](Interval& a, Interval& b) {
         return a.start < b.start;
     });
```

Sorting ensures that meetings are processed chronologically.

### Step 2: Create a Min Heap

```cpp
priority_queue<int, vector<int>, greater<int>> minHeap;
```

The heap stores the ending times of meetings currently assigned to rooms.

The smallest ending time is always at the top.

### Step 3: Process Every Meeting

```cpp
for (auto& meeting : intervals)
```

For every meeting, check whether an existing room is available.

### Step 4: Check Whether a Room Is Available

```cpp
if (!minHeap.empty() &&
    minHeap.top() <= meeting.start)
```

If the earliest ending meeting finishes before or exactly when the current meeting starts, that room can be reused.

Remove the previous ending time:

```cpp
minHeap.pop();
```

### Step 5: Allocate the Current Meeting

Whether we reuse an existing room or allocate a new one, insert the current meeting's ending time:

```cpp
minHeap.push(meeting.end);
```

### Step 6: Return the Number of Rooms

The heap contains one ending time for each room currently in use.

Therefore:

```cpp
return minHeap.size();
```

## 5. Optimized C++ Solution

```cpp
/**
 * Definition of Interval:
 *
 * class Interval {
 * public:
 *     int start, end;
 *
 *     Interval(int start, int end) {
 *         this->start = start;
 *         this->end = end;
 *     }
 * };
 */

class Solution {
public:
    int minMeetingRooms(vector<Interval>& intervals) {

        // Sort meetings by starting time
        sort(intervals.begin(), intervals.end(),
             [](Interval& a, Interval& b) {
                 return a.start < b.start;
             });

        // Min Heap stores meeting ending times
        priority_queue<int, vector<int>, greater<int>> minHeap;

        // Process each meeting
        for (auto& meeting : intervals) {

            // Reuse a room if the earliest meeting has finished
            if (!minHeap.empty() &&
                minHeap.top() <= meeting.start) {

                minHeap.pop();
            }

            // Allocate the current meeting
            minHeap.push(meeting.end);
        }

        // Number of rooms required
        return minHeap.size();
    }
};
```

## 6. Understanding the Code

### Sorting

```cpp
sort(intervals.begin(), intervals.end(),
     [](Interval& a, Interval& b) {
         return a.start < b.start;
     });
```

We sort meetings according to their starting times.

Example:

Before sorting:

```text
[15, 20]
[0, 40]
[5, 10]
```

After sorting:

```text
[0, 40]
[5, 10]
[15, 20]
```

### Min Heap Declaration

```cpp
priority_queue<int, vector<int>, greater<int>> minHeap;
```

This creates a Min Heap.

Unlike the default C++ priority queue, which is a Max Heap, this one keeps the smallest element at the top.

For example:

```text
Heap: [40, 10, 20]

Top: 10
```

The top represents the earliest meeting ending time.

### Checking Room Availability

```cpp
minHeap.top() <= meeting.start
```

If the earliest ending meeting finishes before or exactly when the current meeting starts, we can reuse that room.

### Removing the Finished Meeting

```cpp
minHeap.pop();
```

We remove the previous meeting's ending time because its room is now available.

### Adding the Current Meeting

```cpp
minHeap.push(meeting.end);
```

We insert the current meeting's ending time.

This represents the room being occupied until that meeting finishes.

### Returning Heap Size

```cpp
return minHeap.size();
```

Each heap element represents one room currently occupied by a meeting.

The heap size therefore gives the number of rooms required.

## 7. Dry Run — Step by Step

**Input:**

```cpp
intervals = {
    {0, 40},
    {5, 10},
    {15, 20}
};
```

### Step 1: Sort Meetings

```text
[0, 40]
[5, 10]
[15, 20]
```

Initialize:

```cpp
minHeap = {};
```

### Step 2: Process [0, 40]

Heap is empty.

Allocate a new room.

```cpp
minHeap.push(40);
```

Heap:

```text
[40]
```

Rooms required:

```text
1
```

### Step 3: Process [5, 10]

Current meeting starts at `5`.

Earliest ending meeting:

```text
40
```

Check:

```cpp
40 <= 5
```

False.

The existing room is still occupied.

Therefore, allocate another room.

```cpp
minHeap.push(10);
```

Heap:

```text
[10, 40]
```

Rooms required:

```text
2
```

### Step 4: Process [15, 20]

Current meeting starts at `15`.

Earliest ending meeting:

```text
10
```

Check:

```cpp
10 <= 15
```

True.

The meeting ending at `10` has finished.

Reuse its room:

```cpp
minHeap.pop();
```

Heap:

```text
[40]
```

Now add the current meeting:

```cpp
minHeap.push(20);
```

Heap:

```text
[20, 40]
```

Rooms required:

```text
2
```

### Final Result

```cpp
return minHeap.size();
```

Output:

```text
2
```

## 8. Dry Run — All Meetings Overlap

**Input:**

```cpp
intervals = {
    {0, 10},
    {2, 12},
    {4, 14},
    {6, 16}
};
```

Sorted intervals:

```text
[0, 10]
[2, 12]
[4, 14]
[6, 16]
```

### Process [0, 10]

Heap:

```text
[10]
```

Rooms:

```text
1
```

### Process [2, 12]

Check:

```cpp
10 <= 2
```

False.

Heap:

```text
[10, 12]
```

Rooms:

```text
2
```

### Process [4, 14]

Check:

```cpp
10 <= 4
```

False.

Heap:

```text
[10, 12, 14]
```

Rooms:

```text
3
```

### Process [6, 16]

Check:

```cpp
10 <= 6
```

False.

Heap:

```text
[10, 12, 14, 16]
```

Rooms:

```text
4
```

### Final Result

```text
4
```

All four meetings overlap, so four rooms are required.

## 9. Alternative Optimized Approach — Sorting + Two Pointers

We can also solve this problem without using a Min Heap.

### Idea

Instead of storing meeting intervals together, separate their starting and ending times.

Create two arrays:

```cpp
starts[]
ends[]
```

Sort both arrays independently.

Then use two pointers:

- `i` points to the next meeting starting.
- `j` points to the earliest meeting ending.

### Algorithm

1. Store all starting times in `starts`.
2. Store all ending times in `ends`.
3. Sort both arrays.
4. Traverse the starting times.
5. If a meeting starts before the earliest meeting ends, allocate a new room.
6. Otherwise, a room has become available. Move the ending pointer forward.
7. Track the maximum number of rooms used simultaneously.

### C++ Code

```cpp
class Solution {
public:
    int minMeetingRooms(vector<Interval>& intervals) {

        int n = intervals.size();

        if (n == 0) {
            return 0;
        }

        vector<int> starts;
        vector<int> ends;

        // Separate starting and ending times
        for (auto& meeting : intervals) {
            starts.push_back(meeting.start);
            ends.push_back(meeting.end);
        }

        // Sort both arrays
        sort(starts.begin(), starts.end());
        sort(ends.begin(), ends.end());

        int i = 0;
        int j = 0;

        int rooms = 0;
        int maxRooms = 0;

        while (i < n) {

            // A meeting starts before the earliest meeting ends
            if (starts[i] < ends[j]) {

                rooms++;
                maxRooms++;

                i++;
            }

            // A meeting has finished, so reuse its room
            else {

                rooms--;
                j++;
            }

            maxRooms = max(maxRooms, rooms);
        }

        return maxRooms;
    }
};
```

### Important Understanding

Consider:

```text
starts = [0, 5, 15]
ends   = [10, 20, 40]
```

Process:

- At time `0`, one meeting starts. Rooms = 1.
- At time `5`, another meeting starts before time `10`. Rooms = 2.
- At time `10`, the first meeting ends. One room becomes free.
- At time `15`, another meeting starts. Rooms = 2.

Maximum rooms required:

```text
2
```

### Complexity

- Sorting starts and ends: `O(n log n)`
- Two-pointer traversal: `O(n)`
- Auxiliary Space: `O(n)` for the two arrays.

**Time Complexity: O(n log n)**

**Space Complexity: O(n)**

## 10. Why Does the Min Heap Approach Work?

The heap always gives us the earliest ending meeting.

When a new meeting starts:

- If the earliest meeting has finished, its room can be reused.
- If the earliest meeting has not finished, no other room can be free earlier because all other meetings end at the same time or later.

Therefore, we only need to inspect the smallest ending time.

The heap tracks the meetings currently occupying rooms.

Its maximum size during processing represents the minimum number of rooms required.

## 11. Time and Space Complexity

Let `n` be the number of meetings.

### Min Heap Approach

Sorting:

```text
O(n log n)
```

Each meeting is inserted into the heap once:

```text
O(log n)
```

Each meeting can also be removed from the heap once:

```text
O(log n)
```

Overall:

**Time Complexity: O(n log n)**

**Auxiliary Space: O(n)**

The heap can contain up to `n` ending times.

### Two-Pointer Approach

Sorting both arrays:

```text
O(n log n)
```

Traversing both arrays:

```text
O(n)
```

Overall:

**Time Complexity: O(n log n)**

**Auxiliary Space: O(n)**

The two arrays store all starting and ending times.

## 12. Important Interview Insights

### Q1. Why do we use a Min Heap?

Because we need to know which meeting finishes earliest.

The earliest finishing meeting determines whether a room can be reused.

### Q2. Why store only ending times in the heap?

We only need to know when rooms become available.

The starting times are already processed in sorted order.

### Q3. Why do we check `minHeap.top() <= meeting.start`?

Because meetings ending exactly when another meeting starts do not overlap.

Therefore, the room can be reused.

### Q4. Why do we pop only one element?

We process meetings in increasing order of their starting times.

At most one room needs to be reused for the current meeting.

If the earliest ending meeting has not finished, none of the other meetings have finished either.

### Q5. Why is the heap size equal to the number of rooms currently required?

Each heap element represents one room's current meeting ending time.

When a room becomes available, its old ending time is removed and replaced by the new meeting's ending time.

### Q6. Why is this problem different from Meeting Rooms I?

Meeting Rooms I only checks whether any conflict exists.

Meeting Rooms II calculates how many rooms are required to accommodate all overlapping meetings.

### Q7. Can we solve this without a Min Heap?

Yes.

We can sort starting and ending times separately and use two pointers.

Both approaches have `O(n log n)` time complexity.

## 13. Common Mistakes

- Using a Max Heap instead of a Min Heap.
- Forgetting to sort meetings by starting time.
- Using `<` instead of `<=` when checking whether a room is available.
- Storing starting times instead of ending times in the heap.
- Creating a new room even when an existing room is available.
- Forgetting to remove the previous meeting's ending time before reusing a room.
- Assuming the number of meetings is equal to the number of rooms.
- Confusing this problem with Meeting Rooms I.
- Forgetting to handle empty input if adapting the solution to a version where empty input is allowed.

## 14. Comparison With Other Interval Problems

| Problem | Objective | Main Strategy |
|---|---|---|
| Merge Intervals | Merge overlapping intervals | Sorting + Greedy |
| Insert Interval | Insert and merge a new interval | Three-phase Greedy |
| Non-overlapping Intervals | Minimize removals | Sorting by End + Greedy |
| Meeting Rooms I | Detect scheduling conflicts | Sorting + Adjacent Check |
| Meeting Rooms II | Find minimum rooms required | Sorting + Min Heap |

## 15. Key Takeaways

### Most Important Pattern

**Minimum Meeting Rooms → Track Earliest Ending Meeting → Min Heap**

Remember these steps:

1. Sort meetings by starting time.
2. Create a Min Heap to store ending times.
3. For every meeting, check the earliest ending time.
4. If the earliest meeting has finished, reuse its room.
5. Insert the current meeting's ending time.
6. Return the heap size.

### Most Important Code

Min Heap declaration:

```cpp
priority_queue<int, vector<int>, greater<int>> minHeap;
```

Room availability:

```cpp
if (!minHeap.empty() && minHeap.top() <= meeting.start) {
    minHeap.pop();
}
```

Insert current meeting:

```cpp
minHeap.push(meeting.end);
```

Return rooms required:

```cpp
return minHeap.size();
```

### Final Interview Revision

**Core Insight:**

The minimum number of meeting rooms required equals the maximum number of meetings happening simultaneously.

A Min Heap helps us efficiently identify the earliest ending meeting and reuse its room whenever possible.

**Final Complexity:**

Min Heap Approach:
- Time: `O(n log n)`
- Auxiliary Space: `O(n)`

Two-Pointer Approach:
- Time: `O(n log n)`
- Auxiliary Space: `O(n)`

  # Minimum Interval to Include Each Query

- **Difficulty:** Hard
- **Pattern:** Intervals + Sorting + Min Heap
- **Data Structures:** Vector, Priority Queue
- **Language:** C++
- **Core Concept:** Find the shortest interval containing each query.

## 1. Problem Understanding

We are given:

1. A 2D array `intervals`, where each interval is represented as:

```cpp
[left, right]
```

2. An array `queries`, containing query points.

For every query, we need to find the **length of the smallest interval that contains that query**.

An interval contains a query if:

```cpp
left <= query && query <= right
```

The interval length is calculated as:

```cpp
right - left + 1
```

If no interval contains a particular query, return `-1` for that query.

### Example

**Input:**

```cpp
intervals = {
    {1, 3},
    {2, 3},
    {3, 7},
    {6, 6}
};

queries = {2, 3, 1, 7, 6, 8};
```

**Output:**

```cpp
{2, 2, 3, 5, 1, -1}
```

### Explanation

| Query | Smallest Valid Interval | Length |
|---|---|---:|
| 2 | [2, 3] | 2 |
| 3 | [2, 3] | 2 |
| 1 | [1, 3] | 3 |
| 7 | [3, 7] | 5 |
| 6 | [6, 6] | 1 |
| 8 | No valid interval | -1 |

Notice that the output must follow the original order of queries.

## 2. Core Concept

The challenge is that:

- There can be many intervals.
- There can be many queries.
- We need the shortest valid interval for every query.
- Repeatedly checking every interval for every query would be expensive.

We can optimize this using:

1. Sorting intervals by their starting points.
2. Sorting queries while remembering their original indices.
3. A Min Heap to select the shortest valid interval.

### Why Sorting?

We process queries from smallest to largest.

When processing a query, we can add every interval whose starting point is less than or equal to that query.

Because queries are sorted, we never need to add those intervals again.

### Why Min Heap?

Among all intervals that have started, we want the interval with the smallest length.

Therefore, we use a Min Heap.

Each heap element stores:

```cpp
{intervalLength, rightEndpoint}
```

The heap sorts primarily by interval length.

The shortest interval is always available at:

```cpp
minHeap.top()
```

### Important Observation

An interval is valid for query `q` if:

```cpp
left <= q && q <= right
```

Since we only add intervals with:

```cpp
left <= q
```

we only need to remove intervals that have already expired:

```cpp
right < q
```

After removing expired intervals, the shortest interval at the top of the heap is our answer.

## 3. Approach 1 — Brute Force

### Idea

For every query:

1. Initialize the answer as `INT_MAX`.
2. Traverse every interval.
3. Check whether the query belongs to that interval.
4. If valid, calculate its length.
5. Update the minimum length.
6. If no interval is valid, return `-1`.

### C++ Code

```cpp
class Solution {
public:
    vector<int> minInterval(
        vector<vector<int>>& intervals,
        vector<int>& queries
    ) {

        vector<int> ans;

        for (int q : queries) {

            int minLength = INT_MAX;

            for (auto& interval : intervals) {

                int left = interval[0];
                int right = interval[1];

                if (left <= q && q <= right) {

                    int length = right - left + 1;

                    minLength = min(minLength, length);
                }
            }

            if (minLength == INT_MAX) {
                ans.push_back(-1);
            }
            else {
                ans.push_back(minLength);
            }
        }

        return ans;
    }
};
```

### Complexity

Let:

- `n` = number of intervals.
- `m` = number of queries.

For every query, we check all intervals.

**Time Complexity: O(n × m)**

**Auxiliary Space: O(1)** excluding the output array.

### Limitation

If both arrays are large, checking every interval for every query is too expensive.

We need a better approach.

## 4. Optimized Approach — Sorting + Min Heap

### Step 1: Sort Intervals

Sort intervals according to their starting points.

```cpp
sort(intervals.begin(), intervals.end());
```

Example:

```text
Before:

[3, 7]
[1, 3]
[6, 6]
[2, 3]

After:

[1, 3]
[2, 3]
[3, 7]
[6, 6]
```

### Step 2: Store Queries With Original Indices

We need to process queries in sorted order, but the final answer must preserve their original order.

Therefore, store each query along with its original index.

```cpp
vector<pair<int, int>> sortedQueries;

for (int i = 0; i < queries.size(); i++) {

    sortedQueries.push_back({queries[i], i});
}
```

Each pair contains:

```cpp
{queryValue, originalIndex}
```

Sort these pairs:

```cpp
sort(sortedQueries.begin(), sortedQueries.end());
```

Example:

Original queries:

```text
[2, 3, 1, 7, 6, 8]
```

After sorting:

```text
[1, 2]
[2, 0]
[3, 1]
[6, 4]
[7, 3]
[8, 5]
```

The first value is the query and the second value is its original index.

### Step 3: Create a Min Heap

We store:

```cpp
{intervalLength, rightEndpoint}
```

C++ declaration:

```cpp
priority_queue<
    pair<int, int>,
    vector<pair<int, int>>,
    greater<pair<int, int>>
> minHeap;
```

The heap prioritizes:

1. Smaller interval length.
2. Smaller right endpoint when lengths are equal.

### Step 4: Process Sorted Queries

For every query:

```cpp
int q = sortedQueries[i].first;
```

We perform two important operations.

**Operation A: Add eligible intervals**

Add all intervals whose starting point is less than or equal to the current query.

```cpp
while (j < intervals.size() &&
       intervals[j][0] <= q) {

    int left = intervals[j][0];
    int right = intervals[j][1];

    int length = right - left + 1;

    minHeap.push({length, right});

    j++;
}
```

Why?

Because these intervals have already started and might contain the current query.

**Operation B: Remove expired intervals**

Remove intervals that end before the current query.

```cpp
while (!minHeap.empty() &&
       minHeap.top().second < q) {

    minHeap.pop();
}
```

Why?

Because an interval ending before the query cannot contain it.

Important:

```cpp
right < q
```

We use `<`, not `<=`, because intervals are inclusive.

An interval ending exactly at the query is still valid.

### Step 5: Store the Answer

After adding eligible intervals and removing expired intervals:

- If the heap is empty, no interval contains the query.
- Otherwise, the shortest valid interval is at the top.

```cpp
if (minHeap.empty()) {

    ans[sortedQueries[i].second] = -1;

}
else {

    ans[sortedQueries[i].second] = minHeap.top().first;
}
```

Notice that we use the original query index to store the answer in the correct position.

## 5. Optimized C++ Solution

```cpp
class Solution {
public:
    vector<int> minInterval(
        vector<vector<int>>& intervals,
        vector<int>& queries
    ) {

        // Step 1: Sort intervals by starting point
        sort(intervals.begin(), intervals.end());

        // Step 2: Store queries with their original indices
        vector<pair<int, int>> sortedQueries;

        for (int i = 0; i < queries.size(); i++) {

            sortedQueries.push_back({queries[i], i});
        }

        // Sort queries by value
        sort(sortedQueries.begin(), sortedQueries.end());

        // Step 3: Min Heap stores {length, right}
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > minHeap;

        vector<int> ans(queries.size(), -1);

        int j = 0;
        int n = intervals.size();

        // Step 4: Process queries in sorted order
        for (auto& query : sortedQueries) {

            int q = query.first;
            int originalIndex = query.second;

            // Add all intervals that have started
            while (j < n && intervals[j][0] <= q) {

                int left = intervals[j][0];
                int right = intervals[j][1];

                int length = right - left + 1;

                minHeap.push({length, right});

                j++;
            }

            // Remove intervals that have expired
            while (!minHeap.empty() &&
                   minHeap.top().second < q) {

                minHeap.pop();
            }

            // Get the shortest valid interval
            if (!minHeap.empty()) {

                ans[originalIndex] = minHeap.top().first;
            }
        }

        return ans;
    }
};
```

## 6. Detailed Dry Run

**Input:**

```cpp
intervals = {
    {1, 3},
    {2, 3},
    {3, 7},
    {6, 6}
};

queries = {2, 3, 1, 7, 6, 8};
```

### Step 1: Sort Intervals

```text
[1, 3]
[2, 3]
[3, 7]
[6, 6]
```

### Step 2: Sort Queries With Original Indices

```text
Original:

[2, 3, 1, 7, 6, 8]

Sorted:

[1, 2]
[2, 0]
[3, 1]
[6, 4]
[7, 3]
[8, 5]
```

Initialize:

```cpp
j = 0;
ans = {-1, -1, -1, -1, -1, -1};
```

### Query 1: q = 1

Add intervals where:

```cpp
left <= 1
```

Add:

```text
[1, 3]
```

Its length:

```text
3 - 1 + 1 = 3
```

Heap:

```text
[(3, 3)]
```

No expired interval.

Answer for original index `2`:

```text
ans[2] = 3
```

### Query 2: q = 2

Add intervals where:

```cpp
left <= 2
```

Add:

```text
[2, 3]
```

Its length:

```text
3 - 2 + 1 = 2
```

Heap:

```text
[(2, 3), (3, 3)]
```

No expired interval.

The shortest interval has length `2`.

Answer for original index `0`:

```text
ans[0] = 2
```

### Query 3: q = 3

Add intervals where:

```cpp
left <= 3
```

Add:

```text
[3, 7]
```

Its length:

```text
7 - 3 + 1 = 5
```

Heap:

```text
[(2, 3), (3, 3), (5, 7)]
```

Check expired intervals:

```cpp
right < 3
```

None have expired.

The shortest valid interval is:

```text
[2, 3]
```

Length:

```text
2
```

Answer for original index `1`:

```text
ans[1] = 2
```

### Query 4: q = 6

Add intervals where:

```cpp
left <= 6
```

Add:

```text
[6, 6]
```

Its length:

```text
6 - 6 + 1 = 1
```

Heap before removing expired intervals:

```text
[(1, 6), (2, 3), (3, 3), (5, 7)]
```

Now remove expired intervals.

The shortest interval is:

```text
(1, 6)
```

It is valid because:

```cpp
6 < 6
```

is false.

The next shortest interval is `(2, 3)`.

It is expired because:

```cpp
3 < 6
```

true.

Remove it.

Continue removing:

```text
(3, 3) -> expired
```

Remaining heap:

```text
[(1, 6), (5, 7)]
```

The shortest valid interval has length `1`.

Answer for original index `4`:

```text
ans[4] = 1
```

### Query 5: q = 7

No additional interval needs to be added.

Check the heap:

```text
[(1, 6), (5, 7)]
```

Remove expired intervals:

```text
(1, 6)
```

Because:

```cpp
6 < 7
```

true.

Remaining heap:

```text
[(5, 7)]
```

The shortest valid interval is:

```text
[3, 7]
```

Length:

```text
7 - 3 + 1 = 5
```

Answer for original index `3`:

```text
ans[3] = 5
```

### Query 6: q = 8

No more intervals are available.

Remove expired intervals:

```text
(5, 7)
```

Because:

```cpp
7 < 8
```

true.

Heap becomes empty.

No interval contains query `8`.

Answer for original index `5`:

```text
ans[5] = -1
```

### Final Answer

```cpp
ans = {2, 2, 3, 5, 1, -1};
```

Output:

```text
[2, 2, 3, 5, 1, -1]
```

## 7. Time and Space Complexity

Let:

- `n` = number of intervals.
- `m` = number of queries.

### Sorting Intervals

```cpp
sort(intervals.begin(), intervals.end());
```

Time:

```text
O(n log n)
```

### Sorting Queries

```cpp
sort(sortedQueries.begin(), sortedQueries.end());
```

Time:

```text
O(m log m)
```

### Heap Operations

Every interval is inserted into the heap at most once.

Every interval is removed from the heap at most once.

Each heap operation costs:

```text
O(log n)
```

Total heap processing:

```text
O(n log n)
```

Processing all queries:

```text
O(m)
```

### Overall Time Complexity

```text
O(n log n + m log m)
```

### Space Complexity

- Sorted query array: `O(m)`
- Min Heap: `O(n)`
- Answer array: `O(m)`

Overall:

```text
O(n + m)
```

## 8. Important Interview Insights

### Q1. Why do we sort queries?

Sorting allows us to process queries from smallest to largest.

We can incrementally add intervals instead of checking every interval again for every query.

### Q2. Why do we store the original query index?

Because sorting changes the order of queries.

We must return answers in the same order as the original query array.

### Q3. Why use a Min Heap?

We need the shortest interval among all valid intervals.

The Min Heap gives us the smallest interval length in `O(1)` time at the top.

### Q4. Why store `{length, right}`?

We need:

- `length` to identify the shortest interval.
- `right` to determine whether the interval has expired.

We do not need to store the left endpoint because intervals are inserted only after their starting point becomes eligible.

### Q5. Why remove intervals using `right < q`?

Intervals are inclusive.

For example:

```text
Interval = [2, 5]
Query = 5
```

The interval still contains the query.

Therefore, an interval expires only when:

```cpp
right < q
```

### Q6. Why is the heap top the answer after removing expired intervals?

All intervals are inserted only when:

```cpp
left <= q
```

All expired intervals are removed when:

```cpp
right < q
```

Therefore, every remaining interval contains the current query.

Since the heap prioritizes the smallest length, its top gives the answer.

### Q7. Why don't we remove every expired interval?

We only need to remove expired intervals from the heap's top.

An expired interval that is not at the top cannot affect the current minimum.

It will be removed later if it reaches the top.

### Q8. Can we use a normal queue?

No.

A normal queue does not maintain elements according to interval length.

We need the shortest interval at the top, so a Min Heap is appropriate.

## 9. Common Mistakes

- Forgetting to sort intervals by starting point.
- Forgetting to sort queries.
- Not preserving the original query indices.
- Using `right <= q` instead of `right < q`.
- Forgetting `+1` when calculating inclusive interval length.
- Using a Max Heap instead of a Min Heap.
- Storing only interval length without the right endpoint.
- Removing intervals before adding eligible intervals.
- Checking every interval for every query instead of using the optimized approach.
- Returning answers in sorted-query order instead of original-query order.

## 10. Pattern Recognition — When Should You Use This?

Look for this approach when a problem asks you to:

- Answer multiple queries over intervals.
- Find the shortest or minimum-cost valid interval.
- Process queries independently but efficiently.
- Reuse information from previously processed queries.
- Maintain the minimum or maximum among dynamically eligible candidates.

### Pattern

```text
Sort intervals
      |
Sort queries + preserve original indices
      |
Process queries in increasing order
      |
Add eligible intervals
      |
Remove expired intervals
      |
Min Heap gives the shortest valid interval
      |
Store answer using original index
```

## 11. Key Takeaways

### Most Important Observations

1. Sort intervals by their left endpoint.
2. Sort queries while preserving original indices.
3. Add intervals when `left <= query`.
4. Remove expired intervals when `right < query`.
5. Use a Min Heap ordered by interval length.
6. Store answers using the original query index.

### Most Important Code

Min Heap:

```cpp
priority_queue<
    pair<int, int>,
    vector<pair<int, int>>,
    greater<pair<int, int>>
> minHeap;
```

Add eligible intervals:

```cpp
while (j < n && intervals[j][0] <= q) {

    int length = intervals[j][1] - intervals[j][0] + 1;

    minHeap.push({length, intervals[j][1]});

    j++;
}
```

Remove expired intervals:

```cpp
while (!minHeap.empty() && minHeap.top().second < q) {

    minHeap.pop();
}
```

Store answer:

```cpp
ans[originalIndex] =
    minHeap.empty() ? -1 : minHeap.top().first;
```

### Final Complexity

- **Time:** `O(n log n + m log m)`
- **Space:** `O(n + m)`

**Final Interview Insight:**

This problem is not just about finding the shortest interval. It is about processing offline queries efficiently by sorting them and maintaining eligible intervals using a Min Heap.
- Time: `O(n)`
- Auxiliary Space: `O(1)`
- Output Space: `O(n)`
