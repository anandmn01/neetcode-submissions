# NeetCode 150 – Valid Palindrome

**Category:** Two Pointers  
**Pattern:** String Cleaning + Two Pointers  
**Language:** C++

## Problem

Given a string `s`, determine whether it is a palindrome after:

1. Converting all uppercase letters into lowercase letters.
2. Removing all non-alphanumeric characters.

An alphanumeric character includes:

- Letters: `a-z`, `A-Z`
- Digits: `0-9`

Return `true` if the cleaned string is a palindrome; otherwise, return `false`.

### Examples

**Example 1**

```cpp
Input: s = "A man, a plan, a canal: Panama"
Output: true
```

Cleaned string:

```text
amanaplanacanalpanama
```

**Example 2**

```cpp
Input: s = "race a car"
Output: false
```

Cleaned string:

```text
raceacar
```

**Example 3**

```cpp
Input: s = "0P"
Output: false
```

Cleaned string:

```text
0p
```

---

# Approach 1: Clean the String + Two Pointers

## Logic

This is the approach implemented in my solution.

First, create a helper function that:

1. Traverses the original string.
2. Keeps lowercase letters.
3. Converts uppercase letters into lowercase.
4. Keeps numeric characters.
5. Ignores spaces and special characters.

After cleaning, use two pointers:

- `i` starts from the beginning.
- `j` starts from the end.
- Compare both characters.
- If they differ, return `false`.
- Otherwise, move both pointers towards the centre.

If all comparisons match, return `true`.

## C++ Code

```cpp
class Solution {
public:

    string palindromeStr(string &s) {

        string temp = "";

        for(int i = 0; i < s.size(); i++) {

            if(s[i] >= 'a' && s[i] <= 'z') {
                temp += s[i];
            }
            else if(s[i] >= 'A' && s[i] <= 'Z') {
                temp += tolower(s[i]);
            }
            else if(s[i] >= '0' && s[i] <= '9') {
                temp += s[i];
            }
        }

        return temp;
    }

    bool isPalindrome(string s) {

        string str = palindromeStr(s);

        int i = 0;
        int j = str.size() - 1;

        while(i < j) {

            if(str[i] != str[j]) {
                return false;
            }

            i++;
            j--;
        }

        return true;
    }
};
```

## Important Correction: Digit Condition

My original code contained:

```cpp
s[i] >= '1' && s[i] <= 9
```

This was incorrect because:

- `'1'` is a character.
- `9` is an integer.
- It also excludes the digit `'0'`.

Correct condition:

```cpp
s[i] >= '0' && s[i] <= '9'
```

**Remember:** Use character literals when comparing numeric characters.

---

# Dry Run: Input "0P"

Original string:

```text
0P
```

### Step 1: Clean the string

| Character | Action |
|---|---|
| `0` | Keep |
| `P` | Convert to `p` |

Cleaned string:

```text
0p
```

### Step 2: Two-pointer comparison

```text
i = 0       j = 1

     0   p
     ↑   ↑
     i   j
```

Comparison:

```cpp
str[i] != str[j]
'0' != 'p'
```

Condition is true, so return:

```cpp
false
```

---

# Approach 2: Using isalnum() and tolower()

C++ provides built-in character functions that simplify string cleaning.

### Important Functions

**isalnum():**

Checks whether a character is an alphabet or digit.

**tolower():**

Converts an uppercase character into lowercase.

## Code

```cpp
class Solution {
public:
    bool isPalindrome(string s) {

        string temp = "";

        for(char c : s) {

            if(isalnum(static_cast<unsigned char>(c))) {
                temp += tolower(static_cast<unsigned char>(c));
            }
        }

        int i = 0;
        int j = temp.size() - 1;

        while(i < j) {

            if(temp[i] != temp[j]) {
                return false;
            }

            i++;
            j--;
        }

        return true;
    }
};
```

**Why use `static_cast<unsigned char>`?**

The character functions from `<cctype>` should receive either EOF or a value representable as `unsigned char`. This avoids undefined behavior for negative `char` values on implementations where `char` is signed.

---

# Approach 3: Two Pointers Without Creating a New String

This is a more space-efficient approach.

Instead of creating a cleaned string, directly move the two pointers over the original string.

