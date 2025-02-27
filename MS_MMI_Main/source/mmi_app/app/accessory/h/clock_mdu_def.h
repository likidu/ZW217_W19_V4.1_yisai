//[[define image infomation
RES_ADD_IMG(IMAGE_CLOCK_ALARM_ALERT,"\\MMI_RES_##\\IMAG\\Alarm\\IMAGE_CLOCK_ALARM_ALERT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_ALARM_WEEK_PRESSED,"\\MMI_RES_##\\IMAG\\Alarm\\IMAGE_CLOCK_ALARM_WEEK_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_ALARM_WEEK_ENABLED,"\\MMI_RES_##\\IMAG\\Alarm\\IMAGE_CLOCK_ALARM_WEEK_ENABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_ALARM_WEEK_DISABLED,"\\MMI_RES_##\\IMAG\\Alarm\\IMAGE_CLOCK_ALARM_WEEK_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_ALARM_WEEK_MARK,"\\MMI_RES_##\\IMAG\\Alarm\\IMAGE_CLOCK_ALARM_WEEK_MARK.png",IMG_CMP_PNG_ARGB_RLE,0)

//RES_ADD_IMG(IMAGE_CALENDAR_ALERT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_ALERT.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_SCHEDULE_ARRANGE_01,"\\MMI_RES_##\\IMAG\\Schedule\\IMAGE_SCHEDULE_ARRANGE_1.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(IMAGE_ALARM_ARRANGE,"\\MMI_RES_##\\IMAG\\Schedule\\IMAGE_ALARM_ARRANGE.bmp",IMG_CMP_BMP_16_565_RLE,0)

#ifndef MMI_ALARM_MINI_SUPPORT

RES_ADD_IMG(IMAGE_CLOCK_ALARM_TAB_FOCUSED,"\\MMI_RES_##\\IMAG\\Alarm\\IMAGE_CLOCK_ALARM_TAB_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#if defined MMI_ISTYLE_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_ALARM_ISTYLETAB_DEFAULT,"\\MMI_RES_##\\IMAG\\Alarm\\IMAGE_CLOCK_ALARM_ISTYLETAB_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_ALARM_ISTYLETAB_FOCUSED,"\\MMI_RES_##\\IMAG\\Alarm\\IMAGE_CLOCK_ALARM_ISTYLETAB_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#endif
RES_ADD_IMG(IMAGE_CLOCK_ALARM_TAB_DEFAULT,"\\MMI_RES_##\\IMAG\\Alarm\\IMAGE_CLOCK_ALARM_TAB_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)

//RES_ADD_IMG(IMAGE_CALENDAR_BG,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_BG.png",IMG_CMP_PNG_SABM_RLE |SABM_HOR_STRETCH,0)
//RES_ADD_IMG(IMAGE_CALENDAR_LEFT_ARROW,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_LEFT_ARROW.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_CALENDAR_LEFT_ARROW_DOWN,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_LEFT_ARROW_DOWN.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_CALENDAR_RIGHT_ARROW,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_RIGHT_ARROW.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_CALENDAR_RIGHT_ARROW_DOWN,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_RIGHT_ARROW_DOWN.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_CALENDAR_WEEK_BG,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_WEEK_BG.png",IMG_CMP_PNG_SABM_RLE |SABM_HOR_STRETCH,0)
//RES_ADD_IMG(IMAGE_CALENDAR_UP_ARROW,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_UP_ARROW.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_CALENDAR_UP_ARROW_DOWN,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_UP_ARROW_DOWN.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_CALENDAR_DOWN_ARROW,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_DOWN_ARROW.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_CALENDAR_DOWN_ARROW_DOWN,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_DOWN_ARROW_DOWN.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_CALENDAR_WEEK_DOT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_WEEK_DOT.png",IMG_CMP_PNG_ARGB_RLE,0)

#ifdef MMI_PDA_SUPPORT
#if 0
RES_ADD_IMG(IMG_CALENDAR_YEAR_BG,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_YEAR_BG.png",IMG_CMP_PNG_SABM_RLE|SABM_HOR_STRETCH|SABM_VER_STRETCH,0)
RES_ADD_IMG(IMAGE_CALENDAR_RIGHT_OFF,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_RIGHT_OFF.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CALENDAR_RIGHT_ON,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_RIGHT_ON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CALENDAR_LEFT_OFF,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_LEFT_OFF.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CALENDAR_LEFT_ON,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_LEFT_ON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CALENDAR_COMMENT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_COMMENT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CALENDAR_CUR_FOCUS,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_CUR_FOCUS.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CALENDAR_CUR_DATE,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_CUR_DATE.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CALENDAR_CUR_DATE_COMMENT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_CUR_DATE_COMMENT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CALENDAR_CUR_FOCUS_COMMENT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CALENDAR_CUR_FOCUS_COMMENT.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMG_CALENDAR_SCH_VIEW_TAB_ICON_OFF,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_SCH_VIEW_TAB_ICON_OFF.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CALENDAR_SCH_VIEW_TAB_ICON_ON,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_SCH_VIEW_TAB_ICON_ON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CALENDAR_WEEKLY_VIEW_TAB_ICON_OFF,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_WEEKLY_VIEW_TAB_ICON_OFF.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CALENDAR_WEEKLY_VIEW_TAB_ICON_ON,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_WEEKLY_VIEW_TAB_ICON_ON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CALENDAR_MONTHLY_VIEW_TAB_ICON_OFF,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_MONTHLY_VIEW_TAB_ICON_OFF.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CALENDAR_MONTHLY_VIEW_TAB_ICON_ON,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_MONTHLY_VIEW_TAB_ICON_ON.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(ISTYLE_IMG_CALENDAR_SCH_VIEW_TAB_ICON_OFF,"\\MMI_RES_##\\IMAG\\Calendar\\istyle\\ISTYLE_IMG_CALENDAR_SCH_VIEW_TAB_ICON_OFF.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(ISTYLE_IMG_CALENDAR_SCH_VIEW_TAB_ICON_ON,"\\MMI_RES_##\\IMAG\\Calendar\\istyle\\ISTYLE_IMG_CALENDAR_SCH_VIEW_TAB_ICON_ON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(ISTYLE_IMG_CALENDAR_WEEKLY_VIEW_TAB_ICON_OFF,"\\MMI_RES_##\\IMAG\\Calendar\\istyle\\ISTYLE_IMG_CALENDAR_WEEKLY_VIEW_TAB_ICON_OFF.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(ISTYLE_IMG_CALENDAR_WEEKLY_VIEW_TAB_ICON_ON,"\\MMI_RES_##\\IMAG\\Calendar\\istyle\\ISTYLE_IMG_CALENDAR_WEEKLY_VIEW_TAB_ICON_ON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(ISTYLE_IMG_CALENDAR_MONTHLY_VIEW_TAB_ICON_OFF,"\\MMI_RES_##\\IMAG\\Calendar\\istyle\\ISTYLE_IMG_CALENDAR_MONTHLY_VIEW_TAB_ICON_OFF.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(ISTYLE_IMG_CALENDAR_MONTHLY_VIEW_TAB_ICON_ON,"\\MMI_RES_##\\IMAG\\Calendar\\istyle\\ISTYLE_IMG_CALENDAR_MONTHLY_VIEW_TAB_ICON_ON.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

RES_ADD_IMG(IMG_CALENDAR_MENU_CLEAN_TODAY_ICON,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_MENU_CLEAN_TODAY_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CALENDAR_MENU_ADD_ICON,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_MENU_ADD_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CALENDAR_MENU_CLEAN_GOTO_DATE_ICON,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_MENU_CLEAN_GOTO_DATE_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CALENDAR_MENU_VIEW_ICON,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_MENU_VIEW_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CALENDAR_MENU_LURA_DETAIL_ICON,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_MENU_LURA_DETAIL_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifdef MMI_VCALENDAR_SUPPORT
RES_ADD_IMG(IMG_CALENDAR_MENU_EXPORT_ICON,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_MENU_EXPORT_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
//RES_ADD_IMG(IMG_CALENDAR_LURA_BG,"\\MMI_RES_##\\IMAG\\Calendar\\IMG_CALENDAR_LURA_BG.bmp",IMG_CMP_BMP_16_565_RLE,0)
#endif

RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_ALERT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_ALERT.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifdef CALENDAR_SUPPORT
//#ifndef MMI_GUI_STYLE_MINISCREEN
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_BG,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_BG.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
//#endif
//RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_BLUE_DEFAULT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_BLUE_DEFAULT.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_COMMENT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_COMMENT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_DEFAULT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_DEFAULT.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_FOCUSED,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_FOCUSED.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_GREEN_DEFAULT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_GREEN_DEFAULT.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
#ifdef MMI_CALENDAR_DAILY_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_ORANGE_DEFAULT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_ORANGE_DEFAULT.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
#endif
#ifndef MMI_CALENDAR_MINI_SUPPORT
#ifdef MMI_PDA_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_POINT_DEFAULT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_POINT_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_POINT_FOCUSED,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_POINT_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef TOUCH_PANEL_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_LEFT_DEFAULT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_LEFT_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_RIGHT_DEFAULT,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_RIGHT_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#endif
#if 0
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_LEFT_PRESSED,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_LEFT_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_CALENDAR_RIGHT_PRESSED,"\\MMI_RES_##\\IMAG\\Calendar\\IMAGE_CLOCK_CALENDAR_RIGHT_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#endif
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
RES_ADD_IMG(IMG_CLOCK_AM,"\\MMI_RES_##\\IMAG\\AlmClk\\IMG_CLOCK_AM.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMG_CLOCK_BG,"\\MMI_RES_##\\IMAG\\AlmClk\\IMG_CLOCK_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CLOCK_CENTER,"\\MMI_RES_##\\IMAG\\AlmClk\\IMG_CLOCK_CENTER.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CLOCK_DAY,"\\MMI_RES_##\\IMAG\\AlmClk\\IMG_CLOCK_DAY.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CLOCK_HOUR_HAND,"\\MMI_RES_##\\IMAG\\AlmClk\\IMG_CLOCK_HOUR_HAND.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CLOCK_MIN_HAND,"\\MMI_RES_##\\IMAG\\AlmClk\\IMG_CLOCK_MIN_HAND.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CLOCK_NIGHT,"\\MMI_RES_##\\IMAG\\AlmClk\\IMG_CLOCK_NIGHT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CLOCK_PM,"\\MMI_RES_##\\IMAG\\AlmClk\\IMG_CLOCK_PM.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_CLOCK_TIME_BG,"\\MMI_RES_##\\IMAG\\AlmClk\\IMG_CLOCK_TIME_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
RES_ADD_IMG(IMG_JUMP_CLK_BG,"\\MMI_RES_##\\IMAG\\AlmJump\\IMG_JUMP_CLK_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_JUMP_CLK_CENTER,"\\MMI_RES_##\\IMAG\\AlmJump\\IMG_JUMP_CLK_CENTER.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_JUMP_CLK_HOUR_HANDLE,"\\MMI_RES_##\\IMAG\\AlmJump\\IMG_JUMP_CLK_HOUR_HANDLE.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_JUMP_CLK_MIN_HANDLE,"\\MMI_RES_##\\IMAG\\AlmJump\\IMG_JUMP_CLK_MIN_HANDLE.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMG_JUMP_WIN_BG,"\\MMI_RES_##\\IMAG\\AlmJump\\IMG_JUMP_WIN_BG.bmp",IMG_CMP_BMP_16_565_RLE,0)
#endif

//RES_ADD_IMG(IMG_ST_BG, "\\MMI_RES_##\\IMAG\\stopwatch\\IMG_ST_BG.png",IMG_CMP_PNG_SABM_RLE|SABM_HOR_STRETCH,0)//|SABM_HOR_STRETCH|SABM_VER_STRETCH
#ifdef MMI_RECORD_SUPPORT
RES_ADD_IMG(IMG_SCH_RECORD_ICON,"\\MMI_RES_##\\IMAG\\ALARM\\IMG_SCH_RECORD_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef COUNTEDTIME_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_TIMER_START,"\\MMI_RES_##\\IMAG\\timer\\IMAGE_CLOCK_TIMER_START.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_TIMER_STOP,"\\MMI_RES_##\\IMAG\\timer\\IMAGE_CLOCK_TIMER_STOP.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_TIMER_START_PRESSED,"\\MMI_RES_##\\IMAG\\timer\\IMAGE_CLOCK_TIMER_START_PRESSED.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_TIMER_STOP_PRESSED,"\\MMI_RES_##\\IMAG\\timer\\IMAGE_CLOCK_TIMER_STOP_PRESSED.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
#endif
#ifndef MMI_COUNTEDTIME_MINI_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_ALARM_TIMER_DEFAULT,"\\MMI_RES_##\\IMAG\\timer\\IMAGE_CLOCK_ALARM_TIMER_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_ALARM_TIMER_FOCUSED,"\\MMI_RES_##\\IMAG\\timer\\IMAGE_CLOCK_ALARM_TIMER_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#if defined MMI_ISTYLE_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_TIMER_ISTYLETAB_DEFAULT,"\\MMI_RES_##\\IMAG\\timer\\IMAGE_CLOCK_TIMER_ISTYLETAB_DEFAULT.png",IMG_CMP_PNG_SABM_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_TIMER_ISTYLETAB_FOCUSED,"\\MMI_RES_##\\IMAG\\timer\\IMAGE_CLOCK_TIMER_ISTYLETAB_FOCUSED.png",IMG_CMP_PNG_SABM_RLE,0)
#endif
#endif
#endif

#ifdef STOPWATCH_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_STOPWATCH_RESET_DEFAULT,"\\MMI_RES_##\\IMAG\\stopwatch\\IMAGE_CLOCK_STOPWATCH_RESET_DEFAULT.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_STOPWATCH_RESET_PRESSED,"\\MMI_RES_##\\IMAG\\stopwatch\\IMAGE_CLOCK_STOPWATCH_RESET_PRESSED.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_STOPWATCH_START_PRESSED,"\\MMI_RES_##\\IMAG\\stopwatch\\IMAGE_CLOCK_STOPWATCH_START_PRESSED.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_STOPWATCH_START,"\\MMI_RES_##\\IMAG\\stopwatch\\IMAGE_CLOCK_STOPWATCH_START.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_STOPWATCH_STOP,"\\MMI_RES_##\\IMAG\\stopwatch\\IMAGE_CLOCK_STOPWATCH_STOP.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_STOPWATCH_STOP_PRESSED,"\\MMI_RES_##\\IMAG\\stopwatch\\IMAGE_CLOCK_STOPWATCH_STOP_PRESSED.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_CLOCK_ALARM_STOPWATCH_DEFAULT,"\\MMI_RES_##\\IMAG\\stopwatch\\IMAGE_CLOCK_ALARM_STOPWATCH_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_ALARM_STOPWATCH_FOCUSED,"\\MMI_RES_##\\IMAG\\stopwatch\\IMAGE_CLOCK_ALARM_STOPWATCH_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifdef MMI_ISTYLE_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_STOPWATCH_ISTYLETAB_DEFAULT,"\\MMI_RES_##\\IMAG\\stopwatch\\IMAGE_CLOCK_STOPWATCH_ISTYLETAB_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_STOPWATCH_ISTYLETAB_FOCUSED,"\\MMI_RES_##\\IMAG\\stopwatch\\IMAGE_CLOCK_STOPWATCH_ISTYLETAB_FOCUSED.png",IMG_CMP_PNG_SABM_RLE,0)
#endif

//================================================================================================================================//
//                                                        For watch                                                               //
//================================================================================================================================//
#ifndef ADULT_WATCH_SUPPORT
#ifdef PLATFORM_ANTISW3
RES_ADD_IMG(image_watch_stopwatch_reset,"\\MMI_RES_##\\IMAG\\stopwatch\\res_stopwatch_ic_resest.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(image_watch_stopwatch_play,"\\MMI_RES_##\\IMAG\\stopwatch\\res_stopwatch_ic_play.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(image_watch_stopwatch_pause,"\\MMI_RES_##\\IMAG\\stopwatch\\res_stopwatch_ic_stop.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
#else ifdef PLATFORM_UWS6121E
RES_ADD_IMG(image_watch_stopwatch_reset,"\\MMI_RES_##\\IMAG\\stopwatch\\res_stopwatch_ic_resest.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(image_watch_stopwatch_play,"\\MMI_RES_##\\IMAG\\stopwatch\\res_stopwatch_ic_play.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(image_watch_stopwatch_pause,"\\MMI_RES_##\\IMAG\\stopwatch\\res_stopwatch_ic_stop.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef SCREEN_SHAPE_CIRCULAR
RES_ADD_IMG(res_aw_stopwatch_bg,"\\MMI_RES_##\\IMAG\\stopwatch\\res_aw_stopwatch_bg.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif //SCREEN_SHAPE_CIRCULAR
#else
RES_ADD_IMG(image_watch_stopwatch_pause,"\\MMI_RES_##\\IMAG\\stopwatch\\res_aw_stopwatch_ic_pause.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(image_watch_stopwatch_play,"\\MMI_RES_##\\IMAG\\stopwatch\\res_aw_stopwatch_ic_play.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(image_watch_stopwatch_reset,"\\MMI_RES_##\\IMAG\\stopwatch\\res_aw_stopwatch_ic_resest.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(res_aw_stopwatch_bg,"\\MMI_RES_##\\IMAG\\stopwatch\\res_aw_stopwatch_bg.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
#endif

//================================================================================================================================//
//                                                        For watch                                                               //
//================================================================================================================================//

#endif

//[[define string infomation
#if 1
RES_ADD_STRING(TXT_ALARM_CLOCK1,"Alarm1")

RES_ADD_STRING(TXT_ALARM_CLOCK2,"Alarm2")

//RES_ADD_STRING(TXT_ALARM_ENABLE,"Activate")               //replaced by common ID TXT_COMMON_ENABLE_ON

//RES_ADD_STRING(TXT_ALARM_DISABLE,"Deactivate")            //replaced by common ID TXT_CLOSE          

RES_ADD_STRING(TXT_ALARM_CLOCK3,"Alarm3")
RES_ADD_STRING(TXT_ALARM_CLOCK4,"Alarm4")
RES_ADD_STRING(TXT_ALARM_CLOCK5,"Alarm5")
RES_ADD_STRING(TXT_ALARM_CLOCK6,"Alarm6")
RES_ADD_STRING(TXT_ALARM_CLOCK7,"Alarm7")
RES_ADD_STRING(TXT_ALARM_CLOCK8,"Alarm8")
RES_ADD_STRING(TXT_ALARM_CLOCK9,"Alarm9")
RES_ADD_STRING(TXT_ALARM_CLOCK10,"Alarm10")

//RES_ADD_STRING(TXT_ALARM_EXPIRED_MSG,"Smart event expired, please check")    //case3: defined but not used

//RES_ADD_STRING(TXT_ALARM_START_ALARM,"Set alarm")    //case3: defined but not used

//RES_ADD_STRING(TXT_ALARM_STOP_ALARM,"Stop alarm")    //case3: defined but not used

//RES_ADD_STRING(TXT_ALARM_MODE_CUSTOMED,"Customized")    //case3: defined but not used

RES_ADD_STRING(TXT_AUTOPOWER_ON,"Auto power on")

RES_ADD_STRING(TXT_AUTOPOWER_OFF,"Auto power off")

RES_ADD_STRING(TXT_CALENDAR_LOOK,"View schedule")

RES_ADD_STRING(TXT_CALENDAR_NEW,"New schedule")

RES_ADD_STRING(TXT_CALENDAR_ALL,"All schedule")

//RES_ADD_STRING(TXT_CALENDAR_QUERY,"Find date")    //case3: defined but not used

//RES_ADD_STRING(TXT_CALENDAR_MEMO,"Feast")    //case3: defined but not used

RES_ADD_STRING(TXT_CALENDAR_WEEKLY,"By week")

RES_ADD_STRING(TXT_CALENDAR_MONTHLY,"By month")

//RES_ADD_STRING(TXT_CALENDAR_SOLAR,"Solar")    //case3: defined but not used
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
RES_ADD_STRING(TXT_CALENDAR_LUNAR,"Lunar") 
#endif
//RES_ADD_STRING(TXT_CALENDAR_CAPRICORN,"Capricorn")

//RES_ADD_STRING(TXT_CALENDAR_AQUARIUS,"Aquarius")

//RES_ADD_STRING(TXT_CALENDAR_PISCES,"Pisces")

//RES_ADD_STRING(TXT_CALENDAR_ARIES,"Aries")

//RES_ADD_STRING(TXT_CALENDAR_TAURUS,"Taurus")

//RES_ADD_STRING(TXT_CALENDAR_GEMINI,"Gemini")

//RES_ADD_STRING(TXT_CALENDAR_CANCER,"Cancer")

//RES_ADD_STRING(TXT_CALENDAR_LEO,"Leo")

//RES_ADD_STRING(TXT_CALENDAR_VIRGO,"Virgo")

//RES_ADD_STRING(TXT_CALENDAR_LIBRA,"Libra")

//RES_ADD_STRING(TXT_CALENDAR_SCORPIO,"Scorpio")

//RES_ADD_STRING(TXT_CALENDAR_SAGITTARIUS,"Sagittarius")

//RES_ADD_STRING(TXT_CALENDAR_XH,"小寒")

//RES_ADD_STRING(TXT_CALENDAR_DH,"大寒")

//RES_ADD_STRING(TXT_CALENDAR_LC,"立春")

//RES_ADD_STRING(TXT_CALENDAR_YS,"雨水")

//RES_ADD_STRING(TXT_CALENDAR_JZ,"惊蛰")

//RES_ADD_STRING(TXT_CALENDAR_CF,"春分")

//RES_ADD_STRING(TXT_CALENDAR_QM,"清明")

//RES_ADD_STRING(TXT_CALENDAR_GY,"谷雨")

//RES_ADD_STRING(TXT_CALENDAR_LX,"立夏")

//RES_ADD_STRING(TXT_CALENDAR_XM,"小满")

//RES_ADD_STRING(TXT_CALENDAR_MZ,"芒种")

//RES_ADD_STRING(TXT_CALENDAR_XZ,"夏至")

//RES_ADD_STRING(TXT_CALENDAR_XS,"小暑")

//RES_ADD_STRING(TXT_CALENDAR_DS,"大暑")

//RES_ADD_STRING(TXT_CALENDAR_LQ,"立秋")

//RES_ADD_STRING(TXT_CALENDAR_CS,"处暑")

//RES_ADD_STRING(TXT_CALENDAR_BL,"白露")

//RES_ADD_STRING(TXT_CALENDAR_QF,"秋分")

//RES_ADD_STRING(TXT_CALENDAR_HL,"寒露")

//RES_ADD_STRING(TXT_CALENDAR_SJ,"霜降")

//RES_ADD_STRING(TXT_CALENDAR_LD,"立冬")

//RES_ADD_STRING(TXT_CALENDAR_XX,"小雪")

//RES_ADD_STRING(TXT_CALENDAR_DX,"大雪")

//RES_ADD_STRING(TXT_CALENDAR_DZ,"冬至")

//RES_ADD_STRING(TXT_SCH_TITLE_TIME,"Time:")    //case3: defined but not used

//RES_ADD_STRING(TXT_SCH_TITLE_RING,"Ring:")    //case3: defined but not used

//RES_ADD_STRING(TXT_SCH_TITLE_FREQ,"Period:")    //case3: defined but not used

//RES_ADD_STRING(TXT_SCH_TITLE_CONTENT,"Content:")    //case3: defined but not used

//RES_ADD_STRING(TXT_SCH_SCH,"Schedule")         //replaced by common ID TXT_IDLE_CALENDAR
#ifdef CMCC_UI_STYLE
RES_ADD_STRING(TXT_SCH_AWAKE_TIME,"Awake time") 
RES_ADD_STRING(TXT_RING_DUR_TIME,"Ring duration")
RES_ADD_STRING(TXT_SCH_REMIND_ONTIME,"On time")
RES_ADD_STRING(TXT_SCH_REMIND_FIVE_MIN,"Five minutes")
RES_ADD_STRING(TXT_SCH_REMIND_FIFTEEN_MIN,"Fifteen minutes")
RES_ADD_STRING(TXT_SCH_REMIND_ONE_HOUR,"One hour")
RES_ADD_STRING(TXT_SCH_REMIND_ONE_DAY,"One day")
RES_ADD_STRING(TXT_SCH_REMIND_ONE_WEEK,"One week")
#endif
//RES_ADD_STRING(TXT_SCH_AWAKE_FREQ,"Awake period")    //case3: defined but not used

//RES_ADD_STRING(TXT_SCH_INPUT_DATE,"Input date")    //case3: defined but not used

//RES_ADD_STRING(TXT_SCH_INPUT_TIME,"Input time")    //case3: defined but not used

//RES_ADD_STRING(TXT_SCH_AWAKE_ONCE,"Once")  //replaced by common ID TXT_ALARM_MODE_ONCE

RES_ADD_STRING(TXT_SCH_AWAKE_EVERYDAY,"Everyday")

RES_ADD_STRING(TXT_SCH_AWAKE_EVERYWEEK,"Every week")

RES_ADD_STRING(TXT_SCH_AWAKE_EVERYMONTH,"Every month")

RES_ADD_STRING(TXT_SCH_AWAKE_EVERYYEAR,"Every year")

RES_ADD_STRING(TXT_SCH_AWAKE_NO,"No awake")

//RES_ADD_STRING(TXT_SCH_CONTENT,"Content")    //case3: defined but not used

//RES_ADD_STRING(TXT_SCH_ICON,"Icon")    //case3: defined but not used

//RES_ADD_STRING(TXT_SCH_NEW,"New")    //replaced by common ID TXT_NEW

//RES_ADD_STRING(TXT_SCH_ALLSCH,"All schedule")    //replaced by local common ID TXT_CALENDAR_ALL

//RES_ADD_STRING(TXT_FEAST_FEASTNAME,"Feast name")    //case3: defined but not used

//RES_ADD_STRING(TXT_ALARM_NO_RECORD,"No schedule")    //case3: defined but not used

//RES_ADD_STRING(TXT_ALARM_MORE_RING,"More rings")                 //replaced by common ID TXT_COMMON_MORE_RING

//RES_ADD_STRING(TXT_ALARM_NO_MATCH_RECORD,"No match record")    //case3: defined but not used

//RES_ADD_STRING(TXT_ALARM_MEMORY_FULL,"Memory full")    //case3: defined but not used

//RES_ADD_STRING(TXT_ALARM_NAME_EMPTY,"Name is empty")    //case3: defined but not used

//RES_ADD_STRING(TXT_ALARM_EDIT,"Edit")  //replaced by common ID TXT_EDIT

//RES_ADD_STRING(TXT_ALARM_EDIT_HEADING,"Heading")    //case3: defined but not used

//RES_ADD_STRING(TXT_ALARM_EDIT_RING,"Ring")  //replaced by common ID TXT_RING

RES_ADD_STRING(TXT_ALARM_EDIT_MODE,"Mode")

//RES_ADD_STRING(TXT_ALARM_INHEAR_RING_TYPE,"Inhear ring")

//RES_ADD_STRING(TXT_ALARM_SELFHOOD_RING_TYPE,"Selfhood ring")

//RES_ADD_STRING(TXT_ALARM_MP3_RING_TYPE,"MP3 ring")

RES_ADD_STRING(TXT_ALARM_RECORD_RING_TYPE,"Record ring")

//RES_ADD_STRING(TXT_ALARM_MUST_SEL_ONE_DAY,"Select one day")  //replaced by common ID TXT_ALARM_SELECT_ONE_DAY

//RES_ADD_STRING(TXT_ALARM_EDIT_TIME,"Time")  //replaced by common ID TXT_TIME

RES_ADD_STRING(TXT_ALARM_DELAY,"Delay")

#ifdef MMI_ALARM_MINI_SUPPORT
RES_ADD_STRING(TXT_ALARM_DELAY_MINI,"Delay")
#endif


RES_ADD_STRING(TXT_SCH_INVALID_DEADLINE,"Invalid deadline")

RES_ADD_STRING(TXT_ALARM_WILL_TO,"Will shut down")

RES_ADD_STRING(TXT_ALARM_SHUT_DOWN,"s later")

//RES_ADD_STRING(TXT_SCH_INVALID_DATE_TIME,"Invalid date and time")    //case3: defined but not used

RES_ADD_STRING(TXT_ALARM_DELAY_1,"Delay alarm1")

RES_ADD_STRING(TXT_ALARM_DELAY_2,"Delay alarm2")

RES_ADD_STRING(TXT_ALARM_DELAY_3,"Delay alarm3")
RES_ADD_STRING(TXT_ALARM_DELAY_4,"Delay alarm4")
RES_ADD_STRING(TXT_ALARM_DELAY_5,"Delay alarm5")
RES_ADD_STRING(TXT_ALARM_DELAY_6,"Delay alarm6")
RES_ADD_STRING(TXT_ALARM_DELAY_7,"Delay alarm7")
RES_ADD_STRING(TXT_ALARM_DELAY_8,"Delay alarm8")
RES_ADD_STRING(TXT_ALARM_DELAY_9,"Delay alarm9")
RES_ADD_STRING(TXT_ALARM_DELAY_10,"Delay alarm10")

//RES_ADD_STRING(TXT_ALARM_CLOSE,"Close")           //replaced by common ID TXT_CLOSE

RES_ADD_STRING(TXT_SCH_INVALID_TITLE,"Invalid title")

//RES_ADD_STRING(TXT_ALARM_EDIT_NAME,"Name")    //replaced by common ID STXT_MAIN_MESSAGE 

RES_ADD_STRING(TXT_CALENDAR_DEL_CDR_DAY_PERIODSCH,"Current day has period schedule,also delete")

//RES_ADD_STRING(TXT_SCH_TITLE_NOT_NULL,"Title can't be null")  //replaced by common ID TXT_COMM_NO_TITLE

RES_ADD_STRING(TXT_CALENDAR_UNREAD_SCH,"Missed events")

//calendar
RES_ADD_STRING(TXT_CALENDAR_CLEAR_TODAY,"Clear today calendar")
RES_ADD_STRING(TXT_CALENDAR_CLEAR_ALL,"Clear all")
RES_ADD_STRING(TXT_CALENDAR_EMPTY,"No record")
RES_ADD_STRING(TXT_CALENDAR_NAME,"Name:")
//RES_ADD_STRING(TXT_CALENDAR_DATE,"Date:")  //replaced by common ID TXT_COMMON_DATE
//RES_ADD_STRING(TXT_CALENDAR_TIME,"Time:")  //replaced by common ID TXT_COMMON_TIME
RES_ADD_STRING(TXT_CALENDAR_RING,"Ring:")
//RES_ADD_STRING(TXT_CALENDAR_FREQ,"Freq:")  //replaced by common ID TXT_TYPE
RES_ADD_STRING(TXT_CALENDAR_DEADLINE,"Deadline:")
RES_ADD_STRING(TXT_CALENDAR_TO_DATE,"Go to date")
//RES_ADD_STRING(TXT_CALENDAR_QUERY_SOLAR,"Solar:")  //replaced by common ID TXT_COMMON_DATE
//RES_ADD_STRING(TXT_CALENDAR_QUERY_LUNAR,"Lunar:")
//RES_ADD_STRING(TXT_CALENDAR_WEEK_NO,"NO.")
RES_ADD_STRING(TXT_CALENDAR_WEEK,"Week")    
//RES_ADD_STRING(TXT_CALENDAR_EDIT_NAME,"Name")  //replaced by common ID TXT_COMMON_SUBJECT
//RES_ADD_STRING(TXT_CALENDAR_EDIT_DATE,"Date")  //replaced by common ID TXT_DATE
//RES_ADD_STRING(TXT_CALENDAR_EDIT_TIME,"Time")  //replaced by common ID TXT_TIME
//RES_ADD_STRING(TXT_CALENDAR_EDIT_RING,"Ring")  //replaced by common ID TXT_RING
//RES_ADD_STRING(TXT_CALENDAR_EDIT_FREQ,"Freq")  //replaced by common ID TXT_COMMON_TYPE
RES_ADD_STRING(TXT_CALENDAR_EDIT_DEADLINE,"Deadline")
RES_ADD_STRING(TXT_CALENDAR_LUNAR_DETAIL, "Lunar detail")

#ifdef  TOUCH_PANEL_SUPPORT
RES_ADD_STRING(TXT_CALENDAR_NEW_SCHEDULE, "Touch to create a new schedule")
#else
RES_ADD_STRING(TXT_CALENDAR_NEW_SCHEDULE, "Create a new schedule")
#endif
#ifdef MMI_CALENDAR_DAILY_SUPPORT
RES_ADD_STRING(TXT_CALENDAR_DAILY, "Daily")
#endif

//end of calendar

#ifdef MMI_VCALENDAR_SUPPORT
RES_ADD_STRING(TXT_SCH_DESCRIPTION,"Description:")
RES_ADD_STRING(TXT_SCH_CLASSIFICATION,"Classification:")
RES_ADD_STRING(TXT_SCH_START_DATE,"Start date:")
RES_ADD_STRING(TXT_SCH_START_TIME,"Start time:")
RES_ADD_STRING(TXT_CALENDAR_DEADLINE_TIME,"Mature time:")
RES_ADD_STRING(TXT_CALENDAR_DUE_DATE,"Due date:")
RES_ADD_STRING(TXT_CALENDAR_DUE_TIME,"Due time:")
RES_ADD_STRING(TXT_CALENDAR_COMPLETED_DATE,"Completed date:")
RES_ADD_STRING(TXT_CALENDAR_COMPLETED_TIME,"Completed time:")
RES_ADD_STRING(TXT_SAVE_CALENDAR,"Save calendar")
#endif
//RES_ADD_STRING(TXT_ALARM_FM_TAB,"Station list")  //replaced by common ID TXT_COMM_CHANNEL_LIST
#ifndef MMI_GUI_STYLE_MINISCREEN
RES_ADD_STRING(TXT_ALARM_CUSTOM_FM,"Custom frequency(87.5-108.0)")
#else
RES_ADD_STRING(TXT_ALARM_CUSTOM_FM_TEXT,"Custom frequency:")
RES_ADD_STRING(TXT_ALARM_CUSTOM_FM_FREQ,"87.5-108.0")
#endif
RES_ADD_STRING(TXT_ALARM_FM_RING,"FM broadcast ring")
#endif
//]]define string infomation


#ifdef WORLD_CLOCK_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_DST,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_DST.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifndef MMI_WORLDCLOCK_MINI_SUPPORT
RES_ADD_IMG(IMAGE_WT_OPTION_SET_LOCAL,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_WT_OPTION_SET_LOCAL.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_WT_OPTION_SET_OTHER,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_WT_OPTION_SET_OTHER.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_WT_OPTION_SET_SUMMER,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_WT_OPTION_SET_SUMMER.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_WT_OPTION_CANCEL_SUMMER,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_WT_OPTION_CANCEL_SUMMER.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef ANALOG_CLK_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_DAYTIME_BG,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_DAYTIME_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_DAYTIME_HOUR,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_DAYTIME_HOUR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_DAYTIME_MINUTE,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_DAYTIME_MINUTE.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_DAYTIME_CENTER,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_DAYTIME_CENTER.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_NIGHT_BG,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_NIGHT_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_NIGHT_HOUR,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_NIGHT_HOUR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_NIGHT_MINUTE,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_NIGHT_MINUTE.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_NIGHT_CENTER,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_NIGHT_CENTER.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifdef TOUCH_PANEL_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_ADD_ICON,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_ADD_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifndef MMI_WORLDCLOCK_MINI_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_TAB_DEFAULT,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_TAB_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_TAB_FOCUSED,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_TAB_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#if defined MMI_ISTYLE_SUPPORT
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_ISTYLETAB_DEFAULT,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_ISTYLETAB_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_CLOCK_WORLDCLOCK_ISTYLETAB_FOCUSED,"\\MMI_RES_##\\IMAG\\worldclock\\IMAGE_CLOCK_WORLDCLOCK_ISTYLETAB_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#endif
#endif

//world clock
#ifdef WORLD_CLOCK_SUPPORT
//attention please,下面的字串不能随意删除和改变顺序!!!
RES_ADD_STRING(TXT_ACC_WORLD_GMT_0,"GMT0")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P,"GMT+1")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_P,"GMT+2")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_3_P,"GMT+3")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_3P5_P,"GMT+3.5")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_4_P,"GMT+4")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_4P5_P,"GMT+4.5")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5_P,"GMT+5")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5P5_P,"GMT+5.5")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5P75_P,"GMT+5.75")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_6_P,"GMT+6")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_6P5_P,"GMT+6.5")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_7_P,"GMT+7")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_P,"GMT+8")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_9_P,"GMT+9")
//RES_ADD_STRING(TXT_ACC_WORLD_GMT_9P5_P,"GMT+9.5")    //case3: defined but not used
RES_ADD_STRING(TXT_ACC_WORLD_GMT_10_P,"GMT+10")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_10P5_P,"GMT+10.5")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_11_P,"GMT+11")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_11P5_P,"GMT+11.5")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_12_P,"GMT+12")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_12P75_P,"GMT+12.75")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_12_N,"GMT-12")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_11_N,"GMT-11")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_10_N,"GMT-10")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_9P5_N,"GMT-9.5")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_9_N,"GMT-9")
//RES_ADD_STRING(TXT_ACC_WORLD_GMT_8P5_N,"GMT-8.5")    //case3: defined but not used
RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_N,"GMT-8")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_7_N,"GMT-7")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_6_N,"GMT-6")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5_N,"GMT-5")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_4_N,"GMT-4")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_3P5_N,"GMT-3.5")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_3_N,"GMT-3")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_N,"GMT-2")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_N,"GMT-1")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_0_LISBON,"Lisbon")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_0_CASABLANCA,"Casablanca")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_0_RABAT,"Rabat")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_0_LONDON,"London")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_MADRID,"Madrid")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_BRUSSELS,"Brussels")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_PARIS,"Paris")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_ZURICH,"Zurich")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_GENEVA,"Geneva")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_COPENHAGEN,"Copenhagen")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_BERLIN,"Berlin")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_ROME,"Rome")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_STOCKHOLM,"Stockholm")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_AMSTERDAM,"Amsterdam")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_LUXEMBOURG,"Luxembourg")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_PRAGUE,"Prague")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_WARSAW,"Warsaw")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_VIENNA,"Vienna")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_P_BUDAPEST,"Budapest")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_P_CAPE_TOWN,"Cape Town")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_P_JOHANNESBURG,"Johannesburg")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_P_SOFIA,"Sofia")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_P_BUCHAREST,"Bucharest")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_P_ISTANBUL,"Istanbul")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_P_KIEV,"Kiev")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_P_CAIRO,"Cairo")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_P_ATHENS,"Athens")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_3_P_MOSCOW,"Moscow")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_3P5_P_TEHRAN,"Tehran")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_4_P_ABUDHABI,"Abudhabi")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_4P5_P_KABUL,"Kabul")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5_P_ISLAMABAD,"Islamabad")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5P5_P_NEWDELHI,"New Delhi")

