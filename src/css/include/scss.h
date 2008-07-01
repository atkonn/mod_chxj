
#if !defined(__SCSS_H__)
#define __SCSS_H__

#include "apr_pools.h"
#include "apr_strings.h"

#define ROOT_NODE_SIGNATURE "<rootNode>"

typedef struct _SCSSDoc_t SCSSDoc_t;
typedef SCSSDoc_t *SCSSDocPtr_t;



typedef enum _SCSSType_t SCSSType_t;
enum _SCSSType_t {
  SCSSTYPE_STYLESHEET = 0,
  SCSSTYPE_ATKEYWORD = 1,
  SCSSTYPE_SELECTOR,
  SCSSTYPE_PROPERTY,
};


typedef struct _SCSSNode_t SCSSNode_t;
typedef SCSSNode_t *SCSSNodePtr_t;

struct _SCSSNode_t {
  SCSSType_t type; 
  SCSSNodePtr_t next;
  SCSSNodePtr_t *ref;
  SCSSNodePtr_t child;
  char *name;
  char *value1;
  char *value2;
};

struct _SCSSDoc_t {
  apr_pool_t *pool;
  SCSSNodePtr_t rootNode;
  SCSSNodePtr_t nowNode;
};

#define IS_STYLESHEET(X) ((X)->type == SCSSTYPE_STYLESHEET)
#define IS_ATKEYWORD(X)  ((X)->type == SCSSTYPE_ATKEYWORD )
#define IS_SELECTOR(X)   ((X)->type == SCSSTYPE_SELECTOR  )
#define IS_PROPERTY(X)   ((X)->type == SCSSTYPE_PROPERTY  )

#endif