### Logic

1. Start `i` at index 0.
2. Start `j` at the last index.
3. Skip non-alphanumeric characters from both sides.
4. Convert both characters to lowercase.
5. Compare them.
6. If they differ, return false.
7. Otherwise, move both pointers inward.

## Code

```cpp
class Solution {
public:
    bool isPalindrome(string s) {

        int i = 0;
        int j = s.size() - 1;

        while(i < j) {

            while(i < j &&
                  !isalnum(static_cast<unsigned char>(s[i]))) {
                i++;
            }

            while(i < j &&
                  !isalnum(static_cast<unsigned char>(s[j]))) {
                j--;
            }

            if(tolower(static_cast<unsigned char>(s[i])) !=
               tolower(static_cast<unsigned char>(s[j]))) {
                return false;
            }

            i++;
            j--;
        }

        return true;
    }
};
```

### Complexity

- Time: O(n)
- Auxiliary space: O(1)

This avoids creating another string.

---

# Approach Comparison

| Approach | Time | Auxiliary Space |
|---|---|---|
| Clean + Two Pointers | O(n) | O(n) |
| isalnum() + Cleaned String | O(n) | O(n) |
| Two Pointers Without Cleaning | O(n) | O(1) |

**Recommended:** Understand Approach 1 first, then practise Approach 3 because it achieves O(1) auxiliary space.

---

# Important C++ Concepts

## 1. tolower()

```cpp
tolower('P')
```

Returns:

```text
p
```

## 2. isalnum()

```cpp
isalnum('A')  // true
isalnum('5')  // true
isalnum('@')  // false
isalnum(' ')  // false
```

## 3. Character Range Checking

```cpp
s[i] >= 'a' && s[i] <= 'z'
s[i] >= 'A' && s[i] <= 'Z'
s[i] >= '0' && s[i] <= '9'
```

## 4. Two Pointers

```cpp
int i = 0;
int j = str.size() - 1;

while(i < j) {
    // Compare characters
    i++;
    j--;
}
```

This is a common technique for palindrome and symmetric comparison problems.

---

# Edge Cases

| Input | Output | Reason |
|---|---|---|
| `"A man, a plan, a canal: Panama"` | true | Cleaned string is a palindrome |
| `"race a car"` | false | Characters do not match |
| `"0P"` | false | `0` and `p` differ |
| `""` | true | Empty string |
| `" "` | true | Cleaned string is empty |
| `"a"` | true | Single character |
| `".,,"` | true | No alphanumeric characters |
| `"1a"` | false | Different characters |

---

# Interview Pattern Recognition

When a problem asks you to:

- Check whether a string is a palindrome.
- Ignore spaces and punctuation.
- Compare characters from both ends.
- Avoid creating an extra string.

Think about:

**Two Pointers + Character Validation**

---

# 30-Second Revision

1. Ignore non-alphanumeric characters.
2. Convert uppercase letters into lowercase.
3. Place one pointer at the beginning and another at the end.
4. Compare both characters.
5. If they differ, return false.
6. Move both pointers inward.
7. If all comparisons match, return true.

**Recommended approach:** Two pointers without cleaning.

**Time:** O(n)  
**Auxiliary Space:** O(1)


# NeetCode 150 – Two Sum II: Input Array Is Sorted

**Category:** Two Pointers  
**Pattern:** Opposite Direction Two Pointers  
**Language:** C++

## Problem

Given a **1-indexed sorted array** of integers `numbers` and an integer `target`, find two numbers whose sum equals the target.

Return their indices as a 1-indexed array.

### Important Conditions

- The array is already sorted in non-decreasing order.
- Exactly one valid solution exists.
- You cannot use the same element twice.
- Return 1-based indices, not 0-based indices.

### Example

```cpp
Input: numbers = [2, 7, 11, 15], target = 9

Output: [1, 2]
```

Explanation:

```text
numbers[1] + numbers[2] = 2 + 7 = 9
```

---

# Approach 1: Brute Force

## Logic

Use two nested loops:

1. Select one element.
2. Check every element after it.
3. If their sum equals the target, return their 1-based indices.

