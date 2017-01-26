#ifndef TACTOR_H
#define TACTOR_H

#include <windows.h>

#define TAC1 0x01
#define TAC2 0x02
#define TAC3 0x04
#define TAC4 0x08
#define TAC5 0x10
#define TAC6 0x20
#define TAC7 0x40
#define TAC8 0x80


#define MASTER 0x00
#define SLAVE1 0x01
#define SLAVE2 0x02
#define SLAVE3 0x03
#define SLAVE4 0x04
#define SLAVE5 0x05
#define SLAVE6 0x06
#define SLAVE7 0x07


class CTactor 
{
// Private member variables and functions are for internal API use only
private:
    // Communication variables
    HANDLE hComm;
    DWORD errCode;
    BYTE rBuffer[8];
    DWORD dwRes;
    DWORD dwRead;
    BOOL fRes;
    BOOL fWaitingOnRead;
    DCB properties;
    COMMTIMEOUTS commTimeout;
    DWORD wmWritten;
    BYTE buffer[8];

    // Value representing whether or not we are looking for return data on our function calls
    // if this value is false, 0 will always be returned
    bool bDebug;

public:
    // Constructor
    //
    // debug - set to true and each command you send to the board will wait until it receives
    // a command back from the board before returning.
    CTactor();
    CTactor(bool debug);
    
    // Called by the user to establish the serial connection to the tactor board
    // This portName is the COMMPort name assigned to your TactorControl Board
    // Note that these are usually COM5, COM6, COM1 etc.
    //
    // EX. OpenBTPort("COM5");
    //
    // Also Note that Port numbers over 9 require different syntax then com ports
    // under 10.
    // 
    // Ex. OpenBTPort("\\\\.\\COM10");
    int OpenBTPort(const char * portName);  

    // Turns all of the tactors on the board off
    //
    // board - used to address specific baords in a multi-board system
    // this value defaults to 0 or MASTER, so in a 1 board system you can leave the parameter
    // alone.
    int TurnAllOff(int board = 0x00);

    // Stops playing a sequence on the board
    //
    // board - used to address specific baords in a multi-board system
    // this value defaults to 0 or MASTER, so in a 1 board system you can leave the parameter
    // alone.
    int KillSeq(int board = 0x00);

    // Stops playing any sequences, turns all tactors off & puts the amplifiers into 
    // shutdown mode
    //
    // board - used to address specific baords in a multi-board system
    // this value defaults to 0 or MASTER, so in a 1 board system you can leave the parameter
    // alone.
    int SleepBoard(int board = 0x00);

    // Turns a specified tactor(s) on for a specified time
    //
    // tacNum - Use the defined keywords to reference each tactor (see above)
    //
    // durCenti - Duration in 10 millisecond increments
    //
    // board - used to address specific baords in a multi-board system
    // this value defaults to 0 or MASTER, so in a 1 board system you can leave the parameter
    // alone.
    int TacOnTime(int tacNum, int durCenti, int board = 0x00);

    // Sets the frequency of the primary sinewave signal
    //
    // freq - excepted values range between 30 - 340
    //
    // board - used to address specific baords in a multi-board system
    // this value defaults to 0 or MASTER, so in a 1 board system you can leave the parameter
    // alone.
    int SetSinFreq1(int freq, int board = 0x00);

    // Sets the frequency of the secondary sinewave signal
    //
    // freq - excepted values range between 30 - 340
    //
    // board - used to address specific baords in a multi-board system
    // this value defaults to 0 or MASTER, so in a 1 board system you can leave the parameter
    // alone.
    int SetSinFreq2(int freq, int board = 0x00);

    // Sets the signal source for each of the 2 sets of 4 tactors on the board
    // tactors 1-4 are represented by the bank1 parameter
    // tactors 5-8 are represented by the bank2 parameter
    //
    // bank1 & bank2 - 
    // excepted values for each parameter are 0,1,2, or 3
    // 0 = No Signal
    // 1 = Primary Sinewave Signal
    // 2 = Secondary Sinewave Signal
    // 3 = Summed signal of both sinewave generators
    //
    // board - used to address specific baords in a multi-board system
    // this value defaults to 0 or MASTER, so in a 1 board system you can leave the parameter
    // alone.
    int SetSigSrc(int bank1, int bank2, int board = 0x00);

    // Starts Playing the specified sequence on the tactor board
    //
    // seq - Sequences are 0 based (ie. sequence 0, sequence 1, etc.
    //
    // board - used to address specific baords in a multi-board system
    // this value defaults to 0 or MASTER, so in a 1 board system you can leave the parameter
    // alone.
    int PlaySeq(int seqNum, int board = 0x00);

    // Sets the gain level of the amplifiers
    //
    // gain - valid gain settings are 0, 1, 2 and 3
    //
    // board - used to address specific baords in a multi-board system
    // this value defaults to 0 or MASTER, so in a 1 board system you can leave the parameter
    // alone.
    int SetGain(int gain, int board = 0x00);

    // Cycles through each tactor and turns it on at the current frequency settings
    // Call this function to establish that all tactors are firing
    //
    // board - used to address specific baords in a multi-board system
    // this value defaults to 0 or MASTER, so in a 1 board system you can leave the parameter
    // alone.
    int SelfTest(int board = 0x00);

    // Called to release to close down threads and release resources.
    // This should be called before you exit your application or destroy
    // an object of this class type
    int Shutdown(void);

// Private Methods are for internal API usage only and should not be called by
// your application
private:
    int SetupCommState();

    int SetupCommTimeouts();

    void SendToTactor(BYTE * buf, int numBytes);

    int ReadReply();
};

#endif /* TACTOR_H */