#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "helper_funcs/log.h"

int main( int argc, const char* argv[])
{
  int i;
  timespec res;
  int status;

  status = clock_getres(CLOCK_REALTIME, &res);
  printf("clock_getres(CLOCK_REALTIME,  &res) returned status = %d res.tv_sec=%ld res.tv_nsec=%ld\n",
	 status,
	 res.tv_sec,
	 res.tv_nsec);

  status = clock_getres(CLOCK_MONOTONIC, &res);
  printf("clock_getres(CLOCK_MONOTONIC, &res) returned status = %d res.tv_sec=%ld res.tv_nsec=%ld\n",
	 status,
	 res.tv_sec,
	 res.tv_nsec);


  printf( "\nHello World\n\n");

  for(i=0; i<100;i++)
    {
      log_this_now_dsl_format(LOG_FID_KVH_FORMAT, (char *) "KVH", (char *) "Hello KVH log file");
      log_this_now_dsl_format(LOG_FID_MST_FORMAT, (char *) "MST", (char *) "Hello Microstrain log file");

      usleep(10000);
      
    }
}
