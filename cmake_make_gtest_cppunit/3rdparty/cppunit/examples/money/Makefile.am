# Include cookbook.html in distro
EXTRA_DIST = money.dsp money.dsw configure.ac StdAfx.cpp

AM_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include

TESTS = MoneyApp
check_PROGRAMS = $(TESTS)

MoneyApp_SOURCES = Money.h MoneyTest.h MoneyTest.cpp MoneyApp.cpp StdAfx.h
MoneyApp_CXXFLAGS = $(CPPUNIT_CFLAGS)

# MoneyApp_LDADD = $(CPPUNIT_LIBS)
# USE THIS in your projects, not the line bellow

MoneyApp_LDADD = $(top_builddir)/src/cppunit/libcppunit.la
# this line is needed to build and test CPPUNIT itself.
