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
snsr_tmp_offset - floating (?)  
  
TABLE tmp_rec  
rowid - int (PK)  
28-num - text - (FK)  
date_time - date and time type  
temp_c - floating  
tmp_offset - floating (record used incase changes)  
fan_output - floating  
button_press - boolean (int)
  
TABLE fan_info  
fan_id - int  (PK)
fan_desc - String/char array  
fan_offset - floating (?)  
  

(REMOVE & include in TABLE tmp_rec)  TABLE fan_rec  
fan_id - int  (FK)
date_time - Date and time type  

Questions  
Do I need separate fields for date & time or only one field?  
Do I store floating (ie double) or text and convert in C?  What about boolean vs int?  
Do I need a foreign key for fan_id in the tmp_rec TABLE?   Going with it, not sure it's needed.  
How to write Foreign Key into tmp_rec TABLE for tmp_sensor 28-num and fan_info fan_id?  My guess is to pull the keys from the tables and then write them when writing a row of data?  


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
