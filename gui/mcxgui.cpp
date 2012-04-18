///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 13 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "mcxgui.h"

#include "CB.xpm"
#include "DS.xpm"
#include "Fz.xpm"
#include "LU.xpm"
#include "Ld.xpm"
#include "PL.xpm"
#include "Sv.xpm"
#include "XB.xpm"
#include "cc.xpm"
#include "ng.xpm"
#include "rH.xpm"
#include "rV.xpm"
#include "solar.xpm"
#include "zzz.xpm"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxSIMPLE_BORDER ); 
	m_toolBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	m_toolBar->AddTool( ID_DSO, wxT("tool"), wxBitmap( DS_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Load DSO presets"), wxT("Load DSO presets"), NULL ); 
	
	m_toolBar->AddTool( ID_PLANET, wxT("tool"), wxBitmap( PL_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Load planetary presets"), wxT("Load planetary presets"), NULL ); 
	
	m_toolBar->AddTool( ID_LUNAR, wxT("tool"), wxBitmap( LU_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Load lunar presets"), wxT("Load lunar presets"), NULL ); 
	
	m_toolBar->AddTool( ID_SOLAR, wxT("tool"), wxBitmap( solar_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Load solar presets"), wxT("Load solar presets"), NULL ); 
	
	m_toolBar->AddTool( ID_LOAD, wxT("tool"), wxBitmap( Ld_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Load custom presets"), wxT("Load custom presets"), NULL ); 
	
	m_toolBar->AddTool( ID_SAVE, wxT("tool"), wxBitmap( Sv_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Save presets"), wxT("Save presets"), NULL ); 
	
	m_toolBar->AddSeparator(); 
	
	m_toolBar->AddTool( ID_CROSS_BOX, wxT("tool"), wxBitmap( XB_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Cross box"), wxT("Cross box"), NULL ); 
	
	m_toolBar->AddTool( ID_COLOR_BARS, wxT("tool"), wxBitmap( CB_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Color bars"), wxT("Color bars"), NULL ); 
	
	m_toolBar->AddSeparator(); 
	
	m_toolBar->AddTool( ID_H_REV, wxT("tool"), wxBitmap( rH_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Flip horizontal"), wxT("Flip horizontal"), NULL ); 
	
	m_toolBar->AddTool( ID_V_REV, wxT("tool"), wxBitmap( rV_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Flip vertical"), wxT("Flip vertical"), NULL ); 
	
	m_toolBar->AddTool( ID_NEGATIVE, wxT("tool"), wxBitmap( ng_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Negative image"), wxT("Negative image"), NULL ); 
	
	m_toolBar->AddSeparator(); 
	
	m_toolBar->AddTool( ID_FREEZE, wxT("tool"), wxBitmap( Fz_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Freeze frame"), wxT("Freeze frame"), NULL ); 
	
	m_toolBar->AddSeparator(); 
	
	m_toolBar->AddTool( ID_CCD_MODE, wxT("tool"), wxBitmap( cc_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("CCD Mode"), wxT("Apply CCD Mode settings"), NULL ); 
	
	m_toolBar->AddSeparator(); 
	
	m_toolBar->AddTool( ID_SLEEP, wxT("tool"), wxBitmap( zzz_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Shutdown camera"), wxT("Shutdown camera"), NULL ); 
	
	m_toolBar->Realize(); 
	
	bSizer25->Add( m_toolBar, 0, wxEXPAND, 5 );
	
	
	bSizer25->Add( 0, 10, 0, 0, 5 );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* senseUpSizer;
	senseUpSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Sense Up"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	senseUpSizer->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_senseUp = new wxSlider( this, wxID_ANY, 0, 0, 12, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_senseUp->SetMinSize( wxSize( 160,-1 ) );
	
	senseUpSizer->Add( m_senseUp, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_senseUpVal = new wxStaticText( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_senseUpVal->Wrap( -1 );
	m_senseUpVal->SetMinSize( wxSize( 20,-1 ) );
	
	senseUpSizer->Add( m_senseUpVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	gbSizer2->Add( senseUpSizer, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* exposureSizer;
	exposureSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Exposure") ), wxVERTICAL );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText26 = new wxStaticText( this, wxID_ANY, wxT("ALC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	bSizer9->Add( m_staticText26, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_alc = new wxSlider( this, wxID_ANY, 0, 0, 15, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_alc->SetMinSize( wxSize( 160,-1 ) );
	
	bSizer9->Add( m_alc, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_alcVal = new wxStaticText( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_alcVal->Wrap( -1 );
	m_alcVal->SetMinSize( wxSize( 20,-1 ) );
	
	bSizer9->Add( m_alcVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	exposureSizer->Add( bSizer9, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, wxT("ELC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	bSizer10->Add( m_staticText27, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_elc = new wxSlider( this, wxID_ANY, 0, 0, 9, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_elc->SetMinSize( wxSize( 160,-1 ) );
	
	bSizer10->Add( m_elc, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_elcVal = new wxStaticText( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_elcVal->Wrap( -1 );
	m_elcVal->SetMinSize( wxSize( 20,-1 ) );
	
	bSizer10->Add( m_elcVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	exposureSizer->Add( bSizer10, 1, wxEXPAND, 5 );
	
	
	gbSizer2->Add( exposureSizer, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* integrationSizer;
	integrationSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Integration") ), wxHORIZONTAL );
	
	m_int = new wxComboBox( this, wxID_ANY, wxT("7"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxTE_PROCESS_ENTER );
	m_int->Append( wxT("7") );
	m_int->Append( wxT("14") );
	m_int->Append( wxT("28") );
	m_int->Append( wxT("56") );
	m_int->SetSelection( 0 );
	m_int->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	m_int->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_int->Enable( false );
	m_int->SetMinSize( wxSize( 70,-1 ) );
	
	integrationSizer->Add( m_int, 0, wxALL, 5 );
	
	m_intBtn = new wxButton( this, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	m_intBtn->Enable( false );
	
	integrationSizer->Add( m_intBtn, 0, wxALL, 5 );
	
	
	gbSizer2->Add( integrationSizer, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* agcSizer;
	agcSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("AGC") ), wxVERTICAL );
	
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText28 = new wxStaticText( this, wxID_ANY, wxT("Manual"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	bSizer101->Add( m_staticText28, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_agcMan = new wxSlider( this, wxID_ANY, 0, 0, 9, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_agcMan->SetMinSize( wxSize( 160,-1 ) );
	
	bSizer101->Add( m_agcMan, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_agcManVal = new wxStaticText( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_agcManVal->Wrap( -1 );
	m_agcManVal->SetMinSize( wxSize( 20,-1 ) );
	
	bSizer101->Add( m_agcManVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	agcSizer->Add( bSizer101, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText29 = new wxStaticText( this, wxID_ANY, wxT("Auto"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	bSizer11->Add( m_staticText29, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_agcAuto = new wxSlider( this, wxID_ANY, 0, 0, 9, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_agcAuto->SetMinSize( wxSize( 160,-1 ) );
	
	bSizer11->Add( m_agcAuto, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_agcAutoVal = new wxStaticText( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_agcAutoVal->Wrap( -1 );
	m_agcAutoVal->SetMinSize( wxSize( 20,-1 ) );
	
	bSizer11->Add( m_agcAutoVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	agcSizer->Add( bSizer11, 1, wxEXPAND, 5 );
	
	
	gbSizer2->Add( agcSizer, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* apcSizer;
	apcSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("APC") ), wxVERTICAL );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("H Gain"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer12->Add( m_staticText10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_apcH = new wxSlider( this, wxID_ANY, 0, 0, 8, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_apcH->SetMinSize( wxSize( 160,-1 ) );
	
	bSizer12->Add( m_apcH, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_apcHVal = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_apcHVal->Wrap( -1 );
	m_apcHVal->SetMinSize( wxSize( 20,-1 ) );
	
	bSizer12->Add( m_apcHVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	apcSizer->Add( bSizer12, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText101 = new wxStaticText( this, wxID_ANY, wxT("V Gain"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	bSizer121->Add( m_staticText101, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_apcV = new wxSlider( this, wxID_ANY, 0, 0, 8, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_apcV->SetMinSize( wxSize( 160,-1 ) );
	
	bSizer121->Add( m_apcV, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_apcVVal = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_apcVVal->Wrap( -1 );
	m_apcVVal->SetMinSize( wxSize( 20,-1 ) );
	
	bSizer121->Add( m_apcVVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	apcSizer->Add( bSizer121, 1, wxEXPAND, 5 );
	
	
	gbSizer2->Add( apcSizer, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* wtbSizer;
	wtbSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("White Balance") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	m_atwBtn = new wxRadioButton( this, wxID_ANY, wxT("ATW"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer18->Add( m_atwBtn, 0, wxALL, 5 );
	
	
	fgSizer1->Add( bSizer18, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_awcBtn = new wxRadioButton( this, wxID_ANY, wxT("AWC"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_awcBtn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_awcSet = new wxButton( this, wxID_ANY, wxT("Set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_awcSet->Enable( false );
	m_awcSet->SetMinSize( wxSize( 55,-1 ) );
	
	bSizer20->Add( m_awcSet, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer1->Add( bSizer20, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("Manual"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer21->Add( m_staticText16, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_wtbRbBtn = new wxRadioButton( this, wxID_ANY, wxT("R/B"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_wtbRbBtn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_wtb3200Btn = new wxRadioButton( this, wxID_ANY, wxT("3200K"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_wtb3200Btn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_wtb5600Btn = new wxRadioButton( this, wxID_ANY, wxT("5600K"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_wtb5600Btn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer1->Add( bSizer21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer23->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_staticText17 = new wxStaticText( this, wxID_ANY, wxT("Red"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer23->Add( m_staticText17, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_wtbRed = new wxSlider( this, wxID_ANY, 0, 0, 8, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_wtbRed->Enable( false );
	m_wtbRed->SetMinSize( wxSize( 160,-1 ) );
	
	bSizer23->Add( m_wtbRed, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_wtbRedVal = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( -1,-1 ), 0|wxSIMPLE_BORDER );
	m_wtbRedVal->Wrap( -1 );
	m_wtbRedVal->SetMinSize( wxSize( 20,-1 ) );
	
	bSizer23->Add( m_wtbRedVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer22->Add( bSizer23, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer231;
	bSizer231 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer231->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_staticText171 = new wxStaticText( this, wxID_ANY, wxT("Blue"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText171->Wrap( -1 );
	bSizer231->Add( m_staticText171, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_wtbBlue = new wxSlider( this, wxID_ANY, 0, 0, 8, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_wtbBlue->Enable( false );
	m_wtbBlue->SetMinSize( wxSize( 160,-1 ) );
	
	bSizer231->Add( m_wtbBlue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_wtbBlueVal = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_wtbBlueVal->Wrap( -1 );
	m_wtbBlueVal->SetMinSize( wxSize( 20,-1 ) );
	
	bSizer231->Add( m_wtbBlueVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer22->Add( bSizer231, 1, wxEXPAND, 5 );
	
	
	fgSizer1->Add( bSizer22, 1, wxEXPAND, 5 );
	
	
	wtbSizer->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	
	gbSizer2->Add( wtbSizer, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* tecSizer;
	tecSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("TEC") ), wxVERTICAL );
	
	wxBoxSizer* bSizer201;
	bSizer201 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Noise Detection"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bSizer201->Add( m_staticText21, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tecLevel = new wxSlider( this, wxID_ANY, 0, 0, 9, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_tecLevel->SetMinSize( wxSize( 160,-1 ) );
	
	bSizer201->Add( m_tecLevel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tecLevelVal = new wxStaticText( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_tecLevelVal->Wrap( -1 );
	m_tecLevelVal->SetMinSize( wxSize( 20,-1 ) );
	
	bSizer201->Add( m_tecLevelVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	tecSizer->Add( bSizer201, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer211;
	bSizer211 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText23 = new wxStaticText( this, wxID_ANY, wxT("Dew Removal"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	bSizer211->Add( m_staticText23, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_dewRemoval = new wxSlider( this, wxID_ANY, 2, 0, 2, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_dewRemoval->SetMinSize( wxSize( 160,-1 ) );
	
	bSizer211->Add( m_dewRemoval, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_dewRemovalVal = new wxStaticText( this, wxID_ANY, wxT("60"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_dewRemovalVal->Wrap( -1 );
	m_dewRemovalVal->SetMinSize( wxSize( 20,-1 ) );
	
	bSizer211->Add( m_dewRemovalVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	tecSizer->Add( bSizer211, 0, wxEXPAND, 5 );
	
	
	gbSizer2->Add( tecSizer, wxGBPosition( 2, 0 ), wxGBSpan( 2, 1 ), 0, 5 );
	
	wxBoxSizer* gammaGroupSizer;
	gammaGroupSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* gammaSizer;
	gammaSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Gamma"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	gammaSizer->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_gamma = new wxSlider( this, wxID_ANY, 0, 0, 1, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_gamma->SetMinSize( wxSize( 160,-1 ) );
	
	gammaSizer->Add( m_gamma, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_gammaVal = new wxStaticText( this, wxID_ANY, wxT("0.45"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_gammaVal->Wrap( -1 );
	m_gammaVal->SetMinSize( wxSize( 30,-1 ) );
	
	gammaSizer->Add( m_gammaVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	gammaGroupSizer->Add( gammaSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* zoomSizer;
	zoomSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Zoom"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	zoomSizer->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_zoom = new wxSlider( this, wxID_ANY, 0, 0, 9, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_zoom->SetMinSize( wxSize( 160,-1 ) );
	
	zoomSizer->Add( m_zoom, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_zoomVal = new wxStaticText( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_zoomVal->Wrap( -1 );
	m_zoomVal->SetMinSize( wxSize( 20,-1 ) );
	
	zoomSizer->Add( m_zoomVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	gammaGroupSizer->Add( zoomSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* coronagraphSizer;
	coronagraphSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Coronagraph"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	coronagraphSizer->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_coronagraph = new wxSlider( this, wxID_ANY, 0, 0, 18, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_coronagraph->SetMinSize( wxSize( 160,-1 ) );
	
	coronagraphSizer->Add( m_coronagraph, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_coronagraphVal = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_coronagraphVal->Wrap( -1 );
	m_coronagraphVal->SetMinSize( wxSize( 20,-1 ) );
	
	coronagraphSizer->Add( m_coronagraphVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	gammaGroupSizer->Add( coronagraphSizer, 0, wxEXPAND, 5 );
	
	
	gammaGroupSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* prioritySizer;
	prioritySizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	prioritySizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	label1 = new wxStaticText( this, wxID_ANY, wxT("Port"), wxDefaultPosition, wxDefaultSize, 0 );
	label1->Wrap( -1 );
	prioritySizer->Add( label1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_portChoices[] = { wxT("COM1"), wxT("COM2"), wxT("COM3"), wxT("COM4"), wxT("COM5"), wxT("COM6"), wxT("COM7"), wxT("COM8"), wxT("COM9"), wxT("COM10"), wxT("COM11"), wxT("COM12"), wxT("COM13"), wxT("COM14"), wxT("COM15"), wxT("COM16"), wxT("COM17"), wxT("COM18"), wxT("COM19"), wxT("COM20") };
	int m_portNChoices = sizeof( m_portChoices ) / sizeof( wxString );
	m_port = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_portNChoices, m_portChoices, 0 );
	m_port->SetSelection( 0 );
	prioritySizer->Add( m_port, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer251;
	bSizer251 = new wxBoxSizer( wxVERTICAL );
	
	
	prioritySizer->Add( bSizer251, 1, wxEXPAND, 5 );
	
	wxString m_priorityChoices[] = { wxT("AGC"), wxT("Sense") };
	int m_priorityNChoices = sizeof( m_priorityChoices ) / sizeof( wxString );
	m_priority = new wxRadioBox( this, wxID_ANY, wxT("Priority"), wxDefaultPosition, wxDefaultSize, m_priorityNChoices, m_priorityChoices, 1, wxRA_SPECIFY_COLS );
	m_priority->SetSelection( 0 );
	prioritySizer->Add( m_priority, 0, wxALL, 5 );
	
	
	gammaGroupSizer->Add( prioritySizer, 1, wxEXPAND, 5 );
	
	
	gbSizer2->Add( gammaGroupSizer, wxGBPosition( 2, 2 ), wxGBSpan( 2, 1 ), 0, 5 );
	
	wxBoxSizer* titleGroupSizer;
	titleGroupSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* titleSizer;
	titleSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Title") ), wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer15;
	sbSizer15 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxHORIZONTAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 0, 2, 0, 0 );
	
	m_titleTL = new wxRadioButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_titleTL->SetValue( true ); 
	m_titleTL->SetMinSize( wxSize( 20,-1 ) );
	
	gSizer2->Add( m_titleTL, 0, wxALL, 5 );
	
	m_titleTR = new wxRadioButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_titleTR, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	m_titleBL = new wxRadioButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_titleBL, 0, wxALL, 5 );
	
	m_titleBR = new wxRadioButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_titleBR, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	
	sbSizer15->Add( gSizer2, 0, 0, 5 );
	
	
	bSizer6->Add( sbSizer15, 0, 0, 5 );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	titleSizer->Add( bSizer6, 0, 0, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_title = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_title->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Tahoma") ) );
	m_title->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	m_title->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	bSizer7->Add( m_title, 1, wxALL, 5 );
	
	
	titleSizer->Add( bSizer7, 0, wxEXPAND, 5 );
	
	
	titleGroupSizer->Add( titleSizer, 0, wxEXPAND, 5 );
	
	
	gbSizer2->Add( titleGroupSizer, wxGBPosition( 0, 2 ), wxGBSpan( 2, 1 ), wxEXPAND, 5 );
	
	
	bSizer25->Add( gbSizer2, 0, wxFIXED_MINSIZE, 5 );
	
	
	bSizer25->Add( 0, 30, 0, 0, 5 );
	
	
	this->SetSizer( bSizer25 );
	this->Layout();
	bSizer25->Fit( this );
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( ID_DSO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::dsClicked ) );
	this->Connect( ID_PLANET, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::plClicked ) );
	this->Connect( ID_LUNAR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::luClicked ) );
	this->Connect( ID_SOLAR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::solarClicked ) );
	this->Connect( ID_LOAD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::ldClicked ) );
	this->Connect( ID_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::svClicked ) );
	this->Connect( ID_CROSS_BOX, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::xbClicked ) );
	this->Connect( ID_COLOR_BARS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::cbClicked ) );
	this->Connect( ID_H_REV, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::rhClicked ) );
	this->Connect( ID_V_REV, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::rvClicked ) );
	this->Connect( ID_NEGATIVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::ngClicked ) );
	this->Connect( ID_FREEZE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::fzClicked ) );
	this->Connect( ID_CCD_MODE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::ccClicked ) );
	this->Connect( ID_SLEEP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::sleepClicked ) );
	m_senseUp->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_alc->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_elc->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_int->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MainFrame::intCombobox ), NULL, this );
	m_int->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MainFrame::intKillFocus ), NULL, this );
	m_int->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::intTextEnter ), NULL, this );
	m_intBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::intBtnClicked ), NULL, this );
	m_agcMan->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcAuto->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_apcH->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcV->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_atwBtn->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::atwSelected ), NULL, this );
	m_awcBtn->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::awcSelected ), NULL, this );
	m_wtbRbBtn->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::wtbRBSelected ), NULL, this );
	m_wtb3200Btn->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::wtb3200Selected ), NULL, this );
	m_wtb5600Btn->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::wtb5600Selected ), NULL, this );
	m_wtbRed->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbBlue->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_tecLevel->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_dewRemoval->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_gamma->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_zoom->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_coronagraph->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_port->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::portChoice ), NULL, this );
	m_title->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainFrame::titleText ), NULL, this );
	m_statusBar->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( MainFrame::statusBarLeftUp ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( ID_DSO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::dsClicked ) );
	this->Disconnect( ID_PLANET, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::plClicked ) );
	this->Disconnect( ID_LUNAR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::luClicked ) );
	this->Disconnect( ID_SOLAR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::solarClicked ) );
	this->Disconnect( ID_LOAD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::ldClicked ) );
	this->Disconnect( ID_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::svClicked ) );
	this->Disconnect( ID_CROSS_BOX, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::xbClicked ) );
	this->Disconnect( ID_COLOR_BARS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::cbClicked ) );
	this->Disconnect( ID_H_REV, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::rhClicked ) );
	this->Disconnect( ID_V_REV, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::rvClicked ) );
	this->Disconnect( ID_NEGATIVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::ngClicked ) );
	this->Disconnect( ID_FREEZE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::fzClicked ) );
	this->Disconnect( ID_CCD_MODE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::ccClicked ) );
	this->Disconnect( ID_SLEEP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::sleepClicked ) );
	m_senseUp->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_senseUp->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::senseUpScroll ), NULL, this );
	m_alc->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_alc->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::alcScroll ), NULL, this );
	m_elc->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_elc->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::elcScroll ), NULL, this );
	m_int->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MainFrame::intCombobox ), NULL, this );
	m_int->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MainFrame::intKillFocus ), NULL, this );
	m_int->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::intTextEnter ), NULL, this );
	m_intBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::intBtnClicked ), NULL, this );
	m_agcMan->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcMan->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::agcManScroll ), NULL, this );
	m_agcAuto->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_agcAuto->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::agcAutoScroll ), NULL, this );
	m_apcH->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcH->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::apcHScroll ), NULL, this );
	m_apcV->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_apcV->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::apcVScroll ), NULL, this );
	m_atwBtn->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::atwSelected ), NULL, this );
	m_awcBtn->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::awcSelected ), NULL, this );
	m_wtbRbBtn->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::wtbRBSelected ), NULL, this );
	m_wtb3200Btn->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::wtb3200Selected ), NULL, this );
	m_wtb5600Btn->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::wtb5600Selected ), NULL, this );
	m_wtbRed->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbRed->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::wtbRedScroll ), NULL, this );
	m_wtbBlue->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_wtbBlue->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::wtbBlueScroll ), NULL, this );
	m_tecLevel->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_tecLevel->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::tecLevelScroll ), NULL, this );
	m_dewRemoval->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_dewRemoval->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::dewRemovalScroll ), NULL, this );
	m_gamma->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_gamma->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::gammaScroll ), NULL, this );
	m_zoom->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_zoom->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::zoomScroll ), NULL, this );
	m_coronagraph->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_coronagraph->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::coronagraphScroll ), NULL, this );
	m_port->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::portChoice ), NULL, this );
	m_title->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainFrame::titleText ), NULL, this );
	m_statusBar->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( MainFrame::statusBarLeftUp ), NULL, this );
	
}
