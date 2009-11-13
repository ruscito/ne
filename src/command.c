/* Command table manipulation functions and vectors.

	Copyright (C) 1993-1998 Sebastiano Vigna 
	Copyright (C) 1999-2009 Todd M. Lewis and Sebastiano Vigna

	This file is part of ne, the nice editor.

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the
	Free Software Foundation; either version 2, or (at your option) any
	later version.
	
	This program is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	General Public License for more details.
	
	You should have received a copy of the GNU General Public License along
	with this program; see the file COPYING.  If not, write to the Free
	Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
	02111-1307, USA.  */

#include "ne.h"
#include "help.h"
#include "hash.h"

/* The standard macro descriptor allocation dimension. */

#define STD_MACRO_DESC_SIZE	1024


/* This structure represents a command. It includes a long and a short name,
a NULL-terminated vector of help strings (of specified length) and some flags
which are related to the syntax and the semantics of the arguments. */

typedef struct {
	const char *name, *short_name;
	const char * const *help;
	int help_len;
	int flags;
} command;


#define NO_ARGS					(1<<1)			/* This command must be called without argument. */
#define ARG_IS_STRING			(1<<2)			/* The argument is a string (default is a number). */
#define IS_OPTION					(1<<3)			/* The command controls an option,
															and can be played while exec_only_options is true. */
#define DO_NOT_RECORD			(1<<4)			/* Never record this command. */
#define EMPTY_STRING_OK			(1<<5)			/* This command can accept an empty string ("") as an argument. */


/* These macros makes the following vector more readable. */

#define HELP_LEN(x)			(sizeof(x ## _HELP) / sizeof(char *) - 1)
#define NAHL(x) x ## _NAME, x ##_ABBREV, x ## _HELP, HELP_LEN(x)

/* This is the command vector. Note that the command names come from names.h,
   and the help names come from help.h. */

