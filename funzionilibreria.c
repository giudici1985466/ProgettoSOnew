#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#include "pseudoexec.h"


void* testlib_triangle_print (Exec_struct_internal* arguments, Exec_result* result){                //stampa un traingolo prendendo il primo degli argomenti come parametro
    
    int numargs = arguments->num_args;
    if(numargs!=1){
        printf("+++Wrong number of arguments+++\n");
        return NULL;
    }
    
    int height = *(int*)(arguments->arg1);
    for (int i = 1; i <= height; ++i) {
        for (int j = 1; j <= height - i; ++j) {
            printf(" ");
        }
        for (int j = 1; j <= 2 * i - 1; ++j) {
            printf("*");
        }
        printf("\n");
    }
    *(int*)result->tipo=TIPO_UNKNOWN; 
                                                                        //non ritorna nulla, se non il tipo_unkwown 
}

void* testlib_rectangle_print (Exec_struct_internal* arguments, Exec_result* result){             //stampa un rettangolo con i primi due argomenti come dimensioni
    
    int numargs = arguments->num_args;
    if(numargs!=2){
        printf("+++Wrong number of arguments+++\n");
        return NULL;
    }
    
    
    int width= *(int*)(arguments->arg1);
    int height= *(int*)(arguments->arg2);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            printf("*");
        }
        printf("\n");
    }
    *(int*)result->tipo=TIPO_UNKNOWN;                                                                   //non ritorna nulla, se non il tipo_unkwown 
}

void* testlib_cube_volume (Exec_struct_internal* arguments, Exec_result* result){               //calcola il volume di un cubo usando i 3 argomenti come dimensione
    
    int numargs = arguments->num_args;
    if(numargs!=3){
        printf("+++Wrong number of arguments+++\n");
        return NULL;
    }
    
    
    float a = *(float*)(arguments->arg1);
    float b = *(float*)(arguments->arg2);
    float c = *(float*)(arguments->arg3);
    float res=a*b*c;

    *(float*)(result->dato)=res;                                                                //restituisce un valore tramite la struct result
    *(int*)result->tipo=TIPO_FLOAT;  
}

void* testlib_max_int (Exec_struct_internal* arguments, Exec_result* result){                  //calcola il massimo tra gli argomenti forniti
    int num=arguments->num_args;
    int res=-1;

    switch (num)
    {
    case 1:
        res=*(int*)(arguments->arg1);
        break;
    
    case 2:
        int temp1=*(int*)(arguments->arg1);
        int temp2=*(int*)(arguments->arg2);

        if(temp1>temp2){
            res=temp1;
        }else{
            res=temp2;
        }
        break;
    default:
        printf("+++Wrong number of arguments+++\n");
        return NULL;
        break;
    }

    *(int*)(result->dato)=res;                                                                //restituisce un valore tramite la struct result
    *(int*)result->tipo=TIPO_INT; 

    
}






