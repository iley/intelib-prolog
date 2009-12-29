// INTELIB_CONTRIBUTED
// Written by Igor Bronstein (<igor.bronstein_AT_gmail.com>)

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tests.hpp"

#define MAX_CHARS_IN_NUMBER 20
#define TESTS_DIRECTORY "../tests/refal/"
#define TIME_STRING "time"
#define FORMAT_STRING "--format=\%e"
#define FLAG_L "-l"
#define DIRECTIVES_SUFFIX "_directives.ref"
#define TRANSLATOR1 "../../irina/wrap"
#define TRANSLATOR2 "../../irina/irina"
#define REFAL_SUFFIX_WITH_NUMBER "_%d.ref"
#define REFAL_SUFFIX "_.ref"
#define FILENAMES_ARGUMENT 4
#define TEST_LOG_FILE "time.log"
#define BUFSIZE 100
#define IDENTITY_ARGS 5
#define DIFF_STRING "diff"
#define DIFF_FLAGS "-bB"
#define DOT_CXX ".cxx"
#define MV_STRING "mv"
#define H_CHAR 'h'
#define Y_CHAR 'y'


bool exited_and_with_zero(int status)
{
    return (WIFEXITED(status) != 0 && WEXITSTATUS(status) == 0);
}

bool run_time(const char *fname, int num, const char *translator,
              bool app)
{
    char buf[MAX_CHARS_IN_NUMBER+strlen(REFAL_SUFFIX)+1];
    int child_pid;
    char **argums = new char *[num+FILENAMES_ARGUMENT+2];
    argums[0] = (char *)TIME_STRING;
    argums[1] = (char *)FORMAT_STRING;
    argums[2] = new char[strlen(translator)+1];
    strcpy(argums[2], translator);
    argums[3] = (char *)FLAG_L;
    int len = strlen(fname)+
        strlen(DIRECTIVES_SUFFIX)+1;
    argums[FILENAMES_ARGUMENT] = new char[len];
    argums[FILENAMES_ARGUMENT][len-1] = '\0';
    strncpy(argums[FILENAMES_ARGUMENT],
        fname, strlen(fname));
    strcpy(argums[FILENAMES_ARGUMENT] + strlen(fname),
        DIRECTIVES_SUFFIX);
    for (int i=0; i<num; i++)
    {
        sprintf(buf, REFAL_SUFFIX_WITH_NUMBER, i+1);
        len = strlen(fname)+strlen(buf)+1;
        argums[FILENAMES_ARGUMENT+1+i] = new char[len];
        argums[FILENAMES_ARGUMENT+1+i][len-1] = '\0';
        strncpy(argums[FILENAMES_ARGUMENT+1+i],
            fname, strlen(fname));
        strcpy(argums[FILENAMES_ARGUMENT+1+i]+strlen(fname), buf);
    }
    argums[FILENAMES_ARGUMENT+1+num] = 0;
    if (!app)
    {
       printf("$(TRANSLATOR)");
       for (int i=3; i<FILENAMES_ARGUMENT+1+num; i++)
           printf(" %s", argums[i]);
       putchar('\n');
    }
    int fd = open(TEST_LOG_FILE, O_WRONLY |
        (app ? O_APPEND : (O_CREAT | O_TRUNC)), 0644);
    child_pid = fork();
    if (child_pid == -1)
    {
        perror("Internal error");
        exit(1);
    }
    if (!child_pid)
    {
        dup2(fd, 2);
        close(fd);
        execvp(TIME_STRING, argums);
        perror("Execution failed");
        exit(1);
    }
    else
    {
        close(fd);
        int status;
        wait(&status);
        delete []argums[2];
        argums[2] = 0;
        for (int i=FILENAMES_ARGUMENT; i<FILENAMES_ARGUMENT+1+num; i++)
        {
            delete []argums[i];
            argums[i] = 0;
        }
        delete []argums;
        return(exited_and_with_zero(status));
    }
}

