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
# 40. Min Stack

## 1. Problem Understanding

Design a stack that supports the following operations in O(1) time:

1. `MinStack()` — Initialize the stack.
2. `push(val)` — Push an integer onto the stack.
3. `pop()` — Remove the top element.
4. `top()` — Return the top element.
5. `getMin()` — Return the minimum element currently in the stack.

**Important:** Every operation must take O(1) time.

## 2. Main Intuition — Two Stacks

A normal stack supports:

- Push: O(1)
- Pop: O(1)
- Top: O(1)

But finding the minimum normally requires O(n) time.

To solve this, maintain two stacks:

### Stack 1: Main Stack

Stores all inserted values.

### Stack 2: Min Stack

Stores the minimum value at each corresponding stack level.

Example:

```text
Push: 5

Main Stack:       Min Stack:
    5                 5

Push: 3

Main Stack:       Min Stack:
    3                 3
    5                 5

Push: 7

Main Stack:       Min Stack:
    7                 3
    3                 3
    5                 5
```

Notice:

- Main stack stores actual values.
- Min stack stores the minimum value seen so far at each level.

Therefore, the current minimum is always available at the top of the min stack.

## 3. Visual Explanation

Suppose we perform:

```text
push(5)
push(3)
push(7)
push(2)
```

After all operations:

```text
Main Stack:        Min Stack:

    2                  2
    7                  3
    3                  3
    5                  5
```

Now:

```cpp
top()    = 2
getMin() = 2
```

If we execute:

```cpp
pop();
```

Both stacks remove their top elements.

```text
Main Stack:        Min Stack:

    7                  3
    3                  3
    5                  5
```

Now:

```cpp
top()    = 7
getMin() = 3
```

The minimum is automatically restored without searching the stack.

## 4. C++ Solution — Two Stack Approach

```cpp
class MinStack {

private:
    stack<int> st;
    stack<int> minSt;

public:

    MinStack() {

    }

    void push(int val) {

        st.push(val);

        if (minSt.empty()) {
            minSt.push(val);
        }
        else {
            minSt.push(min(val, minSt.top()));
        }
    }

    void pop() {

        st.pop();
        minSt.pop();
    }

    int top() {

        return st.top();
    }

    int getMin() {

        return minSt.top();
    }
};
```

## 5. Understanding the Important Code

### A. Why do we use two stacks?

```cpp
stack<int> st;
stack<int> minSt;
```

`st` stores actual values.

`minSt` stores the minimum value at every corresponding level.

### B. Understanding push()

```cpp
void push(int val) {

    st.push(val);

    if (minSt.empty()) {
        minSt.push(val);
    }
    else {
        minSt.push(min(val, minSt.top()));
    }
}
```

Suppose:

```text
Current minimum = 3
New value = 7
```

The new minimum remains 3.

```cpp
minSt.push(min(7, 3));
```

If the new value is 2:

```cpp
minSt.push(min(2, 3));
```

The new minimum becomes 2.

**Important:** We push into `minSt` on every push operation, even if the minimum does not change.

### C. Understanding pop()

```cpp
void pop() {

    st.pop();
    minSt.pop();
}
```

Both stacks must remove one element.

Why?

Each position in `minSt` corresponds to the minimum of the main stack up to that position.

### D. Understanding top()

```cpp
int top() {
    return st.top();
}
```

Returns the most recently inserted value.

### E. Understanding getMin()

```cpp
int getMin() {
    return minSt.top();
}
```

Returns the current minimum immediately.

No traversal is required.

## 6. Dry Run

Operations:

```text
push(5)
push(3)
push(7)
getMin()
push(2)
getMin()
pop()
top()
getMin()
```

| Operation | Main Stack | Min Stack | Result |
|---|---|---|---|
| `push(5)` | 5 | 5 | — |
| `push(3)` | 3, 5 | 3, 5 | — |
| `push(7)` | 7, 3, 5 | 3, 3, 5 | — |
| `getMin()` | 7, 3, 5 | 3, 3, 5 | 3 |
| `push(2)` | 2, 7, 3, 5 | 2, 3, 3, 5 | — |
| `getMin()` | 2, 7, 3, 5 | 2, 3, 3, 5 | 2 |
| `pop()` | 7, 3, 5 | 3, 3, 5 | — |
| `top()` | 7, 3, 5 | 3, 3, 5 | 7 |
| `getMin()` | 7, 3, 5 | 3, 3, 5 | 3 |

## 7. Complexity Analysis

| Operation | Time Complexity |
|---|---|
| Constructor | O(1) |
| `push()` | O(1) |
| `pop()` | O(1) |
| `top()` | O(1) |
| `getMin()` | O(1) |

**Auxiliary space: O(n)**

We maintain two stacks, each containing one entry per inserted element.

## 8. Alternative Approach — Single Stack of Pairs

We can also maintain the current minimum alongside every value.

```cpp
class MinStack {

private:
    stack<pair<int, int>> st;

public:

    MinStack() {

    }

    void push(int val) {

        int currentMin = val;

        if (!st.empty()) {
            currentMin = min(val, st.top().second);
        }

        st.push({val, currentMin});
    }

    void pop() {
        st.pop();
    }

    int top() {
        return st.top().first;
    }

    int getMin() {
        return st.top().second;
    }
};
```

