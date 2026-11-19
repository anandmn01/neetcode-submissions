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
    bool validate(TreeNode* root, long long minVal, long long maxVal) {

        // Empty subtree is valid
        if (root == nullptr) {
            return true;
        }

        // Current value must be within the allowed range
        if (root->val <= minVal || root->val >= maxVal) {
            return false;
        }

        // Validate left and right subtrees
        return validate(root->left, minVal, root->val) &&
               validate(root->right, root->val, maxVal);
    }

    bool isValidBST(TreeNode* root) {

        return validate(root, LLONG_MIN, LLONG_MAX);
        
    }
};
