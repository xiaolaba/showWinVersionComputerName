// minGW
// C:\MinGW\msys\1.0\home\user0\win10version_hostname

// 2018-12-14
// by xiaolaba

// 2024-11-12
// gcc main.c -o showWinVersionComputerNameWin11.exe
/*
Windows 11 officially starts at build 10.0.22000 (first release in October 2021).
If CurrentBuildNumber >= 22000, we display "Windows 11" as the version name 
(overriding what ProductName says which sometimes still says "Windows 10" on upgraded systems).
*/

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>  // For atoi

int main() {
    // Get real hostname
    char realHostname[256];
    DWORD hostnameSize = sizeof(realHostname);
    BOOL hasHostname = GetComputerNameA(realHostname, &hostnameSize);

    // Get username
    char username[256];
    DWORD usernameSize = sizeof(username);
    BOOL hasUsername = GetUserNameA(username, &usernameSize);

    // Variables for Windows info
    char displayVersion[128] = "Unknown";
	char productName[128] = "TBA";
    char currentBuild[64] = "0";
    char ubrStr[64] = "0";
    int buildNumber = 0;
	DWORD bufSize;


	//printf("productName default: %s\n", productName);


    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                     "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                     0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        bufSize = sizeof(displayVersion);
        RegQueryValueExA(hKey, "DisplayVersion", NULL, NULL, (BYTE*)displayVersion, &bufSize);
		
		bufSize = sizeof(productName);
		RegQueryValueExA(hKey, "ProductName", NULL, NULL, (BYTE*)productName, &bufSize);		
	
		//printf("productName in registry: %s\n", productName);
		

        bufSize = sizeof(currentBuild);
        RegQueryValueExA(hKey, "CurrentBuildNumber", NULL, NULL, (BYTE*)currentBuild, &bufSize);
        if (strlen(currentBuild) == 0) {
            RegQueryValueExA(hKey, "CurrentBuild", NULL, NULL, (BYTE*)currentBuild, &bufSize);
        }


		// issue, https://www.dell.com/support/kbdoc/en-us/000193571/windows-11-incorrect-os-version-reported
		// soltuion, Here's the updated code that now also displays the OS Build number 
		// (the full build like 26100.3185 or 22631.4754, matching what winver shows: CurrentBuildNumber.UBR).
		// UBR = Update Build Revision
        DWORD ubr = 0;
        bufSize = sizeof(ubr);
        if (RegQueryValueExA(hKey, "UBR", NULL, NULL, (BYTE*)&ubr, &bufSize) == ERROR_SUCCESS) {
            sprintf(ubrStr, "%u", ubr);
        }

        RegCloseKey(hKey);
    }

    // Convert build number to int for comparison
    buildNumber = atoi(currentBuild);
	
	// Show username
    if (hasUsername) {
        printf("Username: %s\n", username);
    } else {
        printf("Username: Failed to get username.\n");
    }

    // Special case: hostname == "T123"
    if (hasHostname && strcmp(realHostname, "T123") == 0) {
        printf("computerName: my PC\n");
        printf("Windows Version: Windows 11\n");
        printf("OS Build: %s.%s\n", currentBuild, ubrStr);  // Still shows real build
		
    } else {
        // Normal case
        if (hasHostname) {
            printf("computerName: %s\n", realHostname);
        } else {
            printf("computerName: Failed to get hostname.\n");
        }

        printf("OS Build (CurrentBuildNumber): %s.%s\n", currentBuild, ubrStr);


        // Decide version name: if build >= 22000 -> Windows 11
        if (buildNumber >= 22000) {
//        if (buildNumber >= 19000) {

			printf("Windows 11 (productName: %s, displayVersion: %s)\n\n", productName, displayVersion);

			printf("******\n"
			       "ver/winver reported diff verison of win11.\n"		
			       "see https://www.dell.com/support/kbdoc/en-us/000193571/windows-11-incorrect-os-version-reported\n"
			       "Windows OS build 10.0.22000 or above is Win11\n"
			       "Why ProductName is 'Windows 10:\n"
			       "  Microsoft keeps it to prevent compatibility issues with \n"
				   "  older software that expects specific strings, so rely on CurrentBuildNumber\n"
				   "  or APIs instead for true Win11 detection\n");			
			
        } else {
            // For older builds (Windows 10 or earlier)
            // Optionally read ProductName if needed, but usually safe to assume Windows 10 below 22000
			printf("Windows Version (by productName, displayVersion) : %s (%s)\n", productName, displayVersion);
        }
		
		
		
	
    }




		


    printf("\nPress any key to exit...");
    _getch();

    return 0;
}