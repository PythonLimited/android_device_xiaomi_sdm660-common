ifneq (,$(filter $(QCOM_BOARD_PLATFORMS),$(TARGET_BOARD_PLATFORM)))
ifneq (, $(filter aarch64 arm arm64, $(TARGET_ARCH)))
ifneq ($(TARGET_USES_AOSP),true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../ipanat/inc
LOCAL_C_INCLUDES += external/icu4c/common
LOCAL_C_INCLUDES += external/dhcpcd
LOCAL_C_INCLUDES += bionic/libstdc++/include
LOCAL_C_INCLUDES += external/libxml2/include
LOCAL_C_INCLUDES += external/libnetfilter_conntrack/include
LOCAL_C_INCLUDES += external/libnfnetlink/include

LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
LOCAL_ADDITIONAL_DEPENDENCIES := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr


LOCAL_CFLAGS := -v
LOCAL_CFLAGS += -DFEATURE_IPA_ANDROID
LOCAL_CFLAGS += -DDEBUG

ifeq ($(TARGET_ARCH),arm)
LOCAL_CFLAGS += -include bionic/libc/kernel/arch-arm/asm/posix_types.h
LOCAL_CFLAGS += -include bionic/libc/kernel/arch-arm/asm/byteorder.h
endif

LOCAL_SRC_FILES := IPACM_Main.cpp \
		IPACM_EvtDispatcher.cpp \
		IPACM_Config.cpp \
		IPACM_CmdQueue.cpp \
		IPACM_Filtering.cpp \
		IPACM_Routing.cpp \
		IPACM_Header.cpp \
		IPACM_Lan.cpp \
		IPACM_Iface.cpp \
		IPACM_Wlan.cpp \
		IPACM_Wan.cpp \
		IPACM_IfaceManager.cpp \
		IPACM_Neighbor.cpp \
		IPACM_Netlink.cpp \
		IPACM_Xml.cpp \
		IPACM_Conntrack_NATApp.cpp\
		IPACM_ConntrackClient.cpp \
		IPACM_ConntrackListener.cpp \
                IPACM_Log.cpp

LOCAL_MODULE := ipacm
LOCAL_MODULE_TAGS := debug

LOCAL_SHARED_LIBRARIES := libipanat
LOCAL_SHARED_LIBRARIES += libxml2
LOCAL_SHARED_LIBRARIES += libnfnetlink
LOCAL_SHARED_LIBRARIES += libnetfilter_conntrack
LOCAL_SHARED_LIBRARIES += libicuuc
LOCAL_SHARED_LIBRARIES += libdhcpcd
include $(BUILD_EXECUTABLE)

################################################################################

define ADD_TEST

include $(CLEAR_VARS)
LOCAL_MODULE       := $1
LOCAL_SRC_FILES    := $1
LOCAL_MODULE_CLASS := ipacm
LOCAL_MODULE_TAGS  := debug
LOCAL_MODULE_PATH  := $(TARGET_OUT_ETC)
include $(BUILD_PREBUILT)

endef

IPACM_FILE_LIST := IPACM_cfg.xml
$(foreach TEST,$(IPACM_FILE_LIST),$(eval $(call ADD_TEST,$(TEST))))

endif # not (TARGET_USES_AOSP)
endif # $(TARGET_ARCH)
endif
