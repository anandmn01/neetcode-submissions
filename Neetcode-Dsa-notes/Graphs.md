# Number of Islands — LeetCode 200

- **Difficulty:** Medium
- **Topics:** Graph, DFS, BFS, Matrix, Connected Components
- **Pattern:** Grid Traversal / Connected Components
- **Language:** C++

## 1. Problem Understanding

Given a 2D grid where:

- `'1'` represents land.
- `'0'` represents water.

Return the total number of islands.

An island is formed by connecting adjacent land cells horizontally or vertically.

**Important:**
- Diagonal connections are NOT allowed.
- Water surrounds the entire grid.
- Every connected group of land represents one island.

### Example

```text
Input:

grid = [
    ["0","1","1","1","0"],
    ["0","1","0","1","0"],
    ["1","1","0","0","0"],
    ["0","0","0","0","0"]
]

Output: 1
```

All land cells are connected, so there is only one island.

---

## 2. Brute Force Approach

A simple nested loop can identify land cells, but counting every `'1'` individually gives an incorrect answer.

We must identify connected groups of land cells.

The efficient way to do this is through graph traversal:

1. Traverse every cell.
2. Find an unvisited land cell.
3. Count it as a new island.
4. Explore all connected land cells using DFS or BFS.
5. Mark visited cells.
6. Continue searching for another unvisited land cell.

---

## 3. Optimized Approach — DFS

### Main Idea

Treat the grid as a graph.

- Every land cell is a node.
- Adjacent land cells are connected.
- Every connected component represents one island.

Whenever we discover an unvisited land cell, we:

1. Increment the island count.
2. Start DFS from that cell.
3. Visit all connected land cells.
4. Mark them as visited by changing `'1'` to `'0'`.

### Four Directions

```cpp
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
```

| Direction | Row Change | Column Change |
|---|---:|---:|
| Up | -1 | 0 |
| Down | +1 | 0 |
| Left | 0 | -1 |
| Right | 0 | +1 |

### C++ Code — DFS

```cpp
class Solution {
public:

    void dfs(int i, int j, vector<vector<char>>& grid,
             int m, int n) {

        // Boundary and water check
        if (i < 0 || j < 0 || i >= m || j >= n ||
            grid[i][j] == '0') {
            return;
        }

        // Mark visited
        grid[i][j] = '0';

        // Explore four directions
        dfs(i - 1, j, grid, m, n);
        dfs(i + 1, j, grid, m, n);
        dfs(i, j - 1, grid, m, n);
        dfs(i, j + 1, grid, m, n);
    }

    int numIslands(vector<vector<char>>& grid) {

        int m = grid.size();
        int n = grid[0].size();

        int count = 0;

        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (grid[i][j] == '1') {

                    count++;

                    dfs(i, j, grid, m, n);
                }
            }
        }

        return count;
    }
};
```

### Why Change `'1'` to `'0'`?

```cpp
grid[i][j] = '0';
```

This marks the cell as visited.

It prevents:
- Revisiting the same cell.
- Infinite recursion.
- Counting the same island multiple times.

**Note:** This implementation modifies the original grid.

---

## 4. Step-by-Step Dry Run

Consider:

```text
grid = [
    ["0","1","1","1","0"],
    ["0","1","0","1","0"],
    ["1","1","0","0","0"],
    ["0","0","0","0","0"]
]
```

Initially:

```text
count = 0
```

### Step 1: Find the First Land Cell

At position `(0,1)`:

```text
grid[0][1] = '1'
```

We discover a new island.

```text
count = 1
```

Call:

```cpp
dfs(0, 1, grid, m, n);
```

### Step 2: Explore Connected Cells

DFS visits all connected land cells:

```text
(0,1)
(0,2)
(0,3)
(1,3)
(1,1)
(2,1)
(2,0)
```

All visited cells are changed to `'0'`.

### Step 3: Continue Traversal

The remaining cells are water or already visited.

No additional island is found.

### Final Output

```text
count = 1
```

---

## 5. Alternative Approach — BFS

### Main Idea

Instead of recursion, BFS uses a queue to explore connected land cells.

Whenever we find an unvisited land cell:

1. Increment the island count.
2. Push its coordinates into a queue.
3. Mark it visited immediately.
4. Remove cells from the queue one by one.
5. Explore their four neighbors.
6. Add valid land neighbors to the queue.
7. Continue until the queue becomes empty.

### C++ Code — BFS

```cpp
class Solution {
public:

    int numIslands(vector<vector<char>>& grid) {

        int m = grid.size();
        int n = grid[0].size();

        int count = 0;

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        queue<pair<int, int>> q;

        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (grid[i][j] == '1') {

                    count++;

                    q.push({i, j});
                    grid[i][j] = '0';

                    while (!q.empty()) {

                        auto [x, y] = q.front();
                        q.pop();

                        for (int k = 0; k < 4; k++) {

                            int nx = x + dx[k];
                            int ny = y + dy[k];

                            if (nx >= 0 && ny >= 0 &&
                                nx < m && ny < n &&
                                grid[nx][ny] == '1') {

                                grid[nx][ny] = '0';

                                q.push({nx, ny});
                            }
                        }
                    }
                }
            }
        }

        return count;
    }
};
```

### Why Mark Visited Before Pushing?

```cpp
grid[nx][ny] = '0';
q.push({nx, ny});
```

Marking the cell immediately prevents multiple neighboring cells from adding the same position to the queue.

---

## 6. DFS vs BFS

| Feature | DFS | BFS |
|---|---|---|
| Data structure | Recursion / Stack | Queue |
| Traversal | Depth first | Level by level |
| Time complexity | O(m × n) | O(m × n) |
| Auxiliary space | O(m × n) worst case | O(m × n) worst case |
| Main risk | Recursion stack depth | Queue memory usage |

Both approaches are correct.

For this problem, DFS is a clean and commonly used solution.

---

## 7. Complexity Analysis

Let:

- `m` = Number of rows.
- `n` = Number of columns.

### Time Complexity

```text
O(m × n)
```

Every cell is visited at most once.

Each land cell explores at most four neighbors.

Therefore, the overall time complexity is O(m × n).

### Space Complexity

**DFS:**

```text
O(m × n)
```

In the worst case, all cells contain land and the recursive call stack can grow up to O(m × n).

**BFS:**

```text
O(m × n)
```

In the worst case, the queue can contain O(m × n) cells.

The original grid is modified in both implementations, so a separate visited matrix is not required.

---

## 8. Compilation Error Encountered and Solution

### Error

During submission, the following compilation error appeared:

```text
error: no member named 'numIslands' in 'Solution'

int output = solution.numIslands(grid);
```

### Why Did This Happen?

The editor contained code from another problem:

**Regular Expression Matching — LeetCode 10**

Its required function is:

```cpp
bool isMatch(string s, string p);
```

However, Number of Islands — LeetCode 200 expects:

```cpp
int numIslands(vector<vector<char>>& grid);
```

The judge internally calls:

```cpp
solution.numIslands(grid);
```

Since the `Solution` class did not contain a `numIslands()` method, compilation failed.

### Correct Solution

Use the exact function signature expected by the current problem:

```cpp
int numIslands(vector<vector<char>>& grid)
```

Remove unrelated problem implementations from the editor before submitting.

### Important Lesson

Each LeetCode problem has its own expected function signature.

| Problem | Required Function |
|---|---|
| Number of Islands | `int numIslands(vector<vector<char>>& grid)` |
| Regular Expression Matching | `bool isMatch(string s, string p)` |
| Longest Increasing Subsequence | `int lengthOfLIS(vector<int>& nums)` |
| House Robber | `int rob(vector<int>& nums)` |

**Never combine different problem solutions inside the same submission class unless the problem explicitly requires it.**

---

## 9. Common Mistakes

1. Counting every land cell instead of counting connected components.
2. Forgetting to mark visited cells.
3. Exploring diagonal directions.
4. Incorrect boundary conditions.
5. Incrementing the island count inside DFS instead of when discovering a new component.
6. Marking cells visited only after exploring neighbors.
7. Using integer `1` and `0` instead of character `'1'` and `'0'`.
8. Forgetting that the DFS solution modifies the original grid.
9. Using the wrong function signature during submission.
10. Accidentally submitting code from a different LeetCode problem.

---

## 10. Important Interview Questions

**Q1. Why is Number of Islands a graph problem?**

Each land cell can be treated as a node, and adjacent land cells are connected through edges.

**Q2. What is a connected component?**

A group of nodes where every node is reachable from every other node in that group.

**Q3. Why do we use DFS or BFS?**

Both algorithms explore all reachable land cells from a starting position.

**Q4. Why do we increment the count before calling DFS?**

Finding an unvisited land cell means we have discovered a new island.

**Q5. Why don't we count diagonal connections?**

The problem allows only horizontal and vertical connections.

**Q6. Can we solve this using Union Find?**

Yes. Disjoint Set Union can connect adjacent land cells and count distinct connected components.

**Q7. What happens if the grid contains only water?**

The answer is `0`.

**Q8. What happens if the entire grid contains land?**

All cells belong to one connected component, so the answer is `1`.

**Q9. What is the difference between DFS and BFS?**

DFS explores deeply before backtracking, whereas BFS explores neighboring cells level by level.

**Q10. Why is the time complexity O(m × n)?**

Every cell is visited at most once, and each cell checks at most four neighbors.

---

## 11. Final Revision Notes

### Core Pattern

```text
Grid Traversal → Connected Components → DFS/BFS
```

### Algorithm

```text
1. Traverse every cell.
2. Skip water cells.
3. If an unvisited land cell is found:
      a. Increment island count.
      b. Start DFS/BFS.
      c. Visit all connected land cells.
      d. Mark visited cells.
4. Return island count.
```

### Complexity

| Complexity | DFS | BFS |
|---|---|---|
| Time | O(m × n) | O(m × n) |
| Auxiliary Space | O(m × n) worst case | O(m × n) worst case |

### Key Takeaways

1. Think of the grid as a graph.
2. Every island represents one connected component.
3. Start DFS/BFS only when an unvisited land cell is found.
4. Mark visited cells immediately.
5. Only four directions are allowed.
6. Count connected components, not individual land cells.
7. Always use the function signature required by the current problem.

**Recommended Approach:** DFS
# Max Area of Island — LeetCode 695

- **Difficulty:** Medium
- **Topics:** Graph, DFS, BFS, Matrix, Connected Components
- **Pattern:** Grid Traversal / Connected Components
- **Language:** C++

## 1. Problem Understanding

Given a 2D grid where:

- `1` represents land.
- `0` represents water.

An island is a group of connected land cells.

Two land cells are connected if they are adjacent horizontally or vertically.

The area of an island is the total number of land cells belonging to that island.

Return the **maximum area** among all islands.

If no island exists, return `0`.

**Important:**
- Diagonal connections are NOT allowed.
- Every land cell contributes `1` to the island's area.
- We need the maximum area, not the total number of islands.

### Example

```text
Input:

grid = [
    [0,1,1,0,1],
    [1,0,1,0,1],
    [0,1,1,0,1],
    [0,1,0,0,1]
]

Output: 5
```

The largest connected island contains 5 land cells.

---

## 2. Brute Force Approach

We need to explore every connected group of land cells and calculate its area.

A nested loop alone cannot calculate the area because adjacent land cells belong to the same island.

We need DFS or BFS to explore each connected component.

The traversal approach:

1. Visit every cell in the grid.
2. If the current cell is water, skip it.
3. If it is unvisited land, start DFS.
4. Count all connected land cells.
5. Update the maximum area.
6. Continue until every cell has been processed.

---

## 3. Optimized Approach — DFS

### Main Idea

Use DFS to explore every connected land cell.

Unlike Number of Islands, where we increment a counter once per island, here DFS returns the total number of cells visited in the current island.

### DFS Logic

```cpp
int dfs(int i, int j, vector<vector<int>>& grid) {

    // Boundary and water check
    if (i < 0 || j < 0 ||
        i >= grid.size() || j >= grid[0].size() ||
        grid[i][j] == 0) {

        return 0;
    }

    // Mark visited
    grid[i][j] = 0;

    // Count current cell and explore neighbors
    int area = 1;

    area += dfs(i - 1, j, grid);
    area += dfs(i + 1, j, grid);
    area += dfs(i, j - 1, grid);
    area += dfs(i, j + 1, grid);

    return area;
}
```

### Complete C++ Code — DFS

```cpp
class Solution {
public:

    int dfs(int i, int j, vector<vector<int>>& grid) {

        int m = grid.size();
        int n = grid[0].size();

        // Boundary and water check
        if (i < 0 || j < 0 || i >= m || j >= n ||
            grid[i][j] == 0) {
            return 0;
        }

        // Mark visited
        grid[i][j] = 0;

        // Count current land cell
        int area = 1;

        // Explore four directions
        area += dfs(i - 1, j, grid);
        area += dfs(i + 1, j, grid);
        area += dfs(i, j - 1, grid);
        area += dfs(i, j + 1, grid);

        return area;
    }

    int maxAreaOfIsland(vector<vector<int>>& grid) {

        int m = grid.size();
        int n = grid[0].size();

        int maxArea = 0;

        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (grid[i][j] == 1) {

                    int area = dfs(i, j, grid);

                    maxArea = max(maxArea, area);
                }
            }
        }

        return maxArea;
    }
};
```

### Why Does DFS Return `area`?

```cpp
int area = 1;

area += dfs(i - 1, j, grid);
area += dfs(i + 1, j, grid);
area += dfs(i, j - 1, grid);
area += dfs(i, j + 1, grid);

return area;
```

Every valid land cell contributes `1`.

The recursive calls return the areas of the connected neighboring regions.

Adding them gives the total area of the current island.

---

## 4. Step-by-Step Dry Run

Consider:

```text
grid = [
    [0,1,1,0,1],
    [1,0,1,0,1],
    [0,1,1,0,1],
    [0,1,0,0,1]
]
```

Initially:

```text
maxArea = 0
```

### Step 1: Find the First Island

At position `(0,1)`:

```text
grid[0][1] = 1
```

Start DFS.

The connected land cells are:

```text
(0,1)
(0,2)
(1,2)
(2,2)
(2,1)
(3,1)
```

Therefore:

```text
area = 6
```

Update:

```cpp
maxArea = max(0, 6);
```

Now:

```text
maxArea = 6
```

### Step 2: Find Another Island

At position `(0,4)`:

```text
grid[0][4] = 1
```

Start another DFS.

Connected land cells:

```text
(0,4)
(1,4)
(2,4)
(3,4)
```

Therefore:

```text
area = 4
```

Update:

```cpp
maxArea = max(6, 4);
```

Now:

```text
maxArea = 6
```

### Step 3: Finish Traversal

All remaining cells are either water or already visited.

No larger island exists.

### Final Output

```text
6
```

---

## 5. Alternative Approach — BFS

### Main Idea

Instead of recursion, use a queue to explore connected land cells.

For every unvisited land cell:

1. Start a new BFS.
2. Push its coordinates into a queue.
3. Mark it visited immediately.
4. Initialize `area = 0`.
5. Remove cells from the queue.
6. Increment the area for every processed cell.
7. Explore all four neighboring cells.
8. Add valid land neighbors to the queue.
9. Update the maximum area after BFS finishes.

### C++ Code — BFS

```cpp
class Solution {
public:

    int maxAreaOfIsland(vector<vector<int>>& grid) {

        int m = grid.size();
        int n = grid[0].size();

        int maxArea = 0;

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        queue<pair<int, int>> q;

        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (grid[i][j] == 1) {

                    int area = 0;

                    q.push({i, j});
                    grid[i][j] = 0;

                    while (!q.empty()) {

                        auto [x, y] = q.front();
                        q.pop();

                        area++;

                        for (int k = 0; k < 4; k++) {

                            int nx = x + dx[k];
                            int ny = y + dy[k];

                            if (nx >= 0 && ny >= 0 &&
                                nx < m && ny < n &&
                                grid[nx][ny] == 1) {

                                grid[nx][ny] = 0;

                                q.push({nx, ny});
                            }
                        }
                    }

                    maxArea = max(maxArea, area);
                }
            }
        }

        return maxArea;
    }
};
```

---

## 6. Number of Islands vs Max Area of Island

Both problems use the same connected-components pattern.

| Feature | Number of Islands | Max Area of Island |
|---|---|---|
| LeetCode | 200 | 695 |
| Main objective | Count islands | Find largest island |
| DFS result | Usually `void` | Returns area |
| Main variable | `count` | `maxArea` |
| Update | `count++` | `max(maxArea, area)` |
| Time complexity | O(m × n) | O(m × n) |

### Core Difference

**Number of Islands:**

```cpp
count++;

dfs(i, j, grid);
```

**Max Area of Island:**

```cpp
int area = dfs(i, j, grid);

maxArea = max(maxArea, area);
```

Remember this distinction during interviews.

---

## 7. Complexity Analysis

Let:

- `m` = Number of rows.
- `n` = Number of columns.

### Time Complexity

```text
O(m × n)
```

Every cell is visited at most once.

Each cell checks at most four neighboring cells.

### Space Complexity

**DFS:**

```text
O(m × n)
```

In the worst case, the recursive call stack can grow up to O(m × n).

**BFS:**

```text
O(m × n)
```

In the worst case, the queue can contain O(m × n) cells.

The original grid is modified to mark visited cells, so no separate visited matrix is needed.

---

## 8. Common Mistakes

1. Counting islands instead of calculating their areas.
2. Returning `1` for every DFS call without adding recursive results.
3. Forgetting to mark visited cells.
4. Exploring diagonal directions.
5. Updating `maxArea` before calculating the complete island area.
6. Using `grid[i][j] == '1'` instead of `grid[i][j] == 1` because this problem uses integers.
7. Forgetting to return `0` for invalid cells.
8. Not handling a grid containing only water.
9. Accidentally counting the same land cell more than once.

---

## 9. Important Interview Questions

**Q1. What is the difference between Number of Islands and Max Area of Island?**

Number of Islands counts connected components, while Max Area of Island calculates the largest connected component's size.

**Q2. Why does DFS return an integer here?**

Because we need to calculate the number of connected land cells in each island.

**Q3. Why initialize area with 1?**

The current cell itself is a valid land cell, so it contributes one to the total area.

**Q4. Why mark the current cell as visited before exploring neighbors?**

To avoid revisiting the same cell and entering infinite recursion.

**Q5. What happens if the grid contains only zeros?**

The maximum area remains `0`.

**Q6. Can BFS solve this problem?**

Yes. BFS explores connected land cells using a queue and counts the number of visited cells.

**Q7. What is the time complexity?**

O(m × n), because every cell is processed at most once.

**Q8. Can we solve this using Union Find?**

Yes. Union Find can connect adjacent land cells and track the size of each connected component.

---

## 10. Final Revision Notes

### Core Pattern

```text
Grid Traversal → Connected Components → DFS/BFS → Component Size
```

### Algorithm

```text
1. Initialize maxArea = 0.
2. Traverse every grid cell.
3. If the current cell is land:
      a. Start DFS/BFS.
      b. Calculate the connected component's area.
      c. Update maxArea.
4. Return maxArea.
```

### Complexity

| Complexity | DFS | BFS |
|---|---|---|
| Time | O(m × n) | O(m × n) |
| Auxiliary Space | O(m × n) worst case | O(m × n) worst case |

### Key Takeaways

1. Think of the grid as a graph.
2. Every island is a connected component.
3. DFS returns the total area of one island.
4. Every valid land cell contributes `1`.
5. Update the maximum only after calculating the complete island area.
6. Mark visited cells immediately.
7. Only four directions are allowed.

**Recommended Approach:** DFS

**Interview Pattern:** Graph Traversal / Connected Components / Grid DFS

# Clone Graph — LeetCode 133

- **Difficulty:** Medium
- **Topics:** Graph, DFS, BFS, Hash Map
- **Pattern:** Graph Traversal + Deep Copy
- **Language:** C++

## 1. Problem Understanding

Given a node of a connected undirected graph, return a **deep copy** of the entire graph.

Each node contains:

- An integer value (`val`).
- A list of its neighboring nodes (`neighbors`).

