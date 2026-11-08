from pathlib import Path

content = r'''# Day 10 — JVM Internals and Garbage Collection

> **Purpose:** Understand how the JVM manages memory, identifies unused objects, performs garbage collection, and exposes runtime behavior through diagnostic tools. These topics are important for Java backend interviews and production troubleshooting.
>
> **Learning context:** These notes follow the eight topics covered in Day 10. JVM implementation details are identified as such where appropriate. Examples focus on HotSpot and Java 21 unless stated otherwise.

---

## Table of Contents

1. [JVM Architecture and Runtime Memory Areas](#1-jvm-architecture-and-runtime-memory-areas)
2. [Heap Memory and Generations](#2-heap-memory-and-generations)
3. [Garbage Collection Fundamentals and GC Roots](#3-garbage-collection-fundamentals-and-gc-roots)
4. [Garbage Collection Algorithms](#4-garbage-collection-algorithms)
5. [Garbage Collectors: Serial, Parallel, G1, ZGC, Shenandoah](#5-garbage-collectors-serial-parallel-g1-zgc-shenandoah)
6. [Memory Leaks, OutOfMemoryError, and StackOverflowError](#6-memory-leaks-outofmemoryerror-and-stackoverflowerror)
7. [JVM Flags and Memory Tuning](#7-jvm-flags-and-memory-tuning)
8. [JIT Compiler, Class Loading, Linking, and Initialization](#8-jit-compiler-class-loading-linking-and-initialization)
9. [Day 10 — Quick Revision Sheet](#9-day-10--quick-revision-sheet)
10. [Interview Questions and Answers](#10-interview-questions-and-answers)
11. [Backend Troubleshooting Connections](#11-backend-troubleshooting-connections)
12. [Completion Checklist](#12-completion-checklist)

---

# 1. JVM Architecture and Runtime Memory Areas

## 1.1 What is the JVM?

The Java Virtual Machine (JVM) is the runtime environment that loads and executes Java bytecode.

Its responsibilities include:
- Loading class definitions.
- Verifying and linking classes.
- Managing runtime memory areas.
- Executing bytecode.
- Compiling frequently executed code through JIT compilation.
- Managing garbage collection.
- Supporting threads, exceptions, and other runtime services.

The JVM is specified by the Java Virtual Machine Specification. HotSpot is one widely used implementation.

## 1.2 High-level JVM architecture

A useful conceptual view:

```text
                 Java Application
                        |
                        v
                .class bytecode
                        |
                        v
              +-------------------+
              | Class Loader      |
              +-------------------+
                        |
                        v
              +-------------------+
              | Linking and       |
              | Initialization    |
              +-------------------+
                        |
                        v
              +-------------------+
              | Runtime Data Areas|
              +-------------------+
                        |
                        v
              +-------------------+
              | Execution Engine  |
              | Interpreter + JIT |
              +-------------------+
                        |
                        v
                 Native execution

       Native Method Interface / Native Libraries
              support native interaction
```

## 1.3 JVM subsystems

### A. Class Loader Subsystem

Loads class definitions into the JVM. Common built-in loaders are:
- Bootstrap ClassLoader.
- Platform ClassLoader.
- Application ClassLoader.

The delegation model normally asks parent loaders to load a class before a child loader tries to load it itself.

### B. Runtime Data Areas

The JVM defines memory areas used during execution:
- Heap.
- Method Area.
- Java Stacks.
- PC Registers.
- Native Method Stacks.

### C. Execution Engine

The execution engine executes bytecode using mechanisms such as:
- Interpreter.
- Just-In-Time (JIT) compiler.
- Garbage collector integration.

### D. JNI and Native Libraries

The Java Native Interface (JNI) allows Java code to interact with native code. Native libraries may be implemented in languages such as C or C++.

## 1.4 Runtime memory areas in detail

### Heap

- Shared among threads.
- Stores objects and arrays conceptually.
- Managed by the garbage collector.
- Usually the largest JVM-managed memory area.

Example:

```java
Employee employee = new Employee();
```

The Employee object is conceptually allocated on the heap. The reference variable may be stored in a local stack frame or another object, depending on where it is declared and JVM optimizations.

### Java Stack

Each thread has its own JVM stack. A method invocation creates a stack frame.

A frame contains information such as:
- Local variables.
- Operand stack.
- Method return and execution information.

Example:

```java
public void processTrip() {
    int tripId = 101;
    calculateFare();
}

public void calculateFare() {
    int baseFare = 200;
}
```

When `processTrip()` calls `calculateFare()`, a new frame is created for `calculateFare()`. When the method returns, its frame is removed.

### Method Area

The JVM specification describes a shared method area containing class-level runtime structures, such as:
- Per-class structures.
- Runtime constant pools.
- Field and method data.
- Method code.

In HotSpot, class metadata is commonly stored in **Metaspace**, which uses native memory. The specification's method area is a logical runtime area; do not assume all JVM implementations use the same physical layout.

### PC Register

Each JVM thread has its own program counter register. It identifies the current JVM instruction being executed, with special handling for native methods.

### Native Method Stack

Supports native method execution. The exact implementation is JVM-dependent.

## 1.5 Runtime areas: comparison

| Area | Shared? | Main purpose |
|---|---:|---|
| Heap | Yes | Objects and arrays |
| Java Stack | No, one per thread | Method frames and local execution state |
| Method Area | Yes | Class-level runtime structures |
| PC Register | No, one per thread | Current instruction position |
| Native Method Stack | Per-thread conceptually | Native method execution |

## 1.6 Stack memory vs heap memory

| Stack | Heap |
|---|---|
| Per-thread | Shared |
| Stores method frames | Stores objects and arrays |
| Frames are pushed/popped with method calls | Objects are reclaimed by GC when unreachable |
| Often smaller and has a per-thread limit | Usually much larger |
| Excessive recursion can cause StackOverflowError | Excessive retained allocation can cause OutOfMemoryError |

**Important:** Stack-versus-heap diagrams are conceptual. JIT optimizations such as escape analysis can change physical allocation behavior.

---

# 2. Heap Memory and Generations

## 2.1 What is heap memory?

The heap is the runtime area where Java objects and arrays are allocated conceptually.

Example:

```java
Employee employee = new Employee();
List<String> names = new ArrayList<>();
```

The Employee and ArrayList objects, along with the backing storage and other objects they reference, are managed as heap objects.

The heap is shared by threads, so objects can be accessed by multiple threads if references are shared.

## 2.2 Generational garbage collection

Many garbage collectors use the generational hypothesis:

> Most objects become unreachable relatively soon after they are created, while a smaller set of objects survive for a long time.

A traditional generational heap model divides the heap into:
- Young Generation.
- Old Generation.

```text
                 Heap
        +----------------------+
        |     Young Generation |
        |  +------+  +------+  |
        |  | Eden |  |Survivor| |
        |  +------+  +------+  |
        +----------------------+
        |      Old Generation  |
        +----------------------+
```

This is a useful conceptual model. Actual region layout differs by collector. For example, G1 uses regions and does not represent the heap as one fixed contiguous Eden/Survivor/Old layout.

## 2.3 Young Generation

New objects are commonly allocated in the young generation in generational collectors.

Traditional subareas:

### Eden

Most new objects are initially allocated in Eden in the traditional model.

### Survivor spaces

Objects that survive a young collection may be copied between survivor spaces. They can survive multiple young collections before being promoted to the old generation.

A simplified flow:

```text
New object
    |
    v
  Eden
    |
    | Young GC
    v
Survivor space
    |
    | Survives more collections
    v
Other survivor space / age increases
    |
    v
Old generation (promotion)
```

The exact age and promotion decisions depend on the collector and runtime conditions.

## 2.4 Old Generation

Objects that remain reachable for a longer time may be promoted to the old generation.

Examples of objects that may live longer:
- Application configuration.
- Long-lived caches.
- Shared service objects.
- Data structures retained by application-level registries.

A long-lived object is not automatically a memory leak. It becomes a problem when objects are retained unnecessarily or the retained data grows without control.

## 2.5 Minor, major, and full GC terminology

These terms are commonly used in interview discussions, but their exact meanings can vary by collector and JVM implementation.

- **Young/minor collection:** Collects young-generation objects in a generational collector.
- **Old/major collection:** Often refers to collection activity involving old-generation objects.
- **Full GC:** Usually refers to a broader collection of the heap, often with a significant pause in traditional collectors.

Always interpret GC log events using the selected collector's terminology. Not every collector uses these labels in the same way.

## 2.6 Why generational collection helps

If most new objects die quickly, the collector can reclaim a large number of short-lived objects by focusing on the young generation.

This can reduce the need to scan the entire heap during every collection cycle.

---

# 3. Garbage Collection Fundamentals and GC Roots

## 3.1 What is Garbage Collection?

Garbage Collection (GC) is the JVM's automatic memory-reclamation process.

The garbage collector identifies objects that are no longer reachable from the application's live references and reclaims their memory.

Example:

```java
public void createEmployee() {
    Employee employee = new Employee();
}
```

When the method returns, the local reference goes out of scope. If no other reachable reference points to that Employee object, it becomes eligible for garbage collection.

**Eligible does not mean collected immediately.** The JVM decides when and how to perform collection.

## 3.2 Reachability

An object is reachable if the application can access it by following references from a set of GC roots.

If an object cannot be reached from any GC root, it is generally considered eligible for reclamation.

## 3.3 What are GC Roots?

GC Roots are starting points used by the garbage collector to determine reachability.

Common examples include:
- References in active thread stacks.
- References from static fields of loaded classes.
- Active Java threads.
- JNI references.
- Certain JVM-internal references.

Conceptual diagram:

```text
             GC Roots
                |
       +--------+--------+
       |        |        |
    Stack    Static    JNI
   reference reference reference
       |        |        |
       v        v        v
    Object A  Object B  Object C
       |
       v
    Object D

Objects reachable from roots are retained.
Objects with no path from roots can be reclaimed.
```

## 3.4 Unreachable objects

```java
Employee employee = new Employee();
employee = null;
```

If no other reference points to the object, it becomes unreachable.

But consider:

```java
Employee first = new Employee();
Employee second = first;

first = null;
```

The object is still reachable through `second`.

## 3.5 Circular references

Circular references do not prevent garbage collection by themselves.

```java
class Node {
    Node next;
}

Node first = new Node();
Node second = new Node();

first.next = second;
second.next = first;

first = null;
second = null;
```

If no GC Root can reach either node, both objects can be collected even though they reference each other.

Modern tracing garbage collectors do not rely only on reference counting.

## 3.6 Can we force garbage collection?

You can request it:

```java
System.gc();
```

But this is only a request/hint to the JVM. The JVM is not required to perform a collection immediately, or even to honor the request in the way the caller expects.

Do not use `System.gc()` as a normal application memory-management strategy.

## 3.7 finalize()

Historically, Java supported `finalize()` as a cleanup hook before reclamation. It is deprecated for removal because it is unpredictable and can cause serious lifecycle and performance problems.

Do not use finalization for resource cleanup. Prefer:
- try-with-resources.
- `AutoCloseable`.
- Explicit lifecycle management.
- Cleaner only for specialized fallback cleanup scenarios.

## 3.8 Stop-the-world pauses

Some GC phases pause application threads so the collector can safely perform work. These pauses are commonly called stop-the-world (STW) pauses.

Modern collectors may perform much work concurrently with application threads, but they can still have some pause phases.

The goal is not necessarily zero pause time; it is to balance throughput, latency, memory use, and CPU overhead.

---

# 4. Garbage Collection Algorithms

Different garbage collectors use different combinations of algorithms and techniques.

## 4.1 Mark and Sweep

Two conceptual phases:

1. **Mark:** Identify objects reachable from GC Roots.
2. **Sweep:** Reclaim memory occupied by unmarked objects.

```text
Before:
[A live] [B garbage] [C live] [D garbage]

Mark:
[A marked] [B unmarked] [C marked] [D unmarked]

Sweep:
[A live] [free space] [C live] [free space]
```

Advantage:
- Can reclaim unreachable objects without moving all live objects.

Disadvantage:
- Can leave fragmented free memory.

## 4.2 Mark and Compact

Phases:
1. Mark reachable objects.
2. Move live objects together.
3. Update references.
4. Reclaim the free space at one end.

```text
Before:
[A live] [free] [B live] [free] [C live]

After compaction:
[A live] [B live] [C live] [large free region]
```

Advantage:
- Reduces fragmentation.

Disadvantage:
- Moving objects and updating references costs time.

## 4.3 Copying algorithm

The copying approach divides memory into regions or spaces.

- Objects are allocated in one area.
- Live objects are copied to another area during collection.
- The old area is then reused.

Traditional young-generation collection often uses copying because many young objects die quickly.

Advantage:
- Efficient when most objects are garbage.
- Can avoid fragmentation in the target area.

Disadvantage:
- Requires space for copying live objects.

## 4.4 Generational collection

Generational collection applies different strategies to objects of different ages.

- Young objects are collected frequently.
- Long-lived objects are collected less frequently.
- Surviving objects may be promoted.

It takes advantage of the observation that many objects have short lifetimes.

## 4.5 Comparison

| Algorithm | Main idea | Advantage | Limitation |
|---|---|---|---|
| Mark-Sweep | Mark live, reclaim dead | Does not need to move all live objects | Fragmentation |
| Mark-Compact | Mark live, move together | Reduces fragmentation | Moving objects costs time |
| Copying | Copy live objects to another area | Efficient for short-lived objects | Requires extra space |
| Generational | Collect areas based on object age | Focuses on likely garbage | Needs age/promotion management |

Real collectors combine techniques. For example, a collector can use copying in one region and compaction or region evacuation elsewhere.

---

# 5. Garbage Collectors: Serial, Parallel, G1, ZGC, Shenandoah

A garbage collector is selected according to application requirements such as throughput, latency, heap size, CPU availability, and pause-time goals.

## 5.1 Serial GC

Serial GC uses a single GC thread for collection work.

Characteristics:
- Simple.
- Low overhead in suitable small applications.
- Can be appropriate for small heaps or limited CPU environments.
- Collection pauses can be longer as heap size and live data grow.

Enable in HotSpot with:

```bash
-XX:+UseSerialGC
```

## 5.2 Parallel GC

Parallel GC uses multiple threads for GC work.

Characteristics:
- Focuses on throughput.
- Uses parallel collection phases.
- Can be suitable for batch-style workloads where overall throughput matters more than very low pause times.

Enable:

```bash
-XX:+UseParallelGC
```

## 5.3 G1 GC — Garbage First

G1 divides the heap into multiple regions rather than requiring one fixed contiguous young/old layout.

Characteristics:
- Region-based heap organization.
- Collects regions with substantial reclaimable space.
- Performs much work concurrently and uses evacuation pauses.
- Designed to balance throughput and pause-time goals.
- Common default on typical server-class HotSpot JVM configurations, including many Java 21 deployments.

Enable explicitly:

```bash
-XX:+UseG1GC
```

G1 is often a reasonable starting point for general-purpose server applications, but actual behavior should be measured.

## 5.4 ZGC

ZGC is designed for low-latency garbage collection and large heaps.

Characteristics:
- Aims for very short pause times.
- Performs much of its work concurrently.
- Uses techniques such as colored pointers and load barriers in its implementation.
- Available in modern Java releases, including Java 21.

Example:

```bash
-XX:+UseZGC
```

ZGC does not mean zero pauses or zero GC overhead. It trades CPU and other resources to achieve low latency.

## 5.5 Shenandoah GC

Shenandoah is another low-pause collector that performs much of its work concurrently with application execution.

Characteristics:
- Concurrent compaction.
- Designed to reduce pause times across a range of heap sizes.
- Availability depends on the JDK distribution and build.

A common HotSpot option where supported:

```bash
-XX:+UseShenandoahGC
```

Check the actual JVM distribution for support.

## 5.6 Collector comparison

| Collector | Main goal | General characteristics |
|---|---|---|
| Serial | Simplicity / low overhead | Single-threaded collection |
| Parallel | Throughput | Parallel GC work; pauses can be longer |
| G1 | Balanced server performance | Region-based, pause-time goals |
| ZGC | Low latency | Mostly concurrent, very short pauses |
| Shenandoah | Low latency | Concurrent evacuation/compaction |

## 5.7 Which collector should you choose?

There is no universally best collector.

Consider:
- Application latency requirements.
- Heap size and live-set size.
- CPU availability.
- Allocation rate.
- Throughput requirements.
- GC pause logs and production measurements.

A sensible process:
1. Start with the JDK's suitable default.
2. Observe GC logs and application latency.
3. Identify whether pauses, CPU overhead, or memory pressure are the actual problem.
4. Test an alternative collector under representative load.
5. Compare results before changing production configuration.

---

# 6. Memory Leaks, OutOfMemoryError, and StackOverflowError

## 6.1 What is a memory leak in Java?

A Java memory leak occurs when the application unintentionally retains references to objects that are no longer useful, preventing garbage collection from reclaiming them.

The object may still be reachable, so the garbage collector correctly considers it live—even though the application no longer needs it.

## 6.2 Common causes of memory leaks

### A. Static collections that keep growing

```java
public class Cache {
    private static final List<Object> values = new ArrayList<>();

    public static void add(Object value) {
        values.add(value);
    }
}
```

If objects are added forever and never removed, the static collection retains them.

### B. Unbounded caches

A cache that stores every request result indefinitely can grow without limit.

Solutions may include:
- Maximum size.
- Expiration.
- Eviction policy.
- External cache with lifecycle controls.

### C. Listeners and callbacks not removed

A long-lived publisher may retain listeners that should have been released.

### D. ThreadLocal misuse

A ThreadLocal value can remain associated with a long-lived worker thread if it is not removed when no longer needed.

### E. Unclosed resources

Unclosed streams, sockets, database resources, and other handles can exhaust resources. Not every resource leak is a heap-memory leak, but it can cause similar production symptoms.

Use try-with-resources for AutoCloseable resources:

```java
try (InputStream input =
             Files.newInputStream(Path.of("data.txt"))) {
    // Read data
}
```

## 6.3 OutOfMemoryError

`OutOfMemoryError` is an Error indicating that the JVM or a relevant native/runtime allocation could not be satisfied.

Common variants include:
- `Java heap space`.
- `GC overhead limit exceeded`.
- `Metaspace`.
- `Direct buffer memory`.
- `unable to create native thread`.

These messages point to different resource areas and should not all be diagnosed as a simple heap-size problem.

## 6.4 Common OutOfMemoryError causes

| Message | Possible cause |
|---|---|
| Java heap space | Heap exhausted or retained live set too large |
| GC overhead limit exceeded | Excessive time in GC with very little memory recovered |
| Metaspace | Class metadata memory pressure |
| Direct buffer memory | Direct/native buffers exhausted |
| Unable to create native thread | OS or native-memory/thread resource limit |

The correct diagnosis requires logs, heap dumps, native-memory data, and runtime metrics as appropriate.

## 6.5 StackOverflowError

`StackOverflowError` occurs when a thread's stack cannot accommodate additional frames.

Common cause: infinite or very deep recursion.

```java
public class RecursionExample {
    static void callForever() {
        callForever();
    }

    public static void main(String[] args) {
        callForever();
    }
}
```

Each recursive call adds another stack frame until the stack limit is reached.

## 6.6 OutOfMemoryError vs StackOverflowError

| OutOfMemoryError | StackOverflowError |
|---|---|
| Memory/resource allocation cannot be satisfied | Thread stack limit is exceeded |
| May involve heap, Metaspace, direct buffers, native threads, etc. | Usually deep recursion or excessive stack use |
| Often investigated with heap/native-memory diagnostics | Often investigated using stack traces and recursion analysis |

## 6.7 Diagnosing a suspected memory leak

A practical investigation:

1. Observe heap usage over time under representative load.
2. Check whether used heap returns to a stable post-GC level.
3. Capture a heap dump when the issue is present.
4. Inspect the largest retained object groups.
5. Find GC Root paths retaining those objects.
6. Identify the collection, cache, listener, ThreadLocal, or other owner.
7. Fix the retention problem and repeat the load test.

A heap dump is evidence; a large object alone does not prove a leak. Investigate why it is retained.

---

# 7. JVM Flags and Memory Tuning

JVM flags control memory limits, stack sizes, garbage collection, diagnostics, and runtime behavior.

Use them based on measured requirements rather than copying arbitrary values into production.

## 7.1 -Xms

Sets the initial and minimum heap size for the JVM heap.

```bash
-Xms512m
```

This requests an initial heap size of 512 MB.

## 7.2 -Xmx

Sets the maximum heap size.

```bash
-Xmx2g
```

This sets the maximum Java heap to 2 GB.

Example:

```bash
java -Xms512m -Xmx2g -jar application.jar
```

The process needs more memory than `-Xmx` alone because Metaspace, thread stacks, direct buffers, JIT code cache, native libraries, and other native allocations also consume memory.

## 7.3 -Xss

Sets the stack size for each Java thread.

```bash
-Xss512k
```

If the application creates many threads, per-thread stack reservations can contribute substantially to process memory use.

Increasing `-Xss` can allow deeper call stacks but also increases per-thread memory requirements. It is not a general fix for uncontrolled recursion.

## 7.4 GC selection flags

Examples:

```bash
-XX:+UseG1GC
-XX:+UseParallelGC
-XX:+UseZGC
-XX:+UseShenandoahGC
-XX:+UseSerialGC
```

Only select a collector supported by the actual JDK build.

## 7.5 GC logging

For modern Java releases, unified JVM logging is available.

Example:

```bash
-Xlog:gc*:file=gc.log:time,uptime,level,tags
```

This writes detailed GC-related logs to a file named `gc.log`.

GC logs can help answer:
- How often does collection happen?
- How long are pauses?
- How much memory is reclaimed?
- Is the heap growing?
- Are full collections occurring?
- Is allocation pressure high?

## 7.6 Heap dump on OutOfMemoryError

```bash
-XX:+HeapDumpOnOutOfMemoryError
-XX:HeapDumpPath=./heapdump.hprof
```

This requests a heap dump when an OutOfMemoryError occurs, if the JVM can create it and the destination is writable with sufficient disk space.

Heap dumps may contain sensitive application data. Store and share them securely.

## 7.7 Useful diagnostic commands

Find Java processes:

```bash
jcmd -l
```

Print JVM command-line flags:

```bash
jcmd <pid> VM.flags
```

Print JVM system properties:

```bash
jcmd <pid> VM.system_properties
```

Print a thread dump:

```bash
jcmd <pid> Thread.print
```

Request a heap dump:

```bash
jcmd <pid> GC.heap_dump ./heapdump.hprof
```

Inspect heap information:

```bash
jcmd <pid> GC.heap_info
```

The exact commands and availability depend on the JDK and permissions. Run diagnostics against the correct process.

## 7.8 Example JVM configuration

```bash
java \
  -Xms512m \
  -Xmx2g \
  -Xss512k \
  -XX:+UseG1GC \
  -Xlog:gc*:file=gc.log:time,uptime,level,tags \
  -XX:+HeapDumpOnOutOfMemoryError \
  -XX:HeapDumpPath=./heapdump.hprof \
  -jar employee-transport-backend.jar
```

This is an illustrative configuration, not a recommended production profile for every server. Confirm container memory limits, total process memory, thread count, and workload before selecting values.

## 7.9 Heap vs process memory

The Java heap is only one part of a JVM process's memory use.

Other contributors include:
- Metaspace.
- Thread stacks.
- Direct byte buffers.
- JIT code cache.
- JNI/native allocations.
- JVM internal structures.
- Native libraries.

Therefore, a process can exceed its container memory limit even when heap usage is below `-Xmx`.

---

# 8. JIT Compiler, Class Loading, Linking, and Initialization

## 8.1 Interpreter

The interpreter executes bytecode instructions. It allows code to start running without first compiling every method to native code.

## 8.2 JIT compiler

The Just-In-Time compiler compiles frequently executed bytecode into native machine code and can optimize hot execution paths using runtime profiling information.

Conceptual flow:

```text
Java source
    |
    v
Bytecode
    |
    v
Interpreter executes
    |
    v
Runtime profiling identifies hot code
    |
    v
JIT compiles / optimizes
    |
    v
Native machine code
```

## 8.3 C1 and C2 compilers

In the HotSpot JVM's traditional tiered compilation model:

- **C1:** Client compiler, designed for relatively quick compilation and optimization.
- **C2:** Server compiler, performs more aggressive optimizations for hot code.

Tiered compilation can use multiple levels of execution and compilation. Exact behavior depends on JVM version and configuration.

Do not assume every JVM implementation uses HotSpot's C1/C2 architecture.

## 8.4 Common JIT optimizations

Examples include:
- Method inlining.
- Loop optimizations.
- Dead-code elimination.
- Escape analysis.
- Lock optimizations where safe.
- Speculative optimizations based on profiling.

The JVM may deoptimize code if runtime assumptions stop being valid.

## 8.5 Class loading

Class loading brings class definitions into the JVM.

Common built-in class loaders:
- Bootstrap.
- Platform.
- Application.

The parent delegation model helps ensure platform classes are loaded from trusted parent loaders before child loaders attempt application-level loading.

## 8.6 Linking

Linking consists of:

### Verification

Checks that class-file bytecode is structurally valid and obeys JVM constraints.

### Preparation

Creates static storage and initializes static fields to default values.

### Resolution

Resolves symbolic references to classes, fields, and methods into direct references as required.

Resolution can be performed lazily or eagerly depending on the JVM and reference.

## 8.7 Class initialization

Class initialization executes static field initializers and static initializer blocks.

Example:

```java
class ApplicationConfig {
    static int port = 8080;

    static {
        System.out.println("Static initialization");
    }
}
```

The initialization process is performed once for a class by the JVM's class initialization mechanism, with synchronization to ensure safe initialization.

## 8.8 Complete class lifecycle overview

```text
Class file
    |
    v
Loading
    |
    v
Linking
    |
    +--> Verification
    |
    +--> Preparation
    |
    +--> Resolution
    |
    v
Initialization
    |
    v
Class is ready for use
```

## 8.9 Escape analysis

Escape analysis is a JIT optimization technique that determines whether an object can be accessed outside a particular scope or thread.

It can enable optimizations such as:
- Scalar replacement.
- Eliminating some allocations.
- Removing some synchronization when safe.

This is one reason why a conceptual statement such as "every object always physically lives on the heap" should be treated carefully. The Java memory model and JVM specification define behavior, while JIT optimization can change physical implementation details.

---

# 9. Day 10 — Quick Revision Sheet

## 9.1 Memory areas

| Area | Key point |
|---|---|
| Heap | Shared; objects and arrays |
| Java Stack | Per-thread; method frames |
| Method Area | Shared class-level runtime structures |
| Metaspace | HotSpot native memory for class metadata |
| PC Register | Per-thread instruction position |
| Native Method Stack | Native method execution support |

## 9.2 GC fundamentals

- GC identifies objects unreachable from GC Roots.
- Stack references, static references, active threads, and JNI references can be roots.
- Circular references can be collected if no root can reach them.
- `System.gc()` is only a request.
- `finalize()` is deprecated for removal; do not use it for cleanup.
- Some GC phases pause application threads.
- Generational collectors take advantage of short-lived objects.

## 9.3 GC algorithms

| Algorithm | Remember |
|---|---|
| Mark-Sweep | Mark live, sweep dead; can fragment |
| Mark-Compact | Move live objects together |
| Copying | Copy live objects into another space |
| Generational | Collect objects/regions based on age |

## 9.4 Collectors

| Collector | Main goal |
|---|---|
| Serial | Simple, single-threaded collection |
| Parallel | Throughput |
| G1 | Balanced server performance |
| ZGC | Low latency |
| Shenandoah | Low latency |

## 9.5 Errors and memory flags

| Item | Purpose |
|---|---|
| `-Xms` | Initial/minimum heap |
| `-Xmx` | Maximum heap |
| `-Xss` | Per-thread stack size |
| `-Xlog:gc*` | GC logs |
| `HeapDumpOnOutOfMemoryError` | Capture heap dump on OOME |
| `OutOfMemoryError` | Heap or another runtime/native resource cannot be allocated |
| `StackOverflowError` | Thread stack exhausted |

## 9.6 High-value interview facts

1. Heap is shared; each thread has its own stack.
2. Metaspace is native memory in HotSpot.
3. GC is based on reachability, not simply reference counting.
4. Circular references can be collected.
5. `System.gc()` does not guarantee immediate collection.
6. G1 is a common default in server-class HotSpot configurations.
7. ZGC and Shenandoah aim for low pause times, not zero pauses.
8. A memory leak can occur even when the garbage collector is working correctly.
9. `-Xmx` does not limit total JVM process memory.
10. JIT compilation optimizes frequently executed code.
11. C1 and C2 are HotSpot compiler components.
12. Escape analysis can enable allocation and synchronization optimizations.

---

# 10. Interview Questions and Answers

## Q1. What are the main components of JVM architecture?

**Answer:** A high-level JVM view includes the Class Loader subsystem, runtime data areas, execution engine, and native interface/library support. The execution engine interprets bytecode and may compile hot code using a JIT compiler.

## Q2. What is the difference between heap and stack?

**Answer:** Heap is shared among threads and stores objects and arrays. Each thread has its own Java stack, which stores method frames and local execution state. Heap objects are reclaimed by GC when unreachable.

## Q3. What is Metaspace?

**Answer:** In HotSpot, Metaspace is native memory used for class metadata. It replaced the old permanent generation design in Java 8. It is not part of the Java heap.

## Q4. What is garbage collection?

**Answer:** Garbage collection is the JVM's automatic memory-reclamation process. It identifies objects that are no longer reachable from GC Roots and reclaims their memory.

## Q5. What are GC Roots?

**Answer:** GC Roots are starting references used to determine object reachability. Examples include references in active thread stacks, static fields, active threads, and JNI references.

## Q6. Can circular references cause a memory leak?

**Answer:** Circular references alone do not prevent collection. If no GC Root can reach the objects in the cycle, a tracing garbage collector can reclaim them.

## Q7. Can we force garbage collection using System.gc()?

**Answer:** No. `System.gc()` requests or suggests collection, but the JVM does not have to perform it immediately or honor the request.

## Q8. What is generational garbage collection?

**Answer:** It divides or organizes heap collection around object age, based on the observation that many objects become unreachable quickly. Young objects are collected frequently, while long-lived objects may be promoted or handled in older regions.

## Q9. What is the difference between Mark-Sweep and Mark-Compact?

**Answer:** Mark-Sweep identifies live objects and reclaims unmarked objects, which can leave fragmented memory. Mark-Compact moves live objects together, reducing fragmentation but requiring object movement and reference updates.

## Q10. What is the copying algorithm?

**Answer:** It copies reachable objects from one space or region into another and then reuses the old space. It is useful when most objects in the collected area are short-lived.

## Q11. What is the difference between Serial and Parallel GC?

**Answer:** Serial GC performs collection using a single GC thread. Parallel GC uses multiple GC threads and focuses on throughput.

## Q12. What is G1 GC?

**Answer:** G1 is a region-based collector designed to balance throughput and pause-time goals. It identifies regions with reclaimable space and performs evacuation work. It is a common default for server-class HotSpot configurations.

## Q13. What is the difference between ZGC and Shenandoah?

**Answer:** Both are low-latency collectors that perform much of their work concurrently with application execution. Their internal algorithms and implementation details differ, and support depends on the JDK build.

## Q14. What is a memory leak in Java?

**Answer:** A memory leak occurs when an application unintentionally retains references to objects it no longer needs. Those objects remain reachable and cannot be reclaimed by GC.

## Q15. What is the difference between OutOfMemoryError and StackOverflowError?

**Answer:** OutOfMemoryError indicates that a memory or runtime resource allocation cannot be satisfied. StackOverflowError occurs when a thread's stack cannot accommodate more frames, often due to deep or infinite recursion.

## Q16. What does -Xmx do?

**Answer:** It sets the maximum Java heap size. It does not set the maximum total memory used by the JVM process.

## Q17. What does -Xms do?

**Answer:** It sets the initial and minimum heap size requested for the JVM.

## Q18. What does -Xss do?

**Answer:** It controls the stack size for each Java thread. A larger stack may support deeper call chains but increases per-thread memory requirements.

## Q19. What is JIT compilation?

**Answer:** JIT compiles frequently executed bytecode into optimized native machine code using runtime profiling information, improving performance on hot code paths.

## Q20. What are C1 and C2?

**Answer:** They are compiler components in HotSpot's traditional tiered compilation architecture. C1 compiles relatively quickly, while C2 performs more aggressive optimization for hot code.

## Q21. What is class loading?

**Answer:** Class loading brings a class definition into the JVM. Built-in class loaders include Bootstrap, Platform, and Application loaders. Parent delegation normally asks parent loaders first.

## Q22. What are the phases of linking?

**Answer:** Verification checks bytecode validity, preparation allocates class-level storage and sets default static values, and resolution resolves symbolic references.

## Q23. What is class initialization?

**Answer:** It executes static field initializers and static initializer blocks for a class when the JVM initializes that class.

## Q24. Does -Xmx control all JVM memory?

**Answer:** No. The process also uses Metaspace, thread stacks, direct buffers, JIT code cache, native libraries, and other native/runtime memory.

---

# 11. Backend Troubleshooting Connections

## 11.1 A Spring Boot application gradually uses more memory

Possible causes:
- Unbounded in-memory collections.
- Cache without eviction.
- Large request/response objects retained.
- Static registries retaining objects.
- ThreadLocal values not removed.
- Too many concurrent requests or large buffers.

Investigation:
1. Observe heap usage and GC behavior.
2. Check whether post-GC usage keeps increasing.
3. Capture a heap dump.
4. Inspect retained objects and their GC Root paths.
5. Identify the owning collection or reference.
6. Fix retention and retest.

Do not immediately increase `-Xmx` without understanding the cause.

## 11.2 Application is slow during garbage collection

Inspect:
- GC pause durations.
- Collection frequency.
- Allocation rate.
- Heap occupancy after collection.
- Whether full collections occur.
- CPU utilization and container limits.

Potential responses depend on the evidence:
- Reduce unnecessary allocation.
- Fix object retention.
- Tune heap sizing.
- Test another collector.
- Reduce large temporary data structures.

## 11.3 Container memory limit is exceeded although heap is below -Xmx

Remember that total process memory includes:
- Java heap.
- Metaspace.
- Thread stacks.
- Direct buffers.
- JIT code cache.
- Native libraries and other allocations.

Check container memory limits, thread count, direct-buffer usage, and native-memory diagnostics. Do not treat `-Xmx` as the container memory limit.

## 11.4 A backend request triggers StackOverflowError

Inspect:
- Recursive method calls.
- Entity `toString()` implementations that traverse bidirectional relationships.
- Recursive JSON serialization.
- Deeply nested input.
- Cyclic object traversal.

Fix the recursion or object traversal design. Increasing `-Xss` may only postpone the failure and increase per-thread memory use.

## 11.5 Useful production evidence

| Evidence | What it helps investigate |
|---|---|
| GC logs | Collection frequency, pauses, heap behavior |
| Heap dump | Retained objects and reference paths |
| Thread dump | Blocked threads, deadlocks, stack traces |
| JVM flags | Actual runtime configuration |
| Container metrics | Process memory and CPU limits |
| Application metrics | Request latency, throughput, allocation symptoms |

Treat heap dumps and thread dumps as sensitive operational artifacts because they may contain application data.

---

# 12. Completion Checklist

- [ ] Explain the main JVM subsystems.
- [ ] Describe heap, stack, method area, PC register, and native method stack.
- [ ] Explain Metaspace and why it is not part of the Java heap in HotSpot.
- [ ] Explain the generational hypothesis and traditional young/old model.
- [ ] Explain Eden, survivor spaces, and promotion conceptually.
- [ ] Explain GC Roots and reachability.
- [ ] Explain why circular references can be collected.
- [ ] Explain why System.gc() does not guarantee collection.
- [ ] Compare Mark-Sweep, Mark-Compact, Copying, and Generational collection.
- [ ] Compare Serial, Parallel, G1, ZGC, and Shenandoah.
- [ ] Explain memory leaks and common retention causes.
- [ ] Distinguish OutOfMemoryError from StackOverflowError.
- [ ] Explain -Xms, -Xmx, and -Xss.
- [ ] Explain GC logging and heap-dump options.
- [ ] Explain why JVM process memory is larger than heap memory.
- [ ] Explain interpreter, JIT, C1, C2, and escape analysis.
- [ ] Describe class loading, linking, and initialization.
- [ ] Explain how to investigate increasing memory usage in a Spring Boot service.

---

**Day 10 status:** JVM Internals and Garbage Collection — all eight planned topics covered.

**Next day:** Day 11 — SQL and DBMS Fundamentals: database architecture, SQL command categories, keys and constraints, joins, subqueries, indexing, transactions, and normalization.
'''

path = Path("/mnt/data/Day_10_JVM_Internals_Garbage_Collection.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
