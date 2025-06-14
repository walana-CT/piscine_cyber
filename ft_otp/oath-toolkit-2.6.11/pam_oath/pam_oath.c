/*
 * pam_oath.c - a PAM module for OATH one-time passwords
 * Copyright (C) 2009-2024 Simon Josefsson
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#include <config.h>

#include "oath.h"
#include "pam_modutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <pwd.h>
#include <unistd.h>

/* Libtool defines PIC for shared objects */
#ifndef PIC
# define PAM_STATIC
#endif

/* These #defines must be present according to PAM documentation. */
#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION
#define PAM_SM_PASSWORD

#ifdef HAVE_SECURITY_PAM_APPL_H
# include <security/pam_appl.h>
#endif
#ifdef HAVE_SECURITY_PAM_MODULES_H
# include <security/pam_modules.h>
#endif

#define D(x) do {							\
    printf ("[%s:%s(%d)] ", __FILE__, __FUNCTION__, __LINE__);		\
    printf x;								\
    printf ("\n");							\
  } while (0)
#define DBG(x) if (cfg.debug) { D(x); }

#ifndef PAM_EXTERN
# ifdef PAM_STATIC
#  define PAM_EXTERN static
# else
#  define PAM_EXTERN extern
# endif
#endif

#define MIN_OTP_LEN 6
#define MAX_OTP_LEN 8

struct cfg
{
  int debug;
  int alwaysok;
  int try_first_pass;
  int use_first_pass;
  char *usersfile;
  unsigned digits;
  unsigned window;
};

static void
parse_cfg (int flags, int argc, const char **argv, struct cfg *cfg)
{
  int i;

  cfg->debug = 0;
  cfg->alwaysok = 0;
  cfg->try_first_pass = 0;
  cfg->use_first_pass = 0;
  cfg->usersfile = NULL;
  cfg->digits = -1;
  cfg->window = 5;

  for (i = 0; i < argc; i++)
    {
      if (strcmp (argv[i], "debug") == 0)
	cfg->debug = 1;
      if (strcmp (argv[i], "alwaysok") == 0)
	cfg->alwaysok = 1;
      if (strcmp (argv[i], "try_first_pass") == 0)
	cfg->try_first_pass = 1;
      if (strcmp (argv[i], "use_first_pass") == 0)
	cfg->use_first_pass = 1;
      if (strncmp (argv[i], "usersfile=", 10) == 0)
	cfg->usersfile = (char *) argv[i] + 10;
      if (strncmp (argv[i], "digits=", 7) == 0)
	cfg->digits = atoi (argv[i] + 7);
      if (strncmp (argv[i], "window=", 7) == 0)
	cfg->window = atoi (argv[i] + 7);
    }

  if (cfg->digits != 6 && cfg->digits != 7 && cfg->digits != 8)
    {
      if (cfg->digits != -1)
	D (("only 6, 7, and 8 OTP lengths are supported: invalid value %d",
	    cfg->digits));
      cfg->digits = 0;
    }

  if (cfg->debug)
    {
      D (("called."));
      D (("flags %d argc %d", flags, argc));
      for (i = 0; i < argc; i++)
	D (("argv[%d]=%s", i, argv[i]));
      D (("debug=%d", cfg->debug));
      D (("alwaysok=%d", cfg->alwaysok));
      D (("try_first_pass=%d", cfg->try_first_pass));
      D (("use_first_pass=%d", cfg->use_first_pass));
      D (("usersfile=%s", cfg->usersfile ? cfg->usersfile : "(null)"));
      D (("digits=%d", cfg->digits));
      D (("window=%d", cfg->window));
    }
}

