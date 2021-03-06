#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "filedels.h"

/* static function declarations */
static void path_fill(FileInfo *file_info);
static void per_check(FileInfo *file_info);
static long int nol_count(char *filename);

static void
path_fill(FileInfo *file_info)
{
	char cwd[SIZE_FILE_LARGE];
	int slash_pos = 0;
	int i;
	int j;

	/* find slash_pos */
	for (i = strlen(file_info->path_file_provided); i >= 0; i--) {
		if (file_info->path_file_provided[i] == '/') {
			slash_pos = ++i;
			break;
		}
	}

	/* path_file_name */
	if (!slash_pos) {
		strcpy(file_info->path_file_name, file_info->path_file_provided);
	} else {
		j = slash_pos;
		for (i = 0; i < (strlen(file_info->path_file_provided) - slash_pos);
				i++) {
			file_info->path_file_name[i] = file_info->path_file_provided[j];
			j++;
		}
	}

	/* path_file_full */
	if (file_info->path_file_provided[0] == '/') {
		strcpy(file_info->path_file_full, file_info->path_file_provided);
	} else {
		/* fill path_file_full */
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			strcpy(file_info->path_file_full, cwd);
			strcat(file_info->path_file_full, "/");
			strcat(file_info->path_file_full, file_info->path_file_name);
		} else {
			fprintf(stderr, "Cannot get current directory!\n");
		}
	}

	/* path_temp */
	strcpy(file_info->path_temp, PATH_TEMP_DIR);
	strcat(file_info->path_temp, file_info->path_file_name);
}

static void
per_check(FileInfo *file_info)
{
	if (access(file_info->path_file_full, F_OK) != -1)
		file_info->per_exist = 1;

	if (access(file_info->path_file_full, R_OK) != -1)
		file_info->per_readable = 1;

	if (access(file_info->path_file_full, W_OK) != -1)
		file_info->per_writable = 1;

	if (access(file_info->path_file_full, F_OK) != -1)
		file_info->per_exist = 1;

	if (access(file_info->path_temp, F_OK) != -1)
		file_info->per_temp_exist = 1;
}

/*
MARK: It is anything better I can do?
*/

static long int
nol_count(char *filename)
{
	long int lines = 0;
	FILE *fPointer;

	fPointer = fopen(filename, "r");
	if (fPointer == NULL) {
		return 0;
	}
	while (EOF != (fscanf(fPointer, "%*[^\n]"), fscanf(fPointer, "%*c")))
		lines++;
	fclose(fPointer);

	return lines;
}

FileInfo
filedels_new(char provided_file[SIZE_FILE_SMALL])
{
	/*
MARK: Implicit declarations due to garbage value
*/
	FileInfo file_info_temp = {"", "", "", "", 0, 0, 0, 0, 0, 0};
	strcpy(file_info_temp.path_file_provided, provided_file);
	path_fill(&file_info_temp);
	per_check(&file_info_temp);
	file_info_temp.nol_file = nol_count(file_info_temp.path_file_name);
	file_info_temp.nol_temp = nol_count(file_info_temp.path_temp);
	return file_info_temp;
}

void
filedels_debug(FileInfo *file_info)
{
	printf("=== FILEINFO DEBUG ===\n");
	printf("Provided path: %s\n", file_info->path_file_provided);
	printf("Full path: %s\n", file_info->path_file_full);
	printf("Filename: %s\n", file_info->path_file_name);
	printf("Temp path: %s\n", file_info->path_temp);
	printf("File exist?: %d\n", file_info->per_exist);
	printf("File readable?: %d\n", file_info->per_readable);
	printf("File writable?: %d\n", file_info->per_writable);
	printf("Temp exist?: %d\n", file_info->per_temp_exist);
	printf("Lines in file: %ld\n", file_info->nol_file);
	printf("Lines in temp: %ld\n", file_info->nol_temp);
	printf("=== FILEINFO END ===\n\n");
}
