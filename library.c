#include "library.h"

/*
 * macosRunComm - Executes a command on the local system with a popen implementation
 * The first line of output will be returned.
 *
 * command      - The command to execute
 *
 * Exit:
 * char array of first line of exec output returned.
 *
 */
char *macosRunComm(char *command){
    //Move input command to static char array
    char toexec[1000];
    if (strlen(command) < 1000)
        strcpy(toexec, command);
    else {
        puts("Error - command is too large.");
        exit(1);
    }
    char commout[1000];
    //move from static to dynamic to mitigate issue
    sprintf(commout, "%s", toexec);
    char *com = commout;
    char out[500];
    //exec with popen
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    //return first line of output
    return out;
}

/*
 * macosRunGetExit - Executes a command on the local system with a popen implementation and grabs the process exit code
 *
 * command      - The command to execute
 *
 * Exit:
 * The exit code of the executed process is returned.
 *
 */
int macosRunGetExit(char *command){
    //redirect stdout and stderr, echo process exit code to stdout
    char *com2 = " >/dev/null 2>/dev/null; echo $?";
    char com1[2400];
    if (strlen(command) < 2400)
        strcpy(com1, command);
    else {
        puts("Error - command is too large.");
        exit(1);
    }
    char commout[2400];
    sprintf(commout, "%s %s", com1, com2);
    char *com = commout;
    char out[2048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    //atoi to convert the returned char array as an int.
    return atoi(out);
}

/*
 * iosSendFile - Transmits A Local File On Host To Connected iDevice via SCP
 * @20/09/2020
 * filetosend   - File to transmit to device
 * remotedir    - The remote directory to place the file - a custom filename can also be defined
 * ipin         - IP of the iDevice
 * port         - Port to access SSH
 * passwd       - SSH passwd
 *
 * Exit Codes:
 * 0 -> File sent & stored - success
 * 1 -> Failed To ID SSHPASS in resources/sshpass OR couldn't send file succesfully...
 */
int iosSendFile(char *filetosend, char *remotedir, char *ipin, char *port,char *passwd){
    //Check file for read permissions
    if (access(filetosend, F_OK) != -1){
        char commout[800];
        if (access("resources/sshpass", F_OK) != -1) {
            sprintf(commout, "resources/sshpass -p %s scp -P %s -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" %s root@%s:%s%s >/dev/null 2>/dev/null \; echo $?", passwd, port, filetosend, ipin, remotedir,filetosend);
            char *com = commout;
            char out[400];
            FILE *shell = popen(com, "r");
            fgets(out, sizeof(out), shell);
            pclose(shell);
            if(DEBUG==1){
                printf("\nLOG -> COMMAND -> %s\n",com);
                printf("\nLOG -> iosSendFile EXIT CODE -> %d\n",atoi(out));
            }
            if (atoi(out)==0){
                return 0;
            } else{
                return 1;
            }
        } else{
            return 1;
        }
    }
}

/*
 * iosRecFile - Transmits A Remote File On The iOS Device To Local Host Using SCP
 *
 * remotesrcfile- File to retrieve from remote iOS Host
 * localdestfile- Local file destination
 * isTar        - Treat process differently if tar is in use.
 * ipin         - IP of the iDevice
 * port         - Port to access SSH
 * passwd       - SSH passwd
 *
 * Exit Codes:
 * 0 -> File retrieved and stored in the destination directory.
 * 1 -> Failed To ID SSHPASS in resources/sshpass OR couldn't retrieve file.
 */
int iosRecFile(char *remotesrcfile, char *localdestfile, int isTar,char *ipin, char *port,char *passwd){
    if(isTar==1){
        char tarcom[400];
        sprintf(tarcom,"find . -name '*.tar' -size +500M -exec tar -f {} -zxv -C SENSITIVE %s \\;", remotesrcfile);
        if(DEBUG==1){
            printf("%s",tarcom);
        }
        system(tarcom);
        //need tar support
        //retrieve file from tar instead using remotesrcfile reference
    } else if(isTar==0){
        if (access("resources/sshpass", F_OK) != -1) {
            char commout[800];
            sprintf(commout,"resources/sshpass -p %s scp -r -P %s -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@%s:%s %s >/dev/null 2>/dev/null \; echo $?",passwd,port,ipin,remotesrcfile, localdestfile);
            char *com = commout;
            char out[2048];
            FILE *shell = popen(com, "r");
            fgets(out, sizeof(out), shell);
            pclose(shell);
            if (atoi(out) == 0) {
                return 0;
            } else {
                return 1;
            }
        } else{
            return 1;
        }
    } else{
        return 1;
    }
}

/*
* iosRunGetExit - Run Command On Connected iDevice & Return Exit Code
*
* command - Command To Execute
* ipin         - IP of the iDevice
* port         - Port to access SSH
* passwd       - SSH passwd
*
* Exit Codes:
* 0 -> Command executed cleanly, no errors.
* OTHER -> Command-specific exit code.
*/
int iosRunGetExit(char *command, char *ipin, char *port, char *passwd){
    char commout[3000];
    if (access("resources/sshpass", F_OK) != -1) {
        sprintf(commout,
                "resources/sshpass -p %s ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@%s -p %s %s 2>/dev/null \; echo $?",
                passwd, ipin, port, command);
        if (DEBUG == 1) {
            printf("LOG -> EXECUTING -> %s\n", commout);
        }
        char *com = commout;
        char out[400];
        FILE *shell = popen(com, "r");
        fgets(out, sizeof(out), shell);
        pclose(shell);
        return atoi(out);
    } else{
        return 1;
    }
}


char *iosRunComm(char *command, char *ipin, char *port, char *passwd){
    char commout[3000];
    if (access("resources/sshpass", F_OK) != -1) {
        sprintf(commout,
                "resources/sshpass -p %s ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@%s -p %s %s 2>/dev/null",
                passwd, ipin, port, command);
        if (DEBUG == 1) {
            printf("LOG -> EXECUTING -> %s\n", commout);
        }
        char *com = commout;
        char out[400];
        FILE *shell = popen(com, "r");
        fgets(out, sizeof(out), shell);
        pclose(shell);
        return out;
    } else{
        return 1;
    }
}

/*
* iosMakeAPFSVol - Create A New APFS Volume On Connected iDevice
*
* volname       - Provide a name for the new volume.
* role          - Provide a role for the new volume, for example "r"
* ipin          - IP of the iDevice
* port          - Port to access SSH
* passwd        - SSH passwd
*
* Exit Codes:
* 0 -> Command executed cleanly, no errors.
* 1 -> Error creating volume.
*/
int iosMakeAPFSVol(char *volname,char *role, char *ipin, char *port, char *passwd){
    char *newfs = "newfs_apfs -o role=";// -A -v";
    char *args = " -A -v";
    char *tdisk = "/dev/disk0s1";
    char commout[240];
    sprintf(commout, "%s%s %s %s %s >/dev/null 2>/dev/null \; echo $?", newfs,role,args,volname,tdisk);
    if (atoi(iosRunGetExit(commout,ipin,port,passwd))==0){
        return 0;
    } else{
        return 1;
    }
}

/*
 * iosFetchDiskWriteAccessState - Checks for write access to system vol on the remote iDevice.
 *
 * ipin         - IP of the iDevice
 * port         - Port to access SSH
 * passwd       - SSH passwd
 *
 * Exit:
 * 0 -> Write Access To System Volume Is Enabled
 * 1 -> Couldn't write file to iDevice
 * 2 -> Error creating empty file on host system
 *
 */
int iosFetchDiskWriteAccessState(int isTar,char *ipin,char *port,char *passwd){
    system("touch .temp 2>/dev/null");
    if(isTar==1){
        return -1;
    } else{
        if (access(".temp", F_OK) != -1){
            // temporary file is available
            if(iosSendFile(".temp","/",ipin,port,passwd)==0){
                return 0;
            } else{
                //couldn't write file to iDevice
                return 1;
            }
        } else{
            //host system err
            return 2;
        }

    }
}

/*
 * iosFetchDiskAccessState - Checks for access to the remote iDevice. Writes an empty file to /var
 *
 * ipin         - IP of the iDevice
 * port         - Port to access SSH
 * passwd       - SSH passwd
 *
 * Exit:
 * 0 -> Access To System Volume Is Enabled
 * 1 -> Couldn't write file to iDevice
 * 2 -> Error creating empty file on host system
 *
 */
int iosFetchDiskAccessState(int isTar,char *ipin,char *port,char *passwd){
    system("touch .temp >/dev/null 2>/dev/null");
    if(isTar==1){
        return -1;
    } else{
        if (access(".temp", F_OK) != -1){
            // temporary file is available
            if(iosSendFile(".temp","/private/var/",ipin,port,passwd)==0){
                return 0;
            } else{
                //couldn't write file to iDevice
                return 1;
            }
        } else{
            //host system err
            return 2;
        }

    }
}

/*
 * iosMountVolume - Mounts a specific volume on the destination iDevice
 *
 * diskid       - The internal disk identifier, for example 'disk0s1s3'
 * mntpnt       - The destination mount point (should be an empty folder in the iDevice filesystem).
 * ipin         - IP of the iDevice
 * port         - Port to access SSH
 * passwd       - SSH passwd
 *
 * Exit:
 * 0 -> Volume Mounted Succesfully.
 * 1 -> Couldn't Mount Volume.
 *
 */
int iosMountVolume(char *diskid, char *mntpnt,char *ipin,char *port,char *passwd){
    char commout[500];
    sprintf(commout, "mount_apfs %s %s >/dev/null 2>/dev/null \;echo $?", diskid, mntpnt);
    if (iosRunGetExit(commout,ipin,port,passwd)==0){
        return 0;
    }else{
        return 1;
    }
}

/*
 * iosMakeDirectory - Mounts a specific volume on the destination iDevice
 *
 * absolutedirectory    - Absolute reference (full path) of the directory to create.
 * ipin         - IP of the iDevice
 * port         - Port to access SSH
 * passwd       - SSH passwd
 *
 * Exit:
 * 0 -> Directory Created Succesfully.
 * 1 -> Directory Either Couldn't Be Created (or already exists).
 *
 */
int iosMakeDirectory(char *absolutedirectory,char *ipin,char *port,char *passwd){
    char *com1 = ("mkdir");
    char *com2 = (">/dev/null 2>/dev/null\; echo \$?");
    char dircheck[500];
    sprintf(dircheck, "%s %s %s", com1, absolutedirectory, com2);
    if (iosRunGetExit(dircheck,ipin,port,passwd)==0){
        return 0;
    } else{
        return 1;
    }
}

/*
 * iosFetchDirectoryState - Should be used in collaboration with iosMakeDirectory...
 * to verify if directory exists prior to creation.
 *
 * dir          - Absolute reference (full path) of the directory to create.
 * ipin         - IP of the iDevice
 * port         - Port to access SSH
 * passwd       - SSH passwd
 *
 * Exit:
 * 0 -> Directory Does Not Exist...Creating!
 * 1 -> Directory Already Exists.
 *
 */
int iosFetchDirectoryState(char *dir,char *ipin,char *port,char *passwd){
    char *com1 = ("mkdir");
    char *com2 = (">/dev/null 2>/dev/null\; echo \$?");
    char dircheck[500];
    sprintf(dircheck, "%s %s %s", com1, dir, com2);
    if (iosRunGetExit(dircheck,ipin,port,passwd)==0){
        return 0;
    } else{
        return 1;
    }
}

/*
 * fetchPortState - Checks if port is open at a specific IP.
 *                  Should be used to identify wether it's worthwhile opening a connection to the iDevice.
 *
 * ipin         - IP of the iDevice
 * port         - Port to access SSH
 *
 * Exit:
 * 0 -> Port Open!
 * 1 -> Port Closed!
 *
 */
int fetchPortState(char *ipin,char *port){
    char *ping = "nc -z";
    char *silence = ">/dev/null 2>/dev/null";
    char command[800];
    sprintf(command,"%s %s %s %s",ping,ipin,port,silence);
    FILE * shell;
    shell = popen(command, "r");
    int r = pclose(shell);
    if(WEXITSTATUS(r)==0)
    {
        return 0;
    } else {
        return 1;
    }
}

/*
 * iosFetchECID - Use ideviceinfo to pull connected device ECID & return as char array.
 *
 * Exit:
 * char array containing ECID (if found)
 * 1 -> Unable to fetch ECID
 *
 */
char *iosFetchECID(){
    if (macosRunGetExit("ideviceinfo | grep UniqueChipID | grep -o '[0-9][0-9][0-9][0-9][0-9][0-9][0-9]*'")==0){
        return macosRunComm("ideviceinfo | grep UniqueChipID | grep -o '[0-9][0-9][0-9][0-9][0-9][0-9][0-9]*'");
    }
    else{
        return 1;
    }
}

/*
 * iosFetchPType - Use ideviceinfo to pull connected device Product Type & return as char array.
 *
 * Exit:
 * char array containing Product Type (if found)
 * 1 -> Unable to fetch Product Type
 *
 */
char *iosFetchPType(){
    if (macosRunGetExit("ideviceinfo | grep ProductType | grep -o 'iPhone.*'")==0){
        return macosRunComm("ideviceinfo | grep ProductType | grep -o 'iPhone.*'");
    }
    else{
        return 1;
    }
}

/*
 * iosFetchBoardConfig - Use ideviceinfo to pull connected device Board Configuration & return as char array.
 *
 * Exit:
 * char array containing Board Configuration (if found)
 * 1 -> Unable to fetch Board Configuration
 *
 */
char *iosFetchBoardConfig(){
    if (macosRunGetExit("ideviceinfo | grep HardwareModel | cut -f2- -d' '")==0){
        return macosRunComm("ideviceinfo | grep HardwareModel | cut -f2- -d' '");
    }
    else{
        return 1;
    }
}

/*
 * iosFetchSHSH - Use tsschecker to fetch SHSH2 restore blobs.
 * tsschecker must be present in working folder for functiont to exec.
 *
 * pytype       - Device Product Type
 * eciddec      - ECID (as decimal value)
 * boardconf    - Device Board Configuration
 *
 *
 * Exit:
 * 0 -> SHSH2 exported to working folder.
 * 1 -> Unable to fetch SHSH2.
 *
 */
int *iosFetchSHSH(char *ptype, char *eciddec, char *boardconf){
    FILE *fileout;
    //check tsschecker is present
    if((fileout = fopen("tsschecker","r"))!=NULL)
    {
        fclose(fileout);
        char comm[1200];
        sprintf(comm,"./tsschecker -s -l -e %s -d %s --boardconfig %s", eciddec, ptype, boardconf);
        printf("%s",comm);
        if(macosRunGetExit(comm)==0){
            return 0;
        }
    } else{
        return 1;
    }
}


//dual-boot related stuff - undocumented for now! (will work on it soon) <3

//patch fstab locally after using ios_rec_f to pull fstab
int ios_fstab_p(char *fstabloc,char *fromval,char *toval){
    //for command to patch concat
    char comm[900];
    sprintf(comm,"sed -i '' 's/%s/%s/g' %s",fromval, toval, fstabloc);
    return macosRunComm(comm);
}

//example ios_sep_mov("/mnt1");
int ios_sep_c(char *newmnt,char *ipin,char *port,char *passwd){
    char mntcheck[900];
    char comm[900];
    sprintf(mntcheck,"df \| grep mnt1 >/dev/null 2>/dev/null \; echo $?",newmnt);
    if (iosRunGetExit(mntcheck,ipin,port,passwd)==0){
        sprintf(comm,"cp -a /usr/standalone/firmware/sep* %s/usr/standalone >/dev/null 2>/dev/null \; echo $?",newmnt);
        return iosRunGetExit(comm,ipin,port,passwd);
    } else{
        return 1;
    }
}

//^
int ios_bb_c(char *newmnt,char *ipin,char *port,char *passwd){
    char mntcheck[900];
    char comm[900];
    sprintf(mntcheck,"df \| grep mnt1 >/dev/null 2>/dev/null \; echo $?",newmnt);
    if (iosRunGetExit(mntcheck,ipin,port,passwd)==0){
        sprintf(comm,"cp -a /usr/local %s/usr/local >/dev/null 2>/dev/null \; echo $?",newmnt);
        return 0;
    } else{
        return 1;
    }
}

