
# Network Delay Time — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, Shortest Path, Dijkstra's Algorithm, Priority Queue
- **Pattern:** Single Source Shortest Path
- **Language:** C++

## 1. Problem Understanding

We are given:

- `n`: Total number of nodes, labeled from `1` to `n`.
- `times`: A list of directed weighted edges.
- `k`: The starting node from which a signal is sent.

Each edge is represented as:

```cpp
times[i] = {ui, vi, ti};
```

Where:

- `ui` = Source node.
- `vi` = Destination node.
- `ti` = Time required to travel from `ui` to `vi`.

We need to calculate the minimum time required for all nodes to receive the signal.

The signal starts from node `k`.

### Important Conditions

1. Edges are directed.
2. Edge weights represent travel time.
3. The signal can travel through multiple edges.
4. Every node must receive the signal.
5. If any node cannot receive the signal, return `-1`.

### Example 1

**Input:**

```cpp
times = {
    {1, 2, 1},
    {1, 4, 4},
    {2, 3, 1},
    {3, 4, 1}
};

n = 4;
k = 1;
```

**Graph:**

```text
       1
   1 -----> 2
   |         |
  4|         |1
   |         |
   v         v
   4 <----- 3
       1
```

Starting node:

```text
k = 1
```

Shortest distances from node 1:

| Node | Shortest Distance |
|---|---:|
| 1 | 0 |
| 2 | 1 |
| 3 | 2 |
| 4 | 3 |

Node 4 can be reached through:

```text
1 -> 2 -> 3 -> 4
```

Total time:

```text
1 + 1 + 1 = 3
```

The signal reaches all nodes after the maximum shortest distance.

**Output:**

```cpp
3
```

---

## 2. Core Concept — Dijkstra's Algorithm

Dijkstra's algorithm finds the shortest distance from a single source node to all other nodes in a weighted graph with non-negative edge weights.

### Why Dijkstra?

Consider the following graph:

```text
       1
   1 -----> 2
   |         |
  4|         |1
   |         |
   v         v
   4 <----- 3
       1
```

There are two possible ways to reach node 4 from node 1:

**Path 1:**

```text
1 -> 4

Cost = 4
```

**Path 2:**

```text
1 -> 2 -> 3 -> 4

Cost = 1 + 1 + 1 = 3
```

Dijkstra identifies the minimum-cost path.

### Important Observation

The answer is not necessarily the direct edge weight.

We must consider all possible paths and select the shortest one.

---

## 3. Graph Representation — Adjacency List

First, convert the given edge list into an adjacency list.

Given:

```cpp
times = {
    {1, 2, 1},
    {1, 4, 4},
    {2, 3, 1},
    {3, 4, 1}
};
```

Create:

```cpp
vector<vector<pair<int, int>>> adj(n + 1);
```

Each adjacency entry stores:

```text
{neighbor, weight}
```

Build the adjacency list:

```cpp
for (auto &edge : times) {

    int u = edge[0];
    int v = edge[1];
    int wt = edge[2];

    adj[u].push_back({v, wt});
}
```

Result:

```text
adj[1] = {(2,1), (4,4)}
adj[2] = {(3,1)}
adj[3] = {(4,1)}
adj[4] = {}
```

### Why `n + 1`?

Nodes are labeled from `1` to `n`, so we use an array of size `n + 1` to access nodes directly by their labels.

### Why `pair<int, int>`?

We need to store:

- The neighboring node.
- The edge weight.

---

## 4. Optimized Approach — Dijkstra + Min Heap

We use:

1. Adjacency List to represent the graph.
2. Distance Array to store shortest distances.
3. Min Heap to process the node with the smallest current distance.

### Step 1: Initialize Distance Array

```cpp
vector<int> dist(n + 1, INT_MAX);
```

Initially, all nodes are considered unreachable.

Set the starting node distance to zero:

```cpp
dist[k] = 0;
```

Why zero?

Because the signal starts from node `k`, so no time is required to reach the starting node.

### Step 2: Initialize Min Heap

```cpp
priority_queue<
    pair<int, int>,
    vector<pair<int, int>>,
    greater<pair<int, int>>
> pq;
```

Store:

```text
{distance, node}
```

Insert the starting node:

```cpp
pq.push({0, k});
```

### Step 3: Process the Minimum Distance Node

```cpp
while (!pq.empty()) {

    auto [time, node] = pq.top();
    pq.pop();

}
```

The min heap always gives us the node with the smallest currently known distance.

### Step 4: Relax All Adjacent Edges

For every neighbor:

```cpp
for (auto &[neighbor, wt] : adj[node]) {

    int newTime = time + wt;

    if (newTime < dist[neighbor]) {

        dist[neighbor] = newTime;

        pq.push({newTime, neighbor});
    }
}
```

This operation is called **relaxation**.

We check whether reaching a neighbor through the current node gives a shorter distance.

If it does, update the distance and push the updated state into the priority queue.

### Step 5: Find the Maximum Distance

After Dijkstra finishes, all reachable nodes have their shortest distances.

The signal reaches everyone only when the slowest node receives it.

Therefore, calculate the maximum shortest distance.

If any node has distance `INT_MAX`, return `-1`.

---

## 5. Complete C++ Code — Optimized Dijkstra

```cpp
class Solution {
public:

    int networkDelayTime(vector<vector<int>>& times,
                         int n,
                         int k) {

        // Step 1: Build adjacency list
        vector<vector<pair<int, int>>> adj(n + 1);

        for (auto &edge : times) {

            int u = edge[0];
            int v = edge[1];
            int wt = edge[2];

            adj[u].push_back({v, wt});
        }

        // Step 2: Initialize distances
        vector<int> dist(n + 1, INT_MAX);

        dist[k] = 0;

        // Step 3: Min Heap
        // {distance, node}
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        pq.push({0, k});

        // Step 4: Dijkstra's Algorithm
        while (!pq.empty()) {

            auto [time, node] = pq.top();
            pq.pop();

            // Ignore outdated entries
            if (time > dist[node]) {
                continue;
            }

            // Explore adjacent nodes
            for (auto &[neighbor, wt] : adj[node]) {

                int newTime = time + wt;

                // Relaxation
                if (newTime < dist[neighbor]) {

                    dist[neighbor] = newTime;

                    pq.push({newTime, neighbor});
                }
            }
        }

        // Step 5: Find maximum shortest distance
        int ans = 0;

        for (int i = 1; i <= n; i++) {

            if (dist[i] == INT_MAX) {
                return -1;
            }

            ans = max(ans, dist[i]);
        }

        return ans;
    }
};
```

---

## 6. Step-by-Step Dry Run

Input:

```cpp
times = {
    {1, 2, 1},
    {1, 4, 4},
    {2, 3, 1},
    {3, 4, 1}
};

n = 4;
k = 1;
```

### Step 1: Initial State

Distance array:

```text
Node:      1    2    3    4

Distance:  0   INF  INF  INF
```

Priority Queue:

```text
{0, 1}
```

Here:

```text
{distance, node}
```

### Step 2: Process Node 1

Pop:

```text
{0, 1}
```

Neighbors of node 1:

```text
1 -> 2 (weight 1)
1 -> 4 (weight 4)
```

Relax node 2:

```text
0 + 1 = 1
```

Update:

```text
dist[2] = 1
```

Relax node 4:

```text
0 + 4 = 4
```

Update:

```text
dist[4] = 4
```

Distance array:

```text
[0, 1, INF, 4]
```

Priority Queue:

```text
{1, 2}
{4, 4}
```

### Step 3: Process Node 2

Pop:

```text
{1, 2}
```

Neighbor:

```text
2 -> 3 (weight 1)
```

Calculate:

```text
1 + 1 = 2
```

Update:

```text
dist[3] = 2
```

Distance array:

```text
[0, 1, 2, 4]
```

Priority Queue:

