// File Name: Team.cpp
// Author: Thomas Mercurio
// Email: tmercuri@nd.edu
// This is the function definition file for the Team class

#include "../inc/Team.h"

Team::Team(): name(), players(), lineup() {}

Team::Team(std::string name): name(name), players(), lineup() {}

//Team::Team(std::string name, Player p0, Player p1, Player p2, Player p3, Player p4, Player p5, Player p6, Player p7, Player p8):
            //name(name), (players(new vec{p0, p1, p2, p3, p4, p5, p6, p7, p8}) {}

std::vector<Player> Team::get_players()
{
  return players;
}

void Team::add_player(Player new_player)
{
  players.push_back(new_player);
  if (players.size() == LINEUP_SIZE) {
    for (int i = 0; i < LINEUP_SIZE; i++) {
      lineup.push_back(players[i]);
    }
  }
}

void Team::add_players(std::vector<Player> new_players)
{
  players.insert(players.end(), new_players.begin(), new_players.end());
}

void Team::set_lineup()
{
  int number;
  std::vector<Player> new_lineup;
  char garbage[256]; // Holds the unwanted line inputs

  std::cout << *this;

  std::cout << "Enter the lineup by player number: ";
  for (int i = 0; i < 9; i++) {
    std::cin >> number;

// If using a larger data set, could change to a hash table to improve lookup time
    for (int j = 0; j < players.size(); j++) {
      if (number == players[j].get_number()) {
        new_lineup.push_back(players[j]);
        break;
      }
      if (j == players.size() - 1) {
        std::cout << "There is no number " << number << " on the team. Please re-enter a correct lineup." << std::endl << std::endl;
        std::cin.getline(garbage, 256);
        set_lineup();
        return;
      }
    }
  }

  lineup = new_lineup;
}

std::ostream& operator<<(std::ostream& out, const Team &T)
{
  out << "Team: " << T.name << std::endl;

  for (int i = 0; i < T.players.size(); i++) {
    out << T.players[i].get_number() << " " << T.players[i].get_name() << std::endl;
  }

  return out;
}

std::string Team::get_name()
{
  return name;
}

void Team::print_lineup()
{
  for (int i = 0; i < lineup.size() - 1; i++) {
    std::cout << "(" << i + 1 << ") " << lineup[i].get_name() << ", ";
  }

  std::cout << "(" << lineup.size() << ") " << lineup[lineup.size() - 1].get_name() << std::endl;
}

