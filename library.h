#ifndef ILIBX_LIBRARY_H
#define ILIBX_LIBRARY_H

#define DEBUG 0

//External Libs
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/file.h>
#include <zconf.h>

//iLibX


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
char *macosRunComm(char *command);

/*
 * macosRunGetExit - Executes a command on the local system with a popen implementation and grabs the process exit code
 *
 * command      - The command to execute
 *
 * Exit:
 * The exit code of the executed process is returned.
 *
 */
int macosRunGetExit(char *command);

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
int iosSendFile(char *filetosend, char *remotedir, char *ipin, char *port,char *passwd);

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
int iosRecFile(char *remotesrcfile, char *localdestfile, int isTar,char *ipin, char *port,char *passwd);

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
int iosRunGetExit(char *command, char *ipin, char *port, char *passwd);

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
int iosMakeAPFSVol(char *volname,char *role, char *ipin, char *port, char *passwd);

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
int iosFetchDiskWriteAccessState(int isTar,char *ipin,char *port,char *passwd);

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
int iosFetchDiskAccessState(int isTar,char *ipin,char *port,char *passwd);

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
int iosMountVolume(char *diskid, char *mntpnt,char *ipin,char *port,char *passwd);

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
int iosMakeDirectory(char *absolutedirectory,char *ipin,char *port,char *passwd);

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
int iosFetchDirectoryState(char *dir,char *ipin,char *port,char *passwd);

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
int fetchPortState(char *ipin,char *port);

/*
 * iosFetchECID - Use ideviceinfo to pull connected device ECID & return as char array.
 *
 * Exit:
 * char array containing ECID (if found)
 * 1 -> Unable to fetch ECID
 *
 */
char *iosFetchECID();

/*
 * iosFetchPType - Use ideviceinfo to pull connected device Product Type & return as char array.
 *
 * Exit:
 * char array containing Product Type (if found)
 * 1 -> Unable to fetch Product Type
 *
 */
char *iosFetchPType();

/*
 * iosFetchBoardConfig - Use ideviceinfo to pull connected device Board Configuration & return as char array.
 *
 * Exit:
 * char array containing Board Configuration (if found)
 * 1 -> Unable to fetch Board Configuration
 *
 */
char *iosFetchBoardConfig();

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
int *iosFetchSHSH(char *ptype, char *eciddec, char *boardconf);

//legacy iLibX functions (i'll document soon!)
int ios_fstab_p(char *fstabloc,char *fromval,char *toval);
int ios_sep_c(char *newmnt,char *ipin,char *port,char *passwd);
int ios_bb_c(char *newmnt,char *ipin,char *port,char *passwd);


#endif //ILIBX_LIBRARY_H
