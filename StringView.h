#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define sv_fmt "%.*s"
#define sv_arg(sv) (sv).count, (sv).data

/*
    To print a string view: 
    StringView sv = sv("Hello World!");
    printf("sv = "sv_fmt"\n", sv_arg(sv));
*/

typedef struct {
    const char *data;
    size_t count;
}   StringView;

/* Construction */
StringView sv(const char *cstr);
char* 	sv_to_cstr( StringView sv );

/* Acces */
char 	sv_at( StringView sv, size_t index );
char 	sv_front( StringView sv );
char 	sv_back( StringView sv );

/* Chop */
void    	sv_chop_right(StringView *sv, size_t n);
void    	sv_chop_left(StringView *sv, size_t n);
StringView  	sv_chop_by_delim( StringView *sv, char delim );
StringView  	sv_chop_by_type( StringView *sv, int (*istype)(int c) );

StringView 	sv_substr( StringView sv, size_t start, size_t count );
StringView 	sv_prefix( StringView sv, size_t count );
StringView 	sv_suffix( StringView sv, size_t count );

/* Trim */
void    sv_trim_left( StringView *sv );
void    sv_trim_right( StringView *sv );
void    sv_trim( StringView *sv );

StringView 	sv_trimmed( StringView sv );
StringView 	sv_trimmed_left( StringView sv );
StringView 	sv_trimmed_right( StringView sv );

/* Recherche */
size_t 	sv_find( StringView sv, char c );
size_t 	sv_find_from( StringView sv, char c, size_t start );

size_t 	sv_find_sv( StringView sv, StringView needle ); // not implemented yet
size_t 	sv_find_sv_from( StringView sv, StringView needle, size_t start );// not implemented yet

size_t 	sv_rfind( StringView sv, char c );
size_t 	sv_rfind_sv( StringView sv, StringView needle );// not implemented yet

/* Comparison */
bool    sv_equal( StringView sv1, StringView sv2 );
bool 	sv_equal_nocase( StringView sv1, StringView sv2 );
int 	sv_compare( StringView sv1, StringView sv2 );
int 	sv_compare_nocase( StringView sv1,  StringView sv2 );

/* Classification */
bool 	sv_all( StringView sv, int (*predicate)(int) );
bool	sv_any( StringView sv, int (*predicate)(int) );
bool 	sv_none( StringView sv, int (*predicate)(int) );
