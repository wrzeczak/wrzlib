//------------------------------------------------------------------------------//
// MESSAGE - Macros for sending debug, error, warning, and todo messages simply //
// and with nice formatting. Written for and by wrzeczak 7/6/2025.              //
//------------------------------------------------------------------------------//


#pragma once

#define todo(l) printf("\nWRZ: TODO: function " l "() is not yet implemented!\n\n"); \
				exit(-2);
				
#ifdef DEBUG
#define dprint(s) printf("WRZ: DEBUG: " s "\n")
#define dprintf(s, ...) printf("WRZ: DEBUG: " s "\n", __VA_ARGS__)
#else
#define dprint(s)
#define dprintf(s, ...)
#endif

#define eprint(s) printf("\nWRZ: ERROR: " s "\n\n")
#define eprintf(s, ...) printf("\nWRZ: ERROR: " s "\n\n", __VA_ARGS__)

#define wprint(s) printf("WRZ: WARN: " s "\n")
#define wprintf(s, ...) printf("WRZ: WARN: " s "\n", __VA_ARGS__)