The graph is represented using an adjacency list.

### What is a Deep Copy?

A deep copy means creating completely new nodes with the same values and connections as the original graph.

For example:

Original graph:

```text
1 ---- 2
|      |
|      |
4 ---- 3
```

Cloned graph:

```text
1' ---- 2'
|        |
|        |
4' ---- 3'
```

The cloned nodes have the same values and relationships, but they occupy different memory locations.

**Important:**
- Do not return the original node.
- Do not reuse original neighbors.
- Every original node must have exactly one corresponding cloned node.
- Cycles must be handled correctly.

---

## 2. Brute Force Approach

A simple traversal without a mapping is not sufficient.

Consider this graph:

```text
1 ---- 2
|      |
|      |
4 ---- 3
```

While traversing node `1`, we visit node `2`.

From node `2`, we can reach node `1` again.

If we keep creating cloned nodes without remembering previously visited nodes, we may:

- Create duplicate cloned nodes.
- Enter infinite recursion because of cycles.
- Lose the correct graph connections.

Therefore, we need a way to remember which original node has already been cloned.

---

## 3. Optimized Approach — DFS + Hash Map

### Main Idea

Use:

1. DFS to traverse the graph.
2. A Hash Map to maintain the relationship between original nodes and cloned nodes.

We use:

```cpp
unordered_map<Node*, Node*> mp;
```

Meaning:

```text
Original Node Address → Cloned Node Address
```

### DFS Logic

For every original node:

**Step 1:** Check whether the node is `nullptr`.

If yes, return `nullptr`.

**Step 2:** Check whether the node already exists in the map.

If it exists, return its previously created clone.

**Step 3:** Create a new node with the same value.

```cpp
Node* clone = new Node(node->val);
```

**Step 4:** Store the original-to-clone mapping immediately.

```cpp
mp[node] = clone;
```

**Step 5:** Traverse all original neighbors using DFS.

For every neighbor, recursively obtain its clone and add it to the current cloned node's neighbor list.

```cpp
for (Node* neighbor : node->neighbors) {
    clone->neighbors.push_back(dfs(neighbor));
}
```

**Step 6:** Return the cloned node.

### Complete C++ Code — DFS

```cpp
class Solution {
public:

    unordered_map<Node*, Node*> mp;

    Node* dfs(Node* node) {

        // Base case
        if (node == nullptr) {
            return nullptr;
        }

        // Already cloned
        if (mp.find(node) != mp.end()) {
            return mp[node];
        }

        // Create cloned node
        Node* clone = new Node(node->val);

        // Store mapping before exploring neighbors
        mp[node] = clone;

        // Clone all neighbors
        for (Node* neighbor : node->neighbors) {

            clone->neighbors.push_back(dfs(neighbor));
        }

        return clone;
    }

    Node* cloneGraph(Node* node) {

        mp.clear();

        return dfs(node);
    }
};
```

---

## 4. Why Do We Store the Mapping Before DFS?

This is the most important concept in this problem.

Consider:

```text
1 ---- 2
|      |
|      |
4 ---- 3
```

Suppose DFS starts from node `1`.

1. Create clone of node `1`.
2. Store `mp[1] = clone1`.
3. Visit node `2`.
4. Create clone of node `2`.
5. Store `mp[2] = clone2`.
6. Visit node `3`.
7. Create clone of node `3`.
8. Store `mp[3] = clone3`.
9. Visit node `4`.
10. Create clone of node `4`.
11. Store `mp[4] = clone4`.

Now DFS reaches node `1` again through node `4`.

The map already contains node `1`.

Therefore:

```cpp
if (mp.find(node) != mp.end()) {
    return mp[node];
}
```

We return the existing clone instead of creating another one.

This prevents infinite recursion and duplicate nodes.

**Interview takeaway:** Always save the newly created node in the map before recursively exploring its neighbors.

---

## 5. Step-by-Step Dry Run

Consider the graph:

```text
1 ---- 2
|      |
|      |
4 ---- 3
```

Assume adjacency lists:

```text
1 -> [2, 4]
2 -> [1, 3]
3 -> [2, 4]
4 -> [1, 3]
```

Initially:

```text
mp = {}
```

### Step 1: Start DFS from Node 1

Create:

```cpp
clone1 = new Node(1);
mp[1] = clone1;
```

Map:

```text
1 -> clone1
```

Visit neighbor `2`.

### Step 2: Visit Node 2

Create:

```cpp
clone2 = new Node(2);
mp[2] = clone2;
```

Map:

```text
1 -> clone1
2 -> clone2
```

Node `2` has neighbors `[1, 3]`.

First visit node `1`.

Node `1` is already present in the map, so return `clone1`.

Now visit node `3`.

### Step 3: Visit Node 3

Create:

```cpp
clone3 = new Node(3);
mp[3] = clone3;
```

Node `3` has neighbors `[2, 4]`.

Node `2` is already cloned, so return `clone2`.

Now visit node `4`.

### Step 4: Visit Node 4

Create:

```cpp
clone4 = new Node(4);
mp[4] = clone4;
```

Node `4` has neighbors `[1, 3]`.

Both nodes are already cloned.

Return:

```text
clone1
clone3
```

### Final Cloned Adjacency List

```text
clone1 -> [clone2, clone4]
clone2 -> [clone1, clone3]
clone3 -> [clone2, clone4]
clone4 -> [clone1, clone3]
```

Every original node has exactly one corresponding cloned node.

---

## 6. Alternative Approach — BFS + Hash Map

We can also solve this problem using BFS and a queue.

### Main Idea

1. Create a clone of the starting node.
2. Store its mapping.
3. Push the original node into the queue.
4. Process nodes level by level.
5. For every original neighbor:
   - If it has not been cloned, create its clone.
   - Store its mapping.
   - Push the original neighbor into the queue.
   - Connect the current cloned node to the neighbor's clone.
6. Return the cloned starting node.

### Complete C++ Code — BFS

```cpp
class Solution {
public:

    Node* cloneGraph(Node* node) {

        if (node == nullptr) {
            return nullptr;
        }

        unordered_map<Node*, Node*> mp;

        queue<Node*> q;

        // Create clone of starting node
        Node* clone = new Node(node->val);

        mp[node] = clone;

        q.push(node);

        while (!q.empty()) {

            Node* curr = q.front();
            q.pop();

            for (Node* neighbor : curr->neighbors) {

                // Create clone if not already created
                if (mp.find(neighbor) == mp.end()) {

                    mp[neighbor] = new Node(neighbor->val);

                    q.push(neighbor);
                }

                // Connect cloned nodes
                mp[curr]->neighbors.push_back(mp[neighbor]);
            }
        }

        return clone;
    }
};
```

### DFS vs BFS

| Feature | DFS | BFS |
|---|---|---|
| Traversal | Recursion | Queue |
| Data structure | Hash Map | Hash Map + Queue |
| Handles cycles | Yes | Yes |
| Deep copy | Yes | Yes |
| Time complexity | O(V + E) | O(V + E) |
| Auxiliary space | O(V) | O(V) |

**Recommended Approach:** DFS + Hash Map

DFS is concise and helps understand recursive graph traversal. BFS is equally valid.

---

## 7. Complexity Analysis

Let:

- `V` = Number of vertices.
- `E` = Number of edges.

### Time Complexity

```text
O(V + E)
```

Why?

- Every original node is cloned exactly once.
- Every adjacency-list entry is processed once.
- Hash Map lookups and insertions take O(1) average time.

### Space Complexity

```text
O(V)
```

Why?

- Hash Map stores one mapping for every vertex.
- DFS recursion stack can reach O(V) in the worst case.
- BFS queue can also contain O(V) nodes.

The cloned graph itself requires O(V + E) memory, which is necessary for the output.

---

## 8. Common Mistakes

1. Creating a new clone every time a node is visited.
2. Forgetting to check whether a node is already cloned.
3. Storing the mapping after recursively processing neighbors.
4. Connecting cloned nodes to original neighbors.
5. Forgetting to handle a `nullptr` starting node.
6. Using `node->val` as the map key instead of the original node pointer.
7. Forgetting that multiple nodes can point to the same neighbor.
8. Not preserving cycles and bidirectional connections.
9. Forgetting to return the cloned starting node.

---

## 9. Important Interview Questions

**Q1. Why do we need a Hash Map?**

To maintain a relationship between original nodes and their cloned counterparts. It prevents duplicate cloning and infinite recursion.

**Q2. Why use `Node*` as the Hash Map key?**

Because we need to identify the exact original node, not just its value.

**Q3. Why can't we use node values as keys?**

Node values are not generally guaranteed to uniquely identify node objects in arbitrary graphs. Pointer identity directly identifies the original node.

**Q4. Why store the mapping before visiting neighbors?**

To handle cycles. A neighbor may point back to the current node.

**Q5. What is the difference between shallow copy and deep copy?**

- Shallow copy: Copies references, potentially sharing the original objects.
- Deep copy: Creates independent objects and reconstructs their relationships.

**Q6. Can BFS solve this problem?**

Yes. Use a queue to traverse the graph and a Hash Map to maintain cloned nodes.

**Q7. Why do we use an adjacency list?**

It stores the neighbors of every node and allows us to reconstruct the graph connections.

**Q8. What happens if the input node is null?**

Return `nullptr`.

**Q9. Can a graph contain cycles?**

Yes. An undirected graph commonly contains cycles, which is why the visited mapping is essential.

---

## 10. Final Revision Notes

### Core Pattern

```text
Graph Traversal + Hash Map + Deep Copy
```

### DFS Algorithm

```text
1. If node is null, return null.
2. If node already exists in map, return its clone.
3. Create a new node with the same value.
4. Store original -> clone mapping.
5. Traverse all original neighbors.
6. Add each neighbor's clone to the cloned node.
7. Return the cloned node.
```

### Most Important Code

```cpp
Node* clone = new Node(node->val);

mp[node] = clone;

for (Node* neighbor : node->neighbors) {
    clone->neighbors.push_back(dfs(neighbor));
}
```

### Complexity

| Metric | Complexity |
|---|---|
| Time | O(V + E) |
| Auxiliary Space | O(V) |
| Output Graph Space | O(V + E) |

### Key Takeaways

1. Graph cloning requires creating new node objects.
2. Use `unordered_map<Node*, Node*>` to map original nodes to clones.
3. Store the mapping before exploring neighbors.
4. Return an existing clone when a node is visited again.
5. DFS and BFS both work.
6. Always handle null input.
7. Preserve every edge, including cycles.

**Recommended Approach:** DFS + Hash Map

**Interview Pattern:** Graph Traversal / Deep Copy / Cycle Handling

# Islands and Treasure — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, BFS, Matrix, Multi-Source BFS
- **Pattern:** Multi-Source Shortest Path
- **Language:** C++

## 1. Problem Understanding

We are given an `m × n` grid containing three possible values:

| Value | Meaning |
|---|---|
| `-1` | Water cell — cannot be traversed |
| `0` | Treasure chest |
| `INF` | Land cell — can be traversed |

Here:

```cpp
int INF = 2147483647;
```

We need to fill every land cell with its shortest distance to the nearest treasure chest.

Movement is allowed in four directions:

- Up
- Down
- Left
- Right

We cannot move diagonally or through water.

If a land cell cannot reach any treasure chest, its value must remain `INF`.

**Important:** Modify the original grid in-place.

### Example

```text
Input:

[
    [INF, -1,  0, INF],
    [INF, INF, INF, -1],
    [INF, -1, INF, -1],
    [0,   -1, INF, INF]
]

Output:

[
    [3, -1, 0, 1],
    [2, 2, 1, -1],
    [1, -1, 2, -1],
    [0, -1, 3, 4]
]
```

---

## 2. Brute Force Approach

One possible approach is to calculate the shortest distance separately for every land cell.

For every cell containing `INF`:

1. Start BFS from that cell.
2. Search for the nearest treasure chest.
3. Stop when a treasure chest is found.
4. Store the distance in the original grid.

However, this approach repeats BFS for many cells.

If the grid contains `m × n` cells, running BFS independently for each land cell can take:

```text
O((m × n) × (m × n))
```

This is inefficient.

We can optimize it using Multi-Source BFS.

---

## 3. Optimized Approach — Multi-Source BFS

### Main Idea

Instead of starting BFS separately from every land cell, start BFS simultaneously from **all treasure chests**.

Why?

Every treasure chest has distance `0`.

By placing all treasure chests into the queue initially, BFS expands outward from all sources simultaneously.

Because BFS explores cells level by level, the first time we reach a land cell, we have found its shortest distance to any treasure chest.

### Algorithm

**Step 1: Initialize a queue.**

```cpp
queue<pair<int, int>> q;
```

**Step 2: Traverse the entire grid.**

Find every treasure chest (`0`) and push its coordinates into the queue.

```cpp
if (grid[i][j] == 0) {
    q.push({i, j});
}
```

**Step 3: Define four directions.**

```cpp
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
```

**Step 4: Process the queue using BFS.**

For every cell:

- Remove it from the queue.
- Explore its four neighboring cells.
- Check whether the neighbor is inside the grid.
- Skip water cells (`-1`).
- Skip cells that are not `INF`.
- Update a valid land cell with the current distance plus one.
- Push the updated cell into the queue.

**Step 5: Continue until the queue becomes empty.**

At the end, all reachable land cells contain their shortest distance to the nearest treasure chest.

---

## 4. Complete C++ Code — Multi-Source BFS

```cpp
class Solution {
public:

    void islandsAndTreasure(vector<vector<int>>& grid) {

        int m = grid.size();
        int n = grid[0].size();

        queue<pair<int, int>> q;

        // Step 1: Add all treasure chests to the queue
        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (grid[i][j] == 0) {
                    q.push({i, j});
                }
            }
        }

        // Four directions: up, down, left, right
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        // Step 2: Multi-source BFS
        while (!q.empty()) {

            auto [x, y] = q.front();
            q.pop();

            // Explore four neighboring cells
            for (int k = 0; k < 4; k++) {

                int nx = x + dx[k];
                int ny = y + dy[k];

                // Boundary check
                if (nx < 0 || ny < 0 ||
                    nx >= m || ny >= n) {
                    continue;
                }

                // Skip water and already visited cells
                if (grid[nx][ny] != INT_MAX) {
                    continue;
                }

                // Update shortest distance
                grid[nx][ny] = grid[x][y] + 1;

                // Add updated cell to queue
                q.push({nx, ny});
            }
        }
    }
};
```

---

## 5. Step-by-Step Dry Run

Consider:

```text
grid = [
    [INF, -1,  0, INF],
    [INF, INF, INF, -1],
    [INF, -1, INF, -1],
    [0,   -1, INF, INF]
]
```

### Step 1: Find All Treasure Chests

Traverse the entire grid.

We find two treasure chests:

```text
(0, 2)
(3, 0)
```

Initially, the queue contains:

```text
Q = [(0,2), (3,0)]
```

Both treasure chests already have distance `0`.

### Step 2: Process the First Treasure Chest

Remove:

```text
(0,2)
```

Its reachable land neighbors include:

```text
(0,3)
(1,2)
```

Update their distances:

```text
grid[0][3] = 1
grid[1][2] = 1
```

Push them into the queue.

### Step 3: Process the Second Treasure Chest

Remove:

```text
(3,0)
```

Its reachable land neighbor:

```text
(2,0)
```

Update:

```text
grid[2][0] = 1
```

### Step 4: Continue BFS

The queue now contains:

```text
[(0,3), (1,2), (2,0)]
```

Process these cells and expand to their valid land neighbors.

For example:

```text
grid[1][3] = -1
```

This is water, so we skip it.

For another reachable neighbor:

```text
grid[1][1] = INF
```

Update:

```text
grid[1][1] = 2
```

Continue processing until the queue becomes empty.

### Final Grid

```text
[
    [3, -1, 0, 1],
    [2, 2, 1, -1],
    [1, -1, 2, -1],
    [0, -1, 3, 4]
]
```

Every reachable land cell now contains its shortest distance to the nearest treasure chest.

---

## 6. Why Multi-Source BFS Works

Consider a simple grid:

```text
[INF, INF, 0, INF, INF]
```

If we start BFS from the treasure chest:

```text
Distance 0:       0
Distance 1:    1     1
Distance 2:  2         2
```

Result:

```text
[2, 1, 0, 1, 2]
```

Now imagine multiple treasure chests.

By inserting all treasure chests into the queue initially, BFS expands from all of them simultaneously.

The closest treasure chest reaches a land cell first.

This guarantees the minimum distance because BFS processes nodes in increasing order of distance in an unweighted graph.

---

## 7. Important Code Explanation

### A. Why Add All Treasure Chests Initially?

```cpp
if (grid[i][j] == 0) {
    q.push({i, j});
}
```

Every treasure chest is a source with distance zero.

Starting from all sources allows us to calculate distances to the nearest treasure chest in a single BFS traversal.

### B. Why Check `grid[nx][ny] != INT_MAX`?

```cpp
if (grid[nx][ny] != INT_MAX) {
    continue;
}
```

This condition skips:

- Water cells (`-1`).
- Treasure chests (`0`).
- Land cells that have already received a distance.

Only unvisited land cells should be processed.

### C. Why Update Using `grid[x][y] + 1`?

```cpp
grid[nx][ny] = grid[x][y] + 1;
```

The current cell already contains its shortest distance.

Moving to an adjacent cell adds exactly one step.

Therefore:

```text
Neighbor Distance = Current Distance + 1
```

### D. Why Don't We Need a Separate Visited Array?

Once we update a land cell from `INT_MAX` to its distance, it is no longer equal to `INT_MAX`.

Therefore, the same cell cannot be added again.

The grid itself acts as the visited structure.

---

## 8. Complexity Analysis

Let:

- `m` = Number of rows.
- `n` = Number of columns.

### Time Complexity

```text
O(m × n)
```

Why?

1. We traverse the entire grid once to find treasure chests.
2. Every reachable land cell is added to the queue at most once.
3. Each cell checks at most four neighbors.

Therefore, the overall time complexity is O(m × n).

### Space Complexity

```text
O(m × n)
```

In the worst case, the queue can contain O(m × n) cells.

We do not use an additional visited matrix.

The grid is modified in-place.

---

## 9. Common Mistakes

1. Starting BFS separately from every land cell instead of using all treasure chests as sources.
2. Adding `INF` cells to the initial queue instead of treasure chests.
3. Forgetting to enqueue all treasure chests.
4. Updating water cells.
5. Forgetting boundary checks.
6. Using the wrong direction arrays.
7. Updating distance with the wrong formula.
8. Using a separate visited array unnecessarily.
9. Forgetting that unreachable land cells must remain `INF`.
10. Confusing multi-source BFS with normal single-source BFS.

---

## 10. Important Interview Questions

**Q1. What is Multi-Source BFS?**

It is BFS that starts simultaneously from multiple source nodes instead of a single source.

**Q2. Why do we start from treasure chests instead of land cells?**

Because we need the shortest distance from every land cell to its nearest treasure chest. Starting from all treasure chests lets BFS expand outward and assign the shortest distances.

**Q3. Why does BFS guarantee the shortest distance?**

Because every movement has equal cost (one step), and BFS explores cells level by level.

**Q4. Why don't we need Dijkstra's algorithm?**

All movements have equal cost. BFS is sufficient for shortest paths in an unweighted graph.

**Q5. What happens if there are no treasure chests?**

The queue remains empty, so no land cells are updated. All `INF` values remain unchanged.

**Q6. What happens if a land cell is surrounded by water?**

It cannot reach a treasure chest, so it remains `INF`.

**Q7. Why do we mark cells when adding them to the queue?**

To prevent duplicate processing and ensure each cell enters the queue at most once.

**Q8. What is the time complexity?**

O(m × n).

---

## 11. Comparison with Previous Graph Problems

| Problem | Main Pattern | Starting Point | Objective |
|---|---|---|---|
| Number of Islands | DFS/BFS | Every unvisited land cell | Count connected components |
| Max Area of Island | DFS/BFS | Every unvisited land cell | Find largest component |
| Clone Graph | DFS/BFS + Hash Map | Given starting node | Deep copy graph |
| Islands and Treasure | Multi-Source BFS | All treasure chests | Find nearest-source distances |

