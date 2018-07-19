/* ----------------------------------------------------------------------

   Time functions

   Modification History:
   DATE         AUTHOR  COMMENT
   23-JUL-2000  LLW     Created and written.
   22-Apr-2001  LLW     Modified for unix or win32 compile
   06-Jan-2004  LLW     Corrected several bugs which caused problems
                        when PC clock was not set to GMT
   21-Jan-2005  SCM     Added support for unix to renav functions
                        Also added added rov unique functions as well
   11-Jun-2008  LLW     Fixed numerous nasties in 2005 unix version
   2008-08-13    mvj    Fixed non-threadsafe use of gmtime.
   2018-07-18   LLW     revised to extend precision of clock from ms to perhaps ns, OS dependent, for ROV_TIME_MODE_NORMAL 
                        ROV_TIME_MODE_RENAV and ROV_TIME_MODE_FASTTIME are still 1ms resolution
---------------------------------------------------------------------- */
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifdef __WIN32__
  // win32 headers
  #include <time.h>
  #include <sys\timeb.h>
  #include <dos.h>
#else
  // unix headers
  #include <time.h>
  #include <sys/time.h>
  #include <sys/timeb.h>
  #include <unistd.h>
#endif

#include "helper_funcs/time_util.h"		/* time utilities */
#include "helper_funcs/fasttime.h"           /* defines fasttime type */

/* variables used for controlling time */
#define ROV_TIME_MODE_SYSTEM 0  /* Normal time, use O/S time */
#define ROV_TIME_MODE_RENAV  1  /* fake time, use atrificial time */

static int ROV_TIME_MODE = ROV_TIME_MODE_NORMAL;
static double  renav_time = {0.0};  /* seconds since 1970 */

/* ----------------------------------------------------------------------

   Modification History:
   DATE         AUTHOR  COMMENT
   11-Jan-2004  LLW      Created and written.
   21-Jan-2005  SCM      Added function to fit current model
---------------------------------------------------------------------- */
void rov_time_mode_set(int mode)
{

  ROV_TIME_MODE = mode;
  
}

/* ----------------------------------------------------------------------

   Modification History:
   DATE         AUTHOR  COMMENT
   11-Jan-2004  LLW      Created and written.

---------------------------------------------------------------------- */
int rov_time_mode_get( void )
{

  return ROV_TIME_MODE;

}

/* ----------------------------------------------------------------------

   Modification History:
   DATE         AUTHOR  COMMENT
   11-Jan-2004  LLW      Created and written.

---------------------------------------------------------------------- */
void rov_time_mode_get(int * mode )
{

  mode = &ROV_TIME_MODE;

}


/* ----------------------------------------------------------------------

   Modification History:
   DATE         AUTHOR  COMMENT
   11-Jan-2004  LLW      Created and written.

---------------------------------------------------------------------- */
void rov_time_set( double secs_since_1970 )
{

  renav_time = secs_since_1970;

}

/* ----------------------------------------------------------------------

   Modification History:
   DATE         AUTHOR  COMMENT
   11-Jan-2004  LLW     Created and written.
   19-JUL-2005  SCM     added unix support
   11-Jun-2008  LLW     Deleted redundant code from 2005 unix version
---------------------------------------------------------------------- */
void rov_time_set( int year, int month, int day, int hour, int min, double sec )
{

  struct tm t;
  time_t    tt;

  // set up tm data structure
  t.tm_year = year - 1900;
  t.tm_mon  = month - 1;
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min  = min;
  t.tm_sec   = (int) floor(sec);
  t.tm_isdst = 0;

  // call mktime to compute time_t time integer seconds since 1970
  tt = mktime(&t);

  // compute sum of integer and fractional seconds since 1970
  renav_time = ((double) tt) +  fmod( sec, 1.0);

}



