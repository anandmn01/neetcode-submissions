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
# NeetCode 150 – 13. Container With Most Water

**Category:** Two Pointers  
**Pattern:** Opposite Direction Two Pointers + Greedy  
**Language:** C++

## Problem

You are given an integer array `height` of length `n`.

Each element represents a vertical line drawn at position `i`, with height `height[i]`.

Find two lines that, together with the x-axis, form a container that holds the maximum amount of water.

Return the maximum possible water area.

### Important Conditions

- Choose two different indices.
- The width is the distance between the two indices.
- The water level is limited by the shorter line.
- Maximize the total area.

### Example

```cpp
Input: height = [1,8,6,2,5,4,8,3,7]

Output: 49
```

Explanation:

Choose the lines at indices 1 and 8 (0-based indexing).

```text
Height = min(8, 7) = 7
Width  = 8 - 1 = 7

Area = 7 * 7 = 49
```

---

# 1. Understanding the Area Formula

The most important formula:

\[
Area = Width \times Height
\]

Where:

```cpp
int width = right - left;

int h = min(height[left], height[right]);

int area = width * h;
```

**Why use the minimum height?**

Because water cannot be stored above the shorter boundary. If one wall is shorter, water will overflow from that side.

Example:

```text
Left height  = 8
Right height = 5

Effective height = min(8, 5) = 5
```

---

# Approach 1: Brute Force

## Logic

Check every possible pair of vertical lines.

1. Select the first line using the outer loop.
2. Select the second line using the inner loop.
3. Calculate the area.
4. Update the maximum area.

## Code

```cpp
class Solution {
public:
    int maxArea(vector<int>& height) {

        int ans = 0;
        int n = height.size();

        for(int i = 0; i < n; i++) {

            for(int j = i + 1; j < n; j++) {

                int width = j - i;

                int h = min(height[i], height[j]);

                int area = width * h;

                ans = max(ans, area);
            }
        }

        return ans;
    }
};
```

## Complexity

- Time: O(n²)
- Auxiliary Space: O(1)

**Limitation:** Checking every possible pair is inefficient for large arrays.

---

# Approach 2: Two Pointers – Optimal

<badge color="success">Recommended interview approach</badge>

## Core Logic

Instead of checking every pair, start with two pointers at opposite ends.

```cpp
int left = 0;
int right = height.size() - 1;
```

Calculate the area and move the pointer at the shorter line.

Why?

- The width decreases whenever either pointer moves inward.
- Keeping the shorter line while moving the taller line cannot produce a larger area than the current one.
- Moving the shorter line gives us a chance to find a taller boundary.

This is the key greedy observation.

## Code

```cpp
class Solution {
public:
    int maxArea(vector<int>& height) {

        int left = 0;
        int right = height.size() - 1;

        int ans = 0;

        while(left < right) {

            int width = right - left;

            int h = min(height[left], height[right]);

            int area = width * h;

            ans = max(ans, area);

            if(height[left] < height[right]) {
                left++;
            }
            else {
                right--;
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
height = [1,8,6,2,5,4,8,3,7]
```

Initial values:

```text
left = 0
right = 8
ans = 0
```

| Left index | Right index | Left height | Right height | Width | Area | Action |
|---:|---:|---:|---:|---:|---:|---|
| 0 | 8 | 1 | 7 | 8 | 8 | Move left |
| 1 | 8 | 8 | 7 | 7 | 49 | Move right |
| 1 | 7 | 8 | 3 | 6 | 18 | Move right |
| 1 | 6 | 8 | 8 | 5 | 40 | Move right |
| 1 | 5 | 8 | 4 | 4 | 16 | Move right |
| 1 | 4 | 8 | 5 | 3 | 15 | Move right |
| 1 | 3 | 8 | 2 | 2 | 4 | Move right |
| 1 | 2 | 8 | 6 | 1 | 6 | Move right |

Maximum area:

```text
ans = 49
```

Final answer:

```cpp
return 49;
```

---

# Why Do We Move the Shorter Pointer?

This is the most important interview question.

Suppose:

```text
Left height  = 8
Right height = 5
Width        = 7
```

Current area:

\[
Area = 7 \times 5 = 35
\]

Now consider moving the taller pointer (left) inward.

- Width becomes smaller.
- The right boundary is still height 5.
- The new effective height cannot exceed 5.

Therefore, the new area cannot exceed the current area.

