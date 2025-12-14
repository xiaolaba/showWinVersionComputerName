// minGW
// C:\MinGW\msys\1.0\home\user0\win10version_hostname
// gcc main.c -o showWinVersionComputerName.exe
// 2018-12-14
// by xiaolaba


#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

int main() {
    // Get real hostname
    char realHostname[256];
    DWORD hostnameSize = sizeof(realHostname);
    BOOL hasHostname = GetComputerNameA(realHostname, &hostnameSize);

    // Get username
    char username[256];
    DWORD usernameSize = sizeof(username);
    BOOL hasUsername = GetUserNameA(username, &usernameSize);

    // Special case: hostname == "DEMO"
//    if (hasHostname && strcmp(realHostname, "LAPTOP1") == 0) {
//        printf("Hostname (computerName): LAPTOP1\n");
//        printf("Windows Version: win10\n");
//		  printf("Username: LAPTOP1_user")
    if (hasHostname && strcmp(realHostname, "LAPTOP") == 0) {	
		printf("DEMO only");
    } else {
        // Normal case: show real hostname and version
        if (hasHostname) {
            printf("Hostname: %s\n", realHostname);
        } else {
            printf("Hostname: Failed to get hostname.\n");
        }

        // Get Windows version from registry
        HKEY hKey;
        char displayVersion[128] = "Unknown";
        char productName[128] = "Windows";
        DWORD bufSize;

        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                         "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                         0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            
            bufSize = sizeof(displayVersion);
            RegQueryValueExA(hKey, "DisplayVersion", NULL, NULL, (BYTE*)displayVersion, &bufSize);

            bufSize = sizeof(productName);
            RegQueryValueExA(hKey, "ProductName", NULL, NULL, (BYTE*)productName, &bufSize);

            RegCloseKey(hKey);
        }

        printf("Windows Version: %s (%s)\n", productName, displayVersion);
		
		// Always show the real username
		if (hasUsername) {
			printf("Username: %s\n", username);
		} else {
			printf("Username: Failed to get username.\n");
		}		
		
    }



    // Wait for key press
    printf("\nPress any key to exit...");
    _getch();

    return 0;
}