static const command commands[ACTION_COUNT] = {

	{ NAHL(ABOUT         ), NO_ARGS                                                               },
	{ NAHL(ADJUSTVIEW    ),           ARG_IS_STRING                                               },
	{ NAHL(ALERT         ), NO_ARGS                                                               },
	{ NAHL(AUTOINDENT    ),                           IS_OPTION                                   },
	{ NAHL(AUTOPREFS     ),                           IS_OPTION                                   },
	{ NAHL(BACKSPACE     ),0                                                                      },
	{ NAHL(BEEP          ), NO_ARGS                                                               },
	{ NAHL(BINARY        ),                           IS_OPTION                                   },
	{ NAHL(CAPITALIZE    ),0                                                                      },
	{ NAHL(CASESEARCH    ),                           IS_OPTION                                   },
	{ NAHL(CENTER        ),0                                                                      },
	{ NAHL(CLEAR         ), NO_ARGS                                                               },
	{ NAHL(CLIPNUMBER    ),                           IS_OPTION                                   },
	{ NAHL(CLOSEDOC      ), NO_ARGS                                                               },
	{ NAHL(COPY          ),0                                                                      },
	{ NAHL(CRLF          ),                           IS_OPTION                                   },
	{ NAHL(CUT           ),0                                                                      },
	{ NAHL(DELETECHAR    ),0                                                                      },
	{ NAHL(DELETEEOL     ), NO_ARGS                                                               },
	{ NAHL(DELETELINE    ),0                                                                      },
	{ NAHL(DELETENEXTWORD),0                                                                      },
	{ NAHL(DELETEPREVWORD),0                                                                      },
	{ NAHL(DOUNDO        ),                           IS_OPTION                                   },
	{ NAHL(ERASE         ),0                                                                      },
	{ NAHL(ESCAPE        ),                                       DO_NOT_RECORD                   },
	{ NAHL(ESCAPETIME    ),                           IS_OPTION                                   },
	{ NAHL(EXEC          ),           ARG_IS_STRING |             DO_NOT_RECORD                   },
	{ NAHL(EXIT          ), NO_ARGS                                                               },
	{ NAHL(FASTGUI       ),                           IS_OPTION                                   },
	{ NAHL(FIND          ),           ARG_IS_STRING                                               },
	{ NAHL(FINDREGEXP    ),           ARG_IS_STRING                                               },
	{ NAHL(FLAGS         ), NO_ARGS |                             DO_NOT_RECORD                   },
	{ NAHL(FLASH         ), NO_ARGS                                                               },
	{ NAHL(FREEFORM      ),                           IS_OPTION                                   },
	{ NAHL(GOTOBOOKMARK  ),           ARG_IS_STRING |                             EMPTY_STRING_OK },
	{ NAHL(GOTOCOLUMN    ),0                                                                      },
	{ NAHL(GOTOLINE      ),0                                                                      },
	{ NAHL(GOTOMARK      ), NO_ARGS                                                               },
	{ NAHL(HELP          ),           ARG_IS_STRING |             DO_NOT_RECORD                   },
	{ NAHL(HEXCODE       ),                           IS_OPTION                                   },
	{ NAHL(INSERT        ),                           IS_OPTION                                   },
	{ NAHL(INSERTCHAR    ),0                                                                      },
	{ NAHL(INSERTLINE    ),0                                                                      },
	{ NAHL(INSERTSTRING  ),           ARG_IS_STRING                                               },
	{ NAHL(KEYCODE       ),                                       DO_NOT_RECORD                   },
	{ NAHL(LINEDOWN      ),0                                                                      },
	{ NAHL(LINEUP        ),0                                                                      },
	{ NAHL(LOADAUTOPREFS ), NO_ARGS                                                               },
	{ NAHL(LOADPREFS     ),           ARG_IS_STRING                                               },
	{ NAHL(MACRO         ),           ARG_IS_STRING |             DO_NOT_RECORD                   },
	{ NAHL(MARK          ),                           IS_OPTION                                   },
	{ NAHL(MARKVERT      ),                           IS_OPTION                                   },
	{ NAHL(MATCHBRACKET  ), NO_ARGS                                                               },
	{ NAHL(MODIFIED      ),                           IS_OPTION                                   },
	{ NAHL(MOVEBOS       ), NO_ARGS                                                               },
	{ NAHL(MOVEEOF       ), NO_ARGS                                                               },
	{ NAHL(MOVEEOL       ), NO_ARGS                                                               },
	{ NAHL(MOVEEOW       ),0                                                                      },
	{ NAHL(MOVEINCDOWN   ), NO_ARGS                                                               },
	{ NAHL(MOVEINCUP     ), NO_ARGS                                                               },
	{ NAHL(MOVELEFT      ),0                                                                      },
	{ NAHL(MOVERIGHT     ),0                                                                      },
	{ NAHL(MOVESOF       ), NO_ARGS                                                               },
	{ NAHL(MOVESOL       ), NO_ARGS                                                               },
	{ NAHL(MOVETOS       ), NO_ARGS                                                               },
	{ NAHL(NEWDOC        ), NO_ARGS                                                               },
	{ NAHL(NEXTDOC       ),0                                                                      },
	{ NAHL(NEXTPAGE      ),0                                                                      },
	{ NAHL(NEXTWORD      ),0                                                                      },
	{ NAHL(NOFILEREQ     ),                           IS_OPTION                                   },
	{ NAHL(NOP           ), NO_ARGS                                                               },
	{ NAHL(OPEN          ),           ARG_IS_STRING                                               },
	{ NAHL(OPENCLIP      ),           ARG_IS_STRING                                               },
	{ NAHL(OPENMACRO     ),           ARG_IS_STRING                                               },
	{ NAHL(OPENNEW       ),           ARG_IS_STRING                                               },
	{ NAHL(PAGEDOWN      ),0                                                                      },
	{ NAHL(PAGEUP        ),0                                                                      },
	{ NAHL(PARAGRAPH     ),0                                                                      },
	{ NAHL(PASTE         ),0                                                                      },
	{ NAHL(PASTEVERT     ),0                                                                      },
	{ NAHL(PLAY          ),0                                                                      },
	{ NAHL(POPPREFS      ),0                                                                      },
	{ NAHL(PRESERVECR    ),                           IS_OPTION                                   },
	{ NAHL(PREVDOC       ),0                                                                      },
	{ NAHL(PREVPAGE      ),0                                                                      },
	{ NAHL(PREVWORD      ),0                                                                      },
	{ NAHL(PUSHPREFS     ),                           IS_OPTION                                   },
	{ NAHL(QUIT          ), NO_ARGS                                                               },
	{ NAHL(READONLY      ),                           IS_OPTION                                   },
	{ NAHL(RECORD        ),                           IS_OPTION | DO_NOT_RECORD                   },
	{ NAHL(REDO          ),0                                                                      },
	{ NAHL(REFRESH       ), NO_ARGS                                                               },
	{ NAHL(REPEATLAST    ),0                                                                      },
	{ NAHL(REPLACE       ),           ARG_IS_STRING |                             EMPTY_STRING_OK },
	{ NAHL(REPLACEALL    ),           ARG_IS_STRING |                             EMPTY_STRING_OK },
	{ NAHL(REPLACEONCE   ),           ARG_IS_STRING |                             EMPTY_STRING_OK },
	{ NAHL(RIGHTMARGIN   ),                           IS_OPTION                                   },
	{ NAHL(SAVE          ), NO_ARGS                                                               },
	{ NAHL(SAVEAS        ),           ARG_IS_STRING                                               },
	{ NAHL(SAVEAUTOPREFS ), NO_ARGS                                                               },
	{ NAHL(SAVECLIP      ),           ARG_IS_STRING                                               },
	{ NAHL(SAVEDEFPREFS  ), NO_ARGS                                                               },
	{ NAHL(SAVEMACRO     ),           ARG_IS_STRING                                               },
	{ NAHL(SAVEPREFS     ),           ARG_IS_STRING                                               },
	{ NAHL(SEARCHBACK    ),                           IS_OPTION                                   },
	{ NAHL(SELECTDOC     ),0                                                                      },
	{ NAHL(SETBOOKMARK   ),           ARG_IS_STRING |                             EMPTY_STRING_OK },
	{ NAHL(STATUSBAR     ),                           IS_OPTION                                   },
	{ NAHL(SUSPEND       ), NO_ARGS                                                               },
	{ NAHL(SYNTAX        ),           ARG_IS_STRING | IS_OPTION                                   },
	{ NAHL(SYSTEM        ),           ARG_IS_STRING                                               },
	{ NAHL(TABSIZE       ),                           IS_OPTION                                   },
	{ NAHL(THROUGH       ),           ARG_IS_STRING                                               },
	{ NAHL(TOGGLESEOF    ), NO_ARGS                                                               },
	{ NAHL(TOGGLESEOL    ), NO_ARGS                                                               },
	{ NAHL(TOLOWER       ),0                                                                      },
	{ NAHL(TOUPPER       ),0                                                                      },
	{ NAHL(TURBO         ),                           IS_OPTION                                   },
	{ NAHL(UNDELLINE     ),0                                                                      },
	{ NAHL(UNDO          ),0                                                                      },
	{ NAHL(UNLOADMACROS  ), NO_ARGS                                                               },
	{ NAHL(UNSETBOOKMARK ),           ARG_IS_STRING |                             EMPTY_STRING_OK },
	{ NAHL(UTF8          ),                           IS_OPTION                                   },
	{ NAHL(UTF8AUTO      ),                           IS_OPTION                                   },
	{ NAHL(UTF8IO        ),                           IS_OPTION                                   },
	{ NAHL(VERBOSEMACROS ),                           IS_OPTION                                   },
	{ NAHL(VISUALBELL    ),                           IS_OPTION                                   },
	{ NAHL(WORDWRAP      ),                           IS_OPTION                                   },

};