Each pair stores:

```text
{actual value, minimum so far}
```

Example:

```text
push(5) → {5, 5}
push(3) → {3, 3}
push(7) → {7, 3}
push(2) → {2, 2}
```

This also achieves O(1) time for every operation and O(n) space.

## 9. Common Interview Mistakes

1. Using only one normal stack and traversing it to find the minimum.
2. Updating the minimum only when a smaller value arrives, without preserving previous minimums.
3. Forgetting to pop from the minimum stack when popping from the main stack.
4. Pushing into the minimum stack only when the minimum changes.
5. Forgetting to handle the first insertion when the minimum stack is empty.
6. Confusing the top element with the minimum element.

## 10. C++ Concepts to Remember

- `stack<int>`: Stack of integers.
- `st.push(val)`: Insert an element.
- `st.pop()`: Remove the top element.
- `st.top()`: Access the top element.
- `st.empty()`: Check whether the stack is empty.
- `min(a, b)`: Returns the smaller value.
- `pair<int, int>`: Stores two related values together.

## 11. Pattern Recognition

Whenever a problem asks you to maintain an additional property such as minimum or maximum while supporting stack operations in O(1), consider an auxiliary stack or storing extra information with each element.

```text
Push / Pop
    ↓
Maintain Minimum Information
    ↓
Get Minimum in O(1)
```

## 12. 30-Second Interview Revision

**Question:** How do you implement a Min Stack with O(1) operations?

**Answer:**

"I maintain two stacks. The first stores all actual values, and the second stores the minimum value at every corresponding stack level. Whenever I push a value, I push the smaller of the new value and the current minimum onto the minimum stack. Whenever I pop, I pop from both stacks. Therefore, the minimum is always available at the top of the auxiliary stack in O(1) time."

**Remember:**

`Main Stack = Actual Values`

`Min Stack = Minimum So Far`

`Push Both → Pop Both → Get Minimum From minSt.top()`

# 41. Evaluate Reverse Polish Notation

## 1. Problem Understanding

You are given an array of strings `tokens` representing an arithmetic expression in Reverse Polish Notation.

Evaluate the expression and return its integer result.

Supported operators:

- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division

**Important:** Division between two integers truncates toward zero.

### Example 1

```text
Input:
tokens = ["2", "1", "+", "3", "*"]

Output:
9
```

Explanation:

```text
(2 + 1) * 3

= 3 * 3

= 9
```

### Example 2

```text
Input:
tokens = ["4", "13", "5", "/", "+"]

Output:
6
```

Explanation:

```text
4 + (13 / 5)

= 4 + 2

= 6
```

### Example 3

```text
Input:
tokens = ["10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"]

Output:
22
```

## 2. Main Intuition — Stack

In normal mathematical notation, we write:

```text
2 + 3
```

In Reverse Polish Notation, we write:

```text
2 3 +
```

The operator comes after its operands.

We use a stack because the last two numbers inserted are the operands of the next operator.

### Basic rules

**When the token is a number:**

Push it into the stack.

**When the token is an operator:**

1. Pop the top element. This is the second operand (`b`).
2. Pop the next element. This is the first operand (`a`).
3. Calculate `a operator b`.
4. Push the result back.

**Important:** Operand order matters for subtraction and division.

## 3. Visual Explanation

Consider:

```text
tokens = ["2", "1", "+", "3", "*"]
```

Initially:

```text
Stack = Empty
```

### Step 1: Read "2"

Number found, push it.

```text
Stack:
2
```

### Step 2: Read "1"

Push it.

```text
Stack:
1
2
```

### Step 3: Read "+"

Pop two numbers:

```text
b = 1
a = 2

2 + 1 = 3
```

Push the result.

```text
Stack:
3
```

### Step 4: Read "3"

Push it.

```text
Stack:
3
3
```

### Step 5: Read "*"

Pop two numbers:

```text
b = 3
a = 3

3 * 3 = 9
```

Push the result.

```text
Stack:
9
```

Final answer:

```text
9
```

## 4. C++ Solution — Optimal Approach

```cpp
class Solution {
public:
    int evalRPN(vector<string>& tokens) {

        stack<int> st;

        for (string token : tokens) {

            // If token is an operator
            if (token == "+" || token == "-" ||
                token == "*" || token == "/") {

                int b = st.top();
                st.pop();

                int a = st.top();
                st.pop();

                int result = 0;

                if (token == "+") {
                    result = a + b;
                }
                else if (token == "-") {
                    result = a - b;
                }
                else if (token == "*") {
                    result = a * b;
                }
                else if (token == "/") {
                    result = a / b;
                }

                st.push(result);
            }

            // If token is a number
            else {
                st.push(stoi(token));
            }
        }

        return st.top();
    }
};
```

## 5. Understanding the Important Code

### A. Why use `stack<int>`?

```cpp
stack<int> st;
```

We need to store operands and intermediate calculation results.

The stack follows LIFO:

Last In, First Out.

### B. How do we identify an operator?

