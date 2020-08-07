#ifndef TYPE_C
#define TYPE_C

#include <stdio.h>
#include "error.c"
#include <Block.h>

typedef enum {boolean, string, block, number, list} cognate_type;

struct __attribute__((packed)) cognate_list
{
  struct cognate_object *start,
                        //*end, // For storing amount of free allocated space. Not currently needed.
                        *top;
};

// Removing packing will give slight performance gains at cost of more memory.
struct __attribute__((packed)) cognate_object
{
  union
  {
    char* string;
    _Bool boolean;
    void (^block)(void);
    double number;
    struct cognate_list *list;
  };
  cognate_type type : 4;
};

typedef struct cognate_object cognate_object;
typedef struct cognate_list   cognate_list;

static cognate_object check_type(cognate_type, cognate_object);
static const char* lookup_type(cognate_type);

static cognate_object check_type(cognate_type expected_type, cognate_object object)
{
  if (object.type == expected_type) 
  {
    return object;
  }
  type_error(lookup_type(expected_type), lookup_type(object.type));
}

static const char* lookup_type(cognate_type type)
{
  switch(type)
  {
    case boolean: return "Boolean";
    case string : return "String";
    case block  : return "Block";
    case number : return "Number";
    case list   : return "List";
  }
  static char type_number[80];
  sprintf(type_number, "Unknown Type %i", type);
  return type_number;
}

#endif