/******************************************************************************/
/*       l o g g e r   s t a n d a r d   m e s s a g e   c a t a l o g        */
/******************************************************************************/

#ifndef _LG_STD_CAT_
#define _LG_STD_CAT_

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
#define     LSTD_PRG_START           100
#define LEV_LSTD_PRG_START           SYS
#define TXT_LSTD_PRG_START           "starting %s"

#define     LSTD_PRG_STOP           101
#define LEV_LSTD_PRG_STOP           SYS
#define TXT_LSTD_PRG_STOP           "quiting %s"

#define     LSTD_GEN_SYS            102
#define LEV_LSTD_GEN_SYS            SYS
#define TXT_LSTD_GEN_SYS            "%s aborted"

#define     LSTD_GEN_CRI            103
#define LEV_LSTD_GEN_CRI            CRI
#define TXT_LSTD_GEN_CRI            "%s aborted"

#define     LSTD_GEN_ERR            104 
#define LEV_LSTD_GEN_ERR            ERR
#define TXT_LSTD_GEN_ERR            "%s aborted"

#define     LSTD_OPEN_FILE_FAILED   110
#define LEV_LSTD_OPEN_FILE_FAILED   ERR
#define TXT_LSTD_OPEN_FILE_FAILED   "failed to open file %s"

#define     LSTD_GET_FILE_SIZE      111
#define LEV_LSTD_GET_FILE_SIZE      ERR
#define TXT_LSTD_GET_FILE_SIZE      "can not get file size for %s" 

#define     LSTD_ERR_READING_FILE   112
#define LEV_LSTD_ERR_READING_FILE   ERR
#define TXT_LSTD_ERR_READING_FILE   "error reading file %s" 

#define     LSTD_ERR_BAD_FD         113
#define LEV_LSTD_ERR_BAD_FD         ERR
#define TXT_LSTD_ERR_BAD_FD         "function %s() called with bad file descriptor"

#define     LSTD_OPEN_DIR_FAILED    114
#define LEV_LSTD_OPEN_DIR_FAILED    ERR
#define TXT_LSTD_OPEN_DIR_FAILED    "cannot open directory %s"

#define     LSTD_MEM_ALLOC_ERROR    120
#define LEV_LSTD_MEM_ALLOC_ERROR    CRI
#define TXT_LSTD_MEM_ALLOC_ERROR    "can not allocate memory"

#define     LSTD_FREE_NULL_ERROR    121
#define LEV_LSTD_FREE_NULL_ERROR    ERR
#define TXT_LSTD_FREE_NULL_ERROR    "free call on NULL pointer"

#define     LSTD_GETHOST_BY_NAME_ERROR  122
#define LEV_LSTD_GETHOST_BY_NAME_ERROR  ERR
#define TXT_LSTD_GETHOST_BY_NAME_ERROR  "Cannot determinate IP Adress (gethostbyname) %s"

#define     LSTD_GETENV_ERROR    123
#define LEV_LSTD_GETENV_ERROR    ERR
#define TXT_LSTD_GETENV_ERROR    "Cannot get environment variable %s"

#define     LSTD_GETENV_WARNING   124
#define LEV_LSTD_GETENV_WARNING   WAR
#define TXT_LSTD_GETENV_WARNING   "Cannot get environment variable %s"

#define     LSTD_INI_SYNTAX_ERROR    200
#define LEV_LSTD_INI_SYNTAX_ERROR    ERR
#define TXT_LSTD_INI_SYNTAX_ERROR    "ini file syntax error near %s" 

#define     LSTD_INI_OPEN_TAG_ERROR  201
#define LEV_LSTD_INI_OPEN_TAG_ERROR  ERR
#define TXT_LSTD_INI_OPEN_TAG_ERROR  "missing open tag near %.20s"

#define     LSTD_INI_CLOSE_TAG_ERROR  202
#define LEV_LSTD_INI_CLOSE_TAG_ERROR  ERR
#define TXT_LSTD_INI_CLOSE_TAG_ERROR  "missing close tag %s"

#define     LSTD_INI_DUPLICATED_KEY   203
#define LEV_LSTD_INI_DUPLICATED_KEY   WAR
#define TXT_LSTD_INI_DUPLICATED_KEY   "ini key %s dupplicated"

#define     LSTD_INI_EMPTY_MAIN_FILE   204
#define LEV_LSTD_INI_EMPTY_MAIN_FILE   ERR
#define TXT_LSTD_INI_EMPTY_MAIN_FILE   "main ini file %s is empty"

#define     LSTD_INI_EMPTY_INC_FILE   205
#define LEV_LSTD_INI_EMPTY_INC_FILE   WAR
#define TXT_LSTD_INI_EMPTY_INC_FILE   "include ini file %s is empty"

#define     LSTD_INI_MAX_INCLUDE_LEVEL 206
#define LEV_LSTD_INI_MAX_INCLUDE_LEVEL ERR
#define TXT_LSTD_INI_MAX_INCLUDE_LEVEL "Maximal include level %d reached"

#define     LSTD_INI_MAX_INCLUDE_FILES 207
#define LEV_LSTD_INI_MAX_INCLUDE_FILES ERR
#define TXT_LSTD_INI_MAX_INCLUDE_FILES "Maximal nr. %d of include files reached"

#define     LSTD_INI_TAG_NULL   208
#define LEV_LSTD_INI_TAG_NULL   ERR
#define TXT_LSTD_INI_TAG_NULL   "ini tag is NULL"

#define     LSTD_INI_SEARCH_STR_CNT_ERR 209
#define LEV_LSTD_INI_SEARCH_STR_CNT_ERR ERR
#define TXT_LSTD_INI_SEARCH_STR_CNT_ERR "Wrong nr. of search string <tag><key><value>"

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/

#endif
