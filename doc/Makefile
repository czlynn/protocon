
tex2webTopPath = ../dep/tex2web
tex2web = $(tex2webTopPath)/bin/tex2web
man2html = man2html

CMAKE=cmake
GODO=$(CMAKE) -E chdir
MKDIR=$(CMAKE) -E make_directory

SPACE :=
SPACE +=


default: all

html/ html/example/ pubhtml/ pubhtml/example/ \
 	pubhtml/examplespec/ pubhtml/examplesett/ \
	pubhtml/examplesynt/ pubhtml/examplesoln/:
	mkdir -p $@
	chmod a+rx $@


$(tex2web):
	if [ ! -f $(tex2webTopPath)/src/tex2web.c ] ; then \
		git clone https://github.com/grencez/tex2web.git $(tex2webTopPath) ; \
		fi
	if [ ! -d $(tex2webTopPath)/bld ] ; then \
		$(MKDIR) $(tex2webTopPath)/bld ; \
		$(GODO) $(tex2webTopPath)/bld $(CMAKE) -D DepPath=../.. ../src ; \
		fi
	#$(GODO) $(tex2webTopPath) git pull origin master
	$(GODO) $(tex2webTopPath)/bld $(MAKE)


define webtex_rule
Targets += html/$(1).html
PubTargets += pubhtml/$(1).html

html/$(1).html: webtex/$(1).tex $(2) $(tex2web)
	$(tex2web) -x $$< -o $$@ \
		-def rel $(subst $(SPACE),/,$(strip $(patsubst %,..,$(subst /, ,$(1)))))/.. \
		-def examplespec '\rel/examplespec' \
		-def examplesett '\rel/examplesett' \
		-def examplesynt '\rel/examplesynt' \
		-def examplesoln '\rel/examplesoln'
	chmod a+r,a-x $$@

html/$(1).html: | $(dir html/$(1))

pubhtml/$(1).html: webtex/$(1).tex $(2) $(tex2web)
	$(tex2web) -x $$< -o $$@ \
		-def rel $(patsubst .%,%,$(subst $(SPACE),/,$(strip $(patsubst %,..,$(subst /, ,$(1)))))) \
		-def examplespec '\rel/examplespec' \
		-def examplesett '\rel/examplesett' \
		-def examplesynt '\rel/examplesynt' \
		-def examplesoln '\rel/examplesoln'
	chmod a+r,a-x $$@

pubhtml/$(1).html: | $(dir pubhtml/$(1))

endef

define example_deps
../examplespec/$(1).prot ../examplesoln/$(1).prot
endef

$(eval \
	$(call webtex_rule,index,webtex/content.tex webtex/changes.tex webtex/thanks.tex) \
	$(call webtex_rule,benchmark,) \
	$(call webtex_rule,legit,) \
	$(call webtex_rule,permit,) \
	$(call webtex_rule,tut,) \
	$(call webtex_rule,example/index,) \
	$(call webtex_rule,example/Coloring,$(call example_deps,ColorRing)) \
	$(call webtex_rule,example/Matching,$(call example_deps,MatchRingOneBit)) \
	$(call webtex_rule,example/Orientation,$(call example_deps,OrientRingOdd)) \
	$(call webtex_rule,example/TokenPassing,$(call example_deps,TokenRingThreeBit)) \
	)

define prot_rule
PubTargets += pubhtml/example$(1).$(2)

pubhtml/example$(1).$(2): ../example$(1).$(2)
	cp -f -T $$< $$@
	chmod a+r,a-x $$@

pubhtml/example$(1).$(2): | $(dir pubhtml/example$(1))

endef

$(eval $(foreach f,$(shell cat ../meta/examplespec.files),$(call prot_rule,spec/$(f),prot)))
$(eval $(foreach f,$(shell cat ../meta/examplesett.files),$(call prot_rule,sett/$(f),args)))
$(eval $(foreach f,$(shell cat ../meta/examplesynt.files),$(call prot_rule,synt/$(f),prot)))
$(eval $(foreach f,$(shell cat ../meta/examplesoln.files),$(call prot_rule,soln/$(f),prot)))

Targets += html/man.html
PubTargets += pubhtml/man.html

html/man.html pubhtml/man.html: protocon.1
	$(man2html) $< > $@
	chmod a+r,a-x $@

html/man.html: | html

pubhtml/man.html: | pubhtml

all: $(Targets)

pub: $(PubTargets)

.PHONY: clean
clean:
	rm -fr html pubhtml
	rm -fr $(tex2webTopPath)/bld

.PHONY: distclean
distclean: clean
	rm -f $(tex2webTopPath)/bin/tex2web