double my_string2double(char *str)
{
    bool point_flag = false;
    double res = 0.0;
    double fract_part = 0.0;
    double coef = 1.0;
    while (*str != '\0')
    {
        if (!point_flag)
        {
            if (*str == '.')
                point_flag = true;
            else
            {
                res *= 10;
                res += *str - '0';
            }
        }
        else
        {
            coef /= 10;
            fract_part += coef * (*str - '0');
        }
        str++;
    }
    return res+fract_part;
}

void count_ratio(int times_num)
{
    int fd = open(TEST_LOG_FILE, O_RDONLY);
    char buf[BUFSIZE];
    int num = read(fd, buf, BUFSIZE);
    close(fd);
    buf[num] = '\0';
    char *temp = buf;
    while (*temp != '\0')
    {
      if (*temp == '\n')
        *temp = '\0';
      temp++;
    }
    char *where_start = (buf[0] >= '0' && buf[0] <= '9') ? buf : buf + 1;
    double d1 = my_string2double(where_start);
    double d2 = 0.0;
    for (int i=0; i<times_num; i++) {
        where_start += strlen(where_start) + 1;
        d2 += my_string2double(where_start);
    }
    printf("TRANSLATOR = %s\t(usual Refal)\ttime1 = %.3lf\n",
        TRANSLATOR1, d1);
    if (times_num == 1) {
        printf("TRANSLATOR = %s\t(InteLib Refal)\ttime2 = %.3lf\n",
               TRANSLATOR2, d2);
    }
    else {
        printf("TRANSLATOR = %s\t(InteLib Refal)\ttime2 = %.3lf "\
               "(%.3lf / %d)\n",
               TRANSLATOR2, d2 / times_num, d2, times_num);
    }
    printf("ratio = time2 / time1 = %lf\n", d2 / times_num / d1);
}

bool move(const char *fname)
{
    char **argums = new char *[4];
    argums[0] = (char *)MV_STRING;
    argums[1] = new char[strlen(fname)+strlen(DOT_CXX)+1];
    strncpy(argums[1], fname, strlen(fname));
    strcpy(argums[1]+strlen(fname), DOT_CXX);
    argums[2] = new char[strlen(argums[1])+1];
    strcpy(argums[2], argums[1]);
    argums[2][strlen(argums[2])-1] = Y_CHAR;
    argums[2][strlen(argums[2])-2] = Y_CHAR;
    argums[3] = 0;

    int child_pid = fork();

    if (child_pid == -1)
    {
        perror("Internal error");
        exit(1);
    }

    if (!child_pid)
    {
        execvp(MV_STRING, argums);
        perror("Execution failed");
        exit(1);
    }

    int status;
    wait(&status);

    argums[1][strlen(argums[1])-3] = H_CHAR;
    argums[2][strlen(argums[2])-3] = H_CHAR;

    child_pid = fork();

    if (child_pid == -1)
    {
        perror("Internal error");
        exit(1);
    }

    if (!child_pid)
    {
        execvp(MV_STRING, argums);
        perror("Execution failed");
        exit(1);
    }

    wait(exited_and_with_zero(status) ? &status : 0);

    delete []argums[1];
    argums[1] = 0;
    delete []argums[2];
    argums[2] = 0;
    delete []argums;

    return exited_and_with_zero(status);
}

