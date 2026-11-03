# NeetCode 150 – DSA Interview Preparation

**Language:** C++  
**Target completion:** 25 November 2026  
**Progress:** 1 / 150 questions  
**Category:** Arrays & Hashing

---

# 01. Contains Duplicate

**Problem:** Given an integer array `nums`, return `true` if any value appears at least twice. Otherwise, return `false`.

Example:

```cpp
Input: nums = {1, 2, 3, 1}
Output: true

Input: nums = {1, 2, 3, 4}
Output: false
```

## Approach 1: Brute Force – Nested Loops

### Logic

Compare every element with every other element.

If two elements are equal, a duplicate exists.

### Code

```cpp
class Solution {
public:
    bool hasDuplicate(vector<int>& nums) {

        for(int i = 0; i < nums.size(); i++) {

            for(int j = i + 1; j < nums.size(); j++) {

                if(nums[i] == nums[j]) {
                    return true;
                }
            }
        }

        return false;
    }
};
```

### Complexity

- Time: O(n²)
- Space: O(1)

**Limitation:** Too slow for large arrays.

---

## Approach 2: Sorting

### Logic

Sort the array first.

After sorting, duplicate elements will always appear next to each other.

Compare adjacent elements to identify duplicates.

### Code

```cpp
class Solution {
public:
    bool hasDuplicate(vector<int>& nums) {

        sort(nums.begin(), nums.end());

        for(int i = 1; i < nums.size(); i++) {

            if(nums[i] == nums[i - 1]) {
                return true;
            }
        }

        return false;
    }
};
```

### Example

Before sorting:

`[4, 2, 3, 1, 2]`

After sorting:

`[1, 2, 2, 3, 4]`

Now `nums[1] == nums[2]`.

Duplicate found!

### Complexity

- Time: O(n log n)
- Auxiliary space: Depends on sorting implementation.
- Note: Modifies the original array.

**When useful:** When sorting is already required or additional hash-table memory should be avoided.

---

## Approach 3: HashMap – Frequency Counting

### Logic

Use an unordered_map to maintain the frequency of every number.

If any number's frequency becomes greater than 1, return true.

### Code

```cpp
class Solution {
public:
    bool hasDuplicate(vector<int>& nums) {

        unordered_map<int, int> mp;

        for(int x : nums) {

            mp[x]++;

            if(mp[x] > 1) {
                return true;
            }
        }

        return false;
    }
};
```

### Complexity

- Time: O(n) average
- Space: O(n)

**When useful:** When you need to count how many times each element occurs.

---

## Approach 4: HashSet – Recommended Approach

### Logic

We don't need the frequency of each number.

We only need to know whether a number has already appeared.

An unordered_set stores unique elements.

Before inserting an element, check whether it already exists.

### Code

```cpp
class Solution {
public:
    bool hasDuplicate(vector<int>& nums) {

        unordered_set<int> st;

        for(int x : nums) {

            if(st.find(x) != st.end()) {
                return true;
            }

            st.insert(x);
        }

        return false;
    }
};
```

### Dry Run

Input:

`nums = {1, 2, 3, 2}`

| Element | Set before processing | Result |
|---|---|---|
| 1 | {} | Insert 1 |
| 2 | {1} | Insert 2 |
| 3 | {1,2} | Insert 3 |
| 2 | {1,2,3} | Duplicate found |

Output: `true`

### Complexity

- Time: O(n) average
- Space: O(n)

**Why preferred?**

We don't need frequency counting. We only need existence checking.

Therefore, HashSet is simpler than HashMap for this problem.

---

## Approach 5: HashSet – Using insert().second

This is a shorter implementation using an important C++ STL feature.

### Code

```cpp
class Solution {
public:
    bool hasDuplicate(vector<int>& nums) {

        unordered_set<int> st;

        for(int x : nums) {

            if(!st.insert(x).second) {
                return true;
            }
        }

        return false;
    }
};
```

### Important STL Concept

`unordered_set.insert(x)` returns a pair.

- `.first`: Iterator pointing to the element.
- `.second`: Boolean indicating whether insertion succeeded.

| Situation | insert(x).second |
|---|---|
| Element is new | true |
| Element already exists | false |

Therefore:

```cpp
if(!st.insert(x).second)
```

means the element already exists.

### Complexity

- Time: O(n) average
- Space: O(n)

---

# Approach Comparison

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Sorting | O(n log n) | Depends on sort |
| HashMap | O(n) average | O(n) |
| HashSet | O(n) average | O(n) |
| HashSet insert().second | O(n) average | O(n) |

Note: Hash-based operations have O(n²) worst-case total time under pathological collision conditions.

---

# Important C++ STL Concepts

### 1. unordered_map

Stores key-value pairs.

```cpp
unordered_map<int, int> mp;

mp[5]++;
```

Useful for:
- Frequency counting.
- Finding occurrences.
- Key-value mapping.

### 2. unordered_set

Stores unique elements.

```cpp
unordered_set<int> st;

st.insert(10);

if(st.find(10) != st.end()) {
    cout << "Found";
}
```

Useful for:
- Duplicate detection.
- Existence checking.
- Removing duplicate values.

### 3. Important Differences

| unordered_map | unordered_set |
|---|---|
| Stores key-value pairs | Stores unique values |
| Can maintain frequency | Cannot maintain frequency directly |
| Uses mp[key] | Uses insert(), find() |
| Useful for counting | Useful for existence checking |

---

# Interview Pattern Recognition

Ask yourself:

