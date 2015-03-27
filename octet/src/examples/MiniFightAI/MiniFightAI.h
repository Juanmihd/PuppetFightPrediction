////////////////////////////////////////////////////////////////////////////////
//
// MiniFightAI is the name of the whole app. 
// This app will contain two players, the user interface.
// The player can be playable or AI controlled! 
// Player 1 has to be a player always!
//

#include "Puppet.h"
#include "UI.h"
#include "PredictiveAI.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

namespace octet {
  namespace PuppetFight{
    /// @brief This class contains the information of the 'game' itself. 
    ///
    class MiniFightAI : public app {
      enum GameState { _INTRO = -1, _INTRO_ANIMATION = 0, _PLAYING = 1, _GAME_OVER = 2 } _game_state;
      //This boolean will containg the cheating mode
      bool cheating;
      //This contains if the player one is controlled by an AI
      int player_one_AI;
      //This contains if the player two is controlled by an AI
      int player_two_AI;
      //This contains the information whereas player one won or lost
      bool player_one_won;
      bool mid_lapse_done;
      //This contains the information of the AI (1-3)
      int type_one_AI;
      //This contains the information of the AI (1-3)
      int type_two_AI;
      //This functions is used to control the 'steps' of the animations (win animation)
      int steps;
      //This will contain the time lapse of the intro animation
      float time_animation;
      //This will contain the lapse of each movement (whenever the actions are resolved)
      float time_lapse;
      //This will be the half of the time the actions are resolved (to show another intermediate frame)
      float half_time_lapse;
      //This two counters are used to control the animation in the intro and the distance between the actions
      std::chrono::time_point<std::chrono::system_clock> previous_action;
      std::chrono::time_point<std::chrono::system_clock> cur_animation;
      std::chrono::time_point<std::chrono::system_clock> click_time;
      // scene for drawing box
      ref<visual_scene> app_scene;
      //This contains the User Interface and the stage
      StageUI stage_puppet;
      //The two players
      Puppet player_one;
      Puppet player_two;
      //This is the module of the AI (implemented with nGram)
      PredictiveAI predictiveAI;
      PredictiveAI predictiveAI_one;

    public:
      /// @brief This is the constructior with parameters (to obtain some information if needed
      MiniFightAI(int argc, char **argv) : app(argc, argv) {
      }

      /// @brief This will be called to initializate the game
      void app_init() {
        //Set really random seed
        unsigned int seed = (unsigned int) std::chrono::system_clock::now().time_since_epoch().count();
        random_gen.set_seed(seed);
        //Create a new scene
        app_scene =  new visual_scene();
        //Init the predictive AI
        predictiveAI.init(4); 
        //Init some booleans, for victory, AI, and chea
        player_one_AI = player_two_AI = 0;
        cheating = false;
        player_one_won = false;
        mid_lapse_done = false;
        //Select the starting mode of the AI (mode 1 mimic)
        type_two_AI = type_one_AI = 1;
        //Set up the scene with default camera and lights
        app_scene->create_default_camera_and_lights();
        //Init the user interface and the players
        stage_puppet.init(app_scene);
        player_one.init(app_scene);
        player_two.init(app_scene,-1);
        //Set up some variables to control the animations and actions lapse
        time_lapse = 0.3f;
        half_time_lapse = time_lapse * 0.5f;
        time_animation = 1.5f;
        previous_action = std::chrono::system_clock::now();
        //Set up the game state to "intro"
        _game_state = _INTRO;
      }

      /// @brief This function will reste the game to start playing again!
      void reset_game(){
        //Reste puppets and stage
        player_one.reset_puppet();
        player_two.reset_puppet();
        stage_puppet.reset_UI();
        //Set up the intro animation
        _game_state = _INTRO_ANIMATION;
        cur_animation = std::chrono::system_clock::now();
        //Reset the speed of the game, in case that it was changed
        time_lapse = 0.3f;
        half_time_lapse = time_lapse * 0.5f;
      }

      /// @brief This will be called when clicked the P V AI button
      void button_p1_ai(int value){
        player_one_AI = value;
      }

      /// @brief This will be called when clicked the P V P button
      void button_p2_ai(int value){
        player_two_AI = value;
      }

      /// @brief This will be called when the buttons on type of AI of player 1 are clicked
      void button_p1_type(int value){
        type_one_AI = value;
      }

      /// @brief This will be called when the buttons on type of AI of player 2 are clicked
      void button_p2_type(int value){
        type_two_AI = value;
      }

