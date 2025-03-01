

//[[define image infomation
#if 0
RES_ADD_IMG(IMAGE_CDMA_SMARTCARD,"\\MMI_RES_##\\IMAG\\PWON\\IMAGE_SMARTCARD.gif",IMG_CMP_GIF_SCI,0)
#endif

#if 1
RES_ADD_STRING(STR_MESSAGE_URGENT,"urgent news")
RES_ADD_STRING(STR_MESSAGE_NOTICE,"notice")
RES_ADD_STRING(STR_MESSAGE_WORK,"work")
RES_ADD_STRING(STR_MESSAGE_ATTENDANCE,"attendance")
RES_ADD_STRING(STR_MESSAGE_COST,"cost")
RES_ADD_STRING(STR_MESSAGE_NEWS,"normal message")
RES_ADD_STRING(STR_MESSAGE_NO_NEWS,"no message")
RES_ADD_STRING(STR_MESSAGE_HAVE_MESSAGE,"message")
#endif
#ifdef ZDT_MESSAGE_DELETE_FEATRUE
RES_ADD_STRING(STR_VIEW_MESSAGE,"view message")
RES_ADD_STRING(STR_DELETE_MESSAGE,"delete message")
RES_ADD_STRING(STR_DELETE_MESSAGE_ALL,"delete all message")
RES_ADD_STRING(STR_CON_DELETE_MESSAGE,"confirm delete message?")
RES_ADD_STRING(STR_CON_DELETE_MESSAGE_ALL,"confirm delete all message?")
#endif
//[[define string infomation
RES_ADD_STRING(STR_CONTA_NOTE_NO_CONTA_TEXT,"Please through mobile App to add a contact")
RES_ADD_STRING(TXT_ZDT_TEST,"ZDT TEST")
RES_ADD_STRING(TXT_ZDT_FIND_WATCH,"Phone is finding watch")
RES_ADD_STRING(TXT_YX_TF_CARD_FULL,"TF Card Full")
RES_ADD_STRING(TXT_YX_NO_TF_CARD,"No TF Card")


RES_ADD_STRING(TXT_ZDT_MAINMENU_DIAL,"dial")
RES_ADD_STRING(TXT_ZDT_MAINMENU_2VM,"2vm")
RES_ADD_STRING(TXT_ZDT_MAINMENU_FRIEND,"jy")
RES_ADD_STRING(TXT_ZDT_MAINMENU_JB,"jb")
RES_ADD_STRING(TXT_ZDT_MAINMENU_JQR,"jqr")
RES_ADD_STRING(TXT_ZDT_MAINMENU_PB,"pb")
RES_ADD_STRING(TXT_ZDT_MAINMENU_CL,"Call Record")
RES_ADD_STRING(TXT_ZDT_MAINMENU_CAM,"Camera")
RES_ADD_STRING(TXT_ZDT_MAINMENU_CAM_SUB,"Camera")
RES_ADD_STRING(TXT_ZDT_MAINMENU_PHOTO,"photo")
RES_ADD_STRING(TXT_ZDT_MAINMENU_TORCH,"torch")
RES_ADD_STRING(TXT_ZDT_MAINMENU_TW,"tw")
RES_ADD_STRING(TXT_ZDT_MAINMENU_TEMP,"Temprature")
RES_ADD_STRING(TXT_ZDT_MAINMENU_HR,"hr")
RES_ADD_STRING(TXT_ZDT_MAINMENU_BLOOD,"Blood")
RES_ADD_STRING(TXT_ZDT_MAINMENU_OV,"OV")
RES_ADD_STRING(TXT_ZDT_MAINMENU_BROW,"Browser")
RES_ADD_STRING(TXT_ZDT_MAINMENU_SET,"Setting")
RES_ADD_STRING(TXT_ZDT_MAINMENU_MSG,"Message")
RES_ADD_STRING(TXT_ZDT_MAINMENU_CLASS,"Class")
#ifdef ZDT_CYHEALTH_SUPPORT
RES_ADD_STRING(TXT_ZDT_MAINMENU_HEALTH_1,"health1")
RES_ADD_STRING(TXT_ZDT_MAINMENU_HEALTH_2,"health2")
#endif

RES_ADD_STRING(TXT_ZDT_MAINMENU_ALARM,"闹钟")

RES_ADD_STRING(TXT_ZDT_MAINMENU_WCHAT,"wchat")
RES_ADD_STRING(TXT_ZDT_MAINMENU_ZFB,"zfb")
RES_ADD_STRING(TXT_ZDT_MAINMENU_PEDO,"pedometer")

