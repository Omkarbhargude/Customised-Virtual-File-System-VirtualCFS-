# Customised Virtual File System – (CVFS)

**Technology:** C programming

---

## Project Overview

This project is a custom implementation of a Virtual File System (VFS) that simulates the core functionality of the Linux file system.
It is built entirely in **C**, with its own custom shell to interact with the virtual environment.
The project provides a practical understanding of system calls, file handling, memory management, and OS internals.

---

## Key Features

* **Custom Shell Interface**

  * Provides Linux-like commands for file operations (`create`, `open`, `read`, `write`, `delete`, `ls`, etc.)

* **System Call Simulation**

  * Implements core Linux file system calls (`open`, `read`, `write`, `lseek`, `close`, `rm`, etc.) using C

* **File System Data Structures**

  * In-core Inode Table
  * File Table
  * UAREA (user area)
  * User File Descriptor Table

* **Platform Independent**

  * Allows Linux-style system-level file handling to be used on any operating system

* **Database-like Functionality**

  * Provides a customised database management layer with structured file handling

---

## Learning Outcomes

* Deep understanding of Linux File System internals
* Practical knowledge of data structures used in OS (inode, file tables, UAREA)
* Strong grasp of system programming in C
* Hands-on experience with shell design & command interpreters
* Application of low-level logic building for OS-like environments

---

## Compilation & Execution

To compile the project, use `gcc` as follows:

```bash
gcc -o myexe main.c virtualfs.c -Wall
```

To run the executable:

```bash
$ ./myexe
```

This will launch the VirtualCFS shell where you can execute commands like `create`, `write`, `read`, `stat`, and `exit`.

---

## Example Usage

```bash
VirtualCFS > create demo.txt 3
[INFO] : File created successfully with fd : 0

VirtualCFS > write demo.txt
Enter the data into the file :
hello this is my first system programming project
Number of bytes going to write : 49

VirtualCFS > read 0 49
[INFO] : Read Operation is successful...
Data from file is : hello this is my first system programming project

VirtualCFS > stat demo.txt
------------ Statistical Information of file -----------
File name         : demo.txt
File size on Disk : 100
Actual File size  : 49
Link count        : 1
File permission   : Read + Write
File type         : Regular file

VirtualCFS > exit
```
## Example Usage Screenshot

![CVFS image](images/Screenshot_from_2025-09-19_11-08-43.png)

