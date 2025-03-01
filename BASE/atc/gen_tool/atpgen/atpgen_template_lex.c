/* A lexical scanner generated by flex */

/* Scanner skeleton version:
 * $Header: /home/daffy/u0/vern/flex/RCS/flex.skl,v 2.91 96/09/10 16:58:48 vern Exp $
 */

#define FLEX_SCANNER
#define TPL_FLEX_MAJOR_VERSION 2
#define TPL_FLEX_MINOR_VERSION 5

#include <stdio.h>


/* cfront 1.2 defines "c_plusplus" instead of "__cplusplus" */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif


#ifdef __cplusplus

#include <stdlib.h>
#include <unistd.h>

/* Use prototypes in function declarations. */
#define TPL_USE_PROTOS

/* The "const" storage-class-modifier is valid. */
#define TPL_USE_CONST

#else	/* ! __cplusplus */

#if __STDC__

#define TPL_USE_PROTOS
#define TPL_USE_CONST

#endif	/* __STDC__ */
#endif	/* ! __cplusplus */

#ifdef __TURBOC__
 #pragma warn -rch
 #pragma warn -use
#include <io.h>
#include <stdlib.h>
#define TPL_USE_CONST
#define TPL_USE_PROTOS
#endif

#ifdef TPL_USE_CONST
#define tplconst const
#else
#define tplconst
#endif


#ifdef TPL_USE_PROTOS
#define TPL_PROTO(proto) proto
#else
#define TPL_PROTO(proto) ()
#endif

/* Returned upon end-of-file. */
#define TPL_NULL 0

/* Promotes a possibly negative, possibly signed char to an unsigned
 * integer for use as an array index.  If the signed char is negative,
 * we want to instead treat it as an 8-bit unsigned char, hence the
 * double cast.
 */
#define TPL_SC_TO_UI(c) ((unsigned int) (unsigned char) c)

/* Enter a start condition.  This macro really ought to take a parameter,
 * but we do it the disgusting crufty way forced on us by the ()-less
 * definition of BEGIN.
 */
#define BEGIN tpl_start = 1 + 2 *

/* Translate the current start state into a value that can be later handed
 * to BEGIN to return to the state.  The TPLSTATE alias is for lex
 * compatibility.
 */
#define TPL_START ((tpl_start - 1) / 2)
#define TPLSTATE TPL_START

/* Action number for EOF rule of a given start state. */
#define TPL_STATE_EOF(state) (TPL_END_OF_BUFFER + state + 1)

/* Special action meaning "start processing a new file". */
#define TPL_NEW_FILE tplrestart( tplin )

#define TPL_END_OF_BUFFER_CHAR 0

/* Size of default input buffer. */
#define TPL_BUF_SIZE 16384

typedef struct tpl_buffer_state *TPL_BUFFER_STATE;

extern int tplleng;
extern FILE *tplin, *tplout;

#define EOB_ACT_CONTINUE_SCAN 0
#define EOB_ACT_END_OF_FILE 1
#define EOB_ACT_LAST_MATCH 2

/* The funky do-while in the following #define is used to turn the definition
 * int a single C statement (which needs a semi-colon terminator).  This
 * avoids problems with code like:
 *
 * 	if ( condition_holds )
 *		tplless( 5 );
 *	else
 *		do_something_else();
 *
 * Prior to using the do-while the compiler would get upset at the
 * "else" because it interpreted the "if" statement as being all
 * done when it reached the ';' after the tplless() call.
 */

/* Return all but the first 'n' matched characters back to the input stream. */

#define tplless(n) \
	do \
		{ \
		/* Undo effects of setting up tpltext. */ \
		*tpl_cp = tpl_hold_char; \
		TPL_RESTORE_TPL_MORE_OFFSET \
		tpl_c_buf_p = tpl_cp = tpl_bp + n - TPL_MORE_ADJ; \
		TPL_DO_BEFORE_ACTION; /* set up tpltext again */ \
		} \
	while ( 0 )

#define unput(c) tplunput( c, tpltext_ptr )

/* The following is because we cannot portably get our hands on size_t
 * (without autoconf's help, which isn't available because we want
 * flex-generated scanners to compile on their own).
 */
typedef unsigned int tpl_size_t;


struct tpl_buffer_state
	{
	FILE *tpl_input_file;

	char *tpl_ch_buf;		/* input buffer */
	char *tpl_buf_pos;		/* current position in input buffer */

	/* Size of input buffer in bytes, not including room for EOB
	 * characters.
	 */
	tpl_size_t tpl_buf_size;

	/* Number of characters read into tpl_ch_buf, not including EOB
	 * characters.
	 */
	int tpl_n_chars;

	/* Whether we "own" the buffer - i.e., we know we created it,
	 * and can realloc() it to grow it, and should free() it to
	 * delete it.
	 */
	int tpl_is_our_buffer;

	/* Whether this is an "interactive" input source; if so, and
	 * if we're using stdio for input, then we want to use getc()
	 * instead of fread(), to make sure we stop fetching input after
	 * each newline.
	 */
	int tpl_is_interactive;

	/* Whether we're considered to be at the beginning of a line.
	 * If so, '^' rules will be active on the next match, otherwise
	 * not.
	 */
	int tpl_at_bol;

	/* Whether to try to fill the input buffer when we reach the
	 * end of it.
	 */
	int tpl_fill_buffer;

	int tpl_buffer_status;
#define TPL_BUFFER_NEW 0
#define TPL_BUFFER_NORMAL 1
	/* When an EOF's been seen but there's still some text to process
	 * then we mark the buffer as TPL_EOF_PENDING, to indicate that we
	 * shouldn't try reading from the input source any more.  We might
	 * still have a bunch of tokens to match, though, because of
	 * possible backing-up.
	 *
	 * When we actually see the EOF, we change the status to "new"
	 * (via tplrestart()), so that the user can continue scanning by
	 * just pointing tplin at a new input file.
	 */
#define TPL_BUFFER_EOF_PENDING 2
	};

static TPL_BUFFER_STATE tpl_current_buffer = 0;

/* We provide macros for accessing buffer states in case in the
 * future we want to put the buffer states in a more general
 * "scanner state".
 */
#define TPL_CURRENT_BUFFER tpl_current_buffer


