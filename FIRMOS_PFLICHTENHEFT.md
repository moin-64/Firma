# FirmOS: Vollständiges Technisches Pflichtenheft

## 1. Einleitung und Systemvision
Dieses Dokument definiert die vollständige technische Spezifikation für die Konzeption, Entwicklung und Implementierung des "FirmOS", eines vollständig integrierten, autarken Unternehmens-Betriebssystems. Die Systemvision ist die Schaffung eines zentralisierten, hochsicheren und KI-gestützten IT-Ökosystems, das sämtliche Geschäftsprozesse des Unternehmens in einer einzigen On-Premises-Plattform bündelt. Das FirmOS ersetzt Insellösungen für Identitäts- und Zugriffsmanagement, OS-Provisioning, Personalwesen (HR), Finanzen, Ticketing, physische Zutrittskontrolle und weitere Unternehmensanwendungen. Das System ist für den Betrieb auf einer massiven, redundanten und hochverfügbaren Server-Infrastruktur mit dedizierten GPU-Clustern konzipiert und unterliegt strengsten Sicherheits- und Datenschutzanforderungen, insbesondere der DSGVO, basierend auf einer Zero-Trust-Architektur.

## 2. Physische und Netzwerk-Infrastruktur
### 2.1. Hardware-Infrastruktur
Das Fundament des FirmOS bildet ein zentrales Serverrack (42-HE-Schränke), das für maximale Rechenleistung, Redundanz und Ausfallsicherheit ausgelegt ist. Alle Komponenten sind für den Dauerbetrieb in einem Rechenzentrumsumfeld konzipiert.
- **Netzwerk-Backbone:** Das Kernnetzwerk wird durch Enterprise-Router und -Switches von Ubiquiti (UniFi-Serie oder äquivalent) realisiert. Es bietet 10/25/40/100-GbE-Verbindungen für High-Speed-Datenübertragung zwischen den Server-Komponenten. Redundante Glasfaser-Links zwischen den Core-Switches stellen die Ausfallsicherheit des Netzwerks sicher.
- **Compute-Nodes:** Mehrere hochleistungsfähige Server-Nodes mit Multi-Core-CPUs (z.B. AMD EPYC oder Intel Xeon Scalable), mindestens 256 GB ECC-RAM und redundanten Netzteilen dienen der Ausführung der Kern-Microservices des FirmOS.
- **GPU-Cluster:** Ein dedizierter Cluster, ausgestattet mit NVIDIA A100/H100 Tensor Core GPUs oder vergleichbaren Beschleunigern, ist für sämtliche KI- und Machine-Learning-Aufgaben zuständig. Die GPUs sind über NVLink oder eine äquivalente PCIe-Fabric-Technologie für maximale Bandbreite bei parallelen Rechenoperationen verbunden.
- **Datenspeicher:** Ein hybrides Speichersystem wird implementiert, bestehend aus hochperformanten NVMe-SSD-Arrays für den schnellen Zugriff auf operative Daten (z.B. Datenbanken) und kosteneffizienten HDD-Arrays für Langzeitspeicherung, Archivierung und Backups. Die Speicher-Cluster werden mit ZFS oder Ceph verwaltet, um Datenintegrität, Snapshots und Skalierbarkeit zu gewährleisten.
- **Ausfallsicherheit:** Die gesamte Hardware-Infrastruktur wird durch überdimensionierte USV-Module abgesichert, die bei einem Stromausfall einen autonomen Betrieb von mehreren Stunden ermöglichen. Redundante Kühlsysteme und ein engmaschiges Netz von Monitoring-Sensoren überwachen kontinuierlich Temperatur, Last und Durchsatz aller kritischen Komponenten.

