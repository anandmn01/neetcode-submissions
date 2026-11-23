class Twitter {
private:

    int timestamp = 0;

        // userId -> {timestamp, tweetId}
            unordered_map<int, vector<pair<int, int>>> tweets;

                // followerId -> set of followees
                    unordered_map<int, unordered_set<int>> following;

                    public:

                        Twitter() {
                            }

                                // Publish a tweet
                                    void postTweet(int userId, int tweetId) {

                                            tweets[userId].push_back({timestamp++, tweetId});
                                                }

                                                    // Get 10 most recent tweets
                                                        vector<int> getNewsFeed(int userId) {

                                                                vector<int> ans;

                                                                        // Max Heap:
                                                                                // {timestamp, userId, index}
                                                                                        priority_queue<tuple<int, int, int>> pq;

                                                                                                // Include the user's own tweets
                                                                                                        unordered_set<int> users = following[userId];

                                                                                                                users.insert(userId);

                                                                                                                        // Insert the latest tweet of every relevant user
                                                                                                                                for (int user : users) {

                                                                                                                                            if (!tweets[user].empty()) {

                                                                                                                                                            int index = tweets[user].size() - 1;

                                                                                                                                                                            int time = tweets[user][index].first;

                                                                                                                                                                                            pq.push({time, user, index});
                                                                                                                                                                                                        }
                                                                                                                                                                                                                }

                                                                                                                                                                                                                        // Extract at most 10 most recent tweets
                                                                                                                                                                                                                                while (!pq.empty() && ans.size() < 10) {

                                                                                                                                                                                                                                            auto [time, user, index] = pq.top();

                                                                                                                                                                                                                                                        pq.pop();

                                                                                                                                                                                                                                                                    // Add tweet ID to answer
                                                                                                                                                                                                                                                                                ans.push_back(tweets[user][index].second);

                                                                                                                                                                                                                                                                                            // Move to the previous tweet of the same user
                                                                                                                                                                                                                                                                                                        if (index > 0) {

                                                                                                                                                                                                                                                                                                                        int prevIndex = index - 1;

                                                                                                                                                                                                                                                                                                                                        int prevTime = tweets[user][prevIndex].first;

                                                                                                                                                                                                                                                                                                                                                        pq.push({prevTime, user, prevIndex});
                                                                                                                                                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                                                                                                                                                    return ans;
                                                                                                                                                                                                                                                                                                                                                                                        }

                                                                                                                                                                                                                                                                                                                                                                                            // Follow another user
                                                                                                                                                                                                                                                                                                                                                                                                void follow(int followerId, int followeeId) {

                                                                                                                                                                                                                                                                                                                                                                                                        following[followerId].insert(followeeId);
                                                                                                                                                                                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                                                                                                                                                                                // Unfollow a user
                                                                                                                                                                                                                                                                                                                                                                                                                    void unfollow(int followerId, int followeeId) {

                                                                                                                                                                                                                                                                                                                                                                                                                            following[followerId].erase(followeeId);
    }
};
