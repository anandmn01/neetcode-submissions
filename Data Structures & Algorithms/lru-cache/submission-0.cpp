class LRUCache {
public:
    struct Node {
        int key;
        int value;

        Node* prev;
        Node* next;

        Node(int k, int v) {
            key = k;
            value = v;
            prev = nullptr;
            next = nullptr;
        }
    };

    int capacity;

    unordered_map<int, Node*> mp;

    Node* head;
    Node* tail;

    // Remove a node from the linked list
    void removeNode(Node* node) {

        Node* prevNode = node->prev;
        Node* nextNode = node->next;

        prevNode->next = nextNode;
        nextNode->prev = prevNode;
    }

    // Insert a node immediately after head
    void insertAtFront(Node* node) {

        Node* nextNode = head->next;

        head->next = node;
        node->prev = head;

        node->next = nextNode;
        nextNode->prev = node;
    }

    // Move an existing node to the front
    void moveToFront(Node* node) {

        removeNode(node);
        insertAtFront(node);
    }

public:

    LRUCache(int capacity) {

        this->capacity = capacity;

        head = new Node(0, 0);
        tail = new Node(0, 0);

        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {

        if (mp.find(key) == mp.end()) {
            return -1;
        }

        Node* node = mp[key];

        // Accessed node becomes recently used
        moveToFront(node);

        return node->value;
    }

    void put(int key, int value) {

        // If key already exists, update its value
        if (mp.find(key) != mp.end()) {

            Node* node = mp[key];

            node->value = value;

            moveToFront(node);

            return;
        }

        // Create a new node
        Node* newNode = new Node(key, value);

        mp[key] = newNode;

        insertAtFront(newNode);

        // If capacity is exceeded, remove LRU node
        if (mp.size() > capacity) {

            Node* lruNode = tail->prev;

            removeNode(lruNode);

            mp.erase(lruNode->key);

            delete lruNode;
        }
    }

    ~LRUCache() {

        Node* curr = head;

        while (curr != nullptr) {

            Node* nextNode = curr->next;

            delete curr;

            curr = nextNode;
        }
        
    }
};
