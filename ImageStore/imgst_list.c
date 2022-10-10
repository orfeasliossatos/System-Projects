/**
 * @file imgst_list.c
 * @brief imgStore library: do_list implementation
 *
 * @author ???
 */

#include "imgStore.h"

void do_list(const imgst_file* imgstfile)
{
    // Null-pointer check (no macro because void return type)
    if (imgstfile == NULL || imgstfile->metadata == NULL) {
        return;
    }

    /// PRINT

    print_header(&(imgstfile->header));

    if(imgstfile->header.num_files == 0) {
        printf("<< empty imgStore >>\n");

    } else {
        // Loop through all metadata, printing when valid
        for (size_t idx = 0; idx < imgstfile->header.max_files; ++idx) {

            if(imgstfile->metadata[idx].is_valid == NON_EMPTY) {
                print_metadata(&(imgstfile->metadata[idx]));
            }
        }
    }
}
