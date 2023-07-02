#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 1028

typedef struct object_t {
    union {
        int value_int; // integer
        char *value_str; // string
    };
    struct object_t *next; // keep a linked list

    int marked; // for garbage collection
} object;

typedef struct {
    object *stack[STACK_SIZE];
    object *top; // points at the top of the stack
    int stackSize;
    int nObjects;
} vm;

vm *init_vm() {
    vm *ptr = (vm*)malloc(sizeof(vm));
    ptr->top = NULL; // maintains a linked list
    ptr->stackSize = 0;
    ptr->nObjects = 0;
    return ptr;
}

object *pop_object(vm *ptr) {
    ptr->stackSize--;
    return ptr->stack[ptr->stackSize];
}

object *add_int(vm *ptr, int value) {
    // create object
    object *obj = (object*)malloc(sizeof(object));
    obj->value_int = value;
    obj->next = ptr->top;
    obj->marked = 0;

    // update vm
    ptr->top = obj;
    ptr->nObjects += 1;

    // update the stack
    ptr->stack[ptr->stackSize] = obj;
    ptr->stackSize+=1;

    return obj;
}

object *add_string(vm *ptr, char *value) {
    // create object
    object *obj = (object*)malloc(sizeof(object));
    obj->value_str = value;
    obj->next = ptr->top;
    obj->marked = 0;

    // update vm
    ptr->top = obj;
    ptr->nObjects += 1;

    // update the stack
    ptr->stack[ptr->stackSize] = obj;
    ptr->stackSize+=1;

    return obj;
}

// uses the mark and sweep algorithm
void garbage_collect(vm *ptr) {
    int startObjects = ptr->nObjects; // store the amount of objects during the start of the garbage collection

    // mark all objects on the stack
    for(int i = 0; i < ptr->stackSize; i++) {
        if(ptr->stack[i] == NULL) break;
        ptr->stack[i]->marked = 1;
    }
    // sweep (remove unmarked objects)
    object **cur = &ptr->top; // store memory address
    while(*cur != NULL) {
        if((*cur)->marked == 0) {
            object *to_remove = *cur;
            *cur = (*cur)->next;
            free(to_remove);
            ptr->nObjects -= 1;
        } else {
            (*cur)->marked = 0;
            cur = &(*cur)->next;
        }
    }
    // print out results
    int collected = startObjects - ptr->nObjects;
    printf("Collected %d object(s)\n", collected);
}

void free_vm(vm *ptr) {
    garbage_collect(ptr);
    ptr->stackSize = 0;
    free(ptr);
}