```cpp
if (token == "+" || token == "-" ||
    token == "*" || token == "/")
```

If the token matches one of these four symbols, we perform an operation.

Otherwise, it represents an integer.

### C. Why pop `b` before `a`?

```cpp
int b = st.top();
st.pop();

int a = st.top();
st.pop();
```

This is extremely important.

Consider:

```text
tokens = ["10", "5", "-"]
```

The stack contains:

```text
5
10
```

We must calculate:

```text
10 - 5 = 5
```

Not:

```text
5 - 10 = -5
```

Therefore:

- First popped value = `b`
- Second popped value = `a`

Always calculate:

```cpp
a - b
a / b
```

### D. Why use `stoi()`?

```cpp
st.push(stoi(token));
```

The input is a vector of strings.

`stoi()` converts a numeric string into an integer.

Example:

```cpp
stoi("123") = 123;
stoi("-11") = -11;
```

### E. Why return `st.top()`?

```cpp
return st.top();
```

After evaluating the complete expression, the stack contains exactly one value: the final result.

## 6. Dry Run

Input:

```text
tokens = ["4", "13", "5", "/", "+"]
```

| Token | Operation | Stack (bottom → top) |
|---|---|---|
| `"4"` | Push 4 | `[4]` |
| `"13"` | Push 13 | `[4, 13]` |
| `"5"` | Push 5 | `[4, 13, 5]` |
| `"/"` | 13 / 5 = 2 | `[4, 2]` |
| `"+"` | 4 + 2 = 6 | `[6]` |

Final answer:

```text
6
```

## 7. Complexity Analysis

Let `n` be the number of tokens.

| Complexity | Value | Reason |
|---|---|---|
| Time | O(n) | Every token is processed once |
| Auxiliary space | O(n) | Stack can hold up to O(n) operands |

## 8. Edge Cases

| Input | Expected output |
|---|---:|
| `["2", "1", "+"]` | 3 |
| `["4", "13", "5", "/", "+"]` | 6 |
| `["10", "6", "-"]` | 4 |
| `["10", "6", "/"]` | 1 |
| `["-11", "2", "/"]` | -5 |
| `["3", "4", "*"]` | 12 |

**Important division example:**

```text
-11 / 2 = -5
```

C++ integer division truncates toward zero, not toward negative infinity.

## 9. Common Interview Mistakes

1. Reversing operand order for subtraction and division.
2. Forgetting to convert numeric strings using `stoi()`.
3. Forgetting to pop both operands.
4. Pushing the operator result before completing the calculation.
5. Assuming normal infix notation instead of Reverse Polish Notation.
6. Using floating-point division instead of integer division.

## 10. C++ Concepts to Remember

- `stack<int>`: Stores operands and intermediate results.
- `st.push(value)`: Adds a value.
- `st.pop()`: Removes the top value.
- `st.top()`: Accesses the top value.
- `stoi(token)`: Converts a string to an integer.
- Integer division: Truncates toward zero.

## 11. Pattern Recognition

Whenever an expression has:

- Operators after operands.
- Nested calculations.
- A need to evaluate expressions in a specific order.

Think:

**Stack — Reverse Polish Notation**

```text
Read Token
    ↓
Number or Operator?
    ↓
Number → Push
    ↓
Operator → Pop Two
    ↓
Calculate a operator b
    ↓
Push Result
    ↓
Return Final Stack Value
```

## 12. 30-Second Interview Revision

**Question:** How do you evaluate an expression in Reverse Polish Notation?

**Answer:**

"I use a stack to store operands. I traverse the tokens from left to right. If a token is a number, I convert it using stoi and push it. If it is an operator, I pop the second operand first and then the first operand, perform the operation in the correct order, and push the result back. At the end, the remaining stack element is the answer. The time and auxiliary space complexities are both O(n)."

**Remember:**

`Number → Push`

`Operator → Pop b → Pop a → Calculate a op b → Push`

`End → Return st.top()`

# 42. Daily Temperatures

## 1. Problem Understanding

You are given an integer array `temperatures`, where `temperatures[i]` represents the temperature on the ith day.

For each day, find how many days you have to wait until a warmer temperature.

If there is no future day with a warmer temperature, store `0`.

Return the resulting array.

### Example 1

```text
Input:
temperatures = [73, 74, 75, 71, 69, 72, 76, 73]

Output:
[1, 1, 4, 2, 1, 1, 0, 0]
```

Explanation:

- Day 1: 73 → warmer temperature 74 after 1 day.
- Day 2: 74 → warmer temperature 75 after 1 day.
- Day 3: 75 → warmer temperature 76 after 4 days.
- Day 4: 71 → warmer temperature 72 after 2 days.
- Day 5: 69 → warmer temperature 72 after 1 day.
- Day 6: 72 → warmer temperature 76 after 1 day.
- Day 7: 76 → no warmer future temperature, so 0.
- Day 8: 73 → no future day, so 0.

## 2. Main Intuition — Monotonic Stack

A brute-force approach would check every future day for every temperature.

That can take O(n²) time.

Instead, use a **monotonic decreasing stack**.

The stack stores indices of days whose warmer future temperature has not yet been found.

