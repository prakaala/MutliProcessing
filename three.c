#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
// Function to calculate f(x) = 1 + 2 + ... + x
int calcSum(int x) {
int sum = 0;
for (int i = 1; i <= x; i++) {
sum += i;
}
return sum;
}
int main(int argc, char *argv[]) {
int fd;
const char *name = "HW2";
int *shared_memory;
pid_t pid;
if (argc != 2) {
fprintf(stderr, "Usage: %s <upper>\n", argv[0]);
return 1;
}
int upper = atoi(argv[1]);
if (upper <= 0) {
fprintf(stderr, "Upper must be a positive integer.\n");
return 1;
}
// Creating shared memory for total sum
fd = shm_open(name, O_CREAT | O_RDWR, 0666);
ftruncate(fd, sizeof(int));
shared_memory = (int *)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED,
fd, 0);
*shared_memory = 0; // Initialize total sum
for (int i = 1; i <= upper; i++) {
pid = fork();
if (pid < 0) {
fprintf(stderr, "Fork failed\n");
return 1;
} else if (pid == 0) {
// This code is executed by the child process
int result = calcSum(i);
// Add the result to the total sum in shared memory
*shared_memory += result;
exit(0);
}
wait(NULL);
}
// Read the total sum from shared memory
int total_sum = *shared_memory;
// Print the total sum
printf("Parent process: Sum of f(1) + f(2) + ... + f(%d) = %d\n", upper,
total_sum);
// Unmap shared memory and unlink (remove) it
munmap(shared_memory, sizeof(int));
shm_unlink(name);
return 0;
}