from pathlib import Path

content = r'''# Day 7 — Core Java Fundamentals

> **Purpose:** Build a strong foundation in Java language and JVM concepts for Java Backend Developer interviews.
>
> **Learning context:** These notes follow the eight topics covered in Day 7. Examples use modern Java syntax and are compatible with Java 21 unless otherwise stated.

---

## Table of Contents

1. [JDK vs JRE vs JVM](#1-jdk-vs-jre-vs-jvm)
2. [Java Execution Process and JVM Internals](#2-java-execution-process-and-jvm-internals)
3. [Object-Oriented Programming Concepts](#3-object-oriented-programming-concepts)
4. [Classes, Objects, and Constructors](#4-classes-objects-and-constructors)
5. [Inheritance, Polymorphism, and Abstraction in Depth](#5-inheritance-polymorphism-and-abstraction-in-depth)
6. [Encapsulation, Interfaces, and Abstract Classes](#6-encapsulation-interfaces-and-abstract-classes)
7. [`==` vs `.equals()` and `hashCode()`](#7--vs-equals-and-hashcode)
8. [String, StringBuilder, and StringBuffer](#8-string-stringbuilder-and-stringbuffer)
9. [Day 7 — Quick Revision Sheet](#9-day-7--quick-revision-sheet)
10. [Interview Questions and Answers](#10-interview-questions-and-answers)
11. [Practical Backend Connections](#11-practical-backend-connections)
12. [Completion Checklist](#12-completion-checklist)

---

# 1. JDK vs JRE vs JVM

## 1.1 What is Java?

Java is a high-level, class-based, object-oriented programming language. A Java program is generally compiled into **bytecode**, which can run on a compatible Java Virtual Machine.

Java's portability is commonly summarized as:

> Write once, run anywhere — provided a compatible JVM and required runtime libraries are available.

Java source code is not normally compiled directly into machine code for one specific processor. The Java compiler produces bytecode, and the JVM executes that bytecode on the target platform.

## 1.2 JVM — Java Virtual Machine

The JVM is the runtime engine that executes Java bytecode.

Main responsibilities:

- Loads compiled `.class` files.
- Verifies bytecode before execution.
- Manages runtime memory areas.
- Executes bytecode using an interpreter and JIT compiler.
- Performs garbage collection.
- Provides runtime services such as thread management and exception handling.

Example:

```java
public class Main {
    public static void main(String[] args) {
        System.out.println("Hello Java");
    }
}
```

After compilation, `Main.class` contains bytecode. The JVM loads and executes that bytecode.

**Important:** JVM is a specification. Different vendors provide JVM implementations, such as HotSpot and OpenJ9.

## 1.3 JRE — Java Runtime Environment

The JRE is the conceptual environment required to run Java applications.

It includes:

- JVM.
- Core Java runtime libraries.
- Supporting runtime files.

A traditional simplified view is:

```text
JRE
 ├── JVM
 ├── Java runtime libraries
 └── Supporting runtime files
```

The JRE is intended for running Java applications, not developing them.

**Modern Java note:** Since Java 9, the JDK distribution and modular runtime model changed. Many modern JDK distributions do not provide a separate, standalone JRE installation in the old way. A custom runtime image can be created with tools such as `jlink`.

## 1.4 JDK — Java Development Kit

The JDK is the development kit used to build, compile, debug, and run Java applications.

It includes:

- Java compiler (`javac`).
- Java launcher (`java`).
- JVM and runtime components.
- Development and diagnostic tools such as `javadoc`, `jdb`, `jcmd`, `jstack`, and `jmap` (availability can vary by distribution).

Conceptual relationship:

```text
JDK
 ├── Development tools
 │    ├── javac
 │    ├── javadoc
 │    ├── jdb
 │    └── diagnostic tools
 └── Runtime components
      ├── JVM
      └── Java libraries
```

## 1.5 JDK vs JRE vs JVM

| Feature | JDK | JRE (conceptual) | JVM |
|---|---|---|---|
| Main purpose | Develop and run Java apps | Run Java apps | Execute bytecode |
| Compiler | Yes | No | No |
| Runtime libraries | Yes | Yes | Uses them |
| Bytecode execution | Through its runtime | Through its JVM | Yes |
| Development tools | Yes | Usually no | No |

### Interview-ready answer

**Q: What is the difference between JDK, JRE, and JVM?**

The JDK is used to develop Java applications and includes the compiler and development tools along with runtime components. The JRE is the conceptual runtime environment containing the JVM and libraries needed to run Java applications. The JVM loads, verifies, and executes Java bytecode. In modern Java distributions, a separate JRE installation is not always provided.

### Common confusion

- `javac` compiles `.java` source files into `.class` bytecode.
- `java` launches a Java application using a JVM.
- The JVM is not the Java compiler.
- Bytecode is platform-independent at the class-file level, but a compatible JVM and runtime environment are still required.

---

# 2. Java Execution Process and JVM Internals

## 2.1 From source code to execution

Consider:

```java
public class EmployeeApp {
    public static void main(String[] args) {
        int salary = 50000;
        System.out.println(salary);
    }
}
```

Save it as `EmployeeApp.java`.

Compile:

```bash
javac EmployeeApp.java
```

This produces:

```text
EmployeeApp.class
```

Run:

```bash
java EmployeeApp
```

The JVM loads the class and executes its bytecode.

## 2.2 Complete execution flow

```text
EmployeeApp.java
       |
       | javac
       v
EmployeeApp.class (bytecode)
       |
       v
Class Loader
       |
       v
Bytecode Verification / Linking
       |
       v
Class Initialization
       |
       v
Runtime Execution
       |
       +----------------------+
       |                      |
       v                      v
   Interpreter             JIT Compiler
       |                      |
       +----------+-----------+
                  |
                  v
             Machine Code
                  |
                  v
              CPU executes
```

The exact runtime strategy is JVM-implementation-dependent. The diagram is a useful conceptual model.

## 2.3 Class Loader Subsystem

The Class Loader subsystem loads class definitions into the JVM.

Common built-in class loaders:

| Class loader | Responsibility |
|---|---|
| Bootstrap ClassLoader | Loads core Java platform classes |
| Platform ClassLoader | Loads platform classes and modules |
| Application ClassLoader | Loads application classes from the class path or module path |

### Parent delegation model

A class loader generally delegates a class-loading request to its parent before attempting to load the class itself.

Why is this useful?

- Avoids repeatedly loading core classes.
- Helps prevent application code from replacing trusted platform classes.
- Maintains a consistent class-loading hierarchy.

A simplified flow:

```text
Application ClassLoader
          |
          v
Platform ClassLoader
          |
          v
Bootstrap ClassLoader
```

If a parent cannot load a class, the request can be handled by the child according to the class loader's loading strategy.

## 2.4 Linking and initialization

After a class is loaded, the JVM performs linking. Linking includes:

1. **Verification:** Checks that bytecode is structurally and semantically valid.
2. **Preparation:** Allocates class-level storage and initializes static fields to default values.
3. **Resolution:** Converts symbolic references into direct references as required by the JVM.

Class initialization then executes static field initializers and static initializer blocks in the required order.

Example:

```java
class AppConfig {
    static int port = 8080;

    static {
        System.out.println("Class initialized");
    }
}
```

The static initialization runs when the class is initialized, not every time an object is created.

## 2.5 JVM Runtime Memory Areas

The JVM defines several runtime data areas.

### A. Heap

- Shared by threads.
- Stores objects and arrays.
- Managed by garbage collection.
- Usually the largest JVM memory area.

Example:

```java
Employee employee = new Employee();
```

The object is conceptually allocated on the heap. The local variable `employee` holds a reference to it.

### B. Java Stack

Each thread has its own JVM stack.

A method invocation creates a stack frame containing information such as:

- Local variables.
- Operand stack.
- Method invocation and return information.

Example:

```java
public void calculate() {
    int total = 100;
    helper();
}

public void helper() {
    int count = 5;
}
```

When `calculate()` calls `helper()`, a new frame is created for `helper()`. When `helper()` returns, its frame is removed.

### C. Method Area

The JVM specification describes a shared method area that stores per-class structures, such as runtime constant pools, field and method data, and method code.

In HotSpot JVM, class metadata is commonly stored in **Metaspace**, which uses native memory. Do not assume that every JVM implementation maps the specification's method area to exactly the same physical structure.

### D. PC Register

Each JVM thread has its own program counter register. It identifies the current JVM instruction being executed for that thread (with special handling for native methods).

### E. Native Method Stack

Supports execution of native methods, for example methods implemented in C or C++ through JNI. Its implementation is JVM-dependent.

## 2.6 Memory-area overview

| Memory area | Shared between threads? | Main purpose |
|---|---:|---|
| Heap | Yes | Objects and arrays |
| Java Stack | No, one per thread | Method frames and local execution state |
| Method Area | Yes | Class-level runtime structures |
| PC Register | No, one per thread | Current instruction position |
| Native Method Stack | Per thread conceptually | Native method execution |

### Important clarification: references and objects

```java
public void createEmployee() {
    Employee employee = new Employee();
}
```

Conceptually:

- `employee` is a local variable in the method's stack frame.
- The `Employee` object is allocated on the heap.
- When the method returns, the local variable goes out of scope.
- If no reachable reference to the object remains, it may become eligible for garbage collection.

JVM optimizations can change the physical allocation details, so stack-versus-heap diagrams are conceptual rather than a guarantee about every object.

## 2.7 Interpreter and JIT Compiler

### Interpreter

The interpreter executes bytecode instructions.

Advantages:
- Can start executing code quickly.
- Does not need to compile all code before execution.

Disadvantage:
- Repeated interpretation of frequently executed code can be slower than optimized machine code.

### JIT — Just-In-Time Compiler

The JIT compiler identifies frequently executed code and compiles it into optimized native machine code.

Typical flow:

```text
Bytecode
   |
   v
Interpreter executes code
   |
   v
Frequently executed ("hot") code detected
   |
   v
JIT compiles and optimizes
   |
   v
Native machine code executes
```

The JVM can use profiling information to optimize hot paths.

### Interview-ready answer

**Q: How does Java achieve platform independence?**

The Java compiler converts source code into platform-independent bytecode. A compatible JVM on each operating system loads and executes that bytecode, translating or compiling it into instructions suitable for the underlying platform. The bytecode is portable; the JVM implementation is platform-specific.

---

# 3. Object-Oriented Programming Concepts

Object-Oriented Programming (OOP) organizes software around objects that combine state and behavior.

Java is strongly object-oriented, although it also has primitive types and static members.

The four commonly discussed pillars are:

1. Encapsulation
2. Inheritance
3. Polymorphism
4. Abstraction

## 3.1 Encapsulation

Encapsulation means keeping an object's data and the operations that control that data together, while restricting uncontrolled access.

Example:

```java
public class Employee {
    private String name;
    private double salary;

    public Employee(String name, double salary) {
        this.name = name;
        setSalary(salary);
    }

    public String getName() {
        return name;
    }

    public double getSalary() {
        return salary;
    }

    public void setSalary(double salary) {
        if (salary < 0) {
            throw new IllegalArgumentException("Salary cannot be negative");
        }
        this.salary = salary;
    }
}
```

Why use encapsulation?

- Protects object state.
- Allows validation at the point of change.
- Reduces accidental misuse.
- Makes implementation easier to change without changing callers.

Without encapsulation, code could directly assign an invalid salary:

```java
employee.salary = -5000; // Not allowed when salary is private
```

## 3.2 Inheritance

Inheritance allows a class to derive fields and behavior from another class.

```java
class Employee {
    void work() {
        System.out.println("Employee is working");
    }
}

class Driver extends Employee {
    void drive() {
        System.out.println("Driver is driving");
    }
}
```

Usage:

```java
Driver driver = new Driver();
driver.work();  // Inherited method
driver.drive(); // Driver-specific method
```

Benefits:
- Reuse of common behavior.
- Supports runtime polymorphism.
- Represents an appropriate "is-a" relationship.

Do not use inheritance only to avoid writing a few repeated lines. Prefer composition when the relationship is "has-a" rather than "is-a".

## 3.3 Polymorphism

Polymorphism means one interface or parent type can represent objects with different concrete behaviors.

Example:

```java
class Employee {
    void performDuty() {
        System.out.println("General employee duty");
    }
}

class Driver extends Employee {
    @Override
    void performDuty() {
        System.out.println("Drive the assigned route");
    }
}

class Admin extends Employee {
    @Override
    void performDuty() {
        System.out.println("Manage employee operations");
    }
}
```

Usage:

```java
Employee employee = new Driver();
employee.performDuty(); // Drive the assigned route

employee = new Admin();
employee.performDuty(); // Manage employee operations
```

The reference type is `Employee`, but the overridden method selected at runtime depends on the actual object.

## 3.4 Abstraction

Abstraction exposes essential operations while hiding implementation details.

Example:

```java
interface PaymentService {
    void pay(double amount);
}

class CardPaymentService implements PaymentService {
    @Override
    public void pay(double amount) {
        System.out.println("Processing card payment: " + amount);
    }
}
```

A caller can depend on `PaymentService` without knowing all the internal details of card processing.

## 3.5 How the four pillars fit together

| Pillar | Main idea | Typical Java mechanism |
|---|---|---|
| Encapsulation | Protect internal state | `private` fields, controlled methods |
| Inheritance | Reuse and specialize behavior | `extends` |
| Polymorphism | Same parent/interface, different behavior | Overriding and dynamic dispatch |
| Abstraction | Expose what an object does, hide how | Interfaces and abstract classes |

### Backend connection

In a Spring Boot application:

- An entity may encapsulate its state.
- A specialized service can implement a service interface.
- A controller can depend on an interface rather than a concrete implementation.
- Spring Dependency Injection can provide the chosen implementation.

OOP is not just about writing classes. It is about managing responsibilities and dependencies so the application remains understandable and maintainable.

---

# 4. Classes, Objects, and Constructors

## 4.1 What is a class?

A class is a blueprint that defines the fields and methods its objects can have.

```java
public class Employee {
    String name;
    int employeeId;

    void displayDetails() {
        System.out.println(name + " - " + employeeId);
    }
}
```

The class defines the structure and behavior. It is not itself an individual employee record.

## 4.2 What is an object?

An object is an instance of a class.

```java
Employee employee1 = new Employee();
employee1.name = "Rahul";
employee1.employeeId = 101;

Employee employee2 = new Employee();
employee2.name = "Priya";
employee2.employeeId = 102;
```

`employee1` and `employee2` refer to separate objects with separate instance state.

## 4.3 Fields and methods

- **Field:** Stores object or class data.
- **Instance method:** Performs behavior using an object.
- **Static field:** Belongs to the class rather than to each individual instance.
- **Static method:** Can be called using the class name and does not have an implicit `this` reference.

Example:

```java
class Employee {
    String name;                 // Instance field
    static String company = "ABC"; // Static field

    void display() {             // Instance method
        System.out.println(name);
    }

    static void showCompany() {  // Static method
        System.out.println(company);
    }
}
```

## 4.4 Constructors

A constructor initializes a new object.

Rules:

- Constructor name must match the class name.
- It has no return type, not even `void`.
- It runs when an object is created using `new`.
- Constructors can be overloaded.
- Constructors are not inherited.

Example:

```java
class Employee {
    String name;
    int employeeId;

    Employee() {
        name = "Unknown";
        employeeId = 0;
    }

    Employee(String name, int employeeId) {
        this.name = name;
        this.employeeId = employeeId;
    }
}
```

Usage:

```java
Employee first = new Employee();
Employee second = new Employee("Anand", 101);
```

## 4.5 Default constructor vs no-argument constructor

These terms are often mixed up in interviews.

### Compiler-provided default constructor

If a class declares **no constructor at all**, the compiler provides a default no-argument constructor.

```java
class Employee {
    String name;
}
```

The compiler supplies a no-argument constructor.

### Explicit no-argument constructor

A developer can write one:

```java
class Employee {
    Employee() {
        System.out.println("Employee created");
    }
}
```

This is a no-argument constructor, but it is not the compiler-provided default constructor.

### Important rule

If you declare any constructor, the compiler does not automatically add the default constructor.

```java
class Employee {
    Employee(String name) {
        // ...
    }
}

// Employee employee = new Employee(); // Compilation error
```

If both forms are needed, declare both constructors.

## 4.6 The `this` keyword

`this` refers to the current object.

Common use: distinguish instance fields from parameters.

```java
class Employee {
    private String name;

    Employee(String name) {
        this.name = name;
    }
}
```

Without `this`, `name = name` would assign the parameter to itself.

## 4.7 Constructor chaining with `this()`

A constructor can call another constructor in the same class using `this()`.

```java
class Employee {
    String name;
    int employeeId;

    Employee() {
        this("Unknown", 0);
    }

    Employee(String name, int employeeId) {
        this.name = name;
        this.employeeId = employeeId;
    }
}
```

Rules:

- `this()` must be the first statement in a constructor.
- It helps avoid duplicated initialization logic.
- A constructor cannot call itself recursively, directly or indirectly.

## 4.8 Static members

A static member belongs to the class.

```java
class Employee {
    static int totalEmployees = 0;

    Employee() {
        totalEmployees++;
    }
}
```

Usage:

```java
new Employee();
new Employee();

System.out.println(Employee.totalEmployees); // 2
```

Use static members for data or behavior that genuinely belongs to the class as a whole. Avoid using mutable static state as a substitute for proper dependency management.

## 4.9 Object creation and Spring Beans

In ordinary Java:

```java
EmployeeService service = new EmployeeService();
```

The application code directly creates the object.

In Spring:

```java
@Service
public class EmployeeService {
    // Business logic
}
```

Spring discovers and creates the bean (subject to component scanning and configuration), manages its lifecycle, and injects it where required.

A Spring bean is still a Java object. The difference is that the Spring IoC container manages it.

### Interview-ready answer

**Q: What happens if you define a parameterized constructor but do not define a no-argument constructor?**

The compiler will not generate a default no-argument constructor because the class already declares a constructor. Therefore, `new Employee()` will fail to compile unless an explicit no-argument constructor is added.

---

# 5. Inheritance, Polymorphism, and Abstraction in Depth

## 5.1 Types of inheritance supported through classes

### Single inheritance

One child extends one parent.

```java
class Employee { }
class Driver extends Employee { }
```

### Multilevel inheritance

A class extends a class that itself extends another class.

```java
class Person { }
class Employee extends Person { }
class Driver extends Employee { }
```

### Hierarchical inheritance

Multiple child classes extend the same parent.

```java
class Employee { }
class Driver extends Employee { }
class Admin extends Employee { }
```

### Multiple inheritance through classes

Java does not allow a class to extend multiple classes.

```java
// Not valid Java:
// class C extends A, B { }
```

A class can implement multiple interfaces:

```java
interface Printable {
    void print();
}

interface Auditable {
    void audit();
}

class Report implements Printable, Auditable {
    public void print() { }
    public void audit() { }
}
```

## 5.2 The `super` keyword

`super` refers to the parent-class portion of the current object.

It is commonly used to:

- Call a parent constructor using `super(...)`.
- Call an overridden parent method using `super.methodName()`.
- Access a parent field when it is hidden by a child field.

Example:

```java
class Employee {
    Employee(String name) {
        System.out.println("Employee: " + name);
    }

    void work() {
        System.out.println("General work");
    }
}

class Driver extends Employee {
    Driver(String name) {
        super(name);
    }

    @Override
    void work() {
        super.work();
        System.out.println("Driving");
    }
}
```

A call to `super(...)` must be the first constructor statement. If no explicit constructor invocation is written, Java normally inserts a no-argument `super()` call. If the parent has no accessible no-argument constructor, the child must explicitly call an available parent constructor.

## 5.3 Method overloading — compile-time polymorphism

Overloading means multiple methods have the same name but different parameter lists.

```java
class Calculator {
    int add(int a, int b) {
        return a + b;
    }

    double add(double a, double b) {
        return a + b;
    }

    int add(int a, int b, int c) {
        return a + b + c;
    }
}
```

The compiler selects the applicable overload based on the method arguments and compile-time types.

Changing only the return type does not create a valid overload.

## 5.4 Method overriding — runtime polymorphism

A child class provides a new implementation of an inherited instance method.

```java
class Employee {
    void getRole() {
        System.out.println("Employee");
    }
}

class Driver extends Employee {
    @Override
    void getRole() {
        System.out.println("Driver");
    }
}
```

Usage:

```java
Employee employee = new Driver();
employee.getRole(); // Driver
```

The actual object's overridden method is selected at runtime.

Important rules and details:

- The overriding method cannot reduce access visibility.
- Its return type must be the same or a valid covariant subtype.
- It cannot declare broader checked exceptions than the overridden method allows.
- `@Override` helps the compiler detect accidental signature mismatches.
- Static methods are hidden, not overridden.
- Private methods are not overridden because they are not inherited in the normal sense.
- Final methods cannot be overridden.

## 5.5 Overloading vs overriding

| Feature | Overloading | Overriding |
|---|---|---|
| Meaning | Same name, different parameters | Child replaces inherited instance-method behavior |
| Usually occurs | In the same class, can also involve inheritance | Between parent and child |
| Selection | Compile time | Runtime dispatch |
| Parameters | Must differ | Must match the overridden signature |
| Return type | Cannot distinguish overloads by itself | Same or covariant return type |
| `static` methods | Can be overloaded | Static methods are hidden, not overridden |

## 5.6 Runtime polymorphism and parent references

```java
Employee employee = new Driver();
```

This is valid because a `Driver` is an `Employee`.

The reference type controls which members are accessible at compile time. The actual object type controls which overridden instance-method implementation runs.

```java
class Employee {
    void work() {
        System.out.println("Employee work");
    }
}

class Driver extends Employee {
    @Override
    void work() {
        System.out.println("Driver work");
    }

    void drive() {
        System.out.println("Driving");
    }
}

Employee employee = new Driver();
employee.work(); // Driver work
// employee.drive(); // Compilation error: not declared on Employee
```

## 5.7 Abstract classes

An abstract class cannot be instantiated directly.

```java
abstract class Employee {
    abstract void performDuty();

    void clockIn() {
        System.out.println("Clocked in");
    }
}
```

A concrete subclass must implement inherited abstract methods unless it is also abstract.

```java
class Driver extends Employee {
    @Override
    void performDuty() {
        System.out.println("Drive assigned route");
    }
}
```

An abstract class can contain:
- Abstract methods.
- Concrete methods.
- Constructors.
- Instance fields.
- Static members.

## 5.8 Interfaces

An interface defines a contract that implementing classes agree to provide.

```java
interface NotificationService {
    void send(String message);
}

class EmailNotificationService implements NotificationService {
    @Override
    public void send(String message) {
        System.out.println("Email: " + message);
    }
}
```

A class can implement multiple interfaces. This is one way Java supports multiple-type inheritance without allowing multiple class inheritance.

## 5.9 Abstract class vs interface

| Feature | Abstract class | Interface |
|---|---|---|
| Declaration | `abstract class` | `interface` |
| Inheritance | A class can extend one class | A class can implement multiple interfaces |
| Constructors | Yes | No constructors |
| Instance state | Can have instance fields | No ordinary per-instance fields |
| Methods | Abstract and concrete methods | Abstract, default, static and private methods |
| Fields | Can have different field types | Fields are implicitly `public static final` |
| Best use | Shared base state and behavior | Contract/capability across different classes |

### When should you use which?

Use an abstract class when closely related classes share state or common implementation.

Use an interface when different classes need to follow the same contract, even if they are otherwise unrelated.

### Spring backend connection

A service interface can define a contract:

```java
public interface EmployeeService {
    EmployeeResponse getEmployee(Long id);
}
```

An implementation supplies the behavior:

```java
@Service
public class EmployeeServiceImpl implements EmployeeService {
    @Override
    public EmployeeResponse getEmployee(Long id) {
        // Business logic
        return null;
    }
}
```

A controller can depend on the interface:

```java
@RestController
@RequestMapping("/api/employees")
public class EmployeeController {

    private final EmployeeService employeeService;

    public EmployeeController(EmployeeService employeeService) {
        this.employeeService = employeeService;
    }
}
```

Spring can inject a bean that implements the interface. If multiple implementations exist, the application needs an unambiguous selection strategy such as `@Primary` or `@Qualifier`.

---

# 6. Encapsulation, Interfaces, and Abstract Classes

## 6.1 Access modifiers

Java provides four commonly discussed access levels.

| Modifier | Same class | Same package | Subclass in another package | Other package, non-subclass |
|---|---:|---:|---:|---:|
| `private` | Yes | No | No | No |
| Package-private (no modifier) | Yes | Yes | No | No |
| `protected` | Yes | Yes | Yes, subject to subclass access rules | No |
| `public` | Yes | Yes | Yes | Yes |

### Example

```java
public class Employee {
    private double salary;
    String department;          // Package-private
    protected String employeeCode;
    public String name;
}
```

Use the narrowest visibility that satisfies the design.

### Why keep fields private?

Private fields allow a class to protect its invariants. For example, a salary setter can reject negative values, and a status transition method can prevent an invalid employee state.

## 6.2 Functional interfaces

A functional interface has exactly one abstract method. It can be used as the target type for a lambda expression or method reference.

```java
@FunctionalInterface
interface Greeting {
    void sayHello(String name);
}
```

Lambda:

```java
Greeting greeting = name ->
        System.out.println("Hello " + name);

greeting.sayHello("Anand");
```

The `@FunctionalInterface` annotation is optional, but it makes the compiler verify the functional-interface rule.

An interface may still contain default and static methods while remaining functional, because those are not additional abstract methods.

## 6.3 Common built-in functional interfaces

These are widely used in Java 8+ and Spring code.

| Interface | Main method | Input | Output | Common use |
|---|---|---|---|---|
| `Predicate<T>` | `test(T)` | T | boolean | Filtering |
| `Function<T, R>` | `apply(T)` | T | R | Transforming |
| `Consumer<T>` | `accept(T)` | T | void | Performing an action |
| `Supplier<T>` | `get()` | None | T | Providing a value |

Examples:

```java
Predicate<Integer> isAdultAge = age -> age >= 18;

Function<String, Integer> length =
        value -> value.length();

Consumer<String> printer =
        value -> System.out.println(value);

Supplier<String> defaultName =
        () -> "Unknown";
```

Usage:

```java
System.out.println(isAdultAge.test(21)); // true
System.out.println(length.apply("Java")); // 4
printer.accept("Hello");
System.out.println(defaultName.get()); // Unknown
```

## 6.4 Default and static methods in interfaces

An interface can provide a default implementation:

```java
interface EmployeeOperations {
    void save();

    default void logOperation() {
        System.out.println("Operation logged");
    }

    static String moduleName() {
        return "Employee Module";
    }
}
```

- Implementing classes inherit the default method unless they override it.
- Static interface methods are called using the interface name: `EmployeeOperations.moduleName()`.
- A static interface method is not inherited as an instance method by implementing classes.

## 6.5 Private methods in interfaces

Interfaces can also define private methods to share implementation between their own default or static methods.

```java
interface ReportService {
    default void generateReport() {
        validate();
        System.out.println("Generating report");
    }

    private void validate() {
        System.out.println("Validating input");
    }
}
```

The private helper is an implementation detail of the interface; implementing classes do not call it directly as an inherited method.

## 6.6 Interface fields

Every field declared in an interface is implicitly:

```java
public static final
```

Example:

```java
interface AppConstants {
    int MAX_RETRIES = 3;
}
```

This is equivalent to:

```java
public static final int MAX_RETRIES = 3;
```

An interface field is a constant, not per-object state.

## 6.7 Why interfaces matter in backend development

Interfaces reduce coupling between a caller and a particular implementation.

For example:

```java
public interface NotificationService {
    void send(String message);
}
```

Possible implementations:

```java
@Service
public class EmailNotificationService implements NotificationService {
    @Override
    public void send(String message) {
        // Send email
    }
}
```

```java
@Service
public class SmsNotificationService implements NotificationService {
    @Override
    public void send(String message) {
        // Send SMS
    }
}
```

A class that depends on `NotificationService` does not need to know the provider's internal details. In a real Spring application with multiple beans of the same interface, choose the implementation explicitly using configuration, `@Primary`, or `@Qualifier`.

---

# 7. `==` vs `.equals()` and `hashCode()`

This topic is important because Java collections such as `HashMap` and `HashSet` rely on the equality and hash-code contracts.

## 7.1 The `==` operator

For primitive values, `==` compares values.

```java
int a = 10;
int b = 10;

System.out.println(a == b); // true
```

For object references, `==` checks whether both references point to the same object.

```java
String first = new String("Java");
String second = new String("Java");

System.out.println(first == second); // false
```

The two String objects have equal text but are distinct objects.

## 7.2 The `.equals()` method

`equals()` is a method inherited from `Object`. Its default implementation compares object identity. Classes can override it to define logical equality.

String overrides `equals()` to compare character content.

```java
String first = new String("Java");
String second = new String("Java");

System.out.println(first.equals(second)); // true
```

For custom classes, define what equality means for the domain.

```java
import java.util.Objects;

class EmployeeKey {
    private final String employeeCode;

    EmployeeKey(String employeeCode) {
        this.employeeCode = employeeCode;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (!(obj instanceof EmployeeKey other)) {
            return false;
        }
        return Objects.equals(employeeCode, other.employeeCode);
    }

    @Override
    public int hashCode() {
        return Objects.hash(employeeCode);
    }
}
```

This example uses `employeeCode` as the logical identity.

## 7.3 String pool and reference comparison

String literals can be interned in the String Pool.

```java
String a = "Java";
String b = "Java";

System.out.println(a == b); // Usually true for these identical literals
System.out.println(a.equals(b)); // true
```

Both variables refer to the same interned literal.

But:

```java
String c = new String("Java");

System.out.println(a == c); // false
System.out.println(a.equals(c)); // true
```

`new String(...)` creates a distinct String object.

Do not use `==` to compare String content. Use `equals()` or an appropriate null-safe comparison.

## 7.4 The `hashCode()` method

`hashCode()` returns an integer hash value used by hash-based data structures.

The important Java contract:

1. If two objects are equal according to `equals()`, they must return the same hash code.
2. If two objects have the same hash code, they are not necessarily equal.
3. A hash code should remain consistent while information used by equality remains unchanged.

In short:

```text
a.equals(b) == true  =>  a.hashCode() == b.hashCode()
```

But the reverse is not guaranteed.

## 7.5 Why override `equals()` and `hashCode()` together?

Suppose a custom key class overrides `equals()` but keeps the default `hashCode()`.

Two logically equal keys may produce different hash values. A `HashMap` may search a different bucket and fail to find the value.

Therefore, if you override `equals()`, you should normally override `hashCode()` consistently.

## 7.6 How HashMap uses hashCode and equals

Conceptually:

```text
map.put(key, value)
       |
       v
Calculate key.hashCode()
       |
       v
Find bucket
       |
       v
If bucket contains candidate entries:
       |
       v
Compare hash and key equality
       |
       +---- Equal key found? ---- Yes --> Replace/update value
       |
       No
       |
       v
Insert a new entry
```

The exact implementation includes hash spreading, bucket indexing, and collision handling, but the key principle is that hash code helps locate a bucket and equality identifies the matching key.

## 7.7 Practical HashMap example

```java
Map<EmployeeKey, String> map = new HashMap<>();

EmployeeKey key1 = new EmployeeKey("EMP-101");
EmployeeKey key2 = new EmployeeKey("EMP-101");

map.put(key1, "Anand");

System.out.println(map.get(key2)); // Anand
```

This works because `key1` and `key2` are logically equal and have matching hash codes.

## 7.8 Mutable keys: a common bug

Avoid changing fields used by `equals()` or `hashCode()` after using an object as a hash-map key.

If the hash code changes after insertion, the map may look in a different bucket when retrieving the entry.

Prefer immutable key objects.

## 7.9 Entity equality caution

JPA entities have lifecycle changes: a generated database ID may be `null` before persistence and assigned later.

Therefore, a simplistic `equals()`/`hashCode()` based on a generated ID can be problematic if the entity is inserted into a `HashSet` before its ID is assigned. Entity equality also needs to account for proxies and inheritance.

For interview purposes, remember:
- Use stable identity where possible.
- Do not casually use mutable fields in hash-based keys.
- Design entity equality deliberately rather than copying a generic implementation.

### Interview-ready answer

**Q: What is the contract between `equals()` and `hashCode()`?**

If two objects are equal according to `equals()`, they must have the same hash code. However, two objects can share a hash code and still be unequal. Hash-based collections use the hash code to locate a bucket and equality to identify the matching key. That is why `equals()` and `hashCode()` should be overridden consistently.

---

# 8. String, StringBuilder, and StringBuffer

## 8.1 String

`String` represents a sequence of characters.

The most important property is that String objects are **immutable**. Once created, their content cannot be changed.

```java
String value = "Hello";
value.concat(" Java");

System.out.println(value); // Hello
```

`concat()` returns a new String. The original variable still refers to `"Hello"` because the result was not assigned.

```java
value = value.concat(" Java");
System.out.println(value); // Hello Java
```

Why is immutability useful?

- Strings can be safely shared.
- They are suitable for keys and identifiers when not modified.
- They support String Pool reuse.
- Their content cannot unexpectedly change through another reference.

## 8.2 String Pool

Java maintains a pool of interned strings so identical literals can share a reference.

```java
String a = "Spring";
String b = "Spring";

System.out.println(a == b); // true
```

A string created with `new` is a distinct object:

```java
String c = new String("Spring");

System.out.println(a == c); // false
System.out.println(a.equals(c)); // true
```

The `intern()` method returns the canonical pooled representation of a string.

```java
String d = c.intern();

System.out.println(a == d); // true
```

Use `intern()` only when you understand the memory and performance implications; it is not required for ordinary String comparison.

## 8.3 String concatenation and performance

Repeated concatenation inside a loop can create many intermediate String objects.

Less suitable for repeated concatenation:

```java
String result = "";

for (int i = 0; i < 1000; i++) {
    result = result + i;
}
```

For repeated modifications, use a mutable builder:

```java
StringBuilder result = new StringBuilder();

for (int i = 0; i < 1000; i++) {
    result.append(i);
}

String finalResult = result.toString();
```

Modern Java compilers may optimize individual concatenation expressions, but `StringBuilder` remains a clear choice for repeated incremental construction.

## 8.4 StringBuilder

`StringBuilder` is mutable and not synchronized.

```java
StringBuilder builder = new StringBuilder("Hello");

builder.append(" Java");
builder.append(" Backend");

System.out.println(builder); // Hello Java Backend
```

Useful methods:

| Method | Purpose |
|---|---|
| `append()` | Add content |
| `insert()` | Insert content at an index |
| `delete()` | Remove a range |
| `replace()` | Replace a range |
| `reverse()` | Reverse content |
| `toString()` | Produce a String |

Use it for repeated string modification in a single-threaded context or when access is otherwise safely coordinated.

## 8.5 StringBuffer

`StringBuffer` is mutable and its main methods are synchronized.

```java
StringBuffer buffer = new StringBuffer("Hello");
buffer.append(" Java");

System.out.println(buffer);
```

Synchronization makes its methods safer for concurrent access to the same buffer, but it can add overhead. It does not automatically make a larger multi-step business operation atomic.

## 8.6 String vs StringBuilder vs StringBuffer

| Feature | String | StringBuilder | StringBuffer |
|---|---|---|---|
| Mutable? | No | Yes | Yes |
| Thread-safe by design? | Safe to share due to immutability | No | Individual methods synchronized |
| Repeated modifications | Creates new results | Efficient | Mutable, synchronized |
| Typical use | Text values, keys, messages | Building strings in one thread | Shared mutable text where synchronized methods are needed |

## 8.7 Backend examples

### Creating a response message

```java
String message = "Employee created successfully";
```

A String is appropriate because the message is not being incrementally modified.

### Building a dynamic report

```java
StringBuilder report = new StringBuilder();

report.append("Employee Report\n");
report.append("Total employees: ").append(120).append('\n');
report.append("Active employees: ").append(110).append('\n');

String response = report.toString();
```

### Important interview point

**Q: Why is String immutable in Java?**

Immutability makes String values safe to share, supports String Pool reuse, and prevents a String's content from changing unexpectedly. It also makes Strings reliable for many common uses such as configuration values and map keys.

---

# 9. Day 7 — Quick Revision Sheet

## 9.1 One-line definitions

| Concept | Remember |
|---|---|
| JVM | Loads, verifies, and executes Java bytecode |
| JRE | Conceptual runtime environment with JVM and libraries |
| JDK | Development kit with compiler and tools |
| Bytecode | Platform-independent class-file instructions |
| Class Loader | Loads class definitions |
| Heap | Runtime area for objects and arrays |
| Stack | Per-thread method frames and local execution state |
| Metaspace | HotSpot native-memory area for class metadata |
| JIT | Compiles hot bytecode paths into optimized machine code |
| Encapsulation | Protects state through controlled access |
| Inheritance | Derives behavior from a parent class |
| Polymorphism | One parent/interface type, multiple implementations |
| Abstraction | Exposes essential behavior and hides implementation |
| Constructor | Initializes a newly created object |
| Overloading | Same method name, different parameter lists |
| Overriding | Child supplies a new implementation of inherited instance method |
| Interface | Contract a class can implement |
| Abstract class | Base class that cannot be instantiated directly |
| `==` | Primitive value comparison or reference identity |
| `equals()` | Logical equality as defined by a class |
| `hashCode()` | Hash value used by hash-based collections |
| String | Immutable text |
| StringBuilder | Mutable, unsynchronized text builder |
| StringBuffer | Mutable text builder with synchronized methods |

## 9.2 Important rules to remember

1. `javac` compiles Java source into bytecode; `java` launches the program.
2. JVM implementations are platform-specific; Java bytecode is portable.
3. Heap is shared; each thread has its own Java stack and PC register.
4. The compiler provides a default constructor only if no constructor is declared.
5. `this()` and `super()` constructor calls must be first.
6. Java classes support single class inheritance.
7. A class can implement multiple interfaces.
8. Overloading is resolved at compile time; overriding uses runtime dispatch.
9. Static methods are hidden, not overridden.
10. Interface fields are implicitly `public static final`.
11. A functional interface has one abstract method.
12. Equal objects must have equal hash codes.
13. Equal hash codes do not guarantee equal objects.
14. Use `.equals()` to compare String content.
15. String is immutable; StringBuilder and StringBuffer are mutable.

## 9.3 Concept map

```text
CORE JAVA
   |
   +-- Java Platform
   |     +-- JDK
   |     +-- JRE (conceptual)
   |     +-- JVM
   |           +-- Class Loading
   |           +-- Linking / Initialization
   |           +-- Runtime Memory
   |           +-- Interpreter / JIT
   |
   +-- OOP
   |     +-- Encapsulation
   |     +-- Inheritance
   |     +-- Polymorphism
   |     +-- Abstraction
   |
   +-- Classes and Objects
   |     +-- Fields and Methods
   |     +-- Constructors
   |     +-- this / super
   |     +-- static
   |
   +-- Type Contracts
   |     +-- Interfaces
   |     +-- Abstract Classes
   |     +-- Access Modifiers
   |
   +-- Object Equality
   |     +-- ==
   |     +-- equals()
   |     +-- hashCode()
   |
   +-- Text Handling
         +-- String
         +-- StringBuilder
         +-- StringBuffer
```

---

# 10. Interview Questions and Answers

## Q1. What is the difference between JDK, JRE, and JVM?

**Answer:** JDK is the development kit containing tools such as the Java compiler and runtime components. JRE is the conceptual environment for running Java applications, including JVM and libraries. JVM loads, verifies, and executes bytecode. Modern JDK distributions may not provide a separate JRE installation.

## Q2. How does Java achieve platform independence?

**Answer:** Java source is compiled into bytecode. A compatible JVM on each target platform executes that bytecode using the platform's runtime implementation. The bytecode is portable, while the JVM is platform-specific.

## Q3. What happens when you run a Java program?

**Answer:** The Java launcher starts the JVM. The class loader loads required classes, the JVM verifies and links them, class initialization occurs when required, and the runtime executes bytecode using interpretation and JIT compilation.

## Q4. What is the difference between heap and stack?

**Answer:** The heap is shared among threads and is used for objects and arrays. Each thread has its own Java stack, which stores method frames and local execution state. Objects are conceptually allocated on the heap, while local variables and references may be stored in stack frames, subject to JVM optimizations.

## Q5. What is JIT compilation?

**Answer:** JIT stands for Just-In-Time compilation. The JVM can compile frequently executed bytecode into optimized native machine code, improving performance on hot execution paths.

## Q6. Explain the four pillars of OOP.

**Answer:** Encapsulation protects object state, inheritance derives behavior from a parent, polymorphism allows one parent or interface type to represent different implementations, and abstraction exposes essential operations while hiding implementation details.

## Q7. What is the difference between a class and an object?

**Answer:** A class defines the structure and behavior. An object is a runtime instance of that class with its own instance state.

## Q8. What is a constructor?

**Answer:** A constructor initializes a new object. It has the same name as the class, has no return type, and is invoked during object creation. Constructors can be overloaded but are not inherited.

## Q9. What happens if a class has only a parameterized constructor?

**Answer:** The compiler does not provide a default no-argument constructor. A no-argument object creation call fails unless an explicit no-argument constructor is declared.

## Q10. What is the difference between `this` and `super`?

**Answer:** `this` refers to the current object and can call another constructor in the same class using `this()`. `super` refers to the parent-class portion and can call a parent constructor or parent implementation using `super()` or `super.method()`.

## Q11. What is the difference between overloading and overriding?

**Answer:** Overloading uses the same method name with different parameter lists and is resolved at compile time. Overriding replaces inherited instance-method behavior in a subclass and is selected through runtime dispatch.

## Q12. Does Java support multiple inheritance?

**Answer:** Java does not allow a class to extend multiple classes. A class can implement multiple interfaces, allowing it to satisfy multiple contracts.

## Q13. What is the difference between an abstract class and an interface?

**Answer:** An abstract class can hold instance state, constructors, and both abstract and concrete methods. An interface defines a contract and supports abstract, default, static, and private methods. A class can extend only one class but implement multiple interfaces.

## Q14. What is a functional interface?

**Answer:** A functional interface has exactly one abstract method and can be used as the target type for a lambda expression or method reference. It may also contain default and static methods.

## Q15. What is the difference between `==` and `.equals()`?

**Answer:** For primitives, `==` compares values. For object references, it checks whether both references identify the same object. `equals()` checks logical equality according to the class implementation.

## Q16. Why must `equals()` and `hashCode()` be consistent?

**Answer:** Hash-based collections use `hashCode()` to locate a bucket and `equals()` to identify a matching key. Equal objects must have equal hash codes; otherwise, lookups can fail.

## Q17. Can two unequal objects have the same hash code?

**Answer:** Yes. This is a hash collision. A hash code is not a unique object identifier. Hash-based collections use equality checks to distinguish colliding keys.

## Q18. Why is String immutable?

**Answer:** Immutability makes String values safe to share, supports String Pool reuse, and prevents content from changing unexpectedly after creation.

## Q19. What is the difference between StringBuilder and StringBuffer?

**Answer:** Both are mutable. StringBuilder is not synchronized and is generally preferred for single-threaded string construction. StringBuffer synchronizes its methods and is useful when synchronized access to the same buffer is required.

## Q20. Why is a Spring bean still considered a Java object?

**Answer:** A Spring bean is an ordinary Java object whose creation, configuration, dependency injection, and lifecycle are managed by the Spring IoC container. Spring adds management around the object; it does not change the fact that it is a Java object.

---

# 11. Practical Backend Connections

The following examples connect Day 7 concepts to a typical Employee Transport backend.

## 11.1 Interface-based service design

```java
public interface DriverService {
    DriverResponse getDriver(Long id);
}
```

```java
@Service
public class DriverServiceImpl implements DriverService {

    @Override
    public DriverResponse getDriver(Long id) {
        // Find driver, apply business rules, map response
        return null;
    }
}
```

Concepts:
- Interface defines a contract.
- Implementation provides behavior.
- Spring can inject the implementation.
- Controller can depend on the interface.

## 11.2 Encapsulation in a domain object

```java
public class Vehicle {
    private String registrationNumber;
    private boolean active;

    public Vehicle(String registrationNumber) {
        if (registrationNumber == null || registrationNumber.isBlank()) {
            throw new IllegalArgumentException(
                    "Registration number is required"
            );
        }

        this.registrationNumber = registrationNumber;
        this.active = true;
    }

    public String getRegistrationNumber() {
        return registrationNumber;
    }

    public boolean isActive() {
        return active;
    }

    public void deactivate() {
        this.active = false;
    }
}
```

The object controls its own state through methods instead of exposing unrestricted field mutation.

## 11.3 Polymorphism for notifications

```java
public interface NotificationService {
    void send(String destination, String message);
}
```

```java
public class EmailNotificationService implements NotificationService {
    @Override
    public void send(String destination, String message) {
        System.out.println("Sending email to " + destination);
    }
}
```

```java
public class SmsNotificationService implements NotificationService {
    @Override
    public void send(String destination, String message) {
        System.out.println("Sending SMS to " + destination);
    }
}
```

A caller can use the interface:

```java
public class AlertManager {
    private final NotificationService notificationService;

    public AlertManager(NotificationService notificationService) {
        this.notificationService = notificationService;
    }

    public void notifyUser(String destination, String message) {
        notificationService.send(destination, message);
    }
}
```

The caller does not need to know whether the implementation sends email or SMS.

## 11.4 Equality for a stable business key

If an application uses an immutable business identifier as a key, equality can be based on that identifier.

```java
public final class EmployeeCode {
    private final String value;

    public EmployeeCode(String value) {
        if (value == null || value.isBlank()) {
            throw new IllegalArgumentException("Employee code is required");
        }
        this.value = value;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof EmployeeCode other)) return false;
        return value.equals(other.value);
    }

    @Override
    public int hashCode() {
        return value.hashCode();
    }
}
```

This illustrates why immutable value objects are useful as map keys: the fields used for equality do not change after insertion.

---

# 12. Completion Checklist

Use this checklist to confirm that you can explain the Day 7 concepts without reading the notes.

- [ ] Explain JDK, JRE, and JVM and how they relate.
- [ ] Explain source compilation, bytecode, class loading, linking, and execution.
- [ ] Describe heap, stack, method area/Metaspace, PC register, and native method stack.
- [ ] Explain interpreter and JIT compiler.
- [ ] Explain all four OOP pillars with Java examples.
- [ ] Distinguish class, object, field, method, constructor, and static member.
- [ ] Explain compiler-provided default constructor vs explicit no-argument constructor.
- [ ] Explain `this()`, `this`, `super()`, and `super`.
- [ ] Describe single, multilevel, and hierarchical inheritance.
- [ ] Explain why Java does not support multiple class inheritance.
- [ ] Distinguish method overloading from method overriding.
- [ ] Explain abstract classes, interfaces, and functional interfaces.
- [ ] Explain access modifiers, including the cross-package protected rule.
- [ ] Explain `==`, `equals()`, and `hashCode()`.
- [ ] Explain how HashMap uses hash code and equality.
- [ ] Explain String immutability and String Pool.
- [ ] Choose between String, StringBuilder, and StringBuffer appropriately.
- [ ] Connect interfaces, encapsulation, and polymorphism to Spring service design.

---

**Day 7 status:** Core Java Fundamentals — all eight planned topics covered.

**Next day:** Day 8 — Java Collections Framework: Collection hierarchy, List, ArrayList, LinkedList, Set, HashMap internals, Queue, Deque, PriorityQueue, sorting, and iterators.
'''

path = Path("/mnt/data/Day_07_Core_Java_Fundamentals.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
