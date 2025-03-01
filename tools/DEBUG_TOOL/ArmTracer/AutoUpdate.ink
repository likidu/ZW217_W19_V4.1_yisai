;--------------------------------------------------------------------------------------------------------------------------------------------------------------
; AutoUpdate.ini文件说明
;   1、将AutoUpdate.exe和该ini文件保存在某软件的根目录中
;   2、目前已实现在线升级的工具包括EngineeringUtility、ArmTracer、DspTracer,RDAAudioCalibrator  
;   3、该ini文件是Audio Calibrator 工具的升级配置文件。
;   4、手工将下面文件中的AudioCalibrator 替换成相应工具即可实现
;      EngineeringUtility、DspTracer工具或其它任意工具的在线升级
;--------------------------------------------------------------------------- ---------------------------------------------------------------------------------

[SoftwareInfo] 

;软件名称                    
SoftName=ArmTracer

;软件执行文件名
SoftEntryPoint=ArmTracer.exe

;软件在服务器上的根目录
SoftRootDir=/ArmTracer

;软件版本信息配置文件
SoftVerListConfigFile=SoftVerList.xml

;软件升级信息配置文件
SoftUpdateConfigFile=Update.xml

;对软件的说明文件
SoftReadme=readme.txt

;当前下载的软件版本及发布日期
CurVer=V6.0.5
CurDate=2019-06-26


[WinControl]
;日志的使能控制
Output=0

;使能升级功能
EnableUpdate=1

;有新版本,是否弹出确认窗口
TipWnd=1

;工具软件在启动前，还是启动后弹出升级确认窗口
BeforeSoftStart=0

;升级软件启动方式,1:随软件启动，0：从工具栏启动
StartMode=0

DelayToNext=4000

[ServerList]
;服务器台数及IP设置
ServerNums=2
0=http://10.41.208.81
1=http://10.41.208.82


Server0=10.41.208.81
User0=hongyuntan
Pass0=969594949596

Server1=10.41.208.82
User1=hongyuntan
Pass1=cfde8991949f93929f9f9f






