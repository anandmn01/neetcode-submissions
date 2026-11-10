# DSA MASTER REVISION NOTEBOOK — DAYS 16–30

**Language:** C++  |  **Purpose:** GitHub-ready learning and revision notes  |  **Coverage:** Arrays through Graphs

> This notebook consolidates the Day 16–30 learning roadmap. Read one day at a time, trace each solution, and re-implement it independently before timed practice.

# PART I — DAYS 16–20: CORE DSA PATTERNS

# Day 16 — DSA Foundations: Arrays, Strings and Problem-Solving

## Topic 1 — Big-O Time and Space Complexity

Complexity describes how resource usage grows as input size `n` grows. It is not a stopwatch measurement; it describes the growth rate.

| Complexity | Name | Typical example |
|---|---|---|
| O(1) | Constant | Array index access |
| O(log n) | Logarithmic | Binary search |
| O(n) | Linear | One array traversal |
| O(n log n) | Linearithmic | Efficient comparison sorting |
| O(n²) | Quadratic | Two nested traversals |
| O(2ⁿ) | Exponential | Generate all subsets |
| O(n!) | Factorial | Generate all permutations |

### Time complexity examples

```cpp
// O(1)
int first = nums[0];

// O(n)
for (int x : nums) cout << x;

// O(n^2)
for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
        work(i, j);

// O(log n)
while (n > 1) n /= 2;
```

### Space complexity
- **Input space** is the memory occupied by the supplied input.
- **Auxiliary space** is extra memory used by the algorithm.
- A recursive algorithm uses call-stack space.
- An output array is often reported separately from auxiliary space.

Example: iterative array reversal uses O(1) auxiliary space; copying the array uses O(n).

### Amortized complexity
A dynamic array append is O(1) amortized: most appends are constant time, while occasional resizing copies elements. Across many appends, the average cost per append remains constant.

## Topic 2 — Array Fundamentals in C++

An array stores same-type elements in contiguous memory. A `vector` is a resizable dynamic array.

```cpp
int a[3] = {10, 20, 30};
vector<int> v = {10, 20, 30};

v.push_back(40);  // amortized O(1)
v.pop_back();     // O(1)
int x = v[1];     // O(1)
sort(v.begin(), v.end()); // O(n log n)
```

| Operation | Typical complexity |
|---|---|
| Index access | O(1) |
| Update by index | O(1) |
| Linear search | O(n) |
| Insert/delete at end of vector | O(1) amortized |
| Insert/delete near beginning or middle | O(n) |
| Sort | O(n log n) |

Use `vector<int>` for most LeetCode array problems. Validate indices and be aware that `vector::size()` returns an unsigned type (`size_t`).

## Topic 3 — Hashing, Frequency Counting and Prefix Sums

### Hash set: Contains Duplicate (LC 217)

Return true if any value appears more than once.

```cpp
bool containsDuplicate(vector<int>& nums) {
    unordered_set<int> seen;
    for (int x : nums) {
        if (seen.count(x)) return true;
        seen.insert(x);
    }
    return false;
}
```

Average time O(n), extra space O(n). Hash operations are average O(1), not guaranteed O(1) in every worst-case scenario.

### Hash map: Two Sum (LC 1)

Return indices of two values whose sum is target.

```cpp
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> index;
    for (int i = 0; i < nums.size(); i++) {
        int need = target - nums[i];
        if (index.count(need)) return {index[need], i};
        index[nums[i]] = i;
    }
    return {};
}
```

Average time O(n), space O(n). Check before inserting the current value so one element is not used twice.

### Prefix sum

Define `prefix[i]` as the sum of elements before index `i`. Then the sum of `[l,r]` is `prefix[r+1] - prefix[l]`.

```cpp
vector<int> prefix(nums.size() + 1, 0);
for (int i = 0; i < nums.size(); i++)
    prefix[i + 1] = prefix[i] + nums[i];

int rangeSum = prefix[r + 1] - prefix[l];
```

Build O(n), each range query O(1), extra space O(n).

### Product of Array Except Self (LC 238) — prefix/suffix

For each index, multiply all values to its left by all values to its right. No division is required.

```cpp
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> ans(n, 1);

    int prefix = 1;
    for (int i = 0; i < n; i++) {
        ans[i] = prefix;
        prefix *= nums[i];
    }

    int suffix = 1;
    for (int i = n - 1; i >= 0; i--) {
        ans[i] *= suffix;
        suffix *= nums[i];
    }
    return ans;
}
```

Time O(n), auxiliary space O(1) excluding output. Product overflow must be considered according to constraints.

## Topic 4 — Two Pointers

Two pointers reduce unnecessary nested searching by moving indices according to a condition.

### Reverse an array / string

```cpp
void reverseArray(vector<int>& a) {
    int l = 0, r = (int)a.size() - 1;
    while (l < r) {
        swap(a[l], a[r]);
        l++;
        r--;
    }
}
```

Time O(n), extra space O(1).

### Valid Palindrome (LC 125)

Ignore non-alphanumeric characters and compare case-insensitively.

```cpp
bool isPalindrome(string s) {
    int l = 0, r = (int)s.size() - 1;

    while (l < r) {
        while (l < r && !isalnum((unsigned char)s[l])) l++;
        while (l < r && !isalnum((unsigned char)s[r])) r--;

        if (tolower((unsigned char)s[l]) !=
            tolower((unsigned char)s[r]))
            return false;

        l++;
        r--;
    }
    return true;
}
```

Time O(n), space O(1). Casting to `unsigned char` before character classification avoids undefined behavior for negative signed-char values.

### Two Sum II (LC 167)

Input is sorted. If sum is too small, move left pointer right; if too large, move right pointer left.

```cpp
vector<int> twoSum(vector<int>& a, int target) {
    int l = 0, r = (int)a.size() - 1;
    while (l < r) {
        int sum = a[l] + a[r];
        if (sum == target) return {l + 1, r + 1};
        if (sum < target) l++;
        else r--;
    }
    return {};
}
```

Time O(n), space O(1). LeetCode expects 1-based indices.

### Remove Duplicates from Sorted Array (LC 26)

Use a write pointer for the next unique value.

```cpp
int removeDuplicates(vector<int>& nums) {
    if (nums.empty()) return 0;
    int write = 1;

    for (int read = 1; read < nums.size(); read++) {
        if (nums[read] != nums[read - 1])
            nums[write++] = nums[read];
    }
    return write;
}
```

Time O(n), extra space O(1). The first `write` elements contain the unique values.

### Container With Most Water (LC 11)

Area is `min(height[l], height[r]) * (r-l)`. Move the pointer at the shorter line because keeping it cannot improve the limiting height while width decreases.

```cpp
int maxArea(vector<int>& h) {
    int l = 0, r = (int)h.size() - 1;
    int best = 0;

    while (l < r) {
        best = max(best, min(h[l], h[r]) * (r - l));
        if (h[l] < h[r]) l++;
        else r--;
    }
    return best;
}
```

Time O(n), space O(1).

## Topic 5 — Sliding Window

Sliding window is useful for contiguous subarrays/substrings. Maintain a window `[left,right]` and update it incrementally.

### Fixed-size window — maximum sum of size k

```cpp
int maxSumSizeK(vector<int>& a, int k) {
    int window = 0;
    for (int i = 0; i < k; i++) window += a[i];

    int best = window;
    for (int right = k; right < a.size(); right++) {
        window += a[right] - a[right - k];
        best = max(best, window);
    }
    return best;
}
```

Time O(n), space O(1). Assumes `1 <= k <= a.size()`.

### Variable-size window — Minimum Size Subarray Sum (LC 209)

For positive values, expand right until sum >= target, then shrink left while the condition remains true.

```cpp
int minSubArrayLen(int target, vector<int>& nums) {
    int left = 0, sum = 0;
    int best = INT_MAX;

    for (int right = 0; right < nums.size(); right++) {
        sum += nums[right];

        while (sum >= target) {
            best = min(best, right - left + 1);
            sum -= nums[left++];
        }
    }
    return best == INT_MAX ? 0 : best;
}
```

Time O(n), space O(1). This standard sliding-window method relies on positive numbers; negative values break the monotonic property.

### Longest Substring Without Repeating Characters (LC 3)

Store the last index of each character. Move left beyond a repeated character if it lies inside the current window.

```cpp
int lengthOfLongestSubstring(string s) {
    vector<int> last(256, -1);
    int left = 0, best = 0;

    for (int right = 0; right < s.size(); right++) {
        unsigned char c = s[right];
        left = max(left, last[c] + 1);
        last[c] = right;
        best = max(best, right - left + 1);
    }
    return best;
}
```

Time O(n), space O(1) for the fixed 256-entry table (assuming byte characters).

## Topic 6 — Interview Problem-Solving Strategy

