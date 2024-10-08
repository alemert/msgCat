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
/*   - logStr2lev                                                             */
/*   - rotateLogFile                                                          */
/*   - catalogVersion                                                         */
/*   - stopLogging                                                    */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
 #define _GNU_SOURCE

// ---------------------------------------------------------
// system include
// ---------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#define C_MODULE_LOGGER_CATALOG
#include "ctl.h"

// ---------------------------------------------------------
// local
// ---------------------------------------------------------
#include <version.h>

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

// ---------------------------------------------------------
// rotate log files
// ---------------------------------------------------------
#define MAX_FILE_SIZE   2*1024*1024      // 2MB
#define MAX_FILE_NR     4
#define ROTATE_ON_START 1
#define ROTATE_ON_RUN   2

/******************************************************************************/
/*   T Y P E S                                                                */
/******************************************************************************/
typedef enum eRotate tRotate ;

/******************************************************************************/
/*   S T R U C T S                                                            */
/******************************************************************************/
enum eRotate
{
  ignore = 0,
  start  = 1,
  run    = 2
};

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

char _gLogFileName[PATH_MAX] ;

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
void rotateLogFile( );

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
/*                                                                            */
/*  attributes:                                                               */
/*    line : number of the source line (set in logger macro)                  */
/*    file : source file name      (set in logger macro)                      */
/*    func : function name         (set in logger macro)                      */
/*    id   : log message id        (passed through logger macro)              */
/*    lev  : log message level     (set by pragma in logger macro)            */
/*    msg  : message               (set by sprintf in logger macro)           */
/*                                            */
/*  description:                                                              */
/*    formated output to log file. loggerFunc can only be called from         */
/*    logger macro (ctl.h), all attributes except >id< are produced in        */
/*    logger macro                                                            */
/*                                            */
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

    if( _gMaxLevel > LOG )
    {
      fprintf( _gLogFP, "%s", dbgBuffer ) ;
    }
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
  if( lev > SYS )
  {
    rotateLogFile(run);
  }
  return 0 ;
}

