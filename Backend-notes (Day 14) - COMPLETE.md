# Day 14 — Advanced Java and Backend Concepts: Exception Handling

> **Java Backend Developer Interview Preparation**  
> Status: Complete Day 14 learning notes — all eight planned topics.  
> Focus: Java exception fundamentals, exception best practices, Spring Boot exception handling, and production-ready API error responses.  
> Examples connect to Java and the Employee Transport Spring Boot backend.

---

## Learning objectives

After studying the topics covered in this part, you should be able to:

- Explain what an exception is and why exception handling is needed.
- Understand the `Throwable` hierarchy and distinguish `Exception` from `Error`.
- Differentiate checked and unchecked exceptions.
- Use `try`, `catch`, `finally`, `throw`, and `throws`.
- Use try-with-resources for resource cleanup.
- Create custom checked and unchecked exceptions.
- Explain how an exception propagates through a method call stack.
- Describe how exceptions can move through a Spring Boot controller/service/repository flow.

---

## Contents

1. Exception Handling Fundamentals
2. Checked vs Unchecked Exceptions
3. `try`, `catch`, `finally`, `throw`, and `throws`
4. Custom Exceptions
5. Exception Propagation in Java
6. Exception Handling Best Practices
7. Exception Handling in Spring Boot
8. Global Exception Handling and Production Scenarios
9. Interview Questions and Answers
10. Complete Day 14 Revision Sheet
11. Day 14 Completion Checklist

---

# 1. Exception Handling Fundamentals

## 1.1 What is an exception?

An exception is an event that occurs during program execution and disrupts the normal flow of instructions.

For example:

```java
int result = 10 / 0;
```

Integer division by zero causes an `ArithmeticException`.

Other examples include:

- Trying to access an object through a `null` reference.
- Accessing an array using an invalid index.
- Converting invalid text into a number.
- Trying to read a file that does not exist.
- Accessing a database when a query or connection operation fails.

Without appropriate handling, an exception can terminate the current operation and may terminate the thread.

## 1.2 Why do we need exception handling?

Exception handling helps an application respond to unexpected or exceptional situations in a controlled way.

It helps developers:

1. Prevent avoidable application crashes.
2. Separate normal business logic from error-handling logic.
3. Provide meaningful error messages.
4. Clean up resources such as files and database-related resources.
5. Log useful diagnostic information.
6. Return appropriate HTTP error responses from backend APIs.

Exception handling does not mean hiding every error. A good application handles errors it can recover from and allows unexpected failures to be reported and diagnosed.

## 1.3 Java exception hierarchy

At the top of Java's exception hierarchy is `Throwable`.

```text
                 Object
                   |
                Throwable
                /       \
             Error     Exception
                         |
                 -----------------
                 |               |
          Checked exceptions   RuntimeException
                               (Unchecked)
```

This is a simplified hierarchy. `Error` and `Exception` are both subclasses of `Throwable`.

### Throwable

`Throwable` is the superclass of objects that can be thrown and caught.

### Error

`Error` represents serious problems that applications generally should not try to recover from in ordinary business logic.

Examples:

- `OutOfMemoryError`
- `StackOverflowError`
- `NoClassDefFoundError`

### Exception

`Exception` represents conditions that an application may be able to handle.

Examples include:

- `IOException`
- `SQLException`
- `ClassNotFoundException`
- `RuntimeException` and its subclasses

## 1.4 Common runtime exceptions

| Exception | Typical cause |
|---|---|
| `NullPointerException` | Calling a method or accessing a field through `null` |
| `ArithmeticException` | Invalid arithmetic operation, such as integer division by zero |
| `NumberFormatException` | Parsing invalid numeric text |
| `ArrayIndexOutOfBoundsException` | Accessing an invalid array index |
| `IndexOutOfBoundsException` | Invalid index in a list or another indexed structure |
| `ClassCastException` | Invalid runtime type cast |
| `IllegalArgumentException` | Passing an argument that violates a method's requirements |
| `IllegalStateException` | Calling a method when an object is in an unsuitable state |

Example:

```java
public class ExceptionExample {

    public static void main(String[] args) {
        String value = null;

        System.out.println(value.length());
    }
}
```

This throws a `NullPointerException`.

## 1.5 Exception vs Error

| Exception | Error |
|---|---|
| Often represents a condition application code may handle | Usually represents a serious runtime or environment problem |
| May be checked or unchecked | Is unchecked |
| Examples: `IOException`, `SQLException`, `NullPointerException` | Examples: `OutOfMemoryError`, `StackOverflowError` |
| Application-level recovery may be possible | Ordinary business code generally should not attempt recovery |

**Interview answer:** `Exception` represents conditions an application may be able to handle. `Error` represents serious problems that application code generally should not try to recover from. Both extend `Throwable`.

---

