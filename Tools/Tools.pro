TEMPLATE = subdirs

PtCore.subdir = $$SRC_DIR/Library/PtCore

PtGui.subdir = $$SRC_DIR/Library/PtGui

PtNetwork.subdir = $$SRC_DIR/Library/PtNetwork
PtNetwork.depends = PtCore

HttpEmulator.depends = PtGui PtNetwork

HashCalculator.depends = PtGui

FileScissors.depends = PtCore
SUBDIRS += \
    PtCore \
    PtGui \
    PtNetwork \
    HttpEmulator \
    HashCalculator \
    FileScissors