### 2.2. Netzwerk-Architektur
Die Netzwerk-Architektur basiert konsequent auf einem Zero-Trust-Modell, bei dem keinem Gerät oder Benutzer standardmäßig vertraut wird.
- **Netzwerksegmentierung:** Das Netzwerk ist vollständig in logische VLANs pro Abteilung (Entwicklung, Finanzen, HR, Produktion, IT, Marketing etc.) segmentiert. Die Kommunikation zwischen den VLANs ist standardmäßig untersagt und wird nur durch explizite Firewall-Regeln gestattet (Default-Deny-Policy).
- **Mikrosegmentierung und SDN:** Ein Software-Defined-Network (SDN) Controller ermöglicht die dynamische Anpassung von Routing-Regeln und die Mikrosegmentierung auf Service-Ebene innerhalb des Kubernetes-Clusters, um die laterale Ausbreitung von Bedrohungen zu verhindern.
- **Sicherheits-Appliances:** Eine Next-Generation Firewall (NGFW) im Core des Netzwerks analysiert den gesamten Traffic zwischen den Segmenten. Zusätzlich werden perimeter- und hostbasierte Firewalls eingesetzt. Ein Intrusion-Detection-/-Prevention-System (IDS/IPS) auf Basis von Suricata oder Zeek überwacht den Netzwerkverkehr in Echtzeit auf Anomalien und bekannte Angriffsmuster.
- **Netzwerkdienste:** Dedizierte, hochverfügbare PXE/iPXE-Server stehen für das vollautomatische Netzwerk-Boot und OS-Provisioning der FirmOS-Clients bereit. VPN-Gateways (SSL/IPSec) mit zwingender Multi-Faktor-Authentifizierung (MFA) ermöglichen einen sicheren Remote-Zugriff für autorisierte Mitarbeiter.
- **Quality of Service (QoS):** QoS-Regeln stellen sicher, dass sicherheitskritische Datenströme, wie die der Face-Recognition-Terminals, Alarmsysteme und der internen Echtzeit-Kommunikation, priorisiert behandelt werden.

## 3. FirmOS – Die Unternehmens-Linux-Distribution
Eine unternehmensspezifisch gehärtete und optimierte Linux-Distribution, basierend auf Ubuntu Server LTS oder Rocky Linux, wird auf allen Arbeitsplätzen und als Basis für die Server-Dienste eingesetzt.
- **Dynamische Konfiguration:** Bei jedem Login wird das System automatisch und dynamisch an die Rolle des Benutzers angepasst. Software, Berechtigungen und Netzwerkkonfigurationen werden in Echtzeit geladen.
- **Sicherheit und Isolation:** Die Distribution unterstützt von Haus aus verschlüsselte Container (z.B. via Podman oder Docker mit LUKS-Encryption) zur sicheren Ausführung von Anwendungen. System-Updates werden zentral gesteuert und automatisch vom Server verteilt.
- **Integrierte Agenten:** Ein integrierter KI-Assistent, der auf dem internen GPU-Cluster läuft, steht jedem Mitarbeiter zur Verfügung. Ein Monitoring-Daemon und ein Logging-Agent übermitteln kontinuierlich System- und Sicherheitsmetriken an die zentralen Monitoring-Server.
- **Konnektivität und Identität:** Ein vorinstallierter Identity-Agent zur nahtlosen Anbindung an den Authentifizierungs-Server (Keycloak/Authentik) und ein VPN-Client, der sich bei Bedarf automatisch verbindet, sind feste Bestandteile des Systems.
- **Software-Bereitstellung:** Jeder Arbeitsplatz erhält ausschließlich die für seine Rolle autorisierten und zentral bereitgestellten Software-Pakete. Die Installation von nicht genehmigter Software wird auf Systemebene unterbunden.

## 4. Server-Komponenten und -Dienste
### 4.1. Authentifizierungs-Server
- **Identity & Access Management (IAM):** Eine gehärtete Instanz von Keycloak oder Authentik dient als zentrale Instanz für das gesamte Identity Management.
- **Multi-Faktor-Authentifizierung (MFA):** Für alle Logins ist eine Kombination aus Passwort, Gesichtserkennung und optional einem Hardware-Token (FIDO2/WebAuthn) oder NFC-Token zwingend erforderlich.
- **Protokolle:** Das System unterstützt moderne Authentifizierungs- und Autorisierungsprotokolle wie OAuth2, SAML und OpenID Connect (OIDC).
- **Hierarchische Rechte:** Das Rechtesystem ist streng hierarchisch aufgebaut. Rechte werden von übergeordneten Rollen an untergeordnete vererbt. Administratoren können keine Rechte an Rollen vergeben, die ihrer eigenen Hierarchiestufe entsprechen oder darüber liegen.
- **Biometrische Datenverwaltung:** Die Gesichtsdaten der Mitarbeiter werden als verschlüsselte Vektoren DSGVO-konform in der Datenbank gespeichert und ausschließlich für die Authentifizierung an PCs und Zutrittsterminals verwendet.