## Code

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {

        for(int i = 0; i < numbers.size(); i++) {

            for(int j = i + 1; j < numbers.size(); j++) {

                if(numbers[i] + numbers[j] == target) {
                    return {i + 1, j + 1};
                }
            }
        }

        return {};
    }
};
```

## Complexity

- Time: O(n²)
- Auxiliary Space: O(1)

**Limitation:** We unnecessarily check many pairs.

---

# Approach 2: HashMap

## Logic

Use a HashMap to remember previously visited numbers and their indices.

For each number:

1. Calculate its complement: `target - numbers[i]`.
2. Check whether the complement exists in the HashMap.
3. If found, return the stored index and current index.
4. Otherwise, store the current number and its index.

## Code

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {

        unordered_map<int, int> mp;

        for(int i = 0; i < numbers.size(); i++) {

            int complement = target - numbers[i];

            if(mp.find(complement) != mp.end()) {
                return {mp[complement] + 1, i + 1};
            }

            mp[numbers[i]] = i;
        }

        return {};
    }
};
```

## Dry Run

Input:

```text
numbers = [2, 7, 11, 15]
target = 9
```

| i | Current number | Complement | HashMap | Action |
|---|---:|---:|---|---|
| 0 | 2 | 7 | Empty | Store 2 → 0 |
| 1 | 7 | 2 | 2 → 0 | Found complement |

Return:

```cpp
{1, 2}
```

## Complexity

- Time: O(n) average.
- Auxiliary Space: O(n).

**Limitation:** The array is already sorted, so we can solve this without an additional HashMap.

---

# Approach 3: Two Pointers – Optimal

<badge color="success">Recommended interview approach</badge>

## Core Logic

Since the array is sorted, we can use two pointers.

Initialize:

```cpp
int i = 0;
int j = numbers.size() - 1;
```

- `i` points to the first element.
- `j` points to the last element.

Calculate:

```cpp
int sum = numbers[i] + numbers[j];
```

There are three possibilities:

### Case 1: Sum equals target

```cpp
if(sum == target)
```

We found the answer.

Return:

```cpp
return {i + 1, j + 1};
```

### Case 2: Sum is greater than target

```cpp
else if(sum > target)
```

Move the right pointer left:

```cpp
j--;
```

Why?

Because the array is sorted. We need a smaller number to reduce the sum.

### Case 3: Sum is smaller than target

```cpp
else
```

Move the left pointer right:

```cpp
i++;
```

Why?

We need a larger number to increase the sum.

## Code

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {

        int i = 0;
        int j = numbers.size() - 1;

        while(i < j) {

            int sum = numbers[i] + numbers[j];

            if(sum == target) {
                return {i + 1, j + 1};
            }
            else if(sum > target) {
                j--;
            }
            else {
                i++;
            }
        }

        return {};
    }
};
```

---

# Detailed Dry Run

Input:

```cpp
numbers = [2, 7, 11, 15]
target = 9
```

Initial pointers:

```text
i = 0
j = 3
```

### Iteration 1

```text
[2, 7, 11, 15]
 ↑           ↑
 i           j
```

```cpp
sum = 2 + 15 = 17
```

17 is greater than 9.

Move right pointer:

```cpp
j--;
```

### Iteration 2

```text
[2, 7, 11, 15]
 ↑     ↑
 i     j
```

```cpp
sum = 2 + 11 = 13
```

13 is greater than 9.

Move right pointer again.

### Iteration 3

```text
[2, 7, 11, 15]
 ↑  ↑
 i  j
```

```cpp
sum = 2 + 7 = 9
```

We found the target.

Return:

```cpp
{1, 2}
```

Remember: The problem asks for 1-based indices, so we return `i + 1` and `j + 1`.

---

# Why Does Two Pointers Work?

The important property is that the array is sorted.

| Condition | Pointer movement | Reason |
|---|---|---|
| `sum > target` | `j--` | Need a smaller sum |
| `sum < target` | `i++` | Need a larger sum |
| `sum == target` | Return | Target found |

Each pointer moves in only one direction.

Therefore, each element is processed at most once.

---

# Approach Comparison

| Approach | Time | Auxiliary Space |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| HashMap | O(n) average | O(n) |
| Two Pointers | O(n) | O(1) |

**Best approach:** Two Pointers.

---

# Important C++ Concepts

## 1. 0-Based vs 1-Based Indexing

C++ vectors use 0-based indexing.

Example:

```text
Index:  0  1  2  3
Value:  2  7 11 15
```

But this problem expects 1-based indices:

```text
Index:  1  2  3  4
Value:  2  7 11 15
```

Therefore:

```cpp
return {i + 1, j + 1};
```

## 2. unordered_map

Stores key-value pairs.

```cpp
unordered_map<int, int> mp;