1. Clarify input, output, constraints and whether input is sorted.
2. Identify edge cases: empty input, one item, duplicates, negative values, overflow.
3. State a brute-force baseline.
4. Identify repeated work or bottleneck.
5. Recognize a pattern: hashing, prefix sum, two pointers or sliding window.
6. Explain the invariant and why pointer movement is safe.
7. Dry-run a normal case and an edge case.
8. State time and auxiliary-space complexity before coding.

### Day 16 quick reference
- Hash set/map: membership, index lookup and frequency.
- Prefix sum: fast range sums and subarray reasoning.
- Two pointers: sorted pair search, in-place compaction, palindrome.
- Sliding window: contiguous ranges.
- Product Except Self: prefix and suffix products, no division.

---

# Day 17 — Advanced Arrays and Strings

## Topic 1 — Kadane's Algorithm: Maximum Subarray (LC 53)

Find the contiguous subarray with maximum sum.

At each index, either start a new subarray at the current element or extend the previous subarray.

```cpp
int maxSubArray(vector<int>& nums) {
    int current = nums[0];
    int best = nums[0];

    for (int i = 1; i < nums.size(); i++) {
        current = max(nums[i], current + nums[i]);
        best = max(best, current);
    }
    return best;
}
```

Time O(n), space O(1). Initialize from the first element, not zero, so all-negative arrays work.

## Topic 2 — 3Sum and 4Sum

### 3Sum (LC 15)

Sort. Fix one value, then use two pointers to find a pair that makes the total zero. Skip duplicate fixed values and duplicate pointer values.

```cpp
vector<vector<int>> threeSum(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> ans;
    int n = nums.size();

    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;

        int l = i + 1, r = n - 1;
        while (l < r) {
            long long sum = (long long)nums[i] + nums[l] + nums[r];

            if (sum < 0) l++;
            else if (sum > 0) r--;
            else {
                ans.push_back({nums[i], nums[l], nums[r]});
                int lv = nums[l], rv = nums[r];
                while (l < r && nums[l] == lv) l++;
                while (l < r && nums[r] == rv) r--;
            }
        }
    }
    return ans;
}
```

Time O(n²), output can be O(n²). Sorting modifies the input. Use a wider sum type to avoid overflow.

### 4Sum (LC 18)

Fix two values, then use two pointers for the remaining pair. Skip duplicates at both fixed positions and after a match. Use `long long` for the sum.

```cpp
vector<vector<int>> fourSum(vector<int>& a, int target) {
    sort(a.begin(), a.end());
    vector<vector<int>> ans;
    int n = a.size();

    for (int i = 0; i < n - 3; i++) {
        if (i && a[i] == a[i-1]) continue;

        for (int j = i + 1; j < n - 2; j++) {
            if (j > i + 1 && a[j] == a[j-1]) continue;

            int l = j + 1, r = n - 1;
            while (l < r) {
                long long sum = (long long)a[i] + a[j] + a[l] + a[r];

                if (sum < target) l++;
                else if (sum > target) r--;
                else {
                    ans.push_back({a[i], a[j], a[l], a[r]});
                    int lv = a[l], rv = a[r];
                    while (l < r && a[l] == lv) l++;
                    while (l < r && a[r] == rv) r--;
                }
            }
        }
    }
    return ans;
}
```

Time O(n³), output may be O(n³).

## Topic 3 — Product of Array Except Self (LC 238)

See Day 16 Topic 3. Core method: output stores prefix product; reverse traversal multiplies by suffix product. Time O(n), O(1) auxiliary space excluding output. Handles zeros without division.

## Topic 4 — Maximum Product Subarray (LC 152)

A negative value can turn the smallest (most negative) product into the largest product. Track both current maximum and minimum products.

```cpp
int maxProduct(vector<int>& nums) {
    int curMax = nums[0], curMin = nums[0];
    int best = nums[0];

    for (int i = 1; i < nums.size(); i++) {
        int x = nums[i];

        if (x < 0) swap(curMax, curMin);

        curMax = max(x, curMax * x);
        curMin = min(x, curMin * x);
        best = max(best, curMax);
    }
    return best;
}
```

Time O(n), space O(1). The swap on a negative number is essential. Zero naturally resets the running product choices.

## Topic 5 — Longest Consecutive Sequence (LC 128)

Use a hash set. Start counting only when `x-1` is absent, so each consecutive run is traversed from its beginning.

```cpp
int longestConsecutive(vector<int>& nums) {
    unordered_set<int> s(nums.begin(), nums.end());
    int best = 0;

    for (int x : s) {
        if (s.count(x - 1)) continue;

        int len = 1;
        while (s.count(x + len)) len++;
        best = max(best, len);
    }
    return best;
}
```

Average time O(n), space O(n). The algorithm is linear on average because each sequence is scanned from its start.

## Topic 6 — Matrix Problems

### Set Matrix Zeroes (LC 73)

Use first row and first column as marker storage, with two booleans recording whether either originally contained zero.

```cpp
void setZeroes(vector<vector<int>>& a) {
    int m = a.size(), n = a[0].size();
    bool firstRow = false, firstCol = false;

    for (int j = 0; j < n; j++)
        if (a[0][j] == 0) firstRow = true;
    for (int i = 0; i < m; i++)
        if (a[i][0] == 0) firstCol = true;

    for (int i = 1; i < m; i++)
        for (int j = 1; j < n; j++)
            if (a[i][j] == 0)
                a[i][0] = a[0][j] = 0;

    for (int i = 1; i < m; i++)
        for (int j = 1; j < n; j++)
            if (a[i][0] == 0 || a[0][j] == 0)
                a[i][j] = 0;

    if (firstRow)
        for (int j = 0; j < n; j++) a[0][j] = 0;
    if (firstCol)
        for (int i = 0; i < m; i++) a[i][0] = 0;
}
```

Time O(mn), auxiliary space O(1).

### Spiral Matrix (LC 54)

Maintain `top`, `bottom`, `left`, `right` boundaries. Traverse top row, right column, bottom row if still valid, then left column if still valid. Shrink boundaries after each traversal.

```cpp
vector<int> spiralOrder(vector<vector<int>>& a) {
    vector<int> ans;
    int top = 0, bottom = a.size() - 1;
    int left = 0, right = a[0].size() - 1;

    while (top <= bottom && left <= right) {
        for (int j = left; j <= right; j++) ans.push_back(a[top][j]);
        top++;

        for (int i = top; i <= bottom; i++) ans.push_back(a[i][right]);
        right--;

        if (top <= bottom) {
            for (int j = right; j >= left; j--) ans.push_back(a[bottom][j]);
            bottom--;
        }

        if (left <= right) {
            for (int i = bottom; i >= top; i--) ans.push_back(a[i][left]);
            left++;
        }
    }
    return ans;
}
```

Time O(mn), output O(mn), auxiliary O(1).

### Rotate Image (LC 48)

Rotate an `n × n` matrix clockwise by 90 degrees: transpose, then reverse every row.

```cpp
void rotate(vector<vector<int>>& a) {
    int n = a.size();

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            swap(a[i][j], a[j][i]);

    for (auto& row : a)
        reverse(row.begin(), row.end());
}
```

Time O(n²), auxiliary space O(1).

### Day 17 quick reference
- Kadane: maximum sum of a contiguous subarray.
- 3Sum/4Sum: sorting + fixed pointers + duplicate skipping.
- Max product: track max and min.
- Longest consecutive: hash set, start only at sequence beginnings.
- Matrix: markers, boundaries, transpose + reverse.

---

# Day 18 — Binary Search and Searching Patterns

## Topic 1 — Binary Search Fundamentals

Binary search applies to sorted data or a search space with a monotonic condition.

```cpp
int binarySearch(vector<int>& a, int target) {
    int l = 0, r = (int)a.size() - 1;

    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (a[mid] == target) return mid;
        if (a[mid] < target) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}
```

Time O(log n), space O(1). `l + (r-l)/2` avoids overflow from `(l+r)/2`.

C++ helpers:
- `lower_bound`: first position with value `>= target`.
- `upper_bound`: first position with value `> target`.
- `binary_search`: whether target exists.

## Topic 2 — Search Insert Position (LC 35)

Find first index whose value is at least target. Return `left` after the loop.

```cpp
int searchInsert(vector<int>& a, int target) {
    int l = 0, r = a.size();

    while (l < r) {
        int mid = l + (r-l)/2;
        if (a[mid] < target) l = mid + 1;
        else r = mid;
    }
    return l;
}
```

Time O(log n), space O(1).

## Topic 3 — Search in Rotated Sorted Array (LC 33)

At least one half around mid is sorted. Identify it and check whether target lies in its range.

```cpp
int search(vector<int>& a, int target) {
    int l = 0, r = a.size() - 1;

    while (l <= r) {
        int mid = l + (r-l)/2;
        if (a[mid] == target) return mid;

        if (a[l] <= a[mid]) {
            if (a[l] <= target && target < a[mid]) r = mid - 1;
            else l = mid + 1;
        } else {
            if (a[mid] < target && target <= a[r]) l = mid + 1;
            else r = mid - 1;
        }
    }
    return -1;
}
```