# 2. Checked vs Unchecked Exceptions

Java divides exceptions into checked and unchecked categories.

## 2.1 Checked exceptions

Checked exceptions are checked by the compiler.

A method that may throw a checked exception must either:

1. Handle it with `try-catch`, or
2. Declare it using `throws`.

Common examples:

- `IOException`
- `FileNotFoundException`
- `SQLException`
- `ClassNotFoundException`

Example:

```java
import java.io.FileReader;
import java.io.IOException;

public class FileExample {

    public static void main(String[] args) {
        try {
            FileReader reader = new FileReader("data.txt");
            reader.close();
        } catch (IOException e) {
            System.out.println("Unable to read the file");
        }
    }
}
```

If the checked exception is neither handled nor declared, the compiler reports an error.

## 2.2 Unchecked exceptions

Unchecked exceptions are not checked by the compiler at compile time.

They include:

- `RuntimeException` and its subclasses.
- `Error` and its subclasses.

Common runtime exceptions include:

- `NullPointerException`
- `ArithmeticException`
- `NumberFormatException`
- `ClassCastException`
- `IllegalArgumentException`

Example:

```java
public class NumberExample {

    public static void main(String[] args) {
        int number = Integer.parseInt("abc");
    }
}
```

This compiles, but throws `NumberFormatException` at runtime.

A method does not have to declare an unchecked exception with `throws`, although it may document one.

## 2.3 Checked vs unchecked comparison

| Checked exception | Unchecked exception |
|---|---|
| Compiler requires handling or declaration | Compiler does not require handling or declaration |
| Subclasses of `Exception` other than `RuntimeException` subclasses | Subclasses of `RuntimeException`; `Error` is also unchecked |
| Often represents recoverable external conditions | Often represents programming errors, invalid state, or invalid arguments |
| Example: `IOException` | Example: `NullPointerException` |

## 2.4 Creating custom checked exceptions

Extend `Exception` when you want the caller to be required to handle or declare the condition.

```java
public class InvalidFileFormatException extends Exception {

    public InvalidFileFormatException(String message) {
        super(message);
    }
}
```

A method that throws it must handle or declare it:

```java
public void importEmployees(String fileName)
        throws InvalidFileFormatException {

    if (!fileName.endsWith(".csv")) {
        throw new InvalidFileFormatException(
                "Only CSV files are supported"
        );
    }
}
```

## 2.5 Creating custom unchecked exceptions

Extend `RuntimeException` when callers should not be forced by the compiler to catch or declare the exception.

```java
public class EmployeeNotFoundException
        extends RuntimeException {

    public EmployeeNotFoundException(String message) {
        super(message);
    }
}
```

Usage:

```java
public Employee getEmployee(Long id) {
    return repository.findById(id)
            .orElseThrow(() ->
                    new EmployeeNotFoundException(
                            "Employee not found: " + id
                    )
            );
}
```

## 2.6 Which type should be used in a Spring Boot backend?

For many business-rule failures in REST applications, custom unchecked exceptions are common.

Examples:

- `EmployeeNotFoundException`
- `DriverNotAvailableException`
- `VehicleNotAvailableException`

The service can throw the exception, and a global exception handler can translate it into an HTTP response.

Checked exceptions remain appropriate when the API contract genuinely requires callers to handle a recoverable condition, such as certain file or I/O operations.

Spring also translates many persistence-related exceptions into unchecked exceptions under its data-access exception hierarchy, including `DataAccessException`.

**Interview answer:** Checked exceptions must be caught or declared. Unchecked exceptions extend `RuntimeException` (or are `Error` types) and do not have that compiler requirement. Choose based on whether callers should be forced to handle the condition, not merely on personal preference.

---

# 3. try, catch, finally, throw, and throws

## 3.1 try block

The `try` block contains code that may throw an exception.

```java
try {
    int result = 10 / 2;
    System.out.println(result);
}
```

A `try` block must be followed by at least one `catch` block, a `finally` block, or both.

## 3.2 catch block

A `catch` block handles a matching exception.

```java
try {
    int result = 10 / 0;
} catch (ArithmeticException e) {
    System.out.println("Cannot divide by zero");
}
```

The exception type in the catch block determines which exceptions it can handle.

## 3.3 Multiple catch blocks

You can handle different exception types separately.

```java
try {
    int number = Integer.parseInt("abc");
    int result = 10 / number;
} catch (NumberFormatException e) {
    System.out.println("Invalid number format");
} catch (ArithmeticException e) {
    System.out.println("Invalid arithmetic operation");
}
```

When catch types are related by inheritance, catch the more specific type before the more general type.

Incorrect ordering:

```java
try {
    // risky operation
} catch (Exception e) {
    // general handler
}
// A later catch for IOException would be unreachable.
```

## 3.4 Multi-catch

