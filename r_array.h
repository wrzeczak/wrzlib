//-----------------------------------------------------------------------------//
// R_ARRAY - A basic resizeable (dynamic?) string/int array written for and by //
// wrzeczak 7/6/2025. Single header, no dependencies (stdlib + wrzlib).        //
//-----------------------------------------------------------------------------//


#pragma once

#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "messages.h"

//-----------------------------------------------------------------------------

typedef enum {
    RA_STRING,
    RA_INT
} ra_type;

union ra_value {
    char * s_value;
    int i_value;
};
typedef union ra_value ra_value;

typedef struct {
    ra_value * elements;
    int count;
    int size;
    ra_type type;
} r_array;

#define RA_ERROR_RETURN 11                                  // value returned on error by all ra_ functions

int __arrays_defined = 0;

//-----------------------------------------------------------------------------

#define ra_typename(ra) (ra->type == RA_INT) ? "RA_INT" : "RA_STRING"

r_array * ra_create(int size, ra_type type);                // initialize r_array of size 0; size for RA_INT should be sizeof(int) and maximum string length for RA_STRING
void ra_destroy(r_array * ra);                              // free pointer created by ra_create()
void ra_exit();                                             // checks __arrays_defined and crashes if non-zero; should be used at end of main() in lieu of return 0;

r_array * ra_resize(r_array * ra, int size);                // (only for RA_STRING) change size of elements, errors if resize fails i.e. elements to large to be shrunk 
r_array * ra_shrink(r_array * ra);                          // (only for RA_STRING) make elements of ra as small as possible, returns new array
r_array * ra_expand(r_array * ra);                          // (only for RA_STRING) expands elements of ra to the nearest power of two for memory padding or whatever

void ra_append(r_array * ra, ra_value element);             // append element to the end of ra
r_array * ra_concat(r_array * ra, r_array * rb);            // appends b to the end of a, e.g. {1, 2, 3} + {4, 5, 6} = {1, 2, 3, 4, 5, 6}, returns concatenated list
void ra_insert(r_array * ra, int idx, ra_value element);    // inserts element at idx, shifting everything behind it, e.g. {1, 2, 3} ra_insert(1, 7) = {1, 7, 2, 3}
void ra_inject(r_array * ra, int idx, r_array * rb);        // inserts rb at idx in ra, shifting everything behind it as in ra_insert()
r_array * ra_slice(r_array * ra, int start, int end);       // returns the array from [start, end] inclusive e.g {0, 1, 2, 3, 4, 5} ra_slice(2, 4) = {2, 3, 4}

int is_member(r_array * ra, ra_value element);              // returns -1 if element not in ra, otherwise returns index where element is in ra
bool ra_cmp(r_array * a, r_array * b);                      // returns true if a and b contain identical values in the same order
bool ra_loosecmp(r_array * a, r_array * b);                 // returns true if a and b contain identical value, not necessarily in the same order or of the same frequency

ra_value ra_rmidx(r_array * ra, int idx);                   // remove item from ra at idx, returns value removed or (ra_value) (char *) NULL if error (prints warning)
ra_value ra_rmval(r_array * ra, ra_value element);          // remove first occurence of element in ra, returns same as ra_rmidx()
ra_value ra_pop(r_array * ra);                              // remove and return last element of ra
ra_value ra_behead(r_array * ra);                           // remove and return first element of ra

void ra_set(r_array * ra);                                  // remove all duplicates in ra, modifies ra (equivalent to list(set()) in python)

void ra_repr(r_array * ra);                                 // single-line print ra
void ra_pprint(r_array * ra, char * label);                 // pretty print ra with an optional label (pass ra, NULL to print no label)

//-----------------------------------------------------------------------------

r_array * ra_create(int size, ra_type type) {
    r_array * ra = (r_array *) malloc(sizeof(r_array));
    ra->size = size;
    ra->type = type;

    ra->count = 0;
    ra->elements = malloc(0);

    __arrays_defined++;

    return ra;
}

void ra_destroy(r_array * ra) {
    if(ra->type == RA_STRING)
        for(int i = 0; i < ra->count; i++)
            free(ra->elements[i].s_value);

    free(ra);

    __arrays_defined--;
}

void ra_exit() {
    if(__arrays_defined != 0) {
        eprintf("RA_SAFETY check failed! Current number of defined arrays: %d", __arrays_defined);
        exit(RA_ERROR_RETURN);
    }

    exit(0);
}

