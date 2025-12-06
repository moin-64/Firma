# FirmOS: Vollständiges Technisches Pflichtenheft

## 1. Einleitung und Systemvision
Dieses Dokument beschreibt die vollständige technische Spezifikation für die Entwicklung und Implementierung des "FirmOS", eines vollständig integrierten, on-premises Unternehmens-Betriebssystems. Ziel ist die Schaffung eines zentralisierten, hochsicheren und KI-gestützten IT-Ökosystems, das alle Geschäftsprozesse des Unternehmens in einer einzigen, autarken Plattform vereint. Das System ersetzt Insellösungen für Identitätsmanagement, OS-Provisioning, HR, Finanzen, Ticketing und physische Zutrittskontrolle. Es ist für den Betrieb auf einer massiven, redundanten und hochverfügbaren Server-Infrastruktur mit eigenen GPU-Clustern ausgelegt und unterliegt strengsten Sicherheits- und Datenschutzanforderungen (DSGVO, Zero-Trust).

## 2. Physische und Netzwerk-Infrastruktur
### 2.1. Hardware-Infrastruktur
Das Fundament des FirmOS bildet ein zentrales Serverrack (42-HE-Schränke), das für maximale Leistung, Redundanz und Ausfallsicherheit ausgelegt ist:
- **Netzwerk-Backbone:** Enterprise-Router und -Switches (z.B. Ubiquiti UniFi) mit 10/25/40/100-GbE-Verbindungen und redundanten Glasfaser-Links.
- **Compute-Nodes:** Mehrere Server mit Multi-Core-CPUs, ECC-RAM und redundanten Netzteilen für die Ausführung der Kern-Services.
- **GPU-Cluster:** Ein Cluster aus NVIDIA A100/H100 GPUs, verbunden über NVLink/PCIe-Fabric, dediziert für alle KI- und Machine-Learning-Aufgaben.
- **Datenspeicher:** Ein hybrides Speichersystem aus NVMe-Arrays für schnellen Datenzugriff und HDD-Arrays für Langzeitspeicherung und Backups, verwaltet durch ein ZFS- oder Ceph-Cluster.
- **Ausfallsicherheit:** Überdimensionierte USV-Module für eine mehrstündige Überbrückung bei Stromausfall, redundante Kühlsysteme und umfassendes Monitoring der Hardware-Sensoren.

### 2.2. Netzwerk-Architektur
Die Netzwerk-Architektur folgt dem Zero-Trust-Prinzip und ist vollständig segmentiert:
- **VLANs:** Jede Abteilung (Entwicklung, Finanzen, HR, Produktion, IT, Marketing) operiert in einem eigenen, isolierten VLAN.
- **Mikrosegmentierung:** Ein Software-Defined-Network (SDN) Controller ermöglicht die dynamische Anpassung von Routing-Regeln und die Mikrosegmentierung von Diensten.
- **Sicherheit:** Eine Next-Gen-Firewall im Core, ergänzt durch perimeter- und hostbasierte Firewalls, sowie IDS/IPS (Suricata/Zeek) zur Erkennung von Anomalien im Netzwerkverkehr.
- **Services:** Dedizierte PXE/iPXE-Server für das OS-Provisioning und VPN-Gateways (SSL/IPSec) mit MFA für sichere Remote-Zugriffe.
- **QoS:** Quality-of-Service-Regeln priorisieren sicherheitskritische Datenströme, wie die von Face-Recognition-Terminals und Alarmsystemen.

## 3. FirmOS: Die Unternehmens-Linux-Distribution
Eine eigene, auf Ubuntu Server LTS oder Rocky Linux basierende Linux-Distribution, die auf allen Arbeitsplätzen zum Einsatz kommt und folgende Merkmale aufweist:
- **Rollenbasierte Konfiguration:** Bei jedem Login wird das System automatisch an die Rolle des Benutzers angepasst.
- **Sicherheit:** Verschlüsselter Container-Support, automatische Updates vom zentralen Server und ein integrierter Monitoring-Daemon.
- **Konnektivität:** Ein vorinstallierter Identity-Agent zur Anbindung an den Authentifizierungs-Server und ein VPN-Client, der sich automatisch verbindet.
- **Software-Management:** Jeder Arbeitsplatz erhält nur die für seine Rolle autorisierten und vom zentralen Server bereitgestellten Software-Pakete.