RES_ADD_STRING(TXT_ZDT_MAINMENU_APP,"App")
RES_ADD_STRING(TXT_ZDT_MAINMENU_MG,"Music")
RES_ADD_STRING(TXT_ZDT_MAINMENU_AI,"AI")
RES_ADD_STRING(TXT_ZDT_MAINMENU_AV,"AV")
RES_ADD_STRING(TXT_ZDT_MAINMENU_XMLY,"FM")

RES_ADD_STRING(TXT_ZDT_CUR_PEDO,"current pedo")
RES_ADD_STRING(TXT_ZDT_CUR_HR,"current temprature")
RES_ADD_STRING(TXT_ZDT_CUR_TW,"current temprature")
RES_ADD_STRING(TXT_NO_CONTACT,"No Contacts")

RES_ADD_IMG(IMAGE_TINYCHAT_MIC,"\\MMI_RES_##\\IMAG\\tinychat\\mic.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TINYCHAT_TALK_ANIM_01,"\\MMI_RES_##\\IMAG\\tinychat\\talk_anim\\1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TINYCHAT_TALK_ANIM_02,"\\MMI_RES_##\\IMAG\\tinychat\\talk_anim\\2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TINYCHAT_TALK_ANIM_03,"\\MMI_RES_##\\IMAG\\tinychat\\talk_anim\\3.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TINYCHAT_TALK_ANIM_04,"\\MMI_RES_##\\IMAG\\tinychat\\talk_anim\\4.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_TINYCHAT_VOICE_LEFT_BG,"\\MMI_RES_##\\IMAG\\tinychat\\left_voice_bg.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TINYCHAT_VOICE_RIGHT_BG,"\\MMI_RES_##\\IMAG\\tinychat\\rihgt_voice_bg.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_TINYCHAT_PLAYING_AUDIO,"\\MMI_RES_##\\IMAG\\tinychat\\icon_playing_audio.png",IMG_CMP_PNG_ARGB_RLE,0)

#ifdef ZDT_PLAT_SHB_SUPPORT
RES_ADD_STRING(STXT_SHB_FIRST_ALERT,"Welcom")
RES_ADD_STRING(STXT_SHB_WELCOME,"Welcom")
RES_ADD_STRING(STXT_SHB_AGREE,"Agree")
RES_ADD_STRING(STXT_SHB_DISAGREE,"Disagree")
RES_ADD_STRING(STXT_SHB_BIND,"Agree?")
RES_ADD_STRING(STXT_SHB_FIRST_ALERT_1,"Welcom")
RES_ADD_STRING(STXT_SHB_FIRST_ALERT_2,"Welcom")
RES_ADD_STRING(STXT_SHB_FIRST_ALERT_3,"Welcom")
RES_ADD_STRING(STXT_SHB_FIRST_ALERT_4,"Welcom")
RES_ADD_STRING(STXT_SHB_CLOSE,"Close")
RES_ADD_STRING(STXT_SHB_OK,"OK")
#endif

#ifdef ZDT_MAIN_MENU_SUPPORT
#ifndef MAIN_MENU_FOUR_GRID_STYLE
#ifdef TOUCH_PANEL_SUPPORT
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_DIAL,"\\MMI_RES_##\\IMAG\\mainmenu\\dial.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_PB,"\\MMI_RES_##\\IMAG\\mainmenu\\pb.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifdef ZDT_MAINMENU_ADD_CL_REMOVE_ALIPAY
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_CL,"\\MMI_RES_##\\IMAG\\mainmenu\\cl.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef ZDT_GSENSOR_SUPPORT
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_JB,"\\MMI_RES_##\\IMAG\\mainmenu\\jb.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef TORCH_SUPPORT
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_TORCH,"\\MMI_RES_##\\IMAG\\mainmenu\\torch.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#if defined(ZDT_HSENSOR_SUPPORT) && !defined(ZDT_HSENSOR_SUPPORT_TW)
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_HR,"\\MMI_RES_##\\IMAG\\mainmenu\\hr.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#if defined(ZDT_HSENSOR_SUPPORT_TW) || defined(ZDT_SUPPORT_TW_GD609) || defined(ZDT_SUPPORT_TW_FM78102)
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_TW,"\\MMI_RES_##\\IMAG\\mainmenu\\tw.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_WL,"\\MMI_RES_##\\IMAG\\mainmenu\\wl.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef CAMERA_SUPPORT
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_CAM,"\\MMI_RES_##\\IMAG\\mainmenu\\cam.png",IMG_CMP_PNG_ARGB_RLE,0)
#if MAIN_MENU_NEED_SUB_CAM
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_CAM_SUB,"\\MMI_RES_##\\IMAG\\mainmenu\\cam_sub.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_PHOTO,"\\MMI_RES_##\\IMAG\\mainmenu\\photo.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef TULING_AI_SUPPORT 
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_AI,"\\MMI_RES_##\\IMAG\\mainmenu\\ai.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef BAIRUI_VIDEOCHAT_SUPPORT 
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_AV,"\\MMI_RES_##\\IMAG\\mainmenu\\av.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef LEBAO_MUSIC_SUPPORT 
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_MG,"\\MMI_RES_##\\IMAG\\mainmenu\\mg.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef XYSDK_SUPPORT
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_XMLY,"\\MMI_RES_##\\IMAG\\mainmenu\\xmly.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif		
#ifdef ZDT_ZFB_SUPPORT
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_ZFB,"\\MMI_RES_##\\IMAG\\mainmenu\\zfb.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif		
#ifdef BROWSER_SUPPORT
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_BROW,"\\MMI_RES_##\\IMAG\\mainmenu\\browser.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_EWM,"\\MMI_RES_##\\IMAG\\mainmenu\\ewm.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_SET,"\\MMI_RES_##\\IMAG\\mainmenu\\set.png",IMG_CMP_PNG_ARGB_RLE,0)

