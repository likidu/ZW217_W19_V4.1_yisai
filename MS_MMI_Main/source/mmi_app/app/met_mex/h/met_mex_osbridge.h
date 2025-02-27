


#ifndef __MET_MEX_OSBRIDGE_H__
#define __MET_MEX_OSBRIDGE_H__


#define MET_MEX_SEND (USER_CUSTOMIZE_TASK_ID_END + 100)


typedef enum
{
    E_MSSC_MMI_TIMER_EXPIRY,
    E_MSSC_MDI_SOUND_PLAYEND_IND,
    E_MSSC_MDI_AUDIO_PLAYEND_IND,
    E_MSSC_MDI_AUDIO_CREATEFILEHANDLEEND,
    
    E_MSSC_PHB_ADD_IND,
    E_MSSC_PHB_UPDATE_IND,
    E_MSSC_PHB_REMOVE_IND,    
    E_MSSC_SOC_EVENT,
    E_MSSC_SOC_DNS_EVENT,

    E_MSSC_MDI_PCM_STOP_IND,
    
	E_MSSC_MAX
} E_Mex_Sprd_Signal_Code;






#endif // __MET_MEX_OSBRIDGE_H__
