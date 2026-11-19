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
    int checkHeight(TreeNode* root) {

        // Base case: empty tree is balanced
        if (root == nullptr) {
            return 0;
        }

        // Calculate left subtree height
        int leftHeight = checkHeight(root->left);

        // If left subtree is unbalanced
        if (leftHeight == -1) {
            return -1;
        }

        // Calculate right subtree height
        int rightHeight = checkHeight(root->right);

        // If right subtree is unbalanced
        if (rightHeight == -1) {
            return -1;
        }

        // Check current node's balance condition
        if (abs(leftHeight - rightHeight) > 1) {
            return -1;
        }

        // Return current subtree height
        return 1 + max(leftHeight, rightHeight);
    }

    bool isBalanced(TreeNode* root) {
        return checkHeight(root) != -1;
        
    }
};
