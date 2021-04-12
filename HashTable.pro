TEMPLATE = subdirs
 
SUBDIRS = HashTableLib \
          HashTableTest \
          HashTableApp
 
# build the project sequentially as listed in SUBDIRS !
#CONFIG += ordered

# where to find the sub projects - give the folders
HashTableLib.subdir = HashTableLib
HashTableTest.subdir = HashTableTest
HashTableApp.subdir = HashTableApp
 
# what subproject depends on others
HashTableTest.depends = HashTableLib
HashTableApp.depends = HashTableLib
