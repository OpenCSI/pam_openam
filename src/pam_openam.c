#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <security/_pam_macros.h>
#include <assert.h>
#include "commons.h"
#include "pam_openam.h"

#define PAM_SM_AUTH

static int
obtain_authtok(pam_handle_t *pamh){
    char *resp;
    const void *item;
    int retval;
    // retval = pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &resp, _("Password: "));

//    if (retval != PAM_SUCCESS)
//    return retval;

    if (resp == NULL)
    return PAM_CONV_ERR;

    /* set the auth token */
    retval = pam_set_item(pamh, PAM_AUTHTOK, resp);

    /* clean it up */
    _pam_overwrite(resp);
    _pam_drop(resp);

    if ( (retval != PAM_SUCCESS) ||
     (retval = pam_get_item(pamh, PAM_AUTHTOK, &item))
     != PAM_SUCCESS ) {
    return retval;
    }

    return retval;
}

PAM_EXTERN int
pam_sm_authenticate (pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    const char *login = NULL;
    const void *token = NULL;
    int retval = PAM_AUTH_ERR, ctrl;

    retval = pam_get_user(pamh, &login, NULL);
    assert(retval == PAM_SUCCESS);

    if ((ctrl & PAM_USE_FPASS_ARG) == 0 && (ctrl & PAM_TRY_FPASS_ARG) == 0) {
       /* Converse to obtain a password */
       retval = obtain_authtok(pamh);
       if (retval != PAM_SUCCESS) {
       pam_syslog(pamh, LOG_ERR, "can not obtain password from user");
       return retval;
       }
    }

    printf("login: %s, password: %s\n", login, (char *) token);
}