## 4. Server-Komponenten und -Dienste
### 4.1. Authentifizierungs-Server
- **Identity Management:** Keycloak oder Authentik als zentrale Instanz für die Verwaltung von Identitäten, Rollen und Berechtigungen.
- **Multi-Faktor-Authentifizierung (MFA):** Eine Kombination aus Passwort, Gesichtserkennung und optional einem Hardware-Token (z.B. YubiKey) oder NFC-Token ist für alle Logins zwingend erforderlich.
- **Hierarchische Rechtevererbung:** Rechte werden von oben nach unten vererbt und können von Administratoren nicht über ihre eigene Hierarchiestufe hinaus geändert werden.
- **Biometrische Daten:** Die Gesichtsdaten der Mitarbeiter werden DSGVO-konform und verschlüsselt in der Datenbank gespeichert und ausschließlich für die Authentifizierung an PCs und Zutrittsterminals verwendet.

### 4.2. Datenbankserver
- **System:** PostgreSQL mit Transparent Data Encryption (TDE) und Replikation für Hochverfügbarkeit.
- **Schema:** Umfassende Tabellen für Mitarbeiter, Rollen, Berechtigungen, Zutritte, Geräte, Logs, Räume/Zonen, Finanzen, HR, Tickets und Dokumente.
- **Datenmanagement:** Automatische Löschfristen (z.B. nach 5 Jahren) für sensible Daten, um die Einhaltung von Datenschutzrichtlinien zu gewährleisten.

### 4.3. Applikationsserver
- **Backend:** Ein in C++ implementiertes Backend stellt die REST-APIs für die verschiedenen Dashboards (Admin, Geschäftsführung, Abteilungen) bereit.
- **API-Gateway:** Ein API-Gateway (z.B. Kong oder Traefik) managt den externen Zugriff auf die REST-APIs.
- **Asynchrone Aufgaben:** Worker-Prozesse (z.B. mit Celery oder einer eigenen C++-Queue) verarbeiten zeitintensive Aufgaben im Hintergrund.
- **Orchestrierung:** Die verschiedenen Microservices des Backends werden in einem Kubernetes-Cluster orchestriert, wobei die Kommunikation zwischen den Services mittels mTLS verschlüsselt ist.

### 4.4. KI-Server / GPU-Cluster
Alle KI-Modelle laufen ausschließlich auf dem internen GPU-Cluster:
- **Face-Recognition:** Ein hochpräzises Modell zur Gesichtserkennung, inklusive Liveness-Detection zur Abwehr von Spoofing-Angriffen.
- **Dokumentenverarbeitung:** OCR- und NLP-Modelle zur automatischen Verarbeitung von Rechnungen, Verträgen und anderen Dokumenten.
- **Analyse und Forecasting:** KI-Modelle zur Analyse von Finanzdaten, zur Erstellung von Forecasts und zur Erkennung von Anomalien im Netzwerkverkehr, bei Zutritten und in Finanztransaktionen.
- **Berichtsgenerierung:** Automatische Erstellung von Berichten und Zusammenfassungen für die Geschäftsführung.
- **NLP-Assistent:** Ein interner Chat-Assistent, der allen Mitarbeitern zur Verfügung steht.
- **MLOps:** MLflow oder Kubeflow zur Verwaltung der Modelle und Trainings-Pipelines.

### 4.5. File- und Backup-Server
- **Speicher:** Ein RAID-Verbund mit ZFS oder Ceph für Snapshots und revisionssichere Dokumentenarchivierung.
- **Backups:** Tägliche, automatisierte und verschlüsselte Backups an zwei physisch getrennte Standorte.

