#include "library.h"
#include <stdio.h>

//FS mgmt related functions - utilising sshpass,scp - using default iOS pw.

//Pass a char array containing the volume name to create.
// ios_makevol_apfs("SystemB","r");
int ios_makevol_apfs(char *volname,char *role){
    char *newfs = "newfs_apfs -o role=";// -A -v";
    char *args = " -A -v";
    char *tdisk = "/dev/disk0s1";
    char commout[240];
    sprintf(commout, "%s%s %s %s %s >/dev/null 2>/dev/null \; echo $?", newfs,role,args,volname,tdisk);
    if (atoi(ios_run_comm(commout,"alpine"))==0){
        return 0;
    }else{
        return 1;
    }
}

//return 1 on error and 0 on success pass diskid in the form disk0s1s3 and mntpnt as absolute directory
//ios_mountdisk("disk0s1s3","/mnt1");
int ios_mountdisk(char *diskid, char *mntpnt){
    char commout[500];
    sprintf(commout, "mount_apfs %s %s >/dev/null 2>/dev/null \;echo $?", diskid, mntpnt);
    if (atoi(ios_run_comm(commout,"alpine"))==0){
        return 0;
    }else{
        return 1;
    }
}

//Make a directory.
//ios_makedir("/mnt1");
int ios_makedir(char *absolutedirectory){
        if (ios_fetch_access() == 0){
        char *com1 = ("mkdir");
        char *com2 = (">/dev/null 2>/dev/null\; echo \$?");
        char dircheck[500];
        sprintf(dircheck, "%s %s %s", com1, absolutedirectory, com2);
        if (atoi(ios_run_comm(dircheck,"alpine"))==0){
            return 0;
        } else{
            return 1;
        }
    }
}


//send file via scp
//ios_send_f("fstab","/mnt1/private/etc");
int ios_send_f(char *filetosend, char *remotedir){
        FILE *fileout;
        if((fileout = fopen(filetosend,"r"))!=NULL)
        {
            fclose(fileout);
            char commout[800];
            sprintf(commout, "sshpass -p alpine scp -P 2222 %s root@127.0.0.1:%s \; echo $?", filetosend, remotedir);
            char *com = commout;
            char out[2048];
            FILE *shell = popen(com, "r");
            fgets(out, sizeof(out), shell);
            pclose(shell);
            if (atoi(out)==0){
                return 0;
            } else{
                return 1;
            }
    }

}

//pull file from device using scp
//ios_send_f("/mnt1/private/etc/fstab","fstab");
int ios_rec_f(char *remotefiledir, char *localfname){
        char commout[800];
        sprintf(commout, "sshpass -p alpine scp -r -P 2222 root@127.0.0.1:%s %s \; echo $?", remotefiledir, localfname);
        char *com = commout;
        char out[2048];
        FILE *shell = popen(com, "r");
        fgets(out, sizeof(out), shell);
        pclose(shell);
        if (atoi(out)==0){
            return 0;
        } else{
            return 1;
        }
}


//returns 0 on success 1 if dir already exists
int ios_checkdirexists(char *dir){
    char *com1 = ("mkdir");
    char *com2 = (">/dev/null 2>/dev/null\; echo \$?");
    char dircheck[500];
    sprintf(dircheck, "%s %s %s", com1, dir, com2);
    if (atoi(ios_run_comm(dircheck,"alpine"))==0){
        return 0;
    } else{
        return 1;
    }
}






