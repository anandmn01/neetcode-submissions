class Solution {
public:
    int maxProduct(vector<int>& nums) {
      int maxProduct = nums[0];
        int minProduct = nums[0];

        int ans = nums[0];

        for (int i = 1; i < nums.size(); i++) {

            int num = nums[i];

            int tempMax = max({
                num,
                num * maxProduct,
                num * minProduct
            });

            int tempMin = min({
                num,
                num * maxProduct,
                num * minProduct
            });

            maxProduct = tempMax;
            minProduct = tempMin;

            ans = max(ans, maxProduct);
        }

        return ans;  
    }
};
