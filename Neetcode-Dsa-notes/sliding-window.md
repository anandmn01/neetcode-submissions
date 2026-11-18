# NeetCode 150 – 15. Best Time to Buy and Sell Stock

**Category:** Arrays & Hashing  
**Pattern:** Greedy + One Pass  
**Language:** C++

## Problem

You are given an array `prices`, where `prices[i]` represents the price of a stock on the `i-th` day.

You want to maximize your profit by choosing:

- One day to buy a stock.
- A future day to sell that stock.

Return the maximum profit possible.

If no profit can be made, return `0`.

### Important Conditions

1. You must buy before selling.
2. You can complete only one transaction.
3. You cannot sell before buying.
4. If prices continuously decrease, return `0`.

### Example

```cpp
Input: prices = [7,1,5,3,6,4]

Output: 5
```

Explanation:

- Buy on day 2 at price 1.
- Sell on day 5 at price 6.

```text
Profit = Selling Price - Buying Price
       = 6 - 1
       = 5
```

---

# Approach 1: Brute Force

## Logic

Check every possible buying and selling combination.

1. Select a buying day using the outer loop.
2. Select a future selling day using the inner loop.
3. Calculate the profit.
4. Update the maximum profit.

## Code

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {

        int ans = 0;
        int n = prices.size();

        for(int i = 0; i < n; i++) {

            for(int j = i + 1; j < n; j++) {

                int profit = prices[j] - prices[i];

                ans = max(ans, profit);
            }
        }

        return ans;
    }
};
```

## Complexity

- Time: O(n²)
- Auxiliary Space: O(1)

**Limitation:** We repeatedly check possible buying and selling combinations.

---

# Approach 2: Greedy – Track Minimum Buying Price

<badge color="success">Recommended interview approach</badge>

## Core Logic

Instead of checking every possible combination, maintain two variables:

```cpp
int minPrice = INT_MAX;
int maxProfit = 0;
```

- `minPrice`: Lowest stock price encountered so far.
- `maxProfit`: Maximum profit found so far.

Traverse the array once.

For every price, perform three operations.

### Step 1: Update Minimum Buying Price

```cpp
minPrice = min(minPrice, prices[i]);
```

This keeps track of the cheapest buying opportunity encountered so far.

### Step 2: Calculate Today's Profit

```cpp
int profit = prices[i] - minPrice;
```

We assume that we buy at the minimum price encountered earlier and sell at today's price.

### Step 3: Update Maximum Profit

```cpp
maxProfit = max(maxProfit, profit);
```

Keep the highest profit found so far.

## Complete C++ Code

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {

        int minPrice = INT_MAX;
        int maxProfit = 0;

        for(int i = 0; i < prices.size(); i++) {

            // Track minimum buying price
            minPrice = min(minPrice, prices[i]);

            // Calculate possible profit
            int profit = prices[i] - minPrice;

            // Update maximum profit
            maxProfit = max(maxProfit, profit);
        }

        return maxProfit;
    }
};
```

---

# Detailed Dry Run

Input:

```cpp
prices = [7, 1, 5, 3, 6, 4]
```

Initial values:

```text
minPrice = INT_MAX
maxProfit = 0
```

| Day | Price | Minimum Price | Today's Profit | Maximum Profit |
|---:|---:|---:|---:|---:|
| 1 | 7 | 7 | 0 | 0 |
| 2 | 1 | 1 | 0 | 0 |
| 3 | 5 | 1 | 4 | 4 |
| 4 | 3 | 1 | 2 | 4 |
| 5 | 6 | 1 | 5 | 5 |
| 6 | 4 | 1 | 3 | 5 |

Final result:

```text
Output: 5
```

Best transaction:

```text
Buy:  Day 2 → Price 1
Sell: Day 5 → Price 6

Maximum Profit = 6 - 1 = 5
```

---

# Why Does This Greedy Approach Work?

The key observation is:

**To maximize profit for a particular selling day, we should buy at the lowest price encountered before or on that day.**

For every day:

```cpp
profit = currentPrice - minPrice;
```

We don't need to remember every previous buying price.

We only need the minimum one because it gives the highest possible profit for the current selling price.

The algorithm checks every selling day while maintaining the best buying opportunity.

This makes it possible to solve the problem in one traversal.

---

# Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Greedy – Minimum Price | O(n) | O(1) |

**Best approach:** Greedy – Track Minimum Buying Price.

---

# Important C++ Concepts

## 1. INT_MAX

```cpp
int minPrice = INT_MAX;
```

Initializes the minimum price with the largest possible integer value.

The first array element will therefore become the initial minimum.

## 2. min()

```cpp
minPrice = min(minPrice, prices[i]);
```

Keeps the smaller of the two values.

## 3. max()

```cpp
maxProfit = max(maxProfit, profit);
```

Keeps the largest profit found so far.

## 4. Greedy Strategy

A greedy algorithm makes a locally optimal decision at each step.

Here, we maintain the lowest buying price seen so far and calculate the best possible profit for each current selling price.

---

# Edge Cases

| Input | Expected Output | Reason |
|---|---:|---|
| `[7,1,5,3,6,4]` | 5 | Buy at 1, sell at 6 |
| `[7,6,4,3,1]` | 0 | Prices continuously decrease |
| `[1,2]` | 1 | Buy at 1, sell at 2 |
| `[2,1]` | 0 | No profitable transaction |
| `[5]` | 0 | Only one day |
| `[3,3,3,3]` | 0 | No price difference |

---

# Interview Pattern Recognition

When a problem asks you to:

- Find maximum profit from buying and selling.
- Buy before selling.
- Track the lowest previous value.
- Optimize repeated comparisons.

Think:

**Greedy + One Pass**

The important observation is that we only need the minimum value encountered so far to calculate the best profit for the current day.

---

# 30-Second Revision

1. Initialize `minPrice = INT_MAX`.
2. Initialize `maxProfit = 0`.
3. Traverse the prices array.
4. Update the minimum buying price.
5. Calculate `profit = prices[i] - minPrice`.
6. Update maximum profit.
7. Return `maxProfit`.

**Recommended approach:** Greedy – Minimum Price Tracking.

**Time:** O(n)  
**Auxiliary Space:** O(1)

# NeetCode 150 – 16. Longest Substring Without Repeating Characters

**Category:** Sliding Window  
**Pattern:** Variable-Size Sliding Window + HashSet  
**Language:** C++

## Problem

Given a string `s`, find the length of the longest substring without repeating characters.

A substring is a contiguous sequence of characters within a string.

