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
