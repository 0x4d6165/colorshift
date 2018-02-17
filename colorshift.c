#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>

int main (int argc, char **argv) {
  char intermBuf[BUFSIZ];
  char inputString[7];
  unsigned long inputInt;
  double r, g, b;

  // Parse input from stdin if there is no input argument
  if(argc != 2) {
    fgets(intermBuf, sizeof intermBuf, stdin);
    if(intermBuf[strlen(intermBuf)-1] != '\n') {
      fprintf(stderr, "Invalid hex color\n");
      exit(EXIT_FAILURE);
    }
    if(strlen(intermBuf)!=7) {
      fprintf(stderr, "Invalid hex color\n");
      exit(EXIT_FAILURE);
    }
    strncpy(inputString, intermBuf, 6);
  }
  // Parse input argument if there is one
  else if(argv[1] != NULL) {
    if(strlen(argv[1])!=6) {
      fprintf(stderr, "Invalid hex color\n");
      exit(EXIT_FAILURE);
    }
    strncpy(inputString, argv[1], 6);
  }
  // User did not provide input
  else {
    fprintf(stderr, "Usage: %s hex-color", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Use regex to see if the user input is a valid 6 digit hex number
  regex_t hexRegex;
  int regexOutput;
  char regexBuf[100];
  // Compile regex
  regexOutput = regcomp(&hexRegex, "[A-Fa-f0-9]", 0);
  if(regexOutput) {
    fprintf(stderr, "Could not compile regex\n");
    exit(EXIT_FAILURE);
  }
  // Execute regex
  regexOutput = regexec(&hexRegex, inputString, (size_t) 0, NULL, 0);
  if(!regexOutput) {
    printf("Match\n");
    inputInt = strtol(inputString, NULL, 16);
    printf("%i\n", (int)inputInt);
  }
  else if(regexOutput == REG_NOMATCH) {
    fprintf(stderr, "Invalid hex color\n");
    exit(EXIT_FAILURE);
  }
  else {
    regerror(regexOutput, &hexRegex, regexBuf, sizeof(regexBuf));
    fprintf(stderr, "Regex match failed: %s\n", regexBuf);
    exit(EXIT_FAILURE);
  }
  regfree(&hexRegex);

  // This should've been caught by now but let's just be safe.
  if(sizeof(inputInt) != 8) {
    fprintf(stderr, "Invalid hex color\n");
    exit(EXIT_FAILURE);
  }

  // Let's split our hex code into their respective r, g, and b values
  r = (double) ((inputInt >> 16) & 0xff);
  printf("Red: %f\n", r);
  g = (double) ((inputInt >> 8) & 0xff);
  printf("Green: %f\n", g);
  b = (double) (inputInt & 0xff);
  printf("Blue: %f\n", b);

  return(EXIT_SUCCESS);
}