      /// @brief This will be called when clicked the 'invisible' cheating button (just below P V AI)
      void button_cheating(){
        cheating = !cheating;
        if (cheating){
          player_one.set_life_damage(0);
          player_two.set_life_damage(0);
        }
        else{
          player_one.set_life_damage(5);
          player_two.set_life_damage(5);
        }
      }

      /// @brief This will be called when the speed buttons are pressed
      void button_speed(float inc){
        time_lapse += inc;
        if (time_lapse < 0.1f) time_lapse = 0.1f;
        else if (time_lapse > 1.0f) time_lapse = 1.0f;
        half_time_lapse = time_lapse * 0.5f;
      }

      /// @brief This function controls all the actions of the mouse
      void mouse(){
        if (is_key_going_down(key_lmb)){
          if (_game_state == _INTRO){ //If the game is in "intro", launch the intro animation
            _game_state = _INTRO_ANIMATION;
            cur_animation = std::chrono::system_clock::now();
          }
          else{
            int x = 0;
            int y = 0;
            get_mouse_pos(x, y);
            int vx = 0;
            int vy = 0;
            get_viewport_size(vx, vy);
            float rx = 1.0f*x / vx;
            float ry = 1.0f*y / vy;
            //printf("X%f, Y%f\n",rx, ry);
            if (rx <= 0.42f){
              if (rx < 0.19f){
                if (rx > 0.12f){
                  if (ry >= 0.72f && ry <= 0.78f){
                    button_p1_ai(0);
                    stage_puppet.set_button_posx(0, 2);
                  }
                  else if (ry >= 0.8f && ry <= 0.85f){
                    button_p1_type(1);
                    stage_puppet.set_button_posx(2, 2);
                  }
                  else if (ry >= 0.88f && ry <= 0.94f){
                    predictiveAI_one.set_dimension(1);
                    stage_puppet.set_button_posx(4, 2);
                  }
                }
              }
              else if (rx < 0.35f){
                if (rx > 0.236f && rx < 0.30f){
                  if (ry >= 0.72f && ry <= 0.78f){
                    button_p1_ai(1);
                    stage_puppet.set_button_posx(0, 1);
                  }
                  else if (ry >= 0.8f && ry <= 0.85f){
                    button_p1_type(2);
                    stage_puppet.set_button_posx(2, 1);
                  }
                  else if (ry >= 0.88f && ry <= 0.94f){
                    predictiveAI_one.set_dimension(4);
                    stage_puppet.set_button_posx(4, 1);
                  }
                }
              }
              else if (rx < 0.42f){
                if (ry >= 0.72f && ry <= 0.78f){
                  stage_puppet.set_button_posx(0, 0);
                  button_p1_ai(-1);
                }
                else if (ry >= 0.8f && ry <= 0.85f){
                  button_p1_type(3);
                  stage_puppet.set_button_posx(2, 0);
                }
                else if (ry >= 0.88f && ry <= 0.94f){
                  predictiveAI_one.resetAI();
                  stage_puppet.press_forget1_button(true);
                  click_time = std::chrono::system_clock::now();
                }
              }
            }
            else if(rx >= 0.58f){
              if (rx > 0.81f){
                if (rx < 0.88f){
                  if (ry >= 0.72f && ry <= 0.78f){
                    button_p2_ai(-1);
                    stage_puppet.set_button_posx(1, 2);
                  }
                  else if (ry >= 0.8f && ry <= 0.85f){
                    button_p2_type(3);
                    stage_puppet.set_button_posx(3, 2);
                  }
                  else if (ry >= 0.88f && ry <= 0.94f){
                    predictiveAI.resetAI();
                    stage_puppet.press_forget2_button(true);
                    click_time = std::chrono::system_clock::now();
                  }
                }
              }
              else if (rx > 0.65f){
                if (rx < 0.764f && rx > 0.7f){
                  if (ry >= 0.72f && ry <= 0.78f){
                    button_p2_ai(1);
                    stage_puppet.set_button_posx(1, 1);
                  }
                  else if (ry >= 0.8f && ry <= 0.85f){
                    button_p2_type(2);
                    stage_puppet.set_button_posx(3, 1);
                  }
                  else if (ry >= 0.88f && ry <= 0.94f){
                    predictiveAI.set_dimension(4);
                    stage_puppet.set_button_posx(5, 1);
                  }
                }
              }
              else if (rx > 0.58f){
                if (ry >= 0.72f && ry <= 0.78f){
                  button_p2_ai(0);
                  stage_puppet.set_button_posx(1, 0);
                }
                else if (ry >= 0.8f && ry <= 0.85f){
                  button_p2_type(1);
                  stage_puppet.set_button_posx(3, 0);
                }
                else if (ry >= 0.88f && ry <= 0.94f){
                  predictiveAI.set_dimension(1);
                  stage_puppet.set_button_posx(5, 0);
                }
              }
            }
            else{
              if (ry >= 0.77f && ry <= 0.83f){
                reset_game();
                stage_puppet.press_reset_button(true);
                click_time = std::chrono::system_clock::now();
              }
              else if (ry >= 0.88f && ry <= 0.94f)
                if (rx > 0.5f){
                  button_speed(-0.05f);
                  stage_puppet.press_plus_button(true);
                  click_time = std::chrono::system_clock::now();
                }
                else{
                  button_speed(0.05f);
                  stage_puppet.press_minus_button(true);
                  click_time = std::chrono::system_clock::now();
                }
              else
                button_cheating();
            }
          }
        }
      }

