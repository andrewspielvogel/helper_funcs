#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "helper_funcs/log.h"

int main( int argc, const char* argv[])
{
  int i;
  timespec res;
  int status;

  fprintf(stderr, "\nFILE %s compiled on %s %s\n",__FILE__,__TIME__,__DATE__); 

  // status = clock_getres(CLOCK_REALTIME, &res);
  // fprintf(stderr, "clock_getres(CLOCK_REALTIME,  &res) returned status = %d res.tv_sec=%ld res.tv_nsec=%ld, clock resolution=%g sec\n",
  // 	  status,
  // 	  res.tv_sec,
  // 	  res.tv_nsec,
  // 	  res.tv_sec + (1e-9*res.tv_nsec)	  
  // 	  );

  // status = clock_getres(CLOCK_MONOTONIC, &res);
  // fprintf(stderr, "clock_getres(CLOCK_MONOTONIC, &res) returned status = %d res.tv_sec=%ld res.tv_nsec=%ld, clock resolution=%g sec\n",	  
  // 	  status,
  // 	  res.tv_sec,
  // 	  res.tv_nsec,
  // 	  res.tv_sec + (1e-9*res.tv_nsec)	  
  // 	  );

  for(i=0; i<500;i++)
    {
      log_this_now_dsl_format(LOG_FID_KVH_FORMAT, (char *) "KVH", (char *) "Hello KVH log file");
      log_this_now_dsl_format(LOG_FID_MST_FORMAT, (char *) "MST", (char *) "Hello Microstrain log file");

      usleep(10000);  // sleep units are microsec
      
    }
}
