# Enterprise Operating System: Comprehensive Technical Specification

## 1. System Goal
This document outlines the technical specification for a fully integrated, AI-powered, on-premises enterprise operating system. The system will unify all business functions—including IT management, security, HR, finance, and operations—into a single, centrally managed platform, replacing all disparate systems.

## 2. System Architecture
### 2.1. Macro Architecture
The system is built on a modular, service-oriented architecture, with a central "Core" server that orchestrates all operations. The Core is composed of several independent but interconnected modules, each responsible for a specific domain.

- **Central Server Unit ("Core"):**
    - **Hardware:**
        - **CPU:** 32+ Cores
        - **RAM:** 128-512 GB
        - **Storage:** RAID-10 SSD array for high availability and performance.
        - **Power:** Redundant Power Supplies
    - **Network:** Dedicated management network, plus VLANs for each department.
    - **OS:** Custom Linux distribution with a C++ backend.
- **Server Modules:**
    - **Identity Core:** Manages users, roles, permissions, and authentication tokens.
    - **Security Core:** AI-driven monitoring, firewall management, logging, and anomaly detection.
    - **Access Core:** Manages physical access via face recognition and NFC.
    - **Device Core:** Manages all network devices (PCs, terminals, routers, IoT).
    - **Deployment Core:** Handles OS provisioning, updates, and configuration.
    - **File Core:** Manages documents, versioning, and access rights.
    - **Accounting Core:** Manages payroll, invoices, and financial flows.
    - **HR Core:** Manages employee records, vacation, and recruitment.
    - **Ticket Core:** Manages internal ticketing and workflows.
    - **Monitoring Core:** Monitors hardware health, network load, and environmental sensors.
    - **Messaging Core:** Secure internal email and messaging.
    - **AI Core:** Provides machine learning models for behavior analysis and risk assessment.
    - **Dashboard Core:** Renders the web-based UIs for different roles.

### 2.2. Micro Architecture & Communication
Each module is a self-contained C++ service with a RESTful API over HTTPS for external communication and a ZeroMQ-based message bus for internal, asynchronous communication. This ensures loose coupling and high scalability.

- **Module Components:**
    - C++ Backend Service
    - RESTful API (HTTPS) & ZeroMQ Pub/Sub Sockets
    - Dedicated Database Schema
    - Asynchronous Worker Processes
    - Event Handlers
    - RBAC Logic

- **Communication Protocols:**
    - **External:** All external communication with the system (e.g., from the web dashboard) will be done via the REST API over HTTPS. All API endpoints will require a valid session token for authentication.
    - **Internal:** Internal communication between modules will be done via a ZeroMQ message bus. This will be used for asynchronous tasks such as triggering a deployment or notifying the security module of a potential threat. All messages on the bus will be signed and encrypted.

## 3. Database Schema
This section defines the PostgreSQL database schema.

### 3.1. Identity & Access Management
- **users**
    - `id` (UUID, PK)
    - `username` (VARCHAR, UNIQUE)
    - `password_hash` (VARCHAR)
    - `role_id` (UUID, FK to roles.id)
    - `face_vector` (BYTEA)
    - `nfc_token` (VARCHAR, UNIQUE)
    - `created_at` (TIMESTAMPTZ)
    - `updated_at` (TIMESTAMPTZ)
- **roles**
    - `id` (UUID, PK)
    - `name` (VARCHAR, UNIQUE)
    - `hierarchy_level` (INTEGER)
- **permissions**
    - `id` (UUID, PK)
    - `name` (VARCHAR, UNIQUE)
- **role_permissions**
    - `role_id` (UUID, FK to roles.id)
    - `permission_id` (UUID, FK to permissions.id)

### 3.2. Device Management
- **devices**
    - `id` (UUID, PK)
    - `hostname` (VARCHAR, UNIQUE)
    - `type` (VARCHAR) -- e.g., 'laptop', 'terminal', 'server'
    - `assigned_user_id` (UUID, FK to users.id)
    - `ip_address` (INET)
    - `mac_address` (MACADDR, UNIQUE)
    - `status` (VARCHAR) -- e.g., 'online', 'offline', 'decommissioned'
    - `last_seen` (TIMESTAMPTZ)

