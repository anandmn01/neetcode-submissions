class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int n = position.size();

        vector<pair<int, double>> cars;

        // Step 1: Store position and arrival time
        for (int i = 0; i < n; i++) {

            double time = (double)(target - position[i]) / speed[i];

            cars.push_back({position[i], time});
        }

        // Step 2: Sort cars by position in descending order
        sort(cars.rbegin(), cars.rend());

        // Step 3: Count fleets
        int fleets = 0;
        double lastTime = 0.0;

        for (auto& car : cars) {

            double currentTime = car.second;

            // A new fleet is formed
            if (currentTime > lastTime) {

                fleets++;
                lastTime = currentTime;
            }
        }

        return fleets;
    }
};
