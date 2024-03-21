#ifndef TEXTUAL_BASE_H
# define TEXTUAL_BASE_H

typedef U32 TextType;
enum
{
  TextType_Null,

  TextType_Unmeaningfull,
  TextType_Title,
  TextType_Heading1,
  TextType_Heading2,
  TextType_Heading3,
  TextType_Paragraph,
  TextType_Link,
  TextType_Structural,
  TextType_Esthetic,
  TextType_Visual,
  TextType_Code,
  TextType_Date,
};

typedef struct Textual Textual;
struct Textual
{
  Textual *next_sibbling;
  String8  text;
  TextType type;
};

typedef struct TextualNode TextualNode;
struct TextualNode
{
  TextualNode *next;
  Textual     textual;
};

typedef struct TextualList TextualList;
struct TextualList
{
  TextualNode *first;
  TextualNode *last;
  U64         node_count;
};
// TODO: Use it for file html parsing
// TODO: it will not belong here
// TODO: better naming
typedef U32 FileFormatType;
enum
{
  FileFormatType_Null  = (1 << 0),
  FileFormatType_HTML  = (1 << 1),
  FileFormatType_TXT   = (1 << 2),
};

typedef struct TextualTable TextualTable;
struct TextualTable
{
  Textual *textuals;
  U64       count;
};

internal TextualTable*
textual_get_table(Arena *arena, Textual *raw_table, U64 size)
{
    TextualTable *table = push_array(arena, TextualTable, 1);
    table->count = size;
    table->textuals = raw_table;
    // NOTE: Check the table
    B32 has_title = 0;
    for(U64 idx = 0; idx < size; ++idx)
    {
      if(raw_table[idx].type == TextType_Title)
      {
        if(has_title)
        {
          // TODO: handle the error correctly (currently still compute)
          printf("ERROR: table has more than one title type\n");
          break;
        }
      }
    }
    return table;
}
#endif