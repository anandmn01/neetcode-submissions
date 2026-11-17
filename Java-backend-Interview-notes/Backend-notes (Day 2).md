content = r'''# Day 2 — Spring Boot Internals

## Day 2 Topic Index

1. What happens when `SpringApplication.run()` executes?
2. Spring Boot application startup lifecycle
3. ApplicationContext in depth
4. `@SpringBootApplication` internals
5. Component scanning
6. Auto-configuration in depth
7. Starter dependencies and dependency management
8. Configuration files and property sources
9. Spring Profiles
10. Logging
11. Spring Boot Actuator
12. ApplicationRunner and CommandLineRunner
13. Application events and listeners
14. Lazy initialization and DevTools
15. Graceful shutdown
16. Failure analysis and startup diagnostics
17. `@Import` and conditional configuration
18. Configuration metadata and custom properties
19. Advanced startup and web server customization
20. Day 2 revision and interview questions

---

## 1. What Happens When `SpringApplication.run()` Executes?

In Day 1, we learned that `SpringApplication.run()` is the entry point used to start a Spring Boot application.

Now let's understand what happens internally.

Consider the main class:

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

When we execute this application, Spring Boot performs several startup operations.

### High-level startup flow

```text
main()
  |
  v
SpringApplication.run()
  |
  v
Prepare Environment
  |
  v
Create ApplicationContext
  |
  v
Load Bean Definitions
  |
  v
Apply Auto-Configuration
  |
  v
Create and Initialize Beans
  |
  v
Start Embedded Web Server
  |
  v
Publish Application Ready Event
  |
  v
Application Running
```

This is a conceptual overview. Some internal operations are interleaved, and the exact sequence depends on the application type and Spring Boot version.

### Important startup responsibilities

| Step | Responsibility |
|---|---|
| Environment preparation | Loads properties, profiles and command-line arguments |
| ApplicationContext creation | Creates the appropriate Spring container |
| Bean definition loading | Discovers and registers bean definitions |
| Auto-configuration | Applies conditional configuration |
| Bean initialization | Creates beans and injects dependencies |
| Web server startup | Starts the embedded server for web applications |
| Application ready | Publishes the event indicating the application is ready |

### What if startup fails?

The application may fail to start because of:

- Missing required dependencies.
- Multiple matching beans without a clear candidate.
- Invalid configuration.
- Database connection failures during startup initialization.
- Port already in use.
- Bean circular dependencies.
- Invalid application properties.

Spring Boot provides failure analysis to help explain many startup errors.

---

## 2. Spring Boot Application Startup Lifecycle

Spring Boot publishes events during application startup and shutdown.

Understanding these events helps when debugging startup problems or running initialization logic.

### Important Spring Boot application events

| Event | Meaning |
|---|---|
| `ApplicationStartingEvent` | Application startup has begun |
| `ApplicationEnvironmentPreparedEvent` | Environment has been prepared |
| `ApplicationContextInitializedEvent` | ApplicationContext has been initialized but bean definitions have not yet been loaded |
| `ApplicationPreparedEvent` | Context has been prepared and bean definitions loaded, before refresh |
| `ContextRefreshedEvent` | ApplicationContext has been refreshed |
| `WebServerInitializedEvent` | Embedded web server has initialized |
| `ApplicationStartedEvent` | Context has refreshed and application startup is progressing |
| `ApplicationReadyEvent` | Application is ready to service requests |
| `ApplicationFailedEvent` | Application startup failed |

The exact event sequence can vary by application type and framework version. For example, a non-web application does not publish a web-server initialization event.

### Listening to an application event

```java
@Component
public class ApplicationReadyListener {

    @EventListener
    public void onApplicationReady(ApplicationReadyEvent event) {
        System.out.println("Application is ready");
    }
}
```

This method executes after Spring Boot publishes the `ApplicationReadyEvent`.

### ApplicationStartedEvent vs ApplicationReadyEvent

| ApplicationStartedEvent | ApplicationReadyEvent |
|---|---|
| Published after context refresh | Published after application and runner processing |
| Startup is progressing | Application is ready to service requests |
| Runners have not yet completed | Runners have completed |

### When is this useful?

Suppose your application needs to log a startup message or publish a notification after it is ready.

`ApplicationReadyEvent` is useful for that purpose.

Do not use it as a substitute for proper database migrations or critical initialization that must complete before the application accepts traffic.

---

## 3. ApplicationContext in Depth

In Day 1, we learned that ApplicationContext is the Spring IoC container.

Now let's understand its responsibilities and relationship with BeanFactory.

### What is ApplicationContext?

`ApplicationContext` is an interface that represents the central Spring container.

It provides:

- Bean creation and management.
- Dependency Injection.
- Bean lifecycle management.
- Application event publication.
- Access to application resources.
- Internationalization support.
- Environment and property access.

### BeanFactory vs ApplicationContext

| BeanFactory | ApplicationContext |
|---|---|
| Basic IoC container | More feature-rich container |
| Provides bean management | Provides bean management plus additional features |
| Supports dependency injection | Supports dependency injection |
| Basic infrastructure | Events, resources and internationalization support |
| Lower-level container abstraction | Commonly used application container |

`ApplicationContext` extends the `BeanFactory` interface.

### Getting a bean from ApplicationContext

```java
ApplicationContext context =
    SpringApplication.run(EmployeeApplication.class, args);

EmployeeService service =
    context.getBean(EmployeeService.class);
```

This retrieves the managed EmployeeService bean.

In normal application code, constructor injection is preferred over repeatedly calling `getBean()`.

### ApplicationContext and bean management

```text
             ApplicationContext
                     |
          -----------------------
          |          |          |
          v          v          v
        Beans    Environment   Events
          |
          v
   Dependency Injection
          |
          v
    Bean Lifecycle
```

### Important interview answer

ApplicationContext is the central Spring IoC container. It manages beans and dependencies and also provides additional features such as event publication, resource loading and internationalization.

---

## 4. `@SpringBootApplication` Internals

This annotation is commonly placed on the main Spring Boot class.

```java
@SpringBootApplication
public class EmployeeApplication {
}
```

It combines three important annotations:

```java
@SpringBootApplication
=
@Configuration
+
@EnableAutoConfiguration
+
@ComponentScan
```

### A. @Configuration

Marks a class as a source of bean definitions.

```java
@Configuration
public class ApplicationConfig {

    @Bean
    public EmailService emailService() {
        return new EmailService();
    }
}
```

### B. @EnableAutoConfiguration

Enables Spring Boot's auto-configuration mechanism.

Spring Boot evaluates configuration conditions and registers suitable beans based on the classpath, existing beans and configuration properties.

### C. @ComponentScan

Tells Spring to scan for components in the configured package and its subpackages by default.

Example:

```text
com.employee.transport
    |
    |-- EmployeeApplication.java
    |
    |-- controller
    |      |-- EmployeeController.java
    |
    |-- service
    |      |-- EmployeeService.java
    |
    |-- repository
           |-- EmployeeRepository.java
```

If the main application class is in `com.employee.transport`, the subpackages are included in the default component scan.

### Why package structure matters

If a controller or service is outside the scanned package hierarchy, Spring may not discover it automatically.

You can configure scanning explicitly when needed:

```java
@SpringBootApplication(
    scanBasePackages = {
        "com.employee.transport",
        "com.shared.components"
    }
)
public class EmployeeApplication {
}
```

### Interview answer

`@SpringBootApplication` combines `@Configuration`, `@EnableAutoConfiguration` and `@ComponentScan`. It provides configuration support, automatic configuration and component discovery.

---

## 5. Component Scanning

Component scanning is the mechanism through which Spring discovers classes marked with component annotations.

Common annotations discovered through scanning include:

- `@Component`
- `@Service`
- `@Repository`
- `@Controller`
- `@RestController`
- Custom annotations meta-annotated with `@Component`

### Example

```java
@Service
public class EmployeeService {
}
```

If the class is inside the component scanning range, Spring can discover and register it as a bean.

### Component scanning diagram

```text
@SpringBootApplication
        |
        v
@ComponentScan
        |
        v
Scans configured packages
        |
        v
Finds annotated classes
        |
        v
Registers bean definitions
        |
        v
Beans created during context startup
```

### What happens if Spring cannot scan a class?

Suppose EmployeeController is outside the configured component scanning packages.

Spring may fail to find the controller or its required dependencies.

Possible solutions:

1. Move the class under the main application package.
2. Configure `scanBasePackages`.
3. Use `@ComponentScan` with appropriate package configuration.

### Important distinction

Component scanning discovers components. Auto-configuration configures infrastructure and other beans conditionally.

They are related but are not the same mechanism.

---

## 6. Auto-Configuration in Depth

Auto-configuration is one of the most important Spring Boot features.

### What is auto-configuration?

Auto-configuration allows Spring Boot to configure application components based on:

- Dependencies available on the classpath.
- Existing Spring beans.
- Configuration properties.
- Conditional annotations.
- Application type.

### Example: Web application

When web dependencies are present, Spring Boot can configure the web application infrastructure.

Depending on the dependencies and application type, this may include:

- Spring MVC infrastructure.
- HTTP message converters.
- Embedded servlet web server.
- DispatcherServlet registration.

### Example: Database configuration

Suppose you include the appropriate JDBC and database driver dependencies and configure a datasource.

Spring Boot can auto-configure a `DataSource` when the relevant conditions are satisfied and no user-defined datasource takes precedence.

Example configuration:

```yaml
spring:
  datasource:
    url: jdbc:postgresql://localhost:9010/employee_transport_db
    username: postgres
    password: ${DB_PASSWORD}
    driver-class-name: org.postgresql.Driver
```

Spring Boot can use these properties to configure database connectivity.

### Conditional annotations

Auto-configuration uses conditions to decide whether a configuration should apply.

| Annotation | Purpose |
|---|---|
| `@ConditionalOnClass` | Applies when a specified class is present |
| `@ConditionalOnMissingBean` | Applies when a matching bean is absent |
| `@ConditionalOnBean` | Applies when a matching bean exists |
| `@ConditionalOnProperty` | Applies when a property matches the configured condition |
| `@ConditionalOnWebApplication` | Applies to a matching web application type |

### Example: @ConditionalOnProperty

```java
@Configuration
public class NotificationConfiguration {

    @Bean
    @ConditionalOnProperty(
        name = "app.notification.enabled",
        havingValue = "true"
    )
    public NotificationService notificationService() {
        return new EmailNotificationService();
    }
}
```

Configuration:

```yaml
app:
  notification:
    enabled: true
```

When the property is set to `true`, the condition matches and the bean can be registered.

### Example: @ConditionalOnMissingBean

```java
@Configuration
public class MessageConfiguration {

    @Bean
    @ConditionalOnMissingBean(MessageService.class)
    public MessageService messageService() {
        return new DefaultMessageService();
    }
}
```

This provides a default bean only when a matching MessageService bean has not already been registered.

This pattern is widely used in auto-configuration so application developers can override defaults.

### How can we debug auto-configuration?

Run the application with the debug flag:

```bash
java -jar employee-backend.jar --debug
```

Spring Boot prints the condition evaluation report, showing which auto-configurations matched and which did not.

### Interview answer

Spring Boot auto-configuration automatically configures application infrastructure based on classpath dependencies, existing beans, properties and conditional annotations. It reduces manual configuration while allowing developers to override defaults.

---

## 7. Starter Dependencies and Dependency Management

A starter is a convenient dependency descriptor that brings together dependencies commonly required for a particular capability.

### Example: Spring Web starter

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```

This brings in dependencies needed for a typical Spring MVC web application.

### Example: Spring Data JPA starter

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-jpa</artifactId>
</dependency>
```

This provides the Spring Data JPA integration and the relevant persistence dependencies.

### Why are starters useful?

Without starters, developers would need to identify and manage multiple related dependencies manually.

Starters make dependency setup easier and help reduce incompatible version combinations.

### Dependency management

Spring Boot provides dependency management so compatible versions can be selected centrally.

In Maven, a Spring Boot parent or imported dependency BOM can manage versions.

Example using the Spring Boot parent:

```xml
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>YOUR_SPRING_BOOT_VERSION</version>
    <relativePath/>
</parent>
```

The version placeholder above should be replaced with the version used by your project.

### Important distinction

A starter is not the same as auto-configuration.

- Starter dependencies bring libraries onto the classpath.
- Auto-configuration uses classpath contents and conditions to configure beans.

They work together.

---

## 8. Configuration Files and Property Sources

Spring Boot applications need configuration such as:

- Server port.
- Database URL.
- Database credentials.
- Application name.
- Logging levels.
- Feature flags.
- External service URLs.

Spring Boot supports `application.properties` and `application.yml`.

### application.properties

```properties
server.port=8080
spring.application.name=employee-transport

spring.datasource.url=jdbc:postgresql://localhost:9010/employee_transport_db
spring.datasource.username=postgres
spring.datasource.password=${DB_PASSWORD}
```

### application.yml

```yaml
server:
  port: 8080

spring:
  application:
    name: employee-transport

  datasource:
    url: jdbc:postgresql://localhost:9010/employee_transport_db
    username: postgres
    password: ${DB_PASSWORD}
```

Both formats represent application configuration.

### Injecting a property with @Value

```java
@Component
public class ApplicationInformation {

    @Value("${spring.application.name}")
    private String applicationName;
}
```

### Type-safe configuration with @ConfigurationProperties

For a group of related properties, `@ConfigurationProperties` is often preferable.

```yaml
app:
  notification:
    enabled: true
    sender: no-reply@example.com
```

```java
@ConfigurationProperties(prefix = "app.notification")
public class NotificationProperties {

    private boolean enabled;
    private String sender;

    public boolean isEnabled() {
        return enabled;
    }

    public void setEnabled(boolean enabled) {
        this.enabled = enabled;
    }

    public String getSender() {
        return sender;
    }

    public void setSender(String sender) {
        this.sender = sender;
    }
}
```

Register it using configuration properties scanning or `@EnableConfigurationProperties`.

```java
@SpringBootApplication
@ConfigurationPropertiesScan
public class EmployeeApplication {
}
```

### Why use @ConfigurationProperties?

- Groups related configuration.
- Supports type conversion.
- Improves maintainability.
- Supports validation when configured.
- Avoids scattering many `@Value` annotations.

### Externalized configuration

Configuration can come from multiple property sources, such as:

- Application configuration files.
- Profile-specific configuration files.
- Environment variables.
- Java system properties.
- Command-line arguments.
- External configuration sources.

When the same property is supplied by multiple sources, Spring Boot applies property-source precedence rules. Higher-precedence sources override lower-precedence sources.

For example, command-line arguments can override values from application configuration files.

### Environment variables

A database password can be provided through an environment variable rather than committed to source control.

Example in PowerShell:

```powershell
$env:DB_PASSWORD = "your-local-password"
```

Then the YAML property:

```yaml
spring:
  datasource:
    password: ${DB_PASSWORD}
```

### Important security point

Never commit real database passwords, JWT secrets, API keys or production credentials to GitHub.

---

## 9. Spring Profiles

Profiles allow different configurations for different environments.

Typical profiles:

- dev
- test
- staging
- prod

### Why do we need profiles?

A developer's local database may be different from the production database.

Similarly:

- Development may use verbose logging.
- Testing may use a test database.
- Production may use managed database infrastructure and stricter logging.

Profiles help separate these settings.

### Configuration files

```text
src/main/resources/
    |
    |-- application.yml
    |-- application-dev.yml
    |-- application-test.yml
    |-- application-prod.yml
```

Base configuration:

```yaml
spring:
  application:
    name: employee-transport
```

Development configuration:

```yaml
server:
  port: 8080

spring:
  datasource:
    url: jdbc:postgresql://localhost:9010/employee_transport_db
```

Production configuration:

```yaml
server:
  port: 8080

spring:
  datasource:
    url: ${PROD_DATABASE_URL}
    username: ${PROD_DATABASE_USERNAME}
    password: ${PROD_DATABASE_PASSWORD}
```

### Activating a profile

In `application.yml`:

```yaml
spring:
  profiles:
    active: dev
```

Or through a command-line argument:

```bash
java -jar employee-backend.jar --spring.profiles.active=prod
```

### Profile-specific beans

```java
@Configuration
@Profile("dev")
public class DevelopmentConfiguration {

    @Bean
    public String environmentName() {
        return "Development";
    }
}
```

A bean annotated with `@Profile("dev")` is registered when the `dev` profile is active.

### Interview answer

Spring Profiles allow us to define environment-specific beans and configuration. They help maintain separate development, testing and production settings without changing application code.

---

## 10. Logging

Logging records application events and helps developers diagnose problems.

Spring Boot supports common logging frameworks and uses Logback by default with the standard logging starter.

### Why is logging important?

Logs help us:

- Debug errors.
- Understand application behavior.
- Monitor production systems.
- Investigate failed requests.
- Identify performance problems.

### Logging levels

| Level | Meaning |
|---|---|
| TRACE | Very detailed diagnostic information |
| DEBUG | Debugging information |
| INFO | General application events |
| WARN | Potential problem |
| ERROR | Error that needs attention |

### Example

```java
@Service
public class EmployeeService {

    private static final Logger logger =
        LoggerFactory.getLogger(EmployeeService.class);

    public void getEmployees() {

        logger.info("Fetching employees");

        try {
            // Business logic
        } catch (Exception exception) {
            logger.error(
                "Unable to fetch employees",
                exception
            );
            throw exception;
        }
    }
}
```

### Configure logging level

```yaml
logging:
  level:
    root: INFO
    com.employee.transport: DEBUG
    org.hibernate.SQL: DEBUG
```

### Important logging practices

- Do not log passwords or access tokens.
- Avoid logging sensitive personal information.
- Include useful identifiers such as correlation IDs where appropriate.
- Use the correct log level.
- Include exception details when logging failures.
- Avoid excessive logging inside high-frequency loops.

### Interview answer

Logging is the process of recording application events. In Spring Boot, we commonly use SLF4J as the logging API with Logback as the default implementation.

---

## 11. Spring Boot Actuator

Actuator provides endpoints and features for monitoring and managing a Spring Boot application.

### Adding the dependency

For Spring Boot 4.x, use the Actuator starter:

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
```

### Common Actuator endpoints

| Endpoint | Purpose |
|---|---|
| `/actuator/health` | Application health |
| `/actuator/info` | Application information |
| `/actuator/metrics` | Application metrics |
| `/actuator/beans` | Registered beans |
| `/actuator/env` | Environment properties |
| `/actuator/loggers` | Logger configuration |

The availability of endpoints depends on the dependencies, exposure configuration and security settings.

### Exposing endpoints

Example:

```yaml
management:
  endpoints:
    web:
      exposure:
        include: health,info,metrics
```

Now the selected endpoints can be exposed over HTTP, subject to the application's security configuration.

### Health endpoint

Example URL:

```text
http://localhost:8080/actuator/health
```

A healthy application may return:

```json
{
  "status": "UP"
}
```

Additional health information may be available depending on configuration and permissions.

### Why is Actuator useful?

In a backend application, operations teams need to know:

- Is the application running?
- Is the database connection healthy?
- Are application metrics available?
- What is the application's health status?

Actuator helps provide this information.

### Production security

Do not expose every management endpoint publicly.

In particular, environment, beans, configuration and similar diagnostic endpoints can reveal sensitive information. Restrict access using appropriate network and Spring Security controls.

### Interview answer

Spring Boot Actuator provides production-ready monitoring and management capabilities through endpoints such as health, metrics and info.

---

## 12. ApplicationRunner and CommandLineRunner

Both interfaces allow code to execute after the Spring application context has been initialized.

They are useful for startup tasks.

### CommandLineRunner

Receives command-line arguments as a String array.

```java
@Component
public class EmployeeStartupRunner
        implements CommandLineRunner {

    @Override
    public void run(String... args) {

        System.out.println("Application startup task");
    }
}
```

### ApplicationRunner

Receives an `ApplicationArguments` object.

```java
@Component
public class EmployeeApplicationRunner
        implements ApplicationRunner {

    @Override
    public void run(ApplicationArguments args) {

        System.out.println("Application runner executed");
    }
}
```

### Difference

| CommandLineRunner | ApplicationRunner |
|---|---|
| Receives `String... args` | Receives `ApplicationArguments` |
| Simple argument access | Structured argument access |
| Runs after context initialization | Runs after context initialization |

### When are they useful?

Examples:

- Loading initial reference data.
- Checking required startup configuration.
- Printing application information.
- Performing non-critical startup tasks.

Avoid putting long-running or fragile tasks into startup runners unless you intentionally want them to delay application readiness.

### Ordering runners

Multiple runners can be ordered using `@Order` or by implementing `Ordered`.

```java
@Component
@Order(1)
public class FirstRunner implements CommandLineRunner {

    @Override
    public void run(String... args) {
        System.out.println("First runner");
    }
}
```

---

## 13. Application Events and Listeners

Spring supports event-driven communication between components within the application context.

One component publishes an event, and another component listens for it.

### Why use events?

Suppose an employee is created.

After creating the employee, the application may need to:

- Send a notification.
- Record an audit event.
- Trigger another internal action.

Instead of directly coupling the employee service to every other component, an event can communicate that the employee was created.

### Step 1: Create an event

```java
public class EmployeeCreatedEvent {

    private final Long employeeId;

    public EmployeeCreatedEvent(Long employeeId) {
        this.employeeId = employeeId;
    }

    public Long getEmployeeId() {
        return employeeId;
    }
}
```

### Step 2: Publish the event

```java
@Service
public class EmployeeService {

    private final ApplicationEventPublisher publisher;

    public EmployeeService(ApplicationEventPublisher publisher) {
        this.publisher = publisher;
    }

    public void createEmployee(Long employeeId) {

        // Save employee first

        publisher.publishEvent(
            new EmployeeCreatedEvent(employeeId)
        );
    }
}
```

### Step 3: Listen to the event

```java
@Component
public class EmployeeEventListener {

    @EventListener
    public void handleEmployeeCreated(
            EmployeeCreatedEvent event) {

        System.out.println(
            "Employee created: " + event.getEmployeeId()
        );
    }
}
```

### Event flow

```text
EmployeeService
      |
      | publishEvent()
      v
ApplicationEventPublisher
      |
      v
Spring Application Event System
      |
      v
EmployeeEventListener
      |
      v
Handle Employee Created Event
```

### Important points

- Spring application events are normally synchronous by default.
- The publisher does not need to directly call the listener.
- `@EventListener` identifies an event-handling method.
- `@Async` can be used for asynchronous event handling when properly configured.
- Asynchronous event handling introduces thread and transaction considerations.

### Spring events vs Kafka

| Spring Application Events | Kafka |
|---|---|
| Usually within one application context | Distributed messaging platform |
| In-memory event publication | Messages stored in Kafka topics |
| Normally synchronous by default | Producers and consumers operate independently |
| Not a durable distributed message broker | Supports durable event streaming |

Spring application events are not a replacement for Kafka when durable communication between independent services is required.

---

## 14. Lazy Initialization and Spring Boot DevTools

### A. Lazy Initialization

By default, singleton beans are generally created eagerly during application context startup.

With lazy initialization, Spring delays bean creation until the bean is needed.

Example:

```java
@Component
@Lazy
public class ReportService {

    public ReportService() {
        System.out.println("ReportService created");
    }
}
```

You can also configure lazy initialization globally:

```yaml
spring:
  main:
    lazy-initialization: true
```

### Eager vs Lazy initialization

| Eager | Lazy |
|---|---|
| Bean created during normal context startup | Bean created when first needed |
| Startup may take longer | Startup may be faster |
| Many configuration errors appear early | Some errors appear later |
| Useful for early validation | Useful when some beans are rarely needed |

### Important point

Lazy initialization does not mean the bean is never created. It means creation is delayed until it is requested.

### B. Spring Boot DevTools

DevTools is a development-time utility that can improve the development workflow.

It supports features such as:

- Automatic application restart after classpath changes.
- Development-oriented defaults.
- Optional LiveReload integration.

Example Maven dependency:

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-devtools</artifactId>
    <scope>runtime</scope>
    <optional>true</optional>
</dependency>
```

DevTools is intended for development and should not be treated as a production dependency.

---

## 15. Graceful Shutdown

Graceful shutdown means allowing an application to stop without abruptly abandoning work that is already being processed.

### Why is it important?

Suppose your backend is processing employee trip assignments.

If the application is stopped abruptly, requests may fail or work may be interrupted.

A graceful shutdown aims to:

1. Stop accepting new requests.
2. Allow in-flight requests to finish within a configured period.
3. Close managed resources.
4. Shut down the application.

### Configure graceful shutdown

```yaml
server:
  shutdown: graceful

spring:
  lifecycle:
    timeout-per-shutdown-phase: 30s
```

This configures graceful shutdown behavior for supported web server and lifecycle components.

The deployment environment must also allow enough time for the application to finish shutting down.

### Important interview answer

Graceful shutdown allows an application to stop accepting new work and gives in-flight requests and managed lifecycle components time to complete before the process exits.

---

## 16. Failure Analysis and Startup Diagnostics

Spring Boot provides failure analysis to make common startup problems easier to understand.

### What is a FailureAnalyzer?

A FailureAnalyzer examines a startup failure and can provide a human-readable description and suggested action.

For example, when an application tries to use a port that is already occupied, the startup failure can explain the problem and suggest checking the process using that port.

### Common startup problems

| Error or problem | Possible cause |
|---|---|
| `NoSuchBeanDefinitionException` | Required bean not found |
| `NoUniqueBeanDefinitionException` | Multiple matching beans |
| `UnsatisfiedDependencyException` | Dependency could not be resolved |
| Port already in use | Another process occupies the port |
| Database connection failure | Incorrect credentials, URL or unavailable database |
| Circular dependency | Beans depend on each other in a cycle |

### Debug auto-configuration

Run:

```bash
mvn spring-boot:run -Dspring-boot.run.arguments=--debug
```

Or:

```bash
java -jar employee-backend.jar --debug
```

This enables the condition evaluation report.

### Interview point

A startup failure should be diagnosed from the root cause in the exception chain, not just the final error message.

---

## 17. @Import and Conditional Configuration

### A. @Import

`@Import` allows one configuration class to include another configuration class.

Example:

```java
@Configuration
public class EmailConfiguration {

    @Bean
    public EmailService emailService() {
        return new EmailService();
    }
}
```

Import it:

```java
@Configuration
@Import(EmailConfiguration.class)
public class ApplicationConfiguration {
}
```

This allows configuration to be modularized.

### B. @ConditionalOnProperty

Registers a bean only when a property condition matches.

```java
@Configuration
public class NotificationConfiguration {

    @Bean
    @ConditionalOnProperty(
        name = "app.notification.enabled",
        havingValue = "true"
    )
    public NotificationService notificationService() {
        return new EmailNotificationService();
    }
}
```

### C. @ConditionalOnClass

Applies configuration only when a specified class is available on the classpath.

```java
@Configuration
@ConditionalOnClass(name = "com.example.SomeLibrary")
public class OptionalLibraryConfiguration {
}
```

### D. @ConditionalOnMissingBean

Applies when a matching bean is not already present.

This is useful for default implementations that users can override.

### ConditionEvaluationReport

Spring Boot can provide a report describing auto-configuration condition matches and failures.

The report is especially useful when an expected bean is not being created.

---

## 18. Configuration Metadata and Custom Properties

### What is configuration metadata?

Configuration metadata describes available configuration properties and can improve IDE support such as autocomplete and documentation.

For example:

```yaml
app:
  notification:
    enabled: true
    sender: no-reply@example.com
```

A configuration properties class:

```java
@ConfigurationProperties(prefix = "app.notification")
public class NotificationProperties {

    private boolean enabled;
    private String sender;

    // Getters and setters
}
```

The configuration metadata processor can generate metadata for IDE assistance.

For Maven:

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-configuration-processor</artifactId>
    <optional>true</optional>
</dependency>
```

### @ConfigurationPropertiesScan

This annotation enables scanning for classes annotated with `@ConfigurationProperties`.

```java
@SpringBootApplication
@ConfigurationPropertiesScan
public class EmployeeApplication {
}
```

### @ConfigurationPropertiesBinding

This annotation can be used to identify custom converters used when binding configuration properties.

It is useful when a configuration value needs conversion into a custom Java type.

---

## 19. Advanced Startup and Web Server Customization

These are more advanced Spring Boot extension points.

### A. ApplicationContextInitializer

An `ApplicationContextInitializer` can customize an ApplicationContext before it is refreshed.

It is useful for early context customization.

Conceptual example:

```java
public class MyContextInitializer
        implements ApplicationContextInitializer<
            ConfigurableApplicationContext> {

    @Override
    public void initialize(
            ConfigurableApplicationContext context) {

        // Customize the context before refresh
    }
}
```

It must be registered through an appropriate Spring Boot initialization mechanism.

### B. SpringApplicationRunListener

`SpringApplicationRunListener` provides callbacks during the SpringApplication startup process.

It is an advanced extension point for observing or customizing startup phases.

It is not normally required for everyday application development.

### C. Jackson2ObjectMapperBuilderCustomizer

This extension point customizes Jackson's ObjectMapper builder when Spring Boot's Jackson auto-configuration is active.

Example:

```java
@Bean
public Jackson2ObjectMapperBuilderCustomizer jsonCustomizer() {

    return builder ->
        builder.indentOutput(true);
}
```

This example configures indented JSON output.

### D. ServletRegistrationBean

Used to register a servlet programmatically in a servlet-based web application.

```java
@Bean
public ServletRegistrationBean<MyServlet> myServlet() {

    return new ServletRegistrationBean<>(
        new MyServlet(),
        "/custom/*"
    );
}
```

### E. WebServerFactoryCustomizer

Used to customize the embedded web server factory.

```java
@Component
public class ServerCustomizer
        implements WebServerFactoryCustomizer<
            ConfigurableServletWebServerFactory> {

    @Override
    public void customize(
            ConfigurableServletWebServerFactory factory) {

        factory.setPort(8081);
    }
}
```

This example customizes the embedded servlet server port programmatically.

For ordinary applications, using `server.port` in application configuration is usually simpler.

---

## 20. Day 2 Complete Revision

### Important concepts

| Concept | Remember |
|---|---|
| `SpringApplication.run()` | Bootstraps the Spring Boot application |
| ApplicationContext | Central Spring IoC container |
| BeanFactory | Basic IoC container interface |
| `@SpringBootApplication` | Combines three important annotations |
| `@Configuration` | Declares configuration class |
| `@EnableAutoConfiguration` | Enables conditional auto-configuration |
| `@ComponentScan` | Discovers application components |
| Auto-configuration | Configures infrastructure based on conditions |
| Starter | Convenient dependency bundle |
| Property sources | Supply application configuration |
| Profiles | Environment-specific configuration |
| Logging | Records application events |
| Actuator | Monitoring and management features |
| ApplicationRunner | Runs after context initialization |
| CommandLineRunner | Runs after context initialization |
| Application events | Internal event-driven communication |
| `@Lazy` | Delays bean initialization |
| DevTools | Development-time productivity features |
| Graceful shutdown | Allows in-flight work to complete |
| FailureAnalyzer | Explains common startup failures |
| `@Import` | Includes configuration classes |
| `@ConfigurationProperties` | Type-safe configuration binding |

### Important startup flow

```text
SpringApplication.run()
        |
        v
Prepare Environment
        |
        v
Create ApplicationContext
        |
        v
Load Bean Definitions
        |
        v
Apply Auto-Configuration
        |
        v
Create and Initialize Beans
        |
        v
Start Embedded Server
        |
        v
ApplicationStartedEvent
        |
        v
Run ApplicationRunner / CommandLineRunner
        |
        v
ApplicationReadyEvent
        |
        v
Application Ready
```

This is a simplified conceptual sequence; exact internal ordering depends on application type and Spring Boot version.

### Day 2 completion checklist

- [ ] Understand what happens during `SpringApplication.run()`.
- [ ] Understand Spring Boot startup events.
- [ ] Understand ApplicationContext and BeanFactory.
- [ ] Understand `@SpringBootApplication` internally.
- [ ] Understand component scanning.
- [ ] Understand auto-configuration and conditional annotations.
- [ ] Understand starter dependencies.
- [ ] Understand application properties and YAML.
- [ ] Understand externalized configuration.
- [ ] Understand Spring Profiles.
- [ ] Understand logging levels and configuration.
- [ ] Understand Actuator.
- [ ] Understand ApplicationRunner and CommandLineRunner.
- [ ] Understand application events and listeners.
- [ ] Understand lazy initialization and DevTools.
- [ ] Understand graceful shutdown.
- [ ] Understand startup failure diagnostics.
- [ ] Understand advanced configuration extension points.

---

## Day 2 — Important Interview Questions

### Q1. What happens when SpringApplication.run() executes?

It bootstraps the Spring Boot application by preparing the environment, creating and refreshing the ApplicationContext, registering and initializing beans, starting the embedded server when applicable and publishing lifecycle events.

### Q2. What is ApplicationContext?

ApplicationContext is the central Spring IoC container. It manages beans, dependencies, lifecycle and additional services such as event publication and resource loading.

### Q3. What is the difference between BeanFactory and ApplicationContext?

BeanFactory provides basic IoC functionality, while ApplicationContext extends it with additional features such as events, resource loading and internationalization.

### Q4. What does @SpringBootApplication contain?

It combines `@Configuration`, `@EnableAutoConfiguration` and `@ComponentScan`.

### Q5. What is auto-configuration?

It is Spring Boot's mechanism for conditionally configuring application infrastructure based on dependencies, existing beans, application properties and application type.

### Q6. What is the difference between a starter and auto-configuration?

A starter brings related dependencies onto the classpath. Auto-configuration uses the classpath and other conditions to configure application beans.

### Q7. What is the difference between ApplicationStartedEvent and ApplicationReadyEvent?

ApplicationStartedEvent is published after the application context has refreshed. ApplicationReadyEvent is published after application startup processing and runners have completed.

### Q8. What is the difference between CommandLineRunner and ApplicationRunner?

CommandLineRunner receives a String array of arguments, while ApplicationRunner receives structured ApplicationArguments.

### Q9. What are Spring Profiles?

Profiles allow us to maintain environment-specific beans and configuration for development, testing, staging and production.

### Q10. What is Spring Boot Actuator?

Actuator provides application monitoring and management features through endpoints such as health, info and metrics.

### Q11. What is graceful shutdown?

Graceful shutdown allows an application to stop accepting new work and gives in-flight requests and lifecycle components time to complete before shutdown.

### Q12. How can we debug auto-configuration?

Run the application with `--debug` and inspect the condition evaluation report.

---

**End of Day 2 Notes**
'''
path = "/mnt/data/Day_02_Spring_Boot_Internals.md"
with open(path, "w", encoding="utf-8") as f:
    f.write(content)
print(f"Created {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