1. Do I need to count occurrences? → HashMap.
2. Do I only need to check whether an element exists? → HashSet.
3. Can sorting make duplicates adjacent? → Sorting.
4. Is the array small enough for nested loops? → Brute force can work.

---

# 30-Second Revision

**Contains Duplicate**

- Brute Force: Compare every pair.
- Sorting: Sort and compare adjacent elements.
- HashMap: Maintain frequency.
- HashSet: Check whether an element already exists.
- STL Trick: `!st.insert(x).second` detects an existing element.

**Recommended interview solution:** HashSet.

**Pattern:** Arrays + Hashing.

**Remember:** Use the simplest data structure that solves the actual requirement. We don't need frequency when existence checking is sufficient.

---

**Status:** Completed  
**Next Topic:** Arrays & Hashing – Valid Anagram

# NeetCode 150 – 02. Valid Anagram

**Category:** Arrays & Hashing  
**Pattern:** Frequency Counting / Hashing  
**Language:** C++

## Problem

Given two strings `s` and `t`, return true if `t` is an anagram of `s`, otherwise return false.

Two strings are anagrams if they contain the same characters with the same frequencies.

### Examples

```cpp
Input: s = "anagram", t = "nagaram"
Output: true

Input: s = "rat", t = "car"
Output: false
```

---

## Approach 1: Sorting

### Logic

1. Check whether both strings have equal lengths.
2. Sort both strings.
3. Compare the sorted strings.
4. If equal, they are anagrams.

### Code

```cpp
class Solution {
public:
    bool isAnagram(string s, string t) {

        if(s.size() != t.size()) {
            return false;
        }

        sort(s.begin(), s.end());
        sort(t.begin(), t.end());

        return s == t;
    }
};
```

### Complexity

- Time: O(n log n)
- Auxiliary space: Depends on sorting implementation.

**Important C++ concept:**

`sort()` modifies the original container. It does not return a sorted string or vector.

---

## Approach 2: Frequency Array (Optimal)

### Logic

Since the problem contains lowercase English letters, we can use an array of size 26.

- Increment the count for each character in `s`.
- Decrement the count for each character in `t`.
- If all counts become zero, both strings are anagrams.

### Code

```cpp
class Solution {
public:
    bool isAnagram(string s, string t) {

        if(s.size() != t.size()) {
            return false;
        }

        vector<int> freq(26, 0);

        for(int i = 0; i < s.size(); i++) {

            freq[s[i] - 'a']++;
            freq[t[i] - 'a']--;
        }

        for(int x : freq) {
            if(x != 0) {
                return false;
            }
        }

        return true;
    }
};
```

### Understanding Character Indexing

```cpp
freq[s[i] - 'a']++;
```

Character-to-index mapping:

- a → 0
- b → 1
- c → 2
- ...
- z → 25

This allows us to store character frequencies in a fixed-size array.

### Complexity

- Time: O(n)
- Space: O(1), fixed array of 26 elements.

---

## Approach 3: Unordered Map

### Logic

Use an unordered_map to store character frequencies.

1. Increment frequency for every character in `s`.
2. Decrement frequency for every character in `t`.
3. Check whether all frequencies are zero.

### Code

```cpp
class Solution {
public:
    bool isAnagram(string s, string t) {

        if(s.size() != t.size()) {
            return false;
        }

        unordered_map<char, int> mp;

        for(char c : s) {
            mp[c]++;
        }

        for(char c : t) {
            mp[c]--;
        }

        for(auto &p : mp) {
            if(p.second != 0) {
                return false;
            }
        }

        return true;
    }
};
```

### Complexity

- Time: O(n) average
- Space: O(k), where k is the number of distinct characters.

---

## Approach Comparison

| Approach | Time | Space |
|---|---|---|
| Sorting | O(n log n) | Depends on sort |
| Frequency Array | O(n) | O(1) |
| Unordered Map | O(n) average | O(k) |

---

## Important C++ STL Concepts

### 1. sort()

```cpp
sort(s.begin(), s.end());
```

Sorts the string in ascending order.

### 2. vector<int> freq(26, 0)

Creates an integer array of 26 elements, all initialized to zero.

### 3. unordered_map<char, int>

Stores character-frequency pairs.

### 4. Range-based loop

```cpp
for(int x : freq)
```

Iterates over every element in the frequency array.

---

## Interview Pattern Recognition

When a problem asks:

- Are two strings anagrams?
- Do two strings have identical character frequencies?
- How many times does each character occur?

Think about **Frequency Counting**.

Possible tools:
- Fixed-size array
- HashMap
- Sorting

### 30-Second Revision

- Sorting: Sort both strings and compare.
- Frequency Array: Increment and decrement character counts.
- HashMap: Maintain character frequencies.
- Optimal for lowercase English letters: Frequency Array.
- Length check: Different lengths mean they cannot be anagrams.

**Important:** Frequency array works with lowercase English letters under this problem's constraints. For arbitrary Unicode text, a more suitable character-counting approach is needed.

**Status:** Completed  
**Next Problem:** Two Sum

# NeetCode 150 – 03. Two Sum

**Category:** Arrays & Hashing  
**Pattern:** HashMap / Complement Technique  
**Language:** C++

## Problem

Given an array of integers `nums` and an integer `target`, return the indices of two numbers whose sum equals the target.

Each input has exactly one solution, and we cannot use the same element twice.

### Example

```cpp
Input: nums = {2, 7, 11, 15}
target = 9

Output: {0, 1}
```

---

## Approach 1: Brute Force – Nested Loops

### Logic

- Pick one element using index `i`.
- Pick another element using index `j`.
- Check whether their sum equals the target.
- If yes, return both indices.