/* Checks whether the command line m starts with the command c. Return 0 on
	success, non-zero on failure. */

int cmdcmp(const unsigned char *c, const unsigned char *m) {

	assert(c != NULL);
	assert(m != NULL);

	while (*c && ascii_up_case[*c] == ascii_up_case[*m]) {
		c++;
		m++;
	}

	return *c || *m && !isasciispace(*m) ;
}





/* This table *can* have conflicts, so that we can keep its size much
	smaller. */

static macro_desc *macro_hash_table[MACRO_HASH_TABLE_SIZE];


/* This is the command name hashing function.We consider only the 5 least
	significant bits because they are the bits which distinguish characters,
	independently of their case. We are not interested in strings which contain
	non-alphabetical characters, because they will certainly generate an error
	(the only exception notably being "R1"). We should subtract 1 to s[i], but
	this doesn't seem to produce any improvement. hash_macro() act as hash(),
	but uses MACRO_HASH_TABLE_SIZE for its modulo. */


static int hash_cmd(const unsigned char * const s, int len) {
	int h = -1;

	while(len-- != 0) h = (h * 31 + ascii_up_case[s[len]]) % HASH_TABLE_SIZE;
	return (h + HASH_TABLE_SIZE) % HASH_TABLE_SIZE;
}


static int hash_macro(const unsigned char * const s, int len) {
	int h = -1;

	while(len-- != 0) h = (h * 31 + ascii_up_case[s[len]]) % MACRO_HASH_TABLE_SIZE;
	return (h + MACRO_HASH_TABLE_SIZE) % MACRO_HASH_TABLE_SIZE;
}



