

# 60. Implement Trie (Prefix Tree)

## 1. Problem Understanding

We need to implement three operations:

| Function | Purpose |
|---|---|
| `insert(word)` | Insert a word into the Trie |
| `search(word)` | Check whether the complete word exists |
| `startsWith(prefix)` | Check whether any inserted word starts with the given prefix |

### Example

```cpp
PrefixTree trie;

trie.insert("apple");

trie.search("apple");    // true
trie.search("app");      // false
trie.startsWith("app");  // true

trie.insert("app");

trie.search("app");      // true
```

**Important difference:**

- `search("app")` checks whether `"app"` is a complete inserted word.
- `startsWith("app")` checks whether `"app"` exists as the beginning of at least one inserted word.

---

## 2. Core Intuition

Imagine inserting these words:

```text
apple
app
bat
ball
```

Instead of storing each word independently, we share common prefixes.

Trie structure:

```text
                 root
                /    \
               a      b
               |      |
               p      a
               |     / \
               p    t   l
              / \       |
             l   END    l
             |          |
             e         END
             |
            END
```

The same prefix characters are shared.

For example:

- `apple` and `app` share `a → p → p`.
- `bat` and `ball` share `b → a`.

Each node contains:
1. References to its children.
2. A boolean indicating whether a complete word ends there.

---

## 3. Trie Node Structure

For this problem, words contain lowercase English letters (`a-z`).

We can represent each node using:

```cpp
struct Node {
    Node* links[26];
    bool isEnd;

    Node() {
        for (int i = 0; i < 26; i++) {
            links[i] = nullptr;
        }

        isEnd = false;
    }
};
```

### Why an array of size 26?

There are 26 lowercase English letters.

We convert a character into an array index:

```cpp
int index = ch - 'a';
```

Example:

| Character | Index |
|---|---:|
| a | 0 |
| b | 1 |
| c | 2 |
| d | 3 |
| z | 25 |

This gives us direct access to the next character's node.

---

## 4. Complete C++ Solution

```cpp
class PrefixTree {
private:

    struct Node {
        Node* links[26];
        bool isEnd;

        Node() {
            for (int i = 0; i < 26; i++) {
                links[i] = nullptr;
            }

            isEnd = false;
        }
    };

    Node* root;

public:

    PrefixTree() {
        root = new Node();
    }

    void insert(string word) {

        Node* node = root;

        for (char ch : word) {

            int index = ch - 'a';

            // Create a node if this character path doesn't exist.
            if (node->links[index] == nullptr) {
                node->links[index] = new Node();
            }

            // Move to the next character node.
            node = node->links[index];
        }

        // Mark the end of a complete word.
        node->isEnd = true;
    }

    bool search(string word) {

        Node* node = root;

        for (char ch : word) {

            int index = ch - 'a';

            // Character path doesn't exist.
            if (node->links[index] == nullptr) {
                return false;
            }

            node = node->links[index];
        }

        // The path exists, but is it a complete word?
        return node->isEnd;
    }

    bool startsWith(string prefix) {

        Node* node = root;

        for (char ch : prefix) {

            int index = ch - 'a';

            // Prefix path doesn't exist.
            if (node->links[index] == nullptr) {
                return false;
            }

            node = node->links[index];
        }

        // Every character in the prefix was found.
        return true;
    }
};
```

---

## 5. Understanding insert() Step by Step

Suppose we insert:

```cpp
trie.insert("apple");
```

Initially:

```text
root
```

We start with:

```cpp
Node* node = root;
```

Now process each character.

| Character | Index (`ch - 'a'`) | Action |
|---|---:|---|
| a | 0 | Create node and move |
| p | 15 | Create node and move |
| p | 15 | Create node and move |
| l | 11 | Create node and move |
| e | 4 | Create node and move |

After processing all characters:

```cpp
node->isEnd = true;
```

This marks that `"apple"` is a complete word.

**Important:** We mark the end only after processing the entire word.

---

## 6. Understanding search()

Suppose we execute:

```cpp
trie.search("app");
```

The Trie contains `"apple"`.

We successfully follow:

```text
root → a → p → p
```

But at the final node:

```cpp
node->isEnd
```

is `false`, because we inserted `"apple"`, not `"app"`.

Therefore:

```text
search("app") = false
```

Now execute:

```cpp
trie.search("apple");
```