```text
{2, 3}
{4, 4}
```

### Step 4: Process Node 3

Pop:

```text
{2, 3}
```

Neighbor:

```text
3 -> 4 (weight 1)
```

Calculate:

```text
2 + 1 = 3
```

Previously:

```text
dist[4] = 4
```

New distance:

```text
3 < 4
```

Therefore, update:

```text
dist[4] = 3
```

Push:

```text
{3, 4}
```

Priority Queue:

```text
{3, 4}
{4, 4}
```

### Step 5: Process Node 4

Pop:

```text
{3, 4}
```

Node 4 has no outgoing edges.

Continue processing the queue.

The next entry is:

```text
{4, 4}
```

This is outdated because:

```text
4 > dist[4]
4 > 3
```

We skip it.

```cpp
if (time > dist[node]) {
    continue;
}
```

### Step 6: Final Distance Array

```text
Node:      1    2    3    4

Distance:  0    1    2    3
```

Maximum distance:

```text
max(0, 1, 2, 3) = 3
```

**Final Answer:**

```cpp
3
```

---

## 7. Important Concept — Relaxation

Relaxation is the most important operation in Dijkstra's algorithm.

Consider:

```text
1 -> 4 (weight 4)
```

Initially:

```cpp
dist[4] = 4;
```

Later, we discover another path:

```text
1 -> 2 -> 3 -> 4
```

Its total cost is:

```text
1 + 1 + 1 = 3
```

Now compare:

```text
new distance = 3
old distance = 4
```

Since:

```text
3 < 4
```

Update:

```cpp
dist[4] = 3;
```

General relaxation formula:

```cpp
if (dist[node] + weight < dist[neighbor]) {

    dist[neighbor] = dist[node] + weight;

}
```

This is how Dijkstra continuously improves shortest-distance estimates.

---

## 8. Why Use a Min Heap?

We need to process the node having the smallest current distance first.

A normal queue follows FIFO order.

A priority queue allows us to retrieve the smallest distance efficiently.

### Min Heap Declaration

```cpp
priority_queue<
    pair<int, int>,
    vector<pair<int, int>>,
    greater<pair<int, int>>
> pq;
```

### What Does `greater<pair<int,int>>` Do?

It creates a min heap.

For example:

```text
Inserted:

{4, 4}
{1, 2}
{2, 3}
```

Elements are removed in this order:

```text
{1, 2}
{2, 3}
{4, 4}
```

The first value is the distance, so the smallest distance is processed first.

---

## 9. Why Do We Skip Outdated Entries?

Consider this situation:

```text
Node 4 initially has distance 4.
Later, a better path gives distance 3.
```

The priority queue may contain:

```text
{3, 4}
{4, 4}
```

After processing `{3, 4}`, the older entry `{4, 4}` is no longer useful.

We skip it using:

```cpp
if (time > dist[node]) {
    continue;
}
```

This avoids unnecessary processing.

**Important:** This check is an optimization commonly used with the priority-queue implementation of Dijkstra.

---

## 10. Complexity Analysis

Let:

- `V` = Number of vertices (`n`).
- `E` = Number of directed edges.

### Building the Adjacency List

Every edge is processed once.

```text
O(V + E)
```

### Dijkstra's Algorithm

Using a binary min heap:

- Each successful relaxation can insert an entry into the heap.
- Each heap insertion or removal takes logarithmic time.
- There can be up to `O(E)` successful relaxations.

Therefore:

```text
O((V + E) log V)
```

### Final Distance Traversal

We inspect all nodes once:

```text
O(V)
```

### Overall Time Complexity

```text
O((V + E) log V)
```

### Space Complexity

Adjacency list:

```text
O(V + E)
```

Distance array:

```text
O(V)
```

Priority queue:

```text
O(E)
```

Overall:

```text
O(V + E)
```

---

## 11. Common Mistakes

1. **Using an ordinary queue instead of a priority queue**

   A normal queue does not process the smallest-distance node first.

   Use a min heap for Dijkstra.

2. **Forgetting that the graph is directed**

   Do not add reverse edges unless the problem explicitly says the graph is undirected.

3. **Incorrect priority queue ordering**

   Store:

   ```cpp
   {distance, node}
   ```

   Not:

   ```cpp
   {node, distance}
   ```

4. **Initializing all distances to zero**

   Use:

   ```cpp
   vector<int> dist(n + 1, INT_MAX);
   ```

5. **Forgetting the starting node distance**

   ```cpp
   dist[k] = 0;
   ```

6. **Returning the last processed distance**

   The answer is the maximum of all shortest distances, not the distance of the last node processed.

7. **Not checking unreachable nodes**

   If any distance is still `INT_MAX`, return `-1`.

8. **Using the wrong relaxation condition**

   Correct:

   ```cpp
   if (newTime < dist[neighbor])
   ```

9. **Ignoring outdated priority queue entries**

   Use:

   ```cpp
   if (time > dist[node]) {
       continue;
   }
   ```

---

## 12. Dijkstra vs BFS vs Bellman-Ford

| Algorithm | Suitable Graph | Main Idea |
|---|---|---|
| BFS | Unweighted graph | Level-by-level traversal |
| Dijkstra | Weighted graph with non-negative weights | Min heap and relaxation |
| Bellman-Ford | Weighted graph with possible negative edges | Repeated edge relaxation |

### When to Use Dijkstra?

Use Dijkstra when:

- The graph is weighted.
- Edge weights are non-negative.
- You need shortest distances from one source.
- You want an efficient shortest-path solution.

### Important Limitation

Standard Dijkstra does not correctly handle graphs containing negative-weight edges.

Bellman-Ford is a suitable alternative when negative weights are allowed.

---

## 13. Important Interview Questions

**Q1. What is Dijkstra's algorithm?**

It is a greedy shortest-path algorithm that calculates minimum distances from one source to all reachable nodes in a weighted graph with non-negative edge weights.

**Q2. Why do we use a priority queue?**

To process the node with the smallest current distance first.

**Q3. Why is the starting node distance zero?**

Because the signal starts from that node.

**Q4. Why do we return the maximum shortest distance?**

The signal reaches all nodes only when the slowest-to-reach node receives it.

**Q5. Why can Dijkstra fail with negative weights?**

Its greedy assumption that the smallest finalized distance cannot later improve is not valid when negative edges exist.

**Q6. What does relaxation mean?**

Checking whether a path through the current node provides a shorter distance to a neighboring node.

**Q7. Why use an adjacency list?**

It stores only existing edges and makes neighbor traversal efficient.

**Q8. What happens if a node is unreachable?**

Its distance remains `INT_MAX`, so the answer is `-1`.

**Q9. Can Dijkstra be used on a directed graph?**

Yes. The adjacency list must preserve the edge direction.

**Q10. What is the time complexity?**

```text
O((V + E) log V)
```

Using an adjacency list and binary min heap.

---

## 14. Related NeetCode Problems

| Problem | Main Pattern |
|---|---|
| Network Delay Time | Dijkstra |
| Word Ladder | BFS Shortest Path |
| Cheapest Flights Within K Stops | Modified Shortest Path |
| Path With Minimum Effort | Dijkstra / Minimax Path |
| Swim in Rising Water | Dijkstra / Minimax Path |
| Number of Islands | Grid BFS / DFS |
| Course Schedule | Topological Sort |

---

## 15. Final Revision Notes

### Core Pattern

```text
Dijkstra + Adjacency List + Min Heap
```

### Algorithm

```text
1. Build an adjacency list.
2. Initialize all distances to INF.
3. Set dist[k] = 0.
4. Insert {0, k} into the min heap.
5. While the priority queue is not empty:
   a. Extract the smallest distance.
   b. Skip outdated entries.
   c. Explore all adjacent nodes.
   d. Relax the edges.
   e. Push improved distances into the heap.
6. Traverse the distance array.
7. If any node is unreachable, return -1.
8. Otherwise, return the maximum shortest distance.
```