Why store indices instead of temperatures?

Because we need to calculate the number of days between two positions:

```cpp
currentIndex - previousIndex
```

### Important rule

When the current temperature is greater than the temperature at the index on top of the stack:

1. The current day is the warmer day for that previous day.
2. Pop the previous index.
3. Calculate the difference between the indices.
4. Continue checking the stack.

If the current temperature is not greater, push its index.

## 3. Visual Explanation

Consider:

```text
temperatures = [73, 74, 75, 71, 69, 72, 76, 73]
```

Initially:

```text
Stack = Empty
Answer = [0, 0, 0, 0, 0, 0, 0, 0]
```

**Step 1: Index 0, Temperature 73**

Push index 0.

```text
Stack: [0]
```

**Step 2: Index 1, Temperature 74**

74 is greater than 73.

Pop index 0.

```cpp
answer[0] = 1 - 0;
```

Push index 1.

```text
Stack: [1]
Answer: [1, 0, 0, 0, 0, 0, 0, 0]
```

**Step 3: Index 2, Temperature 75**

75 is greater than 74.

```cpp
answer[1] = 2 - 1;
```

Push index 2.

```text
Stack: [2]
```

**Step 4: Index 3, Temperature 71**

71 is smaller than 75.

Push index 3.

```text
Stack: [2, 3]
```

**Step 5: Index 4, Temperature 69**

69 is smaller than 71.

Push index 4.

```text
Stack: [2, 3, 4]
```

**Step 6: Index 5, Temperature 72**

72 is greater than 69 and 71.

Pop both indices:

```cpp
answer[4] = 5 - 4; // 1 day
answer[3] = 5 - 3; // 2 days
```

Push index 5.

```text
Stack: [2, 5]
```

**Step 7: Index 6, Temperature 76**

76 is greater than 72 and 75.

```cpp
answer[5] = 6 - 5; // 1 day
answer[2] = 6 - 2; // 4 days
```

Push index 6.

```text
Stack: [6]
```

**Step 8: Index 7, Temperature 73**

73 is smaller than 76.

Push index 7.

```text
Stack: [6, 7]
```

The remaining indices never found a warmer future temperature, so their answers stay 0.

Final answer:

```text
[1, 1, 4, 2, 1, 1, 0, 0]
```

## 4. C++ Solution — Optimal Approach

```cpp
class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {

        int n = temperatures.size();

        vector<int> answer(n, 0);

        stack<int> st;

        for (int i = 0; i < n; i++) {

            // Find warmer temperature for previous days
            while (!st.empty() &&
                   temperatures[i] > temperatures[st.top()]) {

                int prevIndex = st.top();
                st.pop();

                answer[prevIndex] = i - prevIndex;
            }

            // Store current index
            st.push(i);
        }

        return answer;
    }
};
```

## 5. Understanding the Important Code

### A. Why initialize the answer with zeros?

```cpp
vector<int> answer(n, 0);
```

Every position initially contains 0.

If a day never gets a warmer future temperature, its answer remains 0.

### B. Why store indices?

```cpp
stack<int> st;
```

We store indices rather than temperature values because we need to calculate the waiting period.

```cpp
answer[prevIndex] = i - prevIndex;
```

### C. Why use a while loop?

```cpp
while (!st.empty() &&
       temperatures[i] > temperatures[st.top()])
```

One warmer day can resolve multiple previous days.

Example:

```text
Temperatures: 71, 69, 72
```

When we reach 72, it is warmer than both 71 and 69.

Therefore, we must continue popping until the stack is empty or its top represents a temperature greater than or equal to the current temperature.

### D. Why push the current index?

```cpp
st.push(i);
```

The current day may need a warmer temperature in the future.

We store its index until that warmer day appears.

### E. Why don't we process the remaining stack after the loop?

The remaining indices have no warmer future temperature.

Their corresponding answers are already initialized to 0.

## 6. Dry Run

Input:

```text
temperatures = [73, 74, 75, 71, 69, 72]
```

| Index | Temperature | Stack operation | Answer |
|---:|---:|---|---|
| 0 | 73 | Push 0 | [0, 0, 0, 0, 0, 0] |
| 1 | 74 | Pop 0, push 1 | [1, 0, 0, 0, 0, 0] |
| 2 | 75 | Pop 1, push 2 | [1, 1, 0, 0, 0, 0] |
| 3 | 71 | Push 3 | [1, 1, 0, 0, 0, 0] |
| 4 | 69 | Push 4 | [1, 1, 0, 0, 0, 0] |
| 5 | 72 | Pop 4, pop 3, push 5 | [1, 1, 0, 2, 1, 0] |

Final answer:

```text
[1, 1, 0, 2, 1, 0]
```

The third day's answer remains 0 because its warmer day has not appeared in this input.

## 7. Complexity Analysis

Let `n` be the number of temperatures.

| Complexity | Value | Reason |
|---|---|---|
| Time | O(n) | Every index is pushed and popped at most once |
| Auxiliary space | O(n) | Stack can store up to n indices |
| Output space | O(n) | Answer vector contains n values |

**Why isn't the while loop O(n²)?**

