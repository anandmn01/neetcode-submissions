content = r'''# Day 1 — Spring Boot Fundamentals

## Day 1 Topic Index

1. What is Spring Boot?
2. Why do we use Spring Boot?
3. Spring Boot architecture
4. Inversion of Control (IoC)
5. Dependency Injection (DI)
6. Spring Beans
7. `@Component` vs `@Bean`
8. Constructor, Setter and Field Injection
9. Spring Bean Lifecycle
10. Bean Scopes
11. Important Spring Boot annotations

---

## 1. What is Spring Boot?

Before understanding Spring Boot, let's understand the problem it solves.

Java is a programming language. We can use Java to develop backend applications, but building a complete backend requires more than writing Java classes.

For example, imagine you're developing an Employee Management application. It needs to:

- Receive requests from a Flutter mobile application.
- Process employee information.
- Apply business rules.
- Connect to PostgreSQL.
- Authenticate users.
- Return responses in JSON format.

Java provides the language and core libraries, but developers also need frameworks and infrastructure to organize these tasks.

### What is the Spring Framework?

Spring Framework provides infrastructure for developing enterprise Java applications.

It provides features such as:

- Dependency Injection
- Inversion of Control
- Transaction management
- Database integration
- Web application development
- Application configuration

However, traditional Spring applications can require substantial manual configuration.

### What is Spring Boot?

Spring Boot is built on top of the Spring Framework. It simplifies application development by providing sensible defaults and reducing the amount of manual configuration.

For example, a Spring Boot application can start an embedded web server and expose REST APIs without requiring you to install and configure an external Tomcat server separately.

A simple Spring Boot application:

```java
@SpringBootApplication
public class EmployeeApplication {

    public static void main(String[] args) {
        SpringApplication.run(
            EmployeeApplication.class,
            args
        );
    }
}
```

Let's understand these lines:

| Code | Meaning |
|---|---|
| `@SpringBootApplication` | Marks the main Spring Boot configuration class |
| `public static void main` | Java application's entry point |
| `SpringApplication.run()` | Bootstraps the Spring application |

When you execute this application, Spring Boot prepares the application context, configures required components and starts the embedded server when the application is configured as a web application.

### Spring Framework vs Spring Boot

| Spring Framework | Spring Boot |
|---|---|
| Provides core application infrastructure | Simplifies application development using Spring |
| More configuration may be required | Provides auto-configuration |
| Server setup may be external | Supports embedded servers |
| Dependencies can be configured individually | Provides starter dependencies |
| Flexible but may involve more setup | Offers sensible defaults and faster setup |

**Important interview answer:**

Spring Boot is not a replacement for the Spring Framework. It builds on top of Spring and makes application development easier through auto-configuration, starter dependencies and embedded servers.

---

## 2. Why do we use Spring Boot?

Let's understand the practical reasons.

### A. Auto-configuration

Spring Boot attempts to configure application components based on the dependencies present on the classpath and the application's configuration.

For example, suppose you add the Spring Boot web starter.

Spring Boot can configure:

- Web application infrastructure
- Spring MVC components
- HTTP message converters
- An embedded servlet web server

You don't have to manually configure every component.

**Important:** Auto-configuration is conditional. It considers existing beans, classpath dependencies and application properties. It doesn't blindly configure everything.

### B. Starter dependencies

A starter is a convenient dependency bundle for a particular application capability.

For example, instead of individually managing all dependencies required for a Spring MVC application, you can use:

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```

Other commonly used starters:

| Starter | Purpose |
|---|---|
| `spring-boot-starter-web` | Web applications and REST APIs |
| `spring-boot-starter-data-jpa` | JPA-based database access |
| `spring-boot-starter-security` | Authentication and authorization |
| `spring-boot-starter-validation` | Request validation |
| `spring-boot-starter-test` | Testing support |

Starters simplify dependency management. Spring Boot's dependency management also provides compatible dependency versions.

### C. Embedded server

A traditional deployment approach might require:

1. Installing an application server.
2. Configuring the server.
3. Packaging the application.
4. Deploying the application to the server.

With a typical Spring Boot web application, an embedded server such as Tomcat can run inside the application process.

For example:

```bash
mvn spring-boot:run
```

Or, after packaging:

```bash
java -jar employee-backend.jar
```

The application can start its embedded server automatically.

### D. Production-ready features

Spring Boot provides production support through features such as Actuator.

Examples include:

- Health checks
- Application metrics
- Environment information
- Application monitoring

These endpoints must be configured and secured appropriately before production exposure.

### E. Simplified configuration

Spring Boot supports configuration through:

- `application.properties`
- `application.yml`
- Environment variables
- Command-line arguments
- External configuration sources

Example:

```yaml
server:
  port: 8080

spring:
  application:
    name: employee-transport
```

Here, the application name and server port are configured without hardcoding them in Java classes.

### When would we use Spring Boot?

Consider an Employee Transport application:

```text
Flutter Mobile App
        |
        | HTTP / JSON
        v
Spring Boot Backend
        |
        | Business Logic
        v
PostgreSQL Database
```

Spring Boot handles the backend infrastructure, while your Java code implements the application's business rules.

**Interview answer:**

We use Spring Boot because it reduces boilerplate configuration, simplifies dependency management, supports embedded servers and provides production-ready capabilities. It helps developers build REST APIs and enterprise applications more efficiently.

---

## 3. Spring Boot Architecture

Now let's understand how a backend application is organized.

A common Spring Boot application uses a layered architecture.

### A. Controller Layer

The controller is responsible for handling incoming HTTP requests.

Its responsibilities include:

- Receiving API requests.
- Reading request parameters and request bodies.
- Calling the appropriate service methods.
- Returning HTTP responses.

Example:

```java
@RestController
@RequestMapping("/api/employees")
public class EmployeeController {

    private final EmployeeService employeeService;

    public EmployeeController(
            EmployeeService employeeService) {
        this.employeeService = employeeService;
    }

    @GetMapping
    public List<Employee> getEmployees() {
        return employeeService.getAllEmployees();
    }
}
```

Here:

- `@RestController` identifies a REST controller.
- `@RequestMapping` defines the common URL prefix.
- `@GetMapping` maps an HTTP GET request.
- The controller delegates the work to EmployeeService.

The controller should generally not contain complex business logic.

### B. Service Layer

The service layer contains the business logic of the application.

Example:

```java
@Service
public class EmployeeService {

    private final EmployeeRepository employeeRepository;

    public EmployeeService(
            EmployeeRepository employeeRepository) {
        this.employeeRepository = employeeRepository;
    }

    public List<Employee> getAllEmployees() {
        return employeeRepository.findAll();
    }
}
```

Imagine a requirement:

"Only active employees should be assigned to a trip."

This business rule belongs in the service layer, not in the controller or repository.

### C. Repository Layer

The repository layer handles database access.

Spring Data JPA allows us to define repository interfaces.

```java
@Repository
public interface EmployeeRepository
        extends JpaRepository<Employee, Long> {

}
```

`JpaRepository` provides commonly used methods:

```java
save()
findById()
findAll()
deleteById()
existsById()
count()
```

Spring Data JPA provides the implementation at runtime through its repository infrastructure.

### D. Entity Layer

An entity represents a database-managed object.

Example:

```java
@Entity
@Table(name = "employees")
public class Employee {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String name;

    private String email;
}
```

Here:

- `@Entity` marks the class as a JPA entity.
- `@Table` specifies the database table.
- `@Id` identifies the primary key.
- `@GeneratedValue` configures primary-key generation.

### E. DTO Layer

DTO means Data Transfer Object.

It carries data between application boundaries, particularly between the API and its clients.

Why not return database entities directly?

Because an entity may contain internal fields that should not be exposed to clients.

For example, an Employee entity might contain:

```java
private String passwordHash;
private String email;
private String phone;
```

But the API might only need to return:

```java
public class EmployeeResponse {

    private Long id;
    private String name;
    private String email;
}
```

This gives us control over what information is exposed.

### Complete layered architecture

```text
             FLUTTER CLIENT
                   |
                   | HTTP Request
                   v
          EMPLOYEE CONTROLLER
                   |
                   | Calls service
                   v
           EMPLOYEE SERVICE
                   |
                   | Business logic
                   v
         EMPLOYEE REPOSITORY
                   |
                   | Spring Data JPA
                   v
              HIBERNATE
                   |
                   | JDBC
                   v
              POSTGRESQL
```

### Complete request example

Suppose the mobile application sends:

```http
GET /api/employees
```

The flow is:

1. Flutter sends the HTTP request.
2. EmployeeController receives the request.
3. EmployeeController calls EmployeeService.
4. EmployeeService calls EmployeeRepository.
5. Repository infrastructure uses Hibernate/JPA to access the database.
6. PostgreSQL returns the records.
7. The application converts the data into the required response.
8. Spring returns the HTTP response to Flutter.

### Responsibilities: important comparison

| Layer | What it should do | What it should avoid |
|---|---|---|
| Controller | Handle HTTP requests and responses | Complex business logic |
| Service | Apply business rules | Directly handling HTTP details |
| Repository | Database operations | Managing API response formats |
| Entity | Represent persisted data | Acting as the public API contract |
| DTO | Transfer required data | Containing database persistence logic |

**Interview answer:**

In a layered Spring Boot architecture, the controller handles HTTP requests, the service layer contains business logic, the repository layer performs database operations, entities represent persisted data and DTOs control data transfer between application boundaries.

---

## 4. Inversion of Control (IoC)

This is one of the most important concepts in Spring.

Before learning Dependency Injection, we need to understand IoC.

### First understand the problem

Suppose we have two classes:

```java
public class EmployeeService {

    public void getEmployees() {
        System.out.println("Getting employees");
    }
}
```

```java
public class EmployeeController {

    private EmployeeService service =
            new EmployeeService();

    public void getEmployees() {
        service.getEmployees();
    }
}
```

Here, EmployeeController creates EmployeeService itself.

This is called direct object creation.

What problems can arise?

1. The controller is tightly coupled to the concrete implementation.
2. Replacing the service implementation requires changing controller code.
3. Unit testing becomes more difficult.
4. Object creation and lifecycle management are spread across classes.

### What is IoC?

Inversion of Control means transferring the control of object creation and dependency management from application classes to an external container.

In Spring Boot, this container is the Spring IoC container.

Instead of the controller creating the service, Spring creates the service and supplies it to the controller.

### Without IoC vs With IoC

**Without IoC:**

```text
EmployeeController
        |
        | Creates object using new
        v
EmployeeService
```

**With Spring IoC:**

```text
           SPRING CONTAINER
                  |
          Creates and manages
                  |
          EmployeeService
                  |
          Injects dependency
                  |
          EmployeeController
```

### Example using Spring IoC

First, define the service:

```java
@Service
public class EmployeeService {

    public String getMessage() {
        return "Employee service is working";
    }
}
```

Then define the controller:

```java
@RestController
public class EmployeeController {

    private final EmployeeService employeeService;

    public EmployeeController(
            EmployeeService employeeService) {
        this.employeeService = employeeService;
    }

    @GetMapping("/message")
    public String getMessage() {
        return employeeService.getMessage();
    }
}
```

What happens?

1. Spring scans the application packages.
2. It discovers EmployeeService because of `@Service`.
3. It registers EmployeeService as a bean.
4. It discovers EmployeeController because of `@RestController`.
5. It identifies the constructor dependency.
6. It resolves the EmployeeService bean.
7. It creates EmployeeController with that dependency.

The controller doesn't manually create the service object.

### What is the Spring IoC Container?

The Spring IoC container is responsible for:

- Creating beans.
- Configuring beans.
- Injecting dependencies.
- Managing bean lifecycle.
- Destroying beans when appropriate.

Two important container interfaces are:

| Interface | Explanation |
|---|---|
| `BeanFactory` | Basic IoC container functionality |
| `ApplicationContext` | Extends BeanFactory with additional enterprise application features |

We will study ApplicationContext and the application startup process in more detail on Day 2.

### Interview questions

**Q1. What is Inversion of Control?**

IoC is a design principle where object creation and dependency management are transferred from application code to an external container such as the Spring IoC container.

**Q2. Why is IoC useful?**

It reduces tight coupling, centralizes object management and makes applications easier to test and maintain.

**Q3. What is the Spring IoC container?**

It is the part of Spring responsible for creating, configuring and managing beans and their dependencies.

**Q4. Is IoC the same as Dependency Injection?**

No. IoC is the broader design principle. Dependency Injection is one technique used to implement IoC.

---

## 5. Dependency Injection (DI)

In the previous topic, we learned that IoC transfers object management to the Spring container.

Now let's understand how Spring supplies one object's dependencies to another object.

### What is Dependency Injection?

Dependency Injection is a technique in which an object receives the dependencies it needs from an external source instead of creating those dependencies itself.

For example, an EmployeeService needs EmployeeRepository to access employee records.

Instead of writing:

```java
public class EmployeeService {

    private EmployeeRepository repository =
            new EmployeeRepository();
}
```

We allow Spring to provide the repository.

```java
@Service
public class EmployeeService {

    private final EmployeeRepository repository;

    public EmployeeService(EmployeeRepository repository) {
        this.repository = repository;
    }
}
```

Here:

- EmployeeService depends on EmployeeRepository.
- EmployeeRepository is its dependency.
- Spring provides the dependency through the constructor.
- This is Dependency Injection.

### Types of Dependency Injection

Spring supports three commonly discussed injection approaches:

1. Constructor Injection
2. Setter Injection
3. Field Injection

### A. Constructor Injection

Dependencies are supplied through the class constructor.

```java
@Service
public class EmployeeService {

    private final EmployeeRepository repository;

    public EmployeeService(EmployeeRepository repository) {
        this.repository = repository;
    }
}
```

Spring automatically uses the constructor when the class has a single constructor. In this case, `@Autowired` is not required.

**How does it work?**

```text
Spring Container
       |
       v
Creates EmployeeRepository
       |
       v
Creates EmployeeService
       |
       v
Passes Repository to Constructor
       |
       v
EmployeeService Ready
```

Advantages:

- Dependencies are explicit.
- Supports immutable dependency references using `final`.
- Easier to write unit tests.
- Required dependencies are available when the object is constructed.
- Helps avoid partially initialized objects.

### B. Setter Injection

Dependencies are provided through setter methods.

```java
@Service
public class EmployeeService {

    private EmployeeRepository repository;

    @Autowired
    public void setRepository(EmployeeRepository repository) {
        this.repository = repository;
    }
}
```

Here, Spring creates the object and then calls the setter to provide the dependency.

**When is it useful?**

Setter injection can be useful for optional dependencies or dependencies that need to be changed after object construction.

### C. Field Injection

Dependencies are injected directly into class fields.

```java
@Service
public class EmployeeService {

    @Autowired
    private EmployeeRepository repository;
}
```

Spring injects the dependency into the field.

**Why is it generally discouraged?**

- Dependencies are less visible.
- It is harder to create the class independently in unit tests.
- The field cannot be declared `final` for normal field injection.
- It can hide problems with class design.

### Comparison

| Feature | Constructor | Setter | Field |
|---|---|---|---|
| Injection mechanism | Constructor | Setter method | Direct field |
| Required dependency | Excellent | Less suitable | Less suitable |
| Supports final field | Yes | No | No |
| Unit testing | Easy | Easy | More difficult |
| General recommendation | Preferred | Optional dependencies | Generally avoid |

**Interview question: Which Dependency Injection is recommended in Spring Boot?**

Constructor Injection is generally recommended because it makes dependencies explicit, supports immutability and improves testability.

---

## 6. Spring Beans

Now let's understand what Spring manages through its IoC container.

### What is a Spring Bean?

A Spring Bean is an object that is created, configured and managed by the Spring IoC container.

Normally, in Java, we create objects using:

```java
EmployeeService service = new EmployeeService();
```

In Spring, we can register a class as a bean:

```java
@Service
public class EmployeeService {

    public void getEmployees() {
        System.out.println("Getting employees");
    }
}
```

Spring discovers this class through component scanning and registers it as a bean.

### What does Spring manage?

Spring manages several aspects of beans:

1. Object creation.
2. Dependency injection.
3. Initialization callbacks.
4. Bean scope.
5. Bean destruction callbacks.

### Common Spring Bean Annotations

| Annotation | Purpose |
|---|---|
| `@Component` | General-purpose Spring-managed component |
| `@Service` | Business logic component |
| `@Repository` | Data access component |
| `@Controller` | Spring MVC controller |
| `@RestController` | REST API controller |
| `@Configuration` | Configuration class |
| `@Bean` | Registers an object returned by a configuration method |

### Understanding the specialized annotations

**@Component**

Used for general-purpose Spring-managed classes.

```java
@Component
public class EmailUtility {

    public void sendEmail() {
        System.out.println("Email sent");
    }
}
```

**@Service**

Used to indicate a service or business logic component.

```java
@Service
public class EmployeeService {

    public void createEmployee() {
        System.out.println("Employee created");
    }
}
```

**@Repository**

Used to indicate a database access component.

```java
@Repository
public class EmployeeRepository {
}
```

In Spring's persistence integration, `@Repository` also supports translation of certain persistence exceptions into Spring's `DataAccessException` hierarchy.

**@Controller**

Used for Spring MVC controllers, commonly returning views.

```java
@Controller
public class HomeController {

    @GetMapping("/home")
    public String home() {
        return "home";
    }
}
```

**@RestController**

Used for REST APIs. It combines `@Controller` and `@ResponseBody`.

```java
@RestController
public class EmployeeController {

    @GetMapping("/employee")
    public String employee() {
        return "Employee details";
    }
}
```

### Important relationship

```text
              @Component
                  |
        -----------------------
        |          |          |
     @Service  @Repository  @Controller
```

These specialized annotations are meta-annotated with `@Component`.

`@RestController` is itself a composed annotation based on `@Controller` and `@ResponseBody`.

**Interview question: What is the difference between @Component, @Service and @Repository?**

All three can register classes as Spring beans through component scanning. They communicate different roles:

- `@Component`: General component.
- `@Service`: Business logic.
- `@Repository`: Data access.

---

## 7. @Component vs @Bean

This is a frequently asked Spring Boot interview question.

Both annotations help register objects as Spring beans, but they work differently.

### A. @Component

`@Component` is applied directly to a class.

```java
@Component
public class EmailService {

    public void sendEmail() {
        System.out.println("Sending email");
    }
}
```

Spring discovers this class through component scanning.

### B. @Bean

`@Bean` is applied to a method inside a configuration class.

```java
@Configuration
public class AppConfig {

    @Bean
    public EmailService emailService() {
        return new EmailService();
    }
}
```

Here:

1. Spring discovers the configuration class.
2. Spring processes the `@Bean` method.
3. The method returns an EmailService object.
4. Spring registers that returned object as a bean.

### Why do we need @Bean?

Sometimes we need to register a class that we cannot annotate ourselves.

For example, a third-party library may provide a class that does not contain Spring annotations.

We can configure it using `@Bean`.

### Comparison

| Feature | @Component | @Bean |
|---|---|---|
| Applied to | Class | Method |
| Registration | Component scanning | Configuration method |
| Object creation | Spring discovers and instantiates the class | Developer writes the creation method |
| Typical use | Application classes | Third-party classes or explicit configuration |

### Practical example

Suppose an external library provides an object called `ExternalEmailClient`.

You cannot modify its source code to add `@Component`.

You can register it like this:

```java
@Configuration
public class EmailConfiguration {

    @Bean
    public ExternalEmailClient emailClient() {
        return new ExternalEmailClient();
    }
}
```

### Interview answer

`@Component` is used to register a class through component scanning, whereas `@Bean` is used to explicitly register the object returned by a method inside a configuration class.

---

## 8. Constructor Injection in Detail

We have already studied the three injection approaches. Now let's understand why constructor injection is preferred in real applications.

Consider the following architecture:

```text
EmployeeController
        |
        v
EmployeeService
        |
        v
EmployeeRepository
```

The controller needs the service, and the service needs the repository.

### Step 1: Create Repository

```java
@Repository
public class EmployeeRepository {

    public String getEmployeeName() {
        return "Rahul";
    }
}
```

### Step 2: Create Service

```java
@Service
public class EmployeeService {

    private final EmployeeRepository repository;

    public EmployeeService(EmployeeRepository repository) {
        this.repository = repository;
    }

    public String getEmployeeName() {
        return repository.getEmployeeName();
    }
}
```

### Step 3: Create Controller

```java
@RestController
@RequestMapping("/api/employees")
public class EmployeeController {

    private final EmployeeService service;

    public EmployeeController(EmployeeService service) {
        this.service = service;
    }

    @GetMapping("/name")
    public String getEmployeeName() {
        return service.getEmployeeName();
    }
}
```

### What happens internally?

When Spring starts:

1. It discovers EmployeeRepository.
2. It creates the repository bean.
3. It discovers EmployeeService.
4. It sees that EmployeeService needs EmployeeRepository.
5. It supplies the repository bean through the constructor.
6. It discovers EmployeeController.
7. It supplies EmployeeService through the controller constructor.

Conceptual dependency graph:

```text
             SPRING CONTAINER
                    |
                    v
           EmployeeRepository
                    |
                    v
             EmployeeService
                    |
                    v
            EmployeeController
```

### Why use final?

```java
private final EmployeeRepository repository;
```

The `final` keyword ensures that the dependency reference cannot be reassigned after constructor initialization.

It does not make the repository object itself immutable.

### What if a dependency is missing?

Suppose Spring cannot find a suitable EmployeeRepository bean.

Then Spring cannot construct EmployeeService, and application startup normally fails with an unsatisfied dependency error.

This is useful because the application identifies a configuration problem at startup rather than silently operating with a missing mandatory dependency.

### What if there are two implementations?

Suppose we have:

```java
public interface NotificationService {
    void sendNotification();
}
```

Two implementations:

```java
@Service
public class EmailNotificationService
        implements NotificationService {

    public void sendNotification() {
        System.out.println("Email sent");
    }
}
```

```java
@Service
public class SmsNotificationService
        implements NotificationService {

    public void sendNotification() {
        System.out.println("SMS sent");
    }
}
```

Now Spring sees two candidates for the same dependency type.

We can use `@Qualifier`:

```java
@Service
public class EmployeeService {

    private final NotificationService notificationService;

    public EmployeeService(
            @Qualifier("emailNotificationService")
            NotificationService notificationService) {

        this.notificationService = notificationService;
    }
}
```

Alternatively, `@Primary` can mark one implementation as the preferred candidate when no qualifier is specified.

**Interview question: What happens if Spring finds multiple beans of the same type?**

Spring may report a `NoUniqueBeanDefinitionException` when it cannot determine which bean to inject. We can resolve this using `@Qualifier` or `@Primary`.

---

## 9. Spring Bean Lifecycle

A bean does not simply appear and disappear. Spring manages its lifecycle.

### Lifecycle stages

```text
       Bean Definition
              |
              v
      Object Instantiation
              |
              v
       Dependency Injection
              |
              v
     Aware Callbacks (if any)
              |
              v
    BeanPostProcessor Before
       Initialization
              |
              v
       Initialization Callbacks
              |
              v
     BeanPostProcessor After
       Initialization
              |
              v
        Bean Ready for Use
              |
              v
       Container Shutdown
              |
              v
       Destruction Callbacks
```

This is a simplified conceptual lifecycle. Spring has additional internal processing stages.

### Step 1: Bean Definition

Spring discovers bean definitions through component scanning, configuration classes and other registration mechanisms.

### Step 2: Instantiation

Spring creates the object.

### Step 3: Dependency Injection

Spring supplies the required dependencies.

### Step 4: Initialization

Spring runs initialization callbacks.

Two commonly used annotations are:

- `@PostConstruct`
- `InitializingBean.afterPropertiesSet()`

A custom initialization method can also be configured.

### Step 5: Bean Ready

After initialization and post-processing, the bean becomes available for normal use.

### Step 6: Destruction

During normal container shutdown, Spring invokes applicable destruction callbacks for managed beans.

### @PostConstruct

This annotation marks a method that should execute after dependency injection.

Example:

```java
@Component
public class StartupService {

    @PostConstruct
    public void initialize() {
        System.out.println("Bean initialized");
    }
}
```

Why use it?

For example, we may want to initialize some in-memory configuration after the bean's dependencies have been injected.

### @PreDestroy

This annotation marks a destruction callback method.

```java
@Component
public class StartupService {

    @PreDestroy
    public void cleanup() {
        System.out.println("Cleaning up resources");
    }
}
```

It can be used for releasing resources during a normal application shutdown.

### Complete example

```java
@Component
public class EmployeeCache {

    public EmployeeCache() {
        System.out.println("1. Constructor called");
    }

    @PostConstruct
    public void initialize() {
        System.out.println("2. Initialization completed");
    }

    @PreDestroy
    public void cleanup() {
        System.out.println("3. Bean is being destroyed");
    }
}
```

Conceptual output:

```text
1. Constructor called
2. Initialization completed

Application is running...

3. Bean is being destroyed
```

### Important: Singleton vs Prototype lifecycle

Spring manages the lifecycle differently depending on bean scope.

| Singleton | Prototype |
|---|---|
| One instance per container | New instance per container request |
| Spring manages initialization | Spring manages initialization |
| Spring normally invokes destruction callbacks | Spring does not manage the complete destruction lifecycle |
| Default scope | Explicitly configured |

For prototype beans, the client is responsible for cleaning up resources after receiving the bean.

### Important interview questions

**Q1. What is the purpose of @PostConstruct?**

It identifies an initialization callback that executes after dependency injection.

**Q2. What is the purpose of @PreDestroy?**

It identifies a destruction callback invoked when Spring destroys a managed bean.

**Q3. Does Spring call @PreDestroy for prototype beans?**

Spring does not manage the complete destruction lifecycle of prototype beans, so it does not automatically invoke their destruction callbacks in the same way as singleton beans.

**Q4. Does @PostConstruct run before dependency injection?**

No. It runs after the required dependencies have been injected.

---

## 10. Bean Scopes

Bean scope determines how many instances Spring creates and how long they are associated with a particular scope.

### Types of Bean Scopes

Spring supports these commonly used scopes:

| Scope | Description |
|---|---|
| Singleton | One instance per Spring IoC container |
| Prototype | A new instance each time requested from the container |
| Request | One instance per HTTP request |
| Session | One instance per HTTP session |
| Application | One instance per ServletContext |
| WebSocket | One instance per WebSocket session |

Request, session, application and WebSocket scopes are web-aware scopes.

### A. Singleton Scope

Singleton is the default Spring bean scope.

```java
@Service
@Scope("singleton")
public class EmployeeService {
}
```

If multiple components request this bean from the same container, they receive the same managed instance.

**Important:** Singleton does not mean one object across the entire JVM. It means one instance per Spring IoC container.

### B. Prototype Scope

```java
@Component
@Scope("prototype")
public class EmployeeTask {
}
```

Every time the container is asked for this prototype bean, it creates a new instance.

Example:

```java
ApplicationContext context =
    SpringApplication.run(EmployeeApplication.class, args);

EmployeeTask first =
    context.getBean(EmployeeTask.class);

EmployeeTask second =
    context.getBean(EmployeeTask.class);

System.out.println(first == second);
```

Expected output:

```text
false
```

Because the container provides separate prototype instances.

### C. Request Scope

One instance is associated with each HTTP request.

```java
@Component
@RequestScope
public class RequestInformation {
}
```

Useful when storing data specific to a single request.

### D. Session Scope

One instance is associated with an HTTP session.

```java
@Component
@SessionScope
public class UserSession {
}
```

Useful for session-specific information in applications that use HTTP sessions.

### E. Application Scope

One instance is associated with the ServletContext.

```java
@Component
@ApplicationScope
public class ApplicationInformation {
}
```

### F. WebSocket Scope

One instance is associated with a WebSocket session.

This is useful for state that belongs to an individual WebSocket connection.

### Singleton vs Prototype

| Singleton | Prototype |
|---|---|
| One instance per container | New instance per request from container |
| Default scope | Must be configured |
| Shared by dependent components | Separate instances |
| Spring manages destruction | Client handles cleanup |

### Important interview point: Is Singleton thread-safe?

No, not automatically.

A singleton bean may be accessed by multiple threads concurrently.

For example:

```java
@Service
public class CounterService {

    private int count = 0;

    public void increment() {
        count++;
    }
}
```

If multiple requests call `increment()` simultaneously, race conditions can occur.

A typical Spring service should avoid mutable shared instance state unless it is properly synchronized or otherwise designed for concurrency.

---

## 11. Important Spring Boot Annotations

These annotations are frequently used in Spring Boot projects.

| Annotation | Purpose |
|---|---|
| `@SpringBootApplication` | Main application configuration annotation |
| `@Component` | General-purpose component |
| `@Service` | Service/business logic component |
| `@Repository` | Persistence component |
| `@Controller` | MVC controller |
| `@RestController` | REST controller |
| `@Autowired` | Requests dependency injection |
| `@Configuration` | Configuration class |
| `@Bean` | Declares a bean through a method |
| `@Value` | Injects a configuration property |
| `@Scope` | Configures bean scope |
| `@Lazy` | Delays bean initialization |
| `@Qualifier` | Selects a particular bean candidate |
| `@Primary` | Marks a preferred bean candidate |

### @Value example

Suppose your `application.yml` contains:

```yaml
spring:
  application:
    name: employee-transport
```

You can inject the property:

```java
@Component
public class ApplicationInformation {

    @Value("${spring.application.name}")
    private String applicationName;

    public String getApplicationName() {
        return applicationName;
    }
}
```

### @Lazy

By default, singleton beans are generally initialized eagerly during application context startup.

`@Lazy` delays bean initialization until the bean is needed.

```java
@Component
@Lazy
public class ReportService {

    public ReportService() {
        System.out.println("ReportService created");
    }
}
```

Lazy initialization can delay startup work, but it can also move configuration or dependency errors to the time the bean is first requested.

### @Qualifier

Used when multiple beans match the same dependency type.

```java
public EmployeeService(
        @Qualifier("emailNotificationService")
        NotificationService notificationService) {
    this.notificationService = notificationService;
}
```

### @Primary

Marks one implementation as the preferred candidate.

```java
@Service
@Primary
public class EmailNotificationService
        implements NotificationService {
}
```

---

## 12. Day 1 Complete Revision

### Conceptual summary

| Concept | Meaning |
|---|---|
| Spring Boot | Simplifies Spring application development |
| IoC | Transfers object management to the container |
| DI | Supplies dependencies from an external source |
| Spring IoC Container | Creates and manages Spring beans |
| Spring Bean | Object managed by Spring |
| @Component | Component scanning registration |
| @Bean | Explicit object registration through a method |
| Constructor Injection | Dependencies supplied through constructor |
| Setter Injection | Dependencies supplied through setters |
| Field Injection | Dependencies injected into fields |
| @PostConstruct | Initialization callback |
| @PreDestroy | Destruction callback |
| Singleton | One instance per container |
| Prototype | New instance on each container request |
| @Qualifier | Selects a bean candidate |
| @Primary | Preferred bean candidate |
| @Value | Injects configuration values |
| @Lazy | Delays bean initialization |

### Important relationships to remember

```text
IoC
 |
 +---- Dependency Injection
          |
          +---- Constructor Injection
          |
          +---- Setter Injection
          |
          +---- Field Injection


Spring IoC Container
 |
 +---- Creates Beans
 |
 +---- Injects Dependencies
 |
 +---- Manages Lifecycle
 |
 +---- Manages Scopes
```

### Day 1 completion checklist

- [ ] Understand Spring Boot and why it is used.
- [ ] Understand layered architecture.
- [ ] Understand IoC and the Spring container.
- [ ] Understand Dependency Injection.
- [ ] Understand all three DI approaches.
- [ ] Understand Spring Beans.
- [ ] Understand `@Component` vs `@Bean`.
- [ ] Understand constructor injection in detail.
- [ ] Understand bean initialization and destruction.
- [ ] Understand all major bean scopes.
- [ ] Understand `@Qualifier`, `@Primary`, `@Value` and `@Lazy`.
'''
path = "/mnt/data/Day_01_Spring_Boot_Fundamentals.md"
with open(path, "w", encoding="utf-8") as f:
    f.write(content)
print(f"Created: {path}\nCharacters: {len(content):,}\nWords: {len(content.split()):,}")
