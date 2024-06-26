////////////////////////////////////////////////////////////////////////////////
// auls_addshortcut.cpp
////////////////////////////////////////////////////////////////////////////////

#include <aulslib/aviutl.h>
#include <aulslib/exedit.h>
#include <aulslib/memref.h>

using namespace auls;



static const LPSTR FILTER_NAME = "Add Shortcut";
static const LPSTR FILTER_INFORMATION = "Add Auls Shortcut";

static const int SHORTCUT_ID_BEGIN = 4000;

static const char FILTER_CATEGORY[] = {
	// 0 : 使用しない
	// 1 : 入力フィル?
	// 2 : 出力フィル?
	// 3 : 制御フィル?
	// 4 : フィル?効果
	// 5 : フィル?オブジェクト
	1, //  0 : 動画フ?イル
	1, //  1 : 画像フ?イル
	1, //  2 : 音声フ?イル
	1, //  3 : テキスト
	1, //  4 : ??
	1, //  5 : フレ??バッフ?
	1, //  6 : 音声波??示
	1, //  7 : シ?ン
	1, //  8 : シ?ン(音声)
	1, //  9 : 直前オブジェクト
	2, // 10 : 標??画
	2, // 11 : 拡張?画
	0, // 12 : 標?再生 // ※音声出力フィル?は１つしかないので?目は不要
	2, // 13 : パ?ティクル出力
	5, // 14 : シ?ン?ェンジ
	4, // 15 : 色調補正
	5, // 16 : 色調補正
	4, // 17 : クリッピング
	4, // 18 : ぼかし
	4, // 19 : 境界ぼかし
	5, // 20 : ぼかし
	4, // 21 : モザイク
	5, // 22 : モザイク
	4, // 23 : 発光
	5, // 24 : 発光
	4, // 25 : 閃光
	4, // 26 : 拡散光
	5, // 27 : 拡散光
	4, // 28 : グロ?
	5, // 29 : グロ?
	4, // 30 : クロ?キ?
	4, // 31 : カラ?キ?
	4, // 32 : ル?ナンスキ?
	4, // 33 : ライト
	4, // 34 : シャド?
	4, // 35 : 縁取り
	4, // 36 : 凸エッジ
	4, // 37 : エッジ抽出
	4, // 38 : シャ?プ
	4, // 39 : フェ?ド
	4, // 40 : ワイプ
	4, // 41 : ?スク
	4, // 42 : 斜めクリッピング
	4, // 43 : 放射ブラ?
	5, // 44 : 放射ブラ?
	4, // 45 : 方向ブラ?
	5, // 46 : 方向ブラ?
	4, // 47 : レンズブラ?
	5, // 48 : レンズブラ?
	4, // 49 : モ?ションブラ?
	5, // 50 : モ?ションブラ?
	4, // 51 : 座標
	4, // 52 : 拡大率
	4, // 53 : 透明度
	4, // 54 : 回?
	4, // 55 : 領域拡張
	4, // 56 : リサイズ
	4, // 57 : ロ?テ?ション
	4, // 58 : 振動
	5, // 59 : 振動
	4, // 60 : 反?
	5, // 61 : 反?
	4, // 62 : ?ラ?
	4, // 63 : ラス??
	5, // 64 : ラス??
	4, // 65 : 波紋
	4, // 66 : 画像ル?プ
	5, // 67 : 画像ル?プ
	4, // 68 : 極座標変換
	4, // 69 : ディスプレイスメント?ップ
	4, // 70 : ノイズ
	4, // 71 : 色ずれ
	5, // 72 : 色ずれ
	4, // 73 : 単色化
	5, // 74 : 単色化
	4, // 75 : グラデ?ション
	4, // 76 : 拡張色設定
	5, // 77 : 拡張色設定
	4, // 78 : 特定色域変換
	4, // 79 : アニメ?ション効果
	1, // 80 : カス??オブジェクト
	4, // 81 : スクリプト制御
	4, // 82 : 動画フ?イル合成
	4, // 83 : 画像フ?イル合成
	4, // 84 : イン??レ?ス解除
	4, // 85 : カメラ制御オプション
	4, // 86 : オフスクリ?ン?画
	4, // 87 : オブジェクト分割
	5, // 88 : 部分フィル?
	0, // 89 : 音量フェ?ド // ※音声フィル?効果は?イ?ラインに直接配置しても効果が無く、
	0, // 90 : 音声ディレイ // ※数が少ないので一覧から選択する手間がかからないと判断しショ?トカットから除外
	5, // 91 : 音声ディレイ
	0, // 92 : モノラル化 // ※音声フィル?効果
	3, // 93 : 時間制御
	3, // 94 : グル?プ制御
	3, // 95 : カメラ制御
	0, // 96 : カメラ制御(拡張?画) // ※古い??(?)なので使用しない
	4, // 97 : カメラ効果
	4, // 98 : シャド?(カメラ制御)
	4, // 99 : スクリプト(カメラ制御)
};

