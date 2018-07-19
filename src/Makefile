# 2018-07-16 LLW 
CFLAGS=-ggdb -O0 -I ../include

default: log_test

log_test:  time_util.o log.o log_test.o fasttime.o Makefile
	gcc $(CFLAGS) -o log_test log_test.o time_util.o log.o fasttime.o -lm -lrt

log_test.o: log_test.cpp
	gcc $(CFLAGS) -c log_test.cpp

log.o: log.cpp ../include/helper_funcs/log.h ../include/helper_funcs/stderr.h
	gcc $(CFLAGS) -c log.cpp

fasttime.o: fasttime.cpp ../include/helper_funcs/fasttime.h ../include/helper_funcs/stderr.h
	gcc $(CFLAGS) -c fasttime.cpp

time_util.o: time_util.cpp ../include/helper_funcs/time_util.h ../include/helper_funcs/stderr.h
	gcc $(CFLAGS) -c time_util.cpp

clean:
	rm *.o log_test
