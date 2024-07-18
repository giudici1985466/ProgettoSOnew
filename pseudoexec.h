#pragma once
#include "disastrOS_pcb.h"


typedef struct{
    int num_args;
    void* arg1;
    void* arg2;
    void* arg3;

} Exec_struct_internal;


typedef struct {
    char lib [50];
    char func [50];
    Exec_struct_internal arguments;
} Exec_struct_external;

typedef enum {
    TIPO_INT,                                   //0
    TIPO_FLOAT,                                 //1
    TIPO_STRING,                                //2
    TIPO_POINTER,                               //3
    TIPO_UNKNOWN                                //4
} TipoDato;

typedef struct {
    void* dato;
    TipoDato tipo;
} Exec_result;




void execFunction(void* args);