### Key Takeaways

1. Dijkstra finds shortest paths from one source.
2. It works with non-negative edge weights.
3. Use an adjacency list for graph representation.
4. Use a min heap to process the smallest distance first.
5. Relaxation updates better distances.
6. Store `{distance, node}` in the priority queue.
7. Skip outdated entries.
8. The final answer is the maximum shortest distance.
9. Return `-1` if even one node is unreachable.

**Recommended Approach:** Dijkstra + Min Heap.

**Interview Pattern:** Single Source Shortest Path.


# Reconstruct Flight Path — NeetCode 150

- **Difficulty:** Hard
- **Topics:** Graph, DFS, Eulerian Path, Hierholzer's Algorithm
- **Pattern:** DFS + Postorder Traversal
- **Language:** C++

## 1. Problem Understanding

We are given a list of flight tickets.

Each ticket is represented as:

```cpp
tickets[i] = {from, to};
```

Where:
- `from` = Departure airport.
- `to` = Destination airport.

We need to reconstruct the complete flight itinerary.

### Important Conditions

1. The journey always starts from `"JFK"`.
2. Every ticket must be used exactly once.
3. We must return the airports in the order they are visited.
4. If multiple valid itineraries exist, return the lexicographically smallest itinerary.
5. We can assume that at least one valid itinerary exists.

### Example 1

**Input:**

```cpp
tickets = {
    {"MUC", "LHR"},
    {"JFK", "MUC"},
    {"SFO", "SJC"},
    {"LHR", "SFO"}
};
```

**Flight Path:**

```text
JFK -> MUC -> LHR -> SFO -> SJC
```

**Output:**

```cpp
{"JFK", "MUC", "LHR", "SFO", "SJC"}
```

### Example 2

**Input:**

```cpp
tickets = {
    {"JFK", "SFO"},
    {"JFK", "ATL"},
    {"SFO", "ATL"},
    {"ATL", "JFK"},
    {"ATL", "SFO"}
};
```

**Output:**

```cpp
{"JFK", "ATL", "JFK", "SFO", "ATL", "SFO"}
```

There are multiple possible flight paths, but we must select the lexicographically smallest valid itinerary.

---

## 2. Core Concept — Eulerian Path

Before solving this problem, understand the difference between two graph concepts.

### Hamiltonian Path

A Hamiltonian path visits every vertex exactly once.

```text
A -> B -> C -> D
```

Every airport is visited once.

### Eulerian Path

An Eulerian path uses every edge exactly once.

```text
A -> B -> C -> A -> D
```

A vertex can be visited multiple times, but every edge must be used exactly once.

### Which One Does This Problem Use?

This problem requires an Eulerian Path because:

- Airports represent vertices.
- Flight tickets represent directed edges.
- Every ticket must be used exactly once.
- Airports can appear multiple times.

**Important:** Do not confuse visiting every airport with using every ticket.

---

## 3. Graph Representation — Adjacency List

We can represent the flight tickets using an adjacency list.

For every departure airport, store its destination airports.

Example:

```cpp
tickets = {
    {"JFK", "SFO"},
    {"JFK", "ATL"},
    {"SFO", "ATL"},
    {"ATL", "JFK"},
    {"ATL", "SFO"}
};
```

Graph:

```text
JFK -> SFO, ATL
SFO -> ATL
ATL -> JFK, SFO
```

We can use:

```cpp
unordered_map<string, vector<string>> adj;
```

Build the graph:

```cpp
for (auto &ticket : tickets) {

    string from = ticket[0];
    string to = ticket[1];

    adj[from].push_back(to);
}
```

However, we must also handle the lexicographical ordering of destinations.

---

## 4. Optimized Approach — DFS + Hierholzer's Algorithm

The main challenge is that choosing the smallest available destination immediately does not always produce a valid final itinerary.

We need to use DFS with postorder insertion.

This approach is based on Hierholzer's Algorithm.

### Step 1: Build the Adjacency List

```cpp
unordered_map<string, multiset<string>> adj;
```

We use a `multiset` to maintain destinations in sorted order.

Alternatively, we can use vectors and sort them in reverse order.

### Step 2: Start DFS From JFK

The starting airport is always:

```cpp
"JFK"
```

### Step 3: Explore Every Available Flight

For the current airport:

1. Select the lexicographically smallest available destination.
2. Remove that flight ticket from the graph.
3. Recursively visit the destination.
4. Continue until no outgoing tickets remain.

### Step 4: Add Airports After DFS

This is the most important part.

When an airport has no remaining outgoing tickets, add it to the itinerary.

```cpp
route.push_back(airport);
```

This is called **postorder traversal**.

### Step 5: Reverse the Result

Because airports are inserted after exploring all outgoing flights, the resulting route is built in reverse order.

Therefore:

```cpp
reverse(route.begin(), route.end());
```

---

## 5. Why Postorder DFS Is Important

Consider:

```text
JFK -> KUL
JFK -> NRT
NRT -> JFK
```

The lexicographically smaller destination from JFK is KUL.

A simple greedy approach would choose:

```text
JFK -> KUL
```

But after reaching KUL, no flights remain.

The remaining tickets are:

```text
JFK -> NRT -> JFK
```

The valid itinerary must be:

```text
JFK -> NRT -> JFK -> KUL
```

The key is that we cannot finalize the itinerary simply by choosing the smallest destination first.

Instead, DFS explores the available edges and adds airports after their outgoing edges have been exhausted.

This is why postorder traversal is necessary.

---

## 6. Complete C++ Code — Optimized DFS

```cpp
class Solution {
public:

    unordered_map<string, multiset<string>> adj;

    vector<string> route;

    void dfs(string airport) {

        while (!adj[airport].empty()) {

            // Get lexicographically smallest destination
            string next = *adj[airport].begin();

            // Remove the ticket after using it
            adj[airport].erase(adj[airport].begin());

            // Explore the destination
            dfs(next);
        }

        // Postorder insertion
        route.push_back(airport);
    }

    vector<string> findItinerary(
        vector<vector<string>>& tickets) {

        // Build adjacency list
        for (auto &ticket : tickets) {

            string from = ticket[0];
            string to = ticket[1];

            adj[from].insert(to);
        }

        // Start DFS from JFK
        dfs("JFK");

        // Reverse postorder result
        reverse(route.begin(), route.end());

        return route;
    }
};
```

---

## 7. Step-by-Step Dry Run

Input:

```cpp
tickets = {
    {"JFK", "SFO"},
    {"JFK", "ATL"},
    {"SFO", "ATL"},
    {"ATL", "JFK"},
    {"ATL", "SFO"}
};
```

### Step 1: Build the Graph

```text
JFK -> ATL, SFO
ATL -> JFK, SFO
SFO -> ATL
```

Destinations are stored in sorted order.

### Step 2: Start DFS From JFK

Current airport:

```text
JFK
```

Available destinations:

```text
ATL, SFO
```

Choose the smallest:

```text
JFK -> ATL
```

Remove the ticket from the graph.

### Step 3: Process ATL

Available destinations:

```text
JFK, SFO
```

Choose:

```text
ATL -> JFK
```

Remove this ticket and continue.

### Step 4: Process JFK Again

Available destination:

```text
SFO
```

Choose:

```text
JFK -> SFO
```

### Step 5: Process SFO

Available destination:

```text
ATL
```

Choose:

```text
SFO -> ATL
```

### Step 6: Process ATL Again

Available destination:

```text
SFO
```

Choose:

```text
ATL -> SFO
```

### Step 7: Process SFO Again

No outgoing tickets remain.

Insert:

```text
route = [SFO]
```

Now return to ATL.

ATL has no remaining outgoing tickets.

Insert:

```text
route = [SFO, ATL]
```

Return to SFO.

No outgoing tickets remain.

