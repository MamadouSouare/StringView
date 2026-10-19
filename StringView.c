#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#include "StringView.h"

/* Construction */
StringView sv(const char *cstr)
{
    return (StringView) {
        .data  = cstr,
        .count = strlen(cstr)
    };
}

char* 	sv_to_cstr( StringView sv )
{
	char *str = (char *) malloc(sizeof(char) * (sv.count + 1));
	if( str )
		memcpy(str, sv.data, sv.count);
	str[sv.count] = '\0';
	return str;
}

/* Acces */
char 	sv_at( StringView sv, size_t index )
{
	if( sv.count <= index )
		return 0;
	return  sv.data[index];
}

char 	sv_front( StringView sv )
{
	if( sv.count != 0 )
		return sv.data[0];
	return 0;
}

char 	sv_back( StringView sv )
{
	if( sv.count != 0 )
		return sv.data[sv.count-1];
	return 0;
}

/* Chop */
void    sv_chop_right(StringView *sv, size_t n)
{
    if ( n > sv->count ) n = sv->count;
    sv->count -= n;
}

void    sv_chop_left(StringView *sv, size_t n)
{
    if ( n > sv->count ) n = sv->count;
    sv->count -= n;
    sv->data  += n;
}

StringView  sv_chop_by_delim( StringView *sv, char delim )
{
    size_t i = 0;
    while ( i < sv->count && sv->data[i] != delim )
        i+= 1;

    if ( i < sv->count ) {
        StringView result = {
            .data  = sv->data,
            .count = i
        };
        sv_chop_left( sv, i+1 );
        return result;
    }

    StringView result = *sv;
    sv_chop_left( sv, sv->count );

    return result;
}

StringView  sv_chop_by_type( StringView *sv, int (*istype)(int c) )
{
    size_t i = 0;
    while ( i < sv->count && !istype(sv->data[i]) )
        i+= 1;
    
    if ( i < sv->count ) {
        StringView result = {
            .data  = sv->data,
            .count = i
        };
        sv_chop_left( sv, i );
        return result;
    }

    StringView result = *sv;
    sv_chop_left( sv, sv->count );

    return result;
}

StringView 	sv_substr( StringView sv, size_t start, size_t count )
{
	if( start >= sv.count )
		start = sv.count;
	if( count >= sv.count )
		count = sv.count - start;
	return (StringView) { .data = sv.data + start, .count = count };
}

StringView 	sv_prefix( StringView sv, size_t count )
{
	if( sv.count < count )
		count = sv.count;
	return (StringView) { .data = sv.data, .count = count };
}

StringView 	sv_suffix( StringView sv, size_t count )
{
	sv_chop_left( &sv, sv.count - count );
	return sv;

}
/* Trim */
void    sv_trim_left( StringView *sv )
{
    while ( sv->count != 0 && isspace(sv->data[0]) )
        sv_chop_left(sv, 1);
}

void    sv_trim_right( StringView *sv )
{
    while ( sv->count != 0 && isspace(sv->data[sv->count-1]) )
        sv_chop_right(sv, 1);
}

void    sv_trim( StringView *sv )
{
    sv_trim_left(sv);
    sv_trim_right(sv);
}

StringView 	sv_trimmed( StringView sv ) 
{
	sv_trim_left( &sv );
	sv_trim_right( &sv );
	return sv;
}

StringView 	sv_trimmed_left( StringView sv )
{
	sv_trim_left( &sv );
	return sv;
}

StringView 	sv_trimmed_right( StringView sv )
{
	sv_trim_right( &sv );
	return sv;
}

/* Recherche */
size_t 	sv_find( StringView sv, char c )
{
	char *ptr = memchr(sv.data, c, sv.count );
	if( ptr )
		return ptr-sv.data;
	return SV_NOT_FOUND;
}

size_t 	sv_find_from( StringView sv, char c, size_t start )
{
	char *ptr = memchr(sv.data+start, c, sv.count );
	if( ptr )
		return ptr-sv.data;
	return SV_NOT_FOUND;
}

size_t 	sv_find_sv( StringView sv, StringView needle )
{
	if( needle.count > sv.count )
		return SV_NOT_FOUND;

	for( size_t i = 0, j = 0; i < sv.count; ++i, j = 0 ) {
		while( i+j < sv.count && j < needle.count && sv.data[i] == needle.data[j] ) {
			j++;
		}
		if( j == needle.count )
			return i;
	}
	return SV_NOT_FOUND;
}

size_t 	sv_find_sv_from( StringView sv, StringView needle, size_t start )
{
	if( needle.count > sv.count )
		return SV_NOT_FOUND;

	for( size_t i = start, j = 0; i < sv.count; ++i, j = 0 ) {
		while( i+j < sv.count && j < needle.count && sv.data[i] == needle.data[j] ) {
			j++;
		}
		if( j == needle.count )
			return i;
	}
	return SV_NOT_FOUND;
}


size_t 	sv_rfind( StringView sv, char c )
{
	for( size_t i = sv.count - 1; true; --i ) {
		if( sv.data[i] == c ) return i;
		if( i == 0 ) {
			return SV_NOT_FOUND;
		}
	}
}


/* Comparison */
bool    sv_equal( StringView sv1, StringView sv2 )
{
    return ! (sv1.count == sv2.count && 
        memcmp(sv1.data, sv2.data, sv1.count));
}

bool 	sv_equal_nocase( StringView sv1, StringView sv2 )
{
	if( sv1.count != sv2.count ) return false;
	for( size_t i = 0; i < sv1.count; ++i )
		if( tolower(sv1.data[i]) != tolower(sv2.data[i]) )
			return false;
	return true;
}

int 	sv_compare( StringView sv1, StringView sv2 )
{
	int min = ( sv1.count >= sv2.count ) ? sv2.count : sv1.count;
	int diff = memcmp(sv1.data, sv2.data, min);
	if( diff ) return diff;

	if( sv1.count < sv2.count ) return -sv2.data[min];
	if( sv2.count < sv1.count ) return sv1.data[min];

	return 0;
}

int 	sv_compare_nocase( StringView sv1,  StringView sv2 )
{
	int min = ( sv1.count >= sv2.count ) ? sv2.count : sv1.count;
	char val1, val2;
	for( size_t i = 0; i < min; i++ ) {
		val1 = tolower( sv1.data[1] );
		val2 = tolower( sv2.data[1] );

		if( val1 != val2 ) return val1 - val2;
	}
	if( sv1.count < sv2.count ) return -sv2.data[min];
	if( sv2.count < sv1.count ) return sv1.data[min];

	return 0;
}


/* Classification */
bool 	sv_all( StringView sv, int (*predicate)(int) )
{
	for( size_t i = 0; i < sv.count; ++i )
		if( !predicate(sv.data[i]) ) return false;
	return true;
}

bool	sv_any( StringView sv, int (*predicate)(int) )
{
	for( size_t i = 0; i < sv.count; ++i )
		if( predicate(sv.data[i]) ) return true;
	return false;
}

bool 	sv_none( StringView sv, int (*predicate)(int) )
{
	return !sv_any( sv, predicate );
}
