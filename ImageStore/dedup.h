#pragma once

/**
 * @file dedup.h
 * @brief deduplicate if two image have same content
 *
 * @author ???
 */

#include "imgStore.h"


int do_name_and_content_dedup(imgst_file* imgstfile, const uint32_t index);
