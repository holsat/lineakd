COMPILE_FIRST = lineak
COMPILE_LAST = lineakd

ACLOCAL_AMFLAGS=-I m4

####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = lineak utils lineakd

EXTRA_DIST = lineakd.kdevprj admin AUTHORS COPYING ChangeLog INSTALL README TODO lineakd.lsm lineakd.spec lineakd-noarch.spec README_LINEAK_LIBRARY DIRECTIVES 

####### kdevelop will overwrite this part!!! (end)############
# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs

AUTOMAKE_OPTIONS = foreign
DEFS = @DEFS@ -DCONFDIR=\"@sysconfdir@\" -DPLUGINDIR=\"@libdir@/lineakd/plugins\" -DBINDIR=\"@bindir@\"

$(top_srcdir)/configure.in: configure.in.in $(top_srcdir)/subdirs
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in ;

$(top_srcdir)/plugins/configure.in: configure.in.in $(top_srcdir)/subdirs
	cd $(top_srcdir)/plugins/ && $(MAKE) -f ../admin/Makefile.common configure.in ;

$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs

$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/lineak.m4.in
	@cd $(top_srcdir) && cat admin/acinclude.m4.in $(top_srcdir)/admin/lineak.m4.in > acinclude.m4

MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 configure.files


package-messages:
	$(MAKE) -f admin/Makefile.common package-messages

dist-hook:
	cd $(top_distdir) && perl admin/am_edit -padmin
	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs


METASOURCES = AUTO
