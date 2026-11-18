class TimeMap {
public:
    unordered_map<string, vector<pair<int, string>>> mp;

    TimeMap() {
        
    }

    void set(string key, string value, int timestamp) {

        mp[key].push_back({timestamp, value});
    }

    string get(string key, int timestamp) {

        if (mp.find(key) == mp.end()) {
            return "";
        }

        auto &values = mp[key];

        int left = 0;
        int right = values.size() - 1;

        string ans = "";

        while (left <= right) {

            int mid = left + (right - left) / 2;

            if (values[mid].first <= timestamp) {

                ans = values[mid].second;

                // Search for a later valid timestamp
                left = mid + 1;
            }

            else {

                // Timestamp is too large
                right = mid - 1;
            }
        }

        return ans;
        
    }
};