struct MENUITEM {
	int id;
	LPSTR name;
};

const MENUITEM EXEDIT_MENU[] = {
	{ 1037, "Import Object File" },
	{ 1038, "Export Object File" },
	{ 1088, "Select all objects" },
	{ 1089, "Select object that begin after cursor" },
	{ 1090, "Select object that ended before cursor " },
	{ 1051, "Split all selected objects"}, // 既存の物は複数まとめて分割できないので追加
	{ 1057, "Grouping" },
	{ 1058, "Ungrouping" },

	/*/ レイヤ?関連
	{ 1025, "レイヤ?の?示" },
	{ 1075, "他のレイヤ?を全?示/非?示" },
	{ 1032, "レイヤ?の?入" },
	{ 1033, "レイヤ?の削除" },
	{ 1056, "レイヤ?名を変更" },
	{ 1026, "レイヤ?のロック" },
	{ 1062, "座標のリンク" },
	{ 1065, "上のオブジェクトでクリッピング(レイヤ?)" },
	//*/

	{ 1069, "Clip with the object above" },
	{ 1081, "Target of camera control" },
	{ 1083, "Display grid (Camera Control)" },
	{ 1093, "Diaplay the target layer range" },
	{ 1029, "Track selected object" },
	{ 1034, "Snap the object" },
	{ 1096, "Scene settings" },
	{ 1014, "Preferences" },
};

enum SHORTCUT_MENU_ID {
	SMI_SET_TARGET_1 = SHORTCUT_ID_BEGIN,
	SMI_SET_TARGET_2,
	SMI_SET_TARGET_3,
	SMI_SET_TARGET_4,
	SMI_SET_TARGET_5,
	SMI_SET_TARGET_6,
	SMI_SET_TARGET_7,
	SMI_SET_TARGET_8,
	SMI_SET_TARGET_9,
	SMI_SET_TARGET_10,
	SMI_SET_TARGET_11,
	SMI_SET_TARGET_12,
	SMI_TARGET_MOVEUP,
	SMI_TARGET_MOVEDOWN,
	SMI_TARGET_DELETE,
	SMI_TARGET_INIT,
	SMI_TARGET_VALIDATE,
	SMI_TARGET_FOLD,
	SMI_FILTER_TARGET_MAKE_ALIAS,
	SMI_FILTER_VALIDATE_ALL,
	SMI_FILTER_INVALIDATE_EXCEPT_TOP_AND_TARGET,
	SMI_FILTER_FOLD_EXCEPT_TARGET,
	SMI_FILTER_UNFOLD_ALL,
	SMI_OBJDLG_RESET_POS,
	SMI_CHANGE_MEDIA_DEST,
	SMI_CHANGE_EFFECT_DEST,
	SMI_CHANGE_FILTER_DEST,
	SMI_SELECT_SCENE,
};

