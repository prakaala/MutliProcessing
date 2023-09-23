#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
int main(int argc, char *argv[]) {
int fd;
const char *name = "HW2";
char *ptr;
pid_t pid;
if (argc != 2) {
fprintf(stderr, "Usage: %s <upper>\n", argv[0]);
return 1;
}
int upper = atoi(argv[1]);
pid = fork();
if (pid < 0) {
fprintf(stderr, "Fork failed\n");
return 1;
} else if (pid == 0) {
// This code is executed by the child process
int sum = 0;
const char *message = "Sum is: ";
for (int i = 1; i <= upper; i++) {
sum += i;
}
// Creating a shared memory object
fd = shm_open(name, O_CREAT | O_RDWR, 0666);
ftruncate(fd, 4096);
ptr = (char *)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
// Write the sum to shared memory
sprintf(ptr, "%s%d", message, sum);
// Sleep for 2 seconds
sleep(2);
// Close the shared memory object and exit the child process
close(fd);
exit(0);
} else {
// This code is executed by the parent process
wait(NULL);
// Opening the shared memory object in read-only mode
fd = shm_open(name, O_RDONLY, 0666);
ptr = (char *)mmap(0, 4096, PROT_READ, MAP_SHARED, fd, 0);
// Print the result received from the child process
printf("Parent process: %s\n",(char*)ptr);
// Close the shared memory object
close(fd);
// Unlink (remove) the shared memory object
shm_unlink(name);
printf("Child Complete\n");
}
return 0;
}