/******************************************************************************/
/*                                                                            */
/*           B A S I C S   F O R   M E S S A G E   C A T A L L O G            */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*                                                                            */
/*  functions :                                                               */
/*   - loggerFunc                                                             */
/*   - setMaxLogLevel                                                         */
/*   - setLogFileName                                                         */
/*   - getLogTime                                                             */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
// ---------------------------------------------------------
// system include
// ---------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#define C_MODULE_LOGGER_CATALOG
#include "ctl.h"

/******************************************************************************/
/*   G L O B A L S                                                            */
/******************************************************************************/

char *_gLoggerLevel[] = { [INF]="INF",
                          [LOG]="LOG",
                          [WAR]="WAR",
                          [ERR]="ERR",
                          [CRI]="CRI",
                          [SYS]="SYS"
                        } ;

int _gMaxLevel = DEFAULT_LOG_LEVEL ;

char _gLogFileName[256] ;

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define TIME_STR_LNG 32
#define SPACE_OFFSET "                                    "
#define MARKER_OFFSET "= = = = = = = = = = = = = = ="

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/


/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
void getLogTime( char *timeStr ) ;

/******************************************************************************/
/*                                                                            */
/*   F U N C T I O N S                                                        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*  logger function                                                           */
/*                                                                            */
/*  name:                                                                     */
/*    loggerFunc                                                              */
/*  attributes:                                                               */
/*    line : nr of the source line (set in logger macro)                      */
/*    file : source file name      (set in logger macro)                      */
/*    func : function name         (set in logger macro)                      */
/*    id   : log message id        (passed through logger macro)              */
/*    lev  : log message level     (set by pragma in logger macro)            */
/*    msg  : message               (set by sprintf in logger macro)           */
/*  description:                                                              */
/*    formted output to log file. loggerFunc can only be called from          */
/*    logger macro (ctl.h), all attributes except >id< are produced in        */
/*    logger macro                                                            */
/*  return code:                                                              */
/*    return code is opened for future purposes, at the moment loggerFunc     */
/*    always returns 0                                                        */
/*                                                                            */
/******************************************************************************/
int loggerFunc( const int   line,
                const char* file,
                const char* func,
                      int   id,
                      int   lev,
                      char* msg)
{
  char lineBuffer[LOG_BUFFER_LINE_SIZE] ;           // local line buffer
  char dbgBuffer[LOG_BUFFER_LINE_SIZE]  ;           // local line buffer(detail)
  char timeStr[TIME_STR_LNG] ;                      // buffer for time
                                                    // index for actual line
  static int  _sBufferCacheIndex = 0 ;              //  in circular cache
  static char _sBufferCache [LOG_BUFFER_CACHE_SIZE] // cashe for buffering last
                            [LOG_BUFFER_LINE_SIZE]; // LOG_BUFFER_CACHE_SIZE
                                                    // lines
  static int pid = 0 ;                              // pid of the process

  // -------------------------------------------------------
  // get pid only once
  // -------------------------------------------------------
  if( pid == 0 )
  {
    pid = (int) getpid() ;
  }

  // -------------------------------------------------------
  // get actual time
  // -------------------------------------------------------
  getLogTime( timeStr ) ;

  // -------------------------------------------------------
  // fill line buffer
  // -------------------------------------------------------
  snprintf( lineBuffer, LOG_BUFFER_LINE_SIZE  ,
                        "%s %6d %05d %s %s\n" ,
                        timeStr               ,
                        pid                   ,
                        id                    ,
                        _gLoggerLevel[lev]    ,
                        msg )                 ;

  snprintf( dbgBuffer, LOG_BUFFER_LINE_SIZE           ,
                       "%s %s() in %s (line: %05d)\n" ,
                       SPACE_OFFSET                   ,
                       func                           ,
                       file                           ,
                       line                          );

  // -------------------------------------------------------
  // fill logger cache
  // -------------------------------------------------------
  memcpy( _sBufferCache[_sBufferCacheIndex]   ,
          lineBuffer                          ,
          LOG_BUFFER_LINE_SIZE )              ;
  memcpy( _sBufferCache[_sBufferCacheIndex+1] ,
          dbgBuffer                           ,
          LOG_BUFFER_LINE_SIZE )              ;
  _sBufferCacheIndex+=2 ;

  // -------------------------------------------------------
  // log if neccessary
  // -------------------------------------------------------
  if( _gMaxLevel > lev || lev < MIN_LOG_LEVEL )
  {
    printf("%s",lineBuffer) ;
  }

//printf("%d von %d \n",_gMaxLevel,LOG);
  if( _gMaxLevel > LOG )
  {
    printf("%s",dbgBuffer) ;
  }

  // -------------------------------------------------------
  // dump memory chashe if level critical
  // -------------------------------------------------------
  if( lev == CRI )
  {
    int i ;

    printf( MARKER_OFFSET "  begin log cache dump " MARKER_OFFSET "\n") ;
    for( i=_sBufferCacheIndex; i<LOG_BUFFER_CACHE_SIZE; i++ )
    {
      printf( "%s", _sBufferCache[i] ) ;
    }

    for( i=0; i<_sBufferCacheIndex; i++ )
    {
      printf( "%s", _sBufferCache[i] ) ;
    }
    printf( MARKER_OFFSET "  end log cache dump   " MARKER_OFFSET "\n") ;
  }

//_door:
  return 0 ;
}

/******************************************************************************/
/* init logging                                    */
/******************************************************************************/
int initLogging( const char* logName, int logLevel )
{
  int sysRc = 0 ;

  setMaxLogLevel( logLevel ) ;

  sysRc = setLogFileName( logName ) ;
  
_door :
  return sysRc ;
}
/******************************************************************************/
/* set max logging level                                                      */
/******************************************************************************/
void setMaxLogLevel( int maxLevel )
{
  _gMaxLevel = maxLevel ;
}

/******************************************************************************/
/* set logging file name                                  */
/******************************************************************************/
int setLogFileName( const char* fName )
{
  FILE *fp ;

  sprintf(_gLogFileName[256],"%s",(char*)fName);
 
  fp = fopen(_gLogFileName,"a+");
  
 
  return 0 ;
}

/******************************************************************************/
/* get logging time                                                           */
/******************************************************************************/
void getLogTime( char *timeStr )
{
  time_t curTime ;
  struct tm *localTime ;

  curTime = time( NULL ) ;
  localTime = localtime( &curTime )   ;
  strftime( timeStr, TIME_STR_LNG      ,
                     "%Y-%m-%d %H:%M:%S" ,
                     localTime         ) ;
}




