from pathlib import Path

path = Path("/mnt/data/Day_14_Exception_Handling_Part_1.md")
old = path.read_text(encoding="utf-8")

# Preserve the detailed first five topics, and insert the remaining three topics
# before the interview/revision sections so the final document is one complete Day 14 file.
marker = "# 6. Important Interview Questions and Answers"
if marker not in old:
    raise ValueError("Expected Day 14 section marker was not found.")

first_five = old.split(marker, 1)[0]

additional = r'''# 6. Exception Handling Best Practices

Exception handling should make failures understandable, observable, and safe. It should not hide problems or make normal program flow unnecessarily complicated.

## 6.1 Catch only exceptions you can handle

A `catch` block should have a meaningful responsibility.

Good example:

```java
try {
    return Integer.parseInt(input);
} catch (NumberFormatException e) {
    throw new IllegalArgumentException(
            "Age must be a valid number",
            e
    );
}
```

This translates a low-level parsing failure into a clearer validation error while preserving the original cause.

Avoid catching a broad exception and silently continuing:

```java
try {
    processPayment();
} catch (Exception e) {
    // Do nothing
}
```

This is dangerous because the application may behave as if the payment succeeded when it actually failed.

## 6.2 Avoid swallowing exceptions

Swallowing an exception means catching it but doing nothing useful with it.

Bad:

```java
try {
    saveEmployee(employee);
} catch (Exception e) {
    // ignored
}
```

Possible consequences:

- The caller may believe the operation succeeded.
- Data may become inconsistent.
- The original failure becomes difficult to diagnose.
- Production incidents become harder to investigate.

If you cannot recover, let the exception propagate or translate it into a meaningful application exception.

## 6.3 Catch specific exceptions instead of `Exception` everywhere

Prefer:

```java
try {
    importEmployees();
} catch (IOException e) {
    log.error("Unable to read employee import file", e);
    throw new EmployeeImportException(
            "Employee import failed",
            e
    );
}
```

Over:

```java
try {
    importEmployees();
} catch (Exception e) {
    // Too broad for many situations
}
```

A broad catch can be appropriate at a carefully designed boundary, such as a final request-processing or scheduled-job boundary, but it must not hide the failure.

Do not normally catch `Error`. Errors generally indicate serious JVM or environment problems.

## 6.4 Preserve the original exception cause

When translating an exception, retain the cause.

```java
try {
    externalEmployeeClient.fetchEmployees();
} catch (IOException e) {
    throw new EmployeeImportException(
            "Could not fetch employee data",
            e
    );
}
```

Custom exception:

```java
public class EmployeeImportException extends RuntimeException {

    public EmployeeImportException(String message, Throwable cause) {
        super(message, cause);
    }
}
```

Why preserve the cause?

- It retains the original stack trace.
- It helps identify the actual low-level failure.
- It makes logs and debugging more useful.

Do not replace a useful cause with only a generic message.

## 6.5 Do not use exceptions for normal control flow

Exceptions are intended for exceptional situations, not routine decisions.

Avoid:

```java
try {
    int age = Integer.parseInt(input);
} catch (NumberFormatException e) {
    // Used as the normal way to check every input
}
```

If input validation can be performed clearly before parsing, validate it first. Parsing may still fail, so handle invalid input at the appropriate boundary.

Likewise, use ordinary conditions for normal business decisions:

```java
if (!driver.isAvailable()) {
    return AssignmentResult.driverUnavailable();
}
```

A business exception may still be appropriate when the operation contract says the request cannot be completed and the API should return an error.

## 6.6 Use meaningful exception names and messages

Good names communicate the failure:

- `EmployeeNotFoundException`
- `DriverNotAvailableException`
- `VehicleCapacityExceededException`
- `TripAssignmentConflictException`

Avoid vague names such as:

- `MyException`
- `SomethingWrongException`
- `ErrorOccurredException`

A message should provide useful context without exposing secrets or sensitive data.

```java
throw new EmployeeNotFoundException(
        "Employee not found with ID: " + employeeId
);
```

Do not include passwords, access tokens, session identifiers, or other secrets in exception messages or logs.

## 6.7 Log exceptions at the right boundary

Log an exception where the application has enough context to make the log useful and where it will not be logged repeatedly.

```java
try {
    importEmployees();
} catch (IOException e) {
    log.error("Employee import failed for file {}", fileName, e);
    throw new EmployeeImportException(
            "Unable to import employee file",
            e
    );
}
```

Passing the exception as the final logging argument records its stack trace.

Avoid logging the same exception at every layer. Repeated logging creates noise and makes one failure look like several different failures.

A useful log entry generally includes:

- What operation failed.
- Relevant non-sensitive identifier or correlation ID.
- The exception and stack trace.
- Enough context to investigate the failure.

## 6.8 Never expose internal details to API clients

A production API should not return raw stack traces, SQL statements, database passwords, internal hostnames, or implementation details to clients.

Return a safe error response with:

- Appropriate HTTP status.
- A clear client-facing message.
- A stable application error code, where useful.
- A correlation or trace ID, where appropriate.

Keep detailed diagnostic information in protected server-side logs.

## 6.9 Avoid returning `null` after a failure

This pattern hides the failure:

```java
public Employee getEmployee(Long id) {
    try {
        return repository.findById(id).orElseThrow();
    } catch (Exception e) {
        return null;
    }
}
```

The caller may later get a `NullPointerException`, far away from the original problem.

Prefer throwing a meaningful exception or returning an explicit result type when absence is a normal outcome.

## 6.10 Keep `finally` and cleanup code safe

Use try-with-resources for `AutoCloseable` resources where possible.

Avoid returning or throwing a new exception from `finally`, because it can override the original result or exception.

## 6.11 Make transaction boundaries clear

When an operation modifies related database records, define a suitable transaction boundary.

```java
@Transactional
public void assignEmployeeToTrip(Long tripId, Long employeeId) {
    // Load trip and employee.
    // Validate assignment rules.
    // Update the relationship.
}
```

By default, Spring's transaction interceptor rolls back on unchecked exceptions (`RuntimeException` and `Error`), not on checked exceptions. If a checked exception should trigger rollback, configure rollback rules, for example:

```java
@Transactional(rollbackFor = Exception.class)
public void importEmployees() throws IOException {
    // Transactional work
}
```

Remember that Spring's usual declarative transaction behavior is proxy-based. Self-invocation within the same bean does not pass through the proxy in the usual configuration, so the inner method's `@Transactional` annotation may not create a new transaction boundary.

## 6.12 Best-practice summary

| Practice | Why it matters |
|---|---|
| Catch only what you can handle | Prevents accidental hiding of failures |
| Avoid empty catch blocks | Keeps failures visible |
| Prefer specific exception types | Makes handling precise |
| Preserve the cause | Retains diagnostic information |
| Use meaningful custom exceptions | Communicates business failures |
| Do not use exceptions for routine branching | Keeps normal logic clear |
| Log at an appropriate boundary | Reduces duplicate/noisy logs |
| Do not expose stack traces to clients | Protects internal implementation details |
| Avoid returning `null` after failure | Prevents delayed, confusing errors |
| Define transaction rollback rules | Protects data consistency |

---

# 7. Exception Handling in Spring Boot

Spring Boot applications use Java exceptions together with Spring MVC's request-processing and exception-resolution mechanisms.

## 7.1 Where exceptions can occur in a backend request

Consider an Employee Transport API:

```text
Flutter Client
     |
     | HTTP request
     v
DispatcherServlet
     |
     v
EmployeeController
     |
     v
EmployeeService
     |
     v
EmployeeRepository
     |
     v
PostgreSQL
```

An exception can originate in several places:

- Controller: request binding or validation failure.
- Service: business rule violation or missing employee.
- Repository: database access failure.
- External client: remote service or network failure.

The exception may propagate until a suitable handler resolves it.

## 7.2 Controller-level exception handling

A controller can define an `@ExceptionHandler` method for an exception relevant to that controller.

```java
@RestController
@RequestMapping("/api/employees")
public class EmployeeController {

    @GetMapping("/{id}")
    public Employee getEmployee(@PathVariable Long id) {
        // Call service
        return employeeService.getEmployee(id);
    }

    @ExceptionHandler(EmployeeNotFoundException.class)
    public ResponseEntity<String> handleEmployeeNotFound(
            EmployeeNotFoundException exception) {

        return ResponseEntity
                .status(HttpStatus.NOT_FOUND)
                .body(exception.getMessage());
    }
}
```

This handler is local to the controller's exception-handling scope.

For consistent handling across many controllers, centralized handling is usually preferable.

## 7.3 Global exception handling with `@RestControllerAdvice`

`@RestControllerAdvice` provides centralized exception handling for controller requests. It combines controller-advice behavior with response-body serialization.

```java
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(EmployeeNotFoundException.class)
    public ResponseEntity<String> handleEmployeeNotFound(
            EmployeeNotFoundException exception) {

        return ResponseEntity
                .status(HttpStatus.NOT_FOUND)
                .body(exception.getMessage());
    }
}
```

When a matching exception occurs during Spring MVC request processing, Spring can invoke this handler and serialize the returned body.

Benefits:

- Avoids repeating exception-handling methods in every controller.
- Keeps controllers focused on request handling.
- Provides consistent API error responses.
- Centralizes the mapping between application exceptions and HTTP statuses.

## 7.4 `@ExceptionHandler`

`@ExceptionHandler` marks a method that handles one or more specified exception types.

```java
@ExceptionHandler(DriverNotAvailableException.class)
public ResponseEntity<String> handleDriverNotAvailable(
        DriverNotAvailableException exception) {

    return ResponseEntity
            .status(HttpStatus.CONFLICT)
            .body(exception.getMessage());
}
```

The exception type can be inferred from the method parameter in supported handler declarations, but explicitly listing it makes the mapping easy to read.

## 7.5 `ResponseEntity`

`ResponseEntity` represents the full HTTP response, including:

- Status code.
- Headers.
- Body.

Example:

```java
return ResponseEntity
        .status(HttpStatus.NOT_FOUND)
        .body("Employee not found");
```

For successful responses, it can also be used:

```java
return ResponseEntity.ok(employee);
```

## 7.6 Common HTTP status mappings

| Situation | Common status |
|---|---:|
| Request completed successfully | 200 OK |
| Resource created | 201 Created |
| Request is invalid or cannot be processed due to validation | 400 Bad Request |
| Authentication is missing or invalid | 401 Unauthorized |
| Authenticated user lacks permission | 403 Forbidden |
| Requested resource does not exist | 404 Not Found |
| Request conflicts with current resource state | 409 Conflict |
| Unexpected server-side failure | 500 Internal Server Error |
| Required upstream service is unavailable | 503 Service Unavailable |

The correct status depends on the API contract and the actual failure. Do not map every exception to 400 or 500.

## 7.7 Validation exceptions

Spring MVC validation can fail before the service method executes.

Example DTO:

```java
public class CreateEmployeeRequest {

    @NotBlank(message = "Employee name is required")
    private String name;

    @Email(message = "Email must be valid")
    private String email;

    // Getters and setters
}
```

Controller:

```java
@PostMapping
public ResponseEntity<String> createEmployee(
        @Valid @RequestBody CreateEmployeeRequest request) {

    return ResponseEntity
            .status(HttpStatus.CREATED)
            .body("Employee created");
}
```

For many request-body validation failures, Spring MVC raises `MethodArgumentNotValidException`. A global handler can convert the field errors into a structured 400 response.

```java
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
                    )
            );

    return ResponseEntity
            .badRequest()
            .body(errors);
}
```

Example response:

```json
{
  "name": "Employee name is required",
  "email": "Email must be valid"
}
```

This example uses a map for simplicity. In a production API, consider a dedicated error-response DTO and a consistent error schema.

## 7.8 Authentication and authorization exceptions

Spring Security has its own security exception-handling mechanisms.

Typical distinction:

- `AuthenticationEntryPoint`: handles unauthenticated access, commonly HTTP 401.
- `AccessDeniedHandler`: handles authenticated users who lack permission, commonly HTTP 403.

These are configured in the Spring Security filter chain. They are not simply ordinary controller exceptions, because security filters can reject a request before it reaches the controller.

## 7.9 Spring Boot's default error handling

Spring Boot provides default error handling for failures that are not resolved by custom handlers. For Spring MVC applications, the `/error` mechanism and its error attributes can produce an error response.

Do not rely on defaults as the complete production API contract. Define and test a consistent error format, and ensure sensitive details are not exposed.

---

# 8. Global Exception Handling and Production Scenarios

This topic combines the previous concepts into a maintainable API error-handling design.

## 8.1 Create a standard API error response

Instead of returning a plain string for every error, define a response DTO.

```java
import java.time.Instant;

public class ApiErrorResponse {

    private Instant timestamp;
    private int status;
    private String error;
    private String message;
    private String path;

    public ApiErrorResponse(
            Instant timestamp,
            int status,
            String error,
            String message,
            String path) {
        this.timestamp = timestamp;
        this.status = status;
        this.error = error;
        this.message = message;
        this.path = path;
    }

    public Instant getTimestamp() {
        return timestamp;
    }

    public int getStatus() {
        return status;
    }

    public String getError() {
        return error;
    }

    public String getMessage() {
        return message;
    }

    public String getPath() {
        return path;
    }
}
```

A consistent response makes it easier for Flutter or another client to process failures.

Example JSON:

```json
{
  "timestamp": "2026-11-08T12:00:00Z",
  "status": 404,
  "error": "Not Found",
  "message": "Employee not found",
  "path": "/api/employees/42"
}
```

The timestamp and path above are illustrative.

## 8.2 Build a centralized handler

The following example demonstrates a small, consistent handler. It assumes the custom exceptions shown earlier exist.

```java
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(EmployeeNotFoundException.class)
    public ResponseEntity<ApiErrorResponse> handleEmployeeNotFound(
            EmployeeNotFoundException exception,
            HttpServletRequest request) {

        return buildError(
                HttpStatus.NOT_FOUND,
                exception.getMessage(),
                request.getRequestURI()
        );
    }

    @ExceptionHandler(DriverNotAvailableException.class)
    public ResponseEntity<ApiErrorResponse> handleDriverNotAvailable(
            DriverNotAvailableException exception,
            HttpServletRequest request) {

        return buildError(
                HttpStatus.CONFLICT,
                exception.getMessage(),
                request.getRequestURI()
        );
    }

    @ExceptionHandler(MethodArgumentNotValidException.class)
    public ResponseEntity<ApiErrorResponse> handleValidation(
            MethodArgumentNotValidException exception,
            HttpServletRequest request) {

        String message = exception.getBindingResult()
                .getFieldErrors()
                .stream()
                .map(error ->
                        error.getField() + ": " + error.getDefaultMessage()
                )
                .collect(Collectors.joining("; "));

        return buildError(
                HttpStatus.BAD_REQUEST,
                message,
                request.getRequestURI()
        );
    }

    @ExceptionHandler(Exception.class)
    public ResponseEntity<ApiErrorResponse> handleUnexpected(
            Exception exception,
            HttpServletRequest request) {

        // Log the full exception on the server.
        log.error("Unexpected API error on {}", request.getRequestURI(),
                exception);

        // Return a safe generic message to the client.
        return buildError(
                HttpStatus.INTERNAL_SERVER_ERROR,
                "An unexpected error occurred",
                request.getRequestURI()
        );
    }

    private ResponseEntity<ApiErrorResponse> buildError(
            HttpStatus status,
            String message,
            String path) {

        ApiErrorResponse body = new ApiErrorResponse(
                Instant.now(),
                status.value(),
                status.getReasonPhrase(),
                message,
                path
        );

        return ResponseEntity.status(status).body(body);
    }
}
```

Imports and logging setup are omitted from the snippet for readability. For example, it uses `HttpServletRequest`, `HttpStatus`, `ResponseEntity`, `Instant`, `Collectors`, and a configured logger.

The generic `Exception` handler should be a final fallback, not a replacement for specific handlers. It should log the exception server-side and return a safe message.

## 8.3 Production scenario: employee does not exist

Request:

```http
GET /api/employees/999
```

Flow:

```text
Client
  |
  v
EmployeeController
  |
  v
EmployeeService
  |
  v
EmployeeRepository
  |
  v
No employee found
  |
  v
EmployeeNotFoundException
  |
  v
@RestControllerAdvice
  |
  v
HTTP 404 response
```

The service throws a meaningful exception. The global handler converts it to a structured 404 response.

## 8.4 Production scenario: duplicate or conflicting assignment

Suppose an employee is already assigned to a trip and the business rule prevents another assignment.

The service can throw:

```java
throw new TripAssignmentConflictException(
        "Employee is already assigned to this trip"
);
```

A handler can map it to HTTP 409 Conflict.

A 409 response is suitable when the request conflicts with the current state of the resource. The exact choice should follow the API's documented contract.

## 8.5 Production scenario: invalid request data

Suppose a client sends an empty employee name.

The DTO validation annotation rejects the request. Spring MVC raises a validation exception, and the global handler returns HTTP 400 with field-level messages.

This is different from an employee-not-found condition:

- Invalid input: 400 Bad Request.
- Valid request referring to a missing employee: 404 Not Found.

## 8.6 Production scenario: unexpected database failure

A database connection may fail or a query may violate a database constraint.

Spring's data-access exception translation may expose a `DataAccessException` or a more specific subclass.

Do not return raw SQL or database internals to the client.

A typical approach is:

1. Log the full exception securely.
2. Roll back the transaction when required.
3. Return a safe server error or a more specific conflict response if the failure is a known constraint conflict.
4. Preserve enough diagnostic context for investigation.

Avoid blindly converting every database exception into 409. Determine whether it represents a known client conflict or an unexpected server failure.

## 8.7 Production scenario: remote service timeout

If a backend calls another service and the call times out, the application should use a deliberate timeout and resilience policy.

Depending on the API contract, the failure may map to 502 Bad Gateway, 503 Service Unavailable, or another appropriate response.

Do not expose internal service URLs, credentials, or stack traces to the client.

## 8.8 Correlation IDs and observability

A correlation ID helps connect logs belonging to one request across application components or services.

A useful error response may include a request or trace identifier:

```json
{
  "status": 500,
  "message": "An unexpected error occurred",
  "traceId": "example-trace-id"
}
```

The value shown is illustrative. Generate or obtain the identifier through a proper request-tracing mechanism; do not invent one per error response without connecting it to logs.

In a distributed system, trace IDs can help follow a request across multiple services.

## 8.9 Security considerations

A production exception response should not reveal:

- Java stack traces.
- SQL statements or database schema details.
- Passwords or authentication tokens.
- Internal server paths.
- Private service hostnames.
- Secrets or environment variables.

Detailed stack traces belong in protected logs, not in public API responses.

## 8.10 Practical design rules

1. Throw business-specific exceptions from the service layer.
2. Use `@RestControllerAdvice` for consistent Spring MVC error responses.
3. Use `@ExceptionHandler` for specific exception-to-status mappings.
4. Return a stable error-response DTO.
5. Validate request data at the API boundary.
6. Use 401 and 403 handling through Spring Security's security mechanisms.
7. Log unexpected failures with stack traces on the server.
8. Do not expose internal details to clients.
9. Preserve original exception causes when translating failures.
10. Define transaction rollback behavior for checked and unchecked exceptions.
11. Avoid duplicate logging at every layer.
12. Document the API's error response contract for frontend developers.

---

# 9. Interview Questions and Answers

## Q1. What are the best practices for exception handling?

Catch only exceptions you can handle, avoid empty catch blocks, prefer specific exception types, preserve causes, use meaningful messages, log at appropriate boundaries, and do not expose internal details to API clients.

## Q2. Why should we avoid catching `Exception` everywhere?

It can hide programming errors and unrelated failures. Specific exception handling makes recovery behavior more predictable. A broad catch can be used as a final boundary if it logs and responds safely.

## Q3. Why should we not swallow exceptions?

Swallowing exceptions makes failures invisible and can cause the application to continue in an invalid state. It also makes production debugging difficult.

## Q4. Why preserve the original exception cause?

The cause retains the lower-level exception and its stack trace, which helps identify the real source of failure after translating it into an application-specific exception.

## Q5. What is `@ControllerAdvice`?

It allows controller-related concerns, including exception handling and model-related behavior, to be shared across controllers.

## Q6. What is `@RestControllerAdvice`?

It is a convenience annotation combining `@ControllerAdvice` and `@ResponseBody`, so handler return values are written to the response body.

## Q7. What is `@ExceptionHandler`?

It marks a method that handles specified exception types in a controller or controller advice.

## Q8. What is the difference between local and global exception handling?

A local handler is defined within a controller and applies to that controller's scope. A global handler is defined through controller advice and can apply across controllers.

## Q9. How do you handle validation errors in Spring Boot?

Use validation annotations on request DTO fields and `@Valid` on the controller parameter. Handle `MethodArgumentNotValidException` centrally and return field-level messages with HTTP 400.

## Q10. What is the difference between 401 and 403?

401 means the request lacks valid authentication. 403 means the user is authenticated but does not have permission to access the resource. Spring Security commonly handles these through `AuthenticationEntryPoint` and `AccessDeniedHandler`.

## Q11. Should a production API return a stack trace?

No. Return a safe, consistent error response. Keep full stack traces in protected server-side logs.

## Q12. What should happen when an employee is not found?

The service can throw `EmployeeNotFoundException`. A global handler maps it to HTTP 404 and returns the API's standard error body.

## Q13. What is the purpose of a standard error-response DTO?

It gives clients a consistent structure containing information such as timestamp, status, error type, safe message, path, and optionally a trace identifier.

## Q14. How should unexpected exceptions be handled?

Log the full exception securely, return a generic safe HTTP 500 response, and preserve diagnostic information in server-side observability systems.

## Q15. Does `@RestControllerAdvice` handle every exception in a Spring Boot application?

No. It participates in Spring MVC controller exception resolution. Some failures occur before controller invocation or in other execution contexts, such as Spring Security filters, scheduled jobs, asynchronous tasks, or background consumers. Those contexts may need their own handling mechanisms.

## Q16. Does `@Transactional` roll back for checked exceptions by default?

By default, Spring's declarative transaction management rolls back for unchecked exceptions (`RuntimeException`) and `Error`, but not for checked exceptions. Use rollback rules such as `rollbackFor` when required.

## Q17. Should every business condition be represented by an exception?

No. Normal expected decisions can be represented with ordinary return values or result types. Exceptions are appropriate when an operation cannot proceed according to its contract or when an exceptional failure occurs.

## Q18. Why should we avoid logging the same exception at every layer?

It creates duplicate stack traces and noisy logs. Log at a boundary where the failure can be meaningfully recorded, while preserving the exception as it propagates.

## Q19. How do you handle a database constraint violation?

Understand which constraint failed. If it represents a known client conflict, map it to a suitable response such as 409. For an unexpected database failure, log securely and return an appropriate server error. Do not expose SQL internals.

## Q20. How do you handle a downstream service timeout?

Set explicit timeouts, apply an appropriate resilience policy, log the failure with request context, and return a status consistent with the API contract, often 502 or 503 depending on the situation.

---

# 10. Complete Day 14 Revision Sheet

| Topic | Key point |
|---|---|
| Exception | Event that disrupts normal program execution |
| `Throwable` | Parent type for throwable objects |
| `Exception` | Conditions application code may handle |
| `Error` | Serious runtime or environment problem |
| Checked exception | Must be caught or declared |
| Unchecked exception | No compiler requirement to catch or declare |
| `try` | Contains risky code |
| `catch` | Handles a matching exception |
| `finally` | Normally executes; commonly used for cleanup |
| `throw` | Explicitly throws an exception object |
| `throws` | Declares possible exceptions |
| Try-with-resources | Automatically closes `AutoCloseable` resources |
| Custom exception | Represents an application-specific failure |
| Exception cause | Preserves lower-level diagnostic information |
| Propagation | Unhandled exception travels up the call stack |
| Best practice | Catch only what you can handle |
| Logging | Log once at a useful boundary with context |
| `@ExceptionHandler` | Handles selected exceptions |
| `@RestControllerAdvice` | Centralizes Spring MVC exception handling |
| `ResponseEntity` | Represents HTTP status, headers, and body |
| `@Valid` | Triggers request DTO validation |
| `MethodArgumentNotValidException` | Common request-body validation failure |
| HTTP 400 | Invalid request |
| HTTP 401 | Missing or invalid authentication |
| HTTP 403 | Authenticated but forbidden |
| HTTP 404 | Resource not found |
| HTTP 409 | Conflict with current resource state |
| HTTP 500 | Unexpected server-side failure |
| Transaction rollback | Unchecked exceptions and `Error` by default in Spring |

---

# 11. Day 14 Completion Checklist

- [ ] I can explain the `Throwable` hierarchy.
- [ ] I can distinguish `Exception` from `Error`.
- [ ] I can identify common runtime exceptions.
- [ ] I can distinguish checked and unchecked exceptions.
- [ ] I can create custom checked and unchecked exceptions.
- [ ] I understand `try`, `catch`, and `finally`.
- [ ] I can explain `throw` vs `throws`.
- [ ] I understand try-with-resources and `AutoCloseable`.
- [ ] I can preserve an original exception as the cause of a custom exception.
- [ ] I can explain exception propagation through a method call stack.
- [ ] I can explain why swallowing exceptions is dangerous.
- [ ] I can choose specific exception types and meaningful messages.
- [ ] I understand logging and safe error responses.
- [ ] I can explain `@ExceptionHandler`.
- [ ] I can explain `@ControllerAdvice` and `@RestControllerAdvice`.
- [ ] I can handle validation errors in Spring Boot.
- [ ] I can distinguish 400, 401, 403, 404, 409, and 500.
- [ ] I understand the default Spring transaction rollback rules.
- [ ] I can describe a production-ready exception-handling flow.

---

# Day 14 Takeaway

Reliable backend exception handling is not simply about writing `try-catch`. It is about representing failures clearly, allowing exceptions to propagate to the right boundary, preserving diagnostic causes, protecting internal details, and returning predictable API responses.

In a Spring Boot application, keep business decisions in the service layer, use meaningful application exceptions, and centralize controller error responses with `@RestControllerAdvice`. Handle security-filter failures and background-task failures in their own execution contexts.

**Day 14 is now complete: all eight planned topics are included in this single document.**
'''

# Remove any accidental code-generation text if present in the preserved portion.
# The preserved content is the clean learning material from the original file.
final_content = first_five + additional
final_path = Path("/mnt/data/Backend-notes (Day 14).md")
final_path.write_text(final_content, encoding="utf-8")

print(f"Created: {final_path}")
print(f"Words: {len(final_content.split()):,}")
print(f"Characters: {len(final_content):,}")
print(f"Lines: {len(final_content.splitlines()):,}")
