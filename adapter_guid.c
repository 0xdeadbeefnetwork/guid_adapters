#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <string.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")

BOOL GetInformationFromAdapters(WCHAR* matchDesc, char* pOutAdapterStr) {
    DWORD outSize = 0;
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG status = 0;
    BOOL result = FALSE;

    // Get the buffer size needed.
    status = GetAdaptersAddresses(NULL, NULL, NULL, pAddresses, &outSize);
    if (status == ERROR_BUFFER_OVERFLOW) {
        pAddresses = (PIP_ADAPTER_ADDRESSES)malloc(outSize);
        if (pAddresses == NULL) {
            printf("Memory allocation failed for adapter addresses buffer\n");
            return FALSE;
        }

        // Retrieve adapter addresses.
        status = GetAdaptersAddresses(NULL, NULL, NULL, pAddresses, &outSize);
        if (status == NO_ERROR) {
            PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
            while (pCurrAddresses) {
                if (matchDesc == NULL || pOutAdapterStr == NULL) {
                    printf("Description: %S\n", pCurrAddresses->Description);
                    printf("Adapter: %s\n", pCurrAddresses->AdapterName);
                    printf("FriendlyName: %S\n", pCurrAddresses->FriendlyName);
                    puts("\n=============\n");
                }
                else {
                    if (wcsstr(pCurrAddresses->Description, matchDesc)) {
                        memset(pOutAdapterStr, 0x00, 256);
                        memcpy(pOutAdapterStr, pCurrAddresses->AdapterName, strlen(pCurrAddresses->AdapterName));
                        result = TRUE;
                        break;
                    }
                }
                pCurrAddresses = pCurrAddresses->Next;
            }
        }
        else {
            printf("GetAdaptersAddresses failed with error: %lu\n", status);
        }

        free(pAddresses);
    }
    else {
        printf("GetAdaptersAddresses failed with error: %lu\n", status);
    }

    return result;
}

int main() {
    char adapterName[256];
    if (GetInformationFromAdapters(NULL, adapterName)) {
        printf("Adapter found: %s\n", adapterName);
    }
    else {
        printf("Done.");
    }

    return 0;
}