Return the maximum length of a substring in which every character appears only once.

### Examples

**Example 1**

```cpp
Input: s = "abcabcbb"
Output: 3
```

Explanation:

The longest substrings without repeating characters are:

```text
"abc", "bca", "cab"
```

Their maximum length is 3.

**Example 2**

```cpp
Input: s = "bbbbb"
Output: 1
```

Explanation:

The longest valid substring is `"b"`.

**Example 3**

```cpp
Input: s = "pwwkew"
Output: 3
```

Explanation:

The longest valid substring is `"wke"`.

Remember: `"pwke"` is not a valid substring because its characters are not contiguous in the original string.

---

# Approach 1: Brute Force

## Logic

Generate every possible substring and check whether it contains duplicate characters.

1. Select a starting index.
2. Select an ending index.
3. Check whether all characters in that substring are unique.
4. If valid, update the maximum length.

## Code

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {

        int n = s.size();
        int ans = 0;

        for(int i = 0; i < n; i++) {

            for(int j = i; j < n; j++) {

                unordered_set<char> st;
                bool valid = true;

                for(int k = i; k <= j; k++) {

                    if(st.count(s[k])) {
                        valid = false;
                        break;
                    }

                    st.insert(s[k]);
                }

                if(valid) {
                    ans = max(ans, j - i + 1);
                }
            }
        }

        return ans;
    }
};
```

## Complexity

- Time: O(n³) average, considering substring validation with a HashSet.
- Auxiliary Space: O(min(n, character set size)).

**Limitation:** We repeatedly check characters in overlapping substrings.

---

# Approach 2: Sliding Window + HashSet

<badge color="success">Recommended approach</badge>

## Core Logic

Instead of generating every substring, maintain a window containing only unique characters.

A sliding window uses two pointers:

```cpp
int left = 0;
int right = 0;
```

- `left` represents the starting index.
- `right` represents the ending index.

Use a HashSet to store the characters currently inside the window.

### Step 1: Initialize

```cpp
unordered_set<char> st;

int left = 0;
int ans = 0;
```

### Step 2: Expand the window

Traverse the string using `right`.

```cpp
for(int right = 0; right < s.size(); right++)
```

If the current character is not present in the set, insert it.

### Step 3: Handle duplicates

If the current character already exists, shrink the window from the left until the duplicate is removed.

```cpp
while(st.count(s[right])) {
    st.erase(s[left]);
    left++;
}
```

### Step 4: Insert the current character

```cpp
st.insert(s[right]);
```

### Step 5: Update maximum length

The current window length is:

```cpp
right - left + 1
```

Update the answer:

```cpp
ans = max(ans, right - left + 1);
```

## Complete C++ Code

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {

        unordered_set<char> st;

        int left = 0;
        int ans = 0;

        for(int right = 0; right < s.size(); right++) {

            while(st.count(s[right])) {

                st.erase(s[left]);
                left++;
            }

            st.insert(s[right]);

            ans = max(ans, right - left + 1);
        }

        return ans;
    }
};
```

---

# Detailed Dry Run

Input:

```cpp
s = "abcabcbb"
```

Initial values:

```text
left = 0
ans = 0
set = {}
```

| Right | Character | Window after processing | Set | Maximum |
|---:|---|---|---|---:|
| 0 | a | a | {a} | 1 |
| 1 | b | ab | {a,b} | 2 |
| 2 | c | abc | {a,b,c} | 3 |
| 3 | a | bca | {b,c,a} | 3 |
| 4 | b | cab | {c,a,b} | 3 |
| 5 | c | abc | {a,b,c} | 3 |
| 6 | b | cb | {c,b} | 3 |
| 7 | b | b | {b} | 3 |

Final answer:

```text
3
```

### Understanding the duplicate handling

When `right = 3`, the current character is `'a'`.

But `'a'` already exists in the window:

```text
abc
```

Remove characters from the left:

```text
Remove a → bc
```

Now `'a'` is no longer present.

Insert the new `'a'`:

```text
bca
```

The window remains valid.

---

# Approach 3: Sliding Window + HashMap (Optimized)

## Core Logic

The HashSet approach removes characters one by one when a duplicate is found.

We can improve this by remembering the last index of every character using a HashMap.

Instead of shrinking the window one step at a time, directly move `left` past the previous occurrence.

Maintain:

```cpp
unordered_map<char, int> mp;
```

Here:

- Key = character.
- Value = its most recent index.

For every character:

1. Check whether it has appeared before.
2. Make sure its previous occurrence is inside the current window.
3. Move `left` to one position after the previous occurrence.
4. Update the character's latest index.
5. Calculate the window length.

## Code

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {

        unordered_map<char, int> mp;

        int left = 0;
        int ans = 0;

        for(int right = 0; right < s.size(); right++) {

            char ch = s[right];

            if(mp.find(ch) != mp.end() &&
               mp[ch] >= left) {

                left = mp[ch] + 1;
            }

            mp[ch] = right;

            ans = max(ans, right - left + 1);
        }

        return ans;
    }
};
```

## Why Is `mp[ch] >= left` Important?

Consider:

```text
s = "abba"
```

After processing the first three characters:

```text
Current window = "b"
left = 2
```

The previous occurrence of `'a'` is at index 0.

When we encounter the final `'a'` at index 3, we must not move `left` backwards to index 1.

That is why we check:

```cpp
mp[ch] >= left
```

Only previous occurrences inside the current window should change the left pointer.

## Complexity

- Time: O(n) average.
- Auxiliary Space: O(min(n, character set size)).

---

# Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force | O(n³) average | O(min(n, character set size)) |
| Sliding Window + HashSet | O(n) average | O(min(n, character set size)) |
| Sliding Window + HashMap | O(n) average | O(min(n, character set size)) |

**Recommended:** Understand HashSet first, then learn the HashMap optimization.

---

# Important C++ Concepts

## 1. unordered_set

Stores unique characters.

```cpp
unordered_set<char> st;

st.insert('a');

st.count('a');  // 1

st.erase('a');
```

## 2. unordered_map

Stores key-value pairs.

```cpp
unordered_map<char, int> mp;