      /// @brief This function will control all the inputs from keyboard
      void keyboard(){
        //Player 1
        if (!player_one.is_finishing()){
          if (player_one_AI == -1){
            actions action = player_one.random_action();
            if (action == MOVE_RIGHT){
              if (!player_one.collision_with(player_two)){
                player_one.input_action(MOVE_RIGHT);
              }
              else{
                player_one.input_action(PUNCH_MID);
              }
            }
            else if (action == MOVE_LEFT){
              if (player_one.get_position() >= -14){
                player_one.input_action(MOVE_LEFT);
              }
            }
          }
          else if (player_one_AI == 0){
            if (is_key_down('A')){
              if (player_one.get_position() >= -14){
                player_one.input_action(MOVE_LEFT);
              }
            }
            else if (is_key_down('D')){
              if (!player_one.collision_with(player_two)){
                player_one.input_action(MOVE_RIGHT);
              }
              else{
                player_one.input_action(PUNCH_MID);
              }
            }
            else if (is_key_down('E')){
              player_one.input_action(PUNCH_UP);
            }
            else if (is_key_down('C')){
              player_one.input_action(PUNCH_DOWN);
            }
            else if (is_key_down('W')){
              player_one.input_action(BLOCK_UP);
            }
            else if (is_key_down('S')){
              player_one.input_action(BLOCK_MID);
            }
            else if (is_key_down('X')){
              player_one.input_action(BLOCK_DOWN);
            }
          }
          else{
            actions predicted_action = static_cast<actions> (predictiveAI_one.predict());
            switch (type_one_AI){
            case 1:
              player_one.AI_reaction_mimic(predicted_action, player_two);
              break;
            case 2:
              player_one.AI_reaction_defense(predicted_action, player_two);
              break;
            case 3:
              player_one.AI_reaction_balanced(predicted_action, player_two);
              break;
            }
          }
        }
        //Player 2
        if (!player_two.is_finishing() && player_two_AI == 0){
          if (is_key_down('G')){
            if (!player_two.collision_with(player_one))
              player_two.input_action(MOVE_LEFT);
            else
              player_two.input_action(PUNCH_MID);
          }
          else if (is_key_down('J')){
            if (player_two.get_position() <= 14)
              player_two.input_action(MOVE_RIGHT);
          }
          else if (is_key_down('T')){
            player_two.input_action(PUNCH_UP);
          }
          else if (is_key_down('B')){
            player_two.input_action(PUNCH_DOWN);
          }
          else if (is_key_down('Y')){
            player_two.input_action(BLOCK_UP);
          }
          else if (is_key_down('H')){
            player_two.input_action(BLOCK_MID);
          }
          else if (is_key_down('N')){
            player_two.input_action(BLOCK_DOWN);
          }
        }
      }

      /// @brief This function will be called on the half of each 'game-frame'
      void mid_frame(){
        mid_lapse_done = true;
        if (_game_state == _PLAYING){
          //Predict actions
          actions predicted_action = static_cast<actions> (predictiveAI.predict());
          //printf(" predicted action => %i vs action => %i\n", predicted_action, player_one.get_action());
          //Decide action
          if (player_two_AI == 1){ // AI MOVEMENT
            switch (type_two_AI){
            case 1:
              player_two.AI_reaction_mimic(predicted_action, player_one);
              break;
            case 2:
              player_two.AI_reaction_defense(predicted_action, player_one);
              break;
            case 3:
              player_two.AI_reaction_balanced(predicted_action, player_one);
              break;
            }
          }
          else if (player_two_AI == -1){ // RANDOM MOVEMENT!
            actions action = player_two.random_action();
            if (action == MOVE_LEFT){
              if (!player_two.collision_with(player_one)){
                player_two.input_action(MOVE_LEFT);
              }
              else{
                player_two.input_action(PUNCH_MID);
              }
            }
            else if (action == MOVE_RIGHT){
              if (player_two.get_position() <= 14){
                player_two.input_action(MOVE_RIGHT);
              }
            }
            else{
              player_two.input_action(action);
            }
          }
        }
        else if (_game_state == _GAME_OVER){
          if (steps > 0){
            if (player_one_won){
              player_one.move_left();
            }
            else{
              player_two.move_right();
            }
            --steps;
          }
          else if (steps > -2){
            if (player_one_won){
              player_one.move_right();
            }
            else{
              player_two.move_left();
            }
            --steps;
          }
          else{
            steps = 2;
          }
        }
      }

