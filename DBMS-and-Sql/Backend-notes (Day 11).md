from pathlib import Path

content = r"""# Day 11 — SQL and DBMS Fundamentals

> **Java Backend Developer Interview Preparation**  
> Focus: Database fundamentals, SQL commands, keys and constraints, joins, subqueries, CTEs, indexing, query optimization, transactions, ACID, and normalization.  
> Database examples use PostgreSQL and an Employee Transport application.

---

## Learning objectives

By the end of this day, you should be able to:

- Explain what a DBMS and RDBMS do and how a Spring Boot application communicates with a database.
- Distinguish SQL command categories and the effects of `DELETE`, `TRUNCATE`, and `DROP`.
- Design tables using primary keys, foreign keys, and constraints.
- Write and explain common SQL joins.
- Understand subqueries, correlated subqueries, and Common Table Expressions (CTEs).
- Explain indexes and read the basic purpose of `EXPLAIN` and `EXPLAIN ANALYZE`.
- Explain ACID properties, transaction isolation, and common concurrency problems.
- Normalize a relational schema and recognize when denormalization may be useful.

---

# 1. DBMS Fundamentals and Database Architecture

## 1.1 What is data?

Data is information an application needs to store and retrieve.

For an Employee Transport application, examples include:

- Employee ID, name, email, and department.
- Driver details and availability.
- Vehicle registration number and seating capacity.
- Trip date, shift, route, and status.
- Employees assigned to a particular trip.

An application needs a reliable way to store this information so that it remains available after the application stops or restarts.

## 1.2 What is a database?

A database is an organized collection of data that can be stored, queried, updated, and managed.

For example, an employee transport database might contain tables such as:

```sql
employees
departments
drivers
vehicles
trips
trip_employees
```

Each table stores a particular type of information.

## 1.3 What is a DBMS?

A **Database Management System (DBMS)** is software that allows applications and users to create, store, retrieve, update, and manage data.

A DBMS commonly provides:

- Data storage and retrieval.
- Query execution.
- Transaction management.
- Concurrency control.
- Access control and security.
- Backup and recovery.
- Constraint enforcement.

Examples include PostgreSQL, MySQL, Oracle Database, and Microsoft SQL Server.

## 1.4 What is an RDBMS?

An **Relational Database Management System (RDBMS)** stores data in related tables.

A relational table consists of:

- Rows: individual records.
- Columns: attributes of those records.
- Keys: values used to identify records and establish relationships.

Example:

**employees**

| employee_id | name | department_id |
|---:|---|---:|
| 101 | Asha | 10 |
| 102 | Ravi | 20 |
| 103 | Neha | 10 |

**departments**

| department_id | department_name |
|---:|---|
| 10 | Engineering |
| 20 | Operations |

The `department_id` column connects an employee to a department. This relationship can be enforced with a foreign key.

## 1.5 DBMS vs RDBMS

| DBMS | RDBMS |
|---|---|
| General database management system concept | Relational form of DBMS |
| May use different data models | Organizes data into related tables |
| Relationship enforcement depends on the system and model | Uses relational keys and constraints |
| Broad category | A category within DBMS |

PostgreSQL is an RDBMS.

## 1.6 SQL, PostgreSQL, JDBC, Hibernate, and Spring Data JPA

These terms are related, but they are not interchangeable.

| Term | Meaning |
|---|---|
| SQL | Language used to define, query, and manipulate relational data |
| PostgreSQL | Database server that stores and manages data and executes SQL |
| JDBC | Java API for connecting to relational databases and executing SQL |
| JPA | Java specification for object-relational persistence |
| Hibernate | Popular implementation of JPA and an ORM framework |
| Spring Data JPA | Spring abstraction that simplifies repository-based data access |

### How a Spring Boot application reaches PostgreSQL

```text
Flutter Mobile App
        |
        | HTTP request / JSON
        v
Spring Boot Controller
        |
        v
Service Layer
        |
        v
Spring Data JPA Repository
        |
        v
JPA Provider: Hibernate
        |
        v
JDBC Driver
        |
        | SQL over database connection
        v
PostgreSQL Database
        |
        v
Rows returned
        |
        v
Hibernate maps rows to Java objects
        |
        v
Service / Controller returns response
```

For example, a repository method:

```java
Optional<Employee> findById(Long id);
```

can cause Hibernate to execute SQL equivalent to:

```sql
SELECT *
FROM employees
WHERE employee_id = ?;
```

The exact SQL depends on the entity mapping, dialect, and query.

**Important distinction:** Spring Data JPA does not replace PostgreSQL or SQL. It helps Java code access data. PostgreSQL still executes the database query.

## 1.7 What is a schema?

A schema is a named logical container for database objects such as tables, views, indexes, and functions.

PostgreSQL commonly uses the `public` schema by default.

```sql
SELECT *
FROM public.employees;
```

A database can contain multiple schemas, which helps organize objects and permissions.

### Interview answer: What is the difference between SQL and PostgreSQL?

SQL is a query language. PostgreSQL is a relational database system that understands SQL and provides database features such as transactions, constraints, indexes, and concurrency control.

---

# 2. SQL Command Categories

SQL commands are commonly grouped by their purpose.

## 2.1 DDL — Data Definition Language

DDL commands define or change database structures.

Common commands:

- `CREATE`
- `ALTER`
- `DROP`
- `TRUNCATE`

Create a table:

```sql
CREATE TABLE departments (
    department_id BIGINT PRIMARY KEY,
    department_name VARCHAR(100) NOT NULL
);
```

Add a column:

```sql
ALTER TABLE employees
ADD COLUMN phone_number VARCHAR(20);
```

Remove a table:

```sql
DROP TABLE employees;
```

Remove all rows while keeping the table structure:

```sql
TRUNCATE TABLE employees;
```

## 2.2 DML — Data Manipulation Language

DML commands modify table data.

Common commands:

- `INSERT`
- `UPDATE`
- `DELETE`

Insert:

```sql
INSERT INTO departments (department_id, department_name)
VALUES (10, 'Engineering');
```

Update:

```sql
UPDATE departments
SET department_name = 'Product Engineering'
WHERE department_id = 10;
```

Delete selected records:

```sql
DELETE FROM departments
WHERE department_id = 10;
```

**Warning:** An `UPDATE` or `DELETE` without a suitable `WHERE` clause may affect every row in the table.

## 2.3 DQL — Data Query Language

DQL is commonly used to describe commands that retrieve data. The main command is `SELECT`.

```sql
SELECT employee_id, name
FROM employees
WHERE department_id = 10;
```

Some SQL classifications treat `SELECT` as its own DQL category; others include it under a broader DML classification. In interviews, explain the purpose rather than arguing over the label.

## 2.4 DCL — Data Control Language

DCL commands manage database privileges.

Common commands:

- `GRANT`
- `REVOKE`

Example:

```sql
GRANT SELECT ON employees TO reporting_user;

REVOKE SELECT ON employees FROM reporting_user;
```

The exact permissions available depend on the database system and object.

## 2.5 TCL — Transaction Control Language

TCL commands manage transaction boundaries.

Common commands:

- `BEGIN` or `START TRANSACTION`
- `COMMIT`
- `ROLLBACK`
- `SAVEPOINT`

Example:

```sql
BEGIN;

UPDATE vehicles
SET status = 'IN_USE'
WHERE vehicle_id = 5;

UPDATE trips
SET status = 'ASSIGNED'
WHERE trip_id = 1001;

COMMIT;
```

If an operation fails and the transaction should not be applied:

```sql
ROLLBACK;
```

## 2.6 DELETE vs TRUNCATE vs DROP

This is a frequent interview question.

| Feature | DELETE | TRUNCATE | DROP |
|---|---|---|---|
| Main purpose | Remove rows | Remove all rows efficiently | Remove the table object |
| `WHERE` supported | Yes | No | No |
| Table structure remains | Yes | Yes | No |
| Can remove selected rows | Yes | No | Not applicable |
| Typical use | Delete matching records | Empty a table | Remove an object no longer needed |

Examples:

```sql
-- Delete selected rows
DELETE FROM trips
WHERE status = 'CANCELLED';

-- Empty the table
TRUNCATE TABLE trips;

-- Remove the table itself
DROP TABLE trips;
```

### PostgreSQL notes

- PostgreSQL supports transactional `TRUNCATE`; it can be rolled back when executed within a transaction that is rolled back.
- `TRUNCATE` takes a strong table lock.
- `TRUNCATE ... RESTART IDENTITY` can reset owned sequences.
- `TRUNCATE ... CASCADE` can also truncate tables that have foreign-key references, so use it carefully.
- `DELETE` fires row-level delete triggers; `TRUNCATE` has different trigger behavior (`ON TRUNCATE` triggers).

Do not assume every database handles these operations identically.

### Interview answer: Is TRUNCATE faster than DELETE?

Often, truncating a whole table is faster than deleting every row individually because it does not process each row in the same way. But the exact behavior depends on the database, constraints, triggers, and storage engine. `TRUNCATE` is not a substitute when only selected rows must be deleted.

---

# 3. Keys and Constraints

Keys identify records and connect tables. Constraints protect data integrity.

## 3.1 Primary Key

A primary key uniquely identifies each row.

```sql
CREATE TABLE employees (
    employee_id BIGINT PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);
```

Primary key properties:

- Values must be unique.
- Values cannot be `NULL`.
- A table has one primary-key constraint, although it can contain multiple columns (a composite primary key).

## 3.2 Foreign Key

A foreign key establishes a relationship with a key in another table.

```sql
CREATE TABLE departments (
    department_id BIGINT PRIMARY KEY,
    department_name VARCHAR(100) NOT NULL
);

CREATE TABLE employees (
    employee_id BIGINT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    department_id BIGINT,
    CONSTRAINT fk_employee_department
        FOREIGN KEY (department_id)
        REFERENCES departments(department_id)
);
```

The foreign key prevents an employee from referencing a department that does not exist, unless the foreign-key value is `NULL` and the column permits it.

## 3.3 UNIQUE Constraint

A `UNIQUE` constraint prevents duplicate values in the constrained column or column combination.

```sql
CREATE TABLE employees (
    employee_id BIGINT PRIMARY KEY,
    email VARCHAR(150) UNIQUE
);
```

A composite unique constraint:

```sql
ALTER TABLE trip_employees
ADD CONSTRAINT uq_trip_employee
UNIQUE (trip_id, employee_id);
```

This prevents the same employee from being assigned to the same trip more than once.

**PostgreSQL note:** By default, a unique constraint allows multiple `NULL` values because `NULL` values are not considered equal for ordinary uniqueness checks. PostgreSQL also supports `NULLS NOT DISTINCT` when that behavior is desired.

## 3.4 NOT NULL Constraint

Prevents a column from storing `NULL`.

```sql
CREATE TABLE vehicles (
    vehicle_id BIGINT PRIMARY KEY,
    registration_number VARCHAR(30) NOT NULL
);
```

`NOT NULL` means a value must be present. It does not mean the value must be non-empty text; an empty string is still a value.

## 3.5 CHECK Constraint

Ensures that a value satisfies a condition.

```sql
CREATE TABLE vehicles (
    vehicle_id BIGINT PRIMARY KEY,
    seating_capacity INTEGER NOT NULL,
    CONSTRAINT chk_positive_capacity
        CHECK (seating_capacity > 0)
);
```

Another example:

```sql
ALTER TABLE employees
ADD CONSTRAINT chk_employee_age
CHECK (age >= 18);
```

A `CHECK` constraint is not a replacement for all business validation. For example, checking whether a vehicle is available at a particular time may require examining other rows and should usually be handled through transactional application logic and suitable database constraints or locking.

## 3.6 DEFAULT Constraint

Provides a value when an insert omits the column.

```sql
CREATE TABLE trips (
    trip_id BIGINT PRIMARY KEY,
    status VARCHAR(30) NOT NULL DEFAULT 'SCHEDULED'
);
```

The default is used when the column is omitted or `DEFAULT` is specified. Explicitly inserting `NULL` does not automatically invoke the default.

## 3.7 Composite Primary Key

A composite key uses multiple columns together to identify a row.

```sql
CREATE TABLE trip_employees (
    trip_id BIGINT NOT NULL,
    employee_id BIGINT NOT NULL,
    pickup_sequence INTEGER NOT NULL,
    PRIMARY KEY (trip_id, employee_id)
);
```

Neither `trip_id` nor `employee_id` alone needs to be unique. Their combination must be unique.

## 3.8 ON DELETE behavior

Foreign keys can define what happens when the referenced parent row is deleted.

```sql
FOREIGN KEY (department_id)
REFERENCES departments(department_id)
ON DELETE CASCADE
```

Common options:

| Option | Meaning |
|---|---|
| `CASCADE` | Delete dependent child rows |
| `SET NULL` | Set the child foreign-key value to `NULL` |
| `RESTRICT` | Prevent deletion while references exist |
| `NO ACTION` | Enforce the constraint; timing behavior can depend on deferrability |

Use cascading deletion only when the child record should genuinely not exist without the parent. Do not casually cascade-delete business records such as completed trips or financial history.

## 3.9 Why constraints matter

Application validation improves user-facing error messages, but database constraints protect data even if:

- Another service writes to the database.
- A scheduled job inserts records.
- A developer runs SQL manually.
- Two requests arrive concurrently.
- An application bug bypasses a validation check.

A robust system uses both application-level validation and database-level integrity constraints.

### Interview answer: Primary key vs unique key

A primary key is the table's main row identifier and cannot contain `NULL`. A table has one primary-key constraint, which may consist of multiple columns. A table can have multiple unique constraints. In PostgreSQL, a normal unique constraint permits multiple `NULL` values unless configured otherwise.

---

# 4. SQL Joins

A join combines rows from tables based on a related column or condition.

Assume these tables:

**employees**

| employee_id | name | department_id |
|---:|---|---:|
| 101 | Asha | 10 |
| 102 | Ravi | 20 |
| 103 | Neha | 10 |
| 104 | Kabir | NULL |

**departments**

| department_id | department_name |
|---:|---|
| 10 | Engineering |
| 20 | Operations |
| 30 | Finance |

## 4.1 INNER JOIN

Returns only rows that match the join condition in both tables.

```sql
SELECT
    e.employee_id,
    e.name,
    d.department_name
FROM employees e
INNER JOIN departments d
    ON e.department_id = d.department_id;
```

Result conceptually:

| employee_id | name | department_name |
|---:|---|---|
| 101 | Asha | Engineering |
| 102 | Ravi | Operations |
| 103 | Neha | Engineering |

Kabir is excluded because there is no matching department. Finance is excluded because it has no matching employee.

## 4.2 LEFT JOIN (LEFT OUTER JOIN)

Returns every row from the left table and matching rows from the right table. If there is no match, right-side columns are `NULL`.

```sql
SELECT
    e.employee_id,
    e.name,
    d.department_name
FROM employees e
LEFT JOIN departments d
    ON e.department_id = d.department_id;
```

Kabir appears with a `NULL` department name.

To find employees without a matching department:

```sql
SELECT e.*
FROM employees e
LEFT JOIN departments d
    ON e.department_id = d.department_id
WHERE d.department_id IS NULL;
```

## 4.3 RIGHT JOIN (RIGHT OUTER JOIN)

Returns every row from the right table and matching rows from the left table.

```sql
SELECT
    e.name,
    d.department_name
FROM employees e
RIGHT JOIN departments d
    ON e.department_id = d.department_id;
```

Finance appears even though no employee belongs to it.

A right join can usually be rewritten as a left join by reversing table order. Many teams prefer `LEFT JOIN` for readability.

## 4.4 FULL OUTER JOIN

Returns matching rows plus unmatched rows from both sides.

```sql
SELECT
    e.name,
    d.department_name
FROM employees e
FULL OUTER JOIN departments d
    ON e.department_id = d.department_id;
```

This includes employees without departments and departments without employees.

## 4.5 CROSS JOIN

Returns the Cartesian product: every row from the first table combined with every row from the second table.

```sql
SELECT
    e.name,
    d.department_name
FROM employees e
CROSS JOIN departments d;
```

If the first table has 4 rows and the second has 3, the result has 12 combinations.

Use a cross join only when all combinations are intended.

## 4.6 SELF JOIN

A table can be joined to itself. This is useful for hierarchical relationships such as employees and their managers.

Example table:

```sql
CREATE TABLE staff (
    employee_id BIGINT PRIMARY KEY,
    employee_name VARCHAR(100),
    manager_id BIGINT REFERENCES staff(employee_id)
);
```

Query:

```sql
SELECT
    employee.employee_name AS employee,
    manager.employee_name AS manager
FROM staff employee
LEFT JOIN staff manager
    ON employee.manager_id = manager.employee_id;
```

The table is given two aliases so that each role is clear.

## 4.7 JOIN vs UNION

| JOIN | UNION |
|---|---|
| Combines columns from related rows | Combines rows from separate query results |
| Usually uses a join condition | Requires compatible column counts and data types |
| Horizontal combination | Vertical combination |
| Can produce more columns | Keeps the output column structure |

Example `UNION`:

```sql
SELECT name AS person_name FROM employees
UNION
SELECT driver_name AS person_name FROM drivers;
```

`UNION` removes duplicate rows. `UNION ALL` retains duplicates and is often faster when duplicate elimination is unnecessary.

## 4.8 Common join mistakes

### Mistake 1: Missing join condition

```sql
SELECT *
FROM employees
JOIN departments;
```

Depending on syntax and database, this may be invalid or may produce a Cartesian-style result if written as a cross join. Always make the intended relationship explicit.

### Mistake 2: Turning a LEFT JOIN into an INNER JOIN accidentally

This query can remove employees with no department:

```sql
SELECT e.name, d.department_name
FROM employees e
LEFT JOIN departments d
    ON e.department_id = d.department_id
WHERE d.department_name = 'Engineering';
```

The `WHERE` condition rejects rows where `d.department_name` is `NULL`.

If the intention is to preserve all employees but match only Engineering departments, put the filter in the join condition:

```sql
SELECT e.name, d.department_name
FROM employees e
LEFT JOIN departments d
    ON e.department_id = d.department_id
   AND d.department_name = 'Engineering';
```

### Interview answer: INNER JOIN vs LEFT JOIN

An inner join returns only matching rows from both tables. A left join returns all rows from the left table and matching rows from the right; unmatched right-side columns are returned as `NULL`.

---

# 5. Subqueries and Common Table Expressions (CTEs)

## 5.1 What is a subquery?

A subquery is a query nested inside another SQL statement.

Example: find employees whose salary is greater than the average salary.

```sql
SELECT employee_id, name, salary
FROM employees
WHERE salary > (
    SELECT AVG(salary)
    FROM employees
);
```

The inner query calculates the average. The outer query selects employees whose salary is above it.

## 5.2 Scalar subquery

A scalar subquery returns one value (one row and one column).

```sql
SELECT
    name,
    salary,
    (SELECT AVG(salary) FROM employees) AS average_salary
FROM employees;
```

If a scalar subquery returns more than one row, the database raises an error.

## 5.3 Single-row and multiple-row subqueries

A single-row subquery can be compared with operators such as `=`, `>`, or `<`.

```sql
SELECT *
FROM employees
WHERE department_id = (
    SELECT department_id
    FROM departments
    WHERE department_name = 'Engineering'
);
```

A multiple-row subquery is often used with `IN`:

```sql
SELECT *
FROM employees
WHERE department_id IN (
    SELECT department_id
    FROM departments
    WHERE department_name IN ('Engineering', 'Operations')
);
```

## 5.4 IN and NOT IN

`IN` checks whether a value matches any value returned by a list or subquery.

```sql
SELECT *
FROM employees
WHERE department_id IN (10, 20);
```

`NOT IN` checks that a value does not match any returned value:

```sql
SELECT *
FROM employees
WHERE department_id NOT IN (10, 20);
```

**Important NULL trap:** If the subquery used with `NOT IN` returns a `NULL`, the comparison can evaluate to unknown and unexpectedly return no rows.

For anti-matching queries, `NOT EXISTS` is often safer.

## 5.5 EXISTS and NOT EXISTS

`EXISTS` checks whether the subquery returns at least one row.

Find departments that have at least one employee:

```sql
SELECT d.*
FROM departments d
WHERE EXISTS (
    SELECT 1
    FROM employees e
    WHERE e.department_id = d.department_id
);
```

Find departments without employees:

```sql
SELECT d.*
FROM departments d
WHERE NOT EXISTS (
    SELECT 1
    FROM employees e
    WHERE e.department_id = d.department_id
);
```

The value selected inside `EXISTS` is not important; `SELECT 1` is a common convention.

## 5.6 Correlated subquery

A correlated subquery refers to a column from the outer query.

Find employees whose salary is above their own department's average:

```sql
SELECT e.employee_id, e.name, e.salary, e.department_id
FROM employees e
WHERE e.salary > (
    SELECT AVG(e2.salary)
    FROM employees e2
    WHERE e2.department_id = e.department_id
);
```

The inner query uses `e.department_id` from the outer row.

A correlated subquery is conceptually evaluated in relation to outer rows, although the query optimizer may transform it into a more efficient execution plan.

## 5.7 What is a CTE?

A **Common Table Expression (CTE)** is a named temporary result defined with `WITH` and used by the main query.

```sql
WITH department_average AS (
    SELECT department_id, AVG(salary) AS avg_salary
    FROM employees
    GROUP BY department_id
)
SELECT
    e.name,
    e.salary,
    e.department_id
FROM employees e
JOIN department_average da
    ON e.department_id = da.department_id
WHERE e.salary > da.avg_salary;
```

Benefits:

- Makes complex queries easier to read.
- Gives a meaningful name to an intermediate result.
- Can break a large query into logical steps.
- Supports recursive queries for hierarchical data.

A CTE is not automatically faster than a subquery. Query planning and database version matter. PostgreSQL can inline many non-recursive CTEs; materialization behavior can be controlled in supported cases with `MATERIALIZED` or `NOT MATERIALIZED`.

## 5.8 Recursive CTE

Recursive CTEs are useful for hierarchical data such as an employee reporting structure or category tree.

Simplified example:

```sql
WITH RECURSIVE employee_hierarchy AS (
    -- Start with the top-level employee
    SELECT employee_id, employee_name, manager_id, 1 AS level
    FROM staff
    WHERE manager_id IS NULL

    UNION ALL

    -- Find employees reporting to the previous level
    SELECT s.employee_id, s.employee_name, s.manager_id,
           eh.level + 1
    FROM staff s
    JOIN employee_hierarchy eh
        ON s.manager_id = eh.employee_id
)
SELECT *
FROM employee_hierarchy
ORDER BY level, employee_id;
```

A recursive CTE has an anchor query and a recursive query. Ensure the recursion has a valid stopping condition and that the data does not create an unintended cycle.

## 5.9 Subquery vs CTE vs JOIN

| Technique | Good use |
|---|---|
| Subquery | A nested condition or calculation |
| CTE | A named intermediate result that improves readability or supports recursion |
| JOIN | Combining related rows from tables or derived results |

Do not choose a technique only because it sounds faster. Check the query plan and measure performance with representative data.

### Interview answer: EXISTS vs IN

`IN` checks whether a value belongs to a set of returned values. `EXISTS` checks whether a correlated subquery returns at least one row. The optimizer may produce similar plans for many cases. `NOT EXISTS` is often safer than `NOT IN` when `NULL` values may occur in the subquery result.

---

# 6. Indexing and Query Optimization

## 6.1 What is an index?

An index is a database structure that helps the database locate rows without scanning the entire table in many situations.

Think of an index in a book: instead of reading every page to find a topic, you use the index to locate relevant pages.

Example:

```sql
CREATE INDEX idx_employees_email
ON employees(email);
```

A query filtering by email may be able to use this index:

```sql
SELECT *
FROM employees
WHERE email = 'asha@example.com';
```

The database optimizer decides whether an index is beneficial. Creating an index does not force the database to use it.

## 6.2 Why indexes are useful

Indexes can improve:

- Equality lookups.
- Range filters.
- Joins on indexed columns.
- Some sorting and grouping operations.
- Enforcing uniqueness through unique indexes.

But indexes also have costs:

- Extra disk space.
- More work for `INSERT`, `UPDATE`, and `DELETE`.
- Maintenance and vacuum considerations.
- Potentially slower writes when too many indexes exist.

An index should support a real query pattern, not be added to every column automatically.

## 6.3 Common index type: B-tree

PostgreSQL uses B-tree indexes by default for ordinary `CREATE INDEX` statements.

B-tree indexes are useful for many equality and range comparisons:

```sql
CREATE INDEX idx_trips_trip_date
ON trips(trip_date);
```

This can help queries such as:

```sql
SELECT *
FROM trips
WHERE trip_date >= DATE '2026-11-01'
  AND trip_date < DATE '2026-12-01';
```

Other PostgreSQL index types include:

- Hash: equality comparisons.
- GIN: useful for composite values such as arrays, JSONB, and full-text search.
- GiST: extensible indexing for geometric and other specialized data.
- BRIN: useful for very large tables where values correlate with physical row order.

Choose an index type based on the operator and data model.

## 6.4 Composite index

A composite index contains more than one column.

```sql
CREATE INDEX idx_trips_date_shift
ON trips(trip_date, shift);
```

This can support queries filtering by `trip_date`, and often by both `trip_date` and `shift`.

The order of columns matters. For a B-tree index on `(trip_date, shift)`, queries that constrain the leading column are generally the most natural fit.

Example:

```sql
SELECT *
FROM trips
WHERE trip_date = DATE '2026-11-08'
  AND shift = 'MORNING';
```

An index on `(trip_date, shift)` is a plausible fit.

A query filtering only by `shift` may not benefit as much from this index because `shift` is not the leading column. PostgreSQL can sometimes use skip-scan-like strategies depending on version and data distribution, but do not design indexes assuming that behavior will always make a non-leading filter efficient.

## 6.5 Unique index

A unique index prevents duplicate indexed values.

```sql
CREATE UNIQUE INDEX uq_vehicles_registration
ON vehicles(registration_number);
```

A table-level `UNIQUE` constraint is usually preferable when expressing a data-integrity rule because it communicates the intent. PostgreSQL implements a unique constraint using a unique index.

## 6.6 Partial index

A partial index indexes only rows that satisfy a condition.

```sql
CREATE INDEX idx_active_drivers
ON drivers(driver_id)
WHERE status = 'ACTIVE';
```

This may be useful when queries frequently look up active drivers and active rows are a subset of the table.

The query predicate must be compatible with the partial-index condition for the planner to use it.

## 6.7 EXPLAIN

`EXPLAIN` displays the query plan chosen by the database without executing the query in the normal way.

```sql
EXPLAIN
SELECT *
FROM employees
WHERE email = 'asha@example.com';
```

The plan can show operations such as:

- Sequential Scan.
- Index Scan.
- Index Only Scan.
- Bitmap Index Scan.
- Nested Loop.
- Hash Join.
- Sort.
- Aggregate.

A **Sequential Scan** reads table rows. It is not automatically bad. For a small table or a query returning a large proportion of rows, a sequential scan may be cheaper than using an index.

## 6.8 EXPLAIN ANALYZE

`EXPLAIN ANALYZE` executes the query and reports actual execution information.

```sql
EXPLAIN ANALYZE
SELECT *
FROM employees
WHERE email = 'asha@example.com';
```

It helps compare estimates with actual behavior.

Look at:

- Estimated rows vs actual rows.
- Actual execution time.
- Number of loops.
- Scan and join types.
- Whether a sort or sequential scan is expensive.

**Caution:** Because `EXPLAIN ANALYZE` executes the statement, be careful with `UPDATE`, `DELETE`, or other statements that change data. Use a safe transaction and roll it back when appropriate, or analyze a read-only equivalent.

## 6.9 Practical query optimization checklist

1. Retrieve only required columns instead of using `SELECT *` in production-facing queries.
2. Filter early where it is logically correct.
3. Ensure join conditions are correct and supported by suitable indexes when needed.
4. Add indexes based on measured query patterns.
5. Avoid applying functions to indexed columns in ways that prevent ordinary index use, unless an appropriate expression index exists.
6. Use pagination for large result sets.
7. Avoid fetching large collections unnecessarily.
8. Inspect query plans before guessing.
9. Measure with realistic data volume and distribution.
10. Recheck performance after schema or workload changes.

Example: Instead of retrieving every employee column:

```sql
SELECT *
FROM employees
WHERE department_id = 10;
```

retrieve only what the screen needs:

```sql
SELECT employee_id, name, email
FROM employees
WHERE department_id = 10;
```

## 6.10 Offset pagination vs keyset pagination

Offset pagination:

```sql
SELECT employee_id, name
FROM employees
ORDER BY employee_id
LIMIT 20 OFFSET 1000;
```

This is simple, but large offsets can require the database to walk past many rows.

Keyset pagination uses the last-seen key:

```sql
SELECT employee_id, name
FROM employees
WHERE employee_id > 1000
ORDER BY employee_id
LIMIT 20;
```

Keyset pagination is often more efficient for deep sequential browsing when the ordering key is stable and indexed. For a non-unique sort column, use a deterministic tie-breaker, for example `(created_at, employee_id)`.

### Interview answer: Why can too many indexes be harmful?

Indexes consume storage and must be maintained when rows are inserted, updated, or deleted. Too many indexes can increase write cost and complicate maintenance. Index only where measured query patterns justify it.

---

# 7. Transactions and ACID Properties

## 7.1 What is a transaction?

A transaction is a logical unit of database work. It groups operations so that they can be committed together or rolled back.

Consider assigning an employee to a trip:

1. Check that the trip exists.
2. Check that the employee is eligible.
3. Insert the trip-employee assignment.
4. Update the trip or capacity information if required.

If the business operation requires all these database changes to succeed together, they should be coordinated in a transaction.

```sql
BEGIN;

INSERT INTO trip_employees (trip_id, employee_id, pickup_sequence)
VALUES (1001, 101, 1);

UPDATE trips
SET status = 'ASSIGNED'
WHERE trip_id = 1001;

COMMIT;
```

If an operation fails and the transaction is rolled back, the database does not retain the partial changes from that transaction.

## 7.2 ACID properties

ACID describes four important transaction properties.

### A — Atomicity

All operations in a transaction succeed together, or the transaction is rolled back.

Example: A trip assignment should not be half-created if the required related database operation fails.

### C — Consistency

A transaction must preserve database rules and constraints.

Example: A foreign key should not allow an assignment to reference a nonexistent trip.

Consistency depends on correctly defined constraints and application rules.

### I — Isolation

Concurrent transactions should not interfere in ways prohibited by the selected isolation level.

Example: Two administrators should not both allocate the last available seat based on stale data without suitable concurrency control.

### D — Durability

After a transaction commits successfully, its changes should survive failures according to the database's durability guarantees and configuration.

## 7.3 COMMIT and ROLLBACK

`COMMIT` makes the transaction's changes permanent according to the database's transaction semantics.

```sql
BEGIN;

UPDATE vehicles
SET status = 'IN_USE'
WHERE vehicle_id = 5;

COMMIT;
```

`ROLLBACK` cancels uncommitted changes:

```sql
BEGIN;

DELETE FROM trips
WHERE trip_id = 1001;

ROLLBACK;
```

After rollback, the delete is undone.

## 7.4 SAVEPOINT

A savepoint creates a point inside a transaction to which you can roll back without cancelling the entire transaction.

```sql
BEGIN;

UPDATE employees
SET department_id = 20
WHERE employee_id = 101;

SAVEPOINT after_employee_update;

UPDATE vehicles
SET status = 'IN_USE'
WHERE vehicle_id = 5;

ROLLBACK TO SAVEPOINT after_employee_update;

COMMIT;
```

The vehicle update is rolled back to the savepoint; the earlier employee update remains part of the transaction and is committed.

## 7.5 Transaction isolation levels

Isolation levels control what one transaction can observe from other concurrent transactions.

Common phenomena:

### Dirty read

A transaction reads another transaction's uncommitted data.

### Non-repeatable read

A transaction reads the same row twice and sees different committed values because another transaction updated it between the reads.

### Phantom read

A transaction repeats a query over a condition and sees a different set of rows because another transaction inserted or deleted matching rows.

### Isolation level overview

| Isolation level | Dirty reads | Non-repeatable reads | Phantom reads |
|---|---|---|---|
| Read Uncommitted | Standard definition: possible | Possible | Possible |
| Read Committed | Prevented | Possible | Possible |
| Repeatable Read | Prevented | Prevented | Standard definition: possible |
| Serializable | Prevented | Prevented | Prevented |

**PostgreSQL-specific behavior:**

- PostgreSQL's `READ UNCOMMITTED` behaves as `READ COMMITTED`.
- PostgreSQL's default isolation level is `READ COMMITTED`.
- PostgreSQL `REPEATABLE READ` uses snapshot isolation and prevents phantom reads in PostgreSQL's implementation, which is stronger than the minimum behavior required by the SQL standard.
- `SERIALIZABLE` provides the strongest isolation, but a transaction may be aborted with a serialization failure. Applications must be prepared to retry appropriate transactions.

Example:

```sql
BEGIN TRANSACTION ISOLATION LEVEL REPEATABLE READ;

SELECT *
FROM trips
WHERE trip_date = DATE '2026-11-08';

COMMIT;
```

## 7.6 Optimistic locking

Optimistic locking assumes conflicts are relatively uncommon. It detects a conflicting update rather than holding a lock for the entire period of user interaction.

In JPA:

```java
@Entity
public class Vehicle {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String status;

    @Version
    private Long version;
}
```

Hibernate includes the version in the update condition. If another transaction has already changed the version, the update affects no matching version and an optimistic-locking conflict is reported.

Use optimistic locking when concurrent conflicts are possible but not extremely frequent.

## 7.7 Pessimistic locking

Pessimistic locking obtains a database lock to prevent conflicting operations while a transaction is working.

PostgreSQL example:

```sql
BEGIN;

SELECT *
FROM vehicles
WHERE vehicle_id = 5
FOR UPDATE;

-- Check and update the vehicle inside this transaction.

COMMIT;
```

`FOR UPDATE` locks selected rows against conflicting updates and row-locking operations until the transaction ends.

Use it carefully: long-running transactions can block other requests and increase deadlock risk.

## 7.8 Spring Boot `@Transactional`

Spring provides declarative transaction management.

```java
@Service
public class TripAssignmentService {

    private final TripRepository tripRepository;
    private final TripEmployeeRepository assignmentRepository;

    public TripAssignmentService(
            TripRepository tripRepository,
            TripEmployeeRepository assignmentRepository) {
        this.tripRepository = tripRepository;
        this.assignmentRepository = assignmentRepository;
    }

    @Transactional
    public void assignEmployee(Long tripId, Long employeeId) {
        Trip trip = tripRepository.findById(tripId)
            .orElseThrow(() -> new IllegalArgumentException("Trip not found"));

        // Apply business validations.
        // Create and save the assignment.
        // Related database changes participate in the transaction.
    }
}
```

Important points:

- Spring commonly implements declarative transactions using proxies.
- A call that bypasses the proxy, such as self-invocation from one method to another method in the same object, may not trigger the expected transactional advice.
- By default, Spring rolls back on unchecked exceptions (`RuntimeException`) and `Error`, but not on every checked exception.
- `rollbackFor` can customize rollback rules.
- Keep transactions focused and avoid holding database transactions open while waiting for slow external network calls.
- A transaction does not automatically make an operation across multiple independent services atomic.

Example custom rollback rule:

```java
@Transactional(rollbackFor = Exception.class)
public void processAssignment() throws Exception {
    // Database work
}
```

### Interview answer: Optimistic vs pessimistic locking

Optimistic locking uses a version or similar check to detect concurrent modification when updating. Pessimistic locking obtains a database lock to prevent conflicting modifications while the transaction is active. Optimistic locking suits lower-contention workloads; pessimistic locking can suit critical operations with higher conflict risk, but it can reduce concurrency.

---

# 8. Database Normalization and Design

## 8.1 What is normalization?

Normalization is the process of organizing relational data to reduce unnecessary duplication and prevent data anomalies.

Consider an unnormalized table:

| trip_id | employee_name | employee_email | driver_name | vehicle_number |
|---:|---|---|---|---|
| 1001 | Asha | asha@example.com | Ravi | HR01AB1234 |
| 1001 | Neha | neha@example.com | Ravi | HR01AB1234 |
| 1002 | Kabir | kabir@example.com | Meera | HR02CD5678 |

Driver and vehicle details are repeated for every employee assigned to a trip.

This can create update, insertion, and deletion anomalies.

## 8.2 Data anomalies

### Update anomaly

If Ravi's name or other details change, multiple rows may need updating. Missing one row creates inconsistent data.

### Insertion anomaly

It may be difficult to store a driver or vehicle before a trip exists if the design forces all information into one table.

### Deletion anomaly

Deleting the only trip row containing a vehicle's details may accidentally remove the only stored copy of those details.

Normalization helps separate facts into appropriate tables.

## 8.3 First Normal Form (1NF)

A table is in 1NF when each cell contains a single value for the intended attribute and repeating groups are not stored inside a single column.

Bad design:

| employee_id | employee_name | phone_numbers |
|---:|---|---|
| 101 | Asha | 9999000011, 9999000022 |

Better design:

**employees**

| employee_id | employee_name |
|---:|---|
| 101 | Asha |

**employee_phone_numbers**

| employee_id | phone_number |
|---:|---|
| 101 | 9999000011 |
| 101 | 9999000022 |

The correct model depends on the business meaning of the values.

## 8.4 Second Normal Form (2NF)

A table is in 2NF when:

1. It is in 1NF.
2. Every non-key attribute is fully dependent on the whole candidate key, not just part of a composite candidate key.

Example of a problematic table:

```text
trip_employees(
    trip_id,
    employee_id,
    trip_date,
    employee_name,
    pickup_sequence
)
```

Suppose the key is `(trip_id, employee_id)`.

- `pickup_sequence` may depend on the combination.
- `trip_date` depends only on `trip_id`.
- `employee_name` depends only on `employee_id`.

The last two attributes have partial dependencies and should usually be stored in their respective parent tables.

## 8.5 Third Normal Form (3NF)

A table is in 3NF when it is in 2NF and non-key attributes do not depend transitively on a candidate key through another non-key attribute, subject to the formal 3NF definition.

Problematic example:

```text
employees(
    employee_id,
    employee_name,
    department_id,
    department_name
)
```

If `department_id` determines `department_name`, then department name depends on employee ID through department ID.

Better design:

**employees**

```text
employee_id
employee_name
department_id
```

**departments**

```text
department_id
department_name
```

Now department information is stored once.

## 8.6 BCNF — Boyce-Codd Normal Form

BCNF is stricter than 3NF.

A relation is in BCNF when every determinant is a superkey.

For most entry-level backend interviews, know that BCNF addresses certain dependency cases that can remain in 3NF. Be prepared to explain the rule rather than memorizing a complicated example without understanding the functional dependencies.

## 8.7 Normalized Employee Transport schema

A simplified design might contain:

### departments

```sql
CREATE TABLE departments (
    department_id BIGINT GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    department_name VARCHAR(100) NOT NULL UNIQUE
);
```

### employees

```sql
CREATE TABLE employees (
    employee_id BIGINT GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(150) NOT NULL UNIQUE,
    department_id BIGINT REFERENCES departments(department_id)
);
```

### drivers

```sql
CREATE TABLE drivers (
    driver_id BIGINT GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    driver_name VARCHAR(100) NOT NULL,
    status VARCHAR(30) NOT NULL
);
```

### vehicles

```sql
CREATE TABLE vehicles (
    vehicle_id BIGINT GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    registration_number VARCHAR(30) NOT NULL UNIQUE,
    seating_capacity INTEGER NOT NULL CHECK (seating_capacity > 0),
    status VARCHAR(30) NOT NULL
);
```

### trips

```sql
CREATE TABLE trips (
    trip_id BIGINT GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    trip_date DATE NOT NULL,
    shift VARCHAR(30) NOT NULL,
    driver_id BIGINT REFERENCES drivers(driver_id),
    vehicle_id BIGINT REFERENCES vehicles(vehicle_id),
    status VARCHAR(30) NOT NULL
);
```

### trip_employees

```sql
CREATE TABLE trip_employees (
    trip_id BIGINT NOT NULL REFERENCES trips(trip_id),
    employee_id BIGINT NOT NULL REFERENCES employees(employee_id),
    pickup_sequence INTEGER NOT NULL CHECK (pickup_sequence > 0),
    PRIMARY KEY (trip_id, employee_id),
    UNIQUE (trip_id, pickup_sequence)
);
```

This design separates:

- Employee information from department information.
- Driver information from trip information.
- Vehicle information from trip information.
- Trip information from employee assignment information.

The `trip_employees` table models the many-to-many relationship between trips and employees and stores relationship-specific data such as pickup sequence.

A real application may need additional constraints and entities based on business requirements, such as shift definitions, routes, pickup locations, trip events, or assignment history.

## 8.8 Denormalization

Denormalization intentionally stores some duplicated or precomputed data to improve read performance or simplify a particular workload.

Examples:

- A reporting table containing pre-aggregated trip counts.
- A materialized view for expensive reports.
- A snapshot of an employee's name on a historical trip record when the historical display must not change after the employee profile is edited.

Trade-offs:

- Faster or simpler reads in some cases.
- More storage.
- More complicated writes.
- Risk of inconsistent duplicated values if updates are not coordinated.

Normalize transactional data by default, then denormalize only when a measured requirement justifies it.

### Interview answer: Why do we normalize a database?

Normalization reduces unnecessary duplication and prevents insertion, update, and deletion anomalies. It separates facts into related tables and uses keys to preserve relationships. Denormalization may be introduced later for measured performance or reporting needs.

---

# 9. Important Interview Questions and Answers

## Q1. What is the difference between DBMS and RDBMS?

A DBMS is software for managing data. An RDBMS is a DBMS that organizes data into related tables and supports relational concepts such as keys and constraints. PostgreSQL is an RDBMS.

## Q2. What is the difference between SQL and PostgreSQL?

SQL is a language. PostgreSQL is a database system that executes SQL and provides storage, transactions, indexes, constraints, and other database services.

## Q3. What are DDL, DML, DQL, DCL, and TCL?

- DDL defines database structures.
- DML inserts, updates, and deletes data.
- DQL retrieves data, commonly using `SELECT`.
- DCL manages privileges.
- TCL manages transaction boundaries.

## Q4. DELETE vs TRUNCATE vs DROP?

`DELETE` removes rows and supports `WHERE`. `TRUNCATE` removes all rows while retaining the table structure. `DROP` removes the table object itself. PostgreSQL supports transactional `TRUNCATE`, but behavior and details differ across database products.

## Q5. What is a primary key?

A primary key uniquely identifies each row and cannot contain `NULL`. It can consist of one column or multiple columns.

## Q6. What is a foreign key?

A foreign key enforces a relationship between a child table and a referenced key in another table. It helps prevent orphan references.

## Q7. What is a composite key?

A composite key uses multiple columns together to identify a row uniquely. For example, `(trip_id, employee_id)` can identify an employee's assignment to a trip.

## Q8. INNER JOIN vs LEFT JOIN?

An inner join returns only matching rows. A left join preserves every row from the left table and supplies `NULL` for right-side columns when no match exists.

## Q9. What is a self join?

A self join joins a table to itself, usually using different aliases. It is useful for employee-manager hierarchies.

## Q10. JOIN vs UNION?

A join combines columns from related rows. A union combines rows from compatible query results. `UNION` removes duplicates; `UNION ALL` keeps them.

## Q11. What is a subquery?

A subquery is a query nested inside another SQL statement. It can return a scalar value, a set of rows, or be used with `EXISTS`.

## Q12. What is a correlated subquery?

A correlated subquery refers to a column from the outer query. Its result is related to the current outer row, although the optimizer may transform the execution plan.

## Q13. What is a CTE?

A CTE is a named query result defined with `WITH`. It improves readability and supports recursive queries. It is not automatically faster than a subquery.

## Q14. What is an index?

An index is a data structure that helps locate rows efficiently for supported query patterns. It can improve reads but consumes storage and adds write-maintenance cost.

## Q15. What is a composite index?

A composite index contains multiple columns. Column order matters because the leading columns generally determine which query filters can use the index efficiently.

## Q16. What is EXPLAIN ANALYZE?

In PostgreSQL, `EXPLAIN ANALYZE` executes a query and reports its actual execution plan and timing information, allowing comparison between estimated and actual row counts and costs.

## Q17. What are ACID properties?

Atomicity, Consistency, Isolation, and Durability. They describe key guarantees of database transactions.

## Q18. What is a dirty read?

A dirty read occurs when a transaction reads data written by another transaction that has not committed. PostgreSQL does not permit dirty reads; its `READ UNCOMMITTED` behaves as `READ COMMITTED`.

## Q19. What is the default isolation level in PostgreSQL?

`READ COMMITTED`.

## Q20. Optimistic vs pessimistic locking?

Optimistic locking detects conflicting updates, commonly through a version column. Pessimistic locking obtains a database lock to prevent conflicting modifications while the transaction is active.

## Q21. What is normalization?

Normalization organizes relational data to reduce duplication and avoid insertion, update, and deletion anomalies.

## Q22. What is the difference between 2NF and 3NF?

2NF removes partial dependency of non-key attributes on part of a composite candidate key. 3NF additionally addresses transitive dependencies of non-key attributes on candidate keys, subject to the formal definition.

## Q23. Why should every column not be indexed?

Indexes require storage and add work to inserts, updates, and deletes. Indexes should be designed around actual query patterns and verified with query plans.

## Q24. Does using an index always make a query faster?

No. A sequential scan may be cheaper for small tables or queries returning a large portion of rows. The optimizer chooses a plan based on statistics, cost estimates, and query shape.

---

# 10. Quick Revision Sheet

| Topic | Remember |
|---|---|
| DBMS | Software to manage databases |
| RDBMS | Stores related data in tables |
| SQL | Language used to work with relational databases |
| JDBC | Java database connectivity API |
| JPA | Java persistence specification |
| Hibernate | ORM and JPA implementation |
| Spring Data JPA | Repository abstraction |
| DDL | CREATE, ALTER, DROP, TRUNCATE |
| DML | INSERT, UPDATE, DELETE |
| DQL | SELECT |
| DCL | GRANT, REVOKE |
| TCL | BEGIN, COMMIT, ROLLBACK, SAVEPOINT |
| Primary key | Unique and not NULL |
| Foreign key | Enforces a relationship |
| UNIQUE | Prevents duplicate constrained values |
| NOT NULL | Requires a value |
| CHECK | Enforces a condition |
| INNER JOIN | Only matching rows |
| LEFT JOIN | All left rows plus matching right rows |
| RIGHT JOIN | All right rows plus matching left rows |
| FULL OUTER JOIN | All rows from both sides |
| CROSS JOIN | Cartesian product |
| SELF JOIN | A table joined to itself |
| Subquery | Nested query |
| EXISTS | Tests whether a matching row exists |
| CTE | Named query result using WITH |
| Index | Speeds up suitable lookups; adds write/storage cost |
| EXPLAIN | Shows estimated query plan |
| EXPLAIN ANALYZE | Executes and reports actual plan details |
| ACID | Atomicity, Consistency, Isolation, Durability |
| PostgreSQL default isolation | READ COMMITTED |
| Optimistic lock | Detect conflict, often using `@Version` |
| Pessimistic lock | Lock rows during transaction |
| 1NF | Atomic values; no repeating groups |
| 2NF | No partial dependency on part of candidate key |
| 3NF | No problematic transitive dependency |
| BCNF | Every determinant is a superkey |

---

# 11. Practical SQL Patterns to Keep

```sql
-- 1. Select specific columns
SELECT employee_id, name
FROM employees;

-- 2. Filter and sort
SELECT employee_id, name
FROM employees
WHERE department_id = 10
ORDER BY name;

-- 3. Join employees with departments
SELECT e.name, d.department_name
FROM employees e
INNER JOIN departments d
    ON e.department_id = d.department_id;

-- 4. Find departments without employees
SELECT d.*
FROM departments d
WHERE NOT EXISTS (
    SELECT 1
    FROM employees e
    WHERE e.department_id = d.department_id
);

-- 5. Find employees above the company average salary
SELECT employee_id, name, salary
FROM employees
WHERE salary > (
    SELECT AVG(salary)
    FROM employees
);

-- 6. Count employees by department
SELECT department_id, COUNT(*) AS employee_count
FROM employees
GROUP BY department_id;

-- 7. Inspect a query plan
EXPLAIN
SELECT *
FROM trips
WHERE trip_date = DATE '2026-11-08';

-- 8. Create a query-supporting index (only after validating need)
CREATE INDEX idx_trips_date_shift
ON trips(trip_date, shift);

-- 9. Transaction example
BEGIN;
UPDATE vehicles
SET status = 'IN_USE'
WHERE vehicle_id = 5;
COMMIT;
```

---

# 12. Day 11 Completion Checklist

Use this checklist to confirm that the learning material has been covered. It is not a test.

- [ ] I can explain DBMS, RDBMS, SQL, PostgreSQL, JDBC, JPA, Hibernate, and Spring Data JPA.
- [ ] I can explain how a Spring Boot request reaches PostgreSQL.
- [ ] I can distinguish DDL, DML, DQL, DCL, and TCL.
- [ ] I understand `DELETE`, `TRUNCATE`, and `DROP`.
- [ ] I can explain primary keys, foreign keys, unique constraints, and composite keys.
- [ ] I can write INNER, LEFT, RIGHT, FULL OUTER, CROSS, and SELF joins.
- [ ] I understand subqueries, correlated subqueries, `IN`, `EXISTS`, and CTEs.
- [ ] I understand why indexes help and why too many indexes are harmful.
- [ ] I can explain `EXPLAIN` and `EXPLAIN ANALYZE`.
- [ ] I can explain transactions and all four ACID properties.
- [ ] I understand isolation levels and PostgreSQL's default `READ COMMITTED`.
- [ ] I can distinguish optimistic and pessimistic locking.
- [ ] I can explain 1NF, 2NF, 3NF, and BCNF at interview level.
- [ ] I can describe a normalized Employee Transport database design.

---

## Day 11 takeaway

A backend developer should understand more than how to call `repository.save()` or write a simple `SELECT`. Strong database fundamentals mean knowing how data is structured, how tables relate, how queries retrieve records, how indexes affect performance, and how transactions protect correctness when multiple requests execute at the same time.

**Next in the learning sequence:** Day 12 — Advanced SQL: aggregate functions, `GROUP BY`, `HAVING`, window functions, ranking, `LEAD`/`LAG`, views, stored procedures, and triggers.
"""

path = Path("/mnt/data/Day_11_SQL_DBMS_Fundamentals.md")
path.write_text(content, encoding="utf-8")
word_count = len(content.split())
char_count = len(content)
print(f"Created: {path}\nWords: {word_count:,}\nCharacters: {char_count:,}")
