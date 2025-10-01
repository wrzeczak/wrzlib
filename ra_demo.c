#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "messages.h"
#include "r_array.h"

//-----------------------------------------------------------------------------

int main(void) {
    r_array * s = ra_create(128, RA_STRING);
    r_array * n = ra_create(sizeof(int), RA_INT);

    ra_append(s, (ra_value) "Hello");
    ra_append(s, (ra_value) "my");
    ra_append(s, (ra_value) "name");
    ra_append(s, (ra_value) "is");
    ra_append(s, (ra_value) "Wrzeczak!");

    ra_pprint(s, "name");

    for(int i = 0; i < 5; i++)
        ra_append(n, (ra_value) (i * i + 1));

    ra_pprint(n, "numbers");

    printf("Removing '%s' from s, and putting in '%s'!\n", ra_rmval(s, (ra_value) "Wrzeczak!"), "Rorshach");
    ra_append(s, (ra_value) "Rorshach!");

    ra_repr(s);

    ra_destroy(s);

    printf("Removing the last number from n (%d)!\n", ra_pop(n));

    ra_repr(n);

    ra_append(n, n->elements[0]);
    ra_append(n, n->elements[1]);

    // ra_repr(n);

    // printf("Making a set of n...\n");

    // ra_set(n);

    ra_repr(n);

    ra_destroy(n);

    ra_exit();
}