When different unrelated exception types need the same handling, Java supports multi-catch:

```java
try {
    // operation that may fail in different ways
} catch (IOException | SQLException e) {
    System.out.println("External operation failed");
}
```

The alternatives in a multi-catch cannot be related through subclassing.

## 3.5 finally block

The `finally` block is commonly used for cleanup code.

```java
try {
    System.out.println("Opening resource");
} catch (Exception e) {
    System.out.println("Handling error");
} finally {
    System.out.println("Cleanup operation");
}
```

The `finally` block normally executes whether an exception occurs or not.

However, it is not an absolute guarantee: the JVM may terminate, the process may be forcibly stopped, or the machine may fail before it runs.

Avoid returning from `finally`, because it can override a return value or suppress an exception from the `try` or `catch` block.

## 3.6 throw keyword

`throw` explicitly throws an exception object.

```java
public void validateAge(int age) {

    if (age < 18) {
        throw new IllegalArgumentException(
                "Age must be at least 18"
        );
    }
}
```

The method creates and throws the exception when the condition is invalid.

## 3.7 throws keyword

`throws` declares exceptions that a method may pass to its caller.

```java
public void readFile() throws IOException {
    FileReader reader = new FileReader("data.txt");
    reader.close();
}
```

The caller must handle or declare the checked exception.

## 3.8 throw vs throws

| `throw` | `throws` |
|---|---|
| Used inside a method or block | Used in a method declaration |
| Throws an exception object | Declares possible exceptions |
| Followed by an exception instance | Followed by exception class names |
| Example: `throw new IllegalArgumentException()` | Example: `void read() throws IOException` |

## 3.9 try-with-resources

Java provides try-with-resources for objects that implement `AutoCloseable` (including `Closeable`).

It automatically closes the resource when the try block exits, including when an exception occurs.

```java
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class FileReaderExample {

    public static void main(String[] args) {

        try (BufferedReader reader =
                     new BufferedReader(new FileReader("data.txt"))) {

            String line = reader.readLine();
            System.out.println(line);

        } catch (IOException e) {
            System.out.println("File operation failed");
        }
    }
}
```

This is generally preferable to manually closing the resource in a `finally` block.

Multiple resources can be declared:

```java
try (
    FileReader fileReader = new FileReader("data.txt");
    BufferedReader reader = new BufferedReader(fileReader)
) {
    System.out.println(reader.readLine());
}
```

Resources are closed automatically in reverse order of their initialization.

## 3.10 Exception wrapping and preserving the cause

Sometimes a lower-level exception should be translated into a higher-level application exception.

```java
try {
    readExternalFile();
} catch (IOException e) {
    throw new EmployeeImportException(
            "Unable to import employee data",
            e
    );
}
```

The custom exception should have a constructor that accepts the cause:

```java
public class EmployeeImportException extends RuntimeException {

    public EmployeeImportException(String message, Throwable cause) {
        super(message, cause);
    }
}
```

Preserving the original cause helps debugging because the stack trace retains the underlying failure.

---

# 4. Custom Exceptions

## 4.1 What is a custom exception?

A custom exception is an exception class created by the application to represent a meaningful application-specific failure.

Java provides general exceptions such as `IllegalArgumentException`, but a backend often needs clearer business terms.

Examples:

- Employee record does not exist.
- Driver is not available.
- Vehicle capacity is insufficient.
- Trip assignment is not allowed in the current state.

## 4.2 Custom unchecked exception

```java
public class EmployeeNotFoundException
        extends RuntimeException {

    public EmployeeNotFoundException(String message) {
        super(message);
    }
}
```

The class extends `RuntimeException`, so it is unchecked.

## 4.3 Custom exception with a cause

```java
public class EmployeeImportException
        extends RuntimeException {

    public EmployeeImportException(
            String message,
            Throwable cause) {
        super(message, cause);
    }
}
```

Use this when translating a lower-level exception while retaining its cause.

## 4.4 Employee Transport examples

Driver unavailable:

```java
public class DriverNotAvailableException
        extends RuntimeException {

    public DriverNotAvailableException(String message) {
        super(message);
    }
}
```

Vehicle unavailable:

```java
public class VehicleNotAvailableException
        extends RuntimeException {

    public VehicleNotAvailableException(String message) {
        super(message);
    }
}
```

These names describe business conditions more clearly than throwing a generic `RuntimeException`.

## 4.5 Throwing a custom exception from the service layer

```java
@Service
public class EmployeeService {

    private final EmployeeRepository employeeRepository;

    public EmployeeService(EmployeeRepository employeeRepository) {
        this.employeeRepository = employeeRepository;
    }

    public Employee getEmployee(Long employeeId) {

        return employeeRepository.findById(employeeId)
                .orElseThrow(() ->
                        new EmployeeNotFoundException(
                                "Employee not found with ID: " + employeeId
                        )
                );
    }
}
```

