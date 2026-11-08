from pathlib import Path

content = r'''# Day 8 — Java Collections Framework

> **Purpose:** Understand how Java stores, retrieves, orders, and processes groups of objects. These concepts are frequently tested in Java backend interviews and are used throughout Spring Boot applications.
>
> **Learning context:** These notes follow the eight topics covered in Day 8. Examples use standard Java APIs and modern Java syntax.

---

## Table of Contents

1. [Collections Framework Overview](#1-collections-framework-overview)
2. [ArrayList Internals and Complexity](#2-arraylist-internals-and-complexity)
3. [LinkedList and ArrayList vs LinkedList](#3-linkedlist-and-arraylist-vs-linkedlist)
4. [Set, HashSet, LinkedHashSet, and TreeSet](#4-set-hashset-linkedhashset-and-treeset)
5. [HashMap Internals](#5-hashmap-internals)
6. [Hashtable vs HashMap vs ConcurrentHashMap](#6-hashtable-vs-hashmap-vs-concurrenthashmap)
7. [Queue, Deque, PriorityQueue, and Stack](#7-queue-deque-priorityqueue-and-stack)
8. [Comparable, Comparator, Sorting, and Iterators](#8-comparable-comparator-sorting-and-iterators)
9. [Day 8 — Quick Revision Sheet](#9-day-8--quick-revision-sheet)
10. [Interview Questions and Answers](#10-interview-questions-and-answers)
11. [Backend Usage Examples](#11-backend-usage-examples)
12. [Completion Checklist](#12-completion-checklist)

---

# 1. Collections Framework Overview

## 1.1 Why do we need collections?

A variable stores one value:

```java
String employeeName = "Anand";
```

But a backend application usually needs to work with multiple values:

- A list of employees.
- A set of unique employee codes.
- A mapping between employee IDs and employee details.
- A queue of jobs waiting to be processed.

Arrays can store multiple values, but their length is fixed after creation. The Java Collections Framework provides reusable data structures that can grow, shrink, search, sort, and organize objects.

Example:

```java
List<String> employees = new ArrayList<>();

employees.add("Anand");
employees.add("Rahul");
employees.add("Priya");

System.out.println(employees);
```

## 1.2 What is the Java Collections Framework?

The Java Collections Framework is a set of interfaces, implementations, and algorithms for storing and manipulating groups of objects.

It contains:

1. **Interfaces:** Define behavior, such as `List`, `Set`, `Queue`, and `Map`.
2. **Implementations:** Provide concrete data structures, such as `ArrayList`, `HashSet`, and `HashMap`.
3. **Algorithms and utilities:** Methods for sorting, searching, reversing, shuffling, and other operations, mainly through the `Collections` utility class.

## 1.3 Collection vs Collections

These names look similar but have different meanings.

| `Collection` | `Collections` |
|---|---|
| An interface | A utility class |
| Root interface for many collection types | Provides static helper methods |
| Extended by `List`, `Set`, and `Queue` | Provides sorting, reversing, searching, synchronization wrappers, etc. |
| Represents a group of elements | Operates on collections |

Example:

```java
Collection<String> names = new ArrayList<>();
names.add("Anand");
names.add("Priya");

Collections.sort((List<String>) names);
```

A clearer example using a `List`:

```java
List<String> names = new ArrayList<>();
names.add("Anand");
names.add("Priya");

Collections.sort(names);
Collections.reverse(names);
```

## 1.4 Collections hierarchy

A simplified view:

```text
                 Iterable
                    |
                Collection
                    |
       +------------+-------------+
       |            |             |
      List         Set           Queue
       |            |             |
  +----+----+   +---+------+      +----------------+
  |         |   |          |      |                |
ArrayList LinkedList HashSet LinkedHashSet   PriorityQueue
                         |
                      TreeSet
```

`Deque` is a subinterface of `Queue`. `ArrayDeque` and `LinkedList` are common implementations of `Deque`.

**Important:** `Map` belongs to the Collections Framework, but it does **not** extend `Collection`.

```text
Map
 |
 +-- HashMap
 +-- LinkedHashMap
 +-- TreeMap
 +-- Hashtable
 +-- ConcurrentHashMap
```

## 1.5 List

A `List`:

- Maintains a defined order.
- Allows duplicate elements.
- Supports index-based access.
- Can contain multiple `null` values in common implementations such as `ArrayList` and `LinkedList`.

Example:

```java
List<String> employees = new ArrayList<>();

employees.add("Anand");
employees.add("Rahul");
employees.add("Anand");

System.out.println(employees.get(0)); // Anand
System.out.println(employees.size()); // 3
```

## 1.6 Set

A `Set` does not allow duplicate elements according to its equality or ordering rules.

Common implementations:

- `HashSet`: No guaranteed iteration order.
- `LinkedHashSet`: Maintains insertion order.
- `TreeSet`: Maintains sorted order.

## 1.7 Queue

A `Queue` is commonly used to hold elements before they are processed.

A typical queue follows FIFO: First In, First Out.

```java
Queue<String> tasks = new LinkedList<>();

tasks.offer("Send email");
tasks.offer("Generate report");

System.out.println(tasks.poll()); // Send email
```

Not every Queue implementation uses FIFO ordering. For example, `PriorityQueue` orders elements by priority.

## 1.8 Map

A `Map` stores key-value pairs.

- Each key is unique according to the map's equality rules.
- Values can repeat.
- A key maps to at most one current value.
- `HashMap` allows one `null` key and multiple `null` values.

Example:

```java
Map<Long, String> employees = new HashMap<>();

employees.put(101L, "Anand");
employees.put(102L, "Priya");

System.out.println(employees.get(101L)); // Anand
```

## 1.9 Choosing an implementation

| Requirement | Common choice |
|---|---|
| General-purpose indexed list | `ArrayList` |
| Frequent insertion/removal at both ends | `ArrayDeque` |
| Unique values, no order needed | `HashSet` |
| Unique values, insertion order needed | `LinkedHashSet` |
| Unique values, sorted order needed | `TreeSet` |
| Key-value lookup | `HashMap` |
| Key-value lookup with insertion order | `LinkedHashMap` |
| Sorted keys | `TreeMap` |
| Thread-safe concurrent key-value map | `ConcurrentHashMap` |
| Priority-based processing | `PriorityQueue` |

**Practical default:** Start with `ArrayList` for a general-purpose list and `HashMap` for key-value lookup. Choose a different implementation when ordering, concurrency, or operation patterns require it.

---

# 2. ArrayList Internals and Complexity

## 2.1 What is ArrayList?

`ArrayList` is a resizable-array implementation of the `List` interface.

Unlike a normal array, an ArrayList can grow as elements are added.

```java
List<String> employees = new ArrayList<>();

employees.add("Anand");
employees.add("Priya");
employees.add("Rahul");
```

It maintains:
- An internal array.
- A logical size: the number of elements stored.
- An internal capacity: the number of array slots currently available.

## 2.2 Size vs capacity

Suppose an ArrayList has capacity 10 but contains only 3 elements.

```text
Internal array:
+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
| Anand | Priya | Rahul | empty | empty | empty | empty | empty | empty | empty |
+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+

size     = 3
capacity = 10
```

- `size()` returns the number of elements.
- Capacity is an internal implementation detail; there is no public `capacity()` method on ArrayList.

## 2.3 How does ArrayList grow?

When the internal array has no free slot and another element is added:

1. A larger array is allocated.
2. Existing elements are copied into the new array.
3. The internal reference is updated.
4. The new element is inserted.

In modern OpenJDK implementations, growth is commonly approximately 1.5 times the old capacity. This is an implementation detail, not a Java API guarantee.

Conceptual example:

```text
Before:
[ A | B | C ]       capacity 3

Need to add D:
Create larger array
[ A | B | C | _ | _ ]   capacity 5

After:
[ A | B | C | D | _ ]   size 4
```

## 2.4 Time complexity

| Operation | Typical complexity |
|---|---:|
| `get(index)` | O(1) |
| `set(index, value)` | O(1) |
| `add(value)` at end | Amortized O(1) |
| Insert at beginning | O(n) |
| Insert in middle | O(n) |
| Remove from beginning | O(n) |
| Remove from middle | O(n) |
| Search by value | O(n) |

### Why is append amortized O(1)?

Most additions use an available array slot and take constant time. Occasionally, the array must grow and copy existing elements, which takes O(n). Because growth is occasional and spread across many additions, the average cost per append over a sequence of operations is amortized O(1).

## 2.5 Example: ArrayList operations

```java
List<String> employees = new ArrayList<>();

employees.add("Anand");   // Append
employees.add("Priya");
employees.add("Rahul");

String first = employees.get(0); // O(1)

employees.add(1, "Aman"); // Insert at index 1; shifts elements

employees.remove(2);      // Remove at index 2; shifts elements
```

## 2.6 When should you use ArrayList?

Use it when:
- You need indexed access.
- You frequently iterate over elements.
- Most insertions happen at the end.
- You need a general-purpose list.

It is often the best default List implementation because it has good memory locality and low per-element overhead compared with a linked structure.

## 2.7 Common mistakes

- Assuming ArrayList has a fixed size like an array.
- Assuming insertion at the beginning is O(1).
- Confusing logical size with internal capacity.
- Assuming `add()` is worst-case O(1); a resize can take O(n).
- Modifying an ArrayList concurrently without coordination.

---

# 3. LinkedList and ArrayList vs LinkedList

## 3.1 What is LinkedList?

Java's `LinkedList` is a doubly linked list. Each node stores:
- The element.
- A reference to the previous node.
- A reference to the next node.

Conceptually:

```text
null
  ^
  |
+--------+      +--------+      +--------+
| A      | <--> | B      | <--> | C      |
+--------+      +--------+      +--------+
                                      |
                                      v
                                     null
```

Unlike ArrayList, LinkedList does not need one contiguous internal array.

## 3.2 Basic example

```java
LinkedList<String> employees = new LinkedList<>();

employees.add("Anand");
employees.add("Priya");
employees.addFirst("Rahul");
employees.addLast("Aman");

System.out.println(employees);
```

Because LinkedList implements both `List` and `Deque`, it supports indexed-list operations and deque operations.

## 3.3 Time complexity

| Operation | Typical complexity |
|---|---:|
| Add at beginning | O(1) |
| Add at end | O(1) |
| Remove first | O(1) |
| Remove last | O(1) |
| Get by index | O(n) |
| Search by value | O(n) |
| Insert/remove at a known node | O(1) |

The phrase **known node** matters. If you first need to find an element by index or value, traversal can take O(n). The pointer change itself is O(1).

## 3.4 ArrayList vs LinkedList

| Feature | ArrayList | LinkedList |
|---|---|---|
| Internal structure | Resizable array | Doubly linked nodes |
| Indexed lookup | O(1) | O(n) |
| Append | Amortized O(1) | O(1) |
| Add/remove at front | O(n) | O(1) |
| Memory overhead | Usually lower | Higher due to node links |
| Cache locality | Generally better | Generally worse |
| Implements | List | List and Deque |

## 3.5 Which one is better?

For most ordinary application lists, ArrayList is the better default.

LinkedList can be useful when:
- You need frequent additions/removals at both ends.
- You are using it as a Deque.
- You already hold a list iterator at the position where insertion/removal is needed.

Do not assume LinkedList is always faster for insertions. If the required position must first be located, traversal can dominate the operation.

### Interview-ready answer

**Q: Why is ArrayList often preferred over LinkedList?**

ArrayList provides constant-time indexed access, lower per-element memory overhead, and generally better cache locality. LinkedList has constant-time operations at its ends, but indexed access and searching require traversal. In many real applications, ArrayList performs better as a general-purpose list.

---

# 4. Set, HashSet, LinkedHashSet, and TreeSet

## 4.1 What is Set?

A Set is a collection that does not allow duplicate elements.

```java
Set<String> employeeCodes = new HashSet<>();

employeeCodes.add("EMP-101");
employeeCodes.add("EMP-102");
employeeCodes.add("EMP-101");

System.out.println(employeeCodes.size()); // 2
```

The second insertion of `"EMP-101"` does not create another element.

## 4.2 HashSet

`HashSet` is backed internally by a HashMap.

Conceptually:
- The set element is stored as a key in the backing map.
- A shared dummy object is used as the map value.
- Hashing and equality determine uniqueness.

Characteristics:
- Does not guarantee iteration order.
- Allows one `null` element.
- Average O(1) add, remove, and contains under suitable hash distribution.
- Not thread-safe by default.

Example:

```java
Set<Long> employeeIds = new HashSet<>();

employeeIds.add(101L);
employeeIds.add(102L);
employeeIds.add(101L);

System.out.println(employeeIds.contains(102L)); // true
```

## 4.3 Why are equals() and hashCode() important in HashSet?

HashSet uses hashing to locate a bucket and equality to identify an existing element.

If a custom class is stored in a HashSet, implement `equals()` and `hashCode()` consistently.

```java
class EmployeeCode {
    private final String value;

    EmployeeCode(String value) {
        this.value = value;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof EmployeeCode other)) return false;
        return Objects.equals(value, other.value);
    }

    @Override
    public int hashCode() {
        return Objects.hash(value);
    }
}
```

Now two EmployeeCode objects containing the same value are treated as duplicates.

## 4.4 LinkedHashSet

`LinkedHashSet` maintains insertion order.

```java
Set<String> names = new LinkedHashSet<>();

names.add("Rahul");
names.add("Anand");
names.add("Priya");

System.out.println(names);
// [Rahul, Anand, Priya]
```

It combines hash-based lookup with linked ordering information.

Use it when:
- Values must be unique.
- You also want predictable insertion-order iteration.

## 4.5 TreeSet

`TreeSet` stores unique elements in sorted order. It is backed by a NavigableMap implementation, typically a Red-Black Tree.

```java
Set<Integer> numbers = new TreeSet<>();

numbers.add(50);
numbers.add(10);
numbers.add(30);

System.out.println(numbers); // [10, 30, 50]
```

Typical operations such as add, remove, and contains take O(log n).

Ordering comes from:
- Natural ordering through `Comparable`, or
- A supplied `Comparator`.

The comparator must be consistent with the uniqueness behavior you expect. TreeSet treats elements that compare as zero as the same sorted-set element, even if `equals()` would return false.

## 4.6 HashSet vs LinkedHashSet vs TreeSet

| Feature | HashSet | LinkedHashSet | TreeSet |
|---|---|---|---|
| Duplicates | Not allowed | Not allowed | Not allowed by comparator/natural ordering |
| Iteration order | No guarantee | Insertion order | Sorted order |
| Typical add | Average O(1) | Average O(1) | O(log n) |
| Null | One null allowed | One null allowed | Generally does not allow null with natural ordering |
| Internal structure | HashMap | HashMap plus linked order | TreeMap / Red-Black Tree |

### Interview-ready answer

**Q: What is the difference between HashSet, LinkedHashSet, and TreeSet?**

HashSet stores unique values without guaranteeing iteration order. LinkedHashSet preserves insertion order. TreeSet stores unique values in sorted order and generally provides O(log n) operations.

---

# 5. HashMap Internals

## 5.1 What is HashMap?

HashMap stores data as key-value pairs and provides efficient lookup using hashing.

```java
Map<Long, String> employees = new HashMap<>();

employees.put(101L, "Anand");
employees.put(102L, "Priya");

System.out.println(employees.get(101L)); // Anand
```

Important characteristics:
- Keys are unique.
- Values can repeat.
- One `null` key is allowed.
- Multiple `null` values are allowed.
- It does not guarantee iteration order.
- It is not thread-safe for concurrent structural modification.

## 5.2 Internal structure

A HashMap uses an array of buckets. A key's hash helps determine which bucket should hold its entry.

Conceptual structure:

```text
HashMap
  |
  v
Bucket array
  |
  +---- Bucket 0 --> Entry
  |
  +---- Bucket 1 --> Entry --> Entry
  |
  +---- Bucket 2
  |
  +---- Bucket 3 --> Entry
  |
  +---- ...
```

Each entry stores a key, value, hash information, and a link or tree relationship used to handle collisions.

## 5.3 How put() works

Consider:

```java
map.put("EMP-101", "Anand");
```

Conceptual steps:

1. HashMap obtains the key's hash code.
2. It applies an internal hash-spreading operation.
3. It calculates a bucket index from the hash and current table capacity.
4. If the bucket is empty, it inserts the entry.
5. If the bucket already contains entries, it checks for an existing matching key.
6. If an equal key is found, its value is replaced.
7. Otherwise, a new entry is added to the bucket.
8. HashMap checks whether resizing is required.

The exact implementation is more detailed, but this is the interview-level flow.

## 5.4 How get() works

```java
String employee = map.get("EMP-101");
```

Conceptually:

1. Calculate the key's hash.
2. Find the bucket.
3. Inspect candidate entries in that bucket.
4. Compare hash information and key equality.
5. Return the matching value, or `null` if no mapping exists.

This explains why a correct `equals()` and `hashCode()` implementation is essential for custom keys.

## 5.5 What is a collision?

A collision occurs when multiple keys map to the same bucket.

Different keys can have:
- The same hash code, or
- Different hash codes that map to the same bucket index.

Conceptual example:

```text
Bucket 4
   |
   v
[Key A, Value 1] --> [Key B, Value 2] --> [Key C, Value 3]
```

HashMap must distinguish these keys using equality checks.

## 5.6 Collision handling and treeification

In modern OpenJDK implementations, a bucket can use:
- A linked structure for entries.
- A balanced tree structure when collision chains become sufficiently large and table capacity is adequate.

Common OpenJDK implementation thresholds:
- Treeification threshold: 8 entries.
- Minimum table capacity for treeification: 64.
- Untreeification threshold: 6 entries.

These are implementation details, not guarantees of the Map API. The implementation may resize instead of treeifying when the table is still too small.

Tree bins can improve worst-case lookup behavior for heavily collided buckets, though exact behavior depends on key properties and implementation details.

## 5.7 Initial capacity and load factor

Common HashMap defaults:
- Default initial capacity: 16.
- Default load factor: 0.75.
- Initial resize threshold at capacity 16: 12.

The default table is generally allocated lazily when the first entry is inserted.

Threshold formula:

```text
threshold = capacity × load factor

16 × 0.75 = 12
```

When the number of entries crosses the threshold, HashMap resizes the table.

Example:

```java
Map<String, Integer> map = new HashMap<>(32);
```

This supplies an initial capacity request. It does not mean the map can hold only 32 entries.

## 5.8 Resizing

When resizing occurs:
1. A larger bucket array is allocated.
2. Existing entries are redistributed into the new table.
3. The map updates its internal table and threshold.

Resizing can be expensive at that moment because entries need to be redistributed. Over a sequence of operations, hash-map insertion is generally considered amortized O(1) under good hash distribution.

## 5.9 Time complexity

| Operation | Average expected | Worst-case discussion |
|---|---:|---|
| `put()` | O(1) | Can degrade with collisions/resizing |
| `get()` | O(1) | Collision-heavy buckets can be slower |
| `remove()` | O(1) | Depends on bucket structure |
| `containsKey()` | O(1) | Depends on hashing and collisions |

Modern tree bins improve collision-heavy behavior, but do not treat every HashMap operation as an unconditional O(1) guarantee.

## 5.10 Why is HashMap not thread-safe?

If multiple threads modify the same ordinary HashMap concurrently without coordination, updates and visibility can be unsafe. Use an appropriate concurrency strategy, such as:
- `ConcurrentHashMap` for concurrent access.
- External synchronization when the entire operation needs one lock.
- Immutable or safely published maps for read-only sharing.

Do not assume that wrapping individual calls in unrelated locks makes a multi-step check-then-act operation atomic.

## 5.11 Common HashMap interview points

- HashMap uses `hashCode()` and `equals()`.
- One null key and multiple null values are permitted.
- Updating an existing key replaces its value.
- HashMap does not preserve insertion order.
- `LinkedHashMap` preserves insertion order (or can be configured for access order).
- `TreeMap` sorts keys.
- HashMap is not thread-safe.
- Mutable keys can cause lookup problems if fields used by equality/hash code change.

### Interview-ready answer

**Q: How does HashMap work internally?**

HashMap stores entries in an array of buckets. It uses the key's hash code, with internal hash spreading, to calculate a bucket index. If multiple entries land in the same bucket, it handles the collision using a linked structure and, in modern OpenJDK implementations under certain conditions, a tree structure. When retrieving an entry, it uses hash information and `equals()` to identify the key. Its common operations are expected O(1) with a suitable hash distribution.

---

# 6. Hashtable vs HashMap vs ConcurrentHashMap

## 6.1 HashMap

- Not synchronized by default.
- Allows one null key and multiple null values.
- Usually preferred for local, non-concurrent map usage.
- Concurrent modification by multiple threads requires an appropriate strategy.

## 6.2 Hashtable

`Hashtable` is a legacy synchronized map implementation.

Characteristics:
- Its methods are synchronized.
- Does not allow null keys or null values.
- Generally has more synchronization overhead than modern concurrent alternatives.
- Usually not the preferred choice for new concurrent code.

Example:

```java
Map<String, Integer> table = new Hashtable<>();
table.put("A", 1);
```

## 6.3 ConcurrentHashMap

`ConcurrentHashMap` is designed for concurrent access.

Characteristics:
- Supports concurrent retrievals and updates.
- Does not allow null keys or null values.
- Uses modern fine-grained concurrency techniques, including CAS and synchronized operations on relevant bins.
- Does not use the old fixed Segment-based architecture in current Java implementations.
- Iterators are weakly consistent rather than fail-fast in the ordinary collection sense.

Example:

```java
Map<String, Integer> counts = new ConcurrentHashMap<>();

counts.put("LOGIN", 1);
counts.merge("LOGIN", 1, Integer::sum);

System.out.println(counts.get("LOGIN")); // 2
```

`merge()` is useful for atomic per-key updates.

## 6.4 Comparison

| Feature | HashMap | Hashtable | ConcurrentHashMap |
|---|---|---|---|
| Thread-safe for concurrent access | No | Synchronized methods | Designed for concurrency |
| Null key | One allowed | Not allowed | Not allowed |
| Null values | Allowed | Not allowed | Not allowed |
| Legacy status | Modern general-purpose map | Legacy | Modern concurrent map |
| Concurrency strategy | No built-in coordination | Broad method synchronization | Fine-grained synchronization/CAS |
| Iterator behavior | Fail-fast, best effort | Fail-fast, best effort | Weakly consistent |

## 6.5 When should you use each?

- Use **HashMap** when the map is local to one thread or access is externally coordinated.
- Use **ConcurrentHashMap** when multiple threads need to access and update a shared map concurrently.
- Avoid choosing **Hashtable** for new application code unless a legacy API requires it.

### Backend example

Suppose a backend tracks in-memory counters by event type:

```java
private final ConcurrentHashMap<String, Long> eventCounts =
        new ConcurrentHashMap<>();

public void recordEvent(String eventType) {
    eventCounts.merge(eventType, 1L, Long::sum);
}
```

This is safer than doing a separate `get()` followed by `put()` without coordination.

**Important:** In-memory concurrent maps are not replacements for persistent databases or distributed caches. In a multi-instance backend, each application instance has its own local map.

---

# 7. Queue, Deque, PriorityQueue, and Stack

## 7.1 Queue

A Queue holds elements for processing. A common behavior is FIFO.

```java
Queue<String> tasks = new LinkedList<>();

tasks.offer("Task 1");
tasks.offer("Task 2");
tasks.offer("Task 3");

System.out.println(tasks.poll()); // Task 1
System.out.println(tasks.peek()); // Task 2
```

Important Queue methods:

| Method | Meaning | Empty queue behavior |
|---|---|---|
| `add(e)` | Insert | Throws exception if insertion fails |
| `offer(e)` | Insert | Returns false if insertion fails |
| `remove()` | Remove head | Throws exception |
| `poll()` | Remove head | Returns null |
| `element()` | Examine head | Throws exception |
| `peek()` | Examine head | Returns null |

For many queue use cases, `offer()`, `poll()`, and `peek()` make empty/full handling clearer.

## 7.2 Deque

Deque means **Double Ended Queue**.

It supports insertion and removal at both ends.

```java
Deque<String> deque = new ArrayDeque<>();

deque.addFirst("A");
deque.addLast("B");
deque.offerFirst("START");
deque.offerLast("END");

System.out.println(deque.removeFirst());
System.out.println(deque.removeLast());
```

A Deque can be used as:
- A queue (FIFO).
- A stack (LIFO).

`ArrayDeque` is generally preferred over the legacy Stack class for stack operations. ArrayDeque does not allow null elements.

## 7.3 PriorityQueue

A PriorityQueue processes elements according to priority rather than insertion order.

By default, Java's PriorityQueue places the least element at the head according to natural ordering.

```java
PriorityQueue<Integer> numbers = new PriorityQueue<>();

numbers.offer(50);
numbers.offer(10);
numbers.offer(30);

System.out.println(numbers.poll()); // 10
System.out.println(numbers.poll()); // 30
System.out.println(numbers.poll()); // 50
```

Internally, PriorityQueue is based on a heap.

Typical complexities:
- Insert: O(log n)
- Remove head: O(log n)
- Peek head: O(1)
- Search for an arbitrary element: O(n)

The iterator does not promise sorted iteration order. To process in priority order, repeatedly poll or create a sorted copy.

Custom priority:

```java
PriorityQueue<Integer> largestFirst =
        new PriorityQueue<>(Comparator.reverseOrder());

largestFirst.offer(10);
largestFirst.offer(50);
largestFirst.offer(30);

System.out.println(largestFirst.poll()); // 50
```

## 7.4 Stack

Stack follows LIFO: Last In, First Out.

```java
Deque<String> stack = new ArrayDeque<>();

stack.push("A");
stack.push("B");
stack.push("C");

System.out.println(stack.pop());  // C
System.out.println(stack.peek()); // B
```

The old `Stack` class exists, but `Deque` is generally preferred for new code.

## 7.5 Queue vs Deque vs PriorityQueue vs Stack

| Structure | Ordering | Common implementation |
|---|---|---|
| Queue | Usually FIFO | `ArrayDeque`, `LinkedList` |
| Deque | Both ends | `ArrayDeque`, `LinkedList` |
| PriorityQueue | Priority/natural order | `PriorityQueue` |
| Stack | LIFO | `ArrayDeque` used as stack |

## 7.6 Backend examples

- A background worker may consume queued jobs.
- A priority queue can select the next job by urgency or scheduled priority.
- A deque can support work-stealing or operations at both ends.
- A stack can support nested processing or undo-like behavior.

For production distributed job processing, an in-memory Java Queue is not enough if jobs must survive application restarts or be shared across multiple application instances. A durable broker such as Kafka or a queueing system may be more appropriate.

---

# 8. Comparable, Comparator, Sorting, and Iterators

## 8.1 Why do we need sorting?

Backend applications frequently sort:
- Employees by name.
- Trips by date.
- Vehicles by registration number.
- Tasks by priority.
- API response data by creation time.

Java provides two main ways to define ordering: `Comparable` and `Comparator`.

## 8.2 Comparable

`Comparable<T>` defines a class's natural ordering using `compareTo()`.

```java
class Employee implements Comparable<Employee> {
    private final int employeeId;
    private final String name;

    Employee(int employeeId, String name) {
        this.employeeId = employeeId;
        this.name = name;
    }

    @Override
    public int compareTo(Employee other) {
        return Integer.compare(this.employeeId, other.employeeId);
    }

    public int getEmployeeId() {
        return employeeId;
    }

    public String getName() {
        return name;
    }
}
```

Sort:

```java
List<Employee> employees = new ArrayList<>();

employees.add(new Employee(103, "Priya"));
employees.add(new Employee(101, "Anand"));
employees.add(new Employee(102, "Rahul"));

Collections.sort(employees);
```

The list is now ordered by employee ID.

## 8.3 Comparator

`Comparator<T>` defines an external or alternative ordering.

```java
Comparator<Employee> byName =
        Comparator.comparing(Employee::getName);
```

Sort:

```java
employees.sort(byName);
```

Descending:

```java
employees.sort(
        Comparator.comparing(Employee::getName).reversed()
);
```

## 8.4 Comparable vs Comparator

| Comparable | Comparator |
|---|---|
| Defines natural ordering in the class | Defines an external ordering |
| Method: `compareTo()` | Method: `compare()` |
| Usually one natural ordering | Can define many different orderings |
| Class implements Comparable | Comparator can be a separate class or lambda |

## 8.5 Multi-field sorting

```java
Comparator<Employee> employeeComparator =
        Comparator.comparing(Employee::getName)
                  .thenComparingInt(Employee::getEmployeeId);
```

This sorts by name first and employee ID when names are equal.

Null handling can be explicit:

```java
Comparator<Employee> byNameNullSafe =
        Comparator.comparing(
            Employee::getName,
            Comparator.nullsLast(String::compareTo)
        );
```

## 8.6 Sorting with a lambda

```java
employees.sort(
    (first, second) ->
        Integer.compare(first.getEmployeeId(), second.getEmployeeId())
);
```

Prefer `Integer.compare()` over subtraction:

```java
// Avoid: subtraction can overflow for extreme integer values.
(a, b) -> a - b
```

## 8.7 Iterator

An Iterator provides a standard way to traverse a collection.

Main methods:
- `hasNext()`: Checks whether another element exists.
- `next()`: Returns the next element.
- `remove()`: Removes the last returned element, if supported.

Example:

```java
List<String> employees =
        new ArrayList<>(List.of("Anand", "Rahul", "Priya"));

Iterator<String> iterator = employees.iterator();

while (iterator.hasNext()) {
    String name = iterator.next();
    System.out.println(name);
}
```

## 8.8 Removing elements safely while iterating

Avoid structurally modifying many collections directly inside an enhanced for-loop.

Risky:

```java
for (String name : employees) {
    if (name.startsWith("A")) {
        employees.remove(name);
    }
}
```

This can cause `ConcurrentModificationException`.

Use the iterator's own remove method:

```java
Iterator<String> iterator = employees.iterator();

while (iterator.hasNext()) {
    String name = iterator.next();

    if (name.startsWith("A")) {
        iterator.remove();
    }
}
```

Or use `removeIf()` when appropriate:

```java
employees.removeIf(name -> name.startsWith("A"));
```

## 8.9 Fail-fast iterators

Many standard collection iterators are fail-fast. If the collection is structurally modified outside the iterator while iteration is in progress, the iterator may throw `ConcurrentModificationException`.

Important:
- Fail-fast behavior is best effort.
- It is not a thread-safety guarantee.
- Do not rely on the exception for correctness.
- Use the collection's supported concurrent or iteration strategy.

## 8.10 CopyOnWriteArrayList

`CopyOnWriteArrayList` is a thread-safe list designed for situations with many reads and relatively few writes.

When it is modified, it creates a new backing array. Its iterators work over a snapshot and do not reflect later modifications.

Useful when:
- Reads are frequent.
- Writes are infrequent.
- Snapshot iteration is acceptable.

Not ideal for frequent writes because copying the array is expensive.

### Interview-ready answer

**Q: What is the difference between fail-fast and snapshot iteration?**

A fail-fast iterator detects certain structural modifications outside the iterator and may throw `ConcurrentModificationException`; this is best-effort behavior, not synchronization. A CopyOnWriteArrayList iterator traverses a snapshot of the array as it existed when the iterator was created, so later changes are not reflected in that traversal.

---

# 9. Day 8 — Quick Revision Sheet

## 9.1 Core interface map

| Interface | Main purpose |
|---|---|
| `Iterable` | Supports iteration |
| `Collection` | General group of elements |
| `List` | Ordered, indexed, allows duplicates |
| `Set` | Unique elements |
| `Queue` | Holds elements for processing |
| `Deque` | Double-ended queue |
| `Map` | Key-value mappings; not a subtype of Collection |

## 9.2 Implementation cheat sheet

| Class | Internal idea | Order | Typical operation |
|---|---|---|---|
| ArrayList | Resizable array | Insertion/index order | Fast indexed access |
| LinkedList | Doubly linked nodes | Insertion order | Fast end operations |
| HashSet | HashMap-backed | No guarantee | Unique lookup |
| LinkedHashSet | Hashing plus linked order | Insertion order | Unique values with order |
| TreeSet | Balanced search tree | Sorted | Sorted unique values |
| HashMap | Bucket array with collision handling | No guarantee | Key-value lookup |
| LinkedHashMap | Hash map plus linked order | Insertion/access order | Ordered key-value map |
| TreeMap | Balanced search tree | Sorted keys | Sorted key-value map |
| ConcurrentHashMap | Concurrent hash map | No guarantee | Shared concurrent map |
| ArrayDeque | Resizable circular array | Deque order | Operations at both ends |
| PriorityQueue | Heap | Priority order at head | Priority processing |

## 9.3 Complexity cheat sheet

| Data structure / operation | Typical complexity |
|---|---:|
| ArrayList `get(index)` | O(1) |
| ArrayList append | Amortized O(1) |
| ArrayList insert/remove middle | O(n) |
| LinkedList add/remove at ends | O(1) |
| LinkedList get(index) | O(n) |
| HashSet add/contains/remove | Average O(1) |
| LinkedHashSet add/contains/remove | Average O(1) |
| TreeSet add/contains/remove | O(log n) |
| HashMap get/put/remove | Average expected O(1) |
| PriorityQueue peek | O(1) |
| PriorityQueue offer/poll | O(log n) |

Complexities are typical expectations, not universal guarantees under every input and implementation.

## 9.4 High-value interview facts

1. `Collection` is an interface; `Collections` is a utility class.
2. `Map` is part of the framework but does not extend `Collection`.
3. ArrayList uses a resizable array.
4. LinkedList is a doubly linked list.
5. HashSet uses HashMap internally.
6. LinkedHashSet maintains insertion order.
7. TreeSet maintains sorted order.
8. HashMap uses `hashCode()` and `equals()`.
9. HashMap allows one null key and multiple null values.
10. Hashtable and ConcurrentHashMap do not allow null keys or values.
11. ConcurrentHashMap uses modern fine-grained concurrency techniques, not the old Segment design.
12. PriorityQueue is heap-based; its iterator is not sorted.
13. ArrayDeque is usually preferred over legacy Stack.
14. Comparable defines natural ordering; Comparator defines an external ordering.
15. Fail-fast iterators are best effort, not thread safety.
16. CopyOnWriteArrayList uses snapshot iterators.

---

# 10. Interview Questions and Answers

## Q1. What is the Java Collections Framework?

**Answer:** It is a framework of interfaces, implementations, and algorithms for storing and manipulating groups of objects. It includes interfaces such as List, Set, Queue, and Map, implementations such as ArrayList and HashMap, and utility algorithms through the Collections class.

## Q2. What is the difference between Collection and Collections?

**Answer:** Collection is an interface representing a group of elements. Collections is a utility class containing static methods such as sort, reverse, and synchronized wrappers.

## Q3. Does Map extend Collection?

**Answer:** No. Map is part of the Java Collections Framework, but it represents key-value mappings and does not extend Collection.

## Q4. How does ArrayList work internally?

**Answer:** ArrayList uses a resizable array. When the array becomes full, it allocates a larger array and copies the existing elements. Indexed access is O(1), while insertion or removal near the beginning or middle is O(n) because elements must shift. Appending is amortized O(1).

## Q5. What is the difference between size and capacity in ArrayList?

**Answer:** Size is the number of elements currently stored and is returned by `size()`. Capacity is the number of internal array slots available before resizing. Capacity is an implementation detail and is not exposed through a public ArrayList method.

## Q6. Why is ArrayList generally preferred over LinkedList?

**Answer:** ArrayList offers O(1) indexed access, lower memory overhead, and generally better cache locality. LinkedList has efficient operations at both ends, but indexed access and searching require traversal.

## Q7. How does HashSet ensure uniqueness?

**Answer:** HashSet is backed by a HashMap. It uses hash codes to locate buckets and equality checks to determine whether an equal element already exists.

## Q8. What is the difference between HashSet, LinkedHashSet, and TreeSet?

**Answer:** HashSet does not guarantee iteration order, LinkedHashSet preserves insertion order, and TreeSet maintains sorted order. Hash-based operations are typically average O(1), while TreeSet operations are O(log n).

## Q9. How does HashMap work internally?

**Answer:** HashMap uses a bucket array. It calculates a bucket index from a key's hash, stores entries in that bucket, and handles collisions using linked structures and, in modern OpenJDK implementations under certain conditions, tree bins. It uses `equals()` to identify the matching key.

## Q10. What is a collision in HashMap?

**Answer:** A collision occurs when different keys map to the same bucket index. HashMap stores multiple entries in that bucket and distinguishes them using hash information and equality checks.

## Q11. What are the default initial capacity and load factor of HashMap?

**Answer:** The common defaults are an initial capacity of 16 and a load factor of 0.75. The table is generally allocated lazily. At capacity 16, the resize threshold is 12.

## Q12. Why should equals() and hashCode() be overridden together?

**Answer:** Hash-based collections use hashCode to locate a bucket and equals to identify the key. Equal objects must have equal hash codes; otherwise, logically equal keys can become difficult to find.

## Q13. What is the difference between HashMap and ConcurrentHashMap?

**Answer:** HashMap is not thread-safe for concurrent modification. ConcurrentHashMap is designed for concurrent access and uses fine-grained synchronization and CAS-based techniques. ConcurrentHashMap does not allow null keys or values.

## Q14. Why is Hashtable generally not preferred for new code?

**Answer:** Hashtable is a legacy synchronized map with broad method synchronization and no null keys or values. ConcurrentHashMap is generally a better option when concurrent map access is required.

## Q15. What is the difference between Queue and Deque?

**Answer:** A Queue commonly supports insertion at one end and removal from the other. A Deque supports insertion and removal at both ends, so it can be used as either a queue or a stack.

## Q16. How does PriorityQueue work?

**Answer:** PriorityQueue is heap-based. By default, the least element according to natural ordering is at the head. Insertion and head removal are O(log n), while peeking is O(1). Iteration does not guarantee sorted order.

## Q17. Why is ArrayDeque preferred over Stack?

**Answer:** ArrayDeque provides efficient deque and stack operations without the legacy Stack class's inheritance design and method synchronization overhead. It is generally preferred for new stack implementations.

## Q18. What is the difference between Comparable and Comparator?

**Answer:** Comparable defines a class's natural ordering through `compareTo()`. Comparator defines an external ordering through `compare()` and allows multiple sorting strategies without modifying the class.

## Q19. What is a fail-fast iterator?

**Answer:** A fail-fast iterator may throw ConcurrentModificationException when it detects a structural modification made outside the iterator during traversal. This behavior is best effort and is not a concurrency-control mechanism.

## Q20. What is CopyOnWriteArrayList?

**Answer:** It is a thread-safe list that creates a new backing array when modified. Its iterators traverse a snapshot, making it useful for read-heavy, write-light workloads.

---

# 11. Backend Usage Examples

## 11.1 Group employees by department

A Map can represent a department-to-employees relationship.

```java
Map<String, List<String>> employeesByDepartment = new HashMap<>();

employeesByDepartment
        .computeIfAbsent("Engineering", key -> new ArrayList<>())
        .add("Anand");

employeesByDepartment
        .computeIfAbsent("Engineering", key -> new ArrayList<>())
        .add("Rahul");

employeesByDepartment
        .computeIfAbsent("Operations", key -> new ArrayList<>())
        .add("Priya");
```

Result conceptually:

```text
Engineering -> [Anand, Rahul]
Operations  -> [Priya]
```

## 11.2 Remove duplicate employee codes while preserving order

```java
List<String> employeeCodes = List.of(
        "EMP-101",
        "EMP-102",
        "EMP-101",
        "EMP-103"
);

Set<String> uniqueCodes = new LinkedHashSet<>(employeeCodes);

System.out.println(uniqueCodes);
// [EMP-101, EMP-102, EMP-103]
```

## 11.3 Count event types safely

```java
ConcurrentHashMap<String, Long> eventCounts =
        new ConcurrentHashMap<>();

public void record(String eventType) {
    eventCounts.merge(eventType, 1L, Long::sum);
}
```

This demonstrates a concurrent per-key update. It is still only in-memory state for one application instance.

## 11.4 Sort trips by date

Assume a `Trip` class exposes `getTripDate()` and `getTripId()`.

```java
Comparator<Trip> byDateThenId =
        Comparator.comparing(Trip::getTripDate)
                  .thenComparing(Trip::getTripId);

trips.sort(byDateThenId);
```

For large datasets, sorting and pagination should often happen in the database rather than loading every record into Java memory.

## 11.5 Choose collections based on API requirements

Imagine an endpoint returns active driver IDs:

- Use a `List` if response order and duplicates are meaningful.
- Use a `LinkedHashSet` if IDs must be unique while preserving the order in which they were encountered.
- Use a `TreeSet` if IDs must be unique and sorted.
- Use a `Map<Long, DriverResponse>` if each ID needs to map to its response object.

The correct collection depends on the required behavior, not simply which implementation is fastest in isolation.

---

# 12. Completion Checklist

- [ ] Explain the Collections Framework and its major interfaces.
- [ ] Distinguish Collection from Collections.
- [ ] Explain why Map is not a subtype of Collection.
- [ ] Describe ArrayList's internal array, size, and capacity.
- [ ] Explain ArrayList growth and amortized append complexity.
- [ ] Compare ArrayList and LinkedList using operation costs.
- [ ] Explain HashSet and its relationship with HashMap.
- [ ] Compare HashSet, LinkedHashSet, and TreeSet.
- [ ] Explain HashMap buckets, hashing, collisions, and resizing.
- [ ] Explain initial capacity, load factor, and resize threshold.
- [ ] Explain why equals() and hashCode() matter for map keys and set elements.
- [ ] Compare HashMap, Hashtable, and ConcurrentHashMap.
- [ ] Explain Queue, Deque, PriorityQueue, and stack behavior.
- [ ] Explain why PriorityQueue iteration is not sorted.
- [ ] Distinguish Comparable and Comparator.
- [ ] Use an Iterator safely when removing elements during traversal.
- [ ] Explain fail-fast behavior and CopyOnWriteArrayList snapshot iteration.
- [ ] Select a collection based on ordering, uniqueness, access, and concurrency requirements.

---

**Day 8 status:** Java Collections Framework — all eight planned topics covered.

**Next day:** Day 9 — Java Multithreading and Concurrency: processes, threads, thread lifecycle, synchronization, volatile, atomic classes, locks, deadlocks, ExecutorService, and CompletableFuture.
'''

path = Path("/mnt/data/Day_08_Java_Collections_Framework.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
