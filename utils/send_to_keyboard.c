/* gcc -O2 -s -Wall -osend_to_keyboard main.c */
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int i;

  if (argc <= 1) {
    printf("Usage: send_to_keyboard <control codes>\n");
    printf("    e.g. send_to_keyboard ea 17\n");
  }
  
  if (ioperm(0x60, 3, 1) == -1)
	  perror("Could not change port permissions. Please run as root.");

  for (i = 1; i < argc ; i++) {
      int x = strtol(argv[i], 0, 16);

      usleep(300);
      outb(x, 0x60);
  }

  return 0;
}
