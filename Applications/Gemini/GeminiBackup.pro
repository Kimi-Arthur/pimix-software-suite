TEMPLATE = subdirs

PtCore.subdir = $$SRC_DIR/Library/PtCore

PtNetwork.subdir = $$SRC_DIR/Library/PtNetwork
PtNetwork.depends = PtCore

BaiduCloud.subdir = $$SRC_DIR/Modules/BaiduCloud
BaiduCloud.depends = PtCore PtNetwork

GeminiRunner.depends = BaiduCloud

SUBDIRS += \
    PtCore \
    PtNetwork \
    BaiduCloud \
    GeminiRunner

