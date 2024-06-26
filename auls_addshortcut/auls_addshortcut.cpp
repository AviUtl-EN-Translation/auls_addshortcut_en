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
	// 0 : �g�p���Ȃ�
	// 1 : ���̓t�B��?
	// 2 : �o�̓t�B��?
	// 3 : ����t�B��?
	// 4 : �t�B��?����
	// 5 : �t�B��?�I�u�W�F�N�g
	1, //  0 : ����t?�C��
	1, //  1 : �摜�t?�C��
	1, //  2 : �����t?�C��
	1, //  3 : �e�L�X�g
	1, //  4 : ??
	1, //  5 : �t��??�o�b�t?
	1, //  6 : �����g??��
	1, //  7 : �V?��
	1, //  8 : �V?��(����)
	1, //  9 : ���O�I�u�W�F�N�g
	2, // 10 : �W??��
	2, // 11 : �g��?��
	0, // 12 : �W?�Đ� // �������o�̓t�B��?�͂P�����Ȃ��̂�?�ڂ͕s�v
	2, // 13 : �p?�e�B�N���o��
	5, // 14 : �V?��?�F���W
	4, // 15 : �F���␳
	5, // 16 : �F���␳
	4, // 17 : �N���b�s���O
	4, // 18 : �ڂ���
	4, // 19 : ���E�ڂ���
	5, // 20 : �ڂ���
	4, // 21 : ���U�C�N
	5, // 22 : ���U�C�N
	4, // 23 : ����
	5, // 24 : ����
	4, // 25 : �M��
	4, // 26 : �g�U��
	5, // 27 : �g�U��
	4, // 28 : �O��?
	5, // 29 : �O��?
	4, // 30 : �N��?�L?
	4, // 31 : �J��?�L?
	4, // 32 : ��?�i���X�L?
	4, // 33 : ���C�g
	4, // 34 : �V���h?
	4, // 35 : �����
	4, // 36 : �ʃG�b�W
	4, // 37 : �G�b�W���o
	4, // 38 : �V��?�v
	4, // 39 : �t�F?�h
	4, // 40 : ���C�v
	4, // 41 : ?�X�N
	4, // 42 : �΂߃N���b�s���O
	4, // 43 : ���˃u��?
	5, // 44 : ���˃u��?
	4, // 45 : �����u��?
	5, // 46 : �����u��?
	4, // 47 : �����Y�u��?
	5, // 48 : �����Y�u��?
	4, // 49 : ��?�V�����u��?
	5, // 50 : ��?�V�����u��?
	4, // 51 : ���W
	4, // 52 : �g�嗦
	4, // 53 : �����x
	4, // 54 : ��?
	4, // 55 : �̈�g��
	4, // 56 : ���T�C�Y
	4, // 57 : ��?�e?�V����
	4, // 58 : �U��
	5, // 59 : �U��
	4, // 60 : ��?
	5, // 61 : ��?
	4, // 62 : ?��?
	4, // 63 : ���X??
	5, // 64 : ���X??
	4, // 65 : �g��
	4, // 66 : �摜��?�v
	5, // 67 : �摜��?�v
	4, // 68 : �ɍ��W�ϊ�
	4, // 69 : �f�B�X�v���C�X�����g?�b�v
	4, // 70 : �m�C�Y
	4, // 71 : �F����
	5, // 72 : �F����
	4, // 73 : �P�F��
	5, // 74 : �P�F��
	4, // 75 : �O���f?�V����
	4, // 76 : �g���F�ݒ�
	5, // 77 : �g���F�ݒ�
	4, // 78 : ����F��ϊ�
	4, // 79 : �A�j��?�V��������
	1, // 80 : �J�X??�I�u�W�F�N�g
	4, // 81 : �X�N���v�g����
	4, // 82 : ����t?�C������
	4, // 83 : �摜�t?�C������
	4, // 84 : �C��??��?�X����
	4, // 85 : �J��������I�v�V����
	4, // 86 : �I�t�X�N��?��?��
	4, // 87 : �I�u�W�F�N�g����
	5, // 88 : �����t�B��?
	0, // 89 : ���ʃt�F?�h // �������t�B��?���ʂ�?�C?���C���ɒ��ڔz�u���Ă����ʂ������A
	0, // 90 : �����f�B���C // ���������Ȃ��̂ňꗗ����I�������Ԃ�������Ȃ��Ɣ��f���V��?�g�J�b�g���珜�O
	5, // 91 : �����f�B���C
	0, // 92 : ���m������ // �������t�B��?����
	3, // 93 : ���Ԑ���
	3, // 94 : �O��?�v����
	3, // 95 : �J��������
	0, // 96 : �J��������(�g��?��) // ���Â�??(?)�Ȃ̂Ŏg�p���Ȃ�
	4, // 97 : �J��������
	4, // 98 : �V���h?(�J��������)
	4, // 99 : �X�N���v�g(�J��������)
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
	{ 1051, "Split all selected objects"}, // �����̕��͕����܂Ƃ߂ĕ����ł��Ȃ��̂Œǉ�
	{ 1057, "Grouping" },
	{ 1058, "Ungrouping" },

	/*/ ���C��?�֘A
	{ 1025, "���C��?��?��" },
	{ 1075, "���̃��C��?��S?��/��?��" },
	{ 1032, "���C��?��?��" },
	{ 1033, "���C��?�̍폜" },
	{ 1056, "���C��?����ύX" },
	{ 1026, "���C��?�̃��b�N" },
	{ 1062, "���W�̃����N" },
	{ 1065, "��̃I�u�W�F�N�g�ŃN���b�s���O(���C��?)" },
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
// �I�u�W�F�N�g?�C�A���O�N���X
////////////////////////////////////////////////////////////////////////////////

class CObjectDialog {
public:
	HWND hwnd;
	EXEDIT_OBJECT** obj_table;
	int  *obj_index;
	int  *cmd_target;
	BYTE *filter_status; // �܂肽����?����ꊇ�ōs���ɂ͐ݒ�?�C�A���O���ɃR�s?���ꂽ�t�B��?�X�e??�X��ύX����K�v������

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
// �O��?�o���ϐ�
////////////////////////////////////////////////////////////////////////////////

HWND g_aviutl_hwnd = NULL;
HWND g_exedit_hwnd = NULL;

CObjectDialog g_objdlg;


////////////////////////////////////////////////////////////////////////////////
// �⏕�֐�
////////////////////////////////////////////////////////////////////////////////

// ?��Ώۃt�B��?�ɑ΂��鏈��
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
// �t�B��?�֐�
////////////////////////////////////////////////////////////////////////////////




// �t�B��?����������
BOOL func_init(FILTER *fp)
{
	g_aviutl_hwnd = GetWindow(fp->hwnd, GW_OWNER);

	FILTER* exedit = auls::Exedit_GetFilter(fp);
	if(!exedit) return FALSE;
	g_exedit_hwnd = exedit->hwnd;

	// �������Q�Ƃ�?��
	auls::CMemref memref;
	if(!memref.Init(fp)) goto memref_failure;
	EXEDIT_FILTER** filter_table = memref.Exedit_StaticFilterTable();

	if(!filter_table || !g_objdlg.SetMemory(&memref)) goto memref_failure;

	// �������Q�Ƃ̎��s����
	if(0) {
		memref_failure:
		MessageBox(g_aviutl_hwnd, memref.GetErrorString(), FILTER_INFORMATION, MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	int (*add_menu_item)(void*, LPSTR, HWND, int, int, int) = fp->exfunc->add_menu_item;

	// �e��ݒ�V��?�g�J�b�g��ǉ�
	fp->name = "Extra shortcuts (settings)";
	for(int i = 0; i < sizeof(EXEDIT_MENU) / sizeof(*EXEDIT_MENU); i++) {
		add_menu_item(fp, EXEDIT_MENU[i].name, g_exedit_hwnd, EXEDIT_MENU[i].id, 0, 0);
	}
	for(int i = 0; i < sizeof(SHORTCUT_MENU) / sizeof(*SHORTCUT_MENU); i++) {
		add_menu_item(fp, SHORTCUT_MENU[i].name, fp->hwnd, SHORTCUT_MENU[i].id, 0, 0);
	}


	// ���f�B�A�I�u�W�F�N�g�z�u�V��?�g�J�b�g��ǉ�
	fp->name = "Extra shortcuts (media objects)";
	for(DWORD i = 0; filter_table[i]; i++) {
		if(FILTER_CATEGORY[i] >= 1 && FILTER_CATEGORY[i] <= 3) {
			add_menu_item(fp, (LPSTR)filter_table[i]->name, fp->hwnd, auls::EXEDIT_FILTER_ID_BEGIN + i, 0, 0);
		}
	}

	// �t�B��?���ʔz�u�V��?�g�J�b�g��ǉ�
	fp->name = "Extra shortcuts (filter effects)";
	for(DWORD i = 0; filter_table[i]; i++) {
		if(FILTER_CATEGORY[i] == 4) {
			add_menu_item(fp, (LPSTR)filter_table[i]->name, fp->hwnd, auls::EXEDIT_FILTER_ID_BEGIN + i, 0, 0);
		}
	}

	// �t�B��?�I�u�W�F�N�g�z�u�V��?�g�J�b�g��ǉ�
	fp->name = "Extra shortcuts (filter objects)";
	for(DWORD i = 0; filter_table[i]; i++) {
		if(FILTER_CATEGORY[i] == 5) {
			add_menu_item(fp, (LPSTR)filter_table[i]->name, fp->hwnd, auls::EXEDIT_FILTER_ID_BEGIN + i, 0, 0);
		}
	}

	fp->name = (LPSTR)FILTER_NAME;
	return TRUE;
}

// �E�B���h�E���b�Z?�W�̏���
BOOL func_WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, void *editp, FILTER *fp)
{
	switch(msg) {
		case WM_FILTER_CHANGE_WINDOW:
			if(!g_objdlg.hwnd) {
				// WM_FILTER_INIT�̌�A�ŏ��ɌĂяo����郁�b�Z?�W
				// ����?�C?���O�łȂ��ƁA�t�B��?�̃�?�h�����ɂ���Ă̓I�u�W�F�N�g�ݒ�?�C�A���O���쐬����Ă��Ȃ���������
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
// �t�B��?�o?
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