Although there is a while loop inside the for loop, every index enters the stack once and leaves it at most once.

Therefore, the total number of stack operations is O(n).

## 8. Alternative Approach — Brute Force

For every day, check all future days until a warmer temperature is found.

```cpp
class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {

        int n = temperatures.size();

        vector<int> answer(n, 0);

        for (int i = 0; i < n; i++) {

            for (int j = i + 1; j < n; j++) {

                if (temperatures[j] > temperatures[i]) {

                    answer[i] = j - i;
                    break;
                }
            }
        }

        return answer;
    }
};
```

Complexity:

- Time: O(n²)
- Auxiliary space: O(1), excluding output.

This approach is easy to understand but inefficient for large inputs.

**Best interview solution: Monotonic Stack.**

## 9. Edge Cases

| Input | Expected output |
|---|---|
| `[73, 74]` | `[1, 0]` |
| `[75, 74, 73]` | `[0, 0, 0]` |
| `[70, 70, 71]` | `[2, 1, 0]` |
| `[30]` | `[0]` |
| `[30, 40, 50, 60]` | `[1, 1, 1, 0]` |
| `[60, 50, 40, 30]` | `[0, 0, 0, 0]` |

**Important:** Equal temperatures do not count as warmer temperatures. That's why the condition uses `>` rather than `>=`.

## 10. Common Interview Mistakes

1. Storing temperature values instead of indices.
2. Using `>=` instead of `>` and incorrectly treating equal temperatures as warmer.
3. Using `if` instead of `while`, which may leave other previous days unresolved.
4. Forgetting that the answer represents the difference between indices.
5. Forgetting to initialize the answer array with zeros.
6. Assuming every index remaining in the stack needs to be processed after the loop.

## 11. C++ Concepts to Remember

- `vector<int>(n, 0)`: Creates an array of n zeros.
- `stack<int>`: Stores unresolved indices.
- `st.top()`: Gets the most recently stored index.
- `st.pop()`: Removes the top index.
- `st.push(i)`: Stores the current index.
- `temperatures[st.top()]`: Gets the temperature at the top index.

## 12. Pattern Recognition

Whenever a problem asks for the:

- Next greater element.
- Next warmer temperature.
- Next larger value to the right.
- Distance until a greater element appears.

Think:

**Monotonic Decreasing Stack**

```text
Traverse Array
      ↓
Compare Current Value With Stack Top
      ↓
Current Value Greater?
      ↓
Yes → Pop Previous Index
      ↓
Calculate Index Difference
      ↓
Repeat While Greater
      ↓
Push Current Index
```

## 13. 30-Second Interview Revision

**Question:** How do you find the number of days until a warmer temperature for every day?

**Answer:**

"I use a monotonic decreasing stack that stores indices rather than temperatures. I traverse the array from left to right. Whenever the current temperature is greater than the temperature at the index on top of the stack, I pop that index and calculate the difference between the current index and the popped index. This difference gives the number of days to wait. I continue until the stack is empty or its top temperature is greater than or equal to the current temperature. The solution takes O(n) time and O(n) auxiliary space."

**Remember:**

`Store Indices → Find Warmer Day → Pop → Calculate Distance → Push Current Index`


# 43. Car Fleet

## 1. Problem Understanding

You are given:

- `target`: Destination position.
- `position[i]`: Starting position of car `i`.
- `speed[i]`: Speed of car `i`.

All cars move toward the same target.

A faster car behind a slower car will eventually catch up. When it catches up, both cars become part of the same fleet and travel at the slower car's speed.

A car fleet is a group of cars that travel together at the same speed.

Return the total number of car fleets that will arrive at the destination.

### Example 1

```text
Input:
target = 12
position = [10, 8, 0, 5, 3]
speed    = [2,  4, 1, 1, 3]

Output: 3
```

Explanation:

| Position | Speed | Time to target |
|---:|---:|---:|
| 10 | 2 | 1 |
| 8 | 4 | 1 |
| 5 | 1 | 7 |
| 3 | 3 | 3 |
| 0 | 1 | 12 |

Cars at positions 10 and 8 form one fleet because the car at position 8 catches the car at position 10 exactly at the destination.

The car at position 5 forms another fleet.

The cars at positions 3 and 0 form another fleet because the car at position 0 catches the fleet ahead.

Total fleets = 3.

## 2. Main Intuition

The key is to calculate the time required by each car to reach the target.

Formula:

```text
Time = (target - position) / speed
```

Example:

```text
Target = 12
Position = 8
Speed = 4

Time = (12 - 8) / 4
     = 1
```

Now comes the important observation:

**Process cars from the closest position to the target to the farthest position.**

Why?

Because we need to know whether a car behind can catch the fleet ahead.

- If the car behind takes less than or equal to the time of the fleet ahead, it catches up and joins that fleet.
- If the car behind takes more time, it cannot catch up before the target and forms a new fleet.

## 3. Visual Explanation

Consider:

```text
target = 12

Position:  10    8     5     3     0
Speed:      2    4     1     3     1
```

### Step 1: Calculate arrival times

