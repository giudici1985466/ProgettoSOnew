#pragma once
#include "disastrOS_pcb.h"

#define MAX_PSEUDOEXEC_ARGS 3
#define MAX_LENGTH_LIBNAME 20
#define MAX_LENGTH_SYMBOLNAME 50

#define TIPO_INT 0                                   //0
#define TIPO_FLOAT 1                              //1
#define TIPO_STRING 2                                //2
#define TIPO_POINTER 3                               //3
#define TIPO_UNKNOWN 4                              //4


typedef struct{
    int num_args;
    void* arg1;
    void* arg2;
    void* arg3;

} Exec_struct_internal;

typedef struct {
    void* dato;
    int* tipo;
} Exec_result;



typedef struct {
    char lib [MAX_LENGTH_LIBNAME];
    char func [MAX_LENGTH_SYMBOLNAME];
    Exec_struct_internal arguments;
    Exec_result result;
} Exec_struct_external;



void execFunction(void* args);