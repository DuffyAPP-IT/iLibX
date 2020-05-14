#ifndef ILIBX_LIBRARY_H
#define ILIBX_LIBRARY_H
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
//FS mgmt related functions - utilising sshpass,scp - using default iOS pw.
int ios_makevol_apfs(char *volname, char *role); //done
int ios_mountdisk(char *diskid, char *mntpnt); //done
int ios_makedir(char *absolutedirectory); //done
int ios_send_f(char *filetosend, char *remotedir);
int ios_rec_f(char *remotefiledir, char *localfname);
int ios_checkdirexists(char *dir);

//exec commands, pull exit codes and other data from connected device.
int ios_fetch_access(); //done
char *ios_run_comm(char *command, char *pass); //done
int macos_run_comm(char *command); //done
char *macos_run_str(char *command); //done
int *ios_blob_fetch(char *ptype, char *eciddec, char *boardconf); //done
char *ios_ecid_fetch(); //done
char *ios_ptype_fetch(); //done
char *ios_bconf_fetch(); //done
int ios_ver_check(); //done

//dual-boot related functions
int ios_asr_process(char *rootfsdmg, char *rootfsoutdmg);
int ios_fstab_p(char *fstabloc,char *fromval,char *toval);
int ios_sep_c(char *newmnt);
int ios_bb_c(char *newmnt);
int ios_dtre_patch(int iosver,char *dtrein, char *dtreout, char *IM4M);

#endif //ILIBX_LIBRARY_H