r_array * ra_resize(r_array * ra, int size) {
    if(ra->type == RA_INT) {
        wprintf("ra_resize(size = %d) called on array of type RA_INT; this is not supported behavior...", size);
        return ra;
    }

    for(int i = 0; i < ra->count; i++) {
        if(strlen(ra->elements[i].s_value) >= size) {
            eprintf("ra_resize(size = %d) failed because element %d (strlen = %d) could not be shrunk without data loss.", size, i, strlen(ra->elements[i].s_value));
            exit(RA_ERROR_RETURN);
        }
    }

    r_array * ro = ra_create(size, ra->type);

    char sc_buffer[size];
    memset(sc_buffer, 0, size);

    for(int i = 0; i < ra->count; i++) {
        strncpy(sc_buffer, ra->elements[i].s_value, size);
        ra_append(ro, (ra_value) (char *) sc_buffer);
        memset(sc_buffer, 0, size);
    }

    return ro;
}

r_array * ra_shrink(r_array * ra) {
    if(ra->type == RA_INT) {
        wprint("ra_shrink() called on array of type RA_INT; this is not supported behavior...");
        return ra;
    }

    int shrink_size = -1;

    for(int i = 0; i < ra->count; i++)
        if((int) strlen(ra->elements[i].s_value) > shrink_size)
            shrink_size = (int) strlen(ra->elements[i].s_value);

    return ra_resize(ra, shrink_size + 1);
}

r_array * ra_expand(r_array * ra) {
    if(ra->type == RA_INT) {
        wprint("ra_expand() called on array of type RA_INT; this is not supported behavior...");
        return ra;
    }

    r_array * ro = ra_shrink(ra);

    int expand_size = -1;
    int e = 0;

    while(expand_size < ro->size) {
        e++;
        expand_size = (int) pow(2, e);
    }

    return ra_resize(ra, expand_size);
}

void ra_append(r_array * ra, ra_value element) {
    if(ra->type == RA_STRING) {
        if(strlen(element.s_value) > ra->size) {
            eprintf("ra_append: String passed (length %d) exceeds maximum size of array elements (%d)\n%23cString passed: \"%s\"", strlen(element.s_value), ra->size, ' ', element.s_value);
            exit(RA_ERROR_RETURN);
        }
    }

    ra->elements = realloc(ra->elements, (ra->count + 1) * sizeof(ra_value));
    
    if(ra->type == RA_STRING) {
        ra->elements[ra->count] = (ra_value) (char *) malloc(ra->size);
        strncpy(ra->elements[ra->count].s_value, element.s_value, ra->size);
    } else
        ra->elements[ra->count] = element;

    ra->count++;
}

r_array * ra_concat(r_array * ra, r_array * rb) {
    if(ra->size != rb->size) {
        eprintf("ra_concat() attempted to append two arrays with unequal sizes (ra->size = %d, rb->size = %d)!", ra->size, rb->size);
        exit(RA_ERROR_RETURN);
    }

    if(ra->type != rb->type) {
        eprint("ra_concat() attempted to append two arrays with different types!");
        exit(RA_ERROR_RETURN);
    }

    r_array * ro = ra_create(ra->size, ra->type);

    for(int i = 0; i < ra->count; i++)
        ra_append(ro, ra->elements[i]);

    for(int i = 0; i < rb->count; i++)
        ra_append(ro, rb->elements[i]);

    return ro;
}

void ra_insert(r_array * ra, int idx, ra_value element) {
    todo("ra_insert");
}

void ra_inject(r_array * ra, int idx, r_array * rb) {
    todo("ra_inject");
}

r_array * ra_slice(r_array * ra, int start, int end) {
    todo("ra_slice");
}

int is_member(r_array * ra, ra_value element) {
    if(ra->count == 0)
        return -1;

    int idx = INT_MAX;

    switch(ra->type) {
        case RA_INT: {
            for(int i = 0; i < ra->count; i++)
                if(ra->elements[i].i_value == element.i_value)
                    idx = i;
            break;
        }
    case RA_STRING: {
            for(int i = 0; i < ra->count; i++)
                if(strcmp(ra->elements[i].s_value, element.s_value) == 0)
                    idx = i;
            break;
        }
    }

    if(idx == INT_MAX)
        return -1;

    return idx;
}

