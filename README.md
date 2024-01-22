Client and server are communicating in a chunks of 1024 bytes

Server recieves the command in format "COMMAND path" where path meaning depends on the command

Server command list:
1) GET - used with a path to a file on server, responds with this file to the client
2) LIST - used with a path to a folder on server, responds with a list of all entries in folder
3) DELETE - used with a path to a file on server, deletes the file and responds with success or faliure message
4) PUT - after command client must send file to server, used with a path where to place the sended file, responds with success or faliure message
5) INFO - used with a path to a file on server, responds with information about file which contains last modified date/time and size of the file
