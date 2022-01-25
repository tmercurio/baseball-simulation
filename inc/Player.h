// File Name: Player.h
// Author: Thomas Mercurio
// Email: tmercuri@nd.edu
// This is the function header file for the Player class

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <vector>
#include <iomanip>

// Number of outcomes of each at bat possible
#define OUTCOMES 10

/* Used to calculate the proportion of ground outs to air outs based on double
plays grounded into and sacrifice flies hit */
#define GO_AO_ADJUSTMENT 0.3368

/* Proportion of the time an out is recorded for a player when he actually reaches
on an error, based on league error data from 1960-2004 and ground ball/fly ball
rates from 2021 */
#define ERR_FACTOR 0.025

/* A factor that is used in baserunning calculations, meaning that where it is used,
the fastest player in the league is 9% more likely to advance an extra base than an
average player, while the slowest is 9% less likely */
#define SPEED_ADJ 0.09

// represents for how many at bats a runner will stay on a base; used in stolen
// base and caught stealing calculations
#define MOVE_WAIT 1.4; 


typedef struct Stats {

  int pa;
  int hits;
  int doubles;
  int triples;
  int homers;
  int walks;
  int strikeouts;
  int hbps;
  int double_plays;
  int sac_flies;
  int stolen_bases;
  int caught_stealing;

} Stats;

enum outcome {
  STRIKEOUT,
  SINGLE,
  DOUBLE,
  TRIPLE,
  HOME_RUN,
  WALK,
  GROUND_OUT,
  FLY_OUT,
  HBP,
  ROE // reached on error
};

std::string int_to_outcome(int i);

class Player {

  private:

    std::string name;
    int number;
    Stats stats;
    std::vector<float> probs;
    std::vector<float> cum_probs; // Contains cumulative density function values used in simulating the result of an at bat
    float speed; // A factor almost exclusively between -1 and 1 representing the relative speed of the player
    float steal_pct; // The chance that a player steals when on base
    float cs_pct; // The chance that a player is caught stealing when on base

  public:

    Player();

    Player(std::string name, int num);

    Player(std::string name, int num, int pa, int hits, int doubles, int triples,
              int homers, int walks, int strikeouts, int hbps, int double_plays,
                  int sac_flies, int stolen_bases, int caught_stealing);

    void set_name(std::string name);

    void set_number(int num);

    void set_probs();

    void set_cum_probs();

    void set_speed();

    void set_stats(int pa, int hits, int doubles, int triples, int homers, int walks,
                      int strikeouts, int hbps, int double_plays, int sac_flies, int stolen_bases,
                          int caught_stealing);

    int sim_ab();

    void print_ab(int ab);

    std::string get_name() const;

    int get_number() const;

    Stats get_stats() const;

    std::vector<float> get_probs() const;

    std::vector<float> get_cum_probs() const;

    float get_speed() const;

    float get_steal_pct() const;

    float get_cs_pct() const;

    friend std::ostream& operator<<(std::ostream& out, const Player &P);

    friend std::istream& operator>>(std::istream& in, Player &P);

};

#endif