/* tpl_hold_char holds the character lost when tpltext is formed. */
static char tpl_hold_char;

static int tpl_n_chars;		/* number of characters read into tpl_ch_buf */


int tplleng;

/* Points to current character in buffer. */
static char *tpl_c_buf_p = (char *) 0;
static int tpl_init = 1;		/* whether we need to initialize */
static int tpl_start = 0;	/* start state number */

/* Flag which is used to allow tplwrap()'s to do buffer switches
 * instead of setting up a fresh tplin.  A bit of a hack ...
 */
static int tpl_did_buffer_switch_on_eof;

void tplrestart TPL_PROTO(( FILE *input_file ));

void tpl_switch_to_buffer TPL_PROTO(( TPL_BUFFER_STATE new_buffer ));
void tpl_load_buffer_state TPL_PROTO(( void ));
TPL_BUFFER_STATE tpl_create_buffer TPL_PROTO(( FILE *file, int size ));
void tpl_delete_buffer TPL_PROTO(( TPL_BUFFER_STATE b ));
void tpl_init_buffer TPL_PROTO(( TPL_BUFFER_STATE b, FILE *file ));
void tpl_flush_buffer TPL_PROTO(( TPL_BUFFER_STATE b ));
#define TPL_FLUSH_BUFFER tpl_flush_buffer( tpl_current_buffer )

TPL_BUFFER_STATE tpl_scan_buffer TPL_PROTO(( char *base, tpl_size_t size ));
TPL_BUFFER_STATE tpl_scan_string TPL_PROTO(( tplconst char *tpl_str ));
TPL_BUFFER_STATE tpl_scan_bytes TPL_PROTO(( tplconst char *bytes, int len ));

static void *tpl_flex_alloc TPL_PROTO(( tpl_size_t ));
static void *tpl_flex_realloc TPL_PROTO(( void *, tpl_size_t ));
static void tpl_flex_free TPL_PROTO(( void * ));

#define tpl_new_buffer tpl_create_buffer

#define tpl_set_interactive(is_interactive) \
	{ \
	if ( ! tpl_current_buffer ) \
		tpl_current_buffer = tpl_create_buffer( tplin, TPL_BUF_SIZE ); \
	tpl_current_buffer->tpl_is_interactive = is_interactive; \
	}

#define tpl_set_bol(at_bol) \
	{ \
	if ( ! tpl_current_buffer ) \
		tpl_current_buffer = tpl_create_buffer( tplin, TPL_BUF_SIZE ); \
	tpl_current_buffer->tpl_at_bol = at_bol; \
	}

#define TPL_AT_BOL() (tpl_current_buffer->tpl_at_bol)


#define tplwrap() 1
#define TPL_SKIP_TPLWRAP
typedef unsigned char TPL_CHAR;
FILE *tplin = (FILE *) 0, *tplout = (FILE *) 0;
typedef int tpl_state_type;
extern char *tpltext;
#define tpltext_ptr tpltext

static tpl_state_type tpl_get_previous_state TPL_PROTO(( void ));
static tpl_state_type tpl_try_NUL_trans TPL_PROTO(( tpl_state_type current_state ));
static int tpl_get_next_buffer TPL_PROTO(( void ));
static void tpl_fatal_error TPL_PROTO(( tplconst char msg[] ));

/* Done after the current pattern has been matched and before the
 * corresponding action - sets up tpltext.
 */
#define TPL_DO_BEFORE_ACTION \
	tpltext_ptr = tpl_bp; \
	tplleng = (int) (tpl_cp - tpl_bp); \
	tpl_hold_char = *tpl_cp; \
	*tpl_cp = '\0'; \
	tpl_c_buf_p = tpl_cp;

#define TPL_NUM_RULES 10
#define TPL_END_OF_BUFFER 11
static tplconst short int tpl_accept[55] =
    {   0,
        0,    0,    0,    0,   11,    9,    8,    9,    9,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    2,    0,    3,    0,    6,
        0,    5,    0,    0,    1,    0,    0,    0,    0,    0,
        7,    0,    4,    0
    } ;

static tplconst int tpl_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    3,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    4,    5,    6,    7,    8,    1,
        9,    1,   10,    1,    1,   11,   12,   13,   14,   15,
        1,   16,   17,   18,   19,    1,    1,   20,   21,    1,
        1,    1,    1,    1,    1,    1,    4,    5,    6,    7,

        8,    1,    9,    1,   10,    1,    1,   11,   12,   13,
       14,   15,    1,   16,   17,   18,   19,    1,    1,   20,
       21,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,

        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1
    } ;

static tplconst int tpl_meta[22] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1
    } ;

static tplconst short int tpl_base[56] =
    {   0,
        0,    2,    4,    6,   56,   57,   57,   52,   51,   50,
        7,   48,   33,   32,   43,   42,    2,    8,   34,   42,
       41,   23,   35,   29,   34,   28,   23,   23,   28,   29,
       27,   26,   29,   24,   21,   57,   10,   16,   16,   57,
       14,   57,   12,   12,   57,    9,    4,   11,    9,    0,
       57,    1,   57,   57,    0
    } ;

static tplconst short int tpl_def[56] =
    {   0,
       55,   55,   55,   55,   54,   54,   54,   54,   54,   54,
       54,   54,   54,   54,   54,   54,   54,   54,   54,   54,
       54,   54,   54,   54,   54,   54,   54,   54,   54,   54,
       54,   54,   54,   54,   54,   54,   54,   54,   54,   54,
       54,   54,   54,   54,   54,   54,   54,   54,   54,   54,
       54,   54,   54,    0,   54
    } ;

static tplconst short int tpl_nxt[79] =
    {   0,
        6,    7,    8,    7,    8,    7,    9,    7,    9,   12,
       13,   19,   23,   54,   20,   24,   21,   53,   52,   22,
       51,   50,   49,   48,   47,   46,   45,   44,   43,   42,
       41,   40,   39,   38,   37,   36,   35,   34,   33,   32,
       31,   30,   29,   28,   27,   26,   25,   18,   17,   16,
       15,   14,   12,   11,   10,   54,    5,   54,   54,   54,
       54,   54,   54,   54,   54,   54,   54,   54,   54,   54,
       54,   54,   54,   54,   54,   54,   54,   54
    } ;

