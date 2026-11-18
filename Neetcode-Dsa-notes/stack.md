# 39. Valid Parentheses

## 1. Problem Understanding

Given a string `s` containing only the following characters:

- `(`
- `)`
- `{`
- `}`
- `[`
- `]`

Determine whether the input string is valid.

A string is valid if:

1. Every opening bracket has a corresponding closing bracket.
2. Brackets are closed in the correct order.
3. Every closing bracket matches the most recent unmatched opening bracket.

### Example 1

```text
Input: s = "()"

Output: true
```

### Example 2

```text
Input: s = "()[]{}"

Output: true
```

### Example 3

```text
Input: s = "(]"

Output: false
```

### Example 4

```text
Input: s = "([])"

Output: true
```

### Example 5

```text
Input: s = "([)]"

Output: false
```

Why is Example 5 invalid?

Because the brackets are not closed in the correct order.

## 2. Main Intuition — Stack

Think about a stack of plates.

You put a plate on top and remove the topmost plate first.

This is called:

**LIFO = Last In, First Out**

The same concept works for brackets.

Whenever we find an opening bracket, push it into the stack.

Whenever we find a closing bracket:

1. Check whether the stack is empty.
2. Check whether the top opening bracket matches the closing bracket.
3. If it doesn't match, return `false`.
4. Otherwise, pop the opening bracket.

At the end, the stack must be empty.

## 3. Visual Explanation

Consider:

```text
s = "([])"
```

Initially:

```text
Stack = Empty
```

**Step 1:** Read `(`

```text
Stack:
(
```

Push the opening bracket.

**Step 2:** Read `[` 

```text
Stack:
[
(
```

Push another opening bracket.

**Step 3:** Read `]`

The top is `[`, which matches `]`.

Pop it.

```text
Stack:
(
```

**Step 4:** Read `)`

The top is `(`, which matches `)`.

Pop it.

```text
Stack = Empty
```

Final result:

```text
true
```

## 4. C++ Solution — Optimal Approach

```cpp
class Solution {
public:
    bool isValid(string s) {

        stack<char> st;

        for (char ch : s) {

            // Opening brackets
            if (ch == '(' || ch == '{' || ch == '[') {
                st.push(ch);
            }

            // Closing brackets
            else {

                if (st.empty()) {
                    return false;
                }

                char top = st.top();

                if ((ch == ')' && top == '(') ||
                    (ch == '}' && top == '{') ||
                    (ch == ']' && top == '[')) {

                    st.pop();
                }

                else {
                    return false;
                }
            }
        }

        // All opening brackets must be closed
        return st.empty();
    }
};
```

## 5. Understanding the Important Code

### A. Why use `stack<char>`?

```cpp
stack<char> st;
```

We only need to store opening bracket characters.

A stack helps us access the most recent unmatched opening bracket.

### B. Why push opening brackets?

```cpp
if (ch == '(' || ch == '{' || ch == '[') {
    st.push(ch);
}
```

Every opening bracket needs a corresponding closing bracket.

We store it until its matching closing bracket appears.

### C. Why check `st.empty()`?

```cpp
if (st.empty()) {
    return false;
}
```

Consider:

```text
s = ")"
```

We encounter a closing bracket without any previous opening bracket.

Therefore, the string is invalid.

### D. Why check `st.top()`?

```cpp
char top = st.top();
```

The top contains the most recently encountered unmatched opening bracket.

For correct nesting, the current closing bracket must match this opening bracket.

### E. Why pop?

```cpp
st.pop();
```

When a matching pair is found, both brackets are considered resolved.

We remove the opening bracket from the stack.

### F. Why return `st.empty()`?

```cpp
return st.empty();
```

Consider:

```text
s = "((("
```

Every character is an opening bracket, so no mismatch occurs during traversal.

But all three opening brackets remain unmatched.

Therefore, the answer must be `false`.

## 6. Dry Run

Input:

```text
s = "{[()]}"
```

| Character | Operation | Stack |
|---|---|---|
| `{` | Push | `{` |
| `[` | Push | `{ [` |
| `(` | Push | `{ [ (` |
| `)` | Match and pop | `{ [` |
| `]` | Match and pop | `{` |
| `}` | Match and pop | Empty |

Final stack:

```text
Empty
```

Output:

```text
true
```

### Another example: Invalid nesting

Input:

```text
s = "([)]"
```

| Character | Operation | Stack |
|---|---|---|
| `(` | Push | `(` |
| `[` | Push | `( [` |
| `)` | Top is `[`, mismatch | `( [` |

We immediately return:

```text
false
```

## 7. Complexity Analysis

Let `n` be the length of the string.

| Complexity | Value | Reason |
|---|---|---|
| Time | O(n) | Every character is processed once |
| Auxiliary space | O(n) | In the worst case, all characters are opening brackets |

## 8. Edge Cases

| Input | Expected output | Reason |
|---|---|---|
| `""` | `true` | Empty stack |
| `"()"` | `true` | Correct pair |
| `"()[]{}"` | `true` | All pairs match |
| `"(]"` | `false` | Wrong bracket type |
| `"((("` | `false` | Unmatched opening brackets |
| `")))"` | `false` | Closing brackets without openings |
| `"([)]"` | `false` | Incorrect nesting |
| `"{[]}"` | `true` | Correct nesting |

## 9. Common Interview Mistakes

1. Checking only whether the number of opening and closing brackets is equal.
2. Forgetting to check whether the stack is empty before accessing `st.top()`.
3. Forgetting to pop a matched opening bracket.
4. Checking bracket types but ignoring their nesting order.
5. Forgetting the final `st.empty()` check.

**Important:** Equal numbers of opening and closing brackets do not guarantee valid parentheses.

For example:

```text
([)]
```

There are two opening and two closing brackets, but the order is invalid.

## 10. C++ Concepts to Remember

- `stack<char>`: Stores characters using LIFO order.
- `st.push(ch)`: Inserts a character.
- `st.top()`: Returns the top element.
- `st.pop()`: Removes the top element.
- `st.empty()`: Checks whether the stack is empty.
- `for (char ch : s)`: Range-based loop for traversing a string.

## 11. Pattern Recognition

Whenever a problem involves:

- Matching brackets.
- Nested expressions.
- Checking opening and closing order.
- Finding the most recent unmatched opening element.

Think:

**Stack — LIFO**

```text
Opening Bracket
      ↓
    PUSH
      ↓
Closing Bracket
      ↓
Compare With TOP
      ↓
Match? → POP
      ↓
Check Stack Empty
      ↓
Return Answer
```

## 12. 30-Second Interview Revision

**Question:** How do you validate a string containing parentheses?

**Answer:**

"I use a stack to maintain opening brackets. Whenever I encounter an opening bracket, I push it onto the stack. When I encounter a closing bracket, I first check whether the stack is empty. If it is not empty, I compare the closing bracket with the top opening bracket. If they match, I pop the stack; otherwise, I return false. Finally, I return true only if the stack is empty. The time complexity is O(n), and the auxiliary space complexity is O(n)."

**Remember:**

`Opening → Push`

`Closing → Match → Pop`

`End → Stack Must Be Empty`