Time O(log n), space O(1). This version assumes distinct values; duplicates can make sorted-half identification ambiguous.

## Topic 4 — Find Minimum in Rotated Sorted Array (LC 153)

Compare middle with right:
- If `a[mid] > a[right]`, minimum is right of mid.
- Otherwise minimum is at mid or left of it.

```cpp
int findMin(vector<int>& a) {
    int l = 0, r = a.size() - 1;

    while (l < r) {
        int mid = l + (r-l)/2;
        if (a[mid] > a[r]) l = mid + 1;
        else r = mid;
    }
    return a[l];
}
```

Time O(log n), space O(1), distinct values assumption.

## Topic 5 — Find Peak Element (LC 162)

A peak is greater than adjacent values. Compare `a[mid]` and `a[mid+1]`:
- If rising, a peak exists to the right.
- Otherwise, a peak exists at mid or to the left.

```cpp
int findPeakElement(vector<int>& a) {
    int l = 0, r = a.size() - 1;

    while (l < r) {
        int mid = l + (r-l)/2;
        if (a[mid] < a[mid+1]) l = mid + 1;
        else r = mid;
    }
    return l;
}
```

Time O(log n), space O(1).

## Topic 6 — Binary Search on Answer

Use when the answer lies in a numeric range and a feasibility test is monotonic. Find the smallest/largest answer satisfying a condition.

### Koko Eating Bananas (LC 875)

For speed `k`, total hours are sum of `ceil(pile/k)`. Feasibility is monotonic: if speed `k` works, any larger speed works.

```cpp
int minEatingSpeed(vector<int>& piles, int h) {
    int l = 1, r = *max_element(piles.begin(), piles.end());

    auto canFinish = [&](int speed) {
        long long hours = 0;
        for (int p : piles)
            hours += (p + speed - 1) / speed;
        return hours <= h;
    };

    while (l < r) {
        int mid = l + (r-l)/2;
        if (canFinish(mid)) r = mid;
        else l = mid + 1;
    }
    return l;
}
```

Time O(n log M), where M is maximum pile size; extra space O(1).

### Capacity to Ship Packages Within D Days (LC 1011)

Binary-search capacity between maximum package weight and total weight. Feasibility simulates loading in order and counts required days.

```cpp
int shipWithinDays(vector<int>& w, int days) {
    int l = *max_element(w.begin(), w.end());
    int r = accumulate(w.begin(), w.end(), 0);

    auto feasible = [&](int cap) {
        int usedDays = 1, load = 0;
        for (int x : w) {
            if (load + x > cap) {
                usedDays++;
                load = 0;
            }
            load += x;
        }
        return usedDays <= days;
    };

    while (l < r) {
        int mid = l + (r-l)/2;
        if (feasible(mid)) r = mid;
        else l = mid + 1;
    }
    return l;
}
```

Time O(n log(sum weights)), extra space O(1).

### Day 18 quick reference
- Standard binary search: sorted values.
- Lower-bound template: first true / first value >= target.
- Rotated array: determine sorted half.
- Binary search on answer: define bounds + monotonic feasibility function.

---

# Day 19 — Linked Lists

## Topic 1 — Linked List Fundamentals

A singly linked-list node stores a value and a pointer to the next node.

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};
```

Unlike arrays, nodes are not necessarily contiguous. Access by index is O(n); insertion/deletion after a known node is O(1).

## Topic 2 — Insertion and Deletion

### Insert at head
```cpp
ListNode* insertHead(ListNode* head, int value) {
    ListNode* node = new ListNode(value);
    node->next = head;
    return node;
}
```

### Delete first matching value
```cpp
ListNode* deleteValue(ListNode* head, int value) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prev = &dummy;

    while (prev->next && prev->next->val != value)
        prev = prev->next;

    if (prev->next) {
        ListNode* victim = prev->next;
        prev->next = victim->next;
        delete victim;
    }
    return dummy.next;
}
```

A dummy/sentinel node simplifies head deletion. In LeetCode-managed nodes, follow the platform's memory conventions.

## Topic 3 — Reverse Linked List (LC 206)

Iterative method: keep `prev`, `curr`, and `next`.

```cpp
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;

    while (curr) {
        ListNode* nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextNode;
    }
    return prev;
}
```

Time O(n), space O(1).

Recursive idea: reverse the suffix, then point `head->next->next` back to head and set `head->next = nullptr`. Time O(n), recursion space O(n).

## Topic 4 — Middle of Linked List (LC 876)

Fast pointer moves two steps, slow pointer one. When fast reaches the end, slow is at the middle.

```cpp
ListNode* middleNode(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
```

Time O(n), space O(1). For even length, this returns the second middle node.

## Topic 5 — Cycle Detection and Cycle Entry (LC 141, LC 142)

Floyd's algorithm: slow moves one step, fast moves two. If they meet, a cycle exists.

```cpp
bool hasCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}
```

Time O(n), space O(1).

To find cycle entry: after slow and fast meet, reset one pointer to head. Move both one step at a time; their next meeting is the cycle entry.

```cpp
ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            ListNode* p = head;
            while (p != slow) {
                p = p->next;
                slow = slow->next;
            }
            return p;
        }
    }
    return nullptr;
}
```

Time O(n), space O(1).

## Topic 6 — Merge Two Sorted Lists (LC 21)

Use a dummy node and attach the smaller current node.

```cpp
ListNode* mergeTwoLists(ListNode* a, ListNode* b) {
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (a && b) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }

    tail->next = a ? a : b;
    return dummy.next;
}
```

Time O(m+n), auxiliary space O(1) when reusing nodes.

Merge K sorted lists was also revisited on Day 24 using a min heap: keep one current node per list, extract the smallest, then insert its successor. Time O(N log K), heap O(K).

### Day 19 quick reference
- Dummy node simplifies head changes.
- Reverse: three pointers.
- Middle: slow/fast.
- Cycle: Floyd's algorithm.
- Merge: compare current heads.

---

# Day 20 — Stacks and Queues

## Topic 1 — Stack Fundamentals

Stack is LIFO: last in, first out.

```cpp
stack<int> st;
st.push(10);
st.push(20);
int top = st.top();
st.pop();
bool empty = st.empty();
```

Push, pop and top are O(1). `pop()` returns no value; read `top()` first.

## Topic 2 — Valid Parentheses (LC 20)

Push opening brackets. For a closing bracket, stack must be non-empty and its top must match.

```cpp
bool isValid(string s) {
    stack<char> st;

    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else {
            if (st.empty()) return false;

            char t = st.top();
            st.pop();

            if ((c == ')' && t != '(') ||
                (c == ']' && t != '[') ||
                (c == '}' && t != '{'))
                return false;
        }
    }
    return st.empty();
}
```

Time O(n), space O(n).

## Topic 3 — Min Stack (LC 155)

Maintain a second stack storing the minimum at each depth, or store pairs `{value, currentMin}`.

```cpp
class MinStack {
    stack<pair<int,int>> st;

public:
    void push(int val) {
        int mn = st.empty() ? val : min(val, st.top().second);
        st.push({val, mn});
    }

    void pop() { st.pop(); }
    int top() { return st.top().first; }
    int getMin() { return st.top().second; }
};
```

All operations O(1), space O(n).

## Topic 4 — Monotonic Stack: Daily Temperatures / Next Greater Element

A monotonic stack maintains indices or values in a monotonic order. For next-greater problems, pop while the current value is greater than the stack's value.

```cpp
vector<int> dailyTemperatures(vector<int>& t) {
    int n = t.size();
    vector<int> ans(n, 0);
    stack<int> st; // indices

    for (int i = 0; i < n; i++) {
        while (!st.empty() && t[i] > t[st.top()]) {
            int j = st.top();
            st.pop();
            ans[j] = i - j;
        }
        st.push(i);
    }
    return ans;
}
```

Time O(n), space O(n), because each index is pushed and popped at most once.

## Topic 5 — Queue, Deque, Circular Queue and Monotonic Queue

### Queue
FIFO: first in, first out. `push`, `pop`, `front` are O(1).

### Deque
Supports insertion/removal at both ends. Useful for sliding-window algorithms.

### Sliding Window Maximum (LC 239)

Maintain decreasing deque of indices. Remove expired indices from front; remove smaller values from back. Front gives maximum.

```cpp
vector<int> maxSlidingWindow(vector<int>& a, int k) {
    deque<int> dq;
    vector<int> ans;

    for (int i = 0; i < a.size(); i++) {
        while (!dq.empty() && dq.front() <= i - k)
            dq.pop_front();

        while (!dq.empty() && a[dq.back()] <= a[i])
            dq.pop_back();

        dq.push_back(i);
        if (i >= k - 1) ans.push_back(a[dq.front()]);
    }
    return ans;
}
```

Time O(n), space O(k).

### Circular queue
A fixed-size queue can reuse freed slots by wrapping indices using modulo capacity. Empty/full states must be distinguished, often with a size counter or one unused slot.

## Topic 6 — Evaluate Reverse Polish Notation (LC 150)

Operands are pushed. For an operator, pop right operand first, then left operand; calculate `left op right`.

```cpp
int evalRPN(vector<string>& tokens) {
    stack<int> st;

    for (const string& t : tokens) {
        if (t == "+" || t == "-" || t == "*" || t == "/") {
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();

            if (t == "+") st.push(a + b);
            else if (t == "-") st.push(a - b);
            else if (t == "*") st.push(a * b);
            else st.push(a / b); // C++ integer division truncates toward zero
        } else {
            st.push(stoi(t));
        }
    }
    return st.top();
}
```

Time O(n), space O(n). Operand order is a common source of mistakes for subtraction and division.

### Day 20 quick reference
- Stack: LIFO; queue: FIFO.
- Parentheses matching uses stack.
- Min Stack stores current minimum.
- Monotonic stack resolves next greater/smaller.
- Monotonic deque handles sliding-window min/max.
- RPN: pop right operand before left operand.

---

# PART II — DAYS 21–25: TREES, HEAPS, TRIES AND BACKTRACKING

# Day 21 — Trees and Binary Trees

## Topic 1 — Tree Fundamentals and Node Structure

A tree is a connected acyclic structure. A binary tree node has at most two children.

```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