static tplconst short int tpl_chk[79] =
    {   0,
       55,    1,    1,    2,    2,    3,    3,    4,    4,   11,
       11,   17,   18,    0,   17,   18,   17,   52,   50,   17,
       49,   48,   47,   46,   44,   43,   41,   39,   38,   37,
       35,   34,   33,   32,   31,   30,   29,   28,   27,   26,
       25,   24,   23,   22,   21,   20,   19,   16,   15,   14,
       13,   12,   10,    9,    8,    5,   54,   54,   54,   54,
       54,   54,   54,   54,   54,   54,   54,   54,   54,   54,
       54,   54,   54,   54,   54,   54,   54,   54
    } ;

static tpl_state_type tpl_last_accepting_state;
static char *tpl_last_accepting_cpos;

/* The intent behind this definition is that it'll catch
 * any uses of REJECT which flex missed.
 */
#define REJECT reject_used_but_not_detected
#define tplmore() tplmore_used_but_not_detected
#define TPL_MORE_ADJ 0
#define TPL_RESTORE_TPL_MORE_OFFSET
char *tpltext;
#define INITIAL 0
#include <stdio.h>
#include <stdlib.h>

#include "atpgen_from_template.h"

#define TPL 1


/* Macros after this point can all be overridden by user definitions in
 * section 1.
 */

#ifndef TPL_SKIP_TPLWRAP
#ifdef __cplusplus
extern "C" int tplwrap TPL_PROTO(( void ));
#else
extern int tplwrap TPL_PROTO(( void ));
#endif
#endif

#ifndef TPL_NO_UNPUT
static void tplunput TPL_PROTO(( int c, char *buf_ptr ));
#endif

#ifndef tpltext_ptr
static void tpl_flex_strncpy TPL_PROTO(( char *, tplconst char *, int ));
#endif

#ifdef TPL_NEED_STRLEN
static int tpl_flex_strlen TPL_PROTO(( tplconst char * ));
#endif

#ifndef TPL_NO_INPUT
#ifdef __cplusplus
static int tplinput TPL_PROTO(( void ));
#else
static int input TPL_PROTO(( void ));
#endif
#endif

#if TPL_STACK_USED
static int tpl_start_stack_ptr = 0;
static int tpl_start_stack_depth = 0;
static int *tpl_start_stack = 0;
#ifndef TPL_NO_PUSH_STATE
static void tpl_push_state TPL_PROTO(( int new_state ));
#endif
#ifndef TPL_NO_POP_STATE
static void tpl_pop_state TPL_PROTO(( void ));
#endif
#ifndef TPL_NO_TOP_STATE
static int tpl_top_state TPL_PROTO(( void ));
#endif

#else
#define TPL_NO_PUSH_STATE 1
#define TPL_NO_POP_STATE 1
#define TPL_NO_TOP_STATE 1
#endif

#ifdef TPL_MALLOC_DECL
TPL_MALLOC_DECL
#else
#if __STDC__
#ifndef __cplusplus
#include <stdlib.h>
#endif
#else
/* Just try to get by without declaring the routines.  This will fail
 * miserably on non-ANSI systems for which sizeof(size_t) != sizeof(int)
 * or sizeof(void*) != sizeof(int).
 */
#endif
#endif

/* Amount of stuff to slurp up with each read. */
#ifndef TPL_READ_BUF_SIZE
#define TPL_READ_BUF_SIZE 8192
#endif

/* Copy whatever the last rule matched to the standard output. */

#ifndef ECHO
/* This used to be an fputs(), but since the string might contain NUL's,
 * we now use fwrite().
 */
#define ECHO (void) fwrite( tpltext, tplleng, 1, tplout )
#endif

/* Gets input and stuffs it into "buf".  number of characters read, or TPL_NULL,
 * is returned in "result".
 */
#ifndef TPL_INPUT
#define TPL_INPUT(buf,result,max_size) \
	if ( tpl_current_buffer->tpl_is_interactive ) \
		{ \
		int c = '*', n; \
		for ( n = 0; n < max_size && \
			     (c = getc( tplin )) != EOF && c != '\n'; ++n ) \
			buf[n] = (char) c; \
		if ( c == '\n' ) \
			buf[n++] = (char) c; \
		if ( c == EOF && ferror( tplin ) ) \
			TPL_FATAL_ERROR( "input in flex scanner failed" ); \
		result = n; \
		} \
	else if ( ((result = fread( buf, 1, max_size, tplin )) == 0) \
		  && ferror( tplin ) ) \
		TPL_FATAL_ERROR( "input in flex scanner failed" );
#endif

/* No semi-colon after return; correct usage is to write "tplterminate();" -
 * we don't want an extra ';' after the "return" because that will cause
 * some compilers to complain about unreachable statements.
 */
#ifndef tplterminate
#define tplterminate() return TPL_NULL
#endif

/* Number of entries by which start-condition stack grows. */
#ifndef TPL_START_STACK_INCR
#define TPL_START_STACK_INCR 25
#endif

/* Report a fatal error. */
#ifndef TPL_FATAL_ERROR
#define TPL_FATAL_ERROR(msg) tpl_fatal_error( msg )
#endif

/* Default declaration of generated scanner - a define so the user can
 * easily add parameters.
 */
#ifndef TPL_DECL
#define TPL_DECL int tpllex TPL_PROTO(( void ))
#endif

/* Code executed at the beginning of each rule, after tpltext and tplleng
 * have been set up.
 */
#ifndef TPL_USER_ACTION
#define TPL_USER_ACTION
#endif

/* Code executed at the end of each rule. */
#ifndef TPL_BREAK
#define TPL_BREAK break;
#endif

#define TPL_RULE_SETUP \
	TPL_USER_ACTION

