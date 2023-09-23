#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
pid_t pid;
if (argc != 2) {
fprintf(stderr, "Usage: %s <upper>\n", argv[0]);
return 1;
}
int upper = atoi(argv[1]);
pid=fork();
if (pid < 0) {
fprintf(stderr, "Fork failed");
return 1;
}
else if (pid == 0) {
// This code is executed by the child process
int sum = 0;
for (int i = 1; i <= upper; i++) {
sum += i;
}
printf("Sum from 1 to %d is: %d\n ", upper, sum);
} else {
// This code is executed by the parent process
wait(NULL);
printf("In Parent Process* Now child is complete \n");
}
return 0;
}