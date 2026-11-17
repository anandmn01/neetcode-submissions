from pathlib import Path

content = r'''# Day 05 — Hibernate and JPA Internals

> **Learning track:** Java Backend Developer Interview Preparation  
> **Day:** 05  
> **Focus:** Understanding how JPA and Hibernate manage entities, SQL, relationships, transactions, and database performance  
> **Examples:** Employee Transport backend  
> **Stack assumed:** Java 21, Spring Boot 4.x, Hibernate 7.x, Jakarta Persistence, PostgreSQL

---

## Learning objectives

By the end of this day, you should be able to explain:

1. The difference between JPA, Hibernate, and Spring Data JPA.
2. The lifecycle states of a JPA entity.
3. How the persistence context and first-level cache work.
4. Dirty checking, flushing, and committing.
5. First-level and second-level caching.
6. Lazy and eager loading, including the N+1 query problem.
7. Relationship ownership, cascading, and orphan removal.
8. Entity equality and `equals()` / `hashCode()` concerns.
9. JDBC batching and database indexing.
10. Transaction isolation and locking.
11. Connection pooling and query projections.
12. Database migrations, auditing, and constraints.

---

# 1. JPA vs Hibernate vs Spring Data JPA

These three technologies are related, but they are not the same thing.

## 1.1 JPA

JPA (Jakarta Persistence) is a standard API/specification for persistence in Java applications.

It defines concepts and contracts such as:

- `@Entity`
- `@Id`
- `@OneToMany`
- `EntityManager`
- Persistence context
- Entity lifecycle operations

JPA describes how an application can map and manage Java objects in a relational database. It does not, by itself, provide the complete ORM implementation.

## 1.2 Hibernate

Hibernate is an ORM framework and a popular implementation of the Jakarta Persistence specification.

It performs work such as:

- Mapping entities to database tables.
- Generating SQL.
- Managing entity state.
- Tracking changes to managed objects.
- Managing fetching and relationships.
- Coordinating with JDBC and the database.

## 1.3 Spring Data JPA

Spring Data JPA is a Spring abstraction that simplifies repository-based data access.

Instead of writing a DAO implementation for every entity, you can declare an interface:

```java
public interface EmployeeRepository
        extends JpaRepository<Employee, Long> {
}
```

Spring Data JPA creates the repository implementation at runtime and delegates persistence work through JPA.

## 1.4 Comparison

| Technology | Responsibility |
|---|---|
| Jakarta Persistence (JPA) | Persistence specification and API |
| Hibernate | ORM implementation and persistence provider |
| Spring Data JPA | Repository abstraction built on JPA |

A simple way to remember:

```text
Your Service
    |
    v
Spring Data JPA Repository
    |
    v
JPA EntityManager API
    |
    v
Hibernate Provider
    |
    v
JDBC
    |
    v
PostgreSQL
```

**Interview answer:** JPA defines the persistence contract, Hibernate implements ORM behavior, and Spring Data JPA simplifies repository development on top of JPA.

---

# 2. Entity Lifecycle States

A JPA entity can be in different states during its lifecycle.

The four important states are:

1. Transient
2. Managed (Persistent)
3. Detached
4. Removed

## 2.1 Transient state

An object is transient when it has been created using Java code but is not currently associated with a persistence context.

```java
Employee employee = new Employee();
employee.setName("Rahul");
```

At this point, simply creating the object does not save it to the database.

## 2.2 Managed state

An entity is managed when it is associated with an active persistence context.

```java
Employee employee = new Employee();
employee.setName("Rahul");

entityManager.persist(employee);
```

The persistence provider now manages the entity. Its changes can be detected and synchronized with the database.

Entities loaded through an `EntityManager` are generally managed while they remain associated with that persistence context.

## 2.3 Detached state

An entity becomes detached when it is no longer associated with its persistence context.

For example:

```java
Employee employee = entityManager.find(Employee.class, 10L);

entityManager.detach(employee);
```

Changes made to the detached object are not automatically tracked by that persistence context.

## 2.4 Removed state

An entity enters the removed state when it is scheduled for deletion through the persistence context.

```java
Employee employee = entityManager.find(Employee.class, 10L);

entityManager.remove(employee);
```

The deletion is synchronized with the database when the persistence context is flushed.

## 2.5 Lifecycle diagram

```text
       new Employee()
              |
              v
          TRANSIENT
              |
          persist()
              |
              v
          MANAGED
          /      \
         /        \
    detach()     remove()
       |            |
       v            v
   DETACHED       REMOVED
       |
     merge()
       |
       v
   MANAGED
```

`merge()` copies the state of a detached object into a managed instance. It returns the managed instance; the original detached object does not become managed again.

## 2.6 Important EntityManager operations

| Method | Purpose |
|---|---|
| `persist(entity)` | Makes a new entity managed |
| `find(type, id)` | Finds an entity by primary key |
| `detach(entity)` | Detaches one entity |
| `clear()` | Detaches all entities in the persistence context |
| `merge(entity)` | Copies state into a managed instance and returns it |
| `remove(entity)` | Marks a managed entity for deletion |
| `flush()` | Synchronizes pending changes with the database |

---

# 3. Dirty Checking

Dirty checking is a major Hibernate feature.

It means Hibernate tracks managed entities and detects changes to their persistent state.

## 3.1 Example

```java
@Transactional
public void updateEmployee(Long id) {

    Employee employee = employeeRepository.findById(id)
            .orElseThrow();

    employee.setName("Rahul Kumar");

    // No explicit repository.save(employee) is required
    // for a managed entity in this transaction.
}
```

Why can this work?

1. The repository loads the employee.
2. Hibernate associates it with the persistence context.
3. The service changes the entity's name.
4. Hibernate detects that the managed entity changed.
5. During flush, Hibernate issues the required SQL UPDATE.
6. The transaction commits.

## 3.2 Important point

Dirty checking works for managed entities.

If an entity is detached, modifying it does not cause the old persistence context to update the database automatically.

## 3.3 Why is this useful?

- Less repetitive update code.
- Hibernate can determine which managed entities changed.
- Changes can be synchronized together at flush time.
- It supports the Unit of Work pattern.

Do not assume dirty checking means Hibernate executes SQL immediately after every setter call. SQL execution is commonly deferred until flush or commit.

---

# 4. Persistence Context

The persistence context is a central JPA concept.

It is a set of managed entity instances associated with an `EntityManager`.

Hibernate uses it to manage entity identity, changes, and synchronization.

## 4.1 Identity guarantee

Within a persistence context, loading the same entity type and primary key normally gives the same managed Java object instance.

```java
Employee first =
        entityManager.find(Employee.class, 10L);

Employee second =
        entityManager.find(Employee.class, 10L);

System.out.println(first == second); // normally true
```

This is an important part of persistence-context identity management.

## 4.2 Persistence context and first-level cache

The persistence context acts as the first-level cache.

Example:

```java
Employee first =
        entityManager.find(Employee.class, 10L);

Employee second =
        entityManager.find(Employee.class, 10L);
```

The second lookup can be satisfied from the persistence context instead of executing another database query.

## 4.3 `clear()`

```java
entityManager.clear();
```

This detaches all managed entities from the current persistence context.

After clearing, a later lookup may need to query the database again.

## 4.4 `detach()`

```java
entityManager.detach(employee);
```

This detaches only the specified entity.

## 4.5 Why does the persistence context matter?

It supports:

- First-level caching.
- Dirty checking.
- Entity identity.
- Write-behind behavior.
- Relationship management.
- Synchronization with the database.

---

# 5. Flush vs Commit

These concepts are related but different.

## 5.1 Flush

Flush synchronizes pending persistence-context changes with the database.

For example, Hibernate may send an SQL INSERT or UPDATE when a flush occurs.

```java
entityManager.persist(employee);
entityManager.flush();
```

After the flush, the SQL has been sent to the database within the current transaction, but the transaction has not necessarily committed.

## 5.2 Commit

Commit completes the database transaction and makes its changes durable according to the database's transaction guarantees.

## 5.3 Comparison

| Flush | Commit |
|---|---|
| Synchronizes persistence-context changes with database | Completes the transaction |
| Does not itself commit the transaction | Makes transaction changes committed |
| Can happen more than once in a transaction | Usually occurs at the transaction's end |
| SQL errors may surface during flush | Commit can also fail |

## 5.4 Example

```text
Begin transaction
      |
      v
Modify managed entity
      |
      v
Flush
      |
      +--> SQL UPDATE sent
      |
      v
More work
      |
      v
Commit
      |
      v
Transaction completed
```

A transaction can still roll back after a flush. Flushed SQL is not the same thing as committed data.

## 5.5 When does Hibernate flush?

Common flush situations include:

- Explicit `flush()`.
- Transaction commit.
- Before certain queries under `AUTO` flush mode when pending changes could affect query results.

Exact behavior depends on flush mode and provider behavior.

---

# 6. First-Level Cache vs Second-Level Cache

Hibernate supports caching at different levels.

## 6.1 First-level cache

- Associated with the persistence context / `EntityManager`.
- Enabled by default as part of normal persistence-context behavior.
- Stores managed entity instances.
- Does not require a separate cache provider.

## 6.2 Second-level cache

- Shared across persistence contexts within the relevant SessionFactory.
- Optional.
- Requires appropriate provider/configuration.
- Can reduce repeated database reads for cacheable data.
- Requires a strategy for invalidation and consistency.

## 6.3 Comparison

| First-level cache | Second-level cache |
|---|---|
| Persistence-context scoped | Shared across persistence contexts in a SessionFactory |
| Part of normal JPA/Hibernate operation | Optional feature |
| Managed entity identity and dirty checking | Reuse of cached data across sessions |
| No separate provider required | Cache provider/configuration required |

## 6.4 What about Redis?

Redis is an external distributed in-memory data store. It is not automatically the same thing as Hibernate's second-level cache.

A backend may use Redis for:

- Application-level caching.
- Distributed caching.
- Rate limiting.
- Session or token-related data.
- Frequently accessed reference data.

Choose a cache based on the data's consistency, expiry, and invalidation requirements.

---

# 7. Lazy vs Eager Loading

Entity relationships can be loaded using different fetch strategies.

## 7.1 Lazy loading

LAZY means related data is not required to be fetched immediately when the parent entity is loaded. It may be loaded when the relationship is accessed.

Example:

```java
@ManyToOne(fetch = FetchType.LAZY)
@JoinColumn(name = "department_id")
private Department department;
```

## 7.2 Eager loading

EAGER means the relationship must be fetched eagerly as part of loading the entity. The provider controls how the data is fetched; it does not guarantee one particular SQL join.

```java
@ManyToOne(fetch = FetchType.EAGER)
@JoinColumn(name = "department_id")
private Department department;
```

## 7.3 JPA default fetch types

| Relationship | Default |
|---|---|
| `@OneToOne` | EAGER |
| `@ManyToOne` | EAGER |
| `@OneToMany` | LAZY |
| `@ManyToMany` | LAZY |

You can explicitly choose a fetch type where the annotation supports it.

## 7.4 LazyInitializationException

A common problem occurs when lazy data is accessed after the persistence context is no longer available.

Conceptually:

```java
Employee employee = employeeRepository.findById(10L)
        .orElseThrow();

// If the persistence context is already closed,
// accessing a lazy relationship may fail.
String departmentName =
        employee.getDepartment().getName();
```

The exact outcome depends on transaction and persistence-context boundaries.

Possible solutions include:

- Fetch the required relationship inside the service transaction.
- Use a fetch join for the query.
- Use `@EntityGraph`.
- Map the needed values to a DTO while the data is available.
- Use batch fetching where appropriate.

Avoid keeping a database transaction open for a long time merely to make lazy loading work.

---

# 8. N+1 Query Problem

The N+1 query problem is a common ORM performance issue.

## 8.1 Example

Suppose the backend loads 20 employees:

```java
List<Employee> employees = employeeRepository.findAll();
```

Then it accesses each employee's department:

```java
for (Employee employee : employees) {
    System.out.println(employee.getDepartment().getName());
}
```

If the relationship is lazily loaded and not already initialized, the application may execute:

1. One query to load employees.
2. One additional query per employee to load its department.

For 20 employees, that can mean 1 + 20 = 21 queries.

The exact number depends on the mapping, cache, provider, and data.

## 8.2 Solution 1: `JOIN FETCH`

```java
@Query("""
       SELECT e
       FROM Employee e
       JOIN FETCH e.department
       """)
List<Employee> findEmployeesWithDepartment();
```

This requests the related department in the same query.

## 8.3 Solution 2: `@EntityGraph`

```java
@EntityGraph(attributePaths = {"department"})
List<Employee> findByActiveTrue();
```

An entity graph specifies relationships that should be fetched for a particular query.

## 8.4 Solution 3: Batch fetching

Hibernate can fetch related entities in batches rather than issuing one query per parent.

For example, Hibernate's `@BatchSize` can be used to group lazy loads.

## 8.5 Important warning

Changing every relationship to EAGER is not a reliable fix for N+1.

- It may fetch unnecessary data.
- It may still result in multiple SQL statements.
- It can create large object graphs.
- It may cause performance problems elsewhere.

Choose fetching behavior per use case and inspect generated SQL.

## 8.6 Pagination and collection fetch joins

Be careful when combining pagination with collection fetch joins. A collection join can multiply SQL rows for each parent and make database-level pagination difficult or incorrect depending on the query/provider behavior.

For paginated parent records, consider a two-step approach or an entity graph suitable for the query.

---

# 9. Entity Relationships and Mapping Ownership

JPA relationships represent associations between entities.

The important concepts are the owning side, inverse side, foreign keys, and join tables.

## 9.1 `@ManyToOne`

Many employees belong to one department.

```java
@Entity
public class Employee {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "department_id")
    private Department department;
}
```

The foreign key is stored in the employee table.

## 9.2 `@OneToMany`

```java
@Entity
public class Department {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @OneToMany(mappedBy = "department")
    private List<Employee> employees = new ArrayList<>();
}
```

The `mappedBy` value refers to the Java field on the owning side.

## 9.3 `@JoinColumn`

`@JoinColumn` identifies the foreign-key column used by the relationship mapping.

## 9.4 `@JoinTable`

A join table stores associations between two entity tables. It is commonly used for many-to-many mappings and can also be used in other relationship mappings.

## 9.5 Owning side vs inverse side

- The owning side controls the relationship update in the JPA mapping.
- The inverse side uses `mappedBy`.
- In a typical bidirectional one-to-many/many-to-one mapping, the many-to-one side owns the foreign key.

Changing only the inverse collection does not necessarily update the database relationship.

## 9.6 Keep both sides synchronized

A helper method can update both sides:

```java
public void addEmployee(Employee employee) {
    employees.add(employee);
    employee.setDepartment(this);
}

public void removeEmployee(Employee employee) {
    employees.remove(employee);
    employee.setDepartment(null);
}
```

This keeps the in-memory object graph consistent.

---

# 10. Cascade Types and Orphan Removal

Cascade controls whether certain persistence operations propagate from one entity to related entities.

## 10.1 Cascade types

| Cascade type | Meaning |
|---|---|
| `PERSIST` | Persist operation propagates |
| `MERGE` | Merge operation propagates |
| `REMOVE` | Remove operation propagates |
| `REFRESH` | Refresh operation propagates |
| `DETACH` | Detach operation propagates |
| `ALL` | All listed cascade operations propagate |

Example:

```java
@OneToMany(
    mappedBy = "department",
    cascade = CascadeType.PERSIST
)
private List<Employee> employees = new ArrayList<>();
```

When the department is persisted, the persist operation can propagate to its employees.

## 10.2 `orphanRemoval`

`orphanRemoval = true` means that removing a privately owned child from the relationship can cause the child entity to be deleted when the change is synchronized.

```java
@OneToMany(
    mappedBy = "department",
    cascade = CascadeType.ALL,
    orphanRemoval = true
)
private List<Employee> employees = new ArrayList<>();
```

Use it only when the child lifecycle is genuinely dependent on the parent relationship.

## 10.3 Cascade REMOVE vs database ON DELETE CASCADE

These are different mechanisms:

- JPA `CascadeType.REMOVE` propagates a remove operation through the entity relationship.
- Database `ON DELETE CASCADE` is a database foreign-key rule.

Do not apply `CascadeType.ALL` or `REMOVE` casually. Deleting a parent can unexpectedly delete related records if the relationship's lifecycle has not been considered carefully.

---

# 11. `equals()` and `hashCode()` in JPA Entities

Java collections such as `HashSet` and `HashMap` depend on `equals()` and `hashCode()`.

Entity equality requires care because generated database IDs may not exist when an entity is first created.

## 11.1 The problem with generated IDs

Consider a new entity:

```java
Employee employee = new Employee();
```

Its generated ID may be null before persistence.

After insertion, the database may assign an ID.

If `hashCode()` depends directly on a generated ID that changes from null to a value, an entity already stored in a hash-based collection can become difficult to find in that collection.

## 11.2 General guidance

- Override `equals()` and `hashCode()` together when implementing equality.
- Prefer a stable, unique business key only when the domain genuinely has one that is immutable and reliably unique.
- Be careful with generated identifiers and transient entities.
- Consider Hibernate proxies and inheritance when implementing equality.
- Avoid including mutable relationships or mutable fields in hash calculations.

There is no one universal entity equality implementation that is correct for every model. Choose a strategy based on the entity's lifecycle and domain identity.

---

# 12. Hibernate Batch Processing

Batching reduces the overhead of sending many similar SQL statements individually.

## 12.1 Why batching matters

Suppose the application needs to insert 1,000 records.

Without batching, it may send many individual INSERT statements. With JDBC batching, the driver can send groups of statements more efficiently.

## 12.2 Configuration example

A Hibernate configuration may include:

```properties
spring.jpa.properties.hibernate.jdbc.batch_size=50
spring.jpa.properties.hibernate.order_inserts=true
spring.jpa.properties.hibernate.order_updates=true
```

The actual benefit depends on the database driver, identifier strategy, SQL shape, and Hibernate configuration.

## 12.3 Large insert loop

```java
@Transactional
public void importEmployees(List<Employee> employees) {

    int batchSize = 50;

    for (int i = 0; i < employees.size(); i++) {
        entityManager.persist(employees.get(i));

        if ((i + 1) % batchSize == 0) {
            entityManager.flush();
            entityManager.clear();
        }
    }
}
```

Why use `flush()` and `clear()`?

- `flush()` synchronizes pending changes.
- `clear()` detaches managed entities and limits persistence-context memory growth.

## 12.4 Does `saveAll()` guarantee JDBC batching?

No. `saveAll()` is a convenient Spring Data method, but it does not by itself guarantee that JDBC batching is enabled or that the provider will batch SQL.

Check the Hibernate and JDBC configuration and verify behavior when performance matters.

## 12.5 ID generation and batching

Some identifier generation strategies can interfere with insert batching. In particular, an `IDENTITY` strategy often requires the INSERT to obtain the generated key, which can limit batching behavior.

---

# 13. Database Indexing and Query Performance

An index helps the database find rows without scanning the entire table in many query patterns.

## 13.1 What is an index?

An index is a database data structure that helps locate records based on indexed columns.

Example:

```sql
CREATE INDEX idx_employee_email
ON employees(email);
```

A query such as:

```sql
SELECT *
FROM employees
WHERE email = 'rahul@example.com';
```

may benefit from the index.

## 13.2 B-tree index

PostgreSQL commonly uses B-tree indexes for equality and range conditions.

They are useful for patterns such as:

- `=`
- `<`, `>`, `<=`, `>=`
- `BETWEEN`
- Some ordered queries

The query planner decides whether using an index is actually cheaper than another plan.

## 13.3 Composite index

A composite index includes more than one column.

```sql
CREATE INDEX idx_trip_date_status
ON trips(trip_date, status);
```

This can be useful for queries filtering by `trip_date`, or by `trip_date` and `status` together.

For a conventional B-tree composite index, the leading-column order matters. An index on `(trip_date, status)` is not generally equivalent to one on `(status, trip_date)`.

## 13.4 Index tradeoffs

Indexes are not free.

Benefits:

- Faster lookups for suitable queries.
- Can support ordering and joins.
- Can enforce uniqueness with a unique index.

Costs:

- Additional storage.
- Extra work on INSERT, UPDATE, and DELETE.
- More indexes to maintain.

Create indexes based on actual query patterns, not simply on every column.

## 13.5 `EXPLAIN ANALYZE`

PostgreSQL provides query-plan tools:

```sql
EXPLAIN ANALYZE
SELECT *
FROM employees
WHERE email = 'rahul@example.com';
```

`EXPLAIN` shows the planned execution strategy. `EXPLAIN ANALYZE` executes the query and reports actual execution information.

Use it to investigate:

- Sequential scans.
- Index scans.
- Join strategies.
- Estimated vs actual row counts.
- Execution time.

---

# 14. SQL JOINs and Query Optimization

Hibernate ultimately works with relational data. Backend developers should understand the SQL generated by ORM queries.

## 14.1 INNER JOIN

Returns rows with matching records on both sides.

```sql
SELECT e.name, d.name
FROM employees e
INNER JOIN departments d
    ON e.department_id = d.id;
```

## 14.2 LEFT JOIN

Returns all rows from the left table and matching rows from the right table. If no match exists, right-side columns are NULL.

```sql
SELECT e.name, d.name
FROM employees e
LEFT JOIN departments d
    ON e.department_id = d.id;
```

## 14.3 RIGHT JOIN

Returns all rows from the right table and matching rows from the left table.

## 14.4 FULL OUTER JOIN

Returns matching rows plus unmatched rows from both sides.

## 14.5 JPQL join vs SQL join

JPQL joins entity relationships using entity names and fields. SQL joins tables and columns.

A JPQL fetch join can also request that an association be fetched as part of the query:

```java
@Query("""
       SELECT t
       FROM Trip t
       JOIN FETCH t.driver
       WHERE t.id = :tripId
       """)
Optional<Trip> findByIdWithDriver(@Param("tripId") Long tripId);
```

Do not add fetch joins indiscriminately. Fetching multiple collections can multiply result rows and create expensive queries.

---

# 15. Transactions and ACID

A transaction groups database operations into one logical unit of work.

## 15.1 ACID properties

| Property | Meaning |
|---|---|
| Atomicity | All operations in the transaction succeed or are rolled back |
| Consistency | Defined constraints and invariants remain satisfied |
| Isolation | Concurrent transactions are isolated according to the chosen level |
| Durability | Committed changes survive failures according to database guarantees |

## 15.2 Isolation levels

Common SQL isolation levels:

1. READ UNCOMMITTED
2. READ COMMITTED
3. REPEATABLE READ
4. SERIALIZABLE

They control how concurrent transactions can observe each other's work.

## 15.3 Common anomalies

### Dirty read

A transaction reads another transaction's uncommitted changes.

### Non-repeatable read

A transaction reads the same row twice and gets different committed values because another transaction updated it between reads.

### Phantom read

A repeated query using the same search condition returns a different set of rows because another transaction inserted or deleted matching rows.

## 15.4 PostgreSQL isolation behavior

For PostgreSQL:

- Default isolation is READ COMMITTED.
- READ UNCOMMITTED behaves as READ COMMITTED.
- REPEATABLE READ uses a transaction-level snapshot and prevents phantom reads in PostgreSQL's implementation.
- SERIALIZABLE provides stronger guarantees but transactions may be aborted to prevent serialization anomalies. Applications may need to retry.

Do not assume every database implements each isolation level identically.

## 15.5 Spring `@Transactional`

```java
@Transactional
public void assignDriver(Long tripId, Long driverId) {
    // Load trip
    // Load driver
    // Validate business rules
    // Update related entities
}
```

A service method is often a good place for a transaction spanning multiple repository operations.

Spring's default rollback behavior:

- Rollback for unchecked exceptions (`RuntimeException`) and `Error`.
- Checked exceptions do not cause rollback by default.

You can customize this with `rollbackFor` and `noRollbackFor`.

---

# 16. Optimistic vs Pessimistic Locking

Locking helps manage concurrent updates.

Imagine two administrators attempt to assign the same driver to different trips at nearly the same time.

Without appropriate concurrency control, both operations may read the driver as available and then attempt to assign the driver.

## 16.1 Optimistic locking

Optimistic locking assumes conflicts are relatively uncommon and checks whether the record changed before updating it.

JPA supports this through `@Version`.

```java
@Entity
public class Driver {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private boolean assigned;

    @Version
    private Long version;
}
```

When an entity is updated, the version is checked. If another transaction has changed the row, the update can fail with an optimistic locking exception.

The application can handle the conflict and, where appropriate, return HTTP 409 Conflict.

## 16.2 Pessimistic locking

Pessimistic locking obtains a database lock to prevent conflicting operations while a transaction is working.

Repository example:

```java
public interface DriverRepository
        extends JpaRepository<Driver, Long> {

    @Lock(LockModeType.PESSIMISTIC_WRITE)
    @Query("SELECT d FROM Driver d WHERE d.id = :id")
    Optional<Driver> findByIdForUpdate(@Param("id") Long id);
}
```

This should be used within a suitable transaction. Database lock behavior and supported modes depend on the database.

## 16.3 Comparison

| Optimistic locking | Pessimistic locking |
|---|---|
| Detects conflicts when updating | Acquires locks to prevent conflicting updates |
| Often uses a version column | Uses database locking |
| Good when conflicts are relatively rare | Useful when contention is expected and coordination is required |
| Can fail and require retry or conflict response | Can block other transactions and increase deadlock risk |

Choose based on the workload and business consistency requirements.

---

# 17. HikariCP Connection Pooling

Opening a new database connection for every request is expensive.

A connection pool maintains reusable database connections.

Spring Boot commonly uses HikariCP as its JDBC connection pool.

## 17.1 Why use connection pooling?

- Reduces connection creation overhead.
- Reuses established connections.
- Limits concurrent database connections.
- Helps control database resource usage.

## 17.2 Important configuration properties

```properties
spring.datasource.hikari.maximum-pool-size=10
spring.datasource.hikari.minimum-idle=5
spring.datasource.hikari.connection-timeout=30000
spring.datasource.hikari.idle-timeout=600000
spring.datasource.hikari.max-lifetime=1800000
```

| Property | Meaning |
|---|---|
| `maximum-pool-size` | Maximum number of connections in the pool |
| `minimum-idle` | Minimum number of idle connections HikariCP tries to maintain |
| `connection-timeout` | Maximum time a caller waits to obtain a connection |
| `idle-timeout` | Maximum idle time before an idle connection can be retired, subject to pool rules |
| `max-lifetime` | Maximum lifetime of a connection in the pool |

These are example values, not universal production recommendations. Tune them based on database capacity, application concurrency, and deployment topology.

## 17.3 Connection pool exhaustion

Pool exhaustion occurs when all available connections are in use and new requests cannot obtain a connection within the configured timeout.

Possible causes:

- Long-running queries.
- Transactions held open too long.
- Too much application concurrency.
- Connection leaks.
- Pool size too small for the workload.
- Database unable to handle the configured concurrency.

Increasing the pool size blindly can overload the database. Diagnose the underlying cause first.

---

# 18. DTO Projections, Pagination, and Read-Only Transactions

Not every API needs to load a complete entity graph.

## 18.1 DTO projection

A DTO projection retrieves only the fields needed for a response.

Example:

```java
public record EmployeeSummary(
        Long id,
        String name,
        String email
) {}
```

JPQL constructor projection:

```java
@Query("""
       SELECT new com.example.dto.EmployeeSummary(
           e.id, e.name, e.email
       )
       FROM Employee e
       WHERE e.active = true
       """)
List<EmployeeSummary> findActiveEmployeeSummaries();
```

This can avoid loading unnecessary entity state and relationships.

## 18.2 Pagination

Use `Pageable` when a list endpoint needs bounded results.

```java
Page<Employee> findByActiveTrue(Pageable pageable);
```

A `Page` includes total metadata and may require a count query. A `Slice` focuses on whether more results are available and can avoid calculating the total count.

For very large datasets or deep pagination, keyset/seek pagination may be more efficient than large offsets, depending on the query and index design.

## 18.3 `@Transactional(readOnly = true)`

```java
@Transactional(readOnly = true)
public EmployeeResponse getEmployee(Long id) {
    // Read-only service operation
}
```

This marks the transaction as read-only. It can be used as a hint to the transaction manager/provider and may enable optimizations.

It is not a universal guarantee that the database will reject writes. Do not treat it as a security boundary.

---

# 19. EntityGraph and Fetch Plans

An entity graph describes which relationships should be fetched for a particular operation.

It is useful when the same entity is used by different API endpoints that need different related data.

Example:

```java
@EntityGraph(attributePaths = {"department"})
Optional<Employee> findWithDepartmentById(Long id);
```

A query-specific fetch plan is often better than making every relationship EAGER globally.

JPA supports different graph semantics:

- **FETCH graph:** Attributes specified are treated as eager for the operation; unspecified attributes are treated as lazy, subject to provider and mapping rules.
- **LOAD graph:** Specified attributes are treated as eager, while unspecified attributes retain their mapping-defined fetch behavior.

The provider may fetch additional data beyond the graph when required by its mapping or implementation.

---

# 20. Database Migrations: Flyway vs Liquibase

Database schema changes should be version-controlled and applied consistently across environments.

## 20.1 Why migrations?

Suppose a new field is added to the employee table.

Without a migration process, developers may manually change local databases, and production may end up with a different schema.

Migration tools record and apply schema changes in a controlled order.

## 20.2 Flyway

Flyway commonly uses versioned SQL migration files.

Example:

```text
src/main/resources/db/migration/
    V1__create_employees_table.sql
    V2__add_employee_status.sql
    V3__create_trips_table.sql
```

Example migration:

```sql
ALTER TABLE employees
ADD COLUMN active BOOLEAN NOT NULL DEFAULT TRUE;
```

Flyway tracks applied migrations in a schema history table.

## 20.3 Liquibase

Liquibase supports change logs expressed in XML, YAML, JSON, or SQL formats, with change sets and tracking metadata.

## 20.4 Comparison

| Flyway | Liquibase |
|---|---|
| Often SQL-first and straightforward | Structured change-log model |
| Versioned migrations | Change sets |
| Simple migration workflow | Rich change-log and rollback capabilities |
| Good fit for SQL-centric teams | Useful for teams needing structured database change definitions |

## 20.5 Production schema management

A common production approach is:

```properties
spring.jpa.hibernate.ddl-auto=validate
```

This asks Hibernate to validate mappings against the schema rather than automatically modifying production schema.

Use a migration tool to create and evolve the schema. The exact strategy should be chosen by the team.

---

# 21. JPA Auditing

Auditing records metadata about when an entity was created or modified and, optionally, who performed the operation.

## 21.1 Common auditing annotations

| Annotation | Purpose |
|---|---|
| `@CreatedDate` | Creation timestamp |
| `@LastModifiedDate` | Last update timestamp |
| `@CreatedBy` | Creator identity |
| `@LastModifiedBy` | Last modifier identity |

## 21.2 Enable auditing

```java
@Configuration
@EnableJpaAuditing
public class JpaAuditingConfig {
}
```

## 21.3 Auditable base class

```java
@MappedSuperclass
@EntityListeners(AuditingEntityListener.class)
public abstract class AuditableEntity {

    @CreatedDate
    private Instant createdAt;

    @LastModifiedDate
    private Instant updatedAt;
}
```

An entity can extend this base class:

```java
@Entity
public class Employee extends AuditableEntity {
    // Employee fields
}
```

To populate `@CreatedBy` and `@LastModifiedBy`, configure an `AuditorAware` implementation that obtains the current authenticated user.

Auditing metadata is useful for support, troubleshooting, and compliance requirements, but it does not replace a complete business audit trail when detailed before/after history is required.

---

# 22. Database Constraints and Normalization

JPA annotations can describe mappings and some schema constraints, but the database should enforce important integrity rules as well.

## 22.1 Common constraints

| Constraint | Purpose |
|---|---|
| PRIMARY KEY | Uniquely identifies each row |
| FOREIGN KEY | Enforces a valid relationship to another table |
| UNIQUE | Prevents duplicate values or combinations |
| NOT NULL | Requires a value |
| CHECK | Enforces a Boolean condition on values |
| DEFAULT | Supplies a default value when none is provided |

Example:

```sql
CREATE TABLE employees (
    id BIGINT PRIMARY KEY,
    email VARCHAR(255) NOT NULL UNIQUE,
    age INTEGER CHECK (age >= 18),
    active BOOLEAN NOT NULL DEFAULT TRUE
);
```

## 22.2 Application validation vs database constraints

Application validation provides useful error messages and rejects invalid requests early.

Database constraints protect data integrity even if data is written through another application, script, or service.

Use both where appropriate.

## 22.3 Normalization

Normalization organizes relational data to reduce redundancy and update anomalies.

### First Normal Form (1NF)

- Each column contains an atomic value for the chosen data model.
- Avoid repeating groups in a single row.

### Second Normal Form (2NF)

- The relation is in 1NF.
- Every non-key attribute depends on the whole candidate key, not just part of a composite candidate key.

### Third Normal Form (3NF)

- The relation is in 2NF.
- Non-key attributes should not depend transitively on a candidate key through another non-key attribute, subject to the formal definition.

### BCNF

A stronger form where every determinant is a superkey.

## 22.4 Why normalization matters

It reduces:

- Update anomalies.
- Insertion anomalies.
- Deletion anomalies.
- Unnecessary duplication.

Denormalization may be useful for performance or reporting, but it introduces consistency and maintenance tradeoffs.

---

# 23. Common Mistakes to Avoid

1. **Thinking JPA and Hibernate are the same.** JPA is a specification; Hibernate is an implementation.
2. **Assuming `merge()` makes the original object managed.** It returns a separate managed instance containing the copied state.
3. **Assuming a setter immediately executes SQL.** Dirty checking commonly synchronizes at flush time.
4. **Confusing flush with commit.** Flush sends pending changes; commit completes the transaction.
5. **Assuming `clear()` deletes database data.** It detaches managed objects from the persistence context.
6. **Making all relationships EAGER.** This can load unnecessary data and cause performance issues.
7. **Ignoring N+1 queries.** Inspect generated SQL and choose query-specific fetch plans.
8. **Using `CascadeType.ALL` without lifecycle analysis.** Parent deletion can propagate to children.
9. **Implementing entity equality using mutable fields.** Hash-based collections require stable equality/hash behavior.
10. **Assuming `saveAll()` guarantees JDBC batching.** Batching needs suitable provider and driver configuration.
11. **Creating indexes on every column.** Indexes consume space and add write overhead.
12. **Increasing connection-pool size without diagnosis.** This may overload the database.
13. **Using `@Transactional` without understanding rollback rules.** Checked exceptions do not trigger rollback by default.
14. **Ignoring concurrent updates.** Use optimistic or pessimistic locking when business consistency requires it.
15. **Changing production schemas automatically without a migration plan.** Use controlled schema migrations.

---

# 24. Interview Questions and Answers

## Q1. What is the difference between JPA, Hibernate, and Spring Data JPA?

JPA is the persistence specification/API. Hibernate is a popular ORM implementation of JPA. Spring Data JPA provides repository abstractions that simplify persistence code.

## Q2. What are the four entity lifecycle states?

Transient, Managed/Persistent, Detached, and Removed.

## Q3. What is the difference between `persist()` and `merge()`?

`persist()` makes a new entity managed. `merge()` copies the state of a detached or other entity into a managed instance and returns that managed instance. The original detached object remains detached.

## Q4. What is dirty checking?

Dirty checking is the persistence provider's mechanism for detecting changes to managed entities and synchronizing those changes with the database during flush.

## Q5. What is a persistence context?

It is the set of managed entity instances associated with an `EntityManager`. It provides entity identity management, first-level caching, and change tracking.

## Q6. What is the first-level cache?

It is the persistence-context-scoped cache of managed entities. It is part of normal JPA/Hibernate operation.

## Q7. What is the second-level cache?

It is an optional cache shared across persistence contexts within the relevant SessionFactory. It requires suitable configuration and cache-provider support.

## Q8. What is the difference between flush and commit?

Flush synchronizes pending changes with the database. Commit completes the transaction. A transaction can still roll back after a flush.

## Q9. What is the difference between LAZY and EAGER fetching?

LAZY defers loading an association until it is accessed, while EAGER requires it to be fetched eagerly. The provider controls the actual SQL strategy.

## Q10. What is LazyInitializationException?

It commonly occurs when code tries to access an uninitialized lazy association after the relevant persistence context is no longer available.

## Q11. What is the N+1 query problem?

It occurs when loading a list requires one query for the parent rows and additional queries for related data for each parent. Fetch joins, entity graphs, or batch fetching may help.

## Q12. What is the owning side of a JPA relationship?

It is the side that controls relationship updates in the mapping. In a common bidirectional one-to-many/many-to-one mapping, the many-to-one side owns the foreign key.

## Q13. What does `mappedBy` do?

It marks the inverse side of a bidirectional relationship and points to the field on the owning side.

## Q14. What is the difference between cascade and orphan removal?

Cascade propagates persistence operations such as persist, merge, or remove. Orphan removal deletes a privately owned child when it is removed from the relationship, subject to JPA lifecycle rules.

## Q15. Why is `equals()` and `hashCode()` difficult for entities?

Generated identifiers can change from null to a database value, entities may be transient or detached, and Hibernate proxies can complicate type comparisons. Equality must remain consistent for hash-based collections.

## Q16. What is JDBC batching?

It groups similar JDBC statements so they can be sent more efficiently, reducing database round trips and overhead.

## Q17. Does `saveAll()` guarantee batching?

No. JDBC batching depends on Hibernate configuration, the JDBC driver, SQL shape, and identifier-generation strategy.

## Q18. Why do we create database indexes?

Indexes can improve lookups, joins, and ordering for suitable queries, but they consume storage and add overhead to writes.

## Q19. What is the difference between optimistic and pessimistic locking?

Optimistic locking detects conflicting updates, commonly using a version field. Pessimistic locking obtains database locks to prevent conflicting operations while the transaction is running.

## Q20. What is `@Version`?

It marks a version field used by JPA optimistic locking. The provider checks the version during updates to detect concurrent modifications.

## Q21. What is HikariCP?

HikariCP is a high-performance JDBC connection pool commonly used by Spring Boot applications. It manages reusable database connections.

## Q22. What is connection pool exhaustion?

It occurs when all connections are in use and new requests cannot obtain a connection before the configured timeout.

## Q23. What is the purpose of Flyway or Liquibase?

They manage version-controlled database schema changes and apply migrations consistently across environments.

## Q24. What is JPA auditing?

It automatically captures metadata such as creation time, last modification time, and optionally the user who created or modified an entity.

## Q25. Why use both application validation and database constraints?

Application validation gives useful feedback and rejects invalid input early. Database constraints enforce integrity even when data is written outside the application.

---

# 25. Quick Revision Sheet

| Concept | Remember |
|---|---|
| JPA | Persistence specification/API |
| Hibernate | ORM implementation |
| Spring Data JPA | Repository abstraction |
| Entity states | Transient, Managed, Detached, Removed |
| `persist()` | Make a new entity managed |
| `merge()` | Copy state into and return a managed instance |
| Dirty checking | Detect changes to managed entities |
| Persistence context | Managed entity set associated with EntityManager |
| First-level cache | Persistence-context scoped |
| Second-level cache | Optional shared cache |
| Flush | Synchronize changes with database |
| Commit | Complete transaction |
| LAZY | Defer relationship loading |
| EAGER | Fetch relationship eagerly |
| N+1 | One parent query plus additional related-data queries |
| `JOIN FETCH` | Fetch relationship as part of query |
| `@EntityGraph` | Query-specific fetch plan |
| Owning side | Controls relationship update |
| `mappedBy` | Points to owning-side field |
| Cascade | Propagate persistence operations |
| Orphan removal | Delete privately owned orphaned child |
| JDBC batching | Group similar SQL statements |
| B-tree index | Common PostgreSQL index type |
| `EXPLAIN ANALYZE` | Execute and inspect query plan |
| `@Version` | Optimistic locking |
| Pessimistic lock | Database lock for coordination |
| HikariCP | JDBC connection pool |
| Flyway | Versioned database migrations |
| Liquibase | Structured database change logs |
| JPA auditing | Creation/modification metadata |
| `ddl-auto=validate` | Validate mappings against schema |

---

# 26. Day 05 Completion Checklist

- [ ] I can distinguish JPA, Hibernate, and Spring Data JPA.
- [ ] I can explain the four entity lifecycle states.
- [ ] I understand `persist()`, `merge()`, `remove()`, `detach()`, and `clear()`.
- [ ] I can explain dirty checking.
- [ ] I can explain the persistence context and first-level cache.
- [ ] I can distinguish flush from commit.
- [ ] I can compare first-level and second-level caches.
- [ ] I can explain LAZY vs EAGER loading.
- [ ] I understand LazyInitializationException.
- [ ] I can explain and identify the N+1 query problem.
- [ ] I understand `JOIN FETCH`, `@EntityGraph`, and batch fetching.
- [ ] I can explain relationship ownership and `mappedBy`.
- [ ] I understand cascade types and orphan removal.
- [ ] I know why entity `equals()` and `hashCode()` need care.
- [ ] I can explain JDBC batching and its limitations.
- [ ] I can explain indexes and inspect a query plan.
- [ ] I can explain ACID and transaction isolation.
- [ ] I can compare optimistic and pessimistic locking.
- [ ] I understand HikariCP connection pooling.
- [ ] I can explain projections and pagination.
- [ ] I understand Flyway/Liquibase and JPA auditing.
- [ ] I can explain why database constraints matter.

---

## Final persistence architecture to remember

```text
                 SPRING BOOT SERVICE
                         |
                         | @Transactional
                         v
                 SPRING DATA JPA
                    Repository
                         |
                         v
                   JPA EntityManager
                         |
              +----------+----------+
              |                     |
              v                     v
       Persistence Context      Entity State
       (First-level cache)      Dirty checking
              |                     |
              +----------+----------+
                         |
                         v
                      HIBERNATE
                 SQL generation / ORM
                         |
                         v
                        JDBC
                         |
                         v
                    HIKARICP
                 Connection pool
                         |
                         v
                    POSTGRESQL
             Tables / indexes / constraints
             transactions / locks / schema
```

**The key idea:** JPA defines the persistence API, Hibernate manages entity state and translates object operations into SQL, and Spring Data JPA simplifies repository access. The persistence context enables identity management and dirty checking. Fetch plans, indexes, batching, transactions, locking, and connection pooling determine whether persistence code remains correct and performs well as the application grows.
'''

path = Path("/mnt/data/Day_05_Hibernate_JPA_Internals.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