### 4.2. Datenbankserver
- **Technologie:** Ein hochverfügbarer Cluster aus PostgreSQL-Servern mit Streaming-Replikation und TDE (Transparent Data Encryption) auf Dateisystemebene.
- **Datenbankschema:** Das Schema umfasst detaillierte Tabellen für Mitarbeiter, Rollen, Berechtigungen, physische Zutritte, Geräte (Clients, Server, Terminals), Audit-Logs, Räume/Sicherheitszonen sowie alle Daten der Fachanwendungen (Finanzen, HR, Tickets, Dokumente).
- **Datenlebenszyklus:** Automatische Löschfristen (z.B. nach 5 Jahren für bestimmte Protokolldaten) sind implementiert, um die Einhaltung von Datenschutzrichtlinien und gesetzlichen Aufbewahrungsfristen zu gewährleisten.

### 4.3. Applikationsserver
- **Architektur:** Die gesamte Anwendungslogik ist als ein System von Microservices auf einem Kubernetes-Cluster implementiert.
- **Backend und API:** Ein in C++ geschriebenes Backend stellt performante REST-APIs für die verschiedenen Dashboards (Admin, Geschäftsführung, Abteilungen) bereit. Ein API-Gateway (z.B. Kong oder Traefik) managt den externen Zugriff, erzwingt Authentifizierung und Ratenbegrenzung.
- **Asynchrone Verarbeitung:** Eine Message-Queue-basierte Architektur mit Workern (z.B. Celery oder eine C++-Implementierung mit RabbitMQ/NATS) verarbeitet zeitintensive Aufgaben wie Berichtsgenerierung oder Datenanalysen asynchron.
- **Service-Kommunikation:** Die Kommunikation zwischen den Microservices im Kubernetes-Cluster ist zwingend mittels mTLS (mutual TLS) verschlüsselt.

### 4.4. KI-Server / GPU-Cluster
Alle KI-Modelle laufen ausschließlich auf dem internen GPU-Cluster. Es findet kein Datenaustausch mit externen KI-Diensten statt.
- **Kernfunktionen:**
    - **Face-Recognition:** Hochpräzise Gesichtserkennung inklusive Liveness-Detection zur Abwehr von Spoofing-Angriffen.
    - **Dokumentenverarbeitung:** OCR- und NLP-Modelle zur automatischen Erfassung und Klassifizierung von Rechnungen, Verträgen und anderen Dokumenten.
    - **Analyse und Forecasting:** KI-Modelle zur Analyse von Finanzdaten, zur Erstellung von Unternehmens-Forecasts und zur Erkennung von Anomalien in Netzwerkverkehr, Zutrittsprotokollen und Finanztransaktionen.
- **Management und MLOps:** MLflow oder Kubeflow wird zur Verwaltung des gesamten Lebenszyklus der Modelle, von der Datenvorbereitung über das Training bis hin zum Deployment und Monitoring, eingesetzt.
- **Edge-Inference:** Für Echtzeitanwendungen wie die Liveness-Detection auf Zutrittsterminals werden quantisierte Modelle (TensorFlow Lite/ONNX) auf der Edge-Hardware ausgeführt.

### 4.5. File- und Backup-Server
- **Speicherarchitektur:** Ein hochverfügbarer RAID-Verbund mit ZFS oder Ceph ermöglicht Funktionen wie Snapshots zur schnellen Wiederherstellung und eine revisionssichere Dokumentenarchivierung gemäß GoBD.
- **Backup-Strategie:** Tägliche, automatisierte und verschlüsselte Backups werden an zwei physisch getrennte, sichere Standorte übertragen, um eine geographische Redundanz zu gewährleisten.

