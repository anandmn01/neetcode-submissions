from pathlib import Path

content = r'''# Day 9 — Java Multithreading and Concurrency

> **Purpose:** Understand how Java runs multiple tasks, coordinates shared data, and performs asynchronous work. These concepts are important for backend interviews and for building reliable Spring Boot services.
>
> **Learning context:** These notes follow the eight topics covered in Day 9. Examples use modern Java syntax and standard concurrency APIs.

---

## Table of Contents

1. [Process vs Thread and Multithreading Fundamentals](#1-process-vs-thread-and-multithreading-fundamentals)
2. [Creating Threads in Java](#2-creating-threads-in-java)
3. [Thread Lifecycle and Important Methods](#3-thread-lifecycle-and-important-methods)
4. [Synchronization and Race Conditions](#4-synchronization-and-race-conditions)
5. [`synchronized`, `volatile`, and Atomic Classes](#5-synchronized-volatile-and-atomic-classes)
6. [Locks, ReentrantLock, and Deadlocks](#6-locks-reentrantlock-and-deadlocks)
7. [ExecutorService and Thread Pools](#7-executorservice-and-thread-pools)
8. [CompletableFuture and Asynchronous Programming](#8-completablefuture-and-asynchronous-programming)
9. [Day 9 — Quick Revision Sheet](#9-day-9--quick-revision-sheet)
10. [Interview Questions and Answers](#10-interview-questions-and-answers)
11. [Backend Engineering Connections](#11-backend-engineering-connections)
12. [Completion Checklist](#12-completion-checklist)

---

# 1. Process vs Thread and Multithreading Fundamentals

## 1.1 What is a process?

A process is an instance of a running program.

A process typically has:
- Its own virtual address space.
- Its own process-level resources.
- One or more threads.
- Operating-system-managed resources such as open handles or file descriptors.

For example, when you start a Spring Boot application, the operating system starts a Java process for that application.

## 1.2 What is a thread?

A thread is an execution path within a process.

A process can have multiple threads executing different tasks. Threads in the same process share resources such as the process heap and open resources, but each thread has its own execution stack and thread-specific state.

Example:

```text
Java Process
    |
    +-- Main Thread
    |
    +-- HTTP Request Thread
    |
    +-- Database / Background Worker Thread
    |
    +-- Scheduler Thread
```

A Spring Boot server may handle multiple incoming requests using different request-processing threads, depending on the server and configuration.

## 1.3 Process vs Thread

| Process | Thread |
|---|---|
| Running program instance | Execution path inside a process |
| Has its own virtual address space | Shares process address space |
| Own process resources | Shares many process resources |
| More expensive to create and switch in general | Usually lighter-weight than a process |
| Communicates with other processes using IPC mechanisms | Can communicate through shared memory |
| Failure isolation is generally stronger | A serious thread failure can affect the process |

## 1.4 What is multithreading?

Multithreading means using multiple threads within one process so that multiple tasks can make progress independently.

Example: a backend service may:
- Process one HTTP request.
- Run another independent operation.
- Write audit information.
- Execute a scheduled cleanup task.

The goal is not automatically to make every operation faster. Multithreading is useful when independent tasks can make progress concurrently.

## 1.5 Concurrency vs Parallelism

These terms are related but different.

### Concurrency

Concurrency means multiple tasks are in progress during overlapping time periods. The runtime may switch between tasks.

### Parallelism

Parallelism means multiple tasks are literally executing at the same time, typically on different CPU cores.

```text
Concurrency on one execution resource:

Task A:  A A   A A
Task B:    B B   B B
Time  --->

Parallel execution:

Core 1:  A A A A A
Core 2:  B B B B B
Time  --->
```

A concurrent program does not require multiple CPU cores. Parallel execution does.

## 1.6 CPU-bound vs I/O-bound tasks

### CPU-bound

The task spends most of its time doing computation.

Examples:
- Image processing.
- Compression.
- Large calculations.
- Complex data transformations.

More threads do not necessarily improve CPU-bound performance beyond available processing capacity. Too many runnable threads can increase context-switching overhead.

### I/O-bound

The task spends much of its time waiting for external operations.

Examples:
- Database queries.
- HTTP calls.
- File reads.
- Remote service calls.

Concurrency can improve throughput because another task can progress while one task waits. But thread count, connection pools, remote-service limits, and memory still need to be controlled.

## 1.7 Why multithreading matters in backend systems

Imagine a transport application receives requests to:
- Fetch driver information.
- Fetch vehicle information.
- Fetch trip information.

If independent remote calls are performed sequentially, total latency may be close to the sum of their durations. If they can safely run concurrently, the total time may approach the duration of the slowest call plus coordination overhead.

However:
- Do not parallelize operations that depend on one another.
- Do not share non-thread-safe objects without protection.
- Do not overload the database connection pool or remote service.

### Interview-ready answer

**Q: What is the difference between concurrency and parallelism?**

Concurrency means multiple tasks are in progress over overlapping periods, possibly by switching between them. Parallelism means multiple tasks execute at the same instant on separate processing resources. A program can be concurrent on one CPU core without being parallel.

---

# 2. Creating Threads in Java

Java provides several ways to execute work concurrently. It is important to distinguish the task from the thread that executes it.

## 2.1 Extending the Thread class

You can create a subclass of `Thread` and override `run()`.

```java
class MyThread extends Thread {
    @Override
    public void run() {
        System.out.println("Running on: " +
                Thread.currentThread().getName());
    }
}

public class Main {
    public static void main(String[] args) {
        MyThread thread = new MyThread();
        thread.start();
    }
}
```

Calling `start()` asks the JVM to start a new thread. The new thread eventually executes `run()`.

## 2.2 Implementing Runnable

A common approach is to implement `Runnable`.

```java
class EmployeeTask implements Runnable {
    @Override
    public void run() {
        System.out.println("Processing employee task");
    }
}

public class Main {
    public static void main(String[] args) {
        Thread thread = new Thread(new EmployeeTask());
        thread.start();
    }
}
```

This separates:
- The task: `Runnable`.
- The execution mechanism: `Thread`.

It is generally more flexible because the task class remains free to extend another class.

## 2.3 Using a lambda with Runnable

`Runnable` is a functional interface, so a lambda can represent its task.

```java
public class Main {
    public static void main(String[] args) {
        Thread thread = new Thread(() -> {
            System.out.println("Task running");
        });

        thread.start();
    }
}
```

## 2.4 start() vs run()

This is a common interview question.

### Calling start()

```java
Thread thread = new Thread(() ->
        System.out.println(Thread.currentThread().getName())
);

thread.start();
```

- Starts a new thread of execution.
- The new thread invokes `run()`.
- The caller can continue independently.

### Calling run() directly

```java
Thread thread = new Thread(() ->
        System.out.println(Thread.currentThread().getName())
);

thread.run();
```

- Does not start a new thread.
- It is an ordinary method call on the current thread.
- The code runs synchronously in the caller.

**Remember:** `start()` starts a new thread; `run()` is the task method.

Calling `start()` twice on the same Thread instance throws `IllegalThreadStateException`.

## 2.5 Runnable vs Callable

`Runnable` represents a task that does not return a result and does not declare checked exceptions from its `run()` method.

`Callable<V>` represents a task that returns a result and may throw a checked exception.

```java
Callable<Integer> task = () -> {
    return 100 + 200;
};
```

Comparison:

| Runnable | Callable |
|---|---|
| `run()` | `call()` |
| No result | Returns a value |
| Cannot declare checked exceptions in method signature | Can throw checked exceptions |
| Common with Thread and ExecutorService | Common with ExecutorService and Future |

A Callable is generally submitted to an ExecutorService and its result is obtained through a Future.

## 2.6 Thread naming

Naming threads helps debugging.

```java
Thread thread = new Thread(
        () -> System.out.println("Working"),
        "employee-worker-1"
);

thread.start();
```

Thread names are useful in logs and thread dumps.

## 2.7 Why implementing Runnable is often preferred

- Separates task logic from thread management.
- Allows the class to extend another class.
- Works naturally with executor services and thread pools.
- Encourages reuse of task definitions.

In production backend applications, you will usually submit tasks to an executor rather than create a new raw Thread for every request.

---

# 3. Thread Lifecycle and Important Methods

## 3.1 Java thread states

Java exposes these states through `Thread.State`:

1. NEW
2. RUNNABLE
3. BLOCKED
4. WAITING
5. TIMED_WAITING
6. TERMINATED

Conceptual lifecycle:

```text
              start()
    NEW -----------------> RUNNABLE
                              |
                 +------------+-------------+
                 |            |             |
              BLOCKED       WAITING     TIMED_WAITING
                 |            |             |
                 +------------+-------------+
                              |
                              v
                         RUNNABLE
                              |
                         run() ends
                              |
                              v
                         TERMINATED
```

The JVM's RUNNABLE state includes both ready-to-run and actually running states. Java does not expose a separate RUNNING state in `Thread.State`.

## 3.2 NEW

A Thread object has been created, but `start()` has not been called.

```java
Thread thread = new Thread(() -> {
    System.out.println("Hello");
});

System.out.println(thread.getState()); // NEW
```

## 3.3 RUNNABLE

After `start()`, the thread is eligible to run or is running.

```java
thread.start();
```

The operating-system scheduler decides when it gets CPU time.

## 3.4 BLOCKED

A thread is BLOCKED when it is waiting to acquire a monitor lock to enter a `synchronized` block or method.

Example:

```java
synchronized (lock) {
    // Another thread may be BLOCKED waiting for this lock.
}
```

## 3.5 WAITING

A thread is WAITING when it waits indefinitely for another thread or action to perform a particular event.

Examples:
- `Object.wait()` without a timeout.
- `Thread.join()` without a timeout.
- `LockSupport.park()`.

## 3.6 TIMED_WAITING

A thread waits for a limited period.

Examples:
- `Thread.sleep(milliseconds)`.
- `Object.wait(timeout)`.
- `Thread.join(timeout)`.
- `LockSupport.parkNanos(...)`.

## 3.7 TERMINATED

The thread has completed execution because its `run()` method returned or ended due to an uncaught failure.

A terminated Thread instance cannot be restarted.

## 3.8 sleep()

`Thread.sleep()` pauses the current thread for a period.

```java
try {
    Thread.sleep(1000);
} catch (InterruptedException e) {
    Thread.currentThread().interrupt();
}
```

Important:
- It is a static method that pauses the current thread.
- It does not release locks that the thread already holds.
- It can throw `InterruptedException`.
- It does not guarantee the thread resumes at the exact requested time.

## 3.9 wait()

`wait()` is a method on Object and must be called while holding that object's monitor.

```java
synchronized (lock) {
    lock.wait();
}
```

It releases the monitor for that object while waiting. The thread must reacquire the monitor before returning from `wait()`.

Use a condition loop because a thread can wake up without the condition becoming true (spurious wakeup), or another thread may consume the condition first:

```java
synchronized (lock) {
    while (!conditionIsTrue()) {
        lock.wait();
    }

    // Work after condition is satisfied
}
```

## 3.10 sleep() vs wait()

| `sleep()` | `wait()` |
|---|---|
| Defined in Thread | Defined in Object |
| Pauses current thread | Waits for notification or timeout |
| Does not release held monitors | Releases the monitor of the object on which it is called |
| Can be called without owning an object's monitor | Must own that object's monitor |
| Wakes after timeout or interruption | Can wake due to notify, notifyAll, timeout, or interruption |

## 3.11 join()

`join()` makes the current thread wait for another thread to finish.

```java
Thread worker = new Thread(() -> {
    System.out.println("Worker task");
});

worker.start();

try {
    worker.join();
} catch (InterruptedException e) {
    Thread.currentThread().interrupt();
}

System.out.println("Worker completed");
```

The final message is printed after the worker terminates.

## 3.12 interrupt()

Interruption is a cooperative cancellation signal. It does not forcibly kill a thread.

```java
Thread worker = new Thread(() -> {
    while (!Thread.currentThread().isInterrupted()) {
        // Perform a small unit of work
    }
});

worker.start();
worker.interrupt();
```

If a thread is blocked in an interruptible operation such as `sleep()` or `wait()`, it may receive `InterruptedException`.

A common handling pattern:

```java
try {
    Thread.sleep(5000);
} catch (InterruptedException e) {
    Thread.currentThread().interrupt();
    // Stop or propagate cancellation appropriately.
}
```

If a method cannot propagate the checked exception, restoring the interrupt flag preserves the cancellation signal for higher-level code.

## 3.13 Important thread methods

| Method | Purpose |
|---|---|
| `start()` | Starts a new thread |
| `run()` | Contains task code |
| `sleep()` | Pauses current thread |
| `wait()` | Waits on an object's monitor |
| `notify()` | Wakes one waiting thread on that monitor |
| `notifyAll()` | Wakes all threads waiting on that monitor |
| `join()` | Waits for another thread to terminate |
| `interrupt()` | Requests interruption |
| `isInterrupted()` | Checks another/current thread's interrupt status |
| `Thread.interrupted()` | Checks and clears current thread's interrupt status |
| `currentThread()` | Returns the currently executing Thread |

---

# 4. Synchronization and Race Conditions

## 4.1 What is a race condition?

A race condition occurs when the correctness of a result depends on the timing or interleaving of multiple threads accessing shared state.

Example:

```java
class Counter {
    int count = 0;

    void increment() {
        count++;
    }
}
```

If two threads call `increment()` at the same time, the final result may be incorrect.

## 4.2 Why is count++ not atomic?

The expression:

```java
count++;
```

is a read-modify-write operation. Conceptually:

1. Read the current value.
2. Add one.
3. Write the updated value.

Possible interleaving:

```text
Initial count = 0

Thread A reads 0
Thread B reads 0
Thread A calculates 1
Thread B calculates 1
Thread A writes 1
Thread B writes 1

Final count = 1
Expected count = 2
```

This is a lost update.

## 4.3 What is synchronization?

Synchronization coordinates access to shared mutable state so that critical operations are protected.

Java's `synchronized` keyword uses an object's monitor lock.

```java
class Counter {
    private int count = 0;

    synchronized void increment() {
        count++;
    }

    synchronized int getCount() {
        return count;
    }
}
```

Only one thread at a time can hold the same object's monitor and enter a synchronized instance method or block protected by that monitor.

## 4.4 Critical section

A critical section is a portion of code that accesses shared state and must be coordinated.

```java
synchronized (this) {
    count++;
}
```

Keep critical sections as small as practical. Do not hold a lock around slow network calls or database calls unless there is a specific, justified reason.

## 4.5 Instance synchronized method

```java
public synchronized void updateStatus() {
    // Protected by this object's monitor
}
```

The lock is the current object (`this`).

Two threads using the same object cannot simultaneously execute synchronized instance methods that lock that same object. Two different instances have different monitors.

## 4.6 Static synchronized method

```java
public static synchronized void updateGlobalStatus() {
    // Protected by the Class object's monitor
}
```

A static synchronized method locks the Class object, such as `Counter.class`.

It does not lock a particular instance.

## 4.7 Synchronized block

```java
public void update() {
    synchronized (lock) {
        // Critical section
    }
}
```

A block lets you choose the lock object and protect only a specific region.

## 4.8 Synchronization provides visibility too

Synchronization does more than mutual exclusion.

When one thread releases a monitor and another thread subsequently acquires the same monitor, Java's happens-before rules ensure relevant writes made before release are visible after acquisition.

This is important because without a happens-before relationship, one thread is not guaranteed to observe another thread's latest write.

## 4.9 Important backend distinction: JVM lock vs database lock

A Java `synchronized` block protects threads coordinated through the same monitor in one JVM.

It does not coordinate:
- Another Spring Boot instance.
- Another process.
- Another server.
- A separate application using the same database.

For cross-instance business consistency, use appropriate database transactions, constraints, optimistic locking, pessimistic locking, or distributed coordination where justified.

Example: if two backend instances try to assign the same driver to different trips, a local Java lock in each instance cannot prevent both assignments. The database must enforce the relevant consistency rule.

---

# 5. synchronized, volatile, and Atomic Classes

These mechanisms solve related but different concurrency problems.

## 5.1 synchronized

`synchronized` provides:
- Mutual exclusion for code protected by the same monitor.
- Memory visibility through monitor acquire/release happens-before rules.

Example:

```java
class BankAccount {
    private int balance = 1000;

    public synchronized void deposit(int amount) {
        balance += amount;
    }

    public synchronized int getBalance() {
        return balance;
    }
}
```

The same account object's monitor protects these operations.

## 5.2 volatile

`volatile` is used for visibility and ordering guarantees for a variable across threads.

```java
class Worker {
    private volatile boolean running = true;

    public void stop() {
        running = false;
    }

    public void run() {
        while (running) {
            // Do work
        }
    }
}
```

The volatile write to `running` is made visible to threads that subsequently read it.

### What volatile does not do

It does not make compound operations atomic.

```java
private volatile int count = 0;

public void increment() {
    count++; // Still not atomic
}
```

Multiple threads can still lose updates because increment is a read-modify-write operation.

## 5.3 Atomic classes

The `java.util.concurrent.atomic` package provides classes for atomic operations on individual values.

Common classes:
- `AtomicInteger`
- `AtomicLong`
- `AtomicBoolean`
- `AtomicReference<T>`

Example:

```java
import java.util.concurrent.atomic.AtomicInteger;

class Counter {
    private final AtomicInteger count = new AtomicInteger(0);

    public void increment() {
        count.incrementAndGet();
    }

    public int getCount() {
        return count.get();
    }
}
```

The increment is atomic.

## 5.4 CAS — Compare-And-Set

CAS means Compare-And-Set (also commonly called Compare-And-Swap).

Conceptually:

1. Read the current value.
2. Compare it with an expected value.
3. If it still matches, update it.
4. If it does not match, retry or report failure.

This supports lock-free or low-lock algorithms in many atomic classes.

## 5.5 synchronized vs volatile vs AtomicInteger

| Feature | synchronized | volatile | AtomicInteger |
|---|---|---|---|
| Visibility | Yes | Yes | Yes, through atomic memory semantics |
| Mutual exclusion | Yes, for same monitor | No | No general critical-section lock |
| Atomic increment | Yes, inside protected region | No | Yes |
| Multiple-field invariant | Can protect it | Not by itself | Not by itself |
| Typical use | Protect compound shared operations | Shared status/flag | Atomic counter or single-value update |

## 5.6 When should you use each?

Use `volatile` when:
- One variable represents a status or flag.
- Writes and reads need visibility.
- The operation does not require a compound atomic update.

Use an atomic class when:
- You need atomic updates to one value.
- A counter or reference can be updated independently.

Use `synchronized` or an appropriate Lock when:
- Multiple operations must happen as one critical section.
- Multiple fields must maintain a consistent invariant.
- You need mutual exclusion around a larger operation.

---

# 6. Locks, ReentrantLock, and Deadlocks

## 6.1 What is a Lock?

The `Lock` interface provides explicit lock and unlock operations.

`ReentrantLock` is a common implementation.

```java
import java.util.concurrent.locks.ReentrantLock;

class Counter {
    private final ReentrantLock lock = new ReentrantLock();
    private int count = 0;

    public void increment() {
        lock.lock();

        try {
            count++;
        } finally {
            lock.unlock();
        }
    }
}
```

**Important:** Put `unlock()` in a `finally` block so the lock is released even if an exception occurs.

## 6.2 What does reentrant mean?

A lock is reentrant when the thread holding it can acquire it again without deadlocking itself.

For example, one synchronized method can call another synchronized method on the same object because Java's intrinsic monitors are reentrant.

ReentrantLock is also reentrant.

## 6.3 ReentrantLock vs synchronized

| `synchronized` | `ReentrantLock` |
|---|---|
| Language keyword | Class implementing Lock |
| Lock release is automatic when block/method exits | Must unlock explicitly |
| No timed lock acquisition API | Supports `tryLock()` and timed acquisition |
| No interruptible lock acquisition for monitor entry | Supports `lockInterruptibly()` |
| Simple and concise | More flexible control |
| Reentrant | Reentrant |

Use `synchronized` when simple monitor-based coordination is enough. Use `ReentrantLock` when its extra capabilities are genuinely needed.

## 6.4 tryLock()

`tryLock()` attempts to acquire a lock without waiting indefinitely.

```java
if (lock.tryLock()) {
    try {
        // Perform protected work
    } finally {
        lock.unlock();
    }
} else {
    // Lock unavailable: choose a safe alternative
}
```

A timed version can wait for a limited duration.

Do not simply skip required business work when the lock is unavailable. The failure path must be part of the design.

## 6.5 Deadlock

A deadlock occurs when two or more threads are permanently waiting for locks or resources held by one another, so none can make progress.

Example:

```text
Thread A:
  Holds Lock 1
  Waiting for Lock 2

Thread B:
  Holds Lock 2
  Waiting for Lock 1
```

Neither thread can continue.

## 6.6 Four Coffman conditions

The classic necessary conditions for deadlock are:

1. **Mutual exclusion:** At least one resource can be held by only one thread at a time.
2. **Hold and wait:** A thread holds one resource while waiting for another.
3. **No preemption:** Resources cannot simply be taken away from a thread.
4. **Circular wait:** A cycle exists in which each thread waits for a resource held by the next.

Breaking at least one of these conditions can prevent the classic deadlock scenario.

## 6.7 How to prevent deadlocks

### Strategy 1: Consistent lock ordering

Always acquire locks in the same global order.

```text
All threads:
  Lock A first
  Lock B second
```

### Strategy 2: Avoid unnecessary nested locks

Reduce the number of locks held simultaneously.

### Strategy 3: Keep critical sections small

Do not hold locks while performing slow external calls unless essential.

### Strategy 4: Use tryLock with a timeout where appropriate

This can avoid indefinite waiting, but the application must handle partial work and retry safely.

### Strategy 5: Inspect thread dumps

A thread dump can reveal threads waiting on monitors or locks and help identify a deadlock.

Useful Java diagnostic commands include:

```bash
jcmd <pid> Thread.print
```

or:

```bash
jstack <pid>
```

## 6.8 Deadlock example

The following is intentionally unsafe and demonstrates inconsistent lock ordering:

```java
Object lockA = new Object();
Object lockB = new Object();

// Thread 1
synchronized (lockA) {
    synchronized (lockB) {
        // Work
    }
}

// Thread 2
synchronized (lockB) {
    synchronized (lockA) {
        // Work
    }
}
```

If Thread 1 holds `lockA` and Thread 2 holds `lockB`, each can wait forever for the other's lock.

The fix is to make both threads acquire `lockA` and then `lockB`, or redesign the operation to avoid nested locking.

---

# 7. ExecutorService and Thread Pools

## 7.1 Why not create a new Thread for every task?

Creating a new thread for every request or task can cause:
- Thread creation overhead.
- Excessive memory use.
- Too many runnable threads.
- High context-switching overhead.
- Uncontrolled load on databases and external services.

A thread pool reuses a bounded or otherwise managed set of worker threads to execute submitted tasks.

## 7.2 What is ExecutorService?

`ExecutorService` is an abstraction for managing asynchronous task execution.

Instead of creating and managing threads directly, you submit tasks to an executor.

```java
ExecutorService executor = Executors.newFixedThreadPool(3);

executor.submit(() -> {
    System.out.println("Task running on " +
            Thread.currentThread().getName());
});

executor.shutdown();
```

## 7.3 Fixed thread pool

```java
ExecutorService executor =
        Executors.newFixedThreadPool(5);
```

A fixed thread pool uses a fixed number of worker threads. The standard factory uses a shared unbounded work queue, which can allow pending tasks to accumulate if submission exceeds processing capacity.

For production systems, understand the queue and rejection behavior, and consider configuring a `ThreadPoolExecutor` with a bounded queue.

## 7.4 Other common executor factories

| Factory | Behavior |
|---|---|
| `newFixedThreadPool(n)` | Fixed number of workers |
| `newSingleThreadExecutor()` | One worker processes tasks sequentially |
| `newCachedThreadPool()` | Creates/reuses threads dynamically; can grow substantially |
| `newScheduledThreadPool(n)` | Supports delayed and periodic tasks |

These factory methods are convenient, but production services should choose pool size, queue capacity, thread naming, and rejection policy deliberately.

## 7.5 execute() vs submit()

### execute()

Accepts a Runnable and returns no Future.

```java
executor.execute(() -> {
    System.out.println("Executing task");
});
```

### submit()

Accepts Runnable or Callable and returns a Future.

```java
Future<Integer> future = executor.submit(() -> 100 + 200);
```

You can retrieve the result:

```java
try {
    Integer result = future.get();
    System.out.println(result);
} catch (InterruptedException e) {
    Thread.currentThread().interrupt();
} catch (ExecutionException e) {
    throw new RuntimeException("Task failed", e.getCause());
}
```

`Future.get()` blocks until the result is available or the task completes exceptionally.

## 7.6 Runnable vs Callable in ExecutorService

```java
Future<?> noResult = executor.submit(() -> {
    System.out.println("No result");
});

Future<Integer> result = executor.submit(() -> 10 + 20);
```

- Runnable represents work without a result.
- Callable represents work with a result.
- Future represents a pending or completed asynchronous result.

## 7.7 shutdown() vs shutdownNow()

### shutdown()

- Stops accepting new tasks.
- Allows previously submitted tasks to complete.

### shutdownNow()

- Attempts to stop active tasks, usually by interrupting worker threads.
- Returns tasks that were awaiting execution.
- Does not guarantee every active task stops immediately.

Graceful termination example:

```java
executor.shutdown();

try {
    if (!executor.awaitTermination(30, TimeUnit.SECONDS)) {
        executor.shutdownNow();
    }
} catch (InterruptedException e) {
    executor.shutdownNow();
    Thread.currentThread().interrupt();
}
```

## 7.8 Configuring a bounded ThreadPoolExecutor

A bounded queue helps prevent unlimited task accumulation.

```java
ThreadPoolExecutor executor = new ThreadPoolExecutor(
        4,                              // Core pool size
        8,                              // Maximum pool size
        60,                             // Keep-alive time
        TimeUnit.SECONDS,
        new ArrayBlockingQueue<>(100),  // Bounded queue
        new ThreadPoolExecutor.CallerRunsPolicy()
);
```

This is an example configuration, not a universal recommended pool size. Select values based on workload, CPU, blocking time, downstream limits, and load testing.

A rejection policy defines what happens when the executor cannot accept more tasks.

## 7.9 Spring @Async

Spring supports asynchronous method execution using `@Async`, when asynchronous execution is enabled and a suitable executor is configured.

```java
@Configuration
@EnableAsync
public class AsyncConfig {
}
```

```java
@Service
public class NotificationService {

    @Async
    public void sendNotification() {
        // Background task
    }
}
```

Important:
- Spring's async behavior is generally proxy-based.
- Calling an `@Async` method from another method on the same object can bypass the proxy and therefore run synchronously.
- Configure an appropriate executor instead of relying blindly on defaults.
- Handle exceptions from asynchronous work.
- Do not assume the caller's transaction automatically propagates to the async thread.
- Do not share a non-thread-safe EntityManager or persistence context across threads.

---

# 8. CompletableFuture and Asynchronous Programming

## 8.1 What is CompletableFuture?

`CompletableFuture<T>` represents a result that may become available later and supports composing asynchronous operations.

It is useful when:
- Independent service calls can run concurrently.
- One operation depends on another asynchronous result.
- You need to transform, combine, or handle failures in an asynchronous pipeline.

## 8.2 supplyAsync()

Use `supplyAsync()` when the asynchronous task returns a value.

```java
CompletableFuture<String> future =
        CompletableFuture.supplyAsync(() -> {
            return "Employee details";
        });
```

## 8.3 runAsync()

Use `runAsync()` when the task does not return a value.

```java
CompletableFuture<Void> future =
        CompletableFuture.runAsync(() -> {
            System.out.println("Sending notification");
        });
```

## 8.4 thenApply()

Transforms the result and returns a new CompletableFuture.

```java
CompletableFuture<String> future =
        CompletableFuture.supplyAsync(() -> "anand")
                .thenApply(String::toUpperCase);
```

Conceptually:

```text
Input task
   |
   v
"anand"
   |
   v
thenApply(uppercase)
   |
   v
"ANAND"
```

## 8.5 thenAccept()

Consumes the result without producing a new result value.

```java
CompletableFuture<Void> future =
        CompletableFuture.supplyAsync(() -> "Employee created")
                .thenAccept(System.out::println);
```

## 8.6 thenRun()

Runs an action after the previous stage completes, without receiving its result.

```java
CompletableFuture<Void> future =
        CompletableFuture.supplyAsync(() -> "Done")
                .thenRun(() -> System.out.println("Next stage completed"));
```

## 8.7 thenCompose()

Use `thenCompose()` when the next operation itself returns a CompletableFuture and you want to flatten the nested asynchronous result.

```java
CompletableFuture<Employee> employeeFuture =
        findEmployeeAsync(101L);

CompletableFuture<List<Trip>> tripsFuture =
        employeeFuture.thenCompose(employee ->
                findTripsAsync(employee.getId()));
```

Conceptually:

```text
Employee Future
      |
      v
thenCompose
      |
      v
Trips Future
```

Without composition, a mapping operation that returns another future can produce a nested `CompletableFuture<CompletableFuture<T>>`.

## 8.8 thenCombine()

Use `thenCombine()` to combine the results of two independent futures after both complete.

```java
CompletableFuture<String> driverFuture =
        CompletableFuture.supplyAsync(() -> "Driver: Anand");

CompletableFuture<String> vehicleFuture =
        CompletableFuture.supplyAsync(() -> "Vehicle: HR-01");

CompletableFuture<String> summaryFuture =
        driverFuture.thenCombine(
                vehicleFuture,
                (driver, vehicle) -> driver + " | " + vehicle
        );
```

## 8.9 exceptionally()

Handles an exception and can provide a fallback result.

```java
CompletableFuture<String> future =
        CompletableFuture.supplyAsync(() -> {
            throw new RuntimeException("Service unavailable");
        }).exceptionally(ex -> "Fallback response");
```

## 8.10 handle()

`handle()` can process either the successful result or the exception and return a new result.

```java
CompletableFuture<String> future =
        CompletableFuture.supplyAsync(() -> "Success")
                .handle((result, exception) -> {
                    if (exception != null) {
                        return "Recovered";
                    }
                    return result;
                });
```

## 8.11 whenComplete()

`whenComplete()` is useful for observing the result or failure, for example for logging. It does not normally convert a failed computation into a successful one.

```java
CompletableFuture<String> future =
        CompletableFuture.supplyAsync(() -> "Completed")
                .whenComplete((result, exception) -> {
                    if (exception != null) {
                        System.err.println(exception.getMessage());
                    } else {
                        System.out.println(result);
                    }
                });
```

## 8.12 allOf()

`allOf()` completes when all supplied futures complete.

```java
CompletableFuture<String> driver =
        CompletableFuture.supplyAsync(() -> "Driver");

CompletableFuture<String> vehicle =
        CompletableFuture.supplyAsync(() -> "Vehicle");

CompletableFuture<Void> all =
        CompletableFuture.allOf(driver, vehicle);

CompletableFuture<List<String>> results =
        all.thenApply(ignored ->
                List.of(driver.join(), vehicle.join()));
```

`allOf()` returns `CompletableFuture<Void>`, so collect individual results from the original futures after the combined future completes.

## 8.13 join() vs get()

Both wait for a result if it is not yet available.

| `get()` | `join()` |
|---|---|
| Declares checked `InterruptedException` and `ExecutionException` | Throws unchecked `CompletionException` for exceptional completion |
| Common with Future | Convenient with CompletableFuture |
| Can be interrupted while waiting | Does not expose the same checked interruption contract |

Avoid blocking unnecessarily in asynchronous pipelines. Prefer composing stages with `thenApply`, `thenCompose`, and related methods.

## 8.14 Use a custom executor for blocking I/O

Async methods without an explicit executor commonly use the shared ForkJoin common pool for async tasks. Blocking database or HTTP operations should generally use an appropriately configured executor rather than consuming the common pool indiscriminately.

```java
ExecutorService ioExecutor =
        Executors.newFixedThreadPool(10);

CompletableFuture<String> response =
        CompletableFuture.supplyAsync(
                () -> callRemoteService(),
                ioExecutor
        );
```

The pool size and queue should be chosen according to the workload and downstream limits.

## 8.15 CompletableFuture and Spring transactions

A Spring transaction is normally associated with the current thread through transaction infrastructure. It does not automatically propagate to a separate asynchronous thread.

Therefore:
- Keep transactional database work within a clearly defined transaction boundary.
- Avoid sharing an EntityManager or Hibernate persistence context across threads.
- Pass IDs or immutable data to asynchronous tasks and perform their own database access through properly managed Spring components.
- Handle partial failures and retries deliberately.

---

# 9. Day 9 — Quick Revision Sheet

## 9.1 One-line definitions

| Concept | Remember |
|---|---|
| Process | Running program with its own virtual address space |
| Thread | Execution path within a process |
| Multithreading | Multiple threads within one process |
| Concurrency | Multiple tasks make progress over overlapping periods |
| Parallelism | Multiple tasks execute at the same instant |
| Race condition | Result depends on timing/interleaving |
| Critical section | Code accessing shared state that needs coordination |
| synchronized | Monitor-based mutual exclusion and visibility |
| volatile | Visibility and ordering for a variable; not compound atomicity |
| AtomicInteger | Atomic operations on an integer |
| CAS | Compare-and-set update technique |
| ReentrantLock | Explicit reentrant lock with extra control |
| Deadlock | Threads wait indefinitely on resources held by one another |
| ExecutorService | Manages execution of submitted tasks |
| Thread pool | Reuses worker threads |
| Future | Represents a result that may become available later |
| CompletableFuture | Composable asynchronous computation |

## 9.2 Important distinctions

| Comparison | Key difference |
|---|---|
| Process vs Thread | Separate process address space vs shared process resources |
| Concurrency vs Parallelism | Overlapping progress vs simultaneous execution |
| `start()` vs `run()` | New thread vs ordinary method call |
| Runnable vs Callable | No result vs result |
| `sleep()` vs `wait()` | Does not release monitor vs releases object's monitor |
| `synchronized` vs `volatile` | Mutual exclusion + visibility vs visibility/order |
| `volatile` vs AtomicInteger | No atomic increment vs atomic increment |
| synchronized vs ReentrantLock | Simpler monitor vs explicit lock controls |
| execute() vs submit() | No Future returned vs Future returned |
| shutdown() vs shutdownNow() | Stop accepting tasks vs attempt to interrupt active tasks |
| thenApply() vs thenCompose() | Transform result vs chain another future |
| get() vs join() | Checked exceptions vs unchecked completion exception |

## 9.3 Five rules worth memorizing

1. Never call `run()` when you intend to start a new thread; call `start()`.
2. `count++` is not atomic, even when the variable is volatile.
3. `sleep()` does not release a monitor; `wait()` releases the monitor on which it is called.
4. Always release an explicitly acquired Lock in a `finally` block.
5. Do not assume Java synchronization or a Spring transaction automatically coordinates work across separate threads or application instances.

---

# 10. Interview Questions and Answers

## Q1. What is the difference between a process and a thread?

**Answer:** A process is a running program with its own virtual address space and process resources. A thread is an execution path inside a process. Threads in the same process share many resources but have separate stacks and execution state.

## Q2. What is multithreading?

**Answer:** Multithreading allows multiple threads within one process to make progress independently. It can improve responsiveness and throughput when tasks can safely run concurrently.

## Q3. What is the difference between concurrency and parallelism?

**Answer:** Concurrency means multiple tasks are in progress during overlapping periods. Parallelism means multiple tasks execute simultaneously on separate processing resources.

## Q4. What is the difference between CPU-bound and I/O-bound tasks?

**Answer:** CPU-bound tasks spend most of their time computing. I/O-bound tasks spend much of their time waiting for operations such as database, network, or file access. Concurrency often helps I/O-bound workloads by allowing other work to progress during waits.

## Q5. How can you create a thread in Java?

**Answer:** You can extend Thread or provide a Runnable task to a Thread. Callable is another task abstraction when a result or checked exception is needed, generally through ExecutorService. In production, executors are usually preferred over creating raw threads for every task.

## Q6. What is the difference between start() and run()?

**Answer:** `start()` starts a new thread of execution and causes that thread to invoke `run()`. Calling `run()` directly is an ordinary method call and executes on the current thread.

## Q7. What happens if you call start() twice on the same Thread object?

**Answer:** It throws `IllegalThreadStateException`. A Thread instance can be started only once.

## Q8. What are the six Java thread states?

**Answer:** NEW, RUNNABLE, BLOCKED, WAITING, TIMED_WAITING, and TERMINATED.

## Q9. What is the difference between sleep() and wait()?

**Answer:** `sleep()` pauses the current thread and does not release monitors it holds. `wait()` is called on an object while holding that object's monitor; it releases that monitor while waiting and reacquires it before returning.

## Q10. Does interrupt() kill a thread?

**Answer:** No. It requests interruption. The thread or task must cooperate by checking interruption or responding to `InterruptedException`.

## Q11. What is a race condition?

**Answer:** It occurs when multiple threads access shared state and the result depends on the timing or interleaving of operations. A non-atomic increment is a common example.

## Q12. Why is count++ not thread-safe?

**Answer:** It consists conceptually of reading the current value, adding one, and writing the result. Two threads can read the same old value and overwrite each other's updates.

## Q13. What does synchronized provide?

**Answer:** It provides mutual exclusion for code protected by the same monitor and establishes visibility guarantees through monitor acquire/release happens-before rules.

## Q14. What is the difference between instance synchronized and static synchronized methods?

**Answer:** An instance synchronized method locks the current object's monitor. A static synchronized method locks the Class object.

## Q15. What is volatile?

**Answer:** Volatile provides visibility and ordering guarantees for reads and writes of a variable across threads. It does not make compound operations such as `count++` atomic.

## Q16. What is AtomicInteger?

**Answer:** AtomicInteger provides atomic operations on an integer, such as increment and compare-and-set, using concurrency primitives such as CAS.

## Q17. What is the difference between synchronized and ReentrantLock?

**Answer:** Both can provide mutual exclusion. ReentrantLock offers explicit control such as tryLock, timed acquisition, and interruptible acquisition, but it must be released explicitly, usually in a finally block.

## Q18. What is deadlock?

**Answer:** Deadlock occurs when threads wait indefinitely for resources held by one another. Consistent lock ordering, fewer nested locks, and smaller critical sections help prevent it.

## Q19. What is ExecutorService?

**Answer:** ExecutorService manages asynchronous task execution. It accepts Runnable or Callable tasks and uses managed worker threads instead of requiring the application to create a new thread for every task.

## Q20. What is the difference between execute() and submit()?

**Answer:** `execute()` submits a Runnable and returns no result. `submit()` accepts Runnable or Callable and returns a Future that can be used to inspect completion or retrieve a result.

## Q21. What is the difference between shutdown() and shutdownNow()?

**Answer:** `shutdown()` stops accepting new tasks but allows submitted tasks to complete. `shutdownNow()` attempts to interrupt active tasks and returns tasks that were waiting to execute; it cannot guarantee immediate termination.

## Q22. What is CompletableFuture?

**Answer:** CompletableFuture represents a result that may become available later and provides methods to transform, combine, compose, and handle asynchronous computations.

## Q23. What is the difference between thenApply() and thenCompose()?

**Answer:** `thenApply()` transforms a result into another value. `thenCompose()` chains an operation that itself returns a CompletableFuture and flattens the nested future.

## Q24. What is the difference between thenCombine() and allOf()?

**Answer:** `thenCombine()` combines the results of two futures using a function. `allOf()` completes when all supplied futures complete but returns a `CompletableFuture<Void>`; individual results must be retrieved from their original futures.

## Q25. Does a Spring transaction automatically propagate to a CompletableFuture task?

**Answer:** No. A Spring transaction is normally bound to the current thread. A task running on another thread does not automatically share that transaction or persistence context. The asynchronous task should have its own properly managed transaction if needed.

---

# 11. Backend Engineering Connections

## 11.1 Concurrent HTTP requests

A Spring Boot web server can process multiple incoming requests concurrently using server-managed request threads.

That does not mean every service method should create additional threads. The server and application need controlled concurrency, connection pools, and resource limits.

## 11.2 Avoid shared mutable state in services

A typical Spring service bean is singleton-scoped by default. That means multiple request threads may call methods on the same service instance.

Avoid request-specific mutable fields:

```java
@Service
public class TripService {
    // Risky: shared mutable request state
    private Long currentTripId;
}
```

Prefer method-local variables and immutable dependencies:

```java
@Service
public class TripService {
    public TripResponse getTrip(Long tripId) {
        // Local variables are isolated to this invocation.
        return null;
    }
}
```

A singleton bean can be safe for concurrent use when it is stateless or its shared state is correctly coordinated.

## 11.3 Database connection pools and thread pools

A backend has multiple resource limits:
- HTTP request-processing threads.
- Application task executor threads.
- Database connection pool size.
- External API rate limits.
- Database capacity.

If an application creates far more database-using threads than available database connections, many threads may simply wait for connections. Increasing thread count blindly can make latency and resource contention worse.

## 11.4 Asynchronous notifications

A user-facing request may need to save a trip assignment and then send a notification.

The design must distinguish:
- Whether the notification is required before the API response.
- Whether it can be sent after the response.
- How failures are retried.
- Whether a failed notification must be persisted for later processing.

`@Async` or CompletableFuture can help with in-process asynchronous work, but they do not by themselves provide durable delivery. For reliable cross-instance event processing, a message broker or transactional outbox pattern may be more suitable.

## 11.5 Cross-instance consistency

Suppose two application instances attempt to assign the same driver simultaneously.

A Java synchronized block inside each instance does not coordinate the two instances. Use database-level safeguards such as:
- Unique constraints.
- Transactions.
- Optimistic locking with `@Version`.
- Pessimistic row locking where justified.

The concurrency mechanism must protect the resource at the level where it is shared.

---

# 12. Completion Checklist

- [ ] Explain process vs thread.
- [ ] Explain multithreading, concurrency, and parallelism.
- [ ] Distinguish CPU-bound and I/O-bound work.
- [ ] Create a thread using Thread and Runnable.
- [ ] Explain why Runnable is often preferred to extending Thread.
- [ ] Distinguish `start()` and `run()`.
- [ ] Explain Runnable vs Callable.
- [ ] Name all six Java thread states.
- [ ] Explain sleep, wait, notify, notifyAll, join, and interrupt.
- [ ] Explain race conditions and why `count++` is not atomic.
- [ ] Explain synchronized instance methods, static methods, and blocks.
- [ ] Explain volatile visibility and its limitations.
- [ ] Explain AtomicInteger and CAS.
- [ ] Compare synchronized and ReentrantLock.
- [ ] Explain deadlock and the four Coffman conditions.
- [ ] Explain ExecutorService, thread pools, execute, submit, and Future.
- [ ] Distinguish shutdown and shutdownNow.
- [ ] Explain CompletableFuture and its main composition methods.
- [ ] Explain why blocking I/O should use a suitable executor.
- [ ] Explain why transactions and persistence contexts do not automatically cross async thread boundaries.
- [ ] Explain why JVM-level synchronization does not coordinate separate application instances.

---

**Day 9 status:** Java Multithreading and Concurrency — all eight planned topics covered.

**Next day:** Day 10 — JVM Internals and Garbage Collection: runtime memory, heap generations, GC roots, collection algorithms, collectors, memory leaks, JVM flags, JIT, and class loading.
'''

path = Path("/mnt/data/Day_09_Java_Multithreading_Concurrency.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