TPL_DECL
	{
	register tpl_state_type tpl_current_state;
	register char *tpl_cp, *tpl_bp;
	register int tpl_act;




	if ( tpl_init )
		{
		tpl_init = 0;

#ifdef TPL_USER_INIT
		TPL_USER_INIT;
#endif

		if ( ! tpl_start )
			tpl_start = 1;	/* first start state */

		if ( ! tplin )
			tplin = stdin;

		if ( ! tplout )
			tplout = stdout;

		if ( ! tpl_current_buffer )
			tpl_current_buffer =
				tpl_create_buffer( tplin, TPL_BUF_SIZE );

		tpl_load_buffer_state();
		}

	while ( 1 )		/* loops until end-of-file is reached */
		{
		tpl_cp = tpl_c_buf_p;

		/* Support of tpltext. */
		*tpl_cp = tpl_hold_char;

		/* tpl_bp points to the position in tpl_ch_buf of the start of
		 * the current run.
		 */
		tpl_bp = tpl_cp;

		tpl_current_state = tpl_start;
tpl_match:
		do
			{
			register TPL_CHAR tpl_c = tpl_ec[TPL_SC_TO_UI(*tpl_cp)];
			if ( tpl_accept[tpl_current_state] )
				{
				tpl_last_accepting_state = tpl_current_state;
				tpl_last_accepting_cpos = tpl_cp;
				}
			while ( tpl_chk[tpl_base[tpl_current_state] + tpl_c] != tpl_current_state )
				{
				tpl_current_state = (int) tpl_def[tpl_current_state];
				if ( tpl_current_state >= 55 )
					tpl_c = tpl_meta[(unsigned int) tpl_c];
				}
			tpl_current_state = tpl_nxt[tpl_base[tpl_current_state] + (unsigned int) tpl_c];
			++tpl_cp;
			}
		while ( tpl_base[tpl_current_state] != 57 );

tpl_find_action:
		tpl_act = tpl_accept[tpl_current_state];
		if ( tpl_act == 0 )
			{ /* have to back up */
			tpl_cp = tpl_last_accepting_cpos;
			tpl_current_state = tpl_last_accepting_state;
			tpl_act = tpl_accept[tpl_current_state];
			}

		TPL_DO_BEFORE_ACTION;


do_action:	/* This label is used only to access EOF actions. */


		switch ( tpl_act )
	{ /* beginning of action switch */
			case 0: /* must back up */
			/* undo the effects of TPL_DO_BEFORE_ACTION */
			*tpl_cp = tpl_hold_char;
			tpl_cp = tpl_last_accepting_cpos;
			tpl_current_state = tpl_last_accepting_state;
			goto tpl_find_action;

case 1:
TPL_RULE_SETUP
{ BEGIN(TPL); return TPL_BEGIN; }
	TPL_BREAK
case 2:
TPL_RULE_SETUP
{ BEGIN(INITIAL); return TPL_END; }
	TPL_BREAK
case 3:
TPL_RULE_SETUP
{ return TPL_ATCNAME; }
	TPL_BREAK
case 4:
TPL_RULE_SETUP
{ return TPL_ATCNAMENOPLUS; }
	TPL_BREAK
case 5:
TPL_RULE_SETUP
{ return TPL_ATCINDEX; }
	TPL_BREAK
case 6:
TPL_RULE_SETUP
{ return TPL_ATCTYPE; }
	TPL_BREAK
case 7:
TPL_RULE_SETUP
{ return TPL_ATCPARAMNUM; }
	TPL_BREAK
case 8:
TPL_RULE_SETUP
{ return 10; }
	TPL_BREAK
case 9:
TPL_RULE_SETUP
{ return tpltext[0]; }
	TPL_BREAK
case 10:
TPL_RULE_SETUP
ECHO;
	TPL_BREAK
case TPL_STATE_EOF(INITIAL):
case TPL_STATE_EOF(TPL):
	tplterminate();

	case TPL_END_OF_BUFFER:
		{
		/* Amount of text matched not including the EOB char. */
		int tpl_amount_of_matched_text = (int) (tpl_cp - tpltext_ptr) - 1;

		/* Undo the effects of TPL_DO_BEFORE_ACTION. */
		*tpl_cp = tpl_hold_char;
		TPL_RESTORE_TPL_MORE_OFFSET

		if ( tpl_current_buffer->tpl_buffer_status == TPL_BUFFER_NEW )
			{
			/* We're scanning a new file or input source.  It's
			 * possible that this happened because the user
			 * just pointed tplin at a new source and called
			 * tpllex().  If so, then we have to assure
			 * consistency between tpl_current_buffer and our
			 * globals.  Here is the right place to do so, because
			 * this is the first action (other than possibly a
			 * back-up) that will match for the new input source.
			 */
			tpl_n_chars = tpl_current_buffer->tpl_n_chars;
			tpl_current_buffer->tpl_input_file = tplin;
			tpl_current_buffer->tpl_buffer_status = TPL_BUFFER_NORMAL;
			}

		/* Note that here we test for tpl_c_buf_p "<=" to the position
		 * of the first EOB in the buffer, since tpl_c_buf_p will
		 * already have been incremented past the NUL character
		 * (since all states make transitions on EOB to the
		 * end-of-buffer state).  Contrast this with the test
		 * in input().
		 */
		if ( tpl_c_buf_p <= &tpl_current_buffer->tpl_ch_buf[tpl_n_chars] )
			{ /* This was really a NUL. */
			tpl_state_type tpl_next_state;

			tpl_c_buf_p = tpltext_ptr + tpl_amount_of_matched_text;

			tpl_current_state = tpl_get_previous_state();

			/* Okay, we're now positioned to make the NUL
			 * transition.  We couldn't have
			 * tpl_get_previous_state() go ahead and do it
			 * for us because it doesn't know how to deal
			 * with the possibility of jamming (and we don't
			 * want to build jamming into it because then it
			 * will run more slowly).
			 */

			tpl_next_state = tpl_try_NUL_trans( tpl_current_state );

			tpl_bp = tpltext_ptr + TPL_MORE_ADJ;

			if ( tpl_next_state )
				{
				/* Consume the NUL. */
				tpl_cp = ++tpl_c_buf_p;
				tpl_current_state = tpl_next_state;
				goto tpl_match;
				}

			else
				{
				tpl_cp = tpl_c_buf_p;
				goto tpl_find_action;
				}
			}

		else switch ( tpl_get_next_buffer() )
			{
			case EOB_ACT_END_OF_FILE:
				{
				tpl_did_buffer_switch_on_eof = 0;

				if ( tplwrap() )
					{
					/* Note: because we've taken care in
					 * tpl_get_next_buffer() to have set up
					 * tpltext, we can now set up
					 * tpl_c_buf_p so that if some total
					 * hoser (like flex itself) wants to
					 * call the scanner after we return the
					 * TPL_NULL, it'll still work - another
					 * TPL_NULL will get returned.
					 */
					tpl_c_buf_p = tpltext_ptr + TPL_MORE_ADJ;

					tpl_act = TPL_STATE_EOF(TPL_START);
					goto do_action;
					}

				else
					{
					if ( ! tpl_did_buffer_switch_on_eof )
						TPL_NEW_FILE;
					}
				break;
				}

			case EOB_ACT_CONTINUE_SCAN:
				tpl_c_buf_p =
					tpltext_ptr + tpl_amount_of_matched_text;

				tpl_current_state = tpl_get_previous_state();

				tpl_cp = tpl_c_buf_p;
				tpl_bp = tpltext_ptr + TPL_MORE_ADJ;
				goto tpl_match;

			case EOB_ACT_LAST_MATCH:
				tpl_c_buf_p =
				&tpl_current_buffer->tpl_ch_buf[tpl_n_chars];

				tpl_current_state = tpl_get_previous_state();

				tpl_cp = tpl_c_buf_p;
				tpl_bp = tpltext_ptr + TPL_MORE_ADJ;
				goto tpl_find_action;
			}
		break;
		}

	default:
		TPL_FATAL_ERROR(
			"fatal flex scanner internal error--no action found" );
	} /* end of action switch */
		} /* end of scanning one token */
	} /* end of tpllex */


