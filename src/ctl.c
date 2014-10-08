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
/*   - getLogFP                                                               */
/*   - getLogTime                                                             */
/*   - dumpFunc                                                               */
/*   - textornull                                                             */
/*   - logStr2lev                            */
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
/*   D E F I N E S                                                            */
/******************************************************************************/
#define TIME_STR_LNG 32
#define SPACE_OFFSET "                                    "
#define MARKER_OFFSET "= = = = = = = = = = = = = = ="

#define FLW_LSYS_FUNC_ENTRY     "enter function %s() in %s at line %05d"

#define FLW_LSYS_FUNC_EXIT      "exit function %s() in %s at line %05d"

#define DUMP_KEY_FORMAT  "%-30.30s"
#define DUMP_VAL_FORMAT  "%-47.80s"

// ---------------------------------------------------------
// multi line flag values
// ---------------------------------------------------------
#define MULTI_LINE_ON       1
#define MULTI_LINE_OFF      0

/******************************************************************************/
/*   G L O B A L S                                                            */
/******************************************************************************/

char *_gLoggerLevel[] = { [FLW]="FLW",
                          [DBG]="DBG",
                          [INF]="INF",
                          [LOG]="LOG",
                          [WAR]="WAR",
                          [ERR]="ERR",
                          [MLT]="MLT",
                          [CRI]="CRI",
                          [SYS]="SYS"
                        } ;

int _gMaxLevel = DEFAULT_LOG_LEVEL ;

char _gLogFileName[256] ;

FILE *_gLogFP ;

int _gMultiLine = MULTI_LINE_OFF ;

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
/*                                                */
/*  attributes:                                                               */
/*    line : nr of the source line (set in logger macro)                      */
/*    file : source file name      (set in logger macro)                      */
/*    func : function name         (set in logger macro)                      */
/*    id   : log message id        (passed through logger macro)              */
/*    lev  : log message level     (set by pragma in logger macro)            */
/*    msg  : message               (set by sprintf in logger macro)           */
/*                                      */
/*  description:                                                              */
/*    formted output to log file. loggerFunc can only be called from          */
/*    logger macro (ctl.h), all attributes except >id< are produced in        */
/*    logger macro                                                            */
/*                                          */
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
  char lineBuffer[LOG_BUFFER_LINE_SIZE+1]; // local line buffer
  char dbgBuffer[LOG_BUFFER_LINE_SIZE+1] ; // local line buffer(detail)
  char flowBuffer[LOG_BUFFER_LINE_SIZE+1]; // buffer flow for fuction entry/exit
  char timeStr[TIME_STR_LNG] ;           // buffer for time 
                                         //
  // -------------------------------------------------------
  // other vara
  // -------------------------------------------------------
  static int pid = 0  ;      // pid of the process
         int flowFlag ;      // bool entry / exit func flag

  // -------------------------------------------------------
  // check if log FD is open
  // -------------------------------------------------------
  if( _gLogFP == NULL )
  {
    _gLogFP = stderr ;
  }

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
  // set unset multi line flag
  // -------------------------------------------------------
  switch( id )
  {
    case LSYS_MULTILINE_START:
    {
      _gMultiLine = MULTI_LINE_ON ;
      break;
    }
    case LSYS_MULTILINE_ADD  :
    {
      break;
    }
    case LSYS_MULTILINE_END  :
    {
      _gMultiLine = MULTI_LINE_OFF ;
      break;
    }
    default:
    {
      if( _gMultiLine == MULTI_LINE_OFF ) 
      {
	break;
      }
      else
      {
        loggerFunc( line                              ,
                    file                              ,
                    func                              ,
                    LSYS_MULTILINE_END                , 
                    LEV_LSYS_MULTILINE_END            , 
                    "multi file forced switched off" );
      }
      break;
    }
  }

  // -------------------------------------------------------
  // check for function entry / exit 
  // -------------------------------------------------------
  flowFlag = 0 ;
  switch( id )
  {
    case LSYS_FUNC_ENTRY :
    {
      flowFlag = 1 ;
      snprintf( flowBuffer, LOG_BUFFER_LINE_SIZE, 
                            FLW_LSYS_FUNC_ENTRY, func, file, line ) ;
      break ;
    }
    case LSYS_FUNC_EXIT :
    {
      flowFlag = 1 ;
      snprintf( flowBuffer, LOG_BUFFER_LINE_SIZE,
                            FLW_LSYS_FUNC_EXIT, func, file, line ) ;
      break ;
    }
    default :
    {
      break ;
    }
  }

  // -------------------------------------------------------
  // fill line buffer for FUNCTION_ENTRY & FUNCTION_EXIT
  // -------------------------------------------------------
  if( flowFlag )
  {
    snprintf( lineBuffer, LOG_BUFFER_LINE_SIZE  ,
                          "%s %6d %05d %s %s\n" ,
                          timeStr               ,  // %s
                          pid                   ,  // %6d
                          id                    ,  // %05d 
                          _gLoggerLevel[lev]    ,  // %s 
                          flowBuffer           );  // %s
    dbgBuffer[0] = '\0' ;

  #if(0)
    if( lev < FLW )
    {
      goto _door_cache ;
    }
  #endif
  }
  // -------------------------------------------------------
  // fill line buffer
  // -------------------------------------------------------
  else
  {
    if( _gMultiLine == LSYS_MULTILINE_ADD )
    {
      snprintf( lineBuffer, LOG_BUFFER_LINE_SIZE     ,
                            "%s %6d %5.5s %3.3s %s\n",
                            timeStr                  ,  // %s
                            pid                      ,  // %6d 
                            " "                      ,  // %5.5s
                            " "                      ,  // %3.3s 
                            msg )                    ;  // %s
 
      dbgBuffer[0] = '\0' ;
    }
    else
    {
      snprintf( lineBuffer, LOG_BUFFER_LINE_SIZE  ,
                            "%s %6d %05d %s %s\n" ,
                            timeStr               ,  // %s 
                            pid                   ,  // %6d
                            id                    ,  // %05d
                            _gLoggerLevel[lev]    ,  // %s
                            msg )                 ;  // %s
  
      snprintf( dbgBuffer, LOG_BUFFER_LINE_SIZE           ,  //
                           "%s %s() in %s (line: %05d)\n" ,  //
                           SPACE_OFFSET                   ,  // %s 
                           func                           ,  // %s()
                           file                           ,  // %s 
                           line                          );  // %05d
    }
  }

  // -------------------------------------------------------
  // log if neccessary
  // -------------------------------------------------------
  if( _gMaxLevel >= lev || lev < MIN_LOG_LEVEL )
  {
    fprintf( _gLogFP, "%s", lineBuffer ) ;
  }

  if( _gMaxLevel > LOG )
  {
    fprintf( _gLogFP, "%s", dbgBuffer ) ;
  }

