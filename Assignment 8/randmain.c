#include "randcpuid.h"

#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool glue(unsigned long long x, int countBytes) {
  int countDigits = countBytes * 2;
  do {
    if (putchar("0123456789abcdef"[x & 0xf]) < 0) { return false; }
    x >>= 4;
    countDigits--;
  } while (0 < countDigits);
  return 0 <= putchar('\n');
}

int main(int argc, char **argv)
{
  bool argumentIs = false;
  long long countBytes;
  if (argc == 2) {
    char *endptr;
    errno = 0;
    countBytes = strtoll(argv[1], &endptr, 10);
    if (errno) perror(argv[1]);
    else argumentIs = !*endptr && 0 <= countBytes;
  }
  if (!argumentIs) {
    fprintf(stderr, "%s: usage: %s countBytes\n", argv[0], argv[0]);
    return 1;
  }
  if (countBytes == 0)
    return 0;
  unsigned long long(*rand64) (void);

  void* lib;
  if (rdrand_supported())
    {
      lib = dlopen("randlibhw.so", RTLD_NOW);
      if (lib == NULL) {
	printf("Error: cannot open randlibhw.so: %s\n", dlerror());
	exit(1);
      }
      rand64 = dlsym(lib, "hardware_rand64");
      if (dlerror()) {
	printf("Error: cannot find hardware_rand64: %s\n", dlerror());
	exit(1);
      }
    }
  else {
    lib = dlopen("randlibsw.so", RTLD_NOW);
    if (lib == NULL) {
      printf("Error: cannot open randlibsw.so: %s \n", dlerror());
      exit(1);
    }
    rand64 = dlsym(lib, "software_rand64");
    if (dlerror()) {
      printf("Error: cannot find software_rand64: %s\n", dlerror());
      exit(1);
    }
  }
  int wordsize = sizeof rand64();
  int output_errno = 0;
  do {
    unsigned long long x = rand64();
    int outbytes = countBytes < wordsize ? countBytes : wordsize;
    if (!glue(x, outbytes))
      {
	output_errno = errno;
	break;
      }
    countBytes -= outbytes;
  } while (0 < countBytes);
  if (fclose(stdout) != 0)
    output_errno = errno;
  if (output_errno) {
    errno = output_errno;
    perror("output");
    return 1;
  }
  dlclose(lib);
  return 0;
}
