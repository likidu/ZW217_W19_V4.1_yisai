/*==============================================================================
File Name: mmiim_sp_stroke.c
Author: Lianxiang.Zhou
Date: 2010/05/18
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/18   Lianxiang.Zhou  Create.
==============================================================================*/

#ifdef WIN32
#include "std_header.h"
#endif

#include "mmiim_sp_stroke.h"

#if defined(MMIIM_USE_SPIM_KEY)

#include "mmk_msg.h"
#include "mmi_custom_define.h"
#include "mmidisplay_data.h"
#include "guifont.h"

#include "mmiim_internal.h"
#include "mmiim_sp_symbol.h"
#include "spim_ch_asso.h"
#include "mmiim_im_switching.h"


/*==============================================================================
Function Name: Initialize
Description: Initialize
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr);

/*==============================================================================
Function Name: Terminate
Description: Terminate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN Terminate(void *handle);

/*==============================================================================
Function Name: HandleMsg
Description: HandleMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMsg(void *handle, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U *event_data_ptr);

/*==============================================================================
Function Name: HandleSysMsgNoInput
Description: HandleSysMsgNoInput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgNoInput(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/*==============================================================================
Function Name: HandleSysMsgInputing
Description: HandleSysMsgInputing
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgInputing(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/*==============================================================================
Function Name: HandleSysMsgSelecting
Description: HandleSysMsgSelecting
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgSelecting(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/*==============================================================================
Function Name: HandleSysMsgAssociate
Description: HandleSysMsgAssociate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgAssociate(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/*==============================================================================
Function Name: TransMsg
Description: TransMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void TransMsg(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_SP_MSG_T *msg_ptr
);

/*==============================================================================
Function Name: TpDownNormal
Description: TpDownNormal
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpDownNormal(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
);

/*==============================================================================
Function Name: TpMoveNormal
Description: TpMoveNormal
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpMoveNormal(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
);

/*==============================================================================
Function Name: TpUpInputing
Description: TpUpInputing
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpUpInputing(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
);

/*==============================================================================
Function Name: TpUpAssociate
Description: TpUpAssociate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpUpAssociate(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
);

/*==============================================================================
Function Name: UpdateBar
Description: UpdateBar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void UpdateBar(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    SPIM_CONTENT_T *content_ptr
);

/*==============================================================================
Function Name: SetBarAsso
Description: SetBarAsso
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetBarAsso(MMIIM_SP_STROKE_DATA_T *data_ptr, wchar chr);

/*==============================================================================
Function Name: DrawUI
Description: DrawUI
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void DrawUI(MMIIM_SP_STROKE_DATA_T *data_ptr);

/*==============================================================================
Function Name: ReDrawUI
Description: ReDrawUI
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void ReDrawUI(MMIIM_SP_STROKE_DATA_T *data_ptr);

/*==============================================================================
Function Name: CommitChar
Description: CommitChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitChar(MMIIM_SP_STROKE_DATA_T *data_ptr, wchar c);

/*==============================================================================
Function Name: GetIndexChar
Description: GetIndexChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL wchar GetIndexChar(MMIIM_SP_BAR_DATA_T *bar_data_ptr, uint hot_idx);

/*==============================================================================
Function Name: ChangeRect
Description: ChangeRect
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN ChangeRect(MMIIM_SP_STROKE_DATA_T *data_ptr);

/*==============================================================================
Function Name:
Description:
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetToNoInput(MMIIM_SP_STROKE_DATA_T *data_ptr);

/*==============================================================================
Function Name: SetToInputing
Description: SetToInputing
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetToInputing(MMIIM_SP_STROKE_DATA_T *data_ptr);

/*==============================================================================
Function Name: SetToSelecting
Description: SetToSelecting
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetToSelecting(MMIIM_SP_STROKE_DATA_T *data_ptr);

/*==============================================================================
Function Name: SetToAssociate
Description: SetToAssociate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetToAssociate(MMIIM_SP_STROKE_DATA_T *data_ptr, wchar ch);


GUIIM_KERNEL_APIS_T const g_guiim_sp_stroke_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
};


/*==============================================================================
Function Name: Initialize
Description: Initialize
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr)
{
    MMIIM_SP_STROKE_DATA_T *data_ptr = PNULL;
    SPIM_INIT_PARAMETER_T init_params = {0};
    SPIM_HANDLE_T * core_handle = PNULL;
    GUI_RECT_T rect_null = {0};
    GUI_RECT_T win_rect = {0};
    uint8 width_char = GUI_GetFontWidth(MMIIM_SP_TEXT_BAR_FONT_TEXT, UNICODE_HANZI);
    uint8 width_label = GUI_GetFontWidth(MMIIM_SP_TEXT_BAR_FONT_LABEL, UNICODE_ASCII);
    uint8 width_spc = width_label + MMIIM_SP_TEXT_BAR_SPACE;

    //SCI_ASSERT(PNULL != param_ptr); /*assert verified*/
    if(PNULL == param_ptr)
    {
        return  PNULL;
    }
    
    //kernel
    init_params.width_win = MMIIM_SP_TEXT_BAR_CAND_WIDTH;
    init_params.width_chr = width_char;
    init_params.width_spc = width_spc;

    core_handle = SPIM_CH_STROKE_Init(&init_params);

    if (PNULL == core_handle)
    {
        return PNULL;
    }

    data_ptr = (MMIIM_SP_STROKE_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_SP_STROKE_DATA_T));
    SCI_MEMSET(data_ptr, 0, sizeof(MMIIM_SP_STROKE_DATA_T));

    data_ptr->init_param = *param_ptr;
    data_ptr->core_handle = core_handle;

    //rect
    data_ptr->rect = rect_null;
    data_ptr->state = MMIIM_SP_STATE_NOINPUT;

    data_ptr->asso.per_page = MMIIM_SP_TEXT_BAR_CAND_WIDTH / (width_char + width_spc);

	win_rect = MMITHEME_GetClientRectEx(data_ptr->init_param.win_handle);

    //bar
    MMIIM_SP_TextBarInit(&(data_ptr->bar_cand));
    data_ptr->bar_cand.win_id = data_ptr->init_param.win_id;
    data_ptr->bar_cand.lcd_info_ptr = data_ptr->init_param.lcd_info_ptr;
    data_ptr->bar_cand.rect = win_rect;
    data_ptr->bar_cand.rect.top = data_ptr->bar_cand.rect.bottom - MMIIM_SP_TEXT_BAR_HEIGHT + 1;

    data_ptr->bar_cand.width_label = width_label;
    data_ptr->bar_cand.width_space = MMIIM_SP_TEXT_BAR_SPACE;
    data_ptr->bar_cand.has_hot = 1;

    MMIIM_SP_TextBarInit(&(data_ptr->bar_edit));
    data_ptr->bar_edit.win_id = data_ptr->init_param.win_id;
    data_ptr->bar_edit.lcd_info_ptr = data_ptr->init_param.lcd_info_ptr;
    data_ptr->bar_edit.rect = win_rect;
    data_ptr->bar_edit.rect.bottom = data_ptr->bar_cand.rect.top - 1;
    data_ptr->bar_edit.rect.top = data_ptr->bar_edit.rect.bottom - MMIIM_SP_TEXT_BAR_HEIGHT + 1;


    return data_ptr;
}


