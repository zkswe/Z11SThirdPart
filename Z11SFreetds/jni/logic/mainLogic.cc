#pragma once
#include "uart/ProtocolSender.h"
/*
*此文件由GUI工具生成
*文件功能：用于处理用户的逻辑相应代码
*功能说明：
*========================onButtonClick_XXXX
当页面中的按键按下后系统会调用对应的函数，XXX代表GUI工具里面的[ID值]名称，
如Button1,当返回值为false的时候系统将不再处理这个按键，返回true的时候系统将会继续处理此按键。比如SYS_BACK.
*========================onSlideWindowItemClick_XXXX(int index) 
当页面中存在滑动窗口并且用户点击了滑动窗口的图标后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如slideWindow1;index 代表按下图标的偏移值
*========================onSeekBarChange_XXXX(int progress) 
当页面中存在滑动条并且用户改变了进度后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如SeekBar1;progress 代表当前的进度值
*========================ogetListItemCount_XXXX() 
当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表的总数目,XXX代表GUI工具里面的[ID值]名称，
如List1;返回值为当前列表的总条数
*========================oobtainListItemData_XXXX(ZKListView::ZKListItem *pListItem, int index)
 当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表当前条目下的内容信息,XXX代表GUI工具里面的[ID值]名称，
如List1;pListItem 是贴图中的单条目对象，index是列表总目的偏移量。具体见函数说明
*========================常用接口===============
*LOGD(...)  打印调试信息的接口
*mTextXXXPtr->setText("****") 在控件TextXXX上显示文字****
*mButton1Ptr->setSelected(true); 将控件mButton1设置为选中模式，图片会切换成选中图片，按钮文字会切换为选中后的颜色
*mSeekBarPtr->setProgress(12) 在控件mSeekBar上将进度调整到12
*mListView1Ptr->refreshListView() 让mListView1 重新刷新，当列表数据变化后调用
*mDashbroadView1Ptr->setTargetAngle(120) 在控件mDashbroadView1上指针显示角度调整到120度
*
* 在Eclipse编辑器中  使用 “alt + /”  快捷键可以打开智能提示
*/
#include "sybdb.h"
#include "sybfront.h"
#include"utils/TimeHelper.h"
#include <signal.h>
extern "C" int tdsdump_open(const char *filename);



#define MAX_LENGTH 64

static Mutex mutex1;

static char szUsername[MAX_LENGTH]; // = "sa";
static char szPassword[MAX_LENGTH]; //= "admin123";
static char szDBName[MAX_LENGTH]; //= "king"; //数据库名
static char szServer[MAX_LENGTH]; //= "192.168.1.158:1433";//数据库服务器:端口
DBINT result_code;
DBPROCESS *dbprocess;
static bool sDBConnect = false;

typedef struct{
	char name[50];
	int age;
	char time[50];
}Data_Info;

std::vector<Data_Info> dataList;

/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
	//{1,  1000},
};

static void server_on_exit() {

}

static void signal_crash_handler(int sig) {
	LOGD("signal_crash_handler  sig %d \n", sig);
    exit(-1);
}

static void signal_exit_handler(int sig) {
	LOGD("signal_exit_handler  sig %d \n", sig);
    exit(0);
}

static void installHandler() {
	atexit(server_on_exit);

    signal(SIGTERM, signal_exit_handler);
    signal(SIGINT, signal_exit_handler);

    // ignore SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    signal(SIGBUS, signal_crash_handler);	// 总线错误
    signal(SIGSEGV, signal_crash_handler);	// SIGSEGV，非法内存访问
    signal(SIGFPE, signal_crash_handler);	// SIGFPE，数学相关的异常，如被0除，浮点溢出，等等
    signal(SIGABRT, signal_crash_handler);	// SIGABRT，由调用abort函数产生，进程非正常退出
}

/**
 * 当界面构造时触发
 */
static void onUI_init(){
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");
	installHandler();
	tdsdump_open("123.log");
}

/**
 * 当切换到该界面时触发
 */
static void onUI_intent(const Intent *intentPtr) {
    if (intentPtr != NULL) {
        //TODO
    }
}

/*
 * 当界面显示时触发
 */
static void onUI_show() {
	mEditServerPtr->setText("192.168.1.158:1433");
	mEditDBNamePtr->setText("king");
	mEditUserNamePtr->setText("sa");
	mEditPasswordPtr->setText("admin123");
}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {

}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
	dbclose(dbprocess);
}

/**
 * 串口数据回调接口
 */
static void onProtocolDataUpdate(const SProtocolData &data) {

}

/**
 * 定时器触发函数
 * 不建议在此函数中写耗时操作，否则将影响UI刷新
 * 参数： id
 *         当前所触发定时器的id，与注册时的id相同
 * 返回值: true
 *             继续运行当前定时器
 *         false
 *             停止运行当前定时器
 */
