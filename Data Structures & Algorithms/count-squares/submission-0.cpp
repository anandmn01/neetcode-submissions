class CountSquares {
private:
    unordered_map<int, unordered_map<int, int>> points;

public:
    CountSquares() {

    }

    void add(vector<int> point) {
        int x = point[0];
        int y = point[1];

        points[x][y]++;
    }

    int count(vector<int> point) {
        int qx = point[0];
        int qy = point[1];

        int ans = 0;

        // Try every unique stored coordinate as the opposite corner.
        for (const auto& [x, ys] : points) {
            for (const auto& [y, freq] : ys) {

                // Opposite corners cannot share X or Y coordinates.
                if (x == qx || y == qy) {
                    continue;
                }

                // Horizontal and vertical distances must be equal.
                if (abs(x - qx) != abs(y - qy)) {
                    continue;
                }

                // Find the remaining two corners.
                auto it1 = points.find(x);
                auto it2 = points.find(qx);

                if (it1 == points.end() || it2 == points.end()) {
                    continue;
                }

                auto p1 = it1->second.find(qy);
                auto p2 = it2->second.find(y);

                if (p1 != it1->second.end() &&
                    p2 != it2->second.end()) {

                    ans += freq * p1->second * p2->second;
                }
            }
        }

        return ans;
        
    }
};
