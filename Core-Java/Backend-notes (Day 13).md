from pathlib import Path

content = r'''# Day 13 — Advanced Java: Generics, Java 8 Features, Lambda Expressions, and Streams

> **Java Backend Developer Interview Preparation**  
> Focus: Generics, generic classes and methods, wildcards, bounded types, Java 8 features, functional interfaces, lambda expressions, Stream API, advanced collectors, Optional, and method references.  
> Examples connect to Spring Boot backend development.

---

## Learning objectives

By the end of this day, you should be able to:

- Explain why Java Generics provide compile-time type safety.
- Create generic classes and methods.
- Understand wildcards, bounded types, and the PECS rule.
- Describe the major Java 8 language and library features.
- Use lambda expressions with functional interfaces.
- Process collections using the Stream API.
- Use `map`, `filter`, `flatMap`, `reduce`, and collectors.
- Group and partition data using `Collectors`.
- Use `Optional` to represent a potentially absent return value.
- Explain and write method references.

---

# 1. Java Generics and Type Safety

## 1.1 What are Generics?

Generics allow classes, interfaces, and methods to work with different data types while preserving compile-time type safety.

Without generics, a collection may hold objects of different types:

```java
List values = new ArrayList();

values.add("Anand");
values.add(100);
values.add(true);

String name = (String) values.get(0);
```

Problems with this approach:

- The collection can contain unrelated types.
- Developers must cast retrieved values.
- An incorrect cast can fail at runtime with `ClassCastException`.
- The compiler cannot enforce the intended element type.

With generics:

```java
List<String> names = new ArrayList<>();

names.add("Anand");
names.add("Rahul");

// Compile-time error:
// names.add(100);

String name = names.get(0);
```

Now the compiler ensures that the list contains `String` values, and retrieving a value does not require an explicit cast.

## 1.2 Why are Generics useful?

1. **Type safety:** Invalid types are rejected at compile time.
2. **Less casting:** Retrieved values already have the declared type.
3. **Reusable code:** A generic class or method can support multiple types.
4. **Clearer APIs:** The intended input and output types are visible.
5. **Better maintainability:** Many type errors are detected before the application runs.

## 1.3 Generic type syntax

The letter `T` is a conventional type parameter name. It is not a special Java keyword.

Common conventions:

| Letter | Common meaning |
|---|---|
| `T` | Type |
| `E` | Element |
| `K` | Key |
| `V` | Value |
| `N` | Number |
| `R` | Result |

Example:

```java
List<String> names = new ArrayList<>();
Map<Long, String> employeeNames = new HashMap<>();
Set<Integer> employeeIds = new HashSet<>();
```

The type parameters describe the types that the collection is allowed to contain.

## 1.4 Generics and compile-time safety

```java
List<Integer> numbers = new ArrayList<>();

numbers.add(10);
numbers.add(20);

// Does not compile:
// numbers.add("thirty");

Integer first = numbers.get(0);
```

The compiler checks the types. Generics do not make Java dynamically typed; they make APIs more precise while keeping Java's static type system.

---

# 2. Generic Classes and Generic Methods

## 2.1 Generic class

A generic class declares one or more type parameters.

```java
public class Box<T> {

    private T value;

    public void setValue(T value) {
        this.value = value;
    }

    public T getValue() {
        return value;
    }
}
```

Use the class with different types:

```java
Box<String> stringBox = new Box<>();
stringBox.setValue("Hello");

String text = stringBox.getValue();

Box<Integer> integerBox = new Box<>();
integerBox.setValue(100);

Integer number = integerBox.getValue();
```

The same class works with different reference types while preserving type safety.

## 2.2 Generic class with two type parameters

```java
public class Pair<K, V> {

    private final K key;
    private final V value;

    public Pair(K key, V value) {
        this.key = key;
        this.value = value;
    }

    public K getKey() {
        return key;
    }

    public V getValue() {
        return value;
    }
}
```

Usage:

```java
Pair<Long, String> employee =
        new Pair<>(101L, "Anand");

Long employeeId = employee.getKey();
String employeeName = employee.getValue();
```

Here:
- `K` represents the key type.
- `V` represents the value type.

## 2.3 Generic methods

A generic method declares its own type parameter before the return type.

```java
public class Utility {

    public static <T> void printValue(T value) {
        System.out.println(value);
    }
}
```

Usage:

```java
Utility.printValue("Hello");
Utility.printValue(100);
Utility.printValue(true);
```

The compiler infers the type argument from the method call in these examples.

The method-level type parameter is independent of whether the surrounding class is generic.

## 2.4 Generic API response wrapper

A common backend example is a reusable API response type.

```java
public class ApiResponse<T> {

    private boolean success;
    private String message;
    private T data;

    public ApiResponse(boolean success, String message, T data) {
        this.success = success;
        this.message = message;
        this.data = data;
    }

    public boolean isSuccess() {
        return success;
    }

    public String getMessage() {
        return message;
    }

    public T getData() {
        return data;
    }
}
```

The same response class can carry different data:

```java
ApiResponse<EmployeeDto> employeeResponse;
ApiResponse<List<EmployeeDto>> employeeListResponse;
ApiResponse<DriverDto> driverResponse;
```

This is useful when an API uses a consistent response envelope.

## 2.5 Generic repository concept

Spring Data repositories use generics:

```java
public interface EmployeeRepository
        extends JpaRepository<Employee, Long> {
}
```

The type parameters tell Spring Data JPA:

- `Employee`: the entity type managed by this repository.
- `Long`: the type of the entity's identifier.

This is one practical example of generics in everyday Spring Boot development.

---

# 3. Wildcards and Bounded Generics

## 3.1 What is a wildcard?

The wildcard `?` means an unknown type.

```java
List<?> values;
```

This means the list has some specific element type, but the code using this reference does not know which type it is.

You can safely read elements as `Object`:

```java
public static void printAll(List<?> values) {
    for (Object value : values) {
        System.out.println(value);
    }
}
```

This method can accept a `List<String>`, `List<Integer>`, or another list.

You generally cannot add an arbitrary non-NULL value to `List<?>`, because the actual element type is unknown.

## 3.2 Upper-bounded wildcard: `? extends`

```java
List<? extends Number>
```

This represents a list whose element type is `Number` or a subtype of `Number`, such as `Integer`, `Double`, or `Long`.

Example:

```java
public static double calculateTotal(
        List<? extends Number> numbers) {

    double total = 0;

    for (Number number : numbers) {
        total += number.doubleValue();
    }

    return total;
}
```

Usage:

```java
List<Integer> integers = List.of(10, 20, 30);
List<Double> decimals = List.of(1.5, 2.5);

System.out.println(calculateTotal(integers));
System.out.println(calculateTotal(decimals));
```

Why is adding restricted?

A `List<? extends Number>` might actually be a `List<Integer>`. Adding a `Double` would violate the actual list's type. Therefore, the compiler prevents adding arbitrary values.

## 3.3 Lower-bounded wildcard: `? super`

```java
List<? super Integer>
```

This represents a list whose element type is `Integer` or a supertype of `Integer`, such as `Number` or `Object`.

```java
public static void addNumbers(List<? super Integer> values) {
    values.add(10);
    values.add(20);
}
```

The method can safely add `Integer` values because every permitted destination type can hold an Integer.

When reading from such a list, the safe general type is `Object`, because the actual list might be a `List<Object>`.

## 3.4 PECS rule

A useful rule for choosing wildcard bounds is:

**PECS = Producer Extends, Consumer Super.**

- If a structure produces values for your code to read, prefer `? extends T`.
- If a structure consumes values that your code supplies, prefer `? super T`.

Example: a method reads numbers, so it uses `extends`:

```java
static double sum(List<? extends Number> values) {
    double result = 0;

    for (Number value : values) {
        result += value.doubleValue();
    }

    return result;
}
```

A method adds integers, so it uses `super`:

```java
static void addDefaults(List<? super Integer> values) {
    values.add(1);
    values.add(2);
}
```

## 3.5 Bounded type parameters

A generic type parameter can be restricted using `extends`.

```java
public class NumericBox<T extends Number> {

    private final T value;

    public NumericBox(T value) {
        this.value = value;
    }

    public double asDouble() {
        return value.doubleValue();
    }
}
```

Valid:

```java
NumericBox<Integer> integerBox = new NumericBox<>(10);
NumericBox<Double> doubleBox = new NumericBox<>(10.5);
```

Invalid:

```java
// NumericBox<String> textBox = new NumericBox<>("Hello");
```

The type must be `Number` or a subtype.

## 3.6 Multiple bounds

A type parameter can have multiple bounds.

```java
<T extends Number & Comparable<T>>
```

The class bound, if present, must come first, followed by interface bounds.

This means the type must extend `Number` and implement `Comparable<T>`.

---

# 4. Java 8 Features Overview

Java 8 introduced major language and library features that are heavily used in modern Java backend development.

Important features include:

1. Lambda expressions.
2. Functional interfaces.
3. Stream API.
4. Method references.
5. `Optional`.
6. Default and static methods in interfaces.
7. New Date and Time API (`java.time`).

## 4.1 Lambda expressions

Lambdas allow a concise way to provide behavior where a functional interface is expected.

Before lambda:

```java
Runnable task = new Runnable() {
    @Override
    public void run() {
        System.out.println("Task running");
    }
};
```

With lambda:

```java
Runnable task = () -> System.out.println("Task running");
```

## 4.2 Functional interfaces

A functional interface has exactly one abstract method.

Examples:
- `Runnable`
- `Callable`
- `Comparator`
- `Predicate`
- `Function`
- `Consumer`
- `Supplier`

A functional interface can also have default and static methods. The key requirement is one abstract method.

## 4.3 Stream API

The Stream API processes sequences of data using operations such as `filter`, `map`, `sorted`, and `collect`.

```java
List<String> names = List.of("Anand", "Asha", "Ravi");

List<String> result = names.stream()
        .filter(name -> name.startsWith("A"))
        .toList();
```

## 4.4 Method references

Method references provide a shorter way to refer to an existing method when its signature matches the expected functional interface.

```java
names.forEach(System.out::println);
```

## 4.5 Optional

`Optional<T>` represents a value that may or may not be present.

```java
Optional<Employee> employee =
        employeeRepository.findById(101L);
```

It encourages explicit handling of an absent result instead of accidentally dereferencing `null`.

## 4.6 Default and static interface methods

An interface can define a default method with an implementation:

```java
public interface NotificationService {

    void send(String message);

    default void logMessage(String message) {
        System.out.println("Sending: " + message);
    }

    static boolean isValid(String message) {
        return message != null && !message.isBlank();
    }
}
```

A default method is inherited by implementing classes unless overridden. A static interface method is called using the interface name.

## 4.7 Java Date and Time API

Java 8 introduced the modern `java.time` API.

Important types:

| Type | Purpose |
|---|---|
| `LocalDate` | Date without time or timezone |
| `LocalTime` | Time without date or timezone |
| `LocalDateTime` | Date and time without timezone |
| `Instant` | Point on the UTC timeline |
| `ZonedDateTime` | Date and time with a timezone |
| `Duration` | Time-based amount |
| `Period` | Date-based amount |

Example:

```java
LocalDate tripDate = LocalDate.now();
LocalDate tomorrow = tripDate.plusDays(1);
```

For transport scheduling, choosing the correct timezone-aware type matters. A local shift time and a globally unambiguous event timestamp are not necessarily the same thing.

---

# 5. Lambda Expressions and Functional Interfaces

## 5.1 What is a lambda expression?

A lambda expression is a concise way to represent behavior that can be passed as an argument or stored in a functional-interface reference.

General syntax:

```java
(parameters) -> expression
```

Or:

```java
(parameters) -> {
    // multiple statements
}
```

Examples:

```java
() -> System.out.println("Hello");

name -> System.out.println(name);

(a, b) -> a + b;

(a, b) -> {
    int result = a + b;
    return result;
};
```

## 5.2 @FunctionalInterface

```java
@FunctionalInterface
public interface Calculator {
    int calculate(int a, int b);
}
```

Use a lambda:

```java
Calculator addition = (a, b) -> a + b;
Calculator multiplication = (a, b) -> a * b;

System.out.println(addition.calculate(10, 5));
System.out.println(multiplication.calculate(10, 5));
```

`@FunctionalInterface` is optional, but it asks the compiler to verify the functional-interface rule.

## 5.3 Predicate

`Predicate<T>` accepts a value and returns a boolean.

Its abstract method is:

```java
boolean test(T value);
```

Example:

```java
Predicate<Integer> isEven = number -> number % 2 == 0;

System.out.println(isEven.test(10)); // true
System.out.println(isEven.test(7));  // false
```

Backend example:

```java
Predicate<Employee> isActive =
        employee -> employee.isActive();
```

Useful for conditions and Stream filtering.

## 5.4 Function

`Function<T, R>` accepts a value of type `T` and returns a value of type `R`.

Its abstract method is:

```java
R apply(T value);
```

Example:

```java
Function<String, Integer> length =
        text -> text.length();

System.out.println(length.apply("Anand")); // 5
```

Backend example:

```java
Function<Employee, EmployeeDto> toDto =
        employee -> new EmployeeDto(
                employee.getId(),
                employee.getName()
        );
```

## 5.5 Consumer

`Consumer<T>` accepts a value and returns no result.

Its abstract method is:

```java
void accept(T value);
```

Example:

```java
Consumer<String> printer =
        value -> System.out.println(value);

printer.accept("Hello");
```

It is useful for actions such as logging or processing each item.

## 5.6 Supplier

`Supplier<T>` produces a value without accepting an input.

Its abstract method is:

```java
T get();
```

Example:

```java
Supplier<LocalDate> today =
        () -> LocalDate.now();

System.out.println(today.get());
```

## 5.7 Comparison

| Interface | Input | Output | Main method |
|---|---|---|---|
| `Predicate<T>` | T | boolean | `test()` |
| `Function<T, R>` | T | R | `apply()` |
| `Consumer<T>` | T | void | `accept()` |
| `Supplier<T>` | none | T | `get()` |

## 5.8 Combining predicates

Predicates can be combined using `and`, `or`, and `negate`.

```java
Predicate<Employee> active =
        Employee::isActive;

Predicate<Employee> inEngineering =
        employee -> employee.getDepartmentId() == 10L;

Predicate<Employee> activeEngineering =
        active.and(inEngineering);
```

Then:

```java
boolean matches = activeEngineering.test(employee);
```

This is useful for composing reusable conditions.

---

# 6. Stream API Fundamentals

## 6.1 What is a Stream?

A Stream is a sequence of elements that supports declarative data-processing operations.

A Stream is not a collection and does not normally store the data itself. It processes elements from a source such as a collection, array, or generated sequence.

Example:

```java
List<String> names =
        List.of("Anand", "Asha", "Ravi", "Aman");

List<String> result = names.stream()
        .filter(name -> name.startsWith("A"))
        .toList();
```

Result:

```text
[Anand, Asha, Aman]
```

## 6.2 Stream pipeline

A typical pipeline has three parts:

```text
Source
  |
  v
Intermediate operations
  |
  v
Terminal operation
  |
  v
Result / side effect
```

Example:

```java
List<Integer> result = numbers.stream()
        .filter(number -> number > 10)
        .map(number -> number * 2)
        .toList();
```

- Source: `numbers`
- Intermediate operations: `filter`, `map`
- Terminal operation: `toList`

## 6.3 Creating streams

From a collection:

```java
List<String> names = List.of("Asha", "Ravi");
Stream<String> stream = names.stream();
```

From an array:

```java
String[] namesArray = {"Asha", "Ravi"};

Stream<String> stream =
        Arrays.stream(namesArray);
```

Using `Stream.of`:

```java
Stream<String> stream =
        Stream.of("Asha", "Ravi", "Neha");
```

## 6.4 filter()

`filter()` retains elements that satisfy a predicate.

```java
List<Integer> evenNumbers = numbers.stream()
        .filter(number -> number % 2 == 0)
        .toList();
```

Backend example:

```java
List<Employee> activeEmployees = employees.stream()
        .filter(Employee::isActive)
        .toList();
```

## 6.5 map()

`map()` transforms each element into another value.

```java
List<String> names = employees.stream()
        .map(Employee::getName)
        .toList();
```

The number of output elements is normally the same as the number of input elements, unless the mapping itself produces nulls or the pipeline has other operations that change the count.

## 6.6 sorted()

Sorts stream elements.

```java
List<String> sortedNames = names.stream()
        .sorted()
        .toList();
```

Custom sorting:

```java
List<Employee> sortedEmployees = employees.stream()
        .sorted(Comparator.comparing(Employee::getName))
        .toList();
```

Descending salary:

```java
List<Employee> bySalary = employees.stream()
        .sorted(Comparator.comparing(Employee::getSalary).reversed())
        .toList();
```

## 6.7 distinct()

Removes duplicate elements according to equality semantics.

```java
List<Integer> uniqueNumbers = numbers.stream()
        .distinct()
        .toList();
```

For custom objects, `distinct()` depends on correct `equals()` and `hashCode()` implementations.

## 6.8 limit() and skip()

`limit(n)` keeps at most the first `n` elements in encounter order.

`skip(n)` skips the first `n` elements in encounter order.

```java
List<Integer> firstFive = numbers.stream()
        .limit(5)
        .toList();

List<Integer> afterFirstFive = numbers.stream()
        .skip(5)
        .toList();
```

These operations can help with in-memory processing, but database pagination should usually be done in the database for large datasets.

## 6.9 Terminal operations

Common terminal operations:

```java
long count = names.stream().count();

boolean anyMatch = names.stream()
        .anyMatch(name -> name.startsWith("A"));

boolean allMatch = names.stream()
        .allMatch(name -> !name.isBlank());

boolean noneMatch = names.stream()
        .noneMatch(String::isBlank);

Optional<String> first = names.stream().findFirst();
```

Other terminal operations include `forEach`, `toList`, `collect`, and `reduce`.

## 6.10 Stream laziness

Intermediate operations are lazy. They do not process the elements until a terminal operation is invoked.

```java
Stream<String> stream = names.stream()
        .filter(name -> {
            System.out.println("Checking " + name);
            return name.startsWith("A");
        });
```

At this point, the filter has not necessarily processed the elements. A terminal operation triggers evaluation:

```java
List<String> result = stream.toList();
```

## 6.11 Streams cannot normally be reused

A Stream is intended for one pipeline and one terminal operation.

Incorrect:

```java
Stream<String> stream = names.stream();

long count = stream.count();

// IllegalStateException: stream has already been operated upon or closed
// List<String> result = stream.toList();
```

Create a new Stream from the source when another traversal is required:

```java
long count = names.stream().count();

List<String> result = names.stream()
        .filter(name -> name.startsWith("A"))
        .toList();
```

## 6.12 Stream.toList() vs Collectors.toList()

In modern Java:

```java
List<String> result = names.stream().toList();
```

`Stream.toList()` returns an unmodifiable list.

By contrast:

```java
List<String> result = names.stream()
        .collect(Collectors.toList());
```

The API does not guarantee a particular mutability or concrete implementation for the returned list.

If a mutable `ArrayList` is specifically needed:

```java
List<String> result = names.stream()
        .collect(Collectors.toCollection(ArrayList::new));
```

---

# 7. Advanced Stream API

## 7.1 flatMap()

`map()` transforms each element into one result. `flatMap()` transforms each element into a stream and flattens the resulting streams into one stream.

Example:

```java
List<List<String>> groups = List.of(
        List.of("Asha", "Ravi"),
        List.of("Neha", "Kabir")
);

List<String> allNames = groups.stream()
        .flatMap(List::stream)
        .toList();
```

Result:

```text
[Asha, Ravi, Neha, Kabir]
```

Backend example: flatten employees from multiple departments.

```java
List<Employee> allEmployees = departments.stream()
        .flatMap(department -> department.getEmployees().stream())
        .toList();
```

Be careful with lazy-loaded JPA relationships. Accessing them inside a Stream may trigger additional database queries or fail outside an active persistence context.

## 7.2 reduce()

`reduce()` combines stream elements into a single result.

Sum example:

```java
int total = numbers.stream()
        .reduce(0, Integer::sum);
```

Maximum example:

```java
Optional<Integer> maximum = numbers.stream()
        .reduce(Integer::max);
```

The identity value is the starting value. For parallel processing, the reduction operation should be associative and compatible with the identity.

For ordinary numeric totals, `mapToInt(...).sum()` or an aggregate collector may be clearer.

## 7.3 Collectors.groupingBy()

Groups elements according to a classification function.

```java
Map<Long, List<Employee>> employeesByDepartment =
        employees.stream()
                .collect(Collectors.groupingBy(
                        Employee::getDepartmentId
                ));
```

The result is a map where:
- Key: department ID.
- Value: employees in that department.

## 7.4 groupingBy() with counting()

Count employees per department:

```java
Map<Long, Long> employeeCountByDepartment =
        employees.stream()
                .collect(Collectors.groupingBy(
                        Employee::getDepartmentId,
                        Collectors.counting()
                ));
```

## 7.5 groupingBy() with averagingDouble()

Calculate average salary per department:

```java
Map<Long, Double> averageSalaryByDepartment =
        employees.stream()
                .collect(Collectors.groupingBy(
                        Employee::getDepartmentId,
                        Collectors.averagingDouble(Employee::getSalary)
                ));
```

## 7.6 partitioningBy()

`partitioningBy()` divides elements into exactly two groups based on a predicate: `true` and `false`.

```java
Map<Boolean, List<Employee>> partition =
        employees.stream()
                .collect(Collectors.partitioningBy(
                        Employee::isActive
                ));
```

The map has a `true` group and a `false` group, even if one group is empty.

## 7.7 Collectors.counting()

Count the number of elements:

```java
long count = employees.stream()
        .collect(Collectors.counting());
```

This is equivalent in purpose to `stream.count()` for a simple count, though `count()` is more direct.

## 7.8 Collectors.joining()

Join strings with a delimiter:

```java
String employeeNames = employees.stream()
        .map(Employee::getName)
        .collect(Collectors.joining(", "));
```

You can also specify prefix and suffix:

```java
String employeeNames = employees.stream()
        .map(Employee::getName)
        .collect(Collectors.joining(", ", "[", "]"));
```

## 7.9 Collectors.toMap()

Convert stream elements into a map:

```java
Map<Long, String> employeeMap = employees.stream()
        .collect(Collectors.toMap(
                Employee::getId,
                Employee::getName
        ));
```

If two elements produce the same key, this form throws an `IllegalStateException`.

Provide a merge function when duplicate keys are possible:

```java
Map<Long, String> employeeMap = employees.stream()
        .collect(Collectors.toMap(
                Employee::getDepartmentId,
                Employee::getName,
                (first, second) -> first
        ));
```

This keeps the first encountered value for a duplicate department key. Choose a merge rule that makes business sense.

## 7.10 Collectors.summarizingDouble()

Get count, sum, minimum, average, and maximum for a numeric property:

```java
DoubleSummaryStatistics statistics = employees.stream()
        .collect(Collectors.summarizingDouble(Employee::getSalary));

System.out.println(statistics.getCount());
System.out.println(statistics.getSum());
System.out.println(statistics.getAverage());
System.out.println(statistics.getMin());
System.out.println(statistics.getMax());
```

## 7.11 Database aggregation vs Java Streams

Streams are useful for processing data already available in Java memory. They are not a replacement for database queries.

For example, avoid loading every trip just to count them:

```java
List<Trip> allTrips = tripRepository.findAll();

long completedCount = allTrips.stream()
        .filter(trip -> trip.getStatus() == TripStatus.COMPLETED)
        .count();
```

For a large table, prefer a database query:

```sql
SELECT COUNT(*)
FROM trips
WHERE status = 'COMPLETED';
```

This reduces data transfer and application memory use.

---

# 8. Optional and Method References

## 8.1 What is Optional?

`Optional<T>` is a container that may contain a non-NULL value or be empty.

It is often used as a return type when a result may be absent.

```java
Optional<Employee> employee =
        employeeRepository.findById(101L);
```

Instead of returning a possibly absent Employee directly, the API makes the absence explicit.

## 8.2 isPresent()

Checks whether a value exists.

```java
if (employee.isPresent()) {
    System.out.println(employee.get().getName());
}
```

`get()` throws `NoSuchElementException` if the Optional is empty. Avoid calling it without checking.

## 8.3 ifPresent()

Runs an action only if a value exists.

```java
employee.ifPresent(value ->
        System.out.println(value.getName())
);
```

## 8.4 orElse()

Returns the contained value or a fallback value.

```java
String name = employeeNameOptional.orElse("Unknown");
```

The fallback expression is evaluated before the call, even when the Optional contains a value.

## 8.5 orElseGet()

Uses a Supplier to produce a fallback only when the Optional is empty.

```java
String name = employeeNameOptional.orElseGet(
        () -> loadDefaultEmployeeName()
);
```

Use `orElseGet()` when creating the fallback is expensive or has side effects that should happen only when needed.

## 8.6 orElseThrow()

Throws an exception when the Optional is empty.

```java
Employee employee = employeeRepository.findById(id)
        .orElseThrow(() ->
                new EmployeeNotFoundException("Employee not found")
        );
```

This is a common Spring Boot service-layer pattern.

## 8.7 map()

Transforms the value inside an Optional if present.

```java
Optional<String> employeeName =
        employeeRepository.findById(id)
                .map(Employee::getName);
```

If the Optional is empty, the mapped result is also empty.

## 8.8 filter()

Keeps the value only if it matches a predicate.

```java
Optional<Employee> activeEmployee =
        employeeRepository.findById(id)
                .filter(Employee::isActive);
```

If the employee is absent or inactive, the result is empty.

## 8.9 Optional best practices

- Use Optional mainly as a return type when absence is a meaningful outcome.
- Do not call `get()` blindly.
- Prefer `orElseThrow()` when absence is an error.
- Use `orElseGet()` for lazy fallback creation.
- Avoid using Optional as a replacement for every nullable field or method parameter.
- Do not return `null` instead of an Optional; return `Optional.empty()` when no value exists.
- Avoid deeply nested Optional chains that make business logic harder to read.

## 8.10 What is a method reference?

A method reference is a shorter way to refer to an existing method when a functional interface is expected.

Syntax examples:

```java
ClassName::staticMethod
objectName::instanceMethod
ClassName::instanceMethod
ClassName::new
```

## 8.11 Static method reference

```java
Function<String, Integer> parser =
        Integer::parseInt;
```

Equivalent lambda:

```java
Function<String, Integer> parser =
        text -> Integer.parseInt(text);
```

## 8.12 Bound instance method reference

```java
Consumer<String> printer =
        System.out::println;
```

Equivalent lambda:

```java
Consumer<String> printer =
        value -> System.out.println(value);
```

## 8.13 Unbound instance method reference

```java
Function<String, String> uppercase =
        String::toUpperCase;
```

Equivalent lambda:

```java
Function<String, String> uppercase =
        text -> text.toUpperCase();
```

The first input supplied to the functional interface becomes the receiver of the instance method.

## 8.14 Constructor reference

```java
Supplier<ArrayList<String>> listSupplier =
        ArrayList::new;
```

Equivalent lambda:

```java
Supplier<ArrayList<String>> listSupplier =
        () -> new ArrayList<>();
```

## 8.15 Four method-reference forms

| Type | Example |
|---|---|
| Static method | `Integer::parseInt` |
| Bound instance method | `System.out::println` |
| Unbound instance method | `String::toUpperCase` |
| Constructor | `ArrayList::new` |

---

# 9. Important Interview Questions and Answers

## Q1. What are Generics in Java?

Generics allow classes, interfaces, and methods to operate on different types while providing compile-time type safety and reducing explicit casting.

## Q2. Why are Generics useful?

They prevent many invalid type assignments at compile time, reduce casting, improve API clarity, and support reusable code.

## Q3. What is a generic class?

A class that declares type parameters, such as `Box<T>`, so its fields and methods can work with a specified type.

## Q4. What is a generic method?

A method that declares its own type parameter, such as `<T> void print(T value)`. The type parameter is declared before the return type.

## Q5. What does `<?>` mean?

It represents an unknown type. It allows safe reading as `Object`, but does not allow adding arbitrary non-NULL values.

## Q6. What is the difference between `? extends T` and `? super T`?

`? extends T` is useful when reading values as T from a producer. `? super T` is useful when adding T values to a consumer.

## Q7. Explain PECS.

PECS means Producer Extends, Consumer Super. Use `extends` when a source produces values for your code to read, and `super` when a destination consumes values your code supplies.

## Q8. What is a functional interface?

An interface with exactly one abstract method. It can also contain default and static methods. It can be implemented using a lambda expression or method reference.

## Q9. What is a lambda expression?

A concise expression that provides an implementation of a functional interface's abstract method.

## Q10. What is Predicate?

`Predicate<T>` accepts a value and returns a boolean through `test()`.

## Q11. What is Function?

`Function<T, R>` accepts a T and returns an R through `apply()`.

## Q12. What is Consumer?

`Consumer<T>` accepts a T and returns no result through `accept()`.

## Q13. What is Supplier?

`Supplier<T>` takes no input and supplies a T through `get()`.

## Q14. What is the Stream API?

The Stream API provides declarative operations for processing sequences of data, including filtering, mapping, sorting, and aggregation.

## Q15. What is the difference between map and flatMap?

`map` transforms each element into one result. `flatMap` transforms each element into a stream and flattens the resulting streams into one stream.

## Q16. What is lazy evaluation in Streams?

Intermediate operations do not process elements immediately. Processing starts when a terminal operation is invoked.

## Q17. Can a Stream be reused?

No. A Stream is normally consumed by one terminal operation. Create a new Stream from the source for another traversal.

## Q18. What is reduce?

`reduce` combines stream elements into a single result using an accumulator operation.

## Q19. groupingBy vs partitioningBy?

`groupingBy` creates groups based on a classifier and can produce many groups. `partitioningBy` splits elements into two groups based on a boolean predicate.

## Q20. What happens if Collectors.toMap encounters duplicate keys?

The two-argument `toMap` collector throws `IllegalStateException`. Supply a merge function if duplicates are possible.

## Q21. What is Optional?

`Optional<T>` represents a value that may or may not be present. It is commonly used as a return type to make absence explicit.

## Q22. orElse vs orElseGet?

`orElse` evaluates its fallback argument eagerly. `orElseGet` calls a Supplier only if the Optional is empty.

## Q23. What is a method reference?

A method reference is a compact way to refer to an existing method or constructor when its signature matches the expected functional interface.

## Q24. Why should large database filtering be done in SQL rather than Java Streams?

The database can filter and aggregate records close to where they are stored. This avoids transferring large numbers of unnecessary rows into application memory.

---

# 10. Quick Revision Sheet

| Topic | Remember |
|---|---|
| Generics | Reusable code with compile-time type safety |
| `T` | Conventional generic type parameter |
| `Box<T>` | Generic class |
| `<T> method(...)` | Generic method |
| `<?>` | Unknown type |
| `? extends T` | Upper-bounded wildcard; producer |
| `? super T` | Lower-bounded wildcard; consumer |
| PECS | Producer Extends, Consumer Super |
| Functional interface | Exactly one abstract method |
| Lambda | Concise functional-interface implementation |
| Predicate | T → boolean |
| Function | T → R |
| Consumer | T → void |
| Supplier | () → T |
| Stream | Pipeline for processing a data sequence |
| `filter()` | Select matching elements |
| `map()` | Transform each element |
| `flatMap()` | Transform and flatten nested streams |
| `sorted()` | Sort elements |
| `distinct()` | Remove duplicates according to equality |
| `reduce()` | Combine elements into one result |
| `groupingBy()` | Group elements into a Map |
| `partitioningBy()` | Split into true and false groups |
| `Optional` | Value may be present or absent |
| `orElse()` | Eager fallback |
| `orElseGet()` | Lazy Supplier fallback |
| `orElseThrow()` | Throw if empty |
| Method reference | Refer to existing method or constructor |

---

# 11. Day 13 Completion Checklist

This is a learning coverage checklist, not a quiz.

- [ ] I can explain Generics and compile-time type safety.
- [ ] I can write generic classes and generic methods.
- [ ] I understand `<?>`, `? extends`, and `? super`.
- [ ] I can explain the PECS rule.
- [ ] I know the major Java 8 features.
- [ ] I understand functional interfaces and lambda syntax.
- [ ] I can use Predicate, Function, Consumer, and Supplier.
- [ ] I understand Stream sources, intermediate operations, and terminal operations.
- [ ] I can use `filter`, `map`, `sorted`, `distinct`, and `toList`.
- [ ] I understand Stream laziness and single-use behavior.
- [ ] I can use `flatMap`, `reduce`, and common collectors.
- [ ] I understand `groupingBy`, `partitioningBy`, `joining`, and `toMap`.
- [ ] I can explain Optional and its common methods.
- [ ] I can distinguish `orElse` from `orElseGet`.
- [ ] I understand all four common method-reference forms.
- [ ] I know when SQL is more suitable than Java Stream processing.

---

## Day 13 takeaway

Generics make Java APIs reusable and type-safe. Lambdas and functional interfaces let you pass behavior as data. Streams make collection processing more declarative, while Optional makes potentially absent values explicit. These features appear throughout modern Java libraries and Spring Boot code, so understanding their behavior is important for backend interviews and day-to-day development.
'''

path = Path("/mnt/data/Day_13_Advanced_Java_Generics_Streams.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