mp['a'] = 3;
```

Here, character `'a'` was last found at index 3.

## 3. Sliding Window

A window is represented using two pointers:

```cpp
int left = 0;
int right = 0;
```

Expand the window by moving `right`.

Shrink the window by moving `left`.

## 4. Window Length Formula

```cpp
right - left + 1
```

The `+1` is necessary because both endpoints are included.

---

# Edge Cases

| Input | Expected Output | Reason |
|---|---:|---|
| `""` | 0 | Empty string |
| `"a"` | 1 | Single character |
| `"aaaa"` | 1 | All characters repeat |
| `"abcabcbb"` | 3 | Longest unique substring is `abc` |
| `"bbbbb"` | 1 | Only one unique character |
| `"pwwkew"` | 3 | Longest valid substring is `wke` |
| `"abcdef"` | 6 | All characters are unique |
| `"abba"` | 2 | Previous occurrence outside the window must be ignored |

---

# Interview Pattern Recognition

When a problem asks you to:

- Find the longest substring.
- Find a contiguous sequence of characters.
- Maintain unique characters.
- Expand and shrink a range dynamically.

Think:

**Variable-Size Sliding Window + HashSet / HashMap**

The general strategy:

1. Expand the window using the right pointer.
2. Check whether the current window is valid.
3. Shrink from the left if necessary.
4. Update the maximum window length.

---

# 30-Second Revision

1. Initialize `left = 0` and `ans = 0`.
2. Create an `unordered_set<char>`.
3. Expand the window using `right`.
4. If the current character already exists, remove characters from the left.
5. Insert the current character.
6. Calculate `right - left + 1`.
7. Update the maximum answer.
8. Return `ans`.

**Recommended approach:** Sliding Window + HashSet.

**Time:** O(n) average.  
**Auxiliary Space:** O(min(n, character set size)).

**Pattern:** Sliding Window → Variable-Size Window + Duplicate Detection.

**Status:** Notes prepared. Practise the HashSet implementation independently, then try the HashMap optimization.

# NeetCode 150 – 17. Longest Repeating Character Replacement

**Category:** Sliding Window  
**Pattern:** Variable-Size Sliding Window + Frequency Counting  
**Language:** C++

## Problem

Given a string `s` consisting of uppercase English letters and an integer `k`, return the length of the longest substring that contains the same letter after performing at most `k` character replacements.

You can replace any character with another uppercase English letter.

### Examples

**Example 1**

```cpp
Input: s = "ABAB", k = 2
Output: 4
```

Explanation:

Replace both `A` characters with `B`, or both `B` characters with `A`.

The entire string can become:

```text
BBBB
```

**Example 2**

```cpp
Input: s = "AABABBA", k = 1
Output: 4
```

Explanation:

Replace the `B` at index 2 with `A`.

The substring becomes:

```text
AAAA
```

Its length is 4.

---

# 1. Understanding the Core Logic

The main question is:

**How do we know whether the current sliding window can be converted into a string containing only one repeated character using at most k replacements?**

We need two things:

1. Window length.
2. Maximum frequency of any character inside the window.

The formula is:

\[
Replacements = WindowLength - MaxFrequency
\]

Where:

```cpp
int windowLength = right - left + 1;
int replacements = windowLength - maxFreq;
```

If:

```cpp
replacements <= k
```

The current window is valid.

Otherwise, we need to shrink the window.

### Example

```text
s = "AABABBA"
k = 1
```

Consider this window:

```text
A A B A
```

Frequency:

```text
A = 3
B = 1
```

Window length = 4

Maximum frequency = 3

Replacements needed:

```text
4 - 3 = 1
```

Since `1 <= k`, this window is valid.

---

# Approach 1: Brute Force

## Logic

Generate every possible substring and calculate its character frequencies.

For each substring:

1. Count the frequency of every character.
2. Find the maximum frequency.
3. Calculate replacements required.
4. If replacements are at most `k`, update the answer.

## Code

```cpp
class Solution {
public:
    int characterReplacement(string s, int k) {

        int n = s.size();
        int ans = 0;

        for(int i = 0; i < n; i++) {

            int freq[26] = {0};
            int maxFreq = 0;

            for(int j = i; j < n; j++) {

                freq[s[j] - 'A']++;

                maxFreq = max(maxFreq, freq[s[j] - 'A']);

                int windowLength = j - i + 1;

                int replacements = windowLength - maxFreq;

                if(replacements <= k) {
                    ans = max(ans, windowLength);
                }
            }
        }

        return ans;
    }
};
```

## Complexity

- Time: O(26 × n²), which simplifies to O(n²) because the alphabet size is fixed.
- Auxiliary Space: O(26) = O(1).

**Limitation:** We repeatedly examine substrings, making this approach inefficient for large inputs.

---

# Approach 2: Sliding Window + Frequency Array

<badge color="success">Recommended interview approach</badge>

## Core Logic

Instead of generating every substring, maintain a window using two pointers.

```cpp
int left = 0;
int right = 0;
```

Maintain a frequency array:

```cpp
int freq[26] = {0};
```

Since the input contains only uppercase English letters, we need only 26 frequency positions.

### Step 1: Expand the window

Traverse the string using the right pointer.

```cpp
freq[s[right] - 'A']++;
```

### Step 2: Track maximum frequency

```cpp
maxFreq = max(maxFreq, freq[s[right] - 'A']);
```

`maxFreq` represents the highest frequency of any character in the current window or a previously visited window.

### Step 3: Check whether the window is valid

Calculate:

```cpp
int windowLength = right - left + 1;
```

Replacements needed:

```cpp
int replacements = windowLength - maxFreq;
```

If:

```cpp
windowLength - maxFreq > k
```

The window needs more than `k` replacements.

Shrink it from the left:

```cpp
freq[s[left] - 'A']--;
left++;
```

### Step 4: Update the answer

```cpp
ans = max(ans, right - left + 1);
```

## Complete C++ Code

```cpp
class Solution {
public:
    int characterReplacement(string s, int k) {

        int freq[26] = {0};

        int left = 0;
        int maxFreq = 0;
        int ans = 0;

        for(int right = 0; right < s.size(); right++) {

            freq[s[right] - 'A']++;

            maxFreq = max(maxFreq, freq[s[right] - 'A']);

            while((right - left + 1) - maxFreq > k) {

                freq[s[left] - 'A']--;

                left++;
            }

            ans = max(ans, right - left + 1);
        }

        return ans;
    }
};
```

---

# Detailed Dry Run

Input:

```cpp
s = "AABABBA"
k = 1
```

Initial values:

```text
left = 0
maxFreq = 0
ans = 0
```

| Right | Character | Window | Max Frequency | Replacements Needed | Action |
|---:|---|---|---:|---:|---|
| 0 | A | A | 1 | 0 | Valid |
| 1 | A | AA | 2 | 0 | Valid |
| 2 | B | AAB | 2 | 1 | Valid |
| 3 | A | AABA | 3 | 1 | Valid |
| 4 | B | AABAB | 3 | 2 | Shrink |
| 5 | B | ABABB | 3 | 2 | Shrink |
| 6 | A | BABBA | 3 | 2 | Shrink |

After shrinking, the maximum valid window length remains 4.

Final answer:

```text
4
```

---

# Why Does the Formula Work?

The objective is to make every character in the current window identical.

Suppose:

```text
Window = A A B A B
```

Frequency:

```text
A = 3
B = 2
```

To make all characters equal, we should keep the most frequent character and replace the remaining characters.

Therefore:

\[
Replacements = WindowLength - MaxFrequency
\]

\[
= 5 - 3 = 2
\]

If `k = 2`, the window is valid.

If `k = 1`, the window is invalid.

**Key observation:** We only need the maximum frequency, not the frequency of every character individually when calculating the number of replacements.

---

# Important Concept: Why Don't We Decrease maxFreq?

In the optimized solution, when the left pointer moves, we decrease the frequency array, but we do not recalculate `maxFreq`.

Why?

`maxFreq` is allowed to represent the highest frequency encountered so far, rather than the exact maximum frequency in the current window.

This makes the algorithm efficient because we avoid scanning all 26 characters repeatedly.

The window may temporarily be larger than a strictly valid window according to the current frequency counts. However, this stale maximum does not cause the algorithm to overestimate the final maximum achievable window length.

For this problem, the one-step shrink version is also sufficient:

```cpp
if((right - left + 1) - maxFreq > k) {
    freq[s[left] - 'A']--;
    left++;
}
```

The `while` version is easier to understand as a general sliding-window validity pattern, while the `if` version is a common optimized implementation for this problem.

---

# Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Sliding Window + Frequency Array | O(n) | O(1) |

**Best approach:** Sliding Window + Frequency Array.

---

# Important C++ Concepts

## 1. Frequency Array

```cpp
int freq[26] = {0};
```

Stores the frequency of uppercase English letters.

## 2. Character to Array Index

```cpp
freq[s[i] - 'A']++;
```

Examples:

```text
'A' - 'A' = 0
'B' - 'A' = 1
'C' - 'A' = 2
```

## 3. Maximum Frequency

```cpp
maxFreq = max(maxFreq, freq[s[right] - 'A']);
```

Tracks the most frequent character count.

## 4. Sliding Window Length

```cpp
right - left + 1
```

Calculates the current window size.

## 5. Window Validity

```cpp
windowLength - maxFreq <= k
```

Checks whether the window can be converted using at most `k` replacements.

---

# Edge Cases

| Input | k | Expected Output | Reason |
|---|---:|---:|---|
| `"ABAB"` | 2 | 4 | Replace two characters |
| `"AABABBA"` | 1 | 4 | One replacement is allowed |
| `"AAAA"` | 2 | 4 | All characters already match |
| `"ABCDE"` | 1 | 2 | Only one replacement allowed |
| `"A"` | 0 | 1 | Single character |
| `"ABBB"` | 0 | 3 | Longest existing repeated substring |
| `""` | 2 | 0 | Empty string |

---

# Interview Pattern Recognition

When a problem asks you to:

- Find the longest substring.
- Allow a limited number of replacements.
- Maintain character frequencies.
- Expand and shrink a window depending on a condition.

Think:

**Variable-Size Sliding Window + Frequency Counting**

The general strategy:

1. Expand the window using `right`.
2. Maintain character frequencies.
3. Track the maximum character frequency.
4. Calculate replacements required.
5. Shrink the window when replacements exceed `k`.
6. Update the maximum window length.

---

# 30-Second Revision

1. Create a frequency array of size 26.
2. Initialize `left = 0`, `maxFreq = 0`, and `ans = 0`.
3. Expand the window using `right`.
4. Increase the frequency of the current character.
5. Update `maxFreq`.
6. Calculate `windowLength - maxFreq`.
7. If it exceeds `k`, shrink the window from the left.
8. Update the maximum valid window length.
9. Return `ans`.

**Recommended approach:** Sliding Window + Frequency Array.

**Time:** O(n)  
**Auxiliary Space:** O(1), because the frequency array has a fixed size of 26.

**Pattern:** Sliding Window → Variable-Size Window + Character Frequency.

**Status:** Notes prepared. Practise deriving the replacement formula independently.
# NeetCode 150 – 18. Permutation in String

**Category:** Sliding Window  
**Pattern:** Fixed-Size Sliding Window + Character Frequency  
**Language:** C++

## Problem

Given two strings `s1` and `s2`, return `true` if `s2` contains a permutation of `s1`; otherwise, return `false`.

A permutation means rearranging the characters of a string.

**Important conditions:**

- Every character in `s1` must appear in the substring.
- Character frequencies must match exactly.
- The characters can appear in any order.
- The matching substring must be contiguous in `s2`.

### Example 1

```cpp
Input: s1 = "ab", s2 = "eidbaooo"
Output: true
```

Explanation:

The substring `"ba"` is a permutation of `"ab"`.

### Example 2

```cpp
Input: s1 = "ab", s2 = "eidboaoo"
Output: false
```

Explanation:

There is no substring of length 2 containing exactly one `a` and one `b`.

---

# 1. Understanding the Core Logic

The most important observation:

**Every permutation of a string has exactly the same character frequencies as the original string.**

For example:

```text
s1 = "abc"