### Important Difference

**Number of Islands / Max Area of Island:**

We discover each connected component by starting from an unvisited land cell.

**Islands and Treasure:**

We already know all the source cells (treasure chests). We start BFS from all sources together and calculate the shortest distance to the nearest source.

---

## 12. Final Revision Notes

### Core Pattern

```text
Multi-Source BFS → Shortest Distance in an Unweighted Grid
```

### Algorithm

```text
1. Create an empty queue.
2. Find all treasure chests.
3. Push every treasure chest into the queue.
4. While the queue is not empty:
      a. Remove the front cell.
      b. Explore its four neighbors.
      c. Check boundaries.
      d. Skip water and visited cells.
      e. Update the neighbor distance.
      f. Push the neighbor into the queue.
5. Finish when the queue becomes empty.
```

### Complexity

| Metric | Complexity |
|---|---|
| Time | O(m × n) |
| Auxiliary Space | O(m × n) |
| Extra Visited Matrix | Not required |

### Key Takeaways

1. Identify all source nodes before starting BFS.
2. Insert all sources into the queue initially.
3. BFS finds shortest distances in unweighted graphs.
4. Use the current distance plus one for neighboring cells.
5. The grid itself can act as the visited structure.
6. Water cells must never be traversed.
7. Unreachable land cells remain `INF`.

**Recommended Approach:** Multi-Source BFS

**Interview Pattern:** Graph / BFS / Shortest Path / Multiple Sources


# Rotting Fruit — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, BFS, Matrix, Multi-Source BFS
- **Pattern:** Multi-Source BFS / Level-by-Level Traversal
- **Language:** C++

## 1. Problem Understanding

We are given a 2D grid where each cell represents one of three possible states:

| Value | Meaning |
|---|---|
| `0` | Empty cell |
| `1` | Fresh fruit |
| `2` | Rotten fruit |

Every minute, a fresh fruit becomes rotten if it is horizontally or vertically adjacent to an already rotten fruit.

We need to return the **minimum number of minutes required until no fresh fruit remains**.

If it is impossible to rot every fresh fruit, return `-1`.

### Example

**Input:**

```cpp
grid = {
    {2, 1, 1},
    {1, 1, 0},
    {0, 1, 1}
};
```

**Output:**

```text
4
```

The rotting process happens simultaneously in all four directions.

---

## 2. Brute Force Approach

A possible approach is to simulate the process minute by minute.

For every minute:

1. Traverse the entire grid.
2. Find rotten fruits.
3. Check their four adjacent cells.
4. Convert adjacent fresh fruits into rotten fruits.
5. Repeat until no more fresh fruits can rot.

However, directly changing cells during the same traversal can cause a problem: a fruit that becomes rotten in the current minute might incorrectly rot another fruit immediately.

To avoid this, we would need an additional grid or temporary storage for each minute.

This approach is less efficient because we repeatedly traverse the entire grid.

We can solve the problem more efficiently using Multi-Source BFS.

---

## 3. Optimized Approach — Multi-Source BFS

### Main Idea

All initially rotten fruits act as starting points for BFS.

Instead of starting BFS separately from each rotten fruit, insert **all rotten fruits into the queue initially**.

Then process the queue level by level.

Each BFS level represents one minute.

### Algorithm

**Step 1: Initialize variables**

```cpp
queue<pair<int, int>> q;
int fresh = 0;
```

**Step 2: Traverse the grid**

- If a cell contains `2`, push its coordinates into the queue.
- If a cell contains `1`, increase the fresh fruit count.

**Step 3: Define four directions**

```cpp
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
```

**Step 4: Perform BFS**

While the queue is not empty and fresh fruits remain:

1. Get the current queue size.
2. Process exactly that many cells.
3. For each cell, explore its four neighbors.
4. If a neighbor is inside the grid and contains a fresh fruit:
   - Change it to rotten (`2`).
   - Decrease the fresh fruit count.
   - Push its coordinates into the queue.
5. After processing the complete level, increase minutes by one.

**Step 5: Return the answer**

- If `fresh == 0`, return the number of minutes.
- Otherwise, return `-1`.

---

## 4. Complete C++ Code — Multi-Source BFS

```cpp
class Solution {
public:

    int orangesRotting(vector<vector<int>>& grid) {

        int m = grid.size();
        int n = grid[0].size();

        queue<pair<int, int>> q;

        int fresh = 0;
        int minutes = 0;

        // Step 1: Find all rotten and fresh fruits
        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (grid[i][j] == 2) {
                    q.push({i, j});
                }
                else if (grid[i][j] == 1) {
                    fresh++;
                }
            }
        }

        // Four directions
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        // Step 2: Multi-source BFS
        while (!q.empty() && fresh > 0) {

            int size = q.size();

            // Process one complete minute
            for (int i = 0; i < size; i++) {

                auto [x, y] = q.front();
                q.pop();

                // Explore four neighbors
                for (int k = 0; k < 4; k++) {

                    int nx = x + dx[k];
                    int ny = y + dy[k];

                    // Boundary check
                    if (nx < 0 || ny < 0 ||
                        nx >= m || ny >= n) {
                        continue;
                    }

                    // Only fresh fruits can become rotten
                    if (grid[nx][ny] != 1) {
                        continue;
                    }

                    // Make the fresh fruit rotten
                    grid[nx][ny] = 2;

                    fresh--;

                    q.push({nx, ny});
                }
            }

            // One complete BFS level = one minute
            minutes++;
        }

        // Step 3: Check remaining fresh fruits
        return fresh == 0 ? minutes : -1;
    }
};
```

---

## 5. Step-by-Step Dry Run

Consider:

```cpp
grid = {
    {2, 1, 1},
    {1, 1, 0},
    {0, 1, 1}
};
```

### Initial State — Minute 0

Initially, we have:

- Rotten fruits: 1
- Fresh fruits: 6
- Empty cells: 2

Queue:

```text
[(0,0)]
```

Grid:

```text
2 1 1
1 1 0
0 1 1
```

### Minute 1

Process `(0,0)`.

Its fresh neighbors:

```text
(0,1)
(1,0)
```

Both become rotten.

Queue:

```text
[(0,1), (1,0)]
```

Fresh fruits remaining:

```text
4
```

Grid:

```text
2 2 1
2 1 0
0 1 1
```

### Minute 2

Process the two cells from the previous level.

From `(0,1)`:

```text
(0,2)
(1,1)
```

Both become rotten.

From `(1,0)`, there are no additional fresh neighbors.

Queue:

```text
[(0,2), (1,1)]
```

Fresh fruits remaining:

```text
2
```

Grid:

```text
2 2 2
2 2 0
0 1 1
```

### Minute 3

Process the previous level.

From `(1,1)`, the fresh neighbor is:

```text
(2,1)
```

It becomes rotten.

Queue:

```text
[(2,1)]
```

Fresh fruits remaining:

```text
1
```

Grid:

```text
2 2 2
2 2 0
0 2 1
```

### Minute 4

Process `(2,1)`.

Its fresh neighbor:

```text
(2,2)
```

Becomes rotten.

Queue:

```text
[(2,2)]
```

Fresh fruits remaining:

```text
0
```

Final grid:

```text
2 2 2
2 2 0
0 2 2
```

**Final Answer:**

```text
4
```

All fresh fruits have become rotten after 4 minutes.

---

## 6. Why Do We Use `int size = q.size()`?

This is the most important part of this problem.

```cpp
int size = q.size();

for (int i = 0; i < size; i++) {
    // Process current level
}
```

Why?

Because all fruits that become rotten during the same minute must be processed together.

Suppose the queue initially contains:

```text
[(0,0)]
```

After processing this cell, two new cells enter the queue:

```text
[(0,1), (1,0)]
```

These newly rotten fruits should only spread the infection in the **next minute**.

By storing the queue size before processing the level, we ensure that newly inserted cells are not processed until the next iteration.

Therefore:

```text
One complete BFS level = One minute
```

---

## 7. Important Code Explanation

### A. Why Count Fresh Fruits?

```cpp
int fresh = 0;
```

We need to know when all fresh fruits have become rotten.

Whenever a fresh fruit becomes rotten:

```cpp
fresh--;
```

When:

```cpp
fresh == 0
```

There are no fresh fruits remaining.

### B. Why Mark the Fruit Rotten Immediately?

```cpp
grid[nx][ny] = 2;
```

This prevents the same fruit from being added to the queue multiple times.

The grid itself acts as the visited structure.

### C. Why Use a Separate `minutes` Variable?

```cpp
minutes++;
```

Each complete BFS level represents one minute.

We increase the counter only after processing all cells from the current level.

### D. Why Use `fresh > 0` in the While Condition?

```cpp
while (!q.empty() && fresh > 0)
```

There is no reason to continue BFS after all fresh fruits have become rotten.

This avoids unnecessary processing.

---

## 8. Complexity Analysis

Let:

- `m` = Number of rows.
- `n` = Number of columns.

### Time Complexity

```text
O(m × n)
```

Why?

1. We traverse the grid once to identify fresh and rotten fruits.
2. Each fresh fruit becomes rotten at most once.
3. Each rotten fruit is processed once.
4. Each cell checks at most four neighbors.

Therefore, total time complexity is O(m × n).

### Space Complexity

```text
O(m × n)
```

In the worst case, the queue can contain O(m × n) cells.

No additional visited matrix is required because we modify the original grid.

---

## 9. Common Mistakes

1. Starting BFS from only one rotten fruit instead of all rotten fruits.
2. Forgetting to count the initial fresh fruits.
3. Forgetting to decrease the fresh fruit count after rotting a fruit.
4. Increasing minutes for every individual fruit instead of every BFS level.
5. Processing newly rotten fruits in the same minute.
6. Forgetting to mark a fruit rotten before adding it to the queue.
7. Forgetting boundary checks.
8. Returning `minutes` without checking whether fresh fruits remain.
9. Using diagonal movement instead of only four directions.
10. Forgetting that empty cells (`0`) cannot become rotten.

---

## 10. Important Interview Questions

**Q1. Why is this Multi-Source BFS?**

Because we start BFS from every initially rotten fruit simultaneously.

**Q2. Why does one BFS level represent one minute?**

All fruits at the same BFS level become rotten during the same minute. They can only infect the next level in the following minute.

**Q3. Why do we need to count fresh fruits?**

To determine whether all fresh fruits have become rotten and whether we should continue BFS.

**Q4. What if the grid contains no fresh fruits initially?**

The BFS loop does not execute, and the answer is `0`.

**Q5. What if there are fresh fruits but no rotten fruits?**

The queue remains empty while fresh fruits remain. The answer is `-1`.

**Q6. What if some fresh fruits are isolated by empty cells?**

They cannot become rotten. The answer is `-1`.

**Q7. Why is BFS preferred over DFS?**

We need the minimum number of minutes. BFS processes cells level by level and naturally calculates the minimum number of steps in an unweighted graph.

**Q8. Why do we mark a cell rotten when inserting it into the queue?**

To prevent duplicate insertion and processing.

---

## 11. Comparison with Islands and Treasure

| Feature | Islands and Treasure | Rotting Fruit |
|---|---|---|
| Starting points | All treasure chests | All rotten fruits |
| Objective | Distance to nearest treasure | Time until all fresh fruits rot |
| BFS type | Multi-Source BFS | Multi-Source BFS |
| Level meaning | Distance | Minutes |
| Visited marking | Replace `INF` with distance | Replace `1` with `2` |
| Additional variable | Not required | Fresh fruit count |
| Final result | Updated grid | Number of minutes or `-1` |

### Key Difference

**Islands and Treasure:**

We update every reachable land cell with its shortest distance.

**Rotting Fruit:**

We process the grid level by level and count how many minutes are required to rot every fresh fruit.

---

## 12. Final Revision Notes

### Core Pattern

```text
Multi-Source BFS + Level-by-Level Processing
```

### Algorithm

```text
1. Traverse the grid.
2. Push all rotten fruits into the queue.
3. Count all fresh fruits.
4. While queue is not empty AND fresh > 0:
      a. Store the current queue size.
      b. Process exactly that many cells.
      c. Explore four neighboring cells.
      d. If a neighbor is fresh:
           - Make it rotten.
           - Decrease fresh count.
           - Push it into the queue.
      e. Increase minutes by one.
5. Return minutes if fresh == 0.
6. Otherwise return -1.
```

### Complexity

| Metric | Complexity |
|---|---|
| Time | O(m × n) |
| Auxiliary Space | O(m × n) |
| Extra Visited Matrix | Not required |

### Key Takeaways

1. Insert all initial rotten fruits into the queue.
2. Use BFS level-by-level processing.
3. One complete BFS level represents one minute.
4. Use a fresh fruit counter to track progress.
5. Mark fruits rotten when adding them to the queue.
6. Use four-directional movement.
7. Return `-1` if fresh fruits remain after BFS.

**Recommended Approach:** Multi-Source BFS

**Interview Pattern:** Graph / BFS / Matrix / Level-by-Level Traversal



# Pacific Atlantic Water Flow — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, DFS, BFS, Matrix
- **Pattern:** Multi-Source DFS / Reverse Graph Traversal
- **Language:** C++

## 1. Problem Understanding

We are given an `m x n` matrix called `heights`.

Each cell represents the height of that position above sea level.

The island borders two oceans:

- **Pacific Ocean:** Top and left boundaries.
- **Atlantic Ocean:** Bottom and right boundaries.

Water can flow from one cell to an adjacent cell if the neighboring cell's height is **equal to or lower than** the current cell's height.

We need to return all coordinates from which water can flow into **both the Pacific and Atlantic oceans**.

Movement is allowed in four directions:

- Up
- Down
- Left
- Right

Diagonal movement is not allowed.

### Example

```cpp
heights = {
    {4, 2, 7, 3, 4},
    {7, 4, 6, 4, 7},
    {6, 3, 5, 2, 6},
    {5, 2, 3, 4, 5}
};
```

The answer contains all cells that can reach both oceans.

---

## 2. Important Observation

The straightforward approach would be:

1. Start DFS/BFS from every cell.
2. Check whether water can reach the Pacific Ocean.
3. Check whether water can reach the Atlantic Ocean.
4. If both are reachable, add the cell to the answer.

However, this approach repeats many traversals and is inefficient.

We can optimize it using **Reverse Graph Traversal**.

### The Main Trick

Normally, water flows from a higher or equal cell to a lower or equal cell.

For example:

```text
Height 7 -> Height 5 -> Height 3
```

Instead of starting from every cell and following water downhill, we start from the oceans and move inward.

When moving in reverse, we can move from a cell to its neighbor only if:

```cpp
heights[nx][ny] >= heights[x][y]
```

This is because we are reversing the original water-flow direction.

### Why Does This Work?

Suppose water can flow from cell A to cell B:

```text
A (height 7) -> B (height 5)
```

In reverse traversal, starting from B, we can visit A:

```text
B (height 5) -> A (height 7)
```

Therefore, starting DFS from an ocean boundary allows us to discover every cell from which water could originally flow into that ocean.

---

## 3. Optimized Approach — Two DFS Traversals

We maintain two separate visited matrices:

```cpp
vector<vector<bool>> pacific;
vector<vector<bool>> atlantic;
```

- `pacific[i][j] = true`: This cell can reach the Pacific Ocean.
- `atlantic[i][j] = true`: This cell can reach the Atlantic Ocean.

### Step 1: Initialize Visited Matrices

```cpp
vector<vector<bool>> pacific(
    m, vector<bool>(n, false)
);

vector<vector<bool>> atlantic(
    m, vector<bool>(n, false)
);
```

### Step 2: Start DFS from Pacific Boundaries

Pacific Ocean touches:

- Every cell in the first row.
- Every cell in the first column.

Run DFS from all these boundary cells.

### Step 3: Start DFS from Atlantic Boundaries

Atlantic Ocean touches:

- Every cell in the last row.
- Every cell in the last column.

Run DFS from all these boundary cells.

### Step 4: Reverse DFS Condition

For each neighboring cell, check:

```cpp
heights[nx][ny] >= heights[x][y]
```

Also check:

- The neighbor is inside the grid.
- The neighbor has not already been visited.

### Step 5: Find the Intersection

Traverse the entire grid.

If:

```cpp
pacific[i][j] && atlantic[i][j]
```

add `{i, j}` to the answer.

---

## 4. Complete C++ Code — DFS Approach

```cpp
class Solution {
public:

    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    void dfs(
        int r,
        int c,
        vector<vector<int>>& heights,
        vector<vector<bool>>& visited
    ) {

        visited[r][c] = true;

        int m = heights.size();
        int n = heights[0].size();

        for (int k = 0; k < 4; k++) {

            int nr = r + dx[k];
            int nc = c + dy[k];

            // Boundary check
            if (nr < 0 || nc < 0 ||
                nr >= m || nc >= n) {
                continue;
            }

            // Already visited
            if (visited[nr][nc]) {
                continue;
            }

            // Reverse water-flow condition
            if (heights[nr][nc] >= heights[r][c]) {

                dfs(nr, nc, heights, visited);
            }
        }
    }

    vector<vector<int>> pacificAtlantic(
        vector<vector<int>>& heights
    ) {

        int m = heights.size();
        int n = heights[0].size();

        vector<vector<bool>> pacific(
            m, vector<bool>(n, false)
        );

        vector<vector<bool>> atlantic(
            m, vector<bool>(n, false)
        );

        // Pacific DFS: Top and Left boundaries
        for (int i = 0; i < m; i++) {
            dfs(i, 0, heights, pacific);
        }

        for (int j = 0; j < n; j++) {
            dfs(0, j, heights, pacific);
        }

        // Atlantic DFS: Bottom and Right boundaries
        for (int i = 0; i < m; i++) {
            dfs(i, n - 1, heights, atlantic);
        }

        for (int j = 0; j < n; j++) {
            dfs(m - 1, j, heights, atlantic);
        }

        // Find cells that can reach both oceans
        vector<vector<int>> ans;

        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (pacific[i][j] && atlantic[i][j]) {

                    ans.push_back({i, j});
                }
            }
        }

        return ans;
    }
};
```

---

## 5. Step-by-Step Dry Run

Consider a smaller example:

```cpp
heights = {
    {1, 2, 2},
    {3, 2, 3},
    {2, 4, 5}
};
```

### Step 1: Initial Grid

```text
1  2  2
3  2  3
2  4  5
```

Pacific touches:

- Top row
- Left column

Atlantic touches:

- Bottom row
- Right column

### Step 2: Pacific DFS

Start DFS from all Pacific boundary cells.

Using reverse traversal, we can move to neighboring cells whose heights are greater than or equal to the current cell.

Pacific reachable cells:

```text
P  P  P
P  P  P
P  P  P
```

Here, `P` represents a cell reachable from the Pacific during reverse traversal.

### Step 3: Atlantic DFS

Start DFS from all Atlantic boundary cells.

Atlantic reachable cells:

```text
A  A  A
A  A  A
A  A  A
```

Here, `A` represents a cell reachable from the Atlantic during reverse traversal.

### Step 4: Intersection

A cell belongs in the answer only if both visited matrices contain `true`.

```cpp
if (pacific[i][j] && atlantic[i][j])
```

For this example, all nine cells are reachable from both oceans.

**Important:** This small example is intended to illustrate the traversal and intersection process. The exact visited matrices depend on the height constraints for each transition.

---

## 6. Understanding the Reverse DFS Condition

This is the most important part of the problem.

### Original Water Flow

Water can flow from a higher cell to a lower or equal cell.

```text
7 -> 5 -> 3
```

Condition:

```cpp
heights[nr][nc] <= heights[r][c]
```

### Reverse Water Flow

We start from the ocean and move toward cells that could send water into it.

Therefore, the next cell must have an equal or greater height.

```text
3 -> 5 -> 7
```

Condition:

```cpp
heights[nr][nc] >= heights[r][c]
```

**Interview Insight:** Whenever a problem asks which nodes can reach a particular destination, consider reversing the traversal and starting from the destination.

---

## 7. Complexity Analysis

Let:

