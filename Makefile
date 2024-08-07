CC=gcc
CCOPTS=--std=gnu99 -Wall  -Ofast -D_DISASTROS_DEBUG_
AR=ar
HEADERS=disastrOS.h\
	disastrOS_constants.h\
	disastrOS_globals.h\
	disastrOS_pcb.h\
	disastrOS_syscalls.h\
	disastrOS_timer.h\
	linked_list.h\
	pool_allocator.h\
	pseudoexec.h

OBJS=pool_allocator.o\
     linked_list.o\
     disastrOS_timer.o\
     disastrOS_pcb.o\
     disastrOS.o\
     disastrOS_wait.o\
     disastrOS_fork.o\
     disastrOS_spawn.o\
     disastrOS_exit.o\
     disastrOS_sleep.o\
     disastrOS_shutdown.o\
     disastrOS_schedule.o\
     disastrOS_preempt.o\
	 pseudoexec.o#\
	 disastrOS_PSEUDOEXEC.o
	 

LIBS=libdisastrOS.a

BINS=disastrOS_test

TESTLIB=libtestlib.so

#disastros_test

.phony: clean all


all:	$(LIBS) $(BINS) $(TESTLIB)

%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

libdisastrOS.a: $(OBJS) 
	$(AR) -rcs $@ $^
	$(RM) $(OBJS)

disastrOS_test:		disastrOS_test.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^

libtestlib.so:	funzionilibreria.c	
	gcc -shared -o libtestlib.so -fPIC funzionilibreria.c

clean:
	rm -rf *.o *~ $(LIBS) $(BINS) $(TESTLIB)
