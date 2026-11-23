class Solution {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        vector<pair<int, vector<int>>> arr;

                // Calculate squared distance
                        for (auto &point : points) {

                                    int x = point[0];
                                                int y = point[1];

                                                            int dist = x * x + y * y;

                                                                        arr.push_back({dist, point});
                                                                                }

                                                                                        // Sort based on distance
                                                                                                sort(arr.begin(), arr.end());

                                                                                                        vector<vector<int>> ans;

                                                                                                                // Select first k closest points
                                                                                                                        for (int i = 0; i < k; i++) {
                                                                                                                                    ans.push_back(arr[i].second);
                                                                                                                                            }

                                                                                                                                                    return ans;
    }
};
