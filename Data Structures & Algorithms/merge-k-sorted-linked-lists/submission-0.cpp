/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // Min heap to store node pointers
        auto cmp = [](ListNode* a, ListNode* b) {
            return a->val > b->val;
        };

        priority_queue<
            ListNode*,
            vector<ListNode*>,
            decltype(cmp)
        > pq(cmp);

        // Step 1: Insert first node of every non-empty list
        for (ListNode* node : lists) {

            if (node != nullptr) {
                pq.push(node);
            }
        }

        // Dummy node for the result list
        ListNode dummy(0);
        ListNode* tail = &dummy;

        // Step 2: Process the heap
        while (!pq.empty()) {

            // Get the smallest node
            ListNode* smallest = pq.top();
            pq.pop();

            // Attach it to the result
            tail->next = smallest;
            tail = tail->next;

            // Insert its next node into the heap
            if (smallest->next != nullptr) {
                pq.push(smallest->next);
            }
        }

        tail->next = nullptr;

        return dummy.next;
    }
};
