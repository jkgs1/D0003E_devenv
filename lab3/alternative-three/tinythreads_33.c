#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tinythreads.h"

#define NULL            0
#define DISABLE()       cli()
#define ENABLE()        sei()
#define STACKSIZE       80
#define NTHREADS        4
#define SETSTACK(buf,a) *((unsigned int *)(buf)+8) = (unsigned int)(a) + STACKSIZE - 4; \
                        *((unsigned int *)(buf)+9) = (unsigned int)(a) + STACKSIZE - 4

struct thread_block {
    void (*function)(int);   // code to run
    int arg;                 // argument to the above
    thread next;             // for use in linked lists
    jmp_buf context;         // machine state
    char stack[STACKSIZE];   // execution stack space
};

struct thread_block threads[NTHREADS];

struct thread_block initp;

thread freeQ   = threads;
thread readyQ  = NULL;
thread current = &initp;

int initialized = 0;



static void initialize(void) {
    int i;
    for (i=0; i<NTHREADS-1; i++)
        threads[i].next = &threads[i+1];
    threads[NTHREADS-1].next = NULL;

    PORTB = (1 << 7);
    EIMSK = (1 << PCIE1);
    PCMSK1 = (1 << PCINT15);
    // Force timer to start at 0
    TCNT1 = 0;
    // Enable timer output compare A
    TIMSK1 = (1 << 1);
    // OC1A high on compare and CTC mode
    TCCR1A = (1 << COM1A1) | (1 << COM1A0);
    // Timer prescaler 1024, CTC mode
    TCCR1B = (1 << CS12) | (1 << CS10) | (1 << WGM12);
    // (8MHz / 1024)*0.5
    OCR1A = 3910;
    //bool state = false;

    initialized = 1;
}

static void enqueue(thread p, thread *queue) {
    p->next = NULL;
    if (*queue == NULL) {
        *queue = p;
    } else {
        p->next = *queue;
        *queue =p;
    }
}

static thread dequeue(thread *queue){
    thread p = *queue;
    if (*queue) {
        *queue = (*queue)->next;
    } else {
        // Empty queue, kernel panic!!!
        while (1) ;  // not much else to do...
    }
    return p;
}

static void dispatch(thread next) {
    if (setjmp(current->context) == 0) {
        current = next;
        longjmp(next->context,1);
    }
}
void spawn(void (* function)(int), int arg) {
    thread newp;

    DISABLE();
    if (!initialized) initialize();

    newp = dequeue(&freeQ);
    newp->function = function;
    newp->arg = arg;
    newp->next = NULL;
    if (setjmp(newp->context) == 1) {
        ENABLE();
        current->function(current->arg);
        DISABLE();
        enqueue(current, &freeQ);
        dispatch(dequeue(&readyQ));
    }
    SETSTACK(&newp->context, &newp->stack);

    enqueue(current, &readyQ);
    dispatch(newp);
    ENABLE();
}

void yield(void) {
    enqueue(current, &readyQ);
    dispatch(dequeue(&readyQ));
}

void lock(mutex *m) {
    /*
    if variable unlocked
        lock
    else
        queue running thread
        AND run next thread
    */
    DISABLE();
    if (m->locked == 0){
        m->locked = 1;
    }else {
        enqueue(current, &m->waitQ);
        dispatch(dequeue(&readyQ));
    }
    ENABLE();

}

void unlock(mutex *m) {
    /*
    if waitQ !empty
        run next thread
    else
        locked flag shall reset
    */
    DISABLE();
    if (m->waitQ!=NULL){
        thread p = dequeue(&m->waitQ);
        enqueue(current, &readyQ);
        dispatch(p);
    }else {
        m->locked = 0;
    }
    ENABLE();

}