/******************************************************************************/
/* initialize logging                                                         */
/******************************************************************************/
int initLogging( const char* logName, int logLevel )
{
  int sysRc = 0 ;

  setMaxLogLevel( logLevel ) ;

  sysRc = setLogFileName( logName ) ;

  rotateLogFile( start );

  logger( LSTD_PRG_START, program_invocation_short_name  );

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
/* get log file pointer                                                       */
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
/*                                      */
/*  attributes:                                                               */
/*    line   : nr of the source line (set in logger macro)                    */
/*    file   : source file name      (set in logger macro)                    */
/*    func   : function name         (set in logger macro)                    */
/*    offset : offset to value                                                */
/*                                    */
/*    msg  : message                 (list of key,value )                     */
/*                                */
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
    // write flow and debug if level high enough
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
  // house keeping
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
/*    convert level string to level integer                                   */
/*  description:                                                              */
/******************************************************************************/
int logStr2lev( const char *str )
{
  int i;
  if( str == NULL ) return LNA ;
  for( i=SYS; i<=FLW; i++ )
  {
    if( _gLoggerLevel[i] == NULL ) continue ;
    if( strcmp( str, _gLoggerLevel[i]) == 0 ) return i ;
  } 
  return LNA ;
}

/******************************************************************************/
/*  rotate log files                                                          */
/*                                                                            */
/*  description:                                                              */
/*    if the maximal log file size has been reached rotate log file           */
/******************************************************************************/
void rotateLogFile( tRotate rotType )
{
  struct stat fStat ;

  char  baseFileName[PATH_MAX];
  char  highFile[PATH_MAX];
  char  lowFile[PATH_MAX];
  char* p;

  int i;

  int maxFileSize = MAX_FILE_SIZE;

  // ---------------------------------------------------------------------------
  //  check if base file has reached max size 
  // -----------------------------------------------
  if( stat( _gLogFileName, &fStat ) < 0 )       // get file information
  {                                             // if any error, return 
    goto _door ;                                //  from function
  }                                             //
                                                //
  switch( rotType )                             //
  {                                             //
    case ignore: goto _door;                    // do not rotate
    case start :                                // rotate if called on start 
    {                                           //  of the program
      break ;                                   //
    }                                           //
    case run :                                  // rotate if maximal size of 
    {                                           //  the log file reached
      maxFileSize *= 2;                         //
      break ;                                   //
    }                                           //
  }                                             //
                                                //
  switch( _gMaxLevel )                          // increase max size depending
  {                                             //   on the logging level
    case INF:                                   //
    {                                           //
      maxFileSize *= 3/2;                        //
      break;                                    //
    }                                           //
    case DBG :                                  //
    {                                           //
      maxFileSize *= 2;                          //
      break;                                    //
    }                                           //
    case FLW :                                  //
    {                                           //
      maxFileSize *= 3 ;                         //
      break;                                    //
    }                                           //
  }                                             //
                                                //
  if( (int)(fStat.st_size) < maxFileSize )      //
  {                                             //
    goto _door ;                                //
  }

  if( rotType == run ) logger( LSYS_CLOSE_OLD_LOG );     

  if( _gLogFP )
  {
    fclose(_gLogFP) ;
  }

  // -----------------------------------------------
  // get the base file name
  // -----------------------------------------------
  strncpy(baseFileName,_gLogFileName,PATH_MAX); //
                                                //
  p = baseFileName + strlen( baseFileName ) -1; // end of string address; go
  for( ; p > baseFileName; p-- )                // backwards through the string
  {                                             //
    if( *p == '.' )                             // until '.' (start of suffix) 
    {                                           //  has been found
      break;                                    //
    }                                           //
  }                                             //
                                                //
  if( p == baseFileName ) goto _door;           // '.' (suffix) not found, 
                                                // quit function
   p++      ;                                   // new string ends after '.'
  *p = '\0' ;                                   // suffix has been cut off
                                                //
  // -----------------------------------------------
  // rename files (low file to high file)
  // -----------------------------------------------
  for( i=MAX_FILE_NR; i>0; i-- )                //
  {                                             //
    snprintf( highFile, PATH_MAX, "%s%d.log", baseFileName, i   );
    snprintf( lowFile , PATH_MAX, "%s%d.log", baseFileName, i-1 );
                                                // 
    if( stat( highFile, &fStat ) == 0 )         // remove the high file
    {                                           //
      unlink( highFile );                       //
    }                                           //
                                                //
    if( stat( lowFile, &fStat ) == 0 )          // link low file to high file
    {                                           //
      link( lowFile, highFile );                //
    }                                           //
  }                                             //
                                                //
  // -----------------------------------------------
  // handling base file (without the counter)
  // -----------------------------------------------
  snprintf( highFile, PATH_MAX, "%s%d.log", baseFileName, 0 );
                                                //
  if( stat( highFile, &fStat ) == 0 )           // remove the high file
  {                                             //
    unlink( highFile );                         //
  }                                             //
                                                //
  link(   _gLogFileName , highFile );           //
  unlink( _gLogFileName );                      //
                                                // 
  // -----------------------------------------------
  // write in new log that a new log has been opened
  // -----------------------------------------------
  _gLogFP = fopen(_gLogFileName,"a");           //
  if( _gLogFP == NULL )                         //
  {                                             //
    perror( _gLogFileName );                    //
    goto _door ;                                //
  }                                             //
                                                //
  switch( rotType )                             //
  {                                             //
    case start:                                 //
    case run  :                                 //
    {                                           //
      logger( LSYS_START_NEW_LOG );             //
      break;                                    //
    }                                           //
    case ignore :                               //
    {                                           //
      break;                                    //
    }                                           //
  }                                             //

  _door:

  return ;
}

/******************************************************************************/
/*  catalog version                                                           */
/******************************************************************************/
const char* catalogVersion()
{
  return MAJOR_VER"."MINOR_VER"."FIX_VER"."BUILD_VER ;
}

/******************************************************************************/
/*   stop logging              */
/******************************************************************************/
void stopLogging()
{
  if( _gLogFP ) fclose(_gLogFP);
  _gLogFP = NULL ;
}