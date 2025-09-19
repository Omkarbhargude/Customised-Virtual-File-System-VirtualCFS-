////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Description : Header file inclusion
//  Author Name : Omkar Mahadev Bhargude
//  Date        : 10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Description : User defined macros for file specification
//  Author Name : Omkar Mahadev Bhargude
//  Date        : 10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Max file size 
# define MAXFILESIZE 100

// Maximum number of files that we can open
# define MAXOPENEDFILE 20

// Maximum number of files that we can create
# define MAXINODE 5

# define BUFFER_SIZE 1024

// Permission for file 
# define READ 1
# define WRITE 2
# define EXECUTE 4

// File specification
# define REGULARFILE 1
# define SPECIALFILE 2

// 
# define START 0
# define CURRENT 1
# define END 2

//
# define EXECUTE_SUCCESS 0

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Description : User defined macros for error handling
//  Author Name : Omkar Mahadev Bhargude
//  Date        : 10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

# define ERR_INVALID_PARAMETER -1               // If the parameter are wrong       
# define ERR_NO_INODES -2                       // Number of free inodes are 0 
# define ERR_FILE_ALREADY_EXISTS -3             // File already present 
# define ERR_FILE_NOT_EXISTS -4                 // File not present
# define ERR_PERMISSION_DENIED -5               // Permission is not accepted
# define ERR_INSUFFICIENT_SPACE -6              // No space avaliable
# define ERR_INSUFFICIENT_DATA -7               //
# define ERR_DATA_OVERFLOW -8                   // 

////////////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------
// Function Prototypes
// -------------------------------

void ls_file();
void DisplayHelp();
void ManPage(char *name);
int stat_file(char *name);
int UnlinkFile(char *name);
bool IsFileExists(char *name);
int CreateFile(char *name, int permission);
int write_file(int fd, char *data, int size);
int read_file(int fd, char *data, int size);

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name  : BootBlock
//  Description     : Holds the information to boot the operating system
//  Author Name     : Omkar Mahadev Bhargude
//  Date            : 10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name  : SuperBlock
//  Description     : Holds the information about the file system
//  Author Name     : Omkar Mahadev Bhargude
//  Date            : 10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotaInodes;
    int FreeInodes;
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name  : Inode
//  Description     : Holds the information about the file
//  Author Name     : Omkar Mahadev Bhargude
//  Date            : 10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Inode    
{
    char FileName[50];              // Name of the file (max 49 chars + '\0')
    int InodeNumber;                // Unique ID for this inode
    int FileSize;                   // Total size allocated for the file (e.g., block size)
    int ActualFileSize;             // Actual data stored in the file
    int FileType;                   // Type of file (regular, special)
    int ReferenceCount;             // How many processes have this file open
    int Permission;                 // Read/Write/Execute permissions
    int LinkCount;                  // Number of hard links to this file
    char *Buffer;                   // Pointer to the actual data (file contents)
    struct Inode *next;             // Pointer to next inode (linked list)
} INODE, *PINODE, **PPINODE;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name  : FileTable
//  Description     : Holds the information about the opened file
//  Author Name     : Omkar Mahadev Bhargude
//  Date            : 10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct FileTable
{
    int ReadOffSet;             // Position to read the data from file
    int WriteOffSet;            // Position to write the data from file
    int Count;                  // how many reference to this table
    int Mode;                   // Mode of file (write / read)
    PINODE ptrinode;            // specifi pointer to Inode stucture
}FILETABLE, *PFILETABLE;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name  : UAREA
//  Description     : Holds the information about the process
//  Author Name     : Omkar Mahadev Bhargude
//  Date            : 10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[50];                   // Name of process
    PFILETABLE UFDT[MAXOPENEDFILE];         // Specific array of pointer to FileTable structure
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Global variables or objects used in the project
//
////////////////////////////////////////////////////////////////////////////////////////////////