/* tpl_get_next_buffer - try to read in a new buffer
 *
 * Returns a code representing an action:
 *	EOB_ACT_LAST_MATCH -
 *	EOB_ACT_CONTINUE_SCAN - continue scanning from current position
 *	EOB_ACT_END_OF_FILE - end of file
 */

static int tpl_get_next_buffer()
	{
	register char *dest = tpl_current_buffer->tpl_ch_buf;
	register char *source = tpltext_ptr;
	register int number_to_move, i;
	int ret_val;

	if ( tpl_c_buf_p > &tpl_current_buffer->tpl_ch_buf[tpl_n_chars + 1] )
		TPL_FATAL_ERROR(
		"fatal flex scanner internal error--end of buffer missed" );

	if ( tpl_current_buffer->tpl_fill_buffer == 0 )
		{ /* Don't try to fill the buffer, so this is an EOF. */
		if ( tpl_c_buf_p - tpltext_ptr - TPL_MORE_ADJ == 1 )
			{
			/* We matched a single character, the EOB, so
			 * treat this as a final EOF.
			 */
			return EOB_ACT_END_OF_FILE;
			}

		else
			{
			/* We matched some text prior to the EOB, first
			 * process it.
			 */
			return EOB_ACT_LAST_MATCH;
			}
		}

	/* Try to read more data. */

	/* First move last chars to start of buffer. */
	number_to_move = (int) (tpl_c_buf_p - tpltext_ptr) - 1;

	for ( i = 0; i < number_to_move; ++i )
		*(dest++) = *(source++);

	if ( tpl_current_buffer->tpl_buffer_status == TPL_BUFFER_EOF_PENDING )
		/* don't do the read, it's not guaranteed to return an EOF,
		 * just force an EOF
		 */
		tpl_current_buffer->tpl_n_chars = tpl_n_chars = 0;

	else
		{
		int num_to_read =
			tpl_current_buffer->tpl_buf_size - number_to_move - 1;

		while ( num_to_read <= 0 )
			{ /* Not enough room in the buffer - grow it. */
#ifdef TPL_USES_REJECT
			TPL_FATAL_ERROR(
"input buffer overflow, can't enlarge buffer because scanner uses REJECT" );
#else

			/* just a shorter name for the current buffer */
			TPL_BUFFER_STATE b = tpl_current_buffer;

			int tpl_c_buf_p_offset =
				(int) (tpl_c_buf_p - b->tpl_ch_buf);

			if ( b->tpl_is_our_buffer )
				{
				int new_size = b->tpl_buf_size * 2;

				if ( new_size <= 0 )
					b->tpl_buf_size += b->tpl_buf_size / 8;
				else
					b->tpl_buf_size *= 2;

				b->tpl_ch_buf = (char *)
					/* Include room in for 2 EOB chars. */
					tpl_flex_realloc( (void *) b->tpl_ch_buf,
							 b->tpl_buf_size + 2 );
				}
			else
				/* Can't grow it, we don't own it. */
				b->tpl_ch_buf = 0;

			if ( ! b->tpl_ch_buf )
				TPL_FATAL_ERROR(
				"fatal error - scanner input buffer overflow" );

			tpl_c_buf_p = &b->tpl_ch_buf[tpl_c_buf_p_offset];

			num_to_read = tpl_current_buffer->tpl_buf_size -
						number_to_move - 1;
#endif
			}

		if ( num_to_read > TPL_READ_BUF_SIZE )
			num_to_read = TPL_READ_BUF_SIZE;

		/* Read in more data. */
		TPL_INPUT( (&tpl_current_buffer->tpl_ch_buf[number_to_move]),
			tpl_n_chars, num_to_read );

		tpl_current_buffer->tpl_n_chars = tpl_n_chars;
		}

	if ( tpl_n_chars == 0 )
		{
		if ( number_to_move == TPL_MORE_ADJ )
			{
			ret_val = EOB_ACT_END_OF_FILE;
			tplrestart( tplin );
			}

		else
			{
			ret_val = EOB_ACT_LAST_MATCH;
			tpl_current_buffer->tpl_buffer_status =
				TPL_BUFFER_EOF_PENDING;
			}
		}

	else
		ret_val = EOB_ACT_CONTINUE_SCAN;

	tpl_n_chars += number_to_move;
	tpl_current_buffer->tpl_ch_buf[tpl_n_chars] = TPL_END_OF_BUFFER_CHAR;
	tpl_current_buffer->tpl_ch_buf[tpl_n_chars + 1] = TPL_END_OF_BUFFER_CHAR;

	tpltext_ptr = &tpl_current_buffer->tpl_ch_buf[0];

	return ret_val;
	}


