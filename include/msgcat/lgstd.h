/******************************************************************************/
/*       l o g g e r   s t a n d a r d   m e s s a g e   c a t a l o g        */
/******************************************************************************/

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

#define     LSTD_ERR_READING_FILE   111
#define LEV_LSTD_ERR_READING_FILE   ERR
#define TXT_LSTD_ERR_READING_FILE   "error reading file %s" 

#define     LSTD_MEM_ALLOC_ERROR    120
#define LEV_LSTD_MEM_ALLOC_ERROR    ERR
#define TXT_LSTD_MEM_ALLOC_ERROR    "can not allocate memory"

#define     LSTD_INI_SYNTAX_ERROR    200
#define LEV_LSTD_INI_SYNTAX_ERROR    ERR
#define TXT_LSTD_INI_SYNTAX_ERROR    "ini file syntax error near %s" 

#if(0)
#define     LSTD_INI_MISSING_CHAR   203
#define LEV_LSTD_INI_MISSING_CHAR   ERR
#define TXT_LSTD_INI_MISSING_CHAR   "missing char \'%c\' near %.40s"

#define     LSTD_INI_WRONG_CHAR      204
#define LEV_LSTD_INI_WRONG_CHAR      ERR
#define TXT_LSTD_INI_WRONG_CHAR      "wrong char \'%c\' near %.40s"

#define     LSTD_INI_EARLY_EOF       205
#define LEV_LSTD_INI_EARLY_EOF       ERR
#define TXT_LSTD_INI_EARLY_EOF       "unexpected EOF"

#define     LSTD_INI_OPEN_TAG_ERROR  208
#define LEV_LSTD_INI_OPEN_TAG_ERROR  ERR
#define TXT_LSTD_INI_OPEN_TAG_ERROR  "missing open tag near %.20s"

#define     LSTD_INI_CLOSE_TAG_ERROR  209
#define LEV_LSTD_INI_CLOSE_TAG_ERROR  ERR
#define TXT_LSTD_INI_CLOSE_TAG_ERROR  "missing close tag %s"
#endif

#define     LSTD_INI_DUPLICATED_KEY   209
#define LEV_LSTD_INI_DUPLICATED_KEY   ERR
#define TXT_LSTD_INI_DUPLICATED_KEY   "ini key %s dupplicated"

#define     LSTD_INI_EMPTY_MAIN_FILE   210
#define LEV_LSTD_INI_EMPTY_MAIN_FILE   ERR
#define TXT_LSTD_INI_EMPTY_MAIN_FILE   "main ini file %s is empty"

#define     LSTD_INI_EMPTY_INC_FILE   211
#define LEV_LSTD_INI_EMPTY_INC_FILE   WAR
#define TXT_LSTD_INI_EMPTY_INC_FILE   "include ini file %s is empty"

#define     LSTD_INI_MAX_INCLUDE_LEVEL 212 
#define LEV_LSTD_INI_MAX_INCLUDE_LEVEL ERR
#define TXT_LSTD_INI_MAX_INCLUDE_LEVEL "Maximal include level %d reached"

#if(0)
#define     LSTD_GEN_WAR   116
#define LEV_LSTD_GEN_WAR   WAR
#define TXT_LSTD_GEN_WAR   "a warning has occured"

#define     LSTD_GEN_LOG   117
#define LEV_LSTD_GEN_LOG   LOG
#define TXT_LSTD_GEN_LOG   "a log has occured"

#define     LSTD_GEN_INF   118
#define LEV_LSTD_GEN_INF   INF
#define TXT_LSTD_GEN_INF   "an info has occured"

#endif

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/