/* ----------------------------------------------------------------------

   Modification History:
   DATE         AUTHOR  COMMENT
   11-Jan-2004  LLW     Created and written.
   11-Jun-2008  LLW     Check sscanf conversion.  
                        Return success status: 0 if success, -1 if fail
---------------------------------------------------------------------- */
int rov_time_set(char * string)
{

  //time_t  tt;
  //struct tm t;
  int month;
  int day;
  int year;
  int hour;
  int min;
  double sec;
  int status;

  status = sscanf(string, "%d/%d/%d %d:%d:%lf", &month, &day, &year, &hour, &min, &sec);

  if ( status == 6)
    {
      rov_time_set( year, month, day, hour, min, sec);
      return(0);
    }
  else
    return(-1);
 
}

/* ----------------------------------------------------------------------

   Modification History:
   DATE         AUTHOR  COMMENT
   11-Jan-2004  LLW     Created and written.
   19-JUL-2005  SCM     added unix support
   11-Jun-2008  LLW     Deleted redundant code from 2005 unix version
---------------------------------------------------------------------- */
rov_time_t rov_time_compute( int year, int month, int day, int hour, int min, double sec )
{

  struct tm t;
  time_t    tt;
  rov_time_t rov_time;

  // set up tm data structure
  t.tm_year = year - 1900;
  t.tm_mon  = month - 1;
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min  = min;
  t.tm_sec  = (int) floor(sec);
  t.tm_isdst = 0;

  // call mktime to compute time_t time integer seconds since 1970
  tt = mktime(&t);

  // compute sum of integer and fractional seconds since 1970
  rov_time = ((double) tt) +  fmod( sec, 1.0);
  
  return rov_time;

}





/* ----------------------------------------------------------------------

   Modification History:
   DATE         AUTHOR  COMMENT
   23-JUL-2000  LLW      Created and written.

---------------------------------------------------------------------- */
int rov_sprintf_hour_min_only(char * str, rov_time_t t)
{

  // get secs today at surface
  double sec_today;
  double hr;
  double min;
  int    len;

  // compute secs
  sec_today = fmod(t, 3600.0 * 24.0);

  hr  = floor(sec_today / 3600.0);
  min = floor(fmod(sec_today, 3600.0) / 60.0);

  len = sprintf(str,"%02.0f:%02.0f",hr, min);

  return len;

}



/* ----------------------------------------------------------------------


   Modification History:
   DATE         AUTHOR  COMMENT
   23-JUL-2000  LLW      Created and written.

---------------------------------------------------------------------- */
int rov_sprintf_hour_min_sec_only(char * str, rov_time_t t)
{

  // get secs today at surface
  double sec_today;
  double hr;
  double min;
  double sec;
  int    len;

  // compute secs
  sec_today = fmod(t, 3600.0 * 24.0);

  hr  = floor(sec_today / 3600.0);
  min = floor(fmod(sec_today, 3600.0) / 60.0);
  sec = fmod(sec_today,60.0);

  len = sprintf(str,"%02.0f:%02.0f:%02.0f",hr, min, sec);

  return len;

}


/* ----------------------------------------------------------------------

   difference between two time values

   Modification History:
   DATE         AUTHOR  COMMENT
   23-JUL-2000  LLW      Created and written.

---------------------------------------------------------------------- */
double rov_diff_time(rov_time_t t1, rov_time_t t0)

{
  return (t1 - t0);
}


/* ----------------------------------------------------------------------

   get time (since 1970?) in seconds, LSD is microseconds, granularity is
   not known

   Modification History:
   DATE         AUTHOR  COMMENT
   23-JUL-2000  LLW     Created and written.
   22-Apr-2001  LLW     Modified for unix or win32 compile
   06-Jan-2004  LLW     Corrected bugs which caused problem if
                        WIN32 PC clock is not set to GMT time zone
   11-Jun-2008  LLW     Removed needless OS specific code

---------------------------------------------------------------------- */
rov_time_t rov_get_time(void)
{
   rov_time_struct_t t;

   t = rov_get_time_struct();

   return t.sec_rov_time;
}