```text
Position 10: (12 - 10) / 2 = 1
Position  8: (12 -  8) / 4 = 1
Position  5: (12 -  5) / 1 = 7
Position  3: (12 -  3) / 3 = 3
Position  0: (12 -  0) / 1 = 12
```

### Step 2: Sort by position (descending)

```text
Position:       10    8    5    3    0
Arrival time:    1    1    7    3   12
```

### Step 3: Identify fleets

Start with the car at position 10.

Its arrival time is 1.

The car at position 8 also takes 1, so it joins the fleet ahead.

The car at position 5 takes 7, so it forms a new fleet.

The car at position 3 takes 3, which is less than 7. It catches the fleet ahead and joins it.

The car at position 0 takes 12, which is greater than 7. It cannot catch the fleet ahead, so it forms another fleet.

Final fleet arrival times:

```text
1 → 7 → 12
```

Total fleets = 3.

## 4. C++ Solution — Sorting + Greedy

```cpp
class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {

        int n = position.size();

        vector<pair<int, double>> cars;

        // Step 1: Store position and arrival time
        for (int i = 0; i < n; i++) {

            double time = (double)(target - position[i]) / speed[i];

            cars.push_back({position[i], time});
        }

        // Step 2: Sort cars by position in descending order
        sort(cars.rbegin(), cars.rend());

        // Step 3: Count fleets
        int fleets = 0;
        double lastTime = 0.0;

        for (auto& car : cars) {

            double currentTime = car.second;

            // A new fleet is formed
            if (currentTime > lastTime) {

                fleets++;
                lastTime = currentTime;
            }
        }

        return fleets;
    }
};
```

## 5. Understanding the Important Code

### A. Why use pair?

```cpp
vector<pair<int, double>> cars;
```

We store:

```text
{position, arrival time}
```

This helps us sort cars by position while retaining their arrival times.

### B. Why calculate time using double?

```cpp
double time = (double)(target - position[i]) / speed[i];
```

Arrival time can be fractional.

For example:

```text
Distance = 10
Speed = 3

Time = 3.3333...
```

Using integer division would lose this information and could produce incorrect fleet counts.

### C. Why sort in descending order?

```cpp
sort(cars.rbegin(), cars.rend());
```

We process the car closest to the target first.

This lets us determine whether each car behind can catch the fleet ahead.

### D. Why compare with `lastTime`?

```cpp
if (currentTime > lastTime) {
    fleets++;
    lastTime = currentTime;
}
```

There are two possibilities:

**Case 1: Current time is greater**

The car behind takes longer to reach the target.

It cannot catch the fleet ahead.

Therefore, a new fleet is formed.

**Case 2: Current time is less than or equal to lastTime**

The car behind is faster or reaches the target at the same time.

It catches the fleet ahead and joins it.

No new fleet is counted.

## 6. Dry Run

Input:

```text
target = 10

position = [6, 8]
speed    = [3, 2]
```

Calculate arrival times:

| Position | Speed | Arrival time |
|---:|---:|---:|
| 8 | 2 | 1 |
| 6 | 3 | 1.33 |

Sorted order:

```text
Position:      8    6
Arrival time:  1   1.33
```

Initially:

```text
fleets = 0
lastTime = 0
```

**Iteration 1:**

```text
currentTime = 1

1 > 0 → True

fleets = 1
lastTime = 1
```

**Iteration 2:**

```text
currentTime = 1.33

1.33 > 1 → True

fleets = 2
lastTime = 1.33
```

Final answer:

```text
2 fleets
```

The car at position 6 cannot catch the car at position 8.

## 7. Complexity Analysis

Let `n` be the number of cars.

| Operation | Complexity |
|---|---|
| Calculate arrival times | O(n) |
| Sort cars | O(n log n) |
| Count fleets | O(n) |
| **Total time** | **O(n log n)** |
| Auxiliary space | O(n) |

The additional vector stores the position and arrival time of every car.

## 8. Alternative Approach — Monotonic Stack

The same problem can be solved using a stack of arrival times.

After sorting cars by descending position:

1. Calculate each car's arrival time.
2. If the stack is empty or the current time is greater than the top time, push it.
3. Otherwise, the car joins the fleet ahead, so do not push it.
4. The stack size is the number of fleets.

```cpp
class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {

        int n = position.size();

        vector<pair<int, double>> cars;

        for (int i = 0; i < n; i++) {
            double time = (double)(target - position[i]) / speed[i];
            cars.push_back({position[i], time});
        }

        sort(cars.rbegin(), cars.rend());

        stack<double> st;

        for (auto& car : cars) {

            double time = car.second;

            if (st.empty() || time > st.top()) {
                st.push(time);
            }
        }

        return st.size();
    }
};
```

Both approaches have:

- Time: O(n log n)
- Auxiliary space: O(n)

The first approach is simpler because it uses only a fleet counter and the previous fleet's arrival time.

## 9. Common Interview Mistakes

1. Sorting positions in ascending order instead of descending order.
2. Using integer division instead of floating-point division.
3. Counting every car as a separate fleet.
4. Thinking that a faster car always forms a separate fleet.
5. Forgetting that cars cannot pass each other.
6. Treating equal arrival times as separate fleets.
7. Comparing speeds instead of comparing arrival times.

