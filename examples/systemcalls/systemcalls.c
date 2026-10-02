#include "systemcalls.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    int status = system(cmd);
    return (status == 0);
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

        pid_t pid = fork();

        if (pid < 0) 
        {
            // Fork failed
            return false;
        }
 
        else if (pid == 0) 
        {
            // Child process: execute the command
            execv(command[0], command);
        
            // execv only returns if an error occurs
            exit(EXIT_FAILURE);
        }
  
        else 
        {
            // Parent process: wait for child to complete
            int status;
            if (waitpid(pid, &status, 0) == -1) 
            {
                return false;
            }

        // Return true only if child exited normally with exit status 0
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) 
        {
            return true;
        }
    }

    va_end(args);

    return false;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) 
    {
        // Fork failed
        return false;
    }
    else if (pid == 0) 
    {
        // Child Process: Open the output file for writing
        // O_WRONLY: Write-only, O_CREAT: Create if missing, O_TRUNC: Clear existing content
        // 0644: User can read/write, Group/Others can read
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) 
        {
            // If the file can't be opened, exit child process
            exit(EXIT_FAILURE);
        }

        // Redirect standard out (STDOUT_FILENO) to the file descriptor
        if (dup2(fd, STDOUT_FILENO) < 0) 
        {
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Close the original file descriptor as it is no longer needed 
        // after being duplicated to stdout
        close(fd);

        // Execute the command
        execv(command[0], command);
        
        // execv only returns if an error occurred
        exit(EXIT_FAILURE); 
    }
    else 
    {
        // Parent Process: Wait for the specific child to finish
        int status;
        if (waitpid(pid, &status, 0) == -1) 
        {
            return false;
        }

        // Return true only if the child exited normally with exit status 0
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) 
        {
            return true;
        }
    }

    return false;
}
