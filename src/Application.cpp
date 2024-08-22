#include "Application.h"

#include <vector>
#include <string>
#include <ncurses.h>
#include <cstdio>
#include <exception>

#include "Menu.h"
#include "ChoiceMenu.h"
#include "LevelLoader.h"
#include "GameMode.h"
#include "ScoreBoardManager.h"
#include "Game.h"
#include "ConfigurationManager.h"

Application::Application(const ConfigurationManager & conf):
    conf(conf),
    levels(this->conf.getString("levels_dir")) {}

void Application::run() {
    initscr(); 
    noecho(); // so that it is will not write what was pressed behind the button
    curs_set(0); // deactivates cursor
    try {
        checkTerminalWindow();
        levels.loadLevels();
        navigation();
    }
    catch (const std::exception & err) {
        endwin();
        printf("Error: %s\n", err.what());
        return;
    }
    catch (const char * err) {
        endwin();
        printf("Error: %s\n", err);
        return;
    }
    endwin(); // return control to terminal
}

void Application::checkTerminalWindow() const {
    int yMax = 0;
    int xMax = 0;
    getmaxyx(stdscr, yMax, xMax);
    if(yMax < HEIGHT_MIN_WIN || xMax < WIDTH_MIN_WIN) {
        throw("Terminal is too small for this app.");
    }
}

void Application::navigation() {
    while(true) {
        Menu menu;
        int optionMenu = menu.show("Main menu", mainMenuChoices);
        switch(optionMenu) {
            case START_HUMAN_VS_HUMAN: {
                int optionLevels = menu.show("Choose level:", levels.fileNames);
                if(optionLevels == -1) {
                    break;
                }
                menu.clear();
                Game game(HUMAN_VS_HUMAN, levels.fileNames[optionLevels], conf);
                levels.loadLevel(optionLevels, game, HUMAN_VS_HUMAN);
                game.startGame();
                break;
            }
            case START_HUMAN_VS_PC: {
                int optionLevels = menu.show("Choose level:", levels.fileNames);
                if(optionLevels == -1) {
                    break;
                }
                menu.clear();
                Game game(HUMAN_VS_PC, levels.fileNames[optionLevels], conf);
                levels.loadLevel(optionLevels, game, HUMAN_VS_PC);
                game.startGame();
                break;
            }
            case SHOW_SCOREBOARD_HUMAN_VS_HUMAN: {
                int optionLevels = menu.show("Choose level for scoreboard:", levels.fileNames);
                if(optionLevels == -1) {
                    break;
                }
                menu.clear();
                ScoreBoardManager sbManager (HUMAN_VS_HUMAN, levels.fileNames[optionLevels], conf.getJSON("scoreboards"));
                sbManager.print();
                sbManager.getInput();
                sbManager.clear();
                break;
            }
            case SHOW_SCOREBOARD_HUMAN_VS_PC: {
                int optionLevels = menu.show("Choose level for scoreboard:", levels.fileNames);
                if(optionLevels == -1) {
                    break;
                }
                menu.clear();
                ScoreBoardManager sbManager (HUMAN_VS_PC, levels.fileNames[optionLevels], conf.getJSON("scoreboards"));
                sbManager.print();
                sbManager.getInput();
                sbManager.clear();
                break;
            }
            case QUIT:
            case -1:
                return;
            default:
                break;
        }
    }
}