Root is the top node; leaves have no children. Depth counts edges from root; height is the longest downward path to a leaf (state the convention used).

## Topic 2 — DFS Traversals: Inorder, Preorder and Postorder

```cpp
void preorder(TreeNode* root) {
    if (!root) return;
    cout << root->val << " ";
    preorder(root->left);
    preorder(root->right);
}

void inorder(TreeNode* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->val << " ";
    inorder(root->right);
}

void postorder(TreeNode* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    cout << root->val << " ";
}
```

- Preorder: root, left, right.
- Inorder: left, root, right.
- Postorder: left, right, root.

Each traversal is O(n) time and O(h) recursion space, where h is tree height.

## Topic 3 — Level Order Traversal / BFS (LC 102)

Use a queue. Capture queue size at the start of each level so only that level is processed in the current iteration.

```cpp
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> ans;
    if (!root) return ans;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        vector<int> level;

        for (int i = 0; i < size; i++) {
            TreeNode* node = q.front();
            q.pop();
            level.push_back(node->val);

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        ans.push_back(level);
    }
    return ans;
}
```

Time O(n), queue space O(w), where w is maximum width (worst O(n)).

## Topic 4 — Maximum Depth, Minimum Depth and Height

Maximum depth:
```cpp
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
```

Minimum depth must reach a leaf; do not take `min(0, childDepth)` when one child is null.

```cpp
int minDepth(TreeNode* root) {
    if (!root) return 0;
    if (!root->left) return 1 + minDepth(root->right);
    if (!root->right) return 1 + minDepth(root->left);
    return 1 + min(minDepth(root->left), minDepth(root->right));
}
```

Both O(n) time, O(h) recursion space.

## Topic 5 — Diameter and Balanced Binary Tree

### Diameter (LC 543)
Diameter is the maximum number of edges on a path between any two nodes. At each node, candidate diameter is left height + right height.

```cpp
int diameterOfBinaryTree(TreeNode* root) {
    int diameter = 0;

    function<int(TreeNode*)> height = [&](TreeNode* node) {
        if (!node) return 0;
        int l = height(node->left);
        int r = height(node->right);
        diameter = max(diameter, l + r);
        return 1 + max(l, r);
    };

    height(root);
    return diameter;
}
```

O(n) time, O(h) stack.

### Balanced Binary Tree (LC 110)
A tree is height-balanced if every node's left/right subtree heights differ by at most 1.

Use a postorder helper returning `-1` when unbalanced, otherwise height.

```cpp
bool isBalanced(TreeNode* root) {
    function<int(TreeNode*)> height = [&](TreeNode* node) {
        if (!node) return 0;

        int l = height(node->left);
        if (l == -1) return -1;

        int r = height(node->right);
        if (r == -1) return -1;

        if (abs(l-r) > 1) return -1;
        return 1 + max(l, r);
    };

    return height(root) != -1;
}
```

O(n) time, O(h) stack.

## Topic 6 — Binary Search Tree Fundamentals

A BST follows ordering: values in left subtree are smaller; values in right subtree are larger (for the strict-unique-values version).

Search:
```cpp
TreeNode* searchBST(TreeNode* root, int val) {
    while (root && root->val != val) {
        root = (val < root->val) ? root->left : root->right;
    }
    return root;
}
```

Average O(log n) in a balanced tree; worst O(n) if skewed.

Insert:
```cpp
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    if (val < root->val) root->left = insertIntoBST(root->left, val);
    else if (val > root->val) root->right = insertIntoBST(root->right, val);
    return root;
}
```

Validation is best done with lower/upper bounds, not only checking immediate children:

```cpp
bool valid(TreeNode* node, long long low, long long high) {
    if (!node) return true;
    if (node->val <= low || node->val >= high) return false;
    return valid(node->left, low, node->val) &&
           valid(node->right, node->val, high);
}
```

### Day 21 quick reference
- DFS traversals differ by position of root visit.
- BFS uses queue and processes level-size batches.
- Height/depth recursion O(n).
- Diameter can be calculated during height computation.
- BST inorder traversal is sorted for a strict BST.

---

# Day 22 — Advanced Binary Tree Problems

## Topic 1 — Invert Binary Tree (LC 226)

Swap left and right children at every node.

```cpp
TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    swap(root->left, root->right);
    invertTree(root->left);
    invertTree(root->right);
    return root;
}
```

Time O(n), stack O(h). Iterative BFS is also possible.

## Topic 2 — Same Tree and Symmetric Tree (LC 100, LC 101)

Same Tree: values and structure must match.

```cpp
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (!p || !q) return p == q;
    return p->val == q->val &&
           isSameTree(p->left, q->left) &&
           isSameTree(p->right, q->right);
}
```

Symmetric Tree: compare mirrored children.

```cpp
bool mirror(TreeNode* a, TreeNode* b) {
    if (!a || !b) return a == b;
    return a->val == b->val &&
           mirror(a->left, b->right) &&
           mirror(a->right, b->left);
}

bool isSymmetric(TreeNode* root) {
    return !root || mirror(root->left, root->right);
}
```

Time O(n), stack O(h).

## Topic 3 — Subtree of Another Tree (LC 572)

For each node in the main tree, test whether the tree rooted there is identical to the candidate subtree. Reuse `isSameTree`.

```cpp
bool isSubtree(TreeNode* root, TreeNode* subRoot) {
    if (!subRoot) return true;
    if (!root) return false;

    if (isSameTree(root, subRoot)) return true;

    return isSubtree(root->left, subRoot) ||
           isSubtree(root->right, subRoot);
}
```

Worst-case O(mn), where m and n are node counts.

## Topic 4 — Lowest Common Ancestor of a Binary Tree (LC 236)

If current node is null, p or q, return it. If left and right recursive results are both non-null, current node is LCA; otherwise propagate the non-null side.

```cpp
TreeNode* lowestCommonAncestor(TreeNode* root,
                               TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;

    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);

    if (left && right) return root;
    return left ? left : right;
}
```

Time O(n), stack O(h). Standard problem guarantees both nodes exist.

## Topic 5 — Right Side View and Zigzag Level Order (LC 199, LC 103)

Right side view: BFS and take the last node of each level.

```cpp
vector<int> rightSideView(TreeNode* root) {
    vector<int> ans;
    if (!root) return ans;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            TreeNode* node = q.front(); q.pop();
            if (i == sz - 1) ans.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    return ans;
}
```

Zigzag: same BFS, but write each level in forward or reverse index order.

```cpp
vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> ans;
    if (!root) return ans;

    queue<TreeNode*> q;
    q.push(root);
    bool leftToRight = true;

    while (!q.empty()) {
        int sz = q.size();
        vector<int> level(sz);

        for (int i = 0; i < sz; i++) {
            TreeNode* node = q.front(); q.pop();
            int idx = leftToRight ? i : sz - 1 - i;
            level[idx] = node->val;

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        ans.push_back(level);
        leftToRight = !leftToRight;
    }
    return ans;
}
```

Time O(n), queue O(w), output excluded from auxiliary space.

## Topic 6 — Construct Tree from Traversals and Path Sum

### Construct from Preorder + Inorder (LC 105)

Preorder identifies root first. Inorder splits left and right subtrees. A hash map gives inorder positions in O(1) average.

