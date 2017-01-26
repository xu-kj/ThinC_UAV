#include "tactor.h"

#include <windows.h>

CTactor::CTactor(bool debug) {
    hComm           = NULL;
    fWaitingOnRead  = FALSE;
    errCode         = 0;
    rBuffer[8]      = 0;
    dwRes           = 0;
    dwRead          = 0;

    fRes            = FALSE;

    if (debug)
        bDebug = true;
    else
        bDebug = false;

    return;
}


CTactor::CTactor() {
    bool debug=false;
    hComm           = NULL;
    fWaitingOnRead  = FALSE;
    errCode         = 0;
    rBuffer[8]      = 0;
    dwRes           = 0;
    dwRead          = 0;

    fRes            = FALSE;

    if (debug)
        bDebug = true;
    else
        bDebug = false;

    return;
}

int CTactor::OpenBTPort(const char * portName) {
    hComm = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);

    if (hComm == INVALID_HANDLE_VALUE)
    {
        errCode = GetLastError();
        return 8;
    }
    else
    {
        return CTactor::SetupCommState();
    }
}

int CTactor::Shutdown(void) {
    if (CloseHandle(hComm))
        return 0;
    else
        return 8;
}

int CTactor::SetupCommState() {
    GetCommState(hComm, &properties);

    properties.BaudRate  = CBR_115200;
    properties.Parity = NOPARITY;
    properties.ByteSize = 8;
    properties.StopBits = ONESTOPBIT;

    if (SetCommState(hComm, &properties))
        return CTactor::SetupCommTimeouts();
    else
        return 8;
}

int CTactor::SetupCommTimeouts() {
    GetCommTimeouts(hComm, &commTimeout);   

    commTimeout.ReadIntervalTimeout         = 500;
    commTimeout.ReadTotalTimeoutMultiplier  = 500;
    commTimeout.ReadTotalTimeoutConstant    = 500;
    commTimeout.WriteTotalTimeoutConstant   = 2;
    commTimeout.WriteTotalTimeoutMultiplier = 3;

    if(SetCommTimeouts(hComm, &commTimeout))
        return 0;
    else
        return 8;
}

int CTactor::TurnAllOff(int board) {
    buffer[0] = 0x02;
    buffer[1] = board;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[5] = 0x03;

    buffer[4] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[5]^0xEA;

    SendToTactor(buffer, 6);

    if (bDebug)
        return ReadReply();
    else
        return 0;
}

int CTactor::KillSeq(int board) {
    buffer[0] = 0x02;
    buffer[1] = board;
    buffer[2] = 0x01;
    buffer[3] = 0x00;
    buffer[5] = 0x03;

    buffer[4] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[5]^0xEA;

    SendToTactor(buffer, 6);

    if (bDebug)
        return ReadReply();
    else
        return 0;
}

int CTactor::SleepBoard(int board) {
    buffer[0] = 0x02;
    buffer[1] = board;
    buffer[2] = 0x02;
    buffer[3] = 0x00;
    buffer[5] = 0x03;

    buffer[4] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[5]^0xEA;

    SendToTactor(buffer, 6);

    if (bDebug)
        return ReadReply();
    else
        return 0;
}

int CTactor::TacOnTime(int tacNum, int durCenti, int board) {
    buffer[0] = 0x02;
    buffer[1] = board;
    buffer[2] = 0x11;
    buffer[3] = 0x02;

    buffer[4] = tacNum;
    buffer[5] = durCenti;
    
    buffer[7] = 0x03;

    buffer[6] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[4]^buffer[5]^buffer[7]^0xEA;

    SendToTactor(buffer, 8);

    if (bDebug)
        return ReadReply();
    else
        return 0;
}

int CTactor::SetSinFreq1(int freq, int board) {
    buffer[0] = 0x02;
    buffer[1] = board;
    buffer[2] = 0x12;
    buffer[3] = 0x02;
    
    if (freq <= 255)
    {
        buffer[4] = freq;
        buffer[5] = 0x00;
    }
    else 
    {
        buffer[4] = freq - 255;
        buffer[5] = 0x01;
    }
    
    buffer[7] = 0x03;

    buffer[6] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[4]^buffer[5]^buffer[7]^0xEA;

    SendToTactor(buffer, 8);

    if (bDebug)
        return ReadReply();
    else
        return 0;
}

int CTactor::SetSinFreq2(int freq, int board) {
    buffer[0] = 0x02;
    buffer[1] = board;
    buffer[2] = 0x14;
    buffer[3] = 0x02;
    
    if (freq <= 255)
    {
        buffer[4] = freq;
        buffer[5] = 0x00;
    }
    else 
    {
        buffer[4] = freq - 255;
        buffer[5] = 0x01;
    }
    
    buffer[7] = 0x03;

    buffer[6] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[4]^buffer[5]^buffer[7]^0xEA;

    SendToTactor(buffer, 8);

    if (bDebug)
        return ReadReply();
    else
        return 0;
}

