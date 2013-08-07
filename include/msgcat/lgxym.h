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
#define     LXYM_NO_RECIPIENT  2000
#define LEV_LXYM_NO_RECIPIENT  ERR
#define TXT_LXYM_NO_RECIPIENT  "No recipient for message"

#define     LXYM_SEND_INF  2001
#define LEV_LXYM_SEND_INF  INF
#define TXT_LXYM_SEND_INF  "Messae send to Xymon with status %s" 

#define     LXYM_SEND_ERR  2002
#define LEV_LXYM_SEND_ERR  ERR
#define TXT_LXYM_SEND_ERR  "Messae send to Xymon with status %s" 

#define     LXYM_NO_RECIPIENT  2005
#define LEV_LXYM_NO_RECIPIENT  ERR
#define TXT_LXYM_NO_RECIPIENT  "no recipients listed, XMSRV was %s, XMSERVERS was %s"

#endif
