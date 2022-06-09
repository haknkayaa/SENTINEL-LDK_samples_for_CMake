/////////////////////////////////////////////////////////////////////////////
//
// Demo program for Sentinel LDK licensing services
//
//
// Copyright (C) 2021 Thales Group. All rights reserved.
//
//
// Sentinel DEMOMA key required with feature 3 and 42 enabled
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdio.h>

#include <string.h>

#include "hasp_api_cpp.h"
#include "vendor_code.h"
#include "errorprinter.h"

using namespace std;

//Data for the demo of the encryption/decryption functionality
unsigned char data1[] = 
{
    0x74, 0x65, 0x73, 0x74, 0x20, 0x73, 0x74, 0x72,
    0x69, 0x6e, 0x67, 0x20, 0x31, 0x32, 0x33, 0x34
};
const unsigned int dataLen = sizeof(data1);

//The size of the part of the memory which 
//will be printed during the memory demo
const hasp_size_t numBytesToShow = 64;

void displayCloseMessage()
{
	cout<<endl<<"press ENTER to close the sample"<<endl;

    fflush(stdin);
    while (getchar() == EOF);
}

//Displays the content of mdata in a nice format with an ascii
//and a hexadecimal area.
void displayMemory(unsigned char *mdata, unsigned int mdataLen)
{
    //The number of bytes to be shown in each line
    const unsigned int lineLen = 16; 

    //This is printed before the first char in each line
    const char* margin = "     ";

    unsigned int i/*lines*/, ii/*chars*/;

    for(i=0; i<mdataLen; i+=lineLen)
    {
        ostringstream asciiWriter;
        ostringstream hexWriter;

        for(ii=i; ii<lineLen+i; ii++)
        {
            if(ii<mdataLen)
            {
                //Print the active char to the hex view
                unsigned int val = static_cast<unsigned int>(mdata[ii]);
                if(val<16)
                    hexWriter<<'0';
                hexWriter<<uppercase<<hex<<val<<' ';
                
                //... and to the ascii view
                if(mdata[ii]<32 || mdata[ii]>127) //Do not print some characters
                    asciiWriter<<'.';             //because this may create
                else                              //problems on UNIX terminals
                    asciiWriter<<mdata[ii];
            }
            else
            {
                hexWriter<<"   ";
                asciiWriter<<" ";
            }

            //Display a separator after each 4th byte
            if(!((ii-i+1)%4) && (ii-i+1<lineLen))
                hexWriter<<"| ";
        }

        //Write the line
        cout<<margin<<hexWriter.str()<<" ["<<asciiWriter.str()<<"]"<<endl;
    }
}

//Displays a ChaspTime in a nice format
void displayTime(const ChaspTime& haspTime)
{
    cout<<"     Sentinel key time: "<<haspTime.hour();
    cout<<':'<<haspTime.minute()<<':'<<haspTime.second()<<endl;
    cout<<"     Sentinel key date: "<<haspTime.day();
    cout<<'/'<<haspTime.month()<<'/'<<haspTime.year()<<" D/M/Y"<<endl;
}

