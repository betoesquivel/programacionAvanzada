#include <stdio.h>
#include <stdlib.h>

int getCoreCount() {
  FILE * fp;
  char res[128] = {0};
  fp = popen("/bin/cat /proc/cpuinfo |grep -c '^processor'","r");
  fread(res, 1, sizeof(res)-1, fp);
  fclose(fp);
  if (res)
        return atoi(res);
  return 0;
}