bool ra_cmp(r_array * a, r_array * b) {
    if(a->count != b->count)
        return false;

    if(a->type != b->type)
        return false;

    if(a->size != b->size)
        return false;

    switch(a->type) {
        case RA_INT: {
            for(int i = 0; i < a->count; i++)
                if(a->elements[i].i_value != b->elements[i].i_value)
                    return false;
            break;
        }
    case RA_STRING: {
            for(int i = 0; i < a->count; i++)
                if(strcmp(a->elements[i].s_value, b->elements[i].s_value) != 0)
                    return false;
            break;
        }
    }

    return true;
}

bool ra_loosecmp(r_array * a, r_array * b) {
    if(a->type != b->type)
        return false;

    for(int i = 0; i < a->count; i++) {
        ra_value e = a->elements[i];

        if(is_member(b, e) == -1)
            return false;
    }

    return true;
}

ra_value ra_rmidx(r_array * ra, int idx) {
    if((idx < 0) || (idx >= ra->count)) {
        wprintf("ra_rmidx() attempted to remove an element at index %d from an array of length %d.", idx, ra->count);
        return ((ra_value) (char *) NULL);
    }

    ra_value e;
    memcpy(&e, &ra->elements[idx], sizeof(ra_value));

    for(int i = 0; i < ra->count; i++) {
        if(i > idx) {
            memcpy(&ra->elements[i - 1], &ra->elements[i], sizeof(ra_value));
        }
    }

    ra->count--;
    ra->elements = realloc(ra->elements, ra->count * sizeof(ra_value));

    return e;
}

ra_value ra_rmval(r_array * ra, ra_value element) {
    int idx = is_member(ra, element);

    if(idx == -1) {
        switch(ra->type) {
            case RA_INT:
                wprintf("ra_rmval() attempted to remove a value %d that is not contained in the array passed.", element.i_value);
                break;
            case RA_STRING:
                wprintf("ra_rmval() attempted to remove a value '%s' that is not contained in the array passed.", element.s_value);
                break;
        }
        
        return ((ra_value) (char *) NULL);
    }

    return ra_rmidx(ra, idx);
}

ra_value ra_pop(r_array * ra) {
    return ra_rmidx(ra, ra->count - 1);
}

ra_value ra_behead(r_array * ra) {
    return ra_rmidx(ra, 0);
}

void ra_set(r_array * ra) {
    todo("ra_set");
}

void ra_repr(r_array * ra) {
    if(ra->count == 0) {
        printf("[0] (%d)> EMPTY\n", ra->size);
        return;
    }

    printf("[%d] (%d)> ", ra->count, ra->size);

    switch(ra->type) {
        case RA_INT: {
            for(int i = 0; i < ra->count - 1; i++) {
                printf("%d, ", ra->elements[i].i_value);
            }
            printf("%d\n", ra->elements[ra->count - 1].i_value);
            break;
        }
        case RA_STRING: {
            for(int i = 0; i < ra->count - 1; i++) {
                printf("'%s', ", ra->elements[i].s_value);
            }
            printf("'%s'\n", ra->elements[ra->count - 1].s_value);
            break;
        }
    }
}

void ra_pprint(r_array * ra, char * label) {
    if(!(label == NULL))
        printf("%s", label);

    if(ra->count == 0) {
        printf("[0] = { } (empty)\n");
        return;
    }

    if(ra->count == 1) {
        switch(ra->type) {
            case RA_INT: {
                printf("[1] = { d }\n", ra->elements[0].i_value);
                break;
            }
            case RA_STRING: {
                printf("[1] = { '%s' }\n", ra->elements[0].s_value);
                break;
            }
        }
        return;
    }

    printf("[%d] = { ", ra->count);

    char pp_label[256];
    memset(pp_label, 0, 256);
    sprintf(pp_label, "%s[%d] = {", label, ra->count);

    int pp_offset = strlen(pp_label) + 1;

    switch(ra->type) {
        case RA_INT: {
            printf("%d,\n", ra->elements[0].i_value);
            for(int i = 1; i < ra->count - 1; i++) {
                printf("%*c%d,\n", pp_offset, ' ', ra->elements[i].i_value);
            }
            printf("%*c%d }\n", pp_offset, ' ', ra->elements[ra->count - 1].i_value);
            break;
        }
        case RA_STRING: {
            printf("'%s',\n", ra->elements[0].s_value);
            for(int i = 1; i < ra->count - 1; i++) {
                printf("%*c'%s',\n", pp_offset, ' ', ra->elements[i].s_value);
            }
            printf("%*c'%s' }\n", pp_offset, ' ', ra->elements[ra->count - 1].s_value);
            break;
        }
    }
}