// Will simulate the game with the players in the order they are in the vector
int Team::sim_game()
{
  int runs = 0, outs = 0, spot = 0, hits = 0, errs = 0, outcome; // runs will be returned
  int innRuns = 0, innHits = 0, innErrs = 0;
  float value; // Used as a random variable in needed cases
  bool third;
  bool steal;

  std::unordered_map<int, Player> bases; // Contains the players on base

  for (int inning = 1; inning <= INNINGS; inning++) {

    std::cout << "Inning " << inning << ": " << std::endl;
    std::cout << "--------" << std::endl;
    while (outs < 3) {

      value = (float) rand() / RAND_MAX;
      if (bases.count(1) > 0 && bases.count(2) == 0) {
        if (value < bases[1].get_steal_pct()) {
          std::cout << bases[1].get_name() << " stole second." << std::endl;
          bases[2] = bases[1];
          bases.erase(1);
        }
        else if (value < (bases[1].get_steal_pct() + bases[1].get_cs_pct())) {
          std::cout << bases[1].get_name() << " was caught stealing second. " << std::endl;
          bases.erase(1);
          outs++;
          if (outs >= 3) break;
        }
      }
      else if (bases.count(2) > 0) {
        steal = false;
        if (value < (bases[2].get_steal_pct() * 0.15)) {
          std::cout << bases[2].get_name() << " stole third. ";
          bases[3] = bases[2];
          bases.erase(2);
          steal = true;
        }
        else if (value < ((bases[2].get_steal_pct() + bases[2].get_cs_pct()) * 0.15)) {
          std::cout << bases[2].get_name() << " was caught stealing third. ";
          bases.erase(2);
          outs++;
          if (outs >= 3) {
            std::cout << std::endl;
            break;
          }
          steal = true;
        }
        if (steal && bases.count(1) > 0 && value < (bases[2].get_steal_pct() * 0.15 * 0.575)) {
          std::cout << bases[1].get_name() << " stole second. ";
          bases[2] = bases[1];
          bases.erase(1);
        }
        else if (steal && bases.count(1) > 0 && value < (bases[2].get_steal_pct() * 0.15 * 0.63)) {
          std::cout << bases[1].get_name() << " was caught stealing second. ";
          bases.erase(1);
          outs++;
          if (outs >= 3) {
            std::cout << std::endl;
            break;
          }
        }
        if (steal)
          std::cout << std::endl;
      }

      outcome = lineup[spot].sim_ab();
      lineup[spot].print_ab(outcome);

      switch (outcome) {
        case 0: // Strikeout
          outs++;
          break;

        case 1: // Single
          innHits++;
          third = false; // Used to represent if third base was occupied before the hit.
          if (bases.count(3) > 0) {
            std::cout << bases[3].get_name() << " scored from third. ";
            third = true;
            bases.erase(3);
            innRuns++;
          }
          if (bases.count(2) > 0) {
            value = (float) rand() / RAND_MAX;
            switch (outs) {
              case 0: // Probabilities were found using Stathead for the 2021 season.
                if (third || bases.count(1) > 0) { // Someone on base other than second
                  if ((value + 0.004 * bases[2].get_speed()) < 0.009) {
                    std::cout << bases[2].get_name() << " was thrown out at home. ";
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.475) {
                    std::cout << bases[2].get_name() << " scored from second. ";
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.983 || bases.count(1) > 0) {
                    std::cout << bases[2].get_name() << " moved from second to third. ";
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                  else {
                    std::cout << bases[2].get_name() << " held at second. ";
                  }
                }
                else { // Runner only on second -- lower probability of scoring
                  if ((value + 0.004 * bases[2].get_speed()) < 0.008) {
                    std::cout << bases[2].get_name() << " was thrown out at home. ";
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.38) {
                    std::cout << bases[2].get_name() << " scored from second. ";
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.966) {
                    std::cout << bases[2].get_name() << " moved from second to third. ";
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                  else {
                    std::cout << bases[2].get_name() << " held at second. ";
                  }
                }
                break;
              case 1: // Cases 1 and 2 are the same as case 0 with different probabilities
                if (bases.count(1) > 0 && !third) { // Runners on first and second
                  if ((value + 0.007 * bases[2].get_speed()) < 0.014) {
                    std::cout << bases[2].get_name() << " was thrown out at home. ";
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.591) {
                    std::cout << bases[2].get_name() << " scored from second. ";
                    innRuns++;
                    bases.erase(2);
                  }
                  else {
                    std::cout << bases[2].get_name() << " moved from second to third. ";
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else if (bases.count(1) == 0 && third) { // Runners on second and third
                  if ((value + 0.004 * bases[2].get_speed()) < 0.009) {
                    std::cout << bases[2].get_name() << " was thrown out at home. ";
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.408) {
                    std::cout << bases[2].get_name() << " scored from second. ";
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.922) {
                    std::cout << bases[2].get_name() << " moved from second to third. ";
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                  else {
                    std::cout << bases[2].get_name() << " held at second. ";
                  }
                }
                else if (bases.count(1) > 0 && third) { // Bases loaded
                  if ((value + 0.06 * bases[2].get_speed()) < 0.013) {
                    std::cout << bases[2].get_name() << " was thrown out at home. ";
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.511) {
                    std::cout << bases[2].get_name() << " scored from second. ";
                    innRuns++;
                    bases.erase(2);
                  }
                  else {
                    std::cout << bases[2].get_name() << " moved from second to third. ";
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else { // Runner only on second
                  if ((value + 0.011 * bases[2].get_speed()) < 0.022) {
                    std::cout << bases[2].get_name() << " was thrown out at home. ";
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.546) {
                    std::cout << bases[2].get_name() << " scored from second. ";
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.966) {
                    std::cout << bases[2].get_name() << " moved from second to third. ";
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                  else {
                    std::cout << bases[2].get_name() << " held at second. ";
                  }
                }
                break;
              case 2:
                if (bases.count(1) > 0 && !third) { // Runners on first and second
                  if ((value + 0.021 * bases[2].get_speed()) < 0.042) {
                    std::cout << bases[2].get_name() << " was thrown out at home. ";
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.844) {
                    std::cout << bases[2].get_name() << " scored from second. ";
                    innRuns++;
                    bases.erase(2);
                  }
                  else {
                    std::cout << bases[2].get_name() << " moved from second to third. ";
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else if (bases.count(1) == 0 && third) { // Runners on second and third
                  if ((value + 0.011 * bases[2].get_speed()) < 0.022) {
                    std::cout << bases[2].get_name() << " was thrown out at home. ";
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.823) {
                    std::cout << bases[2].get_name() << " scored from second. ";
                    innRuns++;
                    bases.erase(2);
                  }
                  else {
                    std::cout << bases[2].get_name() << " moved from second to third. ";
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else if (bases.count(1) > 0 && third) { // Bases loaded
                  if ((value + 0.011 * bases[2].get_speed()) < 0.023) {
                    std::cout << bases[2].get_name() << " was thrown out at home. ";
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.813) {
                    std::cout << bases[2].get_name() << " scored from second. ";
                    innRuns++;
                    bases.erase(2);
                  }
                  else {
                    std::cout << bases[2].get_name() << " moved from second to third. ";
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else { // Runner only on second
                  if ((value + 0.015 * bases[2].get_speed()) < 0.030) {
                    std::cout << bases[2].get_name() << " was thrown out at home. ";
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.796) {
                    std::cout << bases[2].get_name() << " scored from second. ";
                    innRuns++;
                    bases.erase(2);
                  }
                  else {
                    std::cout << bases[2].get_name() << " moved from second to third. ";
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                break;
            }
          }

          if (bases.count(1) > 0) { // Probabilities were taken from Fangraphs
            value = (float) rand() / RAND_MAX;
            if (bases.count(3) > 0 || (value - SPEED_ADJ * bases[1].get_speed()) < .706) { // In this case, a runner on second already moved to third or the runner doesn't advance to third otherwise.
              std::cout << bases[1].get_name() << " moved from first to second.";
              bases[2] = bases[1];
            }
            else if ((value - 0.005 * bases[1].get_speed()) < .99) { // Half as likely for the fastest player in the league to get thrown out than the average player
              std::cout << bases[1].get_name() << " advanced from first to third.";
              bases[3] = bases[1];
            }
            else {
              std::cout << bases[1].get_name() << " was thrown out at third.";
              outs++;
            }
            bases.erase(1);
          }
          bases[1] = lineup[spot];
          std::cout << std::endl;
          break;

        case 2: // Double
          innHits++;
          if (bases.count(3) > 0) {
            std::cout << bases[3].get_name() << " scored from third. ";
            bases.erase(3);
            innRuns++;
          }
          if (bases.count(2) > 0) {
            std::cout << bases[2].get_name() << " scored from second. ";
            bases.erase(2);
            innRuns++;
          }
          if (bases.count(1) > 0) {
            value = (float) rand() / RAND_MAX;
            switch (outs) {
              case 0: // Probabilities were found at Baseball Reference's Play Finder from the 2021 MLB Regular Season
                if ((value + 0.006 * bases[1].get_speed()) < 0.013) {
                  std::cout << bases[1].get_name() << " was thrown out at home. ";
                  outs++;
                }
                else if ((value - SPEED_ADJ * bases[1].get_speed()) < 0.292) {
                  std::cout << bases[1].get_name() << " scored from first. ";
                  innRuns++;
                }
                else {
                  std::cout << bases[1].get_name() << " moved from first to third. ";
                  bases[3] = bases[1];
                }
                break;
              case 1: // Cases 1 and 2 are the same as case 0 with different probabilities
                if ((value + 0.015 * bases[1].get_speed()) < 0.030) {
                  std::cout << bases[1].get_name() << " was thrown out at home. ";
                  outs++;
                }
                else if ((value - SPEED_ADJ * bases[1].get_speed()) < 0.346) {
                  std::cout << bases[1].get_name() << " scored from first. ";
                  innRuns++;
                }
                else {
                  std::cout << bases[1].get_name() << " moved from first to third. ";
                  bases[3] = bases[1];
                }
                break;
              case 2:
                if ((value + 0.017 * bases[1].get_speed()) < 0.035) {
                  std::cout << bases[1].get_name() << " was thrown out at home. ";
                  outs++;
                }
                else if ((value - SPEED_ADJ * bases[1].get_speed()) < 0.586) {
                  std::cout << bases[1].get_name() << " scored from first. ";
                  innRuns++;
                }
                else {
                  std::cout << bases[1].get_name() << " moved from first to third. ";
                  bases[3] = bases[1];
                }
                break;
              }
              bases.erase(1);
            }
            bases[2] = lineup[spot];
            std::cout << std::endl;
            break;

        case 3: // Triple
          innHits++;
          if (bases.count(3) > 0) {
            std::cout << bases[3].get_name() << " scored from third. ";
            innRuns++;
            bases.erase(3);
          }
          if (bases.count(2) > 0) {
            std::cout << bases[2].get_name() << " scored from second. ";
            innRuns++;
            bases.erase(2);
          }
          if (bases.count(1) > 0) {
            std::cout << bases[1].get_name() << " scored from first. ";
            innRuns++;
            bases.erase(1);
          }
          bases[3] = lineup[spot];
          std::cout << std::endl;
          break;

        case 4: // Home run
          innHits++;
          innRuns += bases.size();
          innRuns++;
          if (bases.count(3) > 0)
            std::cout << bases[3].get_name() << " scored from third. ";
          if (bases.count(2) > 0)
            std::cout << bases[2].get_name() << " scored from second. ";
          if (bases.count(1) > 0)
            std::cout << bases[1].get_name() << " scored from first. ";
          bases.clear();
          std::cout << std::endl;
          break;

        case 5: // Walk
          if (bases.count(1) > 0) {
            if (bases.count(2) > 0) {
              if (bases.count(3) > 0) {
                std::cout << bases[3].get_name() << " scored from third. ";
                innRuns++;
                bases.erase(3);
              }
              std::cout << bases[2].get_name() << " moved from second to third. ";
              bases[3] = bases[2];
              bases.erase(2);
            }
            std::cout << bases[1].get_name() << " moved from first to second. ";
            bases[2] = bases[1];
            bases.erase(1);
          }
          bases[1] = lineup[spot];
          std::cout << std::endl;
          break;

        case 6: // Ground out
          outs++;
          value = (float) rand() / RAND_MAX;
          if (outs >= 3) {
            std::cout << " out." << std::endl;
            break;
          }
          if (bases.size() == 3) { // Bases loaded groundout--Probabilities were found at Baseball Reference's Play Finder from the 2021 MLB Regular Season
            if (outs == 1) { // 0 outs before the groundout
              if (value < 0.051) {
                std::cout << " into a double play. " << bases[3].get_name() << " was thrown out at home. ";
                std::cout << bases[2].get_name() << " moved from second to third. " << std::endl;
                std::cout << bases[1].get_name() << " moved from first to second. " << std::endl;
                outs++;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases.erase(1);
              }
              else if (value < 0.285) {
                std::cout << " into a fielder's choice. " << bases[3].get_name() << " was thrown out at home. ";
                std::cout << bases[2].get_name() << " moved from second to third. " << std::endl;
                std::cout << bases[1].get_name() << " moved from first to second. " << std::endl;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases[1] = lineup[spot];
              }
              else if (value < 0.592) {
                std::cout << " into a double play. " << bases[1].get_name() << " was thrown out at second. ";
                std::cout << bases[3].get_name() << " scored from third. " << std::endl;
                std::cout << bases[2].get_name() << " moved from second to third. " << std::endl;
                outs++;
                innRuns++;
                bases[3] = bases[2];
                bases.erase(2);
              }
              else if (value < 0.753) {
                std::cout << " out. " << std::endl;
                std::cout << bases[3].get_name() << " scored from third. " << std::endl;
                std::cout << bases[2].get_name() << " moved from second to third. " << std::endl;
                std::cout << bases[1].get_name() << " moved from first to second. " << std::endl;
                innRuns++;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases.erase(1);
              }
              else if (value < 0.979) {
                std::cout << " into a fielder's choice. " << bases[1].get_name() << " was thrown out at second. ";
                std::cout << bases[3].get_name() << " scored from third. " << std::endl;
                std::cout << bases[2].get_name() << " moved from second to third. " << std::endl;
                bases[3] = bases[2];
                bases.erase(2);
                bases[1] = lineup[spot];
              }
              else {
                std::cout << " into a fielder's choice. " << bases[2].get_name() << " was thrown out at third. ";
                std::cout << bases[3].get_name() << " scored from third. " << std::endl;
                std::cout << bases[1].get_name() << " moved from first to second. " << std::endl;
                bases.erase(3);
                bases[2] = bases[1];
                bases[1] = lineup[spot];
              }
            }
            else { // One out before the groundout
              if (value < 0.048) {
                std::cout << " into a double play. " << bases[3].get_name() << " was thrown out at home." << std::endl;
                outs++;
                break;
              }
              else if (value < 0.199) {
                std::cout << " into a fielder's choice. " << bases[3].get_name() << " was thrown out at home. ";
                std::cout << bases[2].get_name() << " moved from second to third. " << std::endl;
                std::cout << bases[1].get_name() << " moved from first to second. " << std::endl;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases[1] = lineup[spot];
              }
              else if (value < 0.617) {
                std::cout << " into a double play. " << bases[1].get_name() << " was thrown out at second." << std::endl;
                outs++;
                break;
              }
              else if (value < 0.742) {
                std::cout << " out. " << std::endl;
                std::cout << bases[3].get_name() << " scored from third. " << std::endl;
                std::cout << bases[2].get_name() << " moved from second to third. " << std::endl;
                std::cout << bases[1].get_name() << " moved from first to second. " << std::endl;
                innRuns++;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases.erase(1);
              }
              else if (value < 0.99) {
                std::cout << " into a fielder's choice. " << bases[1].get_name() << " was thrown out at second. ";
                std::cout << bases[3].get_name() << " scored from third. " << std::endl;
                std::cout << bases[2].get_name() << " moved from second to third. " << std::endl;
                bases[3] = bases[2];
                bases.erase(2);
                bases[1] = lineup[spot];
              }
              else {
                std::cout << " into a fielder's choice. " << bases[2].get_name() << " was thrown out at third. ";
                std::cout << bases[3].get_name() << " scored from third. " << std::endl;
                std::cout << bases[1].get_name() << " moved from first to second. " << std::endl;
                bases.erase(3);
                bases[2] = bases[1];
                bases[1] = lineup[spot];
              }
            }
          }

          else if (bases.count(3) > 0 && bases.count(1) == 0) {
            if (bases.count(2) > 0) { // Runners on second and third
              if (value < 0.662) {
                std::cout << " out." << bases[3].get_name() << " scored from third. ";
                innRuns++;
                bases.erase(3);
                if (value < .494) {
                  std::cout << bases[2].get_name() << " moved from second to third. ";
                  bases[3] = bases[2];
                  bases.erase(2);
                }
              }
              else if (value < 0.986) {
                std::cout << " out. The runners held. ";
              }
              else {
                std::cout << " into a double play. " << bases[2].get_name() << " was thrown out. ";
                outs++;
                if (outs >= 3) {
                  std::cout << std::endl;
                  break;
                }
                std::cout << bases[3].get_name() << " scored from third. ";
                innRuns++;
                bases.erase(3);
                bases.erase(2);
              }
            }
            else if (bases.count(1) > 0) { // Runners on first and third
              if (outs == 1) { // 0 outs before the play
                if (value < 0.353) {
                  std::cout << " into a double play. " << bases[1].get_name() << " was out at second. ";
                  if (value < 0.323) {
                    std::cout << bases[3].get_name() << " scored from third. ";
                    innRuns++;
                    bases.erase(3);
                  }
                  outs++;
                }
                else if (value < 0.383) {
                  std::cout << " into a double play. " << bases[3].get_name() << " was thrown out at home. ";
                  std::cout << bases[1].get_name() << " moved from first to second. ";
                  bases.erase(3);
                  bases[2] = bases[1];
                  bases.erase(1);
                  outs++;
                }
                else if (value < 0.670) {
                  std::cout << " into a fielder's choice." << bases[1].get_name() << " was out at second. ";
                  if (value < 0.646) {
                    std::cout << bases[3].get_name() << " scored from third. ";
                    innRuns++;
                    bases.erase(3);
                  }
                  else {
                    std::cout << bases[3].get_name() << " held at third. ";
                  }
                  bases[1] = lineup[spot];
                }
                else {
                  std::cout << " out. " << std::endl;
                  if (value < 0.850) {
                    std::cout << bases[3].get_name() << " scored from third. " << std::endl;
                    innRuns++;
                    bases.erase(3);
                  }
                  else {
                    std::cout << bases[3].get_name() << " held at third. ";
                  }
                  std::cout << bases[1].get_name() << " moved from first to second. " << std::endl;
                  bases[2] = bases[1];
                  bases.erase(1);
                }
              }
              else { // 1 out before the play
                if (value < 0.513) {
                  std::cout << " into a double play. " << bases[1].get_name() << " was out at second." << std::endl;
                  outs++;
                  break;
                }
                else if (value < 0.763) {
                  std::cout << " into a fielder's choice." << bases[1].get_name() << " was out at second. ";
                  if (value < 0.653) {
                    std::cout << bases[3].get_name() << " scored from third. ";
                    innRuns++;
                    bases.erase(3);
                  }
                  else {
                    std::cout << bases[3].get_name() << " held at third. ";
                  }
                  bases[1] = lineup[spot];
                }
                else {
                  std::cout << " out. " << std::endl;
                  if (value < 0.956) {
                    std::cout << bases[3].get_name() << " scored from third. " << std::endl;
                    innRuns++;
                    bases.erase(3);
                  }
                  else {
                    std::cout << bases[3].get_name() << " held at third. ";
                  }
                  std::cout << bases[1].get_name() << " moved from first to second. " << std::endl;
                  bases[2] = bases[1];
                  bases.erase(1);
                }
              }
            }
            else { // Only third base occupied
              if (value < 0.484) {
                std::cout << " out." << bases[3].get_name() << " scored from third. ";
                innRuns++;
                bases.erase(3);
              }
              else if (value < 0.996) {
                std::cout << " out. The runner held. ";
              }
              else {
                std::cout << " into a fielder's choice. " << bases[3].get_name() << " was thrown out at home. ";
                outs++;
                bases.erase(3);
              }
            }
          }

          else if (bases.count(2) > 0) {
            if (bases.count(1) > 0) { // Runners on first and second
              if (value < 0.447) {
                std::cout << " into a double play. " << bases[1].get_name() << " was out at second. ";
                outs++;
                if (outs >= 3) {
                  std::cout << std::endl;
                  break;
                }
                std::cout << players[2].get_name() << " moved from second to third. ";
                bases[3] = bases[2];
                bases.erase(3);
              }
              else if (value < 0.677) {
                std::cout << " out." << bases[2].get_name() << " moved from second to third. ";
                std::cout << bases[1].get_name() << " moved from first to second. ";
                bases[3] = bases[2];
                bases.erase(2);
              }
              else if (value < 0.962) {
                std::cout << " into a fielder's choice. " << bases[1].get_name() << " was out at second. ";
                std::cout << bases[2].get_name() << " moved from second to third. ";
                bases[3] = bases[2];
                bases.erase(2);
                bases[1] = lineup[spot];
              }
              else {
                std::cout << " into a fielder's choice. " << bases[2].get_name() << " was out at third. ";
                std::cout << bases[1].get_name() << " moved from first to second. ";
                bases[2] = bases[1];
                bases[1] = lineup[spot];
              }
            }
            else { // Runner just on second
              if (value < 0.675) {
                std::cout << " out. " << bases[2].get_name() << " advanced to third. ";
                bases[3] = bases[2];
                bases.erase(2);
              }
              else {
                std::cout << " out. " << "the runner held. ";
              }
            }
          }

          else if (bases.count(1) > 0) { // Runner just on first
            if (value < .475) {
              std::cout << " into a double play. " << bases[1].get_name() << " was out at second. ";
              bases.erase(1);
              outs++;
              if (outs >= 3) {
                std::cout << std::endl;
                break;
              }
            }
            else if (value < .742) {
              std::cout << " into a fielder's choice. " << bases[1].get_name() << " was out at second. ";
              bases[1] = lineup[spot];
            }
            else {
              std::cout << " out. " << bases[1].get_name() << " moved to second. ";
              bases[2] = bases[1];
              bases.erase(1);
            }
          }

          else {
            std::cout << " out.";
          }
          std::cout << std::endl;
          break;

        case 7: // Fly out
          outs++;
          value = (float) rand() / RAND_MAX;
          if (outs >= 3) break;
          if (bases.count(3) > 0) {
            if (value - SPEED_ADJ * bases[3].get_speed() < 0.529) {
              std::cout << bases[3].get_name() << " scored from third. ";
              innRuns++;
              bases.erase(3);
              if (bases.count(2) > 0) {
                value = (float) rand() / RAND_MAX;
                if (value - SPEED_ADJ * bases[2].get_speed() < 0.482) {
                  std::cout << bases[2].get_name() << " moved from second to third. ";
                  bases[3] = bases[2];
                  bases.erase(2);
                }
                else if (value > 0.94) {
                  std::cout << bases[2].get_name() << " was doubled off. ";
                  outs++;
                  bases.erase(2);
                  if (outs >= 3) {
                    std::cout << std::endl;
                    break;
                  }
                }
                else break;
              }
            }
            else if (value - 0.009 * bases[3].get_speed() > .981) {
              std::cout << bases[3].get_name() << " was thrown out at home. ";
              outs++;
              bases.erase(3);
              if (outs >= 3) {
                std::cout << std::endl;
                break;
              }
            }
          }
          else if (bases.count(2) > 0) {
            if (value - SPEED_ADJ * bases[2].get_speed() < 0.214) {
              std::cout << bases[2].get_name() << " moved from second to third. ";
              bases[3] = bases[2];
              bases.erase(2);
            }
            else if (value > 0.975) {
              std::cout << bases[2].get_name() << " was doubled off. ";
              outs++;
              bases.erase(2);
              if (outs >= 3) {
                std::cout << std::endl;
                break;
              }
            }
          }
          std::cout << std::endl;
          break;

        case 8: // Hit by Pitch
          if (bases.count(1) > 0) {
            if (bases.count(2) > 0) {
              if (bases.count(3) > 0) {
                std::cout << bases[3].get_name() << " scored from third. ";
                innRuns++;
                bases.erase(3);
              }
            std::cout << bases[2].get_name() << " moved from second to third. ";
            bases[3] = bases[2];
            bases.erase(2);
            }
            std::cout << bases[1].get_name() << " moved from first to second. ";
            bases[2] = bases[1];
            bases.erase(1);
          }
          bases[1] = lineup[spot];
          std::cout << std::endl;
          break;

        case 9: // Reached on Error
          innErrs++;
          value = (float) rand() / RAND_MAX;
          if (bases.count(3) > 0) {
            std::cout << bases[3].get_name() << " scored from third. ";
            innRuns++;
            bases.erase(3);
          }
          if (value < 0.859) { // Probabilities were found at Baseball Reference's Play Finder from the 2021 MLB Regular Season
            if (bases.count(2) > 0) {
              std::cout << bases[2].get_name() << " moved from second to third. ";
              bases[3] = bases[2];
              bases.erase(2);
            }
            if (bases.count(1) > 0) {
              std::cout << bases[1].get_name() << " moved from first to second. ";
              bases[2] = bases[1];
              bases.erase(1);
            }
            bases[1] = lineup[spot];
          }
          else if (value < 0.993) {
            if (bases.count(2) > 0) {
              std::cout << bases[2].get_name() << " scored from second. ";
              innRuns++;
              bases.erase(2);
            }
            if (bases.count(1) > 0) {
              std::cout << bases[1].get_name() << " moved from first to third. ";
              bases[3] = bases[1];
              bases.erase(1);
            }
            if (value < 0.988) {
              std::cout << lineup[spot].get_name() << " advanced to second. ";
              bases[2] = lineup[spot];
            }
            else {
              std::cout << lineup[spot].get_name() << " was out advancing. ";
              outs++;
            }
          }
          else {
            if (bases.count(2) > 0) {
              std::cout << bases[2].get_name() << " scored from second. ";
              innRuns++;
              bases.erase(2);
            }
            if (bases.count(1) > 0) {
              std::cout << bases[1].get_name() << " scored from first. ";
              innRuns++;
              bases.erase(1);
            }
            std::cout << lineup[spot].get_name() << " advanced to third. ";
            bases[3] = lineup[spot];
          }
          std::cout << std::endl;
          break;


      } // end of outcome switch

      spot++;
      if (spot >= 9) spot = 0;
    } // end of while loop

    std::cout << std::endl << innRuns << " runs on " << innHits << " hits and " << innErrs << " errors." << std::endl << std::endl;
    runs += innRuns;
    hits += innHits;
    errs += innErrs;
    innRuns = 0;
    innHits = 0;
    innErrs = 0;
    outs = 0;
    bases.clear();

  } // end of for loop

  std::cout << name << " scored " << runs << " runs in the game on " << hits << " hits and " << errs << " errors." << std::endl << std::endl;
  return runs;
}

// -----------------------------------------------------------------------------

int Team::sim_game_no_print()
{
  int runs = 0, outs = 0, spot = 0, hits = 0, errs = 0, outcome; // runs will be returned
  int innRuns = 0, innHits = 0, innErrs = 0;
  float value; // Used as a random variable in needed cases
  bool third;
  bool steal;

  std::unordered_map<int, Player> bases; // Contains the players on base

  for (int inning = 1; inning <= INNINGS; inning++) {

    while (outs < 3) {

      value = (float) rand() / RAND_MAX;
      if (bases.count(1) > 0 && bases.count(2) == 0) {
        if (value < bases[1].get_steal_pct()) {
          bases[2] = bases[1];
          bases.erase(1);
        }
        else if (value < (bases[1].get_steal_pct() + bases[1].get_cs_pct())) {
          bases.erase(1);
          outs++;
          if (outs >= 3) break;
        }
      }
      else if (bases.count(2) > 0) {
        steal = false;
        if (value < (bases[2].get_steal_pct() * 0.15)) {
          bases[3] = bases[2];
          bases.erase(2);
          steal = true;
        }
        else if (value < ((bases[2].get_steal_pct() + bases[2].get_cs_pct()) * 0.15)) {
          bases.erase(2);
          outs++;
          if (outs >= 3) break;
          steal = true;
        }
        if (steal && bases.count(1) > 0 && value < (bases[2].get_steal_pct() * 0.15 * 0.575)) {
          bases[2] = bases[1];
          bases.erase(1);
        }
        else if (steal && bases.count(1) > 0 && value < (bases[2].get_steal_pct() * 0.15 * 0.63)) {
          bases.erase(1);
          outs++;
          if (outs >= 3) break;
        }
      }

      outcome = lineup[spot].sim_ab();

      switch (outcome) {
        case 0: // Strikeout
          outs++;
          break;

        case 1: // Single
          innHits++;
          third = false; // Used to represent if third base was occupied before the hit.
          if (bases.count(3) > 0) {
            third = true;
            bases.erase(3);
            innRuns++;
          }
          if (bases.count(2) > 0) {
            value = (float) rand() / RAND_MAX;
            switch (outs) {
              case 0: // Probabilities were found using Stathead for the 2021 season.
                if (third || bases.count(1) > 0) { // Someone on base other than second
                  if ((value + 0.004 * bases[2].get_speed()) < 0.009) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.475) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.983 || bases.count(1) > 0) {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else { // Runner only on second -- lower probability of scoring
                  if ((value + 0.004 * bases[2].get_speed()) < 0.008) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.38) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.966) {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                break;
              case 1: // Cases 1 and 2 are the same as case 0 with different probabilities
                if (bases.count(1) > 0 && !third) { // Runners on first and second
                  if ((value + 0.007 * bases[2].get_speed()) < 0.014) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.591) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else if (bases.count(1) == 0 && third) { // Runners on second and third
                  if ((value + 0.004 * bases[2].get_speed()) < 0.009) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.408) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.922) {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else if (bases.count(1) > 0 && third) { // Bases loaded
                  if ((value + 0.006 * bases[2].get_speed()) < 0.013) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.511) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else { // Runner only on second
                  if ((value + 0.011 * bases[2].get_speed()) < 0.022) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.546) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.966) {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                break;
              case 2:
                if (bases.count(1) > 0 && !third) { // Runners on first and second
                  if ((value + 0.021 * bases[2].get_speed()) < 0.042) {
                    outs++;
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.844) {
                    innRuns++;
                  }
                  else {
                    bases[3] = bases[2];
                  }
                }
                else if (bases.count(1) == 0 && third) { // Runners on second and third
                  if ((value + 0.011 * bases[2].get_speed()) < 0.022) {
                    outs++;
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.823) {
                    innRuns++;
                  }
                  else {
                    bases[3] = bases[2];
                  }
                }
                else if (bases.count(1) > 0 && third) { // Bases loaded
                  if ((value + 0.011 * bases[2].get_speed()) < 0.023) {
                    outs++;
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.813) {
                    innRuns++;
                  }
                  else {
                    bases[3] = bases[2];
                  }
                }
                else { // Runner only on second
                  if ((value + 0.015 * bases[2].get_speed()) < 0.030) {
                    outs++;
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.796) {
                    innRuns++;
                  }
                  else {
                    bases[3] = bases[2];
                  }
                }
                bases.erase(2);
                break;
            }
          }

          if (bases.count(1) > 0) { // Probabilities were taken from Fangraphs
            value = (float) rand() / RAND_MAX;
            if (bases.count(3) > 0 || (value - SPEED_ADJ * bases[1].get_speed()) < .706) { // In this case, a runner on second already moved to third or the runner doesn't advance to third otherwise.
              bases[2] = bases[1];
            }
            else if ((value - 0.005 * bases[1].get_speed()) < .99) { // Half as likely for the fastest player in the league to get thrown out than the average player
              bases[3] = bases[1];
            }
            else {
              outs++;
            }
            bases.erase(1);
          }
          bases[1] = lineup[spot];
          break;

        case 2: // Double
          innHits++;
          if (bases.count(3) > 0) {
            bases.erase(3);
            innRuns++;
          }
          if (bases.count(2) > 0) {
            bases.erase(2);
            innRuns++;
          }
          if (bases.count(1) > 0) {
            value = (float) rand() / RAND_MAX;
            switch (outs) {
              case 0: // Probabilities were found at Baseball Reference's Play Finder from the 2021 MLB Regular Season
                if ((value + 0.006 * bases[1].get_speed()) < 0.013) {
                  outs++;
                }
                else if ((value - SPEED_ADJ * bases[1].get_speed()) < 0.292) {
                  innRuns++;
                }
                else {
                  bases[3] = bases[1];
                }
                break;
              case 1: // Cases 1 and 2 are the same as case 0 with different probabilities
                if ((value + 0.015 * bases[1].get_speed()) < 0.030) {
                  outs++;
                }
                else if ((value - SPEED_ADJ * bases[1].get_speed()) < 0.346) {
                  innRuns++;
                }
                else {
                  bases[3] = bases[1];
                }
                break;
              case 2:
                if ((value + 0.017 * bases[1].get_speed()) < 0.035) {
                  outs++;
                }
                else if ((value - SPEED_ADJ * bases[1].get_speed()) < 0.586) {
                  innRuns++;
                }
                else {
                  bases[3] = bases[1];
                }
                break;
              }
              bases.erase(1);
            }
            bases[2] = lineup[spot];
            break;

        case 3: // Triple
          innHits++;
          if (bases.count(3) > 0) {
            innRuns++;
            bases.erase(3);
          }
          if (bases.count(2) > 0) {
            innRuns++;
            bases.erase(2);
          }
          if (bases.count(1) > 0) {
            innRuns++;
            bases.erase(1);
          }
          bases[3] = lineup[spot];
          break;

        case 4: // Home run
          innHits++;
          innRuns += bases.size();
          innRuns++;
          bases.clear();
          break;

        case 5: // Walk
          if (bases.count(1) > 0) {
            if (bases.count(2) > 0) {
              if (bases.count(3) > 0) {
                innRuns++;
                bases.erase(3);
              }
              bases[3] = bases[2];
              bases.erase(2);
            }
            bases[2] = bases[1];
            bases.erase(1);
          }
          bases[1] = lineup[spot];
          break;

        case 6: // Ground out
          outs++;
          value = (float) rand() / RAND_MAX;
          if (outs >= 3) {
            break;
          }
          if (bases.size() == 3) { // Bases loaded groundout--Probabilities were found at Baseball Reference's Play Finder from the 2021 MLB Regular Season
            if (outs == 1) { // 0 outs before the groundout
              if (value < 0.051) {
                outs++;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases.erase(1);
              }
              else if (value < 0.285) {
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases[1] = lineup[spot];
              }
              else if (value < 0.592) {
                outs++;
                innRuns++;
                bases[3] = bases[2];
                bases.erase(2);
              }
              else if (value < 0.753) {
                innRuns++;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases.erase(1);
              }
              else if (value < 0.979) {
                bases[3] = bases[2];
                bases.erase(2);
                bases[1] = lineup[spot];
              }
              else {
                bases.erase(3);
                bases[2] = bases[1];
                bases[1] = lineup[spot];
              }
            }
            else { // One out before the groundout
              if (value < 0.048) {
                outs++;
                break;
              }
              else if (value < 0.199) {
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases[1] = lineup[spot];
              }
              else if (value < 0.617) {
                outs++;
                break;
              }
              else if (value < 0.742) {
                innRuns++;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases.erase(1);
              }
              else if (value < 0.99) {
                bases[3] = bases[2];
                bases.erase(2);
                bases[1] = lineup[spot];
              }
              else {
                bases.erase(3);
                bases[2] = bases[1];
                bases[1] = lineup[spot];
              }
            }
          }

          else if (bases.count(3) > 0 && bases.count(1) == 0) {
            if (bases.count(2) > 0) { // Runners on second and third
              if (value < 0.662) {
                innRuns++;
                bases.erase(3);
                if (value < .494) {
                  bases[3] = bases[2];
                  bases.erase(2);
                }
              }
              else if (value > 0.986) {
                outs++;
                if (outs >= 3) break;
                innRuns++;
                bases.erase(3);
                bases.erase(2);
              }
            }
            else if (bases.count(1) > 0) { // Runners on first and third
              if (outs == 1) { // 0 outs before the play
                if (value < 0.353) {
                  if (value < 0.323) {
                    innRuns++;
                    bases.erase(3);
                  }
                  outs++;
                }
                else if (value < 0.383) {
                  bases.erase(3);
                  bases[2] = bases[1];
                  bases.erase(1);
                  outs++;
                }
                else if (value < 0.670) {
                  if (value < 0.646) {
                    innRuns++;
                    bases.erase(3);
                  }
                  bases[1] = lineup[spot];
                }
                else {
                  if (value < 0.850) {
                    innRuns++;
                    bases.erase(3);
                  }
                  bases[2] = bases[1];
                  bases.erase(1);
                }
              }
              else { // 1 out before the play
                if (value < 0.513) {
                  outs++;
                  break;
                }
                else if (value < 0.763) {
                  if (value < 0.653) {
                    innRuns++;
                    bases.erase(3);
                  }
                  bases[1] = lineup[spot];
                }
                else {
                  if (value < 0.956) {
                    innRuns++;
                    bases.erase(3);
                  }
                  bases[2] = bases[1];
                  bases.erase(1);
                }
              }
            }
            else { // Only third base occupied
              if (value < 0.484) {
                innRuns++;
                bases.erase(3);
              }
              else if (value > 0.996) {
                outs++;
                bases.erase(3);
              }
            }
          }

          else if (bases.count(2) > 0) {
            if (bases.count(1) > 0) { // Runners on first and second
              if (value < 0.447) {
                outs++;
                if (outs >= 3) break;
                bases[3] = bases[2];
                bases.erase(3);
              }
              else if (value < 0.677) {
                bases[3] = bases[2];
                bases.erase(2);
              }
              else if (value < 0.962) {
                bases[3] = bases[2];
                bases.erase(2);
                bases[1] = lineup[spot];
              }
              else {
                bases[2] = bases[1];
                bases[1] = lineup[spot];
              }
            }
            else { // Runner just on second
              if (value < 0.675) {
                bases[3] = bases[2];
                bases.erase(2);
              }
            }
          }

          else if (bases.count(1) > 0) { // Runner just on first
            if (value < .475) {
              bases.erase(1);
              outs++;
              if (outs >= 3) break;
            }
            else if (value < .742) {
              bases[1] = lineup[spot];
            }
            else {
              bases[2] = bases[1];
              bases.erase(1);
            }
          }
          break;

        case 7: // Fly out
          outs++;
          value = (float) rand() / RAND_MAX;
          if (outs >= 3) break;
          if (bases.count(3) > 0) {
            if (value - SPEED_ADJ * bases[3].get_speed() < 0.529) {
              innRuns++;
              bases.erase(3);
              if (bases.count(2) > 0) {
                value = (float) rand() / RAND_MAX;
                if (value - SPEED_ADJ * bases[2].get_speed() < 0.482) {
                  bases[3] = bases[2];
                  bases.erase(2);
                }
                else if (value > 0.94) { // Doubled off
                  outs++;
                  bases.erase(2);
                  if (outs >= 3) break;
                }
                else break;
              }
            }
            else if (value - 0.009 * bases[3].get_speed() > .981) {
              outs++;
              bases.erase(3);
              if (outs >= 3) break;
            }
          }
          else if (bases.count(2) > 0) {
            if (value - SPEED_ADJ * bases[2].get_speed() < 0.214) {
              bases[3] = bases[2];
              bases.erase(2);
            }
            else if (value > 0.975) {
              outs++;
              bases.erase(2);
              if (outs >= 3) break;
            }
          }
          break;

        case 8: // Hit by Pitch
          if (bases.count(1) > 0) {
            if (bases.count(2) > 0) {
              if (bases.count(3) > 0) {
                innRuns++;
                bases.erase(3);
              }
            bases[3] = bases[2];
            bases.erase(2);
            }
            bases[2] = bases[1];
            bases.erase(1);
          }
          bases[1] = lineup[spot];
          break;

        case 9: // Reached on Error
          innErrs++;
          value = (float) rand() / RAND_MAX;
          if (bases.count(3) > 0) {
            innRuns++;
            bases.erase(3);
          }
          if (value < 0.859) { // Probabilities were found at Baseball Reference's Play Finder from the 2021 MLB Regular Season
            if (bases.count(2) > 0) {
              bases[3] = bases[2];
              bases.erase(2);
            }
            if (bases.count(1) > 0) {
              bases[2] = bases[1];
              bases.erase(1);
            }
            bases[1] = lineup[spot];
          }
          else if (value < 0.993) {
            if (bases.count(2) > 0) {
              innRuns++;
              bases.erase(2);
            }
            if (bases.count(1) > 0) {
              bases[3] = bases[1];
              bases.erase(1);
            }
            if (value < 0.988) {
              bases[2] = lineup[spot];
            }
            else {
              outs++;
            }
          }
          else {
            if (bases.count(2) > 0) {
              innRuns++;
              bases.erase(2);
            }
            if (bases.count(1) > 0) {
              innRuns++;
              bases.erase(1);
            }
            bases[3] = lineup[spot];
          }
          break;


      } // end of outcome switch

      spot++;
      if (spot >= 9) spot = 0;
    } // end of while loop

    runs += innRuns;
    hits += innHits;
    errs += innErrs;
    innRuns = 0;
    innHits = 0;
    innErrs = 0;
    outs = 0;
    bases.clear();

  } // end of for loop

  return runs;
}

//------------------------------------------------------------------------------

int Team::sim_game_no_print(std::vector<Player> lineup_in)
{
  int runs = 0, outs = 0, spot = 0, hits = 0, errs = 0, outcome; // runs will be returned
  int innRuns = 0, innHits = 0, innErrs = 0;
  float value; // Used as a random variable in needed cases
  bool third;
  bool steal;

  std::unordered_map<int, Player> bases; // Contains the players on base

  for (int inning = 1; inning <= INNINGS; inning++) {

    while (outs < 3) {

      value = (float) rand() / RAND_MAX;
      if (bases.count(1) > 0 && bases.count(2) == 0) {
        if (value < bases[1].get_steal_pct()) {
          bases[2] = bases[1];
          bases.erase(1);
        }
        else if (value < (bases[1].get_steal_pct() + bases[1].get_cs_pct())) {
          bases.erase(1);
          outs++;
          if (outs >= 3) break;
        }
      }
      else if (bases.count(2) > 0) {
        steal = false;
        if (value < (bases[2].get_steal_pct() * 0.15)) {
          bases[3] = bases[2];
          bases.erase(2);
          steal = true;
        }
        else if (value < ((bases[2].get_steal_pct() + bases[2].get_cs_pct()) * 0.15)) {
          bases.erase(2);
          outs++;
          if (outs >= 3) break;
          steal = true;
        }
        if (steal && bases.count(1) > 0 && value < (bases[2].get_steal_pct() * 0.15 * 0.575)) {
          bases[2] = bases[1];
          bases.erase(1);
        }
        else if (steal && bases.count(1) > 0 && value < (bases[2].get_steal_pct() * 0.15 * 0.63)) {
          bases.erase(1);
          outs++;
          if (outs >= 3) break;
        }
      }

      outcome = lineup_in[spot].sim_ab();

      switch (outcome) {
        case 0: // Strikeout
          outs++;
          break;

        case 1: // Single
          innHits++;
          third = false; // Used to represent if third base was occupied before the hit.
          if (bases.count(3) > 0) {
            third = true;
            bases.erase(3);
            innRuns++;
          }
          if (bases.count(2) > 0) {
            value = (float) rand() / RAND_MAX;
            switch (outs) {
              case 0: // Probabilities were found using Stathead for the 2021 season.
                if (third || bases.count(1) > 0) { // Someone on base other than second
                  if ((value + 0.004 * bases[2].get_speed()) < 0.009) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.475) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.983 || bases.count(1) > 0) {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else { // Runner only on second -- lower probability of scoring
                  if ((value + 0.004 * bases[2].get_speed()) < 0.008) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.38) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.966) {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                break;
              case 1: // Cases 1 and 2 are the same as case 0 with different probabilities
                if (bases.count(1) > 0 && !third) { // Runners on first and second
                  if ((value + 0.007 * bases[2].get_speed()) < 0.014) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.591) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else if (bases.count(1) == 0 && third) { // Runners on second and third
                  if ((value + 0.004 * bases[2].get_speed()) < 0.009) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.408) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.922) {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else if (bases.count(1) > 0 && third) { // Bases loaded
                  if ((value + 0.006 * bases[2].get_speed()) < 0.013) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.511) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                else { // Runner only on second
                  if ((value + 0.011 * bases[2].get_speed()) < 0.022) {
                    outs++;
                    bases.erase(2);
                  }
                  else if ((value - 0.17 * bases[2].get_speed()) < 0.546) {
                    innRuns++;
                    bases.erase(2);
                  }
                  else if (value < 0.966) {
                    bases[3] = bases[2];
                    bases.erase(2);
                  }
                }
                break;
              case 2:
                if (bases.count(1) > 0 && !third) { // Runners on first and second
                  if ((value + 0.021 * bases[2].get_speed()) < 0.042) {
                    outs++;
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.844) {
                    innRuns++;
                  }
                  else {
                    bases[3] = bases[2];
                  }
                }
                else if (bases.count(1) == 0 && third) { // Runners on second and third
                  if ((value + 0.011 * bases[2].get_speed()) < 0.022) {
                    outs++;
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.823) {
                    innRuns++;
                  }
                  else {
                    bases[3] = bases[2];
                  }
                }
                else if (bases.count(1) > 0 && third) { // Bases loaded
                  if ((value + 0.011 * bases[2].get_speed()) < 0.023) {
                    outs++;
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.813) {
                    innRuns++;
                  }
                  else {
                    bases[3] = bases[2];
                  }
                }
                else { // Runner only on second
                  if ((value + 0.015 * bases[2].get_speed()) < 0.030) {
                    outs++;
                  }
                  else if ((value - SPEED_ADJ * bases[2].get_speed()) < 0.796) {
                    innRuns++;
                  }
                  else {
                    bases[3] = bases[2];
                  }
                }
                bases.erase(2);
                break;
            }
          }

          if (bases.count(1) > 0) { // Probabilities were taken from Fangraphs
            value = (float) rand() / RAND_MAX;
            if (bases.count(3) > 0 || (value - SPEED_ADJ * bases[1].get_speed()) < .706) { // In this case, a runner on second already moved to third or the runner doesn't advance to third otherwise.
              bases[2] = bases[1];
            }
            else if ((value - 0.005 * bases[1].get_speed()) < .99) { // Half as likely for the fastest player in the league to get thrown out than the average player
              bases[3] = bases[1];
            }
            else {
              outs++;
            }
            bases.erase(1);
          }
          bases[1] = lineup_in[spot];
          break;

        case 2: // Double
          innHits++;
          if (bases.count(3) > 0) {
            bases.erase(3);
            innRuns++;
          }
          if (bases.count(2) > 0) {
            bases.erase(2);
            innRuns++;
          }
          if (bases.count(1) > 0) {
            value = (float) rand() / RAND_MAX;
            switch (outs) {
              case 0: // Probabilities were found at Baseball Reference's Play Finder from the 2021 MLB Regular Season
                if ((value + 0.006 * bases[1].get_speed()) < 0.013) {
                  outs++;
                }
                else if ((value - SPEED_ADJ * bases[1].get_speed()) < 0.292) {
                  innRuns++;
                }
                else {
                  bases[3] = bases[1];
                }
                break;
              case 1: // Cases 1 and 2 are the same as case 0 with different probabilities
                if ((value + 0.015 * bases[1].get_speed()) < 0.030) {
                  outs++;
                }
                else if ((value - SPEED_ADJ * bases[1].get_speed()) < 0.346) {
                  innRuns++;
                }
                else {
                  bases[3] = bases[1];
                }
                break;
              case 2:
                if ((value + 0.017 * bases[1].get_speed()) < 0.035) {
                  outs++;
                }
                else if ((value - SPEED_ADJ * bases[1].get_speed()) < 0.586) {
                  innRuns++;
                }
                else {
                  bases[3] = bases[1];
                }
                break;
              }
              bases.erase(1);
            }
            bases[2] = lineup_in[spot];
            break;

        case 3: // Triple
          innHits++;
          if (bases.count(3) > 0) {
            innRuns++;
            bases.erase(3);
          }
          if (bases.count(2) > 0) {
            innRuns++;
            bases.erase(2);
          }
          if (bases.count(1) > 0) {
            innRuns++;
            bases.erase(1);
          }
          bases[3] = lineup_in[spot];
          break;

        case 4: // Home run
          innHits++;
          innRuns += bases.size();
          innRuns++;
          bases.clear();
          break;

        case 5: // Walk
          if (bases.count(1) > 0) {
            if (bases.count(2) > 0) {
              if (bases.count(3) > 0) {
                innRuns++;
                bases.erase(3);
              }
              bases[3] = bases[2];
              bases.erase(2);
            }
            bases[2] = bases[1];
            bases.erase(1);
          }
          bases[1] = lineup_in[spot];
          break;

        case 6: // Ground out
          outs++;
          value = (float) rand() / RAND_MAX;
          if (outs >= 3) {
            break;
          }
          if (bases.size() == 3) { // Bases loaded groundout--Probabilities were found at Baseball Reference's Play Finder from the 2021 MLB Regular Season
            if (outs == 1) { // 0 outs before the groundout
              if (value < 0.051) {
                outs++;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases.erase(1);
              }
              else if (value < 0.285) {
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases[1] = lineup_in[spot];
              }
              else if (value < 0.592) {
                outs++;
                innRuns++;
                bases[3] = bases[2];
                bases.erase(2);
              }
              else if (value < 0.753) {
                innRuns++;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases.erase(1);
              }
              else if (value < 0.979) {
                bases[3] = bases[2];
                bases.erase(2);
                bases[1] = lineup_in[spot];
              }
              else {
                bases.erase(3);
                bases[2] = bases[1];
                bases[1] = lineup_in[spot];
              }
            }
            else { // One out before the groundout
              if (value < 0.048) {
                outs++;
                break;
              }
              else if (value < 0.199) {
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases[1] = lineup_in[spot];
              }
              else if (value < 0.617) {
                outs++;
                break;
              }
              else if (value < 0.742) {
                innRuns++;
                bases[3] = bases[2];
                bases[2] = bases[1];
                bases.erase(1);
              }
              else if (value < 0.99) {
                bases[3] = bases[2];
                bases.erase(2);
                bases[1] = lineup_in[spot];
              }
              else {
                bases.erase(3);
                bases[2] = bases[1];
                bases[1] = lineup_in[spot];
              }
            }
          }

          else if (bases.count(3) > 0 && bases.count(1) == 0) {
            if (bases.count(2) > 0) { // Runners on second and third
              if (value < 0.662) {
                innRuns++;
                bases.erase(3);
                if (value < .494) {
                  bases[3] = bases[2];
                  bases.erase(2);
                }
              }
              else if (value > 0.986) {
                outs++;
                if (outs >= 3) break;
                innRuns++;
                bases.erase(3);
                bases.erase(2);
              }
            }
            else if (bases.count(1) > 0) { // Runners on first and third
              if (outs == 1) { // 0 outs before the play
                if (value < 0.353) {
                  if (value < 0.323) {
                    innRuns++;
                    bases.erase(3);
                  }
                  outs++;
                }
                else if (value < 0.383) {
                  bases.erase(3);
                  bases[2] = bases[1];
                  bases.erase(1);
                  outs++;
                }
                else if (value < 0.670) {
                  if (value < 0.646) {
                    innRuns++;
                    bases.erase(3);
                  }
                  bases[1] = lineup_in[spot];
                }
                else {
                  if (value < 0.850) {
                    innRuns++;
                    bases.erase(3);
                  }
                  bases[2] = bases[1];
                  bases.erase(1);
                }
              }
              else { // 1 out before the play
                if (value < 0.513) {
                  outs++;
                  break;
                }
                else if (value < 0.763) {
                  if (value < 0.653) {
                    innRuns++;
                    bases.erase(3);
                  }
                  bases[1] = lineup_in[spot];
                }
                else {
                  if (value < 0.956) {
                    innRuns++;
                    bases.erase(3);
                  }
                  bases[2] = bases[1];
                  bases.erase(1);
                }
              }
            }
            else { // Only third base occupied
              if (value < 0.484) {
                innRuns++;
                bases.erase(3);
              }
              else if (value > 0.996) {
                outs++;
                bases.erase(3);
              }
            }
          }

          else if (bases.count(2) > 0) {
            if (bases.count(1) > 0) { // Runners on first and second
              if (value < 0.447) {
                outs++;
                if (outs >= 3) break;
                bases[3] = bases[2];
                bases.erase(3);
              }
              else if (value < 0.677) {
                bases[3] = bases[2];
                bases.erase(2);
              }
              else if (value < 0.962) {
                bases[3] = bases[2];
                bases.erase(2);
                bases[1] = lineup_in[spot];
              }
              else {
                bases[2] = bases[1];
                bases[1] = lineup_in[spot];
              }
            }
            else { // Runner just on second
              if (value < 0.675) {
                bases[3] = bases[2];
                bases.erase(2);
              }
            }
          }

          else if (bases.count(1) > 0) { // Runner just on first
            if (value < .475) {
              bases.erase(1);
              outs++;
              if (outs >= 3) break;
            }
            else if (value < .742) {
              bases[1] = lineup_in[spot];
            }
            else {
              bases[2] = bases[1];
              bases.erase(1);
            }
          }
          break;

        case 7: // Fly out
          outs++;
          value = (float) rand() / RAND_MAX;
          if (outs >= 3) break;
          if (bases.count(3) > 0) {
            if (value - SPEED_ADJ * bases[3].get_speed() < 0.529) {
              innRuns++;
              bases.erase(3);
              if (bases.count(2) > 0) {
                value = (float) rand() / RAND_MAX;
                if (value - SPEED_ADJ * bases[2].get_speed() < 0.482) {
                  bases[3] = bases[2];
                  bases.erase(2);
                }
                else if (value > 0.94) { // Doubled off
                  outs++;
                  bases.erase(2);
                  if (outs >= 3) break;
                }
                else break;
              }
            }
            else if (value - 0.009 * bases[3].get_speed() > .981) {
              outs++;
              bases.erase(3);
              if (outs >= 3) break;
            }
          }
          else if (bases.count(2) > 0) {
            if (value - SPEED_ADJ * bases[2].get_speed() < 0.214) {
              bases[3] = bases[2];
              bases.erase(2);
            }
            else if (value > 0.975) {
              outs++;
              bases.erase(2);
              if (outs >= 3) break;
            }
          }
          break;

        case 8: // Hit by Pitch
          if (bases.count(1) > 0) {
            if (bases.count(2) > 0) {
              if (bases.count(3) > 0) {
                innRuns++;
                bases.erase(3);
              }
            bases[3] = bases[2];
            bases.erase(2);
            }
            bases[2] = bases[1];
            bases.erase(1);
          }
          bases[1] = lineup_in[spot];
          break;

        case 9: // Reached on Error
          innErrs++;
          value = (float) rand() / RAND_MAX;
          if (bases.count(3) > 0) {
            innRuns++;
            bases.erase(3);
          }
          if (value < 0.859) { // Probabilities were found at Baseball Reference's Play Finder from the 2021 MLB Regular Season
            if (bases.count(2) > 0) {
              bases[3] = bases[2];
              bases.erase(2);
            }
            if (bases.count(1) > 0) {
              bases[2] = bases[1];
              bases.erase(1);
            }
            bases[1] = lineup_in[spot];
          }
          else if (value < 0.993) {
            if (bases.count(2) > 0) {
              innRuns++;
              bases.erase(2);
            }
            if (bases.count(1) > 0) {
              bases[3] = bases[1];
              bases.erase(1);
            }
            if (value < 0.988) {
              bases[2] = lineup_in[spot];
            }
            else {
              outs++;
            }
          }
          else {
            if (bases.count(2) > 0) {
              innRuns++;
              bases.erase(2);
            }
            if (bases.count(1) > 0) {
              innRuns++;
              bases.erase(1);
            }
            bases[3] = lineup_in[spot];
          }
          break;


      } // end of outcome switch

      spot++;
      if (spot >= 9) spot = 0;
    } // end of while loop

    runs += innRuns;
    hits += innHits;
    errs += innErrs;
    innRuns = 0;
    innHits = 0;
    innErrs = 0;
    outs = 0;
    bases.clear();

  } // end of for loop

  return runs;
}

float Team::sim_multiple_games(int numGames)
{
  int sum = 0;

  for (int i = 0; i < numGames; i++) {
    sum += sim_game_no_print();
  }

  return (float) sum / numGames;
}

float Team::sim_multiple_games(std::vector<Player> lineup_in, int numGames)
{
  int sum = 0;

  for (int i = 0; i < numGames; i++) {
    sum += sim_game_no_print(lineup_in);
  }

  return (float) sum / numGames;
}

void Team::sim_every_lineup()
{

  std::vector<Player> cur_lineup(9, Player());
  //std::vector<std::vector<Player>> all_lineups;
  std::vector<Player> best_lineup(9, Player());

  std::unordered_map<int, bool> lineup_spots;
  std::unordered_map<int, bool> placed;
  bool full;

  float max = 0;
  float cur_runs;
  int games = 1;

  for (int b0 = 0; b0 < LINEUP_SIZE; b0++) {
    placed[0] = false;
    if (lineup_spots.count(b0) == 0 || lineup_spots[b0] != true) {
      cur_lineup[b0] = lineup[0];
      lineup_spots[b0] = true;
      placed[0] = true;
    }
    for (int b1 = 0; b1 < LINEUP_SIZE; b1++) {
      placed[1] = false;
      if (lineup_spots.count(b1) == 0 || lineup_spots[b1] != true) {
        cur_lineup[b1] = lineup[1];
        lineup_spots[b1] = true;
        placed[1] = true;
      }
      for (int b2 = 0; b2 < LINEUP_SIZE; b2++) {
        placed[2] = false;
        if (lineup_spots.count(b2) == 0 || lineup_spots[b2] != true) {
          cur_lineup[b2] = lineup[2];
          lineup_spots[b2] = true;
          placed[2] = true;
        }
        for (int b3 = 0; b3 < LINEUP_SIZE; b3++) {
          placed[3] = false;
          if (lineup_spots.count(b3) == 0 || lineup_spots[b3] != true) {
            cur_lineup[b3] = lineup[3];
            lineup_spots[b3] = true;
            placed[3] = true;
          }
          for (int b4 = 0; b4 < LINEUP_SIZE; b4++) {
            placed[4] = false;
            if (lineup_spots.count(b4) == 0 || lineup_spots[b4] != true) {
              cur_lineup[b4] = lineup[4];
              lineup_spots[b4] = true;
              placed[4] = true;
            }
            for (int b5 = 0; b5 < LINEUP_SIZE; b5++) {
              placed[5] = false;
              if (lineup_spots.count(b5) == 0 || lineup_spots[b5] != true) {
                cur_lineup[b5] = lineup[5];
                lineup_spots[b5] = true;
                placed[5] = true;
              }
              for (int b6 = 0; b6 < LINEUP_SIZE; b6++) {
                placed[6] = false;
                if (lineup_spots.count(b6) == 0 || lineup_spots[b6] != true) {
                  cur_lineup[b6] = lineup[6];
                  lineup_spots[b6] = true;
                  placed[6] = true;
                }
                for (int b7 = 0; b7 < LINEUP_SIZE; b7++) {
                  placed[7] = false;
                  if (lineup_spots.count(b7) == 0 || lineup_spots[b7] != true) {
                    cur_lineup[b7] = lineup[7];
                    lineup_spots[b7] = true;
                    placed[7] = true;
                  }
                  for (int b8 = 0; b8 < LINEUP_SIZE; b8++) {
                    placed[8] = false;
                    if (lineup_spots.count(b8) == 0 || lineup_spots[b8] != true) {
                      cur_lineup[b8] = lineup[8];
                      lineup_spots[b8] = true;
                      placed[8] = true;
                    }
                    full = true;
                    for (int i = 1; i < LINEUP_SIZE; i++) {
                      //std::cout << i << " ";
                      if (lineup_spots[i] == false) {
                        full = false;
                        //std::cout << i << "; ";
                        break;
                      }
                    }
                    if (full) {
                      //lineup = cur_lineup;
                      cur_runs = sim_multiple_games(cur_lineup, games);
                      if (cur_runs > max) {
                        max = cur_runs;
                        best_lineup = cur_lineup;
                      }
                    }
                    if (placed[8] == true)
                      lineup_spots[b8] = false;
                  }
                  if (placed[7] == true)
                    lineup_spots[b7] = false;
                }
                if (placed[6] == true)
                  lineup_spots[b6] = false;
              }
              if (placed[5] == true)
                lineup_spots[b5] = false;
            }
            if (placed[4] == true)
              lineup_spots[b4] = false;
          }
          if (placed[3] == true)
            lineup_spots[b3] = false;
        }
        if (placed[2] == true)
          lineup_spots[b2] = false;
      }
      if (placed[1] == true)
        lineup_spots[b1] = false;
    }
    if (placed[0] == true)
      lineup_spots[b0] = false;
  }


  lineup = best_lineup;
  std::cout << "The best lineup is: ";
  print_lineup();
  std::cout << "The lineup scored " << max << " runs per game in " << games << " games." << std::endl;
}

void Team::sim_multiple_lineups()
{

  std::vector<Player> cur_lineup(9, Player());
  std::vector<std::vector<Player>> best_lineups;
  std::vector<std::vector<int>> intervals;

  int numLineups;
  int games;

  std::unordered_map<int, bool> lineup_spots;
  std::unordered_map<int, bool> placed;
  bool full;

  std::vector<float> max;
  float cur_runs;

  intervals.resize(LINEUP_SIZE, std::vector<int>(2, 0));

  set_lineup();


  std::cout << "For each player, enter the first spot in the order they can go followed by a space, and then the last spot in the order they can go: " << std::endl;
  for (int i = 0; i < lineup.size(); i++) {
    std::cout << lineup[i].get_name() << ": ";
    std::cin >> intervals[i][0] >> intervals[i][1];
  }


  std::cout << "Enter the number of games you would like to simulate for each lineup: ";
  std::cin >> games;

  std::cout << "Enter the number of lineups you would like outputted: ";
  std::cin >> numLineups;

  best_lineups.resize(numLineups, std::vector<Player>(LINEUP_SIZE, Player()));
  max.resize(numLineups, 0);


  for (int b0 = intervals[0][0] - 1; b0 < intervals[0][1]; b0++) {
    placed[0] = false;
    if (lineup_spots.count(b0) == 0 || lineup_spots[b0] != true) {
      cur_lineup[b0] = lineup[0];
      lineup_spots[b0] = true;
      placed[0] = true;
    }
    for (int b1 = intervals[1][0] - 1; b1 < intervals[1][1]; b1++) {
      placed[1] = false;
      if (lineup_spots.count(b1) == 0 || lineup_spots[b1] != true) {
        cur_lineup[b1] = lineup[1];
        lineup_spots[b1] = true;
        placed[1] = true;
      }
      for (int b2 = intervals[2][0] - 1; b2 < intervals[2][1]; b2++) {
        placed[2] = false;
        if (lineup_spots.count(b2) == 0 || lineup_spots[b2] != true) {
          cur_lineup[b2] = lineup[2];
          lineup_spots[b2] = true;
          placed[2] = true;
        }
        for (int b3 = intervals[3][0] - 1; b3 < intervals[3][1]; b3++) {
          placed[3] = false;
          if (lineup_spots.count(b3) == 0 || lineup_spots[b3] != true) {
            cur_lineup[b3] = lineup[3];
            lineup_spots[b3] = true;
            placed[3] = true;
          }
          for (int b4 = intervals[4][0] - 1; b4 < intervals[4][1]; b4++) {
            placed[4] = false;
            if (lineup_spots.count(b4) == 0 || lineup_spots[b4] != true) {
              cur_lineup[b4] = lineup[4];
              lineup_spots[b4] = true;
              placed[4] = true;
            }
            for (int b5 = intervals[5][0] - 1; b5 < intervals[5][1]; b5++) {
              placed[5] = false;
              if (lineup_spots.count(b5) == 0 || lineup_spots[b5] != true) {
                cur_lineup[b5] = lineup[5];
                lineup_spots[b5] = true;
                placed[5] = true;
              }
              for (int b6 = intervals[6][0] - 1; b6 < intervals[6][1]; b6++) {
                placed[6] = false;
                if (lineup_spots.count(b6) == 0 || lineup_spots[b6] != true) {
                  cur_lineup[b6] = lineup[6];
                  lineup_spots[b6] = true;
                  placed[6] = true;
                }
                for (int b7 = intervals[7][0] - 1; b7 < intervals[7][1]; b7++) {
                  placed[7] = false;
                  if (lineup_spots.count(b7) == 0 || lineup_spots[b7] != true) {
                    cur_lineup[b7] = lineup[7];
                    lineup_spots[b7] = true;
                    placed[7] = true;
                  }
                  for (int b8 = intervals[8][0] - 1; b8 < intervals[8][1]; b8++) {
                    placed[8] = false;
                    if (lineup_spots.count(b8) == 0 || lineup_spots[b8] != true) {
                      cur_lineup[b8] = lineup[8];
                      lineup_spots[b8] = true;
                      placed[8] = true;
                    }
                    full = true;
                    for (int i = 0; i < LINEUP_SIZE; i++) {
                      //std::cout << i << " ";
                      if (lineup_spots[i] == false) {
                        full = false;
                        //std::cout << i << "; ";
                        break;
                      }
                    }
                    if (full) {
                      //lineup = cur_lineup;
                      cur_runs = sim_multiple_games(cur_lineup, games);

                      for (int i = 0; i < best_lineups.size(); i++) {
                        if (cur_runs > max[i]) {
                          for (int j = best_lineups.size() - 1; j > i; j--) {
                            best_lineups[j] = best_lineups[j - 1];
                            max[j] = max[j - 1];
                          }
                          best_lineups[i] = cur_lineup;
                          max[i] = cur_runs;
                          break;
                        }
                      }

                    }
                    if (placed[8] == true)
                      lineup_spots[b8] = false;
                  }
                  if (placed[7] == true)
                    lineup_spots[b7] = false;
                }
                if (placed[6] == true)
                  lineup_spots[b6] = false;
              }
              if (placed[5] == true)
                lineup_spots[b5] = false;
            }
            if (placed[4] == true)
              lineup_spots[b4] = false;
          }
          if (placed[3] == true)
            lineup_spots[b3] = false;
        }
        if (placed[2] == true)
          lineup_spots[b2] = false;
      }
      if (placed[1] == true)
        lineup_spots[b1] = false;
    }
    if (placed[0] == true)
      lineup_spots[b0] = false;
  }



  std::cout << "The best lineups are: " << std::endl;
  std::cout << "----------------------" << std::endl;
  for (int i = 0; i < best_lineups.size(); i++) {
    lineup = best_lineups[i];
    std::cout << i + 1 << ".\t";
    print_lineup();
    std::cout << "The lineup scored " << max[i] << " runs per game in " << games << " games." << std::endl << std::endl;
  }
}