Instead, move the shorter pointer (right). We might find a taller line that compensates for the reduced width.

**Remember:** Always move the shorter boundary.

If both heights are equal, moving either pointer is safe.

---

# Approach Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Two Pointers | O(n) | O(1) |

**Best approach:** Two Pointers.

---

# Important C++ Concepts

## 1. min()

Returns the smaller of two values.

```cpp
int h = min(height[left], height[right]);
```

## 2. max()

Keeps the maximum area found so far.

```cpp
ans = max(ans, area);
```

## 3. Opposite Direction Pointers

```cpp
int left = 0;
int right = height.size() - 1;
```

One pointer starts at the beginning and the other at the end.

## 4. Greedy Decision

```cpp
if(height[left] < height[right]) {
    left++;
}
else {
    right--;
}
```

We make a local decision that safely eliminates pairs that cannot improve the answer.

---

# Edge Cases

| Input | Expected Output | Reason |
|---|---:|---|
| `[1,1]` | 1 | Minimum valid input |
| `[1,2]` | 1 | Width is 1 |
| `[4,3,2,1,4]` | 16 | Maximum area from both ends |
| `[1,2,1]` | 2 | Best area is between indices 0 and 2 |
| `[5,5,5,5]` | 15 | Equal heights |

---

# Interview Pattern Recognition

When a problem asks you to:

- Find maximum area between two elements.
- Choose two boundaries.
- Work with an array of heights.
- Optimize beyond checking all pairs.

Think:

**Two Pointers + Greedy**

Ask yourself:

1. Can I start from both ends?
2. Is the smaller boundary limiting the result?
3. Which pointer can I move without losing a potentially better answer?

---

# 30-Second Revision

1. Initialize `left = 0`.
2. Initialize `right = n - 1`.
3. Initialize `ans = 0`.
4. Calculate width: `right - left`.
5. Calculate height: `min(height[left], height[right])`.
6. Calculate area and update `ans`.
7. Move the pointer with the smaller height.
8. Continue while `left < right`.
9. Return `ans`.

**Recommended approach:** Two Pointers.

**Time:** O(n)  
**Auxiliary Space:** O(1)

**Pattern:** Two Pointers → Opposite Direction + Greedy.

**Status:** Notes prepared. Practise the pointer-movement reasoning independently.
**Status:** Notes prepared. Practise implementing the duplicate-skipping logic independently.

**Pattern:** Two Pointers → Palindrome Validation.

# NeetCode 150 – 14. Trapping Rain Water

**Category:** Two Pointers  
**Pattern:** Prefix/Suffix Maximum + Two Pointers  
**Language:** C++

## Problem

Given an array of non-negative integers `height`, where each element represents the elevation of a bar, calculate how much rainwater can be trapped between the bars.

Return the total amount of trapped water.

### Example

```cpp
Input: height = [0,1,0,2,1,0,1,3,2,1,2,1]

Output: 6
```

Explanation:

The elevation map traps a total of 6 units of rainwater.

```text
Input: height = [4,2,0,3,2,5]

Output: 9
```

---

# 1. Understanding the Formula

The amount of water trapped at any index depends on the tallest boundary on its left and the tallest boundary on its right.

For every position:

\[
Water[i] = \min(leftMax[i], rightMax[i]) - height[i]
\]

Where:

- `leftMax[i]` = maximum height from index 0 to i.
- `rightMax[i]` = maximum height from index i to the end.
- `height[i]` = current bar height.

**Important:** Water is trapped only when both boundaries are taller than the current bar.

---

# Approach 1: Brute Force

## Logic

For every index:

1. Find the maximum height on its left.
2. Find the maximum height on its right.
3. Calculate the minimum of these two boundaries.
4. Subtract the current height.
5. Add the trapped water to the answer.

## Code

```cpp
class Solution {
public:
    int trap(vector<int>& height) {

        int n = height.size();
        int ans = 0;

        for(int i = 0; i < n; i++) {

            int leftMax = 0;
            int rightMax = 0;

            // Find maximum height on the left
            for(int j = 0; j <= i; j++) {
                leftMax = max(leftMax, height[j]);
            }

            // Find maximum height on the right
            for(int j = i; j < n; j++) {
                rightMax = max(rightMax, height[j]);
            }

            int water = min(leftMax, rightMax) - height[i];

            ans += water;
        }

        return ans;
    }
};
```

## Complexity

