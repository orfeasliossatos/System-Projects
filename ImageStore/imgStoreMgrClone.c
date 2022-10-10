/**
 * @file imgStoreMgr.c
 * @brief imgStore Manager: command line interpretor for imgStore core commands.
 *
 * Image Database Management Tool
 *
 * @author Mia Primorac
 */

#include "util.h" // for _unused
#include "imgStore.h"

#include <stdlib.h>
#include <string.h> // for strlen and strcmp
#include <vips/vips.h> // for vips!

/**
 * Opens imgStore file and calls do_list command.
 */

int lazily_resize(const int res_code, imgst_file* imgstfile, const size_t idx);
int do_list_cmd (const char* filename)
{
    // Null-pointer check
    if (filename == NULL) {
        return ERR_INVALID_ARGUMENT;
    }

    // Declare an imgst_file
    imgst_file imgstfile;

    // Open the file with the given filename in binary read mode
    const size_t err = do_open(filename, "rb+", &imgstfile);

    /// Test for lazily_resize

    if (lazily_resize(RES_SMALL, &imgstfile, 0) != ERR_NONE) {
        printf("RESIZING ERROR!!\n");
    }

    if (lazily_resize(RES_THUMB, &imgstfile, 1) != ERR_NONE) {
        printf("RESIZING ERROR!!\n");
    }

    /// List the contents and then close the file.

    // We cannot list the contents of a file that wasn't correctly opened
    if (err == ERR_NONE) {
        do_list(&imgstfile);
    }

    // Clean up the file
    do_close(&imgstfile);

    return err;
}

/**
 * Prepares and calls do_create command.
 */
int do_create_cmd (const char* filename _unused)
{
    // Null pointer check
    if (filename == NULL) {
        return ERR_INVALID_ARGUMENT;
    }

    // This will later come from the parsing of command line arguments
    const uint16_t thumb_res_x =  64;
    const uint16_t thumb_res_y =  64;
    const uint16_t small_res_x = 256;
    const uint16_t small_res_y = 256;

    puts("Create");

    // Declare an imgst_file
    imgst_file imgstfile;

    // Initialize the values that aren't meant to be changed
    imgstfile.header = (imgst_header) {
        .res_resized = {thumb_res_x, thumb_res_y, small_res_x, small_res_y}
    };

    // Explicitly initialize the rest of the imgst_file.
    const size_t err = do_create(filename, &imgstfile);

    // Print the header if nothing went wrong.
    if (err == ERR_NONE) {
        print_header(&(imgstfile.header));
    }

    // Clean up the file.
    do_close(&imgstfile);

    return err;
}
/**
 * Displays some explanations.
 */
int help ()
{
    printf("imgStoreMgr [COMMAND] [ARGUMENTS]\n"
           "  help: displays this help.\n"
           "  list <imgstore_filename>: list imgStore content.\n"
           "create <imgstore_filename>: create a new imgStore.\n"
           "delete <imgstore_filename> <imgID>: delete image imgID from imgStore.\n");

    // We'll assume that calling help never fails.
    return ERR_NONE;
}

/**
 * Deletes an image from an imgStore
 */
int do_delete_cmd (const char* filename _unused, const char* imgID _unused)
{
    // Only non-null names of a maximum length can legitimately be deleted.
    if (filename == NULL || imgID == NULL || strlen(imgID) > MAX_IMG_ID) {
        return ERR_INVALID_IMGID;
    }

    // Open the file
    imgst_file imgstfile;
    size_t err = do_open(filename, "rb+", &imgstfile);

    // If correctly opened, then delete.
    if (err == ERR_NONE) {
        err = do_delete(imgID, &imgstfile);
    }

    // Clean up the file
    do_close(&imgstfile);

    return err;
}

/**
 * MAIN
 */
int main (int argc, char* argv[])
{
    int ret = 0;

    if (argc < 2) {
        ret = ERR_NOT_ENOUGH_ARGUMENTS;

    } else {

        // START VIPS
        /*if (VIPS_INIT(argv[0])) {
        	vips_error_exit("unable to start VIPS");
        }*/

        argc--; argv++; // skips command call name


        if (!strcmp("list", argv[0])) {
            if (argc < 2) {
                ret = ERR_NOT_ENOUGH_ARGUMENTS;

            } else {
                ret = do_list_cmd(argv[1]);
            }

        } else if (!strcmp("create", argv[0])) {
            if (argc < 2) {
                ret = ERR_NOT_ENOUGH_ARGUMENTS;

            } else {
                ret = do_create_cmd(argv[1]);
            }

        } else if (!strcmp("delete", argv[0])) {
            if (argc < 3) {
                ret = ERR_NOT_ENOUGH_ARGUMENTS;

            } else {
                ret = do_delete_cmd(argv[1], argv[2]);
            }

        } else if (!strcmp("help", argv[0])) {
            ret = help();

        } else {
            ret = ERR_INVALID_COMMAND;
        }
    }

    if (ret) {
        fprintf(stderr, "ERROR: %s\n", ERR_MESSAGES[ret]);
        help();
    }

    // SHUTDOWN VIPS
    //vips_shutdown();

    return ret;
}