/* ----------------------------------------------------------------------

   get time (since 1970?) in seconds, LSD is microseconds, granularity is
   not known

   Modification History:
   DATE         AUTHOR  COMMENT
   23-JUL-2000  LLW     Created and written.
   22-Apr-2001  LLW     Modified for unix or win32 compile
   06-Jan-2004  LLW     Corrected bugs which caused problem if
                        WIN32 PC clock is not set to GMT time zon
   07-JUL-2005  SCM     added support for renav time SCM 7/18/05
   11-Jun-2008  LLW     Removed needless OS specific code
---------------------------------------------------------------------- */
rov_time_t rov_get_time(int time_mode)
{
   rov_time_struct_t t;

   t = rov_get_time_struct(time_mode);

   return t.sec_rov_time;

}



/* ----------------------------------------------------------------------

   sprintfs dsl data time string based on computer system clock

   MODIFICATION HISTORY
   DATE         WHO             WHAT
   -----------  --------------  ----------------------------
   14 APR 1999  Louis Whitcomb  Created and Written
   22-Apr-2001  LLW             Modified for unix or win32 compile
   09 JAN 2004 LLW              Modified to use time_util.cpp
   18-JUL-2005 SCM              added support for renav time SCM 7/18/05
   11-Jun-2008  LLW             Removed needless OS specific code
   2008-06-19    mvj            Overloaded to get specific time.
   2018/02/16 21:22:29 LLW log time to us not ms 
   2018-07-16 LLW  reverted to ms time stamps, above edit was nfg

   ---------------------------------------------------------------------- */
int rov_sprintf_dsl_time_string(char * str, int time_mode)
{

   // 09 JAN 2004 LLW Modified to use time_util.cpp
   rov_time_struct_t     now;
   int    num_chars;

   // 09 JAN 2004 LLW Modified to use time_util.cpp
   now = rov_get_time_struct(time_mode);

   if ((time_mode == ROV_TIME_MODE_RENAV) || (time_mode == ROV_TIME_MODE_FASTTIME))
     // 2018-07-18 LLW old code uses ms resolution fractional seconds     
     {

       // 2018/02/16 21:22:29 LLW log time to us not ms 
       // 2018-07-16 LLW  reverted to ms time stamps, 2018/02/16edit was nfg
       num_chars = sprintf(str,"%02d/%02d/%02d %02d:%02d:%02d.%03d",
			   now.year,
			   now.month,
			   now.day,
			   now.hour,
			   now.min,
			   now.sec_int,
			   now.msec_int);
     }
   else
     // 2018-07-18 LLW new code uses ns resolution fractional seconds
     {
       num_chars = sprintf(str,"%02d/%02d/%02d %02d:%02d:%02d.%09ld",
			   now.year,
			   now.month,
			   now.day,
			   now.hour,
			   now.min,
			   now.sec_int,
			   now.clock_gettime_nsec_int);       
     }

   num_chars = strlen(str);

   return num_chars;
}

int rov_sprintf_dsl_time_string(char * str)
{
  return(rov_sprintf_dsl_time_string(str, ROV_TIME_MODE));
}


/* ----------------------------------------------------------------------

   gets a o/s independent time struct

   MODIFICATION HISTORY
   DATE         WHO             WHAT
   -----------  --------------  ----------------------------
   14 APR 1999  Louis Whitcomb  Created and Written
   06 Jan 2004  LLW             Revised to compute time properly
                                regardless of how the PC time zone
                                is set under WIN32
   11-Jun-2008  LLW             Removed needless OS specific code
   ---------------------------------------------------------------------- */
rov_time_struct_t rov_get_time_struct(void)
{

   rov_time_struct_t  now;

   // get time struct for current time mode
   now = rov_get_time_struct(ROV_TIME_MODE);

   return(now);

}



/* ----------------------------------------------------------------------

   gets a o/s independent time struct

   MODIFICATION HISTORY
   DATE         WHO             WHAT
   -----------  --------------  ----------------------------
   14 APR 1999  Louis Whitcomb  Created and Written
   06 Jan 2004  LLW             Revised to compute time properly
                                regardless of how the PC time zone
                                is set under WIN32
   11 Jun 2008  LLW             Modified to function identically under win and linux
   2008-08-13    mvj            Fixed thread unsafe use of gmtime in favor of
                                using gmtime_r.  In fasttime mode realtime 
				(not fasttime) was regularly stomped on.
    2018-07-18   LLW           revised to extend precision of clock from ms to perhaps ns, OS dependent 

   ---------------------------------------------------------------------- */
