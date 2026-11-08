# Day 1: Spring Boot Fundamentals

## Topics Covered

1. What is Spring Boot?
2. Why do we use Spring Boot?
3. Spring Boot Architecture
4. Inversion of Control (IoC)
5. Dependency Injection (DI)
6. Spring Beans
7. @Component vs @Bean
8. Constructor Injection
9. Bean Lifecycle
10. Bean Scopes
11. Spring Boot Annotations

---

## 1. What is Spring Boot?

Spring Boot is a Java framework built on top of the Spring Framework.

It simplifies the development of Java applications by reducing manual configuration.

It helps developers build:
- REST APIs
- Web applications
- Microservices
- Enterprise applications
- Backend services

### Why Spring Boot?

Before Spring Boot, developers had to configure many things manually, such as:
- Application configuration
- Dependency management
- Server configuration
- Bean configuration

Spring Boot simplifies these tasks using:
- Auto-configuration
- Starter dependencies
- Embedded servers
- Production-ready features

### Interview Answer

Spring Boot is a framework built on top of Spring that simplifies Java application development through auto-configuration, starter dependencies and embedded servers.

---

## 2. Why Do We Use Spring Boot?

Important advantages:

| Feature | Explanation |
|---|---|
| Auto Configuration | Automatically configures components based on dependencies and settings |
| Starter Dependencies | Provides convenient dependency bundles |
| Embedded Server | Runs applications without separately deploying to an external server |
| Less Configuration | Reduces manual XML and Java configuration |
| Production Ready | Supports monitoring and application management through Actuator |
| Microservices Support | Makes it easier to build independent backend services |

### Example

A Spring Boot application can start using:

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
