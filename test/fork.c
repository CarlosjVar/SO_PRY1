#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(void)
{
  /* place semaphore in shared memory */
  sem_t *sema = mmap(NULL, sizeof(*sema), 
      PROT_READ |PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,
      -1, 0);
  if (sema == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  /* create/initialize semaphore */
  if ( sem_init(sema, 1, 0) < 0) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }
  int nloop=10;
  int pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if (pid == 0) { 
    /* child process*/
    for (int i = 0; i < nloop; i++) {
      printf("child unlocks semaphore: %d\n", i);
      if (sem_post(sema) < 0) {
          perror("sem_post");
      }
      sleep(1);
    }
    if (munmap(sema, sizeof(sema)) < 0) {
      perror("munmap");
      exit(EXIT_FAILURE);
    }
      exit(EXIT_SUCCESS);
  }
  if (pid > 0) {
    /* back to parent process */
    for (int i = 0; i < nloop; i++) {
      printf("parent starts waiting: %d\n", i);
      if (sem_wait(sema) < 0) {
        perror("sem_wait");
      }
      printf("parent finished waiting: %d\n", i);
    }
    if (sem_destroy(sema) < 0) {
      perror("sem_destroy failed");
      exit(EXIT_FAILURE);
    }
    if (munmap(sema, sizeof(sema)) < 0) {
      perror("munmap failed");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
  }
}