### 3.3. Deployment
- **os_images**
    - `id` (UUID, PK)
    - `name` (VARCHAR)
    - `version` (VARCHAR)
    - `role_id` (UUID, FK to roles.id)
    - `file_path` (VARCHAR)
- **deployment_jobs**
    - `id` (UUID, PK)
    - `device_id` (UUID, FK to devices.id)
    - `os_image_id` (UUID, FK to os_images.id)
    - `status` (VARCHAR) -- e.g., 'pending', 'in_progress', 'success', 'failed'
    - `created_at` (TIMESTAMPTZ)
    - `completed_at` (TIMESTAMPTZ)

## 4. API Specification
### 4.1. REST API (HTTPS)
- **Authentication**
    - `POST /api/auth/login` - Authenticate and receive a session token.
    - `POST /api/auth/logout` - Invalidate a session token.
- **Users**
    - `GET /api/users` - List all users.
    - `POST /api/users` - Create a new user.
    - `GET /api/users/{id}` - Get user details.
    - `PUT /api/users/{id}` - Update a user.
    - `DELETE /api/users/{id}` - Delete a user.
- **Devices**
    - `GET /api/devices` - List all devices.
    - `POST /api/devices` - Register a new device.
    - `GET /api/devices/{id}` - Get device details.
    - `PUT /api/devices/{id}` - Update a device.
    - `DELETE /api/devices/{id}` - Decommission a device.
- **Deployment**
    - `POST /api/deploy` - Trigger an OS deployment to a device.

### 4.2. ZeroMQ Message Bus (Internal)
- **Topic: `deployment.jobs`**
    - `{"action": "new", "job_id": "...", "device_id": "...", "image_id": "..."}`
    - `{"action": "update", "job_id": "...", "status": "..."}`
- **Topic: `security.events`**
    - `{"event_type": "failed_login", "username": "...", "ip_address": "..."}`
    - `{"event_type": "unauthorized_access", "user_id": "...", "resource": "..."}`
- **Topic: `device.status`**
    - `{"device_id": "...", "status": "online/offline", "timestamp": "..."}`

## 5. Security Architecture
### 5.1. Public Key Infrastructure (PKI)
An internal Certificate Authority (CA) will be established to issue and manage X.509 certificates for all services, devices, and users. This will ensure that all communication is authenticated and encrypted.

### 5.2. Encryption
- **Data at Rest:** All data will be encrypted at rest using LUKS 2 full-disk encryption.
- **Data in Transit:** All data will be encrypted in transit using TLS 1.3.
- **Sensitive Data:** Sensitive data, such as NFC tokens and face vectors, will be additionally encrypted at the application level using AES-256.

### 5.3. Authentication Flow
1.  **User Login:** Users will authenticate to their workstations using a combination of their password and face recognition.
2.  **Session Token:** Upon successful authentication, the Identity Core will issue a short-lived JSON Web Token (JWT) to the user.
3.  **API Requests:** All subsequent API requests must include the JWT in the `Authorization` header.
4.  **Inter-Service Communication:** All messages on the ZeroMQ bus will be signed with the sending service's private key and encrypted with the recipient service's public key.

## 6. Network Topology
### 6.1. VLANs and Network Segmentation
The network will be segmented into multiple VLANs to isolate traffic and improve security.
- **Management VLAN:** For communication between the Core server and other infrastructure components.
- **Deployment VLAN:** For OS provisioning and deployment.
- **Departmental VLANs:** Each department (HR, Finance, etc.) will have its own VLAN.
- **Guest VLAN:** For guest and IoT devices.

### 6.2. Firewall Rules
- **Default Deny:** All traffic between VLANs will be denied by default.
- **Role-Based Access:** Firewall rules will be dynamically configured based on the user's role and the principle of least privilege.
- **Deep Packet Inspection:** All traffic will be subject to deep packet inspection to detect and block malicious activity.

## 7. Technology Stack
- **Backend:** C/C++
- **Server OS:** Custom Linux
- **Message Bus:** ZeroMQ
- **API:** REST over HTTPS
- **Database:** PostgreSQL for structured data, MongoDB for unstructured data.
- **AI/ML:** Custom models built with TensorFlow and PyTorch.
- **PKI:** Custom internal certificate authority.
- **Boot:** PXE/iPXE with signed images.
- **Encryption:** AES-256, TLS 1.3, LUKS 2.
