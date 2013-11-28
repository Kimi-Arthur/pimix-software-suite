TEMPLATE = subdirs

PtCore.subdir = $$SRC_DIR/Library/PtCore
PtNetwork.subdir = $$SRC_DIR/Library/PtNetwork

BaiduCloudWorker.subdir = $$SRC_DIR/Modules/BaiduCloudWorker
BaiduCloudWorker.depends = PtCore PtNetwork

GeminiRunner.depends = BaiduCloudWorker

SUBDIRS += \
    PtCore \
    PtNetwork \
    BaiduCloudWorker \
    GeminiRunner