/* tpl_get_previous_state - get the state just before the EOB char was reached */

static tpl_state_type tpl_get_previous_state()
	{
	register tpl_state_type tpl_current_state;
	register char *tpl_cp;

	tpl_current_state = tpl_start;

	for ( tpl_cp = tpltext_ptr + TPL_MORE_ADJ; tpl_cp < tpl_c_buf_p; ++tpl_cp )
		{
		register TPL_CHAR tpl_c = (*tpl_cp ? tpl_ec[TPL_SC_TO_UI(*tpl_cp)] : 1);
		if ( tpl_accept[tpl_current_state] )
			{
			tpl_last_accepting_state = tpl_current_state;
			tpl_last_accepting_cpos = tpl_cp;
			}
		while ( tpl_chk[tpl_base[tpl_current_state] + tpl_c] != tpl_current_state )
			{
			tpl_current_state = (int) tpl_def[tpl_current_state];
			if ( tpl_current_state >= 55 )
				tpl_c = tpl_meta[(unsigned int) tpl_c];
			}
		tpl_current_state = tpl_nxt[tpl_base[tpl_current_state] + (unsigned int) tpl_c];
		}

	return tpl_current_state;
	}


/* tpl_try_NUL_trans - try to make a transition on the NUL character
 *
 * synopsis
 *	next_state = tpl_try_NUL_trans( current_state );
 */

#ifdef TPL_USE_PROTOS
static tpl_state_type tpl_try_NUL_trans( tpl_state_type tpl_current_state )
#else
static tpl_state_type tpl_try_NUL_trans( tpl_current_state )
tpl_state_type tpl_current_state;
#endif
	{
	register int tpl_is_jam;
	register char *tpl_cp = tpl_c_buf_p;

	register TPL_CHAR tpl_c = 1;
	if ( tpl_accept[tpl_current_state] )
		{
		tpl_last_accepting_state = tpl_current_state;
		tpl_last_accepting_cpos = tpl_cp;
		}
	while ( tpl_chk[tpl_base[tpl_current_state] + tpl_c] != tpl_current_state )
		{
		tpl_current_state = (int) tpl_def[tpl_current_state];
		if ( tpl_current_state >= 55 )
			tpl_c = tpl_meta[(unsigned int) tpl_c];
		}
	tpl_current_state = tpl_nxt[tpl_base[tpl_current_state] + (unsigned int) tpl_c];
	tpl_is_jam = (tpl_current_state == 54);

	return tpl_is_jam ? 0 : tpl_current_state;
	}


#ifndef TPL_NO_UNPUT
#ifdef TPL_USE_PROTOS
static void tplunput( int c, register char *tpl_bp )
#else
static void tplunput( c, tpl_bp )
int c;
register char *tpl_bp;
#endif
	{
	register char *tpl_cp = tpl_c_buf_p;

	/* undo effects of setting up tpltext */
	*tpl_cp = tpl_hold_char;

	if ( tpl_cp < tpl_current_buffer->tpl_ch_buf + 2 )
		{ /* need to shift things up to make room */
		/* +2 for EOB chars. */
		register int number_to_move = tpl_n_chars + 2;
		register char *dest = &tpl_current_buffer->tpl_ch_buf[
					tpl_current_buffer->tpl_buf_size + 2];
		register char *source =
				&tpl_current_buffer->tpl_ch_buf[number_to_move];

		while ( source > tpl_current_buffer->tpl_ch_buf )
			*--dest = *--source;

		tpl_cp += (int) (dest - source);
		tpl_bp += (int) (dest - source);
		tpl_current_buffer->tpl_n_chars =
			tpl_n_chars = tpl_current_buffer->tpl_buf_size;

		if ( tpl_cp < tpl_current_buffer->tpl_ch_buf + 2 )
			TPL_FATAL_ERROR( "flex scanner push-back overflow" );
		}

	*--tpl_cp = (char) c;


	tpltext_ptr = tpl_bp;
	tpl_hold_char = *tpl_cp;
	tpl_c_buf_p = tpl_cp;
	}
#endif	/* ifndef TPL_NO_UNPUT */


#ifdef __cplusplus
static int tplinput()
#else
static int input()
#endif
	{
	int c;

	*tpl_c_buf_p = tpl_hold_char;

	if ( *tpl_c_buf_p == TPL_END_OF_BUFFER_CHAR )
		{
		/* tpl_c_buf_p now points to the character we want to return.
		 * If this occurs *before* the EOB characters, then it's a
		 * valid NUL; if not, then we've hit the end of the buffer.
		 */
		if ( tpl_c_buf_p < &tpl_current_buffer->tpl_ch_buf[tpl_n_chars] )
			/* This was really a NUL. */
			*tpl_c_buf_p = '\0';

		else
			{ /* need more input */
			int offset = tpl_c_buf_p - tpltext_ptr;
			++tpl_c_buf_p;

			switch ( tpl_get_next_buffer() )
				{
				case EOB_ACT_LAST_MATCH:
					/* This happens because tpl_g_n_b()
					 * sees that we've accumulated a
					 * token and flags that we need to
					 * try matching the token before
					 * proceeding.  But for input(),
					 * there's no matching to consider.
					 * So convert the EOB_ACT_LAST_MATCH
					 * to EOB_ACT_END_OF_FILE.
					 */

					/* Reset buffer status. */
					tplrestart( tplin );

					/* fall through */

				case EOB_ACT_END_OF_FILE:
					{
					if ( tplwrap() )
						return EOF;

					if ( ! tpl_did_buffer_switch_on_eof )
						TPL_NEW_FILE;
#ifdef __cplusplus
					return tplinput();
#else
					return input();
#endif
					}

				case EOB_ACT_CONTINUE_SCAN:
					tpl_c_buf_p = tpltext_ptr + offset;
					break;
				}
			}
		}

	c = *(unsigned char *) tpl_c_buf_p;	/* cast for 8-bit char's */
	*tpl_c_buf_p = '\0';	/* preserve tpltext */
	tpl_hold_char = *++tpl_c_buf_p;


	return c;
	}


