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