- Time: O(n²), because we repeatedly scan the left and right sides.
- Auxiliary Space: O(1).

**Limitation:** Repeated maximum calculations make this approach inefficient.

---

# Approach 2: Prefix Maximum + Suffix Maximum

## Core Logic

Instead of finding the left and right maximum repeatedly, calculate them once and store them in arrays.

Create two arrays:

```cpp
vector<int> leftMax(n);
vector<int> rightMax(n);
```

### Step 1: Calculate Left Maximum

For every index, store the maximum height encountered from the beginning.

```cpp
leftMax[0] = height[0];

for(int i = 1; i < n; i++) {
    leftMax[i] = max(leftMax[i - 1], height[i]);
}
```

### Step 2: Calculate Right Maximum

Traverse from right to left.

```cpp
rightMax[n - 1] = height[n - 1];

for(int i = n - 2; i >= 0; i--) {
    rightMax[i] = max(rightMax[i + 1], height[i]);
}
```

### Step 3: Calculate Trapped Water

```cpp
water = min(leftMax[i], rightMax[i]) - height[i];
```

Add the water at every index.

## Code

```cpp
class Solution {
public:
    int trap(vector<int>& height) {

        int n = height.size();

        if(n == 0) {
            return 0;
        }

        vector<int> leftMax(n);
        vector<int> rightMax(n);

        // Calculate prefix maximum
        leftMax[0] = height[0];

        for(int i = 1; i < n; i++) {
            leftMax[i] = max(leftMax[i - 1], height[i]);
        }

        // Calculate suffix maximum
        rightMax[n - 1] = height[n - 1];

        for(int i = n - 2; i >= 0; i--) {
            rightMax[i] = max(rightMax[i + 1], height[i]);
        }

        int ans = 0;

        // Calculate trapped water
        for(int i = 0; i < n; i++) {

            int water = min(leftMax[i], rightMax[i]) - height[i];

            ans += water;
        }

        return ans;
    }
};
```

## Dry Run

Input:

```cpp
height = [4,2,0,3,2,5]
```

Prefix maximum:

```text
leftMax = [4,4,4,4,4,5]
```

Suffix maximum:

```text
rightMax = [5,5,5,5,5,5]
```

Calculate water:

| Index | Height | Left Max | Right Max | Water |
|---:|---:|---:|---:|---:|
| 0 | 4 | 4 | 5 | 0 |
| 1 | 2 | 4 | 5 | 2 |
| 2 | 0 | 4 | 5 | 4 |
| 3 | 3 | 4 | 5 | 1 |
| 4 | 2 | 4 | 5 | 2 |
| 5 | 5 | 5 | 5 | 0 |

Total:

\[
2+4+1+2=9
\]

Output:

```text
9
```

## Complexity

- Time: O(n)
- Auxiliary Space: O(n) for the two arrays.

**Advantage:** Easy to understand and implement.

**Limitation:** Requires additional arrays.

---

# Approach 3: Two Pointers – Optimal Space

<badge color="success">Recommended interview approach</badge>

## Core Logic

We can solve the problem using two pointers without creating prefix and suffix arrays.

Initialize:

```cpp
int left = 0;
int right = height.size() - 1;

int leftMax = 0;
int rightMax = 0;

int ans = 0;
```

Maintain:

- `leftMax`: Maximum height encountered from the left.
- `rightMax`: Maximum height encountered from the right.

### Important Observation

If:

```cpp
height[left] < height[right]
```

We can calculate the water at the left position using `leftMax`.

Why?

The right boundary is taller than the current left boundary, so the left side determines the water level.

Similarly, when the right height is smaller or equal, process the right position using `rightMax`.

### Step-by-Step Logic

**Case 1: Left height is smaller**

```cpp
if(height[left] < height[right])
```

Update the left maximum:

```cpp
leftMax = max(leftMax, height[left]);
```

Calculate water:

```cpp
ans += leftMax - height[left];
```

Move left:

```cpp
left++;
```

**Case 2: Right height is smaller or equal**

```cpp
else
```

Update the right maximum:

```cpp
rightMax = max(rightMax, height[right]);
```

Calculate water:

```cpp
ans += rightMax - height[right];
```

Move right:

```cpp
right--;
```

## Complete Optimal Code

