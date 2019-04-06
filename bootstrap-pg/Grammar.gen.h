#include "parser_base.h"

class Grammar : public DefaultGrammar
{
  public:
    virtual ~Grammar() {}
    virtual Match const *top(Cursor &c);
    virtual Match const *rule(Cursor &c);
    virtual Match const *alt(Cursor &c);
    virtual Match const *seq(Cursor &c);
    virtual Match const *rep(Cursor &c);
    virtual Match const *named(Cursor &c);
    virtual Match const *element(Cursor &c);
    virtual Match const *special(Cursor &c);
    virtual Match const *ref(Cursor &c);
    virtual Match const *capture(Cursor &c);
    virtual Match const *noncapture(Cursor &c);
    virtual Match const *text(Cursor &c);
};
