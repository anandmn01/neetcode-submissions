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
    int maxDepth(TreeNode* root) {
        // Base case
        if (root == nullptr) {
            return 0;
        }

        // Calculate left and right subtree depths
        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);

        // Current node + deeper subtree
        return 1 + max(leftDepth, rightDepth);
    }
};
