#ifndef ILIBX_LIBRARY_H
#define ILIBX_LIBRARY_H

//FS mgmt related functions - utilising sshpass,scp - using default iOS pw.
int ios_makevol_apfs(char *volname, char *role);
int ios_mountdisk(char *diskid, char *mntpnt);
int ios_makedir(char *absolutedirectory);
int ios_send_f(char *filetosend, char *remotedir);
int ios_rec_f(char *remotefiledir, char *localfname);
int ios_checkdirexists(char *dir);

//exec commands, pull exit codes and other data from connected device.
char *ios_runc(char *command);
int *ios_blob_fetch(char *ptype, char *eciddec, char *boardconf);
char *ios_ecid_grab();
char *ios_ptype_grab();
char *ios_bconf_grab();
int ios_ver_check();

//dual-boot related functions
int ios_asr_process(char *rootfsdmg, char *rootfsoutdmg);
int ios_fstab_p(char *fstabloc,char *fromval,char *toval);
int ios_sep_c(char *newmnt);
int ios_bb_c(char *newmnt);
int ios_dtre_patch(int iosver,char *dtrein, char *dtreout, char *IM4M);

#endif //ILIBX_LIBRARY_H
