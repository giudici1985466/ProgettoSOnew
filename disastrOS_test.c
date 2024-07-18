#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <dlfcn.h>
#include <gnu/lib-names.h>


#include "pseudoexec.h"
#include "disastrOS.h"

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void childFunction(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
  for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iterate %d\n", disastrOS_getpid(), i);
    disastrOS_sleep((20-disastrOS_getpid())*5);
  }
  printf("PID: %d, terminating\n", disastrOS_getpid());
  disastrOS_exit(disastrOS_getpid()+1);
}

/*void execFunction(void* args){
  void *handle;
  double (*cosine)(double);
  char *error;
  Exec_struct *values=(Exec_struct*) args;

  handle = dlopen(values->lib, RTLD_LAZY);
  if (!handle) {
    fprintf(stderr, "%s\n", dlerror());
    return;
  }


  //dlerror();     Clear any existing error 

  *(void **) (&cosine) = dlsym(handle, values->func);

   error = dlerror();
  if (error != NULL) {
    fprintf(stderr, "%s\n", error);
    return;
  }
  printf("------------------------------------------------------\n\n\n");
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will calculate the cosine of my pid and then terminate\n");
  printf("%f\n", (*cosine)(*(int*)(values->arg1)));
  printf("%s\n",values->lib);
  printf("%s\n",values->func);
  printf("%d\n",values->num_args);
  
    
  
  printf("------------------------------------------------------\n\n\n");
  dlclose(handle);
  printf("PID: %d, terminating\n", disastrOS_getpid());
  disastrOS_exit(disastrOS_getpid()+1);
}*/




void initFunction(void* args) {
  int a=11;
  Exec_struct_external estr1 ={LIBM_SO,"cos",{1,&a,0,0}};
  
  
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);
  

  printf("I feel like to spawn 10 nice threads\n");
  int alive_children=0;
  for (int i=0; i<10; ++i) {
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  }
  printf("And then 10 more nice threads\n");
  for(int c1=0; c1<10; c1++){
    disastrOS_spawn(execFunction,&estr1);
    alive_children++;
  }

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
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
