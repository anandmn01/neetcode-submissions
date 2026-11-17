from pathlib import Path

content = r'''# Backend Notes — Day 15
## SOLID Principles, Design Patterns, and Backend API Design

> **Java Backend Developer Interview Preparation**  
> **Status:** Complete Day 15 study notes  
> **Primary stack:** Java 21, Spring Boot, Spring MVC, Spring Data JPA  
> **Project context:** Employee Transport backend (employees, drivers, vehicles, trips, passengers and notifications)

---

## Learning objectives

By the end of Day 15, you should be able to:

- Explain all five SOLID principles with Java examples.
- Recognize tightly coupled and poorly structured code.
- Explain what design patterns are and why they are useful.
- Classify patterns into creational, structural and behavioral groups.
- Understand commonly used patterns such as Factory, Builder, Singleton, Adapter, Decorator, Strategy, Observer and Template Method.
- Recognize patterns already used by Spring and Spring Boot.
- Design clear REST APIs with appropriate resources, HTTP methods and status codes.
- Explain DTOs, pagination, filtering, sorting, versioning and idempotency.
- Describe how a Flutter client and Spring Boot backend should agree on an API contract.

## Contents

1. SOLID principles
2. Design patterns — fundamentals and categories
3. Creational design patterns
4. Structural design patterns
5. Behavioral design patterns
6. Design patterns in Spring Boot
7. REST API design fundamentals
8. Practical API design and production considerations
9. Interview questions and answers
10. Quick revision sheet
11. Completion checklist

---

# 1. SOLID Principles in Java

## 1.1 What are SOLID principles?

SOLID is a group of five object-oriented design principles intended to make software easier to maintain, extend, test and understand.

| Letter | Principle |
|---|---|
| S | Single Responsibility Principle |
| O | Open/Closed Principle |
| L | Liskov Substitution Principle |
| I | Interface Segregation Principle |
| D | Dependency Inversion Principle |

SOLID is guidance, not a rule that every class must have an interface or that every small change requires a new abstraction. Apply the principles when they reduce coupling and make responsibilities clearer.

## 1.2 Single Responsibility Principle (SRP)

**Definition:** A class should have one reason to change.

A class should have one cohesive responsibility. It does not mean a class can contain only one method.

### Violation

```java
public class EmployeeService {

    public void saveEmployee() {
        // Database persistence
    }

    public void generateReport() {
        // Report generation
    }

    public void sendEmail() {
        // Email delivery
    }
}
```

This class has multiple unrelated reasons to change: database behavior, report formatting and email delivery.

### Better design

```java
public class EmployeeService {
    public void saveEmployee() {
        // Employee business operation
    }
}

public class EmployeeReportService {
    public void generateReport() {
        // Report generation
    }
}

public class EmailService {
    public void sendEmail() {
        // Email delivery
    }
}
```

### Backend application

For an Employee Transport backend, responsibilities can be separated into:

- `EmployeeController`: handles HTTP requests and responses.
- `EmployeeService`: coordinates employee business rules.
- `EmployeeRepository`: performs persistence operations.
- `NotificationService`: handles notification behavior.
- `TripReportService`: creates trip reports.

These classes may collaborate, but each should have a cohesive responsibility.

## 1.3 Open/Closed Principle (OCP)

**Definition:** Software entities should be open for extension but closed for modification.

You should be able to introduce a new behavior by adding an implementation or extension instead of repeatedly editing a large conditional block.

### Violation

```java
public class NotificationService {

    public void send(String type) {
        if ("EMAIL".equals(type)) {
            System.out.println("Sending email");
        } else if ("SMS".equals(type)) {
            System.out.println("Sending SMS");
        }
    }
}
```

Adding WhatsApp requires modifying this class and adding another condition.

### Better design using an abstraction

```java
public interface Notification {
    void send(String message);
}

public class EmailNotification implements Notification {
    @Override
    public void send(String message) {
        System.out.println("Email: " + message);
    }
}

public class SmsNotification implements Notification {
    @Override
    public void send(String message) {
        System.out.println("SMS: " + message);
    }
}
```

A new implementation can be added without changing the existing notification implementations.

**Important:** OCP does not mean existing code can never change. It means design likely extension points so common additions do not require risky modifications to stable code.

## 1.4 Liskov Substitution Principle (LSP)

**Definition:** An object of a subtype should be usable wherever its parent type is expected without breaking the expected behavior.

A subtype must honor the behavioral contract of the parent abstraction.

### Violation

```java
class Vehicle {
    public void startEngine() {
        System.out.println("Engine started");
    }
}

class Bicycle extends Vehicle {
    @Override
    public void startEngine() {
        throw new UnsupportedOperationException(
                "A bicycle has no engine"
        );
    }
}
```

The base class promises that every `Vehicle` can start an engine, but a bicycle cannot satisfy that contract.

### Better design

```java
interface Vehicle {
    void move();
}

interface EngineVehicle extends Vehicle {
    void startEngine();
}

class Car implements EngineVehicle {
    @Override
    public void move() {
        System.out.println("Car is moving");
    }

    @Override
    public void startEngine() {
        System.out.println("Car engine started");
    }
}

class Bicycle implements Vehicle {
    @Override
    public void move() {
        System.out.println("Bicycle is moving");
    }
}
```

LSP is about behavioral compatibility, not simply whether Java allows a class to extend another class.

## 1.5 Interface Segregation Principle (ISP)

**Definition:** A class should not be forced to depend on methods it does not use.

Prefer smaller, focused interfaces over one large interface containing unrelated operations.

### Violation

```java
interface EmployeeActions {
    void login();
    void driveVehicle();
    void assignTrip();
    void generateSalary();
}
```

A passenger implementation should not be forced to implement driver-only or payroll-only methods.

### Better design

```java
interface LoginCapability {
    void login();
}

interface DrivingCapability {
    void driveVehicle();
}

interface TripAssignmentCapability {
    void assignTrip();
}

interface PayrollCapability {
    void generateSalary();
}
```

Each class implements only the capabilities relevant to its role.

## 1.6 Dependency Inversion Principle (DIP)

**Definition:**

- High-level modules should not depend directly on low-level modules.
- Both should depend on abstractions.
- Abstractions should not depend on implementation details.

### Violation

```java
public class EmailService {
    public void send() {
        System.out.println("Email sent");
    }
}

public class EmployeeService {
    private final EmailService emailService = new EmailService();

    public void registerEmployee() {
        emailService.send();
    }
}
```

`EmployeeService` is tightly coupled to `EmailService`.

### Better design

```java
public interface NotificationService {
    void send();
}

public class EmailNotificationService
        implements NotificationService {

    @Override
    public void send() {
        System.out.println("Email sent");
    }
}

public class SmsNotificationService
        implements NotificationService {

    @Override
    public void send() {
        System.out.println("SMS sent");
    }
}
```

```java
public class EmployeeService {

    private final NotificationService notificationService;

    public EmployeeService(
            NotificationService notificationService) {
        this.notificationService = notificationService;
    }

    public void registerEmployee() {
        notificationService.send();
    }
}
```

The high-level service depends on the interface, not a specific notification class.

### Spring Boot connection

Spring's dependency injection container can provide an implementation of an interface:

```java
@Service
public class EmailNotificationService
        implements NotificationService {
    // implementation
}
```

```java
@Service
public class EmployeeService {

    private final NotificationService notificationService;

    public EmployeeService(NotificationService notificationService) {
        this.notificationService = notificationService;
    }
}
```

If multiple beans implement the same interface, use `@Primary` or `@Qualifier` to select the intended bean.

## 1.7 SOLID comparison

| Principle | Main question |
|---|---|
| SRP | Does this class have one cohesive responsibility? |
| OCP | Can common new behavior be added without repeatedly changing stable code? |
| LSP | Can each implementation honor the abstraction's contract? |
| ISP | Are clients forced to depend on methods they do not need? |
| DIP | Do high-level modules depend on abstractions instead of concrete details? |

## 1.8 Common SOLID mistakes

- Creating an interface for every class without a real need.
- Splitting code into many tiny classes that have no independent responsibility.
- Treating inheritance as automatically satisfying LSP.
- Using OCP as an excuse to build complicated plugin systems for unlikely changes.
- Confusing dependency injection with dependency inversion: DI is a technique; DIP is a design principle.
- Putting all business logic into controllers even though services already exist.

---

# 2. Design Patterns — Fundamentals and Categories

## 2.1 What is a design pattern?

A design pattern is a reusable, named approach to a recurring software design problem.

It is not a ready-made library or a block of code that must be copied into every project. It describes a structure and collaboration between objects that can be adapted to a particular problem.

## 2.2 Why do developers use design patterns?

Patterns can help to:

1. Reduce tight coupling.
2. Make code easier to extend.
3. Give teams a shared vocabulary.
4. Improve maintainability.
5. Make object creation and collaboration more predictable.
6. Avoid repeatedly solving the same design problem from scratch.

A pattern is useful only when it fits the problem. Applying a pattern unnecessarily can make code harder to understand.

## 2.3 Three major categories

| Category | Main purpose | Examples |
|---|---|---|
| Creational | Control or simplify object creation | Singleton, Factory Method, Builder, Abstract Factory |
| Structural | Compose classes and objects into larger structures | Adapter, Decorator, Facade, Proxy |
| Behavioral | Organize communication and responsibilities between objects | Strategy, Observer, Template Method, Chain of Responsibility |

### How to remember

- Creational: How is an object created?
- Structural: How are objects/classes connected?
- Behavioral: How do objects communicate or decide what to do?

---

# 3. Creational Design Patterns

Creational patterns deal with object creation. They help avoid scattered construction logic and can hide implementation details from callers.

## 3.1 Singleton Pattern

**Purpose:** Ensure that a class has one shared instance and provide a way to access it.

### Basic example

```java
public final class AppConfiguration {

    private static final AppConfiguration INSTANCE =
            new AppConfiguration();

    private AppConfiguration() {
    }

    public static AppConfiguration getInstance() {
        return INSTANCE;
    }
}
```

The constructor is private, so outside code cannot create another instance through `new`.

### Thread-safe lazy initialization using a holder

```java
public final class AppConfiguration {

    private AppConfiguration() {
    }

    private static class Holder {
        private static final AppConfiguration INSTANCE =
                new AppConfiguration();
    }

    public static AppConfiguration getInstance() {
        return Holder.INSTANCE;
    }
}
```

The nested holder is initialized when first accessed, using Java class initialization guarantees.

### Singleton in Spring

Spring beans are singleton-scoped by default. This means one bean instance is created per Spring IoC container and shared by injection points in that container.

A Spring singleton bean is not necessarily the same thing as a class-level GoF Singleton implementation. Spring manages the bean lifecycle and scope.

### Important warning

Singleton scope does not automatically make an object thread-safe. Avoid mutable shared state in singleton service beans unless access is safely coordinated.

## 3.2 Factory Method Pattern

**Purpose:** Provide a method or abstraction for creating an object while allowing the concrete type to be selected by an implementation or decision.

### Example

```java
interface Notification {
    void send(String message);
}

class EmailNotification implements Notification {
    public void send(String message) {
        System.out.println("Email: " + message);
    }
}

class SmsNotification implements Notification {
    public void send(String message) {
        System.out.println("SMS: " + message);
    }
}
```

A simple factory helper can centralize selection:

```java
class NotificationFactory {

    public static Notification create(String type) {
        return switch (type.toUpperCase()) {
            case "EMAIL" -> new EmailNotification();
            case "SMS" -> new SmsNotification();
            default -> throw new IllegalArgumentException(
                    "Unsupported notification type: " + type
            );
        };
    }
}
```

Usage:

```java
Notification notification =
        NotificationFactory.create("EMAIL");

notification.send("Welcome");
```

This example is commonly called a Simple Factory. It demonstrates the idea of centralizing object creation. The formal GoF Factory Method pattern uses a creator abstraction whose subclasses decide which product to create.

### When useful

- Creating payment providers.
- Selecting notification implementations.
- Choosing file parsers.
- Constructing different report exporters.

## 3.3 Builder Pattern

**Purpose:** Construct complex objects step by step, especially when there are many optional parameters.

### Problem with many constructor parameters

```java
Employee employee = new Employee(
        "Anand",
        "anand@example.com",
        "Gurugram",
        true,
        25
);
```

It can be difficult to remember what each positional argument represents.

### Builder example

```java
public class Employee {

    private final String name;
    private final String email;
    private final String city;
    private final boolean active;

    private Employee(Builder builder) {
        this.name = builder.name;
        this.email = builder.email;
        this.city = builder.city;
        this.active = builder.active;
    }

    public static class Builder {
        private String name;
        private String email;
        private String city;
        private boolean active = true;

        public Builder name(String name) {
            this.name = name;
            return this;
        }

        public Builder email(String email) {
            this.email = email;
            return this;
        }

        public Builder city(String city) {
            this.city = city;
            return this;
        }

        public Builder active(boolean active) {
            this.active = active;
            return this;
        }

        public Employee build() {
            return new Employee(this);
        }
    }
}
```

Usage:

```java
Employee employee = new Employee.Builder()
        .name("Anand")
        .email("anand@example.com")
        .city("Gurugram")
        .active(true)
        .build();
```

### When useful

- Objects with many optional fields.
- Immutable value objects.
- Request or configuration objects.
- Test data setup.

A builder can validate required fields in `build()` before creating the object.

## 3.4 Abstract Factory Pattern

**Purpose:** Provide an interface for creating families of related objects without exposing their concrete classes.

Imagine a system supports two transport providers, each with its own trip and invoice objects.

```java
interface Trip {
    void create();
}

interface Invoice {
    void generate();
}

interface TransportProviderFactory {
    Trip createTrip();
    Invoice createInvoice();
}
```

A provider-specific factory implements both creation methods:

```java
class ProviderAFactory implements TransportProviderFactory {
    public Trip createTrip() {
        return new ProviderATrip();
    }

    public Invoice createInvoice() {
        return new ProviderAInvoice();
    }
}
```

The concrete `ProviderATrip` and `ProviderAInvoice` classes would implement their respective interfaces.

### Factory Method vs Abstract Factory

| Factory Method | Abstract Factory |
|---|---|
| Focuses on creating a product through a creator method | Focuses on creating a family of related products |
| Often one product hierarchy | Multiple related product hierarchies |
| Subclasses/creator implementations choose the product | A factory supplies related products consistently |

---

# 4. Structural Design Patterns

Structural patterns explain how objects and classes can be combined while keeping the overall structure flexible.

## 4.1 Adapter Pattern

**Purpose:** Make two incompatible interfaces work together by converting one interface into another expected by the client.

### Scenario

Your backend expects a standard payment interface, but an external payment provider exposes a different method name and request format.

```java
interface PaymentService {
    void pay(double amount);
}
```

External provider:

```java
class ExternalPaymentGateway {
    public void makePaymentInRupees(double value) {
        System.out.println("Paid: " + value);
    }
}
```

Adapter:

```java
class PaymentAdapter implements PaymentService {

    private final ExternalPaymentGateway gateway;

    public PaymentAdapter(ExternalPaymentGateway gateway) {
        this.gateway = gateway;
    }

    @Override
    public void pay(double amount) {
        gateway.makePaymentInRupees(amount);
    }
}
```

The application uses `PaymentService`; the adapter translates the call to the external provider's API.

### When useful

- Integrating a third-party API.
- Wrapping a legacy library.
- Converting one data format or interface into another.

## 4.2 Decorator Pattern

**Purpose:** Add behavior to an object dynamically by wrapping it, without changing the original class.

```java
interface Notification {
    void send(String message);
}

class BasicNotification implements Notification {
    public void send(String message) {
        System.out.println("Notification: " + message);
    }
}

class LoggingNotificationDecorator implements Notification {

    private final Notification wrapped;

    public LoggingNotificationDecorator(Notification wrapped) {
        this.wrapped = wrapped;
    }

    public void send(String message) {
        System.out.println("Logging notification");
        wrapped.send(message);
    }
}
```

Usage:

```java
Notification notification =
        new LoggingNotificationDecorator(
                new BasicNotification()
        );

notification.send("Trip assigned");
```

The decorator adds logging around the original behavior.

### When useful

- Adding logging or metrics.
- Adding compression or buffering to streams.
- Adding optional behavior without creating many subclasses.

## 4.3 Facade Pattern

**Purpose:** Provide a simple interface over a complex subsystem.

Suppose trip creation requires several operations:

1. Validate employee.
2. Check driver availability.
3. Check vehicle availability.
4. Create trip.
5. Notify the driver.

A facade can expose one easy-to-use operation:

```java
public class TripCreationFacade {

    private final EmployeeService employeeService;
    private final DriverService driverService;
    private final VehicleService vehicleService;
    private final TripService tripService;

    public TripCreationFacade(
            EmployeeService employeeService,
            DriverService driverService,
            VehicleService vehicleService,
            TripService tripService) {
        this.employeeService = employeeService;
        this.driverService = driverService;
        this.vehicleService = vehicleService;
        this.tripService = tripService;
    }

    public void createTrip(Long employeeId, Long driverId, Long vehicleId) {
        employeeService.validate(employeeId);
        driverService.validateAvailability(driverId);
        vehicleService.validateAvailability(vehicleId);
        tripService.create(employeeId, driverId, vehicleId);
    }
}
```

The facade coordinates the subsystem. It should not become a giant class that absorbs all business logic.

## 4.4 Proxy Pattern

**Purpose:** Provide a substitute object that controls access to another object.

A proxy can add behavior before or after the real object is invoked.

Common uses:

- Access control.
- Lazy initialization.
- Caching.
- Logging.
- Transaction management.

### Spring Boot connection

Spring commonly uses proxies for features such as:

- `@Transactional`
- `@Async`
- Method-level security.
- Some caching operations.

Conceptual flow:

```text
Caller
  |
  v
Spring Proxy
  |
  | Check transaction/security/cache behavior
  v
Target Bean
  |
  v
Actual method
```

The proxy is not the business implementation itself; it intercepts calls and applies framework behavior.

### Important proxy limitation

In typical proxy-based Spring configuration, self-invocation such as `this.someTransactionalMethod()` does not pass through the proxy. Therefore, the inner method's proxy-based advice may not run as expected.

## 4.5 Structural pattern comparison

| Pattern | Main purpose |
|---|---|
| Adapter | Converts one interface into another |
| Decorator | Adds behavior by wrapping an object |
| Facade | Simplifies access to a complex subsystem |
| Proxy | Controls or intercepts access to another object |

---

# 5. Behavioral Design Patterns

Behavioral patterns focus on communication, responsibility and algorithms between objects.

## 5.1 Strategy Pattern

**Purpose:** Define a family of algorithms, encapsulate each one, and make them interchangeable.

### Scenario

A trip fare can be calculated differently depending on the type of trip.

```java
interface FareStrategy {
    double calculateFare(double distance);
}

class NormalFareStrategy implements FareStrategy {
    public double calculateFare(double distance) {
        return distance * 10;
    }
}

class PremiumFareStrategy implements FareStrategy {
    public double calculateFare(double distance) {
        return distance * 18;
    }
}
```

Context:

```java
class FareCalculator {

    private final FareStrategy strategy;

    public FareCalculator(FareStrategy strategy) {
        this.strategy = strategy;
    }

    public double calculate(double distance) {
        return strategy.calculateFare(distance);
    }
}
```

Usage:

```java
FareCalculator calculator =
        new FareCalculator(new PremiumFareStrategy());

double fare = calculator.calculate(10);
```

The caller can choose a different strategy without changing the `FareCalculator` class.

### When useful

- Payment calculation strategies.
- Pricing rules.
- Authentication methods.
- Notification channels.
- Sorting or filtering algorithms.

## 5.2 Observer Pattern

**Purpose:** Define a one-to-many relationship so that when one object changes state, registered observers are notified.

### Scenario

When a trip is assigned, multiple interested components may need to react:

- Notify the driver.
- Notify the passenger.
- Record an audit event.

A simplified Java example:

```java
interface TripObserver {
    void onTripAssigned(Long tripId);
}

class DriverNotifier implements TripObserver {
    public void onTripAssigned(Long tripId) {
        System.out.println("Notify driver for trip " + tripId);
    }
}

class PassengerNotifier implements TripObserver {
    public void onTripAssigned(Long tripId) {
        System.out.println("Notify passenger for trip " + tripId);
    }
}
```

Subject:

```java
class TripEventPublisher {

    private final List<TripObserver> observers = new ArrayList<>();

    public void addObserver(TripObserver observer) {
        observers.add(observer);
    }

    public void assignTrip(Long tripId) {
        System.out.println("Trip assigned: " + tripId);

        for (TripObserver observer : observers) {
            observer.onTripAssigned(tripId);
        }
    }
}
```

### Spring Boot connection

Spring application events provide an event-driven mechanism within an application:

- Publish an event with `ApplicationEventPublisher`.
- Listen with `@EventListener`.

Spring events are generally in-process application events. They are not a replacement for a durable distributed message broker such as Kafka.

## 5.3 Template Method Pattern

**Purpose:** Define the skeleton of an algorithm in a base class while allowing subclasses to customize selected steps.

```java
abstract class ReportGenerator {

    public final void generateReport() {
        fetchData();
        formatData();
        exportReport();
    }

    protected abstract void fetchData();

    protected void formatData() {
        System.out.println("Formatting report");
    }

    protected abstract void exportReport();
}
```

Concrete implementation:

```java
class EmployeeReportGenerator extends ReportGenerator {

    protected void fetchData() {
        System.out.println("Fetching employee data");
    }

    protected void exportReport() {
        System.out.println("Exporting employee report");
    }
}
```

The overall algorithm order is fixed by the parent class, while selected steps vary.

## 5.4 Chain of Responsibility Pattern

**Purpose:** Pass a request through a sequence of handlers. Each handler decides whether to process the request or pass it to the next handler.

### Example

An API request may pass through a sequence of checks:

```text
Incoming Request
      |
      v
Authentication Check
      |
      v
Authorization Check
      |
      v
Input Validation
      |
      v
Controller
```

A simplified handler structure:

```java
abstract class RequestHandler {

    private RequestHandler next;

    public RequestHandler setNext(RequestHandler next) {
        this.next = next;
        return next;
    }

    public void handle(String request) {
        process(request);

        if (next != null) {
            next.handle(request);
        }
    }

    protected abstract void process(String request);
}
```

This example illustrates chaining. Real handlers commonly include a condition to stop processing or reject a request.

### Spring Boot connection

Servlet filters form a chain through which HTTP requests pass. Spring Security also uses a filter chain to apply authentication and authorization behavior. These are familiar examples of chain-style processing.

## 5.5 Behavioral pattern comparison

| Pattern | Main purpose |
|---|---|
| Strategy | Makes algorithms interchangeable |
| Observer | Notifies interested subscribers when an event occurs |
| Template Method | Fixes algorithm structure while allowing selected steps to vary |
| Chain of Responsibility | Passes a request through ordered handlers |

---

# 6. Design Patterns in Spring Boot

Many Spring applications already use design patterns, even when developers do not explicitly name them.

## 6.1 Dependency Injection and Inversion of Control

Spring creates and manages beans and supplies dependencies.

```java
@Service
public class TripService {

    private final TripRepository tripRepository;

    public TripService(TripRepository tripRepository) {
        this.tripRepository = tripRepository;
    }
}
```

The service does not construct the repository itself. The Spring container manages object creation and dependency wiring.

This supports loose coupling and the Dependency Inversion Principle.

## 6.2 Repository Pattern

The Repository pattern separates persistence operations from business logic.

```java
public interface EmployeeRepository
        extends JpaRepository<Employee, Long> {
}
```

Service:

```java
@Service
public class EmployeeService {

    private final EmployeeRepository repository;

    public EmployeeService(EmployeeRepository repository) {
        this.repository = repository;
    }

    public Employee findEmployee(Long id) {
        return repository.findById(id)
                .orElseThrow(() ->
                        new EmployeeNotFoundException(
                                "Employee not found: " + id
                        )
                );
    }
}
```

Spring Data JPA can generate repository implementations at runtime from repository interfaces.

## 6.3 Proxy pattern in `@Transactional`

```java
@Service
public class TripService {

    @Transactional
    public void assignTrip() {
        // Database operations
    }
}
```

In common proxy-based transaction management, the caller invokes a Spring proxy. The proxy starts or joins a transaction, invokes the target method, and commits or rolls back according to the outcome and transaction rules.

## 6.4 Front Controller pattern

Spring MVC uses `DispatcherServlet` as a front controller.

It receives incoming requests and delegates to appropriate handler mappings and controllers.

```text
HTTP Request
     |
     v
DispatcherServlet
     |
     v
Handler Mapping
     |
     v
Controller
     |
     v
Service
     |
     v
Repository
```

## 6.5 MVC pattern

MVC stands for Model–View–Controller.

In a REST backend, the controller handles HTTP interaction, the model/domain represents application data and behavior, and the response is usually serialized as JSON rather than rendered as a server-side HTML view.

Do not assume every Spring REST application has a traditional server-rendered View component.

## 6.6 Pattern summary in Spring

| Spring feature | Related pattern or principle |
|---|---|
| Dependency injection | IoC, Dependency Inversion |
| Spring Data repository | Repository |
| `@Transactional` proxy | Proxy |
| `DispatcherServlet` | Front Controller |
| Servlet/Security filters | Chain of Responsibility style |
| Application events | Observer / publish-subscribe style |
| Bean creation/configuration | Factory-related behavior |

Patterns describe design structures; framework implementations may combine several patterns.

---

# 7. REST API Design Fundamentals

## 7.1 What is an API?

An API (Application Programming Interface) defines how two software components communicate.

For the Employee Transport application:

- Flutter is the client.
- Spring Boot exposes backend endpoints.
- PostgreSQL stores application data.
- Flutter sends HTTP requests.
- Spring Boot processes them and returns HTTP responses, commonly JSON.

```text
Flutter Mobile App
       |
       | HTTP + JSON
       v
Spring Boot REST API
       |
       v
Controller
       |
       v
Service
       |
       v
Repository / JPA
       |
       v
PostgreSQL
```

## 7.2 What is REST?

REST stands for Representational State Transfer. It is an architectural style for networked applications.

Common REST concepts include:

- Resources identified by URLs.
- Standard HTTP methods.
- Stateless request handling.
- Representations such as JSON.
- A uniform interface.

A REST-style API models things as resources, such as employees, drivers, vehicles and trips.

## 7.3 Resource-oriented URL design

Prefer nouns representing resources.

Good examples:

```http
GET    /api/employees
GET    /api/employees/42
POST   /api/employees
PUT    /api/employees/42
PATCH  /api/employees/42
DELETE /api/employees/42
```

Avoid unnecessarily verb-heavy endpoints when the operation maps naturally to standard HTTP methods.

Some business actions do not map neatly to CRUD. An explicit action endpoint may be reasonable when it communicates a domain command clearly, for example:

```http
POST /api/trips/100/confirm
```

The goal is consistency and clarity, not mechanically removing every verb.

## 7.4 HTTP methods

| Method | Typical purpose | Safe? | Idempotent? |
|---|---|---:|---:|
| GET | Retrieve a resource | Yes | Yes |
| POST | Create or execute an operation | No | Not generally |
| PUT | Replace a resource representation | No | Yes |
| PATCH | Partially modify a resource | No | Not guaranteed |
| DELETE | Remove a resource | No | Yes |

**Safe** means the method is intended not to change server state.

**Idempotent** means repeating the same request has the same intended effect as making it once. A DELETE may return a different status on a repeated call, while remaining idempotent in its intended effect.

## 7.5 GET example

Request:

```http
GET /api/employees/42
```

Possible response:

```json
{
  "id": 42,
  "name": "Anand",
  "email": "anand@example.com",
  "active": true
}
```

GET should retrieve information and should not be used to perform state-changing operations.

## 7.6 POST example

Request:

```http
POST /api/employees
Content-Type: application/json
```

Body:

```json
{
  "name": "Anand",
  "email": "anand@example.com"
}
```

A successful creation commonly returns HTTP 201 Created and may include the created representation and a `Location` header identifying the new resource.

## 7.7 PUT vs PATCH

PUT generally represents replacement of the target resource representation.

PATCH represents partial modification.

Example PATCH request:

```http
PATCH /api/employees/42
Content-Type: application/json
```

```json
{
  "active": false
}
```

The API must define how omitted fields and null values are interpreted. PATCH does not automatically define one universal JSON update format.

## 7.8 HTTP status codes

| Status | Meaning | Example |
|---:|---|---|
| 200 | OK | Employee retrieved |
| 201 | Created | Employee created |
| 202 | Accepted | Work accepted for asynchronous processing |
| 204 | No Content | Successful operation with no response body |
| 400 | Bad Request | Invalid request data |
| 401 | Unauthorized | Missing or invalid authentication |
| 403 | Forbidden | User lacks permission |
| 404 | Not Found | Employee does not exist |
| 409 | Conflict | Duplicate or conflicting trip assignment |
| 422 | Unprocessable Content | Semantically invalid content, if used by the API contract |
| 429 | Too Many Requests | Rate limit exceeded |
| 500 | Internal Server Error | Unexpected server failure |
| 502 | Bad Gateway | Invalid response from an upstream service |
| 503 | Service Unavailable | Service temporarily unavailable |
| 504 | Gateway Timeout | Upstream response timed out |

Choose statuses consistently and document them. Not every API needs to use every status code.

## 7.9 Statelessness

In a stateless REST interaction, each request contains the information needed to process it. The server does not rely on hidden conversational session state to understand the request.

For token-based authentication, a client commonly sends its access token with each protected request:

```http
Authorization: Bearer <access-token>
```

The server still stores application data and may store token revocation or other state. Statelessness does not mean the entire system has no database or state.

## 7.10 JSON request and response DTOs

Do not expose persistence entities directly as the public API contract without considering the consequences.

Request DTO:

```java
public class CreateEmployeeRequest {

    @NotBlank
    private String name;

    @Email
    private String email;

    // Getters and setters
}
```

Response DTO:

```java
public record EmployeeResponse(
        Long id,
        String name,
        String email,
        boolean active
) {
}
```

Controller:

```java
@PostMapping
public ResponseEntity<EmployeeResponse> create(
        @Valid @RequestBody CreateEmployeeRequest request) {

    EmployeeResponse response = employeeService.create(request);

    return ResponseEntity
            .status(HttpStatus.CREATED)
            .body(response);
}
```

Benefits of DTOs:

- Control which fields clients can submit.
- Avoid exposing internal entity fields.
- Prevent accidental exposure of passwords or sensitive data.
- Keep API contracts separate from database schema.
- Validate request data at the boundary.

---

# 8. Practical API Design and Production Considerations

## 8.1 Layered API architecture

A common Spring Boot request flow is:

```text
Flutter Client
     |
     | HTTP request / JSON
     v
Controller
     |
     | Request DTO
     v
Service
     |
     | Business rules
     v
Repository
     |
     | JPA / Hibernate
     v
PostgreSQL
     |
     | Entity / query result
     v
Service
     |
     | Response DTO
     v
Controller
     |
     | HTTP response / JSON
     v
Flutter Client
```

Responsibilities:

| Layer | Responsibility |
|---|---|
| Controller | HTTP mapping, request binding, response status |
| DTO | API request and response structure |
| Service | Business rules and use-case coordination |
| Repository | Data access |
| Entity | Persistence/domain representation |
| Database | Durable data storage |

## 8.2 Pagination

Returning thousands of rows in one response can create high memory use, slow queries and slow network transfers.

Use pagination for large collections.

Example request:

```http
GET /api/employees?page=0&size=20
```

Spring Data example:

```java
@GetMapping
public Page<EmployeeResponse> getEmployees(
        @RequestParam(defaultValue = "0") int page,
        @RequestParam(defaultValue = "20") int size) {

    Pageable pageable = PageRequest.of(page, size);

    return employeeService.getEmployees(pageable);
}
```

Service:

```java
public Page<EmployeeResponse> getEmployees(Pageable pageable) {
    return employeeRepository.findAll(pageable)
            .map(employee -> new EmployeeResponse(
                    employee.getId(),
                    employee.getName(),
                    employee.getEmail(),
                    employee.isActive()
            ));
}
```

Production considerations:

- Validate page and size.
- Enforce a maximum page size.
- Use deterministic sorting.
- Consider `Slice` when a total count is unnecessary.
- Consider keyset pagination for large, frequently changing datasets.

## 8.3 Sorting

Example:

```http
GET /api/employees?sort=name,asc
```

Spring Data:

```java
Sort sort = Sort.by(Sort.Direction.ASC, "name");
Pageable pageable = PageRequest.of(0, 20, sort);
```

Never pass arbitrary user-provided text directly into SQL. Whitelist supported sort fields and directions.

## 8.4 Filtering and searching

Example:

```http
GET /api/trips?status=ASSIGNED&shift=MORNING
```

Use query parameters for optional filters.

For more complex search requirements, define a clear request DTO or search specification rather than creating an endpoint for every possible filter combination.

## 8.5 API versioning

Versioning allows an API to evolve while existing clients continue using a compatible contract.

A common URL versioning approach:

```http
/api/v1/employees
/api/v2/employees
```

Other approaches include custom request headers and media-type versioning.

Version when a change is incompatible, such as removing or renaming fields or changing their meaning. Additive optional fields are often backward-compatible if clients tolerate unknown fields.

Have a deprecation and migration policy for older versions.

## 8.6 Idempotency

Idempotency is important when clients retry requests because of network failures.

For example, a Flutter client sends a trip-booking request. The server completes the operation, but the response is lost. The client retries. Without protection, the server might create two trips.

An idempotency key can identify repeated attempts belonging to the same logical operation:

```http
POST /api/trips
Idempotency-Key: 8e03978e-40d5-43e8-bc93-6894a57f9324
```

A robust implementation generally needs to:

1. Receive the key.
2. Associate it with the authenticated client and operation.
3. Store the key and operation result safely.
4. Enforce uniqueness for the relevant scope.
5. Return the stored result for a duplicate request.
6. Define key expiry and behavior when the same key is reused with a different payload.

An idempotency header alone does not make an endpoint idempotent; the server must implement the behavior.

## 8.7 API authentication and authorization

Authentication answers: **Who is the caller?**

Authorization answers: **What is the caller allowed to do?**

Example:

```http
GET /api/trips
Authorization: Bearer <access-token>
```

A driver may be allowed to see assigned trips, while a company administrator may be allowed to manage all trips.

Enforce authorization on the server. Hiding a button in Flutter is not a security boundary.

## 8.8 CORS

Cross-Origin Resource Sharing is a browser security mechanism controlling whether browser-based JavaScript can access resources from another origin.

It is particularly relevant to browser clients such as Flutter Web. Native Flutter mobile applications are not governed by browser CORS enforcement in the same way.

CORS is not authentication and does not replace authorization.

## 8.9 API documentation with OpenAPI

OpenAPI describes an HTTP API in a machine-readable format.

Documentation can describe:

- Endpoints and methods.
- Path and query parameters.
- Request body schemas.
- Response schemas.
- Authentication requirements.
- Error responses.

Spring projects commonly integrate OpenAPI documentation through a compatible library such as springdoc-openapi. Verify compatibility with the Spring Boot version used by the project.

Good API documentation helps frontend and backend developers agree on the request and response contract.

## 8.10 API testing

Common API testing tools include:

- Postman.
- Insomnia.
- `curl`.
- Automated integration tests using Spring Boot test support.

Test more than the happy path:

- Valid request.
- Invalid request body.
- Missing resource.
- Unauthenticated request.
- Authenticated user without permission.
- Duplicate operation.
- Database or dependency failure.
- Pagination boundaries.

For this learning plan, focus on understanding the design and interview reasoning; do not repeat endpoint testing that has already been completed for the Employee Transport project.

## 8.11 Common API design mistakes

- Using GET for state-changing operations.
- Returning database entities without considering exposure or coupling.
- Returning 200 for every outcome.
- Returning stack traces to clients.
- Creating inconsistent response formats.
- Not validating request DTOs.
- Returning unbounded database results.
- Allowing arbitrary sort fields.
- Failing to enforce authorization on the backend.
- Retrying non-idempotent operations without a duplicate-protection strategy.
- Creating a new endpoint for every minor filter combination.
- Changing response fields without considering existing mobile app versions.

---

# 9. Interview Questions and Answers

## SOLID

### Q1. What are SOLID principles?

SOLID is a set of five object-oriented design principles: Single Responsibility, Open/Closed, Liskov Substitution, Interface Segregation and Dependency Inversion. They help improve maintainability, extensibility and testability.

### Q2. Explain Single Responsibility Principle.

A class should have one cohesive responsibility and one primary reason to change. For example, employee persistence, report generation and email delivery should not all be mixed into one class.

### Q3. Explain Open/Closed Principle.

A software component should be designed so new behavior can often be added through extension rather than repeatedly modifying stable existing code.

### Q4. Explain Liskov Substitution Principle.

An implementation of an abstraction must preserve the behavioral expectations of that abstraction. A caller should not encounter unexpected failures simply because a valid subtype was substituted.

### Q5. Explain Interface Segregation Principle.

Clients should not be forced to depend on methods they do not use. Prefer small, focused interfaces over large interfaces containing unrelated responsibilities.

### Q6. Explain Dependency Inversion Principle.

High-level modules should depend on abstractions rather than concrete low-level implementations. Both should depend on abstractions.

### Q7. Is dependency injection the same as dependency inversion?

No. Dependency inversion is a design principle. Dependency injection is a technique for supplying dependencies from outside an object. DI is commonly used to implement DIP.

## Design patterns

### Q8. What is a design pattern?

A design pattern is a reusable approach to a recurring software design problem. It describes a solution structure rather than a fixed piece of code.

### Q9. What are the three categories of design patterns?

Creational patterns manage object creation, structural patterns organize object composition, and behavioral patterns organize communication and responsibilities.

### Q10. What is Singleton?

Singleton aims to provide one shared instance of a class. Spring's default singleton bean scope provides one bean instance per IoC container, but does not automatically make mutable bean state thread-safe.

### Q11. What is Factory Method?

Factory Method defines a creation method through a creator abstraction, allowing concrete creator implementations to decide which product to instantiate. A simple static factory helper is related but is not the formal GoF Factory Method pattern.

### Q12. What is Builder?

Builder constructs an object step by step. It is useful when an object has many optional parameters or when construction needs validation.

### Q13. What is Adapter?

Adapter converts one interface into another interface expected by a client. It is useful when integrating incompatible third-party or legacy components.

### Q14. What is Decorator?

Decorator wraps an object to add behavior while preserving the same interface. It avoids creating many subclasses for combinations of optional behavior.

### Q15. What is Facade?

Facade provides a simpler entry point to a complex subsystem. It hides coordination details from callers.

### Q16. What is Proxy?

Proxy is a substitute object that controls or intercepts access to another object. Spring uses proxies for common features such as declarative transactions.

### Q17. What is Strategy?

Strategy encapsulates interchangeable algorithms behind a common interface. A caller can select a different algorithm without changing the context class.

### Q18. What is Observer?

Observer lets interested subscribers receive notifications when a subject publishes a change or event. Spring application events provide an in-process event mechanism.

### Q19. What is Chain of Responsibility?

It passes a request through a sequence of handlers. Servlet filters and Spring Security filter chains use a similar chain-processing structure.

### Q20. Which design patterns are visible in Spring Boot?

Examples include Repository, Proxy, Front Controller, dependency injection/IoC, event publication and filter-chain processing.

## REST API design

### Q21. What is REST?

REST is an architectural style based on constraints such as resource identification, a uniform interface, stateless interactions and representations of resources.

### Q22. What is the difference between PUT and PATCH?

PUT generally replaces the target resource representation. PATCH applies a partial modification. The API must define the exact update semantics.

### Q23. What is the difference between POST and PUT?

POST commonly submits data for resource creation or processing and is not generally idempotent. PUT generally replaces a resource at a known URI and is defined to be idempotent.

### Q24. What does idempotent mean?

Repeating the same operation has the same intended effect as performing it once. GET, PUT and DELETE are defined as idempotent methods; POST is not generally idempotent.

### Q25. Why use DTOs?

DTOs define the API contract separately from persistence entities. They help control input, output, validation and sensitive-field exposure.

### Q26. Why use pagination?

Pagination limits the amount of data returned per request, improving response size, memory usage and query behavior for large collections.

### Q27. What is API versioning?

API versioning allows incompatible API changes to be introduced while preserving older contracts for existing clients during migration.

### Q28. What is an idempotency key?

It is a client-supplied identifier for a logical operation. The server stores and recognizes repeated attempts so retries do not accidentally create duplicate operations.

### Q29. What is CORS?

CORS is a browser-enforced mechanism that controls whether web pages from one origin can access resources from another origin. It is not an authentication or authorization mechanism.

### Q30. How do you secure a REST API?

Use authentication, server-side authorization, HTTPS, input validation, safe error responses, secure token handling, rate limiting where appropriate, and careful logging that does not expose secrets.

---

# 10. Quick Revision Sheet

| Concept | Remember |
|---|---|
| SOLID | Five object-oriented design principles |
| SRP | One cohesive responsibility |
| OCP | Extend behavior without repeatedly modifying stable code |
| LSP | Subtypes preserve behavioral contracts |
| ISP | Small, focused interfaces |
| DIP | Depend on abstractions |
| Design pattern | Reusable solution structure for a recurring design problem |
| Creational | Object creation |
| Structural | Object composition |
| Behavioral | Object collaboration |
| Singleton | One shared instance within its intended scope |
| Factory Method | Delegate product creation to creator implementations |
| Builder | Step-by-step object construction |
| Abstract Factory | Create families of related objects |
| Adapter | Convert an incompatible interface |
| Decorator | Add behavior by wrapping |
| Facade | Simplify access to a subsystem |
| Proxy | Control or intercept access |
| Strategy | Interchangeable algorithms |
| Observer | Notify subscribers of events |
| Template Method | Fixed algorithm skeleton with customizable steps |
| Chain of Responsibility | Pass request through handlers |
| Repository | Separate persistence access |
| Front Controller | Central request entry point |
| REST | Resource-oriented architectural style |
| GET | Retrieve |
| POST | Create/process |
| PUT | Replace |
| PATCH | Partially modify |
| DELETE | Delete |
| DTO | API data contract |
| Pagination | Limit collection size |
| Versioning | Manage incompatible API evolution |
| Idempotency | Repeated operation has the same intended effect |
| CORS | Browser cross-origin access policy |
| OpenAPI | Machine-readable API description |

---

# 11. Day 15 Completion Checklist

- [ ] I can explain all five SOLID principles.
- [ ] I can identify SRP and ISP violations.
- [ ] I can explain OCP with an extensible implementation.
- [ ] I understand behavioral compatibility in LSP.
- [ ] I can explain DIP and how Spring DI supports it.
- [ ] I can define a design pattern.
- [ ] I can classify creational, structural and behavioral patterns.
- [ ] I understand Singleton, Factory Method, Builder and Abstract Factory.
- [ ] I understand Adapter, Decorator, Facade and Proxy.
- [ ] I understand Strategy, Observer, Template Method and Chain of Responsibility.
- [ ] I can identify patterns used by Spring Boot.
- [ ] I can explain REST and resource-oriented URL design.
- [ ] I understand GET, POST, PUT, PATCH and DELETE.
- [ ] I can distinguish safe and idempotent HTTP methods.
- [ ] I understand HTTP status codes.
- [ ] I can explain why DTOs are useful.
- [ ] I understand pagination, sorting and filtering.
- [ ] I can explain API versioning and idempotency keys.
- [ ] I understand authentication, authorization and CORS.
- [ ] I can describe the Flutter-to-Spring-Boot request flow.

---

# Day 15 Takeaway

SOLID principles help structure object-oriented code so responsibilities and dependencies remain manageable. Design patterns provide proven structures for object creation, composition and collaboration. REST API design defines a clear contract between clients and backend services.

In a Java backend interview, do not only memorize pattern definitions. Be ready to explain the problem each pattern solves, show a small Java example, describe its trade-offs and identify where it appears in Spring Boot.

**End of Day 15 notes.**
'''

out = Path("/mnt/data/Backend-notes (Day 15).md")
out.write_text(content, encoding="utf-8")
print(f"Created: {out}")
print(f"Words: {len(content.split()):,}")
print(f"Lines: {len(content.splitlines()):,}")
print(f"Characters: {len(content):,}")
