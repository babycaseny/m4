/* GNU m4 -- A simple macro processor
   Copyright (C) 1991, 1992, 1993, 1994, 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301  USA
*/

#include <stdio.h>
#include <sys/stat.h>

#include "m4private.h"

static void set_debug_file (m4 *, FILE *);



/* Function to decode the debugging flags OPTS.  Used by main while
   processing option -d, and by the builtin debugmode ().  */
int
m4_debug_decode (m4 *context, int previous, const char *opts)
{
  int level;
  char mode = '\0';

  if (opts == NULL || *opts == '\0')
    level = M4_DEBUG_TRACE_DEFAULT;
  else
    {
      if (*opts == '-' || *opts == '+')
	mode = *opts++;
      for (level = 0; *opts; opts++)
	{
	  switch (*opts)
	    {
	    case 'a':
	      level |= M4_DEBUG_TRACE_ARGS;
	      break;

	    case 'e':
	      level |= M4_DEBUG_TRACE_EXPANSION;
	      break;

	    case 'q':
	      level |= M4_DEBUG_TRACE_QUOTE;
	      break;

	    case 't':
	      level |= M4_DEBUG_TRACE_ALL;
	      break;

	    case 'l':
	      level |= M4_DEBUG_TRACE_LINE;
	      break;

	    case 'f':
	      level |= M4_DEBUG_TRACE_FILE;
	      break;

	    case 'p':
	      level |= M4_DEBUG_TRACE_PATH;
	      break;

	    case 'c':
	      level |= M4_DEBUG_TRACE_CALL;
	      break;

	    case 'i':
	      level |= M4_DEBUG_TRACE_INPUT;
	      break;

	    case 'x':
	      level |= M4_DEBUG_TRACE_CALLID;
	      break;

	    case 'V':
	      level |= M4_DEBUG_TRACE_VERBOSE;
	      break;

	    default:
	      return -1;
	    }
	}
    }

  /* This is to avoid screwing up the trace output due to changes in the
     debug_level.  */

  obstack_free (&context->trace_messages,
		obstack_finish (&context->trace_messages));

  switch (mode)
    {
    case '\0':
      /* Replace old level.  */
      break;

    case '-':
      /* Subtract flags.  */
      level = previous & ~level;
      break;

    case '+':
      /* Add flags.  */
      level |= previous;
      break;

    default:
      assert (!"INTERNAL ERROR: impossible mode from flags");
    }

  return level;
}

/* Change the debug output stream to FP.  If the underlying file is the
   same as stdout, use stdout instead so that debug messages appear in the
   correct relative position.  */
static void
set_debug_file (m4 *context, FILE *fp)
{
  FILE *debug_file;
  struct stat stdout_stat, debug_stat;

  assert (context);

  debug_file = m4_get_debug_file (context);

  if (debug_file != NULL && debug_file != stderr && debug_file != stdout)
    fclose (debug_file);
  m4_set_debug_file (context, fp);

  if (debug_file != NULL && debug_file != stdout)
    {
      if (fstat (fileno (stdout), &stdout_stat) < 0)
	return;
      if (fstat (fileno (debug_file), &debug_stat) < 0)
	return;

      if (stdout_stat.st_ino == debug_stat.st_ino
	  && stdout_stat.st_dev == debug_stat.st_dev)
	{
	  if (debug_file != stderr)
	    fclose (debug_file);
	  m4_set_debug_file (context, stdout);
	}
    }
}

/* Change the debug output to file NAME.  If NAME is NULL, debug output is
   reverted to stderr, and if empty debug output is discarded.  Return true
   iff the output stream was changed.  */
bool
m4_debug_set_output (m4 *context, const char *name)
{
  FILE *fp;

  assert (context);

  if (name == NULL)
    set_debug_file (context, stderr);
  else if (*name == '\0')
    set_debug_file (context, NULL);
  else
    {
      fp = fopen (name, "a");
      if (fp == NULL)
	return false;

      if (set_cloexec_flag (fileno (fp), true) != 0)
	m4_error (context, 0, errno,
		  _("cannot protect debug file across forks"));
      set_debug_file (context, fp);
    }
  return true;
}

/* Print the header of a one-line debug message, starting with "m4debug:".  */
void
m4_debug_message_prefix (m4 *context)
{
  FILE *debug_file;

  assert (context);

  debug_file = m4_get_debug_file (context);
  fprintf (debug_file, "m4debug:");
  if (m4_get_current_line (context))
    {
      if (m4_is_debug_bit (context, M4_DEBUG_TRACE_FILE))
	fprintf (debug_file, "%s:", m4_get_current_file (context));
      if (m4_is_debug_bit (context, M4_DEBUG_TRACE_LINE))
	fprintf (debug_file, "%d:", m4_get_current_line (context));
    }
  putc (' ', debug_file);
}