The service expresses the business outcome: if the requested employee does not exist, it throws a specific exception.

## 4.6 Mapping a custom exception to an HTTP status

A custom exception is a Java exception. It does not automatically mean HTTP 404 or 400.

In Spring MVC, an exception handler can translate it into an HTTP response:

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

The responsibilities are separated:

- Service: detects and throws the business exception.
- Global handler: maps the exception to an HTTP response.
- Client: receives the status and response body.

## 4.7 When should you create a custom exception?

Create one when it adds meaningful information to the code or API behavior.

Good reasons:

- A distinct business failure needs a distinct response.
- The exception communicates a domain concept.
- A handler needs to map the failure to a particular HTTP status.
- The original cause needs to be wrapped with application context.

Avoid creating dozens of exception classes that do not represent distinct conditions or behaviors.

---

# 5. Exception Propagation in Java

## 5.1 What is exception propagation?

Exception propagation is the process through which an exception moves from the method where it occurs to its calling method when the current method does not handle it.

The exception travels up the method call stack until a suitable handler is found.

## 5.2 Basic example

```java
public class ExceptionDemo {

    public static void methodC() {
        int result = 10 / 0;
    }

    public static void methodB() {
        methodC();
    }

    public static void methodA() {
        methodB();
    }

    public static void main(String[] args) {
        methodA();
    }
}
```

Conceptual call flow:

```text
main()
   |
   v
methodA()
   |
   v
methodB()
   |
   v
methodC()
   |
   v
ArithmeticException
```

What happens?

1. `methodC()` performs integer division by zero.
2. An `ArithmeticException` is created and thrown.
3. `methodC()` has no matching handler.
4. The exception propagates to `methodB()`.
5. `methodB()` has no matching handler, so it propagates to `methodA()`.
6. It continues to `main()`.
7. If no suitable handler is found, the exception is uncaught for that thread and the thread terminates.

## 5.3 Handling at a calling method

```java
public class ExceptionDemo {

    public static void methodC() {
        int result = 10 / 0;
    }

    public static void methodB() {
        methodC();
    }

    public static void main(String[] args) {

        try {
            methodB();
        } catch (ArithmeticException e) {
            System.out.println("Cannot divide by zero");
        }

        System.out.println("Application continues");
    }
}
```

Output:

```text
Cannot divide by zero
Application continues
```

The exception travels up the call stack until the `catch` block handles it. Execution then continues after the `try-catch` statement, provided the thread and process remain active.

## 5.4 Checked exception propagation

Checked exceptions must be caught or declared using `throws`.

```java
public class FileService {

    public void readFile() throws IOException {
        FileReader reader = new FileReader("data.txt");
        reader.close();
    }
}
```

The calling method must handle or declare the exception:

```java
public void processFile() throws IOException {
    readFile();
}
```

Or handle it:

```java
public void processFile() {
    try {
        readFile();
    } catch (IOException e) {
        System.out.println("Unable to read file");
    }
}
```

**Important:** Unchecked exceptions do not have to be declared using `throws`. They can also propagate through method calls.

## 5.5 Exception propagation in Spring Boot

Consider this simplified application flow:

```text
HTTP Request
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
Database
```

Suppose an employee does not exist.

The service may throw an exception:

```java
@Service
public class EmployeeService {

    private final EmployeeRepository repository;

    public EmployeeService(EmployeeRepository repository) {
        this.repository = repository;
    }

    public Employee getEmployee(Long id) {
        return repository.findById(id)
                .orElseThrow(() ->
                    new EmployeeNotFoundException(
                        "Employee not found: " + id
                    )
                );
    }
}
```

The exception leaves the service method and travels through the request-processing call stack. If no local handler handles it, Spring MVC's exception-resolution mechanism can find a matching global handler.

```java
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(EmployeeNotFoundException.class)
    public ResponseEntity<String> handleNotFound(
            EmployeeNotFoundException exception) {

        return ResponseEntity
                .status(HttpStatus.NOT_FOUND)
                .body(exception.getMessage());
    }
}
```

The client receives an HTTP `404 Not Found` response instead of an uncontrolled server error.

### Important distinction

Exception propagation is a Java call-stack concept. Spring's global exception handling is a framework mechanism that participates in resolving exceptions raised during request processing. The two work together, but they are not the same mechanism.

## 5.6 What happens if no handler is found?

If no suitable handler catches an exception:

- The exception is reported as uncaught for the current thread.
- The thread terminates.
- The JVM's uncaught-exception handling mechanism may print a stack trace or invoke a configured handler.
- Other application threads may continue running.

An uncaught exception in one request-handling thread does not necessarily shut down the entire Spring Boot application.

---

# 6. Exception Handling Best Practices

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
