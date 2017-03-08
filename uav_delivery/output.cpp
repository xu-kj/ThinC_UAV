#include "output.h"
#include "globals.h"

#include <iostream>
#include <sstream>
#include <list>
#include <windows.h>

using namespace std;
using namespace irr;
using namespace core;

static const char *UAV_EVENT_TEXT[UAV_EVENT::EVENT_COUNT] = {
	"SIMULATION_STARTED", "SIMULATION_ENDED",
	"SIMULATION_PAUSED", "SIMULATION_CONTINUED",
	"WAYPOINT_TARGET_SIGHTED", "WAYPOINT_TARGET_ARRIVED", "WAYPOINT_TARGET_PASSED",
	"WAYPOINT_NONTARGET_SIGHTED", "WAYPOINT_NONTARGET_ARRIVED", "WAYPOINT_NONTARGET_PASSED",
	"WAYPOINT_BASE_ARRIVED",
	"INDICATOR_ON", "INDICATOR_OFF",
	"USER_INDICATES_TARGET", "USER_INDICATES_NONTARGET", 
	"USER_RESPONSE_UNSURE", "USER_MISSED",
	"USER_ALARM_VISUAL_REACTED", "USER_ALARM_VISUAL_MISSED",
	"USER_ALARM_AUDIO_REACTED", "USER_ALARM_AUDIO_MISSED",
	"ALARM_VISUAL_ON", "ALARM_VISUAL_OFF",
	"ALARM_AUDIO_ON", "ALARM_AUDIO_OFF",
	"UAV_STOPPED", "UAV_MOVING",
	"SET_SOUND_VOLUME"
};

const wchar_t * const LOG_FOLDER = L"..\\results\\";
fstream Output::files[OUTPUT_COUNT];
stringc Output::filenames[] = {
    "log",
    "uav1",
    "uav2",
    "uav3",
    "uav4",
    "uav5",
    "uav6",
    "uav7",
    "uav8",
    "uav9",
    "uav1_table",
    "uav2_table",
    "uav3_table",
    "uav4_table",
    "uav5_table",
    "uav6_table",
    "uav7_table",
    "uav8_table",
    "uav9_table",
    "uav_table",
    "events",
    "chat",
	"combined_log",
	"sim_result"
};

void Output::StartLog()
{
    cout << "Setting up output files" << endl;
    stringc str;

    // start the directory path
    stringw dir_path = LOG_FOLDER;
    dir_path += OUTPUT_FILE;

    // add the date/time to the directory path
    SYSTEMTIME systime;
    GetSystemTime(&systime);
    stringstream datetime;
    datetime << systime.wMonth << "-" << systime.wDay << "-" << systime.wYear << "_" << systime.wHour << "." << systime.wMinute << "." << systime.wSecond;
    dir_path += "_";
    dir_path += datetime.str().c_str();

    // make sure the output directories exist
    if(!(CreateOutputDirectory(LOG_FOLDER) &&
         CreateOutputDirectory(dir_path)))
    {
        // failed to create the correct folders
        return;
    }

    // open the files
    std::list<stringw> failed;
    for(int i = 0; i < OUTPUT_COUNT; i++)
    {
        // create and open the current file
        stringw file_path = dir_path;
        file_path += "\\";
        file_path += filenames[i];
        file_path += ".txt";
        files[i].open(file_path.c_str(), ios::out);

        // display start time, etc
        if(files[i])
        {
            // file opened successfully
            WriteHeader((E_OUTPUT)i, file_path);
        }
        else
        {
            // file could not be opened
            failed.push_back(file_path);
        }
    }

    // let the user know which files could not be opened
    if(failed.size() > 0)
    {
        stringw str = "The following files could not be opened for output:";
        for(std::list<stringw>::iterator it = failed.begin(); it != failed.end(); it++)
        {
            str += "\n";
            str += (*it);
        }
        cout << stringc(str).c_str() << endl;
        MessageBox(0, L"Warning: The output file could not be opened for writing.", L"Ruh Roh! o_O", 0);
    }

    cout << endl;
}

void Output::EndLog()
{
    cout << "Closing output files" << endl;
    for(int i = 0; i < OUTPUT_COUNT; i++)
    {
        if(files[i])
        {
            Write("<End of Output - ", (E_OUTPUT)i);
            WriteDateTime((E_OUTPUT)i);
            WriteLine(">", (E_OUTPUT)i);

            files[i].flush();
            files[i].close();
        }
    }
}

void Output::EndLogError(const irr::core::stringc &error)
{
    cout << "Closing ouput file due to error: " << error.c_str() << endl;
    Write("<ENDING OUTPUT DUE TO ERROR: ");
    Write(error);
    WriteLine(">");
    EndLog();
}