/* Parses a command line. This function has an interface which is slightly
	varied with respect to the other functions of ne. In case of a parsing
	error, an error index *with sign inverted* is passed back. In case parsing
	succeeds, an (greater or equal to zero) action is returned, and the
	numerical or string argument is passed in the variables pointed to by
	num_arg or string_arg, respectively, if they are non-NULL. Otherwise, the
	argument is not passed back. The string argument is free()able. -1 and NULL
	denote the lack of an optional numerical or string argument, respectively.
	NOP is returned on a NOP command, or on a comment line (any line whose first
	non-space character is a non alphabetic character). Note that the various
	syntax flags are used here. */

action parse_command_line(const unsigned char * command_line, int * const num_arg, unsigned char ** const string_arg, const int exec_only_options) {

	int h;
	action a;
	const unsigned char *p = command_line;

	if (num_arg) *num_arg = -1;
	if (string_arg) *string_arg = NULL;

	if (!command_line) return NOP_A;

	if (!*p) return NOP_A;

	while(isasciispace(*p)) p++;

	command_line = p;

	if (!isalpha(*p)) return NOP_A;

	while(*p && !isasciispace(*p)) p++;

	h = hash_cmd(command_line, p - command_line);

	if ((a = hash_table[h]) && !cmdcmp(commands[--a].name, command_line)
		|| (a = short_hash_table[h]) && !cmdcmp(commands[--a].short_name, command_line)) {

		while(isasciispace(*p)) p++;

		if (!(*p && (commands[a].flags & NO_ARGS))) {
			if (!*p || (commands[a].flags & ARG_IS_STRING) || isxdigit(*p) || *p == 'x' || *p =='X') {
				if ((commands[a].flags & IS_OPTION) || !exec_only_options) {
					if (*p) {
						if ((commands[a].flags & ARG_IS_STRING) && string_arg) {
							int len = strlen(p);

							if (len > 1 && *p == '"' && p[len - 1] == '"') {
								p++;
								len -= 2;
							}

							if (len == 0 && !(commands[a].flags & EMPTY_STRING_OK)) return -STRING_IS_EMPTY;

							if (!(*string_arg = malloc(len + 1))) return -OUT_OF_MEMORY;
							memcpy(*string_arg, p, len);
							(*string_arg)[len] = 0;
						}
						else if (num_arg) {
							char *q;
							*num_arg = strtol(p, &q, 0);
							if (*q && !isasciispace(*q)) return -NOT_A_NUMBER;
						}
					}
					return a;
				}
				D(fprintf(stderr,"parse_command error: Can execute only options.\n");)
				return -CAN_EXECUTE_ONLY_OPTIONS;
			}
			D(fprintf(stderr,"parse_command error: Has numeric argument.\n");)
			return -HAS_NUMERIC_ARGUMENT;
		}
		D(fprintf(stderr,"parse_command error: Has no argument.\n");)
		return -HAS_NO_ARGUMENT;
	}
   D(fprintf(stderr,"parse_command error: No such command.\n");)
	return -NO_SUCH_COMMAND;
}


