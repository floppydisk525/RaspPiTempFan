# Sqlite3 Notes

### Sqlite3 Links
http://www.sqlitetutorial.net/sqlite-create-table/  
https://www.sqlite.org/faq.html  

Relational DB Overview Link  
https://www.ntu.edu.sg/home/ehchua/programming/sql/relational_database_design.html  

## Linux Install Commands  
sudo apt-get install sqlite3 libsqlite3-dev  
sudo apt-get install sqlitebrowser  
  
## Database Schema
Database schema  

TABLE tmp_snsr
28-num - text  (PK)
location_desc  - char array  
snsr_offset - floating (?)  
  
TABLE tmp_rec  
rowid - int (PK)  
28-num - text - (FK)  
date_time - date and time type  
temp_c - floating  
tmp_offset - floating (record used incase changes)  
fan_output - floating  
  
TABLE fan_info  
fan_id - int  (PK)
fan_desc - String/char array  
fan_offset - floating (?)  
  

(REMOVE)  TABLE fan_rec  
fan_id - int  (FK)
date_time - Date and time type  


## SQLITE3 Commands & Examples

### BEGIN TRANSACTION & COMMIT
The BEGIN TRANSACTION starts a new transaction. It ensures that all the subsequent statements execute successfully or nothing executes at all.  

The COMMIT statement commits all the statements.  

### .open or connect to specific database, check current connection with .databases  
sqlite3 name.db  
  
If name.db does not exist, it will be created.  
You can also open/connect to a database after starting sqlite3 with ".open name.db"  

sqlite> .databases   (shows currently connected databases)  

### Show .tables in a database and show structure  
sqlite> .tables  
Lists the tables in a database.  

sqlite> .schema <table name>
Lists the structure of a table.  Good to check a table after entering it.  

### How to OUTPUT the database to CSV or Excel  
tbd  
