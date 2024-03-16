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
};

typedef struct Textual Textual;
struct Textual
{
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

#endif