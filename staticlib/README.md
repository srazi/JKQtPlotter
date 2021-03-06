# JKQtPlotter
## STATICLIB subdirectory

This directory contains Projects that can be used to build a static library of JKQtPlotter (i.e. the contents of the directory [`../lib`](https://github.com/jkriege2/JKQtPlotter/tree/master/lib)

### QMake

#### Build shared libraries
There are several `.PRO`-files, that can be used to build the full library, or a limited subsets. They will produce a static link libraries that you can include into your projects, e.g. with the following QMake-snippet:

```qmake
# include JKQtPlotter library
DEPENDPATH += <PATHTOJKQTPLOTTERDIR>/lib
INCLUDEPATH += <PATHTOJKQTPLOTTERDIR>/lib
CONFIG (debug, debug|release) {
    LIBS += -L<PATHTOJKQTPLOTTERDIR>/staticlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L<PATHTOJKQTPLOTTERDIR>/staticlib/release -ljkqtplotterlib
}
```

This snippet assumes that you built the libraries with the provided `.PRO`-files.