      /// @brief This function will be called on the end of each 'game-frame'
      void last_frame(){
        //Be sure that the mid lapse happened!
        if (!mid_lapse_done)
          mid_frame();
        mid_lapse_done = false;
        //Reset buttons
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_seconds = now - click_time;
        if (elapsed_seconds.count() > 0.05f){
          stage_puppet.press_reset_button(false);
          stage_puppet.press_plus_button(false);
          stage_puppet.press_minus_button(false);
          stage_puppet.press_forget1_button(false);
          stage_puppet.press_forget2_button(false);
        }
        // Mid frame (small animation + prediction)
        if (_game_state == _PLAYING){
          //Memorize actions
          if (player_one.get_action() != NONE_ACTION && player_one.get_action() < FINISHING)
            predictiveAI.new_input(player_one.get_action());
          //Memorize actions
          if (player_two.get_action() != NONE_ACTION && player_two.get_action() < FINISHING)
            predictiveAI_one.new_input(player_two.get_action());
          //Check if somebody won!
          if (player_one.get_life() <= 0){
            //Player one lost!
            player_one_won = false;
            player_one.move_left();
            steps = 2;
            _game_state = _GAME_OVER;
          }
          else if (player_two.get_life() <= 0){
            //Player one won!
            player_one_won = true;
            player_two.move_right();
            steps = 2;
            _game_state = _GAME_OVER;
          }
        }

        //Resolve actions
        if (player_one.execute_action(player_two)){
          stage_puppet.update_lifes(player_one.get_life(), player_two.get_life());
        }
        if (player_two.execute_action(player_one)){
          stage_puppet.update_lifes(player_one.get_life(), player_two.get_life());
        }

        stage_puppet.update_energies(player_one.get_energy(), player_two.get_energy());
      }

      /// @brief This is called to draw the world
      void draw_world(int x, int y, int w, int h) {
        int vx = 0, vy = 0;
        get_viewport_size(vx, vy);
        app_scene->begin_render(vx, vy);
        // obtain input from mouse and keyboard
        mouse();
        if (_game_state == _INTRO){
          player_one.change_text();
          player_two.change_text();
        }
        else if (_game_state == _PLAYING){
          keyboard();
        }
        else if (_game_state == _INTRO_ANIMATION){
          //Prepare to check frames
          std::chrono::time_point<std::chrono::system_clock> now_animation = std::chrono::system_clock::now();
          std::chrono::duration<float> elapsed_seconds = now_animation - cur_animation;
          float t = 1.0f * elapsed_seconds.count() / time_animation;
          if (t <= 1.0f){
            player_one.animate_intro(t);
            player_two.animate_intro(t);
            stage_puppet.animate_intro(t);
            /*if (t <= 0.7f){
              player_one.change_text();
              player_two.change_text();
            }*/
          }
          else{
            player_one.animate_intro(1.0f);
            player_two.animate_intro(1.0f);
            stage_puppet.animate_intro(1.0f);
            _game_state = _PLAYING;
          }
        }
        //Prepare to check frames
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_seconds = now - previous_action;

        // Mid frame (small animation + prediction)
        if (elapsed_seconds.count() > half_time_lapse){
          //Run mid_frame
          mid_frame();
          //Stop checking half_time_lapse
          half_time_lapse = time_lapse * 2.0f;
        }

        // last frame (finish animation + execute actions!)
        if (elapsed_seconds.count() > time_lapse){
          //Run last_frame
          last_frame();
          //Reset timer
          previous_action = now;
          half_time_lapse = time_lapse * 0.5f;
        }

        // update matrices. assume 30 fps.
        app_scene->update(1.0f/30);

        // draw the scene
        app_scene->render((float)vx / vy);

      }
    };
  }
}
