#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 1028

typedef struct object_t {
    union {
        int value_int; // integer
        char *value_str; // string
    };
    struct object_t *next;
    int marked; // for garbage collection
} object;

typedef struct {
    object *stack[STACK_SIZE];
    object *root;
    int stackSize;
    int nObjects;
} vm;

vm *init_vm() {
    vm *ptr = malloc(sizeof(vm));
    ptr->root = NULL; // maintains a linked list
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
    object *obj = malloc(sizeof(object));
    obj->value_str = "test";
    obj->next = ptr->root;
    obj->marked = 0;

    // update vm
    ptr->root = obj;
    ptr->nObjects += 1;

    // update the stack
    ptr->stack[ptr->stackSize] = obj;
    ptr->stackSize+=1;

    return obj;
}

object *add_string(vm *ptr, char *value) {
    // create object
    object *obj = malloc(sizeof(object));
    obj->value_str = value;
    obj->next = ptr->root;
    obj->marked = 0;

    // update vm
    ptr->root = obj;
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
    object **cur = &ptr->root; // store memory address
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

int main() {
    vm *machine = init_vm();
    // create objects
    object* obj1 = add_int(machine, 5);
    object* obj2 = add_string(machine, "test");

    // pop an object out of the staack
    pop_object(machine);

    // garbage collect the objects
    garbage_collect(machine);

    free_vm(machine);
    return 0;
}
