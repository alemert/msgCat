/******************************************************************************/
/*       l o g g e r   s t a n d a r d   m e s s a g e   c a t a l o g        */
/******************************************************************************/

#ifndef _LG_XYM_CAT_
#define _LG_XYM_CAT_

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/
// ---------------------------------------------------------
// system
// ---------------------------------------------------------

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define     LXYM_SEND_INF        2000
#define LEV_LXYM_SEND_INF        INF
#define TXT_LXYM_SEND_INF        "Messae send to Xymon with status %s" 

#define     LXYM_SEND_ERR        2001
#define LEV_LXYM_SEND_ERR        ERR
#define TXT_LXYM_SEND_ERR        "Messae send to Xymon with status %s" 

#define     LXYM_NO_RECIPIENT    2010
#define LEV_LXYM_NO_RECIPIENT    ERR
#define TXT_LXYM_NO_RECIPIENT    "no recipients listed, XMSRV was %s, XMSERVERS was %s"

#define     LXYM_RECIPIENT_LIST  2011
#define LEV_LXYM_RECIPIENT_LIST  DBG
#define TXT_LXYM_RECIPIENT_LIST  "recipient listed as %s"

#define     LXYM_PORT            2012
#define LEV_LXYM_PORT            DBG
#define TXT_LXYM_PORT            "working with xymon port %d"

#define     LXYM_HTTP_HOST       2013
#define LEV_LXYM_HTTP_HOST       DBG
#define TXT_LXYM_HTTP_HOST       "Xymon: HTTP protocol directtly to host %s"

#define     LXYM_HTTP_PROXY      2014
#define LEV_LXYM_HTTP_PROXY      DBG
#define TXT_LXYM_HTTP_PROXY      "Xymon: HTTP protocol via proxy to host %s"

#endif