- `m` = Number of rows.
- `n` = Number of columns.

### Time Complexity

```text
O(m × n)
```

Why?

1. Pacific DFS visits each cell at most once.
2. Atlantic DFS visits each cell at most once.
3. We traverse the grid once more to find the intersection.

Each cell checks at most four neighbors.

Therefore:

```text
O(m × n) + O(m × n) + O(m × n)
= O(m × n)
```

### Space Complexity

```text
O(m × n)
```

We maintain:

- Pacific visited matrix: O(m × n).
- Atlantic visited matrix: O(m × n).
- DFS recursion stack: O(m × n) in the worst case.
- Result array: O(m × n) in the worst case.

Overall auxiliary space complexity is O(m × n).

---

## 8. Common Mistakes

1. Using the original water-flow condition instead of the reverse condition.
2. Starting DFS from every cell instead of the ocean boundaries.
3. Using only one visited matrix for both oceans.
4. Forgetting that Pacific touches the top and left boundaries.
5. Forgetting that Atlantic touches the bottom and right boundaries.
6. Forgetting to check whether a neighbor is already visited.
7. Allowing diagonal movement.
8. Adding cells to the answer if they reach only one ocean.
9. Forgetting the height comparison.
10. Not handling the case where a cell belongs to both ocean boundaries.

---

## 9. Important Interview Questions

**Q1. Why do we use two visited matrices?**

Because we need to independently track which cells can reach the Pacific and which can reach the Atlantic.

**Q2. Why do we start DFS from ocean boundaries?**

Because every boundary cell is directly connected to its respective ocean. Reverse traversal lets us discover all cells that could flow toward that ocean.

**Q3. Why is the condition `heights[nr][nc] >= heights[r][c]`?**

Because we are traversing in the opposite direction of the original water flow.

**Q4. Why not start DFS from every cell?**

That would repeat many traversals. Starting from the ocean boundaries allows us to process each cell at most once per ocean.

**Q5. Can we use BFS instead of DFS?**

Yes. Both DFS and BFS work because we only need reachability, not shortest distance.

**Q6. What does the intersection of the visited matrices represent?**

Cells that can reach both oceans.

**Q7. What happens when all cells have the same height?**

Every cell can reach both oceans because water can flow between equal-height neighboring cells.

**Q8. What is the graph interpretation?**

Each grid cell represents a graph node. An edge exists between adjacent cells when the original water-flow condition is satisfied. We traverse the reversed graph from the ocean boundaries.

---

## 10. DFS vs BFS

| Feature | DFS | BFS |
|---|---|---|
| Data structure | Recursion / Stack | Queue |
| Traversal | Depth-first | Level-by-level |
| Suitable for reachability | Yes | Yes |
| Shortest path required | Not necessary here | Not necessary here |
| Complexity | O(m × n) | O(m × n) |

Both approaches have the same asymptotic complexity.

DFS is convenient here because we only need to mark reachable cells.

---

## 11. Comparison with Previous Problems

| Problem | Main Pattern | Starting Points | Goal |
|---|---|---|---|
| Number of Islands | DFS / BFS | Every unvisited land cell | Count connected components |
| Max Area of Island | DFS / BFS | Every unvisited land cell | Find largest component |
| Islands and Treasure | Multi-Source BFS | All treasure chests | Calculate shortest distances |
| Rotting Fruit | Multi-Source BFS | All rotten fruits | Calculate minutes |
| Pacific Atlantic Water Flow | Reverse DFS / BFS | Ocean boundaries | Find cells reaching both oceans |

### Important Pattern Recognition

- **Connected components:** Explore from unvisited nodes.
- **Shortest distance from multiple sources:** Multi-Source BFS.
- **Minimum time in an unweighted grid:** BFS level-by-level.
- **Reachability toward multiple destinations:** Consider reverse traversal from each destination.

---

## 12. Final Revision Notes

### Core Pattern

```text
Reverse Graph Traversal + Multi-Source DFS
```

### Algorithm

```text
1. Create two visited matrices:
      pacific
      atlantic

2. Run DFS from all Pacific boundaries:
      - First row
      - First column

3. Run DFS from all Atlantic boundaries:
      - Last row
      - Last column

4. During reverse DFS:
      - Check four directions.
      - Stay inside grid boundaries.
      - Skip visited cells.
      - Move only if next height >= current height.

5. Traverse the complete grid.

6. If pacific[i][j] AND atlantic[i][j]:
      Add {i, j} to the answer.

7. Return answer.
```

### Complexity

| Metric | Complexity |
|---|---|
| Time | O(m × n) |
| Auxiliary Space | O(m × n) |
| Number of visited matrices | 2 |

### Key Takeaways

1. Think in reverse when checking whether cells can reach a destination.
2. Start DFS from the ocean boundaries.
3. Use two separate visited matrices.
4. Reverse the height condition to `>=`.
5. Use the intersection of both visited matrices.
6. DFS and BFS are both valid solutions.

**Recommended Approach:** Reverse DFS

**Interview Pattern:** Graph / Matrix / Multi-Source Traversal / Reverse Graph


# Surrounded Regions — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, DFS, BFS, Matrix
- **Pattern:** Boundary-Based DFS / BFS
- **Language:** C++

## 1. Problem Understanding

We are given an `m x n` matrix called `board` containing two characters:

- `'X'` represents a blocked cell.
- `'O'` represents an open cell.

A region is a group of connected `'O'` cells. Two cells are connected if they are adjacent horizontally or vertically.

A region is **surrounded** if none of its `'O'` cells touch the boundary of the board.

We need to capture all surrounded regions by replacing their `'O'` cells with `'X'`.

The modification must happen in-place.

### Important Observation

An `'O'` connected to any boundary `'O'` cannot be captured.

Why?

Because it has a path to the boundary, so it is not completely surrounded by `'X'`.

Therefore:

- Boundary-connected `'O'` cells must remain `'O'`.
- All other `'O'` cells must become `'X'`.

---

## 2. Example

**Input:**

```cpp
board = {
    {'X','X','X','X'},
    {'X','O','O','X'},
    {'X','X','O','X'},
    {'X','O','X','X'}
};
```

**Output:**

```cpp
{
    {'X','X','X','X'},
    {'X','X','X','X'},
    {'X','X','X','X'},
    {'X','O','X','X'}
};
```

### Explanation

The connected region containing:

```text
O O
  O
```

is completely surrounded by `'X'`, so all its cells become `'X'`.

However, the `'O'` at the bottom boundary remains unchanged because it is connected directly to the boundary.

---

## 3. Brute Force Approach

One possible approach:

1. Traverse the entire board.
2. Whenever we find an unvisited `'O'`, explore its connected component using DFS.
3. Check whether any cell in that component touches the boundary.
4. If the component does not touch the boundary, convert all its cells to `'X'`.
5. Otherwise, preserve all its cells as `'O'`.

This approach works, but we need to store the cells belonging to each component and check whether that component touches the boundary.

We can simplify the solution using boundary-based DFS.

---

## 4. Optimized Approach — Boundary-Based DFS

### Main Idea

Instead of finding surrounded regions directly, identify all regions that are **not surrounded**.

These are the regions connected to the board boundaries.

We temporarily mark their cells with a special character such as `'#'`.

After marking all safe regions:

1. Convert remaining `'O'` into `'X'`.
2. Convert temporary `'#'` cells back into `'O'`.

### Step 1: Traverse the Boundary

Run DFS from every boundary cell containing `'O'`.

There are four boundaries:

- First row
- Last row
- First column
- Last column

### Step 2: Mark Safe Cells

Whenever DFS visits an `'O'`:

```cpp
board[r][c] = '#';
```

Then explore its four adjacent cells.

This marks all boundary-connected `'O'` cells as safe.

### Step 3: Capture Surrounded Regions

Traverse the complete board.

For each cell:

- If it contains `'O'`, convert it into `'X'`.
- If it contains `'#'`, restore it to `'O'`.
- If it contains `'X'`, leave it unchanged.

---

## 5. Complete C++ Code — DFS Approach

```cpp
class Solution {
public:

    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    void dfs(int r, int c, vector<vector<char>>& board) {

        int m = board.size();
        int n = board[0].size();

        // Mark this boundary-connected cell as safe
        board[r][c] = '#';

        // Explore four directions
        for (int k = 0; k < 4; k++) {

            int nr = r + dx[k];
            int nc = c + dy[k];

            // Boundary check
            if (nr < 0 || nc < 0 ||
                nr >= m || nc >= n) {
                continue;
            }

            // Visit only unmarked O cells
            if (board[nr][nc] == 'O') {
                dfs(nr, nc, board);
            }
        }
    }

    void solve(vector<vector<char>>& board) {

        int m = board.size();
        int n = board[0].size();

        // Step 1: DFS from first and last columns
        for (int i = 0; i < m; i++) {

            if (board[i][0] == 'O') {
                dfs(i, 0, board);
            }

            if (board[i][n - 1] == 'O') {
                dfs(i, n - 1, board);
            }
        }

        // Step 2: DFS from first and last rows
        for (int j = 0; j < n; j++) {

            if (board[0][j] == 'O') {
                dfs(0, j, board);
            }

            if (board[m - 1][j] == 'O') {
                dfs(m - 1, j, board);
            }
        }

        // Step 3: Capture surrounded regions
        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (board[i][j] == 'O') {
                    board[i][j] = 'X';
                }

                else if (board[i][j] == '#') {
                    board[i][j] = 'O';
                }
            }
        }
    }
};
```

---

## 6. Step-by-Step Dry Run

Consider the following board:

```cpp
board = {
    {'X','X','X','X'},
    {'X','O','O','X'},
    {'X','X','O','X'},
    {'X','O','X','X'}
};
```

### Step 1: Initial Board

```text
X X X X
X O O X
X X O X
X O X X
```

There are two separate regions of `'O'`.

**Region 1:**

```text
O O
  O
```

**Region 2:**

```text
O
```

### Step 2: Check Boundary Cells

We traverse all four boundaries.

The `'O'` at position `(3,1)` belongs to the bottom boundary.

Therefore, it is safe.

Run DFS from `(3,1)`:

```cpp
board[3][1] = '#';
```

Board becomes:

```text
X X X X
X O O X
X X O X
X # X X
```

There are no additional connected `'O'` cells.

### Step 3: Capture Surrounded Regions

Now traverse the complete board.

Every remaining `'O'` is not connected to any boundary `'O'`.

Therefore, convert them into `'X'`.

```text
X X X X
X X X X
X X X X
X # X X
```

### Step 4: Restore Safe Cells

Convert every temporary `'#'` back into `'O'`.

```text
X X X X
X X X X
X X X X
X O X X
```

**Final Answer:**

```cpp
{
    {'X','X','X','X'},
    {'X','X','X','X'},
    {'X','X','X','X'},
    {'X','O','X','X'}
};
```

---

## 7. Why Do We Use a Temporary Character?

We use:

```cpp
board[r][c] = '#';
```

The temporary character represents a safe `'O'` cell.

We cannot immediately convert boundary-connected `'O'` cells to `'X'`, because they must remain unchanged.

Instead, we temporarily mark them and restore them later.

This allows us to modify the original board without using an additional visited matrix.

---

## 8. Why Do We Start DFS from Boundaries?

This is the most important concept.

Consider:

```text
X X X X X
X O O X X
X X O X X
X X X X X
```

The internal region is completely surrounded.

Now consider:

```text
X X X X X
X O O X X
X X O X X
X O X X X
```

The bottom `'O'` is not connected to the internal region, so the internal region remains surrounded.

However, if the bottom cell were connected to the internal region through adjacent `'O'` cells, the entire connected region would be safe.

Therefore, we only need to discover the `'O'` components connected to the boundaries.

All remaining `'O'` components are surrounded.

---

## 9. Complexity Analysis

Let:

- `m` = Number of rows.
- `n` = Number of columns.

### Time Complexity

```text
O(m × n)
```

Why?

1. We traverse the board boundaries.
2. DFS visits each safe cell at most once.
3. We traverse the complete board once to capture and restore cells.

Each cell is processed a constant number of times.

Therefore, the overall time complexity is `O(m × n)`.

### Space Complexity

```text
O(m × n)
```

The recursive DFS call stack can contain up to `O(m × n)` cells in the worst case.

We do not need a separate visited matrix because we use the original board to mark visited cells.

---

## 10. Common Mistakes

1. Starting DFS from every `'O'` instead of boundary `'O'` cells.
2. Forgetting to check all four boundaries.
3. Forgetting that diagonal connections are not allowed.
4. Converting boundary-connected `'O'` cells into `'X'`.
5. Forgetting to restore temporary `'#'` cells to `'O'`.
6. Forgetting to mark a cell before exploring its neighbors.
7. Using an incorrect boundary condition.
8. Forgetting to handle single-row or single-column boards.
9. Not checking whether a boundary cell contains `'O'` before starting DFS.

---

## 11. Important Interview Questions

**Q1. Why do we start DFS from boundary cells?**

Because any `'O'` connected to a boundary `'O'` cannot be surrounded. We only need to preserve these cells.

**Q2. Why don't we start DFS from every `'O'`?**

Starting from every `'O'` requires checking every connected component separately. Boundary-based DFS directly identifies the cells that must be preserved.

**Q3. Why do we use `'#'`?**

It temporarily marks safe cells so they are not converted into `'X'`.

**Q4. Can we use BFS instead of DFS?**

Yes. BFS can also identify all boundary-connected `'O'` cells. The overall complexity remains the same.

**Q5. Why is a separate visited matrix unnecessary?**

Because changing `'O'` to `'#'` itself marks a cell as visited.

**Q6. What happens if the board contains only `'X'`?**

No DFS is started, and the board remains unchanged.

**Q7. What happens if the board contains only `'O'`?**

Every cell is connected to the boundary, so all cells are preserved as `'O'`.

**Q8. What is the main graph concept used here?**

Connected component traversal from multiple starting points, restricted to boundary cells.

---

## 12. DFS vs BFS

| Feature | DFS | BFS |
|---|---|---|
| Data structure | Recursion / Stack | Queue |
| Traversal | Depth-first | Level-by-level |
| Suitable for connected components | Yes | Yes |
| Time complexity | O(m × n) | O(m × n) |
| Auxiliary space | O(m × n) | O(m × n) |

Both approaches are valid.

DFS is convenient because we only need to identify connected cells, not calculate shortest distances.

---

## 13. Comparison with Previous Problems

| Problem | Main Pattern | Starting Points | Objective |
|---|---|---|---|
| Number of Islands | DFS / BFS | Every unvisited land cell | Count connected components |
| Max Area of Island | DFS / BFS | Every unvisited land cell | Find largest component |
| Pacific Atlantic Water Flow | Reverse DFS / BFS | Ocean boundaries | Find cells reaching both oceans |
| Surrounded Regions | Boundary DFS / BFS | Boundary `'O'` cells | Preserve safe regions |
| Rotting Fruit | Multi-Source BFS | All rotten fruits | Calculate minimum time |

### Pattern Recognition

- **Count connected components:** Start from every unvisited component.
- **Find maximum connected area:** DFS/BFS and count visited cells.
- **Find cells reaching destinations:** Consider reverse traversal from destinations.
- **Preserve boundary-connected regions:** Start DFS/BFS from boundary cells.
- **Calculate minimum time:** Use BFS level-by-level.

---

## 14. Final Revision Notes

### Core Pattern

```text
Boundary-Based DFS + Connected Components
```

### Algorithm

```text
1. Traverse the first and last columns.
2. Traverse the first and last rows.
3. Whenever a boundary cell contains 'O':
      - Start DFS.
      - Mark connected 'O' cells as '#'.

4. Traverse the entire board:
      - If cell == 'O', convert it to 'X'.
      - If cell == '#', restore it to 'O'.

5. The board is modified in-place.
```

### Complexity

| Metric | Complexity |
|---|---|
| Time | O(m × n) |
| Auxiliary Space | O(m × n) worst-case recursion stack |
| Extra Visited Matrix | Not required |

### Key Takeaways

1. Not every `'O'` should be captured.
2. Boundary-connected `'O'` cells are always safe.
3. Start DFS/BFS from the board boundaries.
4. Use a temporary character to mark safe cells.
5. Convert remaining `'O'` cells to `'X'`.
6. Restore temporary markers to `'O'`.
7. Modify the original board in-place.

**Recommended Approach:** Boundary-Based DFS

**Interview Pattern:** Graph / DFS / BFS / Matrix / Connected Components


# Course Schedule — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, DFS, BFS, Topological Sorting
- **Pattern:** Directed Graph Cycle Detection
- **Language:** C++

## 1. Problem Understanding

We are given:

- `numCourses`: Total number of courses, numbered from `0` to `numCourses - 1`.
- `prerequisites`: A 2D array where `prerequisites[i] = [a, b]` means we must complete course `b` before taking course `a`.

We need to determine whether it is possible to finish all courses.

Return:
- `true` if all courses can be completed.
- `false` if completing all courses is impossible.

### Important Observation

This is a **Directed Graph** problem.

For every prerequisite pair:

```cpp
[a, b]
```

We create a directed edge:

```text
b ---> a
```

Because course `b` must be completed before course `a`.

### Main Concept

If the directed graph contains a cycle, we cannot complete all courses.

If the graph is acyclic, all courses can be completed in a valid order.

This is called a **Directed Acyclic Graph (DAG)**.

---

## 2. Example

### Example 1

**Input:**

```cpp
numCourses = 2;
prerequisites = {{1, 0}};
```

Graph:

```text
0 ---> 1
```

Explanation:

- Complete course `0` first.
- Then complete course `1`.

There is no cycle.

**Output:**

```cpp
true
```

### Example 2

**Input:**

```cpp
numCourses = 2;
prerequisites = {{1, 0}, {0, 1}};
```

Graph:

```text
0 ---> 1
^      |
|______|
```

Explanation:

- Course `1` requires course `0`.
- Course `0` requires course `1`.

This creates a cycle.

Neither course can be completed first.

**Output:**

```cpp
false
```

---

## 3. Brute Force Approach

A naive approach would be to repeatedly try different course orders and check whether every prerequisite is satisfied.

However, checking all possible orders can be extremely expensive.

The number of possible orders can grow factorially.

Instead, we use graph cycle detection.

---

## 4. Optimized Approach 1 — DFS Cycle Detection

### Main Idea

Use DFS to detect a cycle in the directed graph.

We maintain a `visited` array with three possible states:

| State | Meaning |
|---|---|
| `0` | Not visited |
| `1` | Currently visiting (DFS path) |
| `2` | Completely processed |

### Why Three States?

In directed graphs, simply checking whether a node was visited is not enough.

We need to identify whether we encounter a node that is already present in the current DFS path.

If we encounter a node with state `1`, we have found a back edge.

This means a cycle exists.

### DFS Logic

For each node:

1. If its state is `1`, a cycle exists.
2. If its state is `2`, it has already been completely processed, so no new cycle is found through it.
3. Mark the current node as `1`.
4. Explore all its neighbors.
5. If any neighbor leads to a cycle, return `true`.
6. After processing all neighbors, mark the node as `2`.
7. Return `false`.

If any DFS call detects a cycle, return `false` from `canFinish()`.

Otherwise, return `true`.

---

## 5. Complete C++ Code — DFS Approach

```cpp
class Solution {
public:

    bool dfs(int node, vector<vector<int>>& adj,
             vector<int>& vis) {

        // Node is already in the current DFS path
        // Therefore, a cycle exists
        if (vis[node] == 1) {
            return true;
        }

        // Node has already been completely processed
        if (vis[node] == 2) {
            return false;
        }

        // Mark as currently visiting
        vis[node] = 1;

        // Explore all neighbors
        for (int neighbor : adj[node]) {

            if (dfs(neighbor, adj, vis)) {
                return true;
            }
        }

        // Mark as completely processed
        vis[node] = 2;

        return false;
    }

    bool canFinish(int numCourses,
                   vector<vector<int>>& prerequisites) {

        vector<vector<int>> adj(numCourses);

        // Build directed graph
        for (auto& p : prerequisites) {

            int a = p[0];
            int b = p[1];

            adj[b].push_back(a);
        }

        vector<int> vis(numCourses, 0);

        // Check every disconnected component
        for (int i = 0; i < numCourses; i++) {

            if (vis[i] == 0) {

                if (dfs(i, adj, vis)) {
                    return false;
                }
            }
        }

        return true;
    }
};
```