```cpp
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    unordered_map<int,int> pos;
    for (int i = 0; i < inorder.size(); i++) pos[inorder[i]] = i;
    int preIndex = 0;

    function<TreeNode*(int,int)> build = [&](int l, int r) -> TreeNode* {
        if (l > r) return nullptr;

        int value = preorder[preIndex++];
        TreeNode* root = new TreeNode(value);
        int mid = pos[value];

        root->left = build(l, mid - 1);
        root->right = build(mid + 1, r);
        return root;
    };

    return build(0, (int)inorder.size() - 1);
}
```

Time O(n), map O(n), recursion O(h). Assumes unique values and consistent traversals.

### Path Sum (LC 112)

Check whether a root-to-leaf path sums to target.

```cpp
bool hasPathSum(TreeNode* root, int target) {
    if (!root) return false;

    if (!root->left && !root->right)
        return target == root->val;

    int remaining = target - root->val;
    return hasPathSum(root->left, remaining) ||
           hasPathSum(root->right, remaining);
}
```

Time O(n), stack O(h).

### Path Sum II (LC 113) — backtracking variant

Maintain current path and remaining sum. Push before recursion; pop after exploring.

---

# Day 23 — Binary Search Trees and Advanced Tree Patterns

## Topic 1 — Kth Smallest in BST (LC 230)

Inorder traversal of a BST visits values in ascending order.

```cpp
int kthSmallest(TreeNode* root, int k) {
    stack<TreeNode*> st;
    TreeNode* cur = root;

    while (cur || !st.empty()) {
        while (cur) {
            st.push(cur);
            cur = cur->left;
        }

        cur = st.top(); st.pop();
        if (--k == 0) return cur->val;
        cur = cur->right;
    }
    return -1;
}
```

Time O(h+k) in typical traversal accounting, worst O(n); stack O(h). Reverse inorder gives kth largest.

## Topic 2 — Lowest Common Ancestor in BST (LC 235)

Use BST ordering. If both values are smaller, go left; if both larger, go right; otherwise current node is split point/LCA.

```cpp
TreeNode* lowestCommonAncestor(TreeNode* root,
                               TreeNode* p, TreeNode* q) {
    while (root) {
        if (p->val < root->val && q->val < root->val)
            root = root->left;
        else if (p->val > root->val && q->val > root->val)
            root = root->right;
        else
            return root;
    }
    return nullptr;
}
```

Time O(h), iterative space O(1).

## Topic 3 — Validate BST (LC 98)

Pass allowed lower and upper bounds. Every node must lie strictly between them.

```cpp
bool validate(TreeNode* node, long long low, long long high) {
    if (!node) return true;
    if (node->val <= low || node->val >= high) return false;

    return validate(node->left, low, node->val) &&
           validate(node->right, node->val, high);
}

bool isValidBST(TreeNode* root) {
    return validate(root, LLONG_MIN, LLONG_MAX);
}
```

Time O(n), stack O(h). `long long` bounds safely handle integer extremes.

## Topic 4 — Recover BST (LC 99)

Two nodes in a BST have had their values swapped. Inorder traversal should be strictly increasing. Find violations:
- First violation: set `first = prev`.
- Every violation: set `second = current`.
Finally swap their values.

```cpp
void recoverTree(TreeNode* root) {
    TreeNode *first = nullptr, *second = nullptr, *prev = nullptr;

    function<void(TreeNode*)> inorder = [&](TreeNode* node) {
        if (!node) return;
        inorder(node->left);

        if (prev && prev->val > node->val) {
            if (!first) first = prev;
            second = node;
        }
        prev = node;
        inorder(node->right);
    };

    inorder(root);
    if (first && second) swap(first->val, second->val);
}
```

Time O(n), recursion O(h).

## Topic 5 — Delete Node in BST (LC 450)

Cases:
1. No child: delete node, return null.
2. One child: replace node with its child.
3. Two children: copy inorder successor (minimum of right subtree), then delete successor from right subtree.

```cpp
TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) return nullptr;

    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    } else {
        if (!root->left) {
            TreeNode* right = root->right;
            delete root;
            return right;
        }
        if (!root->right) {
            TreeNode* left = root->left;
            delete root;
            return left;
        }

        TreeNode* successor = root->right;
        while (successor->left) successor = successor->left;

        root->val = successor->val;
        root->right = deleteNode(root->right, successor->val);
    }
    return root;
}
```

Time O(h), recursion O(h).

## Topic 6 — Balanced BST Construction and TreeMap Concepts

### Convert Sorted Array to BST (LC 108)

Choose middle as root and recursively construct left/right halves.

```cpp
TreeNode* sortedArrayToBST(vector<int>& a) {
    function<TreeNode*(int,int)> build = [&](int l, int r) -> TreeNode* {
        if (l > r) return nullptr;
        int mid = l + (r-l)/2;
        TreeNode* root = new TreeNode(a[mid]);
        root->left = build(l, mid-1);
        root->right = build(mid+1, r);
        return root;
    };
    return build(0, (int)a.size()-1);
}
```

Time O(n), recursion O(log n) for balanced result.

### Balanced tree concepts
- AVL trees maintain strict height balance using rotations.
- Red-Black trees maintain color invariants that bound height.
- Java `TreeMap` and `TreeSet` provide sorted map/set behavior with logarithmic operations and are implemented using Red-Black trees in standard OpenJDK implementations.
- C++ `map` and `set` are commonly implemented as Red-Black trees; the C++ standard specifies complexity/behavior, not a mandated internal tree implementation.

---

# Day 24 — Heaps, Priority Queues and Tries

## Topic 1 — Heap Fundamentals and C++ Priority Queue

A binary heap is a complete binary tree satisfying heap order.
- Min heap: parent <= children.
- Max heap: parent >= children.

Array index formulas:
- Parent: `(i-1)/2`
- Left child: `2*i+1`
- Right child: `2*i+2`

C++:
```cpp
priority_queue<int> maxHeap;
priority_queue<int, vector<int>, greater<int>> minHeap;
```

Operations: `top()` O(1), `push()` O(log n), `pop()` O(log n), `size()`/`empty()` O(1).

A heap is not a fully sorted array; it only guarantees parent/child ordering.

## Topic 2 — Kth Largest Element (LC 215)

Maintain a min heap of size k. The smallest among the k largest values is the answer.

```cpp
int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> pq;

    for (int x : nums) {
        pq.push(x);
        if (pq.size() > k) pq.pop();
    }
    return pq.top();
}
```

Time O(n log k), space O(k).

## Topic 3 — Top K Frequent Elements (LC 347)

Count frequencies, then keep the k highest frequencies in a min heap.

```cpp
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int,int> freq;
    for (int x : nums) freq[x]++;

    using P = pair<int,int>; // frequency, value
    priority_queue<P, vector<P>, greater<P>> pq;

    for (auto [value, count] : freq) {
        pq.push({count, value});
        if (pq.size() > k) pq.pop();
    }

    vector<int> ans;
    while (!pq.empty()) {
        ans.push_back(pq.top().second);
        pq.pop();
    }
    return ans;
}
```

Time O(n + m log k), space O(m+k), where m is number of distinct values. Output order is not necessarily frequency-descending.

## Topic 4 — Find Median from Data Stream (LC 295)

Use two heaps:
- Max heap `low` stores smaller half.
- Min heap `high` stores larger half.
- `low` has either same number of values as `high` or one extra.

```cpp
class MedianFinder {
    priority_queue<int> low;
    priority_queue<int, vector<int>, greater<int>> high;

public:
    void addNum(int num) {
        low.push(num);
        high.push(low.top());
        low.pop();

        if (low.size() < high.size()) {
            low.push(high.top());
            high.pop();
        }
    }

    double findMedian() {
        if (low.size() > high.size()) return low.top();
        return ((long long)low.top() + high.top()) / 2.0;
    }
};
```

Add O(log n), median O(1), space O(n). Cast before addition to avoid integer overflow.

## Topic 5 — Merge K Sorted Lists (LC 23)

Push the head of every non-empty list into a min heap. Repeatedly extract the smallest node, attach it to result, then push its next node.

```cpp
struct Compare {
    bool operator()(ListNode* a, ListNode* b) const {
        return a->val > b->val;
    }
};

ListNode* mergeKLists(vector<ListNode*>& lists) {
    priority_queue<ListNode*, vector<ListNode*>, Compare> pq;

    for (ListNode* node : lists)
        if (node) pq.push(node);

    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (!pq.empty()) {
        ListNode* node = pq.top();
        pq.pop();

        tail->next = node;
        tail = tail->next;

        if (node->next) pq.push(node->next);
    }

    tail->next = nullptr;
    return dummy.next;
}
```

For N total nodes and K lists: O(N log K) time, O(K) heap space.

## Topic 6 — Trie Fundamentals and Implementation (LC 208)

A Trie (prefix tree) stores strings by shared prefixes. Each node has child links and a flag indicating a complete word.

