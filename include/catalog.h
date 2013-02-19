struct sMsgItem
{
  unsigned int id ;
  char define[32] ;
  char txt[256] ;
  char lev[4] ;
  char class[8] ;
  struct sMsgItem *next ;
} ;

typedef struct sMsgItem tMsgItem ;

tMsgItem* buildCatalog() ;
