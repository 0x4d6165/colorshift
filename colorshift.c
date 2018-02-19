#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


int main (int argc, char **argv) {
  // Process Flags
  int lighten = 0;
  double percentage;
  char *lvalue = NULL;
  char *dvalue = NULL;
  opterr = 0;
  int f = getopt(argc, argv, "l:d:");
  if(f == -1) {
    fprintf(stderr, "Usage: %s -d <integer percentage> or -l <integer percentage> <hex color>.\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  switch(f) {
    case 'l':
      lighten = 1;
      lvalue = optarg;
      break;
    case 'd':
      dvalue = optarg;
      break;
    case '?':
        if(optopt == 'l' || optopt == 'd') {
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        }
        else if(isprint(optopt)) {
          fprintf(stderr, "Unknown option `-%c`.\n", optopt);
        }
        else {
          fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
        }

        exit(EXIT_FAILURE);
    default:
      abort();
  }
  if(lighten==1) {
    percentage = (double) atoi(lvalue);
    if(percentage == 0) {
      fprintf(stderr, "Invalid percentage. Either you didn't enter an integer or you tried to lighten by 0 percent.\n");
      exit(EXIT_FAILURE);
    }
  }
  else {
    percentage = (double) atoi(dvalue);
    if(percentage == 0) {
      fprintf(stderr, "Invalid percentage. Either you didn't enter an integer or you tried to darken by 0 percent.\n");
      exit(EXIT_FAILURE);
    }
  }

  char intermBuf[BUFSIZ];
  char inputString[7];
  unsigned long inputInt;
  double r, g, b;

  // Parse input from stdin if there is no input argument
  if(argc == optind) {
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
  else if(argv[optind] != NULL) {
    if(strlen(argv[optind])!=6) {
      fprintf(stderr, "Invalid hex color\n");
      exit(EXIT_FAILURE);
    }
    strncpy(inputString, argv[optind], 6);
  }

  // Check to see if the input is a valid hex color
  if(inputString[strspn(inputString, "0123456789abcdefABCDEF")] != 0) {
      fprintf(stderr, "Invalid hex color\n");
      exit(EXIT_FAILURE);
  }

  inputInt = strtol(inputString, NULL, 16);

  // Let's split our hex code into their respective r, g, and b values
  r = (double) ((inputInt >> 16) & 0xff);
  g = (double) ((inputInt >> 8) & 0xff);
  b = (double) (inputInt & 0xff);

  // Now, let's convert our rgb values into HSL (thanks to dystopiancode.blogspot.com for the algorithm)
  double h = 0.0;
  double s = 0.0f;
  double l = 0.0;
  double c = 0.0;
  r /= 255.0f;
  g /= 255.0f;
  b /= 255.0f;
  double M = fmax(fmax(r,g),b);
  double m = fmin(fmin(r,g),b);
  c = M - m;
  l = (M + m)/2;
  if(c!=0.0) {
    if(M==r){
      h = fmod(((g-b)/c), 6.0);
    }
    else if(M==g) {
      h = ((b-r)/c) + 2.0;
    }
    else {
      h = ((r-g)/c) + 4.0;
    }
    h *= 60.0;
    s = (l<=0.5) ? (c/(M+m)) : (c/(2-M-m));
  }

  // Do the lightening or darkening
  if(lighten) {
    l += percentage/100;
    if(l>1) {
      l = 1;
    }
  }
  else {
    l -= percentage/100;
    if(l<0) {
      l = 0;
    }
  }

  // Convert HSL back to RGB
  double HUE_UPPER_LIMIT = 360;
  double chroma = 0.0;
  double interm = 0.0;
  double x = 0.0;
  chroma = (1.0-fabs(2*l-1.0))*s;
  interm = 1.0*(l-0.5*chroma);
  x = chroma*(1.0-fabs(fmod((h/60.0),2)-1.0));
  if(h>=0.0&&h<(HUE_UPPER_LIMIT/6.0)) {
    r = chroma+interm;
    g = x+interm;
    b = interm;
  }
  else if(h>=(HUE_UPPER_LIMIT/6.0)&&h<(HUE_UPPER_LIMIT/3.0)) {
    r = x+interm;
    g = chroma+interm;
    b = interm;
  }
  else if(h<(HUE_UPPER_LIMIT/3.0)&&h<(HUE_UPPER_LIMIT/2.0)) {
    r = interm;
    g = chroma+interm;
    b = x+interm;
  }
  else if(h>=(HUE_UPPER_LIMIT/2.0)) {
    r = interm;
    g = x+interm;
    b = chroma+interm;
  }
  else if(h>=(2.0*HUE_UPPER_LIMIT/3.0)&&h<(5.0*HUE_UPPER_LIMIT/6.0)) {
    r = x+interm;
    g = interm;
    b = chroma+interm;
  }
  else if(h>=(5.0*HUE_UPPER_LIMIT/6.0)&&h<HUE_UPPER_LIMIT) {
    r = chroma+interm;
    g = interm;
    b = x+interm;
  }
  else {
    r = interm;
    g = interm;
    b = interm;
  }
  r *= 255;
  g *= 255;
  b *= 255;

  // Print the final RGB values as hex
  int roundedR = round(r);
  int roundedG = round(g);
  int roundedB = round(b);
  printf("%02X%02X%02X\n", roundedR, roundedG, roundedB);

  return(EXIT_SUCCESS);
}