## 10. C++ Concepts to Remember

- `vector<pair<int, double>>`: Stores position and arrival time.
- `sort(cars.rbegin(), cars.rend())`: Sorts pairs in descending lexicographical order.
- `double`: Preserves fractional arrival times.
- `pair`: Stores two related values.
- `stack<double>`: Maintains fleet arrival times in the alternative approach.

## 11. Pattern Recognition

Whenever a problem involves:

- Cars moving toward the same destination.
- Cars unable to overtake one another.
- Faster objects catching slower objects.
- Counting groups based on arrival times.

Think:

**Sorting + Greedy + Arrival Time**

```text
Calculate Arrival Time
        ↓
Sort by Position (Descending)
        ↓
Process Closest Car First
        ↓
Compare Arrival Times
        ↓
Greater Time → New Fleet
        ↓
Smaller or Equal Time → Join Existing Fleet
```

## 12. 30-Second Interview Revision

**Question:** How do you calculate the number of car fleets?

**Answer:**

"I calculate the arrival time of every car using distance divided by speed. Then I sort the cars by starting position in descending order so that I process the closest car to the target first. I maintain the arrival time of the fleet ahead. If a car behind takes more time, it cannot catch up and forms a new fleet. Otherwise, it joins the existing fleet. The solution takes O(n log n) time due to sorting and O(n) auxiliary space."

**Remember:**

`Arrival Time = (Target - Position) / Speed`

`Sort Descending → Compare Times → Count New Fleets`

# 44. Largest Rectangle in Histogram

## 1. Problem Understanding

You are given an integer array `heights`, where each element represents the height of a histogram bar.

Every bar has a width of `1`.

Find the area of the largest rectangle that can be formed inside the histogram.

### Example 1

```text
Input:
heights = [2, 1, 5, 6, 2, 3]

Output:
10
```

Explanation:

The largest rectangle is formed using bars with heights `5` and `6`.

```text
        ┌───┐
    ┌───┤   │
    │   │   │
────┴───┴───┴────

Height = 5
Width  = 2

Area = 5 × 2 = 10
```

### Example 2

```text
Input:
heights = [2, 4]

Output:
4
```

## 2. Main Intuition

The biggest challenge is determining how wide a rectangle can extend for each bar.

Consider:

```text
heights = [2, 1, 5, 6, 2, 3]
```

For a bar of height `5`:

- The nearest smaller bar on the left has height `1`.
- The nearest smaller bar on the right has height `2`.
- Therefore, the rectangle can extend across bars `5` and `6`.

Area:

```text
5 × 2 = 10
```

### Important Formula

For every bar:

```text
Width = Right Boundary - Left Boundary - 1

Area = Height × Width
```

Where:
- Left Boundary = Index of nearest smaller bar on the left.
- Right Boundary = Index of nearest smaller bar on the right.

We use a **monotonic increasing stack** to find these boundaries efficiently.

## 3. Main Intuition — Monotonic Stack

The stack stores indices of bars in increasing order of height.

When we encounter a bar shorter than the bar at the top of the stack:

1. The current bar becomes the right smaller boundary.
2. Pop the previous bar from the stack.
3. The new stack top becomes its left smaller boundary.
4. Calculate the rectangle area.

Why does this work?

Because a shorter bar prevents a rectangle from extending beyond it while maintaining the popped bar's height.

## 4. Visual Explanation

Input:

```text
heights = [2, 1, 5, 6, 2, 3]
```

Index:

```text
          0  1  2  3  4  5
Height:   2  1  5  6  2  3
```

Consider bar at index `2`, height `5`.

```text
Index:    0  1  2  3  4  5
Height:   2  1  5  6  2  3
             ↑  ↑
          Left  Bar
```

Its nearest smaller boundaries are:

```text
Left smaller index  = 1
Right smaller index = 4
```

Therefore:

```text
Width = 4 - 1 - 1
      = 2

Area = 5 × 2
     = 10
```

This is the maximum rectangle.

## 5. C++ Solution — Optimal Monotonic Stack

```cpp
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {

        int n = heights.size();

        stack<int> st;

        int maxArea = 0;

        for (int i = 0; i <= n; i++) {

            int currHeight = (i == n) ? 0 : heights[i];

            while (!st.empty() &&
                   heights[st.top()] > currHeight) {

                int height = heights[st.top()];
                st.pop();

                int width;

                if (st.empty()) {
                    width = i;
                }
                else {
                    width = i - st.top() - 1;
                }

                int area = height * width;

                maxArea = max(maxArea, area);
            }

            st.push(i);
        }

        return maxArea;
    }
};
```

## 6. Understanding the Important Code

### A. Why store indices instead of heights?

```cpp
stack<int> st;
```

We need indices to calculate rectangle width.

```cpp
width = i - st.top() - 1;
```

If we stored only heights, we would lose the positional information.

### B. Why use `i <= n`?

```cpp
for (int i = 0; i <= n; i++)
```

We deliberately process one extra imaginary bar at the end.

Its height is zero:

```cpp
int currHeight = (i == n) ? 0 : heights[i];
```

Why?

