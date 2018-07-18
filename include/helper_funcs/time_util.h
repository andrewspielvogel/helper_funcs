/* ----------------------------------------------------------------------

   Time functions

   Modification History:
   DATE         AUTHOR  COMMENT
   23-JUL-2000  LLW     Created and written.
   22-Apr-2001  LLW     Modified for unix or win32 compile 
   22-JUL-2005  SCM     added renewed unix calls
   11-Jun-2008  LLW     Fixed numerous nasties in 2005 unix version
   
   2018-07-18   LLW     revised to extend precision of clock from ms to perhaps ns, OS dependent, for ROV_TIME_MODE_NORMAL 
                        ROV_TIME_MODE_RENAV and ROV_TIME_MODE_FASTTIME are still 1ms resolution
---------------------------------------------------------------------- */
#ifndef TIME_UTIL_INC
#define TIME_UTIL_INC

// ----------------------------------------------------------------------
// DEBUG FLAG:  Uncomment this and recompile to get verbosr debugging 
// ----------------------------------------------------------------------
// #define DEBUG_TIME
// ----------------------------------------------------------------------

typedef double    rov_time_t;

typedef struct
{
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec_int;  // sec of minute
  int msec_int;
  //  2018-07-18   LLW  revised to extend precision of clock from ms to perhaps ns, OS dependent 
  long clock_gettime_sec_int;
  long clock_gettime_nsec_int;  
  double sec_double;   // 0.0 to 59.9999... seconds of the minute
  double sec_today;    // 0.0 to 86,399.9999.. seconds of the day
  double sec_rov_time; /* 0.0 to decimal seconds  unix time - time since midnight GMT beginning Jan 1, 1970 */
} rov_time_struct_t;


extern rov_time_struct_t   rov_get_time_struct();
extern rov_time_struct_t   rov_get_time_struct(int time_mode);

extern rov_time_t          rov_get_time(void);
extern rov_time_t          rov_get_time(int time_mode);

extern double              rov_diff_time(rov_time_t t1, rov_time_t t0);
extern int                 rov_sprintf_dsl_time_string(char * str, int time_mode);
extern int                 rov_sprintf_dsl_time_string(char * str);

extern int                 rov_sprintf_hour_min_only(char * str, rov_time_t t);
extern int                 rov_sprintf_hour_min_sec_only(char * str, rov_time_t t);
extern int                 rov_sprintf_hour_min_sec_only(char * str, rov_time_t t);


extern void  rov_time_mode_set( int mode);

extern int  rov_time_mode_get( void );
extern void rov_time_mode_get( int * mode );

extern void rov_time_set( double secs_since_1970 );
extern void rov_time_set( int year, int month, int day, int hour, int min, double sec );
extern int  rov_time_set( char * string);

extern rov_time_t rov_time_compute( int year, int month, int day, int hour, int min, double sec );
extern int rov_convert_dsl_time_string(double total_secs, char *str);

/* variables used for controlling time */
#define ROV_TIME_MODE_NORMAL 0  /* Normal time, use O/S time */
#define ROV_TIME_MODE_RENAV  1  /* fake time, use atrificial time */
#define ROV_TIME_MODE_FASTTIME 2  /* Spoofed time using integer time type */

#endif



