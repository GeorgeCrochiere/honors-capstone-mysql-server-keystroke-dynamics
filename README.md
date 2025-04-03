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
### Requires Ubuntu Linux (used 22.04)
---

### After cloning - installation:
- This repository can also installed similar to installing the MySQL program from its source code, which can be found [here](https://dev.mysql.com/doc/refman/8.4/en/installing-source-distribution.html)
- The server can be started with the command ```sudo systemctl start mysql```
- The client can be started in the directory ```/usr/local/mysql``` and started with ```sudo ./bin/mysql -u root -p```.
- Keystroke Dynamics will be enabled, but will not kick the root user.
- Data entry needs to be completed 

---
### Keystroke Dynamics data files:
- Located at "/usr/local/src/KDData"
