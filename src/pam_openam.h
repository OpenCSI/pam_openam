#ifndef PAM_OPENAM
#define PAM_OPENAM
/* $Id: pam_userdb.h,v 1.4 2005/09/18 13:04:57 kukuk Exp $ */
   
/* Header files */
#include <security/pam_appl.h>

/* argument parsing */
#define PAM_DEBUG_ARG		0x0001
#define PAM_ICASE_ARG		0x0002
#define PAM_DUMP_ARG		0x0004
#define PAM_UNKNOWN_OK_ARG	0x0010
#define PAM_KEY_ONLY_ARG	0x0020
#define PAM_USE_FPASS_ARG	0x0040
#define PAM_TRY_FPASS_ARG	0x0080

#ifndef MODULE_NAME
#define MODULE_NAME	"pam_openam"
#endif /* MODULE_NAME */

#define DEBUG_PAM

#if defined(DEBUG_PAM)
# if defined(HAVE_SECURITY__PAM_MACROS_H)
#  define DEBUG
#  include <security/_pam_macros.h>
# else
#  define D(x) do {                         \
    printf ("debug: %s:%d (%s): ", __FILE__, __LINE__, __FUNCTION__);   \
    printf x;                               \
    printf ("\n");                          \
  } while (0)
# endif /* HAVE_SECURITY__PAM_MACROS_H */
#else
# define D(x)
#endif /* DEBUG_PAM */

#endif /* PAM_OPENAM */
