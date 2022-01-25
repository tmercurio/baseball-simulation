// File Name: Player.cpp
// Author: Thomas Mercurio
// Email: tmercuri@nd.edu
// This is the function definition file for the Player class

#include "../inc/Player.h"

std::string int_to_outcome(int i)
{
  switch (i) {
    case 0:
      return "Strikeout";
    case 1:
      return "Single";
    case 2:
      return "Double";
    case 3:
      return "Triple";
    case 4:
      return "Home Run";
    case 5:
      return "Walk";
    case 6:
      return "Ground Out";
    case 7:
      return "Fly Out";
    case 8:
      return "HBP";
    case 9:
      return "Reach on Error";
    default:
      return "Total";
  }
}

Player::Player():  name(), number(), stats(), probs(), cum_probs(), speed(), steal_pct(), cs_pct() {}

Player::Player(std::string name, int num): name(name), number (num), stats(), probs(), cum_probs(),
                speed(), steal_pct(), cs_pct() {}

Player::Player(std::string name, int num, int pa, int hits, int doubles, int triples,
          int homers, int walks, int strikeouts, int hbps, int double_plays,
              int sac_flies, int stolen_bases, int caught_stealing) :
      name(name), number(num), stats{pa, hits, doubles, triples, homers, walks,
        strikeouts, hbps, double_plays, sac_flies, stolen_bases, caught_stealing},
          probs(), cum_probs(), speed(), steal_pct(), cs_pct()
           {set_probs(); set_speed();}

void Player::set_name(std::string name)
{
  name = name;
}

void Player::set_number(int num)
{
  number = num;
}

void Player::set_stats(int pa, int hits, int doubles, int triples, int homers, int walks,
                  int strikeouts, int hbps, int double_plays, int sac_flies, int stolen_bases,
                      int caught_stealing)
{
  stats.pa = pa;
  stats.hits = hits;
  stats.doubles = doubles;
  stats.triples = triples;
  stats.homers = homers;
  stats.walks = walks;
  stats.strikeouts = strikeouts;
  stats.hbps = hbps;
  stats.double_plays = double_plays;
  stats.sac_flies = sac_flies;
  stats.stolen_bases = stolen_bases;
  stats.caught_stealing = caught_stealing;
}

// Sets the vector of probabilities for a player for each outcome
void Player::set_probs()
{
  probs.resize(OUTCOMES);
  probs[STRIKEOUT] = (float) stats.strikeouts / stats.pa;
  probs[SINGLE] = (float) (stats.hits - stats.doubles - stats.triples - stats.homers) / stats.pa;
  probs[DOUBLE] = (float) stats.doubles / stats.pa;
  probs[TRIPLE] = (float) stats.triples / stats.pa;
  probs[HOME_RUN] = (float) stats.homers / stats.pa;
  probs[WALK] = (float) stats.walks / stats.pa;
  probs[GROUND_OUT] = (1 - ERR_FACTOR) * (GO_AO_ADJUSTMENT * stats.double_plays / stats.sac_flies / 2) *
                          (stats.pa - stats.hits - stats.walks - stats.strikeouts - stats.hbps) / stats.pa;
  probs[FLY_OUT] = (1 - ERR_FACTOR) * (1 - (GO_AO_ADJUSTMENT * stats.double_plays / stats.sac_flies / 2)) *
                          (stats.pa - stats.hits - stats.walks - stats.strikeouts - stats.hbps) / stats.pa;
  probs[HBP] = (float) stats.hbps / stats.pa;
  probs[ROE] = ERR_FACTOR * (stats.pa - stats.hits - stats.walks - stats.strikeouts - stats.hbps) / stats.pa;

  set_cum_probs();
}

// Loops through stats and sets a cumulative density function to values in the vector
void Player::set_cum_probs()
{
  cum_probs.resize(probs.size() + 1);

  float sum = 0;
  float cur_pct;

  cum_probs[0] = sum;

  for (int i = 0; i < probs.size(); i++) {
    cur_pct = probs[i];
    sum += cur_pct;
    cum_probs[i + 1] = sum;
  }
}

/* Loops through the cumulative probabilities vector and outputs the result of an
   at bat based on a random number */
int Player::sim_ab()
{
  float value = (float) rand() / (RAND_MAX);

  //std::cout << value << std::endl;

  for (int i = 0; i < cum_probs.size() - 1; i++) {
    if (value > cum_probs[i] && value < cum_probs[i + 1])
      return i;
  }

  return cum_probs.size() - 1;
}