//_door_cache:

  // -------------------------------------------------------
  // fill logger cache
  // -------------------------------------------------------
  memcpy( _gBufferCache[_gBufferCacheIndex]   ,
          lineBuffer                          ,
          LOG_BUFFER_LINE_SIZE )              ;
  _gBufferCacheIndex++ ;
  if( _gBufferCacheIndex > LOG_BUFFER_CACHE_SIZE ) 
    _gBufferCacheIndex = 0 ;

  if( dbgBuffer[0] != '\0' )                           
  {
    memcpy( _gBufferCache[_gBufferCacheIndex] ,
            dbgBuffer                           ,
            LOG_BUFFER_LINE_SIZE )              ;
    _gBufferCacheIndex++ ;
    if( _gBufferCacheIndex > LOG_BUFFER_CACHE_SIZE ) 
      _gBufferCacheIndex = 0 ;
  }

  // -------------------------------------------------------
  // dump memory cache if level critical
  // -------------------------------------------------------
  if( lev == CRI  )
//    id  == LSYS_DUMP_START ||  
//    id  == LSYS_DUMP_END    )
  {
    int i ;

    fprintf(_gLogFP,MARKER_OFFSET "  begin log cache dump " MARKER_OFFSET "\n");
    for( i=_gBufferCacheIndex; i<LOG_BUFFER_CACHE_SIZE; i++ )
    {
      fprintf( _gLogFP, "%s", _gBufferCache[i] ) ;
    }

    for( i=0; i<_gBufferCacheIndex; i++ )
    {
      fprintf( _gLogFP, "%s", _gBufferCache[i] ) ;
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
/* get log file pointer                                                */
/******************************************************************************/
FILE* getLogFP()
{
  return _gLogFP ;
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

/******************************************************************************/
/*  dumper function                                                           */
/*                                                                            */
/*  name:                                                                     */
/*    dumpFunc                                                                */
/*                                    */
/*  attributes:                                                               */
/*    line   : nr of the source line (set in logger macro)                    */
/*    file   : source file name      (set in logger macro)                    */
/*    func   : function name         (set in logger macro)                    */
/*    offset : offset to value                                                */
/*                                  */
/*    msg  : message                 (list of key,value )                     */
/*                              */
/******************************************************************************/
int dumpFunc( char* _offset             ,   // print offset to value
              char _msg[][DMP_ITEM_LEN] )   // message to be dumped
{
  int sysRc ;

  int i ;

//char buff[DMP_ITEM_LEN*2+1] ;
//char separator[DMP_ITEM_LEN*2+1] ;
  char buff[LOG_BUFFER_LINE_SIZE+1] ;
  char separator[LOG_BUFFER_LINE_SIZE+1] ;
  
  // -------------------------------------------------------
  // init 
  // -------------------------------------------------------
  if( _gLogFP == NULL )
  {
    _gLogFP = stdin ;
  }

  snprintf( separator,  LOG_BUFFER_LINE_SIZE, 
                        "%s"DUMP_KEY_FORMAT":"
                         DUMP_VAL_FORMAT"\n",
                        _offset             ,
                        "========================================" ,
                        "========================================" );

  if( _gMaxLevel >= DBG ) { fprintf( _gLogFP, "%s", separator ) ; }

  // -------------------------------------------------------
  // print out all items
  // -------------------------------------------------------
  for( i=0; _msg[i][0]!= '\0'; i+=2 )
  {
    if(  _msg[i] == NULL ) break ;

    if( _msg[i+1] == NULL )
    {
      sysRc = 1 ;
      goto _door ;
    } 

    // -----------------------------------------------------
    // build buffer
    // -----------------------------------------------------
    snprintf( buff, LOG_BUFFER_LINE_SIZE, "%s"DUMP_KEY_FORMAT":"
                                           DUMP_VAL_FORMAT"\n",
                                           _offset            ,
                                           _msg[i]            ,
                                           _msg[i+1]        ) ;

    // -----------------------------------------------------
    // write flow and dbg if level high enought
    // -----------------------------------------------------
    if( _gMaxLevel >= DBG ) 
    {
      fprintf( _gLogFP, "%s", buff ) ;
    }

    // -----------------------------------------------------
    // set circular buffer
    // -----------------------------------------------------
    memcpy( _gBufferCache[_gBufferCacheIndex] ,
            separator                         ,
            LOG_BUFFER_LINE_SIZE )            ;
    _gBufferCacheIndex++ ;

    if( _gBufferCacheIndex > LOG_BUFFER_CACHE_SIZE ) 
      _gBufferCacheIndex = 0 ;
  }

  // -------------------------------------------------------
  // haus keeping
  // -------------------------------------------------------
  if( _gMaxLevel >= DBG ) { fprintf( _gLogFP, "%s", separator ) ; }

  memcpy( _gBufferCache[_gBufferCacheIndex] ,
          buff                              ,
          LOG_BUFFER_LINE_SIZE )            ;
  _gBufferCacheIndex++ ;

  if( _gBufferCacheIndex > LOG_BUFFER_CACHE_SIZE )
    _gBufferCacheIndex = 0 ;

  _door :

  return sysRc ;
}

/******************************************************************************/
/*  text or null                                                              */
/*                                                                            */
/*  description:                                                              */
/*    if a string (char*) to be logged is NULL,                               */
/*      it should be replaced by text "NULL"                                  */
/******************************************************************************/
const char* textornull( char *text )
{
  return (text ? text : "(NULL)");
}

/******************************************************************************/
/*  string to level                                                           */
/*                                                                            */
/*  description:                                                              */
/*    convert level string to level intiger                                   */
/******************************************************************************/
int logStr2lev( const char *str )
{
  int i;
  for( i=SYS; i<=FLW; i++ )
  {
    if( _gLoggerLevel[i] == NULL ) continue ;
    if( strcmp( str, _gLoggerLevel[i]) == 0 ) break ;
  } 
  return i ;
}
