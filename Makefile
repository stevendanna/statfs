top_srcdir=.
top_builddir=.

REBAR=$(top_builddir)/rebar3
REGISTRY=$(HOME)/.cache/rebar3/hex/default/registry

all: compile

compile: rebar.config $(REGISTRY)
	$(REBAR) compile

clean: rebar.config
	$(REBAR) clean

$(REGISTRY):
	$(REBAR) update

distclean: clean

.PHONY: all compile clean distclean