static int
parse_usersfile_str (pam_handle_t *pamh, const struct cfg *cfg,
		     const char *user, char **usersfile)
{
  int retval = PAM_SUCCESS;
  size_t name_len = 0;
  size_t home_len = 0;
  size_t len = strlen (cfg->usersfile) + 1;
  char *str = NULL;
  char *u = NULL;
  struct passwd *pw = NULL;

  if (*usersfile)
    {
      return PAM_BUF_ERR;
    }

  if (strstr (cfg->usersfile, "${USER}") == NULL
      && strstr (cfg->usersfile, "${HOME}") == NULL)
    {
      *usersfile = strdup (cfg->usersfile);
      if (!*usersfile)
	return PAM_BUF_ERR;
      return PAM_SUCCESS;
    }

  pw = pam_modutil_getpwnam (pamh, user);
  if (!pw)
    {
      return PAM_USER_UNKNOWN;
    }

  /*
     Find occurrences of the placeholder fields to determine
     userfile buffer length
   */
  name_len = strlen (pw->pw_name);
  home_len = strlen (pw->pw_dir);
  str = cfg->usersfile;
  while ((str = strstr (str, "${USER}")))
    {
      len += name_len;
      len -= 7;
      str += 7;
    }
  str = cfg->usersfile;
  while ((str = strstr (str, "${HOME}")))
    {
      len += home_len;
      len -= 7;
      str += 7;
    }

  *usersfile = malloc (len);
  if (!(*usersfile))
    {
      return PAM_BUF_ERR;
    }
  memset (*usersfile, 0, len);

  str = cfg->usersfile;
  u = *usersfile;
  while (*str)
    {
      char *c = strchr (str, '$');
      if (c)
	{
	  /* Copy all preceding characters */
	  const size_t str_len = c - str;
	  memcpy (u, str, str_len);
	  u += str_len;
	  str += str_len;

	  const char *rpl_str = NULL;
	  size_t rpl_len = 0;

	  if (strncmp (str, "${USER}", 7) == 0)
	    {
	      rpl_str = pw->pw_name;
	      rpl_len = name_len;
	      str += 7;
	    }
	  else if (strncmp (str, "${HOME}", 7) == 0)
	    {
	      rpl_str = pw->pw_dir;
	      rpl_len = home_len;
	      str += 7;
	    }
	  else
	    {
	      rpl_str = "$";
	      rpl_len = 1;
	      str += 1;
	    }

	  memcpy (u, rpl_str, rpl_len);
	  u += rpl_len;
	}
      else
	{
	  size_t str_len = strlen (str);
	  memcpy (u, str, str_len);
	  u += str_len;
	  str += str_len;
	}
    }
done:
  if (retval != PAM_SUCCESS)
    {
      free (*usersfile);
      *usersfile = NULL;
    }
  return retval;
}