We reach the final `e` node, where `isEnd = true`.

Therefore:

```text
search("apple") = true
```

**Key point:** Finding the complete character path is not enough. The final node must also have `isEnd = true`.

---

## 7. Understanding startsWith()

Suppose the Trie contains:

```text
apple
application
```

Now:

```cpp
trie.startsWith("app");
```

We follow:

```text
root → a → p → p
```

Every character exists, so we return `true`.

Unlike `search()`, we do not check `isEnd`.

Why?

Because we only need to confirm that the prefix path exists.

---

## 8. Difference Between search() and startsWith()

This is one of the most important interview concepts.

Suppose we inserted:

```cpp
trie.insert("apple");
```

| Query | search() | startsWith() | Reason |
|---|---|---|---|
| `"apple"` | true | true | Complete word exists |
| `"app"` | false | true | Prefix exists, but not a complete word |
| `"ap"` | false | true | Prefix exists |
| `"bat"` | false | false | No matching path |
| `"applee"` | false | false | Path doesn't exist |

Remember:

```text
search():
Path exists + isEnd == true

startsWith():
Path exists
```

---

## 9. Complexity Analysis

Let `L` be the length of the word or prefix.

| Operation | Time Complexity |
|---|---|
| Constructor | O(1) |
| insert(word) | O(L) |
| search(word) | O(L) |
| startsWith(prefix) | O(L) |

### Space Complexity

Each Trie node contains 26 child pointers and one boolean.

In the worst case, inserting words creates up to O(N) nodes, where N is the total number of characters inserted across all words.

Therefore, auxiliary Trie storage is:

**O(N)**

The fixed array of 26 pointers per node is treated as constant space per node.

---

## 10. Common Interview Mistakes

1. **Forgetting `isEnd`:** Without it, we cannot distinguish a complete word from a prefix.
2. **Checking `isEnd` in `startsWith()`:** This would incorrectly reject valid prefixes.
3. **Not initializing child pointers:** Uninitialized pointers can cause undefined behavior.
4. **Incorrect character indexing:** Use `ch - 'a'` for lowercase English letters.
5. **Creating duplicate nodes unnecessarily:** Reuse existing character paths.
6. **Not moving the pointer:** After checking or creating a child, move to that child.
7. **Marking `isEnd` too early:** Mark it only after processing the entire word.

---

## 11. C++ Concepts to Remember

| Concept | Meaning |
|---|---|
| `Node*` | Pointer to a Trie node |
| `links[26]` | Stores possible next character nodes |
| `nullptr` | No child node exists |
| `ch - 'a'` | Converts character into an array index |
| `isEnd` | Identifies the end of a complete word |
| `new Node()` | Dynamically creates a new Trie node |

---

## 12. Pattern Recognition for Interviews

Think of a Trie when a problem involves:

- Prefix searching.
- Dictionary word lookup.
- Autocomplete suggestions.
- Word insertion and searching.
- Shared prefixes among multiple words.
- Efficient string-based lookups.

Common applications include search suggestions, spell checkers and contact search.

---

## 13. 30-Second Interview Revision

**Q1. What is a Trie?**

A tree-based data structure used to store and search strings efficiently, especially when strings share common prefixes.

**Q2. Why do we use 26 child pointers?**

Because the problem uses lowercase English letters, so each node can have up to 26 possible children.

**Q3. Why do we need `isEnd`?**

To distinguish a complete inserted word from a prefix.

**Q4. Difference between `search()` and `startsWith()`?**

`search()` checks the character path and the end marker. `startsWith()` checks only whether the character path exists.

**Q5. What is the time complexity?**

O(L) for insertion, searching and prefix searching, where L is the length of the input string.

### Final takeaway

```text
TRIE = CHARACTER-BY-CHARACTER TREE

INSERT:
Create missing nodes → mark isEnd = true

SEARCH:
Follow path → check isEnd

STARTSWITH:
Follow path → return true if path exists



# 61. Design Add and Search Words Data Structure

## 1. Problem Understanding

We need to implement two operations:

| Function | Purpose |
|---|---|
| `addWord(word)` | Insert a word into the data structure |
| `search(word)` | Check whether a word exists, supporting `.` as a wildcard |

The wildcard `.` can represent any one lowercase English letter.

### Example

```cpp
WordDictionary dictionary;

dictionary.addWord("bad");
dictionary.addWord("dad");
dictionary.addWord("mad");