```cpp
class Solution {
public:
    int trap(vector<int>& height) {

        int left = 0;
        int right = height.size() - 1;

        int leftMax = 0;
        int rightMax = 0;

        int ans = 0;

        while(left < right) {

            if(height[left] < height[right]) {

                leftMax = max(leftMax, height[left]);

                ans += leftMax - height[left];

                left++;
            }

            else {

                rightMax = max(rightMax, height[right]);

                ans += rightMax - height[right];

                right--;
            }
        }

        return ans;
    }
};
```

---

# Detailed Dry Run – Two Pointers

Input:

```cpp
height = [4,2,0,3,2,5]
```

Initial values:

```text
left = 0
right = 5

leftMax = 0
rightMax = 0
ans = 0
```

| Left | Right | Left Height | Right Height | Action | Water Added |
|---:|---:|---:|---:|---|---:|
| 0 | 5 | 4 | 5 | Process left | 0 |
| 1 | 5 | 2 | 5 | Process left | 2 |
| 2 | 5 | 0 | 5 | Process left | 4 |
| 3 | 5 | 3 | 5 | Process left | 1 |
| 4 | 5 | 2 | 5 | Process left | 2 |

Total water:

\[
0+2+4+1+2=9
\]

Final answer:

```text
9
```

---

# Why Does the Two-Pointer Approach Work?

This is an important interview question.

Suppose:

```text
height[left] < height[right]
```

The current left bar is shorter than the current right bar.

The water at the left position is determined by:

\[
Water[left] = leftMax - height[left]
\]

The right boundary is already taller than the current left bar, so it cannot limit the water at this position.

We can safely process the left side.

The same reasoning applies symmetrically when the right bar is shorter or equal.

**Key idea:** Process the side with the smaller current boundary because its limiting maximum is already known.

---

# Approach Comparison

| Approach | Time | Auxiliary Space |
|---|---|---|
| Brute Force | O(n²) | O(1) |
| Prefix + Suffix Arrays | O(n) | O(n) |
| Two Pointers | O(n) | O(1) |

**Best approach:** Two Pointers.

---

# Important C++ Concepts

## 1. max()

```cpp
leftMax = max(leftMax, height[left]);
```

Maintains the maximum height encountered so far.

## 2. min()

```cpp
int water = min(leftMax, rightMax) - height[i];
```

The shorter boundary determines the water level.

## 3. Prefix Maximum

Maximum element from the beginning through the current index.

```cpp
leftMax[i] = max(leftMax[i - 1], height[i]);
```

## 4. Suffix Maximum

Maximum element from the current index through the end.

```cpp
rightMax[i] = max(rightMax[i + 1], height[i]);
```

## 5. Two Pointers

```cpp
int left = 0;
int right = height.size() - 1;
```

Process the array from both ends.

---

# Edge Cases

| Input | Expected Output | Reason |
|---|---:|---|
| `[]` | 0 | Empty input |
| `[1]` | 0 | One bar cannot trap water |
| `[1,2,3,4]` | 0 | Increasing heights |
| `[4,3,2,1]` | 0 | Decreasing heights |
| `[3,0,2]` | 2 | Water trapped in the middle |
| `[4,2,0,3,2,5]` | 9 | Multiple water levels |

---

# Interview Pattern Recognition

When a problem asks you to:

- Calculate trapped water between bars.
- Find maximum boundaries on the left and right.
- Optimize repeated maximum calculations.
- Reduce auxiliary space.

Think:

**Prefix/Suffix Maximum + Two Pointers**

The important progression is:

1. Brute force: Find left and right maximum for every index.
2. Better: Precompute maximums using two arrays.
3. Optimal: Maintain maximums using two pointers.

---

# 30-Second Revision

1. Initialize `left = 0` and `right = n - 1`.
2. Initialize `leftMax = 0` and `rightMax = 0`.
3. While `left < right`, compare the current boundary heights.
4. If left is smaller, update `leftMax`.
5. Add `leftMax - height[left]` to the answer.
6. Move `left` forward.
7. Otherwise, update `rightMax`.
8. Add `rightMax - height[right]` to the answer.
9. Move `right` backward.
10. Return the total water.

**Recommended approach:** Two Pointers.

**Time:** O(n)  
**Auxiliary Space:** O(1)

**Pattern:** Two Pointers → Boundary Maximums + Greedy Processing.

**Status:** Notes prepared. Practise explaining why the shorter boundary can be processed safely.

**Status:** Initial cleaning + two-pointer approach understood. Practise the O(1) space approach independently.
