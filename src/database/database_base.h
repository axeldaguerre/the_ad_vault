#ifndef DATABASE_BASE_H
# define DATABASE_BASE_H

#define COL_TEXT(name, type) { {(U8 *)name, sizeof(name) - 1}, type }

typedef U32 DB_Error;
enum
{
  DB_Error_Null          = (0 << 0),
  DB_Error_Wrong_Query   = (1 << 0),
  DB_Error_Cant_Connect  = (1 << 1),
  DB_Error_Library       = (1 << 2),
  DB_Error_query_failed  = (1 << 3),
};

typedef U32 ColumnType;
enum
{
  ColumnType_Null,
  ColumnType_Integer,
  ColumnType_Float,
  ColumnType_Text,
  ColumnType_Blob,
};

typedef struct ColumnData ColumnData;
struct ColumnData 
{
  ColumnType type;
  TextType   text_type;
  String8    value;
  String8    name;
};

typedef struct ColumnDataNode ColumnDataNode;
struct ColumnDataNode
{
  ColumnDataNode *next;
  ColumnData     col;
};

typedef struct ColumnDataList ColumnDataList;
struct ColumnDataList
{
  ColumnDataNode *first;
  ColumnDataNode *last;
  U64           node_count;
};

typedef U32 TypeDB;
enum
{
  TypeDB_Null,
  TypeDB_SQLITE,
};

typedef struct StateDB StateDB;
struct StateDB
{
  
  OS_Handle lib;
  B32       is_initialized;
  DB_Error  error;
  TypeDB    db_type;
  U8        memory[200];
};



#endif