Possible permutations:
abc
acb
bac
bca
cab
cba
```

All permutations have:

```text
a = 1
b = 1
c = 1
```

Therefore, we don't need to generate every permutation.

We only need to compare character frequencies.

## Why Fixed-Size Sliding Window?

If `s1` has length 3, every permutation must also have length 3.

Therefore, we maintain a window of exactly:

```cpp
int windowSize = s1.size();
```

We slide this window across `s2` and compare its character frequencies with `s1`.

---

# Approach 1: Brute Force

## Logic

1. Generate every substring of `s2` having length `s1.size()`.
2. Sort the substring.
3. Sort `s1`.
4. Compare both strings.
5. If they match, return `true`.
6. Otherwise, continue searching.

## Code

```cpp
class Solution {
public:
    bool checkInclusion(string s1, string s2) {

        if(s1.size() > s2.size()) {
            return false;
        }

        sort(s1.begin(), s1.end());

        int k = s1.size();

        for(int i = 0; i <= s2.size() - k; i++) {

            string temp = s2.substr(i, k);

            sort(temp.begin(), temp.end());

            if(temp == s1) {
                return true;
            }
        }

        return false;
    }
};
```

## Complexity

Let:
- `n` = length of `s2`
- `m` = length of `s1`

- Time: O(m log m + (n - m + 1) × m log m).
- Auxiliary Space: O(m) for the temporary substring and sorting storage.

**Limitation:** Sorting every window repeatedly is inefficient.

---

# Approach 2: Fixed-Size Sliding Window + Frequency Arrays

<badge color="success">Recommended interview approach</badge>

## Core Logic

Since the problem involves lowercase English letters, we can use two frequency arrays of size 26.

```cpp
vector<int> s1Freq(26, 0);
vector<int> windowFreq(26, 0);
```

- `s1Freq` stores character frequencies in `s1`.
- `windowFreq` stores character frequencies in the current window of `s2`.

### Step 1: Handle impossible cases

```cpp
if(s1.size() > s2.size()) {
    return false;
}
```

A longer string cannot have a permutation inside a shorter string.

### Step 2: Count frequencies of s1

```cpp
for(char c : s1) {
    s1Freq[c - 'a']++;
}
```

### Step 3: Create the first window

The window size must equal `s1.size()`.

```cpp
int k = s1.size();

