AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/lineak -I$(top_srcdir)/lineak/.libs -I$(top_srcdir)/lineakd -I. -I.. -I/usr/include $(all_includes) 

if ! CYGWIN

if EVTEST
   bin_PROGRAMS = evtest xsendkeys xsendkeycode
   evtest_SOURCES = evtest.c
   evtest_LDFLAGS = $(AM_CPPFLAGS)
evtest.1.bz2:
	bzip2 -c evtest.1 > evtest.1.bz2
if SEND_TO_KEYBOARD
install-data-local: send_to_keyboard.8.bz2 evtest.1.bz2 xsendkeycode.8.bz2 xsendkeys.8.bz2
else
install-data-local: evtest.1.bz2 xsendkeycode.8.bz2 xsendkeys.8.bz2
endif
	$(mkinstalldirs) ${DESTDIR}$(mandir)/man1/
	$(INSTALL_DATA) $(srcdir)/evtest.1.bz2 ${DESTDIR}$(mandir)/man1/evtest.1.bz2
	$(mkinstalldirs) ${DESTDIR}$(mandir)/man8/
	$(INSTALL_DATA) $(srcdir)/xsendkeycode.8.bz2 ${DESTDIR}$(mandir)/man8/xsendkeycode.8.bz2
	$(INSTALL_DATA) $(srcdir)/xsendkeys.8.bz2 ${DESTDIR}$(mandir)/man8/xsendkeys.8.bz2
else
   bin_PROGRAMS = xsendkeys xsendkeycode
if SEND_TO_KEYBOARD
install-data-local: send_to_keyboard.8.bz2 xsendkeycode.8.bz2 xsendkeys.8.bz2
	$(mkinstalldirs) $(DESTDIR)$(mandir)/man8/
	$(INSTALL_DATA) $(srcdir)/send_to_keyboard.8.bz2 ${DESTDIR}$(mandir)/man8/send_to_keyboard.8.bz2
else
install-data-local: xsendkeycode.8.bz2 xsendkeys.8.bz2
endif
	$(mkinstalldirs) ${DESTDIR}$(mandir)/man8/
	$(INSTALL_DATA) $(srcdir)/xsendkeycode.8.bz2 ${DESTDIR}$(mandir)/man8/xsendkeycode.8.bz2
	$(INSTALL_DATA) $(srcdir)/xsendkeys.8.bz2 ${DESTDIR}$(mandir)/man8/xsendkeys.8.bz2
endif

if SEND_TO_KEYBOARD
sbin_PROGRAMS = send_to_keyboard
send_to_keyboard_SOURCES = send_to_keyboard.c
send_to_keyboard.8.bz2:
	bzip2 -c send_to_keyboard.8 > send_to_keyboard.8.bz2
	$(mkinstalldirs) ${DESTDIR}$(mandir)/man8/
	chmod 755 $(DESTDIR)$(prefix)/sbin/send_to_keyboard
	if test "$(HELPER_SUID)" != "no" ; then \
	  chown root $(DESTDIR)$(prefix)/sbin/send_to_keyboard ; \
	  chmod +s $(DESTDIR)$(prefix)/sbin/send_to_keyboard ; \
	fi

endif
endif

xsendkeys_SOURCES = xsendkeys.cpp
xsendkeycode_SOURCES = xsendkeycode.c
#lsendcommand_SOURCES = lsendcommand.cpp

xsendkeys_LDADD = $(X_LIBS) -L$(top_srcdir)/lineak -lXtst -llineak -lX11
xsendkeycode_LDADD = $(X_LIBS) -L$(top_srcdir)/lineak -lXtst -lX11
#lsendcommand_LDADD = $(X_LIBS) -L$(top_srcdir)/lineak -llineak -lX11

xsendkeys_LDFLAGS = $(AM_CPPFLAGS) 
xsendkeycode_LDFLAGS = $(AM_CPPFLAGS)


# set the include path found by configure
INCLUDES= $(all_includes) -I$(top_srcdir)/lineak -I. -I..
DEFS = @DEFS@ -DCONFDIR=\"@sysconfdir@\" -DPLUGINDIR=\"@libdir@/lineakd/plugins\"
helper_suid = $(HELPER_SUID)

xsendkeycode.8.bz2:
	bzip2 -c xsendkeycode.8 > xsendkeycode.8.bz2
xsendkeys.8.bz2:
	bzip2 -c xsendkeys.8 > xsendkeys.8.bz2


uninstall-local:
	-rm -f ${DESTDIR}$(mandir)/man8/send_to_keyboard.8.bz2
	-rm -f ${DESTDIR}$(mandir)/man8/xsendkeycode.8.bz2
	-rm -f ${DESTDIR}$(mandir)/man8/xsendkeys.8.bz2
if EVTEST
	-rm -f ${DESTDIR}$(mandir)/man1/evtest.1.bz2
endif