mp[numbers[i]] = i;
```

Here:
- Key = number.
- Value = original 0-based index.

## 3. find()

```cpp
mp.find(complement) != mp.end()
```

Checks whether the complement exists.

## 4. Opposite Direction Pointers

```cpp
int i = 0;
int j = numbers.size() - 1;
```

One pointer starts from the beginning and the other from the end.

---

# Edge Cases

| Input | Target | Output |
|---|---:|---|
| `[2, 7]` | 9 | `[1, 2]` |
| `[1, 2, 3, 4, 5]` | 9 | `[4, 5]` |
| `[-3, 0, 3, 5]` | 0 | `[1, 3]` |
| `[1, 3, 4, 6, 8]` | 10 | `[2, 5]` |

---

# Interview Pattern Recognition

When you see:

- A sorted array.
- Two numbers that must satisfy a target sum.
- A requirement to return their indices.
- A requirement to avoid extra space.

Think:

**Two Pointers – Opposite Direction**

Ask yourself:

1. Is the array sorted?
2. Can I start from both ends?
3. Can I eliminate one pointer position based on the sum?

If yes, Two Pointers may be the optimal approach.

---

# 30-Second Revision

1. Initialize `i = 0`.
2. Initialize `j = n - 1`.
3. Calculate `sum = numbers[i] + numbers[j]`.
4. If sum equals target, return `{i + 1, j + 1}`.
5. If sum is greater, decrement `j`.
6. If sum is smaller, increment `i`.
7. Continue while `i < j`.

**Recommended approach:** Two Pointers.

**Time:** O(n)  
**Auxiliary Space:** O(1)

**Pattern:** Two Pointers → Sorted Array → Opposite Direction.

**Status:** HashMap approach reviewed; two-pointer approach to be practised independently.

# NeetCode 150 – 3Sum

**Category:** Two Pointers  
**Pattern:** Sorting + Two Pointers  
**Language:** C++

## Problem

Given an integer array `nums`, return all unique triplets:

```cpp
nums[i] + nums[j] + nums[k] == 0
```

The triplets must contain three different indices.

**Important conditions:**

- The sum of all three numbers must be zero.
- You cannot reuse the same element.
- The answer must not contain duplicate triplets.
- The order of triplets in the answer does not matter.

### Example

```cpp
Input: nums = {-1, 0, 1, 2, -1, -4}

Output:
[
    [-1, -1, 2],
    [-1, 0, 1]
]
```

Explanation:

```text
-1 + -1 + 2 = 0
-1 +  0 + 1 = 0
```

---

# Approach 1: Brute Force

## Logic

Use three nested loops to generate every possible triplet.

1. First loop selects the first number.
2. Second loop selects the second number.
3. Third loop selects the third number.
4. Check whether their sum equals zero.
5. Store the triplet if valid.
6. Use a set to avoid duplicate triplets.

## Code

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {

        set<vector<int>> st;

        int n = nums.size();

        for(int i = 0; i < n; i++) {

            for(int j = i + 1; j < n; j++) {

                for(int k = j + 1; k < n; k++) {

                    if(nums[i] + nums[j] + nums[k] == 0) {

                        vector<int> temp = {
                            nums[i], nums[j], nums[k]
                        };

                        sort(temp.begin(), temp.end());

                        st.insert(temp);
                    }
                }
            }
        }

        return vector<vector<int>>(st.begin(), st.end());
    }
};
```

## Complexity

- Time: O(n³ log m) approximately, where `m` is the number of unique triplets inserted, due to set insertion and triplet sorting.
- Space: O(m) for storing unique triplets, excluding the output conversion.

**Limitation:** Three nested loops are too slow for large inputs.

---

# Approach 2: Sorting + Two Pointers

