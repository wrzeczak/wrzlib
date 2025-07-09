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

//-----------------------------------------------------------------------------

#define ra_typename(ra) (ra->type == RA_INT) ? "RA_INT" : "RA_STRING"

r_array * ra_create(int size, ra_type type);                // initialize r_array of size 0; size for RA_INT should be sizeof(int) and maximum string length for RA_STRING
void ra_destroy(r_array * ra);                              // free pointer created by ra_create()

void ra_append(r_array * ra, ra_value element);             // append element to the end of ra
void ra_concat(r_array * ra, r_array * rb);                 // appends b to the end of a, e.g. {1, 2, 3} + {4, 5, 6} = {1, 2, 3, 4, 5, 6}
void ra_insert(r_array * ra, int idx, ra_value element);    // inserts element at idx, shifting everything behind it, e.g. {1, 2, 3} ra_insert(1, 7) = {1, 7, 2, 3}
void ra_inject(r_array * ra, int idx, r_array rb);          // inserts rb at idx in ra, shifting everything behind it as in ra_insert()

int is_member(r_array * ra, ra_value element);              // returns -1 if element not in ra, otherwise returns index where element is in ra
bool ra_cmp(r_array * a, r_array * b);                      // returns true if a and b contain identical values in the same order
bool ra_loosecmp(r_array * a, r_array * b);                 // returns true if a and b contain identical value, not necessarily in the same order or of the same frequency

ra_value ra_rmidx(r_array * ra, int idx);                   // remove item from ra at idx, returns value removed or (ra_value) (char *) NULL if error (prints warning)
ra_value ra_rmval(r_array * ra, ra_value element);          // remove first occurence of element in ra, returns same as ra_rmidx()
ra_value ra_pop(r_array * ra);                              // remove and return last element of ra
ra_value ra_cap(r_array * ra);                              // remove and return first element of ra

void ra_set(r_array * ra);                                  // remove all duplicates in ra, modifies ra (equivalent to set() in python)

void ra_repr(r_array * ra, char * label);                   // single-line print ra with an optional label (pass ra, NULL to print no label)
void ra_pprint(r_array * ra, char * label);                 // pretty print ra with an optional label (pass ra, NULL to print no label)

//-----------------------------------------------------------------------------

void ra_repr(r_array * ra, char * label);

r_array * ra_create(int size, ra_type type) {
    r_array * ra = (r_array *) malloc(sizeof(r_array));
    ra->size = size;
    ra->type = type;

    ra->count = 0;
    ra->elements = malloc(0);

    return ra;
}

void ra_destroy(r_array * ra) {
    if(ra->type == RA_STRING)
        for(int i = 0; i < ra->count; i++)
            free(ra->elements[i].s_value);

    free(ra);
}

void ra_append(r_array * ra, ra_value element) {
    if(ra->type == RA_STRING) {
        if(strlen(element.s_value) > ra->size) {
            eprintf("ra_append: String passed (length %d) exceeds maximum size of array elements (%d)\n%23cString passed: \"%s\"", strlen(element.s_value), ra->size, ' ', element.s_value);
            exit(10);
        }
    }

    ra->elements = realloc(ra->elements, (ra->count + 1) * sizeof(ra_value));
    // dprint("Reallocated memory...");
    if(ra->type == RA_STRING) {
        ra->elements[ra->count] = (ra_value) (char *) malloc(ra->size);
        strncpy(ra->elements[ra->count].s_value, element.s_value, ra->size);
    } else
        ra->elements[ra->count] = element;

    ra->count++;
}

void ra_concat(r_array * ra, r_array * rb) {
    todo("ra_concat");
}

void ra_insert(r_array * ra, int idx, ra_value element) {
    todo("ra_insert");
}

void ra_inject(r_array * ra, int idx, r_array rb) {
    todo("ra_inject");
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

ra_value ra_cap(r_array * ra) {
    return ra_rmidx(ra, 0);
}

void ra_set(r_array * ra) {
    todo("ra_set");
}

void ra_repr(r_array * ra, char * label) {
    if(!(label == NULL))
        printf("\"%s\" ", label);

    if(ra->count == 0) {
        printf("[0]> EMPTY\n");
        return;
    }

    printf("[%d]> ", ra->count);

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