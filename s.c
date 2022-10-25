int speedmods[3] = {1,0,-1};
  int slowdowntoend = 0;
  while (position != final_position)
  {
    solution_2.positions[move_number] = position;

    // loop through speedmods in order of importance
    // +1, = or -1 in that order
    if (slowdowntoend == 0)
    {
      for(int i = 0; i < 3; i++)
      {
        solution_2_count++;  // increment counter per check

        int newspeed = speed + speedmods[i]; // calculate newspeed
        int speedchecker = position;
        int possibleMove = 1;
        if (newspeed > _max_road_speed_)
        {
          continue;
        }

        // Check if the next newspeed*(newspeed+1)/2 indexes are above speed limit

        for(int j = 0; j <= newspeed; j++){
          if (possibleMove == 0)
          { 
            break;          
          }
          int maxspeed = newspeed - j;

          if (maxspeed < _min_road_speed_){
            maxspeed = _min_road_speed_;
          }

          for(int k = 0; k < newspeed-j; k++){
            speedchecker++;

            if (speedchecker >= final_position)
            {
              slowdowntoend = 1;
            }

            if(max_road_speed[speedchecker] < maxspeed && slowdowntoend != 1)
            {
              possibleMove = 0;
            }
          }
        }

        if (possibleMove == 1){
          position += newspeed;
          speed = newspeed;
          move_number += 1; 
        } 
        
      }

    } else {
      position += speed;
      speed -= 1;
      move_number += 1;
    }
  }
  speed -= 1;
  solution_2.positions[move_number] = position;
  solution_2_best = solution_2;
  return;