/*==============================================================================
Function Name: Terminate
Description: Terminate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN Terminate(void *handle)
{
    MMIIM_SP_STROKE_DATA_T *data_ptr = (MMIIM_SP_STROKE_DATA_T*)handle;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return  FALSE;
    }
    
    SPIM_CH_STROKE_term(data_ptr->core_handle);

    SCI_FREE(data_ptr);

    return TRUE;
}


/*==============================================================================
Function Name: HandleMsg
Description: HandleMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMsg(void *handle, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U *event_data_ptr)
{
    MMIIM_SP_STROKE_DATA_T *data_ptr = (MMIIM_SP_STROKE_DATA_T*)handle;
    BOOLEAN is_dealt = FALSE;
    BOOLEAN result = FALSE;
	GUI_BOTH_RECT_T pop_menu_rect = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return  FALSE;
    }
    
    switch (event)
    {
    case GUIIM_EVENT_SYS_MSG:

        switch (data_ptr->state)
        {
        case MMIIM_SP_STATE_NOINPUT:
            is_dealt = HandleSysMsgNoInput(data_ptr, event_data_ptr);
            break;

        case MMIIM_SP_STATE_INPUTING:
            is_dealt = HandleSysMsgInputing(data_ptr, event_data_ptr);
            break;

        case MMIIM_SP_STATE_SELECTING:
            is_dealt = HandleSysMsgSelecting(data_ptr, event_data_ptr);
            break;

        case MMIIM_SP_STATE_ASSOCIATE:
            is_dealt = HandleSysMsgAssociate(data_ptr, event_data_ptr);
            break;

        default:
            break;
        }

        if (is_dealt)
        {
            ReDrawUI(data_ptr);
            result = TRUE;
        }
        else if (MSG_APP_STAR == event_data_ptr->sys_msg.msg_id)
        {
            MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_SP_SYMB_TYPE_FULL);
            result = TRUE;
        }
        else if (MSG_APP_HASH == event_data_ptr->sys_msg.msg_id)
        {
            MMIIM_SetPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
            MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_TOP_LEFT);
            //MMIIM_SP_SettingCreateWin(data_ptr->init_param.im_handle);
            result = TRUE;
        }

        break;

    case GUIIM_EVENT_SET_METHOD:
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_METHOD:
        event_data_ptr->get_method = GUIIM_M_STROKE;
        result = TRUE;
        break;

    case GUIIM_EVENT_PAINT:
        DrawUI(data_ptr);
        result = TRUE;
        break;

    case GUIIM_EVENT_SLEEP:
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_CTRL_RECT:
    case GUIIM_EVENT_GET_RECT:
        event_data_ptr->get_rect = data_ptr->rect;
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_EDIT_STATE:

        if (MMIIM_SP_STATE_INPUTING == data_ptr->state)
        {
            event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
        }
        else if (MMIIM_SP_STATE_SELECTING == data_ptr->state)
        {
            event_data_ptr->edit_state = GUIIM_EDIT_SELECTING;
        }
        else if (MMIIM_SP_STATE_ASSOCIATE == data_ptr->state)
        {
            event_data_ptr->edit_state = GUIIM_EDIT_CANDITATE;
        }
        else
        {
            event_data_ptr->edit_state = GUIIM_EDIT_NONE;
        }

        result = TRUE;
        break;

    default:
        break;
    }

    return result;
}


/*==============================================================================
Function Name: HandleSysMsgNoInput
Description: HandleSysMsgNoInput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgNoInput(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    SPIM_EVENT_PARA_T api_para = {0};
    SPIM_CONTENT_T api_content = {0};
    SPIM_RESULT_T api_result = SPIM_UNKNOW_ERROR;
    wchar cmt_chr = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == event_data_ptr)
    {
        return  FALSE;
    }
    
    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        return FALSE;
    }

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_KEY_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
            data_ptr->last_key = msg.para.key;
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    case MMIIM_SP_MSG_KEY_RELEASE:
        is_dealt = TRUE;

        if (MMIIM_SP_KEY_NONE == data_ptr->last_key)
        {
            break;
        }

        data_ptr->last_key = MMIIM_SP_KEY_NONE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
            CommitChar(data_ptr, ' ');
            break;

        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
            api_para.append = '1' + msg.para.key - MMIIM_SP_KEY_1;
            api_result = SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                               SPIM_EVENT_CHAR_APPEND,
                                               &api_para, &api_content);

            // 有输出
            if (SPIM_NO_ERROR == api_result)
            {
                //SCI_ASSERT(api_content.has_data); /*assert verified*/
                if(0 == api_content.has_data)
                {
                    break;
                }
                SetToInputing(data_ptr);

                UpdateBar(data_ptr, &api_content);

                data_ptr->is_rect_change = TRUE;
            }

            break;

        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    case MMIIM_SP_MSG_KEY_LONG_PRESS:
        is_dealt = TRUE;

        if (MMIIM_SP_KEY_NONE == data_ptr->last_key)
        {
            break;
        }

        data_ptr->last_key = MMIIM_SP_KEY_NONE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
            cmt_chr = '0' + msg.para.key - MMIIM_SP_KEY_0;
            CommitChar(data_ptr, cmt_chr);
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    default:
        break;
    }


    return is_dealt;
}