<badge color="success">Recommended interview approach</badge>

## Core Logic

Instead of checking every possible triplet, sort the array first.

After sorting, fix one number and use two pointers to find the other two numbers.

### Step 1: Sort the array

```cpp
sort(nums.begin(), nums.end());
```

Example:

```text
Original: [-1, 0, 1, 2, -1, -4]

Sorted:   [-4, -1, -1, 0, 1, 2]
```

### Step 2: Fix the first number

Use a loop:

```cpp
for(int i = 0; i < n - 2; i++)
```

The first number is:

```cpp
nums[i]
```

We need to find two other numbers whose sum is:

```cpp
target = -nums[i];
```

### Step 3: Initialize two pointers

```cpp
int left = i + 1;
int right = n - 1;
```

Now calculate:

```cpp
int sum = nums[i] + nums[left] + nums[right];
```

There are three possibilities.

**Case 1: Sum equals zero**

```cpp
if(sum == 0)
```

We found a valid triplet.

Store it:

```cpp
ans.push_back({
    nums[i],
    nums[left],
    nums[right]
});
```

Move both pointers:

```cpp
left++;
right--;
```

**Case 2: Sum is smaller than zero**

```cpp
else if(sum < 0)
```

We need a bigger sum.

Move the left pointer forward:

```cpp
left++;
```

**Case 3: Sum is greater than zero**

```cpp
else
```

We need a smaller sum.

Move the right pointer backward:

```cpp
right--;
```

---

