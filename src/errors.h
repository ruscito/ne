/* Error index enum, and extern declaration of the error vector.

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


/* Here we define the table of error messages. These defines are used
whenever a function wants to return a specific error to the caller, and
wants also to specify that an error message has to be printed. The generic
error code which just says "something went wrong" is -1. 0 represents
success. Any positive value is one of these enum items, and can be used to
index the error message table. Whenever this enum is updated, the
corresponding vector in errors.c *must* be updated too. */

#ifdef OK
#undef OK
#endif

#ifdef ERROR
#undef ERROR
#endif

#ifdef ABORT
#undef ABORT
#endif

#define ABORT	(-2)
#define ERROR	(-1)
#define OK		(0)

enum error {
	SYNTAX_ERROR = 1,
	NOT_FOUND,
	CANT_SAVE_EXIT_SUSPENDED,
	NOT_ON_A_BRACKET,
	CANT_FIND_BRACKET,
	BOOKMARK_NOT_SET,
	BOOKMARK_OUT_OF_RANGE,
	CANT_INSERT_0,
	NO_SEARCH_STRING,
	NO_REPLACE_STRING,
	TAB_SIZE_OUT_OF_RANGE,
	MARK_BLOCK_FIRST,
	OUT_OF_MEMORY,
	NOTHING_TO_UNDO,
	NOTHING_TO_REDO,
	UNDO_NOT_ENABLED,
	NO_SUCH_COMMAND,
	CAN_EXECUTE_ONLY_OPTIONS,
	HAS_NUMERIC_ARGUMENT,
	HAS_NO_ARGUMENT,
	REQUIRES_ARGUMENT,
	WRONG_CHAR_AFTER_BACKSLASH,
	CANT_OPEN_FILE,
	CANT_OPEN_TEMPORARY_FILE,
	ERROR_WHILE_WRITING,
	HAS_NO_EXTENSION,
	CANT_FIND_PREFS_DIR,
	CLIP_DOESNT_EXIST,
	MARK_OUT_OF_BUFFER,
	CANT_OPEN_MACRO,
	FILE_IS_READ_ONLY,
	FILE_IS_MIGRATED,
	FILE_IS_DIRECTORY,
	STOPPED,
	IO_ERROR,
	STRING_IS_EMPTY,
	EXTERNAL_COMMAND_ERROR,
	ESCAPE_TIME_OUT_OF_RANGE,
	PREFS_STACK_FULL,
	PREFS_STACK_EMPTY,
	NOT_A_NUMBER,
	INVALID_CHARACTER,
	INVALID_STRING,
	BUFFER_IS_NOT_UTF8,
	INCOMPATIBLE_CLIP_ENCODING,
	INCOMPATIBLE_COMMAND_ENCODING,
	INCOMPATIBLE_SEARCH_STRING_ENCODING,
	INCOMPATIBLE_REPLACE_STRING_ENCODING,
	UTF8_REGEXP_CHARACTER_CLASS_NOT_SUPPORTED,
	UTF8_REGEXP_COMP_CHARACTER_CLASS_NOT_SUPPORTED,
	GROUP_NOT_AVAILABLE,
	SYNTAX_NOT_ENABLED,
	NO_SYNTAX_FOR_EXT,

	ERROR_COUNT
};

enum info {
	SAVING,
	SAVED,
	PRESSF1,
	BLOCK_START_MARKED,
	STARTING_MACRO_RECORDING,
	MACRO_RECORDING_COMPLETED,
	INFO_COUNT
};

extern char *error_msg[ERROR_COUNT];
extern char *info_msg[INFO_COUNT];