int main()
{
    cout<<endl;
    cout<<"This is a simple demo program for Sentinel LDK licensing functions."<<endl;
    cout<<"Copyright (C) Thales Group. All rights reserved."<<endl<<endl;

    //Prints the error messages for the return values of the functions
    ErrorPrinter errorPrinter;

    //Used to hold the return value of the called functions
    haspStatus status;


    //Demonstrates the login to the default feature of a key
    //Searches both locally and remotely for it
    cout<<"login to default feature         : ";

    Chasp hasp1(ChaspFeature::defaultFeature());
    status = hasp1.login(vendorCode);
    errorPrinter.printError(status);

    if (!HASP_SUCCEEDED(status))
    {
        displayCloseMessage();
        return 1;
    }

    //Demonstrates how getSessionInfo can be used to create a scope
    //which can be later used for a login

    //This info template is used to create a scope which limits
    //the login to the current key
    std::string infoTemplate ="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
                              "<haspformat root=\"haspscope\">"
                              "<hasp>"
                              "<attribute name=\"id\" />"
                              "</hasp>"
                              "</haspformat>";

    //The output of the getInfo call
    std::string resultingScope;

    cout<<"get a login scope                : ";
    status = hasp1.getSessionInfo(infoTemplate, resultingScope);
    errorPrinter.printError(status);

    if (!HASP_SUCCEEDED(status))
    {
        displayCloseMessage();
        return 1;
    }

    if (HASP_SUCCEEDED(status))
        hasp1.logout();

    cout<<endl;

    //Demonstrates the login to a specific feature of a key
    //Searches only on the local pc for it
    cout<<"login to feature 42              : ";

    ChaspFeature feature42 = ChaspFeature::fromFeature(42);

    //This is a default scope. When it is used, the API
    //searches for keys both locally and remotely.
	const char* localScope = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
		"<haspscope>"
		"    <license_manager hostname =\"localhost\" />"
		"</haspscope>";

    Chasp hasp2(feature42);
    status = hasp2.login(vendorCode, localScope);
    errorPrinter.printError(status);

    if (HASP_SUCCEEDED(status))
        hasp2.logout();

    

    //Demonstrates the login to a specific feature of a key
    //Searches using the previously got scope
    cout<<"login to feature number 3        : ";

    ChaspFeature feature3 = ChaspFeature::fromFeature(3);

    Chasp hasp3(feature3);
    status = hasp3.login(vendorCode, resultingScope);
    errorPrinter.printError(status);

    if (!HASP_SUCCEEDED(status))
    {
    	displayCloseMessage();
        return 1;
    }

    cout<<endl;

    //Retrieves the attributes and additional information
    //of the connected Sentinel Key
    cout<<"get session info                 : ";

    std::string info;
    status = hasp3.getSessionInfo(Chasp::keyInfo(), info);
    errorPrinter.printError(status);
    cout<<endl;

    if (HASP_SUCCEEDED(status))
    {
        cout<<"==============="<<endl;
        cout<<info;
        cout<<"==============="<<endl;
    }

    cout<<endl;

    //Retrieves the memory size of the main memory file
	//you can also retrieve dynamic memory file size if you pass 
	//the dynamic memory file id to hasp3.getFile() function

    cout<<"retrieving the key's memory size : ";
    
    hasp_size_t size = 0;
    ChaspFile mainFile = hasp3.getFile(ChaspFile::fileReadWrite);

    status = mainFile.getFileSize(size);
    
    if (HASP_SUCCEEDED(status))
        cout<<"Sentinel key memory size is "<<size<<" bytes"<<endl;
    else
        errorPrinter.printError(status);

    //If the key holds a memory perform the memory test
    if(size)
    {
        //Reads a part of the memory into a buffer
		//you can also read dynamic memory file if you pass 
		//the dynamic memory file id to hasp3.getFile() function
        unsigned int fsize = (size < numBytesToShow) ? size : numBytesToShow;
        cout<<"reading "<<fsize<<" bytes from memory     : ";

        unsigned char *memdata = new unsigned char[fsize];
        
        status = mainFile.read(memdata, fsize);
        errorPrinter.printError(status);

        if (HASP_SUCCEEDED(status))
            displayMemory(memdata, fsize);
        else
        {
            delete[] memdata;
            displayCloseMessage();
            return 1;
        }
        cout<<endl;

        cout<<"incrementing every byte in memory buffer"<<endl<<endl;
        
        for (unsigned int i=0; i<fsize; i++) 
            (memdata[i])++;

        //Writes the changed buffer back to the key
		//you can also write dynamic memory file if you pass 
		//the dynamic memory file id to hasp3.getFile() function
        cout<<"writing "<<fsize<<" bytes to memory       : ";
        status = mainFile.write(memdata, fsize);
        errorPrinter.printError(status);

        if (!HASP_SUCCEEDED(status))
        {
            delete[] memdata;
            displayCloseMessage();
            return 1;
        }

        cout<<endl;
        memset(memdata, 0, fsize);

        //Reads the memory again in order to check the write function
        cout<<"reading "<<fsize<<" bytes from memory     : ";
        status = mainFile.read(memdata, fsize);
        errorPrinter.printError(status);

        if (HASP_SUCCEEDED(status))
            displayMemory(memdata, fsize);
        else
        {
            delete[] memdata;
            displayCloseMessage();
            return 1;
        }
        cout<<endl;
        
        delete[] memdata;
    }

    //Encryption demo
    cout<<"buffer before encryption         : "<<endl;
    displayMemory(data1, dataLen);

    cout<<"encrypting the buffer            : ";
    status = hasp3.encrypt(data1, dataLen);
    errorPrinter.printError(status);
    if (HASP_SUCCEEDED(status))
        displayMemory(data1, dataLen);

    cout<<"decrypting the buffer            : ";
    status = hasp3.decrypt(data1, dataLen);
    errorPrinter.printError(status);
    if (HASP_SUCCEEDED(status))
        displayMemory(data1, dataLen);
    
    cout<<endl;

    //Time access demo
    cout<<"reading current time and date    : ";
    ChaspTime haspTime;
    status = hasp3.getRtc(haspTime);
    errorPrinter.printError(status);
    if (HASP_SUCCEEDED(status))
        displayTime(haspTime);

    //Logout
    cout<<"logout from feature number 3     : ";
    status = hasp3.logout();
    errorPrinter.printError(status);
    cout<<endl;

	displayCloseMessage();
    
    return 0;
}