## 5. Rollen und Zugriffsrechte
Das System unterstützt eine granulare, hierarchische Rollenstruktur. Jede Rolle hat präzise definierte Zugriffsrechte auf Module, Daten und Netzwerksegmente. Ein Auszug der Rollen:
- **Geschäftsführung/CEO:** Vollzugriff auf alle Dashboards (read-only für technische Systeme), strategische KI-Berichte, alle Abteilungsmetriken und Sicherheitsmeldungen.
- **HR/Personalwesen:** Zugriff auf das Personalverwaltungsmodul, Bewerbermanagement und Lohn- und Gehaltsabrechnung (höchste Sicherheitsstufe). Kein Zugriff auf die Finanzbuchhaltung.
- **Finanzen/Controlling:** Zugriff auf die Buchhaltungssoftware, das Rechnungssystem, Kostenanalysen und Steuer-Tools. Kein Zugriff auf HR-Daten.
- **IT/Systemadministration:** Zugriff auf die Admin-Konsole, Netzwerk- und Server-Monitoring, das Security Center und das OS-Provisioning. Keine Rechte über der eigenen Hierarchiestufe und kein Zugriff auf persönliche Inhalte von Mitarbeitern.
- **Weitere Rollen:** Spezifische Zugriffsrechte für Einkauf, Vertrieb, Marketing, Produktion, F&E und Logistik.

## 6. Face-Recognition und Zutrittssystem
- **Terminals:** Hardware-Terminals mit HD-Kamera, IR- und Tiefensensor sowie NFC-Leser. Die Terminals führen eine Edge-Inference (quantized TensorFlow Lite/ONNX) zur Liveness-Detection durch, der eigentliche Abgleich der Gesichtsdaten erfolgt jedoch auf dem zentralen KI-Server.
- **Zutrittsrechte:** Die Zugriffsrechte sind abhängig von Rolle, Hierarchie, Uhrzeit und Zone. Bestimmte Terminals sind nur für ausgewählte Mitarbeitergruppen zugänglich.
- **NFC-Fallback:** NFC-Token dienen als Notfall-Alternative und werden ausschließlich von der HR-Abteilung und in Notfällen verwendet.
- **Sicherheit:** Alle Zutritte, Fehlversuche und Sperrungen werden vollständig geloggt. Eine KI erkennt Tailgating und Kamera-Manipulation in Echtzeit.

## 7. OS-Provisioning und Gerätemanagement
- **Workflow:** Ein Administrator wählt im Admin-Dashboard Abteilung, Mitarbeiter und Gerätetyp aus.
- **Image-Generierung:** Der Server erstellt automatisch ein maßgeschneidertes Linux-Image mit den rollenspezifischen Paketen, Zertifikaten, Firewallregeln und der Face-Login-Integration.
- **Deployment:** Der Flash-Prozess erfolgt über PXE/Netboot.
- **Admin-Bridge:** Administratoren können über ihre eigene FirmOS-Distribution als "Brücke" andere PCs flashen.
- **Registrierung:** Jedes neue Gerät erhält ein Zertifikat und wird im Device-Core registriert.

## 8. Dashboards
- **Admin-Dashboard:** Zur Verwaltung von Nutzern, Rollen, Rechten, Systemstatus, Logs und Backups.
- **Geschäftsführungs-Dashboard:** Bietet eine vollständige Unternehmensübersicht mit allen KPIs, KI-generierten Analysen, Risikoanalysen und Compliance-Status.
- **Abteilungs-Dashboards:** Spezifische Dashboards für Vertrieb, HR, Produktion, IT und Marketing.

## 9. Sicherheit, Compliance, Monitoring und Automatisierung
- **Architektur:** Zero-Trust-Architektur mit mTLS zwischen allen Services und MFA-Pflicht für alle Logins.
- **Verschlüsselung:** Ende-zu-Ende-Verschlüsselung für die gesamte interne Kommunikation.
- **Auditing:** Lückenlose Audit-Trails für alle sicherheitsrelevanten Aktionen.
- **Datenschutz:** DSGVO-konforme Verarbeitung biometrischer Daten, automatische Löschfristen und rollenbasierte Datenmaskierung.
- **Monitoring:** Prometheus für Metriken, ein ELK/EFK-Stack für die Log-Aggregation und Grafana für die Visualisierung.
- **Automatisierung:** Eine Workflow-Engine für die Automatisierung von Prozessen wie Rechnungsverarbeitung, Onboarding/Offboarding und Serverwartung, sowie CI/CD-Pipelines für System-Updates.
