/**
 * @file dedup.c
 * @brief deduplicate if two image have same content
 *
 * @author ???
 */


#include "dedup.h"
#include "error.h"
#include "imgStore.h"
#include <string.h>

int do_name_and_content_dedup(imgst_file* imgstfile, const uint32_t index)
{

    // If pointer is Null return an error
    M_REQUIRE_NON_NULL(imgstfile);
    M_REQUIRE_NON_NULL(imgstfile->metadata);

    // Check if index is in range
    M_EXIT_IF(imgstfile->header.max_files <= index,
              ERR_INVALID_ARGUMENT, "dedup index out of range", );


    const char* id = imgstfile->metadata[index].img_id;
    const unsigned char* sha = imgstfile->metadata[index].SHA;

    // Loop over valid metadata.
    // If an image has the same name, return an error.
    // If an image has the same SHA(ie. content) de-duplicate.

    int i = 0;
    int has_content_clone = 0;

    while(i < imgstfile->header.max_files) {
        if(i != index && imgstfile->metadata[i].is_valid) {
            M_EXIT_IF(!strncmp(id, imgstfile->metadata[i].img_id, MAX_IMG_ID),
                      ERR_DUPLICATE_ID, "image with same imgID exists", );

            if(shaCompare(sha, imgstfile->metadata[i].SHA) == 0) {
                memcpy(imgstfile->metadata[index].offset, imgstfile->metadata[i].offset, NB_RES * sizeof(uint64_t));
                memcpy(imgstfile->metadata[index].size, imgstfile->metadata[i].size, NB_RES * sizeof(uint32_t));
                has_content_clone = 1;
            }
        }

        ++i;
    }

    // Tells the function caller that metadata[index] is content-unique
    if(has_content_clone == 0) {
        imgstfile->metadata[index].offset[RES_ORIG] = 0;
    }

    return ERR_NONE;
}
