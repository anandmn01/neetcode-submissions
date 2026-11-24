class Solution {
public:
    bool mergeTriplets(vector<vector<int>>& triplets, vector<int>& target) {
       bool first = false;
        bool second = false;
        bool third = false;

        for (auto& t : triplets) {

            // Ignore triplets that exceed the target
            if (t[0] > target[0] ||
                t[1] > target[1] ||
                t[2] > target[2]) {

                continue;
            }

            // Check which target values are available
            if (t[0] == target[0]) {
                first = true;
            }

            if (t[1] == target[1]) {
                second = true;
            }

            if (t[2] == target[2]) {
                third = true;
            }
        }

        return first && second && third; 
    }
};
