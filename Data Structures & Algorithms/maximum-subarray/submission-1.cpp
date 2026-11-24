class Solution {
public:
    int maxSubArray(vector<int>& nums) {
int prefixSum = 0;
        int minPrefix = 0;

        int maxSum = INT_MIN;

        for (int num : nums) {

            prefixSum += num;

            maxSum = max(maxSum, prefixSum - minPrefix);

            minPrefix = min(minPrefix, prefixSum);
        }

        return maxSum;
    }
};