### Code

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {

        for(int i = 0; i < nums.size(); i++) {

            for(int j = i + 1; j < nums.size(); j++) {

                if(nums[i] + nums[j] == target) {
                    return {i, j};
                }
            }
        }

        return {};
    }
};
```

### Complexity

- Time: O(n²)
- Auxiliary space: O(1).

### Limitation

For every element, we potentially check all remaining elements.

This becomes slow for large arrays.

---

## Approach 2: HashMap – Complement Technique (Optimal)

### Logic

Instead of checking every pair, calculate the missing number.

Formula:

`complement = target - current_number`

Check whether the complement already exists in the HashMap.

If it exists, return its stored index and the current index.

Otherwise, store the current number and its index.

### Code

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {

        unordered_map<int, int> mp;

        for(int i = 0; i < nums.size(); i++) {

            int complement = target - nums[i];

            if(mp.find(complement) != mp.end()) {
                return {mp[complement], i};
            }

            mp[nums[i]] = i;
        }

        return {};
    }
};
```

### Dry Run

Input:

`nums = {2, 7, 11, 15}`

`target = 9`

| Current number | Complement | Map before lookup | Action |
|---|---|---|---|
| 2 | 7 | {} | Store 2 at index 0 |
| 7 | 2 | {2:0} | Found complement |

Output: `{0, 1}`

### Complexity

- Time: O(n) average.
- Space: O(n).

### Why HashMap?

It provides fast average lookup and allows us to store both the number and its original index.

**Important:** Check the complement before inserting the current element. This prevents using the same element twice.

---

## Approach 3: Sorting + Two Pointers

### Logic

1. Store every number with its original index.
2. Sort the pairs by number.
3. Place one pointer at the beginning.
4. Place another pointer at the end.
5. Compare their sum with the target.
6. Move pointers according to the sum.
7. Return original indices when the target is found.

### Code

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {

        vector<pair<int, int>> arr;

        for(int i = 0; i < nums.size(); i++) {
            arr.push_back({nums[i], i});
        }

        sort(arr.begin(), arr.end());

        int left = 0;
        int right = arr.size() - 1;

        while(left < right) {

            int sum = arr[left].first + arr[right].first;

            if(sum == target) {
                return {arr[left].second, arr[right].second};
            }
            else if(sum < target) {
                left++;
            }
            else {
                right--;
            }
        }

        return {};
    }
};
```

### Complexity

- Time: O(n log n).
- Space: O(n) for the additional pair array.

### Important Observation

Why do we store original indices?

Because sorting changes the positions of elements, but the problem requires the original array indices.

---

## Approach Comparison

| Approach | Time | Space |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| HashMap | O(n) average | O(n) |
| Sorting + Two Pointers | O(n log n) | O(n) |

---

## Important C++ Concepts

### 1. unordered_map

```cpp
unordered_map<int, int> mp;
```

Stores number and its original index.

Example:

```cpp
mp[2] = 0;
mp[7] = 1;
```

### 2. find()

```cpp
mp.find(complement) != mp.end()
```

Checks whether a key exists.

### 3. pair<int,int>

```cpp
pair<int, int> p = {10, 2};
```

- `p.first` = 10
- `p.second` = 2

Useful for storing a value and its original index.

---

## Interview Pattern Recognition

When a problem asks:

- Find two numbers whose sum equals a target.
- Find a missing complementary value.
- Find two elements satisfying a mathematical condition.

Think about:

**HashMap + Complement Technique**

Instead of checking all possible pairs, calculate what value you need and search for it.

---

## 30-Second Revision

- Brute Force: Check every possible pair.
- HashMap: Calculate `target - nums[i]` and search for the complement.
- Sorting + Two Pointers: Sort value-index pairs and move pointers.
- Best average time: HashMap O(n).
- Important STL: `unordered_map`, `find()`, `pair`.
- Important rule: Check before inserting to avoid reusing the same element.

**Recommended interview solution:** HashMap.

**Pattern:** Arrays & Hashing → Complement Technique.

**Status:** Completed.

**Next Problem:** Group Anagrams.

# NeetCode 150 – 04. Group Anagrams

**Category:** Arrays & Hashing  
**Pattern:** HashMap + Grouping  
**Language:** C++

## Problem

Given an array of strings, group the anagrams together.

Anagrams are words containing the same characters with the same frequencies.

### Example

```cpp
Input: {"eat", "tea", "tan", "ate", "nat", "bat"}

Output:
{
    {"eat", "tea", "ate"},
    {"tan", "nat"},
    {"bat"}
}
```

The order of groups does not matter.

---

## Approach 1: Sorting + HashMap

### Logic

1. Create an unordered_map where the key is a sorted string and the value is a vector of original strings.
2. Traverse every word.
3. Make a copy of the word.
4. Sort the copy.
5. Use the sorted word as the key.
6. Push the original word into the corresponding group.
7. Return all groups.

### Code

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {

        unordered_map<string, vector<string>> mp;

        for(string &word : strs) {

            string key = word;

            sort(key.begin(), key.end());

            mp[key].push_back(word);
        }

        vector<vector<string>> ans;

        for(auto &it : mp) {
            ans.push_back(it.second);
        }

        return ans;
    }
};
```

### Example

| Word | Sorted key |
|---|---|
| eat | aet |
| tea | aet |
| ate | aet |
| tan | ant |
| nat | ant |
| bat | abt |

HashMap:

```text
aet → [eat, tea, ate]
ant → [tan, nat]
abt → [bat]
```

### Complexity

Let:
- n = Number of strings.
- k = Maximum length of a string.