rov_time_struct_t rov_get_time_struct(int time_mode)
{

  static int first_time = 1;

   struct timeb  ftime_time;
   struct tm     gmtime_time;
   timespec clock_gettime_time;
   // int tz, dl;
   // tz = _timezone;
   // dl = _daylight;

   rov_time_struct_t t;

   
   // 2018-07-18 LLW print high-res clock info on first call
   if( first_time == 1)
     {
       first_time = 0;

       timespec res;
       int status;

       fprintf(stderr, "\nFILE %s compiled on %s %s\n",__FILE__,__TIME__,__DATE__); 

       status = clock_getres(CLOCK_REALTIME, &res);
       fprintf(stderr, "clock_getres(CLOCK_REALTIME,  &res) returned status = %d res.tv_sec=%ld res.tv_nsec=%ld, clock resolution=%g sec\n",
	       status,
	       res.tv_sec,
	       res.tv_nsec,
	       res.tv_sec + (1e-9*res.tv_nsec)	  
	       );

       // status = clock_getres(CLOCK_MONOTONIC, &res);
       // fprintf(stderr, "clock_getres(CLOCK_MONOTONIC, &res) returned status = %d res.tv_sec=%ld res.tv_nsec=%ld, clock resolution=%g sec\n",	  
       // 	       status,
       // 	       res.tv_sec,
       // 	       res.tv_nsec,
       // 	       res.tv_sec + (1e-9*res.tv_nsec)	  
       // 	       );
       
       
     }

   // get seconds since 1970
   // if in RENAV mode, use fake clock
   // 2008-06-17    mvj    Note: this can produce a jitter of 
   //                      up to 1 ms due to truncation error in
   //                      computing _both_ ftime_time.time and 
   //                      ftime_time.millitm (consider when 
   //                      renav_time = 2.99999...9999 (nominally 3.0).
   //                      The fasttime data type has been added to
   //                      avoid this. 
   if (time_mode == ROV_TIME_MODE_RENAV)
     {
       ftime_time.time      = (time_t) floor(renav_time);
       ftime_time.millitm   = (unsigned short int) (fmod(renav_time,1.0) * 1000.0);
       ftime_time.timezone  =  0;
       ftime_time.dstflag   =  0;
     }
   // 2008-06-17    mvj    Added fasttime support.
   // 2008-06-19    mvj    Modified to use functionalized fasttime API.
   else if (time_mode == ROV_TIME_MODE_FASTTIME) 
     {
       fasttime_t ft, fts, ftms;
       
       ft = fasttime_get();
       fts = FASTTIME_FT2S(ft);
       ftms = FASTTIME_FT2MS(ft - FASTTIME_S2FT(fts));

       ftime_time.time      =  (time_t) fts; 
       ftime_time.millitm   =  (unsigned short int) ftms; 
       ftime_time.timezone  =  0;
       ftime_time.dstflag   =  0;
     }
   else // default is to read system time from O/S
     {
       // get integer and fractional seconds since 1970 with ftime()
       
       // 2018-07-18 LLW commented out ftime
       //     ftime(&ftime_time);
       // 2018-07-18 LLW added call to clock_gettime
       clock_gettime(CLOCK_REALTIME, &clock_gettime_time);

       // 2018-07-18 LLW assign integer seconds
       ftime_time.time      =  clock_gettime_time.tv_sec;
       // 2018-07-18 LLW compute integer milliseconds from integer nanoseconds
       ftime_time.millitm   =  clock_gettime_time.tv_nsec / ((long)1000000);
       //       printf("sec=%ld ns=%9ld ftime_time.millitm=%3d\n",clock_gettime_time.tv_sec,clock_gettime_time.tv_nsec,ftime_time.millitm);
	      
       ftime_time.timezone  =  0;
       ftime_time.dstflag   =  0;       
       
     }

   // ------------------------------------------------------
   // BAD method uses seperate calls for secs ans millisecs,
   //     leavinf possibility of phase error
   //get integer secs since 1970 GMT with time()
   // time_time = time(NULL);
   // convert integer secs to a ymdhms structure with gmtime()
   // gmtime_time = *gmtime(&time_time);
   // ------------------------------------------------------

   // ------------------------------------------------------
   // GOOD method compues gmtime struct from ftime resiult,
   //      no chance of phase error
   // convert integer secs to a ymdhms structure with gmtime()
   // 2008-Aug-13 mvj Fixed thread-unsafe use of gmtime with gmtime_r
   gmtime_r(&ftime_time.time, &gmtime_time);

   // ------------------------------------------------------

   // assign results to data structures
   t.year         = gmtime_time.tm_year + 1900;
   t.month        = gmtime_time.tm_mon+1;
   t.day          = gmtime_time.tm_mday;

   t.hour         = gmtime_time.tm_hour;
   t.min          = gmtime_time.tm_min;
   t.sec_int      = gmtime_time.tm_sec;
   t.msec_int     = ftime_time.millitm;
   
   if ((time_mode == ROV_TIME_MODE_RENAV) || (time_mode == ROV_TIME_MODE_FASTTIME))
     // 2018-07-18 LLW old code computes ms resolution fractional seconds     
     {
       t.sec_double   = gmtime_time.tm_sec + (((double)ftime_time.millitm) / 1000.0);
       t.sec_today    = t.sec_double +
	 (60.0 * t.min) +
	 (3600.0* t.hour);
       t.sec_rov_time = ftime_time.time +  (((double)ftime_time.millitm) / 1000.0);
     }
   else 
     // 2018-07-18 LLW new code computes ns resolution fractional seconds
     {
       t.clock_gettime_sec_int   = clock_gettime_time.tv_sec;
       t.clock_gettime_nsec_int	 = clock_gettime_time.tv_nsec;

       t.sec_double   = gmtime_time.tm_sec + (clock_gettime_time.tv_nsec * 1e-9); 
       t.sec_today    = t.sec_double +
	 (60.0 * t.min) +
	 (3600.0* t.hour);
       t.sec_rov_time =  clock_gettime_time.tv_sec + (clock_gettime_time.tv_nsec * 1e-9);
     }

   return t;

}