void Output::Write(const irr::core::stringc &output, E_OUTPUT file)
{
    if(!files[file])
        return;

    files[file] << output.c_str();
    files[file].flush();
}

void Output::WriteLine(const irr::core::stringc &output, E_OUTPUT file)
{
    if(!files[file])
        return;

    files[file] << output.c_str() << endl;
    files[file].flush();
}

void Output::NewLine(E_OUTPUT file)
{
    if(!files[file])
        return;

    files[file] << endl;
    files[file].flush();
}

void Output::WriteDate(E_OUTPUT file)
{
    if(!files[file])
        return;
    
    SYSTEMTIME systime;
    GetSystemTime(&systime);

    files[file] << systime.wMonth << "/" << systime.wDay << "/" << systime.wYear;
    files[file].flush();
}

void Output::WriteTime(E_OUTPUT file)
{
    if(!files[file])
        return;
    
    SYSTEMTIME systime;
    GetSystemTime(&systime);

    files[file] << systime.wHour << ":" << systime.wMinute << ":" << systime.wSecond;
    files[file].flush();
}

void Output::WriteDateTime(E_OUTPUT file)
{
    WriteTime(file);
    Write(" ", file);
    WriteDate(file);
}

void Output::WriteTick(E_OUTPUT file)
{
    if(!files[file])
        return;

    files[file] << "[";
    if(MINUTES < 10) files[file] << "0";
    files[file] << MINUTES << ":";
    if(SECONDS < 10) files[file] << "0";
    files[file] << SECONDS << ".";
    if(MILLISECONDS < 100) files[file] << "0";
    if(MILLISECONDS < 10)  files[file] << "0";
    files[file] << MILLISECONDS << "] ";
    files[file].flush();
}

bool Output::CreateOutputDirectory(const irr::core::stringw &dir)
{
    if(GetFileAttributes(dir.c_str()) == INVALID_FILE_ATTRIBUTES)
    {
        cout << "Output folder \"" << stringc(dir).c_str() << "\" does not exist" << endl;
        if(CreateDirectory(dir.c_str(), NULL))
        {
            cout << "Created output folder \"" << stringc(dir).c_str() << "\"" << endl;
        }
        else
        {
            cout << "Failed to create output folder \"" << stringc(dir).c_str() << "\"" << endl;
            MessageBox(0, L"Warning: Could not create output folder; there will be no ouput file", L"Ruh Roh! o_O", 0);
            return false;
        }
    }
    return true;
}

void Output::WriteHeader(E_OUTPUT file, const irr::core::stringw &file_path)
{
    // every file gets this header
    cout << "Output file \"" << stringc(file_path).c_str() << "\" successfully opened" << endl;
    Write("<Output started - ", file);
    WriteDate(file);
    Write(" ", file);
    WriteTime(file);
    WriteLine(">", file);

    // addition file-specific stuff
    if(file >= OUTPUT_UAV1_TABLE && file <= OUTPUT_UAV_TABLE)
    {
        //UAV_NAME | WP_NAME | TIME_ACTIVE | TIME_LIGHT_CUE1 | TIME_LIGHT_CUE2 | TIME_TACTOR_CUE1 | TIME_TACTOR_CUE2 | TIME_SIGHTED | TIME_REACHED | TIME_DONE | USER_RESPONSE | CORRECTNESS | FEATURE_OR_NOT | BUTTON_CLICKED
        WriteLine("UAV       WAYPOINT      ACTIVE    LIGHT1    LIGHT2   TACTOR1   TACTOR2   SIGHTED   REACHED  RESPONSE  CORRECT?    HAS FEATURE?    BUTTON", file);
    }
}

void Output::WriteColumnName()
{
	fstream &fs = files[OUTPUT_COMBINED];
	fs << "TIMESTAMP" << ","
		<< "TARGET" << ","
		<< "EVENT" << ","
		<< "POS_X" << "," << "POS_Y" << "," << "POS_Z"
		<< endl;
}

void Output::RecordEvent(int target, UAV_EVENT e, double pos_x, double pos_y, double pos_z)
{
    fstream &fs = files[OUTPUT_COMBINED];
    if(MINUTES < 10) 
		fs << "0";
    fs << MINUTES << ":";
    if(SECONDS < 10) 
		fs << "0";
    fs << SECONDS << ".";
    if(MILLISECONDS < 100) 
		fs << "0";
    if(MILLISECONDS < 10)  
		fs << "0";
    fs << MILLISECONDS << ",";
    fs << target << ',';
	fs << UAV_EVENT_TEXT[e - UAV_EVENT::SIMULATION_STARTED];
    fs << ',' << pos_x << ',' << pos_y << ',' << pos_z << endl;
}
