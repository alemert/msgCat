/******************************************************************************/
/* change title on for new project                                            */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <string.h>
#include <stdlib.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include "main.h"
#include "catalog.h"

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define     FORMAT "%05d %-4s %-25s %3s %s\n"
#define TOP_FORMAT "%-5s%-5s %-25s %3s %s\n"

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
void printMsg(       tMsgItem *p      ) ;
void printAllMsg(    tMsgItem *anchor ) ;
void printDefineMsg( tMsgItem *anchor, char *attrValue ) ;
void printClassMsg(  tMsgItem *anchor, char *attrValue ) ;
void printClassList( tMsgItem *anchor ) ;
void printIdMsg(     tMsgItem *anchor, unsigned int id ) ;

/******************************************************************************/
/*                                                                            */
/*                                  M A I N                                   */
/*                                                                            */
/******************************************************************************/
#ifndef __TDD__

int main(int argc, const char* argv[] )
{
  tMsgItem *msgAnchor ;  //

  char *attrValue ;
  int   attrInt ;

  int sysRc = 0 ;

  // -------------------------------------------------------
  // read and check command line attributes
  // -------------------------------------------------------
  sysRc = handleCmdLn( argc, argv ) ;
  if( sysRc != 0 ) goto _door ;

  // -------------------------------------------------------
  // get message catalog
  // -------------------------------------------------------
  msgAnchor = buildCatalog() ;

  // -------------------------------------------------------
  // handle cmdln attr --all
  // -------------------------------------------------------
  if( ! getFlagAttr( "all" ) )
  {
    printAllMsg( msgAnchor ) ;
    goto _door ;
  } 
  
  // -------------------------------------------------------
  // handle cmdln attr --define
  // -------------------------------------------------------
  if( (attrValue = (char*) getStrAttr( "define" ) ))
  {
    printDefineMsg( msgAnchor, attrValue ) ;
    goto _door ;
  } 

  // -------------------------------------------------------
  // handle cmdln attr --class
  // -------------------------------------------------------
  if( (attrValue = (char*)getStrAttr( "class" )) )
  {
    if( strcmp( attrValue, "?" ) == 0 )
    {
      printClassList( msgAnchor ) ;
      goto _door ;
    }
    printClassMsg(  msgAnchor, attrValue ) ;
    goto _door ;
  } 

  // -------------------------------------------------------
  // handle cmdln attr --id
  // -------------------------------------------------------
  if( (attrInt = getIntAttr( "id" )) )
  {
    printIdMsg( msgAnchor, attrInt ) ;
    goto _door ;
  } 

_door :

  return sysRc ;
}

#endif

/******************************************************************************/
/*                                                                            */
/*   F U N C T I O N S                                                        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* print a single catalog message                                             */
/******************************************************************************/
void printMsg( tMsgItem *p ) 
{
  printf( FORMAT, p->id, 
                  p->class, 
                  p->define, 
                  p->lev, 
                  p->txt ) ;
}

/******************************************************************************/
/* print out a whole message catalog                                          */
/******************************************************************************/
void printAllMsg( tMsgItem *anchor )
{
  tMsgItem *p ;

  p = anchor->next ;

  printf( TOP_FORMAT, " ID", "CLASS", " DEFINE", "LEV", "TEXT" ) ;

  while( p != NULL )
  {
    printMsg( p ) ; 
    p = p->next ;
  }
}

/******************************************************************************/
/* grep all messages matching some define                                     */
/******************************************************************************/
void printDefineMsg( tMsgItem *anchor, char *attrValue )
{
  tMsgItem *p ;
  int cnt = 0 ;

  p = anchor ;

  printf( TOP_FORMAT, " ID", "CLASS", " DEFINE", "LEV", "TEXT" ) ;

  while( p->next != NULL )
  {
    p = p->next ;
    if( !(strstr( p->define, attrValue )) ) continue ;
    printMsg( p ) ; 
    cnt++ ;
  }

  if( cnt==0 ) printf( "\nno message found matching define %s\n", attrValue ) ;
}

/******************************************************************************/
/* print all message belonging to some class                                  */
/******************************************************************************/
void printClassMsg( tMsgItem *anchor, char *attrValue )
{
  tMsgItem *p ;
  int cnt = 0 ;

  p = anchor ;

  printf( TOP_FORMAT, " ID", "CLASS", " DEFINE", "LEV", "TEXT" ) ;

  while( p->next != NULL )
  {
    p = p->next ;
    if( (strcmp( p->class, attrValue )) != 0 ) continue ;
    printMsg( p ) ; 
    cnt++ ;
  }

  if( cnt==0 ) printf( "\nno message found matching class %s\n", attrValue ) ;
}

/******************************************************************************/
/* print a list of all message classes                                        */
/******************************************************************************/
void printClassList( tMsgItem *anchor ) 
{
  tMsgItem *p ;
  char **classList ;
  char **classItem ;

  classList = (char**) malloc( sizeof(char*) * 256 ) ;
  *classList = NULL ;
  
  p = anchor ;

  while( p->next != NULL )
  {
    p = p->next ;
    classItem = classList ;
    while( *classItem != NULL )
    {
      if( strcmp(*classItem,p->class) == 0 ) break ;
      classItem++ ;
    }
    if( *classItem != NULL ) continue ;

    *classItem = p->class ;
    classItem++ ;
    *classItem   = NULL ;
  }

  classItem = classList ;
  while( *classItem != NULL )
  {
    printf( "\t%s\n", *classItem ) ;
    classItem++ ;
  }
  free( classList ) ;
}

/******************************************************************************/
/* print a list of all message classes                                        */
/******************************************************************************/
void printIdMsg( tMsgItem *anchor, unsigned int id ) 
{
  tMsgItem *p ;
 
  p = anchor ;

  printf( TOP_FORMAT, " ID", "CLASS", " DEFINE", "LEV", "TEXT" ) ;

  while( p->next != NULL )
  {
    p = p->next ; 
    
    if( p->id == id ) 
    {
      printMsg( p ) ;
      return ;
    }
  } 
}
