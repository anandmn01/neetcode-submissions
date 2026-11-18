class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int left = 1;
        int right = *max_element(piles.begin(), piles.end());

        while (left <= right) {

            int mid = left + (right - left) / 2;

            long long hours = 0;

            // Calculate total hours at speed mid
            for (int pile : piles) {
                hours += (pile + mid - 1) / mid;
            }

            if (hours <= h) {
                // Speed is sufficient; try a smaller speed
                right = mid - 1;
            }
            else {
                // Speed is too slow; increase it
                left = mid + 1;
            }
        }

        return left;
    }
};
