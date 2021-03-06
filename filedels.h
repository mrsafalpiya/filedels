#ifndef filedels_h
#define filedels_h

/* macros */
#define SIZE_FILE_LARGE 1024
#define SIZE_FILE_MED 512
#define SIZE_FILE_SMALL 128
#define PATH_TEMP_DIR "/tmp/flashcard/"

/* structs */
typedef struct {
	char path_file_provided[SIZE_FILE_MED];
	char path_file_full[SIZE_FILE_LARGE];
	char path_file_name[SIZE_FILE_SMALL];
	char path_temp[SIZE_FILE_SMALL];
	int per_exist;
	int per_readable;
	int per_writable;
	int per_temp_exist;
	long int nol_file;
	long int nol_temp;
} FileInfo;

/* function declarations */
FileInfo filedels_new(char provided_file[SIZE_FILE_SMALL]);
void filedels_debug(FileInfo *file_info);

#endif
