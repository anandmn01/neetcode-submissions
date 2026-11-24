class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int totalGas = 0;
        int currentGas = 0;

        int start = 0;

        for (int i = 0; i < gas.size(); i++) {

            int netGas = gas[i] - cost[i];

            totalGas += netGas;

            currentGas += netGas;

            // Current starting point is invalid
            if (currentGas < 0) {

                start = i + 1;

                currentGas = 0;
            }
        }

        // Total gas is insufficient
        if (totalGas < 0) {
            return -1;
        }

        return start;
    }
};