- Time: O(n × k log k)
- Space: O(n × k) for the stored groups and keys.

### Advantages

- Easy to understand.
- Simple implementation.
- Uses sorting as a common signature.

---

## Approach 2: Character Frequency + HashMap

### Logic

Instead of sorting each word, calculate the frequency of every character.

For lowercase English letters, create an array of size 26.

Words with identical character frequencies belong to the same group.

Example:

```text
eat → a:1, e:1, t:1
tea → a:1, e:1, t:1
ate → a:1, e:1, t:1
```

All three words have the same frequency signature.

Use this signature as the HashMap key.

### Code

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {

        unordered_map<string, vector<string>> mp;

        for(string &word : strs) {

            vector<int> freq(26, 0);

            for(char c : word) {
                freq[c - 'a']++;
            }

            string key = "";

            for(int count : freq) {
                key += to_string(count) + "#";
            }

            mp[key].push_back(word);
        }

        vector<vector<string>> ans;

        for(auto &it : mp) {
            ans.push_back(it.second);
        }

        return ans;
    }
};
```

### Why use `#`?

It separates frequency values and prevents ambiguous key representations.

For example, frequencies `[1, 11]` and `[11, 1]` must produce different keys.

### Complexity

- Time: O(n × k), with a fixed 26-character alphabet.
- Space: O(n × k) for storing groups and keys.

### Advantages

- Avoids sorting every word.
- Uses character frequency as a unique signature.
- Efficient for longer strings.

---

## Approach Comparison

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| Sorting + HashMap | O(n × k log k) | O(n × k) |
| Frequency + HashMap | O(n × k) | O(n × k) |

---

## Important C++ Concepts

### 1. unordered_map<string, vector<string>>

Stores:
- Key: Common signature.
- Value: Group of anagram strings.

### 2. sort()

```cpp
sort(key.begin(), key.end());
```

Sorts characters to generate a common key.

### 3. push_back()

```cpp
mp[key].push_back(word);
```

Adds a word to its corresponding group.

### 4. Automatic insertion

```cpp
mp[key].push_back(word);
```

If the key does not exist, unordered_map creates an empty vector automatically.

### 5. Character indexing

```cpp
freq[c - 'a']++;
```

Maps lowercase characters to indices 0–25.

---

## Interview Pattern Recognition

When a problem asks you to:
- Group anagrams.
- Group items with identical properties.
- Find a common signature.
- Group strings based on character frequency.

Think about:

**HashMap + Common Key / Signature**

Possible keys:
- Sorted string.
- Character frequency representation.

---

## 30-Second Revision

- Sorting approach: Sort every word and use the sorted string as the key.
- Frequency approach: Calculate character frequencies and use them as the key.
- HashMap value: Store a vector of original words.
- Simplification: `mp[key].push_back(word)` handles insertion automatically.
- Best asymptotic approach for lowercase letters: Frequency signature.

**Recommended interview solution:** Both approaches are valid. Explain sorting first because it is intuitive, then frequency counting as the linear-time alternative.

**Pattern:** Arrays & Hashing → HashMap Grouping.

**Status:** Completed.

**Next Problem:** Top K Frequent Elements.

# NeetCode 150 – 05. Top K Frequent Elements

**Category:** Arrays & Hashing  
**Pattern:** Frequency Counting + Sorting / Heap / Bucket Sort  
**Language:** C++

## Problem

Given an integer array `nums` and an integer `k`, return the `k` most frequent elements.

The answer can be returned in any order.

### Example

```cpp
Input: nums = {1,1,1,2,2,3}
k = 2

Output: {1,2}
```

Explanation:

- 1 appears 3 times.
- 2 appears 2 times.
- 3 appears 1 time.

The two most frequent elements are 1 and 2.

---

# Approach 1: HashMap + Sorting

### Logic

1. Create an unordered_map to count the frequency of every number.
2. Store each number and its frequency in a pair.
3. Sort the pairs based on frequency.
4. Take the last k elements because default pair sorting is ascending.
5. Return the numbers.

### Code

```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {

        vector<int> ans;
        vector<pair<int,int>> pair_ele;

        unordered_map<int,int> mp;

        // Step 1: Calculate frequency
        for(auto &x : nums) {
            mp[x]++;
        }

        // Step 2: Store frequency and number
        for(auto &t : mp) {
            pair_ele.push_back({t.second, t.first});
        }

        // Step 3: Sort pairs
        sort(pair_ele.begin(), pair_ele.end());

        // Step 4: Extract top k elements
        for(int i = pair_ele.size()-1;
            i >= 0 && ans.size() < k; i--) {

            ans.push_back(pair_ele[i].second);
        }

        return ans;
    }
};
```

### Understanding Pair Storage

```cpp
pair_ele.push_back({t.second, t.first});
```

We store:

- `first` = Frequency
- `second` = Number

Example:

```text
1 → 3
2 → 2
3 → 1
```

Stored pairs:

```text
{3,1}
{2,2}
{1,3}
```

After sorting:

```text
{1,3}
{2,2}
{3,1}
```

Taking the last two elements gives `{1,2}`.

### Complexity

Let:
- n = Total number of elements.
- m = Number of distinct elements.

- Time: O(n + m log m)
- Space: O(m)

### Advantages

- Easy to understand.
- Uses basic sorting and HashMap concepts.
- Good starting approach.

### Limitation

Sorting all distinct elements is unnecessary when we only need the top k elements.

---

# Approach 2: HashMap + Min Heap

### Logic

A Min Heap helps us maintain only the k most frequent elements.

