#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>

#include <string.h>

#include "pam_openam.h"
#include "commons.h"

/* These #defines must be present according to PAM documentation. */
#define PAM_SM_AUTH

#include <security/pam_appl.h>
#include <security/pam_modules.h>

#ifndef PAM_EXTERN
#ifdef PAM_STATIC
#define PAM_EXTERN static
#else
#define PAM_EXTERN extern
#endif
#endif

struct cfg 
{
  int debug;
  int try_first_pass;
  int use_first_pass;
  char *openam_url;
};

#ifdef DBG
#undef DBG
#endif
#define DBG(x) if (cfg->debug) { D(x); }

#if HAVE_CR
static int
display_error(pam_handle_t *pamh, char *message) {
  struct pam_conv *conv;
  struct pam_message *pmsg[1], msg[1];
  struct pam_response *resp = NULL;
  int retval;

  retval = pam_get_item (pamh, PAM_CONV, (const void **) &conv);
  if (retval != PAM_SUCCESS) {
    D(("get conv returned error: %s", pam_strerror (pamh, retval)));
    return retval;
  }

  pmsg[0] = &msg[0];
  msg[0].msg = message;
  msg[0].msg_style = PAM_ERROR_MSG;
  retval = conv->conv(1, (const struct pam_message **) pmsg,
		      &resp, conv->appdata_ptr);

  if (retval != PAM_SUCCESS) {
    D(("conv returned error: %s", pam_strerror (pamh, retval)));
    return retval;
  }

  D(("conv returned: '%s'", resp->resp));
  return retval;
}
#endif /* HAVE_CR */

static void
parse_cfg (int flags, int argc, const char **argv, struct cfg *cfg)
{
  int i;

  memset (cfg, 0, sizeof(struct cfg));

  for (i = 0; i < argc; i++)
    {
      if (strcmp (argv[i], "debug") == 0)
	cfg->debug = 1;
      if (strcmp (argv[i], "try_first_pass") == 0)
	cfg->try_first_pass = 1;
      if (strcmp (argv[i], "use_first_pass") == 0)
	cfg->use_first_pass = 1;
      if (strncmp (argv[i], "openam_url=", 11) == 0)
	cfg->openam_url = (char *) argv[i] + 11; 
    }

  if (cfg->debug)
    {
      D (("called."));
      D (("openam_url=%s", cfg->openam_url ? cfg->openam_url : "(null)"));
      D (("try_first_pass=%d", cfg->try_first_pass));
      D (("use_first_pass=%d", cfg->use_first_pass));
    }
}

PAM_EXTERN int
pam_sm_authenticate (pam_handle_t * pamh,
		     int flags, int argc, const char **argv)
{
  int retval, rc;
  const char *user = NULL;
  const char *password = NULL;
  int valid_token = 0;
  struct pam_conv *conv;
  struct pam_message *pmsg[1], msg[1];
  struct pam_response *resp;
  int nargs = 1;
  struct cfg cfg_st;
  struct cfg *cfg = &cfg_st; /* for DBG macro */
  retval = PAM_SUCCESS;

  parse_cfg (flags, argc, argv, cfg);

  retval = pam_get_user (pamh, &user, NULL);
  if (retval != PAM_SUCCESS)
    {
      DBG (("get user returned error: %s", pam_strerror (pamh, retval)));
      goto done;
    }
  DBG (("get user returned: %s", user));


  if (cfg->try_first_pass || cfg->use_first_pass)
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

  if (cfg->use_first_pass && password == NULL)
    {
      DBG (("use_first_pass set and no password, giving up"));
      retval = PAM_AUTH_ERR;
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
    const char *query_template = "Yubikey for `%s': ";
    size_t len = strlen (query_template) + strlen (user);
    int wrote;

    msg[0].msg = malloc (len);
    if (!msg[0].msg)
      {
        retval = PAM_BUF_ERR;
        goto done;
      }

    wrote = snprintf ((char *) msg[0].msg, len, query_template, user);
    if (wrote < 0 || wrote >= len)
      {
        retval = PAM_BUF_ERR;
        goto done;
      }
      }
      msg[0].msg_style = PAM_PROMPT_ECHO_OFF;
      resp = NULL;

      retval = conv->conv (nargs, (const struct pam_message **) pmsg,
               &resp, conv->appdata_ptr);

      free ((char *) msg[0].msg);

      if (retval != PAM_SUCCESS)
    {
      DBG (("conv returned error: %s", pam_strerror (pamh, retval)));
      goto done;
    }

      if (resp->resp == NULL)
    {
      DBG (("conv returned NULL passwd?"));
      retval = PAM_AUTH_ERR;
      goto done;
    }

      DBG (("conv returned %lu bytes", (unsigned long) strlen(resp->resp)));

      password = resp->resp;
    }

    retval = PAM_AUTH_ERR;

    /* Validate token */
    if (validate_token(cfg->openam_url, (char *) user, (char *) password) == 0)
        retval = PAM_SUCCESS;

done:
  DBG (("done. [%s]", pam_strerror (pamh, retval)));

  return retval;
}

PAM_EXTERN int
pam_sm_setcred (pam_handle_t * pamh, int flags, int argc, const char **argv)
{
  return PAM_SUCCESS;
}

#ifdef PAM_STATIC

struct pam_module _pam_openam_modstruct = {
  "pam_openam",
  pam_sm_authenticate,
  pam_sm_setcred,
  NULL,
  NULL,
  NULL,
  NULL
};

#endif