```cpp
class Trie {
    struct Node {
        Node* child[26]{};
        bool isEnd = false;
    };

    Node* root = new Node();

public:
    void insert(string word) {
        Node* cur = root;
        for (char c : word) {
            int i = c - 'a';
            if (!cur->child[i]) cur->child[i] = new Node();
            cur = cur->child[i];
        }
        cur->isEnd = true;
    }

    bool search(string word) {
        Node* cur = root;
        for (char c : word) {
            int i = c - 'a';
            if (!cur->child[i]) return false;
            cur = cur->child[i];
        }
        return cur->isEnd;
    }

    bool startsWith(string prefix) {
        Node* cur = root;
        for (char c : prefix) {
            int i = c - 'a';
            if (!cur->child[i]) return false;
            cur = cur->child[i];
        }
        return true;
    }
};
```

For word length L: insert/search/prefix O(L). Space proportional to created trie nodes; this implementation assumes lowercase English letters.

---

# Day 25 — Recursion and Backtracking Patterns

## Topic 1 — Recursion Fundamentals and Backtracking

A recursive function calls itself on a smaller problem. It needs:
1. Base case.
2. Progress toward base case.
3. Recursive call.

```cpp
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
```

Call stack example: `factorial(4)` expands to `4 * factorial(3)`, then `3 * factorial(2)`, then base case, followed by stack unwinding.

Backtracking pattern:
1. Choose.
2. Explore recursively.
3. Undo the choice.

```cpp
path.push_back(choice);
backtrack(nextState);
path.pop_back();
```

## Topic 2 — Subsets (LC 78)

For each element, choose include or exclude. There are `2^n` subsets.

```cpp
void dfs(int start, vector<int>& nums,
         vector<int>& path, vector<vector<int>>& ans) {
    ans.push_back(path);

    for (int i = start; i < nums.size(); i++) {
        path.push_back(nums[i]);
        dfs(i + 1, nums, path, ans);
        path.pop_back();
    }
}

vector<vector<int>> subsets(vector<int>& nums) {
    vector<vector<int>> ans;
    vector<int> path;
    dfs(0, nums, path, ans);
    return ans;
}
```

Time O(n·2ⁿ) including copying each subset; recursion/path space O(n), output O(n·2ⁿ).

## Topic 3 — Subsets II (LC 90)

Input can contain duplicates. Sort first and skip duplicate values at the same recursion level.

```cpp
void dfs(int start, vector<int>& nums,
         vector<int>& path, vector<vector<int>>& ans) {
    ans.push_back(path);

    for (int i = start; i < nums.size(); i++) {
        if (i > start && nums[i] == nums[i-1]) continue;

        path.push_back(nums[i]);
        dfs(i + 1, nums, path, ans);
        path.pop_back();
    }
}

vector<vector<int>> subsetsWithDup(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> ans;
    vector<int> path;
    dfs(0, nums, path, ans);
    return ans;
}
```

Critical condition is `i > start`, not `i > 0`. It skips duplicate choices at the same depth but still permits choosing repeated values at deeper levels.

## Topic 4 — Permutations (LC 46)

Order matters. Use a `used` array to track values already in the current permutation.

```cpp
void dfs(vector<int>& nums, vector<bool>& used,
         vector<int>& path, vector<vector<int>>& ans) {
    if (path.size() == nums.size()) {
        ans.push_back(path);
        return;
    }

    for (int i = 0; i < nums.size(); i++) {
        if (used[i]) continue;

        used[i] = true;
        path.push_back(nums[i]);

        dfs(nums, used, path, ans);

        path.pop_back();
        used[i] = false;
    }
}

vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> ans;
    vector<int> path;
    vector<bool> used(nums.size(), false);
    dfs(nums, used, path, ans);
    return ans;
}
```

There are n! permutations. Time O(n·n!) including output copying; auxiliary stack/path/used O(n), output O(n·n!).

## Topic 5 — Combination Sum (LC 39)

Candidates are distinct positive integers; each can be reused unlimited times. Pass `i` again after choosing candidate i. Advancing to `i+1` would prevent reuse.

```cpp
void dfs(int start, int remaining, vector<int>& a,
         vector<int>& path, vector<vector<int>>& ans) {
    if (remaining == 0) {
        ans.push_back(path);
        return;
    }

    for (int i = start; i < a.size(); i++) {
        if (a[i] > remaining) break;

        path.push_back(a[i]);
        dfs(i, remaining - a[i], a, path, ans);
        path.pop_back();
    }
}

vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> ans;
    vector<int> path;
    dfs(0, target, candidates, path, ans);
    return ans;
}
```

Sorting enables early `break`. Complexity is exponential/output-dependent; there is no simple polynomial bound because the number of valid combinations can be large.

## Topic 6 — Word Search (LC 79)

Search a word in a grid using DFS in four directions. A cell cannot be reused in the same path. Temporarily mark the current cell, explore neighbors, then restore it.

```cpp
bool exist(vector<vector<char>>& board, string word) {
    int m = board.size(), n = board[0].size();

    function<bool(int,int,int)> dfs = [&](int r, int c, int k) {
        if (k == word.size()) return true;

        if (r < 0 || c < 0 || r >= m || c >= n ||
            board[r][c] != word[k])
            return false;

        char saved = board[r][c];
        board[r][c] = '#';

        bool found = dfs(r+1,c,k+1) ||
                     dfs(r-1,c,k+1) ||
                     dfs(r,c+1,k+1) ||
                     dfs(r,c-1,k+1);

        board[r][c] = saved;
        return found;
    };

    for (int r = 0; r < m; r++)
        for (int c = 0; c < n; c++)
            if (dfs(r,c,0)) return true;

    return false;
}
```

Upper-bound time O(R·C·3^L) for word length L after the first step (each subsequent step has at most three non-previous directions); auxiliary recursion O(L). This implementation modifies the board temporarily but restores it.

### Day 25 quick reference
- Recursion needs base case and progress.
- Backtracking = choose → explore → undo.
- Subsets: include/exclude or start-index loop.
- Duplicates: sort and skip at same recursion depth.
- Permutations: `used[]` because order matters.
- Combination Sum: reuse index `i`.
- Word Search: mark and restore grid cell.

---

# PART III — DAYS 26–30: GREEDY, DP AND GRAPHS

The following section combines the remaining-day learning guide. Day 26 Topics 1–4 were taught interactively; Topics 5–6 and Days 27–30 were provided as the combined continuation guide. Treat Days 27–30 as planned/compiled learning material until you have worked through them.

# Day 26 — Greedy Algorithms and Intervals

## Topic 1 — Greedy Fundamentals and Activity Selection

A greedy algorithm makes a locally best choice at each step. It is correct only when the greedy-choice property and optimal substructure support that strategy.

Activity selection: sort by finishing time and repeatedly choose the earliest-finishing activity compatible with the last chosen activity.

```cpp
int maxActivities(vector<pair<int,int>>& a) {
    sort(a.begin(), a.end(),
         [](auto x, auto y) { return x.second < y.second; });

    int count = 0;
    int lastEnd = INT_MIN;

    for (auto [start, end] : a) {
        if (start >= lastEnd) {
            count++;
            lastEnd = end;
        }
    }
    return count;
}
```

Time O(n log n), scan O(n), extra scan space O(1).

## Topic 2 — Assign Cookies (LC 455)

Sort greed factors and cookie sizes. Give the smallest sufficient cookie to the least greedy remaining child.

```cpp
int findContentChildren(vector<int>& g, vector<int>& s) {
    sort(g.begin(), g.end());
    sort(s.begin(), s.end());

    int i = 0, j = 0;
    while (i < g.size() && j < s.size()) {
        if (s[j] >= g[i]) {
            i++;
            j++;
        } else {
            j++;
        }
    }
    return i;
}
```

Time O(n log n + m log m), extra space depends on sorting.

## Topic 3 — Jump Game (LC 55)

Maintain `maxReach`, the farthest index reachable so far. If current index is beyond it, return false.

```cpp
bool canJump(vector<int>& nums) {
    int maxReach = 0;

    for (int i = 0; i < nums.size(); i++) {
        if (i > maxReach) return false;
        maxReach = max(maxReach, i + nums[i]);
    }
    return true;
}
```

Time O(n), space O(1).

## Topic 4 — Jump Game II (LC 45)

Track current jump range and farthest next range. When current index reaches `currentEnd`, commit to another jump and update the range.

```cpp
int jump(vector<int>& nums) {
    int jumps = 0, currentEnd = 0, farthest = 0;

    for (int i = 0; i < (int)nums.size() - 1; i++) {
        farthest = max(farthest, i + nums[i]);

        if (i == currentEnd) {
            jumps++;
            currentEnd = farthest;
        }
    }
    return jumps;
}
```

Time O(n), space O(1). LeetCode guarantees the last index is reachable. For generalized input without that guarantee, add an unreachable check.

## Topic 5 — Non-overlapping Intervals (LC 435)

Sort by end time; keep earliest-finishing compatible intervals. Count overlapping intervals discarded.

