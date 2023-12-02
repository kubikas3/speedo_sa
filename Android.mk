LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_MODULE    := Speedo
SRC_FILES := $(shell dir /b /s src\*.cpp)
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)\%,%,$(SRC_FILES))
LOCAL_SRC_FILES += mod/logger.cpp mod/config.cpp
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_CFLAGS += -O2 -mfloat-abi=softfp -DNDEBUG -std=c++17
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)