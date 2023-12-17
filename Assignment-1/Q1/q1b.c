#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int fib(int n) {
    if (n == 0 || n == 1)
        return n;
    return fib(n-1) + fib(n-2);
}

int factorial(int n){
    if (n == 1){
        return n;
    }
    return n * factorial(n-1);
}

int main(int argc, char *argv[]){
    pid_t parentpid = getpid();
    pid_t pid = vfork();
    
    if (pid == 0){
        printf("The factorial of 4 is: %d\n", factorial(4));
    }
    else{
        wait(NULL);
        printf("The fib series upto 16 is:");
        for (int i = 0; i < 16; i++) {
            printf("%d ", fib(i));
        }
    }
    printf("\n");
    return 0;
}
