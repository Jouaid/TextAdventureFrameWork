#pragma once
#include "Process.h"
#include <vector>
#include <map>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#if defined(_WIN32)
#include <direct.h>
#endif
#include <assert.h>
#include "json.hpp"
#include "GameState.h"
#include "Game.h"

//struct InputString;
class File :
  public Process
{
public:
  File() {}
  ~File() {}

  /*void OnUpdate() override;
  void OnAbort() override;
  void OnInit() override;*/

  static const std::map<std::string, std::vector<char>>
    OpenBinaryFiles(std::string directory, bool errorThrow = false)
  {
    std::map<std::string, std::vector<char>> files;

    struct stat info;

    if (stat(directory.c_str(), &info) != 0)
    {

#ifdef _WIN32
      // Buffer the current path
      char buffer[FILENAME_MAX];

#pragma warning( push )
#pragma warning( disable : 6031 )
      _getcwd(buffer, FILENAME_MAX);
#pragma warning (pop)

      // Add directory to the buffer
      std::filesystem::path path(buffer + directory);

      // Go through all the files in the folder and load them automatically
      for (auto& entry : std::filesystem::directory_iterator(path))
      {
        std::vector<char> bufferedFile = OpenBinaryFile(entry.path().string(), errorThrow);

        if (bufferedFile.size() > 0)
          files.insert(std::make_pair<std::string, std::vector<char>>(
            entry.path().string(), std::vector<char>(bufferedFile)));
      }
#endif
    }
    else
      std::cout << "Path doesn't exist";

    return files;
  }

  /// Always call CloseFile afterwards
  static std::vector<char> OpenBinaryFile(std::string fileLocation, bool errorThrow = false)
  {
    std::ifstream file(fileLocation, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
      if (errorThrow)
        throw std::runtime_error("Failed to open: " + fileLocation);
      else
        return std::vector<char>();
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
  }

  // the FileLocation includes the file name
  static std::string ReadFromFile(std::string fileLocation)
  {
    std::ifstream stream(fileLocation.c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << stream.rdbuf();

    stream.close();

    return buffer.str();
  }

  static void GetStateDataFromJson(std::string fileLocation)
  {
    try
    {
      std::string jsonData = ReadFromFile(fileLocation);
      std::istringstream buffer(jsonData, std::ios_base::out);
      picojson::value jsonValue;
      std::string error;

      // Parse our JSon values from 
      error = picojson::parse(jsonValue, buffer);

      // Make sure we don't have an error and that it's an object
      if (error.empty() && jsonValue.is<picojson::object>())
      {
        // Make sure our object has an array named "Inputs"
        if (jsonValue.contains("inputs") && jsonValue.get("inputs").is<picojson::array>())
        {
          // Get the inputs array inside the inputs array (inside objects)
          picojson::array data = jsonValue.get("inputs").get<picojson::array>();

          // For debugging purposes
          //std::cout << "Parse successful" << std::endl;

          // Iterate through all the input entries in the array
          for (size_t i = 0; i < data.size(); i++)
          {
            // Make sure we can get the name
            if (data[i].is<picojson::object>() && data[i].contains("name"))
            {
              // We will switch between the names of the state and add it in the right state
              std::string stateName =
                data[i].get("name").get<std::string>();

              // for debugging purposes
              //std::cout << stateName << std::endl;
              std::vector<StateInput> stateInputs;

              // Make sure our inputs array contains the inputs array
              if (data[i].contains("inputs") && data[i].get("inputs").is<picojson::array>())
              {
                picojson::array inputs = data[i].get("inputs").get<picojson::array>();

                for (size_t value = 0; value < inputs.size(); value++)
                {
                  // Parse data to the state input
                  StateInput stateInput;

                  stateInput.input = inputs[value].get("input").get<std::string>();
                  stateInput.output = inputs[value].get("output").get<std::string>();

                  // Does this input have a action that follows it?
                  if (inputs[value].contains("action") && inputs[value].get("action").is<std::string>())
                  {
                    std::string actionString = inputs[value].get("action").get<std::string>();

                    if (actionString == "switch")
                      stateInput.action = Action::SWITCH;
                  }
                  else
                    stateInput.action = Action::NONE;


                  if (inputs[value].contains("prerequisites") && inputs[value].get("prerequisites").is<std::string>())
                  {
                    stateInput.prerequisites = inputs[value].get("prerequisites").get<std::string>();
                  }

                  if (inputs[value].contains("switchState") && inputs[value].get("switchState").is<std::string>())
                  {
                    std::string stateName = inputs[value].get("switchState").get<std::string>();

                    GameState* state = nullptr;

                    //TODO: This code sucks, we need to change this
                    if (stateName == "GameOverState")
                      state = Game::GetGameState(GameStateEnum::GAME_OVER);
                    else if (stateName == "FactoryState")
                      state = Game::GetGameState(GameStateEnum::FACTORY);

                    if (state != nullptr)
                      stateInput.switchToState = state;
                  }

                  stateInputs.push_back(stateInput);
                }
              }

              GameState* gameState = Game::GetGameState(stateName);

              if (gameState != nullptr && stateInputs.size() > 0)
                gameState->SetInput(stateInputs);
            }
          }
        }
      }
    }
    catch (const std::exception & exception)
    {
      std::cout << exception.what() << std::endl;
    }

  }
};


//struct InputString
//{
//  std::string name;
//  std::vector<std::string> inputs;
//
//  void from_json(const json& j)
//  {
//    j.at("name").get_to(name);
//    j.at("inputs").get_to(inputs);
//  }
//};