```cpp
int eraseOverlapIntervals(vector<vector<int>>& a) {
    if (a.empty()) return 0;
    sort(a.begin(), a.end(),
         [](const auto& x, const auto& y) { return x[1] < y[1]; });

    int removed = 0, lastEnd = a[0][1];
    for (int i = 1; i < a.size(); i++) {
        if (a[i][0] < lastEnd) removed++;
        else lastEnd = a[i][1];
    }
    return removed;
}
```

Time O(n log n).

## Topic 6 — Merge Intervals and Meeting Rooms

Merge intervals (LC 56):
```cpp
vector<vector<int>> merge(vector<vector<int>>& a) {
    if (a.empty()) return {};
    sort(a.begin(), a.end());

    vector<vector<int>> ans{a[0]};
    for (int i = 1; i < a.size(); i++) {
        if (a[i][0] <= ans.back()[1])
            ans.back()[1] = max(ans.back()[1], a[i][1]);
        else
            ans.push_back(a[i]);
    }
    return ans;
}
```
Time O(n log n), output O(n).

Meeting Rooms (LC 252): sort by start; if current start < previous end, conflict. Time O(n log n).

Meeting Rooms II (LC 253): min heap of end times; reuse a room when earliest end <= next start.

```cpp
int minMeetingRooms(vector<vector<int>>& a) {
    sort(a.begin(), a.end());
    priority_queue<int, vector<int>, greater<int>> ends;

    for (auto& x : a) {
        if (!ends.empty() && ends.top() <= x[0]) ends.pop();
        ends.push(x[1]);
    }
    return (int)ends.size();
}
```

Time O(n log n), space O(n).

---

# Day 27 — Dynamic Programming Fundamentals

## Topic 1 — DP, Memoization and Tabulation

DP stores answers to overlapping subproblems and builds larger answers from smaller states. Define state, base cases, transition, iteration order and final answer.

Fibonacci memoization:
```cpp
int fibMemo(int n, vector<int>& dp) {
    if (n <= 1) return n;
    if (dp[n] != -1) return dp[n];
    return dp[n] = fibMemo(n-1, dp) + fibMemo(n-2, dp);
}
```

Tabulation:
```cpp
int fib(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}
```

Tabulation O(n) time, O(1) space in this optimized version.

## Topic 2 — Climbing Stairs (LC 70)

`dp[i] = dp[i-1] + dp[i-2]`.

```cpp
int climbStairs(int n) {
    if (n <= 2) return n;
    int a = 1, b = 2;
    for (int i = 3; i <= n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}
```

O(n) time, O(1) space.

## Topic 3 — House Robber (LC 198)

At each house, either skip it or rob it and add the best result from two houses back.

```cpp
int rob(vector<int>& nums) {
    int twoBack = 0, oneBack = 0;
    for (int x : nums) {
        int cur = max(oneBack, twoBack + x);
        twoBack = oneBack;
        oneBack = cur;
    }
    return oneBack;
}
```

O(n) time, O(1) space.

## Topic 4 — House Robber II (LC 213)

Circular arrangement means first and last cannot both be robbed. Solve ranges `[0,n-2]` and `[1,n-1]`.

```cpp
int robRange(vector<int>& a, int l, int r) {
    int p2 = 0, p1 = 0;
    for (int i = l; i <= r; i++) {
        int cur = max(p1, p2 + a[i]);
        p2 = p1;
        p1 = cur;
    }
    return p1;
}

int rob(vector<int>& a) {
    if (a.size() == 1) return a[0];
    return max(robRange(a, 0, (int)a.size()-2),
               robRange(a, 1, (int)a.size()-1));
}
```

O(n) time, O(1) space.

## Topic 5 — Coin Change (LC 322)

`dp[a]` is minimum coins for amount a. For each coin, consider `1 + dp[a-coin]`.

```cpp
int coinChange(vector<int>& coins, int amount) {
    const int INF = amount + 1;
    vector<int> dp(amount + 1, INF);
    dp[0] = 0;

    for (int a = 1; a <= amount; a++)
        for (int c : coins)
            if (a >= c && dp[a-c] != INF)
                dp[a] = min(dp[a], 1 + dp[a-c]);

    return dp[amount] == INF ? -1 : dp[amount];
}
```

O(amount × number of coins) time, O(amount) space. Greedy is not always optimal for arbitrary denominations.

## Topic 6 — Longest Increasing Subsequence (LC 300)

O(n²) DP: `dp[i]` = LIS length ending at i.

```cpp
int lengthOfLIS(vector<int>& a) {
    int n = a.size(), ans = 0;
    vector<int> dp(n, 1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++)
            if (a[j] < a[i])
                dp[i] = max(dp[i], dp[j] + 1);
        ans = max(ans, dp[i]);
    }
    return ans;
}
```

O(n²) time, O(n) space. Optimized `tails` + `lower_bound` gives O(n log n) time and O(n) space; `tails` length equals LIS length but `tails` itself need not be a real subsequence.

---

# Day 28 — Advanced Dynamic Programming

## Topic 1 — Unique Paths (LC 62)

`dp[i][j] = dp[i-1][j] + dp[i][j-1]`, with first row/column initialized to 1.

```cpp
int uniquePaths(int m, int n) {
    vector<vector<int>> dp(m, vector<int>(n, 1));
    for (int i = 1; i < m; i++)
        for (int j = 1; j < n; j++)
            dp[i][j] = dp[i-1][j] + dp[i][j-1];
    return dp[m-1][n-1];
}
```

O(mn) time and space; can optimize space to O(n).

## Topic 2 — Minimum Path Sum (LC 64)

Add each cell to the smaller of its top and left predecessor.

```cpp
int minPathSum(vector<vector<int>>& g) {
    int m = g.size(), n = g[0].size();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 && j == 0) continue;
            int up = i ? g[i-1][j] : INT_MAX;
            int left = j ? g[i][j-1] : INT_MAX;
            g[i][j] += min(up, left);
        }
    }
    return g[m-1][n-1];
}
```

O(mn) time, O(1) extra space because input is modified.

## Topic 3 — Partition Equal Subset Sum (LC 416)

If total is odd, return false. Otherwise target = total/2. Use 0/1 knapsack and iterate sums backwards so each number is used at most once.

```cpp
bool canPartition(vector<int>& a) {
    int total = accumulate(a.begin(), a.end(), 0);
    if (total % 2) return false;
    int target = total / 2;

    vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int x : a)
        for (int s = target; s >= x; s--)
            dp[s] = dp[s] || dp[s-x];

    return dp[target];
}
```

O(n × target) time, O(target) space.

## Topic 4 — Longest Common Subsequence (LC 1143)

`dp[i][j]` is LCS length of first i and first j characters. If characters match, diagonal + 1; otherwise max of top/left.

```cpp
int longestCommonSubsequence(string a, string b) {
    int m = a.size(), n = b.size();
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            if (a[i-1] == b[j-1])
                dp[i][j] = 1 + dp[i-1][j-1];
            else
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);

    return dp[m][n];
}
```

O(mn) time and space.

## Topic 5 — Edit Distance (LC 72)

Allowed operations: insert, delete, replace. Initialize empty-prefix costs to i and j.

```cpp
int minDistance(string a, string b) {
    int m = a.size(), n = b.size();
    vector<vector<int>> dp(m+1, vector<int>(n+1));

    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            if (a[i-1] == b[j-1])
                dp[i][j] = dp[i-1][j-1];
            else
                dp[i][j] = 1 + min({
                    dp[i-1][j], dp[i][j-1], dp[i-1][j-1]
                });

    return dp[m][n];
}
```

O(mn) time and space.

## Topic 6 — Decode Ways (LC 91)

A valid single digit 1–9 contributes `dp[i-1]`; a valid two-digit value 10–26 contributes `dp[i-2]`.

```cpp
int numDecodings(string s) {
    int n = s.size();
    if (n == 0 || s[0] == '0') return 0;

    vector<int> dp(n+1, 0);
    dp[0] = dp[1] = 1;

    for (int i = 2; i <= n; i++) {
        int one = s[i-1] - '0';
        int two = (s[i-2]-'0') * 10 + one;

        if (one >= 1 && one <= 9) dp[i] += dp[i-1];
        if (two >= 10 && two <= 26) dp[i] += dp[i-2];
    }
    return dp[n];
}
```

O(n) time and space. `"06"` is invalid; zeros can only be paired as 10 or 20.

---

# Day 29 — Advanced DSA Patterns and Problem-Solving

## Topic 1 — Monotonic Stack: Daily Temperatures (LC 739)

See Day 20 Topic 4. Each index is pushed/popped once; O(n) time, O(n) space.

## Topic 2 — Sliding Window Maximum (LC 239)

See Day 20 Topic 5. Decreasing deque of indices; O(n) time, O(k) space.

## Topic 3 — Subarray Sum Equals K (LC 560)

Prefix sum frequency map. Count earlier prefixes equal to `currentSum-k`.