static bool onUI_Timer(int id){
	switch (id) {
	case 0:{
		if(sDBConnect){
			Mutex::Autolock _l(mutex1);
		    struct tm *t = TimeHelper::getDateTime();
		    char name[50] = "Avanti";
			char data[256];
			sprintf(data, "insert into student (name, age, time) values ('%s', 28,' %d-%02d-%02d %02d:%02d:%02d')",
					name, t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			LOGD("insert: data = %s", data);
			dbcmd(dbprocess, data);
			if(dbsqlexec(dbprocess) == FAIL){
				LOGD("insert db data failed!\n");
				return -1;
			}else{
				LOGD("insert db data success!\n");
			}
		}
	}
	break;
		default:
			break;
	}
    return true;
}

/**
 * 有新的触摸事件时触发
 * 参数：ev
 *         新的触摸事件
 * 返回值：true
 *            表示该触摸事件在此被拦截，系统不再将此触摸事件传递到控件上
 *         false
 *            触摸事件将继续传递到控件上
 */
static bool onmainActivityTouchEvent(const MotionEvent &ev) {
    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
			break;
		default:
			break;
	}
	return false;
}
static void onEditTextChanged_EditUserName(const std::string &text) {
    //LOGD(" onEditTextChanged_ EditUserName %s !!!\n", text.c_str());
	memset(szUsername, 0, MAX_LENGTH);
	memcpy(szUsername, text.c_str(), text.length());
}

static bool onButtonClick_ButtonLogin(ZKButton *pButton) {
    LOGD(" ButtonClick ButtonLogin !!!\n");
    //初始化db-library
    dbinit();
    LOGD(" ButtonClick ButtonLogin 2222222222222!!!\n");
	//连接数据库
	LOGINREC *loginrec = dblogin();
	LOGD(" ButtonClick ButtonLogin 33333333333!!!\n");
	DBSETLUSER(loginrec, szUsername);
	LOGD(" ButtonClick ButtonLogin 444444444444!!!\n");
	DBSETLPWD(loginrec, szPassword);
	LOGD(" ButtonClick ButtonLogin 55555555555!!!\n");
	dbprocess = dbopen(loginrec, szServer);//连接数据库
	LOGD(" ButtonClick ButtonLogin 666666666!!!\n");
	if(dbprocess == FAIL)
	{
		LOGD("Conect to MS SQL SERVER fail, exit!\n");
		return -1;
	}
	LOGD("Connect to MS SQL SERVER success!\n");

	if(dbuse(dbprocess, szDBName) == FAIL){
		sDBConnect = false;
		LOGD("Open database failed!\n");
	}else{
		sDBConnect = true;
		LOGD("Open database success!\n");
		mWindow1Ptr->showWnd();
	}
    return false;
}

static void onEditTextChanged_EditDBName(const std::string &text) {
    //LOGD(" onEditTextChanged_ EditDBName %s !!!\n", text.c_str());
	memset(szDBName, 0, MAX_LENGTH);
	memcpy(szDBName, text.c_str(), text.length());
}

static void onEditTextChanged_EditPassword(const std::string &text) {
    //LOGD(" onEditTextChanged_ EditPassword %s !!!\n", text.c_str());
	memset(szPassword, 0, MAX_LENGTH);
	memcpy(szPassword, text.c_str(), text.length());
}

static void onEditTextChanged_EditServer(const std::string &text) {
    //LOGD(" onEditTextChanged_ EditServer %s !!!\n", text.c_str());
	memset(szServer, 0, MAX_LENGTH);
	memcpy(szServer, text.c_str(), text.length());
}

static bool onButtonClick_Button1(ZKButton *pButton) {
    LOGD(" ButtonClick Button1 !!!\n");
    Mutex::Autolock _l(mutex1);
	dbcmd(dbprocess, "select top (1000) * from student");
	if(dbsqlexec(dbprocess) == FAIL){
		printf("Query db data failed!\n");
		return -1;
	}

	int i=0;
	char temp[50];
	DBINT iNameLength=0, timeLenth = 0;
	DBINT age;
	Data_Info datainfo;
	while ((result_code = dbresults(dbprocess)) != NO_MORE_RESULTS) {
		if (result_code == SUCCEED) {
			dataList.clear();
			memset(datainfo.name, 0, 50);
			memset(datainfo.time, 0, 50);
			dbbind(dbprocess, 1, CHARBIND, (DBINT)0, (BYTE *) (datainfo.name));
			dbbind(dbprocess, 2, INTBIND, (DBINT)0, (BYTE *) &(datainfo.age));
			dbbind(dbprocess, 3, CHARBIND, (DBINT)0, (BYTE *) (datainfo.time));

			while (dbnextrow(dbprocess) != NO_MORE_ROWS){

				iNameLength = dbdatlen(dbprocess, 1);
				LOGD("iColLength = %d", iNameLength);
				temp [iNameLength]='\0';
				for(i = 0; i<iNameLength; ++i){
					LOGD("[%x]", temp[i]);
				}
				timeLenth = dbdatlen(dbprocess, 3);
				datainfo.time [timeLenth]='\0';
				LOGD(" name=[%s] age = %d, time = %s\n", datainfo.name, datainfo.age, datainfo.time);
				dataList.push_back(datainfo);
			}
		}
	}
	mListView1Ptr->refreshListView();
    return false;
}

static int getListItemCount_ListView1(const ZKListView *pListView) {
    //LOGD("getListItemCount_ListView1 !\n");
    return dataList.size();;
}

static void obtainListItemData_ListView1(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ ListView1  !!!\n");
	ZKListView::ZKListSubItem *itemAge = pListItem->findSubItemByID(ID_MAIN_SubItem1);
	ZKListView::ZKListSubItem *itemTime = pListItem->findSubItemByID(ID_MAIN_SubItem2);

	pListItem->setText(dataList.at(index).name);
	itemAge->setText(dataList.at(index).age);
	itemTime->setText(dataList.at(index).time);
}

static void onListItemClick_ListView1(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ ListView1  !!!\n");
}
