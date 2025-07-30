
/*** Includes ***/

#include <bits/endian.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/structs.h"
#include "include/constants.h"

/*** Structs and values ***/

FILE *end_file;
struct SUBNET subnet;
uint8_t value = 0;

/*** Core functions ***/

uint32_t moveBits(uint32_t bits) {
    uint32_t convertedBits = 0;
    convertedBits |= (bits & 0x000000ff) << 24;
    convertedBits |= (bits & 0x0000ff00) << 8;
    convertedBits |= (bits & 0x00ff0000) >> 8;
    convertedBits |= (bits & 0xff000000) >> 24;
    return convertedBits;
}

int ifNetworkAdress(){
    if ( (subnet.ip & subnet.mask) == subnet.ip ) {
        if ( (subnet.ip & 0xE0000000) == 0xE0000000 ) {
            printf("That's class D or E address, not a network.\n");
            return 0;
        } else if ( ((subnet.ip & 0xE0000000) == 0xC0000000) && subnet.mask >= 0xffffff00) {
            return 1;
        } else if ( ((subnet.ip & 0xC00000) == 0x800000) && subnet.mask >= 0xffff0000 ) {
            return 1;
        } else if ( ((subnet.ip & 0x800000) == 0) && subnet.mask >= 0xff000000) {
            return 1;
        }
    } else {
        printf("Not a valid network address.\n");
        return 0;
    }
    return 0;
}

uint32_t maskGen(int bit) {
    uint32_t result = 2147483648;
    for (int i = 0; i < bit; i++) {
        result = result / 2;
    }
    return result;
}

int search(int s) {
    end_file = fopen("ipTable", "rb+");
    int index = 0;
    struct SUBNET temp_Search;
    if (end_file != NULL) {
        while(fread(&temp_Search, sizeof(temp_Search), 1, end_file) == 1) {

            if ( s == 3 && temp_Search.mask == 0 ) {
                return index;
            }

            if ( s == 4 && temp_Search.mask != 0 ) {
                printf("%d. Network Address: ", index+1);

                temp_Search.ip = moveBits(temp_Search.ip);
                temp_Search.mask = moveBits(temp_Search.mask);

                for (int i = 0; i < 4; i++) {
                    printf("%u.", temp_Search.ip & 0xff);
                    temp_Search.ip = temp_Search.ip >> 8;
                }
                printf(" | Mask: ");
                for (int i = 0; i < 4; i++) {
                    printf("%u.", temp_Search.mask & 0xff);
                    temp_Search.mask = temp_Search.mask >> 8;
                }
                printf("\n");
                index += 1;
            } else if ((temp_Search.ip == subnet.ip) && (temp_Search.mask == subnet.mask) && (s == 2) && (temp_Search.mask != 0)) {

                fseek(end_file, -sizeof(temp_Search), SEEK_CUR);

                temp_Search.ip = 0;
                temp_Search.mask = 0;

                fwrite(&temp_Search, sizeof(temp_Search), 1, end_file);
                fclose(end_file);
                return 1;

            } else if (((subnet.ip & temp_Search.mask) == temp_Search.ip) && temp_Search.mask != 0) {
                fclose(end_file);
                if (s == 1) {
                    printf("This host is in subnet: ");
                    temp_Search.ip = moveBits(temp_Search.ip);
                    temp_Search.mask = moveBits(temp_Search.mask);
                    for (int i = 0; i < 4; i++) {
                        printf("%u.", temp_Search.ip & 0xff);
                        temp_Search.ip = temp_Search.ip >> 8;
                    }
                    printf("\nWith mask: ");
                    for (int i = 0; i < 4; i++) {
                        printf("%u.", temp_Search.mask & 0xff);
                        temp_Search.mask = temp_Search.mask >> 8;
                    }
                    printf("\n");
                }
                return 1;
            }
            if (s == 3) {
                index = index + 1;
            }
        }
    } else {
        exit(4);
    }
    fclose(end_file);
    if (s == 1) {
        printf("Host not found in any subnet in file.\n");
    }
    if (s == 3) return index;
    return 0;
}