const MENUITEM SHORTCUT_MENU[] = {
	{ SMI_SET_TARGET_1, "Target 1st filter effect for operation" },
	{ SMI_SET_TARGET_2, "Target 2nd filter effect for operation" },
	{ SMI_SET_TARGET_3, "Target 3rd filter effect for operation" },
	{ SMI_SET_TARGET_4, "Target 4th filter effect for operation" },
	{ SMI_SET_TARGET_5, "Target 5th filter effect for operation" },
	{ SMI_SET_TARGET_6, "Target 6th filter effect for operation" },
	{ SMI_SET_TARGET_7, "Target 7th filter effect for operation" },
	{ SMI_SET_TARGET_8, "Target 8th filter effect for operation" },
	{ SMI_SET_TARGET_9, "Target 9th filter effect for operation" },
	{ SMI_SET_TARGET_10, "Target 10th filter effect for operation" },
	{ SMI_SET_TARGET_11, "Target 11th filter effect for operation" },
	{ SMI_SET_TARGET_12, "Target 12th filter effect for operation" },
	{ SMI_TARGET_MOVEUP, "Move target filter effect up" },
	{ SMI_TARGET_MOVEDOWN, "Move target filter effect down" },
	{ SMI_TARGET_DELETE, "Remove target filter effect" },
	{ SMI_TARGET_INIT, "Initialize target filter effect" },
	{ SMI_TARGET_VALIDATE, "Enable/Disable filter effect" },
	{ SMI_TARGET_FOLD, "Fold target filter effect" },
	{ SMI_FILTER_TARGET_MAKE_ALIAS, "Create an alias with the current settings" },
	{ SMI_FILTER_VALIDATE_ALL, "Enable all filter effects" },
	{ SMI_FILTER_INVALIDATE_EXCEPT_TOP_AND_TARGET, "Disable except top and target filter effects" },
	{ SMI_FILTER_FOLD_EXCEPT_TARGET, "Fold filter effects except target filter effects" },
	{ SMI_FILTER_UNFOLD_ALL, "Unfold all filter effects" },
	{ SMI_OBJDLG_RESET_POS, "Reset object dialog position" },
	{ SMI_CHANGE_MEDIA_DEST, "Change media object destination" },
	{ SMI_CHANGE_EFFECT_DEST, "Change filter effect destination" },
	{ SMI_CHANGE_FILTER_DEST, "Change filter object destination" },
	{ SMI_SELECT_SCENE, "Select scene" },
};

////////////////////////////////////////////////////////////////////////////////
// オブジェクト?イアログクラス
////////////////////////////////////////////////////////////////////////////////

class CObjectDialog {
public:
	HWND hwnd;
	EXEDIT_OBJECT** obj_table;
	int  *obj_index;
	int  *cmd_target;
	BYTE *filter_status; // 折りたたみ?作を一括で行うには設定?イアログ側にコピ?されたフィル?ステ??スを変更する必要がある

	bool SetMemory(CMemref *memref)
	{
		OBJECT_BUFFER_INFO* obi = memref->Exedit_ObjectBufferInfo();
		if(!obi) return false;
		obj_table = &(obi->data);
		obj_index = memref->Exedit_ObjDlg_ObjectIndex();
		cmd_target = memref->Exedit_ObjDlg_CommandTarget();
		filter_status = memref->Exedit_ObjDlg_FilterStatus();

		return obj_table && obj_index && cmd_target && filter_status;
	}

	void ResetWindowPos(void)
	{
		RECT wr, dr;
		GetWindowRect(hwnd, &wr);
		SystemParametersInfo(SPI_GETWORKAREA, 0, &dr, 0);
		MoveWindow(hwnd, dr.right - (wr.right - wr.left), dr.top, wr.right - wr.left, wr.bottom - wr.top, TRUE);
	}

	EXEDIT_OBJECT* GetObject(void)
	{
		if(*obj_index == -1) return NULL;
		else return (*obj_table) + *obj_index;
	}

	bool FilterMoveUp(int index)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		int min = (obj->type & (obj->TYPE_OVERLAY | obj->TYPE_CONTROL)) ? 2 : 1;
		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-2 : obj->GetFilterNum()-1;

		if(index < min || index > max) return false;

