
SUBDIRS:= `ls | egrep -v '^(CVS)$$'`

DATESTRING	:=	$(shell date +%Y%m%d)

all:
	$(MAKE) -C $(CURDIR)/graphics/PCXView/ || { exit 1;}
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE) -C $$i || { exit 1;} fi; done;
clean:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE)  -C $$i clean || { exit 1;} fi; done;
install:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE)  -C $$i install || { exit 1;} fi; done;

#---------------------------------------------------------------------------------
dist: clean
#---------------------------------------------------------------------------------
	@tar --exclude=*CVS* --exclude=.svn -cvjf gba-examples-$(DATESTRING).tar.bz2 *