Insert:

```text
route = [SFO, ATL, SFO]
```

Return to JFK.

No outgoing tickets remain.

Insert:

```text
route = [SFO, ATL, SFO, JFK]
```

Return to ATL.

No outgoing tickets remain.

Insert:

```text
route = [SFO, ATL, SFO, JFK, ATL]
```

Finally, return to the original JFK call.

Insert:

```text
route = [SFO, ATL, SFO, JFK, ATL, JFK]
```

### Step 8: Reverse the Result

Before reversing:

```text
[SFO, ATL, SFO, JFK, ATL, JFK]
```

After reversing:

```text
[JFK, ATL, JFK, SFO, ATL, SFO]
```

**Final Output:**

```cpp
{"JFK", "ATL", "JFK", "SFO", "ATL", "SFO"}
```

---

## 8. Alternative Approach — Sort Destinations + DFS

Instead of using a multiset, we can use a vector and sort destinations in descending order.

Why descending order?

Because we can then remove the smallest destination efficiently from the back of the vector.

### C++ Code

```cpp
class Solution {
public:

    unordered_map<string, vector<string>> adj;

    vector<string> route;

    void dfs(string airport) {

        while (!adj[airport].empty()) {

            string next = adj[airport].back();

            adj[airport].pop_back();

            dfs(next);
        }

        route.push_back(airport);
    }

    vector<string> findItinerary(
        vector<vector<string>>& tickets) {

        // Build graph
        for (auto &ticket : tickets) {

            adj[ticket[0]].push_back(ticket[1]);
        }

        // Sort destinations in descending order
        for (auto &entry : adj) {

            sort(
                entry.second.rbegin(),
                entry.second.rend()
            );
        }

        dfs("JFK");

        reverse(route.begin(), route.end());

        return route;
    }
};
```

### Why Is This Approach Useful?

Using a vector:

- Sorting takes `O(E log E)` overall.
- Removing the last element takes `O(1)`.
- No multiset insertion/deletion overhead.

This is generally an efficient implementation for this problem.

---

## 9. Complexity Analysis

Let:

- `E` = Number of flight tickets.
- `V` = Number of airports.

### Approach 1: Multiset + DFS

Building the graph:

```text
O(E log E)
```

Each ticket is removed exactly once.

Each multiset deletion takes logarithmic time.

DFS and edge processing:

```text
O(E log E)
```

Overall Time Complexity:

```text
O(E log E)
```

Space Complexity:

```text
O(V + E)
```

The adjacency list stores all tickets, while the DFS recursion and route require additional space.

### Approach 2: Sorted Vector + DFS

Building the graph:

```text
O(E)
```

Sorting destinations:

```text
O(E log E)
```

Each ticket is removed once in constant time from the back of a vector.

DFS:

```text
O(E)
```

Overall Time Complexity:

```text
O(E log E)
```

Space Complexity:

```text
O(V + E)
```

---

## 10. Common Mistakes

### 1. Using BFS Instead of DFS

BFS does not naturally construct the required Eulerian itinerary with postorder insertion.

Use DFS with Hierholzer's Algorithm.

### 2. Visiting Each Airport Only Once

An airport can appear multiple times.

The requirement is to use each ticket exactly once.

### 3. Forgetting to Remove Used Tickets

If we do not remove an edge after using it, DFS may repeatedly process the same ticket.

Correct:

```cpp
adj[airport].erase(adj[airport].begin());
```

### 4. Returning Immediately After Reaching a Dead End

Do not treat a dead end as the end of the complete itinerary.

Add the airport to the route and return to the previous recursive call.

### 5. Forgetting to Reverse the Result

We insert airports in postorder.

Therefore, reverse the final route.

### 6. Choosing the Smallest Destination and Immediately Finalizing It

Lexicographical ordering alone is not enough.

We must ensure every ticket is used in a valid itinerary.

### 7. Confusing Vertices and Edges

- Airport = Vertex.
- Ticket = Directed Edge.

Every edge must be used exactly once.

---

## 11. Important Interview Questions

**Q1. Which graph algorithm is used in Reconstruct Flight Path?**

Hierholzer's Algorithm using DFS.

**Q2. Is this a Hamiltonian Path or Eulerian Path problem?**

Eulerian Path, because every edge must be used exactly once.

**Q3. Can an airport appear multiple times?**

Yes. Airports can be visited multiple times.

**Q4. Why do we use a multiset?**

To maintain destinations in lexicographical order and efficiently remove the smallest available destination.

**Q5. Why do we insert airports after DFS?**

Because postorder insertion allows the algorithm to correctly handle routes that contain cycles and dead ends.

**Q6. Why do we reverse the final route?**

DFS adds airports after exhausting their outgoing edges, so the route is constructed in reverse order.

**Q7. What is Hierholzer's Algorithm?**

It is an algorithm used to construct an Eulerian path or circuit by traversing unused edges and combining the resulting paths.

**Q8. Why can't we simply use greedy DFS?**

Choosing the smallest destination immediately does not guarantee that the partial path can be finalized at that point.

Postorder insertion handles this correctly.

**Q9. What is the time complexity?**

```text
O(E log E)
```

For the sorted-vector implementation.

**Q10. What does every ticket represent?**

A directed edge from the departure airport to the destination airport.

---

## 12. Related Graph Problems

| Problem | Main Pattern |
|---|---|
| Reconstruct Flight Path | Eulerian Path + DFS |
| Network Delay Time | Dijkstra |
| Word Ladder | BFS Shortest Path |
| Course Schedule | Topological Sort |
| Course Schedule II | Topological Sort |
| Clone Graph | DFS / BFS |
| Number of Connected Components | DFS / BFS / DSU |
| Redundant Connection | Union Find |

---

## 13. Final Revision Notes

### Core Pattern

```text
Eulerian Path + Hierholzer's Algorithm
```

### Algorithm

```text
1. Build a directed adjacency list.
2. Sort destinations lexicographically.
3. Start DFS from JFK.
4. Select the smallest available destination.
5. Remove the used ticket.
6. Recursively visit the destination.
7. When no outgoing tickets remain:
   - Add the current airport to the route.
8. Reverse the final route.
9. Return the itinerary.
```

### Key Takeaways

1. Every flight ticket must be used exactly once.
2. Airports can appear multiple times.
3. This is an Eulerian Path problem.
4. Use Hierholzer's Algorithm.
5. DFS explores unused edges.
6. Postorder insertion is essential.
7. Reverse the final route.
8. Lexicographical ordering determines which valid itinerary to return.
9. A sorted vector with `pop_back()` is an efficient implementation.

**Recommended Approach:** Sorted Adjacency List + DFS + Postorder.

**Interview Pattern:** Eulerian Path / Hierholzer's Algorithm.


# Min Cost to Connect All Points — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, Minimum Spanning Tree, Prim's Algorithm, Kruskal's Algorithm, Union Find
- **Pattern:** Minimum Spanning Tree (MST)
- **Language:** C++

## 1. Problem Understanding

We are given a 2D array `points`, where:

```cpp
points[i] = {xi, yi};
```

Each element represents a distinct point on a 2D plane.

We need to connect all points such that:

1. Every point is connected to every other point through some path.
2. There is exactly one path between every pair of points.
3. The total cost of connecting all points is minimized.

### Cost Calculation

The cost of connecting two points is their Manhattan distance.

Formula:

```text
Cost = |x1 - x2| + |y1 - y2|
```

### Example 1

**Input:**

```cpp
points = {{0,0}, {2,2}, {3,3}, {2,4}, {4,2}};
```

**Output:**

```text
10
```

Explanation:

One possible minimum-cost connection is:

```text
(0,0) -> (2,2) = 4
(2,2) -> (3,3) = 2
(3,3) -> (2,4) = 2
(3,3) -> (4,2) = 2
```

Total cost:

```text
4 + 2 + 2 + 2 = 10
```