RES_ADD_STRING(TXT_ACC_WORLD_GMT_5P5_P_MUMBAI,"Mumbai")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5P5_P_BANGALORE,"Bangalore")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5P5_P_KOLKATA,"Kolkata")

RES_ADD_STRING(TXT_ACC_WORLD_GMT_5P75_P_KATHMANDU,"Kathmandu")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_6_P_DHAKA,"Dhaka")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_6P5_P_YANGON,"Yangon")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_7_P_BANGKOK,"Bangkok")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_7_P_JAKARTA,"Jakarta")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_7_P_HANOI,"Hanoi")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_P_KUALA_LUMPUR,"Kuala Lumpur")
//RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_P_SINGAPORE,"Singapore")    //replaced by common ID TXT_SINGAPORE
RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_P_BEIJING,"Beijing")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_P_HONGKONG,"Hongkong")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_P_TAIPEI,"Taipei")

RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_P_MANILA,"Manila")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_9_P_TOKYO,"Tokyo")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_9P5_P_MELBOURNE,"Melbourne")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_10_P_VLADIVOSTOK,"Vladivostok")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_10_P_GUAM,"Guam")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_10_P_CANBERRA,"Canberra")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_10_P_SYDNEY,"Sydney")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_10P5_P_BRISBANE,"Brisbane")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_11_P_SOLOMON,"Solomon")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_11P5_P_NORFOLD_ISLAND,"Norfold Island")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_12_P_AUCKLAND,"Auckland")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_12_P_WELLINGTON,"Wellington")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_12P75_P_TONGA,"Tonga")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_12_N_KWAJALEIN,"Kwajalein")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_11_N_APIA,"Apia")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_10_N_HONOLULU,"Honolulu")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_9P5_N_POLYNESIA,"Polynesia")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_9_N_ANCHORAGE,"Anchorage")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_8P5_N_EASTER_ISLAND,"Easter Island")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_N_VANCOUVER,"Vancouver")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_N_SAN_FRANCISCO,"San Francisco")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_8_N_LOSANGELES,"Losangeles")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_7_N_PHOENIX,"Phoenix")
//RES_ADD_STRING(TXT_ACC_WORLD_GMT_6_N_MEXICO,"Mexico")     //replaced by common ID TXT_COMMON_WORLD_MEXICO
RES_ADD_STRING(TXT_ACC_WORLD_GMT_6_N_CHICAGO,"Chicago")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5_N_BOGOTA,"Bogota")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5_N_MONTREAL,"Montreal")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_5_N_NEWYORK,"Newyork")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_4_N_SANTIAGO,"Santiago")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_3P5_N_LAPAZ,"Lapaz")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_3_N_BUENOS_AIRES,"Buenos Aires")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_3_N_BRASILIA,"Brasilia")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_2_N_CENTRAL_ATLANTIC,"Central Atlantic")
RES_ADD_STRING(TXT_ACC_WORLD_GMT_1_N_PRAIA,"Praia")
//RES_ADD_STRING(TXT_LOCAL,"Local")
//RES_ADD_STRING(TXT_ACC_WORLD_SELECT_GMT,"Select GMT")    //case3: defined but not used
//RES_ADD_STRING(TXT_ACC_WORLD_SELECT_CITY,"Select City")    //case3: defined but not used

