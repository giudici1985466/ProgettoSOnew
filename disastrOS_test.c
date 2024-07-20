#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
//#include <dlfcn.h>



#include "pseudoexec.h"
#include "disastrOS.h"



void printresult(Exec_struct_external struttura){
  Exec_result risultato=struttura.result;
  printf("*PRITING THE RESULT AFTER THE TERMINATION*\n");
  
  
  switch(*(risultato.tipo)) {
        
        case TIPO_INT:
            printf("The return value of the function is 'int' type:\n");
            printf("%d\n",*(int *)(risultato.dato));
            break;
        case TIPO_FLOAT:
            printf("The return value of the function is 'float' type:\n");
            printf("%f\n",*(float*)risultato.dato);
            break;
        case TIPO_STRING:
            printf("The return value of the function is 'char*' type:\n");
            printf("%s\n",(char*)risultato.dato);
            break;
        case TIPO_POINTER:
            printf("The return value of the function is 'pointer':\n");
            printf("%p\n",(risultato.dato));
            break;
        case TIPO_UNKNOWN:
            printf("The function has no return value\n");
            break;
        default:
            printf("Non valid type\n");
    }

  printf("***************************\n");
}


void initFunction(void* args) {
  int a=10;
  int b=30;
  int c=20;

  float a_f=14.5;
  float b_f=21.6;
  float c_f=32.14;

  int res1,res2,res3,res4,res5,res6,res7;
  int tipo1=TIPO_UNKNOWN,tipo2=TIPO_UNKNOWN,tipo3=TIPO_UNKNOWN,tipo4=TIPO_UNKNOWN,tipo5=TIPO_UNKNOWN,tipo6=TIPO_UNKNOWN,tipo7=TIPO_UNKNOWN;

  Exec_struct_external external_struct_array [7];


  Exec_struct_external estr1 ={PSEUDOEXEC_TESTLIB,"testlib_triangle_print",{1,&a,0,0},{&res1,&tipo1}};
  external_struct_array [0]= estr1;
  
  Exec_struct_external estr2 ={PSEUDOEXEC_TESTLIB,"testlib_rectangle_print",{2,&a,&b,0},{&res2,&tipo2}};
  external_struct_array [1]= estr2;
  
  Exec_struct_external estr3 ={PSEUDOEXEC_TESTLIB,"testlib_cube_volume",{3,&a_f,&b_f,&c_f},{&res3,&tipo3}};
  external_struct_array [2]= estr3;
  
  Exec_struct_external estr4 ={PSEUDOEXEC_TESTLIB,"testlib_max_int",{2,&a,&b,0},{&res4,&tipo4}};
  external_struct_array [3]= estr4;
  
  Exec_struct_external estr5 ={PSEUDOEXEC_TESTLIB,"testlib_max_int",{1,&c,0,0},{&res5,&tipo5}};
  external_struct_array [4]= estr5;

  Exec_struct_external estrerr1 = {"./fictionallib.so","testlib_triangle_print",{1,&a,0,0},{&res6,&tipo6}};
  external_struct_array [5]= estrerr1;
  
  Exec_struct_external estrerr2 = {PSEUDOEXEC_TESTLIB,"fictional_function",{1,&a,0,0},{&res7,&tipo7}};
  external_struct_array [6]= estrerr2;

  
  
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
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n", pid, retval, alive_children);
    printresult(external_struct_array[pid-1]);
    
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