/* Parses and executes a command line. Standard error codes are returned. If
	the search for a standard command fails, we try to execute a macro in ~/.ne
	with the same name. */

int execute_command_line(buffer *b, const unsigned char *command_line) {

	int n, a, len = strlen(command_line);
	encoding_type encoding = detect_encoding(command_line, len);
	unsigned char *p;

	if (b->encoding != ENC_ASCII && encoding != ENC_ASCII && b->encoding != encoding) return INCOMPATIBLE_COMMAND_ENCODING;

	if ((a = parse_command_line(command_line, &n, &p, b->exec_only_options)) >= 0) return do_action(b, a, n, p);

	a = -a;

	if ((a == NO_SUCH_COMMAND) && (a = execute_macro(b, command_line)) == CANT_OPEN_MACRO) a = NO_SUCH_COMMAND;

	return a;
}



/* Allocates a macro descriptor. It does not allocate the internal character
	stream, which has to be allocated and stuffed in separately. */

macro_desc *alloc_macro_desc(void) {
	return calloc(1, sizeof(macro_desc));
}



/* Frees a macro descriptors. */

void free_macro_desc(macro_desc *md) {

	if (!md) return;

	assert_macro_desc(md);

	free(md->name);
	free_char_stream(md->cs);
	free(md);
}



/* Here we record an action in a character stream. The action name is expanded
	in a short or long name, depending on the value of the verbose parameter.  A
	numerical or string argument are expanded and copied, too. If the command
	should not be recorded (for instance, ESCAPE_A) we return. */

void record_action(char_stream *cs, action a, int c, unsigned char *p, int verbose) {

	char t[MAX_INT_LEN + 2];

	if (commands[a].flags & DO_NOT_RECORD) return;

	if (verbose) add_to_stream(cs, commands[a].name, strlen(commands[a].name));
	else add_to_stream(cs, commands[a].short_name, strlen(commands[a].short_name));


	if (c >= 0) {
		sprintf(t, " %d", c);
		add_to_stream(cs, t, strlen(t));
	}
	else if (p) {
		add_to_stream(cs, " ", 1);
		if (!*p || isasciispace(*p)) add_to_stream(cs, "\"", 1);
		add_to_stream(cs, p, strlen(p));
		if (!*p || isasciispace(*p)) add_to_stream(cs, "\"", 1);
	}

	add_to_stream(cs, "", 1);

}


/* A support function for optimize_macro(). It examines a string to see if it
	is a valid "InsertChar ##" command. If it is, then insertchar_val() returns
	the character code, otherwise it returns 0. */