```cpp
int subarraySum(vector<int>& a, int k) {
    unordered_map<int,int> freq;
    freq[0] = 1;
    int sum = 0, count = 0;

    for (int x : a) {
        sum += x;
        if (freq.count(sum-k)) count += freq[sum-k];
        freq[sum]++;
    }
    return count;
}
```

Average O(n) time, O(n) space. Use `long long` if values/sums can exceed int.

## Topic 4 — Product Except Self (LC 238)

See Day 16 Topic 3. Prefix then suffix, no division; O(n) time, O(1) auxiliary space excluding output.

## Topic 5 — Kth Largest / Top K Frequent

Kth Largest: min heap size k, O(n log k) time, O(k) space. Top K Frequent: frequency map plus heap or bucket sort; heap O(n + m log k), where m is distinct values.

## Topic 6 — Pattern Recognition

| Problem clue | Pattern |
|---|---|
| Contiguous subarray/substring | Sliding window or prefix sum |
| Pair in sorted data | Two pointers |
| Frequency / fast lookup | Hash map or set |
| Monotonic sorted search space | Binary search |
| Next greater/smaller | Monotonic stack |
| Top K | Heap |
| Explore choices and undo | Backtracking |
| Repeated subproblems / optimal value | DP |
| Overlapping ranges | Sorting + intervals |
| Connected components | BFS / DFS |
| Fewest edges in unweighted graph | BFS |
| Dependencies | Topological sort |

Interview workflow: clarify constraints → brute force → identify bottleneck → choose pattern → state invariant → complexity → dry run → code.

---

# Day 30 — Graphs: Fundamentals and Traversal Patterns

Graphs are deliberately placed last, as requested.

## Topic 1 — Graph Fundamentals and Representation

A graph contains vertices and edges. It may be directed/undirected and weighted/unweighted.

Adjacency list — O(V+E) space:
```cpp
int n = 5;
vector<vector<int>> adj(n);
adj[0].push_back(1);
adj[1].push_back(0); // reverse edge for undirected graph
```

Adjacency matrix — O(V²) space and O(1) edge lookup:
```cpp
vector<vector<int>> mat(n, vector<int>(n, 0));
mat[0][1] = mat[1][0] = 1;
```

Edge list — O(E) space:
```cpp
vector<pair<int,int>> edges = {{0,1}, {1,2}};
```

## Topic 2 — BFS

BFS explores level by level with a queue and finds shortest path distances in an unweighted graph.

```cpp
vector<int> bfs(int n, vector<vector<int>>& adj, int start) {
    vector<bool> visited(n, false);
    vector<int> order;
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);

        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true; // mark when enqueued
                q.push(v);
            }
        }
    }
    return order;
}
```

O(V+E) time, O(V) space. For disconnected graphs, start a traversal from each unvisited vertex to cover all components.

## Topic 3 — DFS

DFS explores deeply before backtracking.

```cpp
void dfs(int u, vector<vector<int>>& adj, vector<bool>& visited) {
    visited[u] = true;
    for (int v : adj[u])
        if (!visited[v])
            dfs(v, adj, visited);
}
```

O(V+E) time, O(V) visited and worst-case recursion space. Iterative DFS can avoid recursion-depth limits.

## Topic 4 — Number of Islands (LC 200)

Each connected group of land `'1'` is one island. On discovering land, increment answer and flood-fill four directions.

```cpp
class Solution {
    int m, n;

    void dfs(vector<vector<char>>& g, int r, int c) {
        if (r < 0 || c < 0 || r >= m || c >= n || g[r][c] != '1')
            return;

        g[r][c] = '0';
        dfs(g, r+1, c);
        dfs(g, r-1, c);
        dfs(g, r, c+1);
        dfs(g, r, c-1);
    }

public:
    int numIslands(vector<vector<char>>& g) {
        if (g.empty() || g[0].empty()) return 0;
        m = g.size();
        n = g[0].size();
        int count = 0;

        for (int r = 0; r < m; r++)
            for (int c = 0; c < n; c++)
                if (g[r][c] == '1') {
                    count++;
                    dfs(g, r, c);
                }
        return count;
    }
};
```

O(mn) time, worst-case recursion O(mn). This modifies grid; use a separate visited array if needed.

## Topic 5 — Clone Graph (LC 133)

Use a map from original node pointer to clone pointer. Insert the clone in the map before recursively cloning neighbors to handle cycles.

```cpp
class Solution {
    unordered_map<Node*, Node*> copies;

public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        if (copies.count(node)) return copies[node];

        Node* copy = new Node(node->val);
        copies[node] = copy;

        for (Node* nei : node->neighbors)
            copy->neighbors.push_back(cloneGraph(nei));

        return copy;
    }
};
```

O(V+E) time, O(V) map plus recursion.

## Topic 6 — Course Schedule / Directed Cycle Detection (LC 207)

Prerequisite `[course, prerequisite]` creates edge `prerequisite → course`. A valid course order exists iff there is no directed cycle.

Kahn's topological BFS:
1. Build adjacency list and indegrees.
2. Queue every zero-indegree vertex.
3. Pop and process; decrement neighbors' indegrees.
4. Enqueue neighbors when indegree becomes zero.
5. If processed count equals V, no cycle exists.

```cpp
bool canFinish(int n, vector<vector<int>>& prerequisites) {
    vector<vector<int>> adj(n);
    vector<int> indegree(n, 0);

    for (auto& p : prerequisites) {
        int course = p[0], pre = p[1];
        adj[pre].push_back(course);
        indegree[course]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (indegree[i] == 0) q.push(i);

    int processed = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        processed++;

        for (int v : adj[u])
            if (--indegree[v] == 0)
                q.push(v);
    }
    return processed == n;
}
```

O(V+E) time, O(V+E) space.

### Graph concepts to remember
- BFS: queue, level traversal, shortest paths in unweighted graphs.
- DFS: recursion/stack, components and path exploration.
- Topological sort: directed acyclic graphs.
- Dijkstra: non-negative weighted shortest paths.
- Directed and undirected cycle detection differ.
- Mark visited when enqueuing in BFS to avoid duplicate queue entries.

---

# Final DSA Pattern Map

| Pattern | Signal | Example |
|---|---|---|
| Two pointers | Sorted pairs / in-place movement | Two Sum II |
| Sliding window | Contiguous range | Longest Substring Without Repeating Characters |
| Prefix sum + map | Count target-sum subarrays | Subarray Sum Equals K |
| Binary search | Sorted or monotonic answer | Koko Eating Bananas |
| Fast/slow pointers | Linked-list middle/cycle | Linked List Cycle |
| Monotonic stack | Next greater/smaller | Daily Temperatures |
| Monotonic deque | Window maximum/minimum | Sliding Window Maximum |
| Heap | Top K / streaming median | Kth Largest / Median Finder |
| Backtracking | Explore, recurse, undo | Subsets / Word Search |
| Greedy | Safe local choice | Jump Game / Intervals |
| DP | Overlapping subproblems | House Robber / Coin Change |
| BFS | Levels / unweighted shortest path | Graph traversal |
| DFS | Components / deep traversal | Number of Islands |
| Topological sort | Dependency order | Course Schedule |

# Final Revision Checklist

- [ ] I can analyze time and auxiliary space complexity.
- [ ] I can recognize arrays, hashing, prefix sums, two pointers and sliding windows.
- [ ] I can solve Kadane, 3Sum, 4Sum, Product Except Self and Maximum Product Subarray.
- [ ] I can solve matrix marker, spiral and rotation problems.
- [ ] I can write binary search and identify monotonic answer spaces.
- [ ] I can reverse, merge and detect cycles in linked lists.
- [ ] I can use stacks, queues, monotonic stacks and deques.
- [ ] I can implement tree traversals, BFS, DFS and BST operations.
- [ ] I understand heaps, priority queues and tries.
- [ ] I can explain recursion and backtracking with choose/explore/undo.
- [ ] I can justify greedy decisions.
- [ ] I can define DP states, base cases and transitions.
- [ ] I can represent graphs and implement BFS/DFS.
- [ ] I can solve Number of Islands, Clone Graph and Course Schedule.
- [ ] I can explain the chosen pattern and complexity in an interview.

## Notes on provenance and scope

- Days 16–25 are reconstructed from the topics, problems and approaches covered in the prior learning sessions and retained learning records. They are organized into a single consistent revision notebook, not a verbatim transcript of every earlier chat message.
- Day 26 includes the four topics taught interactively in the conversation (Greedy fundamentals, Assign Cookies, Jump Game, Jump Game II) plus the two interval topics from the continuation notes.
- Days 27–30 are the compiled continuation syllabus and solutions from the remaining-day guide. They should be treated as material to learn/review, not as proof that each topic has already been taught interactively.
- The notebook focuses on the named questions and solutions in this roadmap. It does not claim to contain every possible NeetCode 150 problem or every alternative solution discussed in every historical turn.
