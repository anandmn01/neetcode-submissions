/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;

        // Handle empty tree
        if (root == nullptr) {
            return result;
        }

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {

            // Number of nodes at current level
            int size = q.size();

            vector<int> level;

            // Process all nodes of this level
            for (int i = 0; i < size; i++) {

                TreeNode* node = q.front();
                q.pop();

                level.push_back(node->val);

                // Add left child
                if (node->left != nullptr) {
                    q.push(node->left);
                }

                // Add right child
                if (node->right != nullptr) {
                    q.push(node->right);
                }
            }

            // Store current level
            result.push_back(level);
        }

        return result;
    }
};
