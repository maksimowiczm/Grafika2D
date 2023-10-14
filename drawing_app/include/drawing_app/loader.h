#pragma once

#include "context.h"

bool loader_save_to_file(Context *context, const char *file_path);

bool loader_load_from_file(Context *context, const char *file_path);
