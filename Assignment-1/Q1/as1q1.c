#include <sys/types.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char *argv[]){
  pid_t parentpid=getpid();
  printf("Parent(P) is having ID %d\n",parentpid);
  pid_t pid = fork();
  if(pid==0){
    printf("Child is having ID %d\n",getpid());
    printf("My parent ID is %d\n",parentpid);
  }
  else{
    wait(NULL);
    printf("ID of P's Child is %d\n",pid);
  }

}