1. Count frequencies using HashMap.
2. Create a Min Heap storing `{frequency, number}`.
3. Insert every pair into the heap.
4. If heap size exceeds k, remove the smallest frequency.
5. Finally, the heap contains the k most frequent elements.

### Code

```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {

        unordered_map<int,int> mp;

        // Step 1: Frequency counting
        for(int x : nums) {
            mp[x]++;
        }

        // Step 2: Min Heap
        priority_queue<
            pair<int,int>,
            vector<pair<int,int>>,
            greater<pair<int,int>>
        > pq;

        // Step 3: Maintain only k elements
        for(auto &it : mp) {

            pq.push({it.second, it.first});

            if(pq.size() > k) {
                pq.pop();
            }
        }

        // Step 4: Extract answer
        vector<int> ans;

        while(!pq.empty()) {
            ans.push_back(pq.top().second);
            pq.pop();
        }

        return ans;
    }
};
```

### Understanding Min Heap

A Min Heap keeps the smallest frequency at the top.

Example:

```text
Frequency pairs:

{3,1}
{2,2}
{1,3}
```

If k = 2:

- Insert `{3,1}`.
- Insert `{2,2}`.
- Insert `{1,3}`.
- Heap size becomes 3.
- Remove `{1,3}` because it has the smallest frequency.

Remaining elements:

```text
{2,2}
{3,1}
```

Answer: `{2,1}`

The output order does not matter.

### Important C++ STL Concept

```cpp
priority_queue<int> pq;
```

Creates a Max Heap by default.

```cpp
priority_queue<int, vector<int>, greater<int>> pq;
```

Creates a Min Heap.

For this problem:

```cpp
priority_queue<
    pair<int,int>,
    vector<pair<int,int>>,
    greater<pair<int,int>>
> pq;
```

Stores pairs ordered by their first value (frequency), then their second value.

### Complexity

- Time: O(n + m log k)
- Space: O(m + k), including the HashMap and heap.

### Advantages

- Does not sort every distinct element.
- Efficient when k is much smaller than the number of distinct elements.
- Useful for many Top K interview problems.

---

# Approach 3: HashMap + Bucket Sort

### Logic

This is the linear-time approach.

The important observation:

**An element cannot appear more than n times.**

Therefore, we can create buckets indexed by frequency.

1. Count frequencies using HashMap.
2. Create a vector of buckets of size n + 1.
3. Place each number inside the bucket corresponding to its frequency.
4. Traverse buckets from highest frequency to lowest.
5. Collect elements until we have k elements.

### Code

```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {

        unordered_map<int,int> mp;

        // Step 1: Frequency counting
        for(int x : nums) {
            mp[x]++;
        }

        // Step 2: Create frequency buckets
        vector<vector<int>> bucket(nums.size() + 1);

        // Step 3: Place numbers into buckets
        for(auto &it : mp) {

            int frequency = it.second;
            int number = it.first;

            bucket[frequency].push_back(number);
        }

        // Step 4: Extract top k frequent elements
        vector<int> ans;

        for(int i = bucket.size() - 1; i >= 1; i--) {

            for(int x : bucket[i]) {

                ans.push_back(x);

                if(ans.size() == k) {
                    return ans;
                }
            }
        }

        return ans;
    }
};
```

### Dry Run

Input:

```cpp
nums = {1,1,1,2,2,3}
k = 2
```

Frequency map:

```text
1 → 3
2 → 2
3 → 1
```

Bucket representation:

```text
Frequency 1 → [3]
Frequency 2 → [2]
Frequency 3 → [1]
Frequency 4 → []
Frequency 5 → []
Frequency 6 → []
```

Traverse from highest frequency to lowest:

```text
Bucket 3 → [1] → Add 1
Bucket 2 → [2] → Add 2
```

We have collected k = 2 elements.

Output:

```cpp
{1,2}
```

### Complexity

- Time: O(n) average.
- Space: O(n).

### Advantages

- Avoids sorting.
- Avoids heap operations.
- Efficient linear-time solution.
- Excellent interview approach for this problem.

---

# Final Approach Comparison

Let m be the number of distinct elements.

| Approach | Time Complexity | Space Complexity |
|---|---|---|
| HashMap + Sorting | O(n + m log m) | O(m) |
| HashMap + Min Heap | O(n + m log k) | O(m + k) |
| HashMap + Bucket Sort | O(n) average | O(n) |

---

# Important C++ STL Concepts

## 1. unordered_map

```cpp
unordered_map<int,int> mp;
mp[x]++;
```

Used for frequency counting.

## 2. pair<int,int>

```cpp
pair<int,int> p = {3,1};
```

- first = 3
- second = 1

## 3. priority_queue

```cpp
priority_queue<int> pq;
```

Default Max Heap.

Min Heap:

```cpp
priority_queue<int, vector<int>, greater<int>> pq;
```

## 4. sort()

```cpp
sort(arr.begin(), arr.end());
```

Sorts elements in ascending order by default.

For pairs, it first compares `first`, then `second`.

## 5. vector<vector<int>>

```cpp
vector<vector<int>> bucket(n + 1);
```

Useful for bucket-based grouping.

---

# Interview Pattern Recognition

When a problem asks:

- Find the top K frequent elements.
- Find the K largest or smallest elements.
- Find the most common elements.
- Maintain the top K elements efficiently.

Think about:

**HashMap + Heap / Bucket Sort**

Choose the approach based on requirements:

- Sorting: When simplicity is preferred.
- Min Heap: When k is small compared with the number of distinct elements.
- Bucket Sort: When frequencies are bounded and linear time is desirable.

---