PAM_EXTERN int
pam_sm_authenticate (pam_handle_t *pamh,
		     int flags, int argc, const char **argv)
{
  int retval, rc;
  const char *user = NULL;
  const char *password = NULL;
  char *usersfile = NULL;
  char otp[MAX_OTP_LEN + 1];
  int password_len = 0;
  struct pam_conv *conv;
  struct pam_message *pmsg[1], msg[1];
  struct pam_response *resp;
  int nargs = 1;
  struct cfg cfg;
  char *query_prompt = NULL;
  char *onlypasswd = strdup ("");	/* empty passwords never match */

  /* this has to be first in this function to avoid that cfg contain
     uninitialized variables. */
  parse_cfg (flags, argc, argv, &cfg);

  if (!onlypasswd)
    {
      retval = PAM_BUF_ERR;
      goto done;
    }

  retval = pam_get_user (pamh, &user, NULL);
  if (retval != PAM_SUCCESS)
    {
      DBG (("get user returned error: %s", pam_strerror (pamh, retval)));
      goto done;
    }
  DBG (("get user returned: %s", user));

  retval = parse_usersfile_str (pamh, &cfg, user, &usersfile);
  if (retval != PAM_SUCCESS)
    {
      DBG (("parse usersfile string returned error: %s",
	    pam_strerror (pamh, retval)));
      goto done;
    }
  DBG (("usersfile is %s", usersfile));

  // quick check to skip unconfigured users before prompting for password
  {
    time_t last_otp;
    otp[0] = '\0';
    rc = oath_authenticate_usersfile (usersfile,
				      user,
				      otp, cfg.window, onlypasswd, &last_otp);

    DBG (("authenticate first pass rc %d (%s: %s) last otp %s", rc,
	  oath_strerror_name (rc) ? oath_strerror_name (rc) : "UNKNOWN",
	  oath_strerror (rc), ctime (&last_otp)));
    if (rc == OATH_UNKNOWN_USER)
      {
	retval = PAM_USER_UNKNOWN;
	goto done;
      }
  }

  if (cfg.try_first_pass || cfg.use_first_pass)
    {
      retval = pam_get_item (pamh, PAM_AUTHTOK, (const void **) &password);
      if (retval != PAM_SUCCESS)
	{
	  DBG (("get password returned error: %s",
		pam_strerror (pamh, retval)));
	  goto done;
	}
      DBG (("get password returned: %s", password));
    }

  if (cfg.use_first_pass && password == NULL)
    {
      DBG (("use_first_pass set and no password, giving up"));
      retval = PAM_AUTH_ERR;
      goto done;
    }

  rc = oath_init ();
  if (rc != OATH_OK)
    {
      DBG (("oath_init() failed (%d)", rc));
      retval = PAM_AUTHINFO_UNAVAIL;
      goto done;
    }

  if (password == NULL)
    {
      retval = pam_get_item (pamh, PAM_CONV, (const void **) &conv);
      if (retval != PAM_SUCCESS)
	{
	  DBG (("get conv returned error: %s", pam_strerror (pamh, retval)));
	  goto done;
	}

      pmsg[0] = &msg[0];
      {
	const char *query_template = "One-time password (OATH) for `%s': ";
	size_t len = strlen (query_template) + strlen (user);
	size_t wrote;

	query_prompt = malloc (len);
	if (!query_prompt)
	  {
	    retval = PAM_BUF_ERR;
	    goto done;
	  }

	wrote = snprintf (query_prompt, len, query_template, user);
	if (wrote < 0 || wrote >= len)
	  {
	    retval = PAM_BUF_ERR;
	    goto done;
	  }

	msg[0].msg = query_prompt;
      }
      msg[0].msg_style = PAM_PROMPT_ECHO_OFF;
      resp = NULL;

      retval = conv->conv (nargs, (const struct pam_message **) pmsg,
			   &resp, conv->appdata_ptr);

      free (query_prompt);
      query_prompt = NULL;

      if (retval != PAM_SUCCESS)
	{
	  DBG (("conv returned error: %s", pam_strerror (pamh, retval)));
	  goto done;
	}

      DBG (("conv returned: %s", resp->resp));

      password = resp->resp;
    }

  if (password)
    password_len = strlen (password);
  else
    {
      DBG (("Could not read password"));
      retval = PAM_AUTH_ERR;
      goto done;
    }

  if (password_len < MIN_OTP_LEN)
    {
      DBG (("OTP too short: %s", password));
      retval = PAM_AUTH_ERR;
      goto done;
    }
  else if (cfg.digits != 0 && password_len < cfg.digits)
    {
      DBG (("OTP shorter than digits=%d: %s", cfg.digits, password));
      retval = PAM_AUTH_ERR;
      goto done;
    }
  else if (cfg.digits == 0 && password_len > MAX_OTP_LEN)
    {
      DBG (("OTP too long (and no digits=): %s", password));
      retval = PAM_AUTH_ERR;
      goto done;
    }
  else if (cfg.digits != 0 && password_len > cfg.digits)
    {
      free (onlypasswd);
      onlypasswd = strdup (password);
      if (!onlypasswd)
	{
	  retval = PAM_BUF_ERR;
	  goto done;
	}

      /* user entered their system password followed by generated OTP? */

      onlypasswd[password_len - cfg.digits] = '\0';

      DBG (("Password: %s ", onlypasswd));

      memcpy (otp, password + password_len - cfg.digits, cfg.digits);
      otp[cfg.digits] = '\0';

      retval = pam_set_item (pamh, PAM_AUTHTOK, onlypasswd);
      if (retval != PAM_SUCCESS)
	{
	  DBG (("set_item returned error: %s", pam_strerror (pamh, retval)));
	  goto done;
	}
    }
  else
    {
      strcpy (otp, password);
      password = NULL;
    }

  DBG (("OTP: %s", otp));

  {
    time_t last_otp;

    rc = oath_authenticate_usersfile (usersfile,
				      user,
				      otp, cfg.window, onlypasswd, &last_otp);
    DBG (("authenticate rc %d (%s: %s) last otp %s", rc,
	  oath_strerror_name (rc) ? oath_strerror_name (rc) : "UNKNOWN",
	  oath_strerror (rc), ctime (&last_otp)));
  }

  if (rc != OATH_OK)
    {
      DBG (("One-time password not authorized to login as user '%s'", user));
      retval = PAM_AUTH_ERR;
      goto done;
    }

  retval = PAM_SUCCESS;

done:
  oath_done ();
  free (usersfile);
  free (query_prompt);
  free (onlypasswd);
  if (cfg.alwaysok && retval != PAM_SUCCESS)
    {
      DBG (("alwaysok needed (otherwise return with %d)", retval));
      retval = PAM_SUCCESS;
    }
  DBG (("done. [%s]", pam_strerror (pamh, retval)));

  return retval;
}

PAM_EXTERN int
pam_sm_setcred (pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return PAM_SUCCESS;
}

#ifdef PAM_STATIC

struct pam_module _pam_oath_modstruct = {
  "pam_oath",
  pam_sm_authenticate,
  pam_sm_setcred,
  NULL,
  NULL,
  NULL,
  NULL
};

#endif
