#ifndef TEXT_H
#define TEXT_H

typedef int Text;

Text text_create(char *);
Text text_cats(Text t, char *s);
Text text_cat(Text t, Text v);
char * text_string(Text t);

#endif
