from pathlib import Path

content = r'''# Day 04 — Spring Security, Authentication, Authorization, and JWT

> **Learning track:** Java Backend Developer Interview Preparation  
> **Day:** 04  
> **Focus:** Securing Spring Boot REST APIs  
> **Examples:** Employee Transport backend  
> **Stack assumed:** Java 21, Spring Boot 4.x, Spring Security, Jakarta APIs

---

## Learning objectives

By the end of this day, you should be able to explain:

1. Why backend applications need security.
2. The difference between authentication and authorization.
3. How Spring Security processes an HTTP request.
4. The roles of `SecurityFilterChain`, `AuthenticationManager`, `AuthenticationProvider`, and `UserDetailsService`.
5. How password hashing works and why BCrypt is used.
6. How JWT-based authentication works.
7. The difference between access tokens and refresh tokens.
8. How a JWT authentication filter works.
9. How role-based and method-level authorization are configured.
10. The differences between CORS and CSRF.
11. How authentication and authorization failures are returned to clients.
12. Security practices expected in production backend systems.

---

# 1. Why Do We Need Spring Security?

A backend API often handles private or sensitive information. In an Employee Transport application, examples include:

- Employee profile information.
- Driver and vehicle details.
- Trip schedules and assignments.
- Login credentials.
- Admin operations.
- Employee boarding and pickup status.

If every endpoint is publicly accessible, an unknown client could read data or perform operations that should be restricted.

Security helps answer two important questions:

1. **Who is making this request?**
2. **What is this user allowed to do?**

Example:

- An employee can view their own trip details.
- A driver can view assigned pickup information and update pickup progress.
- A company admin can manage employees, drivers, vehicles, and trips.

These permissions should be enforced by the backend. Hiding a button in Flutter is not sufficient security because a client can send HTTP requests independently of the UI.

## 1.1 What is Spring Security?

Spring Security is a framework that provides authentication, authorization, and protection against common application security threats.

It integrates with Spring applications through a filter-based architecture.

It can help with:

- Login and authentication.
- Password verification.
- Role- and permission-based authorization.
- JWT and other authentication mechanisms.
- CSRF protection.
- Security headers.
- CORS integration.
- Session and security-context management.

---

# 2. Authentication vs Authorization

These are two fundamental security concepts and are frequently asked in interviews.

## 2.1 Authentication

Authentication verifies the identity of a user.

It answers:

> Who are you?

Example:

An employee submits an email and password. The backend verifies the credentials. If they are valid, the user is authenticated.

## 2.2 Authorization

Authorization determines what an authenticated user is allowed to do.

It answers:

> What are you allowed to access?

Example:

- An employee can view permitted employee resources.
- A driver can update their own trip progress.
- An admin can create or deactivate employees.

## 2.3 Comparison

| Authentication | Authorization |
|---|---|
| Verifies identity | Checks permissions |
| Happens before access is granted | Evaluates whether an action is permitted |
| Example: verify email/password or token | Example: check ADMIN role |
| Failure commonly results in 401 | Failure commonly results in 403 |

A user can be authenticated but still not authorized to access a particular endpoint.

## 2.4 Example request flow

```text
Client sends request
        |
        v
Is the user authenticated?
        |
   +----+----+
   |         |
  No        Yes
   |         |
  401        v
       Is the user authorized?
              |
         +----+----+
         |         |
        No        Yes
         |         |
        403        v
              Execute request
```

---

# 3. Spring Security Architecture

Spring Security uses a chain of servlet filters to process incoming requests.

Understanding the main components is important for backend interviews.

## 3.1 High-level architecture

```text
HTTP Request
     |
     v
Servlet Container
     |
     v
DelegatingFilterProxy
     |
     v
FilterChainProxy
     |
     v
SecurityFilterChain
     |
     +--> Security context handling
     |
     +--> Authentication filters
     |
     +--> Exception translation
     |
     +--> Authorization checks
     |
     v
DispatcherServlet
     |
     v
Controller
```

The exact filters and their order depend on the security configuration and Spring Security version.

## 3.2 `DelegatingFilterProxy`

The servlet container invokes a filter named `DelegatingFilterProxy`. It delegates to Spring Security's filter infrastructure managed by the Spring application context.

## 3.3 `FilterChainProxy`

`FilterChainProxy` is the central Spring Security filter. It selects the applicable `SecurityFilterChain` for a request and delegates request processing to that chain.

An application can define multiple security filter chains for different URL patterns or security requirements.

## 3.4 `SecurityFilterChain`

A `SecurityFilterChain` defines the filters and security behavior applied to matching requests.

Modern Spring Security configuration commonly defines it as a bean.

```java
@Configuration
@EnableWebSecurity
public class SecurityConfig {

    @Bean
    public SecurityFilterChain securityFilterChain(
            HttpSecurity http) throws Exception {

        http
            .authorizeHttpRequests(auth -> auth
                .requestMatchers("/api/auth/**").permitAll()
                .anyRequest().authenticated()
            );

        return http.build();
    }
}
```

This is a basic illustration. A production configuration must also deliberately decide how authentication is performed, how CSRF is handled, how CORS is configured, and how errors are returned.

## 3.5 `Authentication`

`Authentication` represents an authentication request or the authenticated principal.

It can contain:

- Principal: identity of the user.
- Credentials: for example, a password during login.
- Authorities: granted roles or permissions.
- Authentication status.

After successful authentication, Spring Security stores the resulting `Authentication` in the `SecurityContext`.

## 3.6 `AuthenticationManager`

`AuthenticationManager` is the main abstraction responsible for processing an authentication request.

Its common implementation is `ProviderManager`, which delegates to one or more authentication providers.

## 3.7 `AuthenticationProvider`

An `AuthenticationProvider` performs a particular type of authentication.

For username/password authentication, `DaoAuthenticationProvider` commonly works with:

- `UserDetailsService` to load the user.
- `PasswordEncoder` to verify the submitted password.

Other providers can support different authentication mechanisms.

## 3.8 `UserDetailsService`

`UserDetailsService` loads user-specific information, usually by username.

```java
@Service
public class CustomUserDetailsService
        implements UserDetailsService {

    private final UserRepository userRepository;

    public CustomUserDetailsService(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    @Override
    public UserDetails loadUserByUsername(String email) {
        User user = userRepository.findByEmail(email)
                .orElseThrow(() ->
                        new UsernameNotFoundException("User not found"));

        return org.springframework.security.core.userdetails.User
                .withUsername(user.getEmail())
                .password(user.getPasswordHash())
                .authorities(user.getRole())
                .build();
    }
}
```

This example assumes that the stored role string is already in the correct authority format. Role naming and mapping should be consistent across the application.

## 3.9 `SecurityContextHolder`

`SecurityContextHolder` provides access to the current security context.

Example:

```java
Authentication authentication =
        SecurityContextHolder.getContext().getAuthentication();

String username = authentication.getName();
```

In the usual servlet configuration, the security context is associated with the current request-processing thread. Do not assume it automatically propagates to arbitrary asynchronous tasks.

---

# 4. Password Hashing and `PasswordEncoder`

Passwords must not be stored as plain text.

## 4.1 Why not store plain-text passwords?

Suppose the database contains:

```text
email                 password
rahul@example.com     Rahul@123
```

If the database is exposed, the attackers immediately obtain users' passwords.

Instead, store a password hash produced by a password-hashing algorithm.

## 4.2 Hashing vs encryption

| Hashing | Encryption |
|---|---|
| Designed to be one-way | Designed to be reversible with the key |
| Used for password verification | Used when data must later be recovered |
| No decryption step | Decryption is possible with the appropriate key |

Passwords should generally be hashed using a password-hashing algorithm, not encrypted for later decryption.

## 4.3 BCrypt

BCrypt is a password-hashing algorithm designed to be computationally expensive. It uses a salt and a configurable work factor.

Spring Security provides `BCryptPasswordEncoder`.

```java
@Configuration
public class PasswordConfig {

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }
}
```

Hash a password:

```java
String encodedPassword =
        passwordEncoder.encode("StrongPassword@123");
```

Verify a submitted password:

```java
boolean matches = passwordEncoder.matches(
        submittedPassword,
        storedPasswordHash
);
```

Important:

- Do not compare a newly generated BCrypt hash with the stored hash using `equals()`.
- BCrypt uses a salt, so encoding the same raw password can produce different encoded strings.
- Use `matches(rawPassword, storedHash)` to verify the password.

## 4.4 Password storage flow

```text
Registration
    |
    v
User submits raw password
    |
    v
PasswordEncoder.encode(...)
    |
    v
Store encoded hash in database


Login
    |
    v
User submits raw password
    |
    v
Load stored password hash
    |
    v
PasswordEncoder.matches(raw, hash)
    |
    +--> true: credentials valid
    |
    +--> false: reject login
```

Never log raw passwords or return password hashes in API responses.

---

# 5. JWT Authentication

JWT stands for **JSON Web Token**.

It is a compact token format commonly used to carry claims between parties. In a typical API authentication design, the backend issues a signed token after successful login, and the client sends it with later requests.

## 5.1 Why use JWT?

A token-based approach can be useful for mobile and REST clients.

Typical flow:

1. User submits login credentials.
2. Backend verifies the credentials.
3. Backend issues a signed access token.
4. Client stores the token using an appropriate secure storage mechanism.
5. Client sends the token with later API requests.
6. Backend validates the token and establishes the authenticated security context for that request.

JWT authentication can be stateless from the server's session perspective, but this does not mean there is no server-side user data, token revocation mechanism, or authorization state.

## 5.2 JWT structure

A signed JWT commonly has three Base64URL-encoded parts separated by dots:

```text
HEADER.PAYLOAD.SIGNATURE
```

### Header

Contains token metadata, commonly the signing algorithm and token type.

Example:

```json
{
  "alg": "HS256",
  "typ": "JWT"
}
```

### Payload

Contains claims, such as:

```json
{
  "sub": "rahul@example.com",
  "role": "EMPLOYEE",
  "iat": 1794144000,
  "exp": 1794147600
}
```

- `sub`: subject, often the user identifier.
- `iat`: issued-at time.
- `exp`: expiration time.

### Signature

The signature is calculated using the encoded header, encoded payload, and signing key according to the selected algorithm.

It helps the backend detect whether the token has been modified.

## 5.3 Important JWT security point

A typical signed JWT is **not encrypted**.

Anyone who obtains the token can usually decode and read its header and payload. The signature protects integrity; it does not hide the claims.

Therefore:

- Never put passwords in a JWT payload.
- Avoid putting unnecessary sensitive personal data in claims.
- Use HTTPS.
- Keep signing keys secret.
- Validate signature, expiration, issuer, audience, and other required claims.

## 5.4 JWT authentication flow

```text
             LOGIN
               |
               v
     Client sends credentials
               |
               v
       Spring Security verifies
       username + password
               |
          +----+----+
          |         |
        Invalid    Valid
          |         |
          v         v
       Reject    Generate JWT
                    |
                    v
              Return token
                    |
                    v
             Client stores token
                    |
                    v
        Later request includes:
        Authorization: Bearer <JWT>
                    |
                    v
             JWT filter validates
                    |
                    v
          Set SecurityContext
                    |
                    v
           Authorization check
                    |
                    v
             Controller executes
```

---

# 6. Access Token vs Refresh Token

Many authentication systems use two different token types.

## 6.1 Access token

An access token is sent with API requests to access protected resources.

Typical properties:

- Short lifetime.
- Sent in the `Authorization` header.
- Contains the claims needed for access decisions.
- Must be validated on protected requests.

## 6.2 Refresh token

A refresh token is used to obtain a new access token without asking the user to enter credentials again.

Typical properties:

- Longer lifetime than an access token.
- Used only with a refresh endpoint.
- Must be protected carefully.
- Often stored and tracked server-side so it can be revoked or rotated.

## 6.3 Comparison

| Access Token | Refresh Token |
|---|---|
| Used to access protected APIs | Used to obtain a new access token |
| Usually short-lived | Usually longer-lived |
| Sent with normal API requests | Sent to refresh endpoint only |
| Exposure can permit API access until expiry/revocation | Exposure can permit repeated token renewal if not controlled |

## 6.4 Refresh-token rotation

A common safer design rotates refresh tokens:

1. Client sends a valid refresh token.
2. Backend validates it.
3. Backend issues a new access token and a new refresh token.
4. Backend invalidates the old refresh token.
5. Reuse of an already-rotated token can be treated as suspicious and may cause the token family/session to be revoked.

The exact design depends on the application's threat model and token storage architecture.

## 6.5 Logout and JWT

A self-contained access token can remain cryptographically valid until it expires unless the backend uses a revocation strategy.

Possible strategies include:

- Short-lived access tokens.
- Revocable refresh tokens.
- Token/session versioning.
- A denylist for selected tokens.
- Key rotation for broader invalidation.

A logout endpoint should invalidate the refresh-token/session state where the design supports it. Simply deleting a token from the mobile client does not invalidate a copy that an attacker may already have obtained.

---

# 7. JWT Implementation Architecture in Spring Boot

A JWT-based Spring Security implementation commonly has these components:

```text
security/
|
+-- SecurityConfig.java
|
+-- CustomUserDetailsService.java
|
+-- JwtService.java
|
+-- JwtAuthenticationFilter.java
|
+-- AuthController.java
|
+-- AuthService.java
```

## 7.1 `SecurityConfig`

Responsibilities:

- Define the `SecurityFilterChain`.
- Configure public and protected routes.
- Configure authentication and authorization rules.
- Add the JWT filter at the appropriate place.
- Configure CORS and CSRF behavior deliberately.
- Configure exception handling.

Illustrative configuration:

```java
@Configuration
@EnableWebSecurity
public class SecurityConfig {

    private final JwtAuthenticationFilter jwtFilter;

    public SecurityConfig(JwtAuthenticationFilter jwtFilter) {
        this.jwtFilter = jwtFilter;
    }

    @Bean
    public SecurityFilterChain securityFilterChain(
            HttpSecurity http) throws Exception {

        http
            .csrf(csrf -> csrf.disable())
            .sessionManagement(session -> session
                .sessionCreationPolicy(SessionCreationPolicy.STATELESS)
            )
            .authorizeHttpRequests(auth -> auth
                .requestMatchers("/api/auth/**").permitAll()
                .requestMatchers("/api/admin/**").hasRole("ADMIN")
                .anyRequest().authenticated()
            )
            .addFilterBefore(
                jwtFilter,
                UsernamePasswordAuthenticationFilter.class
            );

        return http.build();
    }
}
```

This is a learning example, not a universal production configuration. Disabling CSRF is appropriate only when the authentication design does not rely on automatically attached browser credentials such as cookies, and the application's threat model has been considered. Configure CORS separately where needed.

## 7.2 `JwtService`

Responsibilities commonly include:

- Generate tokens.
- Extract claims.
- Read subject/user identifier.
- Check token expiration.
- Validate token signature and required claims.

Keep token parsing and cryptographic logic in a focused component rather than spreading it through controllers.

## 7.3 `JwtAuthenticationFilter`

The JWT filter commonly:

1. Reads the `Authorization` header.
2. Checks for the `Bearer ` prefix.
3. Extracts the token.
4. Validates its signature and expiration.
5. Extracts the user identity.
6. Loads user details if required by the design.
7. Creates an authenticated `Authentication`.
8. Stores it in the `SecurityContextHolder`.
9. Continues the filter chain.

A simplified conceptual example:

```java
@Component
public class JwtAuthenticationFilter
        extends OncePerRequestFilter {

    private final JwtService jwtService;
    private final UserDetailsService userDetailsService;

    public JwtAuthenticationFilter(
            JwtService jwtService,
            UserDetailsService userDetailsService) {
        this.jwtService = jwtService;
        this.userDetailsService = userDetailsService;
    }

    @Override
    protected void doFilterInternal(
            HttpServletRequest request,
            HttpServletResponse response,
            FilterChain filterChain)
            throws ServletException, IOException {

        String header = request.getHeader("Authorization");

        if (header == null || !header.startsWith("Bearer ")) {
            filterChain.doFilter(request, response);
            return;
        }

        String token = header.substring(7);
        String username = jwtService.extractUsername(token);

        if (username != null &&
                SecurityContextHolder.getContext().getAuthentication() == null) {

            UserDetails user =
                    userDetailsService.loadUserByUsername(username);

            if (jwtService.isTokenValid(token, user)) {
                UsernamePasswordAuthenticationToken authentication =
                        new UsernamePasswordAuthenticationToken(
                                user,
                                null,
                                user.getAuthorities()
                        );

                authentication.setDetails(
                        new WebAuthenticationDetailsSource()
                                .buildDetails(request)
                );

                SecurityContextHolder.getContext()
                        .setAuthentication(authentication);
            }
        }

        filterChain.doFilter(request, response);
    }
}
```

This is a structural example. A production filter must handle malformed, expired, and invalid tokens safely, and its exception behavior must be coordinated with the configured authentication entry point. Token parsing should verify the signature and all required claims, not merely decode the payload.

## 7.4 Why extend `OncePerRequestFilter`?

`OncePerRequestFilter` is a Spring base class designed to ensure the filter logic is applied once per request dispatch under its documented dispatch behavior.

It is commonly used for custom authentication filters such as JWT filters.

## 7.5 Why add the JWT filter before `UsernamePasswordAuthenticationFilter`?

The JWT filter needs to inspect the bearer token and establish the authenticated security context before authorization is evaluated for the request.

The exact filter ordering should be chosen according to the authentication mechanism and Spring Security configuration.

---

# 8. Role-Based Authorization

After authentication, the backend must determine what the user can do.

## 8.1 What is a role?

A role is a named collection or category of permissions.

Example roles in an Employee Transport application:

- `ADMIN`
- `DRIVER`
- `EMPLOYEE`
- `VENDOR`

A basic authorization matrix might look like:

| Operation | ADMIN | DRIVER | EMPLOYEE | VENDOR |
|---|---:|---:|---:|---:|
| Manage employees | Yes | No | No | No |
| Manage vehicles | Yes | No | No | No |
| View assigned trip | Yes | Yes | Limited | Limited |
| Update pickup progress | Yes | Yes | No | No |
| View own profile | Yes | Yes | Yes | Yes |

The real policy must be based on business requirements. For example, an employee should generally be restricted to their own data, not merely allowed because they have the `EMPLOYEE` role.

## 8.2 URL-based authorization

```java
.authorizeHttpRequests(auth -> auth
    .requestMatchers("/api/auth/**").permitAll()
    .requestMatchers("/api/admin/**").hasRole("ADMIN")
    .requestMatchers("/api/drivers/**")
        .hasAnyRole("ADMIN", "DRIVER")
    .anyRequest().authenticated()
)
```

Important distinction:

- `hasRole("ADMIN")` conventionally checks for the authority `ROLE_ADMIN`.
- `hasAuthority("ROLE_ADMIN")` checks the exact authority string.
- `hasAuthority("TRIP_ASSIGN")` can be used for a more granular permission name.

Avoid accidentally mixing role strings with or without the `ROLE_` prefix.

## 8.3 Method-level security

Enable method security:

```java
@Configuration
@EnableMethodSecurity
public class MethodSecurityConfig {
}
```

Then protect service methods:

```java
@Service
public class EmployeeService {

    @PreAuthorize("hasRole('ADMIN')")
    public void deactivateEmployee(Long employeeId) {
        // Business operation
    }
}
```

You can also use expressions involving method arguments and the authenticated principal. Be careful to ensure object-level access checks cannot be bypassed by changing an ID in the request.

## 8.4 Role vs permission

A role is a broad category. A permission/authority can represent a specific action.

Example:

```text
ROLE_ADMIN
    |
    +--> EMPLOYEE_READ
    +--> EMPLOYEE_CREATE
    +--> EMPLOYEE_UPDATE
    +--> EMPLOYEE_DELETE
```

For small applications, role-based checks may be sufficient. Larger applications may benefit from more granular permissions.

---

# 9. CORS vs CSRF

These two concepts are often confused.

## 9.1 What is CORS?

CORS stands for **Cross-Origin Resource Sharing**.

Browsers enforce a same-origin policy. CORS is a browser security mechanism that lets a server declare which cross-origin browser requests are permitted.

An origin is made up of:

- Scheme, such as `http` or `https`.
- Host.
- Port.

For example, these are different origins:

```text
http://localhost:3000
http://localhost:8080
```

A browser may send a preflight `OPTIONS` request before certain cross-origin requests. The backend must respond with appropriate CORS headers.

CORS is primarily a browser-enforced policy. It is not a replacement for authentication or authorization, and it does not prevent non-browser clients from sending requests.

## 9.2 Configure CORS in Spring Security

A simplified example:

```java
@Bean
CorsConfigurationSource corsConfigurationSource() {

    CorsConfiguration configuration = new CorsConfiguration();

    configuration.setAllowedOrigins(
            List.of("http://localhost:3000")
    );

    configuration.setAllowedMethods(
            List.of("GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS")
    );

    configuration.setAllowedHeaders(
            List.of("Authorization", "Content-Type")
    );

    configuration.setAllowCredentials(false);

    UrlBasedCorsConfigurationSource source =
            new UrlBasedCorsConfigurationSource();

    source.registerCorsConfiguration("/**", configuration);

    return source;
}
```

Then enable CORS integration in the security configuration:

```java
http.cors(Customizer.withDefaults());
```

Use the actual trusted frontend origins in each environment. Do not blindly allow every origin, especially when credentials are enabled.

## 9.3 What is CSRF?

CSRF stands for **Cross-Site Request Forgery**.

It is an attack in which a browser is tricked into sending an unwanted authenticated request to a site where the user is already logged in.

It is especially relevant when browsers automatically attach credentials, such as session cookies.

## 9.4 CORS vs CSRF comparison

| CORS | CSRF |
|---|---|
| Controls browser access to cross-origin responses | Protects against forged requests using a user's authenticated browser context |
| Browser-enforced cross-origin policy | Request-forgery threat |
| Uses server response headers and sometimes preflight | Common defenses include CSRF tokens and suitable cookie settings |
| Does not replace API authentication | Does not replace authorization |

## 9.5 Should CSRF be disabled for a JWT API?

Do not answer this as an unconditional rule.

- If the application uses browser cookies or other automatically attached credentials, CSRF protection may be necessary.
- If the API uses bearer tokens explicitly attached by the client and does not rely on ambient browser credentials, the CSRF risk profile is different.
- A Flutter native mobile client is not subject to the browser's same-origin policy in the same way as Flutter Web.
- Flutter Web runs in a browser and must account for CORS and browser security behavior.

The correct configuration depends on where and how credentials are stored and sent.

---

# 10. Authentication and Authorization Error Handling

A secure API should return appropriate status codes without leaking internal details.

## 10.1 401 Unauthorized

Despite its name, 401 means the request has not been successfully authenticated.

Examples:

- Missing access token for a protected endpoint.
- Invalid token.
- Expired token.

Spring Security uses an `AuthenticationEntryPoint` to commence authentication when authentication is required but missing or invalid.

## 10.2 403 Forbidden

403 means the user is authenticated but is not allowed to perform the operation.

Spring Security uses an `AccessDeniedHandler` for access-denied situations.

## 10.3 Custom `AuthenticationEntryPoint`

Conceptual example:

```java
@Component
public class CustomAuthenticationEntryPoint
        implements AuthenticationEntryPoint {

    @Override
    public void commence(
            HttpServletRequest request,
            HttpServletResponse response,
            AuthenticationException exception)
            throws IOException {

        response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
        response.setContentType("application/json");

        response.getWriter().write(
                "{\"status\":401,\"message\":\"Authentication required\"}"
        );
    }
}
```

## 10.4 Custom `AccessDeniedHandler`

```java
@Component
public class CustomAccessDeniedHandler
        implements AccessDeniedHandler {

    @Override
    public void handle(
            HttpServletRequest request,
            HttpServletResponse response,
            AccessDeniedException exception)
            throws IOException {

        response.setStatus(HttpServletResponse.SC_FORBIDDEN);
        response.setContentType("application/json");

        response.getWriter().write(
                "{\"status\":403,\"message\":\"Access denied\"}"
        );
    }
}
```

These handlers can be configured through the `exceptionHandling` section of `HttpSecurity`.

## 10.5 Why not rely only on `@RestControllerAdvice`?

Spring Security filters execute before a request reaches the controller. Authentication and authorization failures can therefore occur before controller-level exception handling is involved.

Use Spring Security's entry point and access-denied handler for security-filter failures. Use `@RestControllerAdvice` for application/controller-layer exceptions.

---

# 11. Security Best Practices

Security is not just about adding JWT. A production backend needs multiple layers of protection.

## 11.1 Password security

- Never store plain-text passwords.
- Use a suitable password-hashing algorithm such as BCrypt or another supported password-hashing scheme.
- Use `PasswordEncoder.matches()` to verify passwords.
- Never log passwords.
- Never return password hashes in API responses.

## 11.2 Token security

- Use HTTPS.
- Keep signing keys outside source control.
- Use strong, appropriately managed keys.
- Set reasonable access-token expiry.
- Validate signature, expiry, issuer, and audience as appropriate.
- Avoid putting sensitive information in JWT claims.
- Protect refresh tokens and support revocation/rotation where required.

## 11.3 Authorization security

- Follow least privilege.
- Protect every sensitive endpoint on the backend.
- Do not trust role values supplied by the client.
- Enforce ownership checks for user-specific resources.
- Avoid relying only on frontend route guards or hidden buttons.

## 11.4 API security

- Validate incoming request data.
- Use parameterized database queries.
- Configure CORS to trusted origins.
- Apply rate limiting where appropriate.
- Use secure HTTP headers.
- Avoid exposing stack traces or internal exception messages.
- Keep dependencies updated.
- Restrict management and actuator endpoints.
- Avoid exposing unnecessary API documentation or debug endpoints publicly.

## 11.5 Secrets management

Do not hardcode these in source code or commit them to Git:

- JWT signing secrets.
- Database passwords.
- API keys.
- Refresh-token signing keys.
- Third-party credentials.

Use environment variables or a suitable secrets-management system. Rotate credentials if they are exposed.

## 11.6 Least privilege example

An employee should not be able to deactivate another employee simply by changing an ID in the URL.

A secure design checks both:

1. Whether the user has permission for the operation.
2. Whether the target resource belongs to that user or is within the user's permitted scope.

Role checks alone are not always sufficient.

---

# 12. Understanding a Spring Boot Default Security Configuration

When Spring Security is present but the application has no custom security configuration, Spring Boot/Spring Security may apply a default security setup.

A startup log may show a generated development password and an in-memory user configuration.

This tells you that a default user configuration has been created for the development setup. It does **not** prove that the application has implemented JWT authentication.

For an actual project, inspect:

- Security configuration classes.
- `SecurityFilterChain` beans.
- `UserDetailsService` implementations.
- Authentication providers.
- Password encoder beans.
- Login controller and service.
- JWT service and filter, if present.
- Route authorization rules.

Do not infer that JWT is implemented merely because Spring Security is on the classpath.

---

# 13. Common Mistakes to Avoid

1. **Confusing authentication and authorization.** Identity verification and permission checks are different.
2. **Storing plain-text passwords.** Store password hashes.
3. **Using `equals()` to compare BCrypt hashes.** Use `PasswordEncoder.matches()`.
4. **Assuming JWT payload is encrypted.** A signed JWT is normally readable by anyone who has it.
5. **Putting sensitive data in token claims.** Keep claims minimal.
6. **Using long-lived access tokens without a reason.** Token lifetime is a security tradeoff.
7. **Trusting the frontend to enforce roles.** Enforce permissions on the backend.
8. **Allowing all CORS origins without considering credentials.** Allow only the origins required by the application.
9. **Disabling CSRF without understanding credential transport.** Decide based on the actual authentication design.
10. **Treating CORS as authentication.** CORS does not authenticate a user.
11. **Returning the same status for all security failures.** Distinguish 401 from 403.
12. **Handling filter failures only with `@RestControllerAdvice`.** Security filters run before controller handling.
13. **Committing secrets to Git.** Keep signing keys and credentials outside source control.
14. **Checking roles but not resource ownership.** Prevent insecure direct object reference patterns.
15. **Assuming a generated Spring Security password means JWT is active.** Inspect the actual security implementation.

---

# 14. Interview Questions and Answers

## Q1. What is Spring Security?

Spring Security is a framework for authentication, authorization, and protection against common security threats in Spring applications. It integrates with servlet applications through a filter chain.

## Q2. What is the difference between authentication and authorization?

Authentication verifies who the user is. Authorization determines what an authenticated user is allowed to do.

## Q3. What is `SecurityFilterChain`?

It defines the security filters and rules applied to matching HTTP requests. It is commonly configured as a bean using `HttpSecurity`.

## Q4. What is `FilterChainProxy`?

It is Spring Security's central filter that selects the applicable `SecurityFilterChain` and delegates request processing to it.

## Q5. What is `AuthenticationManager`?

It is the main abstraction for processing authentication requests. A common implementation is `ProviderManager`, which delegates to configured authentication providers.

## Q6. What is `AuthenticationProvider`?

It performs a specific authentication mechanism. For username/password authentication, a provider can load user details and verify the password using a password encoder.

## Q7. What is `UserDetailsService`?

It loads user-specific information, commonly by username. Spring Security can use it during username/password authentication.

## Q8. What is `SecurityContextHolder`?

It provides access to the current `SecurityContext`, which contains the current `Authentication` and its principal and authorities.

## Q9. Why should passwords be hashed?

Hashing prevents the database from storing directly usable passwords. A password-hashing algorithm makes offline guessing more expensive and allows verification without recovering the original password.

## Q10. What is BCrypt?

BCrypt is a salted, adaptive password-hashing algorithm with a configurable work factor. Spring Security provides `BCryptPasswordEncoder`.

## Q11. What is JWT?

JWT is a compact token format containing claims. A signed JWT lets the backend verify token integrity and can carry identity and authorization-related claims.

## Q12. What are the three parts of a signed JWT?

Header, payload, and signature.

## Q13. Is JWT encrypted?

Not necessarily. A typical signed JWT is readable after Base64URL decoding. Its signature protects integrity, not confidentiality.

## Q14. What is the difference between access and refresh tokens?

An access token is used to access protected APIs and is usually short-lived. A refresh token is used to obtain new access tokens and is typically longer-lived and more carefully controlled.

## Q15. What is `OncePerRequestFilter`?

It is a Spring filter base class commonly used when custom filter logic should run once per request dispatch under its documented behavior. JWT authentication filters often extend it.

## Q16. Why is a JWT filter added before `UsernamePasswordAuthenticationFilter`?

The JWT filter needs to validate the bearer token and establish the security context before authorization checks are performed for the request.

## Q17. What is the difference between `hasRole` and `hasAuthority`?

`hasRole("ADMIN")` conventionally checks the `ROLE_ADMIN` authority. `hasAuthority("ROLE_ADMIN")` checks the exact authority string.

## Q18. What is method-level security?

It protects methods using annotations such as `@PreAuthorize`. It can enforce authorization at the service-method level in addition to URL-level rules.

## Q19. What is CORS?

CORS is a browser security mechanism that allows a server to specify which cross-origin browser requests are permitted to access its responses.

## Q20. What is CSRF?

CSRF is an attack that tricks a user's browser into sending an unwanted authenticated request to a site where the user is already logged in.

## Q21. What is the difference between CORS and CSRF?

CORS controls browser cross-origin access to responses. CSRF protection helps prevent forged requests that exploit automatically attached browser credentials.

## Q22. What is the difference between 401 and 403?

401 indicates that authentication is missing or unsuccessful. 403 indicates that the user is authenticated but does not have permission.

## Q23. What is `AuthenticationEntryPoint`?

It handles the start of authentication when a protected resource is requested without successful authentication. It commonly returns a 401 response for REST APIs.

## Q24. What is `AccessDeniedHandler`?

It handles access-denied situations, commonly returning 403 when an authenticated user lacks permission.

## Q25. Does JWT make an application automatically secure?

No. The application must still validate tokens correctly, protect signing keys, enforce authorization, validate input, use HTTPS, manage token expiry/revocation, and secure resource ownership.

---

# 15. Quick Revision Sheet

| Concept | Remember |
|---|---|
| Spring Security | Authentication, authorization, and security protections |
| Authentication | Who are you? |
| Authorization | What can you access? |
| `SecurityFilterChain` | Security filters and rules for matching requests |
| `FilterChainProxy` | Selects and delegates to a security filter chain |
| `AuthenticationManager` | Processes authentication requests |
| `AuthenticationProvider` | Performs a specific authentication mechanism |
| `UserDetailsService` | Loads user details |
| `SecurityContextHolder` | Access to current security context |
| `PasswordEncoder` | Encodes and verifies password hashes |
| BCrypt | Salted adaptive password hashing |
| JWT | Compact token carrying claims |
| JWT parts | Header, payload, signature |
| Access token | Used for protected API access |
| Refresh token | Used to obtain a new access token |
| `OncePerRequestFilter` | Common base for a JWT request filter |
| `hasRole` | Conventionally checks `ROLE_` prefixed authority |
| `hasAuthority` | Checks exact authority |
| `@PreAuthorize` | Method-level authorization |
| CORS | Browser cross-origin access policy |
| CSRF | Cross-site request forgery |
| 401 | Authentication unsuccessful/missing |
| 403 | Authenticated but forbidden |
| `AuthenticationEntryPoint` | Handles authentication commencement |
| `AccessDeniedHandler` | Handles authorization denial |

---

# 16. Day 04 Completion Checklist

- [ ] I can explain why backend APIs need Spring Security.
- [ ] I can distinguish authentication from authorization.
- [ ] I can explain the Spring Security filter architecture.
- [ ] I understand `SecurityFilterChain` and `FilterChainProxy`.
- [ ] I understand `AuthenticationManager` and `AuthenticationProvider`.
- [ ] I can explain the role of `UserDetailsService`.
- [ ] I understand `SecurityContextHolder`.
- [ ] I can explain password hashing and BCrypt.
- [ ] I can explain JWT structure and the purpose of each part.
- [ ] I know that a signed JWT is not automatically encrypted.
- [ ] I can explain access tokens and refresh tokens.
- [ ] I understand the purpose of a JWT authentication filter.
- [ ] I can explain role-based and method-level authorization.
- [ ] I can distinguish CORS from CSRF.
- [ ] I can distinguish 401 from 403.
- [ ] I understand `AuthenticationEntryPoint` and `AccessDeniedHandler`.
- [ ] I can name important production security practices.

---

## Final security architecture to remember

```text
                 FLUTTER CLIENT
                       |
                       | Login credentials
                       v
                AUTH CONTROLLER
                       |
                       v
                 AUTH SERVICE
                       |
                       v
              AUTHENTICATION MANAGER
                       |
                       v
              AUTHENTICATION PROVIDER
                       |
              +--------+--------+
              |                 |
              v                 v
       UserDetailsService   PasswordEncoder
              |                 |
              v                 v
          USER DATABASE     Verify hash
              |
              v
       Authentication success
              |
              v
          Generate JWT
              |
              v
        Return access token
              |
              v
       Client sends Bearer token
              |
              v
       JWT AUTHENTICATION FILTER
              |
              v
       Validate signature/claims
              |
              v
       Set SecurityContext
              |
              v
        AUTHORIZATION RULES
              |
        +-----+------+
        |            |
      Denied       Allowed
        |            |
       403            v
                 CONTROLLER
                     |
                     v
                   SERVICE
                     |
                     v
                 REPOSITORY
                     |
                     v
                 DATABASE
```

**The key idea:** Authentication establishes the user's identity. Authorization enforces what that identity is allowed to do. Spring Security applies these checks through its filter chain, while JWT can carry signed identity claims between requests. Password hashing, secure token handling, backend permission checks, and careful CORS/CSRF configuration are all part of a secure backend.
'''

path = Path("/mnt/data/Day_04_Spring_Security_JWT.md")
path.write_text(content, encoding="utf-8")
print(f"Created: {path}\nWords: {len(content.split()):,}\nCharacters: {len(content):,}")
