/* $Id: a2r.c,v 1.13 2010/11/26 15:07:27 ak Exp $ */
/*
**                                                          
**                    ###############                       
**                   #:::::::::::::::##                     
**                   #::::::######:::::#                    
**                   #######     #:::::#                    
**   #############               #:::::######   #########   
**   #::::::::::::#              #:::::##::::###:::::::::#  
**   #########:::::#          ####::::# #:::::::::::::::::# 
**            #::::#     #####::::::##  ##::::::#####::::::#
**     #######:::::#   ##::::::::###     #:::::#     #:::::#
**   ##::::::::::::#  #:::::#####        #:::::#     #######
**  #::::####::::::# #:::::#             #:::::#            
** #::::#    #:::::# #:::::#             #:::::#            
** #::::#    #:::::# #:::::#       #######:::::#            
** #:::::####::::::# #::::::#######:::::##:::::#            
**  #::::::::::##:::##::::::::::::::::::##:::::#            
**   ##########  ###############################            
** 
** Copyright (c) 1997,2001,2002,2009-2010, azumakuniyuki All rights reserved.
** 
** Redistribution and use in source and binary forms, with or without modification,
** are permitted provided that the following conditions are met:
** 
** Redistributions of source code must retain the above copyright notice, this list
** of conditions and the following disclaimer.
** 
** Redistributions in binary form  must reproduce the above copyright notice,  this
** list of conditions and the following disclaimer in the documentation and/or oth-
** er materials provided with the distribution.
** 
** Neither the name of the ``azumakuniyuki''  nor the names of its contributors may
** be used to endorse or promote products  derived from this software  without spe-
** cific prior written permission.
** 
** THIS SOFTWARE IS PROVIDED  BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT LIMITED TO,  THE IMPLIED
** WARRANTIES OF  MERCHANTABILITY  AND FITNESS  FOR A  PARTICULAR PURPOSE  ARE DIS-
** CLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR  CONTRIBUTORS  BE LIABLE FOR
** ANY DIRECT, INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY,  OR CONSEQUENTIAL DAMAGES
** ( INCLUDING,  BUT NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE GOODS  OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS;  OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON
** ANY THEORY OF LIABILITY,  WHETHER IN CONTRACT,  STRICT LIABILITY,  OR  TORT (IN-
** CLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFT-
** WARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <math.h>

#define A2R_OPTIONS "AChMv"
#define A2R_MAXNUMBER   4999
#define MAX_DIGITS  16

#define MODE_A2R    1
#define MODE_R2A    2
#define MODE_CLOCKFACE  4
#define MODE_MIDDLEAGE  8
#define MODE_ABBR   16

static const char *MY_PROGNAME[] = {"a2r"};     /* My name */
/* static const char *MY_DESCRIPTION[] = {"Convert from The Arabic numerals to The Roman numerals"}; */
static const char *MY_VERSION[] = {"1.0.6"};    /* Version number */
static const char *DEFINED_ROMANS[] = {
    "","I","II","III","IV","V","VI","VII","VIII","IX",
    "","X","XX","XXX","XL","L","LX","LXX","LXXX","XC",
    "","C","CC","CCC","CD","D","DC","DCC","DCCC","CM",
    "","M","MM","MMM","MMMM" };

static int OPERATION = MODE_A2R;

int a2r( const int, char * );
int r2a( const char *, int * );
void putHelp(void);
void putVersion(void);