void check_identity(const char *fname)
{
    char buf[BUFSIZE];
    char **argums = new char *[IDENTITY_ARGS];
    argums[0] = (char *)DIFF_STRING;
    argums[1] = (char *)DIFF_FLAGS;
    argums[2] = new char[strlen(fname)+strlen(DOT_CXX)+1];
    strncpy(argums[2], fname, strlen(fname));
    strcpy(argums[2]+strlen(fname), DOT_CXX);
    argums[3] = new char[strlen(argums[2])+1];
    strcpy(argums[3], argums[2]);
    argums[3][strlen(argums[3])-1] = Y_CHAR;
    argums[3][strlen(argums[3])-2] = Y_CHAR;
    argums[4] = 0;
  
    int status;

    int child_pid = fork();
    if (child_pid == -1)
    {
        perror("Internal error");
        exit(1);
    }
    if (!child_pid)
    {
        execvp(DIFF_STRING, argums);
        perror("Execution failed");
        exit(1);
    }
    wait(&status);
    sprintf(buf, "%s_identical_cxx", fname);
    TESTB(buf, exited_and_with_zero(status));

    argums[2][strlen(argums[2])-3] = H_CHAR;
    argums[3][strlen(argums[3])-3] = H_CHAR;

    child_pid = fork();

    if (child_pid == -1)
    {
      perror("Internal error");
      exit(1);
    }
    if (!child_pid)
    {
      execvp(DIFF_STRING, argums);
      perror("Execution failed");
      exit(1);
    }
    wait(&status);
    sprintf(buf, "%s_identical_hxx", fname);
    TESTB(buf, exited_and_with_zero(status));

    delete []argums[2];
    argums[2] = 0;
    delete []argums[3];
    argums[3] = 0;
    delete []argums;
}

void count_num(const char *fname, int &num)
{
    char buf[MAX_CHARS_IN_NUMBER+strlen(REFAL_SUFFIX)+1];
    int len, result;
    len = strlen(fname)+strlen(DIRECTIVES_SUFFIX)+1;
    char *temp;
    temp = new char[len];
    temp[len-1] = '\0';
    strncpy(temp, fname, strlen(fname));
    strcpy(temp + strlen(fname), DIRECTIVES_SUFFIX);
    result = access(temp, F_OK);
    delete []temp; 
    if (result)
        return;
    while (true) {
        sprintf(buf, REFAL_SUFFIX_WITH_NUMBER, num+1);
        len = strlen(fname) + strlen(buf) + 1;
        temp = new char[len];
        temp[len-1] = '\0';
        strncpy(temp, fname, strlen(fname));
        strcpy(temp + strlen(fname), buf);
        result = access(temp, F_OK);
        delete []temp;
        if (!result) {
            num++;
        }
        else {
            break;
        }
    }
}

void test_time(const char *fname,
               int times_num = 1,
               const char *test_name = 0)
{
    if (times_num < 1) {
        printf("Warning: times_num argument must be 1 or greater. "\
               "Assigning times_num to 1.\n");
        times_num = 1;
    }
    int num = 0;
    count_num(fname, num);
    if (test_name == 0) {
        test_name = fname;
    }
    TestSubsection(test_name);
    if (num == 0) {
        char buf[BUFSIZE];
        sprintf(buf, "Error with %s_*.ref", fname);
        perror(buf);
        return;
    }
    if (!run_time(fname, num, TRANSLATOR1, false)) {
        perror("\"run_time\" execution failed");
        return;
    }
    if (!move(fname)) {
        perror("\"move\" execution failed");
        return;
    }
    for (int i=0; i<times_num; i++) {
        if (!run_time(fname, num, TRANSLATOR2, true)) {
            perror("\"run_time\" execution failed");
            return;
        }
    }
    count_ratio(times_num);
    check_identity(fname);
}

int main()
{
    TestSection("RefalTime");

    /*
    The first argument is the name of a group of files
    The second one is the number of times to run Intelib Refal
    The third one is the name of the test
    For example, 'test_time("test001", 1, "very_small_task)'
    means that you will compare time values for:
    1) ./wrap -l test001_directives.ref test001_1.ref test001_2.ref
    and
    2) ./irina  -l test001_directives.ref test001_1.ref test001_2.ref,
    ./irina will be run 1 time
    and the test name will be 'very_small_task'
    */
    test_time("test001", 1, "very_small_task");
    test_time("test002", 1, "self_application");

    TestScore();
}


