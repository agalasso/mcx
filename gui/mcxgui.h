///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb  9 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MCXGUI_H__
#define __MCXGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/choice.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/gbsizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/toolbar.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			ID_DSO = 1000,
			ID_PLANET,
			ID_LUNAR,
			ID_LOAD,
			ID_SAVE,
			ID_CROSS_HAIRS,
			ID_CROSS_BOX,
			ID_COLOR_BARS,
			ID_H_REV,
			ID_V_REV,
			ID_NEGATIVE,
			ID_FREEZE,
			ID_CCD_MODE,
			ID_SLEEP
		};
		
		wxStaticText* m_staticText1;
		wxSlider* m_senseUp;
		wxStaticText* m_senseUpVal;
		wxStaticText* m_staticText26;
		wxSlider* m_alc;
		wxStaticText* m_alcVal;
		wxStaticText* m_staticText27;
		wxSlider* m_elc;
		wxStaticText* m_elcVal;
		wxStaticText* m_staticText28;
		wxStaticText* m_agcManVal;
		wxStaticText* m_staticText29;
		wxSlider* m_agcAuto;
		wxStaticText* m_agcAutoVal;
		wxStaticText* m_staticText10;
		wxSlider* m_apcH;
		wxStaticText* m_apcHVal;
		wxStaticText* m_staticText101;
		wxSlider* m_apcV;
		wxStaticText* m_apcVVal;
		wxRadioButton* m_atwBtn;
		wxRadioButton* m_awcBtn;
		wxButton* m_awcSet;
		wxStaticText* m_staticText16;
		wxRadioButton* m_wtbRbBtn;
		wxRadioButton* m_wtb3200Btn;
		wxRadioButton* m_wtb5600Btn;
		wxStaticText* m_staticText17;
		wxSlider* m_wtbRed;
		wxStaticText* m_wtbRedVal;
		wxStaticText* m_staticText171;
		wxSlider* m_wtbBlue;
		wxStaticText* m_wtbBlueVal;
		wxStaticText* m_staticText21;
		wxSlider* m_tecLevel;
		wxStaticText* m_tecLevelVal;
		wxStaticText* m_staticText23;
		wxSlider* m_dewRemoval;
		wxStaticText* m_dewRemovalVal;
		wxStaticText* m_staticText3;
		wxSlider* m_gamma;
		wxStaticText* m_gammaVal;
		wxStaticText* m_staticText4;
		wxSlider* m_zoom;
		wxStaticText* m_zoomVal;
		wxStaticText* m_staticText5;
		wxSlider* m_coronagraph;
		wxStaticText* m_coronagraphVal;
		wxStaticText* label1;
		wxRadioBox* m_priority;
		wxRadioButton* m_titleTL;
		wxRadioButton* m_titleTR;
		wxRadioButton* m_titleBL;
		wxRadioButton* m_titleBR;
		wxTextCtrl* m_title;
		wxToolBar* m_toolBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void statusBarLeftUp( wxMouseEvent& event ) { event.Skip(); }
		virtual void senseUpScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void alcScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void elcScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void intCombobox( wxCommandEvent& event ) { event.Skip(); }
		virtual void intKillFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void intTextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void intBtnClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void agcManScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void agcAutoScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void apcHScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void apcVScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void atwSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void awcSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void wtbRBSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void wtb3200Selected( wxCommandEvent& event ) { event.Skip(); }
		virtual void wtb5600Selected( wxCommandEvent& event ) { event.Skip(); }
		virtual void wtbRedScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void wtbBlueScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void tecLevelScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void dewRemovalScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void gammaScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void zoomScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void coronagraphScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void portChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void titleText( wxCommandEvent& event ) { event.Skip(); }
		virtual void dsClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void plClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void luClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ldClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void svClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void xhClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void xbClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void cbClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void rhClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void rvClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ngClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void fzClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ccClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void sleepClicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxStatusBar* m_statusBar;
		wxComboBox* m_int;
		wxButton* m_intBtn;
		wxSlider* m_agcMan;
		wxChoice* m_port;
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrame();
	
};

#endif //__MCXGUI_H__