---

## 2. Core Concept — Minimum Spanning Tree (MST)

Before solving this problem, understand Minimum Spanning Tree.

A Minimum Spanning Tree is a subset of edges in a connected, undirected, weighted graph that:

1. Connects all vertices.
2. Contains no cycles.
3. Has exactly `V - 1` edges.
4. Has the minimum possible total edge weight.

### Important Properties

For `V` vertices:

```text
Number of MST edges = V - 1
```

For example, if there are 5 points:

```text
MST edges = 5 - 1 = 4
```

### Why Do We Need MST?

The problem asks us to:

- Connect all points.
- Avoid unnecessary connections.
- Minimize the total connection cost.

These are exactly the requirements of a Minimum Spanning Tree.

**Important:** We do not need to connect every pair directly. We only need enough edges to make the entire graph connected.

---

## 3. Graph Representation

We can consider every point as a vertex.

The cost between any two points is their Manhattan distance.

For `n` points, every point can connect to every other point.

Therefore, this is a complete weighted graph.

Number of possible edges:

```text
E = n * (n - 1) / 2
```

For example, with 5 points:

```text
E = 5 * 4 / 2 = 10
```

However, explicitly building all these edges is not necessary when using Prim's Algorithm.

---

## 4. Optimized Approach — Prim's Algorithm

Prim's Algorithm builds the Minimum Spanning Tree by repeatedly selecting the minimum-cost edge that connects an already visited vertex to an unvisited vertex.

### Main Idea

Maintain three things:

1. `visited[]` — Tracks which points are already included in the MST.
2. `minDist[]` — Stores the minimum cost required to connect each point to the current MST.
3. `totalCost` — Stores the total MST cost.

### Step-by-Step Algorithm

**Step 1: Initialize**

```cpp
int n = points.size();

vector<int> minDist(n, INT_MAX);
vector<bool> visited(n, false);

minDist[0] = 0;
```

We start from point `0`.

Its connection cost is initially zero because it is our starting point.

**Step 2: Select the Minimum-Cost Unvisited Point**

Find the unvisited point with the smallest `minDist`.

**Step 3: Include It in the MST**

Mark the selected point as visited and add its minimum connection cost to the answer.

**Step 4: Update Neighbor Costs**

Calculate the Manhattan distance between the selected point and every unvisited point.

If the newly calculated distance is smaller than the existing `minDist`, update it.

**Step 5: Repeat**

Continue until all points are included in the MST.

---

## 5. Complete C++ Code — Prim's Algorithm

```cpp
class Solution {
public:

    int minCostConnectPoints(vector<vector<int>>& points) {

        int n = points.size();

        vector<int> minDist(n, INT_MAX);

        vector<bool> visited(n, false);

        minDist[0] = 0;

        int totalCost = 0;

        // Include all points in the MST
        for (int count = 0; count < n; count++) {

            int minCost = INT_MAX;
            int u = -1;

            // Find the minimum-cost unvisited point
            for (int i = 0; i < n; i++) {

                if (!visited[i] && minDist[i] < minCost) {

                    minCost = minDist[i];

                    u = i;
                }
            }

            // Include this point in MST
            visited[u] = true;

            totalCost += minCost;

            // Update minimum connection costs
            for (int v = 0; v < n; v++) {

                if (!visited[v]) {

                    int distance =
                        abs(points[u][0] - points[v][0]) +
                        abs(points[u][1] - points[v][1]);

                    minDist[v] = min(minDist[v], distance);
                }
            }
        }

        return totalCost;
    }
};
```

---

## 6. Step-by-Step Dry Run

Input:

```cpp
points = {
    {0,0},
    {2,2},
    {3,3},
    {2,4},
    {4,2}
};
```

Assign indices:

```text
P0 = (0,0)
P1 = (2,2)
P2 = (3,3)
P3 = (2,4)
P4 = (4,2)
```

### Step 1: Initialize

```text
minDist = [0, INF, INF, INF, INF]

visited = [false, false, false, false, false]

totalCost = 0
```

Start from P0.

### Step 2: Select P0

P0 has minimum connection cost `0`.

Mark it visited.

Calculate distances from P0:

```text
P0 -> P1 = |0-2| + |0-2| = 4

P0 -> P2 = |0-3| + |0-3| = 6

P0 -> P3 = |0-2| + |0-4| = 6

P0 -> P4 = |0-4| + |0-2| = 6
```

Updated values:

```text
minDist = [0, 4, 6, 6, 6]

visited = [true, false, false, false, false]

totalCost = 0
```

### Step 3: Select P1

P1 has the smallest unvisited connection cost: `4`.

Add 4 to the answer.

```text
totalCost = 4
```

Calculate distances from P1:

```text
P1 -> P2 = |2-3| + |2-3| = 2

P1 -> P3 = |2-2| + |2-4| = 2

P1 -> P4 = |2-4| + |2-2| = 2
```

Update minimum distances:

```text
minDist = [0, 4, 2, 2, 2]
```

### Step 4: Select P2

P2 has the smallest unvisited connection cost: `2`.

```text
totalCost = 4 + 2 = 6
```

Calculate distances from P2:

```text
P2 -> P3 = |3-2| + |3-4| = 2

P2 -> P4 = |3-4| + |3-2| = 2
```

Updated values:

```text
minDist = [0, 4, 2, 2, 2]
```

### Step 5: Select P3

P3 has a minimum connection cost of `2`.

```text
totalCost = 6 + 2 = 8
```

Calculate distance from P3 to P4:

```text
P3 -> P4 = |2-4| + |4-2| = 4
```

P4 already has a better connection cost of `2`.

Therefore, we do not update it.

### Step 6: Select P4

P4 has a minimum connection cost of `2`.

```text
totalCost = 8 + 2 = 10
```

All points are now visited.

### Final Result

```text
Minimum Cost = 10
```

### Dry Run Summary

| Selected Point | Connection Cost | Total Cost |
|---|---:|---:|
| P0 | 0 | 0 |
| P1 | 4 | 4 |
| P2 | 2 | 6 |
| P3 | 2 | 8 |
| P4 | 2 | 10 |

---

## 7. Alternative Approach — Kruskal's Algorithm

Kruskal's Algorithm also finds the Minimum Spanning Tree.

Instead of expanding the MST from one starting point, it:

1. Generates all possible edges.
2. Sorts edges by increasing weight.
3. Selects the smallest edge.
4. Uses DSU (Disjoint Set Union) to detect cycles.
5. Adds an edge only if it connects two different components.
6. Stops after selecting `n - 1` edges.

### Step 1: Generate All Edges

For every pair of points, calculate Manhattan distance.

```cpp
vector<tuple<int, int, int>> edges;

for (int i = 0; i < n; i++) {

    for (int j = i + 1; j < n; j++) {

        int cost =
            abs(points[i][0] - points[j][0]) +
            abs(points[i][1] - points[j][1]);

        edges.push_back({cost, i, j});
    }
}
```

Each edge contains:

```text
{cost, source, destination}
```

### Step 2: Sort Edges

```cpp
sort(edges.begin(), edges.end());
```

### Step 3: Use DSU

If two points already belong to the same component, adding their edge creates a cycle.

Otherwise, merge their components and add the edge cost.

### Complete C++ Code — Kruskal + DSU

