/*
   The table is designed to allow you to include it multiple times to "pull out" other things
   from the table, such as html tagname as strings or enums, if a tag has a closing tag etc.
*/
#ifndef STRLEN
  #define STRLEN(X) (U64)(sizeof(X) - 1)
#endif
#ifndef STR
  #define STR(x) #x
#endif

#ifndef STR_WITH_TAGS_WITH_CLOSING
  #define STR_WITH_TAGS_WITH_CLOSING(x) STR(<##x></##x>)
#endif

#ifndef STR_WITH_SINGLE_TAG
  #define STR_WITH_SINGLE_TAG(x) STR(<##x>)
#endif

#ifndef STR_WITH_TAGS
  #define STR_WITH_TAGS(x, isSingle)((isSingle)?\
       STR_WITH_SINGLE_TAG(x),STRLEN(STR_WITH_SINGLE_TAG(x)):\
       STR_WITH_TAGS_WITH_CLOSING(x),STRLEN(STR_WITH_TAGS_WITH_CLOSING(x)))
#endif    


#ifndef TAG
  #define TAG(Tag, ClosingType, str, TextType, ...) { HTMLTag_##Tag, HTMLTagEnclosingType_##ClosingType, str, TextType_##TextType, __VA_ARGS__ },
#endif

#ifndef true
  #define true 1
#endif
#ifndef false
  #define false 0
#endif

#define A          { (U8*)"a",         sizeof("a")-1 }
#define P          { (U8*)"p",         sizeof("p")-1 }
#define LI         { (U8*)"li",        sizeof("li")-1 }
#define UL         { (U8*)"ul",        sizeof("ul")-1 }
#define OL         { (U8*)"ol",        sizeof("ol")-1 }
#define H1         { (U8*)"h1",        sizeof("h1")-1 }
#define H2         { (U8*)"h2",        sizeof("h2")-1 }
#define H3         { (U8*)"h3",        sizeof("h3")-1 }
#define H4         { (U8*)"h4",        sizeof("h4")-1 }
#define DIV        { (U8*)"div",       sizeof("div")-1 }
#define IMG        { (U8*)"img",       sizeof("img")-1 }
#define BR         { (U8*)"br",        sizeof("br")-1 }
#define STRONG     { (U8*)"strong",    sizeof("strong")-1 }
#define SPAN       { (U8*)"span",      sizeof("span")-1 }
#define PRE        { (U8*)"pre",       sizeof("pre")-1 }
#define U          { (U8*)"u",         sizeof("u")-1 }
#define EM         { (U8*)"em",        sizeof("em")-1 }
#define CODE       { (U8*)"code",      sizeof("code")-1 }
#define SECTION    { (U8*)"section",   sizeof("section")-1 }
#define META       { (U8*)"meta",      sizeof("meta")-1 }
#define LINK       { (U8*)"link",      sizeof("link")-1 }
#define SCRIPT     { (U8*)"script",    sizeof("script")-1 }
#define SMALL      { (U8*)"small",     sizeof("small")-1 }
#define ARTICLE    { (U8*)"article",   sizeof("article")-1 }
#define NAV        { (U8*)"nav",       sizeof("nav")-1 }
#define HEADER     { (U8*)"header",    sizeof("header")-1 }
#define MAIN       { (U8*)"main",      sizeof("main")-1 }
#define ASIDE      { (U8*)"aside",     sizeof("aside")-1 }
#define TABLE      { (U8*)"table",     sizeof("table")-1 }
#define TR         { (U8*)"tr",        sizeof("tr")-1 }
#define TH         { (U8*)"th",        sizeof("th")-1 }
#define TD         { (U8*)"td",        sizeof("td")-1 }
#define FORM       { (U8*)"form",      sizeof("form")-1 }
#define INPUT      { (U8*)"input",     sizeof("input")-1 }
#define BUTTON     { (U8*)"button",    sizeof("button")-1 }
#define HEAD       { (U8*)"head",      sizeof("head")-1 }
#define DOCTYPE    { (U8*)"!DOCTYPE",  sizeof("!DOCTYPE")-1 }
#define HTML       { (U8*)"html",      sizeof("html")-1 }
#define TITLE      { (U8*)"title",     sizeof("title")-1 }
#define BODY       { (U8*)"body",      sizeof("body")-1 }
#define FOOTER     { (U8*)"footer",    sizeof("footer")-1 }
#define LABEL      { (U8*)"label",     sizeof("label")-1 }

TAG(a,      Paired,   A, Link)
TAG(p,      Paired,   P, Paragraph)
TAG(li,     Paired,   LI, Paragraph)
TAG(ul,     Paired,   UL, Paragraph)
TAG(ol,     Paired,   OL, Paragraph)
TAG(h1,     Paired,   H1, Title)
TAG(h2,     Paired,   H2, Heading1)
TAG(h3,     Paired,   H3, Heading2)
TAG(h4,     Paired,   H4, Heading3)
TAG(div,    Paired,   DIV, Structural)
TAG(img,    Unique,   IMG, Visual)
TAG(br,     Unique,   BR, Esthetic)
TAG(strong, Paired,   STRONG, Esthetic)
TAG(span,   Paired,   SPAN, Esthetic)
TAG(pre,    Paired,   PRE, Esthetic)
TAG(u,      Paired,   U, Esthetic)
TAG(em,     Paired,   EM, Esthetic)
TAG(code,   Paired,   CODE, Esthetic)
TAG(section,Paired,   SECTION, Structural)
TAG(meta,   Unique,   META, Structural)
TAG(link,   Unique,   LINK, Structural)
TAG(script, Paired,   SCRIPT, Structural)
TAG(small,  Paired,   SMALL, Esthetic)
TAG(article,Paired,   ARTICLE, Structural)
TAG(nav,    Paired,   NAV, Structural)
TAG(header, Paired,   HEADER, Structural)
TAG(main,   Paired,   MAIN, Structural)
TAG(aside,  Paired,   ASIDE, Structural)
TAG(table,  Paired,   TABLE, Structural)
TAG(tr,     Paired,   TR, Structural)
TAG(th,     Paired,   TH, Structural)
TAG(td,     Paired,   TD, Structural)
TAG(form,   Paired,   FORM, Structural)
TAG(input,  Unique,   INPUT, Structural)
TAG(button, Paired,   BUTTON, Structural)
TAG(label,  Paired,   LABEL, Structural)
TAG(head,   Paired,   HEAD, Structural)
TAG(doctype,Unique,   DOCTYPE, Structural)
TAG(html,   Paired,   HTML, Structural)
TAG(title,  Paired,   TITLE, Structural)
TAG(body,   Paired,   BODY, Structural)
TAG(footer, Paired,   FOOTER, Structural)

#undef A
#undef P
#undef LI
#undef UL
#undef OL
#undef H1
#undef H2
#undef H3
#undef H4
#undef DIV
#undef IMG
#undef BR
#undef STRONG
#undef SPAN
#undef PRE
#undef U
#undef EM
#undef CODE
#undef SECTION
#undef META
#undef LINK
#undef SCRIPT
#undef SMALL
#undef ARTICLE
#undef NAV
#undef HEADER
#undef MAIN
#undef ASIDE
#undef TABLE
#undef TR
#undef TH
#undef TD
#undef FORM
#undef INPUT
#undef BUTTON
#undef LABEL
#undef HEAD
#undef DOCTYPE
#undef HTML
#undef TITLE
#undef BODY
#undef FOOTER

#undef TAG