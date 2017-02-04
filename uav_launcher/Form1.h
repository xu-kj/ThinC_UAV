#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vcclr.h>
#include <stdlib.h>
#include <iomanip>
using namespace std;

#pragma warning(disable : 4996)

const char * const CONFIG_FILE = "..\\config.ini";
namespace UAVLaunch {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	bool To_string( String^ source, string &target )
	{
		pin_ptr<const wchar_t> wch = PtrToStringChars( source );
		int len = (( source->Length+1) * 2);
		char *ch = new char[ len ];
		bool result = wcstombs( ch, wch, len ) != -1;
		target = ch;
		delete ch;
		return result;
	}

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			load_config();
			this->MaximumSize = this->Size;
			this->MinimumSize = this->Size;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region the controls
	private: System::Windows::Forms::Button^  start_button;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabGeneral;
	private: System::Windows::Forms::TabPage^  tabNav;
	private: System::Windows::Forms::TabPage^  tabCam;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  scenarioFile;

	private: System::Windows::Forms::ComboBox^  navRes;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  navDriver;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::ComboBox^  camDriver;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ComboBox^  baseLightLevel;
	private: System::Windows::Forms::TextBox^  lowLightLevel;
	private: System::Windows::Forms::TextBox^  mediumLightLevel;
	private: System::Windows::Forms::TextBox^  highLightLevel;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::ComboBox^  camRes;
	private: System::Windows::Forms::GroupBox^  groupBox4;





	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::GroupBox^  groupBox6;
	private: System::Windows::Forms::CheckBox^  uavPaths;
	private: System::Windows::Forms::CheckBox^  uavCaptions;
	private: System::Windows::Forms::CheckBox^  uavIcons;
	private: System::Windows::Forms::CheckBox^  ufoIcons;
	private: System::Windows::Forms::CheckBox^  wpIcons;
	private: System::Windows::Forms::CheckBox^  ufoCaptions;
	private: System::Windows::Forms::CheckBox^  wpCaptions;
	private: System::Windows::Forms::CheckBox^  baseCaptions;
	private: System::Windows::Forms::CheckBox^  baseIcons;
	private: System::Windows::Forms::TextBox^  zoomAmount;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::TextBox^  mapIconSize;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::CheckBox^  useLightCues;

	private: System::Windows::Forms::TextBox^  highLightDist;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  mediumLightDist;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::GroupBox^  groupBox7;
	private: System::Windows::Forms::TextBox^  speedFactor;
	private: System::Windows::Forms::TextBox^  fuelFactor;
	private: System::Windows::Forms::Label^  label18;



	private: System::Windows::Forms::GroupBox^  groupBox8;
	private: System::Windows::Forms::TextBox^  downAngle;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::GroupBox^  groupBox9;