/*==============================================================================
Function Name: HandleSysMsgInputing
Description: HandleSysMsgInputing
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgInputing(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    SPIM_EVENT_PARA_T api_para = {0};
    SPIM_CONTENT_T api_content = {0};
    SPIM_RESULT_T api_result = SPIM_UNKNOW_ERROR;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == event_data_ptr)
    {
        return  FALSE;
    }
    
    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        return FALSE;
    }

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_TPDOWN:
        is_dealt = TpDownNormal(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_TPMOVE:
        is_dealt = TpMoveNormal(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_TPUP:
        is_dealt = TpUpInputing(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_KEY_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_STROKE_9KEY_1:
        case MMIIM_SP_STROKE_26KEY_1:
            api_para.append = '1';
            break;

        case MMIIM_SP_STROKE_9KEY_2:
        case MMIIM_SP_STROKE_26KEY_2:
            api_para.append = '2';
            break;

        case MMIIM_SP_STROKE_9KEY_3:
        case MMIIM_SP_STROKE_26KEY_3:
            api_para.append = '3';
            break;

        case MMIIM_SP_STROKE_9KEY_4:
        case MMIIM_SP_STROKE_26KEY_4:
            api_para.append = '4';
            break;

        case MMIIM_SP_STROKE_9KEY_5:
        case MMIIM_SP_STROKE_26KEY_5:
            api_para.append = '5';
            break;

        case MMIIM_SP_STROKE_9KEY_6:
        case MMIIM_SP_STROKE_26KEY_6:
            api_para.append = '6';
            break;

        case MMIIM_SP_KEY_LEFT:
        case MMIIM_SP_KEY_RIGHT:
        case MMIIM_SP_KEY_UP:
        case MMIIM_SP_KEY_DOWN:
        case MMIIM_SP_KEY_OK:
            SetToSelecting(data_ptr);
            break;

        case MMIIM_SP_KEY_CANCEL:
            api_result = SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                               SPIM_EVENT_CHAR_REMOVE,
                                               PNULL, &api_content);
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        //有笔划输入
        if (api_para.append != 0)
        {
            api_result = SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                               SPIM_EVENT_CHAR_APPEND,
                                               &api_para, &api_content);
        }

        break;

    case MMIIM_SP_MSG_KEY_LONG_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_CANCEL:
            (void)SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                        SPIM_EVENT_CHAR_REMOVE_ALL,
                                        PNULL, PNULL);
            SetToNoInput(data_ptr);
            data_ptr->is_rect_change = TRUE;
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    default:
        break;
    }

    if (SPIM_NO_ERROR == api_result)
    {
        if (api_content.has_data)
        {
            UpdateBar(data_ptr, &api_content);
        }
        else
        {
            SetToNoInput(data_ptr);
            data_ptr->is_rect_change = TRUE;
        }
    }


    return is_dealt;
}





/*==============================================================================
Function Name: HandleSysMsgSelecting
Description: HandleSysMsgInputing
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
    上 上一页 - api，page pre
    下 下一页 - api，page next
    左 焦点左移 - 内部操作
    右 焦点右移 - 内部操作
    确定 确定字词选择 - api，word select
    删除 进入 input 状态 - 内部操作
    123456789 以上，字词选择 - api，word select
    0 无效
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgSelecting(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    SPIM_EVENT_PARA_T api_para = {0};
    SPIM_CONTENT_T api_content = {0};
    SPIM_RESULT_T api_result = SPIM_UNKNOW_ERROR;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == event_data_ptr)
    {
        return  FALSE;
    }
    
    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        return FALSE;
    }

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_TPDOWN:
        is_dealt = TpDownNormal(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_TPMOVE:
        is_dealt = TpMoveNormal(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_TPUP:
        //同 inputing
        is_dealt = TpUpInputing(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_KEY_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
            break;

        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
            api_para.select = msg.para.key - MMIIM_SP_KEY_1;
            api_result = SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                               SPIM_EVENT_CAND_SELECT,
                                               &api_para, &api_content);
            break;

        case MMIIM_SP_KEY_UP:

            if (MMIIM_SP_IDC_ACTIVE == data_ptr->bar_cand.idc_left)
            {
                api_result = SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                                   SPIM_EVENT_PAGE_PREVIOUS,
                                                   PNULL, &api_content);
            }

            break;

        case MMIIM_SP_KEY_DOWN:

            if (MMIIM_SP_IDC_ACTIVE == data_ptr->bar_cand.idc_right)
            {
                api_result = SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                                   SPIM_EVENT_PAGE_NEXT,
                                                   PNULL, &api_content);
            }

            break;

        case MMIIM_SP_KEY_LEFT:

            if (data_ptr->bar_cand.hot_index > 0)
            {
                data_ptr->bar_cand.hot_index -= 1;
                data_ptr->is_bar_cand_dirty = TRUE;
            }

            break;


        case MMIIM_SP_KEY_RIGHT:

            if (data_ptr->bar_cand.hot_index + 1 < data_ptr->bar_cand.seg_cnt)
            {
                data_ptr->bar_cand.hot_index += 1;
                data_ptr->is_bar_cand_dirty = TRUE;
            }

            break;

        case MMIIM_SP_KEY_OK:
            api_para.select = data_ptr->bar_cand.hot_index;
            api_result = SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                               SPIM_EVENT_CAND_SELECT,
                                               &api_para, &api_content);
            break;

        case MMIIM_SP_KEY_CANCEL:
            SetToInputing(data_ptr);
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    case MMIIM_SP_MSG_KEY_LONG_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_CANCEL:
            (void)SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                        SPIM_EVENT_CHAR_REMOVE_ALL,
                                        PNULL, PNULL);
            SetToNoInput(data_ptr);
            data_ptr->is_rect_change = TRUE;
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    default:
        break;
    }

    if (SPIM_NO_ERROR == api_result)
    {
        if (api_content.has_data)
        {
            if (api_content.commit[0] > 0)
            {
                SetToAssociate(data_ptr, api_content.commit[0]);
                data_ptr->is_rect_change = TRUE;
            }
            else
            {
                UpdateBar(data_ptr, &api_content);
            }
        }
        else
        {
            SetToNoInput(data_ptr);
            data_ptr->is_rect_change = TRUE;
        }
    }


    return is_dealt;
}



/*==============================================================================
Function Name: HandleSysMsgAssociate
Description: HandleSysMsgAssociate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgAssociate(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    int32 cmt_idx = 0;
    wchar cmt_chr = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == event_data_ptr)
    {
        return  FALSE;
    }
    
    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        return FALSE;
    }

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_TPDOWN:
        is_dealt = TpDownNormal(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_TPMOVE:
        is_dealt = TpMoveNormal(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_TPUP:
        is_dealt = TpUpAssociate(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_KEY_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
            break;

        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
            if (data_ptr->bar_cand.has_label)
            {
                cmt_idx = msg.para.key - MMIIM_SP_KEY_1;

                if (cmt_idx < data_ptr->bar_cand.seg_cnt)
                {
                    cmt_chr = GetIndexChar(&data_ptr->asso, cmt_idx);
                    CommitChar(data_ptr, cmt_chr);
                    SetBarAsso(data_ptr, cmt_chr);
                    data_ptr->bar_cand.has_label = 0;
                }
            }
            else
            {
                //切换到没有输入的状态
                SetToNoInput(data_ptr);
                is_dealt = HandleSysMsgNoInput(data_ptr, event_data_ptr);
            }

            break;

        case MMIIM_SP_KEY_UP:

            if (0 == data_ptr->bar_cand.has_label) data_ptr->bar_cand.has_label = 1;

            if (MMIIM_SP_IDC_ACTIVE == data_ptr->bar_cand.idc_left)
            {
                data_ptr->asso.cur_page -= 1;
                MMIIM_SP_TextBarUpdate(&data_ptr->bar_cand, &data_ptr->asso);
                data_ptr->is_bar_cand_dirty = TRUE;
            }

            break;

        case MMIIM_SP_KEY_DOWN:

            if (0 == data_ptr->bar_cand.has_label) data_ptr->bar_cand.has_label = 1;

            if (MMIIM_SP_IDC_ACTIVE == data_ptr->bar_cand.idc_right)
            {
                data_ptr->asso.cur_page += 1;
                MMIIM_SP_TextBarUpdate(&data_ptr->bar_cand, &data_ptr->asso);
                data_ptr->is_bar_cand_dirty = TRUE;
            }

            break;

        case MMIIM_SP_KEY_LEFT:

            if (0 == data_ptr->bar_cand.has_label) data_ptr->bar_cand.has_label = 1;

            if (data_ptr->bar_cand.hot_index > 0)
            {
                data_ptr->bar_cand.hot_index -= 1;
                data_ptr->is_bar_cand_dirty = TRUE;
            }

            break;


        case MMIIM_SP_KEY_RIGHT:

            if (0 == data_ptr->bar_cand.has_label) data_ptr->bar_cand.has_label = 1;

            if (data_ptr->bar_cand.hot_index + 1 < data_ptr->bar_cand.seg_cnt)
            {
                data_ptr->bar_cand.hot_index += 1;
                data_ptr->is_bar_cand_dirty = TRUE;
            }

            break;

        case MMIIM_SP_KEY_OK:
            cmt_chr = GetIndexChar(&data_ptr->asso, data_ptr->bar_cand.hot_index);
            CommitChar(data_ptr, cmt_chr);
            SetBarAsso(data_ptr, cmt_chr);
            data_ptr->bar_cand.has_label = 0;
            break;

        case MMIIM_SP_KEY_CANCEL:
            SetToNoInput(data_ptr);
            data_ptr->is_rect_change = TRUE;
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    case MMIIM_SP_MSG_KEY_LONG_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_CANCEL:
            SetToNoInput(data_ptr);
            data_ptr->is_rect_change = TRUE;
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    default:
        break;
    }


    return is_dealt;
}


/*==============================================================================
Function Name: TransMsg
Description: TransMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void TransMsg(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_SP_MSG_T *msg_ptr
)
{
    MMIIM_SP_TP_AREA_INFO_T tp_area_info = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == event_data_ptr||PNULL == msg_ptr)
    {
        return;
    }

    
    tp_area_info.bar_ptr[0] = &data_ptr->bar_cand;
    tp_area_info.bar_ptr[1] = &data_ptr->bar_edit;

    MMIIM_SP_TransMsg(event_data_ptr, &tp_area_info, msg_ptr);

    return;
}





/*==============================================================================
Function Name: TpDownNormal
Description: TpDownNormal
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpDownNormal(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == para_ptr)
    {
        return  FALSE;
    }
    
    if (MMIIM_SP_MSG_TP_NONE == para_ptr->area)
    {
        return FALSE;
    }

    switch (para_ptr->area)
    {
    case MMIIM_SP_MSG_TP_BAR0:
        MMIIM_SP_TextBarTPON(&data_ptr->bar_cand,
                             para_ptr->idx,
                             &data_ptr->is_bar_cand_dirty);

        break;

    default:
        break;
    }


    return TRUE;
}



/*==============================================================================
Function Name: TpMoveNormal
Description: TpMoveNormal
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpMoveNormal(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == para_ptr)
    {
        return  FALSE;
    }
    
    data_ptr->is_bar_cand_dirty = MMIIM_SP_TextBarClearIDC(&data_ptr->bar_cand);

    if (MMIIM_SP_MSG_TP_NONE == para_ptr->area)
    {
        return FALSE;
    }

    switch (para_ptr->area)
    {
    case MMIIM_SP_MSG_TP_BAR0:
        MMIIM_SP_TextBarTPON(&data_ptr->bar_cand,
                             para_ptr->idx,
                             &data_ptr->is_bar_cand_dirty);

        break;

    default:
        break;
    }


    return TRUE;
}


/*==============================================================================
Function Name: TpUpInputing
Description: TpUpInputing
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpUpInputing(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
)
{
    SPIM_CONTENT_T api_content = {0};
    SPIM_RESULT_T api_result = SPIM_UNKNOW_ERROR;
    wchar cand_hot_chr = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == para_ptr)
    {
        return  FALSE;
    }
    
    data_ptr->is_bar_cand_dirty = MMIIM_SP_TextBarClearIDC(&data_ptr->bar_cand);

    if (MMIIM_SP_MSG_TP_NONE == para_ptr->area)
    {
        return data_ptr->is_bar_cand_dirty;
    }

    switch (para_ptr->area)
    {
    case MMIIM_SP_MSG_TP_BAR0:

        if (MMIIM_SP_TEXT_BAR_TP_IDC_LEFT == para_ptr->idx)
        {
            api_result = SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                               SPIM_EVENT_PAGE_PREVIOUS,
                                               PNULL, &api_content);
        }
        else if (MMIIM_SP_TEXT_BAR_TP_IDC_RIGHT == para_ptr->idx)
        {
            api_result = SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                               SPIM_EVENT_PAGE_NEXT,
                                               PNULL, &api_content);
        }
        else if (-1 < para_ptr->idx && para_ptr->idx < data_ptr->bar_cand.seg_cnt)
        {
            //cand, 直接提交，并清空
            cand_hot_chr = *(data_ptr->bar_cand.seg[data_ptr->bar_cand.hot_index].ptr);
            SetToAssociate(data_ptr, cand_hot_chr);
            data_ptr->is_rect_change = TRUE;
            (void)SPIM_CH_STROKE_Handle(data_ptr->core_handle,
                                        SPIM_EVENT_CHAR_REMOVE_ALL,
                                        PNULL, PNULL);
        }

        break;

    default:
        break;
    }

    if (SPIM_NO_ERROR == api_result)
    {
        if (api_content.has_data)
        {
            UpdateBar(data_ptr, &api_content);
        }
        else
        {
            SetToNoInput(data_ptr);
            data_ptr->is_rect_change = TRUE;
        }
    }


    return TRUE;
}



/*==============================================================================
Function Name: TpUpAssociate
Description: TpUpAssociate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpUpAssociate(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
)
{
    wchar asso_hot_chr = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == para_ptr)
    {
        return  FALSE;
    }
    
    data_ptr->is_bar_cand_dirty = MMIIM_SP_TextBarClearIDC(&data_ptr->bar_cand);

    if (MMIIM_SP_MSG_TP_NONE == para_ptr->area)
    {
        return data_ptr->is_bar_cand_dirty;
    }

    switch (para_ptr->area)
    {
    case MMIIM_SP_MSG_TP_BAR0:

        if (MMIIM_SP_TEXT_BAR_TP_IDC_LEFT == para_ptr->idx)
        {
            data_ptr->asso.cur_page -= 1;
            MMIIM_SP_TextBarUpdate(&data_ptr->bar_cand, &data_ptr->asso);
            data_ptr->is_bar_cand_dirty = TRUE;
        }
        else if (MMIIM_SP_TEXT_BAR_TP_IDC_RIGHT == para_ptr->idx)
        {
            data_ptr->asso.cur_page += 1;
            MMIIM_SP_TextBarUpdate(&data_ptr->bar_cand, &data_ptr->asso);
            data_ptr->is_bar_cand_dirty = TRUE;
        }
        else if (-1 < para_ptr->idx && para_ptr->idx < data_ptr->bar_cand.seg_cnt)
        {
            //asso
            asso_hot_chr = GetIndexChar(&data_ptr->asso, data_ptr->bar_cand.hot_index);
            CommitChar(data_ptr, asso_hot_chr);
            SetBarAsso(data_ptr, asso_hot_chr);
        }

        break;

    default:
        break;
    }


    return TRUE;
}



/*==============================================================================
Function Name: UpdateBar
Description: UpdateBar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void UpdateBar(
    MMIIM_SP_STROKE_DATA_T *data_ptr,
    SPIM_CONTENT_T *content_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != content_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == content_ptr)
    {
        return;
    }
    
    if (0 == content_ptr->has_data
            || content_ptr->commit[0] > 0)
    {
        return;
    }

    data_ptr->is_bar_edit_dirty = TRUE;
    data_ptr->is_bar_cand_dirty = TRUE;

    // set bars
    // 1 edit
    MMIIM_SP_TextBarGenTextSeg(&(data_ptr->bar_edit),
                               content_ptr->pre_edit, SPIM_INPUT_LEN_MAX,
                               '\0', 1);

    // char cand
    MMIIM_SP_TextBarGenTextStr(&(data_ptr->bar_cand),
                               content_ptr->candidate, content_ptr->cand_cnt);

    data_ptr->bar_cand.idc_left =
        (content_ptr->page_flag & SPIM_PAGE_FLAG_PREVIOUS) ?
        MMIIM_SP_IDC_ACTIVE : MMIIM_SP_IDC_INACTIVE;
    data_ptr->bar_cand.idc_right =
        (content_ptr->page_flag & SPIM_PAGE_FLAG_NEXT) ?
        MMIIM_SP_IDC_ACTIVE : MMIIM_SP_IDC_INACTIVE;


    return;
}



/*==============================================================================
Function Name: SetBarAsso
Description: SetBarAsso
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetBarAsso(MMIIM_SP_STROKE_DATA_T *data_ptr, wchar chr)
{
    int32 asso_cnt = 0;

    asso_cnt = MMIIM_SP_ASSO_CNT_MAX;
    SPIM_CH_QueryAsso(chr, data_ptr->asso_chr, &asso_cnt);

    data_ptr->asso.list_ptr = data_ptr->asso_chr;
    data_ptr->asso.cnt = asso_cnt;
    data_ptr->asso.cur_page = 0;

    MMIIM_SP_TextBarClear(&(data_ptr->bar_cand));
    MMIIM_SP_TextBarUpdate(&data_ptr->bar_cand, &data_ptr->asso);

    data_ptr->is_bar_cand_dirty = TRUE;

    return;
}


/*==============================================================================
Function Name: DrawUI
Description: DrawUI
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void DrawUI(MMIIM_SP_STROKE_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    
    data_ptr->is_rect_change = FALSE;
    data_ptr->is_bar_edit_dirty = FALSE;
    data_ptr->is_bar_cand_dirty = FALSE;

    switch (data_ptr->state)
    {
    case MMIIM_SP_STATE_INPUTING:
    case MMIIM_SP_STATE_SELECTING:
        MMIIM_SP_TextBarDraw(&(data_ptr->bar_edit));
        //lint -fallthrough 
    case MMIIM_SP_STATE_ASSOCIATE:
        MMIIM_SP_TextBarDraw(&(data_ptr->bar_cand));
        break;

    case MMIIM_SP_STATE_NOINPUT:
    default:
        break;
    }

    return;
}


/*==============================================================================
Function Name: ReDrawUI
Description: ReDrawUI
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void ReDrawUI(MMIIM_SP_STROKE_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    
    if (ChangeRect(data_ptr))
    {
        return;
    }

    switch (data_ptr->state)
    {
    case MMIIM_SP_STATE_INPUTING:
    case MMIIM_SP_STATE_SELECTING:

        if (data_ptr->is_bar_edit_dirty)
        {
            MMIIM_SP_TextBarDraw(&(data_ptr->bar_edit));
        }

        //lint -fallthrough 
    case MMIIM_SP_STATE_ASSOCIATE:

        if (data_ptr->is_bar_cand_dirty)
        {
            MMIIM_SP_TextBarDraw(&(data_ptr->bar_cand));
        }

        break;

    case MMIIM_SP_STATE_NOINPUT:
    default:
        break;
    }

    data_ptr->is_bar_edit_dirty = FALSE;
    data_ptr->is_bar_cand_dirty = FALSE;

    return;
}


/*==============================================================================
Function Name: CommitChar
Description: CommitChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitChar(MMIIM_SP_STROKE_DATA_T *data_ptr, wchar c)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    
    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    notify_pack.data.commit_data.commit[0] = c;
    notify_pack.data.commit_data.commit_len = 1;

    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return;
}



/*==============================================================================
Function Name: GetIndexChar
Description: GetIndexChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL wchar GetIndexChar(MMIIM_SP_BAR_DATA_T *bar_data_ptr, uint idx)
{
    uint8 abs_idx = 0;
    wchar cmt_chr = 0;

    //SCI_ASSERT(PNULL != bar_data_ptr); /*assert verified*/
    if(PNULL == bar_data_ptr)
    {
        return cmt_chr;
    }
    
    abs_idx = bar_data_ptr->cur_page * bar_data_ptr->per_page + idx;
    cmt_chr = bar_data_ptr->list_ptr[abs_idx];

    return cmt_chr;
}