// Finding host in subnet and checking for conflict in subnets

/*** Complex functions ***/

int add() {
    end_file = fopen("ipTable", "ab");
    fclose(end_file);
    // Before it has to check if subnet is not in file already
    if (ifNetworkAdress()) {
        if (search(0) == 0) {
            int index = search(3);
            end_file = fopen("ipTable", "r+");

            fseek(end_file, index * 8, SEEK_SET);
            fwrite(&subnet, sizeof(subnet), 1, end_file);
            printf("Network Address added successfuly.\n");
            fclose(end_file);
        } else {
            printf("Subnet already in file.\n");
        }
    } else {
        exit(3);
    }
    return 0;
}

/*** Start of program ***/

int main(int argc, char *argv[]){

    for (int i = 1; i < argc; i ++){
        char *param = argv[i];
        switch (param[0]) {
            case '-': {
                switch (param[1]) {
                    case 'i': { // Checks backwards from value
                        value = value + 1;
                        if (argv[i+1]) {
                            break;
                        } else {
                            return 1;
                        }
                    }
                    case 'm': { // Checks backwards from value
                        value = value + 2;
                        break;
                    }
                    case 'a': {
                        value = value + 4;
                        break;
                    }
                    case 's': {
                        value = value + 8;
                        break;
                    }
                    case 'r': {
                        value = value + 16;
                        break;
                    }
                    case 'l': {
                        value = value + 32;
                        break;
                    }
                    case 'h': {
                        printf("%s",help);
                        return 0;
                    }
                    case 'v': {
                        printf("%s",version);
                        return 0;
                    }
                    default: {
                        printf("Wrong option\n%s", help);
                        return 1;
                    }
                }
                break;
            }
            default: {
                if (argv[i-1][0] == '-' && argv[i-1][1] == 'i') {
                    int octets = 0;
                    char *tempParam =strdup(param); // Copy of parameter
                    char *tempIP = strtok(param, ".");
                    while (tempIP != NULL) { // Checking if parameter has correct structure
                        if ((atoi(tempIP) == 0 && octets == 0) || atoi(tempIP) < 0 || atoi(tempIP) >= 256) {
                            return 2;
                        }
                        tempIP = strtok(NULL, ".");
                        octets = octets + 1;
                    }
                    if (octets == 4) { // If correct structure, save it
                        tempIP = strtok(tempParam, ".");

                        subnet.ip = subnet.ip + atoi(tempIP)*256*256*256;
                        tempIP = strtok(NULL, ".");
                        subnet.ip = subnet.ip + atoi(tempIP)*256*256;
                        tempIP = strtok(NULL, ".");
                        subnet.ip = subnet.ip + atoi(tempIP)*256;
                        tempIP = strtok(NULL, ".");
                        subnet.ip = subnet.ip + atoi(tempIP);

                    } else {
                        return 2;
                    }
                } else if (argv[i-1][0] == '-' && argv[i-1][1] == 'm') {
                    if (atoi(param) <= 32 || atoi(param) > 0){
                        for (int i = 0; i < atoi(param); i++) {
                            subnet.mask = subnet.mask + maskGen(i);
                        }
                    }
                } else {
                    return 1;
                }
                break;
            }
        }
    }


    if (  ((value & 4) == 4) ^ ((value & 8) == 8) ^ ((value & 16) == 16) ^ ((value & 32) == 32) ) {
        if ((value & 4) == 4) {
            add();
        } else if ((value & 8) == 8) {
            search(1);
        } else if ((value & 16) == 16) {
            if(search(2) == 1) {
                printf("Subnet deleted successfuly.\n");
            } else {
                printf("Subnet not found in file.\n");
            }
        } else if ((value & 32) == 32) {
            search(4);
        }
    } else if ( value == 0 ){
        printf("%s", help);
        return 0;
    } else {
        printf("Contradicting options (ex. using both -r and -a)\n");
        return 1;
    }


    return 0;
}