		*cmd_target = index;
		SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_MOVEUP, 0);
		return true;
	}

	bool FilterMoveDown(int index)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		int min = (obj->type & (obj->TYPE_OVERLAY | obj->TYPE_CONTROL)) ? 1 : 0;
		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-3 : obj->GetFilterNum()-2;

		if(index < min || index > max) return false;

		*cmd_target = index;
		SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_MOVEDOWN, 0);
		return true;
	}

	bool FilterDelete(int index)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		if(obj->GetFilterNum() == 1) return false;
		int min = (obj->type & (obj->TYPE_OVERLAY | obj->TYPE_CONTROL)) ? 1 : 0;
		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-2 : obj->GetFilterNum()-1;
		
		if(index < min || index > max) return false;

		*cmd_target = index;
		SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_DELETE, 0);
		return true;
	}

	bool FilterInit(int index)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-2 : obj->GetFilterNum()-1;

		if(index > max) return false;

		*cmd_target = index;
		SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_INIT, 0);
		return true;
	}

	bool FilterValidate(int index)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-2 : obj->GetFilterNum()-1;
		
		if(index > max) return false;

		CheckDlgButton(hwnd, OBJDLG_COMMAND::FILTER_VALIDATE+index, !IsDlgButtonChecked(hwnd, OBJDLG_COMMAND::FILTER_VALIDATE+index));
		SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_VALIDATE+index, 0);
		return true;
	}

	bool FilterFold(int index)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-2 : obj->GetFilterNum()-1;

		if(index > max) return false;

		SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_FOLD+index, 0);
		return true;
	}

	bool FilterMakeAlias(int index)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-2 : obj->GetFilterNum()-1;

		if(index > max) return false;

		*cmd_target = index;
		SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_MAKE_ALIAS, 0);
		return true;
	}

	bool FilterValidateAll(void)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-2 : obj->GetFilterNum()-1;

		for(int i = 0; i <= max; i++) {
			CheckDlgButton(hwnd, OBJDLG_COMMAND::FILTER_VALIDATE+i, BST_CHECKED);
			SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_VALIDATE+i, 0);
		}
		return true;
	}

	bool FilterInvalidateExceptTopAndTarget(int index)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-2 : obj->GetFilterNum()-1;

		for(int i = 0; i <= max; i++) {
			CheckDlgButton(hwnd, OBJDLG_COMMAND::FILTER_VALIDATE+i, i==0 || i==index);
			SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_VALIDATE+i, 0);
		}
		return true;
	}

	bool FilterFoldExceptTarget(int index)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-2 : obj->GetFilterNum()-1;

		for(int i = 0; i <= max; i++) {
			filter_status[i] |= obj->FILTER_STATUS_FOLD;
		}
		SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_FOLD+index, 0);
		return true;
	}

	bool FilterUnfoldAll(void)
	{
		EXEDIT_OBJECT *obj = GetObject();
		if(!obj) return false;

		int max = (obj->type & obj->TYPE_OVERLAY) ? obj->GetFilterNum()-2 : obj->GetFilterNum()-1;

		for(int i = 1; i <= max; i++) {
			filter_status[i] &= ~obj->FILTER_STATUS_FOLD;
		}
		filter_status[0] |= obj->FILTER_STATUS_FOLD;
		SendMessage(hwnd, WM_COMMAND, OBJDLG_COMMAND::FILTER_FOLD, 0);
		return true;
	}
};

////////////////////////////////////////////////////////////////////////////////
// グロ?バル変数
////////////////////////////////////////////////////////////////////////////////

HWND g_aviutl_hwnd = NULL;
HWND g_exedit_hwnd = NULL;

CObjectDialog g_objdlg;


////////////////////////////////////////////////////////////////////////////////
// 補助関数
////////////////////////////////////////////////////////////////////////////////

