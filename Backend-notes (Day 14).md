from pathlib import Path

content = r'''# Day 14 — Advanced Java and Backend Concepts: Exception Handling

> **Java Backend Developer Interview Preparation**  
> Status: Learning notes for the topics covered so far (Topics 1–5 of Day 14).  
> Focus: Exception fundamentals, checked vs unchecked exceptions, `try`/`catch`/`finally`, `throw`/`throws`, custom exceptions, and exception propagation.  
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

# 6. Important Interview Questions and Answers

## Q1. What is an exception?

An exception is an event during program execution that disrupts the normal flow of instructions. Java represents exceptions as objects that can be thrown and caught.

## Q2. What is the difference between Exception and Error?

Both extend `Throwable`. `Exception` represents conditions application code may be able to handle. `Error` represents serious problems that ordinary application code generally should not attempt to recover from.

## Q3. What is the difference between checked and unchecked exceptions?

Checked exceptions must be caught or declared. Unchecked exceptions, including `RuntimeException` subclasses and `Error` types, do not have that compiler requirement.

## Q4. Is NullPointerException checked or unchecked?

It is unchecked because it extends `RuntimeException`.

## Q5. Is IOException checked or unchecked?

`IOException` is checked. The compiler requires the code to catch it or declare it.

## Q6. What is the difference between throw and throws?

`throw` explicitly throws an exception object inside a method or block. `throws` declares possible exceptions in a method signature.

## Q7. What is the purpose of finally?

`finally` is commonly used for cleanup and normally executes after the try/catch flow, whether an exception occurs or not. It is not an absolute guarantee if the JVM or process terminates.

## Q8. Can we use try without catch?

Yes. A `try` statement can use `finally` without a `catch`, or use try-with-resources with an appropriate structure.

## Q9. What is try-with-resources?

It automatically closes resources that implement `AutoCloseable` when the try block exits, including when an exception occurs.

## Q10. What is a custom exception?

A custom exception is an application-defined exception class used to represent a meaningful application-specific failure.

## Q11. When should a custom exception extend RuntimeException?

When callers should not be forced by the compiler to catch or declare that business failure. This is common for many service-layer business exceptions in Spring Boot applications.

## Q12. What is exception propagation?

It is the movement of an unhandled exception up the method call stack until a suitable handler is found or the exception becomes uncaught for the thread.

## Q13. Can unchecked exceptions propagate without a throws declaration?

Yes. The compiler does not require unchecked exceptions to be declared or caught.

## Q14. Can checked exceptions propagate?

Yes, but each method that allows a checked exception to leave its body must catch it or declare it using `throws`.

## Q15. How does Spring Boot handle exceptions globally?

Spring MVC supports centralized exception resolution using `@ExceptionHandler` and `@RestControllerAdvice`. A handler can map a Java exception to an appropriate HTTP status and response body.

## Q16. Does an uncaught exception in one request thread always stop the whole Spring Boot application?

No. It normally terminates that thread's execution. Other server threads may continue handling requests. The application may still fail for other reasons, but one request exception does not automatically mean the entire process shuts down.

---

# 7. Quick Revision Sheet

| Topic | Remember |
|---|---|
| Exception | Event that disrupts normal program execution |
| `Throwable` | Parent type for objects that can be thrown |
| `Exception` | Conditions application code may handle |
| `Error` | Serious runtime/environment problem |
| Checked exception | Must be caught or declared |
| Unchecked exception | No compiler requirement to catch or declare |
| `RuntimeException` | Parent of common unchecked exceptions |
| `try` | Contains code that may fail |
| `catch` | Handles a matching exception |
| `finally` | Commonly used for cleanup |
| `throw` | Explicitly throws an exception object |
| `throws` | Declares possible exceptions in a method signature |
| Try-with-resources | Automatically closes `AutoCloseable` resources |
| Custom exception | Application-specific exception type |
| `super(message)` | Passes a message to the parent exception constructor |
| Exception cause | Preserves the lower-level failure |
| Propagation | Exception travels up the call stack |
| `@ExceptionHandler` | Handles selected exceptions in Spring MVC |
| `@RestControllerAdvice` | Centralized controller exception handling |

---

# 8. Day 14 Progress Checklist

This file documents the first five topics covered so far. Topics 6–8 are intentionally not included because they have not yet been completed in the learning sequence.

- [ ] I can explain exceptions and the `Throwable` hierarchy.
- [ ] I can distinguish `Exception` from `Error`.
- [ ] I can identify common runtime exceptions.
- [ ] I can distinguish checked and unchecked exceptions.
- [ ] I can create custom checked and unchecked exceptions.
- [ ] I understand `try`, `catch`, and `finally`.
- [ ] I can explain `throw` vs `throws`.
- [ ] I understand try-with-resources and `AutoCloseable`.
- [ ] I can preserve an original exception as the cause of a custom exception.
- [ ] I can explain exception propagation through a method call stack.
- [ ] I understand how a Spring MVC global exception handler can convert an exception into an HTTP response.

---

## Day 14 takeaway — Part 1

Exception handling is essential for building reliable Java applications. Understand the exception hierarchy, know when an exception is checked or unchecked, use cleanup mechanisms correctly, and create meaningful custom exceptions for business failures. In Spring Boot, exceptions can propagate from the repository or service layer and be translated into consistent API responses through centralized exception handling.

**Next learning topics in Day 14:**

6. Exception Handling Best Practices  
7. Exception Handling in Spring Boot  
8. Global Exception Handling and Production Scenarios
'''

path = Path("/mnt/data/Day_14_Exception_Handling_Part_1.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