//Exec Commands etc..
//pass ip address of device & relevant port.
// ios_fetch_access("127.0.0.1","2222");
int ios_fetch_access(char *ipin,char *port){
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

//remember escape key for pipes and strtok for removing newlines to prevent multiple commands being executed.
//returns char array of output. redirect output to silent and add '; echo $?' to return exit code.
char *ios_run_comm(char *command,char *pass){
        char *com1 = "sshpass -p";
        char *com2 = "ssh root@127.0.0.1 -p 2222";
        char commout[2400];
        sprintf(commout, "%s %s %s %s", com1, pass, com2, command);
        char *com = commout;
        char out[4048];
        FILE *shell = popen(com, "r");
        fgets(out, sizeof(out), shell);
        pclose(shell);
        return out;
    }

//returns exit code of command run on mac.
int macos_run_comm(char *command){
    char *com2 = ">/dev/null 2>/dev/null;echo $?";
    char com1[2400];
    strcpy(com1, command);
    char commout[2400];
    sprintf(commout, "%s %s", com1, com2);
    char *com = commout;
    char out[2048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return atoi(out);
}

//returns output of command run on mac.
char *macos_run_str(char *command){
    char com2[1024];
    strcpy(com2, command);
    char commout[1024];
    sprintf(commout, "%s", com2);
    char *com = commout;
    char out[4096];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return out;
}

//return ecid as char array
char *ios_ecid_grab(){
    if (macos_run_comm("ideviceinfo | grep UniqueChipID | grep -o '[0-9][0-9][0-9][0-9][0-9][0-9][0-9]*'")==0){
        return macos_run_str("ideviceinfo | grep UniqueChipID | grep -o '[0-9][0-9][0-9][0-9][0-9][0-9][0-9]*'");
    }
    else{
        return 1;
    }
}

//returns product type as char array
char *ios_ptype_grab(){
    if (macos_run_comm("ideviceinfo | grep ProductType | grep -o 'iPhone.*'")==0){
        return macos_run_str("ideviceinfo | grep ProductType | grep -o 'iPhone.*'");
    }
    else{
        return 1;
    }
}

//return board config as char array
char *ios_bconf_grab(){
    if (macos_run_comm("ideviceinfo | grep HardwareModel | cut -f2- -d' '")==0){
        return macos_run_str("ideviceinfo | grep HardwareModel | cut -f2- -d' '");
    }
    else{
        return 1;
    }
}

//ios_blob_fetch(ios_ecid_fetch(),ios_ptype_fetch(),ios_bconf_fetch())
int *ios_blob_fetch(char *ptype, char *eciddec, char *boardconf){
    FILE *fileout;
    //check tsschecker is present
    if((fileout = fopen("tsschecker","r"))!=NULL)
    {
        fclose(fileout);
        char comm[1200];
        sprintf(comm,"./tsschecker -s -l -e %s -d %s --boardconfig %s", eciddec, ptype, boardconf);
        if(macos_run_comm(comm)==0){
            return 0;
        }
    } else{
        return 1;
    }
}

//returns major ios version as int
int ios_ver_check(){
    //add checks to pull iOS version, maybe gotta use grep somewhere, uname?
    //remember to atoi the output when returning as int.
    //return should be 9,10,11,12,13 as examples.
    return atoi(ios_run_comm("sw_vers \| grep ProductVersion \| grep -o '[1][0-4]'","alpine"));
}





//dual-boot related stuff

//patch fstab locally after using ios_rec_f to pull fstab
int ios_fstab_p(char *fstabloc,char *fromval,char *toval){
    //for command to patch concat
    char comm[900];
    sprintf(comm,"sed -i '' 's/%s/%s/g' %s",fromval, toval, fstabloc);
    return macos_run_comm(comm);
}

//example ios_sep_mov("/mnt1");
int ios_sep_c(char *newmnt){
    char mntcheck[900];
    char comm[900];
    sprintf(mntcheck,"df \| grep mnt1 >/dev/null 2>/dev/null \; echo $?",newmnt);
    if (atoi(ios_run_comm(mntcheck,"alpine"))==0){
        sprintf(comm,"cp -a /usr/standalone/firmware/sep* %s/usr/standalone >/dev/null 2>/dev/null \; echo $?",newmnt);
        return atoi(ios_run_comm(comm,"alpine"));
    } else{
        return 1;
    }
}

//^
int ios_bb_c(char *newmnt){
    char mntcheck[900];
    char comm[900];
    sprintf(mntcheck,"df \| grep mnt1 >/dev/null 2>/dev/null \; echo $?",newmnt);
    if (atoi(ios_run_comm(mntcheck,"alpine"))==0){
        sprintf(comm,"cp -a /usr/local %s/usr/local >/dev/null 2>/dev/null \; echo $?",newmnt);
        return 0;
    } else{
        return 1;
    }
}