// Using 2021 stats, sets a relative speed for each player almost exclusively between -1 and 1
// which is then used to tweak the outcome probabilities for running the bases
void Player::set_speed()
{
  // Stolen bases divided by times on first base plus 0.15 times times on second bases, since
  // steals of third base only occur 15% as much as those of second base.
  steal_pct = stats.stolen_bases / ((stats.hits - stats.homers - stats.triples - stats.doubles) + stats.walks + stats.hbps + 0.15 * stats.doubles);
  cs_pct = stats.caught_stealing / ((stats.hits - stats.homers - stats.triples - stats.doubles) + stats.walks + stats.hbps + 0.15 * stats.doubles);

  float raw_speed = steal_pct * 100 + stats.triples / stats.doubles * 20;

  if (raw_speed > 5.60) // Median raw_speed of 132 qualifying hitters from 2021
    speed = (raw_speed - 5.60) / 17.07; // Positive speed score; only Marte, Chisholm, and Merrifield have higher than 1 speed
  else
    speed = (raw_speed - 5.60) / 5.60; // Negative speed score; lowest possible is -1

  steal_pct /= MOVE_WAIT;
  cs_pct /= MOVE_WAIT;
}

void Player::print_ab(int ab)
{

  switch (ab) {
    case 0:
      std::cout << name << " struck out." << std::endl;
      break;
    case 1:
      std::cout << name << " singled. ";
      break;
    case 2:
      std::cout << name << " doubled. ";
      break;
    case 3:
      std::cout << name << " tripled. ";
      break;
    case 4:
      std::cout << name << " homered! ";
      break;
    case 5:
      std::cout << name << " walked. ";
      break;
    case 6:
      std::cout << name << " grounded";
      break;
    case 7:
      std::cout << name << " flew out. ";
      break;
    case 8:
      std::cout << name << " was hit by a pitch. ";
      break;
    case 9:
      std::cout << name << " reached on an error. ";
      break;
    }
}

std::string Player::get_name() const
{
  return name;
}

int Player::get_number() const
{
  return number;
}

Stats Player::get_stats() const
{
  return stats;
}

std::vector<float> Player::get_probs() const
{
  return probs;
}

std::vector<float> Player::get_cum_probs() const
{
  return cum_probs;
}

float Player::get_speed() const
{
  return speed;
}

float Player::get_steal_pct() const
{
  return steal_pct;
}

float Player::get_cs_pct() const
{
  return cs_pct;
}

std::ostream& operator<<(std::ostream& output, const Player &P)
{
  output << "Name: " << P.name << std::endl;
  output << "Number: " << P.number << std::endl << std::endl;

  output << "Plate Appearances: " << P.stats.pa << std::endl;
  output << "Hits: " << P.stats.hits << std::endl;
  output << "Doubles: " << P.stats.doubles << std::endl;
  output << "Triples: " << P.stats.triples << std::endl;
  output << "Home Runs: " << P.stats.homers << std::endl;
  output << "Walks: " << P.stats.walks << std::endl;
  output << "Strikeouts: " << P.stats.strikeouts << std::endl;
  output << "HBP: " << P.stats.hbps << std::endl;
  output << "GIDP: " << P.stats.double_plays << std::endl;
  output << "Sac Flies: " << P.stats.sac_flies << std::endl;
  output << "Stolen Bases: " << P.stats.stolen_bases << std::endl;
  output << "Caught Stealing: " << P.stats.caught_stealing << std::endl;

  std::cout << "Individual Probabilities: " << std::endl;
  for (int i = 0; i < P.probs.size(); i++) {
    std::cout << int_to_outcome(i) << ": ";
    std::cout << P.probs[i] << std::endl;
  }

  std::cout << std::endl;

  std::cout << "Cumulative Probabilities: " << std::endl;
  for (int i = 0; i < P.cum_probs.size(); i++) {
    std::cout << int_to_outcome(i) << ": ";
    std::cout << P.cum_probs[i] << std::endl;
  }

  return output;
}

std::istream& operator>>(std::istream& in, Player& P)
{
  std::string name;
  int number;

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

  if (P.name.empty()) {
    std::cout << "Enter the player's name: ";
    in >> name;
    std::cout << "Enter the player's number: ";
    in >> number;
    P.set_name(name);
    P.set_number(number);
  }

  std::cout << "Plate appearances: ";
  in >> pa;
  std::cout << "Hits: ";
  in >> hits;
  std::cout << "Doubles: ";
  in >> doubles;
  std::cout << "Triples: ";
  in >> triples;
  std::cout << "Home Runs: ";
  in >> homers;
  std::cout << "Walks: ";
  in >> walks;
  std::cout << "Strikeouts: ";
  in >> strikeouts;
  std::cout << "HBP: ";
  in >> hbps;
  std::cout << "GIDP: ";
  in >> double_plays;
  std::cout << "Sac Flies: ";
  in >> sac_flies;
  std::cout << "Stolen Bases: ";
  in >> stolen_bases;
  std::cout << "Caught Stealing: ";
  in >> caught_stealing;

  P.set_stats(pa, hits, doubles, triples, homers, walks, strikeouts, hbps,
                double_plays, sac_flies, stolen_bases, caught_stealing);

  return in;
}
