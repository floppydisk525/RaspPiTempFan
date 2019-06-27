# Sqlite3 Notes

### Sqlite3 Links
http://www.sqlitetutorial.net/sqlite-create-table/  
https://www.sqlite.org/faq.html  

## Linux Install Commands  
sudo apt-get install sqlite3 libsqlite3-dev  
sudo apt-get install sqlitebrowser  
  
## Database Schema
Database schema  

TABLE Tmp_Snsr
PK - int (needed?  Use 28-Num) 
28-Num - int (or str)
Location_Desc  - char array  
Offset - floating (?)  
  
TABLE Tmp_rec  
PK - int  
28-Num - int (or str)  
Date_Time - date and time type  
Temp_c - floating  
Offset - floating (record used incase changes)  
  
TABLE Fan_Info  
Fan_ID - int  
Fan_Desc - String/char array  
  
TABLE Fan_rec  
pk - int  
Fan_ID - int  
Date_Time - Date and time type  
Fan_output - floating  