for(int i = 0; i < k; i++) {
    windowFreq[s2[i] - 'a']++;
}
```

### Step 4: Compare frequencies

```cpp
if(s1Freq == windowFreq) {
    return true;
}
```

If both frequency arrays are equal, the current window is a permutation of `s1`.

### Step 5: Slide the window

Remove the character leaving the window:

```cpp
windowFreq[s2[i - k] - 'a']--;
```

Add the new character entering the window:

```cpp
windowFreq[s2[i] - 'a']++;
```

Compare the frequency arrays again.

---

# Complete C++ Code

```cpp
class Solution {
public:
    bool checkInclusion(string s1, string s2) {

        if(s1.size() > s2.size()) {
            return false;
        }

        vector<int> s1Freq(26, 0);
        vector<int> windowFreq(26, 0);

        int k = s1.size();

        // Count frequencies in s1
        for(char c : s1) {
            s1Freq[c - 'a']++;
        }

        // Create the first window
        for(int i = 0; i < k; i++) {
            windowFreq[s2[i] - 'a']++;
        }

        // Compare first window
        if(s1Freq == windowFreq) {
            return true;
        }

        // Slide the window
        for(int i = k; i < s2.size(); i++) {

            // Remove outgoing character
            windowFreq[s2[i - k] - 'a']--;

            // Add incoming character
            windowFreq[s2[i] - 'a']++;

            // Compare frequencies
            if(s1Freq == windowFreq) {
                return true;
            }
        }

        return false;
    }
};
```

---

# Detailed Dry Run

Input:

```cpp
s1 = "ab"
s2 = "eidbaooo"
```

### Step 1: Frequency of s1

```text
s1 = "ab"

a = 1
b = 1
```

### Step 2: Initial window

Window size:

```text
k = 2
```

First window:

```text
ei
```

Frequency does not match `s1`.

### Step 3: Slide the window

| Window | Character removed | Character added | Match? |
|---|---|---|---|
| `ei` | — | — | No |
| `id` | e | d | No |
| `db` | i | b | No |
| `ba` | d | a | Yes |

When the window becomes `"ba"`:

```text
s1 = "ab"
Window = "ba"
```

Both frequency arrays are equal:

```text
a = 1
b = 1
```

Return:

```cpp
true
```

---

# Approach 3: Sliding Window + Character Match Count

Instead of comparing two complete frequency arrays every time, we can maintain how many of the 26 character frequencies currently match.

## Logic

Maintain:

```cpp
vector<int> s1Freq(26, 0);
vector<int> windowFreq(26, 0);

int matches = 0;
```

Initially, calculate how many character positions have equal frequencies.

As the window slides:

1. Update the outgoing character's frequency.
2. Update the incoming character's frequency.
3. Update `matches` whenever a frequency changes.
4. If all 26 positions match, return `true`.

## Code

```cpp
class Solution {
public:
    bool checkInclusion(string s1, string s2) {

        if(s1.size() > s2.size()) {
            return false;
        }

        vector<int> s1Freq(26, 0);
        vector<int> windowFreq(26, 0);

        for(char c : s1) {
            s1Freq[c - 'a']++;
        }

        int k = s1.size();

        for(int i = 0; i < k; i++) {
            windowFreq[s2[i] - 'a']++;
        }

        int matches = 0;

        for(int i = 0; i < 26; i++) {
            if(s1Freq[i] == windowFreq[i]) {
                matches++;
            }
        }

        if(matches == 26) {
            return true;
        }

        for(int i = k; i < s2.size(); i++) {

            int incoming = s2[i] - 'a';
            int outgoing = s2[i - k] - 'a';

            // Remove outgoing character's old contribution
            if(windowFreq[outgoing] == s1Freq[outgoing]) {
                matches--;
            }

            windowFreq[outgoing]--;

            // Check outgoing character's new contribution
            if(windowFreq[outgoing] == s1Freq[outgoing]) {
                matches++;
            }

            // Remove incoming character's old contribution
            if(windowFreq[incoming] == s1Freq[incoming]) {
                matches--;
            }

            windowFreq[incoming]++;

            // Check incoming character's new contribution
            if(windowFreq[incoming] == s1Freq[incoming]) {
                matches++;
            }

            if(matches == 26) {
                return true;
            }
        }

        return false;
    }
};
```

## Complexity

- Time: O(n + 26), which simplifies to O(n).
- Auxiliary Space: O(26) = O(1).

**Advantage:** Avoids comparing all 26 frequency positions after every window movement.

**Limitation:** The `matches` bookkeeping is more complicated and easier to implement incorrectly.

---

# Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force + Sorting | O(n × m log m) | O(m) |
| Two Frequency Arrays | O(n × 26) = O(n) | O(26) = O(1) |
| Frequency Arrays + Matches | O(n + 26) = O(n) | O(26) = O(1) |

**Recommended interview approach:** Two Frequency Arrays.

It is simple, efficient and easy to explain.

---

# Important C++ Concepts

## 1. Frequency Array

```cpp
vector<int> freq(26, 0);
```

Creates an array for lowercase English letters.

## 2. Character to Index Conversion

```cpp
freq[c - 'a']++;
```

Examples:

```text
'a' - 'a' = 0
'b' - 'a' = 1
'c' - 'a' = 2
```

## 3. Fixed-Size Sliding Window

```cpp
int k = s1.size();
```

The window must always have the same length as `s1`.

## 4. Removing an Outgoing Character

```cpp
windowFreq[s2[i - k] - 'a']--;
```

Removes the character that leaves the window.

## 5. Adding an Incoming Character

```cpp
windowFreq[s2[i] - 'a']++;
```

Adds the new character entering the window.

## 6. Vector Comparison

```cpp
if(s1Freq == windowFreq)
```

C++ compares the elements of both vectors. If all corresponding frequencies match, the result is `true`.

---

# Edge Cases

| Input | Expected Output | Reason |
|---|---|---|
| `s1 = "ab", s2 = "eidbaooo"` | true | Contains `"ba"` |
| `s1 = "ab", s2 = "eidboaoo"` | false | No matching permutation |
| `s1 = "a", s2 = "a"` | true | Exact match |
| `s1 = "a", s2 = "b"` | false | No matching character |
| `s1 = "abc", s2 = "ab"` | false | s1 is longer |
| `s1 = "aabc", s2 = "caaebac"` | true | Contains `"aabc"` as a permutation in `"c a a b"`? No; verify by matching a contiguous window before deciding |

---

# Interview Pattern Recognition

When a problem asks you to:

- Find whether a permutation exists inside another string.
- Compare character frequencies.
- Find a contiguous substring of a specific length.
- Slide a fixed-size window across a string.

Think:

**Fixed-Size Sliding Window + Frequency Counting**

The general strategy:

1. Count frequencies in the target string.
2. Create a window of the same length.
3. Count frequencies in that window.
4. Compare the frequency arrays.
5. Slide the window by removing one character and adding another.
6. Return true when the frequencies match.

---

# 30-Second Revision

1. Check whether `s1.size() > s2.size()`. If yes, return false.
2. Create two frequency arrays of size 26.
3. Count characters in `s1`.
4. Build the first window in `s2` with length `s1.size()`.
5. Compare both frequency arrays.
6. Slide the window one character at a time.
7. Decrease the outgoing character frequency.
8. Increase the incoming character frequency.
9. Compare the frequency arrays.
10. Return true if any window matches; otherwise return false.

**Recommended approach:** Fixed-Size Sliding Window + Two Frequency Arrays.

**Time:** O(n), because the alphabet size is fixed at 26.  
**Auxiliary Space:** O(1).

**Pattern:** Sliding Window → Fixed-Size Window + Character Frequency.

**Status:** Notes prepared. Practise the window update logic independently.

# 19. Minimum Window Substring

**NeetCode Pattern:** Sliding Window  
**Technique:** Variable-Size Sliding Window + Frequency Hash Map  
**Difficulty:** Hard

## 1. Problem Understanding

Given two strings `s` and `t`, find the minimum-length substring of `s` that contains all characters of `t`, including their required frequencies.

Return an empty string if no such window exists.

### Example 1

```text
Input:
s = "ADOBECODEBANC"
t = "ABC"

