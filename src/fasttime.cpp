/* ----------------------------------------------------------------------
   
   Fasttime.  

   100% CPU load simulated time-keeper.

   Revision History
   2008-06-16    mvj    Created.

   ---------------------------------------------------------------------- */

#include "helper_funcs/fasttime.h"

/* ----------------------------------------------------------------------
   
   Fasttime counter.  Starts at zero.

   Revision History
   2008-06-19    mvj    Re-implemented for functionalized API.

   ---------------------------------------------------------------------- */
fasttime_t FastTime = 0;


/* ----------------------------------------------------------------------
   
   fasttime API

   fasttime_get():  Return current fasttime.
   fasttime_tick(): Increment the fasttime count.

   Thread safety: The fasttime count should be incremented by the 
   fasttimer thread exclusively.  It is guaranteed to do so when
   no other threads are running.

   Revision History
   2008-06-19    mvj    Created in favor of extern global.

   ---------------------------------------------------------------------- */
fasttime_t fasttime_get(void)
{
  return(FastTime);
}
fasttime_t fasttime_tick(void)
{
  return(++FastTime);
}
