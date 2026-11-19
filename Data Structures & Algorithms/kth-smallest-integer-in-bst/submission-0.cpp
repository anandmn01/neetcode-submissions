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
    int count = 0;
    int answer = 0;

    void inorder(TreeNode* root, int k) {

        if (root == nullptr) {
            return;
        }

        // Visit left subtree
        inorder(root->left, k);

        // Visit current node
        count++;

        if (count == k) {
            answer = root->val;
            return;
        }

        // Visit right subtree
        inorder(root->right, k);
    }

    int kthSmallest(TreeNode* root, int k) {

        count = 0;
        answer = 0;

        inorder(root, k);

        return answer;
        
    }
};