Output:
"BANC"
```

**Why?**

- The substring `"BANC"` contains A, B and C.
- All characters required by `t` are present.
- Its length is 4, which is the minimum possible.

### Example 2

```text
Input:
s = "a"
t = "a"

Output:
"a"
```

### Example 3

```text
Input:
s = "a"
t = "aa"

Output:
""
```

There are not enough `a` characters in `s`.

---

## 2. Core Intuition

We need to maintain a window inside `s` that contains all characters required by `t`.

There are two operations:

1. **Expand the window:** Move `right` to include more characters.
2. **Shrink the window:** Once the window is valid, move `left` forward to find a smaller valid window.

The objective is to find the smallest valid window.

Think of it this way:

```text
Expand → Find a valid window → Shrink → Update answer
              ↑                         |
              └─────────────────────────┘
```

## 3. Optimal Approach: Sliding Window + Frequency Map

We maintain:

| Variable | Purpose |
|---|---|
| `left` | Starting index of the current window |
| `right` | Ending index of the current window |
| `need` | Frequency of each character required from `t` |
| `window` | Frequency of characters currently inside the window |
| `required` | Number of distinct characters in `t` |
| `formed` | Number of distinct characters whose required frequency is satisfied |
| `minLen` | Length of the smallest valid window |
| `start` | Starting index of the smallest valid window |

### Why do we need `formed`?

Suppose:

```text
t = "AABC"
```

Required frequencies:

```text
A → 2
B → 1
C → 1
```

There are 3 distinct required characters, so:

```cpp
required = 3;
```

A character contributes to `formed` only when its frequency in the current window reaches the required frequency.

For example:

```text
Window = "AABC"

A → 2  ✓
B → 1  ✓
C → 1  ✓

formed = 3
required = 3
```

The window is valid when:

```cpp
formed == required
```

This is better than checking the entire frequency map repeatedly.

## 4. Dry Run

Consider:

```text
s = "ADOBECODEBANC"
t = "ABC"
```

Required frequencies:

```text
A → 1
B → 1
C → 1

