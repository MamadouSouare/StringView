#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "StringView.h"

/* Construction */
inline StringView sv(const char *cstr)
{
	return (StringView) {
		.data  = cstr,
		.count = strlen(cstr)
	};
}

inline char* 	sv_to_cstr( StringView sv )
{
	char *str = (char *) malloc(sizeof(char) * (sv.count + 1));
	if( str )
		memcpy(str, sv.data, sv.count);
	str[sv.count] = '\0';
	return str;
}

/* Acces */
inline char 	sv_at( StringView sv, size_t index )
{
	if( sv.count <= index )
		return 0;
	return  sv.data[index];
}

inline char 	sv_front( StringView sv )
{
	if( sv.count != 0 )
		return sv.data[0];
	return 0;
}

inline char 	sv_back( StringView sv )
{
	if( sv.count != 0 )
		return sv.data[sv.count-1];
	return 0;
}

/* Chop */
inline void    sv_chop_right(StringView *sv, size_t n)
{
	if ( n > sv->count ) n = sv->count;
	sv->count -= n;
}

inline void    sv_chop_left(StringView *sv, size_t n)
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

	StringView result = {
		.data  = sv->data,
		.count = i
	};
	sv_chop_left( sv, i < sv->count ? i + 1 : i );
	return result;
}

StringView  sv_chop_by_type( StringView *sv, int (*istype)(int c) )
{
	size_t i = 0;
	while ( i < sv->count && !istype(sv->data[i]) )
		i+= 1;

	StringView result = {
		.data  = sv->data,
		.count = i
	};
	sv_chop_left( sv, i );
	return result;
}

inline StringView 	sv_substr( StringView sv, size_t start, size_t count )
{
	if( start >= sv.count )
		start = sv.count;
	if( count >= sv.count )
		count = sv.count - start;
	return (StringView) { .data = sv.data + start, .count = count };
}

inline StringView 	sv_prefix( StringView sv, size_t count )
{
	if( sv.count < count )
		count = sv.count;
	return (StringView) { .data = sv.data, .count = count };
}

inline StringView 	sv_suffix( StringView sv, size_t count )
{
	sv_chop_left( &sv, sv.count - count );
	return sv;
}
/* Trim */
inline void    sv_trim_left( StringView *sv )
{
	while ( sv->count != 0 && isspace(sv->data[0]) )
		sv_chop_left(sv, 1);
}

inline void    sv_trim_right( StringView *sv )
{
	while ( sv->count != 0 && isspace(sv->data[sv->count-1]) )
		sv_chop_right(sv, 1);
}

inline void    sv_trim( StringView *sv )
{
	sv_trim_left(sv);
	sv_trim_right(sv);
}

inline StringView 	sv_trimmed_left( StringView sv )
{
	sv_trim_left( &sv );
	return sv;
}

inline StringView 	sv_trimmed_right( StringView sv )
{
	sv_trim_right( &sv );
	return sv;
}

inline StringView 	sv_trimmed( StringView sv ) 
{
	sv_trim_left( &sv );
	sv_trim_right( &sv );
	return sv;
}
/* Recherche */
inline size_t 	sv_find( StringView sv, char c )
{
	char *ptr = memchr( (void *)sv.data, c, sv.count );
	if( ptr )
		return ptr-sv.data;
	return SV_NOT_FOUND;
}

inline size_t 	sv_find_from( StringView sv, char c, size_t start )
{
	char *ptr = memchr((void*)sv.data+start, c, sv.count );
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


inline size_t 	sv_rfind( StringView sv, char c )
{
	for( size_t i = sv.count - 1; true; --i ) {
		if( sv.data[i] == c ) return i;
		if( i == 0 ) {
			return SV_NOT_FOUND;
		}
	}
}


/* Comparison */
inline bool    sv_equal( StringView sv1, StringView sv2 )
{
	return ! (sv1.count == sv2.count && 
		memcmp(sv1.data, sv2.data, sv1.count));
}

inline bool 	sv_equal_nocase( StringView sv1, StringView sv2 )
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
inline bool 	sv_all( StringView sv, int (*predicate)(int) )
{
	for( size_t i = 0; i < sv.count; ++i )
		if( !predicate(sv.data[i]) ) return false;
	return true;
}

inline bool	sv_any( StringView sv, int (*predicate)(int) )
{
	for( size_t i = 0; i < sv.count; ++i )
		if( predicate(sv.data[i]) ) return true;
	return false;
}

inline bool 	sv_none( StringView sv, int (*predicate)(int) )
{
	return !sv_any( sv, predicate );
}

/* Parsing numérique */
bool 	sv_to_int( StringView sv, int *out )
{
	int sign = 1;
	size_t i = 0;
	*out = 0;

	StringView res = sv_chop_by_type( &sv, isalpha );
	if( sv.count != 0 ) return false;
	sv_trim( &res );
	if( i < res.count && res.data[i] == '-' ) {
		sign = -1;
		res.data++;
	} else if( i < res.count && res.data[i] == '+' ) {
		res.data++;
	}
	sv_trim_left( &res );
	
	while( i < res.count && isdigit(res.data[i]) ) {
		int next = *out *10 + (res.data[i] - '0')*sign;
		if( (*out > 0 && next > 0) || *out == 0 || (*out < 0 && next < 0))
			*out = next;
		else {
			*out = 0;
			return false;
		}
		i++;
	}

	if( i == 0 ) return false;
	return true;
}

bool 	sv_to_uint( StringView sv, unsigned int *out )
{
	size_t i = 0;
	*out = 0;

	StringView res = sv_chop_by_type( &sv, isalpha );
	if( sv.count != 0 ) return false;
	
	sv_trim( &res );
	if( i < res.count && res.data[i] == '-' ) {
		return false;
	} else if( i < res.count && res.data[i] == '+' ) {
		res.data++;
	}
	sv_trim_left( &res );
	
	while( i < res.count && isdigit(res.data[i]) ) {
		unsigned int next = *out *10 + (res.data[i] - '0');
		if( next < *out ) {
			*out = 0;
			return false;
		}
		*out = next;

		i++;
	}

	if( i == 0 ) return false;
	return true;
}
