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
#include <errno.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#define C_MODULE_LOGGER_CATALOG
#include "ctl.h"

/******************************************************************************/
/*   G L O B A L S                                                            */
/******************************************************************************/

char *_gLoggerLevel[] = { [FLW]="FLW",
                          [INF]="INF",
                          [LOG]="LOG",
                          [WAR]="WAR",
                          [ERR]="ERR",
                          [CRI]="CRI",
                          [SYS]="SYS"
                        } ;

int _gMaxLevel = DEFAULT_LOG_LEVEL ;

char _gLogFileName[256] ;

FILE *_gLogFP ;

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define TIME_STR_LNG 32
#define SPACE_OFFSET "                                    "
#define MARKER_OFFSET "= = = = = = = = = = = = = = ="

#define FLW_LSYS_FUNC_ENTRY     "enter function %s() in %s at line %05d"

#define FLW_LSYS_FUNC_EXIT      "exit function %s() in %s at line %05d"


/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/


/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
void getLogTime( char *timeStr ) ;
void setMaxLogLevel( int maxLevel ) ;
int  setLogFileName( const char* name ) ;

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
int loggerFunc( const int   line,  // source file line of the logger macro
                const char* file,  // source file name of the logger macro
                const char* func,  // source file function of the logger macro
                      int   id,
                      int   lev,
                      char* msg)
{
  // -------------------------------------------------------
  // local buffer for building log message
  // -------------------------------------------------------
  char lineBuffer[LOG_BUFFER_LINE_SIZE]; // local line buffer
  char dbgBuffer[LOG_BUFFER_LINE_SIZE] ; // local line buffer(detail)
  char flowBuffer[LOG_BUFFER_LINE_SIZE]; // buffer flow for fuction entry/exit
  char timeStr[TIME_STR_LNG] ;           // buffer for time 
                                         //
  // -------------------------------------------------------
  // static buffer for dumping cashed messages in case of CRI errors
  // -------------------------------------------------------
  static char _sBufferCache[LOG_BUFFER_CACHE_SIZE] // circular cache for last 
                           [LOG_BUFFER_LINE_SIZE]; // LOG_BUFFER_CACHE_SIZE msg
  static int  _sBufferCacheIndex = 0 ;             // actual line index for 
                                                   // circular cache
  // -------------------------------------------------------
  // other vara
  // -------------------------------------------------------
  static int pid = 0  ;      // pid of the process
         int flowFlag ;      // bool entry / exit func flag

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
  // check for function entry / exit 
  // -------------------------------------------------------
  flowFlag = 0 ;
  switch( id )
  {
    case LSYS_FUNC_ENTRY :
    {
      flowFlag = 1 ;
      sprintf( flowBuffer, FLW_LSYS_FUNC_ENTRY, func, file, line ) ;
      break ;
    }
    case LSYS_FUNC_EXIT :
    {
      flowFlag = 1 ;
      sprintf( flowBuffer, FLW_LSYS_FUNC_EXIT, func, file, line ) ;
      break ;
    }
    default :
    {
      break ;
    }
  }

  if( flowFlag )
  {
    snprintf( lineBuffer, LOG_BUFFER_LINE_SIZE  ,
                          "%s %6d %05d %s %s\n" ,
                          timeStr               ,  // %s
                          pid                   ,  // %6d
                          id                    ,  // %05d 
                          _gLoggerLevel[lev]    ,  // %s 
                          flowBuffer           );  // %s
    goto _door_cache ;
  }

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
  // log if neccessary
  // -------------------------------------------------------
  if( _gMaxLevel > lev || lev < MIN_LOG_LEVEL )
  {
    fprintf( _gLogFP, "%s", lineBuffer ) ;
  }

  if( _gMaxLevel > LOG )
  {
    fprintf( _gLogFP, "%s", dbgBuffer ) ;
  //printf("%s",dbgBuffer) ;
  }

  _door_cache:

  // -------------------------------------------------------
  // fill logger cache
  // -------------------------------------------------------
  memcpy( _sBufferCache[_sBufferCacheIndex]   ,
          lineBuffer                          ,
          LOG_BUFFER_LINE_SIZE )              ;
  _sBufferCacheIndex++ ;
  if( dbgBuffer[0] == '\0' )                           
  {
    memcpy( _sBufferCache[_sBufferCacheIndex+1] ,
            dbgBuffer                           ,
            LOG_BUFFER_LINE_SIZE )              ;
    _sBufferCacheIndex++ ;
  }

  // -------------------------------------------------------
  // dump memory chashe if level critical
  // -------------------------------------------------------
  if( lev == CRI            ||
      id  == LSYS_FLOW_DUMP  )
  {
    int i ;

    fprintf(_gLogFP,MARKER_OFFSET "  begin log cache dump " MARKER_OFFSET "\n");
    for( i=_sBufferCacheIndex; i<LOG_BUFFER_CACHE_SIZE; i++ )
    {
      fprintf( _gLogFP, "%s", _sBufferCache[i] ) ;
    }

    for( i=0; i<_sBufferCacheIndex; i++ )
    {
      fprintf( _gLogFP, "%s", _sBufferCache[i] ) ;
    }
    fprintf(_gLogFP, MARKER_OFFSET "  end log cache dump   " MARKER_OFFSET"\n");
  }

#if(0)
  _door_exit:
#endif

  fflush( _gLogFP );
  return 0 ;
}

/******************************************************************************/
/* init logging                                                               */
/******************************************************************************/
int initLogging( const char* logName, int logLevel )
{
  int sysRc = 0 ;

  setMaxLogLevel( logLevel ) ;

  sysRc = setLogFileName( logName ) ;
  
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
/* set logging file name                                                      */
/******************************************************************************/
int setLogFileName( const char* fName )
{
  int sysRc =0 ;

  sprintf( _gLogFileName, "%s", (char*)fName );
 
  _gLogFP = fopen(_gLogFileName,"a");
  if( _gLogFP == NULL )
  {
    perror( _gLogFileName );
    sysRc = errno ; 
    goto _door ;
  }

_door :
 
  return sysRc ;
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