# 30-Second Revision

**Top K Frequent Elements**

1. HashMap: Count occurrences.
2. Sorting: Sort frequency-number pairs and take the largest k.
3. Min Heap: Maintain only k elements by removing the smallest frequency.
4. Bucket Sort: Store numbers according to their frequency.
5. Best linear-time approach: Bucket Sort.
6. Important STL: `unordered_map`, `pair`, `priority_queue`, `greater<>`.

**Recommended interview preparation:** Understand all three approaches. Practise Min Heap and Bucket Sort independently.

# NeetCode 150 – 06. Encode and Decode Strings

**Category:** Arrays & Hashing  
**Pattern:** String Manipulation / Serialization / Two Pointers  
**Language:** C++

## Problem

Design an algorithm to encode a list of strings into a single string and decode that single string back into the original list.

The decoded output must exactly match the original input.

### Example

```cpp
Input: {"neet", "code", "love", "you"}

Encoded:
"4#neet4#code4#love3#you"

Decoded:
{"neet", "code", "love", "you"}
```

Important: Strings may contain special characters, including `#`.

---

# Approach 1: Length Prefix + Delimiter (Recommended)

### Logic

Instead of simply joining strings using a delimiter, store the length of each string before the actual string.

Format:

```text
length#string
```

Example:

```text
neet → 4#neet
code → 4#code
love → 4#love
you  → 3#you
```

Combined:

```text
4#neet4#code4#love3#you
```

The decoder first reads the length and then extracts exactly that many characters.

This means even if the original string contains `#`, decoding still works correctly.

## Code

```cpp
class Solution {
public:

    string encode(vector<string>& strs) {

        string ans = "";

        for(string &word : strs) {

            ans += to_string(word.size());
            ans += '#';
            ans += word;
        }

        return ans;
    }

    vector<string> decode(string s) {

        vector<string> ans;

        int i = 0;

        while(i < s.size()) {

            // Find delimiter
            int j = s.find('#', i);

            // Extract length
            int len = stoi(s.substr(i, j - i));

            // Extract actual word
            string word = s.substr(j + 1, len);

            ans.push_back(word);

            // Move to next encoded string
            i = j + 1 + len;
        }

        return ans;
    }
};
```

## Understanding Decode Step by Step

Consider:

```text
s = "4#neet4#code"
```

We use two pointers:

- `i`: Starting position of the length.
- `j`: Position of the delimiter `#`.

### Step 1: Find delimiter

```cpp
int j = s.find('#', i);
```

Initially:

```text
4#neet4#code
 ^
 i
```

`j = 1`

### Step 2: Extract length

```cpp
int len = stoi(s.substr(i, j-i));
```

`len = 4`

### Step 3: Extract word

```cpp
string word = s.substr(j+1, len);
```

Extracted word:

```text
neet
```

### Step 4: Move to next string

```cpp
i = j + 1 + len;
```

Now:

```text
4#neet4#code
      ^
      i
```

The next iteration decodes `code`.

### Dry Run

| Iteration | i | j | Length | Word |
|---|---:|---:|---:|---|
| 1 | 0 | 1 | 4 | neet |
| 2 | 6 | 7 | 4 | code |

Output:

```cpp
{"neet", "code"}
```

## Complexity

Let N be the total number of characters in all input strings.

| Function | Time | Auxiliary Space |
|---|---|---|
| encode | O(N) | O(N) output |
| decode | O(N) | O(N) output |

---

# Approach 2: Length Prefix + Manual Number Parsing

This approach uses the same encoding format but avoids `stoi()` and `substr()` for extracting the length.

It is useful for understanding character-by-character parsing.

### Logic

1. Find the length digits manually.
2. Convert those digits into an integer.
3. Skip the delimiter.
4. Extract the required number of characters.
5. Continue until the encoded string ends.

### Code

```cpp
class Solution {
public:

    string encode(vector<string>& strs) {

        string ans = "";

        for(string &word : strs) {

            ans += to_string(word.size());
            ans += '#';
            ans += word;
        }

        return ans;
    }

    vector<string> decode(string s) {

        vector<string> ans;

        int i = 0;

        while(i < s.size()) {

            int len = 0;

            // Read the length digits
            while(s[i] != '#') {

                len = len * 10 + (s[i] - '0');

                i++;
            }

            // Skip '#'
            i++;

            // Extract exactly len characters
            string word = "";

            for(int j = 0; j < len; j++) {

                word += s[i];
                i++;
            }

            ans.push_back(word);
        }

        return ans;
    }
};
```

### Understanding Number Conversion

Suppose the encoded string starts with:

```text
12#HelloWorld!!
```

The manual conversion works like this:

```cpp
len = len * 10 + (s[i] - '0');
```

- First digit: `1` → len = 1
- Second digit: `2` → len = 12

Now we know that the next 12 characters belong to the string.

### Complexity

- Encoding: O(N)
- Decoding: O(N)
- Auxiliary space: O(N) for decoded output.

**Advantage:** Demonstrates how integer parsing works internally without relying on `stoi()`.

---

# Approach 3: Delimiter-Only Encoding (Why It Fails)

A common initial idea is to join strings using a separator such as `#`.

### Example

```cpp
{"neet", "code", "love"}
```

Encoding:

```text
neet#code#love
```

Decoding:

Split the string whenever `#` appears.

### Problem

Consider:

```cpp
{"neet#code", "love"}
```

Encoded result:

```text
neet#code#love
```

Now the decoder cannot determine whether the first string was:

```text
neet#code
```

or whether the input contained three separate strings:

```text
neet, code, love
```

Information is lost.

