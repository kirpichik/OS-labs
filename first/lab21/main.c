//
// Lab work #21, Tuner
//

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


#define TUNE "\a"
#define BUTTON_CODE 13 // Enter
// 46 is Delete

// Press counter
int count;

// Saved terminal state
struct termios savedOptions;

// Current terminal
int terminal;


void sigQuitHandler(int);
void sigIntHandler(int);
int setupTermAttrs(int, struct termios*);
int resetTermAttrs(int, struct termios*);


int main() {
  // Opens terminal
  terminal = open("/dev/tty", O_RDWR);
  if (terminal == -1) {
    perror("Cannot open terminal");
    return -1;
  }

  // Check valid terminal
  if (!isatty(terminal)) {
    perror("Run in shell mode, need terminal");
    close(terminal);
    return -1;
  }

  // Setup terminal attrs
  if (setupTermAttrs(terminal, &savedOptions)) {
    close(terminal);
    return -1;
  }
  
  // Setup press button signal
  if (sigset(SIGINT, &sigIntHandler) == SIG_ERR) {
    perror("Cannot set INT signal handler");
    return -1;
  }

  // Setup finish signal (Ctrl + Backslash)
  if (sigset(SIGQUIT, &sigQuitHandler) == SIG_ERR) {
    perror("Cannot set QUIT signal handler");
    return -1;
  }

  printf("Press ENTER for tune:\n");

  while (1);

  return 0;
}

/**
 * Handles SIG_QUIT, prints button pressed count and finish program.
 *
 * @param signal Input signal.
 */
void sigQuitHandler(int sig) {
  printf("Count = %d\n", count);

  // Returns previous terminal attrs
  resetTermAttrs(terminal, &savedOptions);
  close(terminal);

  exit(0);
}

/**
 * Handles SIG_INT, write tune symbol to terminal.
 *
 * @param sig Input signal.
 */
void sigIntHandler(int sig) {
  if (write(terminal, TUNE, strlen(TUNE)) == -1) {
    perror("Cannot write tune symbol");
    exit(-1);
  }
  count++;
}

/**
 * Setting up terminal attrs for catch button press.
 * Also disable echo.
 *
 * @param terminal Current terminal
 * @param savedOptions Target for write previous terminal options.
 *
 * @return Zero on success.
 */
int setupTermAttrs(int terminal, struct termios* savedOptions) {
  struct termios newOptions;
  
  // Gets previous terminal options
  if (tcgetattr(terminal, savedOptions)) {
    perror("Cannot get terminal options");
    return -1;
  }

  newOptions = *savedOptions;
  newOptions.c_lflag &= ~ECHO;
  newOptions.c_cc[VINTR] = BUTTON_CODE;

  // Sets new terminal options
  if (tcsetattr(terminal, TCSANOW, &newOptions)) {
    perror("Cannot set terminal options");
    return -1;
  }

  return 0;
}

/**
 * Reset previous terminal options.
 *
 * @param terminal Current terminal.
 * @param savedOptions Previous options.
 *
 * @return Zero on success.
 */
int resetTermAttrs(int terminal, struct termios* savedOptions) {
  if (tcsetattr(terminal, TCSANOW, savedOptions)) {
    perror("Cannot set terminal options");
    return -1;
  }

  return 0;
}