---

## 6. Step-by-Step Dry Run — DFS

Consider:

```cpp
numCourses = 3;

prerequisites = {
    {1, 0},
    {2, 1},
    {0, 2}
};
```

### Step 1: Build the Graph

Edges:

```text
0 ---> 1
1 ---> 2
2 ---> 0
```

### Step 2: Initialize Visited Array

```cpp
vis = {0, 0, 0};
```

### Step 3: Start DFS from Node 0

Mark node `0` as currently visiting:

```cpp
vis = {1, 0, 0};
```

Visit neighbor `1`.

Mark node `1`:

```cpp
vis = {1, 1, 0};
```

Visit neighbor `2`.

Mark node `2`:

```cpp
vis = {1, 1, 1};
```

Visit neighbor `0`.

### Step 4: Detect Cycle

Node `0` already has state `1`.

That means node `0` is already present in the current DFS path.

We found a cycle:

```text
0 ---> 1 ---> 2
^             |
|_____________|
```

Therefore:

```cpp
dfs() = true;
```

And:

```cpp
canFinish() = false;
```

### Important Insight

The cycle is detected because we reached a node that is still being processed.

A node with state `2` does not indicate a cycle because its DFS processing is already complete.

---

## 7. Optimized Approach 2 — BFS (Kahn's Algorithm)

Another way to solve this problem is using **Topological Sorting**.

### Main Idea

A topological ordering is a linear ordering of vertices in a directed graph such that for every directed edge:

```text
u ---> v
```

Node `u` appears before node `v`.

Topological sorting is possible only for a DAG.

### What Is Indegree?

Indegree represents the number of incoming edges to a node.

For example:

```text
0 ---> 2
1 ---> 2
```

Indegree:

```text
Node 0 = 0
Node 1 = 0
Node 2 = 2
```

### Kahn's Algorithm Steps

1. Build the directed adjacency list.
2. Calculate the indegree of every node.
3. Push all nodes having indegree `0` into a queue.
4. Remove a node from the queue.
5. Increase the processed course count.
6. Decrease the indegree of all its neighbors.
7. If a neighbor's indegree becomes `0`, push it into the queue.
8. Continue until the queue becomes empty.

At the end:

- If `processed == numCourses`, return `true`.
- Otherwise, return `false`.

Why?

If a cycle exists, the nodes in that cycle will continue to have incoming edges. They can never become available for processing.

---

## 8. Complete C++ Code — BFS Approach

```cpp
class Solution {
public:

    bool canFinish(int numCourses,
                   vector<vector<int>>& prerequisites) {

        vector<vector<int>> adj(numCourses);
        vector<int> indegree(numCourses, 0);

        // Build graph and calculate indegree
        for (auto& p : prerequisites) {

            int a = p[0];
            int b = p[1];

            adj[b].push_back(a);

            indegree[a]++;
        }

        queue<int> q;

        // Add all nodes with indegree 0
        for (int i = 0; i < numCourses; i++) {

            if (indegree[i] == 0) {
                q.push(i);
            }
        }

        int processed = 0;

        // Kahn's Algorithm
        while (!q.empty()) {

            int node = q.front();
            q.pop();

            processed++;

            // Remove outgoing edges
            for (int neighbor : adj[node]) {

                indegree[neighbor]--;

                if (indegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }

        return processed == numCourses;
    }
};
```

---

## 9. Step-by-Step Dry Run — BFS

Consider:

```cpp
numCourses = 4;

prerequisites = {
    {1, 0},
    {2, 0},
    {3, 1},
    {3, 2}
};
```

### Step 1: Build Graph

```text
       0
      / \
     v   v
     1   2
      \ /
       v
       3
```

### Step 2: Calculate Indegree

| Node | Indegree |
|---|---:|
| 0 | 0 |
| 1 | 1 |
| 2 | 1 |
| 3 | 2 |

Initial queue:

```text
Queue = [0]
```

### Step 3: Process Node 0

Remove `0`.

```text
processed = 1
```

Decrease indegree of nodes `1` and `2`.

```text
indegree[1] = 0
indegree[2] = 0
```

Push both into the queue.

```text
Queue = [1, 2]
```

### Step 4: Process Node 1

Remove `1`.

```text
processed = 2
```

Decrease indegree of node `3`.

```text
indegree[3] = 1
```

Queue:

```text
[2]
```

### Step 5: Process Node 2

Remove `2`.

```text
processed = 3
```

Decrease indegree of node `3`.

```text
indegree[3] = 0
```

Push node `3`.

Queue:

```text
[3]
```

### Step 6: Process Node 3

Remove `3`.

```text
processed = 4
```

Queue becomes empty.

### Step 7: Final Check

```cpp
processed == numCourses
```

```cpp
4 == 4
```

Therefore:

```cpp
return true;
```

One valid course order is:

```text
0 -> 1 -> 2 -> 3
```

Another valid order is:

```text
0 -> 2 -> 1 -> 3
```

---

## 10. Complexity Analysis

Let:

- `V` = Number of courses.
- `E` = Number of prerequisite relationships.

### DFS Approach

| Operation | Complexity |
|---|---|
| Build adjacency list | O(V + E) |
| DFS traversal | O(V + E) |
| Visited array | O(V) |
| Recursion stack | O(V) |

**Time Complexity:**

```text
O(V + E)
```

**Auxiliary Space Complexity:**

```text
O(V)
```

The adjacency list itself requires `O(V + E)` space.

### BFS Approach

| Operation | Complexity |
|---|---|
| Build adjacency list | O(V + E) |
| Calculate indegree | O(E) |
| Initialize queue | O(V) |
| Process all nodes and edges | O(V + E) |

**Time Complexity:**

```text
O(V + E)
```

**Auxiliary Space Complexity:**

```text
O(V)
```

The adjacency list requires an additional `O(V + E)` space.

---

## 11. DFS vs BFS

| Feature | DFS | BFS |
|---|---|---|
| Technique | Cycle detection | Kahn's Algorithm |
| Data structure | Recursion / Stack | Queue |
| Main concept | Three-state visited array | Indegree |
| Cycle detection | Back edge to visiting node | Not all nodes processed |
| Time | O(V + E) | O(V + E) |
| Auxiliary space | O(V) | O(V) |
| Produces course order | Not directly | Yes, through processing order |

Both approaches are correct.

### Which One Should You Prefer?

- **DFS:** Useful when the main objective is detecting a cycle.
- **BFS:** Useful when the problem asks for a valid course order or topological ordering.

For this problem, both are good interview solutions.

---

## 12. Common Mistakes

1. Creating the edge in the wrong direction.

   Correct:

   ```cpp
   adj[b].push_back(a);
   ```

   Because `b` must be completed before `a`.

2. Using only a boolean visited array in directed cycle detection.

3. Forgetting the difference between currently visiting and completely processed nodes.

4. Forgetting to check every disconnected component.

5. Incorrectly calculating indegree.

6. Forgetting to decrease the indegree of neighbors during BFS.

7. Returning `true` just because the queue becomes empty.

8. Forgetting that isolated courses must also be counted.

9. Confusing directed and undirected cycle detection.

---

## 13. Important Interview Questions

**Q1. Why is this a directed graph problem?**

Because prerequisite relationships have a direction. Course `b` must be completed before course `a`.

**Q2. Why does a cycle make completing courses impossible?**

A cycle creates circular dependencies. Every course in the cycle depends on another course in the same cycle.

**Q3. What is a DAG?**

A Directed Acyclic Graph is a directed graph containing no cycles.

**Q4. Why do we use three states in DFS?**

To distinguish between:
- Unvisited nodes.
- Nodes currently in the DFS path.
- Nodes whose processing is complete.

**Q5. What does `vis[node] == 1` indicate?**

The node is currently present in the DFS recursion path. Reaching it again indicates a directed cycle.

**Q6. What does `vis[node] == 2` indicate?**

The node and all its outgoing paths have already been processed.

**Q7. What is indegree?**

The number of incoming edges to a vertex.

**Q8. Why does Kahn's Algorithm start with indegree zero?**

A node with no incoming edges has no prerequisite courses remaining, so it can be processed immediately.

**Q9. Why do we compare processed courses with `numCourses`?**

If fewer courses are processed, some nodes remain blocked by a cycle.

**Q10. Can a graph have multiple valid topological orders?**

Yes. If multiple nodes have indegree zero, they can be processed in different valid orders.

---

## 14. Related Problems

| Problem | Main Pattern |
|---|---|
| Course Schedule | Directed Cycle Detection |
| Course Schedule II | Topological Sorting |
| Graph Valid Tree | Connected Components + Cycle Detection |
| Alien Dictionary | Directed Graph + Topological Sorting |
| Minimum Height Trees | Graph + Topological Processing |

### Pattern Recognition

Use DFS with three states when:
- You need to detect cycles in a directed graph.
- You need to identify back edges.
- You need to check whether a directed graph is acyclic.

Use BFS with indegree when:
- You need topological ordering.
- You need to process dependencies.
- You need to detect whether all vertices can be processed.

---

## 15. Final Revision Notes

### Core Concept

```text
Course Schedule = Directed Graph + Cycle Detection
```

### DFS Pattern

```text
1. Build adjacency list: b -> a.
2. Initialize visited array with 0.
3. Start DFS from every unvisited node.
4. Mark current node as 1.
5. If a neighbor has state 1, cycle exists.
6. If a neighbor has state 2, skip it.
7. After processing neighbors, mark current node as 2.
8. If any cycle exists, return false.
9. Otherwise, return true.
```

### BFS Pattern

```text
1. Build adjacency list: b -> a.
2. Calculate indegree of all nodes.
3. Push all indegree-0 nodes into a queue.
4. Process nodes one by one.
5. Decrease indegree of their neighbors.
6. Push neighbors when indegree becomes 0.
7. Count processed nodes.
8. Return processed == numCourses.
```

### Complexity

| Metric | DFS | BFS |
|---|---|---|
| Time | O(V + E) | O(V + E) |
| Auxiliary Space | O(V) | O(V) |
| Cycle Detection | Yes | Yes |
| Topological Order | Not directly | Yes |

### Key Takeaways

1. Prerequisites form a directed graph.
2. Always create the edge from prerequisite to dependent course.
3. A directed cycle means courses cannot all be completed.
4. DFS uses three states for cycle detection.
5. BFS uses indegree and Kahn's Algorithm.
6. Topological sorting is possible only for a DAG.
7. Always account for disconnected components and isolated courses.

