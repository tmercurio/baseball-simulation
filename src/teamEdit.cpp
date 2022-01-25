int Team::sim_game_no_print()
{
  int runs = 0, outs = 0, spot = 0, hits = 0, errs = 0, outcome; // runs will be returned
  int innRuns = 0, innHits = 0, innErrs = 0;
  float value; // Used as a random variable in needed cases
  bool third;

  std::unordered_map<int, Player> bases; // Contains the players on base

  for (int inning = 1; inning <= INNINGS; inning++) {

    while (outs < 3) {
      outcome = players[spot].sim_ab();

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
          bases[1] = players[spot];
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
            bases[2] = players[spot];
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
          bases[3] = players[spot];
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
          bases[1] = players[spot];
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
                bases[1] = players[spot];
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
                bases[1] = players[spot];
              }
              else {
                bases.erase(3);
                bases[2] = bases[1];
                bases[1] = players[spot];
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
                bases[1] = players[spot];
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
                bases[1] = players[spot];
              }
              else {
                bases.erase(3);
                bases[2] = bases[1];
                bases[1] = players[spot];
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
                  bases[1] = players[spot];
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
                  bases[1] = players[spot];
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
                bases[1] = players[spot];
              }
              else {
                bases[2] = bases[1];
                bases[1] = players[spot];
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
              bases[1] = players[spot];
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
          bases[1] = players[spot];
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
            bases[1] = players[spot];
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
              bases[2] = players[spot];
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
            bases[3] = players[spot];
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