#ifdef TPL_USE_PROTOS
void tplrestart( FILE *input_file )
#else
void tplrestart( input_file )
FILE *input_file;
#endif
	{
	if ( ! tpl_current_buffer )
		tpl_current_buffer = tpl_create_buffer( tplin, TPL_BUF_SIZE );

	tpl_init_buffer( tpl_current_buffer, input_file );
	tpl_load_buffer_state();
	}


#ifdef TPL_USE_PROTOS
void tpl_switch_to_buffer( TPL_BUFFER_STATE new_buffer )
#else
void tpl_switch_to_buffer( new_buffer )
TPL_BUFFER_STATE new_buffer;
#endif
	{
	if ( tpl_current_buffer == new_buffer )
		return;

	if ( tpl_current_buffer )
		{
		/* Flush out information for old buffer. */
		*tpl_c_buf_p = tpl_hold_char;
		tpl_current_buffer->tpl_buf_pos = tpl_c_buf_p;
		tpl_current_buffer->tpl_n_chars = tpl_n_chars;
		}

	tpl_current_buffer = new_buffer;
	tpl_load_buffer_state();

	/* We don't actually know whether we did this switch during
	 * EOF (tplwrap()) processing, but the only time this flag
	 * is looked at is after tplwrap() is called, so it's safe
	 * to go ahead and always set it.
	 */
	tpl_did_buffer_switch_on_eof = 1;
	}


#ifdef TPL_USE_PROTOS
void tpl_load_buffer_state( void )
#else
void tpl_load_buffer_state()
#endif
	{
	tpl_n_chars = tpl_current_buffer->tpl_n_chars;
	tpltext_ptr = tpl_c_buf_p = tpl_current_buffer->tpl_buf_pos;
	tplin = tpl_current_buffer->tpl_input_file;
	tpl_hold_char = *tpl_c_buf_p;
	}


#ifdef TPL_USE_PROTOS
TPL_BUFFER_STATE tpl_create_buffer( FILE *file, int size )
#else
TPL_BUFFER_STATE tpl_create_buffer( file, size )
FILE *file;
int size;
#endif
	{
	TPL_BUFFER_STATE b;

	b = (TPL_BUFFER_STATE) tpl_flex_alloc( sizeof( struct tpl_buffer_state ) );
	if ( ! b )
		TPL_FATAL_ERROR( "out of dynamic memory in tpl_create_buffer()" );

	b->tpl_buf_size = size;

	/* tpl_ch_buf has to be 2 characters longer than the size given because
	 * we need to put in 2 end-of-buffer characters.
	 */
	b->tpl_ch_buf = (char *) tpl_flex_alloc( b->tpl_buf_size + 2 );
	if ( ! b->tpl_ch_buf )
		TPL_FATAL_ERROR( "out of dynamic memory in tpl_create_buffer()" );

	b->tpl_is_our_buffer = 1;

	tpl_init_buffer( b, file );

	return b;
	}


#ifdef TPL_USE_PROTOS
void tpl_delete_buffer( TPL_BUFFER_STATE b )
#else
void tpl_delete_buffer( b )
TPL_BUFFER_STATE b;
#endif
	{
	if ( ! b )
		return;

	if ( b == tpl_current_buffer )
		tpl_current_buffer = (TPL_BUFFER_STATE) 0;

	if ( b->tpl_is_our_buffer )
		tpl_flex_free( (void *) b->tpl_ch_buf );

	tpl_flex_free( (void *) b );
	}


#ifndef TPL_ALWAYS_INTERACTIVE
#ifndef TPL_NEVER_INTERACTIVE
extern int isatty TPL_PROTO(( int ));
#endif
#endif

#ifdef TPL_USE_PROTOS
void tpl_init_buffer( TPL_BUFFER_STATE b, FILE *file )
#else
void tpl_init_buffer( b, file )
TPL_BUFFER_STATE b;
FILE *file;
#endif


	{
	tpl_flush_buffer( b );

	b->tpl_input_file = file;
	b->tpl_fill_buffer = 1;

#if TPL_ALWAYS_INTERACTIVE
	b->tpl_is_interactive = 1;
#else
#if TPL_NEVER_INTERACTIVE
	b->tpl_is_interactive = 0;
#else
	b->tpl_is_interactive = file ? (isatty( fileno(file) ) > 0) : 0;
#endif
#endif
	}


#ifdef TPL_USE_PROTOS
void tpl_flush_buffer( TPL_BUFFER_STATE b )
#else
void tpl_flush_buffer( b )
TPL_BUFFER_STATE b;
#endif

	{
	if ( ! b )
		return;

	b->tpl_n_chars = 0;

	/* We always need two end-of-buffer characters.  The first causes
	 * a transition to the end-of-buffer state.  The second causes
	 * a jam in that state.
	 */
	b->tpl_ch_buf[0] = TPL_END_OF_BUFFER_CHAR;
	b->tpl_ch_buf[1] = TPL_END_OF_BUFFER_CHAR;

	b->tpl_buf_pos = &b->tpl_ch_buf[0];

	b->tpl_at_bol = 1;
	b->tpl_buffer_status = TPL_BUFFER_NEW;

	if ( b == tpl_current_buffer )
		tpl_load_buffer_state();
	}


#ifndef TPL_NO_SCAN_BUFFER
#ifdef TPL_USE_PROTOS
TPL_BUFFER_STATE tpl_scan_buffer( char *base, tpl_size_t size )
#else
TPL_BUFFER_STATE tpl_scan_buffer( base, size )
char *base;
tpl_size_t size;
#endif
	{
	TPL_BUFFER_STATE b;

	if ( size < 2 ||
	     base[size-2] != TPL_END_OF_BUFFER_CHAR ||
	     base[size-1] != TPL_END_OF_BUFFER_CHAR )
		/* They forgot to leave room for the EOB's. */
		return 0;

	b = (TPL_BUFFER_STATE) tpl_flex_alloc( sizeof( struct tpl_buffer_state ) );
	if ( ! b )
		TPL_FATAL_ERROR( "out of dynamic memory in tpl_scan_buffer()" );

	b->tpl_buf_size = size - 2;	/* "- 2" to take care of EOB's */
	b->tpl_buf_pos = b->tpl_ch_buf = base;
	b->tpl_is_our_buffer = 0;
	b->tpl_input_file = 0;
	b->tpl_n_chars = b->tpl_buf_size;
	b->tpl_is_interactive = 0;
	b->tpl_at_bol = 1;
	b->tpl_fill_buffer = 0;
	b->tpl_buffer_status = TPL_BUFFER_NEW;

	tpl_switch_to_buffer( b );

	return b;
	}
