# Customised-Virtual-File-System-VirtualCFS-,
## Technology : C programming

## Project Overview :
This project is a custom implementation of a Virtual file system (VFS) that simulates the core functionality of the Linux file system. It is built entirely in C programming language , with its own custom shell to interact with the virtual environment. The project provides a practical understanding of system calls, file handling, memory management, and OS internals.

## Key Features :
.   Custom Shell Interface 
      . Provides Linux-like commands for file operations(create, open, read, write, delete, ls, etc)

.  System Call Simulation 
    . Implements core Linux file system calls (open, read, write, lseek, close, rm, etc) using C.

.  File System Data Structure 
    . Incore Inode Table
    . File Table
    . UAREA (user area)
    . User File Descriptor Table

. Platform Independent
  . Allows system-level file handling functionalities of liunx to be used on any operating system platform.

. Database-like Functionality
  . Provides a customised database management layer with structured file handling.

## Learning Outcomes :
  . Deep understanding of Linux File System internals.
  . Practical knowledge of data structure used in OS (inode, file tables, UAREA).
  . Strong grasp of system programming in C.
  . Hands-on with shell design & command interpreter.
  . Application of low-level logic building for OS-like environments.

## Example Uages :

$ ./myexe

VirtualCFS > creat demo.txt 3
[INFO] : File created successfully with fd : 0
VirtualCFS > write demo.txt
Enter the data into the file : 
hello this is my first system programming project
Number of bytes going to write : 49
VirtualCFS > read 0 49
[INFO] : Read Operation is succesfull...
Data from file is : hello this is my first system programming project
VirtualCFS > stat demo.txt
------------ Statistical Information of file -----------
File name : demo.txt
File size on Disk : 100
Actual File size : 49
Link count : 1
File permission : Read + Write
File type : Regular file
VirtualCFS > exit
