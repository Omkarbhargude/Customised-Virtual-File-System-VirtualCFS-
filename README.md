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

## 🖼 Screenshots

### Shell Interface
![Shell](https://github.com/Omkarbhargude/Customised-Virtual-File-System-VirtualCFS-/blob/main/image/img1.png?raw=true)

### Write Example
![Write](https://github.com/Omkarbhargude/Customised-Virtual-File-System-VirtualCFS-/blob/main/image/img2.png?raw=true)

### Read Example
![Read](https://github.com/Omkarbhargude/Customised-Virtual-File-System-VirtualCFS-/blob/main/image/img4.png?raw=true)


## Author 
Omkar Mahadev Bhargude
📌 [LinkedIn](https://www.linkedin.com/in/omkar-bhargude-609a92311/)

