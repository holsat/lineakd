bin_PROGRAMS = lineakd

lineakd_SOURCES = lineakd_core_functions.cpp cmdprefs.cpp main.cpp 
lineakd_LDADD   = -lpthread -llineak -lXext -lX11 -ldl
lineakd_LDFLAGS = -rdynamic -L../lineak $(all_libraries) @X_LIBS@ @X_PRE_LIBS@

EXTRA_DIST = main.cpp eakprocs.h cmdprefs.cpp cmdprefs.h lineakkb.def lineakd.1.bz2 OLDCODE lineakd_core_functions.h lineakd_core_functions.cpp 

INCLUDES= $(all_includes) -I$(top_srcdir)/lineak -I. -I..
DEFS = @DEFS@ -DCONFDIR=\"@sysconfdir@\" -DPLUGINDIR=\"@libdir@/lineakd/plugins\" -DBINDIR=\"@bindir@\"
AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/lineak -I$(top_srcdir)/lineakd -I. -I.. $(all_includes) 

#man_MANS = lineakd.1.gz

lineakd.1.bz2:
	bzip2 -c lineakd.1 > lineakd.1.bz2

install-data-local: lineakd.1.bz2
<<<<<<< Makefile.am
	$(mkinstalldirs) ${DESTDIR}$(sysconfdir)/
	$(mkinstalldirs) ${DESTDIR}$(mandir)/man1/
	$(mkinstalldirs) ${DESTDIR}$(pkglibdir)/plugins/
	$(INSTALL_DATA) $(srcdir)/lineakkb.def ${DESTDIR}$(sysconfdir)/lineakkb.def
	$(INSTALL_DATA) $(srcdir)/lineakd.1 ${DESTDIR}$(mandir)/man1/lineakd.1.bz2
=======
	$(mkinstalldirs) $(sysconfdir)/
	$(mkinstalldirs) $(mandir)/man1/
	$(mkinstalldirs) $(pkglibdir)/plugins/
	$(INSTALL_DATA) $(srcdir)/lineakkb.def $(sysconfdir)/lineakkb.def
	$(INSTALL_DATA) $(srcdir)/lineakd.1.bz2 $(mandir)/man1/lineakd.1.bz2
>>>>>>> 1.12
	 if test "$(SETKEYCODES_SUID)" != "no" ; then \
           chown root `which setkeycodes` ; \
           chmod +s `which setkeycodes` ; \
        fi

uninstall-local:
	-rm -f $(sysconfdir)/lineakkb.def
	-rm -f $(mandir)/man1/lineakd.1.bz2