struct BootBlock bootobj;
struct SuperBlock superobj;
PINODE p_head = NULL;
struct UAREA uareaobj;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :  InitialiseUAREA()
//  Author          :  Omkar Mahadev Bhargude
//  Date            :  10/08/2025
//  Description     :  It is used to initialise the contents of UAREA
//  Parameter       :  NONE
//  Returns         :  NONE
//
////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName, "Myexe");           // Used to copy the ProcessName 

    int i = 0;

    while(i < MAXOPENEDFILE)
    {
        uareaobj.UFDT[i] = NULL;                     // Initialising UFDT pointer to NULL 
        i++;
    }

    printf("VirtualCFS : UAREA initialised successfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :  InitialiseSuperBlock()
//  Author          :  Omkar Mahadev Bhargude
//  Date            :  10/08/2025
//  Description     :  It is used to initialise the contents of SuperBlock
//  Parameter       :  NONE
//  Returns         :  NONE
//
////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotaInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;
    printf("VirtualCFS : SuperBlock initialised successfully\n");

}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :  CreateDILB()
//  Author          :  Omkar Mahadev Bhargude
//  Date            :  10/08/2025
//  Description     :  Used to create the linkedlist of Inodes
//  Parameter       :  NONE
//  Returns         :  NONE
//
////////////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = p_head;

    while(i < MAXINODE)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        newn -> InodeNumber = i;
        newn -> FileSize = 0;
        newn -> ActualFileSize = 0;
        newn -> FileType = 0;
        newn -> ReferenceCount = 0;
        newn -> Permission = 0;
        newn -> LinkCount = 0;
        newn -> Buffer = NULL;
        newn -> next = NULL;

        if(temp == NULL)
        {
            p_head = newn;
            temp = p_head;
        }
        else
        {
            temp->next = newn;
            temp = temp -> next;
        }

        i++;
    }

}// End of CreateDILB

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :  StartAuxilaryDataInitialisation()
//  Author          :  Omkar Mahadev Bhargude
//  Date            :  10/08/2025
//  Description     :  Used is to initialise the Auxilary data
//  Parameter       :  NONE
//  Returns         :  NONE
//
////////////////////////////////////////////////////////////////////////////////////////////////

