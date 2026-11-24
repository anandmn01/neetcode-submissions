/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }
    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }
    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};
*/

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