## 5. Rollen und Zugriffsrechte (RBAC)
Das System implementiert ein strenges, hierarchisches Role-Based Access Control (RBAC)-Modell. Jede Rolle hat präzise definierte Zugriffsrechte.
- **Geschäftsführung/CEO:** Erhält Vollzugriff auf alle strategischen Dashboards (read-only für technische Systeme), KI-generierte Berichte, alle Abteilungsmetriken und kritische Sicherheitsmeldungen. Definiert und gibt kritische, systemweite Prozesse frei.
- **HR/Personalwesen:** Hat exklusiven Zugriff auf das Personalverwaltungsmodul, Bewerbermanagement und die Lohn- und Gehaltsabrechnung (höchste Sicherheitsstufe). Jeglicher Zugriff auf die Finanzbuchhaltung ist ausgeschlossen.
- **Finanzen/Controlling:** Arbeitet mit der Buchhaltungssoftware, dem Rechnungssystem, Kostenanalysen und Steuer-Tools. Jeglicher Zugriff auf sensible HR-Daten ist ausgeschlossen.
- **IT/Systemadministration:** Administriert das System über die Admin-Konsole, verwaltet Benutzer, Netzwerke, Server und das OS-Provisioning. Hat Zugriff auf das Security Center mit Logs und Alerts. Kann keine Rechte über der eigenen Hierarchiestufe vergeben und hat keinen Zugriff auf persönliche Inhalte von Mitarbeitern.
- **Weitere Rollen:** Spezifische, auf das Notwendigste beschränkte Zugriffsrechte für Einkauf (Lieferantenmanagement), Vertrieb (CRM), Marketing (Kampagnentools), Produktion (Produktions-Dashboard), F&E (CAD, Git) und Logistik (Lagerverwaltung).

## 6. Face-Recognition und Zutrittssystem
Das physische Zutrittssystem ist vollständig in das FirmOS integriert.
- **Terminal-Hardware:** Die Terminals sind mit HD-Kameras, Infrarot- und Tiefensensoren sowie NFC-Lesern ausgestattet.
- **Authentifizierungsprozess:** Die Gesichtserkennung läuft auf dem zentralen KI-Server und beinhaltet eine obligatorische Liveness-Detection, um Foto- und Video-Spoofing zu verhindern.
- **Zutrittslogik:** Die Zutrittsrechte sind dynamisch an die Rolle, die Hierarchie, die Uhrzeit und die definierte Sicherheitszone gekoppelt. Bestimmte Hochsicherheitsbereiche sind nur für ausgewählte Mitarbeitergruppen zugänglich.
- **NFC-Fallback:** NFC-Token dienen als Notfall-Alternative bei Ausfall der Biometrie und werden ausschließlich von der HR-Abteilung und in Notfällen ausgegeben und verwaltet.
- **Sicherheit und Logging:** Alle Zutrittsversuche (erfolgreich und fehlgeschlagen) sowie Sperrungen werden vollständig und revisionssicher geloggt. Eine KI erkennt Tailgating (das unbefugte Hindurchschlüpfen einer zweiten Person) und Manipulationsversuche an den Kameras in Echtzeit und löst automatisch einen Alarm aus.

## 7. OS-Provisioning und Gerätemanagement
Der Prozess zur Bereitstellung neuer Arbeitsplätze ist vollständig automatisiert.
- **Workflow:** Ein IT-Administrator wählt im Admin-Dashboard die Abteilung, den Mitarbeiter und den Gerätetyp aus.
- **Automatisierte Image-Generierung:** Der Server erstellt daraufhin automatisch ein maßgeschneidertes, gehärtetes Linux-Image. Dieses Image enthält alle rollenspezifischen Software-Pakete, die notwendigen Zertifikate für die Authentifizierung im Netzwerk, spezifische Firewallregeln, die korrekten Berechtigungen und die Face-Login-Integration.
- **Deployment-Prozess:** Der Flash-Prozess wird über PXE/Netboot initiiert. Der Client bootet ins Netzwerk, erhält das Image vom Server und installiert es vollautomatisch.
- **Admin-Bridge-Funktion:** Administratoren können über ihre eigene, bereits installierte FirmOS-Distribution als "Brücke" fungieren, um andere PCs direkt mit dem Provisioning-Server zu verbinden und den Flash-Prozess zu initiieren.
- **Geräteregistrierung:** Jedes erfolgreich provisionierte Gerät erhält ein einzigartiges Gerätezertifikat und wird automatisch im Device-Core des FirmOS registriert und von da an zentral verwaltet.

