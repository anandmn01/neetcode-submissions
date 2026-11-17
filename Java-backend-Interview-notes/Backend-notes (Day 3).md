from pathlib import Path

content = r'''# Day 03 — Spring Boot REST APIs, Validation, JPA, and Transactions

> **Learning track:** Java Backend Developer Interview Preparation  
> **Day:** 03  
> **Focus:** Building and understanding REST APIs with Spring Boot  
> **Examples:** Employee Transport backend  
> **Stack assumed:** Java 21, Spring Boot 4.x, Jakarta APIs, PostgreSQL

---

## Learning objectives

By the end of this day, you should be able to explain:

1. What REST is and how HTTP requests and responses work.
2. How Spring maps HTTP requests to controller methods.
3. Why Controller, Service, and Repository layers should have separate responsibilities.
4. Why applications use Request DTOs and Response DTOs instead of exposing entities directly.
5. How JSON is converted into Java objects and back.
6. How to validate incoming request data.
7. How to handle exceptions consistently across an API.
8. How HTTP status codes and `ResponseEntity` are used.
9. How Spring Data JPA, Hibernate, and `JpaRepository` work together.
10. How entity relationships, derived queries, JPQL, pagination, and transactions work.

---

# 1. REST API Fundamentals

## 1.1 What is an API?

API stands for **Application Programming Interface**. It defines how two software components communicate.

For example, an employee mobile application may need to:

- Log in an employee.
- Fetch employee details.
- Display assigned trips.
- Update a driver's availability.
- Confirm that an employee has boarded a vehicle.

The Flutter application does not normally connect directly to PostgreSQL. It sends an HTTP request to the Spring Boot backend. The backend validates the request, performs business logic, accesses the database, and returns an HTTP response.

```text
Flutter Mobile App
       |
       | HTTP request (JSON)
       v
Spring Boot REST API
       |
       v
Controller
       |
       v
Service (business rules)
       |
       v
Repository
       |
       v
Hibernate / JPA
       |
       v
PostgreSQL
```

## 1.2 What is REST?

REST stands for **Representational State Transfer**. It is an architectural style for designing networked applications.

A REST-style API commonly:

- Uses HTTP as its communication protocol.
- Treats application data as resources.
- Uses resource-oriented URLs.
- Uses HTTP methods to describe the operation.
- Sends representations of resources, commonly JSON.
- Is stateless at the request level: each request contains the information needed to process it.

Example resources:

| Resource | Example URL |
|---|---|
| Employees | `/api/employees` |
| One employee | `/api/employees/15` |
| Drivers | `/api/drivers` |
| Trips | `/api/trips` |
| One trip | `/api/trips/42` |

A URL should generally identify a resource. The HTTP method describes what the client wants to do with it.

## 1.3 HTTP methods

| Method | Typical purpose | Example |
|---|---|---|
| `GET` | Read a resource | `GET /api/employees/15` |
| `POST` | Create a resource or trigger an operation | `POST /api/employees` |
| `PUT` | Replace/update a resource representation | `PUT /api/employees/15` |
| `PATCH` | Partially update a resource | `PATCH /api/employees/15/deactivate` |
| `DELETE` | Delete a resource | `DELETE /api/employees/15` |

### GET

Used to retrieve information. A GET request should not intentionally change server-side state.

```http
GET /api/employees/15
```

### POST

Commonly used to create a resource or submit an operation.

```http
POST /api/employees
Content-Type: application/json
```

```json
{
  "name": "Rahul",
  "email": "rahul@example.com"
}
```

### PUT vs PATCH

- **PUT** generally represents replacing the resource with the submitted representation.
- **PATCH** represents applying a partial modification.

For example, changing only a driver's active status is naturally represented by a PATCH operation:

```http
PATCH /api/drivers/8/deactivate
```

Actual API contracts may define details differently, so clients and servers must agree on the request semantics.

## 1.4 What does stateless mean?

Stateless does not mean the application has no database or does not store user data.

It means the server does not rely on hidden conversational state from a previous HTTP request to understand the current request. Each request carries the information needed for authentication and processing, such as an access token.

For example:

```http
GET /api/trips
Authorization: Bearer <access-token>
```

The server validates the token and processes the request. The trip data can still be stored persistently in PostgreSQL.

## 1.5 JSON request and response

JSON is a common format for exchanging data between a Flutter client and a Spring Boot backend.

Example request:

```json
{
  "employeeId": 101,
  "tripDate": "2026-11-08",
  "shift": "MORNING"
}
```

Example response:

```json
{
  "tripId": 501,
  "status": "SCHEDULED",
  "employeeCount": 12
}
```

Spring MVC uses message converters, commonly Jackson-based JSON conversion, to convert JSON into Java objects and Java objects into JSON.

---

# 2. Spring Boot Controller and Request Mapping

## 2.1 What is a Controller?

A Controller is the HTTP entry point of a Spring MVC application. It receives HTTP requests, reads path/query/body data, calls the appropriate service method, and returns an HTTP response.

A controller should not become the place where all business rules and database operations are written.

Example:

```java
@RestController
@RequestMapping("/api/employees")
public class EmployeeController {

    private final EmployeeService employeeService;

    public EmployeeController(EmployeeService employeeService) {
        this.employeeService = employeeService;
    }

    @GetMapping("/{id}")
    public EmployeeResponse getEmployee(@PathVariable Long id) {
        return employeeService.getEmployeeById(id);
    }
}
```

Important annotations:

- `@RestController`: marks a controller whose return values are written to the response body.
- `@RequestMapping`: defines a common URL prefix or request mapping.
- `@GetMapping`: maps HTTP GET requests.
- `@PostMapping`: maps HTTP POST requests.
- `@PutMapping`: maps HTTP PUT requests.
- `@PatchMapping`: maps HTTP PATCH requests.
- `@DeleteMapping`: maps HTTP DELETE requests.

`@RestController` is effectively a combination of `@Controller` and `@ResponseBody`.

## 2.2 `@Controller` vs `@RestController`

| `@Controller` | `@RestController` |
|---|---|
| Commonly used for MVC pages/views | Commonly used for REST APIs |
| A returned string can represent a view name | A returned object is written to the response body |
| Use `@ResponseBody` when returning response data directly | Response-body behavior is applied by default |

For a JSON API, `@RestController` is usually the convenient choice.

## 2.3 Getting values from the request

### `@PathVariable`

Use it when a value is part of the URL path.

```java
@GetMapping("/{id}")
public EmployeeResponse getById(@PathVariable Long id) {
    return employeeService.getEmployeeById(id);
}
```

Request:

```http
GET /api/employees/25
```

Here, `id` is `25`.

### `@RequestParam`

Use it for query parameters.

```java
@GetMapping
public List<EmployeeResponse> search(
        @RequestParam(required = false) String department) {
    return employeeService.searchEmployees(department);
}
```

Request:

```http
GET /api/employees?department=Engineering
```

You can also provide defaults:

```java
@GetMapping("/active")
public List<EmployeeResponse> getActiveEmployees(
        @RequestParam(defaultValue = "true") boolean active) {
    return employeeService.getEmployeesByActiveStatus(active);
}
```

### `@RequestBody`

Use it to read the request body and convert JSON into a Java object.

```java
@PostMapping
public EmployeeResponse create(
        @RequestBody EmployeeCreateRequest request) {
    return employeeService.createEmployee(request);
}
```

The JSON field names are mapped to compatible Java properties by the configured JSON message converter.

## 2.4 Request flow in Spring MVC

```text
HTTP request
    |
    v
DispatcherServlet
    |
    v
HandlerMapping finds matching controller method
    |
    v
HandlerAdapter invokes the method
    |
    +--> Path variables / query parameters are resolved
    |
    +--> JSON body is converted into a Java object
    |
    v
Controller method
    |
    v
Service method
    |
    v
Repository / database
    |
    v
Controller return value
    |
    v
HTTP response (often JSON)
```

The `DispatcherServlet` is the central front controller in Spring MVC. It coordinates request handling; it is not the place where your application's business rules should live.

---

# 3. Layered Architecture: Controller, Service, Repository

A common Spring Boot backend separates responsibilities into layers.

## 3.1 Controller layer

Responsibilities:

- Receive HTTP requests.
- Read request data.
- Trigger request validation.
- Call service methods.
- Return response data and appropriate status codes.

It should avoid implementing complex business rules or writing SQL directly.

## 3.2 Service layer

Responsibilities:

- Implement business rules.
- Coordinate multiple repositories when needed.
- Decide whether an operation is allowed.
- Define transaction boundaries.
- Convert between application/domain data and response data where appropriate.

Example:

```java
@Service
public class EmployeeService {

    private final EmployeeRepository employeeRepository;

    public EmployeeService(EmployeeRepository employeeRepository) {
        this.employeeRepository = employeeRepository;
    }

    public EmployeeResponse getEmployeeById(Long id) {
        Employee employee = employeeRepository.findById(id)
                .orElseThrow(() ->
                        new EmployeeNotFoundException(
                                "Employee not found: " + id));

        return new EmployeeResponse(
                employee.getId(),
                employee.getName(),
                employee.getEmail()
        );
    }
}
```

The service decides what should happen if the employee does not exist. The repository is responsible for data access.

## 3.3 Repository layer

Responsibilities:

- Perform database access.
- Provide CRUD operations.
- Define query methods.
- Return entities or projections.

Example:

```java
@Repository
public interface EmployeeRepository
        extends JpaRepository<Employee, Long> {
}
```

Spring Data JPA creates a runtime implementation for the repository interface.

## 3.4 Why separate layers?

Suppose a driver can be assigned to a trip only when:

1. The driver exists.
2. The driver is active.
3. The vehicle is available.
4. The trip has not already been assigned.

These are business rules and belong in the service layer, not in the controller or repository.

Benefits:

- Easier testing.
- Clear responsibility boundaries.
- Reusable business logic.
- Easier maintenance.
- Better transaction management.
- Less coupling between HTTP and database code.

## 3.5 Full request flow

```text
Flutter client
    |
    | POST /api/trips/501/assign
    v
TripController
    |
    | calls assignDriver(...)
    v
TripService
    |
    | checks trip and driver rules
    | coordinates repository operations
    v
TripRepository + DriverRepository
    |
    v
Hibernate / JPA
    |
    v
PostgreSQL
```

**Interview point:** A controller should be thin. The service layer owns business decisions. The repository layer owns persistence operations.

---

# 4. Entity vs Request DTO vs Response DTO

This is one of the most important backend concepts.

## 4.1 What is an Entity?

An Entity represents a persistent object mapped to a database table.

```java
import jakarta.persistence.*;

@Entity
@Table(name = "employees")
public class Employee {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String name;

    private String email;

    private String passwordHash;

    // Constructors, getters and setters
}
```

The `Employee` entity is managed by JPA/Hibernate and maps to the `employees` table.

## 4.2 What is a Request DTO?

DTO means **Data Transfer Object**.

A Request DTO represents the data the client is allowed or expected to send.

```java
public class EmployeeCreateRequest {

    private String name;
    private String email;
    private String password;

    // Getters and setters
}
```

The client should not decide database-generated fields such as the employee ID. It should not submit internal fields such as `passwordHash`.

## 4.3 What is a Response DTO?

A Response DTO defines what the backend sends back to the client.

```java
public record EmployeeResponse(
        Long id,
        String name,
        String email
) {}
```

Notice that `passwordHash` is not included.

## 4.4 Why not expose Entity directly?

Exposing entities directly can cause several problems:

1. **Security:** Internal fields such as password hashes could accidentally be exposed.
2. **Tight coupling:** A database schema change can unexpectedly change the public API.
3. **Relationship serialization:** Bidirectional entity relationships can cause recursion or very large JSON responses.
4. **Over-posting:** Clients may submit fields they should not control.
5. **API stability:** The API contract should be designed independently from persistence details.

A DTO creates a clear boundary between the external API and internal persistence model.

## 4.5 Request-to-response flow

```text
Incoming JSON
    |
    v
EmployeeCreateRequest
    |
    v
Controller
    |
    v
Service (business rules)
    |
    v
Employee Entity
    |
    v
EmployeeRepository
    |
    v
Database
    |
    v
Employee Entity
    |
    v
EmployeeResponse
    |
    v
Outgoing JSON
```

## 4.6 Mapping example

```java
@Service
public class EmployeeService {

    private final EmployeeRepository employeeRepository;

    public EmployeeService(EmployeeRepository employeeRepository) {
        this.employeeRepository = employeeRepository;
    }

    public EmployeeResponse createEmployee(
            EmployeeCreateRequest request) {

        Employee employee = new Employee();
        employee.setName(request.getName());
        employee.setEmail(request.getEmail());

        // In a real application, hash the password with
        // a suitable PasswordEncoder. Never store raw passwords.
        employee.setPasswordHash(hashPassword(request.getPassword()));

        Employee saved = employeeRepository.save(employee);

        return new EmployeeResponse(
                saved.getId(),
                saved.getName(),
                saved.getEmail()
        );
    }

    private String hashPassword(String rawPassword) {
        // Placeholder only: use Spring Security PasswordEncoder
        // in a real application.
        throw new UnsupportedOperationException(
                "Use a configured PasswordEncoder");
    }
}
```

The method above illustrates the mapping flow. The placeholder deliberately does not implement password hashing; production code should inject a configured `PasswordEncoder`.

## 4.7 Entity, Request DTO, Response DTO comparison

| Feature | Entity | Request DTO | Response DTO |
|---|---|---|---|
| Main purpose | Database persistence | Incoming client data | Outgoing client data |
| Database mapping | Yes | No | No |
| Contains database relationships | Often | Usually no | Only if API requires them |
| Controls public API fields | Not ideally | Yes, accepted input | Yes, exposed output |
| Example | `Employee` | `EmployeeCreateRequest` | `EmployeeResponse` |

---

# 5. Global Exception Handling

## 5.1 What is exception handling?

An exception represents an abnormal condition during program execution.

Examples in a backend:

- Requested employee does not exist.
- Email is already registered.
- Request data is invalid.
- A database operation fails.
- A user does not have permission to perform an action.

Without consistent handling, the application may return inconsistent error formats or expose internal implementation details.

## 5.2 Custom exception

Create a meaningful exception for a business condition:

```java
public class EmployeeNotFoundException
        extends RuntimeException {

    public EmployeeNotFoundException(String message) {
        super(message);
    }
}
```

Throw it from the service:

```java
public EmployeeResponse getEmployeeById(Long id) {
    Employee employee = employeeRepository.findById(id)
            .orElseThrow(() ->
                    new EmployeeNotFoundException(
                            "Employee not found with id: " + id));

    return new EmployeeResponse(
            employee.getId(),
            employee.getName(),
            employee.getEmail()
    );
}
```

This is clearer than returning `null` or throwing a generic exception.

## 5.3 `@ExceptionHandler`

`@ExceptionHandler` maps a particular exception type to a handler method.

```java
@RestController
@RequestMapping("/api/employees")
public class EmployeeController {

    @GetMapping("/{id}")
    public EmployeeResponse getById(@PathVariable Long id) {
        // ...
        return null;
    }

    @ExceptionHandler(EmployeeNotFoundException.class)
    public ResponseEntity<String> handleNotFound(
            EmployeeNotFoundException exception) {

        return ResponseEntity
                .status(HttpStatus.NOT_FOUND)
                .body(exception.getMessage());
    }
}
```

This handler is local to that controller.

## 5.4 `@RestControllerAdvice`

For application-wide exception handling, use `@RestControllerAdvice`.

```java
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(EmployeeNotFoundException.class)
    public ResponseEntity<ApiError> handleEmployeeNotFound(
            EmployeeNotFoundException exception) {

        ApiError error = new ApiError(
                404,
                exception.getMessage()
        );

        return ResponseEntity
                .status(HttpStatus.NOT_FOUND)
                .body(error);
    }
}
```

Example error DTO:

```java
public record ApiError(
        int status,
        String message
) {}
```

Now different controllers can use the same exception-handling policy.

## 5.5 Why global exception handling?

- Consistent response format.
- Less repeated try/catch code in controllers.
- Central place to map known exceptions to HTTP statuses.
- Easier logging and maintenance.
- Avoids exposing stack traces to clients.

Do not return raw stack traces, SQL statements, credentials, or internal server details in production API responses. Log diagnostic details securely on the server.

---

# 6. Input Validation

Validation checks whether incoming data satisfies defined rules before business processing.

For example, an employee creation request might require:

- Name must not be blank.
- Email must have a valid email format.
- Password must have a minimum length.

## 6.1 Validation annotations

Common Jakarta Bean Validation annotations include:

| Annotation | Purpose |
|---|---|
| `@NotNull` | Value must not be null |
| `@NotBlank` | String must not be null and must contain a non-whitespace character |
| `@NotEmpty` | String/collection/array must not be empty (and not null) |
| `@Email` | Value should have an email-like format |
| `@Size` | Defines size/length limits |
| `@Min` | Numeric value must be at least a minimum |
| `@Max` | Numeric value must not exceed a maximum |
| `@Pattern` | Value must match a regular expression |

The validation starter/dependency must be present in the project for these annotations to be executed by Spring's validation integration.

## 6.2 Add validation to a DTO

```java
import jakarta.validation.constraints.*;

public class EmployeeCreateRequest {

    @NotBlank(message = "Name is required")
    @Size(max = 100, message = "Name cannot exceed 100 characters")
    private String name;

    @NotBlank(message = "Email is required")
    @Email(message = "Enter a valid email address")
    private String email;

    @NotBlank(message = "Password is required")
    @Size(min = 8, message = "Password must contain at least 8 characters")
    private String password;

    // Getters and setters
}
```

## 6.3 Use `@Valid` in the controller

```java
@PostMapping
public ResponseEntity<EmployeeResponse> create(
        @Valid @RequestBody EmployeeCreateRequest request) {

    EmployeeResponse response =
            employeeService.createEmployee(request);

    return ResponseEntity
            .status(HttpStatus.CREATED)
            .body(response);
}
```

When validation fails, Spring MVC raises a validation exception (commonly `MethodArgumentNotValidException` for a validated request body). A global handler can convert it into a structured `400 Bad Request` response.

## 6.4 Handle validation errors globally

```java
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(MethodArgumentNotValidException.class)
    public ResponseEntity<Map<String, String>> handleValidation(
            MethodArgumentNotValidException exception) {

        Map<String, String> errors = new HashMap<>();

        exception.getBindingResult()
                .getFieldErrors()
                .forEach(error ->
                        errors.put(
                                error.getField(),
                                error.getDefaultMessage()
                        ));

        return ResponseEntity
                .badRequest()
                .body(errors);
    }
}
```

Possible response:

```json
{
  "name": "Name is required",
  "email": "Enter a valid email address"
}
```

For production APIs, use a consistent error response structure and consider including a timestamp, field-level errors, and a trace/correlation identifier. Avoid returning internal exception details.

## 6.5 Validation vs business rules

These are related but different.

**Input validation:**
- Is the email syntactically valid?
- Is the name blank?
- Is the page number negative?

**Business validation:**
- Is this driver active?
- Is the vehicle already assigned to another trip?
- Is the trip in a state where assignment is allowed?

Bean Validation is useful for input constraints. Business rules generally belong in the service layer.

---

# 7. HTTP Status Codes and `ResponseEntity`

An HTTP status code tells the client the broad outcome of its request.

## 7.1 Common status codes

| Status | Meaning | Typical API use |
|---|---|---|
| `200 OK` | Request succeeded | Successful GET or update with a response |
| `201 Created` | Resource created | Successful POST creating a resource |
| `204 No Content` | Succeeded, no response body | Successful delete or action with no body |
| `400 Bad Request` | Invalid request | Validation or malformed input |
| `401 Unauthorized` | Authentication is missing/invalid | Missing or invalid login credentials/token |
| `403 Forbidden` | Authenticated but not allowed | User lacks required permission |
| `404 Not Found` | Resource not found | Employee or trip ID does not exist |
| `409 Conflict` | Conflict with current resource state | Duplicate or conflicting update |
| `500 Internal Server Error` | Unexpected server failure | Unhandled server-side failure |

Important distinction:

- `401`: the client is not successfully authenticated.
- `403`: the client is authenticated but does not have permission for the operation.

## 7.2 What is `ResponseEntity`?

`ResponseEntity<T>` represents the complete HTTP response: status code, headers, and body.

Example:

```java
@GetMapping("/{id}")
public ResponseEntity<EmployeeResponse> getById(
        @PathVariable Long id) {

    EmployeeResponse response =
            employeeService.getEmployeeById(id);

    return ResponseEntity.ok(response);
}
```

Create:

```java
@PostMapping
public ResponseEntity<EmployeeResponse> create(
        @Valid @RequestBody EmployeeCreateRequest request) {

    EmployeeResponse response =
            employeeService.createEmployee(request);

    return ResponseEntity
            .status(HttpStatus.CREATED)
            .body(response);
}
```

No content:

```java
@DeleteMapping("/{id}")
public ResponseEntity<Void> delete(@PathVariable Long id) {
    employeeService.deleteEmployee(id);
    return ResponseEntity.noContent().build();
}
```

## 7.3 When should you use `ResponseEntity`?

Use it when the method needs to control status codes or headers explicitly. For simple successful responses, returning a DTO directly is also valid; Spring will serialize it and use the default successful status.

---

# 8. Spring Data JPA and Hibernate

## 8.1 What is ORM?

ORM stands for **Object-Relational Mapping**.

A relational database stores data in tables and rows. Java applications work with objects and classes. ORM maps Java objects to relational database structures.

Example:

```text
Java class: Employee
       |
       | JPA mapping
       v
Database table: employees
       |
       +-- id
       +-- name
       +-- email
```

Instead of writing all basic SQL manually, you can use entity objects and repository methods.

## 8.2 JPA vs Hibernate vs Spring Data JPA

| Technology | What it is |
|---|---|
| JPA | Java specification/API for object-relational persistence |
| Hibernate | Popular implementation of JPA; ORM framework |
| Spring Data JPA | Spring abstraction that simplifies repository/data-access code |

Think of it this way:

- JPA defines standard contracts and annotations.
- Hibernate implements those persistence contracts.
- Spring Data JPA builds convenient repository abstractions on top.

## 8.3 What is `JpaRepository`?

`JpaRepository<T, ID>` provides common persistence operations.

```java
public interface EmployeeRepository
        extends JpaRepository<Employee, Long> {
}
```

Here:

- `Employee` is the entity type.
- `Long` is the entity's ID type.

Common inherited methods include:

| Method | Purpose |
|---|---|
| `save(entity)` | Save or update an entity |
| `findById(id)` | Find by primary key; returns `Optional<T>` |
| `findAll()` | Retrieve all entities |
| `existsById(id)` | Check whether an ID exists |
| `deleteById(id)` | Delete by ID |
| `count()` | Count records |
| `flush()` | Synchronize pending persistence changes with the database |

The exact SQL and timing depend on the persistence context and transaction.

## 8.4 What happens when `save()` is called?

At a high level:

1. Application calls `repository.save(employee)`.
2. Spring Data JPA delegates to the JPA `EntityManager`.
3. The persistence provider (such as Hibernate) manages the entity.
4. SQL may be executed immediately or later during flush/commit, depending on the operation and strategy.
5. The transaction commits or rolls back.

Do not assume every `save()` call means an immediate SQL INSERT or UPDATE at that exact line.

---

# 9. Entity Relationships

Relational databases use foreign keys to connect tables. JPA uses relationship annotations to represent those connections in Java entities.

Assume an employee transport system with employees, trips, and vehicles.

## 9.1 `@ManyToOne`

Many employees may belong to one department.

```java
@Entity
public class Employee {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String name;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "department_id")
    private Department department;
}
```

The `employees` table contains the foreign key `department_id`.

## 9.2 `@OneToMany`

One department can have many employees.

```java
@Entity
public class Department {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String name;

    @OneToMany(mappedBy = "department")
    private List<Employee> employees = new ArrayList<>();
}
```

`mappedBy = "department"` means the `department` field in `Employee` owns the relationship mapping. The Department collection is the inverse side.

## 9.3 `@OneToOne`

One employee may have one profile record, and one profile belongs to one employee.

```java
@Entity
public class EmployeeProfile {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @OneToOne
    @JoinColumn(name = "employee_id", unique = true)
    private Employee employee;
}
```

A unique constraint on `employee_id` helps enforce one-to-one cardinality in the database.

## 9.4 `@ManyToMany`

Many employees can be assigned to many trips.

```java
@Entity
public class Trip {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @ManyToMany
    @JoinTable(
        name = "trip_employees",
        joinColumns = @JoinColumn(name = "trip_id"),
        inverseJoinColumns = @JoinColumn(name = "employee_id")
    )
    private Set<Employee> employees = new HashSet<>();
}
```

A join table stores the relationship:

```text
trips
  id

employees
  id

trip_employees
  trip_id
  employee_id
```

If the relationship itself needs extra information—such as pickup order, boarding status, or pickup timestamp—an explicit entity such as `TripEmployee` is usually better than a bare `@ManyToMany`.

## 9.5 `mappedBy` and `@JoinColumn`

- `@JoinColumn` identifies the foreign-key column used by the owning side.
- `mappedBy` identifies the field on the owning side from the inverse side.
- The owning side controls updates to the relationship in the object-relational mapping.

For bidirectional relationships, keep both Java-side references synchronized using helper methods when appropriate.

## 9.6 Lazy vs Eager loading

- **LAZY:** Related data is loaded when accessed, subject to persistence-context/provider behavior.
- **EAGER:** Related data is fetched eagerly as part of loading the entity, though the provider controls the SQL strategy.

Common JPA defaults:

| Relationship | Default fetch type |
|---|---|
| `@ManyToOne` | EAGER |
| `@OneToOne` | EAGER |
| `@OneToMany` | LAZY |
| `@ManyToMany` | LAZY |

You can explicitly configure fetch behavior. Be careful with EAGER relationships: they can load more data than needed and do not automatically solve query-performance problems.

---

# 10. Custom Queries in Spring Data JPA

Spring Data JPA supports derived query methods and explicit queries.

## 10.1 Derived query methods

Spring Data can derive a query from a repository method name.

```java
public interface EmployeeRepository
        extends JpaRepository<Employee, Long> {

    List<Employee> findByName(String name);

    Optional<Employee> findByEmail(String email);

    List<Employee> findByActiveTrue();

    List<Employee> findByDepartmentName(String departmentName);

    boolean existsByEmail(String email);
}
```

Spring parses the method name and builds the query according to its conventions.

Derived query methods are convenient for straightforward queries. Very long or complex method names can become difficult to maintain.

## 10.2 JPQL using `@Query`

JPQL queries entity classes and their Java attributes, not raw table and column names.

```java
public interface EmployeeRepository
        extends JpaRepository<Employee, Long> {

    @Query("SELECT e FROM Employee e WHERE e.active = true")
    List<Employee> findAllActiveEmployees();
}
```

Here `Employee` is the entity name and `active` is a Java entity property.

## 10.3 Parameterized JPQL

```java
@Query("SELECT e FROM Employee e WHERE e.email = :email")
Optional<Employee> findEmployeeByEmail(
        @Param("email") String email);
```

Named parameters improve readability and avoid unsafe query-string concatenation.

## 10.4 Native SQL query

Use `nativeQuery = true` when you need database SQL directly.

```java
@Query(
    value = "SELECT * FROM employees WHERE active = true",
    nativeQuery = true
)
List<Employee> findActiveEmployeesNative();
```

JPQL is generally more portable across databases. Native SQL is useful for database-specific features or queries that are awkward to express in JPQL.

Never build SQL by concatenating untrusted user input. Use parameter binding.

## 10.5 JPQL vs Native SQL

| JPQL | Native SQL |
|---|---|
| Uses entity names and Java properties | Uses database table and column names |
| More database-independent | Database-specific features are available |
| Operates on the entity model | Operates directly on SQL schema |
| Useful for normal entity queries | Useful for specialized SQL |

---

# 11. Sorting and Pagination

A backend should avoid returning an unbounded number of database records for large collections.

For example, an employee table may eventually contain thousands of rows. Returning all rows in one response can consume database, server, and mobile memory.

## 11.1 Pagination

Spring Data provides `Pageable` and `Page`.

```java
@GetMapping("/page")
public Page<EmployeeResponse> getEmployees(
        @PageableDefault(size = 20, sort = "name")
        Pageable pageable) {

    return employeeService.getEmployees(pageable);
}
```

A request may look like:

```http
GET /api/employees/page?page=0&size=20&sort=name,asc
```

Page numbering is normally zero-based in Spring Data.

## 11.2 Repository pagination

```java
public interface EmployeeRepository
        extends JpaRepository<Employee, Long> {

    Page<Employee> findByActiveTrue(Pageable pageable);
}
```

Service example:

```java
public Page<EmployeeResponse> getEmployees(Pageable pageable) {
    return employeeRepository.findByActiveTrue(pageable)
            .map(employee -> new EmployeeResponse(
                    employee.getId(),
                    employee.getName(),
                    employee.getEmail()
            ));
}
```

`Page.map(...)` maps each content item while preserving page metadata.

## 11.3 `Page` vs `Slice`

| `Page<T>` | `Slice<T>` |
|---|---|
| Provides total element/page information | Focuses on whether another slice exists |
| Often requires a count query | Can avoid a total-count query |
| Useful when UI needs total pages/count | Useful for next/previous navigation |

Choose based on the client requirement. A total count can be expensive for large or complex queries.

---

# 12. Transactions and `@Transactional`

## 12.1 What is a transaction?

A transaction is a unit of work that should succeed or fail as a whole.

Example: assigning a driver and vehicle to a trip may require multiple database updates.

```text
Begin transaction
    |
    +--> Update trip assignment
    |
    +--> Mark driver as assigned
    |
    +--> Mark vehicle as assigned
    |
    v
Commit if all operations succeed
Rollback if a qualifying failure occurs
```

Without a transaction, one update might succeed while another fails, leaving inconsistent data.

## 12.2 ACID properties

| Property | Meaning |
|---|---|
| Atomicity | All operations in the transaction succeed or the transaction is rolled back |
| Consistency | A transaction preserves defined data constraints and invariants |
| Isolation | Concurrent transactions are isolated according to the chosen isolation level |
| Durability | Committed changes survive failures according to the database's durability guarantees |

## 12.3 `@Transactional`

Spring's `@Transactional` is commonly used at the service layer.

```java
@Service
public class TripService {

    private final TripRepository tripRepository;
    private final DriverRepository driverRepository;

    public TripService(
            TripRepository tripRepository,
            DriverRepository driverRepository) {
        this.tripRepository = tripRepository;
        this.driverRepository = driverRepository;
    }

    @Transactional
    public void assignDriver(Long tripId, Long driverId) {

        Trip trip = tripRepository.findById(tripId)
                .orElseThrow(() ->
                        new TripNotFoundException("Trip not found"));

        Driver driver = driverRepository.findById(driverId)
                .orElseThrow(() ->
                        new DriverNotFoundException("Driver not found"));

        if (!driver.isActive()) {
            throw new IllegalStateException("Driver is not active");
        }

        trip.setDriver(driver);
        driver.setAssigned(true);
    }
}
```

The example illustrates a transaction boundary. Production code should use a meaningful business exception for an invalid assignment and enforce important consistency rules at the database level where appropriate.

## 12.4 What happens on rollback?

If an unchecked exception escapes a Spring-managed transactional method, Spring's default rollback rules generally mark the transaction for rollback.

Important default behavior:

- `RuntimeException` and `Error`: rollback by default.
- Checked exceptions: do not cause rollback by default.

You can customize the rule:

```java
@Transactional(rollbackFor = Exception.class)
public void performOperation() throws Exception {
    // transactional work
}
```

Use rollback rules deliberately; do not assume every exception automatically causes rollback.

## 12.5 Where should `@Transactional` be placed?

A common practice is to put transaction boundaries around service methods because a business operation may involve several repository calls.

Repository methods may also have transaction behavior, but the service layer is usually the right place to define a transaction spanning multiple operations.

## 12.6 Important proxy/self-invocation limitation

In Spring's common proxy-based transaction management, a call from one method to another method on the same object may bypass the proxy.

For example:

```java
@Service
public class ReportService {

    public void generate() {
        saveReport(); // self-invocation
    }

    @Transactional
    public void saveReport() {
        // ...
    }
}
```

Do not assume the `@Transactional` annotation will be applied to `saveReport()` in this self-invocation scenario. A call through the Spring proxy (often by moving the transactional method to another bean) is the usual solution.

---

# 13. End-to-End CRUD API Architecture

This section combines the concepts into a typical Employee API.

## 13.1 Request flow

```text
POST /api/employees
        |
        v
EmployeeController
        |
        | @Valid @RequestBody
        v
EmployeeCreateRequest
        |
        v
EmployeeService
        |
        | business rules + mapping
        v
Employee entity
        |
        v
EmployeeRepository
        |
        v
Hibernate / JPA
        |
        v
PostgreSQL
        |
        v
Saved Employee entity
        |
        v
EmployeeResponse
        |
        v
201 Created + JSON
```

## 13.2 Example package structure

```text
com.example.employee
|
+-- controller
|   +-- EmployeeController.java
|
+-- service
|   +-- EmployeeService.java
|
+-- repository
|   +-- EmployeeRepository.java
|
+-- entity
|   +-- Employee.java
|
+-- dto
|   +-- EmployeeCreateRequest.java
|   +-- EmployeeUpdateRequest.java
|   +-- EmployeeResponse.java
|
+-- exception
|   +-- EmployeeNotFoundException.java
|   +-- GlobalExceptionHandler.java
|   +-- ApiError.java
|
+-- EmployeeApplication.java
```

A project may organize DTOs into request/response subpackages as it grows. The important point is consistent separation, not one mandatory folder structure.

## 13.3 Example endpoint design

| Operation | Method | Endpoint | Typical success status |
|---|---|---|---|
| List employees | GET | `/api/employees` | 200 |
| Get employee | GET | `/api/employees/{id}` | 200 |
| Create employee | POST | `/api/employees` | 201 |
| Update employee | PUT | `/api/employees/{id}` | 200 |
| Partially update status | PATCH | `/api/employees/{id}/deactivate` | 200 or 204 |
| Delete employee | DELETE | `/api/employees/{id}` | 204 |

The exact response status and response body are API design choices. Keep them consistent and document the contract for the client.

---

# 14. Common Mistakes to Avoid

1. **Putting all logic in the controller.** Keep business rules in services.
2. **Returning JPA entities directly.** Use DTOs to control the API contract and avoid exposing internal fields.
3. **Returning `null` for missing records.** Use a meaningful exception and map it to a 404 response.
4. **Using generic `Exception` for every business failure.** Create meaningful exception types.
5. **Forgetting `@Valid`.** Validation annotations on a request DTO are not automatically applied to every controller method; trigger validation appropriately.
6. **Confusing `@PathVariable` and `@RequestParam`.** One reads path segments; the other reads query parameters.
7. **Using EAGER fetching everywhere.** It can load unnecessary data and does not guarantee efficient queries.
8. **Ignoring N+1 query behavior.** Check generated SQL and choose an appropriate fetch strategy.
9. **Returning every database row.** Use pagination for large collections.
10. **Assuming `save()` always executes SQL immediately.** Hibernate may defer SQL until flush or commit.
11. **Putting transactions only around individual repository calls when a business operation spans several calls.** Define the transaction boundary around the business operation.
12. **Assuming checked exceptions always trigger rollback.** Spring's default rollback rules distinguish checked and unchecked exceptions.
13. **Exposing stack traces or SQL details to API clients.** Return safe error responses and log details securely.
14. **Building SQL by string concatenation.** Use parameter binding.
15. **Treating validation and business rules as identical.** Input format checks and domain decisions belong to different concerns.

---

# 15. Interview Questions and Answers

## Q1. What is REST?

REST is an architectural style for designing networked applications. REST APIs commonly expose resources through URLs and use HTTP methods such as GET, POST, PUT, PATCH, and DELETE to interact with those resources.

## Q2. What is the difference between PUT and PATCH?

PUT generally represents replacing a resource representation, while PATCH applies a partial modification. The exact semantics should be defined by the API contract.

## Q3. What is the difference between `@Controller` and `@RestController`?

`@Controller` is commonly used for Spring MVC controllers that return views. `@RestController` combines `@Controller` and `@ResponseBody`, so returned objects are written to the HTTP response body, commonly as JSON.

## Q4. What is the role of `DispatcherServlet`?

`DispatcherServlet` is Spring MVC's front controller. It receives incoming requests and coordinates handler mapping, invocation of controller methods, and response processing.

## Q5. Why do we use a service layer?

The service layer contains business logic and coordinates operations. It keeps controllers focused on HTTP concerns and repositories focused on persistence.

## Q6. What is a DTO, and why is it useful?

A Data Transfer Object carries data between application boundaries. Request and response DTOs control which fields clients can send and receive, reduce coupling to database entities, and help prevent accidental exposure of internal data.

## Q7. What is the difference between an Entity and a DTO?

An Entity is a persistence-mapped object managed by JPA/Hibernate. A DTO is a data-transfer structure used at application boundaries and is not itself a JPA entity.

## Q8. What is `@RestControllerAdvice`?

It provides centralized controller advice, commonly used to handle exceptions across multiple controllers and return consistent API error responses.

## Q9. What is the difference between `@ExceptionHandler` and `@RestControllerAdvice`?

`@ExceptionHandler` defines a method that handles specified exceptions. It can be declared inside a controller or inside a controller-advice class. `@RestControllerAdvice` provides a shared, application-wide location for such handlers and applies response-body semantics.

## Q10. What is the purpose of `@Valid`?

`@Valid` triggers Bean Validation for an object, such as a request DTO. Constraints like `@NotBlank`, `@Email`, and `@Size` are evaluated, and validation failures can be handled as a bad request.

## Q11. What is the difference between 401 and 403?

401 means the request has not been successfully authenticated. 403 means the client is authenticated but is not authorized to perform the requested operation.

## Q12. What is `ResponseEntity`?

`ResponseEntity<T>` represents an HTTP response including its status code, headers, and body. It is useful when the controller needs explicit control over the response.

## Q13. What is ORM?

Object-Relational Mapping maps object-oriented application structures to relational database structures. It lets Java code work with entity objects while the ORM provider handles persistence mapping and SQL generation.

## Q14. What is the difference between JPA, Hibernate, and Spring Data JPA?

JPA is a persistence specification/API. Hibernate is an implementation of JPA and an ORM framework. Spring Data JPA provides repository abstractions that simplify data access using JPA.

## Q15. What is `JpaRepository`?

`JpaRepository` is a Spring Data interface that provides common CRUD, paging, sorting, and related persistence operations for an entity type.

## Q16. What is the difference between `findById()` and `getReferenceById()`?

`findById()` returns an `Optional` and performs a lookup to determine whether the entity exists. `getReferenceById()` obtains a reference/proxy and may defer database access until the entity is accessed; behavior when the row does not exist can surface later.

## Q17. What is the difference between `@OneToMany` and `@ManyToOne`?

`@OneToMany` models one parent associated with multiple child entities. `@ManyToOne` models multiple child entities associated with one parent. In a typical relational mapping, the many-to-one side commonly owns the foreign key.

## Q18. What does `mappedBy` mean?

`mappedBy` identifies the field on the owning entity that maps the relationship. It marks the current side as the inverse side; it does not define a second foreign key.

## Q19. What is the difference between LAZY and EAGER fetching?

LAZY fetching defers loading a relationship until it is accessed, while EAGER fetching requires the relationship to be fetched eagerly. The provider's SQL strategy may vary. Choose fetch behavior based on use cases and query performance.

## Q20. What is the N+1 query problem?

It occurs when one query loads a collection of parent entities and then additional queries are executed for related data for each parent. For N parent rows, this can result in one initial query plus N additional queries. Fetch joins, entity graphs, or batch fetching can help, depending on the use case.

## Q21. What is the difference between JPQL and native SQL?

JPQL queries entity classes and Java properties, while native SQL queries database tables and columns directly. JPQL is generally more portable; native SQL is useful for database-specific features.

## Q22. What is pagination and why is it important?

Pagination retrieves a subset of records rather than the entire result set. It reduces response size and resource usage and supports scalable list endpoints.

## Q23. What is the difference between `Page` and `Slice`?

`Page` includes total-count/page metadata and may require a count query. `Slice` focuses on whether more data exists and can avoid calculating the total number of records.

## Q24. What is a transaction?

A transaction is a unit of database work that should be completed as a whole. It helps preserve consistency when multiple database operations belong to one business operation.

## Q25. What does `@Transactional` do?

It defines a Spring-managed transaction boundary. Spring coordinates transaction start, commit, and rollback according to the configured transaction manager and rollback rules.

## Q26. Does every exception cause rollback with `@Transactional`?

No. By default, Spring rolls back for unchecked exceptions (`RuntimeException`) and `Error`. Checked exceptions do not trigger rollback by default unless rollback rules are configured.

## Q27. Why is `@Transactional` commonly used in the service layer?

A single business operation may call multiple repositories. Putting the transaction boundary around the service operation lets those database changes participate in one transaction.

## Q28. Why should we avoid exposing entities directly from controllers?

Direct entity exposure can leak internal fields, couple the API to the database model, cause serialization problems with relationships, and allow clients to submit fields they should not control. DTOs provide a safer boundary.

---

# 16. Quick Revision Sheet

| Concept | Remember |
|---|---|
| REST | Resource-oriented API style using HTTP |
| GET | Retrieve data |
| POST | Create/submit operation |
| PUT | Replace/update representation |
| PATCH | Partial update |
| `@RestController` | REST controller with response-body behavior |
| `@PathVariable` | Read value from URL path |
| `@RequestParam` | Read query parameter |
| `@RequestBody` | Convert request body (often JSON) into Java object |
| Controller | HTTP handling |
| Service | Business logic |
| Repository | Data access |
| Entity | Database-mapped object |
| Request DTO | Incoming data contract |
| Response DTO | Outgoing data contract |
| `@RestControllerAdvice` | Centralized REST exception handling |
| `@Valid` | Trigger validation |
| 200 | OK |
| 201 | Created |
| 204 | No Content |
| 400 | Bad Request |
| 401 | Unauthenticated |
| 403 | Forbidden |
| 404 | Not Found |
| 409 | Conflict |
| ORM | Object-relational mapping |
| JPA | Persistence specification/API |
| Hibernate | ORM/JPA implementation |
| Spring Data JPA | Repository abstraction |
| `JpaRepository` | CRUD, paging, sorting repository API |
| `@ManyToOne` | Many children point to one parent |
| `@OneToMany` | One parent has many children |
| `mappedBy` | Inverse side refers to owning-side field |
| LAZY | Defer relationship loading |
| EAGER | Fetch relationship eagerly |
| JPQL | Query entities and Java properties |
| Native SQL | Query database tables/columns directly |
| `Page` | Page data plus total metadata |
| `Slice` | Data plus whether more exists |
| `@Transactional` | Transaction boundary |
| ACID | Atomicity, Consistency, Isolation, Durability |

---

# 17. Day 03 Completion Checklist

Use this checklist while revising the day:

- [ ] I can explain REST and the purpose of HTTP methods.
- [ ] I can distinguish path variables, query parameters, and request bodies.
- [ ] I can explain the role of `DispatcherServlet`.
- [ ] I can explain Controller, Service, and Repository responsibilities.
- [ ] I can explain why Request DTOs and Response DTOs are useful.
- [ ] I can describe JSON-to-Java and Java-to-JSON conversion.
- [ ] I can explain custom exceptions and global exception handling.
- [ ] I can explain `@Valid` and common validation annotations.
- [ ] I can choose suitable HTTP status codes.
- [ ] I can explain JPA vs Hibernate vs Spring Data JPA.
- [ ] I can explain what `JpaRepository` provides.
- [ ] I can describe common JPA entity relationships.
- [ ] I can explain owning side, inverse side, and `mappedBy`.
- [ ] I can explain LAZY vs EAGER fetching and the N+1 problem.
- [ ] I can distinguish derived queries, JPQL, and native SQL.
- [ ] I can explain pagination with `Page` and `Slice`.
- [ ] I can explain transactions, ACID, and Spring rollback defaults.

---

## Final architecture to remember

```text
                 FLUTTER CLIENT
                       |
                       | HTTP + JSON
                       v
              SPRING BOOT REST API
                       |
                DispatcherServlet
                       |
                       v
                  CONTROLLER
             - HTTP mapping
             - Request DTO
             - Input validation
                       |
                       v
                    SERVICE
             - Business rules
             - Transaction boundary
             - DTO/entity mapping
                       |
                       v
                   REPOSITORY
             - JpaRepository
             - Derived / JPQL queries
                       |
                       v
                HIBERNATE / JPA
             - ORM mapping
             - Persistence context
             - SQL generation
                       |
                       v
                  POSTGRESQL
```

**The key idea:** The client communicates with controllers through HTTP. Controllers delegate to services. Services enforce business rules and coordinate repository operations. Repositories use JPA/Hibernate to work with the database. DTOs define the API boundary, validation protects incoming data, and transactions protect multi-step database operations.
'''

path = Path("/mnt/data/Day_03_Spring_Boot_REST_APIs.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
