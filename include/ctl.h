/******************************************************************************/
/*                                                                            */
/*           B A S I C S   F O R   M E S S A G E   C A T A L L O G            */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <stdio.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
// ---------------------------------------------------------
// logging levels
// ---------------------------------------------------------
#define FLW  10            // flow
#define DBG   9            // debugger (dumps)
#define INF   8            // information (incl source lines)
#define LOG   7            // log (without source lines)
#define WAR   6            // warning
#define ERR   5            // error
#define MLT   4            // multiline output enabled
#define CRI   1            // critical (abort program)
#define SYS   0            // system (base program information)
#define NOK (-1)           // Unknown level

#define MIN_LOG_LEVEL 2    // every level less then this 
                           //  will be logged in any case
// ---------------------------------------------------------
// log defaults
// ---------------------------------------------------------
#define DEFAULT_LOG_LEVEL      ERR
#define LOG_BUFFER_LINE_SIZE   256
//#define LOG_BUFFER_CACHE_SIZE  512
#define LOG_BUFFER_CACHE_SIZE  64

#define DMP_ITEM_LEN           100

// ---------------------------------------------------------
// reserved id's
// ---------------------------------------------------------
#define     LSYS_FUNC_ENTRY     1
#define LEV_LSYS_FUNC_ENTRY   FLW
#define TXT_LSYS_FUNC_ENTRY   " "

#define     LSYS_FUNC_EXIT      2
#define LEV_LSYS_FUNC_EXIT    FLW
#define TXT_LSYS_FUNC_EXIT    " "

#define     LSYS_DUMP_START    5
#define LEV_LSYS_DUMP_START   DBG
#define TXT_LSYS_DUMP_START   "Memory dump start for %s"

#define     LSYS_DUMP_END      6
#define LEV_LSYS_DUMP_END     DBG
#define TXT_LSYS_DUMP_END     "Memory dump end for %s"

#define     LSYS_MULTILINE_START      7
#define LEV_LSYS_MULTILINE_START     MLT
#define TXT_LSYS_MULTILINE_START     "BEGIN output for %s"

#define     LSYS_MULTILINE_END      8
#define LEV_LSYS_MULTILINE_END     MLT
#define TXT_LSYS_MULTILINE_END     "END output for %s"

#define     LSYS_MULTILINE_ADD      9
#define LEV_LSYS_MULTILINE_ADD     MLT
#define TXT_LSYS_MULTILINE_ADD     "END output for %s"

/******************************************************************************/
/*   G L O B A L E S                                                          */
/******************************************************************************/
#ifdef C_MODULE_LOGGER_CATALOG

  char _gLoggerBuffer[LOG_BUFFER_LINE_SIZE] ;

  // -------------------------------------------------------
  // global circular buffer for dumping cashed messages in case of CRI errors
  // -------------------------------------------------------
  static char _gBufferCache[LOG_BUFFER_CACHE_SIZE+1]// circular cache for last
                           [LOG_BUFFER_LINE_SIZE+1];// LOG_BUFFER_CACHE_SIZE msg
  static int  _gBufferCacheIndex = 0 ;              // actual line index for
                                                    // circular cache
#else

  extern char _gLoggerBuffer[] ;

#endif

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/
#ifdef __SUNPRO_C                                 // define gnu macro 
                                                  //   __FUNCTION__ 
#define __FUNCTION__ __func__                     //   for sun compiler
#pragma error_messages (off,E_ARGUEMENT_MISMATCH) // disable warning:
                                                  //   argument mismatch
#endif                                            //   for this macro only

#define logger( id, ... )                         \
  snprintf( _gLoggerBuffer, LOG_BUFFER_LINE_SIZE, \
                            TXT_##id,             \
                            ##__VA_ARGS__ );      \
  loggerFunc( __LINE__     ,                      \
              __FILE__     ,                      \
              __FUNCTION__ ,                      \
              id           ,                      \
              LEV_##id     ,                      \
              _gLoggerBuffer )

#define logFuncCall( ) logger( LSYS_FUNC_ENTRY ) 
#define logFuncExit( ) logger( LSYS_FUNC_EXIT  ) 

#define dumper( offset, comment, msg )   \
{                                        \
  logger(   LSYS_DUMP_START, comment ) ; \
  dumpFunc( offset         , msg     ) ; \
  logger(   LSYS_DUMP_END  , comment ) ; \
}

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
int loggerFunc( const int   line  ,
                const char* file  ,
                const char* func  ,
                      int   id    ,
                      int   lev   ,
                      char* msg ) ;

int dumpFunc( char* _offset, char _msg[][DMP_ITEM_LEN] ) ;

#if( 0 )
void setMaxLogLevel( int maxLevel ) ;
int  setLogFileName( const char* name ) ;
#endif

int initLogging( const char* logName, int logLevel ) ;
FILE* getLogFP() ;
const char* textornull( char *text ) ;
int logStr2lev( const char *str );