/* ----------------------------------------------------------------------

   sprintfs dsl data time string based on an input time in seconds

   MODIFICATION HISTORY
   DATE         WHO             WHAT
   -----------  --------------  ----------------------------
   11 Mar 2002  J Howland       created from the above function
   07 JUL 2005  SCM             added from time_util the llw way
   11 Jun 2008  LLW             Corrected bad code - complete rewrite
                                Should not reference time mode.
   2008-06-17    mvj            Some bad code left: references to 
                                renav_time changed to total_sec.
				Note, however, that converting from
				double to integer time is subject to
                                truncation error problems.
   2008-08-13    mvj            Fixed thread-unsafe use of gmtime with
                                gmtime_r.

   ---------------------------------------------------------------------- */
int rov_convert_dsl_time_string (double total_secs, char *str)
{

  int           num_chars;
  struct tm     gmtime_time;
  struct timeb  ftime_time;
  double        sec;

  // setup ftime struct
  ftime_time.time      = (time_t)         floor(total_secs);
  ftime_time.millitm   = (unsigned short) (fmod(total_secs,1.0) * 1000.0);
  ftime_time.timezone  =  0;
  ftime_time.dstflag   =  0;

  // convert to gmtime struct
  // 2008-Aug-13 mvj Fixed use of thread-unsafe gmtime with gmtime_r
  gmtime_r(&ftime_time.time, &gmtime_time);

  sec = fmod (total_secs, 60.0);

  num_chars = sprintf (str, "%04d/%02d/%02d %02d:%02d:%06.3f", 
		       (int) gmtime_time.tm_year +1900, 
		       (int) gmtime_time.tm_mon + 1, 
		       (int) gmtime_time.tm_mday, 
		       (int) gmtime_time.tm_hour, 
		       (int) gmtime_time.tm_min, 
		       sec);


  return num_chars;

}

