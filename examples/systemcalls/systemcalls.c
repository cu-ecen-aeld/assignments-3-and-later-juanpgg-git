#include "systemcalls.h"
#include <stdlib.h>  // for system()
#include <stdbool.h> // for bool

#include <stdarg.h>   // for variadic arguments
#include <unistd.h>   // for execv, fork
#include <sys/wait.h> // for waitpid
#include <stdlib.h>   // for exit


#include <stdarg.h>   // for variadic arguments
#include <unistd.h>   // for execv, fork, dup2
#include <sys/wait.h> // for waitpid
#include <fcntl.h>    // for open


/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
	int ret = system(cmd);  // call system() with the provided command
    return (ret == 0);      // return true if the command executed successfully

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
    char *command[count + 1];  // create a list of command and arguments
    for (int i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;  // terminate the list
    va_end(args);

    pid_t pid = fork();  // fork a child process
    if (pid == 0)
    {
        // Child process: execute the command
        execv(command[0], command);
        exit(EXIT_FAILURE);  // exit if execv() fails
    }
    else if (pid < 0)
    {
        return false;  // fork failed
    }
    else
    {
        // Parent process: wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);
        return (WIFEXITED(status) && WEXITSTATUS(status) == 0);  // return true if child exited successfully
    }
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
    char *command[count + 1];
    for (int i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);

    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process: redirect output to the file
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);  // redirect stdout to the file
        close(fd);

        // Execute the command
        execv(command[0], command);
        exit(EXIT_FAILURE);  // exit if execv() fails
    }
    else if (pid < 0)
    {
        return false;  // fork failed
    }
    else
    {
        // Parent process: wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);
        return (WIFEXITED(status) && WEXITSTATUS(status) == 0);  // return true if child exited successfully
    }
}
