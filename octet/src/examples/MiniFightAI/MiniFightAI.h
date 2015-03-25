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
      //This contains if the player two is controlled by an AI
      bool player_two_AI;
      //This contains the information whereas player one won or lost
      bool player_one_won;
      //This contains the information of the AI (1-3)
      int type_AI;
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
      // scene for drawing box
      ref<visual_scene> app_scene;
      //This contains the User Interface and the stage
      StageUI stage_puppet;
      //The two players
      Puppet player_one;
      Puppet player_two;
      //This is the module of the AI (implemented with nGram)
      PredictiveAI predictiveAI;

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
        player_two_AI = false;
        cheating = false;
        player_one_won = false;
        //Select the starting mode of the AI (mode 1)
        type_AI = 1;
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

      /// @brief This will be called when clicked the P V P button
      void button_p_vs_p(){
        player_two_AI = false;
      }

      /// @brief This will be called when clicked the P V AI button
      void button_p_vs_ai(){
        player_two_AI = true;
      }

      /// @brief This will be called when clicked the AI 1 button
      void button_ai_one(){
        type_AI = 1;
      }

      /// @brief This will be called when clicked the AI 2 button
      void button_ai_two(){
        type_AI = 2;
      }

      /// @brief This will be called when clicked the AI 3 button
      void button_ai_three(){
        type_AI = 3;
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
        if (time_lapse < 0.15f) time_lapse = 0.15f;
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
            if (rx <= 0.5f){
              if (rx < 0.19f){
                if (rx > 0.12f){
                  if (ry >= 0.8f && ry <= 0.85f)
                    reset_game();
                  else if (ry >= 0.88f && ry <= 0.94f)
                    button_speed(0.05f);
                }
              }
              else if (rx < 0.35f){
                if (rx > 0.236f && rx < 0.30f){
                  if (ry >= 0.8f && ry <= 0.85f)
                    button_p_vs_p();
                  else if (ry >= 0.88f && ry <= 0.94f)
                    button_speed(-0.05f);
                }
              }
              else if (rx < 0.42f){
                if (ry >= 0.8f && ry <= 0.85f)
                  button_p_vs_ai();
                else if (ry >= 0.88f && ry <= 0.94f)
                  button_cheating();
              }
            }
            else{
              if (rx > 0.81f){
                if (rx < 0.88f){
                  if (ry >= 0.8f && ry <= 0.85f)
                    button_ai_one();
                  else if (ry >= 0.88f && ry <= 0.94f)
                    predictiveAI.set_dimension(1);
                }
              }
              else if (rx > 0.65f){
                if (rx < 0.764f && rx > 0.7f){
                  if (ry >= 0.8f && ry <= 0.85f)
                    button_ai_two();
                  else if (ry >= 0.88f && ry <= 0.94f)
                    predictiveAI.set_dimension(4);
                }
              }
                else if (rx > 0.58f){
                  if (ry >= 0.8f && ry <= 0.85f)
                    button_ai_three();
                  else if (ry >= 0.88f && ry <= 0.94f)
                    predictiveAI.resetAI();
                }
            }
          }
        }
      }

      void keyboard(){
        //Player 1
        if (!player_one.is_finishing()){
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
        //Player 2
        if (!player_two.is_finishing() && !player_two_AI){
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

      void mid_frame(){
        if (_game_state == _PLAYING){
          //Predict actions
          int predicted_action = predictiveAI.predict();
          printf(" predicted action => %i vs action => %i\n", predicted_action, player_one.get_action());
          //Decide action
          if (player_two_AI)
            switch (type_AI){
            case 1:
              player_two.AI_reaction_mimic((actions)predicted_action, player_one);
              break;
            case 2:
              player_two.AI_reaction_defense((actions)predicted_action, player_one);
              break;
            case 3:
              player_two.AI_reaction_balanced((actions)predicted_action, player_one);
              break;
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

      void last_frame(){
        if (_game_state == _PLAYING){
          //Memorize actions
          if (player_one.get_action() != NONE_ACTION && player_one.get_action() < FINISHING)
            predictiveAI.new_input(player_one.get_action());
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
      }

      /// this is called to draw the world
      void draw_world(int x, int y, int w, int h) {
        int vx = 0, vy = 0;
        get_viewport_size(vx, vy);
        app_scene->begin_render(vx, vy);
        // obtain input from mouse and keyboard
        mouse();
        if (_game_state == _PLAYING)
          keyboard();
        else if (_game_state == _INTRO_ANIMATION){
          //Prepare to check frames
          std::chrono::time_point<std::chrono::system_clock> now_animation = std::chrono::system_clock::now();
          std::chrono::duration<float> elapsed_seconds = now_animation - cur_animation;
          float t = 1.0f * elapsed_seconds.count() / time_animation;
          if (t <= 1){
            player_one.animate_intro(t);
            player_two.animate_intro(t);
            stage_puppet.animate_intro(t);
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
