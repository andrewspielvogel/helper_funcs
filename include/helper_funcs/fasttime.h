/* ----------------------------------------------------------------------
   
   Fasttime.  

   100% CPU load simulated time-keeper.

   Revision History
   2008-06-16    mvj    Created.

   ---------------------------------------------------------------------- */

#ifndef FASTTIME_H
#define FASTTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>

/* ----------------------------------------------------------------------
   
   Constants.

   FASTTIME_HZ: Sets the fasttimer time granularity.  Best performance 
                should be attained for a value equal to the fastest 
		realtime rate in the system.  As of 2008-Jun-16, the
		fastest rate in the ROV code was 10 Hz.

   Integer time conversion macros.  These are deliberately integer 
   operations.

   FASTTIME_MS2FT/FT2MS: Conversion between fasttime and integer ms.  
                         Must be consistent with FASTTIME_HZ.
   FASTTIME_S2FT/FT2S:   Conversion between fasttime and integer seconds.  
                         Must be consistent with FASTTIME_HZ.


   ---------------------------------------------------------------------- */
#define FASTTIME_HZ 10.0
#define FASTTIME_MS2FT(ms) ((ms)/100) 
#define FASTTIME_FT2MS(ft) ((ft)*100) 
#define FASTTIME_S2FT(s) ((s)*10)
#define FASTTIME_FT2S(ft) ((ft)/10)

/* ----------------------------------------------------------------------

   fasttime_t

   A counter with granularity in terms of real time defined by FASTTIME_HZ

   Revision History
   2008-06-17    mvj    Created.
   2008-06-19    mvj    Removed extern definition in favor of 
                        functionalized API.

   ---------------------------------------------------------------------- */
typedef unsigned long long fasttime_t;
extern fasttime_t fasttime_get(void); 
extern fasttime_t fasttime_tick(void);

#endif
