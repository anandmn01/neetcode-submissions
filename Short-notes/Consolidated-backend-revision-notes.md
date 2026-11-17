from pathlib import Path

content = r'''# Java Backend Interview Preparation — Complete Short Notes (Days 1–15)

> **Purpose:** One compact revision notebook covering the main concepts studied across Days 1–15.  
> **Stack focus:** Java 21, Spring Boot, Spring MVC, Spring Security, Hibernate/JPA, PostgreSQL, Kafka, Docker.  
> **DSA language:** C++ (the Java backend concepts below are language-specific).  
> **How to use:** Read this for quick revision. Use the individual Day 1–15 detailed notes when you need full explanations, examples or implementation detail.

---

## Contents

1. Day 1 — Spring Boot Fundamentals
2. Day 2 — Spring Boot Internals
3. Day 3 — REST APIs and Spring Data JPA
4. Day 4 — Spring Security and JWT
5. Day 5 — Hibernate and JPA Internals
6. Day 6 — Microservices, Kafka and Docker
7. Day 7 — Core Java Fundamentals
8. Day 8 — Java Collections Framework
9. Day 9 — Multithreading and Concurrency
10. Day 10 — JVM Internals and Garbage Collection
11. Day 11 — SQL and DBMS Fundamentals
12. Day 12 — Advanced SQL
13. Day 13 — Generics, Java 8 and Streams
14. Day 14 — Exception Handling
15. Day 15 — SOLID, Design Patterns and REST API Design
16. Cross-topic interview cheat sheet

---

# Day 1 — Spring Boot Fundamentals

## Spring vs Spring Boot

- **Spring Framework:** Provides core infrastructure such as IoC, DI, AOP, MVC and transaction management.
- **Spring Boot:** Simplifies Spring application setup using auto-configuration, starter dependencies and embedded servers.
- **Starter dependencies:** Convenient dependency bundles, e.g. `spring-boot-starter-web`.
- **Auto-configuration:** Configures beans based on classpath, properties and existing beans.
- **Embedded server:** Run a web application without separately deploying to an external Tomcat installation.

## Layered architecture

```text
Client
  |
Controller  -> handles HTTP request/response
  |
Service     -> business logic
  |
Repository  -> data access
  |
Database
```

- **Entity:** Represents persisted data.
- **DTO:** Defines data transferred through an API.
- Keep controllers thin and business rules in services.

## IoC, DI and beans

- **IoC:** The Spring container manages object creation and lifecycle instead of application code constructing every dependency.
- **DI:** Dependencies are supplied to an object by the container.
- **ApplicationContext:** Main Spring container; manages beans and provides additional enterprise features.
- **Constructor injection:** Preferred for required dependencies; supports `final` fields and easier testing.
- **Setter injection:** Useful for optional dependencies.
- **Field injection:** Generally avoid; less explicit and harder to test.

## Stereotype annotations

| Annotation | Purpose |
|---|---|
| `@Component` | Generic Spring-managed component |
| `@Service` | Service/business layer |
| `@Repository` | Persistence layer; supports exception translation |
| `@Controller` | MVC controller |
| `@RestController` | Controller whose return values are response bodies |

- `@Bean`: Declares a bean from a method, often for third-party classes.
- `@Configuration`: Class containing bean definitions.
- `@Component` is discovered through scanning; `@Bean` explicitly registers the method's returned object.

## Bean selection, lifecycle and scope

- `@Primary`: Preferred bean when multiple candidates exist.
- `@Qualifier`: Selects a specific bean.
- Lifecycle overview: definition → instantiation → dependency injection → aware callbacks/post-processors → initialization → ready → destruction.
- `@PostConstruct` / `@PreDestroy`: Initialization and cleanup callbacks.
- Common scopes: singleton, prototype, request, session, application, WebSocket.
- Spring singleton means one instance per IoC container; it does **not** automatically mean thread-safe.

---

# Day 2 — Spring Boot Internals

## Startup and container

- `SpringApplication.run()` bootstraps the application, prepares the environment, creates the ApplicationContext, registers/refreshes beans and starts the web server.
- `ApplicationContext` manages beans and application infrastructure.
- `BeanFactory` is the core bean container abstraction; `ApplicationContext` builds on it.

## Core annotations

```java
@SpringBootApplication
```

Combines:
- `@Configuration`
- `@EnableAutoConfiguration`
- `@ComponentScan`

## Auto-configuration

- Configures common beans based on available classes, properties and existing beans.
- Conditional annotations include:
  - `@ConditionalOnClass`
  - `@ConditionalOnMissingBean`
  - `@ConditionalOnProperty`
- User-defined beans can cause default auto-configuration to back off.
- Starters provide dependencies; auto-configuration configures beans. They are related but not the same.

## Configuration

- `application.properties` and `application.yml` hold configuration.
- `@Value` injects individual values.
- `@ConfigurationProperties` binds grouped, type-safe configuration.
- External configuration can come from command-line arguments, environment variables, files and other property sources.
- Profiles such as `dev`, `test`, `prod` select environment-specific configuration.
- Never commit secrets; use environment-specific secret management.

## Operational features

- Logging: use levels such as TRACE, DEBUG, INFO, WARN, ERROR.
- Actuator exposes operational endpoints such as health and metrics; secure sensitive endpoints.
- `CommandLineRunner` and `ApplicationRunner` execute after startup.
- Application events communicate lifecycle events within the application.
- Graceful shutdown allows in-flight work to finish within configured limits.
- `--debug` can help inspect auto-configuration condition reports.
- `ApplicationReadyEvent` indicates the application is ready to service requests.

---

# Day 3 — REST APIs and Spring Data JPA

## REST and HTTP

- REST models resources and uses a uniform HTTP interface.
- Common methods: GET (read), POST (create/process), PUT (replace), PATCH (partial update), DELETE (remove).
- `@RestController` defines REST endpoints.
- `@GetMapping`, `@PostMapping`, `@PutMapping`, `@PatchMapping`, `@DeleteMapping` map requests.
- `@RequestBody` converts JSON into a Java request object.
- `@PathVariable` reads a path segment; `@RequestParam` reads query parameters.
- `ResponseEntity` controls status, headers and body.

## Request flow

```text
HTTP -> Controller -> Service -> Repository -> Database
                  <- DTO       <- Entity/query result
```

- Request DTO: incoming fields and validation.
- Entity: persistence representation.
- Response DTO: fields returned to the client.
- Do not expose passwords or internal entity fields in API responses.

## Validation and exceptions

- `@Valid` triggers request validation.
- Common constraints: `@NotBlank`, `@NotNull`, `@Email`, `@Size`, `@Min`, `@Max`.
- `@ExceptionHandler` handles selected exceptions.
- `@RestControllerAdvice` centralizes controller exception handling.
- Common statuses: 200, 201, 204, 400, 401, 403, 404, 409, 500.

## Spring Data JPA

- JPA is a persistence specification; Hibernate is a common JPA implementation; Spring Data JPA simplifies repository access.
- `JpaRepository<Entity, ID>` provides CRUD, pagination and sorting methods.
- Derived queries: `findByEmail(...)`.
- `@Query` supports JPQL or native SQL.
- `Pageable`, `Page`, `Slice`, `Sort` support result navigation and ordering.
- `@Transactional` defines transaction boundaries; default rollback is for unchecked exceptions and `Error`.

## Relationships

- `@OneToOne`, `@OneToMany`, `@ManyToOne`, `@ManyToMany`.
- `mappedBy` marks the inverse side; the owning side controls the relationship mapping.
- `@JoinColumn` maps a foreign key.
- Keep both sides of bidirectional relationships synchronized in helper methods.
- Use DTOs to avoid exposing entity relationships and recursive JSON serialization.

---

# Day 4 — Spring Security and JWT

## Authentication vs authorization

- **Authentication:** Who are you?
- **Authorization:** What are you allowed to do?

## Security architecture

Typical concepts:
- `SecurityFilterChain`
- `FilterChainProxy`
- `AuthenticationManager`
- `AuthenticationProvider`
- `UserDetailsService`
- `PasswordEncoder`
- `SecurityContextHolder`

## Passwords and JWT

- Store password hashes, never plaintext passwords.
- BCrypt is a password-hashing algorithm designed for password storage.
- JWT commonly contains header, payload and signature.
- A signed JWT is not automatically encrypted; do not put secrets in its payload.
- Access token: short-lived credential for API access.
- Refresh token: used to obtain a new access token; protect, rotate and revoke according to design.
- `OncePerRequestFilter` is commonly used for JWT request filtering.
- Validate signature, expiry and relevant claims.

## Authorization and web security

- `hasRole("ADMIN")` typically checks authority `ROLE_ADMIN`.
- `hasAuthority("...")` checks the exact authority.
- Method security can use `@PreAuthorize`.
- 401: authentication missing/invalid.
- 403: authenticated but not authorized.
- CORS controls browser cross-origin access; it is not authentication.
- CSRF protects against certain cross-site request forgery scenarios; assess based on browser credentials and architecture.
- Secure secrets, use HTTPS, least privilege and restrict Actuator endpoints.

---

# Day 5 — Hibernate and JPA Internals

## JPA, Hibernate and persistence context

- JPA: specification/API.
- Hibernate: ORM implementation.
- Spring Data JPA: repository abstraction.
- Entity states: transient, managed/persistent, detached, removed.
- Persistence Context: first-level cache and identity map associated with an EntityManager.
- Dirty checking: Hibernate detects changes to managed entities and writes updates during flush.
- `merge()` copies detached state into a managed instance; use the returned instance.
- `flush()` synchronizes persistence-context changes with the database; it does not commit the transaction.
- Commit completes the transaction.

## Fetching and relationships

- LAZY loads associated data when accessed; EAGER requests eager loading.
- N+1: one query loads parent rows, then additional queries load related data.
- Possible remedies: `JOIN FETCH`, `@EntityGraph`, batch fetching and DTO projections.
- Avoid assuming EAGER always solves N+1.
- Cascade controls operation propagation; `orphanRemoval` deletes removed children from the relationship.
- Use cascade REMOVE carefully.

## Performance and consistency

- L1 cache is associated with the persistence context; L2 cache is optional and provider/configuration dependent.
- JDBC batching can reduce database round trips; `saveAll()` alone does not guarantee batching.
- Indexes speed suitable reads but add storage and write overhead.
- Use `EXPLAIN` / `EXPLAIN ANALYZE` to inspect PostgreSQL query plans.
- `@Version` supports optimistic locking.
- Pessimistic locking can lock rows during a transaction.
- HikariCP is a common JDBC connection pool.
- Use Flyway or Liquibase for schema migrations; production schema management should be deliberate.
- JPA auditing can populate created/modified metadata.

## ACID

- Atomicity: all or nothing.
- Consistency: valid state before and after.
- Isolation: concurrent transactions are controlled.
- Durability: committed changes persist.
- PostgreSQL default isolation is READ COMMITTED.

---

# Day 6 — Microservices, Kafka and Docker

## Architecture

- Monolith: application deployed as one unit.
- Microservices: independently deployable services organized around business capabilities.
- Microservices add operational complexity: networking, observability, data consistency, deployment and security.
- Do not split a system into services without a clear reason.

## Communication and infrastructure

- Synchronous: caller waits for response (e.g. REST, OpenFeign).
- Asynchronous: sender publishes work/event without waiting for full processing.
- Service discovery: services locate service instances, e.g. Eureka.
- API Gateway: common entry point for routing and cross-cutting concerns.
- Load balancing distributes traffic among instances.
- Centralized configuration stores shared service configuration.
- Circuit breaker prevents repeated calls to a failing dependency.
- Retry, timeout and fallback must be designed carefully; retries can amplify load.

## Kafka

- Kafka is a distributed event-streaming platform.
- Producer writes records to topics.
- Topic is divided into partitions.
- Broker stores partitions and replicas.
- Consumer reads records and tracks offsets.
- Consumers in the same group divide partition work.
- Ordering is guaranteed within a partition, not globally across a topic.
- Delivery semantics include at-most-once, at-least-once and exactly-once processing features; end-to-end exactly-once requires careful design.
- Make consumers idempotent where duplicate delivery is possible.
- Event-driven architecture communicates through events.

## Distributed systems patterns

- Saga: coordinates a multi-service business transaction through steps and compensating actions.
- Choreography: services react to events.
- Orchestration: a coordinator directs the workflow.
- Distributed tracing and correlation IDs connect logs across services.
- Docker packages an application and its runtime dependencies into an image.
- A container is a running instance of an image.
- Use secure images, configuration, health checks and resource limits.

---

# Day 7 — Core Java Fundamentals

## Java platform

- JDK: development tools plus runtime components.
- JRE: conceptual runtime environment (JVM plus libraries); modern JDK distributions may not ship a separate JRE.
- JVM: executes Java bytecode.
- Flow: `.java` → `javac` → `.class` bytecode → class loading/verification → execution by interpreter/JIT.

## JVM basics

- Class loaders: Bootstrap, Platform, Application.
- Runtime areas include heap, Java stacks, PC registers, native method stacks and method-area-related metadata.
- JIT compiles frequently executed code to native machine code.
- GC reclaims unreachable objects.

## OOP

- Encapsulation: hide internal state and expose controlled operations.
- Inheritance: derive behavior from a parent type.
- Polymorphism: same abstraction, different implementations.
- Abstraction: expose essential behavior, hide implementation details.
- Constructors initialize objects; if no constructor is declared, Java provides a default no-argument constructor.
- `this` refers to the current object; `super` refers to parent-class members/constructor.

## Equality and strings

- `==`: primitive value comparison or reference identity.
- `.equals()`: logical equality as implemented by a class.
- Equal objects must have equal `hashCode()` values.
- Override `equals()` and `hashCode()` consistently.
- String is immutable; StringBuilder is mutable and not synchronized; StringBuffer is synchronized.
- String literals may be shared through the string pool.

---

# Day 8 — Java Collections Framework

## Core interfaces

- `Iterable` → `Collection` → List / Set / Queue.
- `Map` belongs to the Collections Framework but does not extend `Collection`.
- `Collections` is a utility class; `Collection` is an interface.

## Main implementations

| Type | Characteristics | Typical complexity |
|---|---|---|
| ArrayList | Resizable array, indexed access | Get O(1), middle insert/delete O(n) |
| LinkedList | Doubly linked nodes | Indexed access O(n) |
| HashSet | Unique elements, no guaranteed order | Average add/contains O(1) |
| LinkedHashSet | Unique elements, insertion order | Average O(1) |
| TreeSet | Sorted set | O(log n) |
| HashMap | Key-value, no guaranteed order | Average get/put O(1) |
| TreeMap | Sorted keys | O(log n) |
| PriorityQueue | Heap-based priority ordering | Peek O(1), insertion/removal O(log n) |
| ArrayDeque | Double-ended queue | Efficient operations at both ends |

## HashMap internals

- Uses hash values to locate buckets.
- Uses `equals()` to identify matching keys.
- Collisions occur when keys map to the same bucket.
- Modern OpenJDK HashMap can treeify heavily collided buckets under certain capacity/threshold conditions.
- Default initial capacity is 16 when allocated; default load factor is 0.75.
- HashMap is not thread-safe and allows one null key and null values.
- ConcurrentHashMap supports concurrent access and disallows null keys/values.

## Other concepts

- Comparable: natural ordering via `compareTo`.
- Comparator: external/custom ordering via `compare`.
- Iterator: `hasNext()`, `next()`, optional `remove()`.
- Fail-fast behavior is best-effort, not a thread-safety guarantee.
- ArrayDeque is generally preferred over legacy Stack for stack operations.

---

# Day 9 — Multithreading and Concurrency

## Fundamentals

- Process: running program with its own address space.
- Thread: execution path within a process; threads share process memory but have their own stacks.
- Concurrency: tasks make progress in overlapping periods.
- Parallelism: tasks execute simultaneously on multiple processing units.

## Threads and lifecycle

- Create tasks with `Runnable` or `Callable`; use `Thread` or an executor to execute.
- `start()` starts a new thread; calling `run()` directly is an ordinary method call.
- States: NEW, RUNNABLE, BLOCKED, WAITING, TIMED_WAITING, TERMINATED.
- `sleep()` does not release a monitor.
- `wait()` releases the monitor of the object on which it is called.
- `join()` waits for another thread.
- `interrupt()` signals interruption; it does not forcibly kill a thread.

## Thread safety

- Race condition: result depends on timing/interleaving.
- `synchronized`: mutual exclusion and visibility through monitor rules.
- `volatile`: visibility/order guarantees, but not atomic compound updates.
- Atomic classes use atomic operations such as compare-and-set.
- ReentrantLock offers explicit locking features; unlock in `finally`.
- Deadlock prevention: consistent lock ordering, small critical sections, avoid unnecessary nested locks.

## Executors and async

- ExecutorService manages worker threads.
- `execute()` submits Runnable without a Future result.
- `submit()` returns Future.
- Callable returns a result and can throw checked exceptions.
- `shutdown()` stops accepting new tasks; `shutdownNow()` attempts interruption.
- CompletableFuture supports async composition: `thenApply`, `thenCompose`, `thenCombine`, `allOf`, `exceptionally`, `handle`.
- Use an appropriate bounded/configured executor for production work.
- Thread-local transaction context does not automatically propagate to another async thread.

---

# Day 10 — JVM Internals and Garbage Collection

## Runtime memory

- Heap: objects and arrays.
- Java stack: method frames, local variables and call state per thread.
- Metaspace: class metadata in native memory.
- PC register: current instruction position per thread.
- Native method stack: native execution support.

## Garbage collection

- GC reclaims objects that are no longer reachable from GC roots.
- Circular references can still be collected if unreachable.
- GC roots include active thread stacks, static references and other JVM roots.
- Algorithms: mark-sweep, mark-compact, copying and generational collection.
- Collectors include Serial, Parallel, G1, ZGC and Shenandoah.
- Collector choice balances throughput, pause time and memory.
- `System.gc()` is only a request/hint, not a guarantee.

## Memory problems and tuning

- `OutOfMemoryError`: JVM cannot allocate required memory.
- `StackOverflowError`: thread stack exhausted, often due to deep/unbounded recursion.
- Memory leak: objects remain reachable unintentionally.
- Useful flags: `-Xms`, `-Xmx`, `-Xss`, GC logging options.
- Heap dumps and GC logs help diagnose memory issues.
- JIT uses runtime profiling/compilation to optimize hot code.
- Class loading includes loading, linking (verification, preparation, resolution) and initialization.

---

# Day 11 — SQL and DBMS Fundamentals

## DBMS and SQL

- DBMS manages data; RDBMS organizes data in related tables.
- PostgreSQL is an RDBMS.
- JDBC is Java's database connectivity API; Hibernate/JPA use JDBC under the hood.
- Schema groups database objects.

## SQL command categories

| Category | Commands |
|---|---|
| DDL | CREATE, ALTER, DROP, TRUNCATE |
| DML | INSERT, UPDATE, DELETE |
| DQL | SELECT |
| DCL | GRANT, REVOKE |
| TCL | BEGIN, COMMIT, ROLLBACK, SAVEPOINT |

- DELETE removes selected rows.
- TRUNCATE removes all rows efficiently; PostgreSQL supports transactional TRUNCATE.
- DROP removes the database object.

## Keys and constraints

- Primary key uniquely identifies a row and cannot be NULL.
- Foreign key enforces referential integrity.
- UNIQUE prevents duplicate values according to database NULL semantics.
- NOT NULL, CHECK and DEFAULT enforce data rules.
- Composite key uses multiple columns.
- `ON DELETE CASCADE` propagates parent deletion to related rows.

## Joins and queries

- INNER JOIN: matching rows.
- LEFT JOIN: all left rows plus matches.
- RIGHT JOIN: all right rows plus matches.
- FULL OUTER JOIN: rows from both sides, matched where possible.
- CROSS JOIN: Cartesian product.
- SELF JOIN: table joined to itself.
- Subqueries can be scalar, multi-row or correlated.
- `EXISTS` tests whether rows exist.
- `NOT IN` can behave unexpectedly when the subquery returns NULL.
- CTE (`WITH`) makes query structure easier to organize; it is not automatically faster.

## Indexing and normalization

- Indexes improve suitable reads but cost storage and write work.
- PostgreSQL commonly uses B-tree indexes.
- Composite indexes depend on column order and query predicates.
- Use `EXPLAIN ANALYZE` to inspect actual execution.
- 1NF: atomic values/no repeating groups.
- 2NF: no partial dependency on part of a candidate key.
- 3NF: remove problematic transitive dependencies.
- BCNF: every determinant is a superkey.

## Transactions

ACID: Atomicity, Consistency, Isolation, Durability.

Isolation phenomena:
- Dirty read: reading uncommitted data.
- Non-repeatable read: same row read gives different committed values.
- Phantom read: repeated predicate query returns a changed row set.
- PostgreSQL default: READ COMMITTED.
- PostgreSQL REPEATABLE READ uses a stable snapshot and prevents phantom reads in PostgreSQL's implementation.
- SERIALIZABLE may abort transactions; retry may be needed.

---

# Day 12 — Advanced SQL

## Aggregate functions

- `COUNT`, `SUM`, `AVG`, `MIN`, `MAX`.
- `COUNT(*)` counts rows; `COUNT(column)` ignores NULL values.
- Aggregates generally ignore NULL inputs except `COUNT(*)`.
- `COALESCE(value, fallback)` returns the first non-NULL value.

## GROUP BY and HAVING

- GROUP BY forms groups for aggregate calculations.
- Every selected non-aggregated column generally must be grouped.
- WHERE filters rows before grouping.
- HAVING filters groups after aggregation.

## Window functions

Window functions calculate across related rows while retaining individual rows.

```sql
SELECT
    employee_id,
    department_id,
    salary,
    RANK() OVER (
        PARTITION BY department_id
        ORDER BY salary DESC
    ) AS salary_rank
FROM employees;
```

- `OVER`: defines the window.
- `PARTITION BY`: divides rows into groups.
- `ORDER BY`: defines order inside the window.
- `ROW_NUMBER`: unique sequence number.
- `RANK`: ties share rank and leave gaps.
- `DENSE_RANK`: ties share rank without gaps.
- `LAG` / `LEAD`: access previous/next row values.
- Running totals use an aggregate with an explicit window frame.

## Other SQL objects

- View: stored query definition.
- Materialized view: stores query results; must be refreshed.
- Function: reusable database routine that returns a value/table.
- Procedure: invoked with `CALL`; transaction control capabilities differ from functions.
- Trigger: executes in response to table events; PostgreSQL trigger functions use `OLD` and `NEW` where applicable.
- Avoid moving complex business logic into triggers without a clear reason.

---

# Day 13 — Generics, Java 8 and Streams

## Generics

- Generics provide compile-time type safety and reduce casts.
- Generic class: `class Box<T>`.
- Generic method: `<T> void print(T value)`.
- `?` is an unknown type.
- `? extends T`: producer/read-oriented upper bound.
- `? super T`: consumer/write-oriented lower bound.
- PECS: Producer Extends, Consumer Super.
- Type erasure means generic type information is mostly erased at runtime.

## Java 8 functional programming

- Lambda: concise implementation of a functional interface.
- Functional interface: one abstract method (default/static methods may also exist).
- Predicate: takes input, returns boolean.
- Function: transforms input to output.
- Consumer: accepts input, returns no result.
- Supplier: supplies a value.

## Streams

Typical pipeline:

```text
Source -> Intermediate operations -> Terminal operation
```

- Intermediate operations: `filter`, `map`, `sorted`, `distinct`, `flatMap`.
- Terminal operations: `collect`, `toList`, `count`, `forEach`, `reduce`.
- Streams are lazy until a terminal operation and cannot be reused after consumption.
- `flatMap` flattens nested streams.
- `groupingBy` groups items.
- `partitioningBy` splits into true/false groups.
- `reduce` combines elements into a result.
- Prefer database filtering/aggregation for large persisted datasets when appropriate.

## Optional and method references

- Optional represents a value that may be absent.
- Useful methods: `of`, `ofNullable`, `empty`, `map`, `filter`, `orElse`, `orElseGet`, `orElseThrow`, `ifPresent`.
- `orElse` evaluates its fallback argument eagerly; `orElseGet` evaluates the supplier only when empty.
- Method-reference forms: static method, bound instance method, unbound instance method, constructor reference.

---

# Day 14 — Exception Handling

## Exception hierarchy

```text
Throwable
├── Error
└── Exception
    ├── Checked exceptions
    └── RuntimeException (unchecked)
```

- Exception: condition application code may handle.
- Error: serious JVM/environment problem; generally not handled as normal business flow.
- Checked: must be caught or declared.
- Unchecked: RuntimeException subclasses and Error; no compiler requirement to catch/declare.

Examples:
- Checked: IOException, SQLException, ClassNotFoundException.
- Unchecked: NullPointerException, IllegalArgumentException, NumberFormatException, ArithmeticException.

## Handling keywords

- `try`: risky code.
- `catch`: handles matching exception.
- `finally`: normally runs for cleanup; not an absolute guarantee if the process/JVM terminates.
- `throw`: explicitly throws an exception object.
- `throws`: declares possible exceptions in method signature.
- Try-with-resources automatically closes `AutoCloseable` resources.

## Custom exceptions and propagation

- Extend `RuntimeException` for common unchecked business exceptions.
- Extend `Exception` for a custom checked exception.
- Preserve causes using `super(message, cause)`.
- Unhandled exceptions propagate up the method call stack until handled.
- Avoid swallowing exceptions, returning null after failures, or using exceptions for routine branching.
- Catch specific exceptions and log at an appropriate boundary.
- Do not expose stack traces, SQL details, passwords or tokens to clients.

## Spring Boot exception handling

- `@ExceptionHandler`: handles selected exception types.
- `@ControllerAdvice`: shared controller advice.
- `@RestControllerAdvice`: centralized exception handling with response-body serialization.
- `ResponseEntity`: status, headers and body.
- `@Valid` triggers request validation; `MethodArgumentNotValidException` is common for invalid request bodies.
- Spring Security filter failures may need `AuthenticationEntryPoint` (401) and `AccessDeniedHandler` (403); they are not ordinary controller exceptions.
- `@Transactional` defaults to rollback for unchecked exceptions and Error, not checked exceptions; use `rollbackFor` when required.

## Common status mapping

| Status | Meaning |
|---:|---|
| 400 | Invalid request |
| 401 | Missing/invalid authentication |
| 403 | Authenticated but forbidden |
| 404 | Resource not found |
| 409 | Conflict with current state |
| 500 | Unexpected server failure |
| 502/503/504 | Upstream/service availability or timeout failures, according to contract |

Production error responses should be consistent and safe. Log detailed diagnostics server-side and return a client-safe message with a correlation ID when appropriate.

---

# Day 15 — SOLID, Design Patterns and REST API Design

## SOLID

| Principle | Meaning |
|---|---|
| SRP | One cohesive responsibility per class |
| OCP | Open for extension, closed to repeated modification |
| LSP | Subtypes preserve parent behavioral contracts |
| ISP | Small, focused interfaces |
| DIP | High-level and low-level modules depend on abstractions |

- Dependency Injection is a technique; Dependency Inversion is a design principle.
- Spring DI supports programming to interfaces.
- Do not create interfaces without a meaningful design benefit.

## Design patterns

A design pattern is a reusable approach to a recurring design problem.

| Category | Patterns | Purpose |
|---|---|---|
| Creational | Singleton | Shared instance within a defined scope |
| Creational | Factory Method | Delegate object creation |
| Creational | Builder | Construct complex objects step by step |
| Creational | Abstract Factory | Create related families of objects |
| Structural | Adapter | Convert incompatible interfaces |
| Structural | Decorator | Add behavior by wrapping |
| Structural | Facade | Simplify a complex subsystem |
| Structural | Proxy | Control/intercept access |
| Behavioral | Strategy | Interchangeable algorithms |
| Behavioral | Observer | Notify subscribers of events |
| Behavioral | Template Method | Fixed algorithm skeleton with customizable steps |
| Behavioral | Chain of Responsibility | Pass requests through handlers |

## Patterns in Spring

- Repository: separates persistence access.
- Proxy: used for features such as `@Transactional`.
- Front Controller: `DispatcherServlet`.
- Chain of Responsibility style: servlet/Spring Security filter chains.
- Observer/publish-subscribe style: Spring application events.
- IoC/DI: container manages object creation and wiring.
- Spring singleton bean scope is per IoC container; mutable singleton beans still need thread-safety consideration.

## REST API design

- Model resources with nouns: `/api/employees`, `/api/trips`.
- GET retrieves; POST creates/processes; PUT replaces; PATCH partially updates; DELETE removes.
- Safe methods should not change server state.
- Idempotent means repeated requests have the same intended effect.
- GET, PUT and DELETE are defined as idempotent; POST is not generally idempotent.
- Use DTOs to separate API contracts from entities and protect sensitive fields.
- Use pagination, sorting and filtering for collection endpoints.
- Validate sort fields and page sizes.
- Version APIs for incompatible changes.
- Idempotency keys can prevent duplicate processing when clients retry operations, but server-side storage/enforcement is required.
- Authentication identifies caller; authorization controls permitted actions.
- CORS is a browser cross-origin policy, not authentication.
- OpenAPI documents endpoints, schemas, parameters, security and responses.

---

# Cross-topic Interview Cheat Sheet

## Frequently compared concepts

| Comparison | Key difference |
|---|---|
| Spring vs Spring Boot | Framework vs opinionated setup/auto-configuration layer |
| IoC vs DI | Container control principle vs dependency-supplying technique |
| `@Component` vs `@Bean` | Component scanning vs explicit bean factory method |
| JPA vs Hibernate | Specification vs implementation |
| `save()` vs `flush()` | Persist/merge operation vs synchronize pending SQL |
| Flush vs commit | Send pending changes to DB vs complete transaction |
| Lazy vs eager | Load on access vs request eager loading |
| Authentication vs authorization | Identity vs permission |
| 401 vs 403 | Not authenticated vs not allowed |
| Checked vs unchecked | Compiler requires catch/declare vs does not |
| `throw` vs `throws` | Throw object vs declare possible exception |
| `==` vs `equals()` | Reference identity/value comparison vs logical equality |
| ArrayList vs LinkedList | Dynamic array vs doubly linked nodes |
| HashMap vs ConcurrentHashMap | Not thread-safe vs concurrent access support |
| `synchronized` vs `volatile` | Mutual exclusion/visibility vs visibility/order, not compound atomicity |
| Runnable vs Callable | No result vs result/checked exception |
| `execute()` vs `submit()` | No Future result vs Future |
| Process vs thread | Separate process address space vs execution path sharing process resources |
| Heap vs stack | Objects/arrays vs per-thread method frames |
| WHERE vs HAVING | Filter rows vs filter groups |
| GROUP BY vs window function | Collapse into groups vs retain individual rows |
| POST vs PUT | Create/process, not generally idempotent vs replacement, idempotent |
| PUT vs PATCH | Replace representation vs partial modification |
| Page vs Slice | Includes total-count metadata vs can avoid total count |
| Monolith vs microservices | Single deployable unit vs independently deployable services |

## HTTP status reminders

- 200: OK
- 201: Created
- 204: No Content
- 400: Bad Request
- 401: Unauthorized (authentication required/invalid)
- 403: Forbidden
- 404: Not Found
- 409: Conflict
- 429: Too Many Requests
- 500: Internal Server Error
- 502: Bad Gateway
- 503: Service Unavailable
- 504: Gateway Timeout

## Core architecture flow to remember

```text
Flutter / Web Client
        |
        | HTTP + JSON + access token
        v
Spring Security Filter Chain
        |
        v
DispatcherServlet
        |
        v
Controller
        |
        | Request DTO / validation
        v
Service
        |
        | Business logic / transaction
        v
Repository
        |
        | JPA / Hibernate / JDBC
        v
PostgreSQL
```

Response flows back through service/controller as a response DTO and HTTP status. Exceptions raised during MVC processing can be translated by centralized advice; security filter failures use security-specific handlers.

---

## Final revision strategy

1. First revise this short notebook to refresh terminology and relationships.
2. Open the relevant detailed Day 1–15 file whenever a topic is unclear.
3. Explain each concept aloud using a small Java or backend example.
4. For patterns and principles, explain the problem first, then the solution and trade-off.
5. For backend topics, trace a request from the client through controller, service, repository and database.
6. Keep DSA practice separate; your DSA language is C++, while these notes focus on Java backend concepts.

**End of consolidated Days 1–15 short notes.**
'''

out = Path("/mnt/data/Java_Backend_Revision_Short_Notes_Days_01_to_15.md")
out.write_text(content, encoding="utf-8")
print(f"Created: {out}")
print(f"Words: {len(content.split()):,}")
print(f"Lines: {len(content.splitlines()):,}")
print(f"Characters: {len(content):,}")