int main( int argc, char *argv[] ) {

    int opt_result;                         /* The result value of the getopt */
    int statuscode = 0;                     /* return(); */
    int arabic_numeral = 0;                 /* The Arabic numerals */
    int num_of_args = argc - 1;             /* The Number Of Arguments */
    unsigned char given_numberlength = 0;   /* The Length of Argument. */
    unsigned char autodetect_count = 0;     /* The Counter for the Auto Detection */
    char roman_numeral[MAX_DIGITS] = "";    /* The Roman numerals */
    char source_value[MAX_DIGITS] = "";     /* Input value. */

    while(1) {
        opt_result = getopt( argc, argv, A2R_OPTIONS );

        if( opt_result == -1 ) {
            break;
        }

        switch( opt_result ) {

            case 'A': /* Medieval abbreviation */
                OPERATION |= MODE_ABBR;
                num_of_args--;
                break;

            case 'C': /* Clock face (IV is IIII) */
                OPERATION |= MODE_CLOCKFACE;
                num_of_args--;
                break;

            case 'h': /* Help! */
                putHelp();
                exit(EXIT_SUCCESS);
                /* break; */

            case 'M': /* The Roman numerals in the Middle Ages. */
                OPERATION |= MODE_MIDDLEAGE;
                num_of_args--;
                break;

            case 'v': /* Version number */
                putVersion();
                exit(EXIT_SUCCESS);
                /* break; */

            case '?':
            default:
                fprintf( stderr, "try -h option\n" );
                break;

        } /* end of switch */

    } /* end of while */


    if( num_of_args < 1 ) {
        /* Read From STDIN, If There is no String in Arguments. */
        char __number_in_stdin;
        short int __the_counter = 0;

        while( ( __number_in_stdin = getchar() ) != EOF ) {
            /* Check That The Character is a number. */
            if( ! isalnum(__number_in_stdin) ) {
                break;
            }

            if( __the_counter < ( MAX_DIGITS - 1 ) ) {
                source_value[__the_counter] = __number_in_stdin;
                __the_counter++;

            } else {
                break;
            }
        }

        source_value[__the_counter] = '\0';
        given_numberlength = strlen(source_value);

    } else {
        /* Read a source value from an argument. */
        given_numberlength = strlen( argv[ argc - 1 ] );
        (void) strlcpy( source_value, argv[ argc - 1 ], sizeof( source_value ) );
    }


    for( autodetect_count = 0; autodetect_count < given_numberlength; autodetect_count++ ) {
        /* Auto Detect The Operation Mode. */
        if( ! isnumber(source_value[autodetect_count]) ) {
            /* If It inlucdes Any Alphabet, Deal As a Roman Numerals. */
            OPERATION |= MODE_R2A;
            break;
        }
    }


    if( OPERATION & MODE_R2A ) {
        /* A source value is the Roman numerals. */
        (void) strlcpy( roman_numeral, source_value, sizeof( roman_numeral ) );

        if( r2a( roman_numeral, &arabic_numeral ) ) {
            printf( "%d\n", arabic_numeral );
        }

    } else if( OPERATION & MODE_A2R ) {
        /* A source value is the Arabic numerals. */
        arabic_numeral = atoi( source_value );

        if( a2r( arabic_numeral, roman_numeral ) ) {
            printf( "%s\n", roman_numeral );
        }
    }
    
    return( statuscode );

} /* end of main */

int a2r(const int an, char *rn) {
    char temporary_romans[MAX_DIGITS];
    int arabic_maxdigit = floor( log(A2R_MAXNUMBER) / log(10) ) + 1;
    int arabic_column[arabic_maxdigit]; /* Each column of the Arabic numerals. */
    int arabic_count = 0;               /* The loop counter for the above. */
    int arabic_index = 0;               /* The index number for the above. */
    int arabic_sum_of_modulus = 0;      /* Amari */
    int roman_count = 0;                /* The Loop counter for the roman numerals. */
    int roman_length = 0;               /* String length of the Roman numerals. */

    if( an > A2R_MAXNUMBER ) {
        fprintf( stderr, "Too large value %d (Max is %d)\n", an, A2R_MAXNUMBER );
        return(0);

    } else if( an < 0 ) {
        fprintf( stderr, "Too Small value %d (Min is 1)\n", an );
        return(0);

    } else if( an == 0 ) {

        if( OPERATION & MODE_MIDDLEAGE ) {
            /* 
             *  About 725, Bede or one of his colleagues used the letter N, 
             *  the initial of nullae, in a table of epacts, all written in Roman numerals.
             */
            (void) strlcpy( rn, "N", sizeof( rn ) );

        } else {
            exit(EXIT_SUCCESS);
        }

    } else {
        /* Convert from the Arabic numeral to the Roman numeral */
        for( arabic_count = arabic_maxdigit; arabic_count > 0; --arabic_count ) {
            /* Pick each column up in the arabic numeral */
            arabic_index = arabic_maxdigit - arabic_count;
            arabic_column[ arabic_index ] = an % (int) pow( 10, arabic_index + 1 ) - arabic_sum_of_modulus;
            arabic_sum_of_modulus += arabic_column[ arabic_index ];
            arabic_column[ arabic_index ] /= (int) pow( 10, arabic_index );

            if( arabic_sum_of_modulus == an ) break;
        }

        for( roman_count = arabic_index; roman_count >= 0; --roman_count ) {
            /*
            *  Select the roman numerals from the array
            *  Lookup and Insert
            */
            if( OPERATION & MODE_CLOCKFACE && roman_count == 0 && arabic_column[0] == 4 ) {
                /* 4 is IIII */
                (void) strlcat( rn, "IIII", strlen( rn ) + 5 );
                continue;

            } else if( OPERATION & MODE_MIDDLEAGE ) {
                /* The Middle Ages */
                if( roman_count == 0 && arabic_column[0] == 5 ) {
                    /* 5 is IIIII */
                    (void) strlcat( rn, "IIIII", strlen( rn ) + 6 );
                    continue;

                } else if( roman_count == 0 && arabic_column[0] == 8 ) {
                    /* 8 is IIX */
                    (void) strlcat( rn, "IIX", strlen( rn ) + 4 );
                    continue;
                } else if( roman_count == 1 && arabic_column[1] == 1 ) {
                    /* 10 is VV */
                    (void) strlcat( rn, "VV", strlen( rn ) + 3 );
                    continue;
                }

            } else if( OPERATION & MODE_ABBR ) {
                /* Medieval abbreviation */
                switch( roman_count ) {
                    case 0:
                        if( arabic_column[0] == 5 ) {
                            /* 5 is A, Resembles an upside-down V. Also said to equal 500. */
                            (void) strlcat( rn, "A", strlen( rn ) + 2 );
                            continue;
                        }
                        break;

                    case 1:
                        if( arabic_column[1] == 7 ) {
                            /* 70 is S, Also could stand for 7, and has same etymology. */
                            (void) strlcat( rn, "S", strlen( rn ) + 2 );
                            continue;

                        } else if( arabic_column[1] == 9 ) {
                            /* 90 is N, Presumed abbreviation of nonaginta, Latin for 90. */
                            (void) strlcat( rn, "N", strlen( rn ) + 2 );
                            continue;
                        }
                        break;

                    case 2:
                        if( arabic_column[2] == 5 )
                        {
                            /* 500 is Q, Redundant with D, abbreviation for quingenti, Latin for 500. */
                            (void) strlcat( rn, "Q", strlen( rn ) + 2 );
                            continue;

                        } else if( arabic_column[2] == 8 ) {
                            /* 800 is W, More properly, the Greek omega, as W was a fairly new creation. */
                            (void) strlcat( rn, "W", strlen( rn ) + 2 );
                            continue;
                        }
                        break;

                    case 3:
                        if( arabic_column[3] == 2 ) {
                            /* 2000 is Z */
                            (void) strlcat( rn, "Z", strlen( rn ) + 2 );
                            continue;
                        }
                        break;
                    case '?':
                    default:
                        break;
                } /* End of 'switch' */
            }

            roman_length = strlen( DEFINED_ROMANS[ roman_count * 10 + arabic_column[ roman_count ]] );

            strlcpy( temporary_romans, 
                DEFINED_ROMANS[roman_count * 10 + arabic_column[ roman_count ]] ,
                sizeof( temporary_romans ) );
            (void) strlcat( rn, temporary_romans, strlen( rn ) + roman_length + 1 );

        } /* End of the loop for() */
    }
    return(1);
}