dictionary.search("pad"); // false
dictionary.search("bad"); // true
dictionary.search(".ad"); // true
dictionary.search("b.."); // true
dictionary.search("..."); // true
dictionary.search("..");  // false
```

Notice:

- `"bad"` searches for the exact word.
- `".ad"` means any one letter followed by `"ad"`.
- `"b.."` means a word starting with `b` and having two more letters.
- `"..."` means any word of exactly three characters.

---

## 2. Core Intuition

We will use a **Trie**, just like the previous problem.

However, there is one new challenge:

What happens when we encounter `.`?

For a normal character, we follow only one child.

For `.`, we must explore all available children because any letter can match.

This is where DFS (Depth-First Search) comes into the picture.

### Normal search

For:

```text
bad
```

We follow:

```text
root → b → a → d
```

### Wildcard search

For:

```text
.ad
```

At `.` we explore all possible children:

```text
                 root
               /   |   \
              a    b    c ...
              |    |
              d    a
                   |
                   d
```

If any branch successfully matches the remaining characters and ends at a complete word, return `true`.

Otherwise, return `false`.

---

## 3. Trie Node Structure

Each node stores:

1. An array of 26 child pointers.
2. A boolean `isEnd` indicating whether a complete word ends there.

```cpp
struct Node {
    Node* links[26];
    bool isEnd;

    Node() {
        for (int i = 0; i < 26; i++) {
            links[i] = nullptr;
        }

        isEnd = false;
    }
};
```

We convert a character into an index using:

```cpp
int index = ch - 'a';
```

For example:

| Character | Index |
|---|---:|
| a | 0 |
| b | 1 |
| c | 2 |
| z | 25 |

---

## 4. Complete C++ Solution

```cpp
class WordDictionary {
private:

    struct Node {
        Node* links[26];
        bool isEnd;

        Node() {
            for (int i = 0; i < 26; i++) {
                links[i] = nullptr;
            }

            isEnd = false;
        }
    };

    Node* root;

    // DFS helper function
    bool dfs(Node* node, string& word, int index) {

        // All characters have been processed.
        if (index == word.size()) {
            return node->isEnd;
        }

        char ch = word[index];

        // Wildcard: explore every available child.
        if (ch == '.') {

            for (int i = 0; i < 26; i++) {

                if (node->links[i] != nullptr) {

                    if (dfs(node->links[i], word, index + 1)) {
                        return true;
                    }
                }
            }

            return false;
        }

        // Normal character: follow only its matching child.
        int position = ch - 'a';

        if (node->links[position] == nullptr) {
            return false;
        }

        return dfs(node->links[position], word, index + 1);
    }

public:

    WordDictionary() {
        root = new Node();
    }

    void addWord(string word) {

        Node* node = root;

        for (char ch : word) {

            int index = ch - 'a';

            if (node->links[index] == nullptr) {
                node->links[index] = new Node();
            }

            node = node->links[index];
        }

        node->isEnd = true;
    }

    bool search(string word) {

        return dfs(root, word, 0);
    }
};
```

---

## 5. Understanding addWord()

This is almost identical to the `insert()` function from the previous Trie problem.

Suppose:

```cpp
dictionary.addWord("bad");
```

We process:

| Character | Index | Action |
|---|---:|---|
| b | 1 | Create node |
| a | 0 | Create node |
| d | 3 | Create node |

After processing all characters:

```cpp
node->isEnd = true;
```

The Trie now stores `"bad"` as a complete word.

We can insert multiple words while sharing common prefixes.

---

## 6. Understanding search() and DFS

This is the most important part of the problem.

We use:

```cpp
return dfs(root, word, 0);
```

The DFS function receives:

- `node`: Current Trie node.
- `word`: Word or pattern being searched.
- `index`: Current character position.

There are two possible situations.

### Situation A: Normal character

Suppose the search pattern is:

```text
bad
```

At each character, we follow only the matching child.

```cpp
int position = ch - 'a';

if (node->links[position] == nullptr) {
    return false;
}

return dfs(node->links[position], word, index + 1);
```

If the required child doesn't exist, the search fails immediately.

### Situation B: Wildcard character (`.`)

Suppose the search pattern is:

```text
.ad
```

When we encounter `.`, we don't know which character should match.

Therefore, we explore all existing children:

```cpp
for (int i = 0; i < 26; i++) {

    if (node->links[i] != nullptr) {

        if (dfs(node->links[i], word, index + 1)) {
            return true;
        }
    }
}

