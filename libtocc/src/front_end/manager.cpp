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

#include <string>

#include "database/database.h"
#include "file_system/file_manager.h"
#include "engine/tags_engine.h"
#include "engine/files_engine.h"
#include "utilities/file_info_converter.h"
#include "libtocc.h"


namespace libtocc
{

  /*
   * Name of the database file, which should be existed in the root
   * of the path.
   */
  const std::string DATABASE_FILE = "TOCCFILES.DB";

  class Manager::PrivateData
  {
  public:
    PrivateData(Database* database,
                TagsEngine* tags_engine,
                FilesEngine* files_engine)
    {
      this->database = database;
      this->tags_engine = tags_engine;
      this->files_engine = files_engine;
    }

    ~PrivateData()
    {
      delete this->database;
      this->database = NULL;
      delete this->tags_engine;
      this->tags_engine = NULL;
      delete this->files_engine;
      this->files_engine = NULL;
    }

    Database* database;
    TagsEngine* tags_engine;
    FilesEngine* files_engine;
  };

  Manager::Manager(const char* base_path)
  {
    std::string database_path(base_path);
    database_path += "/";
    database_path += DATABASE_FILE;

    Database* database = new Database(database_path);
    FileManager* file_manager = new FileManager(base_path);
    TagsEngine* tags_engine = new TagsEngine(database);
    FilesEngine* files_engine = new FilesEngine(database,
                                                file_manager,
                                                tags_engine);

    this->private_data = new PrivateData(database,
                                         tags_engine, files_engine);
  }

  Manager::~Manager()
  {
    delete this->private_data;
    this->private_data = NULL;
  }

  FileInfo Manager::get_file_info(const char* file_id)
  {
    IntFileInfo info = this->private_data->database->get(file_id);

    return to_external_file_info(&info);
  }

  FileInfo Manager::copy_file(const char* source_path,
                              const char* title,
                              const char* traditional_path)
  {
    IntFileInfo new_file_info =
        this->private_data->files_engine->copy_file(source_path,
                                                    title, traditional_path);

    return to_external_file_info(&new_file_info);
  }

  FileInfo Manager::copy_file(const char* source_path,
                              const char* title,
                              const char* traditional_path,
                              const TagsCollection* tags)
  {
    std::vector<std::string> tags_vector = tags_to_vector(tags);

    IntFileInfo new_file_info =
        this->private_data->files_engine->copy_file(source_path,
                                                    title,
                                                    traditional_path,
                                                    tags_vector);

    return to_external_file_info(&new_file_info);
  }

  void Manager::assign_tags(const char* file_ids[],
                            int file_ids_size,
                            const TagsCollection* tags)
  {
    if (file_ids_size <= 0)
    {
      file_ids_size = sizeof(file_ids) / sizeof(file_ids[0]);
    }

    // Converting file_ids to vector.
    std::vector<std::string> file_ids_vector;
    for (int i = 0; i < file_ids_size; ++i)
    {
      file_ids_vector.push_back(file_ids[i]);
    }

    // Converting tags to vector.
    std::vector<std::string> tags_vector = tags_to_vector(tags);

    // Calling the engine to do the job.
    this->private_data->tags_engine->assign_tags(file_ids_vector, tags_vector);
  }

  /*
   * Assigns tags to a file.
   *
   * @param file_id: File to assign tags to.
   * @param tags: Tags to assign to file.
   */
  void Manager::assign_tags(const char* file_id, const TagsCollection* tags)
  {
    // Converting tags to vector.
    std::vector<std::string> tags_vector = tags_to_vector(tags);

    this->private_data->tags_engine->assign_tags(file_id, tags_vector);
  }

  /*
   * Assigns a tag to a file.
   *
   * @param file_id: File to assign tags to.
   * @param tag: Tag to assign to file.
   */
  void Manager::assign_tags(const char* file_id, const char* tag)
  {
    this->private_data->tags_engine->assign_tags(file_id, tag);
  }

}