```cpp
class Solution {
public:

    vector<int> parent;
    vector<int> rankSize;

    int findParent(int node) {

        if (parent[node] == node) {
            return node;
        }

        return parent[node] = findParent(parent[node]);
    }

    bool unite(int u, int v) {

        u = findParent(u);
        v = findParent(v);

        if (u == v) {
            return false;
        }

        if (rankSize[u] < rankSize[v]) {
            swap(u, v);
        }

        parent[v] = u;

        rankSize[u] += rankSize[v];

        return true;
    }

    int minCostConnectPoints(vector<vector<int>>& points) {

        int n = points.size();

        parent.resize(n);
        rankSize.assign(n, 1);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }

        vector<tuple<int, int, int>> edges;

        // Generate all possible edges
        for (int i = 0; i < n; i++) {

            for (int j = i + 1; j < n; j++) {

                int cost =
                    abs(points[i][0] - points[j][0]) +
                    abs(points[i][1] - points[j][1]);

                edges.push_back({cost, i, j});
            }
        }

        // Sort edges by cost
        sort(edges.begin(), edges.end());

        int totalCost = 0;
        int edgesUsed = 0;

        // Construct MST
        for (auto &[cost, u, v] : edges) {

            if (unite(u, v)) {

                totalCost += cost;

                edgesUsed++;

                if (edgesUsed == n - 1) {
                    break;
                }
            }
        }

        return totalCost;
    }
};
```

---

## 8. Prim's vs Kruskal's Algorithm

| Feature | Prim's | Kruskal's |
|---|---|---|
| Main technique | Greedy + Graph traversal | Greedy + Sorting |
| Starting point | Starts from one vertex | No starting vertex |
| Edge selection | Minimum edge connecting MST to an unvisited vertex | Globally smallest available edge |
| Cycle detection | Avoided using visited array | DSU |
| Graph representation | Can avoid explicitly storing edges | Stores all edges |
| Best suited for | Dense graphs | Sparse graphs |

### Which Approach Should We Prefer?

For this particular problem, prefer **Prim's Algorithm with an array-based implementation**.

Why?

- The graph is complete.
- Every pair of points can be connected.
- We can calculate distances when needed.
- We do not need to store all possible edges.
- It provides `O(n²)` time complexity.

---

## 9. Complexity Analysis

Let `n` be the number of points.

### Prim's Algorithm — Array-Based

Finding the minimum unvisited point:

```text
O(n)
```

Updating distances:

```text
O(n)
```

We repeat these operations for all `n` points.

Overall Time Complexity:

```text
O(n²)
```

Space Complexity:

```text
O(n)
```

We maintain:

- `minDist[]`
- `visited[]`

No complete adjacency list is required.

### Kruskal's Algorithm + DSU

Number of possible edges:

```text
E = n * (n - 1) / 2
```

Therefore:

```text
E = O(n²)
```

Generating edges:

```text
O(n²)
```

Sorting edges:

```text
O(n² log n)
```

DSU operations:

```text
O(n² * α(n))
```

Overall Time Complexity:

```text
O(n² log n)
```

Space Complexity:

```text
O(n²)
```

Because we store all possible edges.

---

## 10. Common Mistakes

### 1. Using Euclidean Distance Instead of Manhattan Distance

Incorrect:

```cpp
sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
```

Correct:

```cpp
abs(x1 - x2) + abs(y1 - y2);
```

### 2. Connecting Every Pair of Points

We do not need every possible edge in the final answer.

An MST contains exactly:

```text
n - 1 edges
```

### 3. Forgetting Cycle Prevention in Kruskal

Without DSU, we might add an edge that creates a cycle.

Use:

```cpp
if (unite(u, v))
```

### 4. Updating Visited Points

In Prim's Algorithm, only update distances for unvisited points.

```cpp
if (!visited[v])
```

### 5. Confusing MST With Shortest Path

MST minimizes the total cost of connecting all vertices.

Shortest Path finds the minimum distance between specific vertices.

These are different graph problems.

### 6. Forgetting the Initial Point Cost

The starting point has a connection cost of zero.

```cpp
minDist[0] = 0;
```

---

## 11. Important Interview Questions

**Q1. Which algorithm is used to solve this problem?**

Prim's or Kruskal's Algorithm.

**Q2. What is a Minimum Spanning Tree?**

A tree connecting all vertices with the minimum possible total edge weight.

**Q3. How many edges does an MST contain?**

For `n` vertices:

```text
n - 1
```

**Q4. Why do we use Manhattan Distance?**

Because the problem explicitly defines connection cost as:

```text
|x1 - x2| + |y1 - y2|
```

**Q5. Why is Prim's Algorithm preferred here?**

Because the graph is complete, and array-based Prim's calculates distances dynamically without storing all edges.

**Q6. How does Kruskal detect cycles?**

Using Disjoint Set Union (DSU).

**Q7. What is the time complexity of Prim's Algorithm here?**

```text
O(n²)
```

**Q8. What is the time complexity of Kruskal's Algorithm here?**

```text
O(n² log n)
```

**Q9. Can an MST contain a cycle?**

No. An MST must be a tree and therefore cannot contain cycles.

**Q10. Is the MST always unique?**

No. If multiple edges have equal weights, multiple different MSTs may have the same minimum total cost.

---

## 12. Related Graph Problems

| Problem | Main Pattern |
|---|---|
| Min Cost to Connect All Points | Prim's / Kruskal's |
| Redundant Connection | DSU |
| Number of Connected Components | DFS / BFS / DSU |
| Graph Valid Tree | DFS / DSU |
| Network Delay Time | Dijkstra |
| Reconstruct Flight Path | Eulerian Path |
| Connecting Cities With Minimum Cost | MST |

---

## 13. Final Revision Notes

### Core Pattern

```text
Minimum Spanning Tree (MST)
```

### Prim's Algorithm

```text
1. Initialize minDist[] with INF.
2. Set minDist[0] = 0.
3. Select the minimum-cost unvisited point.
4. Mark it visited.
5. Add its connection cost to totalCost.
6. Calculate Manhattan distances to unvisited points.
7. Update minDist[] with smaller distances.
8. Repeat until all points are visited.
9. Return totalCost.
```

### Kruskal's Algorithm

```text
1. Generate all possible edges.
2. Calculate Manhattan distance for each edge.
3. Sort edges by increasing cost.
4. Initialize DSU.
5. Select edges in sorted order.
6. Add an edge only if it connects different components.
7. Stop after selecting n - 1 edges.
8. Return totalCost.
```

### Key Takeaways

1. Recognize the MST pattern when asked to connect all points at minimum total cost.
2. Manhattan distance is the edge weight.
3. Prim's grows the MST one vertex at a time.
4. Kruskal's selects edges globally in increasing order.
5. DSU is used for cycle detection in Kruskal.
6. An MST always contains `n - 1` edges.
7. Array-based Prim's is especially efficient for this complete graph.
8. Preferred complexity: `O(n²)` time and `O(n)` space.

**Recommended Interview Approach:** Prim's Algorithm using `minDist[]` and `visited[]`.

**Alternative:** Kruskal's Algorithm using DSU.


# Cheapest Flights Within K Stops — NeetCode 150

- **Difficulty:** Medium
- **Topics:** Graph, Shortest Path, Bellman-Ford, BFS, Dijkstra
- **Pattern:** Shortest Path with Constraints
- **Language:** C++

## 1. Problem Understanding

We are given:

- `n` airports numbered from `0` to `n - 1`.
- `flights[i] = {from, to, price}` representing a directed flight.
- `src`: Starting airport.
- `dst`: Destination airport.
- `k`: Maximum number of stops allowed.

We need to return the cheapest price from `src` to `dst` using **at most `k` stops**.

If no valid route exists, return `-1`.

### Important: Stops vs Flights

This is the most important detail in the problem.

A stop is an intermediate airport. The source and destination are not counted as stops.

Therefore:

```text
Maximum stops = k
Maximum flights (edges) = k + 1
```

Example:

```text
0 -> 1 -> 2 -> 3
```

- Stops: 2 (airports 1 and 2)
- Flights: 3

If `k = 2`, this route is valid.

---

## 2. Example

**Input:**

```cpp
n = 4;

flights = {
    {0, 1, 100},
    {1, 2, 100},
    {2, 3, 100},
    {0, 2, 500},
    {1, 3, 600}
};

src = 0;
dst = 3;
k = 1;
```