int CTactor::SetSigSrc(int bank1, int bank2, int board) {
    buffer[0] = 0x02;
    buffer[1] = board;
    buffer[2] = 0x15;
    buffer[3] = 0x01;

    buffer[4] = ((bank1 * 16) + bank2);
    
    buffer[6] = 0x03;

    buffer[5] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[4]^buffer[6]^0xEA;

    SendToTactor(buffer, 7);

    if (bDebug)
        return ReadReply();
    else
        return 0;
}

int CTactor::PlaySeq(int seqNum, int board) {
    buffer[0] = 0x02;
    buffer[1] = board;
    buffer[2] = 0x1A;
    buffer[3] = 0x01;

    buffer[4] = seqNum;
    
    buffer[6] = 0x03;

    buffer[5] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[4]^buffer[6]^0xEA;

    SendToTactor(buffer, 7);

    if (bDebug)
        return ReadReply();
    else
        return 0;
}

int CTactor::SetGain(int gain, int board) {
    buffer[0] = 0x02;
    buffer[1] = board;
    buffer[2] = 0x20;
    buffer[3] = 0x01;

    buffer[4] = (gain);
        
    buffer[6] = 0x03;

    buffer[5] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[4]^buffer[6]^0xEA;

    SendToTactor(buffer, 7);

    if (bDebug)
        return ReadReply();
    else
        return 0;
}

int CTactor::SelfTest(int board) {
    buffer[0] = 0x02;
    buffer[1] = board;
    buffer[2] = 0x30;
    buffer[3] = 0x00;
    buffer[5] = 0x03;
    
    buffer[4] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[5]^0xEA;
    
    SendToTactor(buffer, 6);

    if (bDebug)
        return ReadReply();
    else
        return 0;
}

void CTactor::SendToTactor(BYTE * buf, int numBytes) {
    OVERLAPPED osWrite = {0};

    osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if(osWrite.hEvent == NULL)
    {
    }
    
    while(!fRes)
    {
        if(!WriteFile(hComm, buf, numBytes, &wmWritten, &osWrite))
        {
            errCode = GetLastError();
            
            if(errCode != ERROR_IO_PENDING)
            {
            }
            else
            {
                dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
                switch(dwRes)
                {
                case WAIT_OBJECT_0:
                    if(!GetOverlappedResult(hComm, &osWrite, &wmWritten, FALSE))
                    {
                        fRes = FALSE;
                    }
                    else
                    {
                        fRes = TRUE;
                    }
                    break;
                default:
                    fRes = FALSE;
                    break;
                }
            }
        }
        else
        {
            fRes = TRUE;
        }

    }

    fRes = FALSE;

    return;
}

int CTactor::ReadReply() {
    OVERLAPPED osReader = {0};
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    int numToRead = 3;
    bool bHeaderRead = false;
    bool bTailRead = false;

    while(!bHeaderRead)
    {
        if(osReader.hEvent == NULL)
        {
            return 8;
        }

        if(!fWaitingOnRead)
        {
            if(!ReadFile(hComm, rBuffer, numToRead, &dwRead, &osReader))
            {
                errCode = GetLastError();
            
                if(errCode != ERROR_IO_PENDING)
                    return 8;
                else
                    fWaitingOnRead = TRUE;
            }
            else
            {
                numToRead = rBuffer[2] + 2;
                bHeaderRead = true;
            }
        }

        if(fWaitingOnRead)
        {
            dwRes = WaitForSingleObject(osReader.hEvent, 700);
            switch(dwRes)
            {
            case WAIT_OBJECT_0:
                if(!GetOverlappedResult(hComm, &osReader, &dwRead, FALSE))
                {
                    errCode = GetLastError();
                    return 8;
                }
                else
                    bHeaderRead = true;
    
                fWaitingOnRead = FALSE;
                break;
            case WAIT_TIMEOUT:
                break;
            default:
                return 8;
            }
        }
    }

    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    while(!bTailRead)
    {
        if(osReader.hEvent == NULL)
        {
            return 8;
        }

        if(!fWaitingOnRead)
        {
            if(!ReadFile(hComm, &rBuffer[3], numToRead, &dwRead, &osReader))
            {
                errCode = GetLastError();
            
                if(errCode != ERROR_IO_PENDING)
                    return 8;
                else
                    fWaitingOnRead = TRUE;
            }
            else
                bTailRead = true;
        }

        if(fWaitingOnRead)
        {
            dwRes = WaitForSingleObject(osReader.hEvent, 700);
            switch(dwRes)
            {
            case WAIT_OBJECT_0:
                if(!GetOverlappedResult(hComm, &osReader, &dwRead, FALSE))
                {
                    errCode = GetLastError();
                    return 8;
                }
                else
                    bTailRead = true;
    
                fWaitingOnRead = FALSE;
                break;
            case WAIT_TIMEOUT:
                break;
            default:
                return 8;
            }
        }
    }

    if (rBuffer[1] == 0xC8)
        return 0;
    else if (rBuffer[1] == 0xC4)
    {
        return rBuffer[3];
    }
}

