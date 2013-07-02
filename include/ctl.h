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
#define FLW   9     // flow
#define INF   8
#define LOG   7
#define WAR   6
#define ERR   5
#define CRI   1
#define SYS   0

#define MIN_LOG_LEVEL 2    // every level less then this 
                           //  will be logged in any case
// ---------------------------------------------------------
// log defaults
// ---------------------------------------------------------
#define DEFAULT_LOG_LEVEL      ERR
#define LOG_BUFFER_LINE_SIZE   256
#define LOG_BUFFER_CACHE_SIZE   32

// ---------------------------------------------------------
// reserved id's
// ---------------------------------------------------------
#define     LSYS_FUNC_ENTRY      1
#define LEV_LSYS_FUNC_ENTRY      FLW
#define TXT_LSYS_FUNC_ENTRY " "

#define     LSYS_FUNC_EXIT      2
#define LEV_LSYS_FUNC_EXIT      FLW
#define TXT_LSYS_FUNC_EXIT " "

#define     LSYS_FLOW_DUMP      99

/******************************************************************************/
/*   G L O B A L E S                                                          */
/******************************************************************************/
#ifdef C_MODULE_LOGGER_CATALOG

  char _gLoggerBuffer[LOG_BUFFER_LINE_SIZE] ;

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

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
int loggerFunc( const int   line,
                const char* file,
                const char* func,
                      int   id,
                      int   lev,
                      char* msg) ;
#if( 0 )
void setMaxLogLevel( int maxLevel ) ;
int  setLogFileName( const char* name ) ;
#endif
int initLogging( const char* logName, int logLevel ) ;
