#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <dlfcn.h>
#include <gnu/lib-names.h>

#include "disastrOS.h"


#include "pseudoexec.h"



void execFunction(void* args){
    void *handle;
    void (*funzione)(Exec_struct_internal*, Exec_result*);
    int res=0;
    Exec_result risultato={&res,TIPO_UNKNOWN};
    char *error;
    Exec_struct_external *values=(Exec_struct_external*) args;

    handle = dlopen(values->lib, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return;
    }


    dlerror();    /* Clear any existing error */

    *(void **) (&funzione) = dlsym(handle, values->func);

    error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        return;
    }
    printf("------------------------------------------------------\n\n\n");
    printf("Hello, I am the child function %d\n",disastrOS_getpid());
    
    printf("I will execute the requested function given the arguments in the exec_struct_internal, populate the exec_result struct with the result,print it and then terminate\n");
    
    funzione(&(values->arguments),&risultato);              //chiamata della funzione
    //printf("%f\n", (*cosine)(*(int*)(values->arg1)));
    switch(risultato.tipo) {
        case TIPO_INT:
            printf("The return value of the function is 'int' type:\n");
            printf("%d\n",*(int*)risultato.dato);
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
            break;
        case TIPO_UNKNOWN:
            printf("The function has no return value\n");
            printf("%p\n",risultato.dato);
            break;
        default:
            printf("Non valid type\n");
    }

    printf("Lib: %s\n",values->lib);
    printf("Symbol: %s\n",values->func);
    printf("#arguments: %d\n",values->arguments.num_args);
  
    
  
    printf("------------------------------------------------------\n\n\n");
    dlclose(handle);
    printf("PID: %d, terminating\n", disastrOS_getpid());
    disastrOS_exit(disastrOS_getpid()+1);
}