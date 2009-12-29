// INTELIB_CONTRIBUTED
// Written by Igor Bronstein (<igor.bronstein_AT_intelib.org>)

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MEMORY "-l64"
#define FILENAMES_ARGUMENT 2



int main(int argc, char **argv)
{
    char **argvnew = new char*[argc+3];
    argvnew[0] = (char *)REFGO;
    argvnew[1] = (char *)MEMORY;
    argvnew[FILENAMES_ARGUMENT] = (char *)RSLPLUS;
    for (int i=1; i<argc; i++)
    {
        argvnew[FILENAMES_ARGUMENT+i] = new char[strlen(argv[i])+1];
        strcpy(argvnew[FILENAMES_ARGUMENT+i], argv[i]);
        if (argv[i][0] == '-')
        {
            argvnew[FILENAMES_ARGUMENT+i][0]='~';
        }
    }
    argvnew[FILENAMES_ARGUMENT+argc] = 0;
    int child_pid = fork();
    if (child_pid == -1)
    {
        perror("Internal error: fork() is impossible\n\n");
        exit(1);
    }
    if (!child_pid)
    {
        execvp(REFGO, argvnew);
        perror("1 Error was found during IRINA execution:\n\n");
        perror("No \"refgo\" file was found!\n\n");
        perror("Execution failed.\n\n");
        exit(1);
    }
    else
    {
      for (int i=FILENAMES_ARGUMENT+1; i<FILENAMES_ARGUMENT+argc; i++)
      {
          delete []argvnew[i];
          argvnew[i] = 0;
      }
      delete []argvnew;
      int status;
      wait(&status);
      exit(status);
    }
}


