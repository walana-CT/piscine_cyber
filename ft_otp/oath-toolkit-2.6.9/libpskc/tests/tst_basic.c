/*
 * tst_basic.c - self-tests for libpskc basic functions
 * Copyright (C) 2012-2023 Simon Josefsson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <config.h>

#include <pskc/pskc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
my_log (const char *msg)
{
  if (msg == NULL)
    {
      printf ("log got NULL msg?\n");
      exit (EXIT_FAILURE);
    }
}

int
main (void)
{
  pskc_rc rc;
  int i;

  /* Check version. */

  if (!pskc_check_version (PSKC_VERSION))
    {
      printf ("pskc_check_version (%s) failed [%s]\n", PSKC_VERSION,
	      pskc_check_version (NULL));
      return 1;
    }

  if (pskc_check_version (NULL) == NULL)
    {
      printf ("pskc_check_version (NULL) == NULL\n");
      return 1;
    }

  if (pskc_check_version ("999.999"))
    {
      printf ("pskc_check_version (999.999) succeeded?!\n");
      return 1;
    }

  if (!pskc_check_version ("0"))
    {
      printf ("pskc_check_version (0) didn't fail?!\n");
      return 1;
    }

  if (PSKC_VERSION_NUMBER < 0x02060300 || PSKC_VERSION_NUMBER >= 0x03000000)
    {
      printf ("PSKC_VERSION_NUMBER out of range?!\n");
      return 1;
    }

  /* Test initialization. */

  rc = pskc_global_init ();
  if (rc != PSKC_OK)
    {
      printf ("pskc_global_init: %d\n", rc);
      return 1;
    }

  rc = pskc_global_init ();
  if (rc != PSKC_OK)
    {
      printf ("pskc_global_init: %d\n", rc);
      return 1;
    }

  rc = pskc_global_init ();
  if (rc != PSKC_OK)
    {
      printf ("pskc_global_init: %d\n", rc);
      return 1;
    }

  /* Test deinitialization. */

  pskc_global_done ();
  pskc_global_done ();
  pskc_global_done ();
  pskc_global_done ();
  pskc_global_done ();

  rc = pskc_output (NULL, 42, NULL, NULL);
  if (rc != PSKC_UNKNOWN_OUTPUT_FORMAT)
    {
      printf ("pskc_output: %d\n", rc);
      return 1;
    }

  pskc_global_log (my_log);

  /* Test enums. */

  for (i = 0; i <= PSKC_PINUSAGEMODE_LAST; i++)
    {
      pskc_pinusagemode m;
      const char *str;

      str = pskc_pinusagemode2str (i);
      if (str == NULL)
	{
	  printf ("pskc_pinusagemode2str(%d) == NULL\n", i);
	  return 1;
	}

      m = pskc_str2pinusagemode (str);
      if (m != i)
	{
	  printf ("pskc_str2pinusagemode(%s/%d) = %d\n", str, i, m);
	  return 1;
	}
    }

  for (i = 0; i <= PSKC_VALUEFORMAT_LAST; i++)
    {
      pskc_valueformat m;
      const char *str;

      str = pskc_valueformat2str (i);
      if (str == NULL)
	{
	  printf ("pskc_valueformat2str(%d) == NULL\n", i);
	  return 1;
	}

      m = pskc_str2valueformat (str);
      if (m != i)
	{
	  printf ("pskc_str2valueformat(%s/%d) = %d\n", str, i, m);
	  return 1;
	}
    }

  for (i = 1; i <= PSKC_KEYUSAGE_LAST; i = i << 1)
    {
      pskc_keyusage m;
      const char *str;

      str = pskc_keyusage2str (i);
      if (str == NULL)
	{
	  printf ("pskc_keyusage2str(%d) == NULL\n", i);
	  return 1;
	}

      m = pskc_str2keyusage (str);
      if (m != i)
	{
	  printf ("pskc_str2keyusage(%d/%s) = %d\n", i, str, m);
	  return 1;
	}
    }

  {
    const char *str = pskc_keyusage2str (42);
    if (strcmp (str, "Unknown") != 0)
      {
	printf ("pskc_keyusage2str(42) == %s\n", str);
	return 1;
      }

    if (pskc_str2keyusage ("foobar") != PSKC_KEYUSAGE_UNKNOWN)
      {
	printf ("pskc_str2keyusage (\"foobar\") == %d\n",
		pskc_str2keyusage ("foobar"));
	return 1;
      }
  }

  return 0;
}
