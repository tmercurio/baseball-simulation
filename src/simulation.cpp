// File Name: simulation.cpp
// Author: Thomas Mercurio
// Email: tmercuri@nd.edu
// This is the main simulation file for the Baseball Simulation project.

#include "../inc/Player.h"
#include "../inc/Team.h"

int main()
{
  Player trout = Player("Mike Trout", 27, 681, 173, 32, 5, 29, 116, 137, 11, 5, 5, 30, 7);

  Team white_sox("White Sox");

  white_sox.add_player(Player("Tim Anderson", 7, 551, 163, 29, 2, 17, 22, 119, 1, 5, 1, 18, 7));
  white_sox.add_player(Player("Yoan Moncada", 10, 616, 137, 33, 1, 14, 84, 157, 10, 6, 2, 3, 2));
  white_sox.add_player(Player("Jose Abreu", 79, 659, 148, 30, 2, 30, 61, 143, 22, 28, 10, 1, 0));
  white_sox.add_player(Player("Yasmani Grandal", 24, 375, 67, 9, 0, 23, 87, 82, 3, 15, 5, 0, 0));
  white_sox.add_player(Player("Luis Robert", 88, 296, 93, 22, 1, 13, 14, 61, 5, 4, 2, 6, 1));
  white_sox.add_player(Player("Eloy Jimenez", 74, 231, 53, 10, 0, 10, 16, 57, 1, 8, 1, 0, 0));
  white_sox.add_player(Player("Andrew Vaughn", 25, 469, 98, 22, 0, 15, 41, 101, 6, 14, 5, 1, 1));
  white_sox.add_player(Player("Brian Goodwin", 18, 271, 52, 10, 1, 8, 33, 58, 1, 2, 1, 1, 0));
  white_sox.add_player(Player("Leury Garcia", 0, 474, 111, 22, 4, 5, 41, 97, 4, 12, 5, 6, 2));
  white_sox.add_player(Player("Danny Mendick", 20, 186, 36, 5, 0, 2, 18, 42, 2, 1, 1, 0, 1));
  //white_sox.add_player(Player("Terrible Hitter", 1, 500, 36, 5, 0, 2, 18, 342, 2, 1, 1, 0, 1));

  //std::cout << anderson;
  //std::cout << white_sox;

  //white_sox.set_lineup();

  srand(time(0)); // Needs to be here so random is not reset each time sim_ab method is called.

  //int runs = white_sox.sim_game();

/*

  int sims = 100000;


  std::cout << std::endl << "Team: " << white_sox.get_name() << std::endl;
  std::cout << "Lineup: ";
  white_sox.print_lineup();
  std::cout << "Games simulated: " << sims << std::endl;
  std::cout << "Average Runs Per Game: " << std::setprecision(3) << white_sox.sim_multiple_games(sims) << std::endl;

  */


  white_sox.sim_multiple_lineups();

}
