#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

double getMaxColor(double r, double g, double b) {
  double max = 0;
  if(r>max) {
    max = r;
  }
  if(g>max) {
    max = g;
  }
  if(b>max) {
    max = b;
  }
  return max;
}
double getMinColor(double r, double g, double b) {
  double min = 255;
  if(r<min) {
    min = r;
  }
  if(g<max) {
    min = g;
  }
  if(b<min) {
    max = b;
  }
  return min;
}

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

  // Check to see if the input is a valid hex color
  if(inputString[strspn(inputString, "0123456789abcdefABCDEF")] != 0) {
      fprintf(stderr, "Invalid hex color\n");
      exit(EXIT_FAILURE);
  }

  inputInt = strtol(inputString, NULL, 16);
  printf("%i\n", (int)inputInt);

  // Let's split our hex code into their respective r, g, and b values
  r = (double) ((inputInt >> 16) & 0xff);
  printf("Red: %f\n", r);
  g = (double) ((inputInt >> 8) & 0xff);
  printf("Green: %f\n", g);
  b = (double) (inputInt & 0xff);
  printf("Blue: %f\n", b);

  // Now, let's convert our rgb values into HSL (thanks to dystopiancode.blogspot.com for the algorithm)
  double h = 0.0;
  double s = 0.0;
  double l = 0.0;
  double c = 0.0;

  double M = getMaxColor(r,g,b);
  double m = getMinColor(r,g,b);t


  return(EXIT_SUCCESS);
}
