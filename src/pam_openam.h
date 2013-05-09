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

#endif /* PAM_OPENAM */