## 8. Dashboards
- **Admin-Dashboard:** Bietet eine zentrale Oberfläche zur Verwaltung von Nutzern und Rollen, zur Überwachung des Systemstatus und der Netzwerkübersicht, zum Management von Berechtigungen (stets nur unterhalb der eigenen Hierarchiestufe), zum Zugriff auf Audit-Logs, zur Konfiguration von Backups und KI-Modellen sowie zur Einsicht in die allgemeine Sicherheitslage.
- **Geschäftsführungs-Dashboard:** Stellt eine vollständige, hochaggregierte Unternehmensübersicht bereit. Es visualisiert alle relevanten KPIs aus Vertrieb, HR, Finanzen, Produktion und Marketing. KI-generierte Interpretationen der Unternehmenslage, Risikoanalysen, Forecasts, der Compliance-Status und kritische Eskalationsmeldungen werden hier zentral angezeigt.
- **Abteilungs-Dashboards:** Jede Abteilung erhält ein eigenes Dashboard mit den für sie relevanten Kennzahlen und Werkzeugen: Vertrieb (Leads, Umsatz, Forecast), HR (Bewerber, Mitarbeiterstatus, Vertragslaufzeiten), Produktion (Maschinenstatus, Auslastung, Ausfälle), IT (Systemverfügbarkeit, offene Tickets, Sicherheits-Alerts), Marketing (Kampagnen-Performance, Reichweite, KPIs).

## 9. KI-Integration
Alle KI-Funktionen laufen ausschließlich auf dem internen GPU-Cluster und sind tief in die Systemprozesse integriert.
- **Automatisierte Unternehmensanalyse:** KI-Modelle analysieren kontinuierlich Geschäftsdaten (Umsatz, Produktion, Personal, Kosten) und identifizieren Trends, Risiken und Optimierungspotenziale.
- **Intelligente Dokumentenverarbeitung:** Eingehende Dokumente wie Rechnungen, Verträge oder Mitarbeiterdokumente werden automatisch per OCR erfasst, klassifiziert und den entsprechenden Workflows zugeführt.
- **Automatisierte Berichterstattung:** Tägliche, wöchentliche oder monatliche Reports für das Management werden von der KI automatisch erstellt und zusammengefasst.
- **Interaktiver Assistent:** Ein Chat-Assistent steht allen Mitarbeitern zur Verfügung, um bei der Ausfüllung von Formularen zu helfen, Informationen aus der Wissensdatenbank bereitzustellen oder einfache Datenabrufe zu tätigen.
- **Proaktive Anomalie-Erkennung:** Die KI überwacht Finanztransaktionen, Netzwerkverkehr und Produktivitätsmetriken, um verdächtige Muster zu erkennen und proaktiv Alarme auszulösen.
- **Predictive Maintenance:** Die KI analysiert die Zustandsdaten der Hardware-Infrastruktur, um potenzielle Ausfälle vorherzusagen und Wartungsarbeiten proaktiv zu planen.

## 10. Sicherheit, Compliance, Monitoring und Automatisierung
- **Zero-Trust und MFA:** Die gesamte Architektur folgt einem Zero-Trust-Modell mit mTLS zwischen allen Services. MFA ist für alle Logins ohne Ausnahme obligatorisch.
- **Verschlüsselung und Auditing:** Die gesamte interne Kommunikation ist Ende-zu-Ende-verschlüsselt. Alle sicherheitsrelevanten Aktionen werden in unveränderlichen Audit-Trails protokolliert.
- **Datenschutz (DSGVO):** Die Verarbeitung biometrischer Daten erfolgt streng nach DSGVO-Richtlinien. Automatische Löschfristen und rollenbasierte Datenmaskierung sind fest implementiert.
- **SIEM und Incident Response:** Das System ist an ein Security Information and Event Management (SIEM)-System angebunden. Die KI ermöglicht eine automatische Risikoerkennung und leitet erste Schritte zur Incident-Response ein.
- **Monitoring und Logging:** Prometheus wird für das Sammeln von Metriken von Servern, Netzwerkkomponenten und Endgeräten eingesetzt. Ein ELK/EFK-Stack (Elasticsearch, Logstash/Fluentd, Kibana) aggregiert alle Logs. Grafana dient der Visualisierung von Metriken und Logs in Echtzeit.
- **Workflow-Automatisierung:** Eine integrierte Workflow-Engine automatisiert wiederkehrende Prozesse wie die Rechnungsverarbeitung, das Mitarbeiter-Onboarding und -Offboarding, die Serverwartung, das automatische Skalieren von Diensten und die Bereitstellung neuer FirmOS-Clients.
- **Infrastructure as Code (IaC) und CI/CD:** Die gesamte Systemkonfiguration wird mittels Infrastructure as Code (Terraform/Ansible) verwaltet. System-Updates werden über CI/CD-Pipelines automatisiert, getestet und ausgerollt.