Some bars may remain in the stack after processing the entire histogram.

The imaginary zero-height bar forces all remaining bars to be popped and their areas calculated.

This technique is called a **sentinel**.

### C. Why pop when current height is smaller?

```cpp
while (!st.empty() &&
       heights[st.top()] > currHeight)
```

When the current bar is shorter, the popped bar cannot extend further to the right.

We now know its right boundary.

The stack also tells us its left boundary.

### D. How do we calculate width?

```cpp
if (st.empty()) {
    width = i;
}
else {
    width = i - st.top() - 1;
}
```

After popping the current bar:

- If the stack is empty, there is no smaller bar on the left. The rectangle starts at index `0`.
- Otherwise, the new stack top is the nearest smaller bar on the left.

The current index `i` is the right smaller boundary.

### E. How do we calculate area?

```cpp
int area = height * width;

maxArea = max(maxArea, area);
```

For every popped bar, we calculate the largest rectangle in which that bar is the limiting height.

## 7. Dry Run

Input:

```text
heights = [2, 1, 5, 6, 2, 3]
```

Initially:

```text
Stack = Empty
maxArea = 0
```

| Index | Height | Operation | Maximum area |
|---:|---:|---|---:|
| 0 | 2 | Push index 0 | 0 |
| 1 | 1 | Pop 2: area = 2 × 1; push 1 | 2 |
| 2 | 5 | Push index 2 | 2 |
| 3 | 6 | Push index 3 | 2 |
| 4 | 2 | Pop 6: area = 6 × 1 | 6 |
| 4 | 2 | Pop 5: area = 5 × 2 | 10 |
| 4 | 2 | Push index 4 | 10 |
| 5 | 3 | Push index 5 | 10 |
| 6 | 0 | Pop 3: area = 3 × 1 | 10 |
| 6 | 0 | Pop 2: area = 2 × 4 | 10 |

Final answer:

```text
10
```

The maximum rectangle is formed by heights `5` and `6`.

## 8. Complexity Analysis

Let `n` be the number of histogram bars.

| Complexity | Value | Reason |
|---|---|---|
| Time | O(n) | Every index is pushed and popped at most once |
| Auxiliary space | O(n) | Stack can contain up to n indices |

Although there is a `while` loop inside the `for` loop, the overall complexity is O(n).

Each index enters the stack once and leaves it at most once.

## 9. Alternative Approach — Brute Force

For every bar, expand left and right while the bars are at least as tall as the current bar.

Calculate:

```text
Area = Current Height × Width
```

This approach takes O(n²) time in the worst case.

The monotonic stack improves this to O(n).

## 10. Edge Cases

| Input | Expected output |
|---|---:|
| `[2, 1, 5, 6, 2, 3]` | 10 |
| `[2, 4]` | 4 |
| `[2, 2, 2]` | 6 |
| `[5, 4, 3, 2, 1]` | 9 |
| `[1, 2, 3, 4, 5]` | 9 |
| `[0]` | 0 |
| `[0, 0, 0]` | 0 |

## 11. Common Interview Mistakes

1. Storing heights instead of indices.
2. Forgetting to subtract 1 when calculating width.
3. Calculating width using the wrong stack boundary.
4. Forgetting to process remaining stack elements.
5. Using `>=` or `>` inconsistently without understanding duplicate heights.
6. Assuming the largest rectangle must include the tallest bar.
7. Forgetting that the rectangle height is limited by the shortest bar within its width.

**Important:** The implementation above uses `>` and correctly handles equal-height bars.

## 12. C++ Concepts to Remember

- `stack<int>`: Stores histogram indices.
- `st.top()`: Returns the top index.
- `st.pop()`: Removes the top index.
- `st.push(i)`: Inserts the current index.
- `max(a, b)`: Returns the larger value.
- Ternary operator: `(i == n) ? 0 : heights[i]`.
- Sentinel: An imaginary element used to simplify boundary processing.

## 13. Pattern Recognition

Whenever a problem asks for the largest rectangle or area based on continuous heights, think:

**Monotonic Increasing Stack**

```text
Traverse Histogram
       ↓
Maintain Increasing Heights
       ↓
Current Height Smaller?
       ↓
Pop Previous Bar
       ↓
Find Left and Right Boundaries
       ↓
Calculate Width
       ↓
Calculate Area
       ↓
Update Maximum
```

This problem is closely related to:
- Daily Temperatures
- Next Greater Element
- Trapping Rain Water

## 14. 30-Second Interview Revision

**Question:** How do you find the largest rectangle in a histogram in O(n) time?

**Answer:**

"I use a monotonic increasing stack that stores indices. I traverse the histogram and, whenever the current height is smaller than the height at the stack top, I pop that index. The popped bar's height becomes the rectangle height, the current index gives its right boundary, and the new stack top gives its left boundary. I calculate the width and area and update the maximum. I also process an extra zero-height sentinel at the end to ensure all remaining bars are evaluated. Every index is pushed and popped at most once, giving O(n) time and O(n) auxiliary space."

**Remember:**

`Increasing Stack → Pop Smaller Boundary → Calculate Width → Calculate Area → Update Maximum`
