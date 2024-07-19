#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
//#include <dlfcn.h>



#include "pseudoexec.h"
#include "disastrOS.h"

void initFunction(void* args) {
  int a=10;
  int b=30;
  int c=20;

  float a_f=14.5;
  float b_f=21.6;
  float c_f=32.14;

  Exec_struct_external estr1 ={PSEUDOEXEC_TESTLIB,"testlib_triangle_print",{1,&a,0,0}};
  Exec_struct_external estr2 ={PSEUDOEXEC_TESTLIB,"testlib_rectangle_print",{2,&a,&b,0}};
  Exec_struct_external estr3 ={PSEUDOEXEC_TESTLIB,"testlib_cube_volume",{3,&a_f,&b_f,&c_f}};
  Exec_struct_external estr4 ={PSEUDOEXEC_TESTLIB,"testlib_max_int",{2,&a,&b,0}};
  Exec_struct_external estr5 ={PSEUDOEXEC_TESTLIB,"testlib_max_int",{1,&c,0,0}};

  Exec_struct_external estrerr1 = {"./fictionallib.so","testlib_triangle_print",{1,&a,0,0}};
  Exec_struct_external estrerr2 = {PSEUDOEXEC_TESTLIB,"fictional_function",{1,&a,0,0}};

  
  
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  int alive_children=0;


  printf("Testing the lib using pseudoexec \n");
  
  disastrOS_spawn(execFunction,&estr1);
  alive_children++;
  
  disastrOS_spawn(execFunction,&estr2);
  alive_children++;

  disastrOS_spawn(execFunction,&estr3);
  alive_children++;

  disastrOS_spawn(execFunction,&estr4);
  alive_children++;

  disastrOS_spawn(execFunction,&estr5);
  alive_children++;
  
  /*disastrOS_spawn(execFunction,&estrerr1);
  alive_children++;

  disastrOS_spawn(execFunction,&estrerr2);
  alive_children++;
  */

  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  }
  printf("shutdown!");
  disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue

  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