required = 3
```

The window expands until it contains A, B and C.

| Window | formed | Action |
|---|---:|---|
| `ADOBEC` | 3 | Valid; record length 6 |
| `DOBEC` | 3 | Valid; record length 5 |
| `OBEC` | 2 | Invalid; stop shrinking |
| `...` | ... | Continue expanding |
| `BANC` | 3 | Valid; record length 4 |
| `ANC` | 2 | Invalid; stop shrinking |

The smallest valid window is:

```text
"BANC"
```

Answer: `"BANC"`.

## 5. C++ Optimal Code

```cpp
class Solution {
public:
    string minWindow(string s, string t) {

        if (t.empty() || s.size() < t.size()) {
            return "";
        }

        unordered_map<char, int> need;
        unordered_map<char, int> window;

        for (char c : t) {
            need[c]++;
        }

        int required = need.size();
        int formed = 0;

        int left = 0;
        int minLen = INT_MAX;
        int start = 0;

        for (int right = 0; right < s.size(); right++) {

            char c = s[right];
            window[c]++;

            // Check whether this character's required frequency is satisfied
            if (need.count(c) && window[c] == need[c]) {
                formed++;
            }

            // Try shrinking while the window is valid
            while (formed == required) {

                // Update the minimum window
                if (right - left + 1 < minLen) {
                    minLen = right - left + 1;
                    start = left;
                }

                char leftChar = s[left];
                window[leftChar]--;

                // The window becomes invalid if a required frequency falls below need
                if (need.count(leftChar) &&
                    window[leftChar] < need[leftChar]) {
                    formed--;
                }

                left++;
            }
        }

        if (minLen == INT_MAX) {
            return "";
        }

        return s.substr(start, minLen);
    }
};
```

## 6. Understanding the Most Important Conditions

### Condition 1: When does `formed` increase?

```cpp
if (need.count(c) && window[c] == need[c]) {
    formed++;
}
```

Suppose:

```text
t = "AABC"
```

When the window gets its first `A`:

```text
window[A] = 1
need[A] = 2
```

The required frequency is not satisfied.

When the second `A` enters:

```text
window[A] = 2
need[A] = 2
```

Now `formed` increases by 1.

### Condition 2: Why use `while` instead of `if`?

```cpp
while (formed == required)
```

Because one valid window may contain unnecessary characters.

We repeatedly remove characters from the left while the window remains valid, trying to find the smallest possible window.

### Condition 3: When does `formed` decrease?

```cpp
if (need.count(leftChar) &&
    window[leftChar] < need[leftChar]) {
    formed--;
}
```

If removing a character makes its frequency insufficient, the window is no longer valid.

For example:

```text
need[A] = 2
window[A] = 2
```

After removing one `A`:

```text
window[A] = 1
```

Now the required frequency is not satisfied, so `formed` decreases.

### Condition 4: Why store `start` and `minLen`?

```cpp
start = left;
minLen = right - left + 1;
```

We store the best window's indices instead of creating a new substring every time.

Finally:

```cpp
return s.substr(start, minLen);
```

## 7. Complexity Analysis

Let:

- `N` = length of `s`
- `M` = length of `t`

| Operation | Complexity |
|---|---|
| Build frequency map | O(M) |
| Expand right pointer | O(N) |
| Shrink left pointer | O(N) |
| Extract final substring | O(N) maximum |

**Time Complexity: O(N + M)** average.

Why? Both pointers move only forward. Each character enters and leaves the window at most once. Hash-map operations are O(1) on average.

**Space Complexity: O(K)**, where `K` is the number of distinct characters stored in the maps. For a fixed character set, this is O(1).

## 8. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Checking only whether a character exists | Duplicate character requirements may be missed |
| Using `if` instead of `while` | Does not shrink the window completely |
| Increasing `formed` for every occurrence | Can count a character multiple times |
| Decreasing `formed` whenever a character leaves | Only decrease when its frequency falls below the required count |
| Returning `s.substr(left, right-left+1)` at the end | The pointers no longer necessarily represent the best window |
| Forgetting `t.empty()` | Can lead to incorrect handling of an empty target |

## 9. Edge Cases to Test

| Input `s` | Input `t` | Expected |
|---|---|---|
| `"ADOBECODEBANC"` | `"ABC"` | `"BANC"` |
| `"a"` | `"a"` | `"a"` |
| `"a"` | `"aa"` | `""` |
| `"aa"` | `"aa"` | `"aa"` |
| `"ab"` | `"b"` | `"b"` |
| `"bba"` | `"ab"` | `"ba"` |
| `"aaflslflsldkalskaaa"` | `"aaa"` | `"aaa"` |

## 10. C++ Concepts to Remember

- `unordered_map<char, int>`: Stores character frequencies.
- `need.size()`: Gives the number of distinct required characters.
- `need.count(c)`: Checks whether a character exists in the required map.
- `s.substr(start, length)`: Extracts a substring.
- `INT_MAX`: Useful for initializing the minimum length.
- Two pointers: Both pointers move forward, avoiding repeated scanning.

## 11. Pattern Recognition for Interviews

Recognize this pattern when a problem asks you to:

- Find the smallest or largest substring satisfying a condition.
- Maintain character frequencies in a changing window.
- Include all required characters, possibly with duplicates.
- Expand a window until valid, then shrink it to optimize the answer.

**Remember:**

- Fixed-size window → Window size remains constant.
- Variable-size window → Expand and shrink depending on validity.
- Minimum Window Substring → Expand until valid, then shrink while valid.

## 12. 30-Second Revision

1. Build `need` frequency map from `t`.
2. Expand the window using `right`.
3. Update `window` frequency.
4. Increase `formed` when a required character reaches its exact required frequency.
5. When `formed == required`, shrink using `left`.
6. Update the minimum window before shrinking.
7. Decrease `formed` only when removing a required character makes its frequency insufficient.
8. Return the substring using stored `start` and `minLen`.

**One-line interview explanation:**

"I use a variable-size sliding window with two frequency maps. I expand the right pointer until all required character frequencies are satisfied, then shrink the left pointer while the window remains valid, keeping track of the smallest valid substring."

---

**Next problem:** Sliding Window Maximum.

# 20. Sliding Window Maximum

**NeetCode Pattern:** Sliding Window  
**Technique:** Monotonic Deque (Decreasing Deque)  
**Difficulty:** Hard

## 1. Problem Understanding

You are given an integer array `nums` and an integer `k`.

A window of size `k` moves from left to right across the array.

Return the maximum element from every window.

### Example

```text
Input:
nums = [1,3,-1,-3,5,3,6,7]
k = 3

Output:
[3,3,5,5,6,7]
```

The windows are:

```text
[1, 3, -1], -3, 5, 3, 6, 7  → 3
 1, [3, -1, -3], 5, 3, 6, 7 → 3
 1, 3, [-1, -3, 5], 3, 6, 7 → 5
 1, 3, -1, [-3, 5, 3], 6, 7 → 5
 1, 3, -1, -3, [5, 3, 6], 7 → 6
 1, 3, -1, -3, 5, [3, 6, 7] → 7
```

Final answer:

```text
[3, 3, 5, 5, 6, 7]
```

---

## 2. Core Intuition

We need to find the maximum in every window of size `k`.

A brute-force approach checks every element in every window. That repeats work.

Instead, we use a **deque (double-ended queue)** to store useful indices.

Our deque follows a decreasing order of values:

```text
Front → Largest value
         Smaller value
         Smaller value