//DWORD CTactor::ReadStart(LPVOID lpParam)
//{
//  OVERLAPPED osReader = {0};
//  osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//
//  int numToRead = 6;
//
//  while(true)
//  {
//      //printf("ReadLoopStart\n");
//
//      if(osReader.hEvent == NULL)
//      {
//          printf("Error Creating Overlapped Event\n");
//      }
//
//
//      // Break the readfile out into a funtion, that get passed the numTo Read
//      if(!fWaitingOnRead)
//      {
//          if(!ReadFile(hComm, rBuffer, numToRead, &dwRead, &osReader))
//          {
//              errCode = GetLastError();
//          
//              if(errCode != ERROR_IO_PENDING)
//              {
//                  printf("Read Failed & Isn't Delayed\n");
//                  printf("Error Code: %i\n", errCode);
//                  //Sleep(3000);
//              }
//              else
//              {
//                  fWaitingOnRead = TRUE;
//              }
//
//          }
//          else
//          {
//              printf("Read finished immediately\n");
//              for (int i = 0; i < dwRead; i++)
//              {
//                  printf("%i ",rBuffer[i]);
//              }
//              
//
//              numToRead = rBuffer[2] + 1;
//
//              printf("\n");
//          }
//      }
//
//      if(fWaitingOnRead)
//      {
//          dwRes = WaitForSingleObject(osReader.hEvent, 700);
//          switch(dwRes)
//          {
//          case WAIT_OBJECT_0:
//              if(!GetOverlappedResult(hComm, &osReader, &dwRead, FALSE))
//              {
//                  printf("Error in communications\n");
//                  errCode = GetLastError();
//          
//                  printf("Error Code: %i\n", errCode);
//                  //Sleep(3000);
//              }
//              else
//              {
//                  //printf("Read finished delayed\n");
//                  for (int i = 0; i < dwRead; i++)
//                  {
//                      printf("%i ",rBuffer[i]);
//                  }
//                  if (dwRead > 0)
//                      printf("\n");
//              }
//
//              fWaitingOnRead = FALSE;
//              break;
//          case WAIT_TIMEOUT:
//              //printf("Read Is Not Finshed Yet\n");
//              break;
//          
//          default:
//              printf("Error in WaitFor SingleObject\n");
//              //Sleep(3000);
//              break;
//          }
//      }
//      //Sleep(500);
//  }
//}

//void CTactor::ReadState(void)
//{
//  buffer[0] = 0x02;
//  buffer[1] = 0x00;
//  buffer[2] = 0x40;
//  buffer[3] = 0x00;
//  buffer[4] = 0xAB;
//  buffer[5] = 0x03;
//
//  SendToTactor(buffer, 6);
//}

//char * CTactor::GetFirmwareVersion(void)
//{  
//  buffer[0] = 0x02;
//  buffer[1] = 0x00;
//  buffer[2] = 0x42;
//  buffer[3] = 0x00;
//  buffer[4] = 0xA9;
//  buffer[5] = 0x03;
//
//  SendToTactor(buffer, 6);
//
//  return "Test";
//}

//void CTactor::ReadiVals(int tacNum)
//{
//  buffer[0] = 0x02;
//  buffer[1] = 0x00;
//  buffer[2] = 0x50;
//  buffer[3] = 0x01;
//
//  buffer[4] = tacNum;
//  
//  buffer[6] = 0x03;
//
//  buffer[5] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[4]^buffer[6]^0xEA;
//
//  SendToTactor(buffer, 7);
//
//  return; 
//}

//void CTactor::ReadSinVal1(void)
//{
//  buffer[0] = 0x02;
//  buffer[1] = 0x00;
//  buffer[2] = 0x60;
//  buffer[3] = 0x00;
//  buffer[4] = 0x8B;
//  buffer[5] = 0x03;
//
//  SendToTactor(buffer, 6);
//
//  return;
//}

//void CTactor::ReadSinVal2(void)
//{
//  buffer[0] = 0x02;
//  buffer[1] = 0x00;
//  buffer[2] = 0x61;
//  buffer[3] = 0x00;
//  buffer[4] = 0x8A;
//  buffer[5] = 0x03;
//
//  SendToTactor(buffer, 6);
//
//  return;
//}

//void CTactor::SetTactors(void)
//{
//  buffer[0] = 0x02;
//  buffer[1] = 0x00;
//  buffer[2] = 0x80;
//  buffer[3] = 0x01;
//  buffer[4] = 0x24;
//  buffer[6] = 0x03;
//  buffer[5] = buffer[0]^buffer[1]^buffer[2]^buffer[3]^buffer[4]^buffer[6]^0xEA;
//
//  SendToTactor(buffer, 7);
//
//  return;     
//}