RES_ADD_STRING(TXT_ACC_WORLD_CITY_LIST_ORDER,"City List Order")    //case3: defined but not used
RES_ADD_STRING(TXT_ACC_WORLD_CITY_LIST_ORDER_BY_GMT,"Order By GMT")    //case3: defined but not used
RES_ADD_STRING(TXT_ACC_WORLD_CITY_LIST_ORDER_BY_ALPHABET,"Order By Alphabet")    //case3: defined but not used

// end of world clock
#endif


//schedule
#ifdef MMI_PDA_SUPPORT
RES_ADD_STRING(TXT_SCH_PREVIOUS_DAY, "Previous day")
RES_ADD_STRING(TXT_SCH_NEXT_DAY, "Next day")
#endif

//yaye.jiang stopwatch
#if defined ( STOPWATCH_SUPPORT ) 
RES_ADD_STRING(TXT_ACC_STOPWATCH,"Stopwatch")
RES_ADD_STRING(TXT_ACC_COUNT,"Count")
#endif
//RES_ADD_STRING(TXT_RESET, "Reset")

#ifdef SCH_ASSISTANT_WIDGET
RES_ADD_STRING(TXT_SCH_ADD_SCH_ASSIITANT,"The schedule assistant will be added to desktop")
#endif

#ifdef PDA_UI_DROPDOWN_WIN
RES_ADD_STRING(TXT_UNREAD_ALARM,"Unsettled alarm")
RES_ADD_STRING(TXT_UNREAD_ALARMS,"Unsettled alarms")
#endif