Back  → Smallest value
```

For example:

```text
Deque values = [5, 3, 1]
```

The maximum is always at the front.

### Why store indices instead of values?

We need indices for two reasons:

1. To identify whether an element has gone outside the current window.
2. To access its value using `nums[index]`.

---

## 3. Monotonic Deque Rules

There are three important operations.

### Rule 1: Remove expired indices

```cpp
if (!dq.empty() && dq.front() <= i - k) {
    dq.pop_front();
}
```

If an index is outside the current window, remove it.

For the current index `i`, the valid window starts at:

```cpp
i - k + 1
```

Therefore, any index less than or equal to `i-k` is expired.

### Rule 2: Remove smaller elements from the back

```cpp
while (!dq.empty() && nums[dq.back()] <= nums[i]) {
    dq.pop_back();
}
```

Suppose:

```text
Deque values = [5, 3, 1]
New value = 4
```

We remove `1` and `3`, because `4` is larger and will remain useful at least as long as those older elements.

The deque becomes:

```text
[5, 4]
```

**Important:** A smaller element before a newer, larger element can never become the maximum while the newer element remains in the window.

### Rule 3: Insert the current index

```cpp
dq.push_back(i);
```

After removing smaller elements, insert the current index.

The deque remains decreasing by value.

---

## 4. Optimal C++ Solution

```cpp
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {

        deque<int> dq;
        vector<int> ans;

        for (int i = 0; i < nums.size(); i++) {

            // 1. Remove indices outside the current window
            if (!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }

            // 2. Maintain decreasing order
            while (!dq.empty() && nums[dq.back()] <= nums[i]) {
                dq.pop_back();
            }

            // 3. Insert current index
            dq.push_back(i);

            // 4. Record maximum once the first full window is formed
            if (i >= k - 1) {
                ans.push_back(nums[dq.front()]);
            }
        }

        return ans;
    }
};
```

---

## 5. Dry Run

Input:

```text
nums = [1, 3, -1, -3, 5, 3, 6, 7]
k = 3
```

We store indices in the deque, but show both indices and values below.

| i | nums[i] | Deque values after processing | Maximum |
|---:|---:|---|---:|
| 0 | 1 | `[1]` | — |
| 1 | 3 | `[3]` | — |
| 2 | -1 | `[3, -1]` | 3 |
| 3 | -3 | `[3, -1, -3]` | 3 |
| 4 | 5 | `[5]` | 5 |
| 5 | 3 | `[5, 3]` | 5 |
| 6 | 6 | `[6]` | 6 |
| 7 | 7 | `[7]` | 7 |

The answer is recorded when:

```cpp
i >= k - 1
```

For `k = 3`, the first complete window ends at index `2`.

Final output:

```text
[3, 3, 5, 5, 6, 7]
```

---

## 6. Why Does This Work?

Consider this window:

```text
[5, 3, 6]
```

When `6` enters:

- `3` is smaller than `6`, so remove it.
- `5` is also smaller than `6`, so remove it.
- Insert the index of `6`.

Deque:

```text
[6]
```

Why is it safe to remove `5` and `3`?

Because `6` is newer and larger than both. Until `6` leaves the window, neither `5` nor `3` can be the maximum.

This is the main property of a monotonic decreasing deque.

---

## 7. Complexity Analysis

Let `n` be the length of `nums`.

| Operation | Complexity |
|---|---|
| Insert an index | O(1) amortized |
| Remove from front | O(1) |
| Remove from back | O(1) amortized |
| Read maximum | O(1) |

**Time Complexity: O(n)**

Although there is a `while` loop, every index is inserted once and removed at most once. Therefore, total deque operations are linear.

**Space Complexity: O(k)** for the deque, plus O(n-k+1) for the output vector.

---

## 8. Other Approaches

### Approach 1: Brute Force

For every window, scan all `k` elements to find the maximum.

```cpp
vector<int> ans;

for (int i = 0; i <= nums.size() - k; i++) {

    int maxi = INT_MIN;

    for (int j = i; j < i + k; j++) {
        maxi = max(maxi, nums[j]);
    }

    ans.push_back(maxi);
}
```

- Time: O((n-k+1) × k), or O(nk)
- Extra space: O(1), excluding output

### Approach 2: Max Heap

Use a max heap containing values and indices.

- Insert each new element.
- Remove heap-top entries whose indices are outside the window.
- The heap top gives the current maximum.

Typical complexity:

- Time: O(n log n)
- Space: O(n)

A heap can contain expired elements below the top until they reach the top, so it may grow beyond `k`.

### Approach 3: Monotonic Deque

- Keep only useful indices.
- Maintain decreasing values.
- Maximum is always at the front.

- Time: O(n)
- Deque space: O(k)

**Best interview solution: Monotonic Deque.**

---

## 9. Common Mistakes

| Mistake | Why it is wrong |
|---|---|
| Storing only values | Difficult to identify expired elements |
| Removing smaller elements from the front | The front must contain the maximum |
| Forgetting to remove expired indices | Maximum may belong to an old window |
| Using `i > k - 1` | Skips the first complete window |
| Using `nums[dq.back()] < nums[i]` | Works too, but keeps older equal values unnecessarily |
| Thinking the `while` loop makes it O(n²) | Each index is inserted and removed at most once |

---

## 10. Edge Cases

| Input | k | Expected output |
|---|---:|---|
| `[1]` | 1 | `[1]` |
| `[1,3,2]` | 3 | `[3]` |
| `[5,4,3,2]` | 2 | `[5,4,3]` |
| `[1,1,1,1]` | 2 | `[1,1,1]` |
| `[-7,-8,-6,-5]` | 2 | `[-7,-6,-5]` |
| `[9,8,7,6]` | 1 | `[9,8,7,6]` |

---

## 11. C++ Concepts to Remember

- `deque<int>`: Double-ended queue.
- `push_back()`: Insert at the back.
- `pop_back()`: Remove from the back.
- `pop_front()`: Remove from the front.
- `front()`: Access the first element.
- `back()`: Access the last element.
- `nums[dq.front()]`: Get the maximum value using its stored index.

---

## 12. Pattern Recognition for Interviews

Recognize **Monotonic Deque** when a problem asks you to:

- Find maximum or minimum in every sliding window.
- Maintain an ordered collection of useful candidates.
- Remove elements that are no longer relevant.
- Retrieve the current maximum or minimum in O(1).

### Remember the difference

| Problem | Main technique |
|---|---|
| Minimum Window Substring | Variable-size sliding window + frequency map |
| Sliding Window Maximum | Fixed-size sliding window + monotonic deque |

---

## 13. 30-Second Revision

1. Create `deque<int>` to store indices.
2. Remove expired indices from the front: `dq.front() <= i-k`.
3. Remove smaller or equal values from the back.
4. Push the current index.
5. Once `i >= k-1`, the front contains the maximum.
6. Add `nums[dq.front()]` to the answer.

**One-line interview explanation:**

"I use a monotonic decreasing deque that stores indices. I remove expired indices from the front and smaller elements from the back. This keeps the maximum at the front, allowing each sliding-window maximum to be retrieved in O(1), with O(n) total time."
**Pattern:** Greedy + One Pass + Minimum So Far.

**Status:** Notes prepared. Practise explaining why we only need to track the minimum previous price.
