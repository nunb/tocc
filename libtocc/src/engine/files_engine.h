/*
 * This file is part of Tocc. (see <http://www.github.com/aidin36/tocc>)
 * Copyright (C) 2013, 2014, Aidin Gharibnavaz <tocc@aidinhut.com>
 *
 * Tocc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tocc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *  along with Tocc.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBTOCC_FILES_ENGINE_H_INCLUDED
#define LIBTOCC_FILES_ENGINE_H_INCLUDED

#include <string>
#include <vector>

#include "common/int_file_info.h"
#include "database/database.h"
#include "file_system/file_manager.h"
#include "engine/tags_engine.h"

namespace libtocc
{

  /*
   * Provides operations for working with files.
   */
  class FilesEngine
  {
  public:

    FilesEngine(Database* database,
                FileManager* file_manager,
                TagsEngine* tags_engine);

    /*
     * Gets information of a file.
     * It raises exception if file does not exists.
     *
     * @param file_id: ID of the file to get its information.
     *
     * @return: Information of the file.
     */
    IntFileInfo get(std::string file_id);

    /*
     * Copies a file from the path to the file system.
     *
     * @param source_path: Path to the source file.
     * @param title: (optional) title of the file.
     * @param traditional_path: (optional) traditional path of the file.
     *
     * @return: Information of the newly created file.
     */
    IntFileInfo copy_file(std::string source_path,
                          std::string title="",
                          std::string traditional_path="");

    /*
     * Copies a file from the path to the file system.
     *
     * @param source_path: Path to the source file.
     * @param title: title of the file.
     * @param traditional_path: traditional path of the file.
     *   (Can be empty string.)
     * @param tags: Tags to assign to the file.
     *
     * @return: Information of the newly created file.
     */
    IntFileInfo copy_file(std::string source_path,
                          std::string title,
                          std::string traditional_path,
                          std::vector<std::string> tags);

  private:
    Database* database;
    FileManager* file_manager;
    TagsEngine* tags_engine;
  };

}

#endif /* LIBTOCC_FILES_ENGINE_H_INCLUDED */
