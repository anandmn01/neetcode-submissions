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
    ListNode* reverseKGroup(ListNode* head, int k) {
      if (head == nullptr || k == 1) {
            return head;
        }

        ListNode dummy(0);
        dummy.next = head;

        ListNode* groupPrev = &dummy;

        while (true) {

            // Step 1: Find the kth node
            ListNode* kth = groupPrev;

            for (int i = 0; i < k && kth != nullptr; i++) {
                kth = kth->next;
            }

            // Not enough nodes remaining
            if (kth == nullptr) {
                break;
            }

            // Step 2: Save the next group's starting node
            ListNode* groupNext = kth->next;

            // Step 3: Reverse the current group
            ListNode* prev = groupNext;
            ListNode* curr = groupPrev->next;

            while (curr != groupNext) {

                ListNode* nextNode = curr->next;

                curr->next = prev;

                prev = curr;
                curr = nextNode;
            }

            // Step 4: Reconnect the reversed group
            ListNode* oldGroupStart = groupPrev->next;

            groupPrev->next = kth;

            groupPrev = oldGroupStart;
        }

        return dummy.next;  
    }
};
