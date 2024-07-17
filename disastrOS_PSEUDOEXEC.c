#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"

void internal_pseudoexec(){


    void* handle_dlopen;
    char *error_dlopen;
    void* (*func_dlopen)(void*);
    int num_values_dlopen=0;
    double arg1=0;
    double arg2=0;


    static PCB* new_pcb;                                
    new_pcb=PCB_alloc();                                //definita in disastrOS_pcb.c
    if (!new_pcb) {
        running->syscall_retvalue=DSOS_EPSEUDOEXEC;     //define per l'errore della pseudoexec
        return;
    }

    
    new_pcb->parent = running;                          //il nuovo processo sarà figlio dell'attuale running (a.k.a avrà running come parent)
    new_pcb->status = Ready;                            //setto lo stato del nuovo processo ready


    PCBPtr* new_pcb_ptr=PCBPtr_alloc(new_pcb);
    assert(new_pcb_ptr);                                                                       //per rendere effettivi righe 16 e 17 il processo deve essere aggiunto rispettivamente
    List_insert(&running->children, running->children.last, (ListItem*) new_pcb_ptr);          //nella lista di children del running e in ready list                                                                                     
    List_insert(&ready_list, ready_list.last, (ListItem*) new_pcb);


    running->syscall_retvalue=new_pcb->pid;                                                    //come nella fork restituiamo al parent del nuovo processo creato
                                                                                               //(ossia running) il valore del pid del figlio

    
    //creazione del contesto del nuovo processo
    
    getcontext(&new_pcb->cpu_state);                                                                        //partiamo da una base "conosciuta"
    new_pcb->cpu_state.uc_stack.ss_sp = new_pcb->stack;                                                     //sulla quale applichiamo delle modifiche
    new_pcb->cpu_state.uc_stack.ss_size = STACK_SIZE;
    new_pcb->cpu_state.uc_stack.ss_flags = 0;
    sigemptyset(&new_pcb->cpu_state.uc_sigmask);
    new_pcb->cpu_state.uc_link = &main_context;





    //TODO
    //puntatore a funzione alla funzione che si vuole eseguire (usando dlopen) da mettere poi nel context con la funzione makecontext
    //running->syscall_args[0] è il path della libreria
    //running->syscall_args[1] è il nome della funzione all'interno della libreria stessa
    //running->syscall_args[2] è un array double** NULL terminated (similmente alla execvp)

    /*-------------------------------------------------*/
    handle_dlopen = dlopen(running->syscall_args[0], RTLD_LAZY | RTLD_NODELETE);     //RTLD_LAZY è una flag che indica alla funzione di "risolvere" i simboli nel momento in cui il codice che li usa viene eseguito

    if (!handle_dlopen) {
        running->syscall_retvalue=DSOS_EPSEUDOEXEC;           //Gestione dell'errore
        return;
    }

    dlerror();                                                //Clear di eventuali errori precedenti legati a dlopen



    //cose
    *(void **) (&func_dlopen) = dlsym(handle_dlopen, running->syscall_args[1]);


     error_dlopen = dlerror();
           if (error_dlopen!= NULL) {
               running->syscall_retvalue=DSOS_EPSEUDOEXEC;     //define per l'errore della pseudoexec
               return;
           }

    //printf("%f\n", (*cosine)(2.0));
    dlclose(handle_dlopen);

    //in running->syscall_args[2] avrò l'array NULL terminated come per l'execvp che contiene i parametri da passare alla funzione

    if((running->syscall_args[2])==NULL){
        num_values_dlopen=0;
    }else{
        double** args_dlopen=running->syscall_args[2];
        for(int c1=0;c1<=PSEUDOEXEC_MAX_ARGS;c1++){
            if(args_dlopen[c1]==NULL){
                num_values_dlopen=c1;
                break;
            }else{
                if(c1==0){
                    arg1= *(args_dlopen[c1]);
                }else if(c1==1){
                    arg2 = *(args_dlopen[c1]);
                }
            }

        }
    }



    //void (*new_function) (void*)= (void(*)(void*))  running->syscall_args[0];
    if(num_values_dlopen==0){
        makecontext(&new_pcb->cpu_state, (void(*)())  func_dlopen,0);
    }else if(num_values_dlopen==1){
        makecontext(&new_pcb->cpu_state, (void(*)())  func_dlopen,num_values_dlopen,arg1);
    }else{
        makecontext(&new_pcb->cpu_state, (void(*)())  func_dlopen,num_values_dlopen,arg1,arg2);
    }
    
     /*-------------------------------------------------*/
}
                               