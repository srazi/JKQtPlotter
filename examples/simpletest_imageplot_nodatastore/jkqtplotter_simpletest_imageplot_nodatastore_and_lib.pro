TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_imageplot_nodatastore

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

jkqtplotter_simpletest_imageplot_nodatastore.file=$$PWD/jkqtplotter_simpletest_imageplot_nodatastore.pro
jkqtplotter_simpletest_imageplot_nodatastore.depends = jkqtplotterlib
