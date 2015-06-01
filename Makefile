top_srcdir=.
top_builddir=.

REBAR=$(top_builddir)/rebar3

all: compile

compile: rebar.config
	$(REBAR) compile

clean: rebar.config
	$(REBAR) clean

distclean: clean

.PHONY: all compile clean distclean