// ?作対象フィル?に対する処理
void ExtraCommand(auls::CFilterWrapper fp, int cmd)
{
	switch(cmd) {
		case SMI_SET_TARGET_1:
		case SMI_SET_TARGET_2:
		case SMI_SET_TARGET_3:
		case SMI_SET_TARGET_4:
		case SMI_SET_TARGET_5:
		case SMI_SET_TARGET_6:
		case SMI_SET_TARGET_7:
		case SMI_SET_TARGET_8:
		case SMI_SET_TARGET_9:
		case SMI_SET_TARGET_10:
		case SMI_SET_TARGET_11:
		case SMI_SET_TARGET_12:
			fp->SetTrackValue(0, cmd - SMI_SET_TARGET_1 + 1, true);
			break;
		case SMI_TARGET_MOVEUP:
			if(g_objdlg.FilterMoveUp(fp->track[0]-1)) {
				fp->AddTrackValue(0, -1, true);
			}
			break;
		case SMI_TARGET_MOVEDOWN:
			if(g_objdlg.FilterMoveDown(fp->track[0]-1)) {
				fp->AddTrackValue(0, 1, true);
			}
			break;
		case SMI_TARGET_DELETE:
			g_objdlg.FilterDelete(fp->track[0]-1);
			break;
		case SMI_TARGET_INIT:
			g_objdlg.FilterInit(fp->track[0]-1);
			break;
		case SMI_TARGET_VALIDATE:
			g_objdlg.FilterValidate(fp->track[0]-1);
			break;
		case SMI_TARGET_FOLD:
			g_objdlg.FilterFold(fp->track[0]-1);
			break;
		case SMI_FILTER_TARGET_MAKE_ALIAS:
			g_objdlg.FilterMakeAlias(fp->track[0]-1);
			break;
		case SMI_FILTER_VALIDATE_ALL:
			g_objdlg.FilterValidateAll();
			break;
		case SMI_FILTER_INVALIDATE_EXCEPT_TOP_AND_TARGET:
			g_objdlg.FilterInvalidateExceptTopAndTarget(fp->track[0]-1);
			break;
		case SMI_FILTER_FOLD_EXCEPT_TARGET:
			g_objdlg.FilterFoldExceptTarget(fp->track[0]-1);
			break;
		case SMI_FILTER_UNFOLD_ALL:
			g_objdlg.FilterUnfoldAll();
			break;
		case SMI_OBJDLG_RESET_POS:
			g_objdlg.ResetWindowPos();
			break;
		case SMI_CHANGE_MEDIA_DEST:
		case SMI_CHANGE_EFFECT_DEST:
		case SMI_CHANGE_FILTER_DEST:
			fp->ReverseCheckValue(cmd - SMI_CHANGE_MEDIA_DEST, true);
			break;
		case SMI_SELECT_SCENE:
			SendMessage(g_exedit_hwnd, WM_LBUTTONDOWN, 0, 0);
			break;
	}
}

