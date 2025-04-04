# Clarkson University Honors Capstone Thesis
## George D. Crochiere
Software Engineering Major<br/>
Clarkson University - Class of 2025<br/>
Department of Electrical & Computer Engineering<br/>
Honors Program<br/>

---

## Improving Server Security by Integrating Keystroke Dynamic Authentication into System Utilities: A Case Study with MySQL

---
### Original MySQL Source Code: https://github.com/mysql/mysql-server
Modified Files:
 - mysql-server-trunk/client/mysql.cc
 - mysql-server-trunk/client/keystroke/... (all files)
---
### Requires Ubuntu Linux (used 22.04 - https://releases.ubuntu.com/jammy/)
---

### Installation:
- NOTE: The installation process can take some time due to the MySQL build process.
- This repository can also installed similar to installing the MySQL program from its source code, which can be found [here](https://dev.mysql.com/doc/refman/8.4/en/installing-source-distribution.html)
  - Ensure MySQL is not installed on the system
  - Clone the repository to your system
  - Navigate to inside the directory ```honors-capstone-mysql-server-keystroke-dynamics``` from the cloned repository
  - ```sudo apt update```
  - ```sudo apt upgrade```
  - ```sudo apt-get install build-essential cmake libncurses5-dev git libssl-dev pkg-config bison -y```
  - ```sudo groupadd mysql```
  - ```sudo useradd -r -g mysql -s /bin/false mysql```
  - ```mkdir build```
  - ```cd build```
  - ```cmake ../mysql-server-trunk -DWITH_SSL=```
  - ```make``` (This will take a long time, ex. 8+ hours)
  - ```sudo make install```
  - ```cd /usr/local/mysql```
  - ```sudo mkdir mysql-files```
  - ```sudo chown mysql:mysql mysql-files```
  - ```sudo chmod 750 mysql-files```
  - ```sudo ./bin/mysql --initialize --user=mysql```
  - Keep track of the temporary password that is generated from this command; you'll need it as the initial password for starting MySQL
  - ```sudo ./bin/mysqld_safe --user=mysql```
  - The server can be started with the command ```sudo ./bin/mysqld_safe --user=mysql```
    - This command needs to be executed each time the system is started from the ```/usr/local/mysql``` directory.
- The client can be started in the directory ```/usr/local/mysql``` and started with ```sudo ./bin/mysql -u root -p``` for the root user.
- The password of a user can be changed with ```alter user '<user>'@'localhost' identified by "<password>";```
- Keystroke Dynamics will be enabled, but will not kick the root user.
- Data entry needs to be completed for a set of keywords to be used. For the purpose of this project, the words need to be entered fully with all keystrokes 10 times for the keywords to take effect (ex. "show databases;").

---
### Keystroke Dynamics data files:
- Located at "/usr/local/src/KDData"