# Complete Optimal C++ Code

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {

        vector<vector<int>> ans;

        sort(nums.begin(), nums.end());

        int n = nums.size();

        for(int i = 0; i < n - 2; i++) {

            // Skip duplicate first numbers
            if(i > 0 && nums[i] == nums[i - 1]) {
                continue;
            }

            int left = i + 1;
            int right = n - 1;

            while(left < right) {

                int sum = nums[i] + nums[left] + nums[right];

                if(sum == 0) {

                    ans.push_back({
                        nums[i],
                        nums[left],
                        nums[right]
                    });

                    left++;
                    right--;

                    // Skip duplicate left values
                    while(left < right &&
                          nums[left] == nums[left - 1]) {
                        left++;
                    }

                    // Skip duplicate right values
                    while(left < right &&
                          nums[right] == nums[right + 1]) {
                        right--;
                    }
                }

                else if(sum < 0) {
                    left++;
                }

                else {
                    right--;
                }
            }
        }

        return ans;
    }
};
```

---

# Detailed Dry Run

Input:

```cpp
nums = {-1, 0, 1, 2, -1, -4}
```

After sorting:

```text
[-4, -1, -1, 0, 1, 2]
```

### Iteration 1: i = 0

```text
nums[i] = -4
left = 1
right = 5
```

Current triplet:

```text
-4 + -1 + 2 = -3
```

Sum is smaller than zero.

Move `left` forward.

Continue until the pointers meet:

```text
-4 + 1 + 2 = -1
```

No triplet is found for `-4`.

### Iteration 2: i = 1

```text
nums[i] = -1
left = 2
right = 5
```

Current triplet:

```text
-1 + -1 + 2 = 0
```

<badge color="success">Triplet found</badge>

Store:

```text
[-1, -1, 2]
```

Move both pointers.

Now:

```text
left = 3
right = 4
```

Calculate:

```text
-1 + 0 + 1 = 0
```

<badge color="success">Triplet found</badge>

Store:

```text
[-1, 0, 1]
```

The pointers meet, so stop this iteration.

### Iteration 3: i = 2

```cpp
nums[2] == nums[1]
```

Both are `-1`.

Skip this iteration because we already processed `-1` as the first number.

### Remaining iterations

For `i = 3` and `i = 4`, there are not enough remaining elements to form another valid triplet.

### Final Output

```cpp
[
    [-1, -1, 2],
    [-1, 0, 1]
]
```

---

# How Do We Avoid Duplicate Triplets?

This is one of the most important parts of 3Sum.

Consider:

```text
[-1, -1, 0, 1, 1]
```

Without duplicate handling, we might generate the same triplet multiple times.

We handle duplicates in three places.

## 1. Skip duplicate first numbers

```cpp
if(i > 0 && nums[i] == nums[i - 1]) {
    continue;
}
```

This ensures that we don't process the same first number again.

## 2. Skip duplicate left values

After finding a valid triplet:

```cpp
while(left < right &&
      nums[left] == nums[left - 1]) {
    left++;
}
```

## 3. Skip duplicate right values

```cpp
while(left < right &&
      nums[right] == nums[right + 1]) {
    right--;
}
```

**Important:** We skip duplicate pointer values after finding a valid triplet, not on every iteration.

---

# Why Does the Two-Pointer Approach Work?

Sorting gives us an important advantage.

If the sum is too small:

```cpp
sum < 0
```

Move `left` forward because the next number is greater than or equal to the current number.

If the sum is too large:

```cpp
sum > 0
```

Move `right` backward because the previous number is smaller than or equal to the current number.

This lets us eliminate possible pairs without checking all combinations.

---

# Complexity Analysis

Let `n` be the number of elements.

| Operation | Complexity |
|---|---|
| Sorting | O(n log n) |
| Outer loop | O(n) |
| Two-pointer traversal per fixed number | O(n) |
| Total | O(n²) |

**Time Complexity:** O(n²)

**Auxiliary Space:** O(1) for the algorithm's additional working variables, excluding sorting implementation details and the output. `std::sort` typically uses O(log n) stack space.

**Output Space:** Depends on the number of unique triplets.

---

# Approach Comparison

| Approach | Time | Extra Space |
|---|---|---|
| Brute Force + Set | O(n³ log m) approximately | O(m) |
| Sorting + Two Pointers | O(n²) | O(1) auxiliary, excluding sort stack and output |

**Best approach:** Sorting + Two Pointers.

---

# Important C++ Concepts

## 1. Sorting

```cpp
sort(nums.begin(), nums.end());
```

Arranges elements in ascending order.

## 2. vector<vector<int>>

```cpp
vector<vector<int>> ans;
```

Stores multiple triplets.

## 3. push_back()

```cpp
ans.push_back({-1, 0, 1});
```

Adds a triplet to the answer.

## 4. Duplicate Checking

```cpp
if(i > 0 && nums[i] == nums[i - 1])
```

Prevents processing the same starting value more than once.

## 5. Two Pointers

```cpp
int left = i + 1;
int right = n - 1;
```

Finds the other two numbers after fixing the first number.

---

# Edge Cases

| Input | Expected Output |
|---|---|
| `[]` | `[]` |
| `[0]` | `[]` |
| `[0, 0]` | `[]` |
| `[0, 0, 0]` | `[[0, 0, 0]]` |
| `[-1, 0, 1]` | `[[-1, 0, 1]]` |
| `[-1, -1, 2, 2]` | `[[-1, -1, 2]]` |
| `[1, 2, -2, -1]` | `[[-2, 1, 1]]` only if another 1 exists; otherwise no triplet |

---

# Interview Pattern Recognition

When you see:

- Find three numbers that satisfy a target sum.
- Return unique triplets.
- The array is unsorted.
- Brute force uses three nested loops.

Think:

**Sorting + Two Pointers**

The general strategy:

1. Sort the array.
2. Fix one element.
3. Use two pointers for the remaining elements.
4. Skip duplicates.
5. Store valid combinations.

---

# 30-Second Revision

1. Sort the array.
2. Run a loop from `i = 0` to `n - 3`.
3. Skip duplicate values of `nums[i]`.
4. Initialize `left = i + 1` and `right = n - 1`.
5. Calculate the sum of all three numbers.
6. If sum is zero, store the triplet and move both pointers.
7. If sum is negative, increment `left`.
8. If sum is positive, decrement `right`.
9. Skip duplicate left and right values after finding a triplet.
10. Return the answer.

**Time:** O(n²)  
**Auxiliary Space:** O(1), excluding sorting stack and output.

**Pattern:** Two Pointers → Sorting + Fixed Element + Pair Search.

**Status:** Notes prepared. Practise implementing the duplicate-skipping logic independently.

**Pattern:** Two Pointers → Palindrome Validation.

**Status:** Initial cleaning + two-pointer approach understood. Practise the O(1) space approach independently.