void StartAuxilaryDataInitialisation()
{
    strcpy(bootobj.Information, "Boot process of Operating System done successfully");

    printf("%s\n",bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("VirtualCFS : Auxilary data initialised successfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     IsFileExists
//  Description :       It is used to check whether the given file name is exist or not
//  Input :             It accepts the file name
//  Return :            It returns boolean value 
//                      True : if present 
//                      False : if not present
//
//  Author :            Omkar Mahadev Bhargude
//  Date :              10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//  Helper function
//

bool IsFileExists(
                    char *name             // Name of that we want to check  
            )
{
    PINODE temp = p_head;
    bool bFlag = false;

    while(temp != NULL)
    {
        // Checks if name & filetype matched
        if((strcmp(name, temp->FileName) == 0) && (temp->FileType) == REGULARFILE)
        {
            bFlag = true;
            break;
        }

        temp = temp->next;
    }

    return bFlag;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :  CreateFile()
//  Description     :  It is used to create a new file
//  Parameter       :  It accpets the [char *] file name and [int] permission for the file
//  Returns         :  It returns the [integer] file descriptor of the file
//  Author          :  Omkar Mahadev Bhargude
//  Date            :  10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//  VirtualCFS > creat file_name permission (creat demo.txt 3)

int CreateFile(
                    char *name,                 // Name of file
                    int permission              // Permission to create file
            )
{
    PINODE temp = p_head;
    int i = 0;

    //
    // Filters
    //

    // If file name is not given
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // If entered permission is invalid
    if(permission < 1 || permission > 3 )
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check whether empty inode is ther or not
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // Check whether file is already present or not
    if(IsFileExists(name) == true)
    {
        return ERR_FILE_ALREADY_EXISTS;
    }

    // Loop to search free inode
    while(temp != NULL)
    {
        if(temp -> FileType == 0)
        {
            break;
        }

        temp = temp->next;
    }

    // Inode not found
    if(temp == NULL)
    {
        return ERR_NO_INODES;
    }

    // Search first NON-NULL value from UFDT
    for(i = 0; i < MAXINODE; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    if(i == MAXINODE)
    {
        printf("[WARN] Unable to create file : MAX OPENED FILE LIMIT REACHED");
        return -1;
    }

    // Allocate memory for file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialise elements of file table
    uareaobj.UFDT[i] -> ReadOffSet = 0;
    uareaobj.UFDT[i] -> WriteOffSet = 0;
    uareaobj.UFDT[i] -> Count = 1;
    uareaobj.UFDT[i] -> Mode = permission;

    // Connect file table to IIT [incore inode table]
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise IIT contents
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName, name);
    uareaobj.UFDT[i] -> ptrinode ->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i] -> ptrinode -> ActualFileSize = 0;
    uareaobj.UFDT[i] -> ptrinode -> FileType = REGULARFILE;
    uareaobj.UFDT[i] -> ptrinode -> ReferenceCount = 1;
    uareaobj.UFDT[i] -> ptrinode -> LinkCount = 1;
    uareaobj.UFDT[i] -> ptrinode -> Permission = permission;

    // Allocate memory for Buffer
    uareaobj.UFDT[i] -> ptrinode -> Buffer = (char *)malloc((MAXFILESIZE));


    // Decrement the number of free inodes by 1
    superobj.FreeInodes--;

    // fd [file descriptor]
    return i;

}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   write_file()
//  Author          :   Omkar Mahadev Bhargude
//  Date            :   10/08/2025
//
//  Description     :   It is used to write the contents into the file
//  
//  Input           :   [integer] File descriptor
//                      [char *] Address of buffer which contains data.
//                      [integer] Size of data that we want to write.
//
//  Returns         :   Number of bytes succesfully written into the file
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//  VirtualCFS >  write file_name or write fd (optional both works same)
//

int write_file(
                    int fd,                     // File descriptor of file
                    char *data,                 // Data that we want to write
                    int size
            )
{
    printf("Number of bytes going to write : %d\n",size);

    // Invalid value of fd
    if(fd < 0 || fd > MAXOPENEDFILE)
    {
        return ERR_INVALID_PARAMETER;
    }

    // File is not opened or created with given fd
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // If there is no permission to write the data into the file
    if(uareaobj.UFDT[fd] -> ptrinode -> Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Unable to write as there is no sufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffSet) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // For safety if the first check fails 
    if(uareaobj.UFDT[fd] -> WriteOffSet > MAXFILESIZE)
    {
        return ERR_DATA_OVERFLOW;
    }

    // write the actual data
    strncpy(uareaobj.UFDT[fd] -> ptrinode -> Buffer + uareaobj.UFDT[fd] -> WriteOffSet,data,size);

    // Update the writeoffset
    uareaobj.UFDT[fd] -> WriteOffSet = uareaobj.UFDT[fd] -> WriteOffSet + size;

    // Update the actual size of file after writing the data
    uareaobj.UFDT[fd] -> ptrinode -> ActualFileSize = uareaobj.UFDT[fd] -> ptrinode -> ActualFileSize + size;

    return size;

}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     read_file
//  Description   :      It is used to read the contents from the file
//  Input         :     File descriptor
//                      Address of empty buffer
//                      Size of data that we want to read
//
//  Returns       :      Number of bytes succesfully read from the file
//  Author        :      Omkar Mahadev Bhargude
//  Date          :      10/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//  VirtualCFS > read fd size (read [fd] 1 [size] 10)
// 

int read_file(
                    int fd,                 // File Descriptor
                    char *data,             // Address of empty buffer
                    int size                // Number of bytes that we want to read
            )
{
    // Invalid value of fd
    if(fd < 0 || fd > MAXOPENEDFILE)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL || size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    // File is not opened or created with the given fd
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // If there is no permission to read the data into the file
    if(uareaobj.UFDT[fd] -> ptrinode -> Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Unable to read as there is no sufficient data
    if((MAXFILESIZE - uareaobj.UFDT[fd] -> ReadOffSet) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // read the data from buffer into data 
    strncpy(data, uareaobj.UFDT[fd] -> ptrinode -> Buffer + uareaobj.UFDT[fd] -> ReadOffSet, size);

    // Update the readoffset
    uareaobj.UFDT[fd] -> ReadOffSet = uareaobj.UFDT[fd] -> ReadOffSet + size;

    return size;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :  UnlinkFile()
//  Author          :  Omkar Mahadev Bhargude
//  Date            :  10/08/2025
//  Description     :  It is used to delete a regular file
//  Parameter       :  It accepts file name 
//  Returns         :  NONE
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//  VirtualCFS > unlink file_name   (unlink demo.txt)
// 

int UnlinkFile(
                    char *name
            )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    for(i = 0; i < MAXINODE; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName,name) == 0 )
            {
                // Deallocate memory buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);

                // Reset all values of inode
                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->LinkCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;


                // Deallocate memory of file table
                free(uareaobj.UFDT[i]);

                // Set NULL to the UFDT member
                uareaobj.UFDT[i] = NULL;

                // Increament the value of free inodes count
                superobj.FreeInodes++;

                break;
            }
        }
    }

    return EXECUTE_SUCCESS;

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     ManPage
//  Description :       It is used to Display the manual page of the command
//  Input :             It accepts the command name
//  Output :            Displays the manual details of the command
//  Author :            Omkar Mahadev Bhargude
//  Date :              11/08/2025
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
//  VirtualCFS > man command_name (man creat)
//
void ManPage(
                char *name      // Name of command
            )
{
    if(strcmp(name, "creat") == 0)
    {
        printf("Description : This command is used to create new regular file on our file system \n");

        printf("Usage : creat File_name Permissions\n");
        printf("File_name : The name of file that you want to create\n");
        printf("Permissions : \n1 : Read \n2 : Write \n3 : Read + Write\n");

    }
    else if(strcmp(name, "exit") == 0)
    {
        printf("Description : This command is used to terminate the Marvellous CVFS \n");

        printf("Usage : exit\n");
    }
    else if(strcmp(name, "unlink") == 0)
    {
        printf("Description : This command is used to delete regular file from our file system \n");

        printf("Usage : unlink File_name\n");
        printf("File_name : The name of file that you want to delete\n");
    }
    else if(strcmp(name, "stat") == 0)
    {
        printf("Description : This command is used to display statistical information about the file \n");

        printf("Usage : stat File_name\n");
        printf("File_name : The name of file whose information you want to display\n");
    }
    else if(strcmp(name, "ls") == 0)
    {
        printf("Description : This command is used to list all file names from  directory \n");

        printf("Usage : ls\n");
    }
    else if(strcmp(name, "write") == 0)
    {
        printf("Description : This command is used to write data into the file \n");

        printf("Usage : write File_Descriptor\n");
    }
    else if(strcmp(name, "read") == 0)
    {
        printf("Description : This command is used to read the data from the file \n");

        printf("Usage : read File_Descriptor Size\n");

        printf("File_Descriptor : Its the value returned by create system call\n");
        printf("Size : Number of bytes that you want to read\n");
    }

    // Add more options here
    else
    {
        printf("No manual entry for %s\n",name);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :  DisplayHelp()
//  Author          :  Omkar Mahadev Bhargude
//  Date            :  10/08/2025
//  Description     :  Used to display information about commands
//  Parameter       :  NONE
//  Returns         :  NONE
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//  VirtualCFS > help
//

void DisplayHelp()
{
    printf("---------------------------------------------------------------------------\n");
    printf("--------------------- Command manual of VirtualCFS ------------------------\n");
    printf("---------------------------------------------------------------------------\n");

    //
    // Descriptions :
    //
    printf("man     : Displays the manual page of a command.\n");
    printf("         Shows syntax, options, and usage details.\n\n");

    printf("exit    : Terminates the Marvellous CVFS shell.\n");
    printf("         Ends the current session safely.\n\n");

    printf("clear   : Clears the console screen.\n");
    printf("         Provides a clean view for new commands.\n\n");

    printf("creat   : Creates a new regular file.\n");
    printf("         Use with filename to generate the file.\n\n");

    printf("unlink  : Deletes an existing file.\n");
    printf("         Removes the file from the filesystem.\n\n");

    printf("stat    : Displays detailed information of a file.\n");
    printf("         Shows size, permissions, and timestamps.\n\n");

    printf("ls      : Lists all files in the current directory.\n");
    printf("         Can show hidden or detailed file info.\n\n");

    printf("write   : Writes data into a file.\n");
    printf("         Allows adding or overwriting content.\n\n");

    printf("read    : Reads data from a file.\n");
    printf("         Displays content stored in the file.\n\n");

    printf("---------------------------------------------------------------------------\n");

    // To Add

}// End of DisplayHelp


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   ls_file
//  Description     :   It is used to diplsy the information about all files in the directory
//  Input           :   NONE
//  Output          :   NONE
//  Author          :   Omkar Mahadev Bhargude
//  Date            :   15/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

void ls_File()
{
    PINODE temp = p_head;

    if(temp == NULL)
    {
        printf("[INFO] : No file present\n");
        return;
    }

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s\n",temp->FileName);
        }

        temp = temp -> next;
    }

}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   stat_file
//  Description     :   It is used to diplsy the information about the given files
//  Input           :   file name
//  Output          :   NONE
//  Author          :   Omkar Mahadev Bhargude
//  Date            :   15/08/2025
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//  VirtualCFS > stat file_name (stat demo.txt)
//

int stat_file(
                char *name          // Name of file
            )
{            
    PINODE temp = p_head;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    while(temp != NULL)
    {

        if((strcmp(name, temp->FileName) == 0) && (temp -> FileType != 0))
        {
            printf("------------ Statistical Information of file -----------\n");
            
            printf("File name : %s\n",temp->FileName);
            printf("File size on Disk : %d\n",temp->FileSize);
            printf("Actual File size : %d\n",temp->ActualFileSize);
            printf("Link count : %d\n",temp->LinkCount);
            
            printf("File permission : ");
            if(temp->Permission == READ)
            {
                printf("Read\n");
            }
            else if(temp->Permission == WRITE)
            {
                printf("Write\n");
            }
            else if(temp->Permission == READ + WRITE)
            {
                printf("Read + Write\n");
            }
            
            printf("File type : ");
            if(temp->FileType == REGULARFILE)
            {
                printf("Regular file\n");
            }
            else if(temp->FileType == SPECIALFILE)
            {
                printf("Special file\n");
            }

            printf("--------------------------------------------------------\n");

        }

        temp = temp -> next;
    }

}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Entry point function of VirtualCFS (main)   
//
////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    int iRet = 0, iCount = 0, fd = 0, i = 0;    
    char str[80] = {'\0'};                             // Used for commands
    char Command[5][80] = {'\0'};                      // Used to break the commands in sperate block
    char InputBuffer[MAXFILESIZE] = {'\0'};            // Used for writing data in file
    char *EmptyBuffer = NULL;
    char s_buf[BUFFER_SIZE] = {'\0'};
    PINODE temp = NULL;
    FILE *fp = NULL;

    StartAuxilaryDataInitialisation();

    printf("---------------------------------------------------------------------------\n");
    printf("--------------------- VirtualCFS Started Successfully ---------------------\n");
    printf("---------------------------------------------------------------------------\n");

    //
    // Custom Shell loop
    //
    while(1)        
    {

        fflush(stdin);                          // Clears any extra character in input buffer

        strcpy(str,"");

        printf("VirtualCFS > ");
        fgets(str,sizeof(str),stdin);           // Used to take input 

        iCount = sscanf(str," %s %s %s %s",Command[0],Command[1],Command[2],Command[3]);    // Breaks every char 

        fflush(stdin);

        ///////////////////////////////////////////////////////////////////////////////////////////////
        if(iCount == 1)
        {
            // VirtualCFS > exit
            if(strcmp(Command[0], "exit") == 0)             // Compare to string and return 0 if true
            {

                printf("Thank you for using VirtualCFS\n");
                printf("Deallocating all the resources\n");

                break;
            }

            // VirtualCFS > help
            else if(strcmp(Command[0], "help") == 0)
            {
                DisplayHelp();
            }

            // Virtual > clear
            else if(strcmp(Command[0], "clear") == 0)
            {
                system("clear");
            }

            // Virtual > ls
            else if(strcmp(Command[0], "ls") == 0)
            {
                ls_File();
            }

            // Wrong Command
            else
            {
                printf("[ERROR] : Command not found\n");
                printf("Please refer help option or use man command\n");
            }


        } //   End of if (iCount == 1)

        ///////////////////////////////////////////////////////////////////////////////////////////////
        else if(iCount == 2)
        {
            // Virtual CFS > man creat          // 2 str
            if(strcmp(Command[0], "man") == 0)
            {
                ManPage(Command[1]);
            }

            // VirtualCFS > unlink Demo.txt

            else if(strcmp(Command[0], "unlink") == 0)
            {
                iRet = UnlinkFile(Command[1]);

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("Unlink Opertaion is succesfully performed\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("[ERROR] : Unable to do unlink activity as file is not present\n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                { 
                    printf("[ERROR] : Invalid parameter for this function.\n");
                    printf("[INFO] : Check the man page for usage info.\n");
                }
            }
            
            // VirtualCFS > stat Demo.txt
            else if(strcmp(Command[0], "stat") == 0)
            {
                iRet = stat_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("[ERROR] : Unable to display statistic file not present\n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("[ERROR] : Invalid parameter for this function.\n");
                    printf("[INFO] : Check the man page for usage info.\n");
                }
            }

            // VirtualCFS > write demo.txt
            else if(strcmp(Command[0], "write") == 0)
            {
                printf("Enter the data into the file : \n");
                fgets(InputBuffer,MAXFILESIZE,stdin);

                iRet = write_file(atoi(Command[1]), InputBuffer, strlen(InputBuffer)-1);

                if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("[ERROR] : Insufficient Space in data block of file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("[ERROR] Permission Denied : No write permission for this file\n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("[ERROR] : Invalid parameter for this function.\n");
                    printf("[INFO] : Check the man page for usage info.\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("[ERROR] : FD is invalid\n");
                }
            }


        }//   End of if (iCount == 2)

        ///////////////////////////////////////////////////////////////////////////////////////////////
        else if(iCount == 3)
        {
            // VirtualCFS > creat demo.txt 3
            if(strcmp(Command[0], "creat") == 0)
            {
                iRet = CreateFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("[ERROR] : Invalid parameter for this function.\n");
                    printf("[INFO] : Check the man page for usage info.\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXISTS)
                {
                    printf("[ERROR] Cannot create: file already exists.\n");
                }
                else if(iRet == ERR_NO_INODES)
                {
                    printf("[ERROR] File creation failed: no available inodes.\n");
                }
                else
                {
                    printf("[INFO] : File created successfully with fd : %d\n",iRet);
                }
            }

            // VirtualCFS > read 3 10

            else if(strcmp(Command[0], "read") == 0)
            {
                EmptyBuffer = (char *)malloc(sizeof(atoi(Command[2])));

                iRet = read_file(atoi(Command[1]),EmptyBuffer,atoi(Command[2]));


                if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("[ERROR] : Insufficient data in the data block of the file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("[ERROR] : No read permission for this file\n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("[ERROR] : Invalid parameter for this function.\n");
                    printf("[INFO] : Check the man page for usage info.\n");
                }
                else if(iRet ==  ERR_FILE_NOT_EXISTS)
                {
                    printf("[ERROR] : FD is invalid\n");
                }
                else 
                {
                    printf("[INFO] : Read Operation is succesfull...\n");
                    printf("Data from file is : %s\n",EmptyBuffer);

                    free(EmptyBuffer);
                }
            }
            else
            {

                printf("[ERROR] : Command not found\n");
                printf("Please refer help option or use man command\n");
            }

        }//   End of if (iCount == 3)

        ///////////////////////////////////////////////////////////////////////////////////////////////
        else
        {   
            printf("[ERROR] : Invalid parameter for this function.\n");
            printf("[INFO] : Check the man page for usage info.\n");
        }
    }

    return 0;
}

/*
    Future functions to be add 
*/