### Conclusion

Delimiter-only encoding is not reliable when the original strings can contain the delimiter.

**Lesson:** Always design an encoding format that can be decoded unambiguously.

---

# Approach Comparison

| Approach | Reliable? | Main concept |
|---|---|---|
| Length Prefix + Delimiter | Yes | Store length before string |
| Manual Length Parsing | Yes | Parse digits manually |
| Delimiter Only | No | Ambiguous when delimiter occurs in data |

---

# Important C++ Concepts

## 1. to_string()

Converts an integer into a string.

```cpp
int n = 123;

string s = to_string(n);
```

Result: `"123"`

## 2. stoi()

Converts a numeric string into an integer.

```cpp
string s = "123";

int n = stoi(s);
```

Result: `123`

## 3. find()

Finds the position of a character or substring.

```cpp
int j = s.find('#', i);
```

Searches for `#` starting from index `i`.

## 4. substr()

Extracts a substring.

```cpp
string word = s.substr(2, 4);
```

Arguments:
- First: Starting index.
- Second: Number of characters.

## 5. Character to Integer Conversion

```cpp
int digit = s[i] - '0';
```

Converts a numeric character into its integer value.

Example:

`'5' - '0' = 5`

---

# Important Interview Learning

### Why not use a class-level vector to store original strings?

An initial implementation might store the original strings during encoding and return them directly during decoding.

However, this is not actual decoding.

The decoder must reconstruct the original strings using only the encoded string.

The implementation should work independently, without depending on data saved during a previous function call.

### Why use length prefix?

Because the decoder knows exactly how many characters belong to each string.

It does not matter if the original string contains:
- `#`
- Numbers
- Spaces
- Special characters

The length determines where each string ends.

---

# Interview Pattern Recognition

When a problem asks you to:

- Convert multiple strings into one string.
- Reconstruct original data.
- Serialize and deserialize data.
- Handle special characters without losing information.

Think about:

**Serialization + Length Prefix Encoding**

---

# 30-Second Revision

1. Encoding: `length + delimiter + original string`.
2. Decoding: Read length, skip delimiter, extract exact characters.
3. `to_string()` converts integer to string.
4. `stoi()` converts string to integer.
5. `find()` locates delimiter.
6. `substr()` extracts characters.
7. Never depend on previously stored original data during decoding.
8. Delimiter-only splitting can lose information.

**Recommended interview solution:** Length Prefix + Delimiter.

**Pattern:** Arrays & Hashing → Serialization / String Parsing.

**Status:** Completed.

**Next Problem:** Product of Array Except Self.

# NeetCode 150 – 07. Product of Array Except Self

**Category:** Arrays & Hashing  
**Pattern:** Prefix Product + Suffix Product  
**Language:** C++

## Problem

Given an integer array `nums`, return an array `answer` such that:

`answer[i]` is equal to the product of all elements of `nums` except `nums[i]`.

**Important constraints:**
- Do not use division.
- Solve in O(n) time.
- Use O(1) auxiliary space (output array does not count).

### Example

```cpp
Input: nums = {1, 2, 3, 4}

Output: {24, 12, 8, 6}
```

Explanation:

- Index 0: 2 × 3 × 4 = 24
- Index 1: 1 × 3 × 4 = 12
- Index 2: 1 × 2 × 4 = 8
- Index 3: 1 × 2 × 3 = 6

---

# Approach 1: Brute Force – Nested Loops

## Logic

For every index `i`:
1. Initialize product = 1.
2. Traverse the complete array.
3. Skip the current index.
4. Multiply all other elements.
5. Store the result.

## Code

```cpp
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {

        int n = nums.size();
        vector<int> ans(n, 1);

        for(int i = 0; i < n; i++) {

            for(int j = 0; j < n; j++) {

                if(i != j) {
                    ans[i] *= nums[j];
                }
            }
        }

        return ans;
    }
};
```

## Complexity

- Time: O(n²)
- Auxiliary space: O(1), excluding output.

## Limitation

For every element, we traverse the entire array again.

This becomes inefficient for large inputs.

---

# Approach 2: Using Division

## Logic

Instead of calculating the product repeatedly:

1. Calculate the product of all non-zero elements.
2. Count the number of zeros.
3. Handle zero cases separately.
4. If there are no zeros, divide the total product by each element.
5. If there is exactly one zero, only the zero's position gets the product of all non-zero elements.
6. If there are two or more zeros, every answer is zero.

## Code

```cpp
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {

        int product = 1;
        int zeroCount = 0;

        for(int x : nums) {

            if(x == 0) {
                zeroCount++;
            }
            else {
                product *= x;
            }
        }

        vector<int> ans(nums.size(), 0);

        if(zeroCount > 1) {
            return ans;
        }

        for(int i = 0; i < nums.size(); i++) {

            if(zeroCount == 1) {

                if(nums[i] == 0) {
                    ans[i] = product;
                }
            }
            else {
                ans[i] = product / nums[i];
            }
        }

        return ans;
    }
};
```

## Complexity

- Time: O(n)
- Auxiliary space: O(1), excluding output.

## Limitation

- Division is not allowed in the original problem.
- Integer overflow can occur while calculating the total product.

**Interview note:** This is a useful mathematical alternative, but it is not a valid submission for the original NeetCode constraints.

---

# Approach 3: Prefix Array + Suffix Array

## Logic

Instead of multiplying every element repeatedly, calculate products from both directions.

### Prefix Array

`prefix[i]` stores the product of all elements to the left of index `i`.

Example:

```text
nums   = [1, 2, 3, 4]
prefix = [1, 1, 2, 6]
```

