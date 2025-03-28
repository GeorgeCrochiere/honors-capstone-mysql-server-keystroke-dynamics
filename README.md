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
Method 1: Debugging
1. Disable any current MySQL Servers
2. Create a build folder in the repository
    - honors-capstone-mysql-server-keystroke-dynamics/build
3. Execute the "make" command inside the build folder
4. Initialize the server with "sudo ./bin/mysqld --initialize"
5. Obtain the temporary password:
    - Open the "error.log" file located in "/var/log/mysql"
    - You may need to change the mysql permissions
6. Execute the server start command with "sudo ./bin/mysqld --debug"
7. In a new terminal, launch the MySQL client
    - Change to the build folder directory
    - Execute the command: "sudo mysql -uroot -p"
    - For the MySQL password initially, enter the password from the error.log file
    - Treat the system as a normal MySQL terminal
  
Method 2: Normal
- This repository can also installed similar to installing the MySQL program from its source code, which can be found [here](https://dev.mysql.com/doc/refman/8.4/en/installing-source-distribution.html)

---
### Keystroke Dynamics data files:
- Located at "/usr/local/src/KDData"

---
### To Terminate:
1. Close the MySQL Client with the "quit" command.
2. In a terminal, execute "ps -al"
3. Use the "sudo kill x" command to terminate the MySQL process. Replace "x" with the PID of the "mysql" process.