static int insertchar_val(const unsigned char *p) {
	int h;
	const unsigned char *cmd;
	action a;
    
	if ( !p || !*p) return 0;
    
	while(isasciispace(*p)) p++;

	cmd = p;
	
	if (!isalpha(*p)) return 0;
	
	while(*p && !isasciispace(*p)) p++;
	
	h = hash_cmd(cmd, p - cmd);
	
	if (((a = hash_table[h]) && !cmdcmp(commands[--a].name, cmd)
		|| (a = short_hash_table[h]) && !cmdcmp(commands[--a].short_name, cmd))	&& a == INSERTCHAR_A) {

		while(isasciispace(*p)) p++;
		h = strtol(p, (char **)&cmd, 0);
		return *cmd || h < 0 ? 0 : h;
	}
	return 0;
}

/* Looks through the macro stream for consecutive runs of InsertChar commands
	and replaces them with appropriate InsertString commands. This makes macros
	much easier to read if and when they have to be edited. Note that if the
	character inserted by InsertChar is not an ASCII character, then we should
	leave it as an InsertChar command to maximize portability of the macros. */

void optimize_macro(char_stream *cs, int verbose) {
	char *cmd;
	int	pos;
	int	chr;
	int	building = 0;
	 
	if (!cs || !cs->len) return;

	for (pos = 0; pos < cs->len; pos += strlen(&cs->stream[pos]) + 1) {
		cmd = &cs->stream[pos];
		chr = insertchar_val(cmd);
		if (chr < 0x80 && isprint(chr)) {
			char two[2] = " ";
			two[0] = chr;
			delete_from_stream(cs, pos, strlen(cmd) + 1);
			if (building) {
				building++;
				insert_in_stream(cs, two, building, 1);
			}
			else {
				const char *insert;
				int	len;
				insert = verbose ? INSERTSTRING_NAME : INSERTSTRING_ABBREV;
				len	 = strlen(insert);
				insert_in_stream(cs, "\"",	pos, 2);	/* Closing quote */
				insert_in_stream(cs, two,	 pos, 1);		/* The character itself */
				insert_in_stream(cs, " \"",  pos, 2);	/* space and openning quote */
				insert_in_stream(cs, insert, pos, len);	/* The command itself */
				building = pos + len + 2;						/* This is where the char is now */
			}
		}
		else {
			building = 0;
		}
	}
}
  

/* This function is the ultimate goal of this file. It plays a character
	stream, considering each line as a command line. It polls the global stop
	variable in order to check for the user interrupting. Note that the macro is
	duplicated before execution: this is absolutely necessary, for otherwise a
	call to CloseDoc, Record or UnloadMacros could free() the block of memory
	which we are executing. */

int play_macro(buffer *b, char_stream *cs) {

	int error = OK, len;
	unsigned char *p, *stream;

	if (!cs) return ERROR;

	/* If len is 0 or 1, the character stream does not contain anything. */

	if ((len = cs->len) < 2) return OK;

	if (!(p = stream = malloc(len))) return OUT_OF_MEMORY;

	memcpy(stream, cs->stream, len);

	stop = FALSE;

	while(!stop && p - stream < len) {	
#ifdef NE_TEST
		fprintf(stderr, "%s\n", p); /* During tests, we output to stderr the current command. */
#endif

		if (error = execute_command_line(b, p))
#ifndef NE_TEST
			break /* During tests, we never interrupt a macro. */
#endif
				;

#ifdef NE_TEST
		refresh_window(cur_buffer);
		draw_status_bar();
#endif
		p += strlen(p) + 1;
	}

	free(stream);

	return stop ? STOPPED : error;
}



/* Loads a macro, and puts it in the global macro hash table.  file_part is
	applied to the name argument before storing it and hashing it.  Note that if
	the macro can't be opened, we retry prefixing its name with the preferences
	directory name (~/.ne/). Thus, for instance, all autopreferences file whose
	name does not conflict with internal commands can be executed transparently
	just by typing their name. */