void AddFilter(FILTER *fp, int id)
{
	switch(FILTER_CATEGORY[id-auls::EXEDIT_FILTER_ID_BEGIN]) {
		case 1:
			SendMessage((*g_objdlg.obj_index == -1 || fp->check[0]) ? g_exedit_hwnd : g_objdlg.hwnd, WM_COMMAND, id, 0);
			break;
		case 2:
			SendMessage(g_objdlg.hwnd, WM_COMMAND, id, 0);
			break;
		case 3:
			SendMessage(g_exedit_hwnd, WM_COMMAND, id, 0);
			break;
		case 4:
			SendMessage((*g_objdlg.obj_index == -1 || fp->check[1]) ? g_exedit_hwnd : g_objdlg.hwnd, WM_COMMAND, id, 0);
			break;
		case 5:
			SendMessage((*g_objdlg.obj_index == -1 || fp->check[2]) ? g_exedit_hwnd : g_objdlg.hwnd, WM_COMMAND, id, 0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// フィル?関数
////////////////////////////////////////////////////////////////////////////////




// フィル?初期化処理
BOOL func_init(FILTER *fp)
{
	g_aviutl_hwnd = GetWindow(fp->hwnd, GW_OWNER);

	FILTER* exedit = auls::Exedit_GetFilter(fp);
	if(!exedit) return FALSE;
	g_exedit_hwnd = exedit->hwnd;

	// メモリ参照の?備
	auls::CMemref memref;
	if(!memref.Init(fp)) goto memref_failure;
	EXEDIT_FILTER** filter_table = memref.Exedit_StaticFilterTable();

	if(!filter_table || !g_objdlg.SetMemory(&memref)) goto memref_failure;

	// メモリ参照の失敗処理
	if(0) {
		memref_failure:
		MessageBox(g_aviutl_hwnd, memref.GetErrorString(), FILTER_INFORMATION, MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	int (*add_menu_item)(void*, LPSTR, HWND, int, int, int) = fp->exfunc->add_menu_item;

	// 各種設定ショ?トカットを追加
	fp->name = "Extra shortcuts (settings)";
	for(int i = 0; i < sizeof(EXEDIT_MENU) / sizeof(*EXEDIT_MENU); i++) {
		add_menu_item(fp, EXEDIT_MENU[i].name, g_exedit_hwnd, EXEDIT_MENU[i].id, 0, 0);
	}
	for(int i = 0; i < sizeof(SHORTCUT_MENU) / sizeof(*SHORTCUT_MENU); i++) {
		add_menu_item(fp, SHORTCUT_MENU[i].name, fp->hwnd, SHORTCUT_MENU[i].id, 0, 0);
	}


	// メディアオブジェクト配置ショ?トカットを追加
	fp->name = "Extra shortcuts (media objects)";
	for(DWORD i = 0; filter_table[i]; i++) {
		if(FILTER_CATEGORY[i] >= 1 && FILTER_CATEGORY[i] <= 3) {
			add_menu_item(fp, (LPSTR)filter_table[i]->name, fp->hwnd, auls::EXEDIT_FILTER_ID_BEGIN + i, 0, 0);
		}
	}

	// フィル?効果配置ショ?トカットを追加
	fp->name = "Extra shortcuts (filter effects)";
	for(DWORD i = 0; filter_table[i]; i++) {
		if(FILTER_CATEGORY[i] == 4) {
			add_menu_item(fp, (LPSTR)filter_table[i]->name, fp->hwnd, auls::EXEDIT_FILTER_ID_BEGIN + i, 0, 0);
		}
	}

	// フィル?オブジェクト配置ショ?トカットを追加
	fp->name = "Extra shortcuts (filter objects)";
	for(DWORD i = 0; filter_table[i]; i++) {
		if(FILTER_CATEGORY[i] == 5) {
			add_menu_item(fp, (LPSTR)filter_table[i]->name, fp->hwnd, auls::EXEDIT_FILTER_ID_BEGIN + i, 0, 0);
		}
	}

	fp->name = (LPSTR)FILTER_NAME;
	return TRUE;
}

// ウィンドウメッセ?ジの処理
BOOL func_WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, void *editp, FILTER *fp)
{
	switch(msg) {
		case WM_FILTER_CHANGE_WINDOW:
			if(!g_objdlg.hwnd) {
				// WM_FILTER_INITの後、最初に呼び出されるメッセ?ジ
				// この?イ?ングでないと、フィル?のロ?ド順序によってはオブジェクト設定?イアログが作成されていない事がある
				g_objdlg.hwnd = auls::ObjDlg_GetWindow(g_exedit_hwnd);
			}
			break;

		case WM_FILTER_COMMAND:
			{
				int cmdid = LOWORD(wp);
				if(cmdid >= SHORTCUT_ID_BEGIN) {
					ExtraCommand(fp, LOWORD(wp));
				} else if(cmdid >= auls::EXEDIT_FILTER_ID_BEGIN && cmdid < auls::EXEDIT_ALIAS_ID_BEGIN) {
					AddFilter(fp, cmdid);
				} else {
					SendMessage(g_exedit_hwnd, WM_COMMAND, wp, lp);
				}
			}
			break;
	}
	
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// フィル?登?
////////////////////////////////////////////////////////////////////////////////

LPSTR track_name[] = { "To be manipulated" };
int   track_def[]  = { 1 };
int   track_min[]  = { 1 };
int   track_max[]  = { 12 };

LPSTR check_name[] = { "Place media objects in TL", "Place filter effects in TL", "Place filter objects in TL" };
int   check_def[]  = { 1, 0, 0, 1 };

FILTER_DLL filter = {
	FILTER_FLAG_ALWAYS_ACTIVE | FILTER_FLAG_EX_INFORMATION,
	0, 0,
	FILTER_NAME,
	1, track_name, track_def, track_min, track_max,
	3, check_name, check_def,
	NULL,
	func_init,
	NULL,
	NULL,
	func_WndProc,
	NULL,
	NULL,
	NULL,
	NULL,
	FILTER_INFORMATION,
};

extern "C" __declspec(dllexport) FILTER_DLL *GetFilterTable(void)
{
	return &filter;
}