/*==============================================================================
Function Name: ChangeRect
Description: ChangeRect
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN ChangeRect(MMIIM_SP_STROKE_DATA_T *data_ptr)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUI_RECT_T rect_null = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return FALSE;
    }
    
    if (data_ptr->is_rect_change)
    {
        switch (data_ptr->state)
        {
        case MMIIM_SP_STATE_NOINPUT:
            data_ptr->rect = rect_null;
            break;

        case MMIIM_SP_STATE_INPUTING:
        case MMIIM_SP_STATE_SELECTING:
            data_ptr->rect = data_ptr->bar_cand.rect;
            data_ptr->rect.top = data_ptr->bar_edit.rect.top;
            break;

        case MMIIM_SP_STATE_ASSOCIATE:
            data_ptr->rect = data_ptr->bar_cand.rect;
            break;

        default:
            break;
        }

        notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
    }

    return data_ptr->is_rect_change;
}



/*==============================================================================
Function Name: SetToNoInput
Description: SetToNoInput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetToNoInput(MMIIM_SP_STROKE_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    
    MMIIM_SP_TextBarClear(&(data_ptr->bar_cand));
    MMIIM_SP_TextBarClear(&(data_ptr->bar_edit));
    data_ptr->state = MMIIM_SP_STATE_NOINPUT;

    return;
}


/*==============================================================================
Function Name: SetToInputing
Description: SetToInputing
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetToInputing(MMIIM_SP_STROKE_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    
    data_ptr->state = MMIIM_SP_STATE_INPUTING;
    data_ptr->bar_cand.has_label = 0;
    data_ptr->is_bar_edit_dirty = TRUE;
    data_ptr->is_bar_cand_dirty = TRUE;

    return;
}


/*==============================================================================
Function Name: SetToSelecting
Description: SetToSelecting
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetToSelecting(MMIIM_SP_STROKE_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    
    data_ptr->state = MMIIM_SP_STATE_SELECTING;
    data_ptr->bar_cand.has_label = 1;
    data_ptr->is_bar_edit_dirty = TRUE;
    data_ptr->is_bar_cand_dirty = TRUE;

    return;
}


/*==============================================================================
Function Name: SetToAssociate
Description: SetToAssociate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetToAssociate(MMIIM_SP_STROKE_DATA_T *data_ptr, wchar ch)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    
    CommitChar(data_ptr, ch);
    SetBarAsso(data_ptr, ch);
    data_ptr->state = MMIIM_SP_STATE_ASSOCIATE;
    data_ptr->bar_cand.has_hot = 1;
    data_ptr->bar_cand.hot_index = 0;
    data_ptr->bar_cand.has_label = 0;
    data_ptr->is_bar_cand_dirty = TRUE;
    MMIIM_SP_TextBarClear(&(data_ptr->bar_edit));

    return;
}

#else
GUIIM_KERNEL_APIS_T const g_guiim_sp_stroke_apis = {0};

#endif //MMIIM_USE_SPIM_KEY


