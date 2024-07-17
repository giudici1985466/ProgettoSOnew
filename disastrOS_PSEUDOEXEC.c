#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"

void internal_pseudoexec(){
    static PCB* new_pcb;                                //con "static" la variabile è visibile in tutto il programma
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
    void (*new_function) (void*)= (void(*)(void*))  running->syscall_args[0];
    makecontext(&new_pcb->cpu_state, (void(*)())  new_function, 1, (void*)running->syscall_args[1]);
}
                               