#endif


#ifndef TPL_NO_SCAN_STRING
#ifdef TPL_USE_PROTOS
TPL_BUFFER_STATE tpl_scan_string( tplconst char *tpl_str )
#else
TPL_BUFFER_STATE tpl_scan_string( tpl_str )
tplconst char *tpl_str;
#endif
	{
	int len;
	for ( len = 0; tpl_str[len]; ++len )
		;

	return tpl_scan_bytes( tpl_str, len );
	}
#endif


#ifndef TPL_NO_SCAN_BYTES
#ifdef TPL_USE_PROTOS
TPL_BUFFER_STATE tpl_scan_bytes( tplconst char *bytes, int len )
#else
TPL_BUFFER_STATE tpl_scan_bytes( bytes, len )
tplconst char *bytes;
int len;
#endif
	{
	TPL_BUFFER_STATE b;
	char *buf;
	tpl_size_t n;
	int i;

	/* Get memory for full buffer, including space for trailing EOB's. */
	n = len + 2;
	buf = (char *) tpl_flex_alloc( n );
	if ( ! buf )
		TPL_FATAL_ERROR( "out of dynamic memory in tpl_scan_bytes()" );

	for ( i = 0; i < len; ++i )
		buf[i] = bytes[i];

	buf[len] = buf[len+1] = TPL_END_OF_BUFFER_CHAR;

	b = tpl_scan_buffer( buf, n );
	if ( ! b )
		TPL_FATAL_ERROR( "bad buffer in tpl_scan_bytes()" );

	/* It's okay to grow etc. this buffer, and we should throw it
	 * away when we're done.
	 */
	b->tpl_is_our_buffer = 1;

	return b;
	}
#endif


#ifndef TPL_NO_PUSH_STATE
#ifdef TPL_USE_PROTOS
static void tpl_push_state( int new_state )
#else
static void tpl_push_state( new_state )
int new_state;
#endif
	{
	if ( tpl_start_stack_ptr >= tpl_start_stack_depth )
		{
		tpl_size_t new_size;

		tpl_start_stack_depth += TPL_START_STACK_INCR;
		new_size = tpl_start_stack_depth * sizeof( int );

		if ( ! tpl_start_stack )
			tpl_start_stack = (int *) tpl_flex_alloc( new_size );

		else
			tpl_start_stack = (int *) tpl_flex_realloc(
					(void *) tpl_start_stack, new_size );

		if ( ! tpl_start_stack )
			TPL_FATAL_ERROR(
			"out of memory expanding start-condition stack" );
		}

	tpl_start_stack[tpl_start_stack_ptr++] = TPL_START;

	BEGIN(new_state);
	}
#endif


#ifndef TPL_NO_POP_STATE
static void tpl_pop_state()
	{
	if ( --tpl_start_stack_ptr < 0 )
		TPL_FATAL_ERROR( "start-condition stack underflow" );

	BEGIN(tpl_start_stack[tpl_start_stack_ptr]);
	}
#endif


#ifndef TPL_NO_TOP_STATE
static int tpl_top_state()
	{
	return tpl_start_stack[tpl_start_stack_ptr - 1];
	}
#endif

#ifndef TPL_EXIT_FAILURE
#define TPL_EXIT_FAILURE 2
#endif

#ifdef TPL_USE_PROTOS
static void tpl_fatal_error( tplconst char msg[] )
#else
static void tpl_fatal_error( msg )
char msg[];
#endif
	{
	(void) fprintf( stderr, "%s\n", msg );
	exit( TPL_EXIT_FAILURE );
	}



/* Redefine tplless() so it works in section 3 code. */

#undef tplless
#define tplless(n) \
	do \
		{ \
		/* Undo effects of setting up tpltext. */ \
		tpltext[tplleng] = tpl_hold_char; \
		tpl_c_buf_p = tpltext + n; \
		tpl_hold_char = *tpl_c_buf_p; \
		*tpl_c_buf_p = '\0'; \
		tplleng = n; \
		} \
	while ( 0 )


/* Internal utility routines. */

#ifndef tpltext_ptr
#ifdef TPL_USE_PROTOS
static void tpl_flex_strncpy( char *s1, tplconst char *s2, int n )
#else
static void tpl_flex_strncpy( s1, s2, n )
char *s1;
tplconst char *s2;
int n;
#endif
	{
	register int i;
	for ( i = 0; i < n; ++i )
		s1[i] = s2[i];
	}
#endif

#ifdef TPL_NEED_STRLEN
#ifdef TPL_USE_PROTOS
static int tpl_flex_strlen( tplconst char *s )
#else
static int tpl_flex_strlen( s )
tplconst char *s;
#endif
	{
	register int n;
	for ( n = 0; s[n]; ++n )
		;

	return n;
	}
#endif


#ifdef TPL_USE_PROTOS
static void *tpl_flex_alloc( tpl_size_t size )
#else
static void *tpl_flex_alloc( size )
tpl_size_t size;
#endif
	{
	return (void *) malloc( size );
	}

#ifdef TPL_USE_PROTOS
static void *tpl_flex_realloc( void *ptr, tpl_size_t size )
#else
static void *tpl_flex_realloc( ptr, size )
void *ptr;
tpl_size_t size;
#endif
	{
	/* The cast to (char *) in the following accommodates both
	 * implementations that use char* generic pointers, and those
	 * that use void* generic pointers.  It works with the latter
	 * because both ANSI C and C++ allow castless assignment from
	 * any pointer type to void*, and deal with argument conversions
	 * as though doing an assignment.
	 */
	return (void *) realloc( (char *) ptr, size );
	}

#ifdef TPL_USE_PROTOS
static void tpl_flex_free( void *ptr )
#else
static void tpl_flex_free( ptr )
void *ptr;
#endif
	{
	free( ptr );
	}

#if TPL_MAIN
int main()
	{
	tpllex();
	return 0;
	}
#endif