**Recommended Approach:** DFS or BFS (Kahn's Algorithm)

**Interview Pattern:** Graph / Directed Cycle Detection / Topological Sorting


# Course Schedule II — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, BFS, DFS, Topological Sorting
- **Pattern:** Directed Graph + Topological Ordering
- **Language:** C++

## 1. Problem Understanding

We are given:

- `numCourses`: Total number of courses, numbered from `0` to `numCourses - 1`.
- `prerequisites`: A 2D array where `prerequisites[i] = [a, b]` means we must complete course `b` before taking course `a`.

We need to return a valid ordering of courses that allows us to finish all courses.

If multiple valid orderings exist, return any one of them.

If completing all courses is impossible because of a cycle, return an empty array.

### Important Observation

This is a **Directed Graph** problem.

For every prerequisite pair:

```cpp
[a, b]
```

Create a directed edge:

```text
b ---> a
```

Because course `b` must be completed before course `a`.

### Main Concept

We need to find a **Topological Ordering** of the directed graph.

Topological sorting is possible only when the directed graph is a DAG (Directed Acyclic Graph).

If a cycle exists, return an empty array.

---

## 2. Examples

### Example 1

**Input:**

```cpp
numCourses = 3;
prerequisites = {{1, 0}};
```

Graph:

```text
0 ---> 1

2
```

Explanation:

- Course `0` must be completed before course `1`.
- Course `2` has no prerequisites.
- Course `2` can appear anywhere in the valid ordering.

One valid output:

```cpp
[0, 1, 2]
```

Another valid output:

```cpp
[0, 2, 1]
```

### Example 2

**Input:**

```cpp
numCourses = 3;
prerequisites = {{0, 1}, {1, 2}, {2, 0}};
```

Graph:

```text
0 ---> 1
^      |
|      v
2 <----
```

There is a directed cycle.

No valid course ordering exists.

**Output:**

```cpp
[]
```

---

## 3. Brute Force Approach

A naive approach would try different permutations of courses and check whether each ordering satisfies all prerequisite relationships.

However, there can be `numCourses!` possible permutations.

This is highly inefficient.

Instead, we use Topological Sorting.

---

## 4. Optimized Approach 1 — BFS (Kahn's Algorithm)

### Main Idea

Kahn's Algorithm uses the concept of **Indegree**.

Indegree = Number of incoming edges to a node.

For example:

```text
0 ---> 2
1 ---> 2
```

Indegree:

```text
Node 0 = 0
Node 1 = 0
Node 2 = 2
```

A node with indegree `0` has no remaining prerequisites, so we can process it immediately.

### Algorithm Steps

1. Create an adjacency list.
2. Calculate the indegree of every course.
3. Push all courses with indegree `0` into a queue.
4. Create an empty result vector.
5. Remove a course from the queue.
6. Add that course to the result.
7. Decrease the indegree of all its neighboring courses.
8. If a neighbor's indegree becomes `0`, push it into the queue.
9. Repeat until the queue becomes empty.
10. If the result contains all courses, return it.
11. Otherwise, return an empty array because a cycle exists.

### Why Does This Work?

A course can be taken only after all its prerequisites are completed.

When its indegree becomes `0`, all its prerequisites have been processed.

Therefore, it is safe to add that course to the result.

If some courses cannot be processed, they are blocked by a cycle.

---

## 5. Complete C++ Code — BFS Approach

```cpp
class Solution {
public:

    vector<int> findOrder(
        int numCourses,
        vector<vector<int>>& prerequisites
    ) {

        vector<vector<int>> adj(numCourses);
        vector<int> indegree(numCourses, 0);

        // Build directed graph
        for (auto& p : prerequisites) {

            int a = p[0];
            int b = p[1];

            adj[b].push_back(a);

            indegree[a]++;
        }

        queue<int> q;

        // Add all courses with indegree 0
        for (int i = 0; i < numCourses; i++) {

            if (indegree[i] == 0) {
                q.push(i);
            }
        }

        vector<int> ans;

        // Kahn's Algorithm
        while (!q.empty()) {

            int node = q.front();
            q.pop();

            // Add course to ordering
            ans.push_back(node);

            // Process neighboring courses
            for (int neighbor : adj[node]) {

                indegree[neighbor]--;

                if (indegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }

        // Cycle detection
        if (ans.size() != numCourses) {
            return {};
        }

        return ans;
    }
};
```

---

## 6. Step-by-Step Dry Run — BFS

Consider:

```cpp
numCourses = 4;

prerequisites = {
    {1, 0},
    {2, 0},
    {3, 1},
    {3, 2}
};
```

### Step 1: Build Graph

```text
       0
      / \
     v   v
     1   2
      \ /
       v
       3
```

Edges:

```text
0 -> 1
0 -> 2
1 -> 3
2 -> 3
```

### Step 2: Calculate Indegree

| Course | Indegree |
|---|---:|
| 0 | 0 |
| 1 | 1 |
| 2 | 1 |
| 3 | 2 |

Initial queue:

```text
Queue = [0]
```

Initial result:

```text
ans = []
```

### Step 3: Process Course 0

Remove `0` from the queue.

Add it to the result:

```text
ans = [0]
```

Decrease indegree of its neighbors:

```text
indegree[1] = 0
indegree[2] = 0
```

Both courses are now ready.

Queue:

```text
[1, 2]
```

### Step 4: Process Course 1

Remove `1`.

```text
ans = [0, 1]
```

Decrease indegree of course `3`:

```text
indegree[3] = 1
```

Queue:

```text
[2]
```

### Step 5: Process Course 2

Remove `2`.

```text
ans = [0, 1, 2]
```

Decrease indegree of course `3`:

```text
indegree[3] = 0
```

Push course `3` into the queue.

Queue:

```text
[3]
```

### Step 6: Process Course 3

Remove `3`.

```text
ans = [0, 1, 2, 3]
```

Queue becomes empty.

### Step 7: Final Check

```cpp
ans.size() == numCourses
```

```cpp
4 == 4
```

Therefore, return:

```cpp
[0, 1, 2, 3]
```

All courses have been successfully ordered.

---

## 7. Cycle Detection in BFS

Consider:

```cpp
numCourses = 3;

prerequisites = {
    {1, 0},
    {2, 1},
    {0, 2}
};
```

Graph:

```text
0 ---> 1 ---> 2
^             |
|_____________|
```

Indegree:

```text
Course 0 = 1
Course 1 = 1
Course 2 = 1
```

Initial queue:

```text
Queue = []
```

No course has indegree `0`.

Therefore, no course can be processed.

Final result:

```text
ans = []
```

Since:

```cpp
ans.size() != numCourses
```

We return:

```cpp
{}
```

This indicates that no valid course ordering exists.

---

## 8. Optimized Approach 2 — DFS Topological Sorting

We can also solve this problem using DFS.

### Main Idea

In DFS topological sorting, we add a node to the result only after all its neighboring nodes have been processed.

We use three states:

| State | Meaning |
|---|---|
| `0` | Not visited |
| `1` | Currently visiting |
| `2` | Completely processed |

### Algorithm Steps

1. Build the adjacency list.
2. Initialize the visited array with `0`.
3. Start DFS from every unvisited node.
4. Mark the current node as `1`.
5. Explore all neighboring nodes.
6. If a neighbor has state `1`, a cycle exists.
7. If a neighbor has state `2`, skip it.
8. After processing all neighbors, mark the current node as `2`.
9. Push the current node into the result.
10. If a cycle exists, return an empty array.
11. Otherwise, reverse the result and return it.

### Why Do We Reverse the Result?

DFS pushes a node after processing all its outgoing neighbors.

For an edge:

```text
0 ---> 1
```

DFS processes node `1` before adding node `0` to the result.

The result therefore contains the reverse of the required topological order.

We reverse it at the end.

---

## 9. Complete C++ Code — DFS Approach

```cpp
class Solution {
public:

    bool dfs(int node,
             vector<vector<int>>& adj,
             vector<int>& vis,
             vector<int>& ans) {

        // Currently visiting: cycle detected
        if (vis[node] == 1) {
            return true;
        }

        // Already completely processed
        if (vis[node] == 2) {
            return false;
        }

        // Mark as currently visiting
        vis[node] = 1;

        for (int neighbor : adj[node]) {

            if (dfs(neighbor, adj, vis, ans)) {
                return true;
            }
        }

        // Mark as completely processed
        vis[node] = 2;

        // Add after processing all neighbors
        ans.push_back(node);

        return false;
    }

    vector<int> findOrder(
        int numCourses,
        vector<vector<int>>& prerequisites
    ) {

        vector<vector<int>> adj(numCourses);

        // Build graph
        for (auto& p : prerequisites) {

            int a = p[0];
            int b = p[1];

            adj[b].push_back(a);
        }

        vector<int> vis(numCourses, 0);
        vector<int> ans;

        // Check all components
        for (int i = 0; i < numCourses; i++) {

            if (vis[i] == 0) {

                if (dfs(i, adj, vis, ans)) {
                    return {};
                }
            }
        }

        // Reverse to get topological ordering
        reverse(ans.begin(), ans.end());

        return ans;
    }
};
```

---

## 10. Complexity Analysis

Let:

- `V` = Number of courses.
- `E` = Number of prerequisite relationships.

### BFS Approach

| Operation | Complexity |
|---|---|
| Build adjacency list | O(V + E) |
| Calculate indegree | O(E) |
| Initialize queue | O(V) |
| Process all vertices and edges | O(V + E) |

**Time Complexity:**

```text
O(V + E)
```

**Auxiliary Space Complexity:**

```text
O(V)
```

The adjacency list requires an additional `O(V + E)` space.

### DFS Approach

| Operation | Complexity |
|---|---|
| Build adjacency list | O(V + E) |
| DFS traversal | O(V + E) |
| Visited array | O(V) |
| Recursion stack | O(V) |
| Result vector | O(V) |

**Time Complexity:**

```text
O(V + E)
```

**Auxiliary Space Complexity:**

```text
O(V)
```

The adjacency list requires an additional `O(V + E)` space.

---

## 11. DFS vs BFS

| Feature | DFS | BFS |
|---|---|---|
| Technique | DFS Topological Sort | Kahn's Algorithm |
| Data structure | Recursion / Stack | Queue |
| Main concept | Three-state visited array | Indegree |
| Cycle detection | Back edge | Processed count |
| Produces ordering | Yes, after reversing | Yes, directly |
| Time complexity | O(V + E) | O(V + E) |
| Auxiliary space | O(V) | O(V) |

### Which Approach Should You Prefer?

**BFS (Kahn's Algorithm)** is a very natural choice for Course Schedule II because it builds the ordering directly.

DFS is also important because it helps understand:
- Directed cycle detection.
- DFS finishing order.
- Topological sorting using recursion.

---

## 12. Common Mistakes

1. Creating the graph edge in the wrong direction.

   Correct:

   ```cpp
   adj[b].push_back(a);
   ```

2. Forgetting to calculate indegree.

3. Forgetting to decrease the indegree of neighbors.

4. Returning the result without checking whether all courses were processed.

5. Forgetting isolated courses.

6. In DFS, forgetting to reverse the result.

7. Using a simple visited array instead of three states for directed cycle detection.

8. Returning a partial ordering when a cycle exists.

9. Confusing Course Schedule with Course Schedule II.

---

## 13. Important Interview Questions

**Q1. What is topological sorting?**

It is a linear ordering of vertices in a directed acyclic graph such that for every edge `u -> v`, vertex `u` appears before vertex `v`.

**Q2. Why is a DAG required for topological sorting?**

A directed cycle creates contradictory ordering requirements. Therefore, a valid topological ordering cannot exist.

**Q3. What is Kahn's Algorithm?**

It is a BFS-based topological sorting algorithm that repeatedly processes vertices with indegree zero.

**Q4. Why do we use a queue?**

The queue stores all currently available courses whose prerequisites have been completed.

**Q5. Why do we return an empty array if the result size is smaller than the number of courses?**

Some courses could not be processed, indicating that a cycle exists.

**Q6. Why do we reverse the DFS result?**

DFS adds each node after processing its outgoing neighbors. Reversing the finishing order gives a valid topological ordering.

**Q7. Can multiple valid course orderings exist?**

Yes. Whenever multiple courses have indegree zero, they can be processed in different orders.

**Q8. What is the difference between Course Schedule and Course Schedule II?**

- Course Schedule: Return whether all courses can be completed.
- Course Schedule II: Return a valid ordering of all courses.

**Q9. Can BFS and DFS both detect cycles in a directed graph?**

Yes. DFS detects a back edge to a currently visiting node, while Kahn's Algorithm detects a cycle when fewer than `V` nodes can be processed.

---

## 14. Related Problems

| Problem | Main Pattern |
|---|---|
| Course Schedule | Directed Cycle Detection |
| Course Schedule II | Topological Sorting |
| Alien Dictionary | Directed Graph + Topological Sorting |
| Graph Valid Tree | Connected Components + Cycle Detection |
| Minimum Height Trees | Graph + Topological Processing |

---

## 15. Final Revision Notes

### Core Concept

```text
Course Schedule II = Directed Graph + Topological Sorting
```

### BFS Pattern — Kahn's Algorithm

```text
1. Build adjacency list: b -> a.
2. Calculate indegree of every node.
3. Push all indegree-0 nodes into a queue.
4. Create an empty result vector.
5. Pop a node and add it to the result.
6. Decrease indegree of all its neighbors.
7. Push neighbors whose indegree becomes 0.
8. Repeat until the queue is empty.
9. If result size == numCourses, return result.
10. Otherwise, return an empty array.
```

### DFS Pattern

```text
1. Build adjacency list: b -> a.
2. Initialize visited array with 0.
3. Start DFS from every unvisited node.
4. Mark current node as 1.
5. If a neighbor has state 1, cycle exists.
6. If a neighbor has state 2, skip it.
7. Process all neighbors.
8. Mark current node as 2.
9. Push current node into result.
10. If a cycle exists, return an empty array.
11. Reverse the result and return it.
```

### Complexity

| Metric | BFS | DFS |
|---|---|---|
| Time | O(V + E) | O(V + E) |
| Auxiliary Space | O(V) | O(V) |
| Cycle Detection | Yes | Yes |
| Topological Ordering | Directly | Reverse finishing order |

### Key Takeaways

1. Course Schedule II is a topological sorting problem.
2. Build edges from prerequisite to dependent course.
3. Kahn's Algorithm uses indegree and a queue.
4. DFS uses three-state cycle detection.
5. BFS produces the ordering directly.
6. DFS requires reversing the finishing order.
7. Always check whether all courses were processed.
8. Return an empty array if a cycle exists.

**Recommended Approach:** BFS — Kahn's Algorithm

**Interview Pattern:** Graph / Topological Sorting / Directed Cycle Detection


# Graph Valid Tree — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, DFS, BFS, Union Find
- **Pattern:** Undirected Graph + Cycle Detection + Connectivity
- **Language:** C++

## 1. Problem Understanding

We are given:

- `n`: Number of nodes labelled from `0` to `n - 1`.
- `edges`: A list of undirected edges connecting pairs of nodes.

We need to determine whether the given graph forms a valid tree.

A graph is a valid tree if it satisfies two conditions:

1. **Connected:** Every node must be reachable from every other node.
2. **Acyclic:** The graph must not contain any cycle.

### Important Tree Properties

A tree with `n` nodes always contains exactly:

```text
n - 1 edges
```

For example:

```text
    0
   /|\
  1 2 3
  |
  4
```

Here:

- Number of nodes = 5
- Number of edges = 4
- The graph is connected.
- There is no cycle.

Therefore, it is a valid tree.

### Important Observation

We can solve this problem using:

1. DFS with cycle detection and connectivity checking.
2. BFS with cycle detection and connectivity checking.
3. Union Find (Disjoint Set Union).

There is also a useful mathematical shortcut:

```text
A graph is a valid tree if:
1. Number of edges == n - 1
2. The graph is connected
```

When both conditions hold, the graph must be acyclic.

---

## 2. Examples

### Example 1

**Input:**

```cpp
n = 5;
edges = {{0,1}, {0,2}, {0,3}, {1,4}};
```

Graph:

```text
       0
      /|\
     1 2 3
     |
     4
```

Output:

```cpp
true
```

Explanation:

- All five nodes are connected.
- There is no cycle.
- Number of edges = `5 - 1 = 4`.

Therefore, it is a valid tree.

### Example 2

**Input:**

```cpp
n = 5;
edges = {
    {0,1},
    {1,2},
    {2,3},
    {1,3},
    {1,4}
};
```

Graph:

```text
    0
    |
    1
   /|\
  2 3 4
   \/
```

Output:

```cpp
false
```

Explanation:

Nodes `1`, `2`, and `3` form a cycle:

```text
1 -> 2 -> 3 -> 1
```

Therefore, it is not a valid tree.

### Example 3 — Disconnected Graph

**Input:**

```cpp
n = 4;
edges = {{0,1}, {2,3}};
```

Graph:

```text
0 --- 1

2 --- 3
```

Output:

```cpp
false
```

Explanation:

There is no cycle, but the graph is disconnected.

A valid tree must be connected.

---

## 3. Brute Force / Initial Observations

Before implementing DFS, we can check the number of edges.

A valid tree must have exactly:

```cpp
edges.size() == n - 1
```

If this condition fails, return `false`.

However, this condition alone is not sufficient.

Consider:

```text
0 --- 1

2 --- 3
```

For `n = 4`, there are exactly three? No: there are only two edges, so this particular graph fails the edge-count check.

A stronger example with the required edge count is:

```text
0 --- 1
|     |
+-----+

2 --- 3
```

Here the graph has four nodes and three edges, but node `3` is disconnected and nodes `0`, `1` form a cycle.

Therefore, we must also check connectivity or detect cycles.

---

## 4. Optimized Approach 1 — DFS

### Main Idea

We use DFS to:

1. Traverse the graph.
2. Detect cycles.
3. Check whether all nodes are visited.

Because this is an **undirected graph**, we must track the parent node.

### Why Do We Need a Parent?

Consider:

```text
0 --- 1
```

Adjacency list:

```text
0 -> 1
1 -> 0
```

Start DFS from node `0`:

- Visit node `0`.
- Move to node `1`.
- From node `1`, we see node `0` again.

Is this a cycle?

No!

Node `0` is simply the parent of node `1`.

Therefore, in undirected graph DFS, we ignore the edge that leads back to the parent.

### Cycle Detection Rule

While exploring a node:

```cpp
if (neighbor != parent && vis[neighbor]) {
    return false;
}
```

This means:

- If the neighbor is already visited.
- And it is not the parent.
- Then we found a cycle.

### Algorithm Steps

1. Build the adjacency list.
2. Create a visited array initialized with `false`.
3. Start DFS from node `0`.
4. Mark the current node as visited.
5. Explore its neighbors.
6. Ignore the parent node.
7. If another visited neighbor is found, a cycle exists.
8. Continue DFS for unvisited neighbors.
9. After DFS, check whether all nodes were visited.
10. Return `true` only if there is no cycle and all nodes are connected.

---

## 5. Complete C++ Code — DFS Approach

```cpp
class Solution {
public:

    bool dfs(int node, int parent,
             vector<vector<int>>& adj,
             vector<bool>& vis) {

        vis[node] = true;

        for (int neighbor : adj[node]) {

            // Ignore the edge back to parent
            if (neighbor == parent) {
                continue;
            }

            // Visited neighbor other than parent = cycle
            if (vis[neighbor]) {
                return false;
            }

            if (!dfs(neighbor, node, adj, vis)) {
                return false;
            }
        }

        return true;
    }

    bool validTree(int n, vector<vector<int>>& edges) {

        // A tree must have exactly n - 1 edges
        if (edges.size() != n - 1) {
            return false;
        }

        vector<vector<int>> adj(n);

        // Build undirected graph
        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<bool> vis(n, false);

        // Check for cycles from node 0
        if (!dfs(0, -1, adj, vis)) {
            return false;
        }

        // Check connectivity
        for (int i = 0; i < n; i++) {

            if (!vis[i]) {
                return false;
            }
        }

        return true;
    }
};
```

### Important Optimization

Notice this condition:

```cpp
if (edges.size() != n - 1) {
    return false;
}
```

Since a valid tree must have exactly `n - 1` edges, we can reject invalid graphs immediately.

After this check, if DFS reaches every node without detecting a cycle, the graph is a valid tree.

In fact, with the edge-count check, connectivity alone is sufficient to establish that the graph is a tree.

---

## 6. Step-by-Step Dry Run — DFS

Consider:

```cpp
n = 5;

edges = {
    {0,1},
    {0,2},
    {0,3},
    {1,4}
};
```

### Step 1: Edge Count Check

```cpp
edges.size() == n - 1
```

```text
4 == 5 - 1
4 == 4
```

The condition passes.

### Step 2: Build Adjacency List

```text
0 -> 1, 2, 3
1 -> 0, 4
2 -> 0
3 -> 0
4 -> 1
```

### Step 3: Start DFS

Initial state:

```text
vis = [false, false, false, false, false]
```

Call:

```cpp
dfs(0, -1)
```

Mark node `0` visited:

```text
vis = [true, false, false, false, false]
```

### Step 4: Visit Node 1

Node `0` explores neighbor `1`.

Call:

```cpp
dfs(1, 0)
```

Mark node `1` visited:

```text
vis = [true, true, false, false, false]
```

Node `1` has neighbors:

```text
0, 4
```

- Neighbor `0` is the parent, so skip it.
- Neighbor `4` is unvisited, so visit it.

### Step 5: Visit Node 4

Call:

```cpp
dfs(4, 1)
```

Mark node `4` visited:

```text
vis = [true, true, false, false, true]
```

Node `4` has neighbor `1`.

Since `1` is the parent, skip it.

DFS returns `true`.

### Step 6: Back to Node 0

Continue exploring remaining neighbors:

- Node `2` is unvisited.
- Visit node `2`.
- Node `2` sees parent `0`, so skip it.
- Return `true`.

Then:

- Node `3` is unvisited.
- Visit node `3`.
- Node `3` sees parent `0`, so skip it.
- Return `true`.

### Step 7: Final Visited Array

```text
vis = [true, true, true, true, true]
```

Every node is visited.

No cycle was detected.

Therefore:

```cpp
return true;
```

---

## 7. Cycle Detection Example

Consider:

```cpp
n = 4;

edges = {
    {0,1},
    {1,2},
    {2,0},
    {2,3}
};
```

Graph:

```text
      0
     / \
    1---2
        |
        3
```

Start DFS from node `0`.

```text
0 -> 1 -> 2
```

At node `2`, its neighbors are:

```text
1, 0, 3
```

- Node `1` is the parent, so skip it.
- Node `0` is already visited and is not the parent.
- Therefore, a cycle exists.

The function returns:

```cpp
false
```

---

## 8. Optimized Approach 2 — BFS

### Main Idea

We can perform the same cycle detection using BFS.

We maintain a queue containing:

```cpp
{currentNode, parentNode}
```

The parent is important because this is an undirected graph.

### Algorithm Steps

1. Build the adjacency list.
2. Check that the number of edges equals `n - 1`.
3. Create a visited array.
4. Push `{0, -1}` into the queue.
5. Mark node `0` visited.
6. Pop a node and explore its neighbors.
7. Skip the parent.
8. If another neighbor is already visited, a cycle exists.
9. Otherwise, mark the neighbor visited and push it with the current node as parent.
10. Check whether all nodes were visited.
11. Return `true` if the graph is connected and acyclic.

### Complete C++ Code — BFS

```cpp
class Solution {
public:

    bool validTree(int n, vector<vector<int>>& edges) {

        if (edges.size() != n - 1) {
            return false;
        }

        vector<vector<int>> adj(n);

        // Build undirected graph
        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<bool> vis(n, false);

        queue<pair<int, int>> q;

        q.push({0, -1});
        vis[0] = true;

        while (!q.empty()) {

            auto [node, parent] = q.front();
            q.pop();

            for (int neighbor : adj[node]) {

                // Ignore parent
                if (neighbor == parent) {
                    continue;
                }

                // Cycle detected
                if (vis[neighbor]) {
                    return false;
                }

                vis[neighbor] = true;

                q.push({neighbor, node});
            }
        }

        // Check connectivity
        for (int i = 0; i < n; i++) {

            if (!vis[i]) {
                return false;
            }
        }

        return true;
    }
};
```

---

## 9. Optimized Approach 3 — Union Find (DSU)

### Main Idea

Union Find helps us efficiently detect cycles in an undirected graph.

It maintains groups of connected components.

We use two operations:

1. `find(x)`: Finds the representative (root) of the component containing `x`.
2. `unite(a, b)`: Combines two different components.

### Cycle Detection Logic

For every edge:

```cpp
u -- v
```

Check their roots:

```cpp
find(u)
find(v)
```

If both roots are the same, then `u` and `v` already belong to the same component.

Adding another edge between them creates a cycle.

If their roots are different, merge the components.

### Algorithm Steps

1. Initialize every node as its own parent.
2. Process every edge.
3. Find the roots of both endpoints.
4. If the roots are equal, return `false`.
5. Otherwise, union the two components.
6. After processing all edges, check that only one connected component exists.
7. Return `true` if the graph is connected and acyclic.

### Complete C++ Code — Union Find

```cpp
class Solution {
public:

    vector<int> parent;
    vector<int> size;

    int find(int x) {

        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }

        return parent[x];
    }

    bool unite(int a, int b) {

        int rootA = find(a);
        int rootB = find(b);

        // Already connected = cycle
        if (rootA == rootB) {
            return false;
        }

        // Union by size
        if (size[rootA] < size[rootB]) {
            swap(rootA, rootB);
        }

        parent[rootB] = rootA;
        size[rootA] += size[rootB];

        return true;
    }

    bool validTree(int n, vector<vector<int>>& edges) {

        if (edges.size() != n - 1) {
            return false;
        }

        parent.resize(n);
        size.assign(n, 1);

        // Initially, every node is its own parent
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }

        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            if (!unite(u, v)) {
                return false;
            }
        }

        return true;
    }
};
```

### Why Does Union Find Work Without a Separate Connectivity Check?

We already checked:

```cpp
edges.size() == n - 1
```

If every edge successfully joins two different components, no cycle exists.

Starting with `n` components, processing `n - 1` successful unions leaves exactly one component.

Therefore, the graph is connected and acyclic.

---

## 10. Complexity Analysis

Let:

- `V` = Number of nodes.
- `E` = Number of edges.

### DFS

| Operation | Complexity |
|---|---|
| Build adjacency list | O(V + E) |
| DFS traversal | O(V + E) |
| Connectivity check | O(V) |

**Time Complexity:**

```text
O(V + E)
```

**Auxiliary Space Complexity:**

```text
O(V)
```

The adjacency list takes `O(V + E)` additional space.

### BFS

| Operation | Complexity |
|---|---|
| Build adjacency list | O(V + E) |
| BFS traversal | O(V + E) |
| Connectivity check | O(V) |

**Time Complexity:**

```text
O(V + E)
```

**Auxiliary Space Complexity:**

```text
O(V)
```

The adjacency list takes `O(V + E)` additional space.

### Union Find

With path compression and union by size:

**Time Complexity:**

```text
O(V + E * α(V))
```

Here, `α(V)` is the inverse Ackermann function, which grows extremely slowly.

**Auxiliary Space Complexity:**

```text
O(V)
```

---

## 11. DFS vs BFS vs Union Find

| Feature | DFS | BFS | Union Find |
|---|---|---|---|
| Main technique | Recursion | Queue | Disjoint Set |
| Cycle detection | Parent tracking | Parent tracking | Same root |
| Connectivity | Visited array | Visited array | Component merging |
| Time | O(V + E) | O(V + E) | O(V + E α(V)) |
| Auxiliary space | O(V) | O(V) | O(V) |
| Adjacency list required | Yes | Yes | No |

### Which Approach Should You Prefer?

**DFS** is a good starting point because it teaches:

- Undirected graph traversal.
- Parent tracking.
- Cycle detection.
- Connectivity checking.

**Union Find** is especially useful when:
- We process edges one by one.
- We need to detect whether two nodes are already connected.
- We solve connectivity or cycle-detection problems repeatedly.

---

## 12. Common Mistakes

1. Forgetting that the graph is undirected.

   Correct graph construction:

   ```cpp
   adj[u].push_back(v);
   adj[v].push_back(u);
   ```

2. Detecting the parent as a cycle.

   Always skip the parent edge during DFS or BFS.

3. Checking cycles but forgetting connectivity.

4. Checking connectivity but forgetting cycles.

5. Assuming `n - 1` edges alone guarantees a tree.

6. Forgetting to mark a node visited before adding it to the BFS queue.

7. Forgetting to check disconnected components.

8. Using directed graph cycle-detection logic for an undirected graph.

9. Not considering a graph containing only one node.

   For `n = 1` and no edges, the graph is a valid tree.

---

## 13. Important Interview Questions

**Q1. What are the two conditions for a valid tree?**

The graph must be connected and acyclic.

**Q2. How many edges does a tree with `n` nodes have?**

Exactly `n - 1`.

**Q3. Does having `n - 1` edges guarantee a valid tree?**

No. We also need to establish connectivity or absence of cycles.

**Q4. Why do we track the parent in undirected DFS?**

Because every undirected edge appears in both adjacency lists. We must ignore the edge leading back to the parent to avoid falsely detecting a cycle.

**Q5. How is cycle detection different in directed and undirected graphs?**

- Undirected graph: Track the parent to ignore the reverse edge.
- Directed graph: Use recursion-stack states or three-state DFS.

**Q6. Can BFS detect a cycle in an undirected graph?**

Yes. By tracking the parent of each node, BFS can detect an already visited neighbor that is not the parent.

**Q7. What is Union Find?**

A data structure that maintains disjoint connected components using `find` and `union` operations.

**Q8. Why do we use path compression?**

It makes future `find` operations faster by connecting visited nodes directly to their component root.

**Q9. Why is a single node with no edges a valid tree?**

A single node is connected to itself, and there are no edges that could form a cycle. It also satisfies `E = V - 1`.

---

## 14. Related Problems

| Problem | Main Pattern |
|---|---|
| Number of Connected Components | Graph + DFS / Union Find |
| Number of Provinces | Connected Components |
| Redundant Connection | Union Find + Cycle Detection |
| Course Schedule | Directed Cycle Detection |
| Course Schedule II | Topological Sorting |
| Accounts Merge | Union Find |
| Graph Valid Tree | Undirected Graph + Connectivity |

---

## 15. Final Revision Notes

### Core Concept

```text
Graph Valid Tree = Connected + Acyclic
```

### Mathematical Property

```text
A valid tree with V vertices has exactly V - 1 edges.
```

### DFS Pattern

```text
1. Build an undirected adjacency list.
2. Check edges.size() == n - 1.
3. Start DFS from node 0.
4. Mark the current node visited.
5. Ignore the parent node.
6. If another visited neighbor is found, a cycle exists.
7. Visit all unvisited neighbors.
8. Check whether every node was visited.
9. Return true only if all conditions are satisfied.
```

### BFS Pattern

```text
1. Build an undirected adjacency list.
2. Check edges.size() == n - 1.
3. Push {0, -1} into the queue.
4. Mark node 0 visited.
5. Pop a node and explore its neighbors.
6. Ignore the parent.
7. If another neighbor is already visited, a cycle exists.
8. Otherwise, mark it visited and push it with its parent.
9. Check whether every node was visited.
10. Return true if the graph is valid.
```

### Union Find Pattern

```text
1. Initialize each node as its own parent.
2. Check edges.size() == n - 1.
3. Process every edge.
4. Find the roots of both endpoints.
5. If roots are equal, a cycle exists.
6. Otherwise, merge the components.
7. If all unions succeed, return true.
```

### Key Takeaways

1. A tree must be connected and acyclic.
2. A tree with `n` nodes has exactly `n - 1` edges.
3. In undirected DFS, parent tracking is essential.
4. BFS can also detect cycles using parent tracking.
5. Union Find detects cycles when both endpoints have the same root.
6. With exactly `n - 1` edges, successful unions guarantee a tree.
7. A single node with no edges is a valid tree.

**Recommended Approach:** DFS with parent tracking.

**Alternative Approach:** Union Find.

**Interview Pattern:** Undirected Graph / Cycle Detection / Connectivity / DSU


# Number of Connected Components in an Undirected Graph — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, DFS, BFS, Union Find
- **Pattern:** Connected Components in an Undirected Graph
- **Language:** C++

## 1. Problem Understanding

We are given:

- `n`: Number of nodes labelled from `0` to `n - 1`.
- `edges`: A list of undirected edges connecting pairs of nodes.

We need to return the total number of connected components in the graph.

A **connected component** is a group of nodes where every node is reachable from every other node in that group.

### Example 1

**Input:**

```cpp
n = 5;
edges = {{0,1}, {1,2}, {3,4}};
```

Graph:

```text
0 --- 1 --- 2

3 --- 4
```

**Output:**

```cpp
2
```

Explanation:

There are two separate connected components:

- Component 1: `{0, 1, 2}`
- Component 2: `{3, 4}`

### Example 2

**Input:**

```cpp
n = 5;
edges = {{0,1}, {1,2}, {2,3}, {3,4}};
```

Graph:

```text
0 --- 1 --- 2 --- 3 --- 4
```

**Output:**

```cpp
1
```

Explanation:

Every node is reachable from every other node. Therefore, the entire graph is one connected component.

### Example 3 — Isolated Nodes

**Input:**

```cpp
n = 5;
edges = {{0,1}, {2,3}};
```

Graph:

```text
0 --- 1

2 --- 3

4
```

**Output:**

```cpp
3
```

Explanation:

- Component 1: `{0, 1}`
- Component 2: `{2, 3}`
- Component 3: `{4}`

Important: An isolated node also counts as a connected component.

---

## 2. Core Concept — Connected Components

Imagine a graph containing several separate groups.

```text
Component 1       Component 2       Component 3

0 --- 1           3 --- 4           5

      |
      2
```

To count connected components, we need to identify how many independent groups exist.

### Important Observation

We can solve this using DFS, BFS, or Union Find.

The main observation is:

**Every time we find an unvisited node, we have discovered a new connected component.**

We increase the answer and explore all nodes belonging to that component.

---

## 3. Optimized Approach 1 — DFS

### Main Idea

We use DFS to explore every connected component.

Whenever we encounter an unvisited node:

1. Increment the component count.
2. Start DFS from that node.
3. Mark all reachable nodes as visited.
4. Continue scanning the remaining nodes.

When all nodes have been processed, the component count is our answer.

### Algorithm Steps

1. Build an adjacency list from the given edges.
2. Create a visited array initialized with `false`.
3. Initialize `count = 0`.
4. Iterate through every node from `0` to `n - 1`.
5. If the current node is already visited, skip it.
6. Otherwise:
   - Increment `count`.
   - Start DFS from the current node.
7. DFS marks every reachable node as visited.
8. Return `count`.

### Complete C++ Code — DFS

```cpp
class Solution {
public:

    void dfs(int node,
             vector<vector<int>>& adj,
             vector<bool>& vis) {

        vis[node] = true;

        for (int neighbor : adj[node]) {

            if (!vis[neighbor]) {
                dfs(neighbor, adj, vis);
            }
        }
    }

    int countComponents(int n, vector<vector<int>>& edges) {

        // Build adjacency list
        vector<vector<int>> adj(n);

        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<bool> vis(n, false);

        int count = 0;

        // Visit every node
        for (int i = 0; i < n; i++) {

            if (!vis[i]) {

                // Found a new component
                count++;

                dfs(i, adj, vis);
            }
        }

        return count;
    }
};
```

---

## 4. Step-by-Step Dry Run — DFS

Consider:

```cpp
n = 5;

edges = {
    {0,1},
    {1,2},
    {3,4}
};
```

### Step 1: Build Adjacency List

```text
0 -> 1
1 -> 0, 2
2 -> 1
3 -> 4
4 -> 3
```

### Step 2: Initialize Visited Array

```cpp
vis = {false, false, false, false, false};
```

Initial component count:

```cpp
count = 0;
```

### Step 3: Start Iterating

**Iteration 1: Node 0**

Node `0` is unvisited.

Therefore:

```cpp
count++;
dfs(0, adj, vis);
```

DFS traversal:

```text
0 -> 1 -> 2
```

All three nodes are marked visited.

Visited array:

```text
{true, true, true, false, false}
```

Component count:

```text
count = 1
```

**Iteration 2: Node 1**

Node `1` is already visited.

Skip it.

**Iteration 3: Node 2**

Node `2` is already visited.

Skip it.

**Iteration 4: Node 3**

Node `3` is unvisited.

Therefore:

```cpp
count++;
dfs(3, adj, vis);
```

DFS traversal:

```text
3 -> 4
```

Visited array:

```text
{true, true, true, true, true}
```

Component count:

```text
count = 2
```

**Iteration 5: Node 4**

Node `4` is already visited.

Skip it.

### Step 4: Final Answer

```cpp
return count;
```

Output:

```text
2
```

### Dry Run Summary

| Node | Visited Before DFS? | Action | Component Count |
|---|---|---|---|
| 0 | No | Start DFS: 0, 1, 2 | 1 |
| 1 | Yes | Skip | 1 |
| 2 | Yes | Skip | 1 |
| 3 | No | Start DFS: 3, 4 | 2 |
| 4 | Yes | Skip | 2 |

---

## 5. Optimized Approach 2 — BFS

### Main Idea

We can use BFS instead of DFS to explore each connected component.

Whenever we find an unvisited node:

1. Increment the component count.
2. Push the node into a queue.
3. Mark it visited.
4. Process all reachable neighbors using BFS.
5. Continue with the next unvisited node.

### Complete C++ Code — BFS

```cpp
class Solution {
public:

    int countComponents(int n, vector<vector<int>>& edges) {

        vector<vector<int>> adj(n);

        // Build adjacency list
        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<bool> vis(n, false);

        int count = 0;

        for (int i = 0; i < n; i++) {

            if (vis[i]) {
                continue;
            }

            // New connected component
            count++;

            queue<int> q;

            q.push(i);
            vis[i] = true;

            while (!q.empty()) {

                int node = q.front();
                q.pop();

                for (int neighbor : adj[node]) {

                    if (!vis[neighbor]) {

                        vis[neighbor] = true;

                        q.push(neighbor);
                    }
                }
            }
        }

        return count;
    }
};
```

### Important BFS Detail

Mark nodes visited when pushing them into the queue:

```cpp
vis[neighbor] = true;
q.push(neighbor);
```

This prevents the same node from being inserted into the queue multiple times.

---

## 6. Optimized Approach 3 — Union Find (DSU)

### Main Idea

Union Find maintains different connected components using parent relationships.

Initially, every node belongs to its own component.

For example:

```text
0   1   2   3   4
```

Initially:

```text
count = 5
```

Whenever an edge connects two different components, we merge them and decrease the component count by one.

### Union Find Operations

**1. Find**

Finds the representative (root) of a component.

**2. Union**

Combines two different components.

### Important Observation

For every edge `(u, v)`:

- If `find(u) != find(v)`, merge the components.
- If `find(u) == find(v)`, both nodes already belong to the same component. No new component is removed.

Unlike Graph Valid Tree, finding an edge within the same component is not an error here. We simply ignore it.

### Complete C++ Code — Union Find

```cpp
class Solution {
public:

    vector<int> parent;
    vector<int> size;

    int find(int x) {

        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }

        return parent[x];
    }

    void unite(int a, int b) {

        int rootA = find(a);
        int rootB = find(b);

        if (rootA == rootB) {
            return;
        }

        // Union by size
        if (size[rootA] < size[rootB]) {
            swap(rootA, rootB);
        }

        parent[rootB] = rootA;

        size[rootA] += size[rootB];
    }

    int countComponents(int n, vector<vector<int>>& edges) {

        parent.resize(n);
        size.assign(n, 1);

        // Initially, every node is a separate component
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }

        int count = n;

        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            int rootU = find(u);
            int rootV = find(v);

            if (rootU != rootV) {

                unite(rootU, rootV);

                count--;
            }
        }

        return count;
    }
};
```

---

## 7. Step-by-Step Dry Run — Union Find

Input:

```cpp
n = 5;

edges = {
    {0,1},
    {1,2},
    {3,4}
};
```

### Step 1: Initialize

Initially, every node is its own parent.

```text
parent = [0, 1, 2, 3, 4]
```

Initial component count:

```text
count = 5
```

### Step 2: Process Edge (0, 1)

Find roots:

```text
find(0) = 0
find(1) = 1
```

Different roots, so merge.

```text
parent[1] = 0
```

Component count:

```text
count = 4
```

### Step 3: Process Edge (1, 2)

Find roots:

```text
find(1) = 0
find(2) = 2
```

Different roots, so merge.

```text
parent[2] = 0
```

Component count:

```text
count = 3
```

### Step 4: Process Edge (3, 4)

Find roots:

```text
find(3) = 3
find(4) = 4
```

Different roots, so merge.

```text
parent[4] = 3
```

Component count:

```text
count = 2
```

### Step 5: Final Answer

```cpp
return count;
```

Output:

```text
2
```

### Key Formula

```text
Initial Components = n

For every successful union:
    count--

Final Answer = Number of Connected Components
```

---

## 8. Complexity Analysis

Let:

- `V` = Number of vertices.
- `E` = Number of edges.

### DFS

| Operation | Complexity |
|---|---|
| Build adjacency list | O(V + E) |
| Traverse all components | O(V + E) |

**Time Complexity:**

```text
O(V + E)
```

**Auxiliary Space Complexity:**

```text
O(V)
```

The adjacency list requires `O(V + E)` additional space.

### BFS

**Time Complexity:**

```text
O(V + E)
```

**Auxiliary Space Complexity:**

```text
O(V)
```

The adjacency list requires `O(V + E)` additional space.

### Union Find

With path compression and union by size:

**Time Complexity:**

```text
O(V + E * α(V))
```

**Auxiliary Space Complexity:**

```text
O(V)
```

Here, `α(V)` is the inverse Ackermann function.

---

## 9. DFS vs BFS vs Union Find

| Feature | DFS | BFS | Union Find |
|---|---|---|---|
| Main technique | Recursion | Queue | Disjoint Set |
| Counts components | Yes | Yes | Yes |
| Requires adjacency list | Yes | Yes | No |
| Time Complexity | O(V + E) | O(V + E) | O(V + E α(V)) |
| Auxiliary Space | O(V) | O(V) | O(V) |
| Best use | Graph traversal | Level-based traversal | Component merging |

### Which Approach Should You Prefer?

**DFS:** Best for understanding connected components through graph traversal.

**BFS:** Useful when iterative traversal is preferred or recursion depth is a concern.

**Union Find:** Very useful when edges are processed dynamically or when we need to merge and query components.

---

## 10. Common Mistakes

1. Forgetting to iterate over all nodes.

   Starting DFS only from node `0` will not discover disconnected components.

2. Incrementing the component count for every node instead of every unvisited node.

3. Forgetting to mark a node visited.

4. Forgetting that isolated nodes are also connected components.

5. Treating an already-connected pair in Union Find as an error.

   In this problem, an unsuccessful union does not mean the graph is invalid.

6. Decreasing the component count for every edge instead of only successful unions.

7. Building a directed adjacency list for an undirected graph.

   Correct construction:

   ```cpp
   adj[u].push_back(v);
   adj[v].push_back(u);
   ```

---

## 11. Important Interview Questions

**Q1. What is a connected component?**

A maximal group of vertices where every vertex is reachable from every other vertex.

**Q2. Why do we iterate over all nodes?**

Because the graph can contain multiple disconnected components. Starting from one node only explores its own component.

**Q3. Why do we increment the count before starting DFS?**

Because encountering an unvisited node means we have discovered a new connected component.

**Q4. What happens if a node has no edges?**

It forms its own connected component.

**Q5. Can BFS solve this problem?**

Yes. BFS explores all reachable nodes in the current component.

**Q6. How does Union Find count components?**

Initialize the count to `n`. Decrease it whenever two different components are successfully merged.

**Q7. What is the difference between Graph Valid Tree and this problem?**

- Graph Valid Tree: Checks whether the graph is connected and acyclic.
- Number of Connected Components: Counts the separate connected groups.

**Q8. Can a graph have cycles and still have connected components?**

Yes. Connected components do not need to be acyclic.

**Q9. What is the time complexity of DFS for this problem?**

`O(V + E)` because every vertex and edge is processed a constant number of times.

---

## 12. Related NeetCode Problems

| Problem | Main Pattern |
|---|---|
| Number of Islands | Grid DFS / BFS |
| Max Area of Island | Grid DFS / BFS |
| Graph Valid Tree | Connectivity + Cycle Detection |
| Number of Provinces | Connected Components |
| Redundant Connection | Union Find |
| Accounts Merge | Union Find |
| Clone Graph | Graph Traversal |
| Course Schedule | Directed Cycle Detection |

---

## 13. Final Revision Notes

### Core Concept

```text
Every unvisited node represents a new connected component.
```

### DFS Pattern

```text
1. Build adjacency list.
2. Initialize visited array.
3. Set count = 0.
4. Iterate over all nodes.
5. If a node is unvisited:
   a. Increment count.
   b. Start DFS.
   c. Mark all reachable nodes visited.
6. Return count.
```

### BFS Pattern

```text
1. Build adjacency list.
2. Initialize visited array.
3. Set count = 0.
4. Iterate over all nodes.
5. If a node is unvisited:
   a. Increment count.
   b. Start BFS using a queue.
   c. Mark neighbors visited when adding them.
6. Return count.
```

### Union Find Pattern

```text
1. Initialize parent[i] = i.
2. Initialize count = n.
3. Process every edge (u, v).
4. Find roots of u and v.
5. If roots are different:
   a. Merge the components.
   b. Decrease count.
6. Return count.
```

### Key Takeaways

1. A connected component is a separate reachable group of nodes.
2. An isolated node counts as one component.
3. DFS and BFS explore one complete component at a time.
4. Every new unvisited node starts a new DFS/BFS traversal.
5. Union Find starts with `n` components.
6. Every successful union decreases the component count by one.
7. Unlike Graph Valid Tree, cycles are allowed.

**Recommended Approach:** DFS.

**Alternative Approaches:** BFS and Union Find.

**Interview Pattern:** Graph Traversal / Connected Components / Disjoint Set Union.


# Redundant Connection — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, Union Find, Disjoint Set Union (DSU)
- **Pattern:** Cycle Detection using Union Find
- **Language:** C++

## 1. Problem Understanding

We are given an undirected graph with:

- `n` nodes labelled from `1` to `n`.
- Initially, the graph was a tree containing exactly `n - 1` edges.
- One additional edge has been added to the graph.

We need to return the edge that can be removed so that the graph becomes a valid tree again.

If multiple answers exist, return the edge that appears last in the input.

### Important Observations

A tree has two important properties:

1. It must be connected.
2. It must not contain any cycle.

Adding one extra edge to a tree creates exactly one cycle.

Therefore, our task is to identify the edge that creates this cycle.

### Example 1

**Input:**

```cpp
edges = {{1,2}, {1,3}, {3,4}, {2,4}};
```

Graph:

```text
    1
   / \
  2   3
   \   |
     4
```

The edges are:

```text
1 -- 2
1 -- 3
3 -- 4
2 -- 4
```

There is a cycle:

```text
1 -> 2 -> 4 -> 3 -> 1
```

Removing edge `[2,4]` breaks the cycle.

**Output:**

```cpp
{2,4}
```

---

## 2. Core Concept — Union Find (DSU)

Union Find is a data structure used to maintain and manage disjoint sets.

It supports two main operations:

### 1. Find

Returns the representative (root) of the component containing a node.

```cpp
find(x);
```

### 2. Union

Combines two different components.

```cpp
unite(x, y);
```

### How Does It Help Detect a Cycle?

Consider an edge `(u, v)`.

Before adding this edge, check the roots of both nodes:

```cpp
if (find(u) == find(v))
```

If both nodes have the same root, they are already connected.

Adding an edge between them creates a cycle.

Therefore, this is our redundant edge.

Otherwise, merge their components.

```cpp
unite(u, v);
```

### Main Logic

```text
For every edge (u, v):

    Find root of u.
    Find root of v.

    If both roots are equal:
        Return this edge.

    Otherwise:
        Union both components.
```

---

## 3. Optimized Approach — Union Find

### Step 1: Initialize Parent Array

Nodes are labelled from `1` to `n`, so we need an array of size `n + 1`.

Initially, every node is its own parent.

```cpp
parent[i] = i;
```

For example, if `n = 4`:

```text
Node:    1  2  3  4
Parent:  1  2  3  4
```

### Step 2: Process Every Edge

For each edge `(u, v)`:

1. Find the root of `u`.
2. Find the root of `v`.
3. If both roots are equal, the edge creates a cycle.
4. Otherwise, merge the two components.

### Step 3: Return the Redundant Edge

The edge connecting two nodes already belonging to the same component is the redundant connection.

---

## 4. Complete C++ Code — Union Find

```cpp
class Solution {
public:

    vector<int> parent;
    vector<int> size;

    // Find with Path Compression
    int find(int x) {

        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }

        return parent[x];
    }

    // Union by Size
    bool unite(int u, int v) {

        int rootU = find(u);
        int rootV = find(v);

        // Both nodes already belong to the same component
        if (rootU == rootV) {
            return false;
        }

        // Attach smaller component to larger component
        if (size[rootU] < size[rootV]) {
            swap(rootU, rootV);
        }

        parent[rootV] = rootU;

        size[rootU] += size[rootV];

        return true;
    }

    vector<int> findRedundantConnection(
        vector<vector<int>>& edges
    ) {

        int n = edges.size();

        parent.resize(n + 1);
        size.assign(n + 1, 1);

        // Initially, every node is its own parent
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
        }

        // Process every edge
        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            // If union fails, this edge creates a cycle
            if (!unite(u, v)) {
                return edge;
            }
        }

        return {};
    }
};
```

### Why Does `unite()` Return a Boolean?

We return:

- `true` when two different components are successfully merged.
- `false` when both nodes already belong to the same component.

This makes cycle detection simple:

```cpp
if (!unite(u, v)) {
    return edge;
}
```

---

## 5. Step-by-Step Dry Run

Consider:

```cpp
edges = {
    {1,2},
    {1,3},
    {3,4},
    {2,4}
};
```

### Step 1: Initialize

Initially:

```text
parent = [0, 1, 2, 3, 4]
size   = [1, 1, 1, 1, 1]
```

Each node belongs to its own component.

### Step 2: Process Edge (1, 2)

Find roots:

```text
find(1) = 1
find(2) = 2
```

Roots are different.

Merge:

```text
parent[2] = 1
```

Components:

```text
{1, 2}, {3}, {4}
```

### Step 3: Process Edge (1, 3)

Find roots:

```text
find(1) = 1
find(3) = 3
```

Roots are different.

Merge:

```text
parent[3] = 1
```

Components:

```text
{1, 2, 3}, {4}
```

### Step 4: Process Edge (3, 4)

Find roots:

```text
find(3) = 1
find(4) = 4
```

Roots are different.

Merge:

```text
parent[4] = 1
```

Components:

```text
{1, 2, 3, 4}
```

All nodes now belong to one component.

### Step 5: Process Edge (2, 4)

Find roots:

```text
find(2) = 1
find(4) = 1
```

Both roots are equal.

This means nodes `2` and `4` are already connected through another path.

Adding edge `(2, 4)` creates a cycle.

Therefore:

```cpp
return {2, 4};
```

### Final Output

```cpp
{2, 4}
```

---

## 6. Dry Run Summary

| Edge | Root of U | Root of V | Action |
|---|---:|---:|---|
| (1, 2) | 1 | 2 | Union |
| (1, 3) | 1 | 3 | Union |
| (3, 4) | 1 | 4 | Union |
| (2, 4) | 1 | 1 | Cycle detected |

The last edge is redundant.

---

## 7. Alternative Approach — DFS

We can also solve this problem using DFS.

### Main Idea

Before adding an edge `(u, v)` to the graph, check whether `u` and `v` are already connected.

If a path already exists between them, adding this edge creates a cycle.

Otherwise, add the edge to the adjacency list.

### Algorithm

1. Initialize an empty adjacency list.
2. Iterate through the edges in input order.
3. For each edge `(u, v)`:
   - Run DFS from `u` to check whether `v` is reachable.
   - If reachable, return `{u, v}`.
   - Otherwise, add the edge in both directions.
4. Return an empty array if no redundant edge is found.

### Complete C++ Code — DFS

```cpp
class Solution {
public:

    bool dfs(int node, int target,
             vector<vector<int>>& adj,
             vector<bool>& vis) {

        if (node == target) {
            return true;
        }

        vis[node] = true;

        for (int neighbor : adj[node]) {

            if (!vis[neighbor]) {

                if (dfs(neighbor, target, adj, vis)) {
                    return true;
                }
            }
        }

        return false;
    }

    vector<int> findRedundantConnection(
        vector<vector<int>>& edges
    ) {

        int n = edges.size();

        vector<vector<int>> adj(n + 1);

        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            vector<bool> vis(n + 1, false);

            // Check if a path already exists
            if (dfs(u, v, adj, vis)) {
                return edge;
            }

            // No existing path: safely add this edge
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        return {};
    }
};
```

### DFS Complexity

Each edge may trigger a graph traversal.

- **Time Complexity:** `O(E * (V + E))`
- **Space Complexity:** `O(V + E)`

Union Find is more efficient for this problem.

---

## 8. Complexity Analysis

Let:

- `V` = Number of vertices.
- `E` = Number of edges.

For this problem, `V = E = n`.

### Union Find

With path compression and union by size:

**Time Complexity:**

```text
O(E * α(V))
```

Initialization takes `O(V)`.

Here, `α(V)` is the inverse Ackermann function, which grows extremely slowly.

**Auxiliary Space Complexity:**

```text
O(V)
```

For the parent and size arrays.

### DFS

**Time Complexity:**

```text
O(E * (V + E))
```

**Auxiliary Space Complexity:**

```text
O(V + E)
```

For the adjacency list, visited array and recursion stack.

### Comparison

| Approach | Time Complexity | Auxiliary Space |
|---|---|---|
| Union Find | O(V + E α(V)) | O(V) |
| DFS | O(E * (V + E)) | O(V + E) |

---

## 9. Common Mistakes

1. **Incorrect parent array size**

   Nodes are labelled from `1` to `n`, so initialize:

   ```cpp
   vector<int> parent(n + 1);
   ```

2. **Forgetting path compression**

   Without path compression, repeated Find operations can become slower.

3. **Incorrectly checking node values instead of roots**

   Always compare:

   ```cpp
   find(u) == find(v)
   ```

   Do not directly compare:

   ```cpp
   parent[u] == parent[v]
   ```

   Parent values may not represent the final root.

4. **Unioning before checking for a cycle**

   First determine whether both nodes have the same root. If they do, return the edge.

5. **Forgetting that the graph is undirected**

   In the DFS approach, add both directions:

   ```cpp
   adj[u].push_back(v);
   adj[v].push_back(u);
   ```

6. **Using `n` instead of `n + 1`**

   Remember that node labels start at `1`.

7. **Returning the wrong edge**

   Process edges in their original order. The problem asks for the redundant edge appearing last if multiple answers exist.

---

## 10. Important Interview Questions

**Q1. What is a redundant connection?**

An edge that connects two nodes already connected through another path, thereby creating a cycle.

**Q2. How does Union Find detect a cycle?**

If two endpoints of an edge have the same root, they already belong to the same component. Adding that edge creates a cycle.

**Q3. Why do we use Path Compression?**

It makes future Find operations faster by directly connecting visited nodes to their root.

**Q4. What is Union by Size?**

We attach the smaller component to the larger component to keep the DSU tree relatively shallow.

**Q5. Why is the parent array of size `n + 1`?**

Because nodes are labelled from `1` through `n`, inclusive.

**Q6. What is the difference between Number of Connected Components and Redundant Connection?**

- Number of Connected Components: Counts separate groups.
- Redundant Connection: Detects an edge that joins nodes already in the same group.

**Q7. Can DFS also detect cycles?**

Yes. Before inserting an edge, DFS can check whether a path already exists between its endpoints.

**Q8. Why is Union Find preferred?**

It efficiently processes edges and detects cycles without repeatedly traversing the graph.

---

## 11. Related NeetCode Problems

| Problem | Main Pattern |
|---|---|
| Number of Connected Components | DFS / BFS / Union Find |
| Graph Valid Tree | Connectivity + Cycle Detection |
| Redundant Connection | Union Find |
| Accounts Merge | Union Find |
| Number of Provinces | Connected Components |
| Number of Islands | Grid DFS / BFS |

---

## 12. Final Revision Notes

### Core Logic

```text
For every edge (u, v):

    rootU = find(u)
    rootV = find(v)

    If rootU == rootV:
        This edge creates a cycle.
        Return {u, v}.

    Otherwise:
        Union the two components.
```

### DSU Template

```cpp
int find(int x) {

    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }

    return parent[x];
}

bool unite(int u, int v) {

    int rootU = find(u);
    int rootV = find(v);

    if (rootU == rootV) {
        return false;
    }

    parent[rootV] = rootU;

    return true;
}
```

### Key Takeaways

1. A tree contains exactly `n - 1` edges and has no cycles.
2. Adding an extra edge to a tree creates a cycle.
3. Union Find maintains connected components.
4. If two nodes have the same root, they are already connected.
5. A failed union indicates a cycle in this problem.
6. Path compression improves Find operations.
7. Union by size helps keep DSU trees shallow.
8. Node labels start from `1`, so use arrays of size `n + 1`.

**Recommended Approach:** Union Find with Path Compression and Union by Size.

**Alternative Approach:** DFS.

**Interview Pattern:** Disjoint Set Union (DSU) / Cycle Detection.

**Most Important Line:**

```cpp
if (find(u) == find(v)) {
    return edge;
}
```

# Word Ladder — NeetCode 150

- **Difficulty:** Hard
- **Topics:** Graph, BFS, Hash Set, Shortest Path
- **Pattern:** BFS on an Implicit Graph
- **Language:** C++

## 1. Problem Understanding

We are given:

- `beginWord`: The starting word.
- `endWord`: The target word.
- `wordList`: A list of valid words.

We need to transform `beginWord` into `endWord` by following these rules:

1. Each transformation must change exactly one character.
2. Every transformed word must exist in `wordList`.
3. We can perform as many transformations as required.

Return the minimum number of words in the transformation sequence, including both `beginWord` and `endWord`.

If no valid transformation exists, return `0`.

### Example 1

**Input:**

```cpp
beginWord = "cat";
endWord = "sag";
wordList = {"bat", "bag", "sag", "dag", "dot"};
```

**Transformation:**

```text
cat -> bat -> bag -> sag
```

Explanation:

- `cat` -> `bat`: Change `c` to `b`.
- `bat` -> `bag`: Change `t` to `g`.
- `bag` -> `sag`: Change `b` to `s`.

There are 4 words in the sequence.

**Output:**

```cpp
4
```

### Example 2

**Input:**

```cpp
beginWord = "cat";
endWord = "sag";
wordList = {"bat", "bag", "dag"};
```

**Output:**

```cpp
0
```

The target word `"sag"` does not exist in `wordList`, so the transformation is impossible.

---

## 2. Core Concept — BFS

This problem is a shortest-path problem in an unweighted graph.

### Why BFS?

Imagine every word is a graph node.

An edge exists between two words if they differ by exactly one character.

For example:

```text
cat ---- bat
           |
          bag ---- sag
```

BFS explores the graph level by level.

- Level 1: Starting word.
- Level 2: Words reachable in one transformation.
- Level 3: Words reachable in two transformations.
- And so on.

The first time we reach `endWord`, we have found the shortest transformation sequence.

### Important Observation

We do not need to explicitly construct the entire graph.

Instead, we generate possible neighboring words by changing one character at a time.

For example, for `"cat"`:

```text
aat, bat, cat, dat, ..., zat

caa, cab, cac, ..., caz

cAt, cBt, ...
```

For each position, try all lowercase English letters and check whether the generated word exists in `wordList`.

---

## 3. Optimized Approach — BFS + Unordered Set

### Step 1: Store Words in a Hash Set

```cpp
unordered_set<string> st(wordList.begin(), wordList.end());
```

Why?

- Fast average-case lookup.
- Prevents revisiting words.
- Allows us to remove words once they enter the BFS queue.

### Step 2: Check Whether the Target Exists

```cpp
if (!st.count(endWord)) {
    return 0;
}
```

If `endWord` is not present, no valid sequence can reach it.

### Step 3: Initialize BFS

```cpp
queue<pair<string, int>> q;
q.push({beginWord, 1});
```

We start with a count of `1` because the starting word is included in the answer.

### Step 4: Process Each Word

For every word:

1. Remove one character at a time.
2. Replace it with each letter from `'a'` to `'z'`.
3. Check whether the generated word exists in the hash set.
4. If it exists:
   - If it is `endWord`, return the current count plus one.
   - Otherwise, push it into the queue and remove it from the set.

### Step 5: Return Zero

If BFS finishes without finding `endWord`, return `0`.

---

## 4. Complete C++ Code — Optimized BFS

```cpp
class Solution {
public:

    int ladderLength(string beginWord,
                     string endWord,
                     vector<string>& wordList) {

        unordered_set<string> st(
            wordList.begin(),
            wordList.end()
        );

        // Target must exist in the dictionary
        if (!st.count(endWord)) {
            return 0;
        }

        queue<pair<string, int>> q;

        q.push({beginWord, 1});

        while (!q.empty()) {

            auto [word, steps] = q.front();
            q.pop();

            // Try changing every character
            for (int i = 0; i < word.size(); i++) {

                char original = word[i];

                // Try all lowercase letters
                for (char ch = 'a'; ch <= 'z'; ch++) {

                    word[i] = ch;

                    // Check whether the generated word exists
                    if (st.count(word)) {

                        // Target found
                        if (word == endWord) {
                            return steps + 1;
                        }

                        // Add to BFS queue
                        q.push({word, steps + 1});

                        // Mark visited
                        st.erase(word);
                    }
                }

                // Restore original character
                word[i] = original;
            }
        }

        return 0;
    }
};
```

---

## 5. Step-by-Step Dry Run

Consider:

```cpp
beginWord = "cat";
endWord = "sag";

wordList = {
    "bat",
    "bag",
    "sag",
    "dag",
    "dot"
};
```

### Step 1: Initialize

Hash Set:

```text
{
    bat, bag, sag, dag, dot
}
```

Queue:

```text
[("cat", 1)]
```

### Step 2: Process "cat"

Current word:

```text
cat
```

Try changing the first character.

```text
aat
bat  <- Found
cat
dat
...
```

`bat` exists in the hash set.

Push it into the queue and remove it from the set.

Queue:

```text
[("bat", 2)]
```

Remaining set:

```text
{bag, sag, dag, dot}
```

### Step 3: Process "bat"

Current word:

```text
bat
```

Try changing the third character.

```text
baa
bab
bag  <- Found
bat
...
```

`bag` exists.

Push it into the queue and remove it from the set.

Queue:

```text
[("bag", 3)]
```

Remaining set:

```text
{sag, dag, dot}
```

### Step 4: Process "bag"

Current word:

```text
bag
```

Try changing the first character.

```text
aag
bag
cag
...
sag  <- Found
```

We found the target word.

Return:

```cpp
steps + 1
```

Since `steps = 3`:

```cpp
return 4;
```

### Final Output

```cpp
4
```

### Transformation Sequence

```text
cat (1)
 |
bat (2)
 |
bag (3)
 |
sag (4)
```

---

## 6. Why Do We Erase Words From the Set?

This is an important BFS optimization.

Consider:

```text
        cat
       /   \
     bat   dat
       \   /
        bag
```

The same word might be reachable through multiple paths.

If we do not remove visited words, we may insert and process the same word repeatedly.

Therefore:

```cpp
st.erase(word);
```

This ensures that every dictionary word is added to the BFS queue at most once.

**Important:** Remove the word when it is added to the queue, not when it is removed from the queue. This prevents duplicate insertions.

---

## 7. Complexity Analysis

Let:

- `N` = Number of words in `wordList`.
- `L` = Length of each word.
- `26` = Number of lowercase English letters.

### Time Complexity

For each word:

- We examine `L` character positions.
- At each position, we try 26 possible letters.
- Each generated string requires up to `O(L)` time to hash or compare.

Therefore:

```text
O(N * L * 26 * L)
```

Or:

```text
O(26 * N * L²)
```

Since 26 is constant:

```text
O(N * L²)
```

This is the expected hash-set-based complexity when accounting for string hashing.

### Space Complexity

The hash set stores up to `N` words.

The BFS queue also stores up to `N` words.

Therefore:

```text
O(N * L)
```

The `L` factor accounts for storing strings.

---

## 8. Alternative Approach — Bidirectional BFS

We can optimize BFS further using Bidirectional BFS.

### Main Idea

Instead of searching only from `beginWord`, search from both ends:

- Forward search starts from `beginWord`.
- Backward search starts from `endWord`.

At every step, expand the smaller search frontier.

When both searches meet, we have found the shortest transformation sequence.

### Why Can It Be Faster?

Normal BFS may explore many nodes from one direction.

Bidirectional BFS reduces the search depth from both ends.

For a graph with branching factor `b` and shortest path depth `d`:

- Normal BFS can explore approximately `O(b^d)` nodes.
- Bidirectional BFS can explore approximately `O(b^(d/2))` nodes from each direction.

This is especially useful when the graph has many possible neighbors.

### Important Implementation Detail

Maintain two sets:

```cpp
unordered_set<string> beginSet;
unordered_set<string> endSet;
```

At each iteration:

1. Expand the smaller set.
2. Generate all one-character transformations.
3. If a generated word exists in the opposite search set, return the current sequence length.
4. Otherwise, add valid unvisited words to the next frontier.
5. Remove visited words from the dictionary.

---

## 9. Common Mistakes

1. **Starting the count at zero**

   The starting word is included in the sequence.

   ```cpp
   q.push({beginWord, 1});
   ```

2. **Not checking whether `endWord` exists**

   If the target is absent from `wordList`, return `0`.

3. **Forgetting to restore the original character**

   After trying all replacements at one position:

   ```cpp
   word[i] = original;
   ```

4. **Not marking words as visited**

   This can cause repeated queue insertions and unnecessary processing.

5. **Marking visited too late**

   Erase a word when it is inserted into the queue.

6. **Using DFS for shortest path**

   DFS does not guarantee that the first path found is the shortest.

   BFS is the natural choice for an unweighted shortest-path problem.

7. **Returning the wrong sequence length**

   The answer counts both `beginWord` and `endWord`.

---

## 10. Important Interview Questions

**Q1. Why is BFS used instead of DFS?**

BFS explores nodes level by level and guarantees the shortest path in an unweighted graph.

**Q2. Why do we use an unordered set?**

For efficient average-case word lookup and visited-word management.

**Q3. Why do we start with a count of 1?**

Because `beginWord` is part of the transformation sequence.

**Q4. Why do we try all 26 characters?**

Because each transformation can change a character to any lowercase English letter.

**Q5. Why do we restore the original character?**

So that changing the next position starts from the original word rather than a previously modified word.

**Q6. Can we use Bidirectional BFS?**

Yes. It searches from both ends and can significantly reduce the search space.

**Q7. What is the graph representation in this problem?**

Each word is a node, and two nodes are connected if their words differ by exactly one character.

**Q8. Do we need to build the graph explicitly?**

No. We generate neighbors dynamically by changing one character at a time.

---

## 11. Related NeetCode Problems

| Problem | Main Pattern |
|---|---|
| Number of Islands | Grid BFS / DFS |
| Rotting Fruit | Multi-source BFS |
| Pacific Atlantic Water Flow | Reverse DFS / BFS |
| Word Ladder | BFS Shortest Path |
| Clone Graph | Graph Traversal |
| Course Schedule | Topological Sort |
| Shortest Path in Binary Matrix | BFS |

---

## 12. Final Revision Notes

### Core Pattern

```text
BFS + Hash Set + Shortest Path
```

### Algorithm

```text
1. Insert all words into an unordered_set.
2. Check whether endWord exists.
3. Initialize queue with (beginWord, 1).
4. While the queue is not empty:
   a. Extract the current word and its level.
   b. Change each character to 'a' through 'z'.
   c. If the generated word exists:
      - If it is endWord, return level + 1.
      - Otherwise, push it into the queue.
      - Erase it from the set.
5. Return 0 if no sequence exists.
```

### Key Takeaways

1. Word Ladder is a shortest-path problem.
2. BFS is preferred for unweighted graphs.
3. Every word represents a graph node.
4. Words differing by exactly one character are adjacent.
5. We can generate neighbors without constructing the graph.
6. Use `unordered_set` for efficient lookup.
7. Mark words visited when inserting them into the queue.
8. Start the sequence count at `1`.
9. Bidirectional BFS is an optimization worth knowing.

**Recommended Approach:** BFS + Unordered Set.

**Advanced Approach:** Bidirectional BFS.

**Interview Pattern:** Shortest Path in an Unweighted Graph.

**Interview Pattern:** Graph Traversal / Connected Components / Matrix DFS

