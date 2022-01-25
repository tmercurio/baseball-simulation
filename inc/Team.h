// File Name: Team.h
// Author: Thomas Mercurio
// Email: tmercuri@nd.edu
// This is the function header file for the Team class

#include "Player.h"

#ifndef TEAM_H
#define TEAM_H

#define INNINGS 9
#define LINEUP_SIZE 9

class Team {

  private:

    std::string name;
    std::vector<Player> players;
    std::vector<Player> lineup;

  public:

    Team();

    Team(std::string name);

    Team(std::string name, Player p0, Player p1, Player p2, Player p3, Player p4, Player p5, Player p6, Player p7, Player p8);

    std::vector<Player> get_players();

    void add_player(Player new_player);

    void add_players(std::vector<Player> new_players);

    void set_lineup();

    friend std::ostream& operator<<(std::ostream& out, const Team &T);

    std::string get_name();

    void print_lineup();

    int sim_game();

    int sim_game_no_print();

    int sim_game_no_print(std::vector<Player> lineup_in);

    float sim_multiple_games(int numGames);

    float sim_multiple_games(std::vector<Player> lineup_in, int numGames);

    void sim_every_lineup();

    void sim_multiple_lineups();

};

#endif