#ifdef ZDT_PLAT_YX_SUPPORT_LZ
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_HEALTH,"\\MMI_RES_##\\IMAG\\mainmenu\\health.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_TRAVEL,"\\MMI_RES_##\\IMAG\\mainmenu\\travel.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_NUCLEICACID,"\\MMI_RES_##\\IMAG\\mainmenu\\NUCLEICACID.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_VAA,"\\MMI_RES_##\\IMAG\\mainmenu\\VAA.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#if defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_MESSAGE,"\\MMI_RES_##\\IMAG\\mainmenu\\MESSAGE.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef MMI_ADD_STUDENT_INFO
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_STUDENT_INFO,"\\MMI_RES_##\\IMAG\\mainmenu\\studentinfo.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_STUDENT_IMAGE,"\\MMI_RES_##\\IMAG\\mainmenu\\studentimage.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef ZDT_ClASS_ANSWER_SUPPORT
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_CLASS,"\\MMI_RES_##\\IMAG\\mainmenu\\class.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef ZDT_CYHEALTH_SUPPORT
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_HEALTH_1,"\\MMI_RES_##\\IMAG\\mainmenu\\health_1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_HEALTH_2,"\\MMI_RES_##\\IMAG\\mainmenu\\health_2.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

#endif

#ifdef LOCAL_ALARM_CLOCK_SUPPORT
RES_ADD_IMG(IMAGE_MAIN_MENU_ZDT_ALARM,"\\MMI_RES_##\\IMAG\\mainmenu\\alarm.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

#endif

#ifdef ZDT_PLAT_YX_SUPPORT_LZ
RES_ADD_IMG(IMAGE_TRIP_GREEN,"\\MMI_RES_##\\IMAG\\health\\trip_green.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TRIP_YELLOW,"\\MMI_RES_##\\IMAG\\health\\trip_yellow.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_HEALTH_CODE,"\\MMI_RES_##\\IMAG\\health\\health_code.png",IMG_CMP_PNG_ARGB_RLE,0)
#elif defined(ZDT_PLAT_YX_SUPPORT_CY)
#ifdef ZDT_CYHEALTH_SUPPORT
RES_ADD_IMG(IMAGE_TRIP_GREEN,"\\MMI_RES_##\\IMAG\\health\\pass.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TRIP_RED,"\\MMI_RES_##\\IMAG\\health\\nopass.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_HEALTH_EMPTY,"\\MMI_RES_##\\IMAG\\health\\empty.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_HEALTH_YIN,"\\MMI_RES_##\\IMAG\\health\\yin.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_HEALTH_YANG,"\\MMI_RES_##\\IMAG\\health\\yang.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_HEALTH_YGYM,"\\MMI_RES_##\\IMAG\\health\\xgym.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#endif

#if 1
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY0_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY0_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY0_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY0_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY1_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY1_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY1_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY1_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY2_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY2_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY2_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY2_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY3_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY3_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY3_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY3_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY4_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY4_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY4_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY4_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY5_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY5_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY5_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY5_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY6_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY6_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY6_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY6_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY7_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY7_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY7_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY7_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY8_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY8_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY8_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY8_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY9_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY9_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEY9_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEY9_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEYSTAR_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEYSTAR_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEYSTAR_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEYSTAR_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEYHASH_ICON_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEYHASH_ICON_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_NUMBER_KEYHASH_ICON,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_NUMBER_KEYHASH_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)


RES_ADD_IMG(IMAGE_ZDT_DIAL_EDIT_BG,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_DIAL_EDIT_BG.png", IMG_CMP_PNG_SABM_RLE | SABM_HOR_STRETCH,0)

RES_ADD_IMG(IMAGE_ZDT_BUTTON_DELKEY,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_DELKEY.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_BUTTON_DELKEY_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_DELKEY_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_BUTTON_DELKEY_BG_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_DELKEY_BG_P.png", IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_ZDT_BUTTON_HIDE_KEYBOARD,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_HIDE_KEYBOARD.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_BUTTON_HIDE_KEYBOARD_2,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_HIDE_KEYBOARD_2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_BUTTON_HIDE_KEYBOARD_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_HIDE_KEYBOARD_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_BUTTON_HIDE_KEYBOARD_P_2,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_HIDE_KEYBOARD_P_2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_BUTTON_SHOW_KEYBOARD,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_SHOW_KEYBOARD.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_BUTTON_SHOW_KEYBOARD_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_SHOW_KEYBOARD_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_BUTTON_DIAL,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_DIAL.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_BUTTON_DIAL_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_DIAL_P.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_BUTTON_FUNC_BG_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_FUNC_BG_P.png", IMG_CMP_PNG_ARGB_RLE,0)

//RES_ADD_IMG(IMAGE_ZDT_BUTTON_SMS,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_SMS.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_ZDT_BUTTON_SMS_P,"\\MMI_RES_##\\IMAG\\Dial\\IMAGE_BUTTON_SMS_P.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

#ifdef ZTE_WATCH
RES_ADD_IMG(IMAGE_ZDT_PB_CALL,"\\MMI_RES_##\\IMAG\\PB\\call.png",IMG_CMP_PNG_ARGB,0)
RES_ADD_IMG(IMAGE_ZDT_PB_FATHER,"\\MMI_RES_##\\IMAG\\PB\\zte\\father.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_MOTHER,"\\MMI_RES_##\\IMAG\\PB\\zte\\mother.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_GRANDPA,"\\MMI_RES_##\\IMAG\\PB\\zte\\grandpa.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_GRANDMA,"\\MMI_RES_##\\IMAG\\PB\\zte\\grandma.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_SISTER,"\\MMI_RES_##\\IMAG\\PB\\zte\\sister.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_BROTHER,"\\MMI_RES_##\\IMAG\\PB\\zte\\brother.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_OTHER,"\\MMI_RES_##\\IMAG\\PB\\zte\\other.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_GROUP,"\\MMI_RES_##\\IMAG\\PB\\zte\\group.png",IMG_CMP_PNG_ARGB_RLE,0)
// big image
RES_ADD_IMG(IMAGE_ZDT_PB_FATHER_BIG,"\\MMI_RES_##\\IMAG\\PB\\zte\\father_big.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_MOTHER_BIG,"\\MMI_RES_##\\IMAG\\PB\\zte\\mother_big.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_GRANDPA_BIG,"\\MMI_RES_##\\IMAG\\PB\\zte\\grandpa_big.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_GRANDMA_BIG,"\\MMI_RES_##\\IMAG\\PB\\zte\\grandma_big.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_SISTER_BIG,"\\MMI_RES_##\\IMAG\\PB\\zte\\sister_big.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_BROTHER_BIG,"\\MMI_RES_##\\IMAG\\PB\\zte\\brother_big.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_OTHER_BIG,"\\MMI_RES_##\\IMAG\\PB\\zte\\other_big.png",IMG_CMP_PNG_ARGB_RLE,0)
#else
RES_ADD_IMG(IMAGE_ZDT_PB_CALL,"\\MMI_RES_##\\IMAG\\PB\\call.png",IMG_CMP_PNG_ARGB,0)
RES_ADD_IMG(IMAGE_ZDT_PB_FATHER,"\\MMI_RES_##\\IMAG\\PB\\father.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_MOTHER,"\\MMI_RES_##\\IMAG\\PB\\mother.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_GRANDPA,"\\MMI_RES_##\\IMAG\\PB\\grandpa.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_GRANDMA,"\\MMI_RES_##\\IMAG\\PB\\grandma.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_UNCLE,"\\MMI_RES_##\\IMAG\\PB\\shushu.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_AUNT,"\\MMI_RES_##\\IMAG\\PB\\ayi.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_WAIGONG,"\\MMI_RES_##\\IMAG\\PB\\waigong.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_WAIPO,"\\MMI_RES_##\\IMAG\\PB\\waipo.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_OTHERMAN,"\\MMI_RES_##\\IMAG\\PB\\other_man.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_PB_OTHERWOMAN,"\\MMI_RES_##\\IMAG\\PB\\other_woman.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif


RES_ADD_IMG(IMAGE_ZDT_JP_BG,"\\MMI_RES_##\\IMAG\\jp\\jp_bg.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_YX_2VM,"\\MMI_RES_##\\IMAG\\2vm\\2vm_yx.png",IMG_CMP_PNG_ARGB_RLE,0)

#ifdef ZTE_WATCH
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM1,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM2,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM3,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim3.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM4,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim4.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM5,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim5.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM6,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim6.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM7,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim7.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM8,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim8.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM9,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim9.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM10,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim10.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM11,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim11.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM12,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim12.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM13,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim13.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM14,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_anim14.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_FULL,"\\MMI_RES_##\\IMAG\\charge\\zte\\zte_charging_full.png",IMG_CMP_PNG_ARGB_RLE,0)
#else
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM1,"\\MMI_RES_##\\IMAG\\charge\\charging_anim1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM2,"\\MMI_RES_##\\IMAG\\charge\\charging_anim2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM3,"\\MMI_RES_##\\IMAG\\charge\\charging_anim3.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM4,"\\MMI_RES_##\\IMAG\\charge\\charging_anim4.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM5,"\\MMI_RES_##\\IMAG\\charge\\charging_anim5.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM6,"\\MMI_RES_##\\IMAG\\charge\\charging_anim6.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_ANIM7,"\\MMI_RES_##\\IMAG\\charge\\charging_anim7.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_FULL,"\\MMI_RES_##\\IMAG\\charge\\charging_full.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZDT_CHARGING_BG,"\\MMI_RES_##\\IMAG\\charge\\charging_bg.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

RES_ADD_IMG(IMAGE_TINYCHAT_HEAD,"\\MMI_RES_##\\IMAG\\tinychat\\header_img.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TINYCHAT_HEAD_SERVER,"\\MMI_RES_##\\IMAG\\tinychat\\header_img_server.png",IMG_CMP_PNG_ARGB_RLE,0)   //xiongkai
RES_ADD_IMG(IMAGE_TINYCHAT_AUDIO_LEFT,"\\MMI_RES_##\\IMAG\\tinychat\\icon_playing_audio_left.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TINYCHAT_AUDIO_RIGHT,"\\MMI_RES_##\\IMAG\\tinychat\\icon_playing_audio_right.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TINYCHAT_UNREAD,"\\MMI_RES_##\\IMAG\\tinychat\\unread.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
RES_ADD_IMG(IMAGE_PP_BG,"\\MMI_RES_##\\IMAG\\pp\\pp_bg.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_PP_FAIL,"\\MMI_RES_##\\IMAG\\pp\\pp_fail.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_PP_SUCCESS,"\\MMI_RES_##\\IMAG\\pp\\pp_success.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

#if defined(ZDT_NEW_BIND_STYLE)
RES_ADD_IMG(IMAGE_WATCH_BIND_STEP_1,"\\MMI_RES_##\\IMAG\\2vm\\bind_step_1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_WATCH_BIND_STEP_2,"\\MMI_RES_##\\IMAG\\2vm\\bind_step_2.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

#ifndef ZDT_NEW_BIND_STYLE   //  xiongkai  去掉没用到的图片  节省空间        
RES_ADD_IMG(IMAGE_QRCODE_SELECT_BG,"\\MMI_RES_##\\IMAG\\2vm\\select_qrcode_bg.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

RES_ADD_STRING(TXT_NET_2VM_DOWNLOAD,"Download App")
RES_ADD_STRING(TXT_NET_2VM_BIND,"Bind Watch")
RES_ADD_IMG(IMAGE_FIND_WATCH,"\\MMI_RES_##\\IMAG\\find_watch\\find_watch.gif",IMG_CMP_GIF_SCI,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_0,"\\MMI_RES_##\\IMAG\\classmode\\0.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_1,"\\MMI_RES_##\\IMAG\\classmode\\1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_2,"\\MMI_RES_##\\IMAG\\classmode\\2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_3,"\\MMI_RES_##\\IMAG\\classmode\\3.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_4,"\\MMI_RES_##\\IMAG\\classmode\\4.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_5,"\\MMI_RES_##\\IMAG\\classmode\\5.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_6,"\\MMI_RES_##\\IMAG\\classmode\\6.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_7,"\\MMI_RES_##\\IMAG\\classmode\\7.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_8,"\\MMI_RES_##\\IMAG\\classmode\\8.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_9,"\\MMI_RES_##\\IMAG\\classmode\\9.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_NUM_COLON,"\\MMI_RES_##\\IMAG\\classmode\\colon.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLASSMODE_BG,"\\MMI_RES_##\\IMAG\\classmode\\classmode_bg.png",IMG_CMP_PNG_ARGB_RLE,0)

#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
RES_ADD_IMG(IMAGE_CHAT_RECORDING_BG,"\\MMI_RES_##\\IMAG\\tinychat\\recording_bg.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CHAT_NO_RECORD_BG,"\\MMI_RES_##\\IMAG\\tinychat\\no_record_bg.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

#if defined(ZDT_HSENSOR_SUPPORT) && !defined(ZDT_HSENSOR_SUPPORT_TW)
//RES_ADD_IMG(IMAGE_HEARTRATE_ANIM_0,"\\MMI_RES_##\\IMAG\\heartrate\\0.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_HEARTRATE_ANIM_1,"\\MMI_RES_##\\IMAG\\heartrate\\1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_HEARTRATE_ANIM_2,"\\MMI_RES_##\\IMAG\\heartrate\\2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_HEARTRATE_ANIM_3,"\\MMI_RES_##\\IMAG\\heartrate\\3.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_HEARTRATE_ANIM_4,"\\MMI_RES_##\\IMAG\\heartrate\\4.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_HEARTRATE_ANIM_5,"\\MMI_RES_##\\IMAG\\heartrate\\5.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

#if defined(ZDT_HSENSOR_SUPPORT_TW) || defined(ZDT_SUPPORT_TW_GD609) || defined(ZDT_SUPPORT_TW_FM78102)
RES_ADD_IMG(IMAGE_TW_ANIM_1,"\\MMI_RES_##\\IMAG\\tw\\1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TW_ANIM_2,"\\MMI_RES_##\\IMAG\\tw\\2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TW_ANIM_3,"\\MMI_RES_##\\IMAG\\tw\\3.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TW_ANIM_4,"\\MMI_RES_##\\IMAG\\tw\\4.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_TW_ANIM_5,"\\MMI_RES_##\\IMAG\\tw\\5.png",IMG_CMP_PNG_ARGB_RLE,0)

//RES_ADD_IMG(IMAGE_HEARTRATE_ANIM_6,"\\MMI_RES_##\\IMAG\\heartrate\\6.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
RES_ADD_STRING(TXT_WEATHER_TODAY_WEATHER,"Today WT:")
RES_ADD_STRING(TXT_WEATHER_CUR_TEMPERATURE,"Cur temp:")
RES_ADD_STRING(TXT_WEATHER_TEMPERATURE_DU,"du")

RES_ADD_STRING(TXT_WEATHER_SUNNY,"1")
RES_ADD_STRING(TXT_WEATHER_RAIN,"2")
RES_ADD_STRING(TXT_WEATHER_CLOUD,"3")
RES_ADD_STRING(TXT_WEATHER_SNOW,"4")
RES_ADD_STRING(TXT_WEATHER_UNKNOW,"5")

RES_ADD_IMG(IMAGE_WEATHER_WINDY,"\\MMI_RES_##\\IMAG\\weather\\ic_windy.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_WEATHER_UNKNOWN,"\\MMI_RES_##\\IMAG\\weather\\ic_unknown.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_WEATHER_STORMS,"\\MMI_RES_##\\IMAG\\weather\\ic_t_storms.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_WEATHER_SUNNY,"\\MMI_RES_##\\IMAG\\weather\\ic_sunny.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_WEATHER_SNOW,"\\MMI_RES_##\\IMAG\\weather\\ic_snow.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_WEATHER_SLEET,"\\MMI_RES_##\\IMAG\\weather\\ic_sleet.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_WEATHER_SHOWERS,"\\MMI_RES_##\\IMAG\\weather\\ic_showers.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_WEATHER_CLOUDY,"\\MMI_RES_##\\IMAG\\weather\\ic_cloudy.png",IMG_CMP_PNG_ARGB_RLE,0)

#if 0//defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
RES_ADD_IMG(IMAGE_QRCODE_MANUAL,"\\MMI_RES_##\\IMAG\\2vm\\manual_bak.png",IMG_CMP_PNG_ARGB_RLE,0)
//#else
RES_ADD_IMG(IMAGE_QRCODE_MANUAL,"\\MMI_RES_##\\IMAG\\2vm\\manual.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

RES_ADD_IMG(IMAGE_BIG_WEATHER_SUNNY,"\\MMI_RES_##\\IMAG\\weather\\BIG_IMG\\sun.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_BIG_WEATHER_RAIN,"\\MMI_RES_##\\IMAG\\weather\\BIG_IMG\\rain.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_BIG_WEATHER_CLOUD,"\\MMI_RES_##\\IMAG\\weather\\BIG_IMG\\cloud.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_BIG_WEATHER_SNOW,"\\MMI_RES_##\\IMAG\\weather\\BIG_IMG\\snow.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_BIG_WEATHER_UNKNOW,"\\MMI_RES_##\\IMAG\\weather\\BIG_IMG\\unknow.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_BIG_WEATHER_BG,"\\MMI_RES_##\\IMAG\\weather\\BIG_IMG\\bg.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_STRING(TXT_TEST_SWVER,"Softver")
RES_ADD_STRING(TXT_TEST_SIM,"Sim")
RES_ADD_STRING(TXT_TEST_LOOPBACK,"Loopback")
RES_ADD_STRING(TXT_TEST_SPEAKER,"Speaker")
RES_ADD_STRING(TXT_TEST_KEY,"Key")
RES_ADD_STRING(TXT_TEST_MAINL,"Main LCD")
RES_ADD_STRING(TXT_TEST_TP,"TP")
RES_ADD_STRING(TXT_TEST_VIB,"Vibrate")
RES_ADD_STRING(TXT_TEST_WIFI,"Wifi")
RES_ADD_STRING(TXT_TEST_GSENSOR,"Gsensor")
RES_ADD_STRING(TXT_TEST_HSENSOR,"Hsensor")
RES_ADD_STRING(TXT_TEST_HSENSOR_PRE,"HsensorPre")
RES_ADD_STRING(TXT_TEST_TW,"Temp")
RES_ADD_STRING(TXT_TEST_ECG,"ECG")
RES_ADD_STRING(TXT_TEST_CAMERA_MAIN,"Main Cam")
RES_ADD_STRING(TXT_TEST_CAMERA_SUB,"Sub Cam")
RES_ADD_STRING(TXT_TEST_CALL,"Call")
RES_ADD_STRING(TXT_TEST_RESTORE,"Restore")
RES_ADD_STRING(TXT_TEST_FOTA,"Fota")
RES_ADD_STRING(TXT_TEST_GPS,"GPS")
RES_ADD_STRING(TXT_TEST_WL,"WL")
RES_ADD_STRING(TXT_TEST_NFC,"NFC")

RES_ADD_STRING(TXT_HEALTH_GET_WAIT,"Waiting")
RES_ADD_STRING(TXT_HEALTH_HEALTH_CODE,"Health Code")
RES_ADD_STRING(TXT_HEALTH_GREEN_CODE,"Green Code")
RES_ADD_STRING(TXT_HEALTH_Red_CODE,"Red Code")
RES_ADD_STRING(TXT_HEALTH_Yellow_CODE,"Yellow Code")
RES_ADD_STRING(TXT_HEALTH_Grey_CODE,"Grey Code")
RES_ADD_STRING(TXT_HEALTH_OTHER,"Other")
RES_ADD_STRING(TXT_HEALTH_TRAVEL,"Travel")
RES_ADD_STRING(TXT_HEALTH_NUCLEDIC_ACID,"Nucleic acid")
RES_ADD_STRING(TXT_HEALTH_HOUR,"hour")
RES_ADD_STRING(TXT_HEALTH_ACID_FEMIN,"feminine")
RES_ADD_STRING(TXT_HEALTH_ACID_POSITIVE,"positive")
RES_ADD_STRING(TXT_HEALTH_COMPLETE,"complete")
RES_ADD_STRING(TXT_HEALTH_NEEDLE,"needle")
RES_ADD_STRING(TXT_HEALTH_NEW_VACC,"new crown vaccine")

RES_ADD_STRING(TXT_TINYCHAT_RECORDING,"Recording")

RES_ADD_STRING(TXT_WLDT_YESNO,"Yes Or No")
RES_ADD_STRING(TXT_WLDT_SINGLE,"Single")
RES_ADD_STRING(TXT_WLDT_MULTI,"Multi")
RES_ADD_STRING(TXT_WLDT_ANSWER_YES,"Yes")
RES_ADD_STRING(TXT_WLDT_ANSWER_NO,"No")
RES_ADD_STRING(TXT_WLDT_ANSWER_A,"A")
RES_ADD_STRING(TXT_WLDT_ANSWER_B,"B")
RES_ADD_STRING(TXT_WLDT_ANSWER_C,"C")
RES_ADD_STRING(TXT_WLDT_ANSWER_D,"D")

RES_ADD_IMG(IMAGE_INSERT_SIM_TIP,"\\MMI_RES_##\\IMAG\\insert_sim_tip.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_STRING(TXT_NO_SIM_OR_DATA,"No SIM card or network data")

RES_ADD_IMG(IMAGE_WEATHER_LOCATION,"\\MMI_RES_##\\IMAG\\dropdown\\weather_location.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_DROPDOWN_SETTINGS,"\\MMI_RES_##\\IMAG\\dropdown\\dropdown_settings.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_DROPDOWN_TOOL,"\\MMI_RES_##\\IMAG\\dropdown\\dropdown_tool.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_DROPDOWN_SOUND_OPEN,"\\MMI_RES_##\\IMAG\\dropdown\\dropdown_sound_open.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_DROPDOWN_SOUND_CLOSE,"\\MMI_RES_##\\IMAG\\dropdown\\dropdown_sound_close.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_PHONE_ICON,"\\MMI_RES_##\\IMAG\\PB\\phone.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_VIDEO_CALL_ICON,"\\MMI_RES_##\\IMAG\\video_call\\video_call_icon.png",IMG_CMP_PNG_ARGB_RLE,0)


#if defined(ZDT_PLAT_YX_SUPPORT_FRIEND)
RES_ADD_IMG(find_friend_ping,"\\MMI_RES_##\\IMAG\\friend\\icon_watch_blue.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(find_friend_pong,"\\MMI_RES_##\\IMAG\\friend\\icon_watch_pink.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(find_friend_fail,"\\MMI_RES_##\\IMAG\\friend\\icon_fail.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(find_friend_success,"\\MMI_RES_##\\IMAG\\friend\\icon_success.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(find_friend_waiting1,"\\MMI_RES_##\\IMAG\\friend\\find_friend_waiting1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(find_friend_waiting2,"\\MMI_RES_##\\IMAG\\friend\\find_friend_waiting2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(find_friend_waiting3,"\\MMI_RES_##\\IMAG\\friend\\find_friend_waiting3.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(find_friend_waiting4,"\\MMI_RES_##\\IMAG\\friend\\find_friend_waiting4.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(find_friend_waiting5,"\\MMI_RES_##\\IMAG\\friend\\find_friend_waiting5.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_STRING(TXT_FIND_FIREND_SEARCH,"Searching")
RES_ADD_STRING(TXT_FIND_FIREND_SEARCH_TIP,"Find in same time")
RES_ADD_STRING(TXT_FIND_FIREND_FAIL_TIP,"Find together")

#endif

#ifdef ZTE_WATCH
RES_ADD_IMG(IMAGE_ZTE_DROP_PROGRESS, "\\MMI_RES_##\\IMAG\\statusbar\\IMAGE_ZTE_DROP_PROGRESS.png", IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ZTE_DROP_PROGRESS_BG, "\\MMI_RES_##\\IMAG\\statusbar\\IMAGE_ZTE_DROP_PROGRESS_BG.png", IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(statusbar_flashlight,"\\MMI_RES_##\\IMAG\\statusbar\\statusbar_flashlight.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(statusbar_long_battery,"\\MMI_RES_##\\IMAG\\statusbar\\statusbar_long_battery.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(statusbar_brightness,"\\MMI_RES_##\\IMAG\\statusbar\\bright_full.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(statusbar_volume,"\\MMI_RES_##\\IMAG\\statusbar\\volume_full.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(statusbar_ring_and_vibrate,"\\MMI_RES_##\\IMAG\\statusbar\\ring_and_vibrate.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

RES_ADD_STRING(TXT_FATHER,"Father") //爸爸
RES_ADD_STRING(TXT_MOTHER,"Mother") //妈妈
RES_ADD_STRING(TXT_FATHER_EX,"Father") //父亲
RES_ADD_STRING(TXT_MOTHER_EX,"Mother") //母亲
RES_ADD_STRING(TXT_GRANDPA,"Grandpa")
RES_ADD_STRING(TXT_GRANDMA,"Grandma")
RES_ADD_STRING(TXT_SISTER,"Sister")
RES_ADD_STRING(TXT_BROTHER,"Brother")
RES_ADD_STRING(TXT_YOUNGER_SISTER,"Younger Sister")
RES_ADD_STRING(TXT_YOUNGER_BROTHER,"Younger Brother")
RES_ADD_STRING(TXT_UNCLE,"Uncle")
RES_ADD_STRING(TXT_AUNT,"AUNT")
RES_ADD_STRING(TXT_WAIPO,"Grandma")
RES_ADD_STRING(TXT_WAIGONG,"Grandpa")
RES_ADD_STRING(TXT_GROUP,"Group")

RES_ADD_STRING(TXT_VOLTE_TIP_TEXT,"无高清语音服务")
