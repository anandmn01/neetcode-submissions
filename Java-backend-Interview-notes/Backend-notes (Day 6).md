from pathlib import Path

content = r'''# Day 06 — Spring Boot Microservices, Kafka, Docker, and Distributed Systems

> **Learning track:** Java Backend Developer Interview Preparation  
> **Day:** 06  
> **Focus:** Microservice architecture and distributed backend engineering  
> **Examples:** Employee Transport platform  
> **Stack context:** Java 21, Spring Boot 4.x, Spring Cloud, Kafka, Docker

---

## Learning objectives

By the end of this day, you should be able to explain:

1. Why microservices are used and how they differ from a monolith.
2. How services communicate using REST and messaging.
3. What service discovery and an API Gateway do.
4. How client-side and server-side load balancing work.
5. How OpenFeign simplifies service-to-service HTTP calls.
6. Synchronous vs asynchronous communication.
7. Kafka brokers, topics, partitions, producers, and consumers.
8. Consumer groups, offsets, and delivery semantics.
9. Event-driven architecture.
10. Circuit breakers, retries, timeouts, and fallbacks.
11. Distributed transactions and the Saga pattern.
12. Centralized configuration, tracing, and correlation IDs.
13. How Docker packages microservices.
14. Security and design patterns in distributed systems.

---

# 1. Monolithic Architecture vs Microservices

## 1.1 What is a monolithic application?

A monolithic application is built and deployed as one application unit.

For example, an Employee Transport backend may contain:

- Authentication module.
- Employee management.
- Driver management.
- Vehicle management.
- Trip management.
- Notification functionality.

In a monolith, these modules can be organized into separate packages, but they are still part of one deployable application.

```text
             Flutter Application
                     |
                     v
          Employee Transport Backend
          +-------------------------+
          | Authentication          |
          | Employee Management     |
          | Driver Management       |
          | Vehicle Management      |
          | Trip Management         |
          | Notifications           |
          +-------------------------+
                     |
                     v
                 PostgreSQL
```

### Advantages

- Easier to start developing.
- Simpler deployment.
- In-process method calls are straightforward.
- Easier local debugging.
- Fewer distributed-system failure modes.

### Disadvantages

- The application can become large and difficult to maintain.
- A deployment may require releasing the whole application.
- Scaling one busy module may require scaling the entire application.
- A failure or resource problem can affect the whole process.
- Teams may become tightly coupled through shared code and release cycles.

## 1.2 What are microservices?

Microservices architecture divides an application into smaller services organized around business capabilities.

Each service:

- Owns a specific responsibility.
- Can be developed and deployed independently.
- Communicates with other services through defined contracts.
- Often owns its data and persistence boundary.
- Can be scaled independently when the architecture supports it.

Example:

```text
                 Flutter Client
                       |
                       v
                  API Gateway
                       |
       +---------------+----------------+
       |               |                |
       v               v                v
  Auth Service    Employee Service   Trip Service
       |               |                |
       v               v                v
   Auth DB        Employee DB         Trip DB
                                        |
                                        v
                                  Driver Service
                                        |
                                        v
                                    Driver DB
```

This is a conceptual architecture. A real system may combine services or use different boundaries.

## 1.3 Monolith vs microservices

| Monolith | Microservices |
|---|---|
| One deployable application unit | Multiple independently deployable services |
| Often one shared database | Services commonly own separate data |
| In-process communication between modules | Network or messaging communication |
| Simpler operational model initially | More complex deployment and monitoring |
| Scaling often applies to the whole application | Services can be scaled independently |
| A code change may require whole-app deployment | A service can often be released independently |

## 1.4 When should a team use microservices?

Microservices are not automatically better.

They can be useful when:

- Different business capabilities need independent release cycles.
- Teams need clear ownership boundaries.
- Parts of the system have very different scaling requirements.
- The organization can operate distributed systems reliably.
- The domain is sufficiently understood to define service boundaries.

A modular monolith may be a better starting point for a small product or team. Splitting too early adds network, deployment, data-consistency, and observability complexity.

**Interview point:** Microservices solve some scaling and organizational problems, but introduce distributed-system complexity.

---

# 2. Inter-Service Communication

Microservices need to communicate with one another.

Two common approaches are:

1. Synchronous communication.
2. Asynchronous communication.

## 2.1 Synchronous REST communication

A service sends an HTTP request and waits for a response.

Example:

```text
Trip Service
     |
     | GET /api/drivers/15
     v
Driver Service
     |
     | Driver details
     v
Trip Service continues
```

The Trip Service cannot complete that part of its work until it receives a response or a timeout/failure occurs.

### Advantages

- Simple request-response model.
- Easy to understand and debug initially.
- Useful when an immediate result is required.
- HTTP tooling and conventions are widely supported.

### Disadvantages

- Caller depends on the availability and latency of the callee.
- Multiple service calls can increase total response time.
- Failures can propagate through a call chain.
- Tight runtime coupling can develop.

## 2.2 Asynchronous communication

A service publishes a message or event and continues without waiting for every consumer to finish.

```text
Trip Service
     |
     | Publish TripAssigned event
     v
   Kafka
     |
     +--------------------+
     |                    |
     v                    v
Notification Service   Analytics Service
```

### Advantages

- Producer and consumer can be less tightly coupled in time.
- Consumers can process messages independently.
- Useful for background work and event-driven workflows.
- Message brokers can buffer work during temporary consumer slowdowns.

### Disadvantages

- Eventual consistency must be handled.
- Debugging and tracing require additional tooling.
- Duplicate message processing must be considered.
- Ordering and retry behavior require deliberate design.

---

# 3. Service Discovery — Eureka

In a microservices environment, service instances may start, stop, or change network locations.

Hardcoding every service IP address is fragile.

Service discovery helps services find one another dynamically.

## 3.1 What is service discovery?

A service registry stores information about available service instances.

A service instance registers itself, and other services discover its location through the registry or a discovery-aware client.

## 3.2 Eureka architecture

Netflix Eureka is a service registry commonly used with Spring Cloud.

```text
             Eureka Server
             (Registry)
              /       \
             /         \
            v           v
      Driver Service  Trip Service
        registers       registers
            ^           |
            |           |
            +-----------+
          discovers service
```

Conceptual flow:

1. Eureka Server starts.
2. Driver Service starts and registers its instance.
3. Trip Service starts and registers its instance.
4. Trip Service discovers Driver Service by its logical service name.
5. The client-side load-balancing mechanism selects an available instance.

## 3.3 Why not hardcode URLs?

Without discovery:

```text
http://192.168.1.20:8082/api/drivers/15
```

If the service moves or scales to multiple instances, configuration becomes difficult.

With discovery, the caller can use a logical service name such as `DRIVER-SERVICE`, subject to the discovery and load-balancing configuration.

## 3.4 Important considerations

- A registry needs health and availability management.
- Registration and discovery behavior must be configured correctly.
- Discovery does not remove the need for timeouts and failure handling.
- Kubernetes environments often provide service discovery through Kubernetes Services and DNS, so Eureka is not always necessary.

---

# 4. API Gateway

An API Gateway is an entry point through which clients access backend services.

Without a gateway, a mobile or web client may need to know the address of multiple services.

With a gateway, the client calls one public entry point.

## 4.1 Architecture

```text
                 Flutter Client
                       |
                       v
                  API Gateway
                       |
        +--------------+--------------+
        |              |              |
        v              v              v
   Auth Service   Employee Service  Trip Service
```

## 4.2 Responsibilities

An API Gateway may handle:

- Request routing.
- Authentication integration.
- Rate limiting.
- Request/response header handling.
- CORS policy.
- TLS termination in some deployments.
- Request logging and correlation IDs.
- API aggregation in selected designs.

The gateway should not become a place for all business logic. Domain rules should remain in the services that own those capabilities.

## 4.3 Example routes

| Public route | Destination service |
|---|---|
| `/api/auth/**` | Auth Service |
| `/api/employees/**` | Employee Service |
| `/api/trips/**` | Trip Service |
| `/api/drivers/**` | Driver Service |

## 4.4 Advantages

- One public entry point.
- Clients do not need to know internal service addresses.
- Shared cross-cutting concerns can be centralized.
- Internal services can remain private to the network.

## 4.5 Risks

- The gateway can become a bottleneck if poorly sized.
- It can become a single point of failure without redundancy.
- Overloading it with business logic creates tight coupling.
- Gateway configuration is security-sensitive.

---

# 5. Load Balancing

Load balancing distributes incoming requests across multiple service instances.

## 5.1 Why is load balancing needed?

Suppose Driver Service has three instances:

```text
Driver Service Instance 1
Driver Service Instance 2
Driver Service Instance 3
```

If every request goes to only one instance, that instance may become overloaded while the others remain idle.

A load balancer distributes requests among eligible instances.

## 5.2 Common strategies

| Strategy | Description |
|---|---|
| Round Robin | Select instances in rotation |
| Random | Select an instance randomly |
| Least Connections | Prefer the instance with fewer active connections |
| Weighted | Send more traffic to instances with higher assigned weight |

Actual strategy availability depends on the load-balancing implementation.

## 5.3 Client-side vs server-side load balancing

**Client-side load balancing:**

The calling service obtains available instances and chooses one.

**Server-side load balancing:**

The caller sends requests to a load balancer, which chooses the destination instance.

```text
Client-side:
Trip Service -> chooses Driver Service instance

Server-side:
Trip Service -> Load Balancer -> Driver Service instance
```

## 5.4 Spring Cloud LoadBalancer

Spring Cloud LoadBalancer provides client-side load-balancing abstractions that can work with service discovery.

Do not assume the older Netflix Ribbon library is the current default. Ribbon is no longer the standard Spring Cloud load-balancing choice.

---

# 6. OpenFeign in Depth

OpenFeign is a declarative HTTP client. It allows a service to describe a remote HTTP API using a Java interface.

Instead of manually constructing HTTP requests for every call, the application declares the client contract.

## 6.1 Without OpenFeign

A service may need to use `RestClient`, `WebClient`, or another HTTP client and manually handle:

- URL construction.
- HTTP methods.
- Headers.
- Request and response conversion.
- Error handling.

## 6.2 With OpenFeign

Example client:

```java
@FeignClient(name = "DRIVER-SERVICE")
public interface DriverClient {

    @GetMapping("/api/drivers/{id}")
    DriverResponse getDriverById(@PathVariable Long id);
}
```

Use it in a service:

```java
@Service
public class TripService {

    private final DriverClient driverClient;

    public TripService(DriverClient driverClient) {
        this.driverClient = driverClient;
    }

    public DriverResponse getDriver(Long driverId) {
        return driverClient.getDriverById(driverId);
    }
}
```

The interface describes the remote endpoint. Spring Cloud OpenFeign provides the client implementation.

## 6.3 Important concepts

- `@FeignClient`: declares a declarative HTTP client.
- `name`: identifies the target service; with discovery/load balancing, this can be a logical service name.
- Mapping annotations: describe the remote HTTP endpoint.
- DTOs: define request and response contracts.
- Error decoder: can translate remote HTTP errors into application exceptions.
- Timeouts: prevent a caller from waiting indefinitely.

## 6.4 When to use OpenFeign?

It is useful for synchronous service-to-service HTTP communication when the team prefers declarative interfaces.

For reactive or streaming use cases, `WebClient` may be a more suitable choice. For new synchronous Spring applications, `RestClient` is also an important option.

---

# 7. Synchronous vs Asynchronous Communication

This topic is important because distributed systems should not use synchronous calls for every operation.

## 7.1 Synchronous example

When a trip is created, Trip Service calls Driver Service and waits to confirm that the selected driver exists and is eligible.

```text
Trip Service
    |
    | HTTP request
    v
Driver Service
    |
    | HTTP response
    v
Trip Service continues
```

## 7.2 Asynchronous example

After a trip is assigned, the system publishes an event. Notification Service sends a message to the driver independently.

```text
Trip Service
    |
    | TripAssigned event
    v
Kafka
    |
    v
Notification Service
    |
    v
Send notification
```

## 7.3 Comparison

| Synchronous | Asynchronous |
|---|---|
| Caller waits for response | Producer does not wait for consumer completion |
| Immediate response is possible | Processing may happen later |
| Runtime availability dependency | Broker/message flow can decouple services |
| Simpler request-response reasoning | Requires event and delivery handling |
| Useful for immediate decisions | Useful for background workflows |

A real system often uses both approaches.

---

# 8. Kafka Fundamentals

Apache Kafka is a distributed event-streaming platform used to publish, store, and consume event records.

It is commonly used for event-driven architectures, data pipelines, log aggregation, and asynchronous communication.

## 8.1 What is a message/event?

An event represents something that happened.

Examples:

- `EmployeeCreated`
- `TripCreated`
- `TripAssigned`
- `DriverAssigned`
- `EmployeeBoarded`
- `TripCompleted`

An event is normally expressed as a fact in the past tense.

## 8.2 What is a Kafka topic?

A topic is a named stream of records.

Examples:

```text
employee-events
trip-events
notification-events
```

A producer publishes records to a topic. Consumers subscribe to the topic and process records.

## 8.3 Kafka architecture

```text
                  Kafka Cluster
          +---------------------------+
          |                           |
          |       Broker 1            |
          |       Broker 2            |
          |       Broker 3            |
          |                           |
          +---------------------------+
                    ^
                    |
          +---------+---------+
          |                   |
          v                   v
       Producer            Consumer
          |                   |
          v                   v
     trip-events         trip-events
```

## 8.4 Important components

| Component | Meaning |
|---|---|
| Producer | Publishes records |
| Consumer | Reads records |
| Topic | Named record stream |
| Partition | Ordered log segment within a topic |
| Broker | Kafka server that stores and serves partitions |
| Consumer Group | Consumers cooperating to process a topic |
| Offset | Position of a record within a partition |
| Replication | Copies partition data across brokers for resilience |

## 8.5 Why Kafka?

- High-throughput event streaming.
- Durable record storage for configured retention.
- Horizontal scalability through partitions and brokers.
- Multiple independent consumer applications can process the same events.
- Supports replaying records by resetting consumer offsets, subject to retention.

Kafka is not simply a traditional queue. Records are retained according to topic configuration and can be read by multiple consumer groups.

---

# 9. Kafka Architecture: Brokers, Topics, and Partitions

## 9.1 Broker

A broker is a Kafka server responsible for storing and serving topic partitions.

A Kafka cluster normally consists of multiple brokers.

## 9.2 Partition

A topic is divided into partitions.

Example:

```text
Topic: trip-events

Partition 0: [E1] [E4] [E7] [E10]
Partition 1: [E2] [E5] [E8]
Partition 2: [E3] [E6] [E9]
```

Each partition is an ordered log. Records within a partition have an order based on their offsets.

Kafka does not provide a single total ordering across all partitions of a topic.

## 9.3 Why partitions?

Partitions allow:

- Parallel consumption.
- Distribution of data across brokers.
- Higher throughput.
- Horizontal scaling of consumers.

## 9.4 Replication

Kafka can replicate partitions across brokers.

A partition has a leader replica and follower replicas. Producers and consumers normally interact with the leader for that partition, while followers replicate data.

Replication helps availability and durability, depending on settings such as replication factor, acknowledgments, and minimum in-sync replicas.

## 9.5 Choosing a message key

A producer can specify a record key.

For example, using `tripId` as the key can route records for the same trip to the same partition under the default partitioning behavior.

This helps preserve per-trip ordering, provided the partitioning strategy and partition count remain compatible with the ordering requirement.

---

# 10. Kafka Producers and Consumers

## 10.1 Producer

A producer publishes records to a Kafka topic.

Conceptual Spring Kafka example:

```java
@Service
public class TripEventProducer {

    private final KafkaTemplate<String, TripAssignedEvent> kafkaTemplate;

    public TripEventProducer(
            KafkaTemplate<String, TripAssignedEvent> kafkaTemplate) {
        this.kafkaTemplate = kafkaTemplate;
    }

    public void publish(TripAssignedEvent event) {
        kafkaTemplate.send(
                "trip-events",
                event.tripId().toString(),
                event
        );
    }
}
```

The trip ID is used as the record key so events for the same trip can be routed consistently by the partitioner.

## 10.2 Consumer

A consumer subscribes to a topic and processes records.

```java
@Service
public class TripEventConsumer {

    @KafkaListener(
        topics = "trip-events",
        groupId = "notification-service"
    )
    public void consume(TripAssignedEvent event) {
        // Process the event and send a notification.
    }
}
```

This is a simplified illustration. A real application must configure serializers/deserializers, error handling, retry policy, and delivery/acknowledgment behavior.

## 10.3 Producer responsibilities

- Choose the topic.
- Serialize the key and value.
- Choose an appropriate record key.
- Configure acknowledgments and retries.
- Handle send failures.
- Publish events with a stable schema and event identity.

## 10.4 Consumer responsibilities

- Deserialize records.
- Process messages safely.
- Handle failures and retries.
- Commit offsets according to the processing model.
- Be prepared for duplicate delivery.
- Monitor lag and processing health.

---

# 11. Consumer Groups and Partitions

A consumer group is a set of consumers that coordinate to consume a topic.

## 11.1 One consumer group

Suppose a topic has three partitions:

```text
Topic: trip-events

Partition 0
Partition 1
Partition 2

Consumer Group: notification-service
    Consumer A -> Partition 0
    Consumer B -> Partition 1
    Consumer C -> Partition 2
```

Within a consumer group, a partition is assigned to at most one consumer at a time under normal group assignment.

## 11.2 More consumers than partitions

If a topic has three partitions and the group has five consumers, at least two consumers will be idle for that topic assignment.

The number of active consumers in a group cannot exceed the number of partitions assigned to that group.

## 11.3 Multiple consumer groups

Different consumer groups can independently consume the same topic.

```text
                 trip-events
                      |
          +-----------+-----------+
          |                       |
          v                       v
 Notification Group          Analytics Group
          |                       |
          v                       v
 Send messages              Build reports
```

Each group maintains its own offsets.

---

# 12. Kafka Offsets and Delivery Semantics

## 12.1 What is an offset?

An offset is a record's position within a partition.

Example:

```text
Partition 0:
Offset 0 -> TripCreated
Offset 1 -> TripAssigned
Offset 2 -> EmployeeBoarded
Offset 3 -> TripCompleted
```

Consumers track offsets to know where they have progressed.

## 12.2 Offset commit

A consumer group commits offsets to record its progress.

The choice of when to commit matters:

- Commit before processing: a failure can cause a record to be skipped.
- Commit after processing: a failure after processing but before commit can cause the record to be processed again.

This is why duplicate-safe processing is important.

## 12.3 At-most-once

A record is processed zero or one time from the application's perspective. Failures can result in lost processing.

## 12.4 At-least-once

A record is processed one or more times. Duplicate processing is possible.

This is common in reliable consumer designs. Consumers should be idempotent.

## 12.5 Exactly-once

Kafka supports exactly-once processing capabilities for specific Kafka read-process-write workflows using transactions and appropriate configuration.

However, exactly-once effects across arbitrary external systems—such as sending an email or updating a separate database—require additional design. Do not claim that enabling one Kafka setting automatically makes every external side effect exactly once.

## 12.6 Idempotency

An operation is idempotent when repeating it has the same intended effect as performing it once.

Example:

Instead of incrementing a trip's notification count every time an event arrives, record that a particular event ID has already been processed.

A consumer can maintain an inbox/deduplication table keyed by event ID to avoid applying the same business effect repeatedly.

---

# 13. Event-Driven Architecture

Event-driven architecture uses events to communicate that something happened.

## 13.1 Example workflow

When a trip is assigned:

1. Trip Service updates the trip.
2. It publishes `TripAssigned`.
3. Notification Service consumes the event and notifies the driver.
4. Analytics Service consumes the event and updates reporting data.

```text
                  Trip Service
                       |
                       | TripAssigned
                       v
                     Kafka
                 /           \
                /             \
               v               v
       Notification Service  Analytics Service
               |               |
               v               v
          Send alert       Update metrics
```

Trip Service does not need to know the internal implementation of each consumer.

## 13.2 Event vs command

- An **event** states that something has happened: `TripAssigned`.
- A **command** asks a component to do something: `AssignDriverToTrip`.

Events are generally facts. Commands express intent.

## 13.3 Eventual consistency

When services update their own databases independently, their views of business data may not become consistent at exactly the same instant.

For example:

- Trip Service commits the assignment.
- Notification Service processes the event a little later.
- Analytics Service updates its projection after that.

This is eventual consistency.

The system must define which operations require immediate consistency and which can tolerate delayed updates.

## 13.4 Event schema design

A useful event commonly includes:

- Event ID.
- Event type.
- Event timestamp.
- Aggregate/business identifier, such as `tripId`.
- Schema/version.
- Relevant event data.

Example:

```json
{
  "eventId": "evt-12345",
  "eventType": "TripAssigned",
  "eventVersion": 1,
  "occurredAt": "2026-11-08T12:00:00Z",
  "tripId": 501,
  "driverId": 15
}
```

Avoid placing unnecessary sensitive data in events. Treat event schemas as contracts that need compatibility planning.

---

# 14. Circuit Breaker — Resilience4j

In distributed systems, a remote service may become slow or unavailable.

Repeatedly calling a failing service can waste threads and connections and make the overall system unhealthy.

A circuit breaker helps stop calls temporarily when failures cross configured thresholds.

## 14.1 Circuit breaker states

Three common states:

1. CLOSED
2. OPEN
3. HALF_OPEN

### CLOSED

Calls are allowed. The circuit breaker monitors outcomes.

### OPEN

Calls are rejected quickly without invoking the remote service.

### HALF_OPEN

After a configured wait, a limited number of trial calls are allowed to determine whether the dependency has recovered.

## 14.2 Diagram

```text
          CLOSED
        Calls allowed
             |
       Failure threshold
             |
             v
           OPEN
      Calls rejected
             |
       Wait duration
             |
             v
         HALF_OPEN
        Trial calls
          /     \
      Success   Failure
        |          |
        v          v
      CLOSED     OPEN
```

## 14.3 Why use a circuit breaker?

- Prevent repeated calls to an unhealthy dependency.
- Reduce cascading failures.
- Fail quickly when a dependency is unavailable.
- Give the dependency time to recover.

A circuit breaker does not repair the remote service. It controls how the caller behaves during failure.

## 14.4 Resilience4j

Resilience4j provides fault-tolerance modules such as:

- Circuit Breaker.
- Retry.
- Rate Limiter.
- Bulkhead.
- Time Limiter.

It is commonly used in Spring applications for resilience policies.

---

# 15. Retry, Timeout, and Fallback

These mechanisms are related but solve different problems.

## 15.1 Timeout

A timeout limits how long the caller waits for a remote operation.

Without a timeout, a slow dependency may hold application resources for too long.

Always configure suitable connection and response/read timeouts for remote calls.

## 15.2 Retry

A retry repeats an operation after a failure.

Retries can help with temporary failures, such as a short network interruption.

However, retries can make an outage worse if every client retries aggressively.

Use:

- A bounded number of attempts.
- Backoff between attempts.
- Jitter where appropriate.
- Retry only for failures that are plausibly transient.
- Idempotency for operations that may be repeated.

Do not blindly retry non-idempotent operations such as creating a payment or booking unless the operation has a reliable idempotency mechanism.

## 15.3 Fallback

A fallback is an alternative response or behavior when the primary operation cannot complete.

Example:

If a non-critical analytics service is unavailable, the trip details endpoint might still return core trip information without analytics data.

A fallback should not silently pretend that a critical operation succeeded.

## 15.4 Comparison

| Mechanism | Purpose |
|---|---|
| Timeout | Stop waiting after a defined period |
| Retry | Attempt an operation again |
| Circuit breaker | Stop calling a failing dependency temporarily |
| Fallback | Provide alternative behavior |
| Bulkhead | Isolate resources so one dependency cannot consume everything |

## 15.5 Retry and circuit breaker together

Their ordering and configuration matter. Poorly configured retries can increase the number of calls made to an unhealthy service. Define a total request deadline and ensure nested retries do not multiply unexpectedly.

---

# 16. Distributed Transactions

A transaction across one database is relatively straightforward. A business operation spanning multiple services and databases is much harder.

## 16.1 The problem

Suppose a trip assignment involves:

- Trip Service updating the trip database.
- Driver Service updating driver availability.
- Notification Service sending a notification.

These operations may involve different databases and external systems.

A normal local `@Transactional` transaction cannot atomically commit all of these independent systems.

## 16.2 Why not use one database transaction?

A local transaction manager generally controls one transactional resource or a configured set of resources. A network call to another service does not automatically join the caller's database transaction.

Distributed transaction protocols such as two-phase commit exist, but they add coordination, operational, and availability tradeoffs and are not always appropriate for microservices.

## 16.3 Saga pattern

A Saga breaks a long-running business transaction into a sequence of local transactions.

If a later step fails, compensating actions can be executed for earlier completed steps.

Example:

```text
Step 1: Create trip
        |
        v
Step 2: Reserve driver
        |
        v
Step 3: Reserve vehicle
        |
        v
Step 4: Confirm trip
        |
        v
Step 5: Publish TripConfirmed
```

If vehicle reservation fails, the workflow may compensate by releasing the driver and marking the trip as requiring reassignment.

A compensation is a business action that counteracts an earlier action. It is not always a literal database rollback.

## 16.4 Choreography vs orchestration

### Choreography

Services react to events and publish subsequent events.

```text
TripCreated
    |
    v
Driver Service reserves driver
    |
    v
DriverReserved
    |
    v
Vehicle Service reserves vehicle
    |
    v
VehicleReserved
```

No central coordinator directs every step.

### Orchestration

A central orchestrator coordinates the workflow and tells participants which action to perform next.

```text
          Trip Saga Orchestrator
             /      |      \
            v       v       v
          Trip    Driver  Vehicle
         Service Service Service
```

## 16.5 Saga tradeoffs

- Requires explicit workflow state.
- Compensating actions must be designed.
- Events may be delayed or duplicated.
- Failure recovery and observability are important.
- Business operations may be eventually consistent.

---

# 17. Centralized Configuration — Spring Cloud Config

Microservices often need configuration such as:

- Database connection properties.
- Service URLs.
- Feature flags.
- Timeout settings.
- External API configuration.

Maintaining configuration separately in every service can become difficult.

Spring Cloud Config provides centralized configuration management.

## 17.1 Architecture

```text
                 Config Repository
                 (e.g. Git)
                       |
                       v
                Config Server
                       |
          +------------+------------+
          |            |            |
          v            v            v
       Trip Service Driver Service Auth Service
```

Services obtain environment-specific configuration through the Config Server according to the configured setup.

## 17.2 Benefits

- Centralized configuration.
- Environment-specific settings.
- Easier configuration governance.
- Reduced duplication.

## 17.3 Security considerations

- Do not store plaintext production secrets in a publicly accessible repository.
- Protect Config Server access.
- Integrate with a proper secrets-management solution.
- Understand refresh behavior and which properties can be changed dynamically.

Centralized configuration does not automatically mean every configuration change is instantly applied to every running service.

---

# 18. Distributed Tracing and Monitoring

A request may travel through several services.

For example:

```text
Flutter
  |
  v
API Gateway
  |
  v
Trip Service
  |
  v
Driver Service
  |
  v
Kafka
  |
  v
Notification Service
```

If the final notification is delayed, it can be difficult to identify where the delay occurred without observability.

## 18.1 Logs, metrics, and traces

| Signal | What it tells you |
|---|---|
| Logs | What happened at a point in time |
| Metrics | Numerical measurements over time |
| Traces | How one request or operation moved across components |

## 18.2 Distributed tracing

A trace represents an end-to-end operation. It contains spans representing work performed by individual components.

Example:

```text
Trace: Assign Trip
    |
    +--> Gateway span
    |
    +--> Trip Service span
    |       |
    |       +--> Database span
    |
    +--> Driver Service span
    |
    +--> Kafka publish span
```

A trace ID helps correlate spans across services.

## 18.3 OpenTelemetry

OpenTelemetry provides APIs, SDKs, and instrumentation for collecting traces, metrics, and logs.

It can export telemetry to compatible observability platforms.

## 18.4 Monitoring

Useful service metrics include:

- Request count.
- Request latency.
- Error rate.
- CPU and memory usage.
- Database connection-pool usage.
- Kafka consumer lag.
- Retry and circuit-breaker events.

Metrics help identify trends and service health before users report every issue.

---

# 19. Logging and Correlation IDs

Logs from multiple services are much more useful when related requests can be connected.

## 19.1 What is a correlation ID?

A correlation ID is an identifier associated with a request or workflow and propagated across services.

Example:

```text
Request enters API Gateway
Correlation-ID: req-abc-123

Trip Service:
Correlation-ID: req-abc-123

Driver Service:
Correlation-ID: req-abc-123
```

This allows an engineer to search logs across services using the same identifier.

## 19.2 Correlation ID vs trace ID

A correlation ID is a general identifier used to associate related operations.

A trace ID belongs to a distributed tracing context and is used by tracing instrumentation to connect spans.

They can be related, but they are not necessarily identical.

## 19.3 Logging best practices

- Use structured logs where possible.
- Include service name and environment.
- Include trace/correlation identifiers.
- Log relevant business identifiers carefully.
- Do not log passwords, tokens, or sensitive personal data.
- Avoid logging full request bodies indiscriminately.
- Use appropriate log levels.

---

# 20. Dockerizing Microservices

Docker packages an application and its runtime dependencies into an image.

A container is a running instance of an image.

## 20.1 Why Docker?

- Consistent runtime environment.
- Easier local development.
- Portable deployment artifact.
- Process and filesystem isolation.
- Useful for CI/CD and container orchestration.

## 20.2 Basic Dockerfile for Spring Boot

```dockerfile
FROM eclipse-temurin:21-jre

WORKDIR /app

COPY target/application.jar app.jar

EXPOSE 8080

ENTRYPOINT ["java", "-jar", "app.jar"]
```

This is a basic example. A production image should use a maintained, appropriately pinned base image and follow the organization's security and image-building practices.

## 20.3 Build and run

```bash
docker build -t trip-service:1.0 .
```

Run:

```bash
docker run -p 8081:8080 trip-service:1.0
```

The host port `8081` maps to the container port `8080`.

## 20.4 Docker Compose

Docker Compose can run multiple related containers locally.

Example conceptual file:

```yaml
services:
  trip-service:
    image: trip-service:1.0
    ports:
      - "8081:8080"
    environment:
      SPRING_PROFILES_ACTIVE: docker

  driver-service:
    image: driver-service:1.0
    ports:
      - "8082:8080"
    environment:
      SPRING_PROFILES_ACTIVE: docker
```

A real setup also needs suitable networking, database configuration, health checks, secrets handling, and dependency readiness.

## 20.5 Containers vs virtual machines

| Container | Virtual machine |
|---|---|
| Shares host OS kernel | Runs a guest operating system |
| Usually lighter-weight | Usually heavier |
| Starts quickly | Typically takes longer to boot |
| Isolates application processes | Provides virtualized machine environment |

Docker is a container platform; Kubernetes is a container orchestration platform.

---

# 21. Microservices Security

Security becomes more complex when several services communicate over a network.

## 21.1 Common requirements

- Authenticate external clients.
- Authorize access to each business operation.
- Protect service-to-service communication.
- Use HTTPS/TLS where appropriate.
- Validate tokens and claims.
- Avoid trusting client-supplied identity or roles.
- Protect internal service endpoints.
- Store secrets securely.
- Apply least privilege.

## 21.2 Gateway security is not enough

The gateway can authenticate requests, but internal services should not blindly trust any request merely because it came through the gateway.

Depending on the architecture, internal services may validate tokens themselves or use another trusted service identity mechanism.

## 21.3 Service-to-service authentication

Common approaches include:

- OAuth 2.0 client credentials.
- Mutual TLS.
- Signed service tokens.
- Platform/workload identity mechanisms.

Choose the approach based on the environment and threat model.

## 21.4 Network boundaries

Internal services should not be publicly exposed without a reason. Restrict network access and use identity-based authorization rather than relying only on network location.

---

# 22. Important Microservices Design Patterns

## 22.1 API Gateway pattern

Provides a common entry point for client requests and routes them to internal services.

## 22.2 Database per service

Each service owns its data boundary. Other services should access that data through an API or event contract rather than directly coupling to another service's tables.

This does not necessarily mean every service must have a separate physical database server. The important principle is independent ownership and controlled access.

## 22.3 Saga pattern

Coordinates a multi-step business transaction across services using local transactions and compensating actions.

## 22.4 Circuit Breaker pattern

Stops repeated calls to an unhealthy dependency temporarily.

## 22.5 CQRS

CQRS stands for Command Query Responsibility Segregation.

It separates models or paths used to change data (commands) from those used to read data (queries).

It can be useful when read and write workloads have different requirements, but it adds complexity and should be justified.

## 22.6 Event Sourcing

Event Sourcing stores state changes as a sequence of events and derives current state by replaying or projecting those events.

It is different from merely publishing domain events to Kafka. A system can use event-driven messaging without using Event Sourcing as its persistence model.

## 22.7 Strangler pattern

The Strangler pattern gradually replaces parts of a legacy application with new services or components while the old system continues to operate during migration.

It can reduce the risk of a large all-at-once rewrite.

---

# 23. Spring Cloud Version Compatibility

Spring Cloud components must be compatible with the Spring Boot version used by the application.

For a Spring Boot 4.x project:

- Use a Spring Cloud release train that explicitly supports the selected Spring Boot 4.x version.
- Import the corresponding Spring Cloud BOM rather than independently guessing versions for every component.
- Check the current Spring Cloud compatibility matrix before selecting dependencies.
- Do not copy old tutorials blindly; starter artifact names and supported modules can change between release trains.

For example, Spring Cloud Gateway's newer WebFlux server starter naming differs from older tutorials. Verify the artifact name and compatibility for the exact release train being used.

**Interview point:** Dependency compatibility is part of backend engineering. A correct architectural design can still fail at build or runtime if incompatible framework versions are combined.

---

# 24. Common Mistakes to Avoid

1. **Assuming microservices are always better than monoliths.** They introduce distributed-system complexity.
2. **Splitting services by technical layer.** Service boundaries should generally follow business capabilities.
3. **Sharing one database schema freely across services.** This creates data ownership and deployment coupling.
4. **Hardcoding service IP addresses.** Use stable service discovery or platform DNS where appropriate.
5. **Making every service call synchronous.** This can create long call chains and cascading failures.
6. **Using Kafka without considering duplicates.** Consumers should be idempotent.
7. **Assuming Kafka provides one total order across a topic.** Ordering is per partition.
8. **Adding more consumers than partitions and expecting all to be active.** A group cannot have more active partition assignments than partitions.
9. **Committing offsets before processing without accepting loss risk.** Choose offset behavior deliberately.
10. **Claiming exactly-once behavior for external side effects.** Kafka transactions do not automatically make database writes or emails exactly once.
11. **Retrying every failure.** Retries can amplify outages and duplicate non-idempotent actions.
12. **Using a circuit breaker without timeouts.** A call can still occupy resources while waiting.
13. **Treating a Saga compensation as a database rollback.** Compensation is a new business action.
14. **Putting all business logic in the API Gateway.** Keep domain ownership in services.
15. **Logging tokens or sensitive data.** Protect user and service credentials.
16. **Ignoring observability.** Distributed failures are difficult to diagnose without logs, metrics, and traces.
17. **Using incompatible Spring Boot and Spring Cloud versions.** Import a compatible BOM and check the release matrix.
18. **Using Docker as if it were Kubernetes.** Docker packages/runs containers; Kubernetes orchestrates them.

---

# 25. Interview Questions and Answers

## Q1. What is a microservice?

A microservice is a small, independently deployable service organized around a business capability. It communicates with other services through defined APIs or messaging contracts.

## Q2. What is the difference between monolith and microservices?

A monolith is deployed as one application unit. Microservices split business capabilities into independently deployable services. Microservices support independent scaling and releases but introduce network, data consistency, deployment, and observability complexity.

## Q3. What is service discovery?

Service discovery allows services to find available instances dynamically rather than relying on hardcoded network addresses.

## Q4. What is Eureka?

Eureka is a service registry used in some Spring Cloud architectures. Service instances register with it, and clients can discover available instances.

## Q5. What is an API Gateway?

An API Gateway is an entry point that routes client requests to backend services and can handle cross-cutting concerns such as authentication integration, rate limiting, and request logging.

## Q6. What is load balancing?

Load balancing distributes requests across multiple service instances to improve resource utilization, throughput, and availability.

## Q7. What is OpenFeign?

OpenFeign is a declarative HTTP client. A Java interface annotated with `@FeignClient` describes remote endpoints, and Spring Cloud provides the client implementation.

## Q8. What is the difference between synchronous and asynchronous communication?

Synchronous communication waits for a response. Asynchronous communication sends a message or event and allows processing to happen independently later.

## Q9. What is Kafka?

Kafka is a distributed event-streaming platform used to publish, store, and consume event records at scale.

## Q10. What is a Kafka topic?

A topic is a named stream of records. Producers write to topics and consumers read from them.

## Q11. What is a Kafka partition?

A partition is an ordered log within a topic. Partitions allow data distribution and parallel consumption. Ordering is guaranteed within a partition, not across all partitions of a topic.

## Q12. What is a Kafka broker?

A broker is a Kafka server that stores and serves topic partitions.

## Q13. What is a consumer group?

A consumer group is a set of consumers that coordinate to consume a topic. Partitions are assigned among consumers in the group.

## Q14. What happens if there are more consumers than partitions?

Some consumers remain idle for that topic because a partition is assigned to at most one consumer in the group at a time under normal group assignment.

## Q15. What is an offset?

An offset is a record's position in a partition. Consumer groups commit offsets to track their processing progress.

## Q16. What is the difference between at-most-once and at-least-once delivery?

At-most-once can lose processing but avoids reprocessing from the delivery model. At-least-once avoids losing acknowledged work under its assumptions but can produce duplicates, so consumers should be idempotent.

## Q17. Does Kafka guarantee exactly-once processing?

Kafka supports exactly-once capabilities for particular Kafka read-process-write workflows using transactions and suitable configuration. It does not automatically guarantee exactly-once side effects in arbitrary external systems.

## Q18. What is event-driven architecture?

It is an architecture in which components communicate by publishing and consuming events that represent things that have happened.

## Q19. What is eventual consistency?

It means independently updated parts of a distributed system may temporarily show different states but converge after events or updates are processed.

## Q20. What is a circuit breaker?

A circuit breaker monitors failures and temporarily stops calls to an unhealthy dependency. It commonly uses CLOSED, OPEN, and HALF_OPEN states.

## Q21. What is the difference between retry and circuit breaker?

Retry attempts an operation again after a failure. A circuit breaker stops calls to a dependency when failure conditions cross a threshold.

## Q22. What is the Saga pattern?

Saga coordinates a business transaction across services as a sequence of local transactions. If a later step fails, compensating actions can counteract earlier completed steps.

## Q23. What is the difference between Saga choreography and orchestration?

Choreography uses events and service reactions without a central coordinator. Orchestration uses a central component to direct the workflow.

## Q24. What is distributed tracing?

Distributed tracing follows an operation across multiple services using traces and spans, helping identify latency and failures across service boundaries.

## Q25. What is a correlation ID?

It is an identifier propagated across related operations so logs and diagnostic records from multiple services can be associated.

## Q26. What is Docker?

Docker is a platform for building and running containers. Containers package applications and dependencies into portable runtime units.

## Q27. What is the difference between Docker and Kubernetes?

Docker is commonly used to build and run containers. Kubernetes orchestrates containers across a cluster, including scheduling, scaling, and service management.

## Q28. What is database per service?

It is a microservices principle in which each service owns its persistence model and controls access to its data through APIs or events rather than allowing other services to couple directly to its tables.

## Q29. What is CQRS?

CQRS separates the model or path used for commands (changes) from the model or path used for queries (reads). It can help when read and write requirements differ significantly.

## Q30. What is Event Sourcing?

Event Sourcing persists a sequence of events representing state changes and derives current state from those events. It is not the same as simply publishing events to Kafka.

---

# 26. Quick Revision Sheet

| Concept | Remember |
|---|---|
| Monolith | One deployable application unit |
| Microservices | Independently deployable business-capability services |
| Service discovery | Finds service instances dynamically |
| Eureka | Service registry |
| API Gateway | Common client entry point and routing layer |
| Load balancing | Distributes requests across instances |
| OpenFeign | Declarative HTTP client |
| Synchronous | Caller waits for response |
| Asynchronous | Work continues through messages/events |
| Kafka | Distributed event-streaming platform |
| Topic | Named stream of records |
| Partition | Ordered log within a topic |
| Broker | Kafka server |
| Producer | Publishes records |
| Consumer | Reads records |
| Consumer group | Consumers sharing topic processing |
| Offset | Position in a partition |
| At-most-once | Possible loss, no redelivery guarantee |
| At-least-once | Possible duplicates |
| Idempotency | Repetition has the same intended effect |
| Event-driven architecture | Components react to published events |
| Circuit breaker | Stops calls to an unhealthy dependency |
| Retry | Attempts operation again |
| Timeout | Limits waiting duration |
| Fallback | Alternative behavior |
| Saga | Distributed workflow using local transactions and compensation |
| Config Server | Centralized configuration |
| Distributed tracing | Follows work across services |
| Correlation ID | Connects related logs/operations |
| Docker | Builds and runs containers |
| Kubernetes | Orchestrates containers |
| CQRS | Separates command and query models |
| Event Sourcing | Stores state changes as events |

---

# 27. Day 06 Completion Checklist

- [ ] I can explain monolithic architecture and microservices.
- [ ] I can explain when microservices are useful and when they are unnecessary.
- [ ] I understand synchronous REST and asynchronous messaging.
- [ ] I can explain service discovery and Eureka.
- [ ] I can explain API Gateway responsibilities.
- [ ] I can distinguish client-side and server-side load balancing.
- [ ] I understand how OpenFeign works.
- [ ] I can explain Kafka topics, brokers, and partitions.
- [ ] I understand producers and consumers.
- [ ] I can explain consumer groups and partition assignment.
- [ ] I understand offsets and commit behavior.
- [ ] I can distinguish at-most-once, at-least-once, and exactly-once claims.
- [ ] I understand idempotent consumers.
- [ ] I can explain event-driven architecture and eventual consistency.
- [ ] I understand circuit breaker states.
- [ ] I can distinguish timeout, retry, fallback, and bulkhead.
- [ ] I can explain distributed transactions and Saga.
- [ ] I can compare Saga choreography and orchestration.
- [ ] I understand centralized configuration.
- [ ] I can explain logs, metrics, traces, and correlation IDs.
- [ ] I understand Docker images, containers, and Compose.
- [ ] I can explain service security and database ownership.
- [ ] I can distinguish CQRS from Event Sourcing.
- [ ] I know to check Spring Boot/Spring Cloud compatibility.

---

## Final microservices architecture to remember

```text
                         FLUTTER CLIENT
                               |
                               v
                          API GATEWAY
                               |
             +-----------------+-----------------+
             |                 |                 |
             v                 v                 v
        AUTH SERVICE     EMPLOYEE SERVICE    TRIP SERVICE
             |                 |                 |
             v                 v                 v
           AUTH DB         EMPLOYEE DB         TRIP DB
                                                   |
                                      +------------+-----------+
                                      |                        |
                                      v                        v
                               DRIVER SERVICE             KAFKA
                                      |                        |
                                      v                 +------+------+
                                  DRIVER DB              |             |
                                                        v             v
                                                  NOTIFICATION    ANALYTICS
                                                     SERVICE       SERVICE

Cross-cutting infrastructure:
- Service discovery / platform DNS
- Load balancing
- Centralized configuration
- Timeouts, retries, circuit breakers
- Logs, metrics, distributed tracing
- Docker / container orchestration
```

**The key idea:** Microservices divide a system around business capabilities, but they introduce network failures, independent data ownership, eventual consistency, and operational complexity. REST and OpenFeign support synchronous communication; Kafka supports event-driven asynchronous workflows. Resilience patterns, Saga, observability, security, and containerization help make distributed systems reliable and operable.

---

## Version compatibility note

This learning material uses current architectural concepts, but framework dependencies must be selected for the exact versions in your project. For Spring Boot 4.x, verify the compatible Spring Cloud release train and use its BOM. Do not copy dependency versions or starter artifact names from older Spring Boot tutorials without checking compatibility.
'''

path = Path("/mnt/data/Day_06_Microservices_Kafka_Docker.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