**Output:**

```text
600
```

### Explanation

Possible routes:

**Route 1:**

```text
0 -> 1 -> 2 -> 3

Cost = 100 + 100 + 100 = 300
Stops = 2
```

This route is invalid because `k = 1`.

**Route 2:**

```text
0 -> 2 -> 3
```

This route is not available because there is no direct flight from 2 to 3 at the cost shown in this example's route structure? 
```

The valid routes from the given flights are:

```text
0 -> 1 -> 3
Cost = 100 + 600 = 700
Stops = 1

0 -> 2
Cost = 500
```

The destination is airport 3, so the cheapest valid route is:

```text
0 -> 1 -> 3

100 + 600 = 700
```

**Correct output for this input is 700.**

---

## 3. Core Concept — Shortest Path with a Stop Constraint

In a normal shortest-path problem, we try to find the minimum cost to reach a destination.

Here, we have an additional condition:

```text
We can use at most k + 1 edges.
```

A normal shortest-path algorithm may find a cheaper route that uses too many flights.

Therefore, we must track the number of edges or limit how many times we relax the edges.

### Important Pattern Recognition

Whenever a graph problem asks for:

- Cheapest path with at most K stops.
- Shortest path with a maximum number of edges.
- Minimum cost within a limited number of transitions.

Think about:

1. Bellman-Ford with limited iterations.
2. BFS with level tracking.
3. Dijkstra with additional state.

---

## 4. Optimized Approach — Bellman-Ford Algorithm

### What is Bellman-Ford?

Bellman-Ford is a shortest-path algorithm that repeatedly relaxes all edges.

For a graph with `V` vertices, the standard algorithm performs up to:

```text
V - 1 iterations
```

Each iteration allows the shortest path to use one additional edge.

For this problem, instead of running `V - 1` iterations, we only need:

```text
k + 1 iterations
```

Because at most `k + 1` flights are allowed.

### Main Idea

Maintain a distance array:

```cpp
vector<int> dist(n, INT_MAX);
```

`dist[i]` represents the minimum cost to reach airport `i`.

Initialize:

```cpp
dist[src] = 0;
```

Then relax all flights for `k + 1` rounds.

### Important: Why Do We Need a Temporary Array?

We must use a copy of the previous round's distances.

```cpp
vector<int> temp = dist;
```

Why?

Because each iteration should represent paths using at most one additional flight.

If we directly update `dist`, multiple flights may be processed in the same iteration, allowing more than one additional edge.

Using `temp` ensures that every round uses only the previous round's results.

---

## 5. Complete C++ Code — Bellman-Ford

```cpp
class Solution {
public:

    int findCheapestPrice(
        int n,
        vector<vector<int>>& flights,
        int src,
        int dst,
        int k
    ) {

        vector<int> dist(n, INT_MAX);

        dist[src] = 0;

        // At most k stops means at most k + 1 flights
        for (int i = 0; i <= k; i++) {

            vector<int> temp = dist;

            // Relax every flight
            for (auto& flight : flights) {

                int u = flight[0];
                int v = flight[1];
                int price = flight[2];

                if (dist[u] != INT_MAX) {

                    temp[v] = min(
                        temp[v],
                        dist[u] + price
                    );
                }
            }

            // Move to the next iteration
            dist = temp;
        }

        return dist[dst] == INT_MAX ? -1 : dist[dst];
    }
};
```

---

## 6. Step-by-Step Dry Run

Input:

```cpp
n = 4;

flights = {
    {0, 1, 100},
    {1, 2, 100},
    {2, 3, 100},
    {0, 2, 500},
    {1, 3, 600}
};

src = 0;
dst = 3;
k = 1;
```

Maximum number of flights:

```text
k + 1 = 2
```

### Initial State

```text
dist = [0, INF, INF, INF]
```

We start from airport 0.

### Iteration 1 — Maximum 1 Flight

Copy the previous distances:

```text
temp = [0, INF, INF, INF]
```

Process flights:

**Flight 0 -> 1, Cost 100**

```text
temp[1] = min(INF, 0 + 100)
        = 100
```

**Flight 1 -> 2, Cost 100**

`dist[1]` is still INF in this iteration.

Therefore, we cannot use this newly discovered route yet.

**Flight 2 -> 3, Cost 100**

`dist[2]` is INF.

No update.

**Flight 0 -> 2, Cost 500**

```text
temp[2] = min(INF, 0 + 500)
        = 500
```

**Flight 1 -> 3, Cost 600**

`dist[1]` is still INF.

No update.

After iteration 1:

```text
dist = [0, 100, 500, INF]
```

We can reach:

- Airport 1 with cost 100.
- Airport 2 with cost 500.

### Iteration 2 — Maximum 2 Flights

Copy the previous distances:

```text
temp = [0, 100, 500, INF]
```

Process flights:

**Flight 0 -> 1, Cost 100**

```text
temp[1] = min(100, 0 + 100)
        = 100
```

**Flight 1 -> 2, Cost 100**

```text
temp[2] = min(500, 100 + 100)
        = 200
```

**Flight 2 -> 3, Cost 100**

`dist[2]` is 500 in the previous iteration.

```text
temp[3] = min(INF, 500 + 100)
        = 600
```

**Flight 0 -> 2, Cost 500**

```text
temp[2] = min(200, 500)
        = 200
```

**Flight 1 -> 3, Cost 600**

```text
temp[3] = min(600, 100 + 600)
        = 600
```

After iteration 2:

```text
dist = [0, 100, 200, 600]
```

### Final Answer

```text
dist[dst] = dist[3] = 600
```

Therefore:

```text
Output = 600
```

The important point is that the route `0 -> 1 -> 2 -> 3` requires three flights, so it is not allowed when `k = 1`.

---

## 7. Alternative Approach — BFS with Level Tracking

We can also solve this problem using BFS.

Normally, BFS is used to find the shortest path in an unweighted graph.

Here, edges have different prices, so ordinary BFS is not enough.

However, we can use BFS level by level while tracking the minimum cost.

### Main Idea

Each BFS level represents the number of flights taken.

Maintain:

- A queue containing `{airport, cost}`.
- A distance array containing the cheapest known cost.
- A level counter representing the number of flights used.

Process only `k + 1` levels.

### Complete C++ Code

```cpp
class Solution {
public:

    int findCheapestPrice(
        int n,
        vector<vector<int>>& flights,
        int src,
        int dst,
        int k
    ) {

        vector<vector<pair<int, int>>> adj(n);

        for (auto& flight : flights) {

            int u = flight[0];
            int v = flight[1];
            int price = flight[2];

            adj[u].push_back({v, price});
        }

        vector<int> dist(n, INT_MAX);

        queue<pair<int, int>> q;

        q.push({src, 0});

        dist[src] = 0;

        int stops = 0;

        while (!q.empty() && stops <= k) {

            int size = q.size();

            vector<int> temp = dist;

            while (size--) {

                auto [node, cost] = q.front();

                q.pop();

                for (auto& [next, price] : adj[node]) {

                    int newCost = cost + price;

                    if (newCost < temp[next]) {

                        temp[next] = newCost;

                        q.push({next, newCost});
                    }
                }
            }

            dist = temp;

            stops++;
        }

        return dist[dst] == INT_MAX ? -1 : dist[dst];
    }
};
```

### Important Observation

We process BFS level by level.

Each level represents one additional flight.

The temporary array prevents a route discovered in the current level from being used as if it were already available in that same level.

**Note:** This approach can enqueue multiple states for the same airport. It is useful for understanding the level-based pattern, but Bellman-Ford is simpler and more predictable for this problem.

---

## 8. Alternative Approach — Dijkstra with State

Dijkstra's Algorithm can also solve this problem if we track both:

1. The current airport.
2. The number of flights used.

A state looks like:

```text
{cost, airport, flightsUsed}
```

### Main Idea

Use a min-heap to process the cheapest available state first.

For each state:

- If the destination is reached, return its cost.
- If the maximum number of flights has already been reached, do not expand it.
- Otherwise, explore its outgoing flights.

### Complete C++ Code

```cpp
class Solution {
public:

    int findCheapestPrice(
        int n,
        vector<vector<int>>& flights,
        int src,
        int dst,
        int k
    ) {

        vector<vector<pair<int, int>>> adj(n);

        for (auto& flight : flights) {

            int u = flight[0];
            int v = flight[1];
            int price = flight[2];

            adj[u].push_back({v, price});
        }

        // {cost, airport, flightsUsed}
        priority_queue<
            tuple<int, int, int>,
            vector<tuple<int, int, int>>,
            greater<tuple<int, int, int>>
        > pq;

        pq.push({0, src, 0});

        vector<vector<int>> best(
            n,
            vector<int>(k + 2, INT_MAX)
        );

        best[src][0] = 0;

        while (!pq.empty()) {

            auto [cost, node, flightsUsed] = pq.top();

            pq.pop();

            if (node == dst) {
                return cost;
            }

            if (flightsUsed == k + 1) {
                continue;
            }

            if (cost > best[node][flightsUsed]) {
                continue;
            }

            for (auto& [next, price] : adj[node]) {

                int newCost = cost + price;

                int newFlights = flightsUsed + 1;

                if (newCost < best[next][newFlights]) {

                    best[next][newFlights] = newCost;

                    pq.push({
                        newCost,
                        next,
                        newFlights
                    });
                }
            }
        }

        return -1;
    }
};
```

### Why Do We Track Flights Used?

Reaching the same airport with different numbers of flights represents different states.

For example:

```text
Airport 2 reached with:
Cost = 200, Flights = 2

Airport 2 reached with:
Cost = 300, Flights = 1
```

The second route costs more, but it leaves more flights available.

Therefore, we cannot always discard a state simply because another route reaches the same airport at a lower cost.

---

## 9. Bellman-Ford vs BFS vs Dijkstra

| Feature | Bellman-Ford | BFS with Levels | Dijkstra with State |
|---|---|---|---|
| Main technique | Edge relaxation | Level traversal | Greedy + Min Heap |
| Tracks flight limit | Iterations | Levels | State |
| Handles weighted edges | Yes | Yes, with extra cost tracking | Yes |
| Implementation | Simple | Moderate | More complex |
| Recommended here | Yes | Alternative | Alternative |

### Which Approach Should We Prefer?

For this problem, start with **Bellman-Ford using a temporary distance array**.

Why?

- The maximum number of flights is known.
- Each iteration represents one additional flight.
- The implementation is straightforward.
- We do not need to build an adjacency list.

---

## 10. Complexity Analysis

Let:

- `V` = Number of airports.
- `E` = Number of flights.
- `K` = Maximum number of stops.

### Bellman-Ford

We process all flights for `k + 1` iterations.

Time Complexity:

```text
O((k + 1) * E)
```

Space Complexity:

```text
O(V)
```

We maintain two distance arrays.

### BFS with Levels

Each flight may be processed at different levels.

Worst-case Time Complexity:

```text
O((k + 1) * E)
```

Space Complexity:

```text
O(V + E)
```

This includes the adjacency list, queue, and distance arrays. The queue can contain multiple states.

### Dijkstra with State

There can be up to:

```text
O(V * (k + 2))
```

different airport/flight-count states.

Using a priority queue, the approximate worst-case bounds are:

Time Complexity:

```text
O((V * (k + 2) + E * (k + 1)) * log(V * (k + 2)))
```

Space Complexity:

```text
O(V * (k + 2) + E)
```

---

## 11. Common Mistakes

### 1. Confusing Stops with Flights

Incorrect:

```cpp
for (int i = 0; i < k; i++)
```

This processes only `k` flights.

Correct:

```cpp
for (int i = 0; i <= k; i++)
```

Because at most `k + 1` flights are allowed.

### 2. Updating the Same Distance Array

Incorrect:

```cpp
for (auto& flight : flights) {

    dist[v] = min(dist[v], dist[u] + price);
}
```

This can allow multiple edges to be used in one iteration.

Correct:

```cpp
vector<int> temp = dist;
```

Use the previous iteration's `dist` for every relaxation.

### 3. Forgetting the Unreachable Check

Before adding a flight price:

```cpp
if (dist[u] != INT_MAX)
```

Otherwise, adding a price to `INT_MAX` can cause integer overflow.

### 4. Using Ordinary Dijkstra Without Tracking Stops

A cheaper route may use more than `k` stops.

Therefore, normal Dijkstra with only one distance value per airport is not sufficient.

### 5. Returning the First Route Found

The first route found may not be the cheapest valid route.

We must consider all routes within the allowed flight limit.

### 6. Assuming Fewer Flights Always Means a Cheaper Route

A route with fewer flights can be more expensive.

A route with more flights can be cheaper.

Both cost and flight count matter.

---

## 12. Important Interview Questions

**Q1. Why is this problem different from a normal shortest-path problem?**

Because we have a maximum number of stops constraint.

**Q2. How many flights are allowed when `k = 2`?**

```text
k + 1 = 3 flights
```

**Q3. Why is Bellman-Ford suitable here?**

Because each iteration allows paths with one additional edge, so we can limit the number of iterations.

**Q4. Why do we need a temporary distance array?**

To ensure each iteration uses only the previous iteration's distances and does not accidentally use multiple additional edges in one round.

**Q5. What happens if `src == dst`?**

The problem states `src != dst`, but generally the minimum cost would be `0`.

**Q6. Can normal Dijkstra solve this problem directly?**

No. It must be modified to track the number of flights or stops.

**Q7. What is the Bellman-Ford time complexity for this problem?**

```text
O((k + 1) * E)
```

**Q8. What does each Bellman-Ford iteration represent?**

The minimum costs using at most one additional flight.

**Q9. Why can't we simply use BFS?**

Because flight prices are different. Ordinary BFS minimizes the number of edges, not total cost.

**Q10. Can a more expensive route to an airport still be useful?**

Yes. It may use fewer flights and leave more flights available for reaching the destination.

---

## 13. Related Graph Problems

| Problem | Main Pattern |
|---|---|
| Network Delay Time | Dijkstra |
| Cheapest Flights Within K Stops | Bellman-Ford with limited iterations |
| Reconstruct Flight Path | Eulerian Path |
| Min Cost to Connect All Points | Minimum Spanning Tree |
| Course Schedule | Topological Sort |
| Course Schedule II | Topological Sort |
| Word Ladder | BFS |
| Number of Connected Components | DFS / BFS / DSU |

---

## 14. Final Revision Notes

### Core Pattern

```text
Shortest Path with a Constraint
```

### Bellman-Ford Steps

```text
1. Initialize dist[] with INF.
2. Set dist[src] = 0.
3. Repeat k + 1 times.
4. Copy dist[] into temp[].
5. Process every flight.
6. Relax edges using the previous dist[].
7. Assign temp[] back to dist[].
8. Return dist[dst], or -1 if unreachable.
```

### Most Important Formula

```text
Maximum flights = Maximum stops + 1
```

### Complexity

```text
Time:  O((k + 1) * E)
Space: O(V)
```

### Key Takeaways

1. Recognize the stop-limit constraint.
2. Distinguish between stops and flights.
3. Bellman-Ford can limit the number of edges through iterations.
4. Use a temporary array to prevent multiple-edge updates in one round.
5. Normal Dijkstra needs modification to account for flight count.
6. A cheaper intermediate route is not always the best state if it uses too many flights.
7. Prefer Bellman-Ford for a clean interview solution.

**Recommended Interview Approach:** Bellman-Ford with a temporary distance array.

**Alternative Approaches:** BFS with level tracking and Dijkstra with state.