macro_desc *load_macro(const char *name) {

	int h;
	char *macro_dir, *prefs_dir;
	char_stream *cs;
	macro_desc *md, **m;

	assert(name != NULL);

	if (!(md = alloc_macro_desc())) return NULL;

	cs = load_stream(md->cs, name, FALSE, FALSE);

	if (!cs && (prefs_dir = exists_prefs_dir()) && (macro_dir = malloc(strlen(prefs_dir) + 2 + strlen(name)))) {
		strcat(strcpy(macro_dir, prefs_dir), name);
		cs = load_stream(md->cs, macro_dir, FALSE, FALSE);
		free(macro_dir);
	}

	if (!cs && (prefs_dir = exists_gprefs_dir()) && (macro_dir = malloc(strlen(prefs_dir) + 2 + strlen(name)))) {
		strcat(strcpy(macro_dir, prefs_dir), name);
		cs = load_stream(md->cs, macro_dir, FALSE, FALSE);
		free(macro_dir);
	}

	if (cs) {
		md->cs = cs;
		md->name = str_dup(file_part(name));

		h = hash_macro(md->name, strlen(md->name));

		m = &macro_hash_table[h];

		while(*m) m = &((*m)->next);

		*m = md;

		return md;
	}

	free_macro_desc(md);
	return NULL;
}



/* Executes a named macro. If the macro is not in the global
macro list, it is loaded. A standard error code is returned. */

int execute_macro(buffer *b, const char *name) {

	const char *p;
	macro_desc *md;
	int h;

	p = file_part(name);

	h = hash_macro(p, strlen(p));

	md = macro_hash_table[h];

	while(md && cmdcmp(md->name, p)) md = md->next;

	if (!md) md = load_macro(name);

	assert_macro_desc(md);

	if (md) return play_macro(b, md->cs);

	return CANT_OPEN_MACRO;
}


/* Clears up the macro table. */

void unload_macros(void) {

	int i;
	macro_desc *m, *n;

	for(i = 0; i < MACRO_HASH_TABLE_SIZE; i++) {

		m = macro_hash_table[i];
		macro_hash_table[i] = NULL;

		while(m) {
			n = m->next;
			free_macro_desc(m);
			m = n;
		}
	}
}



/* This function helps. The help text relative to the command name pointed to
	by p is displayed (p can also contain arguments). If p is NULL, the
	alphabetically ordered list of commands is displayed with the string
	requester. The help finishes when the user escapes. */

void help(char *p) {

	action a;
	int j, i = 0;
	D(fprintf(stderr,"Help Called with parm %ld.\n",p);)
	do {
		print_message("Help: select Command and press Enter, or F1 or Escape or Escape-Escape");
		if (p || (i = request_strings(command_names, ACTION_COUNT, i, MAX_COMMAND_WIDTH, FALSE)) >= 0) {
         D(fprintf(stderr,"Help check #2: p=%lx, i=%d\n",p,i);)
			if (p) {
				for(i = 0; i < strlen(p); i++) if (isasciispace((unsigned char)p[i])) break;

				i = hash_cmd(p, i);

				if ((a = hash_table[i]) && !cmdcmp(commands[--a].name, p)
				|| (a = short_hash_table[i]) && !cmdcmp(commands[--a].short_name, p)) i = a;
				else i = -1;

				p = NULL;
			}
			else {
				D(fprintf(stderr,"Gonna parse_command_line(\"%s\",NULL,NULL,FALSE);\n",command_names[i]);)
				i = parse_command_line(command_names[i], NULL, NULL, FALSE);
				D(fprintf(stderr,"...and got i=%d\n",i);)
			}
			
			if (i < 0) {
				i = 0;
				continue;
			}

			assert(i >= 0 && i < ACTION_COUNT);

			print_message("Help: press Enter, or F1 or Escape or Escape-Escape");
			if ((j = request_strings(commands[i].help, commands[i].help_len, 0, ne_columns, FALSE)) < 0 ) i = j;
		}
	} while(i >= 0);
	draw_status_bar();
}