#include "systemcalls.h"

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
    if(system(cmd) == 0) return true;
    else return false;

    return true;
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
    char *path;
    int i;
 
/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    int wstatus;
    __pid_t pid = fork();
    if(pid < 0){
        perror("fork error");
        return false;
    }
    else if(pid == 0){
        printf(" This is child process. ");

        for (i = 0; i < count; i++){
            if (i == 0){
                path = va_arg(args, char *);
                continue;
            }
            command[i - 1] = va_arg(args, char *);
        }

        command[count -1] = NULL;
        command[count - 1] = command[count - 1];

        if(execv(path,command) < 0 ){
            perror("execv error: ");
            exit(-1);
        }
    }
    else
    {

        int waitID = waitpid(pid, &wstatus, 0);
        if (waitID == -1)
            return false;
        else if ((waitID == pid) && WIFEXITED(wstatus)){
            // printf("child status %d %d %d **", pid, waitID, (int8_t)WEXITSTATUS(wstatus));
            return ((int8_t)WEXITSTATUS(wstatus) == 0) ? true : false;
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
    char *path;
    int i;

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    int fd = open(outputfile, O_RDWR|O_CREAT|O_TRUNC, 0777);
    if (fd < 0) { 
        perror("open :"); 
        exit(-1);
    }
    
    int wstatus;
    __pid_t pid = fork();
    printf(" pid is %d",pid);
    if(pid < 0){
        perror("fork error");
        return false;
    }
    else if(pid == 0){
        printf(" This is child process. test_redirec start");

        for (i = 0; i < count; i++){
            if (i == 0){
                path = va_arg(args, char *);
                continue;
            }
            command[i - 1] = va_arg(args, char *);
        }

        command[count -1] = NULL;
        command[count - 1] = command[count - 1];

        if (dup2(fd,1) < 0) { 
            perror("dup2");
            exit(-1);
        }
        if (dup2(fd,2) < 0) { 
            perror("dup2");
            exit(-1);
        }
        close(fd);
        //write(fd,command,50);
        //write(1,command,20);
        if(execv(path,command) < 0 ){
            perror("execv error: ");
            exit(-1);
        }
    }
    else
    {
        close(fd);
        int waitID = waitpid(pid, &wstatus, 0);
        if (waitID == -1)
            return false;
        else if ((waitID == pid) && WIFEXITED(wstatus)){
            //printf("child status %d %d %d **", pid, waitID, (int8_t)WEXITSTATUS(wstatus));
            return ((int8_t)WEXITSTATUS(wstatus) == 0) ? true : false;
        }
    }
    close(fd);
    va_end(args);
    return false;
}