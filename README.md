# Clarkson University Honors Capstone Thesis
## George D. Crochiere
Software Engineering Major<br/>
Clarkson University - Class of 2025<br/>
Department of Electrical & Computer Engineering<br/>
Honors Program<br/>

---

# Honors Thesis Capstone
## Improving Server Security by Integrating Keystroke Dynamic Authentication into System Utilities: A Case Study with MySQL

---
### After cloning - installation:
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

---
### Keystroke Dynamics data files:
- Located at "/usr/local/src/KDData"

---
### To Terminate:
1. Close the MySQL Client with the "quit" command.
2. In a terminal, execute "ps -al"
3. Use the "sudo kill x" command to terminate the MySQL process. Replace "x" with the PID of the "mysql" process.