	private: System::Windows::Forms::CheckBox^  useGPSFail;
	private: System::Windows::Forms::CheckBox^  showVersion;
	private: System::Windows::Forms::CheckBox^  showTime;



private: System::Windows::Forms::TabPage^  tabColors;






private: System::Windows::Forms::GroupBox^  groupBox11;
private: System::Windows::Forms::TextBox^  uav1g;

private: System::Windows::Forms::TextBox^  uav1r;

private: System::Windows::Forms::TextBox^  uav1a;

private: System::Windows::Forms::Label^  label27;
private: System::Windows::Forms::Label^  label26;
private: System::Windows::Forms::GroupBox^  groupBox19;
private: System::Windows::Forms::Label^  label58;
private: System::Windows::Forms::TextBox^  uav9b;

private: System::Windows::Forms::Label^  label59;
private: System::Windows::Forms::TextBox^  uav9g;

private: System::Windows::Forms::TextBox^  uav9r;

private: System::Windows::Forms::TextBox^  uav9a;

private: System::Windows::Forms::Label^  label60;
private: System::Windows::Forms::Label^  label61;
private: System::Windows::Forms::GroupBox^  groupBox18;
private: System::Windows::Forms::Label^  label54;
private: System::Windows::Forms::TextBox^  uav8b;

private: System::Windows::Forms::Label^  label55;
private: System::Windows::Forms::TextBox^  uav8g;

private: System::Windows::Forms::TextBox^  uav8r;

private: System::Windows::Forms::TextBox^  uav8a;

private: System::Windows::Forms::Label^  label56;
private: System::Windows::Forms::Label^  label57;
private: System::Windows::Forms::GroupBox^  groupBox17;
private: System::Windows::Forms::Label^  label50;
private: System::Windows::Forms::TextBox^  uav7b;

private: System::Windows::Forms::Label^  label51;
private: System::Windows::Forms::TextBox^  uav7g;

private: System::Windows::Forms::TextBox^  uav7r;

private: System::Windows::Forms::TextBox^  uav7a;

private: System::Windows::Forms::Label^  label52;
private: System::Windows::Forms::Label^  label53;
private: System::Windows::Forms::GroupBox^  groupBox16;
private: System::Windows::Forms::Label^  label46;
private: System::Windows::Forms::TextBox^  uav6b;

private: System::Windows::Forms::Label^  label47;
private: System::Windows::Forms::TextBox^  uav6g;

private: System::Windows::Forms::TextBox^  uav6r;

private: System::Windows::Forms::TextBox^  uav6a;

private: System::Windows::Forms::Label^  label48;
private: System::Windows::Forms::Label^  label49;
private: System::Windows::Forms::GroupBox^  groupBox15;
private: System::Windows::Forms::Label^  label42;
private: System::Windows::Forms::TextBox^  uav5b;

private: System::Windows::Forms::Label^  label43;
private: System::Windows::Forms::TextBox^  uav5g;

private: System::Windows::Forms::TextBox^  uav5r;

private: System::Windows::Forms::TextBox^  uav5a;

private: System::Windows::Forms::Label^  label44;
private: System::Windows::Forms::Label^  label45;
private: System::Windows::Forms::GroupBox^  groupBox14;
private: System::Windows::Forms::Label^  label38;
private: System::Windows::Forms::TextBox^  uav4b;

private: System::Windows::Forms::Label^  label39;
private: System::Windows::Forms::TextBox^  uav4g;

private: System::Windows::Forms::TextBox^  uav4r;

private: System::Windows::Forms::TextBox^  uav4a;

private: System::Windows::Forms::Label^  label40;
private: System::Windows::Forms::Label^  label41;
private: System::Windows::Forms::GroupBox^  groupBox13;
private: System::Windows::Forms::Label^  label34;
private: System::Windows::Forms::TextBox^  uav3b;

private: System::Windows::Forms::Label^  label35;
private: System::Windows::Forms::TextBox^  uav3g;

private: System::Windows::Forms::TextBox^  uav3r;

private: System::Windows::Forms::TextBox^  uav3a;

private: System::Windows::Forms::Label^  label36;
private: System::Windows::Forms::Label^  label37;
private: System::Windows::Forms::GroupBox^  groupBox12;
private: System::Windows::Forms::Label^  label30;
private: System::Windows::Forms::TextBox^  uav2b;

private: System::Windows::Forms::Label^  label31;
private: System::Windows::Forms::TextBox^  uav2g;

private: System::Windows::Forms::TextBox^  uav2r;

private: System::Windows::Forms::TextBox^  uav2a;

private: System::Windows::Forms::Label^  label32;
private: System::Windows::Forms::Label^  label33;
private: System::Windows::Forms::Label^  label29;
private: System::Windows::Forms::TextBox^  uav1b;

private: System::Windows::Forms::Label^  label28;
private: System::Windows::Forms::Label^  label24;
private: System::Windows::Forms::TabPage^  tabTactors;
private: System::Windows::Forms::GroupBox^  groupBox10;
private: System::Windows::Forms::GroupBox^  groupBox20;

private: System::Windows::Forms::Label^  label63;


private: System::Windows::Forms::Label^  label62;


private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::CheckBox^  tactor1Pulse;
private: System::Windows::Forms::TextBox^  tactor1PulseInterval;












private: System::Windows::Forms::Label^  label66;
private: System::Windows::Forms::Label^  label65;


private: System::Windows::Forms::Label^  label64;
private: System::Windows::Forms::CheckBox^  useTactorCues;
private: System::Windows::Forms::TextBox^  tactor1Dist;




private: System::Windows::Forms::CheckBox^  extUAVPaths;
private: System::Windows::Forms::ComboBox^  shadeArea;
private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::CheckBox^  showProgressBar;
private: System::Windows::Forms::CheckBox^  zoomNearTarget;
private: System::Windows::Forms::CheckBox^  uavStartIcons;
private: System::Windows::Forms::TextBox^  tactor1Duration;

private: System::Windows::Forms::TextBox^  tactor1Freq2;

private: System::Windows::Forms::TextBox^  tactor1Freq1;

private: System::Windows::Forms::TextBox^  tactorGain;
private: System::Windows::Forms::TextBox^  tactorComPort1;



private: System::Windows::Forms::GroupBox^  groupBox3;
private: System::Windows::Forms::TextBox^  tactor2Duration;

private: System::Windows::Forms::TextBox^  tactor2Freq2;

private: System::Windows::Forms::TextBox^  tactor2Freq1;
private: System::Windows::Forms::TextBox^  tactor2Dist;


private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::CheckBox^  tactor2Pulse;

private: System::Windows::Forms::TextBox^  tactor2PulseInterval;

private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::Label^  label67;
private: System::Windows::Forms::Label^  label68;
private: System::Windows::Forms::Label^  label69;
private: System::Windows::Forms::ComboBox^  tactor2Source;
private: System::Windows::Forms::ComboBox^  tactor1Source;
private: System::Windows::Forms::Label^  label70;
private: System::Windows::Forms::CheckBox^  runTactorTest;
private: System::Windows::Forms::TextBox^  outputFile;
private: System::Windows::Forms::Label^  label72;









































private: System::Windows::Forms::CheckBox^  camFailAsAngle;
private: System::Windows::Forms::CheckBox^  useCamFail;
private: System::Windows::Forms::CheckBox^  allowSpeedChanges;
private: System::Windows::Forms::TabPage^  tabFeatures;
private: System::Windows::Forms::CheckBox^  lightAfterPass;

private: System::Windows::Forms::GroupBox^  groupBox24;
private: System::Windows::Forms::Label^  label86;
private: System::Windows::Forms::TextBox^  featureVisibleDist;

private: System::Windows::Forms::Label^  lblwhatever;

private: System::Windows::Forms::GroupBox^  groupBox23;
private: System::Windows::Forms::Label^  label82;
private: System::Windows::Forms::TextBox^  fub;
private: System::Windows::Forms::Label^  label83;
private: System::Windows::Forms::TextBox^  fug;
private: System::Windows::Forms::TextBox^  fur;
private: System::Windows::Forms::TextBox^  fua;
private: System::Windows::Forms::Label^  label84;
private: System::Windows::Forms::Label^  label85;
private: System::Windows::Forms::GroupBox^  groupBox22;
private: System::Windows::Forms::Label^  label78;
private: System::Windows::Forms::TextBox^  fdb;
private: System::Windows::Forms::Label^  label79;
private: System::Windows::Forms::TextBox^  fdg;
private: System::Windows::Forms::TextBox^  fdr;
private: System::Windows::Forms::TextBox^  fda;
private: System::Windows::Forms::Label^  label80;
private: System::Windows::Forms::Label^  label81;
private: System::Windows::Forms::GroupBox^  groupBox21;
private: System::Windows::Forms::Label^  label74;
private: System::Windows::Forms::TextBox^  fcb;
private: System::Windows::Forms::Label^  label75;
private: System::Windows::Forms::TextBox^  fcg;
private: System::Windows::Forms::TextBox^  fcr;
private: System::Windows::Forms::TextBox^  fca;
private: System::Windows::Forms::Label^  label76;
private: System::Windows::Forms::Label^  label77;
private: System::Windows::Forms::Label^  label73;
private: System::Windows::Forms::TextBox^  featureMissedDist;
private: System::Windows::Forms::CheckBox^  timestampOutput;
private: System::Windows::Forms::Label^  label90;
private: System::Windows::Forms::Label^  label89;
private: System::Windows::Forms::Label^  label88;
private: System::Windows::Forms::TextBox^  camPosY;
private: System::Windows::Forms::TextBox^  camPosX;
private: System::Windows::Forms::Label^  label87;
private: System::Windows::Forms::TextBox^  navPosY;
private: System::Windows::Forms::TextBox^  navPosX;
private: System::Windows::Forms::CheckBox^  allowPause;
private: System::Windows::Forms::Label^  label91;
private: System::Windows::Forms::TextBox^  tactorComPort2;
private: System::Windows::Forms::TextBox^  tactorComPort3;


private: System::Windows::Forms::Label^  label94;
private: System::Windows::Forms::TextBox^  tactorComPort6;
private: System::Windows::Forms::TextBox^  tactorComPort4;



private: System::Windows::Forms::Label^  label96;
private: System::Windows::Forms::Label^  label93;
private: System::Windows::Forms::TextBox^  tactorFrequency;
private: System::Windows::Forms::Label^  label92;
private: System::Windows::Forms::TextBox^  tactorComPort5;

private: System::Windows::Forms::Label^  label95;





private: System::Windows::Forms::Label^  label71;


#pragma endregion
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->start_button = (gcnew System::Windows::Forms::Button());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabGeneral = (gcnew System::Windows::Forms::TabPage());
			this->groupBox9 = (gcnew System::Windows::Forms::GroupBox());
			this->allowPause = (gcnew System::Windows::Forms::CheckBox());
			this->allowSpeedChanges = (gcnew System::Windows::Forms::CheckBox());
			this->useGPSFail = (gcnew System::Windows::Forms::CheckBox());
			this->showVersion = (gcnew System::Windows::Forms::CheckBox());
			this->showTime = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->speedFactor = (gcnew System::Windows::Forms::TextBox());
			this->fuelFactor = (gcnew System::Windows::Forms::TextBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->timestampOutput = (gcnew System::Windows::Forms::CheckBox());
			this->outputFile = (gcnew System::Windows::Forms::TextBox());
			this->label72 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->scenarioFile = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->tabNav = (gcnew System::Windows::Forms::TabPage());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->uavStartIcons = (gcnew System::Windows::Forms::CheckBox());
			this->extUAVPaths = (gcnew System::Windows::Forms::CheckBox());
			this->mapIconSize = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->zoomAmount = (gcnew System::Windows::Forms::TextBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->ufoIcons = (gcnew System::Windows::Forms::CheckBox());
			this->wpIcons = (gcnew System::Windows::Forms::CheckBox());
			this->ufoCaptions = (gcnew System::Windows::Forms::CheckBox());
			this->wpCaptions = (gcnew System::Windows::Forms::CheckBox());
			this->baseCaptions = (gcnew System::Windows::Forms::CheckBox());
			this->baseIcons = (gcnew System::Windows::Forms::CheckBox());
			this->uavPaths = (gcnew System::Windows::Forms::CheckBox());
			this->uavCaptions = (gcnew System::Windows::Forms::CheckBox());
			this->uavIcons = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->navPosY = (gcnew System::Windows::Forms::TextBox());
			this->navPosX = (gcnew System::Windows::Forms::TextBox());
			this->label90 = (gcnew System::Windows::Forms::Label());
			this->label89 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->navRes = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->navDriver = (gcnew System::Windows::Forms::ComboBox());
			this->tabCam = (gcnew System::Windows::Forms::TabPage());
			this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
			this->camFailAsAngle = (gcnew System::Windows::Forms::CheckBox());
			this->useCamFail = (gcnew System::Windows::Forms::CheckBox());
			this->zoomNearTarget = (gcnew System::Windows::Forms::CheckBox());
			this->showProgressBar = (gcnew System::Windows::Forms::CheckBox());
			this->downAngle = (gcnew System::Windows::Forms::TextBox());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label88 = (gcnew System::Windows::Forms::Label());
			this->camPosY = (gcnew System::Windows::Forms::TextBox());
			this->camPosX = (gcnew System::Windows::Forms::TextBox());
			this->label87 = (gcnew System::Windows::Forms::Label());
			this->camRes = (gcnew System::Windows::Forms::ComboBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->camDriver = (gcnew System::Windows::Forms::ComboBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->shadeArea = (gcnew System::Windows::Forms::ComboBox());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->mediumLightDist = (gcnew System::Windows::Forms::TextBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->highLightDist = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->useLightCues = (gcnew System::Windows::Forms::CheckBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->baseLightLevel = (gcnew System::Windows::Forms::ComboBox());
			this->lowLightLevel = (gcnew System::Windows::Forms::TextBox());
			this->mediumLightLevel = (gcnew System::Windows::Forms::TextBox());
			this->highLightLevel = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->tabColors = (gcnew System::Windows::Forms::TabPage());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->groupBox19 = (gcnew System::Windows::Forms::GroupBox());
			this->label58 = (gcnew System::Windows::Forms::Label());
			this->uav9b = (gcnew System::Windows::Forms::TextBox());
			this->label59 = (gcnew System::Windows::Forms::Label());
			this->uav9g = (gcnew System::Windows::Forms::TextBox());
			this->uav9r = (gcnew System::Windows::Forms::TextBox());
			this->uav9a = (gcnew System::Windows::Forms::TextBox());
			this->label60 = (gcnew System::Windows::Forms::Label());
			this->label61 = (gcnew System::Windows::Forms::Label());
			this->groupBox18 = (gcnew System::Windows::Forms::GroupBox());
			this->label54 = (gcnew System::Windows::Forms::Label());
			this->uav8b = (gcnew System::Windows::Forms::TextBox());
			this->label55 = (gcnew System::Windows::Forms::Label());
			this->uav8g = (gcnew System::Windows::Forms::TextBox());
			this->uav8r = (gcnew System::Windows::Forms::TextBox());
			this->uav8a = (gcnew System::Windows::Forms::TextBox());
			this->label56 = (gcnew System::Windows::Forms::Label());
			this->label57 = (gcnew System::Windows::Forms::Label());
			this->groupBox17 = (gcnew System::Windows::Forms::GroupBox());
			this->label50 = (gcnew System::Windows::Forms::Label());
			this->uav7b = (gcnew System::Windows::Forms::TextBox());
			this->label51 = (gcnew System::Windows::Forms::Label());
			this->uav7g = (gcnew System::Windows::Forms::TextBox());
			this->uav7r = (gcnew System::Windows::Forms::TextBox());
			this->uav7a = (gcnew System::Windows::Forms::TextBox());
			this->label52 = (gcnew System::Windows::Forms::Label());
			this->label53 = (gcnew System::Windows::Forms::Label());
			this->groupBox16 = (gcnew System::Windows::Forms::GroupBox());
			this->label46 = (gcnew System::Windows::Forms::Label());
			this->uav6b = (gcnew System::Windows::Forms::TextBox());
			this->label47 = (gcnew System::Windows::Forms::Label());
			this->uav6g = (gcnew System::Windows::Forms::TextBox());
			this->uav6r = (gcnew System::Windows::Forms::TextBox());
			this->uav6a = (gcnew System::Windows::Forms::TextBox());
			this->label48 = (gcnew System::Windows::Forms::Label());
			this->label49 = (gcnew System::Windows::Forms::Label());
			this->groupBox15 = (gcnew System::Windows::Forms::GroupBox());
			this->label42 = (gcnew System::Windows::Forms::Label());
			this->uav5b = (gcnew System::Windows::Forms::TextBox());
			this->label43 = (gcnew System::Windows::Forms::Label());
			this->uav5g = (gcnew System::Windows::Forms::TextBox());
			this->uav5r = (gcnew System::Windows::Forms::TextBox());
			this->uav5a = (gcnew System::Windows::Forms::TextBox());
			this->label44 = (gcnew System::Windows::Forms::Label());
			this->label45 = (gcnew System::Windows::Forms::Label());
			this->groupBox14 = (gcnew System::Windows::Forms::GroupBox());
			this->label38 = (gcnew System::Windows::Forms::Label());
			this->uav4b = (gcnew System::Windows::Forms::TextBox());
			this->label39 = (gcnew System::Windows::Forms::Label());
			this->uav4g = (gcnew System::Windows::Forms::TextBox());
			this->uav4r = (gcnew System::Windows::Forms::TextBox());
			this->uav4a = (gcnew System::Windows::Forms::TextBox());
			this->label40 = (gcnew System::Windows::Forms::Label());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->groupBox13 = (gcnew System::Windows::Forms::GroupBox());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->uav3b = (gcnew System::Windows::Forms::TextBox());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->uav3g = (gcnew System::Windows::Forms::TextBox());
			this->uav3r = (gcnew System::Windows::Forms::TextBox());
			this->uav3a = (gcnew System::Windows::Forms::TextBox());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->groupBox12 = (gcnew System::Windows::Forms::GroupBox());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->uav2b = (gcnew System::Windows::Forms::TextBox());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->uav2g = (gcnew System::Windows::Forms::TextBox());
			this->uav2r = (gcnew System::Windows::Forms::TextBox());
			this->uav2a = (gcnew System::Windows::Forms::TextBox());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->groupBox11 = (gcnew System::Windows::Forms::GroupBox());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->uav1b = (gcnew System::Windows::Forms::TextBox());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->uav1g = (gcnew System::Windows::Forms::TextBox());
			this->uav1r = (gcnew System::Windows::Forms::TextBox());
			this->uav1a = (gcnew System::Windows::Forms::TextBox());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->tabTactors = (gcnew System::Windows::Forms::TabPage());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->tactor2Source = (gcnew System::Windows::Forms::ComboBox());
			this->label69 = (gcnew System::Windows::Forms::Label());
			this->tactor2Duration = (gcnew System::Windows::Forms::TextBox());
			this->tactor2Freq2 = (gcnew System::Windows::Forms::TextBox());
			this->tactor2Freq1 = (gcnew System::Windows::Forms::TextBox());
			this->tactor2Dist = (gcnew System::Windows::Forms::TextBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->tactor2Pulse = (gcnew System::Windows::Forms::CheckBox());
			this->tactor2PulseInterval = (gcnew System::Windows::Forms::TextBox());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->label67 = (gcnew System::Windows::Forms::Label());
			this->label68 = (gcnew System::Windows::Forms::Label());
			this->groupBox20 = (gcnew System::Windows::Forms::GroupBox());
			this->tactor1Source = (gcnew System::Windows::Forms::ComboBox());
			this->label70 = (gcnew System::Windows::Forms::Label());
			this->tactor1Duration = (gcnew System::Windows::Forms::TextBox());
			this->tactor1Freq2 = (gcnew System::Windows::Forms::TextBox());
			this->tactor1Freq1 = (gcnew System::Windows::Forms::TextBox());
			this->tactor1Dist = (gcnew System::Windows::Forms::TextBox());
			this->label71 = (gcnew System::Windows::Forms::Label());
			this->tactor1Pulse = (gcnew System::Windows::Forms::CheckBox());
			this->tactor1PulseInterval = (gcnew System::Windows::Forms::TextBox());
			this->label66 = (gcnew System::Windows::Forms::Label());
			this->label65 = (gcnew System::Windows::Forms::Label());
			this->label64 = (gcnew System::Windows::Forms::Label());
			this->label63 = (gcnew System::Windows::Forms::Label());
			this->groupBox10 = (gcnew System::Windows::Forms::GroupBox());
			this->tactorComPort2 = (gcnew System::Windows::Forms::TextBox());
			this->label91 = (gcnew System::Windows::Forms::Label());
			this->runTactorTest = (gcnew System::Windows::Forms::CheckBox());
			this->tactorGain = (gcnew System::Windows::Forms::TextBox());
			this->tactorComPort1 = (gcnew System::Windows::Forms::TextBox());
			this->useTactorCues = (gcnew System::Windows::Forms::CheckBox());
			this->label62 = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->tabFeatures = (gcnew System::Windows::Forms::TabPage());
			this->groupBox24 = (gcnew System::Windows::Forms::GroupBox());
			this->lightAfterPass = (gcnew System::Windows::Forms::CheckBox());
			this->featureMissedDist = (gcnew System::Windows::Forms::TextBox());
			this->label86 = (gcnew System::Windows::Forms::Label());
			this->featureVisibleDist = (gcnew System::Windows::Forms::TextBox());
			this->lblwhatever = (gcnew System::Windows::Forms::Label());
			this->groupBox23 = (gcnew System::Windows::Forms::GroupBox());
			this->label82 = (gcnew System::Windows::Forms::Label());
			this->fub = (gcnew System::Windows::Forms::TextBox());
			this->label83 = (gcnew System::Windows::Forms::Label());
			this->fug = (gcnew System::Windows::Forms::TextBox());
			this->fur = (gcnew System::Windows::Forms::TextBox());
			this->fua = (gcnew System::Windows::Forms::TextBox());
			this->label84 = (gcnew System::Windows::Forms::Label());
			this->label85 = (gcnew System::Windows::Forms::Label());
			this->groupBox22 = (gcnew System::Windows::Forms::GroupBox());
			this->label78 = (gcnew System::Windows::Forms::Label());
			this->fdb = (gcnew System::Windows::Forms::TextBox());
			this->label79 = (gcnew System::Windows::Forms::Label());
			this->fdg = (gcnew System::Windows::Forms::TextBox());
			this->fdr = (gcnew System::Windows::Forms::TextBox());
			this->fda = (gcnew System::Windows::Forms::TextBox());
			this->label80 = (gcnew System::Windows::Forms::Label());
			this->label81 = (gcnew System::Windows::Forms::Label());
			this->groupBox21 = (gcnew System::Windows::Forms::GroupBox());
			this->label74 = (gcnew System::Windows::Forms::Label());
			this->fcb = (gcnew System::Windows::Forms::TextBox());
			this->label75 = (gcnew System::Windows::Forms::Label());
			this->fcg = (gcnew System::Windows::Forms::TextBox());
			this->fcr = (gcnew System::Windows::Forms::TextBox());
			this->fca = (gcnew System::Windows::Forms::TextBox());
			this->label76 = (gcnew System::Windows::Forms::Label());
			this->label77 = (gcnew System::Windows::Forms::Label());
			this->label73 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->tactorComPort4 = (gcnew System::Windows::Forms::TextBox());
			this->tactorComPort6 = (gcnew System::Windows::Forms::TextBox());
			this->label94 = (gcnew System::Windows::Forms::Label());
			this->tactorComPort3 = (gcnew System::Windows::Forms::TextBox());
			this->label95 = (gcnew System::Windows::Forms::Label());
			this->tactorComPort5 = (gcnew System::Windows::Forms::TextBox());
			this->label92 = (gcnew System::Windows::Forms::Label());
			this->tactorFrequency = (gcnew System::Windows::Forms::TextBox());
			this->label93 = (gcnew System::Windows::Forms::Label());
			this->label96 = (gcnew System::Windows::Forms::Label());
			this->tabControl1->SuspendLayout();
			this->tabGeneral->SuspendLayout();
			this->groupBox9->SuspendLayout();
			this->groupBox7->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->tabNav->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->tabCam->SuspendLayout();
			this->groupBox8->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->tabColors->SuspendLayout();
			this->groupBox19->SuspendLayout();
			this->groupBox18->SuspendLayout();
			this->groupBox17->SuspendLayout();
			this->groupBox16->SuspendLayout();
			this->groupBox15->SuspendLayout();
			this->groupBox14->SuspendLayout();
			this->groupBox13->SuspendLayout();
			this->groupBox12->SuspendLayout();
			this->groupBox11->SuspendLayout();
			this->tabTactors->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox20->SuspendLayout();
			this->groupBox10->SuspendLayout();
			this->tabFeatures->SuspendLayout();
			this->groupBox24->SuspendLayout();
			this->groupBox23->SuspendLayout();
			this->groupBox22->SuspendLayout();
			this->groupBox21->SuspendLayout();
			this->SuspendLayout();
			// 
			// start_button
			// 
			this->start_button->Location = System::Drawing::Point(278, 516);
			this->start_button->Name = L"start_button";
			this->start_button->Size = System::Drawing::Size(77, 29);
			this->start_button->TabIndex = 0;
			this->start_button->Text = L"Start";
			this->start_button->UseVisualStyleBackColor = true;
			this->start_button->Click += gcnew System::EventHandler(this, &Form1::start_button_Click);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabGeneral);
			this->tabControl1->Controls->Add(this->tabNav);
			this->tabControl1->Controls->Add(this->tabCam);
			this->tabControl1->Controls->Add(this->tabColors);
			this->tabControl1->Controls->Add(this->tabTactors);
			this->tabControl1->Controls->Add(this->tabFeatures);
			this->tabControl1->Location = System::Drawing::Point(12, 12);
			this->tabControl1->Multiline = true;
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(343, 502);
			this->tabControl1->TabIndex = 1;
			// 
			// tabGeneral
			// 
			this->tabGeneral->Controls->Add(this->groupBox9);
			this->tabGeneral->Controls->Add(this->groupBox7);
			this->tabGeneral->Controls->Add(this->groupBox4);
			this->tabGeneral->Location = System::Drawing::Point(4, 40);
			this->tabGeneral->Name = L"tabGeneral";
			this->tabGeneral->Padding = System::Windows::Forms::Padding(3);
			this->tabGeneral->Size = System::Drawing::Size(335, 416);
			this->tabGeneral->TabIndex = 0;
			this->tabGeneral->Text = L"General";
			this->tabGeneral->UseVisualStyleBackColor = true;
			// 
			// groupBox9
			// 
			this->groupBox9->Controls->Add(this->allowPause);
			this->groupBox9->Controls->Add(this->allowSpeedChanges);
			this->groupBox9->Controls->Add(this->useGPSFail);
			this->groupBox9->Controls->Add(this->showVersion);
			this->groupBox9->Controls->Add(this->showTime);
			this->groupBox9->Location = System::Drawing::Point(13, 187);
			this->groupBox9->Name = L"groupBox9";
			this->groupBox9->Size = System::Drawing::Size(309, 140);
			this->groupBox9->TabIndex = 9;
			this->groupBox9->TabStop = false;
			this->groupBox9->Text = L"Options Moar PLz";
			// 
			// allowPause
			// 
			this->allowPause->AutoSize = true;
			this->allowPause->Location = System::Drawing::Point(10, 116);
			this->allowPause->Name = L"allowPause";
			this->allowPause->Size = System::Drawing::Size(129, 17);
			this->allowPause->TabIndex = 5;
			this->allowPause->Text = L"Allow Pause (Escape)";
			this->allowPause->UseVisualStyleBackColor = true;
			// 
			// allowSpeedChanges
			// 
			this->allowSpeedChanges->AutoSize = true;
			this->allowSpeedChanges->Location = System::Drawing::Point(10, 93);
			this->allowSpeedChanges->Name = L"allowSpeedChanges";
			this->allowSpeedChanges->Size = System::Drawing::Size(178, 17);
			this->allowSpeedChanges->TabIndex = 4;
			this->allowSpeedChanges->Text = L"Allow Speed Changes (+/- keys)";
			this->allowSpeedChanges->UseVisualStyleBackColor = true;
			// 
			// useGPSFail
			// 
			this->useGPSFail->AutoSize = true;
			this->useGPSFail->Location = System::Drawing::Point(10, 70);
			this->useGPSFail->Name = L"useGPSFail";
			this->useGPSFail->Size = System::Drawing::Size(104, 17);
			this->useGPSFail->TabIndex = 3;
			this->useGPSFail->Text = L"Use GPS Failure";
			this->useGPSFail->UseVisualStyleBackColor = true;
			// 
			// showVersion
			// 
			this->showVersion->AutoSize = true;
			this->showVersion->Location = System::Drawing::Point(10, 47);
			this->showVersion->Name = L"showVersion";
			this->showVersion->Size = System::Drawing::Size(198, 17);
			this->showVersion->TabIndex = 2;
			this->showVersion->Text = L"Show Version in Navigation Window";
			this->showVersion->UseVisualStyleBackColor = true;
			// 
			// showTime
			// 
			this->showTime->AutoSize = true;
			this->showTime->Location = System::Drawing::Point(10, 24);
			this->showTime->Name = L"showTime";
			this->showTime->Size = System::Drawing::Size(186, 17);
			this->showTime->TabIndex = 1;
			this->showTime->Text = L"Show Time in Navigation Window";
			this->showTime->UseVisualStyleBackColor = true;
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->speedFactor);
			this->groupBox7->Controls->Add(this->fuelFactor);
			this->groupBox7->Controls->Add(this->label18);
			this->groupBox7->Controls->Add(this->label1);
			this->groupBox7->Location = System::Drawing::Point(12, 102);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(310, 79);
			this->groupBox7->TabIndex = 8;
			this->groupBox7->TabStop = false;
			this->groupBox7->Text = L"UAV Configuration";
			// 
			// speedFactor
			// 
			this->speedFactor->Location = System::Drawing::Point(112, 48);
			this->speedFactor->Name = L"speedFactor";
			this->speedFactor->Size = System::Drawing::Size(51, 20);
			this->speedFactor->TabIndex = 3;
			// 
			// fuelFactor
			// 
			this->fuelFactor->Location = System::Drawing::Point(112, 22);
			this->fuelFactor->Name = L"fuelFactor";
			this->fuelFactor->Size = System::Drawing::Size(51, 20);
			this->fuelFactor->TabIndex = 2;
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(6, 25);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(89, 13);
			this->label18->TabIndex = 1;
			this->label18->Text = L"Fuel Factor (float)";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 51);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(100, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Speed Factor (float)";
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->timestampOutput);
			this->groupBox4->Controls->Add(this->outputFile);
			this->groupBox4->Controls->Add(this->label72);
			this->groupBox4->Controls->Add(this->button1);
			this->groupBox4->Controls->Add(this->scenarioFile);
			this->groupBox4->Controls->Add(this->label2);
			this->groupBox4->Location = System::Drawing::Point(13, 15);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(310, 81);
			this->groupBox4->TabIndex = 7;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Input/Output";
			// 
			// timestampOutput
			// 
			this->timestampOutput->AutoSize = true;
			this->timestampOutput->Checked = true;
			this->timestampOutput->CheckState = System::Windows::Forms::CheckState::Checked;
			this->timestampOutput->Enabled = false;
			this->timestampOutput->Location = System::Drawing::Point(205, 49);
			this->timestampOutput->Name = L"timestampOutput";
			this->timestampOutput->Size = System::Drawing::Size(99, 17);
			this->timestampOutput->TabIndex = 10;
			this->timestampOutput->Text = L"Add Timestamp";
			this->timestampOutput->UseVisualStyleBackColor = true;
			// 
			// outputFile
			// 
			this->outputFile->Location = System::Drawing::Point(81, 47);
			this->outputFile->Name = L"outputFile";
			this->outputFile->Size = System::Drawing::Size(115, 20);
			this->outputFile->TabIndex = 9;
			// 
			// label72
			// 
			this->label72->AutoSize = true;
			this->label72->Location = System::Drawing::Point(7, 50);
			this->label72->Name = L"label72";
			this->label72->Size = System::Drawing::Size(55, 13);
			this->label72->TabIndex = 8;
			this->label72->Text = L"Ouput File";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(279, 19);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(25, 23);
			this->button1->TabIndex = 7;
			this->button1->Text = L"...";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// scenarioFile
			// 
			this->scenarioFile->Location = System::Drawing::Point(81, 21);
			this->scenarioFile->Name = L"scenarioFile";
			this->scenarioFile->Size = System::Drawing::Size(192, 20);
			this->scenarioFile->TabIndex = 5;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(7, 24);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(49, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Scenario";
			// 
			// tabNav
			// 
			this->tabNav->Controls->Add(this->groupBox6);
			this->tabNav->Controls->Add(this->groupBox5);
			this->tabNav->Location = System::Drawing::Point(4, 40);
			this->tabNav->Name = L"tabNav";
			this->tabNav->Padding = System::Windows::Forms::Padding(3);
			this->tabNav->Size = System::Drawing::Size(335, 416);
			this->tabNav->TabIndex = 1;
			this->tabNav->Text = L"Navigation Window";
			this->tabNav->UseVisualStyleBackColor = true;
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->uavStartIcons);
			this->groupBox6->Controls->Add(this->extUAVPaths);
			this->groupBox6->Controls->Add(this->mapIconSize);
			this->groupBox6->Controls->Add(this->label3);
			this->groupBox6->Controls->Add(this->zoomAmount);
			this->groupBox6->Controls->Add(this->label16);
			this->groupBox6->Controls->Add(this->ufoIcons);
			this->groupBox6->Controls->Add(this->wpIcons);
			this->groupBox6->Controls->Add(this->ufoCaptions);
			this->groupBox6->Controls->Add(this->wpCaptions);
			this->groupBox6->Controls->Add(this->baseCaptions);
			this->groupBox6->Controls->Add(this->baseIcons);
			this->groupBox6->Controls->Add(this->uavPaths);
			this->groupBox6->Controls->Add(this->uavCaptions);
			this->groupBox6->Controls->Add(this->uavIcons);
			this->groupBox6->Location = System::Drawing::Point(13, 98);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(310, 195);
			this->groupBox6->TabIndex = 8;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Map Interface";
			// 
			// uavStartIcons
			// 
			this->uavStartIcons->AutoSize = true;
			this->uavStartIcons->Location = System::Drawing::Point(10, 116);
			this->uavStartIcons->Name = L"uavStartIcons";
			this->uavStartIcons->Size = System::Drawing::Size(102, 17);
			this->uavStartIcons->TabIndex = 14;
			this->uavStartIcons->Text = L"UAV Start Icons";
			this->uavStartIcons->UseVisualStyleBackColor = true;
			// 
			// extUAVPaths
			// 
			this->extUAVPaths->AutoSize = true;
			this->extUAVPaths->Location = System::Drawing::Point(118, 116);
			this->extUAVPaths->Name = L"extUAVPaths";
			this->extUAVPaths->Size = System::Drawing::Size(126, 17);
			this->extUAVPaths->TabIndex = 13;
			this->extUAVPaths->Text = L"Extended UAV Paths";
			this->extUAVPaths->UseVisualStyleBackColor = true;
			// 
			// mapIconSize
			// 
			this->mapIconSize->Location = System::Drawing::Point(123, 165);
			this->mapIconSize->Name = L"mapIconSize";
			this->mapIconSize->Size = System::Drawing::Size(46, 20);
			this->mapIconSize->TabIndex = 12;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(7, 168);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(92, 13);
			this->label3->TabIndex = 11;
			this->label3->Text = L"Icon Size (integer)";
			// 
			// zoomAmount
			// 
			this->zoomAmount->Location = System::Drawing::Point(123, 139);
			this->zoomAmount->Name = L"zoomAmount";
			this->zoomAmount->Size = System::Drawing::Size(46, 20);
			this->zoomAmount->TabIndex = 10;
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(7, 142);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(113, 13);
			this->label16->TabIndex = 9;
			this->label16->Text = L"Zoom Increment (float)";
			// 
			// ufoIcons
			// 
			this->ufoIcons->AutoSize = true;
			this->ufoIcons->Location = System::Drawing::Point(10, 93);
			this->ufoIcons->Name = L"ufoIcons";
			this->ufoIcons->Size = System::Drawing::Size(77, 17);
			this->ufoIcons->TabIndex = 8;
			this->ufoIcons->Text = L"UFO Icons";
			this->ufoIcons->UseVisualStyleBackColor = true;
			// 
			// wpIcons
			// 
			this->wpIcons->AutoSize = true;
			this->wpIcons->Location = System::Drawing::Point(10, 70);
			this->wpIcons->Name = L"wpIcons";
			this->wpIcons->Size = System::Drawing::Size(100, 17);
			this->wpIcons->TabIndex = 7;
			this->wpIcons->Text = L"Waypoint Icons";
			this->wpIcons->UseVisualStyleBackColor = true;
			// 
			// ufoCaptions
			// 
			this->ufoCaptions->AutoSize = true;
			this->ufoCaptions->Location = System::Drawing::Point(118, 93);
			this->ufoCaptions->Name = L"ufoCaptions";
			this->ufoCaptions->Size = System::Drawing::Size(92, 17);
			this->ufoCaptions->TabIndex = 6;
			this->ufoCaptions->Text = L"UFO Captions";
			this->ufoCaptions->UseVisualStyleBackColor = true;
			// 
			// wpCaptions
			// 
			this->wpCaptions->AutoSize = true;
			this->wpCaptions->Location = System::Drawing::Point(118, 70);
			this->wpCaptions->Name = L"wpCaptions";
			this->wpCaptions->Size = System::Drawing::Size(115, 17);
			this->wpCaptions->TabIndex = 5;
			this->wpCaptions->Text = L"Waypoint Captions";
			this->wpCaptions->UseVisualStyleBackColor = true;
			// 
			// baseCaptions
			// 
			this->baseCaptions->AutoSize = true;
			this->baseCaptions->Location = System::Drawing::Point(118, 47);
			this->baseCaptions->Name = L"baseCaptions";
			this->baseCaptions->Size = System::Drawing::Size(94, 17);
			this->baseCaptions->TabIndex = 4;
			this->baseCaptions->Text = L"Base Captions";
			this->baseCaptions->UseVisualStyleBackColor = true;
			// 
			// baseIcons
			// 
			this->baseIcons->AutoSize = true;
			this->baseIcons->Location = System::Drawing::Point(10, 47);
			this->baseIcons->Name = L"baseIcons";
			this->baseIcons->Size = System::Drawing::Size(79, 17);
			this->baseIcons->TabIndex = 3;
			this->baseIcons->Text = L"Base Icons";
			this->baseIcons->UseVisualStyleBackColor = true;
			// 
			// uavPaths
			// 
			this->uavPaths->AutoSize = true;
			this->uavPaths->Location = System::Drawing::Point(218, 24);
			this->uavPaths->Name = L"uavPaths";
			this->uavPaths->Size = System::Drawing::Size(78, 17);
			this->uavPaths->TabIndex = 2;
			this->uavPaths->Text = L"UAV Paths";
			this->uavPaths->UseVisualStyleBackColor = true;
			// 
			// uavCaptions
			// 
			this->uavCaptions->AutoSize = true;
			this->uavCaptions->Location = System::Drawing::Point(118, 24);
			this->uavCaptions->Name = L"uavCaptions";
			this->uavCaptions->Size = System::Drawing::Size(92, 17);
			this->uavCaptions->TabIndex = 1;
			this->uavCaptions->Text = L"UAV Captions";
			this->uavCaptions->UseVisualStyleBackColor = true;
			// 
			// uavIcons
			// 
			this->uavIcons->AutoSize = true;
			this->uavIcons->Location = System::Drawing::Point(10, 24);
			this->uavIcons->Name = L"uavIcons";
			this->uavIcons->Size = System::Drawing::Size(77, 17);
			this->uavIcons->TabIndex = 0;
			this->uavIcons->Text = L"UAV Icons";
			this->uavIcons->UseVisualStyleBackColor = true;
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->navPosY);
			this->groupBox5->Controls->Add(this->navPosX);
			this->groupBox5->Controls->Add(this->label90);
			this->groupBox5->Controls->Add(this->label89);
			this->groupBox5->Controls->Add(this->label4);
			this->groupBox5->Controls->Add(this->navRes);
			this->groupBox5->Controls->Add(this->label5);
			this->groupBox5->Controls->Add(this->navDriver);
			this->groupBox5->Location = System::Drawing::Point(13, 15);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(310, 77);
			this->groupBox5->TabIndex = 7;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Window Preferences";
			// 
			// navPosY
			// 
			this->navPosY->Location = System::Drawing::Point(250, 41);
			this->navPosY->Name = L"navPosY";
			this->navPosY->Size = System::Drawing::Size(54, 20);
			this->navPosY->TabIndex = 13;
			// 
			// navPosX
			// 
			this->navPosX->Location = System::Drawing::Point(250, 19);
			this->navPosX->Name = L"navPosX";
			this->navPosX->Size = System::Drawing::Size(54, 20);
			this->navPosX->TabIndex = 12;
			// 
			// label90
			// 
			this->label90->AutoSize = true;
			this->label90->Location = System::Drawing::Point(190, 44);
			this->label90->Name = L"label90";
			this->label90->Size = System::Drawing::Size(54, 13);
			this->label90->TabIndex = 11;
			this->label90->Text = L"Y Position";
			// 
			// label89
			// 
			this->label89->AutoSize = true;
			this->label89->Location = System::Drawing::Point(190, 22);
			this->label89->Name = L"label89";
			this->label89->Size = System::Drawing::Size(54, 13);
			this->label89->TabIndex = 10;
			this->label89->Text = L"X Position";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(7, 44);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(57, 13);
			this->label4->TabIndex = 6;
			this->label4->Text = L"Resolution";
			// 
			// navRes
			// 
			this->navRes->FormattingEnabled = true;
			this->navRes->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"1024x768"});
			this->navRes->Location = System::Drawing::Point(70, 41);
			this->navRes->Name = L"navRes";
			this->navRes->Size = System::Drawing::Size(100, 21);
			this->navRes->TabIndex = 7;
			this->navRes->Text = L"<RESOLUTION>";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(7, 22);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(35, 13);
			this->label5->TabIndex = 4;
			this->label5->Text = L"Driver";
			// 
			// navDriver
			// 
			this->navDriver->FormattingEnabled = true;
			this->navDriver->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"OPENGL", L"DIRECT3D8", L"DIRECT3D9"});
			this->navDriver->Location = System::Drawing::Point(70, 19);
			this->navDriver->Name = L"navDriver";
			this->navDriver->Size = System::Drawing::Size(100, 21);
			this->navDriver->TabIndex = 5;
			this->navDriver->Text = L"<DRIVER>";
			// 
			// tabCam
			// 
			this->tabCam->Controls->Add(this->groupBox8);
			this->tabCam->Controls->Add(this->groupBox1);
			this->tabCam->Controls->Add(this->groupBox2);
			this->tabCam->Location = System::Drawing::Point(4, 40);
			this->tabCam->Name = L"tabCam";
			this->tabCam->Size = System::Drawing::Size(335, 416);
			this->tabCam->TabIndex = 2;
			this->tabCam->Text = L"Camera Window";
			this->tabCam->UseVisualStyleBackColor = true;
			// 
			// groupBox8
			// 
			this->groupBox8->Controls->Add(this->camFailAsAngle);
			this->groupBox8->Controls->Add(this->useCamFail);
			this->groupBox8->Controls->Add(this->zoomNearTarget);
			this->groupBox8->Controls->Add(this->showProgressBar);
			this->groupBox8->Controls->Add(this->downAngle);
			this->groupBox8->Controls->Add(this->label23);
			this->groupBox8->Location = System::Drawing::Point(13, 98);
			this->groupBox8->Name = L"groupBox8";
			this->groupBox8->Size = System::Drawing::Size(310, 94);
			this->groupBox8->TabIndex = 3;
			this->groupBox8->TabStop = false;
			this->groupBox8->Text = L"General Camera Configuration";
			// 
			// camFailAsAngle
			// 
			this->camFailAsAngle->AutoSize = true;
			this->camFailAsAngle->Enabled = false;
			this->camFailAsAngle->Location = System::Drawing::Point(132, 69);
			this->camFailAsAngle->Name = L"camFailAsAngle";
			this->camFailAsAngle->Size = System::Drawing::Size(166, 17);
			this->camFailAsAngle->TabIndex = 20;
			this->camFailAsAngle->Text = L"Camera Fail As Angle Change";
			this->camFailAsAngle->UseVisualStyleBackColor = true;
			// 
			// useCamFail
			// 
			this->useCamFail->AutoSize = true;
			this->useCamFail->Location = System::Drawing::Point(132, 46);
			this->useCamFail->Name = L"useCamFail";
			this->useCamFail->Size = System::Drawing::Size(118, 17);
			this->useCamFail->TabIndex = 19;
			this->useCamFail->Text = L"Use Camera Failure";
			this->useCamFail->UseVisualStyleBackColor = true;
			this->useCamFail->CheckedChanged += gcnew System::EventHandler(this, &Form1::useCamFail_CheckedChanged);
			// 
			// zoomNearTarget
			// 
			this->zoomNearTarget->AutoSize = true;
			this->zoomNearTarget->Location = System::Drawing::Point(10, 69);
			this->zoomNearTarget->Name = L"zoomNearTarget";
			this->zoomNearTarget->Size = System::Drawing::Size(113, 17);
			this->zoomNearTarget->TabIndex = 18;
			this->zoomNearTarget->Text = L"Zoom Near Target";
			this->zoomNearTarget->UseVisualStyleBackColor = true;
			// 
			// showProgressBar
			// 
			this->showProgressBar->AutoSize = true;
			this->showProgressBar->Location = System::Drawing::Point(10, 46);
			this->showProgressBar->Name = L"showProgressBar";
			this->showProgressBar->Size = System::Drawing::Size(116, 17);
			this->showProgressBar->TabIndex = 17;
			this->showProgressBar->Text = L"Show Progress Bar";
			this->showProgressBar->UseVisualStyleBackColor = true;
			// 
			// downAngle
			// 
			this->downAngle->Location = System::Drawing::Point(117, 20);
			this->downAngle->Name = L"downAngle";
			this->downAngle->Size = System::Drawing::Size(42, 20);
			this->downAngle->TabIndex = 16;
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(7, 23);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(88, 13);
			this->label23->TabIndex = 9;
			this->label23->Text = L"Downward Angle";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label88);
			this->groupBox1->Controls->Add(this->camPosY);
			this->groupBox1->Controls->Add(this->camPosX);
			this->groupBox1->Controls->Add(this->label87);
			this->groupBox1->Controls->Add(this->camRes);
			this->groupBox1->Controls->Add(this->label15);
			this->groupBox1->Controls->Add(this->camDriver);
			this->groupBox1->Controls->Add(this->label8);
			this->groupBox1->Location = System::Drawing::Point(13, 15);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(310, 77);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Window Preferences";
			// 
			// label88
			// 
			this->label88->AutoSize = true;
			this->label88->Location = System::Drawing::Point(190, 44);
			this->label88->Name = L"label88";
			this->label88->Size = System::Drawing::Size(54, 13);
			this->label88->TabIndex = 12;
			this->label88->Text = L"Y Position";
			// 
			// camPosY
			// 
			this->camPosY->Location = System::Drawing::Point(250, 41);
			this->camPosY->Name = L"camPosY";
			this->camPosY->Size = System::Drawing::Size(54, 20);
			this->camPosY->TabIndex = 11;
			// 
			// camPosX
			// 
			this->camPosX->Location = System::Drawing::Point(250, 19);
			this->camPosX->Name = L"camPosX";
			this->camPosX->Size = System::Drawing::Size(54, 20);
			this->camPosX->TabIndex = 10;
			// 
			// label87
			// 
			this->label87->AutoSize = true;
			this->label87->Location = System::Drawing::Point(190, 22);
			this->label87->Name = L"label87";
			this->label87->Size = System::Drawing::Size(54, 13);
			this->label87->TabIndex = 9;
			this->label87->Text = L"X Position";
			// 
			// camRes
			// 
			this->camRes->FormattingEnabled = true;
			this->camRes->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"1024x768", L"1152x864", L"2560x1600"});
			this->camRes->Location = System::Drawing::Point(70, 41);
			this->camRes->Name = L"camRes";
			this->camRes->Size = System::Drawing::Size(100, 21);
			this->camRes->TabIndex = 8;
			this->camRes->Text = L"<RESOLUTION>";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(7, 44);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(57, 13);
			this->label15->TabIndex = 7;
			this->label15->Text = L"Resolution";
			// 
			// camDriver
			// 
			this->camDriver->FormattingEnabled = true;
			this->camDriver->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"OPENGL", L"DIRECT3D8", L"DIRECT3D9"});
			this->camDriver->Location = System::Drawing::Point(70, 19);
			this->camDriver->Name = L"camDriver";
			this->camDriver->Size = System::Drawing::Size(100, 21);
			this->camDriver->TabIndex = 6;
			this->camDriver->Text = L"<DRIVER>";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(7, 22);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(35, 13);
			this->label8->TabIndex = 5;
			this->label8->Text = L"Driver";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->shadeArea);
			this->groupBox2->Controls->Add(this->label19);
			this->groupBox2->Controls->Add(this->mediumLightDist);
			this->groupBox2->Controls->Add(this->label17);
			this->groupBox2->Controls->Add(this->highLightDist);
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Controls->Add(this->useLightCues);
			this->groupBox2->Controls->Add(this->label7);
			this->groupBox2->Controls->Add(this->baseLightLevel);
			this->groupBox2->Controls->Add(this->lowLightLevel);
			this->groupBox2->Controls->Add(this->mediumLightLevel);
			this->groupBox2->Controls->Add(this->highLightLevel);
			this->groupBox2->Controls->Add(this->label14);
			this->groupBox2->Controls->Add(this->label13);
			this->groupBox2->Controls->Add(this->label12);
			this->groupBox2->Location = System::Drawing::Point(13, 198);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(310, 184);
			this->groupBox2->TabIndex = 0;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Lighting Cues";
			// 
			// shadeArea
			// 
			this->shadeArea->FormattingEnabled = true;
			this->shadeArea->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"VIEWPORT", L"FULL"});
			this->shadeArea->Location = System::Drawing::Point(117, 150);
			this->shadeArea->Name = L"shadeArea";
			this->shadeArea->Size = System::Drawing::Size(82, 21);
			this->shadeArea->TabIndex = 17;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(7, 153);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(71, 13);
			this->label19->TabIndex = 16;
			this->label19->Text = L"Shading Area";
			// 
			// mediumLightDist
			// 
			this->mediumLightDist->Location = System::Drawing::Point(249, 71);
			this->mediumLightDist->Name = L"mediumLightDist";
			this->mediumLightDist->Size = System::Drawing::Size(54, 20);
			this->mediumLightDist->TabIndex = 15;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(166, 74);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(82, 13);
			this->label17->TabIndex = 14;
			this->label17->Text = L"Active Distance";
			// 
			// highLightDist
			// 
			this->highLightDist->Location = System::Drawing::Point(249, 45);
			this->highLightDist->Name = L"highLightDist";
			this->highLightDist->Size = System::Drawing::Size(54, 20);
			this->highLightDist->TabIndex = 13;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(166, 48);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(82, 13);
			this->label6->TabIndex = 12;
			this->label6->Text = L"Active Distance";
			// 
			// useLightCues
			// 
			this->useLightCues->AutoSize = true;
			this->useLightCues->Location = System::Drawing::Point(10, 24);
			this->useLightCues->Name = L"useLightCues";
			this->useLightCues->Size = System::Drawing::Size(112, 17);
			this->useLightCues->TabIndex = 11;
			this->useLightCues->Text = L"Use Lighting Cues";
			this->useLightCues->UseVisualStyleBackColor = true;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(7, 126);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(76, 13);
			this->label7->TabIndex = 10;
			this->label7->Text = L"Baseline Level";
			// 
			// baseLightLevel
			// 
			this->baseLightLevel->FormattingEnabled = true;
			this->baseLightLevel->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"LOW", L"MEDIUM", L"HIGH"});
			this->baseLightLevel->Location = System::Drawing::Point(117, 123);
			this->baseLightLevel->Name = L"baseLightLevel";
			this->baseLightLevel->Size = System::Drawing::Size(82, 21);
			this->baseLightLevel->TabIndex = 9;
			// 
			// lowLightLevel
			// 
			this->lowLightLevel->Location = System::Drawing::Point(118, 97);
			this->lowLightLevel->Name = L"lowLightLevel";
			this->lowLightLevel->Size = System::Drawing::Size(42, 20);
			this->lowLightLevel->TabIndex = 8;
			// 
			// mediumLightLevel
			// 
			this->mediumLightLevel->Location = System::Drawing::Point(118, 71);
			this->mediumLightLevel->Name = L"mediumLightLevel";
			this->mediumLightLevel->Size = System::Drawing::Size(42, 20);
			this->mediumLightLevel->TabIndex = 7;
			// 
			// highLightLevel
			// 
			this->highLightLevel->Location = System::Drawing::Point(118, 45);
			this->highLightLevel->Name = L"highLightLevel";
			this->highLightLevel->Size = System::Drawing::Size(42, 20);
			this->highLightLevel->TabIndex = 6;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(6, 100);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(92, 13);
			this->label14->TabIndex = 2;
			this->label14->Text = L"Low Level (0-255)";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(7, 74);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(109, 13);
			this->label13->TabIndex = 3;
			this->label13->Text = L"Medium Level (0-255)";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(7, 48);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(94, 13);
			this->label12->TabIndex = 1;
			this->label12->Text = L"High Level (0-255)";
			// 
			// tabColors
			// 
			this->tabColors->Controls->Add(this->label24);
			this->tabColors->Controls->Add(this->groupBox19);
			this->tabColors->Controls->Add(this->groupBox18);
			this->tabColors->Controls->Add(this->groupBox17);
			this->tabColors->Controls->Add(this->groupBox16);
			this->tabColors->Controls->Add(this->groupBox15);
			this->tabColors->Controls->Add(this->groupBox14);
			this->tabColors->Controls->Add(this->groupBox13);
			this->tabColors->Controls->Add(this->groupBox12);
			this->tabColors->Controls->Add(this->groupBox11);
			this->tabColors->Location = System::Drawing::Point(4, 40);
			this->tabColors->Name = L"tabColors";
			this->tabColors->Padding = System::Windows::Forms::Padding(3);
			this->tabColors->Size = System::Drawing::Size(335, 416);
			this->tabColors->TabIndex = 3;
			this->tabColors->Text = L"UAV Colors";
			this->tabColors->UseVisualStyleBackColor = true;
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(14, 11);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(286, 39);
			this->label24->TabIndex = 16;
			this->label24->Text = L"Colors consist of red, green, and blue values. The level of\r\ntransparency is dete" 
				L"rmined by the alpha. All of these values\r\nrange between 0 and 255.";
			// 
			// groupBox19
			// 
			this->groupBox19->Controls->Add(this->label58);
			this->groupBox19->Controls->Add(this->uav9b);
			this->groupBox19->Controls->Add(this->label59);
			this->groupBox19->Controls->Add(this->uav9g);
			this->groupBox19->Controls->Add(this->uav9r);
			this->groupBox19->Controls->Add(this->uav9a);
			this->groupBox19->Controls->Add(this->label60);
			this->groupBox19->Controls->Add(this->label61);
			this->groupBox19->Location = System::Drawing::Point(228, 295);
			this->groupBox19->Name = L"groupBox19";
			this->groupBox19->Size = System::Drawing::Size(94, 115);
			this->groupBox19->TabIndex = 14;
			this->groupBox19->TabStop = false;
			this->groupBox19->Text = L"UAV 9";
			// 
			// label58
			// 
			this->label58->AutoSize = true;
			this->label58->Location = System::Drawing::Point(7, 91);
			this->label58->Name = L"label58";
			this->label58->Size = System::Drawing::Size(28, 13);
			this->label58->TabIndex = 13;
			this->label58->Text = L"Blue";
			// 
			// uav9b
			// 
			this->uav9b->Location = System::Drawing::Point(47, 88);
			this->uav9b->Name = L"uav9b";
			this->uav9b->Size = System::Drawing::Size(37, 20);
			this->uav9b->TabIndex = 12;
			// 
			// label59
			// 
			this->label59->AutoSize = true;
			this->label59->Location = System::Drawing::Point(7, 68);
			this->label59->Name = L"label59";
			this->label59->Size = System::Drawing::Size(36, 13);
			this->label59->TabIndex = 11;
			this->label59->Text = L"Green";
			// 
			// uav9g
			// 
			this->uav9g->Location = System::Drawing::Point(47, 65);
			this->uav9g->Name = L"uav9g";
			this->uav9g->Size = System::Drawing::Size(37, 20);
			this->uav9g->TabIndex = 10;
			// 
			// uav9r
			// 
			this->uav9r->Location = System::Drawing::Point(47, 42);
			this->uav9r->Name = L"uav9r";
			this->uav9r->Size = System::Drawing::Size(37, 20);
			this->uav9r->TabIndex = 9;
			// 
			// uav9a
			// 
			this->uav9a->Location = System::Drawing::Point(47, 19);
			this->uav9a->Name = L"uav9a";
			this->uav9a->Size = System::Drawing::Size(37, 20);
			this->uav9a->TabIndex = 8;
			// 
			// label60
			// 
			this->label60->AutoSize = true;
			this->label60->Location = System::Drawing::Point(7, 45);
			this->label60->Name = L"label60";
			this->label60->Size = System::Drawing::Size(27, 13);
			this->label60->TabIndex = 7;
			this->label60->Text = L"Red";
			// 
			// label61
			// 
			this->label61->AutoSize = true;
			this->label61->Location = System::Drawing::Point(7, 22);
			this->label61->Name = L"label61";
			this->label61->Size = System::Drawing::Size(34, 13);
			this->label61->TabIndex = 6;
			this->label61->Text = L"Alpha";
			// 
			// groupBox18
			// 
			this->groupBox18->Controls->Add(this->label54);
			this->groupBox18->Controls->Add(this->uav8b);
			this->groupBox18->Controls->Add(this->label55);
			this->groupBox18->Controls->Add(this->uav8g);
			this->groupBox18->Controls->Add(this->uav8r);
			this->groupBox18->Controls->Add(this->uav8a);
			this->groupBox18->Controls->Add(this->label56);
			this->groupBox18->Controls->Add(this->label57);
			this->groupBox18->Location = System::Drawing::Point(121, 295);
			this->groupBox18->Name = L"groupBox18";
			this->groupBox18->Size = System::Drawing::Size(94, 115);
			this->groupBox18->TabIndex = 14;
			this->groupBox18->TabStop = false;
			this->groupBox18->Text = L"UAV 8";
			// 
			// label54
			// 
			this->label54->AutoSize = true;
			this->label54->Location = System::Drawing::Point(7, 91);
			this->label54->Name = L"label54";
			this->label54->Size = System::Drawing::Size(28, 13);
			this->label54->TabIndex = 13;
			this->label54->Text = L"Blue";
			// 
			// uav8b
			// 
			this->uav8b->Location = System::Drawing::Point(47, 88);
			this->uav8b->Name = L"uav8b";
			this->uav8b->Size = System::Drawing::Size(37, 20);
			this->uav8b->TabIndex = 12;
			// 
			// label55
			// 
			this->label55->AutoSize = true;
			this->label55->Location = System::Drawing::Point(7, 68);
			this->label55->Name = L"label55";
			this->label55->Size = System::Drawing::Size(36, 13);
			this->label55->TabIndex = 11;
			this->label55->Text = L"Green";
			// 
			// uav8g
			// 
			this->uav8g->Location = System::Drawing::Point(47, 65);
			this->uav8g->Name = L"uav8g";
			this->uav8g->Size = System::Drawing::Size(37, 20);
			this->uav8g->TabIndex = 10;
			// 
			// uav8r
			// 
			this->uav8r->Location = System::Drawing::Point(47, 42);
			this->uav8r->Name = L"uav8r";
			this->uav8r->Size = System::Drawing::Size(37, 20);
			this->uav8r->TabIndex = 9;
			// 
			// uav8a
			// 
			this->uav8a->Location = System::Drawing::Point(47, 19);
			this->uav8a->Name = L"uav8a";
			this->uav8a->Size = System::Drawing::Size(37, 20);
			this->uav8a->TabIndex = 8;
			// 
			// label56
			// 
			this->label56->AutoSize = true;
			this->label56->Location = System::Drawing::Point(7, 45);
			this->label56->Name = L"label56";
			this->label56->Size = System::Drawing::Size(27, 13);
			this->label56->TabIndex = 7;
			this->label56->Text = L"Red";
			// 
			// label57
			// 
			this->label57->AutoSize = true;
			this->label57->Location = System::Drawing::Point(7, 22);
			this->label57->Name = L"label57";
			this->label57->Size = System::Drawing::Size(34, 13);
			this->label57->TabIndex = 6;
			this->label57->Text = L"Alpha";
			// 
			// groupBox17
			// 
			this->groupBox17->Controls->Add(this->label50);
			this->groupBox17->Controls->Add(this->uav7b);
			this->groupBox17->Controls->Add(this->label51);
			this->groupBox17->Controls->Add(this->uav7g);
			this->groupBox17->Controls->Add(this->uav7r);
			this->groupBox17->Controls->Add(this->uav7a);
			this->groupBox17->Controls->Add(this->label52);
			this->groupBox17->Controls->Add(this->label53);
			this->groupBox17->Location = System::Drawing::Point(13, 295);
			this->groupBox17->Name = L"groupBox17";
			this->groupBox17->Size = System::Drawing::Size(94, 115);
			this->groupBox17->TabIndex = 14;
			this->groupBox17->TabStop = false;
			this->groupBox17->Text = L"UAV 7";
			// 
			// label50
			// 
			this->label50->AutoSize = true;
			this->label50->Location = System::Drawing::Point(7, 91);
			this->label50->Name = L"label50";
			this->label50->Size = System::Drawing::Size(28, 13);
			this->label50->TabIndex = 13;
			this->label50->Text = L"Blue";
			// 
			// uav7b
			// 
			this->uav7b->Location = System::Drawing::Point(47, 88);
			this->uav7b->Name = L"uav7b";
			this->uav7b->Size = System::Drawing::Size(37, 20);
			this->uav7b->TabIndex = 12;
			// 
			// label51
			// 
			this->label51->AutoSize = true;
			this->label51->Location = System::Drawing::Point(7, 68);
			this->label51->Name = L"label51";
			this->label51->Size = System::Drawing::Size(36, 13);
			this->label51->TabIndex = 11;
			this->label51->Text = L"Green";
			// 
			// uav7g
			// 
			this->uav7g->Location = System::Drawing::Point(47, 65);
			this->uav7g->Name = L"uav7g";
			this->uav7g->Size = System::Drawing::Size(37, 20);
			this->uav7g->TabIndex = 10;
			// 
			// uav7r
			// 
			this->uav7r->Location = System::Drawing::Point(47, 42);
			this->uav7r->Name = L"uav7r";
			this->uav7r->Size = System::Drawing::Size(37, 20);
			this->uav7r->TabIndex = 9;
			// 
			// uav7a
			// 
			this->uav7a->Location = System::Drawing::Point(47, 19);
			this->uav7a->Name = L"uav7a";
			this->uav7a->Size = System::Drawing::Size(37, 20);
			this->uav7a->TabIndex = 8;
			// 
			// label52
			// 
			this->label52->AutoSize = true;
			this->label52->Location = System::Drawing::Point(7, 45);
			this->label52->Name = L"label52";
			this->label52->Size = System::Drawing::Size(27, 13);
			this->label52->TabIndex = 7;
			this->label52->Text = L"Red";
			// 
			// label53
			// 
			this->label53->AutoSize = true;
			this->label53->Location = System::Drawing::Point(7, 22);
			this->label53->Name = L"label53";
			this->label53->Size = System::Drawing::Size(34, 13);
			this->label53->TabIndex = 6;
			this->label53->Text = L"Alpha";
			// 
			// groupBox16
			// 
			this->groupBox16->Controls->Add(this->label46);
			this->groupBox16->Controls->Add(this->uav6b);
			this->groupBox16->Controls->Add(this->label47);
			this->groupBox16->Controls->Add(this->uav6g);
			this->groupBox16->Controls->Add(this->uav6r);
			this->groupBox16->Controls->Add(this->uav6a);
			this->groupBox16->Controls->Add(this->label48);
			this->groupBox16->Controls->Add(this->label49);
			this->groupBox16->Location = System::Drawing::Point(228, 174);
			this->groupBox16->Name = L"groupBox16";
			this->groupBox16->Size = System::Drawing::Size(94, 115);
			this->groupBox16->TabIndex = 14;
			this->groupBox16->TabStop = false;
			this->groupBox16->Text = L"UAV 6";
			// 
			// label46
			// 
			this->label46->AutoSize = true;
			this->label46->Location = System::Drawing::Point(7, 91);
			this->label46->Name = L"label46";
			this->label46->Size = System::Drawing::Size(28, 13);
			this->label46->TabIndex = 13;
			this->label46->Text = L"Blue";
			// 
			// uav6b
			// 
			this->uav6b->Location = System::Drawing::Point(47, 88);
			this->uav6b->Name = L"uav6b";
			this->uav6b->Size = System::Drawing::Size(37, 20);
			this->uav6b->TabIndex = 12;
			// 
			// label47
			// 
			this->label47->AutoSize = true;
			this->label47->Location = System::Drawing::Point(7, 68);
			this->label47->Name = L"label47";
			this->label47->Size = System::Drawing::Size(36, 13);
			this->label47->TabIndex = 11;
			this->label47->Text = L"Green";
			// 
			// uav6g
			// 
			this->uav6g->Location = System::Drawing::Point(47, 65);
			this->uav6g->Name = L"uav6g";
			this->uav6g->Size = System::Drawing::Size(37, 20);
			this->uav6g->TabIndex = 10;
			// 
			// uav6r
			// 
			this->uav6r->Location = System::Drawing::Point(47, 42);
			this->uav6r->Name = L"uav6r";
			this->uav6r->Size = System::Drawing::Size(37, 20);
			this->uav6r->TabIndex = 9;
			// 
			// uav6a
			// 
			this->uav6a->Location = System::Drawing::Point(47, 19);
			this->uav6a->Name = L"uav6a";
			this->uav6a->Size = System::Drawing::Size(37, 20);
			this->uav6a->TabIndex = 8;
			// 
			// label48
			// 
			this->label48->AutoSize = true;
			this->label48->Location = System::Drawing::Point(7, 45);
			this->label48->Name = L"label48";
			this->label48->Size = System::Drawing::Size(27, 13);
			this->label48->TabIndex = 7;
			this->label48->Text = L"Red";
			// 
			// label49
			// 
			this->label49->AutoSize = true;
			this->label49->Location = System::Drawing::Point(7, 22);
			this->label49->Name = L"label49";
			this->label49->Size = System::Drawing::Size(34, 13);
			this->label49->TabIndex = 6;
			this->label49->Text = L"Alpha";
			// 
			// groupBox15
			// 
			this->groupBox15->Controls->Add(this->label42);
			this->groupBox15->Controls->Add(this->uav5b);
			this->groupBox15->Controls->Add(this->label43);
			this->groupBox15->Controls->Add(this->uav5g);
			this->groupBox15->Controls->Add(this->uav5r);
			this->groupBox15->Controls->Add(this->uav5a);
			this->groupBox15->Controls->Add(this->label44);
			this->groupBox15->Controls->Add(this->label45);
			this->groupBox15->Location = System::Drawing::Point(121, 174);
			this->groupBox15->Name = L"groupBox15";
			this->groupBox15->Size = System::Drawing::Size(94, 115);
			this->groupBox15->TabIndex = 14;
			this->groupBox15->TabStop = false;
			this->groupBox15->Text = L"UAV 5";
			// 
			// label42
			// 
			this->label42->AutoSize = true;
			this->label42->Location = System::Drawing::Point(7, 91);
			this->label42->Name = L"label42";
			this->label42->Size = System::Drawing::Size(28, 13);
			this->label42->TabIndex = 13;
			this->label42->Text = L"Blue";
			// 
			// uav5b
			// 
			this->uav5b->Location = System::Drawing::Point(47, 88);
			this->uav5b->Name = L"uav5b";
			this->uav5b->Size = System::Drawing::Size(37, 20);
			this->uav5b->TabIndex = 12;
			// 
			// label43
			// 
			this->label43->AutoSize = true;
			this->label43->Location = System::Drawing::Point(7, 68);
			this->label43->Name = L"label43";
			this->label43->Size = System::Drawing::Size(36, 13);
			this->label43->TabIndex = 11;
			this->label43->Text = L"Green";
			// 
			// uav5g
			// 
			this->uav5g->Location = System::Drawing::Point(47, 65);
			this->uav5g->Name = L"uav5g";
			this->uav5g->Size = System::Drawing::Size(37, 20);
			this->uav5g->TabIndex = 10;
			// 
			// uav5r
			// 
			this->uav5r->Location = System::Drawing::Point(47, 42);
			this->uav5r->Name = L"uav5r";
			this->uav5r->Size = System::Drawing::Size(37, 20);
			this->uav5r->TabIndex = 9;
			// 
			// uav5a
			// 
			this->uav5a->Location = System::Drawing::Point(47, 19);
			this->uav5a->Name = L"uav5a";
			this->uav5a->Size = System::Drawing::Size(37, 20);
			this->uav5a->TabIndex = 8;
			// 
			// label44
			// 
			this->label44->AutoSize = true;
			this->label44->Location = System::Drawing::Point(7, 45);
			this->label44->Name = L"label44";
			this->label44->Size = System::Drawing::Size(27, 13);
			this->label44->TabIndex = 7;
			this->label44->Text = L"Red";
			// 
			// label45
			// 
			this->label45->AutoSize = true;
			this->label45->Location = System::Drawing::Point(7, 22);
			this->label45->Name = L"label45";
			this->label45->Size = System::Drawing::Size(34, 13);
			this->label45->TabIndex = 6;
			this->label45->Text = L"Alpha";
			// 
			// groupBox14
			// 
			this->groupBox14->Controls->Add(this->label38);
			this->groupBox14->Controls->Add(this->uav4b);
			this->groupBox14->Controls->Add(this->label39);
			this->groupBox14->Controls->Add(this->uav4g);
			this->groupBox14->Controls->Add(this->uav4r);
			this->groupBox14->Controls->Add(this->uav4a);
			this->groupBox14->Controls->Add(this->label40);
			this->groupBox14->Controls->Add(this->label41);
			this->groupBox14->Location = System::Drawing::Point(13, 174);
			this->groupBox14->Name = L"groupBox14";
			this->groupBox14->Size = System::Drawing::Size(94, 115);
			this->groupBox14->TabIndex = 15;
			this->groupBox14->TabStop = false;
			this->groupBox14->Text = L"UAV 4";
			// 
			// label38
			// 
			this->label38->AutoSize = true;
			this->label38->Location = System::Drawing::Point(7, 91);
			this->label38->Name = L"label38";
			this->label38->Size = System::Drawing::Size(28, 13);
			this->label38->TabIndex = 13;
			this->label38->Text = L"Blue";
			// 
			// uav4b
			// 
			this->uav4b->Location = System::Drawing::Point(47, 88);
			this->uav4b->Name = L"uav4b";
			this->uav4b->Size = System::Drawing::Size(37, 20);
			this->uav4b->TabIndex = 12;
			// 
			// label39
			// 
			this->label39->AutoSize = true;
			this->label39->Location = System::Drawing::Point(7, 68);
			this->label39->Name = L"label39";
			this->label39->Size = System::Drawing::Size(36, 13);
			this->label39->TabIndex = 11;
			this->label39->Text = L"Green";
			// 
			// uav4g
			// 
			this->uav4g->Location = System::Drawing::Point(47, 65);
			this->uav4g->Name = L"uav4g";
			this->uav4g->Size = System::Drawing::Size(37, 20);
			this->uav4g->TabIndex = 10;
			// 
			// uav4r
			// 
			this->uav4r->Location = System::Drawing::Point(47, 42);
			this->uav4r->Name = L"uav4r";
			this->uav4r->Size = System::Drawing::Size(37, 20);
			this->uav4r->TabIndex = 9;
			// 
			// uav4a
			// 
			this->uav4a->Location = System::Drawing::Point(47, 19);
			this->uav4a->Name = L"uav4a";
			this->uav4a->Size = System::Drawing::Size(37, 20);
			this->uav4a->TabIndex = 8;
			// 
			// label40
			// 
			this->label40->AutoSize = true;
			this->label40->Location = System::Drawing::Point(7, 45);
			this->label40->Name = L"label40";
			this->label40->Size = System::Drawing::Size(27, 13);
			this->label40->TabIndex = 7;
			this->label40->Text = L"Red";
			// 
			// label41
			// 
			this->label41->AutoSize = true;
			this->label41->Location = System::Drawing::Point(7, 22);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(34, 13);
			this->label41->TabIndex = 6;
			this->label41->Text = L"Alpha";
			// 
			// groupBox13
			// 
			this->groupBox13->Controls->Add(this->label34);
			this->groupBox13->Controls->Add(this->uav3b);
			this->groupBox13->Controls->Add(this->label35);
			this->groupBox13->Controls->Add(this->uav3g);
			this->groupBox13->Controls->Add(this->uav3r);
			this->groupBox13->Controls->Add(this->uav3a);
			this->groupBox13->Controls->Add(this->label36);
			this->groupBox13->Controls->Add(this->label37);
			this->groupBox13->Location = System::Drawing::Point(228, 53);
			this->groupBox13->Name = L"groupBox13";
			this->groupBox13->Size = System::Drawing::Size(94, 115);
			this->groupBox13->TabIndex = 14;
			this->groupBox13->TabStop = false;
			this->groupBox13->Text = L"UAV 3";
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(7, 91);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(28, 13);
			this->label34->TabIndex = 13;
			this->label34->Text = L"Blue";
			// 
			// uav3b
			// 
			this->uav3b->Location = System::Drawing::Point(47, 88);
			this->uav3b->Name = L"uav3b";
			this->uav3b->Size = System::Drawing::Size(37, 20);
			this->uav3b->TabIndex = 12;
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(7, 68);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(36, 13);
			this->label35->TabIndex = 11;
			this->label35->Text = L"Green";
			// 
			// uav3g
			// 
			this->uav3g->Location = System::Drawing::Point(47, 65);
			this->uav3g->Name = L"uav3g";
			this->uav3g->Size = System::Drawing::Size(37, 20);
			this->uav3g->TabIndex = 10;
			// 
			// uav3r
			// 
			this->uav3r->Location = System::Drawing::Point(47, 42);
			this->uav3r->Name = L"uav3r";
			this->uav3r->Size = System::Drawing::Size(37, 20);
			this->uav3r->TabIndex = 9;
			// 
			// uav3a
			// 
			this->uav3a->Location = System::Drawing::Point(47, 19);
			this->uav3a->Name = L"uav3a";
			this->uav3a->Size = System::Drawing::Size(37, 20);
			this->uav3a->TabIndex = 8;
			// 
			// label36
			// 
			this->label36->AutoSize = true;
			this->label36->Location = System::Drawing::Point(7, 45);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(27, 13);
			this->label36->TabIndex = 7;
			this->label36->Text = L"Red";
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Location = System::Drawing::Point(7, 22);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(34, 13);
			this->label37->TabIndex = 6;
			this->label37->Text = L"Alpha";
			// 
			// groupBox12
			// 
			this->groupBox12->Controls->Add(this->label30);
			this->groupBox12->Controls->Add(this->uav2b);
			this->groupBox12->Controls->Add(this->label31);
			this->groupBox12->Controls->Add(this->uav2g);
			this->groupBox12->Controls->Add(this->uav2r);
			this->groupBox12->Controls->Add(this->uav2a);
			this->groupBox12->Controls->Add(this->label32);
			this->groupBox12->Controls->Add(this->label33);
			this->groupBox12->Location = System::Drawing::Point(121, 53);
			this->groupBox12->Name = L"groupBox12";
			this->groupBox12->Size = System::Drawing::Size(94, 115);
			this->groupBox12->TabIndex = 14;
			this->groupBox12->TabStop = false;
			this->groupBox12->Text = L"UAV 2";
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(7, 91);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(28, 13);
			this->label30->TabIndex = 13;
			this->label30->Text = L"Blue";
			// 
			// uav2b
			// 
			this->uav2b->Location = System::Drawing::Point(47, 88);
			this->uav2b->Name = L"uav2b";
			this->uav2b->Size = System::Drawing::Size(37, 20);
			this->uav2b->TabIndex = 12;
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(7, 68);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(36, 13);
			this->label31->TabIndex = 11;
			this->label31->Text = L"Green";
			// 
			// uav2g
			// 
			this->uav2g->Location = System::Drawing::Point(47, 65);
			this->uav2g->Name = L"uav2g";
			this->uav2g->Size = System::Drawing::Size(37, 20);
			this->uav2g->TabIndex = 10;
			// 
			// uav2r
			// 
			this->uav2r->Location = System::Drawing::Point(47, 42);
			this->uav2r->Name = L"uav2r";
			this->uav2r->Size = System::Drawing::Size(37, 20);
			this->uav2r->TabIndex = 9;
			// 
			// uav2a
			// 
			this->uav2a->Location = System::Drawing::Point(47, 19);
			this->uav2a->Name = L"uav2a";
			this->uav2a->Size = System::Drawing::Size(37, 20);
			this->uav2a->TabIndex = 8;
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(7, 45);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(27, 13);
			this->label32->TabIndex = 7;
			this->label32->Text = L"Red";
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(7, 22);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(34, 13);
			this->label33->TabIndex = 6;
			this->label33->Text = L"Alpha";
			// 
			// groupBox11
			// 
			this->groupBox11->Controls->Add(this->label29);
			this->groupBox11->Controls->Add(this->uav1b);
			this->groupBox11->Controls->Add(this->label28);
			this->groupBox11->Controls->Add(this->uav1g);
			this->groupBox11->Controls->Add(this->uav1r);
			this->groupBox11->Controls->Add(this->uav1a);
			this->groupBox11->Controls->Add(this->label27);
			this->groupBox11->Controls->Add(this->label26);
			this->groupBox11->Location = System::Drawing::Point(13, 53);
			this->groupBox11->Name = L"groupBox11";
			this->groupBox11->Size = System::Drawing::Size(94, 115);
			this->groupBox11->TabIndex = 3;
			this->groupBox11->TabStop = false;
			this->groupBox11->Text = L"UAV 1";
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(7, 91);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(28, 13);
			this->label29->TabIndex = 13;
			this->label29->Text = L"Blue";
			// 
			// uav1b
			// 
			this->uav1b->Location = System::Drawing::Point(47, 88);
			this->uav1b->Name = L"uav1b";
			this->uav1b->Size = System::Drawing::Size(37, 20);
			this->uav1b->TabIndex = 12;
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(7, 68);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(36, 13);
			this->label28->TabIndex = 11;
			this->label28->Text = L"Green";
			// 
			// uav1g
			// 
			this->uav1g->Location = System::Drawing::Point(47, 65);
			this->uav1g->Name = L"uav1g";
			this->uav1g->Size = System::Drawing::Size(37, 20);
			this->uav1g->TabIndex = 10;
			// 
			// uav1r
			// 
			this->uav1r->Location = System::Drawing::Point(47, 42);
			this->uav1r->Name = L"uav1r";
			this->uav1r->Size = System::Drawing::Size(37, 20);
			this->uav1r->TabIndex = 9;
			// 
			// uav1a
			// 
			this->uav1a->Location = System::Drawing::Point(47, 19);
			this->uav1a->Name = L"uav1a";
			this->uav1a->Size = System::Drawing::Size(37, 20);
			this->uav1a->TabIndex = 8;
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(7, 45);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(27, 13);
			this->label27->TabIndex = 7;
			this->label27->Text = L"Red";
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(7, 22);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(34, 13);
			this->label26->TabIndex = 6;
			this->label26->Text = L"Alpha";
			// 
			// tabTactors
			// 
			this->tabTactors->Controls->Add(this->groupBox3);
			this->tabTactors->Controls->Add(this->groupBox20);
			this->tabTactors->Controls->Add(this->groupBox10);
			this->tabTactors->Location = System::Drawing::Point(4, 40);
			this->tabTactors->Name = L"tabTactors";
			this->tabTactors->Size = System::Drawing::Size(335, 458);
			this->tabTactors->TabIndex = 4;
			this->tabTactors->Text = L"Tactor System";
			this->tabTactors->UseVisualStyleBackColor = true;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->tactor2Source);
			this->groupBox3->Controls->Add(this->label69);
			this->groupBox3->Controls->Add(this->tactor2Duration);
			this->groupBox3->Controls->Add(this->tactor2Freq2);
			this->groupBox3->Controls->Add(this->tactor2Freq1);
			this->groupBox3->Controls->Add(this->tactor2Dist);
			this->groupBox3->Controls->Add(this->label20);
			this->groupBox3->Controls->Add(this->tactor2Pulse);
			this->groupBox3->Controls->Add(this->tactor2PulseInterval);
			this->groupBox3->Controls->Add(this->label21);
			this->groupBox3->Controls->Add(this->label22);
			this->groupBox3->Controls->Add(this->label67);
			this->groupBox3->Controls->Add(this->label68);
			this->groupBox3->Location = System::Drawing::Point(13, 137);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(310, 153);
			this->groupBox3->TabIndex = 1;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Low Signal";
			// 
			// tactor2Source
			// 
			this->tactor2Source->FormattingEnabled = true;
			this->tactor2Source->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"FREQ1", L"FREQ2", L"MIXED"});
			this->tactor2Source->Location = System::Drawing::Point(245, 36);
			this->tactor2Source->Name = L"tactor2Source";
			this->tactor2Source->Size = System::Drawing::Size(59, 21);
			this->tactor2Source->TabIndex = 6;
			// 
			// label69
			// 
			this->label69->AutoSize = true;
			this->label69->Location = System::Drawing::Point(172, 39);
			this->label69->Name = L"label69";
			this->label69->Size = System::Drawing::Size(73, 13);
			this->label69->TabIndex = 10;
			this->label69->Text = L"Signal Source";
			// 
			// tactor2Duration
			// 
			this->tactor2Duration->Location = System::Drawing::Point(120, 74);
			this->tactor2Duration->Name = L"tactor2Duration";
			this->tactor2Duration->Size = System::Drawing::Size(46, 20);
			this->tactor2Duration->TabIndex = 7;
			// 
			// tactor2Freq2
			// 
			this->tactor2Freq2->Location = System::Drawing::Point(120, 48);
			this->tactor2Freq2->Name = L"tactor2Freq2";
			this->tactor2Freq2->Size = System::Drawing::Size(46, 20);
			this->tactor2Freq2->TabIndex = 5;
			// 
			// tactor2Freq1
			// 
			this->tactor2Freq1->Location = System::Drawing::Point(120, 22);
			this->tactor2Freq1->Name = L"tactor2Freq1";
			this->tactor2Freq1->Size = System::Drawing::Size(46, 20);
			this->tactor2Freq1->TabIndex = 4;
			// 
			// tactor2Dist
			// 
			this->tactor2Dist->Location = System::Drawing::Point(120, 100);
			this->tactor2Dist->Name = L"tactor2Dist";
			this->tactor2Dist->Size = System::Drawing::Size(46, 20);
			this->tactor2Dist->TabIndex = 8;
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(7, 103);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(111, 13);
			this->label20->TabIndex = 9;
			this->label20->Text = L"Active Distance (float)";
			// 
			// tactor2Pulse
			// 
			this->tactor2Pulse->AutoSize = true;
			this->tactor2Pulse->Location = System::Drawing::Point(9, 128);
			this->tactor2Pulse->Name = L"tactor2Pulse";
			this->tactor2Pulse->Size = System::Drawing::Size(84, 17);
			this->tactor2Pulse->TabIndex = 9;
			this->tactor2Pulse->Text = L"Pulse Signal";
			this->tactor2Pulse->UseVisualStyleBackColor = true;
			// 
			// tactor2PulseInterval
			// 
			this->tactor2PulseInterval->Location = System::Drawing::Point(245, 126);
			this->tactor2PulseInterval->Name = L"tactor2PulseInterval";
			this->tactor2PulseInterval->Size = System::Drawing::Size(59, 20);
			this->tactor2PulseInterval->TabIndex = 10;
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(109, 129);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(136, 13);
			this->label21->TabIndex = 6;
			this->label21->Text = L"Pulse Interval (milliseconds)";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(7, 77);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(112, 13);
			this->label22->TabIndex = 5;
			this->label22->Text = L"Duration (milliseconds)";
			// 
			// label67
			// 
			this->label67->AutoSize = true;
			this->label67->Location = System::Drawing::Point(6, 51);
			this->label67->Name = L"label67";
			this->label67->Size = System::Drawing::Size(107, 13);
			this->label67->TabIndex = 2;
			this->label67->Text = L"Frequency 2 (integer)";
			// 
			// label68
			// 
			this->label68->AutoSize = true;
			this->label68->Location = System::Drawing::Point(7, 25);
			this->label68->Name = L"label68";
			this->label68->Size = System::Drawing::Size(107, 13);
			this->label68->TabIndex = 0;
			this->label68->Text = L"Frequency 1 (integer)";
			// 
			// groupBox20
			// 
			this->groupBox20->Controls->Add(this->tactor1Source);
			this->groupBox20->Controls->Add(this->label70);
			this->groupBox20->Controls->Add(this->tactor1Duration);
			this->groupBox20->Controls->Add(this->tactor1Freq2);
			this->groupBox20->Controls->Add(this->tactor1Freq1);
			this->groupBox20->Controls->Add(this->tactor1Dist);
			this->groupBox20->Controls->Add(this->label71);
			this->groupBox20->Controls->Add(this->tactor1Pulse);
			this->groupBox20->Controls->Add(this->tactor1PulseInterval);
			this->groupBox20->Controls->Add(this->label66);
			this->groupBox20->Controls->Add(this->label65);
			this->groupBox20->Controls->Add(this->label64);
			this->groupBox20->Controls->Add(this->label63);
			this->groupBox20->Location = System::Drawing::Point(13, 296);
			this->groupBox20->Name = L"groupBox20";
			this->groupBox20->Size = System::Drawing::Size(310, 153);
			this->groupBox20->TabIndex = 2;
			this->groupBox20->TabStop = false;
			this->groupBox20->Text = L"High Signal";
			// 
			// tactor1Source
			// 
			this->tactor1Source->FormattingEnabled = true;
			this->tactor1Source->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"FREQ1", L"FREQ2", L"MIXED"});
			this->tactor1Source->Location = System::Drawing::Point(245, 35);
			this->tactor1Source->Name = L"tactor1Source";
			this->tactor1Source->Size = System::Drawing::Size(59, 21);
			this->tactor1Source->TabIndex = 12;
			// 
			// label70
			// 
			this->label70->AutoSize = true;
			this->label70->Location = System::Drawing::Point(172, 38);
			this->label70->Name = L"label70";
			this->label70->Size = System::Drawing::Size(73, 13);
			this->label70->TabIndex = 16;
			this->label70->Text = L"Signal Source";
			// 
			// tactor1Duration
			// 
			this->tactor1Duration->Location = System::Drawing::Point(120, 74);
			this->tactor1Duration->Name = L"tactor1Duration";
			this->tactor1Duration->Size = System::Drawing::Size(46, 20);
			this->tactor1Duration->TabIndex = 13;
			// 
			// tactor1Freq2
			// 
			this->tactor1Freq2->Location = System::Drawing::Point(120, 48);
			this->tactor1Freq2->Name = L"tactor1Freq2";
			this->tactor1Freq2->Size = System::Drawing::Size(46, 20);
			this->tactor1Freq2->TabIndex = 11;
			// 
			// tactor1Freq1
			// 
			this->tactor1Freq1->Location = System::Drawing::Point(120, 22);
			this->tactor1Freq1->Name = L"tactor1Freq1";
			this->tactor1Freq1->Size = System::Drawing::Size(46, 20);
			this->tactor1Freq1->TabIndex = 10;
			// 
			// tactor1Dist
			// 
			this->tactor1Dist->Location = System::Drawing::Point(120, 100);
			this->tactor1Dist->Name = L"tactor1Dist";
			this->tactor1Dist->Size = System::Drawing::Size(46, 20);
			this->tactor1Dist->TabIndex = 14;
			// 
			// label71
			// 
			this->label71->AutoSize = true;
			this->label71->Location = System::Drawing::Point(7, 103);
			this->label71->Name = L"label71";
			this->label71->Size = System::Drawing::Size(111, 13);
			this->label71->TabIndex = 9;
			this->label71->Text = L"Active Distance (float)";
			// 
			// tactor1Pulse
			// 
			this->tactor1Pulse->AutoSize = true;
			this->tactor1Pulse->Location = System::Drawing::Point(9, 128);
			this->tactor1Pulse->Name = L"tactor1Pulse";
			this->tactor1Pulse->Size = System::Drawing::Size(84, 17);
			this->tactor1Pulse->TabIndex = 15;
			this->tactor1Pulse->Text = L"Pulse Signal";
			this->tactor1Pulse->UseVisualStyleBackColor = true;
			// 
			// tactor1PulseInterval
			// 
			this->tactor1PulseInterval->Location = System::Drawing::Point(245, 125);
			this->tactor1PulseInterval->Name = L"tactor1PulseInterval";
			this->tactor1PulseInterval->Size = System::Drawing::Size(59, 20);
			this->tactor1PulseInterval->TabIndex = 16;
			// 
			// label66
			// 
			this->label66->AutoSize = true;
			this->label66->Location = System::Drawing::Point(109, 128);
			this->label66->Name = L"label66";
			this->label66->Size = System::Drawing::Size(136, 13);
			this->label66->TabIndex = 6;
			this->label66->Text = L"Pulse Interval (milliseconds)";
			// 
			// label65
			// 
			this->label65->AutoSize = true;
			this->label65->Location = System::Drawing::Point(7, 77);
			this->label65->Name = L"label65";
			this->label65->Size = System::Drawing::Size(112, 13);
			this->label65->TabIndex = 5;
			this->label65->Text = L"Duration (milliseconds)";
			// 
			// label64
			// 
			this->label64->AutoSize = true;
			this->label64->Location = System::Drawing::Point(6, 51);
			this->label64->Name = L"label64";
			this->label64->Size = System::Drawing::Size(107, 13);
			this->label64->TabIndex = 2;
			this->label64->Text = L"Frequency 2 (integer)";
			// 
			// label63
			// 
			this->label63->AutoSize = true;
			this->label63->Location = System::Drawing::Point(7, 25);
			this->label63->Name = L"label63";
			this->label63->Size = System::Drawing::Size(107, 13);
			this->label63->TabIndex = 0;
			this->label63->Text = L"Frequency 1 (integer)";
			// 
			// groupBox10
			// 
			this->groupBox10->Controls->Add(this->label96);
			this->groupBox10->Controls->Add(this->label93);
			this->groupBox10->Controls->Add(this->tactorFrequency);
			this->groupBox10->Controls->Add(this->label92);
			this->groupBox10->Controls->Add(this->tactorComPort5);
			this->groupBox10->Controls->Add(this->label95);
			this->groupBox10->Controls->Add(this->tactorComPort3);
			this->groupBox10->Controls->Add(this->label94);
			this->groupBox10->Controls->Add(this->tactorComPort6);
			this->groupBox10->Controls->Add(this->tactorComPort4);
			this->groupBox10->Controls->Add(this->tactorComPort2);
			this->groupBox10->Controls->Add(this->label91);
			this->groupBox10->Controls->Add(this->runTactorTest);
			this->groupBox10->Controls->Add(this->tactorGain);
			this->groupBox10->Controls->Add(this->tactorComPort1);
			this->groupBox10->Controls->Add(this->useTactorCues);
			this->groupBox10->Controls->Add(this->label62);
			this->groupBox10->Controls->Add(this->label25);
			this->groupBox10->Location = System::Drawing::Point(13, 15);
			this->groupBox10->Name = L"groupBox10";
			this->groupBox10->Size = System::Drawing::Size(310, 116);
			this->groupBox10->TabIndex = 0;
			this->groupBox10->TabStop = false;
			this->groupBox10->Text = L"General";
			// 
			// tactorComPort2
			// 
			this->tactorComPort2->Location = System::Drawing::Point(75, 39);
			this->tactorComPort2->Name = L"tactorComPort2";
			this->tactorComPort2->Size = System::Drawing::Size(47, 20);
			this->tactorComPort2->TabIndex = 6;
			// 
			// label91
			// 
			this->label91->AutoSize = true;
			this->label91->Location = System::Drawing::Point(7, 42);
			this->label91->Name = L"label91";
			this->label91->Size = System::Drawing::Size(62, 13);
			this->label91->TabIndex = 5;
			this->label91->Text = L"COM Port 2";
			// 
			// runTactorTest
			// 
			this->runTactorTest->AutoSize = true;
			this->runTactorTest->Location = System::Drawing::Point(167, 90);
			this->runTactorTest->Name = L"runTactorTest";
			this->runTactorTest->Size = System::Drawing::Size(116, 17);
			this->runTactorTest->TabIndex = 4;
			this->runTactorTest->Text = L"Run test on startup";
			this->runTactorTest->UseVisualStyleBackColor = true;
			// 
			// tactorGain
			// 
			this->tactorGain->Location = System::Drawing::Point(92, 65);
			this->tactorGain->Name = L"tactorGain";
			this->tactorGain->Size = System::Drawing::Size(43, 20);
			this->tactorGain->TabIndex = 3;
			// 
			// tactorComPort1
			// 
			this->tactorComPort1->Location = System::Drawing::Point(75, 13);
			this->tactorComPort1->Name = L"tactorComPort1";
			this->tactorComPort1->Size = System::Drawing::Size(47, 20);
			this->tactorComPort1->TabIndex = 2;
			// 
			// useTactorCues
			// 
			this->useTactorCues->AutoSize = true;
			this->useTactorCues->Location = System::Drawing::Point(167, 71);
			this->useTactorCues->Name = L"useTactorCues";
			this->useTactorCues->Size = System::Drawing::Size(106, 17);
			this->useTactorCues->TabIndex = 1;
			this->useTactorCues->Text = L"Use Tactor Cues";
			this->useTactorCues->UseVisualStyleBackColor = true;
			// 
			// label62
			// 
			this->label62->AutoSize = true;
			this->label62->Location = System::Drawing::Point(7, 68);
			this->label62->Name = L"label62";
			this->label62->Size = System::Drawing::Size(70, 13);
			this->label62->TabIndex = 3;
			this->label62->Text = L"Gain (integer)";
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(7, 16);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(62, 13);
			this->label25->TabIndex = 0;
			this->label25->Text = L"COM Port 1";
			// 
			// tabFeatures
			// 
			this->tabFeatures->Controls->Add(this->groupBox24);
			this->tabFeatures->Controls->Add(this->groupBox23);
			this->tabFeatures->Controls->Add(this->groupBox22);
			this->tabFeatures->Controls->Add(this->groupBox21);
			this->tabFeatures->Controls->Add(this->label73);
			this->tabFeatures->Location = System::Drawing::Point(4, 40);
			this->tabFeatures->Name = L"tabFeatures";
			this->tabFeatures->Size = System::Drawing::Size(335, 416);
			this->tabFeatures->TabIndex = 5;
			this->tabFeatures->Text = L"Features";
			this->tabFeatures->UseVisualStyleBackColor = true;
			// 
			// groupBox24
			// 
			this->groupBox24->Controls->Add(this->lightAfterPass);
			this->groupBox24->Controls->Add(this->featureMissedDist);
			this->groupBox24->Controls->Add(this->label86);
			this->groupBox24->Controls->Add(this->featureVisibleDist);
			this->groupBox24->Controls->Add(this->lblwhatever);
			this->groupBox24->Location = System::Drawing::Point(17, 174);
			this->groupBox24->Name = L"groupBox24";
			this->groupBox24->Size = System::Drawing::Size(305, 101);
			this->groupBox24->TabIndex = 19;
			this->groupBox24->TabStop = false;
			this->groupBox24->Text = L"Delayed Reporting";
			// 
			// lightAfterPass
			// 
			this->lightAfterPass->AutoSize = true;
			this->lightAfterPass->Location = System::Drawing::Point(10, 71);
			this->lightAfterPass->Name = L"lightAfterPass";
			this->lightAfterPass->Size = System::Drawing::Size(213, 17);
			this->lightAfterPass->TabIndex = 21;
			this->lightAfterPass->Text = L"Keep View Lit After UAV Passed Target";
			this->lightAfterPass->UseVisualStyleBackColor = true;
			// 
			// featureMissedDist
			// 
			this->featureMissedDist->Location = System::Drawing::Point(130, 45);
			this->featureMissedDist->Name = L"featureMissedDist";
			this->featureMissedDist->Size = System::Drawing::Size(58, 20);
			this->featureMissedDist->TabIndex = 8;
			// 
			// label86
			// 
			this->label86->AutoSize = true;
			this->label86->Location = System::Drawing::Point(7, 48);
			this->label86->Name = L"label86";
			this->label86->Size = System::Drawing::Size(114, 13);
			this->label86->TabIndex = 7;
			this->label86->Text = L"Missed Distance (float)";
			// 
			// featureVisibleDist
			// 
			this->featureVisibleDist->Location = System::Drawing::Point(130, 19);
			this->featureVisibleDist->Name = L"featureVisibleDist";
			this->featureVisibleDist->Size = System::Drawing::Size(58, 20);
			this->featureVisibleDist->TabIndex = 1;
			// 
			// lblwhatever
			// 
			this->lblwhatever->AutoSize = true;
			this->lblwhatever->Location = System::Drawing::Point(7, 22);
			this->lblwhatever->Name = L"lblwhatever";
			this->lblwhatever->Size = System::Drawing::Size(111, 13);
			this->lblwhatever->TabIndex = 6;
			this->lblwhatever->Text = L"Visible Distance (float)";
			// 
			// groupBox23
			// 
			this->groupBox23->Controls->Add(this->label82);
			this->groupBox23->Controls->Add(this->fub);
			this->groupBox23->Controls->Add(this->label83);
			this->groupBox23->Controls->Add(this->fug);
			this->groupBox23->Controls->Add(this->fur);
			this->groupBox23->Controls->Add(this->fua);
			this->groupBox23->Controls->Add(this->label84);
			this->groupBox23->Controls->Add(this->label85);
			this->groupBox23->Location = System::Drawing::Point(228, 53);
			this->groupBox23->Name = L"groupBox23";
			this->groupBox23->Size = System::Drawing::Size(94, 115);
			this->groupBox23->TabIndex = 20;
			this->groupBox23->TabStop = false;
			this->groupBox23->Text = L"Unsure";
			// 
			// label82
			// 
			this->label82->AutoSize = true;
			this->label82->Location = System::Drawing::Point(7, 91);
			this->label82->Name = L"label82";
			this->label82->Size = System::Drawing::Size(28, 13);
			this->label82->TabIndex = 13;
			this->label82->Text = L"Blue";
			// 
			// fub
			// 
			this->fub->Location = System::Drawing::Point(47, 88);
			this->fub->Name = L"fub";
			this->fub->Size = System::Drawing::Size(37, 20);
			this->fub->TabIndex = 4;
			// 
			// label83
			// 
			this->label83->AutoSize = true;
			this->label83->Location = System::Drawing::Point(7, 68);
			this->label83->Name = L"label83";
			this->label83->Size = System::Drawing::Size(36, 13);
			this->label83->TabIndex = 11;
			this->label83->Text = L"Green";
			// 
			// fug
			// 
			this->fug->Location = System::Drawing::Point(47, 65);
			this->fug->Name = L"fug";
			this->fug->Size = System::Drawing::Size(37, 20);
			this->fug->TabIndex = 3;
			// 
			// fur
			// 
			this->fur->Location = System::Drawing::Point(47, 42);
			this->fur->Name = L"fur";
			this->fur->Size = System::Drawing::Size(37, 20);
			this->fur->TabIndex = 2;
			// 
			// fua
			// 
			this->fua->Location = System::Drawing::Point(47, 19);
			this->fua->Name = L"fua";
			this->fua->Size = System::Drawing::Size(37, 20);
			this->fua->TabIndex = 1;
			// 
			// label84
			// 
			this->label84->AutoSize = true;
			this->label84->Location = System::Drawing::Point(7, 45);
			this->label84->Name = L"label84";
			this->label84->Size = System::Drawing::Size(27, 13);
			this->label84->TabIndex = 7;
			this->label84->Text = L"Red";
			// 
			// label85
			// 
			this->label85->AutoSize = true;
			this->label85->Location = System::Drawing::Point(7, 22);
			this->label85->Name = L"label85";
			this->label85->Size = System::Drawing::Size(34, 13);
			this->label85->TabIndex = 6;
			this->label85->Text = L"Alpha";
			// 
			// groupBox22
			// 
			this->groupBox22->Controls->Add(this->label78);
			this->groupBox22->Controls->Add(this->fdb);
			this->groupBox22->Controls->Add(this->label79);
			this->groupBox22->Controls->Add(this->fdg);
			this->groupBox22->Controls->Add(this->fdr);
			this->groupBox22->Controls->Add(this->fda);
			this->groupBox22->Controls->Add(this->label80);
			this->groupBox22->Controls->Add(this->label81);
			this->groupBox22->Location = System::Drawing::Point(121, 53);
			this->groupBox22->Name = L"groupBox22";
			this->groupBox22->Size = System::Drawing::Size(94, 115);
			this->groupBox22->TabIndex = 19;
			this->groupBox22->TabStop = false;
			this->groupBox22->Text = L"Denied";
			// 
			// label78
			// 
			this->label78->AutoSize = true;
			this->label78->Location = System::Drawing::Point(7, 91);
			this->label78->Name = L"label78";
			this->label78->Size = System::Drawing::Size(28, 13);
			this->label78->TabIndex = 13;
			this->label78->Text = L"Blue";
			// 
			// fdb
			// 
			this->fdb->Location = System::Drawing::Point(47, 88);
			this->fdb->Name = L"fdb";
			this->fdb->Size = System::Drawing::Size(37, 20);
			this->fdb->TabIndex = 4;
			// 
			// label79
			// 
			this->label79->AutoSize = true;
			this->label79->Location = System::Drawing::Point(7, 68);
			this->label79->Name = L"label79";
			this->label79->Size = System::Drawing::Size(36, 13);
			this->label79->TabIndex = 11;
			this->label79->Text = L"Green";
			// 
			// fdg
			// 
			this->fdg->Location = System::Drawing::Point(47, 65);
			this->fdg->Name = L"fdg";
			this->fdg->Size = System::Drawing::Size(37, 20);
			this->fdg->TabIndex = 3;
			// 
			// fdr
			// 
			this->fdr->Location = System::Drawing::Point(47, 42);
			this->fdr->Name = L"fdr";
			this->fdr->Size = System::Drawing::Size(37, 20);
			this->fdr->TabIndex = 2;
			// 
			// fda
			// 
			this->fda->Location = System::Drawing::Point(47, 19);
			this->fda->Name = L"fda";
			this->fda->Size = System::Drawing::Size(37, 20);
			this->fda->TabIndex = 1;
			// 
			// label80
			// 
			this->label80->AutoSize = true;
			this->label80->Location = System::Drawing::Point(7, 45);
			this->label80->Name = L"label80";
			this->label80->Size = System::Drawing::Size(27, 13);
			this->label80->TabIndex = 7;
			this->label80->Text = L"Red";
			// 
			// label81
			// 
			this->label81->AutoSize = true;
			this->label81->Location = System::Drawing::Point(7, 22);
			this->label81->Name = L"label81";
			this->label81->Size = System::Drawing::Size(34, 13);
			this->label81->TabIndex = 6;
			this->label81->Text = L"Alpha";
			// 
			// groupBox21
			// 
			this->groupBox21->Controls->Add(this->label74);
			this->groupBox21->Controls->Add(this->fcb);
			this->groupBox21->Controls->Add(this->label75);
			this->groupBox21->Controls->Add(this->fcg);
			this->groupBox21->Controls->Add(this->fcr);
			this->groupBox21->Controls->Add(this->fca);
			this->groupBox21->Controls->Add(this->label76);
			this->groupBox21->Controls->Add(this->label77);
			this->groupBox21->Location = System::Drawing::Point(13, 53);
			this->groupBox21->Name = L"groupBox21";
			this->groupBox21->Size = System::Drawing::Size(94, 115);
			this->groupBox21->TabIndex = 18;
			this->groupBox21->TabStop = false;
			this->groupBox21->Text = L"Confirmed";
			// 
			// label74
			// 
			this->label74->AutoSize = true;
			this->label74->Location = System::Drawing::Point(7, 91);
			this->label74->Name = L"label74";
			this->label74->Size = System::Drawing::Size(28, 13);
			this->label74->TabIndex = 13;
			this->label74->Text = L"Blue";
			// 
			// fcb
			// 
			this->fcb->Location = System::Drawing::Point(47, 88);
			this->fcb->Name = L"fcb";
			this->fcb->Size = System::Drawing::Size(37, 20);
			this->fcb->TabIndex = 4;
			// 
			// label75
			// 
			this->label75->AutoSize = true;
			this->label75->Location = System::Drawing::Point(7, 68);
			this->label75->Name = L"label75";
			this->label75->Size = System::Drawing::Size(36, 13);
			this->label75->TabIndex = 11;
			this->label75->Text = L"Green";
			// 
			// fcg
			// 
			this->fcg->Location = System::Drawing::Point(47, 65);
			this->fcg->Name = L"fcg";
			this->fcg->Size = System::Drawing::Size(37, 20);
			this->fcg->TabIndex = 3;
			// 
			// fcr
			// 
			this->fcr->Location = System::Drawing::Point(47, 42);
			this->fcr->Name = L"fcr";
			this->fcr->Size = System::Drawing::Size(37, 20);
			this->fcr->TabIndex = 2;
			// 
			// fca
			// 
			this->fca->Location = System::Drawing::Point(47, 19);
			this->fca->Name = L"fca";
			this->fca->Size = System::Drawing::Size(37, 20);
			this->fca->TabIndex = 1;
			// 
			// label76
			// 
			this->label76->AutoSize = true;
			this->label76->Location = System::Drawing::Point(7, 45);
			this->label76->Name = L"label76";
			this->label76->Size = System::Drawing::Size(27, 13);
			this->label76->TabIndex = 7;
			this->label76->Text = L"Red";
			// 
			// label77
			// 
			this->label77->AutoSize = true;
			this->label77->Location = System::Drawing::Point(7, 22);
			this->label77->Name = L"label77";
			this->label77->Size = System::Drawing::Size(34, 13);
			this->label77->TabIndex = 6;
			this->label77->Text = L"Alpha";
			// 
			// label73
			// 
			this->label73->AutoSize = true;
			this->label73->Location = System::Drawing::Point(14, 11);
			this->label73->Name = L"label73";
			this->label73->Size = System::Drawing::Size(286, 39);
			this->label73->TabIndex = 17;
			this->label73->Text = L"Colors consist of red, green, and blue values. The level of\r\ntransparency is dete" 
				L"rmined by the alpha. All of these values\r\nrange between 0 and 255.";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(18, 67);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(120, 13);
			this->label9->TabIndex = 0;
			this->label9->Text = L"High Light Level (0-255)";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(18, 95);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(135, 13);
			this->label10->TabIndex = 2;
			this->label10->Text = L"Medium Light Level (0-255)";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(18, 118);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(118, 13);
			this->label11->TabIndex = 1;
			this->label11->Text = L"Low Light Level (0-255)";
			// 
			// tactorComPort4
			// 
			this->tactorComPort4->Location = System::Drawing::Point(167, 39);
			this->tactorComPort4->Name = L"tactorComPort4";
			this->tactorComPort4->Size = System::Drawing::Size(47, 20);
			this->tactorComPort4->TabIndex = 8;
			// 
			// tactorComPort6
			// 
			this->tactorComPort6->Location = System::Drawing::Point(257, 39);
			this->tactorComPort6->Name = L"tactorComPort6";
			this->tactorComPort6->Size = System::Drawing::Size(47, 20);
			this->tactorComPort6->TabIndex = 10;
			// 
			// label94
			// 
			this->label94->AutoSize = true;
			this->label94->Location = System::Drawing::Point(129, 16);
			this->label94->Name = L"label94";
			this->label94->Size = System::Drawing::Size(35, 13);
			this->label94->TabIndex = 11;
			this->label94->Text = L"Port 3";
			// 
			// tactorComPort3
			// 
			this->tactorComPort3->Location = System::Drawing::Point(167, 13);
			this->tactorComPort3->Name = L"tactorComPort3";
			this->tactorComPort3->Size = System::Drawing::Size(47, 20);
			this->tactorComPort3->TabIndex = 12;
			// 
			// label95
			// 
			this->label95->AutoSize = true;
			this->label95->Location = System::Drawing::Point(222, 16);
			this->label95->Name = L"label95";
			this->label95->Size = System::Drawing::Size(35, 13);
			this->label95->TabIndex = 13;
			this->label95->Text = L"Port 5";
			// 
			// tactorComPort5
			// 
			this->tactorComPort5->Location = System::Drawing::Point(257, 13);
			this->tactorComPort5->Name = L"tactorComPort5";
			this->tactorComPort5->Size = System::Drawing::Size(47, 20);
			this->tactorComPort5->TabIndex = 14;
			// 
			// label92
			// 
			this->label92->AutoSize = true;
			this->label92->Location = System::Drawing::Point(7, 94);
			this->label92->Name = L"label92";
			this->label92->Size = System::Drawing::Size(77, 13);
			this->label92->TabIndex = 15;
			this->label92->Text = L"Frequency (int)";
			// 
			// tactorFrequency
			// 
			this->tactorFrequency->Location = System::Drawing::Point(92, 91);
			this->tactorFrequency->Name = L"tactorFrequency";
			this->tactorFrequency->Size = System::Drawing::Size(43, 20);
			this->tactorFrequency->TabIndex = 16;
			// 
			// label93
			// 
			this->label93->AutoSize = true;
			this->label93->Location = System::Drawing::Point(129, 42);
			this->label93->Name = L"label93";
			this->label93->Size = System::Drawing::Size(35, 13);
			this->label93->TabIndex = 17;
			this->label93->Text = L"Port 4";
			// 
			// label96
			// 
			this->label96->AutoSize = true;
			this->label96->Location = System::Drawing::Point(222, 42);
			this->label96->Name = L"label96";
			this->label96->Size = System::Drawing::Size(35, 13);
			this->label96->TabIndex = 18;
			this->label96->Text = L"Port 6";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(368, 552);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->start_button);
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"UAV Flight Simulation Launch Application";
			this->tabControl1->ResumeLayout(false);
			this->tabGeneral->ResumeLayout(false);
			this->groupBox9->ResumeLayout(false);
			this->groupBox9->PerformLayout();
			this->groupBox7->ResumeLayout(false);
			this->groupBox7->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->tabNav->ResumeLayout(false);
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->tabCam->ResumeLayout(false);
			this->groupBox8->ResumeLayout(false);
			this->groupBox8->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->tabColors->ResumeLayout(false);
			this->tabColors->PerformLayout();
			this->groupBox19->ResumeLayout(false);
			this->groupBox19->PerformLayout();
			this->groupBox18->ResumeLayout(false);
			this->groupBox18->PerformLayout();
			this->groupBox17->ResumeLayout(false);
			this->groupBox17->PerformLayout();
			this->groupBox16->ResumeLayout(false);
			this->groupBox16->PerformLayout();
			this->groupBox15->ResumeLayout(false);
			this->groupBox15->PerformLayout();
			this->groupBox14->ResumeLayout(false);
			this->groupBox14->PerformLayout();
			this->groupBox13->ResumeLayout(false);
			this->groupBox13->PerformLayout();
			this->groupBox12->ResumeLayout(false);
			this->groupBox12->PerformLayout();
			this->groupBox11->ResumeLayout(false);
			this->groupBox11->PerformLayout();
			this->tabTactors->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox20->ResumeLayout(false);
			this->groupBox20->PerformLayout();
			this->groupBox10->ResumeLayout(false);
			this->groupBox10->PerformLayout();
			this->tabFeatures->ResumeLayout(false);
			this->tabFeatures->PerformLayout();
			this->groupBox24->ResumeLayout(false);
			this->groupBox24->PerformLayout();
			this->groupBox23->ResumeLayout(false);
			this->groupBox23->PerformLayout();
			this->groupBox22->ResumeLayout(false);
			this->groupBox22->PerformLayout();
			this->groupBox21->ResumeLayout(false);
			this->groupBox21->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: void load_config()
		{
			ifstream in(CONFIG_FILE);
			if(in)
			{
				bool error = false;
				while(in)
				{
					string	var;
					string	str_val;
					float	f_val;
					int		int_val;
					bool	b_val;

					// read in the variable and skip commented lines
					in >> var;
					if(var == "#" || var == "")
					{
						getline(in, var);
						continue;
					}

					// assign the value
					// =======================================================================
					// RESOLUTION AND DRIVERS
					// =======================================================================
					if(var == "NAV_RESOLUTION")
					{
						if(!(in >> str_val)) {error = true; break;}
						this->navRes->Text = gcnew String(str_val.c_str());
					}
					else if(var == "NAV_DRIVER")
					{
						if(!(in >> str_val)) {error = true; break;}
						this->navDriver->Text = gcnew String(str_val.c_str());
					}
					else if(var == "NAV_POS_X")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->navPosX->Text = Convert::ToString(f_val);
					}
					else if(var == "NAV_POS_Y")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->navPosY->Text = Convert::ToString(f_val);
					}
					else if(var == "CAM_RESOLUTION")
					{
						if(!(in >> str_val)) {error = true; break;}
						this->camRes->Text = gcnew String(str_val.c_str());
					}
					else if(var == "CAM_DRIVER")
					{
						if(!(in >> str_val)) {error = true; break;}
						this->camDriver->Text = gcnew String(str_val.c_str());
					}
					else if(var == "CAM_POS_X")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->camPosX->Text = Convert::ToString(f_val);
					}
					else if(var == "CAM_POS_Y")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->camPosY->Text = Convert::ToString(f_val);
					}

					// =======================================================================
					// GENERAL CONFIGURATION
					// =======================================================================
					else if(var == "SCENARIO_FILE")
					{
						getline(in, str_val);

						// front
						size_t index = str_val.find_first_of("\"");
						if(index != std::string::npos)
							str_val = str_val.substr(index + 1, str_val.size() - index - 1);
						else {error = true; break;}
						index = str_val.find_last_of("\"");
						// back
						if(index != std::string::npos)
							str_val = str_val.substr(0, index);
						else {error = true; break;}
				
						// save out the filename
						this->scenarioFile->Text = gcnew String(str_val.c_str());
					}
					else if(var == "OUTPUT_FILE")
					{
						getline(in, str_val);

						// front
						size_t index = str_val.find_first_of("\"");
						if(index != std::string::npos)
							str_val = str_val.substr(index + 1, str_val.size() - index - 1);
						else {error = true; break;}
						index = str_val.find_last_of("\"");
						// back
						if(index != std::string::npos)
							str_val = str_val.substr(0, index);
						else {error = true; break;}
				
						// save out the filename
						this->outputFile->Text = gcnew String(str_val.c_str());
					}
					else if(var == "TIMESTAMP_OUTPUT")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->timestampOutput->Checked = b_val;
					}
					else if(var == "ZOOM_NEAR_TARGET")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->zoomNearTarget->Checked = b_val;
					}
					else if(var == "FEATURE_VISIBLE_DIST")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->featureVisibleDist->Text = Convert::ToString(f_val);
					}
					else if(var == "FEATURE_MISSED_DIST")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->featureMissedDist->Text = Convert::ToString(f_val);
					}

					else if(var == "LIGHT_AFTER_PASS")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->lightAfterPass->Checked = b_val;
					}

					else if(var == "SHOW_VERSION")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->showVersion->Checked = b_val;
					}
					else if(var == "SHOW_TIME")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->showTime->Checked = b_val;
					}
					else if(var == "USE_GPS_FAIL")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->useGPSFail->Checked = b_val;
					}
					else if(var == "ALLOW_SPEED_CHANGES")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->allowSpeedChanges->Checked = b_val;
					}
					else if(var == "ALLOW_PAUSE")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->allowPause->Checked = b_val;
					}
					else if(var == "USE_CAMERA_FAIL")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->useCamFail->Checked = b_val;
					}
					else if(var == "CAM_FAIL_AS_ANGLE")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->camFailAsAngle->Checked = b_val;
					}
					else if(var == "USE_FULL_CAM_SHADING")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->shadeArea->Text = "FULL";
					}
					else if(var == "USE_VIEW_CAM_SHADING")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->shadeArea->Text = "VIEWPORT";
					}
					else if(var == "SHOW_PROGRESS_BAR")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->showProgressBar->Checked = b_val;
					}
					else if(var == "FUEL_FACTOR")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->fuelFactor->Text = Convert::ToString(f_val);
					}
					else if(var == "SPEED_FACTOR")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->speedFactor->Text = Convert::ToString(f_val);
					}
					else if(var == "DOWN_ANGLE")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->downAngle->Text = Convert::ToString(f_val);
					}

					// =======================================================================
					// MAP INTERFACE
					// =======================================================================
					else if(var == "ZOOM_AMOUNT")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->zoomAmount->Text = Convert::ToString(f_val);
					}
					else if(var == "SHOW_BASE_TEXT")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->baseCaptions->Checked = b_val;
					}
					else if(var == "SHOW_BASE_ICONS")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->baseIcons->Checked = b_val;
					}
					else if(var == "SHOW_WAYPOINT_TEXT")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->wpCaptions->Checked = b_val;
					}
					else if(var == "SHOW_WAYPOINT_ICONS")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->wpIcons->Checked = b_val;
					}
					else if(var == "SHOW_UAV_TEXT")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->uavCaptions->Checked = b_val;
					}
					else if(var == "SHOW_UAV_ICONS")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->uavIcons->Checked = b_val;
					}
					else if(var == "SHOW_UAV_LINES")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->uavPaths->Checked = b_val;
					}
					else if(var == "SHOW_EXT_UAV_LINES")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->extUAVPaths->Checked = b_val;
					}
					else if(var == "SHOW_UFO_TEXT")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->ufoCaptions->Checked = b_val;
					}
					else if(var == "SHOW_UFO_ICONS")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->ufoIcons->Checked = b_val;
					}
					else if(var == "SHOW_UAV_START_ICONS")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->uavStartIcons->Checked = b_val;
					}
					else if(var == "MAP_ICON_SIZE")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->mapIconSize->Text = Convert::ToString(int_val);
					}

					// =======================================================================
					// LIGHTING CUES
					// =======================================================================
					else if(var == "USE_LIGHT_CUES")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->useLightCues->Checked = b_val;
					}
					else if(var == "HIGH_LIGHT_LEVEL")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->highLightLevel->Text = Convert::ToString(int_val);
					}
					else if(var == "MEDIUM_LIGHT_LEVEL")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->mediumLightLevel->Text = Convert::ToString(int_val);
					}
					else if(var == "LOW_LIGHT_LEVEL")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->lowLightLevel->Text = Convert::ToString(int_val);
					}
					else if(var == "HIGH_LIGHT_DIST")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->highLightDist->Text = Convert::ToString(f_val);
					}
					else if(var == "MEDIUM_LIGHT_DIST")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->mediumLightDist->Text = Convert::ToString(f_val);
					}
					else if(var == "BASE_LIGHT_LEVEL")
					{
						if(!(in >> str_val)) {error = true; break;}
						this->baseLightLevel->Text = gcnew String(str_val.c_str());
					}
					else if(var == "CAM_SHADE_AREA")
					{
						if(!(in >> str_val)) {error = true; break;}
						this->shadeArea->Text = gcnew String(str_val.c_str());
					}

					// =======================================================================
					// TACTOR CUES
					// =======================================================================
					else if(var == "RUN_TACTOR_TEST")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->runTactorTest->Checked = b_val;
					}
					else if(var == "USE_TACTOR_CUES")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->useTactorCues->Checked = b_val;
					}
					else if(var == "TACTOR_COM_PORT_1")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactorComPort1->Text = Convert::ToString(f_val);
					}
					else if(var == "TACTOR_COM_PORT_2")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactorComPort2->Text = Convert::ToString(f_val);
					}
					else if(var == "TACTOR_COM_PORT_3")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactorComPort3->Text = Convert::ToString(f_val);
					}
					else if(var == "TACTOR_COM_PORT_4")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactorComPort4->Text = Convert::ToString(f_val);
					}
					if(var == "TACTOR_COM_PORT_5")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactorComPort5->Text = Convert::ToString(f_val);
					}
					else if(var == "TACTOR_COM_PORT_6")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactorComPort6->Text = Convert::ToString(f_val);
					}
					else if(var == "TACTOR_GAIN")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactorGain->Text = Convert::ToString(f_val);
					}
					else if(var == "TACTOR_FREQ")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactorFrequency->Text = Convert::ToString(f_val);
					}
					else if(var == "HIGH_TACTOR_FREQ_1")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactor1Freq1->Text = Convert::ToString(f_val);
					}
					else if(var == "HIGH_TACTOR_FREQ_2")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactor1Freq2->Text = Convert::ToString(f_val);
					}
					else if(var == "HIGH_TACTOR_SOURCE")
					{
						if(!(in >> str_val)) {error = true; break;}
						this->tactor1Source->Text = gcnew String(str_val.c_str());
					}
					else if(var == "HIGH_TACTOR_LENGTH")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactor1Duration->Text = Convert::ToString(f_val);
					}
					else if(var == "HIGH_TACTOR_DIST")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactor1Dist->Text = Convert::ToString(f_val);
					}
					else if(var == "HIGH_TACTOR_PULSE")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->tactor1Pulse->Checked = b_val;
					}
					else if(var == "HIGH_TACTOR_PULSE_INTERVAL")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactor1PulseInterval->Text = Convert::ToString(f_val);
					}
					else if(var == "LOW_TACTOR_FREQ_1")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactor2Freq1->Text = Convert::ToString(f_val);
					}
					else if(var == "LOW_TACTOR_FREQ_2")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactor2Freq2->Text = Convert::ToString(f_val);
					}
					else if(var == "LOW_TACTOR_SOURCE")
					{
						if(!(in >> str_val)) {error = true; break;}
						this->tactor2Source->Text = gcnew String(str_val.c_str());
					}
					else if(var == "LOW_TACTOR_LENGTH")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactor2Duration->Text = Convert::ToString(f_val);
					}
					else if(var == "LOW_TACTOR_DIST")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactor2Dist->Text = Convert::ToString(f_val);
					}
					else if(var == "LOW_TACTOR_PULSE")
					{
						if(!(in >> b_val)) {error = true; break;}
						this->tactor2Pulse->Checked = b_val;
					}
					else if(var == "LOW_TACTOR_PULSE_INTERVAL")
					{
						if(!(in >> f_val)) {error = true; break;}
						this->tactor2PulseInterval->Text = Convert::ToString(f_val);
					}

					// =======================================================================
					// UAV COLORS
					// =======================================================================
					else if(var == "COLOR_UAV1A")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav1a->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV1R")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav1r->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV1G")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav1g->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV1B")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav1b->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV2A")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav2a->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV2R")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav2r->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV2G")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav2g->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV2B")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav2b->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV3A")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav3a->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV3R")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav3r->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV3G")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav3g->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV3B")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav3b->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV4A")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav4a->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV4R")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav4r->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV4G")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav4g->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV4B")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav4b->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV5A")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav5a->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV5R")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav5r->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV5G")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav5g->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV5B")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav5b->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV6A")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav6a->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV6R")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav6r->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV6G")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav6g->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV6B")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav6b->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV7A")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav7a->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV7R")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav7r->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV7G")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav7g->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV7B")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav7b->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV8A")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav8a->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV8R")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav8r->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV8G")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav8g->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV8B")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav8b->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV9A")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav9a->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV9R")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav9r->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV9G")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav9g->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_UAV9B")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->uav9b->Text = Convert::ToString(int_val);
					}

					// =======================================================================
					// WAYPOINT FEATURE COLORS
					// =======================================================================
					else if(var == "COLOR_FCA")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fca->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FCR")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fcr->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FCG")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fcg->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FCB")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fcb->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FDA")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fda->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FDR")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fdr->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FDG")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fdg->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FDB")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fdb->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FUA")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fua->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FUR")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fur->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FUG")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fug->Text = Convert::ToString(int_val);
					}
					else if(var == "COLOR_FUB")
					{
						if(!(in >> int_val)) {error = true; break;}
						this->fub->Text = Convert::ToString(int_val);
					}
				}
				in.close();
			}
		}
	private: void save_config()
		{
			ofstream out(CONFIG_FILE);
			if(out)
			{
				string str;
				string str1;
				int width = 30;

				To_string(DateTime::Now.ToString("MM/dd/yy - HH:mm:ss"), str);
				out << "# UAV Sim Configuration File" << endl;
				out << "# University of Michigan THInC Lab" << endl;
				out << "# Generated " << str << endl;
				out << endl;

				out << "# resolution and drivers" << endl;
				To_string(this->navRes->Text, str);
				out << setw(width) << left << "NAV_RESOLUTION" << str << endl;
				To_string(this->navDriver->Text, str);
				out << setw(width) << left << "NAV_DRIVER" << str << endl;
				To_string(this->navPosX->Text, str);
				out << setw(width) << left << "NAV_POS_X" << str << endl;
				To_string(this->navPosY->Text, str);
				out << setw(width) << left << "NAV_POS_Y" << str << endl;
				To_string(this->camRes->Text, str);
				out << setw(width) << left << "CAM_RESOLUTION" << str << endl;
				To_string(this->camDriver->Text, str);
				out << setw(width) << left << "CAM_DRIVER" << str << endl;
				To_string(this->camPosX->Text, str);
				out << setw(width) << left << "CAM_POS_X" << str << endl;
				To_string(this->camPosY->Text, str);
				out << setw(width) << left << "CAM_POS_Y" << str << endl;
				out << endl;

				out << "# general configuration" << endl;
				To_string(this->scenarioFile->Text, str);
				str = "\"" + str + "\"";
				out << setw(width) << left << "SCENARIO_FILE" << str << endl;
				To_string(this->outputFile->Text, str);
				str = "\"" + str + "\"";
				out << setw(width) << left << "OUTPUT_FILE" << str << endl;
				str = (timestampOutput->Checked) ? "1" : "0";
				out << setw(width) << left << "TIMESTAMP_OUTPUT" << str << endl;
				To_string(this->fuelFactor->Text, str);
				out << setw(width) << left << "FUEL_FACTOR" << str << endl;
				To_string(this->speedFactor->Text, str);
				out << setw(width) << left << "SPEED_FACTOR" << str << endl;
				To_string(this->downAngle->Text, str);
				out << setw(width) << left << "DOWN_ANGLE" << str << endl;
				str = (zoomNearTarget->Checked) ? "1" : "0";
				out << setw(width) << left << "ZOOM_NEAR_TARGET" << str << endl;
				To_string(this->featureVisibleDist->Text, str);
				out << setw(width) << left << "FEATURE_VISIBLE_DIST" << str << endl;
				To_string(this->featureMissedDist->Text, str);
				out << setw(width) << left << "FEATURE_MISSED_DIST" << str << endl;
				str = (lightAfterPass->Checked) ? "1" : "0";
				out << setw(width) << left << "LIGHT_AFTER_PASS" << str << endl;
				str = (showVersion->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_VERSION" << str << endl;
				str = (showTime->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_TIME" << str << endl;
				str = (useGPSFail->Checked) ? "1" : "0";
				out << setw(width) << left << "USE_GPS_FAIL" << str << endl;
				str = (allowSpeedChanges->Checked) ? "1" : "0";
				out << setw(width) << left << "ALLOW_SPEED_CHANGES" << str << endl;
				str = (allowPause->Checked) ? "1" : "0";
				out << setw(width) << left << "ALLOW_PAUSE" << str << endl;
				str = (useCamFail->Checked) ? "1" : "0";
				out << setw(width) << left << "USE_CAMERA_FAIL" << str << endl;
				str = (camFailAsAngle->Checked) ? "1" : "0";
				out << setw(width) << left << "CAM_FAIL_AS_ANGLE" << str << endl;
				str = (showProgressBar->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_PROGRESS_BAR" << str << endl;
				out << setw(width) << left << "BUTTON_CLICK_MS" << "200" << endl;
				out << endl;

				out << "# map interface" << endl;
				To_string(this->zoomAmount->Text, str);
				out << setw(width) << left << "ZOOM_AMOUNT" << str << endl;
				str = (baseCaptions->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_BASE_TEXT" << str << endl;
				str = (baseIcons->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_BASE_ICONS" << str << endl;
				str = (wpCaptions->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_WAYPOINT_TEXT" << str << endl;
				str = (wpIcons->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_WAYPOINT_ICONS" << str << endl;
				str = (uavCaptions->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_UAV_TEXT" << str << endl;
				str = (uavIcons->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_UAV_ICONS" << str << endl;
				str = (uavPaths->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_UAV_LINES" << str << endl;
				str = (extUAVPaths->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_EXT_UAV_LINES" << str << endl;
				str = (ufoCaptions->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_UFO_TEXT" << str << endl;
				str = (ufoIcons->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_UFO_ICONS" << str << endl;
				str = (uavStartIcons->Checked) ? "1" : "0";
				out << setw(width) << left << "SHOW_UAV_START_ICONS" << str << endl;
				To_string(this->mapIconSize->Text, str);
				out << setw(width) << left << "MAP_ICON_SIZE" << str << endl;
				out << endl;

				out << "# lighting cues" << endl;
				str = (useLightCues->Checked) ? "1" : "0";
				out << setw(width) << left << "USE_LIGHT_CUES" << str << endl;
				To_string(this->highLightLevel->Text, str);
				out << setw(width) << left << "HIGH_LIGHT_LEVEL" << str << endl;
				To_string(this->mediumLightLevel->Text, str);
				out << setw(width) << left << "MEDIUM_LIGHT_LEVEL" << str << endl;
				To_string(this->lowLightLevel->Text, str);
				out << setw(width) << left << "LOW_LIGHT_LEVEL" << str << endl;
				To_string(this->highLightDist->Text, str);
				out << setw(width) << left << "HIGH_LIGHT_DIST" << str << endl;
				To_string(this->mediumLightDist->Text, str);
				out << setw(width) << left << "MEDIUM_LIGHT_DIST" << str << endl;
				To_string(this->baseLightLevel->Text, str);
				out << setw(width) << left << "BASE_LIGHT_LEVEL" << str << endl;
				To_string(this->shadeArea->Text, str);
				out << setw(width) << left << "CAM_SHADE_AREA" << str << endl;
				out << endl;

				out << "# tactor system and cues" << endl;
				str = (runTactorTest->Checked) ? "1" : "0";
				out << setw(width) << left << "RUN_TACTOR_TEST" << str << endl;
				str = (useTactorCues->Checked) ? "1" : "0";
				out << setw(width) << left << "USE_TACTOR_CUES" << str << endl;
				To_string(this->tactorComPort1->Text, str);
				out << setw(width) << left << "TACTOR_COM_PORT_1" << str << endl;
				To_string(this->tactorComPort2->Text, str);
				out << setw(width) << left << "TACTOR_COM_PORT_2" << str << endl;
				To_string(this->tactorComPort3->Text, str);
				out << setw(width) << left << "TACTOR_COM_PORT_3" << str << endl;
				To_string(this->tactorComPort4->Text, str);
				out << setw(width) << left << "TACTOR_COM_PORT_4" << str << endl;
				To_string(this->tactorComPort5->Text, str);
				out << setw(width) << left << "TACTOR_COM_PORT_5" << str << endl;
				To_string(this->tactorComPort6->Text, str);
				out << setw(width) << left << "TACTOR_COM_PORT_6" << str << endl;
				To_string(this->tactorGain->Text, str);
				out << setw(width) << left << "TACTOR_GAIN" << str << endl;
				To_string(this->tactorFrequency->Text, str);
				out << setw(width) << left << "TACTOR_FREQ" << str << endl;
				To_string(this->tactor1Freq1->Text, str);
				out << setw(width) << left << "HIGH_TACTOR_FREQ_1" << str << endl;
				To_string(this->tactor1Freq2->Text, str);
				out << setw(width) << left << "HIGH_TACTOR_FREQ_2" << str << endl;
				To_string(this->tactor1Source->Text, str);
				out << setw(width) << left << "HIGH_TACTOR_SOURCE" << str << endl;
				To_string(this->tactor1Duration->Text, str);
				out << setw(width) << left << "HIGH_TACTOR_LENGTH" << str << endl;
				To_string(this->tactor1Dist->Text, str);
				out << setw(width) << left << "HIGH_TACTOR_DIST" << str << endl;
				str = (tactor1Pulse->Checked) ? "1" : "0";
				out << setw(width) << left << "HIGH_TACTOR_PULSE" << str << endl;
				To_string(this->tactor1PulseInterval->Text, str);
				out << setw(width) << left << "HIGH_TACTOR_PULSE_INTERVAL" << str << endl;
				To_string(this->tactor2Freq1->Text, str);
				out << setw(width) << left << "LOW_TACTOR_FREQ_1" << str << endl;
				To_string(this->tactor2Freq2->Text, str);
				out << setw(width) << left << "LOW_TACTOR_FREQ_2" << str << endl;
				To_string(this->tactor2Source->Text, str);
				out << setw(width) << left << "LOW_TACTOR_SOURCE" << str << endl;
				To_string(this->tactor2Duration->Text, str);
				out << setw(width) << left << "LOW_TACTOR_LENGTH" << str << endl;
				To_string(this->tactor2Dist->Text, str);
				out << setw(width) << left << "LOW_TACTOR_DIST" << str << endl;
				str = (tactor2Pulse->Checked) ? "1" : "0";
				out << setw(width) << left << "LOW_TACTOR_PULSE" << str << endl;
				To_string(this->tactor2PulseInterval->Text, str);
				out << setw(width) << left << "LOW_TACTOR_PULSE_INTERVAL" << str << endl;
				out << endl;

				out << "# waypoint feature colors" << endl;
				To_string(this->fca->Text, str);
				out << setw(width) << left << "COLOR_FCA" << str << endl;
				To_string(this->fcr->Text, str);
				out << setw(width) << left << "COLOR_FCR" << str << endl;
				To_string(this->fcg->Text, str);
				out << setw(width) << left << "COLOR_FCG" << str << endl;
				To_string(this->fcb->Text, str);
				out << setw(width) << left << "COLOR_FCB" << str << endl;
				To_string(this->fda->Text, str);
				out << setw(width) << left << "COLOR_FDA" << str << endl;
				To_string(this->fdr->Text, str);
				out << setw(width) << left << "COLOR_FDR" << str << endl;
				To_string(this->fdg->Text, str);
				out << setw(width) << left << "COLOR_FDG" << str << endl;
				To_string(this->fdb->Text, str);
				out << setw(width) << left << "COLOR_FDB" << str << endl;
				To_string(this->fua->Text, str);
				out << setw(width) << left << "COLOR_FUA" << str << endl;
				To_string(this->fur->Text, str);
				out << setw(width) << left << "COLOR_FUR" << str << endl;
				To_string(this->fug->Text, str);
				out << setw(width) << left << "COLOR_FUG" << str << endl;
				To_string(this->fub->Text, str);
				out << setw(width) << left << "COLOR_FUB" << str << endl;
				out << endl;

				out << "# uav colors" << endl;
				To_string(this->uav1a->Text, str);
				out << setw(width) << left << "COLOR_UAV1A" << str << endl;
				To_string(this->uav1r->Text, str);
				out << setw(width) << left << "COLOR_UAV1R" << str << endl;
				To_string(this->uav1g->Text, str);
				out << setw(width) << left << "COLOR_UAV1G" << str << endl;
				To_string(this->uav1b->Text, str);
				out << setw(width) << left << "COLOR_UAV1B" << str << endl;
				To_string(this->uav2a->Text, str);
				out << setw(width) << left << "COLOR_UAV2A" << str << endl;
				To_string(this->uav2r->Text, str);
				out << setw(width) << left << "COLOR_UAV2R" << str << endl;
				To_string(this->uav2g->Text, str);
				out << setw(width) << left << "COLOR_UAV2G" << str << endl;
				To_string(this->uav2b->Text, str);
				out << setw(width) << left << "COLOR_UAV2B" << str << endl;
				To_string(this->uav3a->Text, str);
				out << setw(width) << left << "COLOR_UAV3A" << str << endl;
				To_string(this->uav3r->Text, str);
				out << setw(width) << left << "COLOR_UAV3R" << str << endl;
				To_string(this->uav3g->Text, str);
				out << setw(width) << left << "COLOR_UAV3G" << str << endl;
				To_string(this->uav3b->Text, str);
				out << setw(width) << left << "COLOR_UAV3B" << str << endl;
				To_string(this->uav4a->Text, str);
				out << setw(width) << left << "COLOR_UAV4A" << str << endl;
				To_string(this->uav4r->Text, str);
				out << setw(width) << left << "COLOR_UAV4R" << str << endl;
				To_string(this->uav4g->Text, str);
				out << setw(width) << left << "COLOR_UAV4G" << str << endl;
				To_string(this->uav4b->Text, str);
				out << setw(width) << left << "COLOR_UAV4B" << str << endl;
				To_string(this->uav5a->Text, str);
				out << setw(width) << left << "COLOR_UAV5A" << str << endl;
				To_string(this->uav5r->Text, str);
				out << setw(width) << left << "COLOR_UAV5R" << str << endl;
				To_string(this->uav5g->Text, str);
				out << setw(width) << left << "COLOR_UAV5G" << str << endl;
				To_string(this->uav5b->Text, str);
				out << setw(width) << left << "COLOR_UAV5B" << str << endl;
				To_string(this->uav6a->Text, str);
				out << setw(width) << left << "COLOR_UAV6A" << str << endl;
				To_string(this->uav6r->Text, str);
				out << setw(width) << left << "COLOR_UAV6R" << str << endl;
				To_string(this->uav6g->Text, str);
				out << setw(width) << left << "COLOR_UAV6G" << str << endl;
				To_string(this->uav6b->Text, str);
				out << setw(width) << left << "COLOR_UAV6B" << str << endl;
				To_string(this->uav7a->Text, str);
				out << setw(width) << left << "COLOR_UAV7A" << str << endl;
				To_string(this->uav7r->Text, str);
				out << setw(width) << left << "COLOR_UAV7R" << str << endl;
				To_string(this->uav7g->Text, str);
				out << setw(width) << left << "COLOR_UAV7G" << str << endl;
				To_string(this->uav7b->Text, str);
				out << setw(width) << left << "COLOR_UAV7B" << str << endl;
				To_string(this->uav8a->Text, str);
				out << setw(width) << left << "COLOR_UAV8A" << str << endl;
				To_string(this->uav8r->Text, str);
				out << setw(width) << left << "COLOR_UAV8R" << str << endl;
				To_string(this->uav8g->Text, str);
				out << setw(width) << left << "COLOR_UAV8G" << str << endl;
				To_string(this->uav8b->Text, str);
				out << setw(width) << left << "COLOR_UAV8B" << str << endl;
				To_string(this->uav9a->Text, str);
				out << setw(width) << left << "COLOR_UAV9A" << str << endl;
				To_string(this->uav9r->Text, str);
				out << setw(width) << left << "COLOR_UAV9R" << str << endl;
				To_string(this->uav9g->Text, str);
				out << setw(width) << left << "COLOR_UAV9G" << str << endl;
				To_string(this->uav9b->Text, str);
				out << setw(width) << left << "COLOR_UAV9B" << str << endl;
				out << endl;

				out.close();
			}
		}

	private: System::Void start_button_Click(System::Object^  sender, System::EventArgs^  e) {
			save_config();
			try
			{
				// set current working directory to the folder the UAV sim is in and start it
				Environment::CurrentDirectory += "";
				System::Diagnostics::Process::Start("UAV Sim Remake.exe");
				exit(0);
			}
			catch(...)
			{
				MessageBox::Show("Unable to find \"UAV Sim Remake.exe\"", "Error",
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 String ^ cDir = Environment::CurrentDirectory;

			 OpenFileDialog ^ ofd = gcnew OpenFileDialog();
			 ofd->Filter = "UAV Simulation files (*.uav)|*.uav";
			 ofd->InitialDirectory = "scenario";

			 if(ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				scenarioFile->Text = ofd->FileName;

			 Environment::CurrentDirectory = cDir;
		}
private: System::Void useCamFail_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(useCamFail->Checked)	camFailAsAngle->Enabled = true;
			 else						camFailAsAngle->Enabled = false;
		 }
};
}

