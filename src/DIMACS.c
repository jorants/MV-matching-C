#include "DIMACS.h"


#define MAX_LINE_LENGTH 512

void _skip_whitespace ( FILE *fp, int *linenr )
{
    char c;
    do {
        c = getc ( fp );
        if ( c == '\n' ) {
            ( *linenr ) ++;
        }
    } while(isspace ( c ) && c != EOF);
    
    if ( c != EOF ) {
        ungetc ( c, fp );
    }
}

void _skip_line ( FILE *fp )
{
    char c;
    do {
        c = getc ( fp );
    } while(c != '\n' && c != '\r' );
}

void _read_line ( FILE *fp, char *line, int max_length, bool *success )
{
    char c, *ptr = line, *max_ptr = line + max_length - 1;
    do {
        c = getc ( fp );
        *ptr = c;
        if ( ptr++ > max_ptr ) {
            *success = 0;
            return;
        }
    } while(c != '\n' && c != '\r' && c != EOF);

    *ptr = 0;
    *success = 1;
}

void *_DIMACS_read_graph
(
    FILE *fp,
    void * ( *init ) ( uint, uint ),
    void ( *add_edge ) ( void*, uint, uint ),
    bool *success,
    char *failure_message // Should have length >= 1024
)
{
    char line[MAX_LINE_LENGTH];
    int linenr = 0;

    uint size, numedge;


    while ( 1 ) {
        _skip_whitespace ( fp, &linenr );
        if (feof(fp)) {
            *success = false;
            sprintf ( failure_message, "Unexpected EOF in line %d.", linenr );
        }
        char c = fgetc ( fp );
        if ( c == 'c' || c == 'C' ) {
            _skip_line ( fp );
            linenr++;
        } else if ( c == 'p' || c == 'P' ) {
            _read_line ( fp, line, MAX_LINE_LENGTH, success );
            linenr++;
            if ( !*success || sscanf ( line, "%*[ ]edge%*[ ]%d%*[ ]%d%*[ ]", &size, &numedge ) != 2 ) {
                *success = false;
                sprintf ( failure_message, "Bad graph file format, expectd 3 numbers after 'e' in line %d.", linenr );
                return 0;
            }
            break;
        } else {
            printf ( "Bad graph file format in line %d, expected 'p', found '%c'.", linenr, c );
            return 0;
        }
    }

    void *data = init ( size, numedge );

    uint i, j, tmp;

    while ( 1 ) {
        _skip_whitespace ( fp, &linenr );
        if (feof(fp)) {
            *success = false;
            sprintf ( failure_message, "Unexpected EOF in line %d.", linenr );
        }
        char c = fgetc ( fp );
        if ( c == 'c' || c == 'C' ) {
            _skip_line ( fp );
            linenr++;
        } else if ( c == 'e' || c == 'E' ) {
            _read_line ( fp, line, MAX_LINE_LENGTH, success );
            linenr++;
            if ( !*success || sscanf ( line, "%*[ ]%i%*[ ]%i%*[ ]%i%*[ ]", &i, &j, &tmp ) != 3 ) {
                sprintf ( failure_message, "Bad graph file format, expectd 3 numbers after 'e' in line %d.", linenr );
                *success = false;
                return data;
            }
            add_edge ( data, i, j );
        } else {
            *success = false;
            sprintf ( failure_message, "Bad graph file format in line %d, expected 'e', found '%c'.", linenr, c );
            return data;
        }
    }
    *success = true;
    return data;
}
