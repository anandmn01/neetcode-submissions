from pathlib import Path

content = r"""# Day 12 — Advanced SQL

> **Java Backend Developer Interview Preparation**  
> Focus: Aggregate functions, `GROUP BY`, `HAVING`, window functions, ranking, `LEAD`/`LAG`, running totals, views, materialized views, stored procedures, functions, and triggers.  
> Examples use PostgreSQL and an Employee Transport application.

---

## Learning objectives

By the end of this day, you should be able to:

- Summarize rows using aggregate functions.
- Group data and filter grouped results correctly.
- Explain the difference between `WHERE` and `HAVING`.
- Use window functions without collapsing individual rows.
- Rank records using `ROW_NUMBER`, `RANK`, and `DENSE_RANK`.
- Compare rows using `LAG` and `LEAD`, and calculate running totals.
- Explain views and materialized views.
- Understand database functions, stored procedures, and triggers.
- Recognize when database-side aggregation is more appropriate than processing large result sets in Java.

---

# 1. Aggregate Functions

## 1.1 What is an aggregate function?

An aggregate function takes values from multiple rows and produces a summary value.

For example, an Employee Transport system may need to know:

- Total number of trips.
- Average number of employees assigned per trip.
- Highest vehicle seating capacity.
- Total number of active drivers.
- Minimum and maximum trip dates.

SQL provides aggregate functions for these tasks.

## 1.2 Common aggregate functions

| Function | Purpose |
|---|---|
| `COUNT()` | Counts rows or non-NULL values |
| `SUM()` | Adds numeric values |
| `AVG()` | Calculates an average |
| `MIN()` | Finds the minimum value |
| `MAX()` | Finds the maximum value |

Example table: `vehicles`

| vehicle_id | registration_number | seating_capacity |
|---:|---|---:|
| 1 | HR01AB1234 | 4 |
| 2 | HR02CD5678 | 6 |
| 3 | HR03EF9012 | 8 |

Find the total number of vehicles:

```sql
SELECT COUNT(*) AS total_vehicles
FROM vehicles;
```

Find the total seating capacity:

```sql
SELECT SUM(seating_capacity) AS total_seats
FROM vehicles;
```

Find the average seating capacity:

```sql
SELECT AVG(seating_capacity) AS average_capacity
FROM vehicles;
```

Find the smallest and largest capacities:

```sql
SELECT
    MIN(seating_capacity) AS minimum_capacity,
    MAX(seating_capacity) AS maximum_capacity
FROM vehicles;
```

## 1.3 COUNT(*) vs COUNT(column)

This distinction is important.

```sql
SELECT COUNT(*)
FROM employees;
```

`COUNT(*)` counts rows, including rows where some columns contain `NULL`.

```sql
SELECT COUNT(phone_number)
FROM employees;
```

`COUNT(phone_number)` counts only rows where `phone_number` is not `NULL`.

Example:

| employee_id | name | phone_number |
|---:|---|---|
| 101 | Asha | 9999000011 |
| 102 | Ravi | NULL |
| 103 | Neha | 9999000033 |

Results:

- `COUNT(*)` = 3
- `COUNT(phone_number)` = 2

## 1.4 COUNT(DISTINCT column)

`DISTINCT` removes duplicate values before counting.

```sql
SELECT COUNT(DISTINCT department_id)
FROM employees;
```

This counts the number of distinct non-NULL department IDs represented in the employee table.

To inspect the distinct values:

```sql
SELECT DISTINCT department_id
FROM employees;
```

## 1.5 How aggregate functions handle NULL

Most aggregate functions ignore `NULL` inputs.

For example:

| seating_capacity |
|---:|
| 4 |
| 6 |
| NULL |

`AVG(seating_capacity)` calculates the average of 4 and 6, not 4, 6, and zero.

`COUNT(column)` ignores NULL values. `COUNT(*)` counts the rows.

If a NULL should be treated as a specific value, use `COALESCE` deliberately:

```sql
SELECT AVG(COALESCE(seating_capacity, 0))
FROM vehicles;
```

This changes the meaning of the calculation: NULL capacities are now treated as zero. That may or may not be correct for the business.

## 1.6 COALESCE

`COALESCE` returns the first non-NULL expression.

```sql
SELECT
    employee_id,
    COALESCE(phone_number, 'Not provided') AS phone
FROM employees;
```

It is useful for reports and response formatting.

**Interview point:** Do not confuse `COALESCE` with an aggregate function. It is a general SQL expression for handling NULL values.

---

# 2. GROUP BY

## 2.1 What is GROUP BY?

`GROUP BY` groups rows that share the same value or combination of values. Aggregate functions can then calculate a result for each group.

Suppose the `trips` table contains:

| trip_id | driver_id | status |
|---:|---:|---|
| 1001 | 1 | COMPLETED |
| 1002 | 1 | COMPLETED |
| 1003 | 2 | CANCELLED |
| 1004 | 2 | COMPLETED |
| 1005 | 3 | SCHEDULED |

Count trips for each driver:

```sql
SELECT
    driver_id,
    COUNT(*) AS total_trips
FROM trips
GROUP BY driver_id;
```

Conceptual result:

| driver_id | total_trips |
|---:|---:|
| 1 | 2 |
| 2 | 2 |
| 3 | 1 |

The database forms groups by `driver_id`, then calculates `COUNT(*)` for each group.

## 2.2 GROUP BY with multiple columns

You can group by more than one column.

Count trips by driver and status:

```sql
SELECT
    driver_id,
    status,
    COUNT(*) AS trip_count
FROM trips
GROUP BY driver_id, status
ORDER BY driver_id, status;
```

Each distinct `(driver_id, status)` combination becomes a group.

## 2.3 GROUP BY with SUM and AVG

Example: calculate total and average vehicle capacity by vehicle type.

```sql
SELECT
    vehicle_type,
    COUNT(*) AS vehicle_count,
    SUM(seating_capacity) AS total_capacity,
    AVG(seating_capacity) AS average_capacity
FROM vehicles
GROUP BY vehicle_type;
```

## 2.4 GROUP BY rules

A common SQL rule is:

Every selected expression must either:
- Be part of the `GROUP BY`, or
- Be used inside an aggregate function, or
- Be functionally dependent on grouped columns where the database recognizes that dependency.

Incorrect in a typical query:

```sql
SELECT driver_id, status, COUNT(*)
FROM trips
GROUP BY driver_id;
```

`status` is neither grouped nor aggregated, so SQL cannot determine which status to show for a driver who has trips with multiple statuses.

Correct:

```sql
SELECT driver_id, status, COUNT(*)
FROM trips
GROUP BY driver_id, status;
```

Or, if status is not needed:

```sql
SELECT driver_id, COUNT(*)
FROM trips
GROUP BY driver_id;
```

## 2.5 GROUP BY and ORDER BY

`GROUP BY` creates groups. `ORDER BY` sorts the final output.

```sql
SELECT
    driver_id,
    COUNT(*) AS total_trips
FROM trips
GROUP BY driver_id
ORDER BY total_trips DESC;
```

This shows drivers with the highest trip count first.

---

# 3. HAVING vs WHERE

This is one of the most common SQL interview questions.

## 3.1 WHERE

`WHERE` filters individual rows before grouping and aggregation.

Example: count completed trips for each driver.

```sql
SELECT
    driver_id,
    COUNT(*) AS completed_trips
FROM trips
WHERE status = 'COMPLETED'
GROUP BY driver_id;
```

The database first filters to completed trips, then groups the remaining rows by driver.

## 3.2 HAVING

`HAVING` filters groups after grouping and aggregation.

Example: find drivers with more than five completed trips.

```sql
SELECT
    driver_id,
    COUNT(*) AS completed_trips
FROM trips
WHERE status = 'COMPLETED'
GROUP BY driver_id
HAVING COUNT(*) > 5;
```

The query:
1. Filters rows to completed trips.
2. Groups them by driver.
3. Counts trips in each group.
4. Keeps only groups whose count is greater than five.

## 3.3 WHERE vs HAVING comparison

| WHERE | HAVING |
|---|---|
| Filters individual rows | Filters grouped results |
| Applied before grouping | Applied after grouping |
| Usually does not use aggregate conditions directly | Commonly uses aggregate conditions |
| Can be used without `GROUP BY` | Commonly used with `GROUP BY` |

You can use both in one query:

```sql
SELECT
    driver_id,
    COUNT(*) AS completed_trips
FROM trips
WHERE status = 'COMPLETED'
  AND trip_date >= DATE '2026-01-01'
GROUP BY driver_id
HAVING COUNT(*) >= 10
ORDER BY completed_trips DESC;
```

This filters the source rows first, then filters the resulting driver groups.

## 3.4 Simplified logical SQL processing order

A useful conceptual order is:

```text
FROM / JOIN
      |
      v
WHERE
      |
      v
GROUP BY
      |
      v
HAVING
      |
      v
SELECT
      |
      v
DISTINCT
      |
      v
ORDER BY
      |
      v
LIMIT / OFFSET
```

This is a logical model for understanding query semantics, not necessarily the physical execution order chosen by the query optimizer.

### Interview answer: WHERE vs HAVING

`WHERE` filters rows before aggregation. `HAVING` filters groups after aggregation. For example, use `WHERE status = 'COMPLETED'` to include only completed trips, and `HAVING COUNT(*) > 5` to retain drivers with more than five such trips.

---

# 4. Window Functions

## 4.1 What is a window function?

A window function performs a calculation across a set of related rows while preserving the individual rows in the output.

This is the key difference from ordinary aggregation.

With `GROUP BY`, several rows become one result row per group.

With a window function, the original rows remain, and a calculated value is added to each row.

Example table: `employees`

| employee_id | name | department_id | salary |
|---:|---|---:|---:|
| 101 | Asha | 10 | 70000 |
| 102 | Ravi | 10 | 60000 |
| 103 | Neha | 20 | 80000 |
| 104 | Kabir | 20 | 50000 |

## 4.2 OVER clause

Window functions use the `OVER` clause to define the window of rows used for a calculation.

```sql
SELECT
    employee_id,
    name,
    salary,
    AVG(salary) OVER () AS company_average
FROM employees;
```

Every employee row is retained. The company average is repeated on each row.

## 4.3 PARTITION BY

`PARTITION BY` divides the rows into groups for the window calculation, without collapsing them.

```sql
SELECT
    employee_id,
    name,
    department_id,
    salary,
    AVG(salary) OVER (
        PARTITION BY department_id
    ) AS department_average
FROM employees;
```

Each employee receives the average salary for their department.

Conceptually:

| name | department_id | salary | department_average |
|---|---:|---:|---:|
| Asha | 10 | 70000 | 65000 |
| Ravi | 10 | 60000 | 65000 |
| Neha | 20 | 80000 | 65000 |
| Kabir | 20 | 50000 | 65000 |

## 4.4 ORDER BY inside OVER

`ORDER BY` inside the window definition controls the order used by order-sensitive window calculations.

```sql
SELECT
    employee_id,
    name,
    salary,
    ROW_NUMBER() OVER (
        ORDER BY salary DESC
    ) AS salary_position
FROM employees;
```

This assigns a row number according to salary, highest first.

The `ORDER BY` inside `OVER` does not guarantee the final display order of the query. Add an outer `ORDER BY` when output ordering matters.

## 4.5 GROUP BY vs window functions

| GROUP BY | Window function |
|---|---|
| Produces one row per group | Preserves input rows |
| Aggregates collapse rows | Calculations appear alongside each row |
| Useful for summary reports | Useful for row-level comparisons and ranking |
| Example: total trips per driver | Example: each trip plus the driver's total trip count |

Example with GROUP BY:

```sql
SELECT department_id, AVG(salary)
FROM employees
GROUP BY department_id;
```

Example with a window function:

```sql
SELECT
    employee_id,
    name,
    department_id,
    salary,
    AVG(salary) OVER (
        PARTITION BY department_id
    ) AS department_average
FROM employees;
```

## 4.6 Explicit window frames

For running calculations, understand the window frame.

Example:

```sql
SELECT
    trip_date,
    trip_id,
    COUNT(*) OVER (
        ORDER BY trip_date, trip_id
        ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
    ) AS trips_so_far
FROM trips;
```

The frame means: from the first row in the ordered window through the current row.

An explicit `ROWS` frame can avoid surprises involving the default frame and peer rows with equal ordering values. Include a deterministic tie-breaker such as `trip_id` when row-by-row ordering matters.

---

# 5. ROW_NUMBER, RANK, and DENSE_RANK

These ranking functions are frequently asked in interviews.

Assume employee salaries:

| employee | salary |
|---|---:|
| Asha | 90000 |
| Ravi | 80000 |
| Neha | 80000 |
| Kabir | 70000 |

## 5.1 ROW_NUMBER()

Assigns a unique sequential number to each row in the window ordering.

```sql
SELECT
    name,
    salary,
    ROW_NUMBER() OVER (
        ORDER BY salary DESC, employee_id
    ) AS row_num
FROM employees;
```

Conceptual output:

| name | salary | row_num |
|---|---:|---:|
| Asha | 90000 | 1 |
| Ravi | 80000 | 2 |
| Neha | 80000 | 3 |
| Kabir | 70000 | 4 |

Even when salaries tie, each row gets a different number. The extra `employee_id` ordering makes the result deterministic.

## 5.2 RANK()

Rows with equal ordering values receive the same rank. The next rank contains a gap.

```sql
SELECT
    name,
    salary,
    RANK() OVER (
        ORDER BY salary DESC
    ) AS salary_rank
FROM employees;
```

Conceptual output:

| name | salary | salary_rank |
|---|---:|---:|
| Asha | 90000 | 1 |
| Ravi | 80000 | 2 |
| Neha | 80000 | 2 |
| Kabir | 70000 | 4 |

## 5.3 DENSE_RANK()

Rows with equal values receive the same rank, but there is no gap after a tie.

```sql
SELECT
    name,
    salary,
    DENSE_RANK() OVER (
        ORDER BY salary DESC
    ) AS salary_rank
FROM employees;
```

Conceptual output:

| name | salary | salary_rank |
|---|---:|---:|
| Asha | 90000 | 1 |
| Ravi | 80000 | 2 |
| Neha | 80000 | 2 |
| Kabir | 70000 | 3 |

## 5.4 Comparison

| Function | Tie behavior | Result after a tie |
|---|---|---|
| `ROW_NUMBER()` | Different number for every row | No gap |
| `RANK()` | Same rank for tied rows | Gap |
| `DENSE_RANK()` | Same rank for tied rows | No gap |

## 5.5 Find the second-highest distinct salary

Use `DENSE_RANK()` because the question asks for the second-highest distinct salary.

```sql
WITH salary_ranking AS (
    SELECT
        employee_id,
        name,
        salary,
        DENSE_RANK() OVER (
            ORDER BY salary DESC
        ) AS salary_rank
    FROM employees
)
SELECT employee_id, name, salary
FROM salary_ranking
WHERE salary_rank = 2;
```

This returns all employees who earn the second-highest distinct salary.

If the question instead asks for the second row after sorting, `ROW_NUMBER()` may be appropriate. Clarify the meaning of “second highest” in an interview.

## 5.6 Top N employees per department

```sql
WITH ranked_employees AS (
    SELECT
        employee_id,
        name,
        department_id,
        salary,
        ROW_NUMBER() OVER (
            PARTITION BY department_id
            ORDER BY salary DESC, employee_id
        ) AS row_num
    FROM employees
)
SELECT *
FROM ranked_employees
WHERE row_num <= 3;
```

This returns up to three employees per department. `ROW_NUMBER()` ensures at most three rows per department. If all salary ties at the cutoff should be retained, use `RANK()` or `DENSE_RANK()` according to the desired rule.

---

# 6. LEAD, LAG, and Running Totals

## 6.1 LAG()

`LAG()` accesses a value from a preceding row in the window ordering.

Example: compare each trip's date with the previous trip date for the same driver.

```sql
SELECT
    driver_id,
    trip_id,
    trip_date,
    LAG(trip_date) OVER (
        PARTITION BY driver_id
        ORDER BY trip_date, trip_id
    ) AS previous_trip_date
FROM trips;
```

For the first row in each driver's partition, there is no previous row, so the result is `NULL` unless a default is supplied.

## 6.2 LEAD()

`LEAD()` accesses a value from a following row.

```sql
SELECT
    driver_id,
    trip_id,
    trip_date,
    LEAD(trip_date) OVER (
        PARTITION BY driver_id
        ORDER BY trip_date, trip_id
    ) AS next_trip_date
FROM trips;
```

## 6.3 LAG and LEAD syntax

General form:

```sql
LAG(expression, offset, default_value)
OVER (
    PARTITION BY ...
    ORDER BY ...
)
```

- `expression`: value to retrieve.
- `offset`: how many rows backward or forward; defaults to 1.
- `default_value`: value returned when the target row does not exist; defaults to `NULL`.

Example:

```sql
SELECT
    trip_id,
    trip_date,
    LAG(trip_date, 1, DATE '2000-01-01') OVER (
        ORDER BY trip_date, trip_id
    ) AS previous_date
FROM trips;
```

Use a default value only when it has a meaningful business interpretation. An artificial date may be misleading in a real report.

## 6.4 Calculate the difference from the previous row

```sql
SELECT
    driver_id,
    trip_id,
    trip_date,
    trip_date - LAG(trip_date) OVER (
        PARTITION BY driver_id
        ORDER BY trip_date, trip_id
    ) AS days_since_previous_trip
FROM trips;
```

In PostgreSQL, subtracting one `DATE` from another returns the number of days between them.

## 6.5 Running total

A running total calculates an accumulated value from the beginning of the ordered window through the current row.

Assume `trip_payments` contains one row per payment.

```sql
SELECT
    payment_date,
    payment_id,
    amount,
    SUM(amount) OVER (
        ORDER BY payment_date, payment_id
        ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
    ) AS running_total
FROM trip_payments;
```

The explicit frame and tie-breaker make the row-by-row accumulation clear.

## 6.6 Running total per driver

```sql
SELECT
    driver_id,
    payment_date,
    payment_id,
    amount,
    SUM(amount) OVER (
        PARTITION BY driver_id
        ORDER BY payment_date, payment_id
        ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
    ) AS driver_running_total
FROM driver_payments;
```

Each driver's running total starts again at the beginning of that driver's partition.

## 6.7 Practical use cases

- Compare an employee's salary with the previous salary record.
- Find the previous and next trip for a driver.
- Calculate cumulative trip counts.
- Compare daily revenue with the previous day.
- Identify gaps between scheduled trips.
- Rank drivers within a shift or region.

**Important:** Window functions operate on the rows produced by the query's `FROM`, `JOIN`, and `WHERE` stages. If you filter rows before the window calculation, those removed rows are not available to the window function.

---

# 7. Views, Materialized Views, Functions, and Stored Procedures

## 7.1 What is a view?

A view is a named SQL query that can be queried like a table.

Example:

```sql
CREATE VIEW active_drivers AS
SELECT driver_id, driver_name, status
FROM drivers
WHERE status = 'ACTIVE';
```

Query it:

```sql
SELECT *
FROM active_drivers;
```

A normal PostgreSQL view stores the query definition, not a separate stored copy of the query result. When queried, PostgreSQL uses the view definition as part of query planning.

### Why use views?

- Simplify repeated queries.
- Hide complex joins from consumers.
- Present a consistent data interface.
- Restrict exposure to selected columns or rows when combined with appropriate permissions and security settings.

A view is not automatically a security boundary. Permissions, ownership, and PostgreSQL view security behavior must be configured correctly.

## 7.2 What is a materialized view?

A materialized view stores the result of a query physically.

Example:

```sql
CREATE MATERIALIZED VIEW daily_trip_summary AS
SELECT
    trip_date,
    COUNT(*) AS total_trips
FROM trips
GROUP BY trip_date;
```

Query it:

```sql
SELECT *
FROM daily_trip_summary
ORDER BY trip_date;
```

Because the result is stored, reads can be faster for expensive reports. But the data can become stale when the underlying tables change.

Refresh it:

```sql
REFRESH MATERIALIZED VIEW daily_trip_summary;
```

PostgreSQL also supports concurrent refresh when the materialized view has a suitable unique index and other requirements are met:

```sql
REFRESH MATERIALIZED VIEW CONCURRENTLY daily_trip_summary;
```

A concurrent refresh avoids blocking ordinary reads in the same way as a regular refresh, but it has requirements and operational trade-offs. It is not automatic; the application or a scheduled process must refresh the view.

## 7.3 View vs materialized view

| View | Materialized view |
|---|---|
| Stores query definition | Stores query result |
| Reflects current underlying data when queried | Can become stale until refreshed |
| Usually no separate result storage | Uses storage for the materialized result |
| Useful for reusable query logic | Useful for expensive read-heavy reports |

## 7.4 What is a database function?

A function is a named database routine that can accept parameters and return a value or a set of rows.

PostgreSQL example:

```sql
CREATE OR REPLACE FUNCTION get_trip_count(p_driver_id BIGINT)
RETURNS BIGINT
LANGUAGE SQL
AS $$
    SELECT COUNT(*)
    FROM trips
    WHERE driver_id = p_driver_id;
$$;
```

Call it:

```sql
SELECT get_trip_count(1);
```

Functions can encapsulate reusable database logic. PostgreSQL functions can be written in SQL, PL/pgSQL, and other supported languages.

## 7.5 What is a stored procedure?

A stored procedure is a named database routine invoked with `CALL`.

PostgreSQL example:

```sql
CREATE OR REPLACE PROCEDURE mark_trip_completed(p_trip_id BIGINT)
LANGUAGE SQL
AS $$
    UPDATE trips
    SET status = 'COMPLETED'
    WHERE trip_id = p_trip_id;
$$;
```

Call it:

```sql
CALL mark_trip_completed(1001);
```

PostgreSQL procedures and functions have different invocation and transaction-control rules. In particular, a procedure can perform transaction control in supported contexts, while a function cannot independently commit or roll back the surrounding transaction.

## 7.6 Function vs procedure

| Function | Procedure |
|---|---|
| Invoked as part of an expression or query, depending on return type | Invoked using `CALL` |
| Returns a value or set according to its definition | Does not use the same function-return model |
| Cannot independently commit/rollback the surrounding transaction in PostgreSQL | Can perform transaction control in permitted invocation contexts |
| Useful for reusable calculations and query logic | Useful for procedural operations and workflows |

The exact differences vary across database products. This section describes PostgreSQL behavior.

---

# 8. Triggers

## 8.1 What is a trigger?

A trigger is database logic that runs automatically when a configured database event occurs.

Common trigger events include:

- `INSERT`
- `UPDATE`
- `DELETE`
- In PostgreSQL, also `TRUNCATE` for statement-level triggers.

Triggers can be configured to run `BEFORE`, `AFTER`, or `INSTEAD OF` certain events, depending on the trigger type and database.

## 8.2 Why use triggers?

Typical use cases include:

- Maintaining audit records.
- Recording changes to important fields.
- Enforcing database-side rules that must apply regardless of which application writes the data.
- Maintaining derived data in carefully designed cases.

Avoid placing all business logic inside triggers. Hidden database behavior can make application flows difficult to understand and debug.

## 8.3 OLD and NEW

In PostgreSQL trigger functions:

- `OLD` refers to the previous row for applicable update/delete operations.
- `NEW` refers to the new row for applicable insert/update operations.

For an `INSERT`, `NEW` contains the inserted row.
For an `UPDATE`, `OLD` and `NEW` represent the row before and after the update.
For a `DELETE`, `OLD` contains the deleted row.

## 8.4 Example: audit status changes

Create an audit table:

```sql
CREATE TABLE trip_status_audit (
    audit_id BIGINT GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    trip_id BIGINT NOT NULL,
    old_status VARCHAR(30),
    new_status VARCHAR(30),
    changed_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);
```

Create a trigger function:

```sql
CREATE OR REPLACE FUNCTION log_trip_status_change()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
    IF OLD.status IS DISTINCT FROM NEW.status THEN
        INSERT INTO trip_status_audit (
            trip_id,
            old_status,
            new_status
        )
        VALUES (
            OLD.trip_id,
            OLD.status,
            NEW.status
        );
    END IF;

    RETURN NEW;
END;
$$;
```

Create the trigger:

```sql
CREATE TRIGGER trg_trip_status_audit
AFTER UPDATE ON trips
FOR EACH ROW
EXECUTE FUNCTION log_trip_status_change();
```

Now, when a trip's status changes, the trigger inserts an audit record.

`IS DISTINCT FROM` is useful because it compares values with NULL-aware semantics.

## 8.5 Row-level vs statement-level triggers

A row-level trigger runs once for each affected row.

A statement-level trigger runs once for the SQL statement, regardless of how many rows it affects.

In PostgreSQL:

```sql
FOR EACH ROW
```

creates a row-level trigger.

```sql
FOR EACH STATEMENT
```

creates a statement-level trigger.

For example, one `UPDATE` statement affecting 100 trips causes a row-level trigger to run 100 times, but a statement-level trigger runs once.

## 8.6 BEFORE vs AFTER trigger

- `BEFORE` triggers run before the row operation is completed and can modify `NEW` for applicable operations.
- `AFTER` triggers run after the row operation and are useful when the change should already have taken place.
- `INSTEAD OF` triggers are commonly used with views to define how writes to the view are handled.

Choose the timing based on the required behavior.

## 8.7 Trigger best practices

- Keep trigger logic small and predictable.
- Document triggers so application developers know they exist.
- Avoid recursive trigger behavior.
- Consider performance when a trigger runs once per row.
- Ensure audit tables and permissions are designed correctly.
- Do not duplicate complex business rules in several places.
- Test transaction rollback behavior: trigger work normally participates in the triggering transaction, so it is rolled back if that transaction rolls back.

---

# 9. How Advanced SQL Connects to a Java Backend

A backend developer often has a choice: fetch raw records and calculate results in Java, or ask the database to calculate the result.

For large datasets, database-side filtering and aggregation are often preferable because the database can use indexes, query planning, and optimized execution.

## 9.1 Example: trip count per driver

Database-side aggregation:

```sql
SELECT
    driver_id,
    COUNT(*) AS completed_trip_count
FROM trips
WHERE status = 'COMPLETED'
GROUP BY driver_id;
```

This returns one row per driver instead of sending every completed trip to Java.

## 9.2 Example: repository query

A Spring Data JPA repository can expose an aggregate query:

```java
public interface TripRepository extends JpaRepository<Trip, Long> {

    @Query(value = """
        SELECT driver_id AS driverId,
               COUNT(*) AS tripCount
        FROM trips
        WHERE status = 'COMPLETED'
        GROUP BY driver_id
        """, nativeQuery = true)
    List<Object[]> countCompletedTripsByDriver();
}
```

This is a simplified example. For maintainable production code, consider a projection interface or DTO mapping instead of exposing raw `Object[]` values.

## 9.3 When should calculation happen in SQL?

SQL is often suitable when:

- The calculation can be expressed as filtering, joining, grouping, or aggregation.
- The dataset is large.
- Only summarized data is required by the API.
- The database can reduce the number of rows sent over the network.

Java is often suitable when:

- The calculation involves complex application behavior.
- The data is already loaded and small.
- The logic depends on external services or application-specific objects.
- The computation is not naturally relational.

Do not transfer millions of rows to Java merely to calculate a simple `COUNT`, `SUM`, or `AVG` that the database can compute directly.

---

# 10. Important Interview Questions and Answers

## Q1. What are aggregate functions?

Aggregate functions calculate a summary from multiple rows. Common examples are `COUNT`, `SUM`, `AVG`, `MIN`, and `MAX`.

## Q2. What is the difference between COUNT(*) and COUNT(column)?

`COUNT(*)` counts rows. `COUNT(column)` counts only rows where that column is not `NULL`.

## Q3. How does AVG handle NULL?

`AVG` ignores NULL values. If NULL should count as zero, use `AVG(COALESCE(column, 0))`, but only when that matches the business meaning.

## Q4. What is GROUP BY?

`GROUP BY` groups rows that share values in selected columns so aggregate functions can calculate a result per group.

## Q5. WHERE vs HAVING?

`WHERE` filters rows before grouping. `HAVING` filters groups after aggregation.

## Q6. What is a window function?

A window function calculates a value across related rows while preserving the individual rows in the output.

## Q7. GROUP BY vs PARTITION BY?

`GROUP BY` collapses rows into one row per group. `PARTITION BY` defines groups for a window calculation while retaining the original rows.

## Q8. What is the OVER clause?

`OVER` defines the window used by a window function. It can include `PARTITION BY`, `ORDER BY`, and a frame specification.

## Q9. ROW_NUMBER vs RANK vs DENSE_RANK?

`ROW_NUMBER` gives every row a unique sequential number. `RANK` gives ties the same rank and leaves gaps. `DENSE_RANK` gives ties the same rank without gaps.

## Q10. How do you find the second-highest distinct salary?

Use `DENSE_RANK()` ordered by salary descending, then filter for rank 2.

## Q11. How do you find the top three employees in each department?

Use `ROW_NUMBER()` or a ranking function with `PARTITION BY department_id`, order by salary descending, then filter the result to the desired rank. Choose the ranking function based on tie behavior.

## Q12. What is LAG?

`LAG` returns a value from a preceding row in the window ordering. It is useful for comparing a record with the previous record.

## Q13. What is LEAD?

`LEAD` returns a value from a following row in the window ordering.

## Q14. What is a running total?

A running total is an accumulated aggregate over an ordered window, commonly implemented with `SUM(...) OVER (ORDER BY ... ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW)`.

## Q15. What is a view?

A view is a named SQL query that can be queried like a table. A normal PostgreSQL view stores the query definition rather than a separate materialized result.

## Q16. What is a materialized view?

A materialized view stores the query result. It can speed up expensive reports, but it must be refreshed to reflect changes in the underlying data.

## Q17. View vs materialized view?

A normal view evaluates its query when used. A materialized view stores a result that can become stale until refreshed.

## Q18. What is a stored procedure?

A stored procedure is a named database routine invoked with `CALL` in PostgreSQL. It can encapsulate database operations and, in permitted contexts, perform transaction control.

## Q19. Function vs procedure in PostgreSQL?

A function is used as a function expression or query source and returns according to its definition. A procedure is invoked with `CALL` and has different transaction-control capabilities.

## Q20. What is a trigger?

A trigger is database logic that runs automatically when a configured event such as an insert, update, or delete occurs.

## Q21. What are OLD and NEW in a trigger?

In PostgreSQL trigger functions, `OLD` represents the previous row for update/delete operations, while `NEW` represents the new row for insert/update operations.

## Q22. Row-level vs statement-level trigger?

A row-level trigger runs once for every affected row. A statement-level trigger runs once for the SQL statement.

## Q23. Should all business logic be placed in database triggers?

No. Triggers can be useful for audit or database-wide integrity behavior, but excessive hidden business logic makes systems harder to understand, test, and debug.

## Q24. Why aggregate in SQL instead of Java?

The database can filter and aggregate data close to where it is stored, reducing network transfer and application memory use. Java remains appropriate for complex application logic and non-relational processing.

---

# 11. Quick Revision Sheet

| Topic | Remember |
|---|---|
| `COUNT(*)` | Counts rows |
| `COUNT(column)` | Counts non-NULL values |
| `COUNT(DISTINCT column)` | Counts distinct non-NULL values |
| `SUM()` | Adds values |
| `AVG()` | Averages non-NULL values |
| `MIN()` / `MAX()` | Minimum / maximum |
| `COALESCE()` | First non-NULL expression |
| `GROUP BY` | Creates groups and can aggregate them |
| `WHERE` | Filters rows before grouping |
| `HAVING` | Filters groups after aggregation |
| `OVER()` | Defines a window for a window function |
| `PARTITION BY` | Divides rows into window partitions |
| `ROW_NUMBER()` | Unique sequential row number |
| `RANK()` | Ties share rank; gaps follow |
| `DENSE_RANK()` | Ties share rank; no gaps |
| `LAG()` | Reads a preceding row's value |
| `LEAD()` | Reads a following row's value |
| Running total | Windowed `SUM` with an explicit frame |
| View | Stores a query definition |
| Materialized view | Stores query result; refreshes required |
| Function | Reusable database routine that returns according to its definition |
| Procedure | PostgreSQL routine invoked with `CALL` |
| Trigger | Runs automatically on configured database events |
| `OLD` | Previous row in applicable trigger operations |
| `NEW` | New row in applicable trigger operations |

---

# 12. Day 12 Completion Checklist

This is a learning coverage checklist, not a quiz.

- [ ] I understand `COUNT`, `SUM`, `AVG`, `MIN`, and `MAX`.
- [ ] I can explain `COUNT(*)` vs `COUNT(column)` and NULL handling.
- [ ] I can use `COALESCE`.
- [ ] I can group records by one or multiple columns.
- [ ] I can explain `WHERE` vs `HAVING`.
- [ ] I understand the `OVER` clause and `PARTITION BY`.
- [ ] I can explain `GROUP BY` vs window functions.
- [ ] I can distinguish `ROW_NUMBER`, `RANK`, and `DENSE_RANK`.
- [ ] I understand how to find the second-highest distinct salary.
- [ ] I can use `LAG` and `LEAD`.
- [ ] I can calculate a running total with an explicit window frame.
- [ ] I understand views and materialized views.
- [ ] I can explain PostgreSQL functions vs procedures.
- [ ] I understand triggers, `OLD`, `NEW`, and row-level vs statement-level triggers.
- [ ] I know when to aggregate data in SQL rather than Java.

---

## Day 12 takeaway

Advanced SQL helps a backend developer build efficient reporting and data-access features without loading unnecessary records into application memory. `GROUP BY` summarizes data, window functions analyze rows while preserving detail, and views or database routines can encapsulate reusable database logic.

**Next in the learning sequence:** Day 13 — Advanced Java: Generics, Java 8 features, Lambda Expressions, Functional Interfaces, Stream API, Advanced Collectors, Optional, and Method References.
"""

path = Path("/mnt/data/Day_12_Advanced_SQL.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