RES_ADD_STRING(TXT_SCH_TITLE_CONTENT,"Content:")
#define TXT_ALM_CONFLIT STR_ID_OPERA_V11_24//CR177677
RES_ADD_STRING(TXT_ALM_CONFLIT,"Settings conflicted")

RES_ADD_IMG(res_aw_alarm_ic_close,"\\MMI_RES_##\\IMAG\\Alarm\\res_aw_alarm_ic_close.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifdef SCREEN_SHAPE_CIRCULAR
RES_ADD_IMG(res_aw_alarm_ic_add,"\\MMI_RES_##\\IMAG\\Alarm\\res_aw_alarm_ic_add.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(res_aw_alarm_ic_alarm,"\\MMI_RES_##\\IMAG\\Alarm\\res_aw_alarm_ic_alarm.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifdef ADULT_WATCH_SUPPORT
RES_ADD_IMG(res_aw_alarm_ic_back,"\\MMI_RES_##\\IMAG\\Alarm\\res_aw_alarm_ic_back.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(res_aw_alarm_ic_none,"\\MMI_RES_##\\IMAG\\Alarm\\res_aw_alarm_ic_none.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
RES_ADD_IMG(res_aw_alarm_ic_done,"\\MMI_RES_##\\IMAG\\Alarm\\res_aw_alarm_ic_done.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(res_aw_alarm_ic_next,"\\MMI_RES_##\\IMAG\\Alarm\\res_aw_alarm_ic_next.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(res_aw_alarm_ic_snooze,"\\MMI_RES_##\\IMAG\\Alarm\\res_aw_alarm_ic_snooze.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_STRING(TXT_ALARM_DELAY_NOTE,"Ring again in 5 minutes")
#endif

