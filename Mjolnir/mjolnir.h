#ifndef MJOLNIRAPP_H
#define MJOLNIRAPP_H

#include <wx/listctrl.h>

class wxDragListBox : wxCheckListBox
{
public:
	wxDragListBox(wxWindow* parent)
		: wxCheckListBox(parent, wxID_ANY, wxDefaultPosition, wxSize(200, 300), 0, NULL, wxLB_SINGLE)
	{
        Connect(wxEVT_LEFT_UP, wxMouseEventHandler(wxDragListBox::OnLeftUp));
        Connect(wxEVT_MOTION, wxMouseEventHandler(wxDragListBox::OnMouseMove));
	}

	void AddItem(const wxString& item)
	{
		wxListBoxBase::Append(item);
	}
private:
	bool m_readyToDrag;
	bool m_gotFirstItem;
	int m_draggedItemIndex;
	int m_dropIndex;

	void OnLeftUp(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
};

enum
{
	ID_New = wxID_HIGHEST + 1,
	ID_Options,
	ID_Map
};

enum
{
	TOOL_SELECT = 0,
	TOOL_ZOOM,
	TOOL_CAMERA,
	TOOL_ENTITY,
	TOOL_CBLOCK,
	TOOL_SBLOCK,
	TOOL_PFACE,
	TOOL_PBLOCK
};

enum
{
	VIEW_TOP = 0,
	VIEW_FRONT,
	VIEW_SIDE
};

class AboutDialog : public wxDialog
{
public:
	AboutDialog(wxWindow* parent);
};

class MapFrame : public wxMDIParentFrame
{
public:
	wxPanel* texturePanel;
	wxPanel* visgroupPanel;
	wxPanel* objectPanel;

	MapFrame(const wxString& title);
	~MapFrame();
private:
	void OnNew(wxCommandEvent& event);
	void OnOptions(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnMap(wxCommandEvent& event);
	void OnShowDialog(wxCommandEvent& event);

	wxAuiManager m_mgr;

	DECLARE_EVENT_TABLE();
};

class AngleControl : public wxPanel
{
public:
	AngleControl(wxWindow* parent, int initialAngle = 0);
	~AngleControl();
	void OnPaint(wxPaintEvent& event);
	void SetAngle(int parm_angle);
	void OnMouseLeftDown(wxMouseEvent& event);
private:
	void OnTimer(wxTimerEvent& event);

	wxTimer* rotationTimer;
	int angle;
	bool dragging;

	DECLARE_EVENT_TABLE();
};

class TextureBrowserDialog : public wxDialog
{
public:
	TextureBrowserDialog(wxWindow* parent, const wxString& title);
	void OnItemSelected(wxListEvent& event);
private:
	void InitImageList();

	wxListCtrl* m_listCtrl;
	wxImageList* m_imageList;
};

class OptionsPropertySheetDialog : public wxPropertySheetDialog
{
public:
	OptionsPropertySheetDialog(wxWindow* parent);
};

class ObjectPropertiesSheetDialog : public wxPropertySheetDialog
{
public:
	ObjectPropertiesSheetDialog(wxWindow* parent);
private:
	void OnListBoxSelect(wxCommandEvent& event);
	wxChoice* brushEntityChoice;
	wxTextCtrl* attributeValues;
	wxChoice* attributeChoices;
	wxBoxSizer* rowSizer2;
	wxPanel* objectPage;
	bool isChoice;
};

class MapView3D : public wxGLCanvas
{
public:
	MapView3D(wxSplitterWindow* parent);
	~MapView3D();
	void OnEraseBackground(wxEraseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMovementTimer(wxTimerEvent& event);
	void Render(wxPaintEvent& event);
private:
	wxTimer* movementTimer;
	DECLARE_EVENT_TABLE();
};

class MapView2D : public wxGLCanvas
{
public:
	MapView2D(wxScrolledWindow* parent);
	void OnEraseBackground(wxEraseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseUp(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void Render();

	void SetRenderMode(int mode) { rendermode = mode; }
	void EnterKey(bool handled) { enterKeyHandled = handled; }
	void ConvertWindowToGridCoordinates(int mouseX, int mouseY, float& gridX, float& gridY);
private:
	wxScrolledWindow* scrollWin;
	float zoom = 0.0f;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	bool readyToPlace;
	bool enterKeyHandled;
	int lastMouseX, lastMouseY;
	int rendermode;

	DECLARE_EVENT_TABLE();
};

class MapDoc : public wxMDIChildFrame
{
public:
	MapDoc(wxMDIParentFrame* parent, const wxString& title);
	~MapDoc();
private:
	void OnLeftSplitterSashChanged(wxSplitterEvent& event);
	void OnRightSplitterSashChanged(wxSplitterEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnResetEnterKeyHandled(wxTimerEvent& event);

	bool enterKeyHandled;

	wxTimer* timer;
	wxTimer* resetTimer;

	wxSplitterWindow* m_leftSplitter;
    wxSplitterWindow* m_rightSplitter;

	MapView2D *glCanvasTop;
	MapView2D *glCanvasFront;
	MapView2D *glCanvasSide;

	DECLARE_EVENT_TABLE();
};

class MjolnirApp : public wxApp
{
public:
	virtual bool OnInit();
};

extern wxToolBar* tbar;
extern wxChoice* entityChoice;
extern MapFrame* frame;
extern int currentTexture;

DECLARE_APP(MjolnirApp)

#endif