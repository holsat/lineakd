METASOURCES = AUTO
AM_CPPFLAGS = -I$(top_srcdir)/lineakd -I. -I.. $(all_includes)
  
# set the include path found by configure
INCLUDES= $(all_includes) 
DEFS = @DEFS@ -DCONFDIR=\"@sysconfdir@\" -DPLUGINDIR=\"@libdir@/lineakd/plugins\" -DBINDIR=\"@bindir@\" -DSBINDIR=\"@sbindir@\"
  
lib_LTLIBRARIES = liblineak.la
  
liblineak_la_METASOURCES = AUTO
  
liblineak_la_SOURCES = pluginmanager.cpp globals.cpp displayctrl.cpp xmgr.cpp cdromctrl.cpp lineak_core_functions.cpp lineak_util_functions.cpp lockctrl.cpp lcommand.cpp saver.cpp msgpasser.cpp loader.cpp lobject.cpp lkey.cpp lbutton.cpp lkbd.cpp ldef.cpp lconfigdata.cpp lconfig.cpp defloader.cpp configloader.cpp configdirectives.cpp commandexec.cpp plugin_definitions.cpp


#if test \"$(target_os)\" = \"cygwin\"; then \
#   liblineak_la_LDFLAGS = -version-info 8:0:8 $(all_libraries) -L/usr/X11R6/lib @X_LIBS@ @X_PRE_LIBS@  -lpthread -lXt -lXext -lX11 -lSM -lICE -L/usr/X11R6/lib; \
#   liblineak_la_LIBADD   = -lX11 -lXext $(X_LIBS)  -lXt -lSM -lICE -L/usr/X11R6/lib; \
#else \
#   liblineak_la_LDFLAGS = -version-info 8:0:8 $(all_libraries); \
#fi;

liblineak_la_LIBADD  = -lX11 -lXext $(X_LIBS)  -lXt -lSM -lICE -L/usr/X11R6/lib
liblineak_la_LDFLAGS = -version-info 0 -release $(VERSION) $(all_libraries) -L/usr/X11R6/lib @X_LIBS@ @X_PRE_LIBS@  -lpthread
  
lineak_HEADERS = configdirectives.h configloader.h definitions.h defloader.h lconfig.h lconfigdata.h ldef.h lobject.h lkbd.h lkey.h lbutton.h loader.h msgpasser.h saver.h lcommand.h lockctrl.h lineak_util_functions.h lineak_core_functions.h cdromctrl.h xmgr.h displayctrl.h plugin_definitions.h commandexec.h pluginmanager.h
  
lineakdir = $(includedir)/lineak
