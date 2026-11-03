class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
 int n = nums.size();
        vector<int> output(n, 1);

        // Store the product of all elements to the left of i
        int prefix = 1;
        for (int i = 0; i < n; i++) {
            output[i] = prefix;
            prefix *= nums[i];
        }

        // Multiply by the product of all elements to the right of i
        int suffix = 1;
        for (int i = n - 1; i >= 0; i--) {
            output[i] *= suffix;
            suffix *= nums[i];
        }

        return output;
    }
};