return false;
```

The important logic:

- Try every available child.
- Continue searching the remaining pattern.
- If any branch succeeds, return `true`.
- If all branches fail, return `false`.

This is DFS with backtracking.

---

## 7. Understanding the Base Case

Consider:

```cpp
if (index == word.size()) {
    return node->isEnd;
}
```

This condition means we have processed every character in the search pattern.

But does that automatically mean the word exists?

No.

We must check:

```cpp
node->isEnd
```

For example, suppose we inserted:

```text
apple
```

Searching:

```text
app
```

The path exists, but `app` is not a complete word.

Therefore, the result must be `false`.

This is why the base case checks `isEnd`.

---

## 8. Complete Dry Run

Suppose we inserted:

```text
bad
dad
mad
```

Now search:

```cpp
dictionary.search(".ad");
```

### Step 1: Start from root

```text
Pattern: .ad
Index:   0
```

The first character is `.`.

So we explore available children.

### Step 2: Try the `b` branch

```text
root → b
```

Next character is `a`.

```text
b → a
```

Next character is `d`.

```text
a → d
```

Now all characters are processed.

```cpp
return node->isEnd;
```

Since `"bad"` was inserted, `isEnd` is `true`.

Therefore:

```text
search(".ad") = true
```

We don't need to explore the remaining branches because one successful branch is enough.

### Another example

Search:

```cpp
dictionary.search("b..");
```

Traversal:

```text
root → b → a → d
```

Both `.` characters can match `a` and `d`.

The final node has `isEnd = true`.

Result:

```text
true
```

---

## 9. Complexity Analysis

Let:

- `L` = length of the inserted or searched word.
- `N` = total number of Trie nodes.
- `26` = number of possible lowercase English characters.

| Operation | Time Complexity |
|---|---|
| `addWord()` | O(L) |
| `search()` without wildcard | O(L) |
| `search()` with wildcards | O(26^L) worst case |

The wildcard search may explore multiple branches at each wildcard position.

The worst-case bound is O(26^L), although actual work is limited by the existing Trie nodes and can be much smaller.

**Space Complexity:**

- Trie storage: O(N).
- DFS recursion stack: O(L).

---

## 10. Common Interview Mistakes

1. **Treating `.` as a normal character:** It represents any one letter, not a literal dot.
2. **Checking only one child for `.`:** We must explore all existing children.
3. **Returning true as soon as a path is found:** The final node must have `isEnd = true`.
4. **Forgetting the base case:** Once all characters are processed, check `isEnd`.
5. **Returning false after the first failed wildcard branch:** Other branches may still produce a match.
6. **Not advancing the index:** Every successful character match must move to `index + 1`.

---

## 11. Previous Trie vs Current Problem

| Feature | Implement Trie | Word Dictionary |
|---|---|---|
| Insert words | Yes | Yes |
| Exact search | Yes | Yes |
| Prefix search | Yes | No |
| Wildcard `.` | No | Yes |
| Search approach | Iterative traversal | DFS + backtracking |

The important new concept is exploring multiple branches when we encounter a wildcard.

---

## 12. Pattern Recognition for Interviews

Think about Trie + DFS when:

- Searching words with wildcard characters.
- One character can match multiple possibilities.
- You need to explore alternative character paths.
- A string search can branch into multiple possible routes.

Similar concepts appear in:

- Word Search.
- Pattern matching.
- Dictionary wildcard search.
- Recursive backtracking problems.

---

## 13. 30-Second Interview Revision

**Q1. Why use a Trie?**

To store words character by character and share common prefixes.

**Q2. What does `.` represent?**

Any single lowercase English character.

**Q3. Why use DFS?**

Because a wildcard can match any available child, so we need to explore multiple possible paths.

**Q4. When does DFS return true?**

When all pattern characters are consumed and the current Trie node has `isEnd = true`.

**Q5. What is the worst-case search complexity?**

O(26^L), where L is the search pattern length.

### Final takeaway

```text
ADD WORD:
Create missing Trie nodes.
Mark the final node as isEnd = true.

SEARCH:
Normal character → Follow one child.
Wildcard (.)     → Explore all existing children.
All characters processed → Check isEnd.
```


```

