#include <stdio.h>
#include <stdlib.h>

#include "gc.h"

int main() {
    // create the virtual machine
    vm *machine = init_gc();

    // allocate data
    object* obj1 = add_int(machine, 5);
    object* obj2 = add_string(machine, "test");

    // update objects
    update(obj1, 10);
    update(obj2, "test2");

    // pop an object out of the stack
    pop_object(machine);

    // garbage collect the objects
    garbage_collect(machine);

    // free the virtual machine
    free_vm(machine);
    return 0;
}