int r2a(const char *rn, int *an) {
    char upper_temporary = 0;       /* Variable to convert to upper case temporary. */
    char upper_roman[MAX_DIGITS];   /* The Roman numerals which are upper case. */
    char roman_buffer[6];           /* Temporary buffer for converting to the arabic numerals. */

    int upper_count = 0;            /* Counter in the while(*rn) loop. */
    int arabic_count = 0;           /* Counter in the for loop. */
    int select_count = 0;           /* Counter in the for loop to select characters from the array "upper_roman" */
    int insert_count = 0;           /* Counter in the for loop to insert characters to the array "roman_buffer" */
    int roman_position = 0;         /* Hold an index number of the array(upper_roman) */
    int compare_count = 0;          /* Counter for strcmp in the for() loop. */
    int n_defined_romans = 35;      /* The number of DEFINED_ROMANS */

    bool found_the_roman_numerals = 0;  /* Search flag. */
    bool invalid_order_string = 0;      /* Invalid order as the Roman numerals */


    if( strlen( rn ) > 16 ) {
        fprintf( stderr, "Too many characters in argument (Max is 16)\n" );
        return(0);
    }

    /* Convert to Upper case */
    while( *rn ) {
        upper_temporary = (unsigned char) toupper( *rn );
        switch( upper_temporary ) {
            case 'I':
            case 'V':
            case 'X':
            case 'L':
            case 'C':
            case 'D':
            case 'M':
                /* Valid roman character */
                upper_roman[ upper_count ] = upper_temporary;
                ++upper_count;
                break;
            default:
                /* Nothing to do */
                break;

        } /* End of switch() */

        *rn++;

    }

    upper_roman[upper_count] = '\0';

    while( arabic_count < upper_count ) {

        for( select_count = 5; select_count > 0; --select_count ) {
            /*
             *  Select 5 characters from the array upper_roman and
             *  insert selected characters to the array roman_buffer.
             *  [M][M][M][D][C]CLXI
             */
            if( select_count > upper_count ) continue;

            insert_count = 0;
            for( insert_count = 0; insert_count < select_count; ++insert_count ) {

                if( upper_roman[ roman_position + insert_count ] ) {
                    roman_buffer[ insert_count ] = upper_roman[ roman_position + insert_count ];
                    if( insert_count + 1 == select_count ) {
                        roman_buffer[ insert_count + 1 ] = '\0';
                    }
                } else {
                    /* String length is less than the select_count */
                    roman_buffer[ insert_count ] = '\0';
                    break;
                }
            }

            /* Check the string as a format of the clock face */
            if( OPERATION & MODE_CLOCKFACE ) {
                if( select_count == 4 && strcmp( roman_buffer, "IIII" ) == 0 ) {
                    found_the_roman_numerals = 1;
                    compare_count = 4;
                }
            }

            /* Check the string as a format of the Middle Age */
            if( OPERATION & MODE_MIDDLEAGE ) {

                if( select_count == 5 && strcmp( roman_buffer, "IIIII" ) == 0 ) {
                    found_the_roman_numerals = 1;
                    compare_count = 5;

                } else if( select_count == 3 && strcmp( roman_buffer, "IIX" ) == 0 ) {
                    found_the_roman_numerals = 1;
                    compare_count = 8;

                } else if( select_count == 2 && strcmp( roman_buffer, "VV" ) == 0 ) {
                    found_the_roman_numerals = 1;
                    compare_count = 11;
                }
            }

            /* Check the string as a format of the Medieval abbreviation */
            if( OPERATION & MODE_ABBR ) {

                switch( select_count ) {
                    case 1:
                        if( strcmp( roman_buffer, "A" ) == 0 ) {
                            /* 'A' is 5 */
                            found_the_roman_numerals = 1;
                            compare_count = 5;

                        } else if( strcmp( roman_buffer, "S" ) == 0 ) {
                            /* 'S' is 70 */
                            found_the_roman_numerals = 1;
                            compare_count = 71;

                        } else if( strcmp( roman_buffer, "N" ) == 0 ) {
                            /* 'N' is 90 */
                            found_the_roman_numerals = 1;
                            compare_count = 91;

                        } else if( strcmp( roman_buffer, "Q" ) == 0 ) {
                            /* 'Q' is 500 */
                            found_the_roman_numerals = 1;
                            compare_count = 501;

                        } else if( strcmp( roman_buffer, "W" ) == 0 ) {
                            /* 'W' is 800 */
                            found_the_roman_numerals = 1;
                            compare_count = 801;

                        } else if( strcmp( roman_buffer, "Z" ) == 0 ) {
                            /* 'Z' is 2000 */
                            found_the_roman_numerals = 1;
                            compare_count = 2001;
                        }
                        break;

                    case '?':
                    default:
                        break;
                }
            }

            if( ! found_the_roman_numerals ) {
                /* Compare */
                for( compare_count = 1; compare_count < n_defined_romans; ++compare_count ) {

                    if( ( compare_count % 10 ) == 0 ) {
                        /* Skip if the index number is 10,20,30 */
                        continue;

                    } else {
                        if( roman_buffer && DEFINED_ROMANS[ compare_count ] 
                            && strcmp( roman_buffer, DEFINED_ROMANS[ compare_count ]) == 0 ){

                            found_the_roman_numerals = 1;
                            break;

                        } else if( select_count == 1 && ( compare_count + 1 ) == n_defined_romans ) {
                            /* Invalid string order as the Roman numerals */
                            invalid_order_string = 1;
                            break;
                        }
                    }
                }
            }

            if( invalid_order_string ) {
                roman_position += select_count;
                arabic_count += select_count;
                invalid_order_string = 0;
                break;

            } else if( found_the_roman_numerals ) {
                 /* Add the value to the *an, ex) 34 = 10^3 * 4 */
                int digit_01 = compare_count % 10;
                int digit_10 = ( compare_count - digit_01 ) / 10;
                *an += ( pow( 10, digit_10 ) * digit_01 );

                roman_position += select_count;
                arabic_count += select_count;
                n_defined_romans = compare_count - digit_01;
                found_the_roman_numerals = 0;
                break;
            }

        } /* End of the loop for() (select|insert) */

        if( n_defined_romans < 1 ) {
            /* Invalid order string as the Roman numerals */
            break;
        }

    } /* End of the loop while() */

    return(*an);
}

void putHelp(void) {
    printf( "Usage: %s [Options] (The Arabic numerals|The Roman numerals)\n", *MY_PROGNAME );
    puts( "Options:" );
    puts( "  -A    : Medieval abbreviation." );
    puts( "  -C    : Clock face, 4 is IIII." );
    puts( "  -h    : Help message(This screen)." );
    puts( "  -M    : The Roman numerals used in the Middle Ages." );
    puts( "  -v    : Show version number." );
    puts( "");
    puts( " See http://en.wikipedia.org/wiki/Roman_numerals" );
}

void putVersion(void) {
    printf( "%s\n",*MY_VERSION );
}