### Suffix Array

`suffix[i]` stores the product of all elements to the right of index `i`.

```text
nums   = [1, 2, 3, 4]
suffix = [24, 12, 4, 1]
```

### Final Formula

```cpp
ans[i] = prefix[i] * suffix[i];
```

Final output:

```text
[24, 12, 8, 6]
```

## Code

```cpp
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {

        int n = nums.size();

        vector<int> prefix(n, 1);
        vector<int> suffix(n, 1);
        vector<int> ans(n);

        for(int i = 1; i < n; i++) {
            prefix[i] = prefix[i-1] * nums[i-1];
        }

        for(int i = n-2; i >= 0; i--) {
            suffix[i] = suffix[i+1] * nums[i+1];
        }

        for(int i = 0; i < n; i++) {
            ans[i] = prefix[i] * suffix[i];
        }

        return ans;
    }
};
```

## Complexity

- Time: O(n)
- Auxiliary space: O(n), for prefix and suffix arrays.

## Advantage

Easy to understand and demonstrates the relationship between prefix and suffix products.

---

# Approach 4: Prefix + Suffix Variables (Optimal)

<badge color="success">Recommended interview solution</badge>

This is the approach used in my accepted implementation.

## Logic

Instead of maintaining separate prefix and suffix arrays, use two variables:

- `prefix` stores the product of elements to the left.
- `suffix` stores the product of elements to the right.

### First Loop – Prefix Product

```cpp
int prefix = 1;

for(int i = 0; i < n; i++) {

    output[i] = prefix;

    prefix *= nums[i];
}
```

This stores the product of all elements to the left of each index.

For:

```text
nums = [1, 2, 3, 4]
```

After the first loop:

```text
output = [1, 1, 2, 6]
```

### Second Loop – Suffix Product

```cpp
int suffix = 1;

for(int i = n-1; i >= 0; i--) {

    output[i] *= suffix;

    suffix *= nums[i];
}
```

This multiplies each prefix product by the corresponding suffix product.

Final output:

```text
[24, 12, 8, 6]
```

## Complete Code

```cpp
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {

        int n = nums.size();

        vector<int> output(n, 1);

        // Calculate prefix products
        int prefix = 1;

        for(int i = 0; i < n; i++) {

            output[i] = prefix;

            prefix *= nums[i];
        }

        // Multiply by suffix products
        int suffix = 1;

        for(int i = n - 1; i >= 0; i--) {

            output[i] *= suffix;

            suffix *= nums[i];
        }

        return output;
    }
};
```

## Dry Run

Input:

```text
nums = [1, 2, 3, 4]
```

### First loop

| Index | Current number | Prefix stored | Updated prefix |
|---|---:|---:|---:|
| 0 | 1 | 1 | 1 |
| 1 | 2 | 1 | 2 |
| 2 | 3 | 2 | 6 |
| 3 | 4 | 6 | 24 |

After prefix calculation:

```text
output = [1, 1, 2, 6]
```

### Second loop

| Index | Prefix value | Suffix value | Final output |
|---|---:|---:|---:|
| 3 | 6 | 1 | 6 |
| 2 | 2 | 4 | 8 |
| 1 | 1 | 12 | 12 |
| 0 | 1 | 24 | 24 |

Final answer:

```text
[24, 12, 8, 6]
```

## Complexity

- Time: O(n) – two traversals.
- Auxiliary space: O(1), excluding the output array.

## Why is this optimal?

- No division.
- No additional prefix array.
- No additional suffix array.
- Handles zero values naturally.
- Linear time complexity.

---

# Important Edge Cases

### 1. Array contains zero

```text
Input: [1, 2, 0, 4]

Output: [0, 0, 8, 0]
```

The prefix/suffix approach handles this automatically.

### 2. Array contains two zeros

```text
Input: [0, 2, 0, 4]

Output: [0, 0, 0, 0]
```

### 3. Negative numbers

```text
Input: [-1, 1, 0, -3, 3]

Output: [0, 0, 9, 0, 0]
```

### 4. Single element

```text
Input: [5]

Output: [1]
```

There are no other elements to multiply, so the product is 1.

---

# Approach Comparison

| Approach | Time | Auxiliary Space |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Division | O(n) | O(1) |
| Prefix + Suffix Arrays | O(n) | O(n) |
| Prefix + Suffix Variables | O(n) | O(1) |

---

# Important Interview Questions

**Q1. Why initialize prefix and suffix with 1?**

Because 1 is the multiplicative identity. Multiplying by 1 does not change the product.

**Q2. Why do we traverse backwards in the second loop?**

Because we need the product of elements to the right of the current index.

**Q3. How does the approach handle zeros?**

If any element is zero, the corresponding products naturally become zero where required. No special zero handling is necessary.

**Q4. Why is auxiliary space O(1)?**

We only use two integer variables, `prefix` and `suffix`, apart from the output array.

**Q5. Why can't we use division?**

The original problem explicitly prohibits division.

---

# Interview Pattern Recognition

When a problem asks you to:

- Calculate products excluding the current element.
- Find products of elements on the left or right.
- Calculate cumulative products.
- Avoid repeatedly traversing an array.

Think about:

**Prefix + Suffix Pattern**

Other problems that use related concepts include:
- Running Sum of Array
- Range Sum Queries
- Product of Array Except Self
- Prefix Sum problems

---

# 30-Second Revision

1. Brute Force: Multiply every element

**Pattern:** Arrays & Hashing → Frequency Counting + Top K.

**Status:** Completed.

**Next Problem:** Product of Array Except Self.

