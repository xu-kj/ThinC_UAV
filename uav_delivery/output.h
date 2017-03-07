#ifndef OUTPUT_H
#define OUTPUT_H

#include <fstream>
#include <irrlicht.h>

/* The Output class uses E_OUTPUT to specify which file to write to (default OUTPUT_LOG). */

enum E_OUTPUT { 
	OUTPUT_LOG = 0,            
	OUTPUT_UAV1,            
	OUTPUT_UAV2,
	OUTPUT_UAV3,
	OUTPUT_UAV4,
	OUTPUT_UAV5,
	OUTPUT_UAV6,
	OUTPUT_UAV7,
	OUTPUT_UAV8,
	OUTPUT_UAV9,
	OUTPUT_UAV1_TABLE,
	OUTPUT_UAV2_TABLE,
	OUTPUT_UAV3_TABLE,
	OUTPUT_UAV4_TABLE,
	OUTPUT_UAV5_TABLE,
	OUTPUT_UAV6_TABLE,
	OUTPUT_UAV7_TABLE,
	OUTPUT_UAV8_TABLE,
	OUTPUT_UAV9_TABLE,
	OUTPUT_UAV_TABLE,
	OUTPUT_EVENTS,
	OUTPUT_CHAT,
	OUTPUT_COMBINED,
	OUTPUT_RESULT,
	OUTPUT_COUNT // DO NOT USE
};
// add the following constant to the UAV id to write to its table file
const int OUTPUT_TABLE_FILE = 9;

enum UAV_EVENT {
	SIMULATION_STARTED,
	SIMULATION_ENDED,
	SIMULATION_PAUSED,
	SIMULATION_CONTINUED,
    WAYPOINT_TARGET_SIGHTED, 
    WAYPOINT_TARGET_ARRIVED,
    WAYPOINT_TARGET_PASSED,
    WAYPOINT_NONTARGET_SIGHTED,
    WAYPOINT_NONTARGET_ARRIVED,
    WAYPOINT_NONTARGET_PASSED,
	WAYPOINT_BASE_ARRIVED,
    INDICATOR_ON,
    INDICATOR_OFF,
    USER_TARGET,
    USER_NONTARGET,
    USER_UNSURE,
    USER_MISSED,
    USER_ALARM_VISUAL_REACTED,
    USER_ALARM_VISUAL_MISSED,
    USER_ALARM_AUDIO_REACTED,
    USER_ALARM_AUDIO_MISSED,
    ALARM_VISUAL_ON,
    ALARM_VISUAL_OFF,
    ALARM_AUDIO_ON,
    ALARM_AUDIO_OFF,
	UAV_STOPPED,
	UAV_MOVING,
    EVENT_COUNT
};

class Output {
public:
    static Output& Instance() {
        static Output theOutput;
        return theOutput;
    }

    void StartLog();
    void EndLog();
    void EndLogError(const irr::core::stringc &error);

    void NewLine(E_OUTPUT file = OUTPUT_LOG);
    void Write(const irr::core::stringc &output, E_OUTPUT file = OUTPUT_LOG);
    void WriteLine(const irr::core::stringc &output, E_OUTPUT file = OUTPUT_LOG);
    void WriteDate(E_OUTPUT file = OUTPUT_LOG);
    void WriteTime(E_OUTPUT file = OUTPUT_LOG);
    void WriteDateTime(E_OUTPUT file = OUTPUT_LOG);
    void WriteTick(E_OUTPUT file = OUTPUT_LOG);

	void WriteColumnName();
    void RecordEvent(int target, UAV_EVENT e, double pos_x, double pos_y, double pos_z);
	void GetResult();

private:
    Output() {}
    Output(Output const&);
    Output& operator=(Output const&);
    ~Output() {}

    void WriteHeader(E_OUTPUT file, const irr::core::stringw &file_path);
    bool CreateOutputDirectory(const irr::core::stringw &dir);

    static std::fstream files[OUTPUT_COUNT];
    static irr::core::stringc filenames[];
};

#endif /* OUTPUT_H */