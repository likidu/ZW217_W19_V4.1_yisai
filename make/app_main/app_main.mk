ifeq ($(strip $(SPRD_APP_RELEASE)), FALSE)
include make/app_main/app_macro.mk
else
include make/app_main/release_app_macro.mk
endif

include make/gui_main/gui_feature.mk

#--------------------------------------------SRC & INC-----------------------------------------------------------------#

MINCPATH = $(MMI_DIR)/source/mmi_app/app/accessory/h
MINCPATH += MS_Ref/source/nvm/inc
MINCPATH += MS_Ref/source/engpc/server/inc
MINCPATH += $(MMI_DIR)/source/mmi_app/app/audio/h
MINCPATH += MS_Ref/source/cam_sensor_drv/inc
MINCPATH += $(MMI_DIR)/source/mmi_app/app/bt/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/camera/h
ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/cc_adult/h
else
MINCPATH += $(MMI_DIR)/source/mmi_app/app/cc/h
endif
MINCPATH += $(MMI_DIR)/source/mmi_app/app/idle/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/cl/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/connection/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/cs/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/dcd/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/dl/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/dm/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/dynamic/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ebook/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/eng/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/engtd/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/envset/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/fdn/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/fm/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/fmm/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ussd/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/parse/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/json_paser/h

ifeq ($(strip $(ATEST_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/setting/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/accessory/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/im/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mainmenu/h
MINCPATH += $(MMI_DIR)/source/mmi_kernel/source/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Tab/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/title/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/im/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/iconfolder/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/progress/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/listbox/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/softkey/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/dropdownlist/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/editbox/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/setlist/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/toolbar/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/iconlist/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/button/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/msgbox/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/progressbar/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/textbox/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/richtext/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/label/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/scrollkey/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/tips/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/iconlist/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/form/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/ownerdraw/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/statusbar/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/anim/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/menu/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/mainmenu/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/basecontrol/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/typemanager/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/watchface/h
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
endif

ifneq ($(strip $(DRM_SUPPORT)),)
    ifneq ($(strip $(DRM_SUPPORT)), NONE)
    MINCPATH += $(MMI_DIR)/source/mmi_app/app/drm/h
    endif
endif

ifeq ($(strip $(GAME_SUPPORT)),TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_boxman/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_snake/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_quintet/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_link/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_manager/h
endif
ifeq ($(strip $(XYSDK_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/xysdk/h
MINCPATH += Third-party/xmly/api/h
endif
MINCPATH += $(MMI_DIR)/source/mmi_app/app/screensaver/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/keylock/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/wallpaper/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/taskmanager/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/im/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mainmenu/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mmi3d/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mms/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mobiletv/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/atv/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/audioplayer/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/msd/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/multim/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ocr/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/otapush/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pb/h
ifeq ($(strip $(MODEM_SUPPORT_SPRD)),TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp/h
else ifeq ($(strip $(MODEM_SUPPORT_RDA)),TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp_6121E/h
endif
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp_6121E/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/phone/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/plmn/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pic/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pic_viewer/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pim/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/setting/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/sms/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ss/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/stk/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/printscreen/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/takescreenphoto/h
#MINCPATH += $(MMI_DIR)/source/mmi_app/app/streaming/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/theme/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tp/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/udisk/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/vcard/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/vt/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/clipbrd/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/search/h
MINCPATH += $(MMI_DIR)/source/mmi_app/common/h
MINCPATH += $(MMI_DIR)/source/mmi_app/custom/h
MINCPATH += $(MMI_DIR)/source/mmi_app/kernel/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/include
MINCPATH += $(MMI_DIR)/source/mmi_gui/include
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += $(MMI_DIR)/source/mmi_spml/include
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
MINCPATH += $(MMI_DIR)/source/spim/include
MINCPATH += $(MMI_DIR)/source/pim/h
MINCPATH += $(MMI_DIR)/source/resource
MINCPATH += chip_drv/export/inc/outdated
MINCPATH += chip_drv/chip_plf/export/inc
MINCPATH += chip_drv/chip_plf/$(strip $(PLATFORM))
MINCPATH += CAF/Interface/source/resource
MINCPATH += CAF/Template/include
MINCPATH += DAPS/source/wbxml_parser/inc
MINCPATH += DAPS/source/wsp_header_decoder/inc
MINCPATH += DAPS/source/xml_expat/inc
MINCPATH += RTOS/export/inc
MINCPATH += MSL/l4/inc
MINCPATH += MS_Ref/export/inc
MINCPATH += MS_Ref/source/libcamera/common
MINCPATH += MS_Ref/source/libcamera/ispalg/common/inc
MINCPATH += MS_Ref/source/libcamera/ispalg/ae/sprd/inc
MINCPATH += MS_Ref/source/cam_drv/interface
MINCPATH += MS_Ref/source/isp_service/inc
MINCPATH += $(MMI_DIR)/source/mmi_app/app/launcher/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/slidepage/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/slidewin/h
ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
else
MINCPATH   += DAPS/export/inc/tcpip
endif
MINCPATH += MS_Ref/source/engpc/sprd_fts_inc 
MINCPATH += chip_drv/export/inc
MINCPATH += MS_MMI_Main/source/PARSER/export/inc
MINCPATH += Third-party/cstar/include
MINCPATH += $(ATC_INCPATH)
MINCPATH += $(L4_INCPATH)
MINCPATH += $(SIM_INCPATH)
MINCPATH += $(PS_INCPATH)
MINCPATH += $(L1_INCPATH)
#MINCPATH += BASE/PS/export/inc
#MINCPATH += BASE/Layer1/export/inc
ifeq ($(strip $(UAL_SUPPORT)),TRUE)
MINCPATH += ual/inc
endif
MINCPATH += ual/inc/bt
MINCPATH += ual/inc/tele
MINCPATH += ual/inc/device

ifeq ($(strip $(GPS_SUPPORT)),TRUE)
MINCPATH += MN_Gps/h
endif

ifeq ($(strip $(RECORD_SUPPORT)),TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/record/h
endif
ifeq (SOGOU, $(findstring SOGOU, $(strip $(IM_ENGINE))))
MINCPATH += Third-party/sogou/include
endif

ifeq ($(strip $(IM_ENGINE)), T9)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/im/t9/h
MINCPATH += Third-party/t9/include
endif

MINCPATH += Third-party/juniversalchardet/include
MINCPATH += Third-party/juniversalchardet/c/include

ifeq ($(strip $(IKEYBACKUP_SUPPORT)),TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ikeybackup/h
endif

ifeq ($(strip $(WRE_SUPPORT)),TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/wre_boot/h
MINCPATH += Third-party\zlib\include
ifeq ($(strip $(NES_SUPPORT)),TRUE)
MINCPATH += Third-party/nes/inc
MINCPATH += Third-party/nes
endif
endif

#API文件
#MINCPATH += CMMB/source/mbbms/inc
ifeq ($(strip $(KURO_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/kuro/h
endif

#ifeq ($(strip $(MMIWIDGET_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/widget/h
#endif

ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/widget/h
endif

ifeq ($(strip $(QBTHEME_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/widget/qbtheme/h
endif

ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/winse/h
endif

ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
MINCPATH += CMMB/export/inc
endif

ifeq ($(strip $(TTS_SUPPORT)), IFLYTEK)
MINCPATH += Third-party/InterSound4/h
MINCPATH += Third-party/InterSound4/Lib
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tts/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tts/iflytek/h
endif

ifeq ($(strip $(TTS_SUPPORT)), YOUNGTONE)
MINCPATH += Third-party/youngtone/inc
MINCPATH += Third-party/youngtone/h
MINCPATH += Third-party/youngtone/lib
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tts/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tts/youngtong/h
endif

ifeq ($(strip $(TTS_SUPPORT)), EJ)
MINCPATH += Third-party/ejTTS/h
MINCPATH += Third-party/ejTTS/LE_Lib
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tts/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tts/ej/h
endif

ifeq ($(strip $(CARD_LOG)), TRUE)
MINCPATH += MS_Ref/source/ulog/logsave/export/inc
endif

ifeq ($(strip $(DEV_MANAGE_SUPPORT)), TRUE)
ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
MINCPATH += Third-party/formal/Dm/import
MINCPATH += Third-party/formal/Dm/include
else
MINCPATH += Third-party/Dm/import
MINCPATH += Third-party/Dm/include
endif
endif

ifeq ($(strip $(BROWSER_SUPPORT_NETFRONT)), TRUE)
    ifeq ($(strip $(NETFRONT_SUPPORT)), VER35)
        ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
            MINCPATH += Third-party/formal/netfront/Netfront3.5/include
            MINCPATH += Third-party/formal/netfront/Netfront3.5/include/config
        else
            MINCPATH += Third-party/netfront/Netfront3.5/include
            MINCPATH += Third-party/netfront/Netfront3.5/include/config
        endif
    else
        ifeq ($(strip $(NETFRONT_SUPPORT)), VER40)
            ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
                MINCPATH += Third-party/formal/netfront/Netfront4.0/include
                MINCPATH += Third-party/formal/netfront/Netfront4.0/include/config
            else
                MINCPATH += Third-party/netfront/Netfront4.0/include
                MINCPATH += Third-party/netfront/Netfront4.0/include/config
            endif
        else
            ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
                MINCPATH += Third-party/formal/netfront/Netfront4.2/include
                MINCPATH += Third-party/formal/netfront/Netfront4.2/include/config
            else
                MINCPATH += Third-party/netfront/Netfront4.2/include
                MINCPATH += Third-party/netfront/Netfront4.2/include/config
            endif
        endif
    endif
endif

ifeq ($(strip $(VT_SUPPORT)), TRUE)
ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
MINCPATH += Third-party/formal/Mplapi/include
MINCPATH += Third-party/formal/Mplapi/Arm_lib/normal
else
MINCPATH += Third-party/Mplapi/include
MINCPATH += Third-party/Mplapi/Arm_lib/normal
endif
endif


#sun java api
ifeq ($(strip $(JAVA_SUPPORT)), SUN)
MINCPATH        +=      $(MMI_DIR)/source/mmi_app/app/java/h\
			$(MMI_DIR)/source/sun_java/h \
                        $(MMI_DIR)/source/sun_java/h/javacall/midp \
                        $(MMI_DIR)/source/sun_java/h/javacall/common \
                        $(MMI_DIR)/source/sun_java/h/javacall \
                        $(MMI_DIR)/source/sun_java/h/javacall/jsr135_mmapi \
                        $(MMI_DIR)/source/sun_java/h/javacall/jsr120_wma \
                        $(MMI_DIR)/source/sun_java/h/platform \
                        $(MMI_DIR)/source/sun_java/h/javacall/jsr75_pim_fc\
                        $(MMI_DIR)/source/sun_java/c/javacall/midp
endif

ifeq ($(strip $(JAVA_SUPPORT)), IA)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/h/jblend
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/h/jblend/gui/common
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/h/jblend/gui/inner
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/h/jblend/gui/user
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/h/extlib
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/h/ajsc
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/h/port
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/h/jas
endif

ifeq ($(strip $(JAVA_SUPPORT)), MYRIAD)
    PRIVATE_HEADER_DIR:=$(MMI_DIR)/source/mmi_app/app/myriad_java/jbed/h

    ifeq ($(strip $(JAVA_SUPPORT_TYPE)), SVM)
    PRIVATE_HEADER_DIR:=$(strip $(PRIVATE_HEADER_DIR))/svm
    endif

    ifeq ($(strip $(MODULES)), simulator)
    PRIVATE_HEADER_DIR:=$(strip $(PRIVATE_HEADER_DIR))/sim
    else
    PRIVATE_HEADER_DIR:=$(strip $(PRIVATE_HEADER_DIR))/tgt
    endif

    ifeq ($(strip $(JAVA_SUPPORT_CMCC)), TRUE)
    PRIVATE_HEADER_DIR:=$(strip $(PRIVATE_HEADER_DIR))/cmcc
    MINCPATH += $(MMI_DIR)/source/mmi_app/app/myriad_java/jbed/h/i18n
    endif

    MINCPATH += $(strip $(PRIVATE_HEADER_DIR))
    MINCPATH += $(MMI_DIR)/source/mmi_app/app/myriad_java/jbed/h
    MINCPATH += $(MMI_DIR)/source/mmi_app/app/myriad_java/mmijava/h
    MINCPATH += $(MMI_DIR)/source/mmi_app/app/myriad_java/jbed/h/preinstall
endif

ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
	ifeq ($(strip $(IM_HANDWRITING)), HANWANG)
		MINCPATH += Third-party/hw/include
	endif
else
	ifeq ($(strip $(IM_HANDWRITING)), HANWANG)
		MINCPATH += Third-party/hw/include
	endif
endif

ifeq ($(strip $(BROWSER_SUPPORT_NETFRONT)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/www/h
endif

ifeq ($(strip $(VT_SUPPORT)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/vt/h
endif

ifeq ($(strip $(BROWSER_SUPPORT)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/browser_manager/h
endif

ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/browser/h
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/browser/control/inc
endif
ifeq ($(strip $(SNS_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/sns/h
endif


ifeq ($(strip $(MMI_MOCOR_SAMPLE)), TRUE)
 MINCPATH		+=  $(MMI_DIR)/source/mmi_app/app/sample/h
endif

#ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
# MINCPATH +=    $(MMI_DIR)/source/mmi_app/app/streaming/h/nex_inc \
#                $(MMI_DIR)/source/mmi_app/app/streaming/h/nex_inc/nxsys
#endif

ifeq ($(strip $(OCR_SUPPORT)), TRUE)
 MINCPATH += Third-party/ocr/include
endif

#ifeq ($(strip $(GPS_SUPPORT)), TRUE)
# MINCPATH += $(MMI_DIR)/source/mmi_app/app/gps/h
#endif

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/videoplayer/h
endif

ifeq ($(strip $(KING_MOVIE_SUPPORT)), ALL)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/h
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), EXTEND)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/h
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), STAND)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/h
endif
endif
endif


ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
ifeq ($(strip $(MOBILE_VIDEO_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mobile_video/h \
            $(MMI_DIR)/source/mmi_app/app/mobile_video/h/include \
            $(MMI_DIR)/source/mmi_app/app/mobile_video/h/mv
endif
endif

ifeq ($(strip $(ASP_SUPPORT)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/asp/h
endif

ifeq ($(strip $(PUSH_EMAIL_SUPPORT)), TRUE)
MINCPATH  +=  $(MMI_DIR)/source/mmi_app/app/push_mail/h
MINCPATH  +=  $(MMI_DIR)/source/push_mail/comm/h
MINCPATH  +=  $(MMI_DIR)/source/push_mail/push_mail/h
endif

ifeq ($(strip $(CSC_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/customer/h
endif

#MINCPATH += $(PS_INCPATH) $(L1_INCPATH) $(ATC_INCPATH)

ifeq ($(strip $(TF_LOAD_SUPPORT)), TRUE)
MINCPATH += fdl_bootloader/tf_fdl/inc
endif

MSRCPATH = $(MMI_DIR)/source/mmi_app
MSRCPATH += $(MMI_DIR)/source/mmi_app/kernel/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/common/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/theme/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/accessory/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/audio/c
ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/cc_adult/c
else
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/cc/c
endif
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/cl/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/connection/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/ebook/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/eng/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/envset/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/fm/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/fmm/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/json_paser/c
ifneq ($(strip $(DRM_SUPPORT)),)
    ifneq ($(strip $(DRM_SUPPORT)), NONE)
    MSRCPATH += $(MMI_DIR)/source/mmi_app/app/drm/c
    endif
endif
ifeq ($(strip $(GAME_SUPPORT)),TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_boxman/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_snake/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_quintet/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_link/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_manager/c
endif
ifeq ($(strip $(XYSDK_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/xysdk/c
endif
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/screensaver/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/keylock/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/wallpaper/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/taskmanager/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/idle/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/mainmenu/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/mms/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/otapush/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/msd/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/pb/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/fdn/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/phone/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/plmn/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/pic/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/pic_viewer/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/setting/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/sms/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/stk/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/printscreen/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/takescreenphoto/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/udisk/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/vcard/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/vt/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/engtd/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/audioplayer/c
#MSRCPATH += $(MMI_DIR)/source/mmi_app/app/streaming/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/tp/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/bt/c
ifeq ($(strip $(MODEM_SUPPORT_SPRD)),TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/pdp/c
else ifeq ($(strip $(MODEM_SUPPORT_RDA)),TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/pdp_6121E/c
endif
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/pdp_6121E/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/dcd/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/cs/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/mmi3d/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/ocr/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/dynamic/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/im/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/dm/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/camera/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/mobiletv/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/atv/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/www/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/dl/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/ussd/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/parse/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/launcher/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/slidepage/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/slidewin/c

MINCPATH += MS_MMI_Main/source/mmi_ctrl/source/qrcode/h


ifeq ($(strip $(IM_ENGINE)), T9)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/im/t9/c
endif

ifeq ($(strip $(RECORD_SUPPORT)),TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/record/c
endif

ifeq ($(strip $(WRE_SUPPORT)),TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/wre_boot/c
endif

ifeq ($(strip $(IKEYBACKUP_SUPPORT)),TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/ikeybackup/c
endif

ifeq ($(strip $(JAVA_SUPPORT)), SUN)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/java/c
endif
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/pim/c
ifeq ($(strip $(BROWSER_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/browser_manager/c
endif
ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/browser/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/browser/control/src
endif
ifeq ($(strip $(SNS_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/sns/c
endif

MSRCPATH += $(MMI_DIR)/source/mmi_app/app/clipbrd/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/search/c

ifeq ($(strip $(JAVA_SUPPORT)), IA)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/c/port
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/c/port/ksc
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/c/port/jsc
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/c/jas
endif

ifeq ($(strip $(JAVA_SUPPORT)), MYRIAD)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/myriad_java/jbed/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/myriad_java/mmijava/c
endif

ifeq ($(strip $(SQLITE_SUPPORT)), TRUE)
MINCPATH += external/libc
MINCPATH += Third-party/sqlite
endif

#API文件
ifeq ($(strip $(KURO_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/kuro/c
endif

ifeq ($(strip $(GPS_SUPPORT)), TRUE)
MCFLAG_OPT += -D_MNGPS_SUPPORT_
#MSRCPATH		+=  $(MMI_DIR)/source/mmi_app/app/gps/c
MSRCPATH		+=  BASE/l4/export/inc
endif

ifeq ($(strip $(ASP_SUPPORT)), TRUE)
MSRCPATH		+=  $(MMI_DIR)/source/mmi_app/app/asp/c
endif

ifeq ($(strip $(MMI_MOCOR_SAMPLE)), TRUE)
MSRCPATH		+=  $(MMI_DIR)/source/mmi_app/app/sample/c
endif

ifeq ($(strip $(TTS_SUPPORT)), IFLYTEK)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/tts/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/tts/iflytek/c
endif

ifeq ($(strip $(TTS_SUPPORT)), YOUNGTONE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/tts/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/tts/youngtong/c
endif

ifeq ($(strip $(TTS_SUPPORT)), EJ)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/tts/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/tts/ej/c
endif

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
 MSRCPATH += $(MMI_DIR)/source/mmi_app/app/videoplayer/c
endif

ifeq ($(strip $(KING_MOVIE_SUPPORT)), ALL)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/c
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), EXTEND)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/c
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), STAND)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/c
endif
endif
endif


ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
ifeq ($(strip $(MOBILE_VIDEO_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/mobile_video/c \
            $(MMI_DIR)/source/mmi_app/app/mobile_video/c/mv
endif
endif

ifeq ($(strip $(MMIWIDGET_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/widget/c
endif

ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/widget/c
endif

ifeq ($(strip $(QBTHEME_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/widget/qbtheme/c
endif
ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/winse/c
endif
ifeq ($(strip $(PUSH_EMAIL_SUPPORT)), TRUE)
MSRCPATH  +=  $(MMI_DIR)/source/mmi_app/app/push_mail/c
endif

ifeq ($(strip $(CSC_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/customer/c
endif

ifeq ($(strip $(GOODIX_SUPPORT)), GR5515)
MINCPATH += Third-party/goodix/inc
MINCPATH += MS_Ref/export/inc/sensor
MINCPATH += MS_Ref/source/ucom/inc
endif

MINCPATH +=  build/$(PROJECT)_builddir/tmp

ifeq ($(strip $(CARD_LOG)),TRUE)
MINCPATH += BASE/logsave/export/inc
endif

SOURCES	       += app_tasks.c block_mem.c mmi_atc.c mmi_atc_wintab.c mmi_filetask.c mmi_modu_main.c mmi_osbridge.c mmi_port.c mmi_res_outside.c\
                mmi_rglexpr.c mmimain.c mmi_nv.c mmi_default.c mmi_menutable.c \
                mmiparse_dial.c mmidisplay_data.c mmi_applet_table.c\
                mmi_solarlunar.c mmiacc_winmain.c mmicalc_main.c mmiwclk_main.c mmialarm.c mmialarm_service.c mmicalendar_main.c mmiacc_menutable.c \
                mmialarm_menutable.c mmischedule.c mmiacc_nv.c mmialarm_nv.c  mmischedule_atc.c  mmialarm_clock.c mmialarm_jump_clock.c\
                mmiaudio.c mmiaudio_ctrl.c mmiaudio_vibrate.c mmicc_new_app.c mmicc_audio.c mmicc_nv.c mmicc_speeddial_wintab.c mmiussd.c mmicc_menutable.c mmicc_speeddial.c  mmicc_main.c mmicc_nops_ecc.c \
				watch_cc_view.c watch_pb_view.c watch_cl_view.c watch_cc_dialpad.c\
                mmicl.c mmicl_costs_wintab.c mmicl_wintab.c mmicl_ldn.c mainapp.c mmiidle_hwicon.c mmi_subwintab.c window_parse.c mmi_applet_manager.c mmi_applet_conflict.c\
                mmiconn_manager_wintab.c mmiconnection_menutable.c mmiconnection.c mmitv_out.c \
                mmiebook_file.c mmiebook_nv.c mmiebook_wintab.c mmiebook_menutable.c mmiebook_task.c \
                mmiebook_grid.c mmiebook_bookshelf.c \
                mmieng_main.c mmieng_nv.c mmieng_win.c mmieng_menutable.c mmieng_uitestwin.c mmieng_base.c\
                mmieng_dynamic_main.c \
                mmienvset.c mmienvset_wintab.c \
                mmifmm.c mmifmm_menutable.c mmifmm_id.c mmifmm_interface.c mmifmm_nv.c mmifmm_pic.c mmifmm_srv.c mmifmmsrv_api.c mmifmm_lsearch.c \
                mmifmm_app.c mmifmm_autotest.c mmifmm_comfunc.c mmifmm_comwin.c mmifmm_copywin.c mmifmm_createwin.c mmifmm_deletewin.c mmifmm_detailwin.c mmifmm_drm.c \
                mmifmm_findfilewin.c mmifmm_mainwin.c mmifmm_mem.c mmifmm_movewin.c mmifmm_picwin.c mmifmm_renamewin.c mmifmm_sd.c mmifmm_sort.c mmifmm_videowin.c mmifmm_vthumbnail.c \
                mainmenu_win.c mmiidle_istyle.c \
                mmitheme_slider.c mmitheme_progress.c mmitheme_lettersbar.c mmitheme_combox.c\
                mmiota_file.c mmiota_id.c mmiota_main.c mmiota_menutable.c mmiota_wintable.c \
                mmimsd.c mmimsd_wintab.c mmi_filemgr.c \
                mmipb_atc.c  mmipb_view.c\
                mmipb_adapt.c mmipb_interface.c mmipb_storage.c mmipb_menu.c  mmipb_app.c mmipb_task.c mmipb_datalist.c  mmipb_mem.c\
		            mmipb_nv.c mmipb_search.c mmifdn.c mmipb_set.c\
                mmiphone.c mmiphone_wintab.c mmiphone_nitz.c mmiphone_network_name.c mmiphone_charge.c mmiphone_onoff.c mmi_startup.c mmiplmn_export.c mmiplmn_info.c mmiphone_pinpuk.c\
                mmi_pubwin.c mmi_pubeditwin.c mmi_pubmenuwin.c mmi_pubtextwin.c mmi_pubrichtextwin.c mmi_publistwin.c mmi_pubformwin.c\
                mmi_setlist_win.c \
                mmiset_call.c mmiset_func.c mmiset_nv.c mmiset_security.c mmiset_display.c mmiset_menutable.c mmiset_ring.c mmiset_wintab.c mmiset_lock_effect.c \
                mmiset_shortcut.c mmiset_mainmenu.c mmiset_longpower.c mmiset_datetime.c mmiset_tts.c mmiset_screensaver.c mmiset_theme.c mmiset_displaywin.c mmiset_phonewin.c mmiset_mv.c mmiset_callwin.c mmiset_sos.c\
                watch_set_main.c watch_set_brightness.c watch_set_sound.c watch_set_mmsound.c watch_set_notesound.c watch_set_more.c watch_set_clock.c watch_set_about.c  watch_set_backlight.c\
                watch_extra_main.c\
                mmisms_api.c mmisms_nv.c mmismsapp_order.c mmismscb_control.c mmisms_menutable.c mmismsapp_main.c mmismscb_wintab.c mmisms_delallwin.c mmisms_settingwin.c mmisms_appcontrol.c mmisms_securitywin.c mmisms_messagecountwin.c mmisms_pushmsgboxwin.c mmisms_simboxwin.c mmisms_showsmswin.c\
                mmisms_delete.c mmisms_edit.c mmisms_read.c mmisms_receive.c mmisms_save.c mmisms_send.c mmisms_set.c mmisms_voicemail.c mmisms_filewin.c mmisms_filecontrol.c mmisms_timermsg_win.c mmisms_timermsg_control.c mmisms_chatreadwin.c mmisms_mainmenuwin.c mmisms_messageboxwin.c mmisms_commonui.c\
		            mmisms_voicemailwin.c mmisms_phizlistwin.c mmisms_emojilistwin.c mmisms_wordlistwin.c mmisms_editsmswin.c mmisms_sendpromptwin.c mmisms_sendnumoptwin.c mmismsapp_interface.c watch_sms_main.c watch_sms_presetlist.c watch_sms_message.c watch_sms_chat.c\
                mmistk_app.c mmistk_win.c mmitheme_anim.c mmitheme_iconfolder.c mmitheme_iconlist.c mmitheme_menu.c mmitheme_prgbox.c mmitheme_pubwin.c mmi_theme.c mmitheme_tab.c mmitheme_label.c \
                mmitheme_statusbar.c mmitheme_tips.c mmitheme_dropdownlist.c mmitheme_edit.c mmitheme_im.c mmitheme_text.c mmitheme_setlist.c mmitheme_form.c mmitheme_button.c mmitheme_checkbox.c mmitheme_picker.c \
                mmitheme_block.c mmitheme_pos.c mmitheme_update.c mmitheme_toolbar.c mmitheme_softkey.c mmitheme_list.c mmitheme_title.c mmitheme_richtext.c mmitheme_coverbar.c mmitheme_graph_data.c mmitheme_scroll.c\
                mmi_sd.c mmi_sd_plug.c mmiudisk.c mmiudisk_wintab.c \
                mmivcard.c vf_create_object.c vf_modified.c vf_strings.c vf_access.c vf_delete.c vf_parser.c vf_writer.c \
                vf_access_calendar.c vf_malloc.c vf_search.c vf_access_wrappers.c vf_malloc_stdlib.c vf_string_arrays.c \
                mmipdp_api.c mmipdp_list.c mmipdp_ps_adap.c \
                coordinate_wintab.c tp_func.c tp_nv.c \
                mmi_id.c mmiacc_id.c mmicc_id.c mmiconnection_id.c mmieng_id.c \
                mmipb_id.c mmiset_id.c mmisms_id.c mmistk_id.c tp_id.c mmialarm_id.c mmiebook_id.c \
                mmimp_location.c mmiphonesearch_wintab.c \
                mmiset_multisim.c mmicountedtime_main.c mmistopwatch_main.c mmiflashlight.c mmi_timer_main.c\
                mmicom_panel.c mmicom_extract.c watch_common_btn.c watch_common_list.c watch_common_progressbar.c watch_listWin_demo.c watch_common.c watch_common_radiolist_win.c \
                watch_commonwin_internal.c watch_commonwin_query.c watch_commonwin_note.c watch_commonwin_waiting.c\
                mmitheme_special.c mmitheme_special_func.c mmitheme_layer_special.c mmitheme_frame_engine.c \
                mmiidle_shortcut.c mmiidle_dial.c\
                mmiss_screensaver.c mmiss_seed.c mmiss_snake.c mmiss_special.c mmiss_star.c \
                mmidyna_win.c mmidyna_id.c mmidyna_menutable.c mmi_resource.c mmi3d.c mmicamera_manager.c mmidv_manager.c \
                mmiunitconversion.c mmi_autotest.c mmiptscr_main.c mmiptscr_save.c \
                mmitheme_clipbrd.c mmiclipbrd.c mmiclipbrd_id.c mmiclipbrd_menutable.c mmiclipbrd_wintab.c mmiclipbrd_toolbar.c mmiclipbrd_slider.c mmiclipbrd_magnify.c\
                mmisearch_app.c mmisearch_searchapplication.c mmisearch_wintab.c mmisearch_main.c mmisearch_id.c mmisearch_menutable.c mmivcalendar.c mmivc_common.c\
                mmiidle_func.c mmiidle_statusbar.c mmiidle_cstyle.c mmiidle_display.c mmitakescrphoto_main.c \
				mmi_gettextstring.c JSON_parser.c \
                mmi_mainmenu_data.c mmi_mainmenu_item_func.c mmi_mainmenu_matrix.c mmitheme_mainmenu.c mmi_mainmenu_pm.c mmi_mainmenu_istyle.c \
                mmi_mainmenu_qbtheme.c mmi_mainmenu_internal.c mmi_mainmenu_synchronize.c mmi_mainmenu_istyle_u_se.c  \
                mmieng_factorytest.c watch_qrcode_win.c
		
ifeq ($(strip $(_SW_ZDT_PRODUCT_)), TRUE)
#SOURCES += mmiset_about.c mmiwatchidle_effect0.c mmiwatchidle_effect1.c mmiwatchidle_util.c mmiwatchidle_main.c 
endif

ifeq ($(strip $(MMI_KEY_LOCK_SUPPORT)),TRUE)
SOURCES	 += mmikl_keylock.c
endif
ifeq ($(strip $(MMI_WALLPAPER_SUPPORT)), TRUE)
SOURCES  += mmi_wallpaper.c
SOURCES  += mmifmm_wallpaper.c
SOURCES  += mmiset_wallpaper.c
SOURCES  += mmiset_videowallpaper.c
endif
ifeq ($(strip $(OLD_CC_SUPPORT)), TRUE)
SOURCES	 += mmicc_wintab.c
endif
ifeq ($(strip $(OLD_STOPWATCH_SUPPORT)), TRUE)
SOURCES	 += mmistopwatch_main.c
endif
ifeq ($(strip $(STOPWATCH_SUPPORT)), TRUE)
SOURCES	 += watch_stopwatch.c
endif
ifeq ($(strip $(DROPDOWN)), NOTIFY_SHORTCUT)
SOURCES	 += watch_sms_notifyconnect.c 
endif

ifeq ($(strip $(SCREEN_SHAPE)), CIRCULAR)
SOURCES	 +=  watch_alarm_main.c watch_alarm_edit.c
else
SOURCES	 += watch_set_alarmlist.c  watch_set_alarmedit.c
endif

SOURCES	 += watch_commonwin_progress.c

ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
SOURCES	 += watch_set_display.c watch_set_wear.c watch_set_system.c
SOURCES	 += watch_set_call_forwarding.c set_ss_flow.c
endif
ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
SOURCES	 += watch_flashlight.c
endif
ifeq ($(strip $(GPS_SUPPORT)), TRUE)
ifeq ($(strip $(MMI_WIFI_SUPPORT)), TRUE)
SOURCES	 += mmieng_gps_ota.c
endif
endif
###  mmimenu_qbtheme.c  mmimenu_cube.c mmimenu_crystalcube.c mmimenu_cyclone.c cyclone.c mmimenu_icon.c mmimenu_internal.c
###  mmimenu_istyle.c mmimenu_optionpage.c mmimenu_rotation.c mmimenu_slide_page.c mmimenu_u_mainmenu_se.c

ifeq ($(strip $(GAME_SUPPORT)), TRUE)
SOURCES	 += mmigame_boxman_func.c mmigame_boxman_nv.c mmigame_boxman_menutable.c mmigame_boxman_wintab.c \
                mmigame_snake_func.c mmigame_snake_nv.c mmigame_snake_menutable.c mmigame_snake_wintab.c \
                mmigame_quintet_func.c mmigame_quintet_menutable.c mmigame_quintet_nv.c mmigame_quintet_wintab.c\
                mmigame_link_func.c mmigame_link_nv.c mmigame_link_menutable.c mmigame_link_wintab.c \
                mmigame.c mmigame_menutable.c mmigame_wintab.c mmigame_boxman_id.c mmigame_link_id.c mmigame_snake_id.c mmigame_id.c
endif

ifeq ($(strip $(XYSDK_SUPPORT)), TRUE)
SOURCES	 += xmly_config_info.c 
endif

ifeq ($(strip $(GUIF_SCROLLKEY)), TRUE)
SOURCES	 += mmitheme_scrollkey.c
endif

ifeq ($(strip $(RECORD_SUPPORT)),TRUE)
ifeq ($(strip $(SCREEN_SHAPE)), CIRCULAR)
SOURCES	 += adult_watch_record_mainwin.c adult_watch_record_filelistwin.c adult_watch_record_playwin.c
else
SOURCES	 += watch_record_mainwin.c watch_record_filelistwin.c watch_record_playwin.c
endif

SOURCES	 += watch_record_optlistwin.c   mmirecord_appcontrol.c mmirecord_main.c \
            mmirecord_menutable.c mmirecord_nv.c  mmirecord_id.c mmirecord_api.c mmirecord_setting.c
           ifeq ($(strip $(MMI_GUI_STYLE_TYPICAL)),TRUE)
           SOURCES	 += mmirecord_typical_wintab.c
           else
           ifeq ($(strip $(TOUCHPANEL_TYPE)), NONE)
           SOURCES	 += mmirecord_barphone_wintab.c
           else
           SOURCES	 += mmirecord_wintab.c
           endif
           endif
endif
ifneq ($(strip $(BLUETOOTH_SUPPORT)), NONE)
      SOURCES	 +=  mmibt_a2dp.c mmibt_menutable.c mmibt_nv.c mmibt_func.c  mmibt_app.c mmibt_id.c \
      				mmibt_devicelist.c mmibt_mainwin.c mmibt_editwin.c mmibt_devicedetails.c mmibt_filetransfer.c \
      				mmibt_inquireddevice.c mmibt_fileincoming.c mmibt_deviceservice.c mmibt_savelocation.c \
      				mmibt_setvisibility.c mmibt_sharedfile.c mmibt_uitest.c mmibt_headset.c watch_bt_main_win.c
    ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
          SOURCES     +=adultwatch_bt_win.c adultwatch_bt_pair.c adultwatch_bt_view.c adultwatch_bt_name_win.c adultwatch_bt_pininput_win.c
        ifneq ($(strip $(BBM_ROLE_SUPPORT)), NONE)
            SOURCES     +=watch_set_bt_ble_manage.c
        endif
    endif
endif
ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			SOURCES	 +=     mmi_mainmenu_slide_page.c
endif

ifneq ($(strip $(DRM_SUPPORT)),)
    ifneq ($(strip $(DRM_SUPPORT)), NONE)
    SOURCES	 +=     mmi_drmfilemgr.c mmidrm.c mmidrm_export.c mmidrm_nv.c
    	ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)
    	SOURCES	 += mmipicview_drm.c
    	endif
    endif
endif


ifeq ($(strip $(KING_MOVIE_SUPPORT)), ALL)
MINCPATH  +=  MS_Ref/source/video_security/export
MINCPATH  +=  MS_Ref/source/video_security_kmv/export
MINCPATH  +=  MS_Ref/source/video_security_hmv/export
SOURCES	 += mmikm_wintab.c mmikm_main.c mmikm_menutable.c mmikm_playlist.c
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), EXTEND)
MINCPATH  +=  MS_Ref/source/video_security/export
MINCPATH  +=  MS_Ref/source/video_security_kmv/export
SOURCES	 += mmikm_wintab.c mmikm_main.c mmikm_menutable.c mmikm_playlist.c
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), STAND)
MINCPATH  +=  MS_Ref/source/video_security/export
SOURCES	 += mmikm_wintab.c mmikm_main.c mmikm_menutable.c mmikm_playlist.c
endif
endif
endif
MINCPATH += MS_Ref/source/isp_service/inc
MINCPATH += MS_Ref/source/image_proc/inc
MINCPATH  +=  MS_Ref/source/ulog/inc

ifeq ($(strip $(MEDIA_DEMO_SUPPORT)), TRUE)

MSRCPATH  +=  $(MMI_DIR)/source/mmi_app/app/eng/media_demo/audio_demo/src
MINCPATH  +=  $(MMI_DIR)/source/mmi_app/app/eng/media_demo/audio_demo/data
MINCPATH  +=  $(MMI_DIR)/source/mmi_app/app/eng/media_demo/audio_demo/inc

MSRCPATH  +=  $(MMI_DIR)/source/mmi_app/app/eng/media_demo/video_demo/src
MINCPATH  +=  $(MMI_DIR)/source/mmi_app/app/eng/media_demo/video_demo/data
MINCPATH  +=  $(MMI_DIR)/source/mmi_app/app/eng/media_demo/video_demo/inc



MINCPATH += MS_Ref/source/dc/dc_6x00/inc
MINCPATH += MS_Ref/source/isp_service/inc
MINCPATH += MS_Ref/source/dc/dc_common/inc
MINCPATH += chip_drv/export/inc/outdated
MINCPATH += chip_drv/chip_plf/export/inc
MINCPATH += MS_Ref/source/image_proc/inc

SOURCES += display_yuv_demo.c

SOURCES	+= aac_stream_demo.c amr_stream_demo.c amr_record_demo.c audio_express_demo.c mp3_stream_demo.c \
           wav_stream_demo.c

SOURCES += dc_video_demo.c


ifeq ($(strip $(CAT_MVOICE_SUPPORT)), TRUE)
SOURCES			+= cat_mvoice_demo.c
endif

ifeq ($(strip $(MIC_SAMPLE_SUPPORT)), TRUE)
MCFLAG_OPT		+=     -DMIC_SAMPLE_DEMO_SUPPORT
SOURCES			+= mic_sample_demo.c
endif

endif

#API文件

SOURCES	 +=     mmipim_export.c mmiocr_export.c \
                mmidm_export.c mmiengtd_export.c mmifm_export.c mmidcd_export.c mmimms_export.c mmipb_export.c mmipic_edit_export.c \
                mmiudisk_export.c tp_export.c mmibt_export.c mmimtv_export.c mmiatv_export.c

ifeq ($(strip $(KURO_SUPPORT)), TRUE)
SOURCES	 += mmikur.c
endif
#if IM_ENGINE is t9, just add file which t9 needs
ifeq ($(strip $(IM_ENGINE)), T9)
		SOURCES	 +=   mmiim_im_switching.c \
									mmiim_base.c mmiim.c mmiim_wintab.c mmiim_id.c mmiim_nv.c mmiim_menutable.c \
								  mmiim_sp_basic.c mmiim_sp_tp_basic.c mmiim_sp_symbol.c mmiim_sp_symbol_data.c \
								  mmiim_sp_multitap.c mmiim_sp_ml_data.c mmiim_sp_ml9key_data.c mmiim_sp_ml26key_data.c

		SOURCES	 +=     mmiim_t9.c mmiim_t9_func.c mmiim_t9_base.c t9lang.c t9gen.c t9cgen.c
		ifeq ($(strip $(IM_SIMP_CHINESE_SUPPORT)), TRUE)
			SOURCES	 +=   mmiim_t9_chs.c
		endif
else
		SOURCES	 +=   mmiim_im_switching.c mmiim_tp_ui.c \
							mmiim_base.c mmiim.c mmiim_wintab.c mmiim_id.c mmiim_nv.c mmiim_menutable.c \
						  mmiim_sp_basic.c mmiim_sp_tp_basic.c mmiim_sp_hw.c mmiim_sp_symbol.c mmiim_sp_symbol_data.c \
						  mmiim_sp_multitap.c mmiim_sp_ml_data.c mmiim_sp_ml9key_data.c mmiim_sp_ml26key_data.c \
						  mmiim_tp_multitap_26key.c mmiim_tp_multitap_9key.c
ifeq (SOGOU, $(findstring SOGOU, $(strip $(IM_ENGINE))))
#SOURCES	 +=     mmiim_sp_stroke.c mmiim_sp_py9key.c mmiim_sp_en9key.c mmiim_tp_sp.c mmiim_sp_py26key.c mmiim_sp_en26key.c
MSRCPATH	+= $(MMI_DIR)/source/mmi_app/app/im/sogou/c
SOURCES	 +=     mmiim_sogou.c  mmiim_tp_sogou.c wdp_encrpty.c wdp_update.c wdp_sg_zhanxunfunc.c wdp_update_wintab.c \
						  wdp_SG_Interface_net.c mmiim_tp_sogou_keyboard_cfg.c
SOURCES   += wd_publicIme.c SG_English_Public.c  SG_Bengali_Public.c \
			SG_Hindi_Public.c SG_Indonesian_Public.c   SG_Arabic_Public.c SG_French_Public.c SG_Malay_Public.c SG_Persian_Public.c  SG_Portuguese_Public.c \
			SG_Russian_Public.c  SG_Spanish_Public.c  SG_Thai_Public.c  SG_Turkish_Public.c  SG_Vietnamese_Public.c SG_Urdu_Public.c  SG_Tagalog_Public.c \
			SG_Slovenian_Public.c SG_Romanian_Public.c SG_Italian_Public.c  SG_Hungarian_Public.c  SG_Greek_Public.c  SG_German_Public.c  SG_Czech_Public.c \
			SG_Hebrew_Public.c  SG_Swahili_Public.c SG_Polish_Public.c SG_Dutch_Public.c SG_Bulgarian_Public.c SG_Telugu_Public.c SG_Danish_Public.c SG_Swedish_Public.c \
			SG_Finnish_Public.c  SG_Slovak_Public.c SG_Marathi_Public.c SG_Tamil_Public.c SG_Burmese_Public.c SG_Gujarati_Public.c SG_Hausa_Public.c \
			SG_Khmer_Public.c SG_Lao_Public.c SG_Punjabi_Public.c SG_Uighur_Public.c SG_Sinhalese_Public.c
		#SOURCES	 +=     mmiim_sp_tp_common.c mmiim_sp_tp_en26key.c mmiim_sp_tp_abc26key.c mmiim_sp_tp_digital.c \
		#                mmiim_sp_tp_py26key.c mmiim_sp_tp_stroke.c mmiim_sp_tp_en9key.c mmiim_sp_tp_abc9key.c
		else
		SOURCES	 +=     mmiim_cstar.c  mmiim_tp_cstar.c mmiim_tp_cstar_keyboard_cfg.c
		endif
endif
ifeq ($(strip $(IM_HANDWRITING)), SOGOU)
SOURCES   += wd_hwDatas.c
endif

###use pda im, when exist touch panel
ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
SOURCES	 +=     mmiim_touch_draw.c mmiim_touch_common.c mmiim_touch_ime_common.c mmiim_touch_set.c
  ifeq (SOGOU, $(findstring SOGOU, $(strip $(IM_ENGINE))))
      SOURCES	 +=     mmiim_touch_ime_sogou.c mmiim_touch_ime_hw.c mmiim_touch_app_sogou.c
  else
      ifeq ($(strip $(IM_ENGINE)), CSTAR)
        SOURCES	 +=     mmiim_touch_ime_cstar.c mmiim_touch_ime_hw.c mmiim_touch_app_cstar.c
      else
        SOURCES	 +=     mmiim_touch_app_sprd.c
      endif
  endif
endif

ifeq ($(strip $(UPDATE_TIME_SUPPORT)),TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/aut/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/aut/c
SOURCES	 += mmiaut_wintab.c  mmiaut_net.c  mmiaut_export.c
endif

MINCPATH += $(MMI_DIR)/source/mmi_app/app/datashare/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/datashare/c
SOURCES  += mmidata_share.c

### mmi slide support
SOURCES	 += mmi_slide.c  #.c和.h均用MMI_SLIDE_SUPPORT 控制，如果MMI_SLIDE_SUPPORT屏蔽，则所有slide相关code不参与编译和link

ifeq ($(strip $(CMCC_UI_STYLE)), TRUE)
SOURCES	 +=     mmics_export.c mmics_id.c mmics_main.c mmics_menutable.c mmics_nv.c mmics_wintab.c
endif


ifeq ($(strip $(APP_PRODUCT_DM)), TRUE)
SOURCES	 +=     mmiengtd_menutable.c mmiengtd_win.c mmiengtd_nv.c mmiengtd_main.c mmiengtd_id.c
endif

ifeq ($(strip $(TTS_SUPPORT)), IFLYTEK)
SOURCES	 +=     TTS_api.c tts_iflytek_api.c tts_iflytek_audio.c
endif

ifeq ($(strip $(TTS_SUPPORT)), YOUNGTONE)
SOURCES	 +=     TTS_api.c tts_youngtong_api.c tts_youngtong_audio.c youngtone_tts_interface.c  yte_sms.c
endif

ifeq ($(strip $(TTS_SUPPORT)), EJ)
SOURCES	 +=    TTS_api.c ejTTSPlayer_audio.c file_func.c ejTTSPlayer_api.c
endif

ifneq ($(strip $(FM_SUPPORT)), FALSE)
ifneq ($(strip $(FM_SUPPORT)), NONE)
SOURCES	 +=     mmifm_app.c mmifm_nv.c mmifm_wintab.c mmifm_id.c
endif
endif

ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
SOURCES	 +=     mmipic_edit.c mmipic_edit_wintab.c mmipic_id.c mmipic_menutable.c
endif

ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)
SOURCES	 +=    mmipicview.c mmipicview_id.c mmipicview_menutable.c mmipicview_nv.c mmipicview_list.c mmipicview_preview.c mmipicview_wintab.c  mmipicview_zoom.c watch_gallery_main.c watch_gallery_vp.c watch_gallery_pic.c
else
SOURCES	 +=    mmipicview_export.c
endif

ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
SOURCES	 +=     mmimtv_data.c mmimtv_main.c mmimtv_nv.c mmimtv_osd_display.c mmimtv_wintab.c mbbms_api_dummy.c mmimbbms_main.c mmimbbms_wintab.c mmimtv_simu.c mmimtv_menutable.c
endif

ifneq ($(strip $(ATV_SUPPORT)), NONE)
SOURCES	 +=     mmiatv_data.c mmiatv_main.c mmiatv_wintab.c mmiatv_nv.c mmiatv_menutable.c mmiatv_simu.c mmiatv_fsm.c
endif

ifeq ($(strip $(KURO_SUPPORT)), TRUE)
SOURCES	 +=     kdbparser.c mmikur_bt.c mmikur_lyric.c mmikur_menutable.c mmikur_musicstyle.c mmikur_nv.c mmikur_wintab.c kuro_depends.c
endif

ifeq ($(strip $(UI_SPRITE_ENABLE)), TRUE)
SOURCES	 +=     mmiss_sprite.c
endif

#ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
#SOURCES	 +=     mmist_id.c tester_cal_ral.c mmist.c mmist_nv.c app_common_event_handlers.c mmist_wintab.c nexsal.c \
#                app_reg_sys2sal.c nexcr_bbamr.c app_streaming_event_handlers.c mmist_menutable.c nexcr_mv8602.c
#endif


ifeq ($(strip $(OCR_SUPPORT)), TRUE)
SOURCES	 +=     mmiocr_id.c mmiocr.c mmiocr_lib.c mmiocr_wintab.c
endif

#ifeq ($(strip $(VT_SUPPORT)), TRUE)
SOURCES	 +=    mmivt_kernel_324adapter.c mmivt_kernel_process.c mmivt_kernel_refadapter.c mmivt_main.c \
                mmivt_ui.c mmivt_id.c mmivt_nv.c
#endif


ifneq ($(strip $(QQ_SUPPORT)), NONE)
ifeq ($(strip $(QQ_SUPPORT)),2009)
MCFLAG_OPT	+= -DQQ_SUPPORT_2009
endif
MSRCPATH 	+= $(MMI_DIR)/source/mmi_app/app/qq/c
MINCPATH 	+= $(MMI_DIR)/source/mmi_app/app/qq/h
SOURCES	+= mmiqq_ui.c  mmiqq_file.c mmiqq_input.c mmiqq_main.c mmiqq_socket.c mmiqq_system.c
endif

ifeq ($(strip $(MET_MEX_SUPPORT)), TRUE)

MINCPATH += $(MMI_DIR)/source/mmi_app/app/met_mex/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/met_mex/Lib
MINCPATH += $(MMI_DIR)/source/mmi_app/app/met_mex/System

MSRCPATH += $(MMI_DIR)/source/mmi_app/app/met_mex/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/met_mex/Lib
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/met_mex/System
SOURCES  += met_mex_wintab.c MexConfig.c MexInterface.c met_mex_export.c
ifeq ($(strip $(MET_MEX_AQ_SUPPORT)), TRUE)
SOURCES    += met_mex_aq.c
endif
endif

ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
MSRCPATH 	+= $(MMI_DIR)/source/mmi_app/app/weather/c
MINCPATH 	+= $(MMI_DIR)/source/mmi_app/app/weather/h

SOURCES	    += mmiweather.c mmiweather_export.c mmiweather_funcs.c mmiweather_id.c mmiweather_menutable.c \
               mmiweather_nv.c mmiweather_parse.c mmiweather_win.c mmiweather_special_citylist.c
endif

ifeq ($(strip $(GPS_SUPPORT)), TRUE)
SOURCES  +=    mmieng_gpstest.c mmieng_gpstest_nmea_log.c
endif

ifeq ($(strip $(UAL_RF_AM_SUPPORT)), TRUE)
SOURCES  +=    mmieng_rf_am_test_win.c
endif

ifeq ($(strip $(BROWSER_SUPPORT_NETFRONT)), TRUE)
SOURCES	 +=    mmiwww.c mmiwww_func.c mmiwww_menutab.c mmiwww_netfront.c mmiwww_netfront_cmd.c mmiwww_nv.c \
                mmiwww_slim_peer_alt.c mmiwww_slim_peer_debug.c mmiwww_slim_peer_file.c mmiwww_slim_peer_net.c \
                mmiwww_slim_peer_system.c mmiwww_slim_peer_time.c mmiwww_slim_peer_wave.c mmiwww_wintab.c mmiwww_bookmark.c mmiwww_id.c
endif

ifeq ($(strip $(DL_SUPPORT)), TRUE)
SOURCES	 +=    mmidl.c mmidl_file.c mmidl_func.c mmidl_menutab.c mmidl_nv.c mmidl_wintab.c
endif

ifeq ($(strip $(MMI_MOCOR_SAMPLE)), TRUE)
SOURCES		+=  sample_anim_ctrl.c sample_dropdownlist_ctrl.c sample_editbox_ctrl.c sample_form_ctrl.c \
					sample_iconlist_ctrl.c sample_id.c sample_im.c sample_main.c sample_menutable.c sample_nv.c \
					sample_richtext_ctrl.c sample_tab_ctrl.c sample_test_ctrl.c
endif

ifeq ($(strip $(PIM_SUPPORT)), TRUE)
SOURCES		+=  mmipim_id.c mmipim_menutable.c mmipim_nv.c mmipim_wintab.c mmipim.c mmipim_pb.c Vcalendar_parse.c Vcard_parse.c
endif

ifeq ($(strip $(BROWSER_SUPPORT)), TRUE)
SOURCES	 +=    mmibrowser_manager_api.c mmibrowser_manager_func.c mmibrowser_manager_wintable.c mmibrowser_manager_nv.c mmibrowser_manager_menutable.c
endif
ifeq ($(strip $(MBEDTLS_VERSION)), V206)
MINCPATH += Third-party/mbedtls/V206/include
else
MINCPATH += Third-party/mbedtls/V224/include
endif
ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
SOURCES	 +=    brw_wtai.c brw_temp.c brw_core_adapter.c \
               brw_gwt.c brw_dlinterface.c brw_history.c brw_control.c brw_httpinterface.c brw_snapshot.c brw_plinterface.c brw_utility.c \
               mmibrowser_dorado_api.c mmibrowser_memutable.c mmibrowser_bookmark.c mmibrowser_wintable_history.c mmibrowser_wintable_snapshot.c \
               mmibrowser_setting.c mmibrowser_wintable.c mmibrowser_nv.c mmibrowser_wintable_bookmark.c mmibrowser_wintable_setting.c \
               mmibrowser_dummy.c mmibrowser_wintable_flowrate.c brw_input.c mmibrowser_func.c mmibrowser_wintable_search.c mmibrowser_punycode.c
endif

ifeq ($(strip $(SNS_SUPPORT)), TRUE)
SOURCES	 += mmisns_nv.c mmisns_export.c mmisns_srv.c mmisns_sqlite.c
endif

ifeq ($(strip $(MMS_SUPPORT)), DORADO)
SOURCES	 += mmimms_control_main.c mmimms_edit.c mmimms_editwin.c  mmimms_file.c mmimms_menutable_new.c mmimms_net.c mmimms_id.c\
                    mmimms_previewwin.c mmimms_push.c mmimms_setting.c mmimms_settingwin.c mmimms_main_new.c mmimms_waitingwin.c
endif

ifeq ($(strip $(ASP_SUPPORT)), TRUE)
SOURCES	 += mmiasp_api.c mmiasp_cortrol_layer.c mmiasp_id.c mmiasp_menutable.c mmiasp_nv.c mmiasp_wintab.c
endif

ifeq ($(strip $(DCD_SUPPORT)), TRUE)
SOURCES	 += mmidcd_file.c mmidcd_http.c mmidcd_id.c mmidcd_main.c mmidcd_menutable.c mmidcd_nv.c mmidcd_wintab.c mmidcd_xml_parse.c mmidcd_setting.c
endif

ifeq ($(strip $(BAIDU_DRIVE_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/baidu_drive/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/baidu_drive/c
SOURCES	+= mmibaidu_api.c mmibaidu_main.c mmibaidu_wintab.c
endif

ifeq ($(strip $(DEV_MANAGE_SUPPORT)), TRUE)
SOURCES	 += mmdm_id.c mmidm_app.c mmidm_menutable.c mmidm_nv.c mmidm_wintab.c mmidm_task.c mmidm_internal.c \
            mmidm_pl_mmi.c mmidm_pl_os.c mmidm_pl_comm.c mmidm_pl_string.c
ifeq ($(strip $(DM_SUPPORT)), DFRESH)
SOURCES	 += mmidm_parsexml.c mmidm_md5_b64.c mmidm_buildxml.c
endif
else
ifeq ($(strip $(DM_ONLY_SELFREG_SUPPORT)), TRUE)
SOURCES  += mmdm_id.c mmidm_app.c mmidm_wintab.c mmidm_menutable.c mmidm_nv.c
endif
endif

#camera module
#export CAMERA_SUPPORT    = TRUE         ### Camera support:
                                        # Option1:TRUE
                                        # Option2:FALSE

ifeq ($(strip $(CAMERA_SUPPORT)), TRUE)
    ifeq ($(strip $(CAMERA_SENSOR_ANGLE)), "")
    export CAMERA_SENSOR_ANGLE = 0    ### camera sensor direction
                                      # Option1:0	    #define CAMERA_SENSOR_ANGLE_0
                                      # Option2:90   	#define CAMERA_SENSOR_ANGLE_90
    endif

    export BURST_VOICE = EVERY        ### camera sensor direction
                                      # Option1:ONCE	    #define BURST_VOICE_ONCE
                                      # Option2:EVERY   	#define BURST_VOICE_EVERY


    # camera sensor direction
    #screen mode CAMERA_SCREEN_PORTRAIT and CAMERA_SCREEN_LANDSCAPE
    #if define macro CAMERA_SENSOR_ANGLE_90
    #then define macro CAMERA_SCREEN_LANDSCAPE
    ifeq ($(strip $(CAMERA_SENSOR_ANGLE)), 90)
        MCFLAG_OPT += -DCAMERA_SENSOR_ANGLE_90
        MCFLAG_OPT += -DCAMERA_SCREEN_LANDSCAPE
    else
        MCFLAG_OPT += -DCAMERA_SENSOR_ANGLE_0

        ifeq ($(strip $(MAINLCD_LOGIC_ANGLE)), 0)
            MCFLAG_OPT += -DCAMERA_SCREEN_PORTRAIT
        endif

        ifeq ($(strip $(MAINLCD_LOGIC_ANGLE)), 90)
            MCFLAG_OPT += -DCAMERA_SCREEN_LANDSCAPE
        endif
    endif

    #burst voice
    ifeq ($(strip $(BURST_VOICE)), ONCE)
        MCFLAG_OPT += -DBURST_VOICE_ONCE
    else
        MCFLAG_OPT += -DBURST_VOICE_EVERY
    endif

    #desktop bar
    ifeq ($(strip $(MAINLCD_SIZE)), 240X320)
        MMI_CAMERA_UI_STYLE = DESKTOP_ICON_BAR
    endif

    ifeq ($(strip $(MAINLCD_SIZE)), 240X400)
        MMI_CAMERA_UI_STYLE = DESKTOP_ICON_BAR
    endif

    ifeq ($(strip $(MAINLCD_SIZE)), 320X480)
        MMI_CAMERA_UI_STYLE = DESKTOP_ICON_BAR
    endif

    ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
        MMI_CAMERA_UI_STYLE = DESKTOP_PDA
        MMI_CAMERA_LAST_IMAGE = FALSE
    endif

    #mini camera default
    ifeq ($(strip $(LOW_MEMORY_SUPPORT)), NONE)
       ifeq ($(strip $(MAINLCD_SIZE)), 128X160)
           MMI_MINI_DEFAULT = TRUE
       endif
       ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
           MMI_MINI_DEFAULT = TRUE
       endif
    endif
    ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X64)
        MMI_MINI_DEFAULT = TRUE
    endif
    ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X16)
        MMI_MINI_DEFAULT = TRUE
    endif

    ifeq ($(strip $(MMI_MINI_DEFAULT)), TRUE)
    	MMI_CAMERA_UI_STYLE = DESKTOP_MINI
    	MMI_CAMERA_PHOTO_DATE = FALSE

        MMI_MINI_RESOURCE_SIZE = TRUE
        MMI_MINI_MEMORY_SIZE = FALSE
   ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X16)
    ifeq ($(strip $(MAINLCD_SIZE)), 128X160)
			MCFLAG_OPT += -DMMIDC_MINI_DISPLAY_STYLE
		endif
		ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
			MCFLAG_OPT += -DMMIDC_MINI_DISPLAY_STYLE
    endif
   endif

   ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X64)
    	ifeq ($(strip $(MAINLCD_SIZE)), 128X160)
				MCFLAG_OPT += -DMMIDC_MINI_3264_STYLE
    	endif
    	ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
				MCFLAG_OPT += -DMMIDC_MINI_3264_STYLE
				MCFLAG_OPT += -DMMIDC_ADD_SPECIAL_EFFECT
    	endif
    	ifeq ($(strip $(MAINLCD_SIZE)), 240X320)
    	ifneq ($(strip $(MMI_MINI_RESOURCE_SIZE)), TRUE)
				MCFLAG_OPT += -DMMIDC_DESKTOP_ICON_BAR
			endif
		  endif
    endif

    ifeq ($(strip $(LOW_MEMORY_SUPPORT)), NONE)
       ifeq ($(strip $(MAINLCD_SIZE)), 128X160)
           MCFLAG_OPT += -DMMIDC_MINI_DISPLAY_STYLE
       endif
       ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
           MCFLAG_OPT += -DMMIDC_MINI_DISPLAY_STYLE
       endif
    endif
    endif


    #PDA data type 16 or 32 BIT
     ifeq ($(strip $(DISPLAY_PIXELDEPTH_MAX)), 16)
        MCFLAG_OPT += -DMMIDC_F_PDA_BIT_16

        MMI_CAMERA_F_ANIM = FALSE
    endif


    ifeq ($(strip $(MMI_CAMERA_UI_STYLE)), DESKTOP_PDA)
        ifneq ($(strip $(MMI_CAMERA_F_ANIM)), FALSE)
            MCFLAG_OPT += -DMMIDC_F_PREVIEW_ANIM
            MCFLAG_OPT += -DMMIDC_F_CAPTURE_ANIM
        endif
    endif

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
    ifeq ($(strip $(PRODUCT_CONFIG)), sc6800h_sp6804h)
        MMI_CAMERA_F_SOFTWARE_ANGLE = TRUE
    endif

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
    #camera photo frame support.
    ifneq ($(strip $(MMI_CAMERA_UI_STYLE)), DESKTOP_PDA)
    ifneq ($(strip $(MMI_MINI_RESOURCE_SIZE)), TRUE)
        MMI_CAMERA_F_FRAME = TRUE
    endif
    endif

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
    #dv dc zoom
#    ifneq ($(strip $(MMI_MINI_RESOURCE_SIZE)), TRUE)
        MMI_CAMERA_F_DV_ZOOM = TRUE
        MMI_CAMERA_F_DC_ZOOM = TRUE
#    endif

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    ifeq ($(strip $(DC_FLASH_SUPPORT)), TRUE)
        MMI_CAMERA_F_FLASH = TRUE
    endif

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    MCFLAG_OPT += -DMMIDC_F_WORK_MODE

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
    ##u camera
    ifeq ($(strip $(U_CAMERA_SUPPORT)), TRUE)
        MMI_CAMERA_U_CAMERA = TRUE

        MMI_CAMERA_F_N_IN_ONE = TRUE
        MMI_CAMERA_F_EFFECT_360 = TRUE
        MMI_CAMERA_F_SMILE_AUTO_PHOTO = TRUE
    endif

    #@effect 360
    ifeq ($(strip $(MMI_CAMERA_F_EFFECT_360)), TRUE)
        MMI_CAMERA_F_LOMO = TRUE
        MMI_CAMERA_F_FISH_EYE = TRUE
        MMI_CAMERA_F_HDR = TRUE
        MMI_CAMERA_F_PENCIL_SKETCH = TRUE
        MMI_CAMERA_F_X_RAY = TRUE
        MMI_CAMERA_F_INFRARED = TRUE
        MMI_CAMERA_F_EXPOSAL = TRUE
        MMI_CAMERA_F_NEGATIVE = TRUE
    endif
    #@effect 360
    ##u camera
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    ifeq ($(strip $(CMCC_UI_STYLE)), FALSE)
    MMI_CAMERA_VIDEO_352X288 = TRUE
		endif

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
    ifeq ($(strip $(DC_2M_SUPPORT)), TRUE)
        MMI_CAMERA_PHOTO_DATE = FALSE
    endif
    #----------------------------------------------#
    #----------------------------------------------#


    #@define macro
    #==============================================#

    ifeq ($(strip $(MMI_MINI_RESOURCE_SIZE)), TRUE)
        MCFLAG_OPT += -DMMIDC_MINI_RESOURCE_SIZE
    endif

    ifeq ($(strip $(MMI_MINI_MEMORY_SIZE)), TRUE)
        MCFLAG_OPT += -DMMIDC_MINI_MEMORY_SIZE
    endif
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    #@photo date
    ifeq ($(strip $(MMI_CAMERA_PHOTO_DATE)), TRUE)
	        MCFLAG_OPT += -DMMI_CAMERA_F_PHOTO_DATE
    endif
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    ifeq ($(strip $(MMI_CAMERA_UI_STYLE)), DESKTOP_ICON_BAR)
        MCFLAG_OPT += -DMMIDC_DESKTOP_ICON_BAR
    endif

    ifeq ($(strip $(MMI_CAMERA_UI_STYLE)), DESKTOP_PDA)
        MCFLAG_OPT += -DMMIDC_DESKTOP_PDA
    endif
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    #@sensor software angle
    ifeq ($(strip $(MMI_CAMERA_F_SOFTWARE_ANGLE)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_SOFTWARE_ANGLE
    endif
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    ifeq ($(strip $(MMI_CAMERA_F_FRAME)), TRUE)
        MCFLAG_OPT      += -DCAMERA_FRAME_SUPPORT
    endif
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    ifeq ($(strip $(MMI_CAMERA_F_FLASH)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_FLASH
    endif

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    ifeq ($(strip $(MMI_CAMERA_LAST_IMAGE)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_LAST_IMAGE
        ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
   	        MCFLAG_OPT		+= -DVIDEOTHUMBNAIL_SUPPORT      	#视频文件缩略图提取
        endif
    endif

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    ifeq ($(strip $(MMI_CAMERA_U_CAMERA)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_U_CAMERA
    endif

    ifeq ($(strip $(MMI_CAMERA_F_N_IN_ONE)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_N_IN_ONE
    endif

    ifeq ($(strip $(MMI_CAMERA_F_SMILE_AUTO_PHOTO)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_SMILE_AUTO_PHOTO
    endif

    #@effect 360
    ifeq ($(strip $(MMI_CAMERA_F_EFFECT_360)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_EFFECT_360
    endif

    ifeq ($(strip $(MMI_CAMERA_F_LOMO)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_LOMO
    endif

    ifeq ($(strip $(MMI_CAMERA_F_FISH_EYE)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_FISH_EYE
    endif

    ifeq ($(strip $(MMI_CAMERA_F_HDR)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_HDR
    endif

    ifeq ($(strip $(MMI_CAMERA_F_PENCIL_SKETCH)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_PENCIL_SKETCH
    endif

    ifeq ($(strip $(MMI_CAMERA_F_X_RAY)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_X_RAY
    endif

    ifeq ($(strip $(MMI_CAMERA_F_INFRARED)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_INFRARED
    endif

    ifeq ($(strip $(MMI_CAMERA_F_EXPOSAL)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_EXPOSAL
    endif

    ifeq ($(strip $(MMI_CAMERA_F_NEGATIVE)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_NEGATIVE
    endif
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

    ifeq ($(strip $(MMI_CAMERA_VIDEO_352X288)), TRUE)
        MCFLAG_OPT += -DMMIDC_F_VIDEO_352X288
    endif

    ifeq ($(strip $(MMI_CAMERA_F_DV_ZOOM)), TRUE)
         MCFLAG_OPT += -DMMIDC_F_DV_ZOOM
    endif

    ifeq ($(strip $(MMI_CAMERA_F_DC_ZOOM)), TRUE)
         MCFLAG_OPT += -DMMIDC_F_DC_ZOOM
    endif
    
    MCFLAG_OPT += -DDC_WATCH_UI_SUPPORT
    #==============================================#

    SOURCES  += mmidc_full_osd_display.c mmidc_flow.c mmidc_gui.c \
                mmidc_save.c mmidc_setting.c mmidc_main_wintab.c \
                mmidc_window_option.c mmidc_setting_nv.c mmidc_osd_option.c \
                mmidc_photodate.c mmidc_guipda.c mmidc_guiiconbar.c \
                mmidc_workmode.c \
                mmidc_imageprocess.c mmidc_ninone.c mmidc_effect360.c mmidc_smileautophoto.c \
                mmidc_ucamera_wintab.c mmidc_conflict.c mmidc_srv.c
endif

#always include this file mmidc_export.c
SOURCES  += mmidc_export.c

#end camera module

ifeq ($(strip $(JAVA_SUPPORT)), IA)
SOURCES	       += mmijava_func.c mmijava_nv.c mmijava_wintab.c java_menutable.c mmijava_id.c \
		  AJEvent.c jblend_logFiler.c jwe.c jwe_eime.c jwe_log.c jwe_log_chk.c \
		  jwe_thread_com.c jwe_thread_kve.c \
		  jas_gfx_font.c AudioClient.c recordclient.c\
		  mediaControl.c media_midiControl.c media_toneControl.c media_mp3Control.c media_waveControl.c media_amrControl.c media_aacControl.c media_fileStream.c media_recordAmrControl.c media_cameraControl.c\
		  JgurToolKit.c JkMediaControl.c JkMediaCustomProtocol.c \
		  JkMediaMIDIDrv.c JkMediaStream.c JkMediaTone.c JkSoftkey.c JkUDP.c UDPResource.c \
		  UDPTaskBase.c UDPTaskProcess.c \
		  kjava_adv_fs.c kjava_sys.c kjava_sys_core.c kjava_sys_data_content.c \
		  kjava_sys_device.c kjava_sys_ext.c kjava_sys_ext_cert.c kjava_sys_fs.c kjava_sys_gfx.c \
		  kjava_sys_gfx_font_intra.c kjava_sys_http.c kjava_sys_log_out.c kjava_sys_pe.c \
		  kjava_sys_push.c kjava_sys_serial.c kjava_sys_sock.c kjava_sys_wma.c kjava_dev_fs.c JkPIM.c pim_contact.c pim_common.c JkVmBackground.c \
		  JkVmTimer.c JkCpuCache.c JkCommConnection.c
endif

ifeq ($(strip $(JAVA_SUPPORT)), SUN)
SOURCES	 += preload00_gen.c mmijava_main.c  mmijava_export.c mmijava_id.c mmijava_nv.c
endif

ifeq ($(strip $(JAVA_SUPPORT)), MYRIAD)
SOURCES	 += mmijava_export.c mmijava_menutable.c mmijava_id.c mmijava_nv.c \
            jpl_ams_signalEvent.c os_main.c cpl_pim.c \
            nativeGUI.c wma.c cpl_file.c cpl_font.c cpl_gfx.c cpl_media.c cpl_midletinfo.c jbed_headFilePreinstall.c \
            cpl_midletinfo_helper.c cpl_net.c cpl_tm.c cpl_tpal.c jbed_app.c jbed_configuration.c jbed_i18n.c \
            jbed_os.c jbed_props.c jpl_textfield.c jpl_phone.c  \
            mmijava_amswin.c mmijava_dlwin.c mmijava_aboutwin.c mmijava_backgroundwin.c \
            mmijava_localinstallwin.c mmijava_midletinfowin.c mmijava_runwin.c mmijava_permissionsetwin.c \
            mmijava_netsetwin.c mmijava_imwin.c mmijava_installprocesswin.c mmijava_commonfun.c mmijava_taskmgrwin.c\
            mmijava_dynamicmenufun.c mmijava_suitefun.c
endif

ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
MSRCPATH +=  build/$(PROJECT)_builddir/tmp
SOURCES	 +=  $(PROJECT)_ui_special_effect_table.c
endif


ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
 SOURCES	 += mmivp_api.c mmivp_bookmarklist.c mmivp_fsm.c mmivp_historylist.c mmivp_locallist.c \
			mmivp_nv.c mmivp_setting.c mmivp_wintable.c mmivp_id.c mmivp_menutable.c mmivp_thumbnail.c mmivp_mini_wintable.c
ifneq ($(strip $(STREAMING_SUPPORT)), TRUE)
 SOURCES	 +=mmivp_dummy.c
endif
endif


ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
ifeq ($(strip $(MOBILE_VIDEO_SUPPORT)), TRUE)
SOURCES	      += mv_fs_api.c  mv_mm_api.c  mv_net_api.c  mv_sms_api.c mv_base_api.c  \
                 mv_contact_api.c mv_date_api.c mv_output_api.c mv_player_api.c mv_string_api.c mv_ui_api.c mv_timer_api.c \
                 mv_path_transfer.c mmimv_wintab.c mmimv_id.c mmimv.c mmimv_export.c
endif
endif


#keylock effect start
ifeq ($(strip $(MMI_KEY_LOCK_SUPPORT)), TRUE)
SOURCES	      += mmikl_effectmain.c          #MMI_KEY_LOCK_EFFECT_SUPPORT
SOURCES	      += mmikl_slidedown.c           #MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT
SOURCES	      += mmikl_slidescrollkey.c      #MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
SOURCES	      += mmikl_doublecirclelock.c    #MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
SOURCES	      += mmikl_specdoublecirclelock.c	#MMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
#SOURCES	      += mmikl_qqvgalock.c  				 #MMI_KEY_LOCK_EFFECT_QQVGA
SOURCES	      += mmikl_smartslide.c          #MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT
SOURCES	      += mmikl_drawgraphic.c         #MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
SOURCES	      += mmikl_dragunlock.c          #MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT
SOURCES	      += mmikl_fruitsplit.c          #MMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT
SOURCES	      += mmikl_effectpsw.c           #MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
SOURCES	      += mmikl_Istyleslide.c         #MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT
#keylock effect end
endif
#灭屏即锁屏
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT

ifeq ($(strip $(CSC_SUPPORT)), TRUE)
SOURCES	 +=     mmicus_export.c mmicus_xmlparser.c mmicus_func.c mmicus_nv.c
endif

ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)

#MCFLAG_OPT		+=  -DEFFECT_LIGHT_GRID_WALLPAPER_SUPPORT

#call log不显示主菜单
MCFLAG_OPT		+=  -DMMI_CALLLOG_HIDE_MAINMENU

MSRCPATH           += $(MMI_DIR)/source/mmi_app/app/mmise/c
MINCPATH            += $(MMI_DIR)/source/mmi_app/app/mmise/h

endif


ifeq ($(strip $(MMI_MEMO_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/memo/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/memo/c
SOURCES	 +=    mmimemo_api.c mmimemo_memutable.c mmimemo_wintable.c mmimemo_nv.c mmimemo_func.c
endif

#mmi_service
MINCPATH  +=  $(MMI_DIR)/source/mmi_service/export/inc
MSRCPATH +=   $(MMI_DIR)/source/mmi_service/source/mmisrvaud_test/c
MSRCPATH +=   $(MMI_DIR)/source/mmi_service/source/mmisrvadapt/c
SOURCES	 +=    mmisrvadapt.c
SOURCES	 +=    Mmisrvaud_test.c

ifeq ($(strip $(LIVE_WALLPAPER_FRAMEWORK_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/livewallpaper/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/livewallpaper/h/def
endif

ifeq ($(strip $(LIVE_WALLPAPER_FRAMEWORK_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/livewallpaper/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/livewallpaper/c/framework
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/livewallpaper/c/instance
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/livewallpaper/c/wincfg
endif

ifeq ($(strip $(LIVE_WALLPAPER_FRAMEWORK_SUPPORT)), TRUE)
SOURCES += mmi_livewallpaper.c mmi_livewallpaper_nv.c mmi_livewallpaper_win_cfg.c mmi_livewallpaper_instance_cfg.c mmi_livewallpaper_option.c mmi_livewallpaper_preview_wintab.c mmi_livewallpaper_pop.c \
           mmi_livewallpaper_bee.c mmi_livewallpaper_linear.c mmi_livewallpaper_galaxy.c mmi_livewallpaper_sakura.c mmi_livewallpaper_seaworld.c mmi_livewallpaper_lightgrid.c mmi_livewallpaper_ripple.c\
           mmi_livewallpaper_collision.c mmi_livewallpaper_littlecat.c mmi_livewallpaper_slideshow.c mmi_livewallpaper_sweettree.c mmi_livewallpaper_collision_balloon.c mmi_livewallpaper_collision_menu.c\
	   mmi_livewallpaper_windmill.c mmi_livewallpaper_rainbowcity.c mmi_livewallpaper_papertown.c mmi_livewallpaper_aquarium.c mmi_livewallpaper_girlswing.c  mmi_livewallpaper_spring.c
endif

#SOURCES		+=   dummy_mneng_api.c

ifeq ($(strip $(MMI_WIFI_SUPPORT)), TRUE)
    MINCPATH += $(MMI_DIR)/source/mmi_app/app/wlan/h
    MINCPATH += MS_Customize/source/product/driver/wifi/wifisupp
    MSRCPATH += $(MMI_DIR)/source/mmi_app/app/wlan/c
    SOURCES  += mmiwlan_id.c mmiwlan_manager.c mmiwlan_menutable.c mmiwlan_nv.c mmiwlan_profile.c mmiwlan_win.c mmiwlan_portal.c mmiwlan_portal_net.c \
                mmiwlan_main_win.c mmiwlan_password_win.c

    MINCPATH += $(MMI_DIR)/source/mmi_service/source/mmisrvwifi/h
    MSRCPATH += $(MMI_DIR)/source/mmi_service/source/mmisrvwifi/c
    SOURCES  += mmisrvwifi.c
else
ifeq ($(strip $(MMI_ENG_WIFI_SUPPORT)), TRUE)
    MINCPATH += $(MMI_DIR)/source/mmi_app/app/wlan/h
endif
endif

ifeq ($(strip $(_SW_ZDT_PRODUCT_)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/qr_encode/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/qr_encode/c
SOURCES += qr_encode.c
endif

ifeq ($(strip $(WLDT_APP_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/wldt/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/wldt/c
SOURCES += wldt_app.c wldt_wintable.c wldt_nv.c wldt_db.c wldt_menutable.c wldt_id.c wldt_common.c 
ifeq ($(strip $(WLDT_NET_SUPPORT)), TRUE)
SOURCES += wldt_net.c
ifeq ($(strip $(WLDT_TCP_SUPPORT)), TRUE)
SOURCES += wldt_tcp.c
endif
ifeq ($(strip $(WLDT_PLAT_CY_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/wldt/h/cy
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/wldt/c/cy
SOURCES += wldt_cy.c wldt_cy_net.c wldt_cy_api.c wldt_cy_db.c wldt_cy_func.c wldt_cy_wintable.c
endif
endif
endif

ifeq ($(strip $(ZDT_NET_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/qrencode/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/qrencode/c
SOURCES	 += bitstream.c mask.c mmask.c mqrspec.c qr.c qrcmd.c qrcnv.c qrcnv_bmp.c qrcnv_png.c qrcnv_svg.c qrcnv_tiff.c \
            qrenc.c qrencode.c qrinput.c qrspec.c rscode.c split.c qr_srv.c
endif

ifeq ($(strip $(ZDT_HTTP_APP_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zdthttp/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zdthttp/c
SOURCES += zdthttp_app.c zdthttp_wintable.c zdthttp_nv.c zdthttp_menutable.c zdthttp_id.c zdthttp_common.c zdthttp_api.c zdthttp_net.c
endif

ifeq ($(strip $(ZDT_ZFB_SUPPORT)), TRUE)
MINCPATH += Third-party/alipay/include
MINCPATH += Third-party/alipay/alimbedtls/include
MINCPATH += Third-party/alipay/alimbedtls/include/alimbedtls
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zfb/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zfb/c
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zfb/id2/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zfb/id2/c
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zfb/alipay/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zfb/alipay/c
SOURCES += alipay_kal.c alipay_vendor.c
SOURCES += aes.c md5.c sha1.c sha256.c ls_hal_aes.c ls_hal_hash.c ls_hal_km.c ls_hal_rand.c ls_hal_sst.c ls_osa.c zfb_prov_base64.c
SOURCES += zfb_app.c zfb_alipay.c zfb_wintable.c zfb_nv.c zfb_db.c zfb_menutable.c zfb_id.c zfb_common.c zfb_net.c zfb_id2.c
endif

ifeq ($(strip $(ZDT_APP_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/c
SOURCES += zdt_app.c zdt_wintable.c zdt_factory_win.c zdt_nv.c zdt_db.c zdt_menutable.c zdt_id.c zdt_common.c zdt_sha2.c 
SOURCES += zdt_dropdown_win.c

ifeq ($(strip $(FOTA_SUPPORT)), CMIOT)
MINCPATH += Third-party/oneos/ota/cmiot/inc
SOURCES += zdt_wintable_fota.c
endif

ifeq ($(strip $(ZDT_PCBA_ZW31_SUPPORT)), TRUE)
SOURCES += zdt_zw31.c
endif
ifeq ($(strip $(ZDT_LED_SUPPORT)), TRUE)
SOURCES += zdt_led.c
endif 
ifeq ($(strip $(ZDT_SMS_SUPPORT)), TRUE)
SOURCES += zdt_sms.c
endif
ifeq ($(strip $(ZDT_PLAT_SHB_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/h/shb
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/c/shb
SOURCES += zdt_shb.c zdt_shb_http.c zdt_shb_api.c zdt_shb_db.c zdt_shb_func.c zdt_shb_sms.c zdt_shb_wintable.c
endif
ifeq ($(strip $(ZDT_NET_SUPPORT)), TRUE)
SOURCES += zdt_net.c
ifeq ($(strip $(ZDT_UDP_SUPPORT)), TRUE)
SOURCES += zdt_udp.c
endif
ifeq ($(strip $(ZDT_TCP_SUPPORT)), TRUE)
SOURCES += zdt_tcp.c
endif
ifeq ($(strip $(ZDT_PLAT_HYHL_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/h/hyhl
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/c/hyhl
SOURCES += zdt_hyhl.c zdt_hyhl_net.c zdt_hyhl_api.c zdt_hyhl_db.c zdt_hyhl_func.c 
endif
ifeq ($(strip $(ZDT_PLAT_YX_SUPPORT)), TRUE)
ifeq ($(strip $(ZDT_PLAT_YX_SUPPORT_YS)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/h/yx_ys
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/c/yx_ys
endif
ifeq ($(strip $(ZDT_PLAT_YX_SUPPORT_FZD)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/h/yx_ys
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/c/yx_ys
endif
ifeq ($(strip $(ZDT_PLAT_YX_SUPPORT_TX)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/h/yx_ys
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/c/yx_ys
endif
ifeq ($(strip $(ZDT_PLAT_YX_SUPPORT_LZ)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/h/yx_lz
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/c/yx_lz
endif
ifeq ($(strip $(ZDT_PLAT_YX_SUPPORT_CY)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/h/yx_cy
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zdt/c/yx_cy
endif
SOURCES += zdt_yx.c zdt_yx_net.c zdt_yx_api.c zdt_yx_db.c zdt_yx_func.c zdt_yx_captrue.c zdt_yx_voice.c zdt_yx_sms.c zdt_yx_wintable.c
endif
endif
endif

ifeq ($(strip $(ZDT_RTMP_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/rtmp
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/rtmp
MINCPATH += $(MMI_DIR)/source/mmi_app/app/rtmp/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/rtmp/api
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/rtmp/api
MINCPATH += $(MMI_DIR)/source/mmi_app/app/rtmp/api/librtmp
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/rtmp/api/librtmp
SOURCES += amf.c log.c parseurl.c rtmp.c minirtmp.c rtmp_player.c rtmp_player.c rtmp_mp3_info.c rtmp_demo.c
endif
ifeq ($(strip $(TULING_AI_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tulingai/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/tulingai/c
#SOURCES += tuling_menutable.c tuling_ai_wifi.c zdt_yx_net.c tuling_api.c tuling_db.c tuling_func.c zdt_yx_captrue.c tuling_voice.c aes.c
SOURCES += tuling_wintable.c ai_voice.c ai_tcp.c ai_net.c ai_ase.c ai_func.c ai_app.c ai_common.c ai_http.c
endif



ifeq ($(strip $(BAIRUI_VIDEOCHAT_SUPPORT)), TRUE)
MINCPATH += Third-party/anychat/include
MINCPATH += $(MMI_DIR)/source/mmi_app/app/videochat/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/videochat/c
SOURCES += videochat_wintable.c videochat_http.c videochat_app.c 
endif

ifeq ($(strip $(LOCAL_ALARM_CLOCK_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/alarmClock/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/alarmClock/c
SOURCES += alarm_corepush.c alarmClock_alive.c alarmClock_main.c alarmClock_nv.c 
#SOURCES += alarmClock_select.c alarmClock_select_edit.c 
endif

ifeq ($(strip $(ZTE_PEDOMETER_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/pedometer/c
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pedometer/h
SOURCES	 += pedometer_main.c pedometer_today.c pedometer_statistics.c pedometer_util.c pedometer_nv.c pedometer_loading.c
endif

ifeq ($(strip $(ZTE_WEATHER_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/zteweather/c
MINCPATH += $(MMI_DIR)/source/mmi_app/app/zteweather/h
SOURCES	 += zteweather_main.c zteweather_http.c
endif

ifeq ($(strip $(ZTE_STOPWATCH_SUPPORT)), TRUE)
MINCPATH        +=  MS_MMI_Main/source/mmi_app/app/zte_stopwatch/h
MSRCPATH        +=  MS_MMI_Main/source/mmi_app/app/zte_stopwatch/c
SOURCES        +=  zte_stopwatch_win.c  
endif

ifeq ($(strip $(ZTE_MENU_SUPPORT)), TRUE)
MINCPATH        +=  MS_MMI_Main/source/mmi_app/app/zte_menu/h
MSRCPATH        +=  MS_MMI_Main/source/mmi_app/app/zte_menu/c
SOURCES        +=  zte_menu_semicircle_win.c  
endif

ifeq ($(strip $(BEACON_APP_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/beacon/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/beacon/c
SOURCES += beacon_app.c beacon_wintable.c beacon_nv.c beacon_menutable.c beacon_id.c beacon_common.c 
endif


ifeq ($(strip $(MMIWIDGET_SUPPORT)), TRUE)
SOURCES		+=   mmiwidget.c mmiwidget_clock.c mmiwidget_display.c mmi_widget_grid.c mmiwidget_mp3.c \
                mmiwidget_nv.c mmiwidget_pb.c mmiwidget_id.c mmitheme_widget.c mmiwidget_memo.c mmiwidget_shortcut_menu.c \
                mmiwidget_calendar.c  mmiwidget_network_name.c  mmiwidget_custom_word.c  mmiwidget_NewSms.c \
                mmiwidget_calllog.c   mmiwidget_fm.c mmiwidget_menutable.c mmiwidget_alarm.c mmiwidget_shortcut_set.c \
                mmiwidget_Aclock.c mmiwidget_elve.c mmiwidget_Search.c mmiwidget_dcd.c mmiwidget_dual_clock.c mmiwidget_dual_aclock.c \
                mmiwidget_autoanswer_log.c mmiwidget_weather.c mmiwidget_schedule.c mmiwidget_sns.c mmiwidget_time.c
endif
ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
SOURCES		+=   mmi_winse.c mmi_winse_imp.c mmi_winse_imp_app.c mmi_winse_imp_fade_inout.c mmi_winse_imp_vertical_inout.c
endif
ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
SOURCES		+=  mmigrid_idle.c  mmigrid_display.c mmi_widget_grid.c \
								mmiwidget_virtualwin.c mmiwidget_page.c mmiwidget_item.c \
								mmiwidget_grid.c mmiwidget_wallpaper.c mmiqbtheme_menuwin.c\
                mmigrid_pb.c mmigrid_imgviewer.c mmiwidget_nv.c mmiwidget_id.c mmitheme_widget.c \
								mmiwidget_clock.c  mmiwidget_mp3.c \
                mmiwidget_memo.c mmiwidget_shortcut_menu.c mmiwidget_calendar.c  mmiwidget_network_name.c \
                mmiwidget_fm.c mmiwidget_menutable.c mmiwidget_alarm.c mmiwidget_shortcut_set.c \
                mmiwidget_Aclock.c mmiwidget_Search.c mmiwidget_dcd.c mmiwidget_dual_clock.c mmiwidget_dual_aclock.c \
                mmiwidget_weather.c mmiwidget_netsearch.c mmiwidget_countedtime.c mmigrid_alarm.c mmiwidget_schedule.c mmiwidget_sns.c\
                mmiwidget_time.c mmiwidget_weather2.c mmiwidget_weather_se.c mmiwidget_special_effect.c mmiwidget_aclock_se.c
SOURCES	 +=     mmiwidget_speed_dial.c mmiwidget_pwr_saving.c mmiwidget_mp3_se.c
endif

ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
SOURCES += adult_watch_charge_win.c
else
SOURCES += watch_charge_win.c
endif

ifeq ($(strip $(DYNAMIC_WATCHFACE_SUPPORT)), FALSE)
ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
SOURCES	 += watch_launcher_adult_common.c watch_launcher_adult_editclockwin.c watch_launcher_multe_function_dial.c watch_launcher_common_panel_item.c
else
SOURCES	 += watch_launcher_common.c watch_launcher_editclockwin.c
endif
endif

SOURCES	 += watch_slidepage.c watch_launcher_main.c 
SOURCES += watch_slidewin.c
SOURCES += watch_panel_main.c
ifeq ($(strip $(QBTHEME_SUPPORT)), TRUE)
SOURCES		+=  mmiqbtheme_idlewin.c mmiqbtheme_init.c  \
							mmiqbtheme_dockbar.c mmiqbtheme_virtualwin.c mmiqbtheme_page.c \
							mmiqbtheme_item.c  mmiqbtheme_grid.c
endif

ifeq ($(strip $(PUSH_EMAIL_SUPPORT)), TRUE)
SOURCES += mail_file.c mail_util.c mail_setting.c mail_main.c mail_welcom.c mmimail_wintab.c mmimail_edit.c \
           mmimail_read.c mmimail_menutable.c mmimail_id.c mail_export.c
endif
# Auto Testing Support
ifeq ($(strip $(ATEST_SUPPORT)), TRUE)
MSRCPATH +=  $(MMI_DIR)/source/mmi_app/app/atest/c
MINCPATH +=  $(MMI_DIR)/source/mmi_app/app/atest/h
SOURCES	 +=  atest_main.c atest_key_step.c atest_location.c atest_pb.c atest_r8.c atest_ota.c atest_inputmethod.c atest_calendar.c atest_plmn.c
endif

ifeq ($(strip $(SQLITE_SUPPORT)), TRUE)
# MCFLAG_OPT		+= -DSQLITE_FILEARRAY_SUPPORT
endif

ifeq ($(strip $(SQLITE_FILEARRAY_SUPPORT)), TRUE)
SOURCES	 += mmivirtualarray_sql.c mmifilearray_sql.c
else
SOURCES	 += mmivirtualarray.c mmifilearray.c
endif

#存在复杂文本,需要严格的计算字符串的宽高
#需要临时变量做或判断
#arabic, thai, hindi, hebrew, tibet等语言为复杂文本
ifeq ($(strip $(IM_ARABIC_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_PERSIAN_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_URDU_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_THAI_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_HINDI_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_HEBREW_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_BENGALI_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_MYANMAR_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

MCFLAG_OPT += $(MCFLAG_OPT_COMPLEX_TEMP)
#存在复杂文本,需要严格的计算字符串的宽高

#下拉窗口
ifeq ($(strip $(PDA_UI_DROPDOWN_WIN)), TRUE)
MSRCPATH 	+= $(MMI_DIR)/source/mmi_app/app/dropdownwin/c
MINCPATH 	+= $(MMI_DIR)/source/mmi_app/app/dropdownwin/h
SOURCES	+= mmidropdownwin_main.c mmidropdownwin_nv.c mmidropdownwin_id.c
endif

ifeq ($(strip $(DROPDOWN)), NOTIFY_SHORTCUT)
MSRCPATH 	+= $(MMI_DIR)/source/mmi_app/app/notifycenter/c
MINCPATH 	+= $(MMI_DIR)/source/mmi_app/app/notifycenter/h
SOURCES	+= watch_notifycenter_shortcutwin.c watch_notifycenter.c watch_notifycenter_id.c watch_notifycenter_notifywin.c
endif

ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
MSRCPATH 	+= $(MMI_DIR)/source/mmi_app/app/sensor/c
MINCPATH 	+= $(MMI_DIR)/source/mmi_app/app/sensor/h
SOURCES	+= watch_sensor.c watch_sensor_id.c watch_sensor_raise_wrist.c watch_heartrate.c watch_steps.c 
endif

ifeq ($(strip $(MMI_AZAN_SUPPORT)), TRUE)
MCFLAG_OPT	+= -DMMI_AZAN_MATRIX_SUPPORT
MINCPATH	+= $(MMI_DIR)/source/mmi_app/app/azan/h
MSRCPATH	+= $(MMI_DIR)/source/mmi_app/app/azan/c
SOURCES		+= mmiazan_wintab.c mmiazan.c
endif

ifeq ($(strip $(WRE_SUPPORT)),TRUE)
SOURCES		+= mmiwre_adaptor.c mmiwre_app.c mmiwre_boot_stub.c mmiwre_cfg.c mmiwre_charset.c mmiwre_dial.c mmiwre_export.c mmiwre_file.c mmiwre_font.c \
               mmiwre_gdi.c mmiwre_id.c mmiwre_image.c mmiwre_ime.c mmiwre_layer.c mmiwre_media.c mmiwre_menutable.c mmiwre_nv.c mmiwre_other.c mmiwre_sema.c \
               mmiwre_socket.c mmiwre_startup.c mmiwre_tapi.c mmiwre_thread.c mmiwre_timer.c mmiwre_wintab.c mmiwre_videoplayer.c mmiwre_pb.c mmiwre_device.c \
               mmiwre_qbtheme.c
endif

ifeq ($(strip $(IKEYBACKUP_SUPPORT)),TRUE)
SOURCES		+= mmiikeybackup_api.c mmiikeybackup_data.c mmiikeybackup_id.c mmiikeybackup_menutable.c mmiikeybackup_nv.c mmiikeybackup_task.c mmiikeybackup_wintab.c
endif

ifeq ($(strip $(MMIEMAIL_SUPPORT)),TRUE)
MSRCPATH 	+= $(MMI_DIR)/source/mmi_app/app/email/c
MINCPATH 	+= $(MMI_DIR)/source/mmi_app/app/email/h
SOURCES	+= mmiemail_edit.c mmiemail_export.c  mmiemail_comm.c mmiemail_view.c \
           mmiemail_main.c mmiemail_menutable.c mmiemail_setting.c mmiemail_sp.c \
					 mmiemail_sp_config.c mmiemail_sp_store.c mmiemail_sp_transaction.c mmiemail_vector.c mmiemail_utils.c\
					 mmiemail_dlg.c mmiemail_box.c mmiemail_net.c mmiemail_signal.c


MINCPATH 	+= DAPS/export/inc/email

MCFLAG_OPT += -DMMIPB_MAIL_SUPPORT
endif

ifeq ($(strip $(QBTHEME_SUPPORT)),TRUE)
MSRCPATH 	+= $(MMI_DIR)/source/mmi_app/app/qbtheme/c
MINCPATH 	+= $(MMI_DIR)/source/mmi_app/app/qbtheme/h
SOURCES	+=  mmi_qbtheme_map.c mmiqbtheme_app.c mmiqbtheme_export.c mmiqbtheme_id.c mmiqbtheme_nv.c mmiqbtheme_wintab.c

endif



ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
    SOURCES 	+= mmise_calculation.c

    MSRCPATH        +=   $(MMI_DIR)/source/mmi_app/app/mmise/c

    MINCPATH         +=  $(MMI_DIR)/source/mmi_app/app/mmise/h
endif

#增加灭屏特效的宏
ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
    ifeq ($(strip $(SE_CRT_SUPPORT)), TRUE)
    	ifeq ($(strip $(MMI_PDA_SUPPORT)),TRUE)
#        LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT = TRUE
#        MCFLAG_OPT	+= -DLOCK_SCREEN_SPECIAL_EFFECT_SUPPORT
    	endif
    endif
endif

ifeq ($(strip $(LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT)),TRUE)
	SOURCES	 +=  mmiss_screenclose_special.c
endif#存在复杂文本,需要严格的计算字符串的宽高


OPERA_SUPPORT=FALSE
ifeq ($(strip $(OPERA_MINI_SUPPORT)), VER6)
MINCPATH 	 += $(MMI_DIR)/source/mmi_app/app/operamini6
MSRCPATH      += $(MMI_DIR)/source/mmi_app/app/operamini6
SOURCES  += OpDevOperaMini.c
MCFLAG_OPT += -DOD_HAVE_SCHEME_API -DOD_TWEAK_IMPLEMENT_OPERAMINI_API -DOD_HAVE_ENTRY -DOD_TWEAK_PHYSICALKEYS=12 -DOD_NO_SIMULATOR -DOD_6530_SUPPORT -DOD_8501_MAIN
OPERA_SUPPORT=TRUE
endif
ifeq ($(strip $(OPERA_MINI_SUPPORT)), VER42)
MINCPATH 	 += $(MMI_DIR)/source/mmi_app/app/operamini4
MSRCPATH      += $(MMI_DIR)/source/mmi_app/app/operamini4
SOURCES  += OpDevOperaMini.c
ifeq ($(strip $(MMI_PDA_SUPPORT)),TRUE )
MCFLAG_OPT += -DOD_HAVE_SCHEME_API -DOD_TWEAK_IMPLEMENT_OPERAMINI_API -DOD_HAVE_ENTRY -DOD_TWEAK_PHYSICALKEYS=12 -DOD_NO_SIMULATOR -DOD_6530_SUPPORT -DOD_8501_MAIN
else
MCFLAG_OPT += -DOD_HAVE_SCHEME_API -DOD_TWEAK_IMPLEMENT_OPERAMINI_API -DOD_HAVE_ENTRY -DOD_NO_SIMULATOR -DOD_6530_SUPPORT -DOD_8501_MAIN
endif
endif
ifeq ($(strip $(OPERA_MOBILE_SUPPORT)), V11)
OPERA_SUPPORT=TRUE
endif



#图片浏览器快速浏览界面
#ifeq ($(strip $(PIC_QUICKVIEW_SUPPORT)),TRUE)
#SOURCES  += mmipicview_quickview.c
#endif

ifneq ($(strip $(KEYPAD_TYPE)),FOUR_KEY_UNIVERSE)
		MCFLAG_OPT	+= -DMMI_WIDGET_DIR_KEY_SUPPORT
endif

ifneq ($(strip $(UZONE_SUPPORT)), NONE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/uzone/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/uzone/c
ifeq ($(strip $(UZONE_SUPPORT)), GLASS)
MCFLAG_OPT	+= -DMMIUZONE_STYLE_GLASS
endif
SOURCES	 +=     mmiuzone.c mmiuzone_id.c mmiuzone_nv.c mmiuzone_wintab.c mmiuzone_export.c mmiuzone_display.c mmiuzone_se.c
endif

ifeq ($(strip $(MMS_SMS_IN_1_SUPPORT)), TRUE)
MCFLAG_OPT += -DMMIMMS_SMS_IN_1_SUPPORT
#for mms/sms contact search
MCFLAG_OPT += -DMMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
endif

ifeq ($(strip $(CHATMODE_ONLY_SUPPORT)), TRUE)
MCFLAG_OPT += -DMMISMS_CHATMODE_ONLY_SUPPORT
endif

ifeq ($(strip $(PIC_CROP_SUPPORT)), TRUE)
ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
MCFLAG_OPT		+=  -DMMI_PIC_CROP_SUPPORT
endif
endif


ifeq ($(strip $(HERO_ENGINE_SUPPORT)), TRUE) 

    ifeq ($(strip $(HERO_ENGINE_SUPPORT_CLIENT_INFO)),TRUE) #客户定制配置要求;要去除前一定要和客户确认;
		#MCFLAG_OPT += -D__HERO_ENGINE_SUPPORT_CLIENT_INFO__

		MCFLAG_OPT += -DHERO_ENGINE_CFG_VAR_HANDSETID_FIRST=$(strip $(HERO_ENGINE_CFG_VAR_HANDSETID_FIRST))
		MCFLAG_OPT += -DHERO_ENGINE_CFG_VAR_HANDSETID_SEC=$(strip $(HERO_ENGINE_CFG_VAR_HANDSETID_SEC))
    	ifeq ($(strip $(HERO_ENGINE_CFG_VAR_HANDSETID_FIRST)),) ##ifneq ($(strip $(HERO_ENGINE_CFG_VAR_HANDSETID_FIRST)),)
    	$(warning ERROR: Current value is NULL,Please define custom info value of HERO_ENGINE_CFG_VAR_HANDSETID_FIRST !!   )
    	DEPENDENCY_CONFLICT = TRUE
    	else
    	$(warning warning: current value:HERO_ENGINE_CFG_VAR_HANDSETID_FIRST=$(strip $(HERO_ENGINE_CFG_VAR_HANDSETID_FIRST)))
    	endif
    
    	ifeq ($(strip $(HERO_ENGINE_CFG_VAR_HANDSETID_SEC)),) ##ifneq ($(strip $(HERO_ENGINE_CFG_VAR_HANDSETID_SEC)),)
    	$(warning ERROR: Current value is NULL,Please define custom info value of HERO_ENGINE_CFG_VAR_HANDSETID_SEC !!   )
    	DEPENDENCY_CONFLICT = TRUE
    	else
    	$(warning warning: current value:HERO_ENGINE_CFG_VAR_HANDSETID_SEC=$(strip $(HERO_ENGINE_CFG_VAR_HANDSETID_SEC)))
    	endif

    	ifdef DEPENDENCY_CONFLICT
        	ifeq ($(strip $(DEPENDENCY_CONFLICT)),TRUE)
        	$(error PLEASE check above dependency errors!)
        	endif
    	endif
    
    endif ##ifeq ($(strip $(HERO_ENGINE_SUPPORT_CLIENT_INFO)),TRUE)
    
MINCPATH   +=  $(MMI_DIR)/source/mmi_app/app/heroengine/h \
		$(MMI_DIR)/source/mmi_app/app/wlan/h \
		$(MMI_DIR)/source/mmi_gui/source/anim/h \
		$(MMI_DIR)/export/inc \
		$(MMI_DIR)/source/dc/dc_6x00/inc \
		$(MMI_DIR)/source/isp_service/inc \
		$(MMI_DIR)/source/dc/dc_common/inc \
		$(MMI_DIR)/source/mmi_ctrl/source/anim/h \
	    $(MMI_DIR)/source/mmi_ctrl/source/basecontrol/h \
        $(MMI_DIR)/source/mmi_ctrl/source/typemanager/h \
        $(MMI_DIR)/source/mmi_gui/include \
		$(MMI_DIR)/source/image_proc/inc 
MSRCPATH   +=  $(MMI_DIR)/source/mmi_app/app/heroengine/c
#SOURCES	   +=  heroEngineFunc.c heroEngineWintab.c heroEngineApp.c heroEngineMotion.c heroEnginePhonebook.c heroEngineCamera.c heroAppAdapter.c heroEngineApn.c
SOURCES	   +=  heroEngineFunc.c heroEngineWintab.c heroEngineApp.c heroEngineMotion.c heroEnginePhonebook.c heroEngineCamera.c heroEngineSimulatorApi.c heroEngineFm.c


ifeq ($(strip $(HERO_CAMERA_SUPPORT)), TRUE)
MINCPATH += MS_Ref/source/isp_service/inc \
			 MS_Ref/source/dc/dc_6x00/inc \
			 MS_Ref/source/image_proc/inc \
			 MS_Ref/source/display/inc \
			 MS_Ref/source/image_proc/inc \
			 MS_Ref/source/lcd/inc
SOURCES	   +=  herocamera.c
endif

ifeq ($(strip $(HERO_QR_DECODE_SUPPORT)), TRUE)
SOURCES	   +=  heroqrdecipher.c
endif

ifeq ($(strip $(HERO_ENGINE_TTS_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/heroengine/tts/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/heroengine/tts/Lib
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tts/h

MSRCPATH += $(MMI_DIR)/source/mmi_app/app/tts/c
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/heroengine/tts/c

SOURCES	 +=     TTS_api.c hero_tts_api.c hero_tts_ui.c

endif
endif

#add by zack@20110117 for sky start
ifeq ($(strip $(MRAPP_SUPPORT)), TRUE)
MINCPATH   +=  $(MMI_DIR)/source/mmi_app/app/mrapp/h
MINCPATH   +=  $(MMI_DIR)/source/mmi_ctrl/source/anim/h
MINCPATH   +=  $(MMI_DIR)/source/mmi_ctrl/source/basecontrol/h
MINCPATH   +=  $(MMI_DIR)/source/mmi_ctrl/source/typemanager/h
MSRCPATH   +=  $(MMI_DIR)/source/mmi_app/app/mrapp/c
SOURCES	   +=  mmimrapp_func.c mmimrapp_wintab.c mmimrapp.c mmimrapp_motion.c mmimrapp_phonebook.c
endif
#add by zack@20110117 for sky end

ifeq ($(strip $(PIC_CROP_SUPPORT)), TRUE)
ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/pic_crop/c
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pic_crop/h
SOURCES	 += mmipic_crop.c mmipic_crop_wintab.c mmipic_crop_id.c mmipic_crop_menutable.c mmipic_crop_nv.c
endif
endif

ifeq ($(strip $(SXH_APPLE_SUPPORT)), TRUE)
MINCPATH  += $(MMI_DIR)/source/mmi_app/app/apple/h \
			 MS_Ref/source/isp_service/inc \
			 MS_Ref/source/dc/dc_6x00/inc \
			 MS_Ref/source/image_proc/inc \
			 MS_Ref/source/display/inc \
			 MS_Ref/source/image_proc/inc \
			 MS_Ref/source/lcd/inc \
			 $(MMI_DIR)/source/mmi_gui/source/listbox/h
#MS_Ref/source/dc/dc_6600l/inc \

MSRCPATH +=  $(MMI_DIR)/source/mmi_app/app/apple/c

SOURCES   += apple_func.c apple_id.c apple_nv.c apple_wintab.c apple_export.c
endif

ifeq ($(strip $(MCARE_V31_SUPPORT)), TRUE)
MCFLAG_OPT += -DSPREAD_PLAT_SC6530
ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
MCFLAG_OPT += -DSPREAD_PLAT_SC6530_PDA
endif

MINCPATH += $(MMI_DIR)/source/mmi_app/app/TencentMcareV31/inc
MINCPATH += $(MMI_DIR)/source/mmi_app/app/TencentMcareV31/src/resource/romizeddata
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/TencentMcareV31/src
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/TencentMcareV31/src/resource/romizeddata
MINCPATH += $(MMI_DIR)/MS_Ref/source/display/inc
MINCPATH += MS_Ref/source/display/inc

SOURCES += amr_record.c \
			McfInterface.c \
			McfZlib.c \
			MAdpCfgRomData.c
endif

ifeq ($(strip $(TOUCHPANEL_TYPE)), HW)
ifneq ($(strip $(TPC_TYPE)), NONE)
MCFLAG_OPT += -DTP_TEST_SUPPORT
endif
endif
MSRCPATH +=  $(MMI_DIR)/source/mmi_app/app/U_launcher/c
MINCPATH +=  $(MMI_DIR)/source/mmi_app/app/U_launcher/h

ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
SOURCES	 += u_menu_se.c
endif

ifeq ($(strip $(PDA_UI_SUPPORT_U_IDLE)), TRUE)
SOURCES	 += u_idle_se.c mmigrid_uidle_se.c
endif
ifeq ($(strip $(AUTODEMO_SUPPORT)), TRUE)
MINCPATH   +=  $(MMI_DIR)/source/mmi_app/app/autodemo/h
MSRCPATH   +=  $(MMI_DIR)/source/mmi_app/app/autodemo/c
SOURCES	   +=  mmiautodemo_task.c mmiautodemo_api.c mmiautodemo_main.c
endif
ifeq ($(strip $(SPECIAL_EFFECT_ICON_SIZE)), 48X48)
MCFLAG_OPT += -DMAIN_MENU_48X48
else
MCFLAG_OPT += -DMAIN_MENU_60X60
endif

SOURCES	 += mmi_uilayer_test.c

ifeq ($(strip $(UPB_IDLE_SUPPORT)),TRUE)
SOURCES += mmiidle_upb.c
endif

ifeq ($(strip $(MENU_SWEETS_SUPPORT)), TRUE)
    SOURCES 	+= mmi_mainmenu_sweets.c
endif

#ifeq ($(strip $(SALES_SUPPORT)), TRUE)
#MCFLAG_OPT += -DSALES_SUPPORT
SOURCES	 += sales.c
MSRCPATH +=  $(MMI_DIR)/source/mmi_app/app/sales
MINCPATH +=  $(MMI_DIR)/source/mmi_app/app/sales
#endif

ifeq ($(strip $(SFR_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/selfreg/c
MINCPATH += $(MMI_DIR)/source/mmi_app/app/selfreg/h \
            Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/sdksrc/sdk \
            external/libc/h \
            external/libc/h/sys \
            external/libc/

SOURCES	 += mmisfr_main.c mmisfr_lwm2m.c sfr_module.c sfr_http_sender.c sfr_app.c sfr_nv.c sfr_http_parser.c sfr_sms.c sfr_json_maker.c \
            sfr_dm_interface.c sfr_carrier_manager.c
endif

ifeq ($(strip $(SFR_SUPPORT_CTCC_SMS)), TRUE)
SOURCES += sfr_carrier_ctcc.c
endif

ifeq ($(strip $(SFR_SUPPORT_CTCC)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/selfreg_ctcc/c
MINCPATH += $(MMI_DIR)/source/mmi_app/app/selfreg_ctcc/h

SOURCES	 += mmisfr_ctcc_main.c  mmisfr_ctcc_pdp.c mmisfr_ctcc_http.c mmisfr_ctcc_json.c
endif

ifeq ($(strip $(MMIAP_SUPPORT)), TRUE)
    ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 256X64)
        MMI_AP_F_HISTORY_PLAYLIST = TRUE
    endif

    ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 128X64)
        MMI_AP_F_HISTORY_PLAYLIST = TRUE
    endif

    ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 64X64)
        MMI_AP_F_HISTORY_PLAYLIST = TRUE
    endif

    ifeq ($(strip $(LOW_MEMORY_SUPPORT)), NONE)
        MMI_AP_F_HISTORY_PLAYLIST = TRUE
    endif

    #@if first search flag is true, then search all music
    ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 64X64)
        ifeq ($(strip $(MAINLCD_SIZE)), 240X400)
        MMI_AP_F_FIRST_SEARCH = TRUE
        endif
    endif

    #@后台非播放状态(包括暂停)，释放audio handle
    MMI_AP_F_RELEASE_BG_AUDIO = TRUE

    ##==============================================##
    ifeq ($(strip $(MMI_AP_F_HISTORY_PLAYLIST)), TRUE)
        MCFLAG_OPT += -DMMIAP_F_HISTORY_PLAYLIST
    endif

    ifeq ($(strip $(MMI_AP_F_FIRST_SEARCH)), TRUE)
        MCFLAG_OPT += -DMMIAP_F_FIRST_SEARCH
    endif

    ifeq ($(strip $(MMI_AP_F_RELEASE_BG_AUDIO)), TRUE)
        MCFLAG_OPT += -DMMIAP_F_RELEASE_BG_AUDIO
    endif




endif

ifeq ($(strip $(MMI_SMART_IDLE_SUPPORT)), SMALL_ICON)
SOURCES +=mmiidle_displaymini.c
endif

ifneq ($(strip $(NEST_WALLPAPER_SUPPORT)), FALSE)

		ifeq ($(strip $(MAINLCD_SIZE)), 240X320)
			MINCPATH +=  $(MMI_DIR)/source/mmi_app/app/livewallpaper/c/instance/springnest/240X320/inc
			MSRCPATH +=  $(MMI_DIR)/source/mmi_app/app/livewallpaper/c/instance/springnest/240X320/src
			SOURCES	   += Nest_PeachGarden_3D.c   Nest_MayaDatainc.c   Nest_QuatSlerpData.c     mmi_livewallpaper_nest.c
		else

			ifeq ($(strip $(MAINLCD_SIZE)), 320X480)
				MINCPATH +=  $(MMI_DIR)/source/mmi_app/app/livewallpaper/c/instance/springnest/320X480/inc
				MSRCPATH +=  $(MMI_DIR)/source/mmi_app/app/livewallpaper/c/instance/springnest/320X480/src
				SOURCES	   +=  Nest_PeachGarden_3D.c   Nest_MayaDatainc.c   Nest_QuatSlerpData.c     mmi_livewallpaper_nest.c
			endif
		endif

		ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
			MINCPATH +=  $(MMI_DIR)/source/mmi_app/app/livewallpaper/c/instance/springnest/176X220/inc
			MSRCPATH +=  $(MMI_DIR)/source/mmi_app/app/livewallpaper/c/instance/springnest/176X220/src
			SOURCES	   +=  Nest_PeachGarden_3D.c   Nest_MayaDatainc.c   Nest_QuatSlerpData.c     mmi_livewallpaper_nest.c
		endif
endif

#支持Mocor示例代码
ifeq ($(strip $(MMI_REFACTOR_SAMPLE)), TRUE)

SOURCES	 += mmiappsample_menutable.c mmiappsample_nv.c mmiappsample_id.c mmiappsample_appmain.c mmiappsample_mainwin.c mmiappsample_windatawin.c mmiappsample_eventwin.c mmiappsample_filterwin.c
SOURCES	 += mmiappsample_funcwinlist.c mmiappsample_funcwina.c mmiappsample_funcwinb.c mmiappsample_funcwinc.c
MINCPATH += $(MMI_DIR)/source/mmi_app/app/appsample/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/appsample/c

endif

ifeq ($(strip $(OD_GAME_SUPPORT)),TRUE)
    ifeq ($(findstring 7701,$(strip $(CHIP_VER))),7701)
	    MMI_PATH := MS_MMI_Main
    else
	    MMI_PATH := MS_MMI
    endif
include $(MMI_PATH)/source/mmi_app/app/od_game/build.mk
endif

#add volte src file
SOURCES  += mmiphone_volte.c

#add memtester src file
SOURCES  += mmieng_memtester.c

ifeq ($(strip $(MN_RPC_SUPPORT)), TRUE)
MINCPATH += MN_Rpc/export/inc
endif

#add dial_cl_contact FEATURE
ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/dial_cl_contact/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/dial_cl_contact/c
SOURCES	 +=  watch_dial_cl_contact_view.c watch_dial_cl_contact_data.c
endif


ifneq ($(strip $(BBM_ROLE_SUPPORT)), NONE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/bbm/c
MINCPATH += $(MMI_DIR)/source/mmi_app/app/bbm/h
SOURCES	 += bbm_control.c bbm_data.c
endif

ifeq ($(strip $(ESIM_SUPPORT)), HB)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/esim_hb/c
MINCPATH += $(MMI_DIR)/source/mmi_app/app/esim_hb/h
MINCPATH += Third-party/esim_hb/h
SOURCES	 += mmiesim_hb_app.c mmiesim_hb_id.c mmiesim_hb_main.c mmiesim_hb_connection.c
endif

ifeq ($(strip $(APP_STORE_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/appstore
MINCPATH += $(MMI_DIR)/source/mmi_app/app/appstore
SOURCES  += appstore_control.c appstore_home_win_data.c appstore_home_win_view.c appstore_flow.c appstore_operating_note_win_view.c appstore_uninstall_query_win_view.c appstore_operate_fail_note_win_view.c appstore_insufficient_space_note_win_view.c appstore_bg_operation.c
SOURCES  += appstore_no_data_note_win_view.c
endif

SOURCES  += json_parser_config.c

MSRCPATH += $(MMI_DIR)/source/mmi_app/common/cjson
MINCPATH += $(MMI_DIR)/source/mmi_app/common/cjson
SOURCES  += cJSON.c

ifeq ($(strip $(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT)), TRUE)
MINCPATH += JEE/include
MINCPATH += JEE/include/export
MINCPATH += JEE/src/packagemanager/inc
endif



ifeq ($(strip $(ALIIOT_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/aliiot/h
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/aliiot/c
MINCPATH += Third-party/alios/inc
MINCPATH += external/libc/h
MINCPATH += external/libc
SOURCES	 += aliiot_alipay_interface.c aliiot_alipay_menu.c mmialiiot_nv.c mmialiiot_id.c
endif

ifeq ($(strip $(DYNAMIC_WATCHFACE_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_app/app/watchfacemanage
MINCPATH += $(MMI_DIR)/source/mmi_app/app/watchfacemanage
SOURCES  += watchface_set_win_data.c watchface_set_win_view.c watchface_set_flow.c watchface_edit_win.c watchface_config.c watchface_delete_query_win_view.c watchface_set_note_win_view.c watchface_http.c watchface_set_operate_fail_note_win_view.c watchface_config_adapter.c watchface_multifunctional_dial.c \
watchface_factory_reset_waiting_win_view.c watchface_install_note_win_view.c

MSRCPATH += $(MMI_DIR)/source/mmi_app/app/watchface
MINCPATH += $(MMI_DIR)/source/mmi_app/app/watchface
SOURCES  += json_parser_watchface.c watchface_flow.c watchface_view.c
endif
SOURCES  += json_parser_config.c
MSRCPATH += $(MMI_DIR)/source/mmi_app/common/cjson
MINCPATH += $(MMI_DIR)/source/mmi_app/common/cjson
SOURCES  += cJSON.c

MSRCPATH += $(MMI_DIR)/source/mmi_app/common/ctrl_adapter
MINCPATH += $(MMI_DIR)/source/mmi_app/common/ctrl_adapter
SOURCES  += watchface_ctrl_adapter.c

ifeq ($(strip $(AUDIOPLAYER_DEMOAPP_SUPPORT)), TRUE)
SOURCES += audioplayer_demoapp_play_win_data.c audioplayer_demoapp_play_win_view.c audioplayer_demoapp_flow.c \
            audioplayer_demoapp_main_win.c
endif

XML_PATH = common/nv_parameters/common_config

RD_H_PATH = common/nv_parameters/common_config/h
RD_XML = engine_test_result.xml

MINCPATH += MS_MMI_Main/source/mmi_ctrl/source/Progressbar/h
MINCPATH += MS_MMI_Main/source/mmi_ctrl/source/basecontrol/h
MINCPATH += MS_MMI_Main/source/mmi_ctrl/source/TypeManager/h
ifeq ($(strip $(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT)), TRUE)
MINCPATH += MS_MMI_Main/source/mmi_app/app/permissionmanager/inc
MSRCPATH += MS_MMI_Main/source/mmi_app/app/permissionmanager/src
SOURCES += permission_check.c permission_ui.c
endif
MINCPATH += MS_MMI_Main/source/mmi_app/app/http/h
MSRCPATH += MS_MMI_Main/source/mmi_app/app/http/c
SOURCES += native_http.c

#ifeq ($(strip $(MMIAP_SUPPORT)), TRUE)
    SOURCES +=  mmiap_applet.c mmiap_control.c  mmiap_setting.c \
                mmiapwin_main.c mmiapwin_menu.c mmiapwin_set.c \
                mmiap_earphone.c mmiap_menutable.c mmiap_nv.c mmiap_id.c
    SOURCES += mmiap_appmain.c mmiapwin_main_mini.c mmiapwin_main_pda.c \
               mmiapwin_common.c mmiap_common.c mmiap_play.c
    SOURCES += mmiap_list.c mmiap_list_manager.c mmiapwin_multilist.c mmiap_list_control.c
    SOURCES += mmiap_musicmark.c mmiapwin_musicmark.c mmiap_musicarray.c
    SOURCES += mmiap_spectrum.c
    SOURCES += mmiap_lyric.c
    SOURCES += mmiap_album.c
    SOURCES += mmiap_lastinfo_ctrl.c mmiap_lastinfo_manager.c
    SOURCES += mmiap_multimediamenu.c
#endif

ifeq ($(strip $(UAL_TELE_SUPPORT)), TRUE)
MINCPATH   += ual/inc ual/inc/tele ual/src/tele/sim ual/src/tele ual/src/adap_common ual/src/tele/radio ual/src/tele/call
MINCPATH   += BASE/l4/export/inc common/export/inc BASE/PS/export/inc CAF/Template/include
endif

ifeq ($(strip $(SFR_SUPPORT_CMCC)), TRUE)
MINCPATH += MS_MMI_Main/source/mmi_app/app/selfreg_cmcc/h \
            MS_MMI_Main/source/mmi_app/app/selfreg/h \
            external/libc/h \
            external/libc/h/sys \
            external/libc \
            Third-party/liblwm2m/include
ifeq ($(strip $(UAL_TELE_SUPPORT)), FALSE)
MINCPATH += ual/inc/tele
endif
MSRCPATH += MS_MMI_Main/source/mmi_app/app/selfreg_cmcc/c

SOURCES += mmisfr_cmcc_main.c  mmisfr_cmcc_lwm2m.c
endif

ifeq ($(strip $(LEBAO_MUSIC_SUPPORT)), TRUE)
	MINCPATH += $(MMI_DIR)/source/mmi_app/app/lebao/applib/base/inc
	MINCPATH += $(MMI_DIR)/source/mmi_app/app/lebao/applib/base/src/simulator
	MINCPATH += $(MMI_DIR)/source/mmi_app/app/lebao/applib/v2/inc
	MINCPATH += $(MMI_DIR)/source/mmi_app/app/lebao/h
	MINCPATH += $(MMI_DIR)/source/mmi_app/app/lebao/music
	MINCPATH += $(MMI_DIR)/source/mmi_app/app/lebao/music/ctrl
	MINCPATH += $(MMI_DIR)/source/mmi_app/app/lebao/music/view
	MSRCPATH += $(MMI_DIR)/source/mmi_app/app/lebao/music/view
	SOURCES += lebao_wintable.c lebao_app.c lebao_win_base.c
endif

ifeq ($(strip $(VIDEO_CALL_AGORA_SUPPORT)), TRUE)
MINCPATH += Third-party/agora_rtc_sdk/include
#MINCPATH += MS_Ref/source/lcd/inc
MINCPATH += MS_MMI_Main/source/mmi_app/app/video_call/h

MINCPATH += MS_MMI_Main/source/mmi_app/app/video_call_demo/h
#MSRCPATH += MS_MMI_Main/source/mmi_app/app/video_call_mw/c
#SOURCES += agora_video_call.c g722_decode.c g722_encode.c avc_capture.c avc_decode.c avc_encode.c avc_rtc.c avc_render.c avc_platform_w217.c avc_pipeline.c avc_ringbuf.c avc_license.c tjpgd.c libyuv.c fps_cc.c

MSRCPATH += MS_MMI_Main/source/mmi_app/app/video_call/c
SOURCES += video_agora_api.c video_call_win.c
endif
#开机向导
ifeq ($(strip $(SETUP_WIZARD_SUPPORT)), TRUE)
MINCPATH += MS_MMI_Main/source/mmi_app/app/setupwizard/h
MSRCPATH += MS_MMI_Main/source/mmi_app/app/setupwizard/c
SOURCES += setup_wizard_win.c
endif

# ZDT_TOOLS_MENU_SUPPORT
ifeq ($(strip $(ZDT_TOOLS_MENU_SUPPORT)), TRUE)
MINCPATH += MS_MMI_Main/source/mmi_app/app/accessory/h
MSRCPATH += MS_MMI_Main/source/mmi_app/app/accessory/c
SOURCES += watch_tools_main.c
endif

##zmt add start
ifeq ($(strip $(MATH_COUNT_SUPPORT)), TRUE)
MINCPATH += MS_MMI_Main/source/mmi_app/app/math_count/h
#MINCPATH += Third-party/zmt_math/api/h
MSRCPATH +=  MS_MMI_Main/source/mmi_app/app/math_count/c
SOURCES +=  math_count_win.c
endif
ifeq ($(strip $(FORMULA_SUPPORT)), TRUE)
MINCPATH += MS_MMI_Main/source/mmi_app/app/formula/h
#MINCPATH += Third-party/zmt_formula/api/h
MSRCPATH += MS_MMI_Main/source/mmi_app/app/formula/c
SOURCES += formula_win.c
endif
ifeq ($(strip $(WORD_CARD_SUPPORT)), TRUE)
MINCPATH += MS_MMI_Main/source/mmi_app/app/zmt_word/h
#MINCPATH += Third-party/zmt_word/api/h
MSRCPATH +=  MS_MMI_Main/source/mmi_app/app/zmt_word/c
SOURCES +=  zmt_word_main.c zmt_word_id.c zmt_word_data.c zmt_main_file.c
endif
ifeq ($(strip $(HANZI_CARD_SUPPORT)), TRUE)
MINCPATH += MS_MMI_Main/source/mmi_app/app/zmt_hanzi/h
#MINCPATH += Third-party/zmt_hanzi/api/h
MSRCPATH +=  MS_MMI_Main/source/mmi_app/app/zmt_hanzi/c
SOURCES +=  zmt_hanzi_main.c zmt_hanzi_data.c zmt_main_file.c
endif
##zmt add end
ifeq ($(strip $(ZMT_PINYIN_SUPPORT)), TRUE)
MINCPATH		+=  MS_MMI_Main/source/mmi_app/app/zmt_pinyin/h
MSRCPATH		+=  MS_MMI_Main/source/mmi_app/app/zmt_pinyin/c
SOURCES			+=  zmt_pinyin_win.c zmt_pinyin_data.c
endif



ifeq ($(strip $(ZMT_YINBIAO_SUPPORT)), TRUE)
MINCPATH		+=  MS_MMI_Main/source/mmi_app/app/zmt_yinbiao/h
MSRCPATH		+=  MS_MMI_Main/source/mmi_app/app/zmt_yinbiao/c
SOURCES			+=  zmt_yinbiao_win.c zmt_yinbiao_data.c
endif