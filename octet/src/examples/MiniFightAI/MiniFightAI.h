////////////////////////////////////////////////////////////////////////////////
//
// MiniFightAI is the name of the whole app. 
// This app will contain two players, the user interface.
// The player can be playable or AI controlled! 
// Player 1 has to be a player always!
//

#include "UI.h"
#include "Puppet.h"
#include "PredictiveAI.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

namespace octet {
  namespace PuppetFight{
    /// Scene containing a box with octet.
    class MiniFightAI : public app {
      StageUI stage_puppet;
      Puppet player_one;
      bool player_two_AI;
      int type_AI;
      Puppet player_two;

      // scene for drawing box
      ref<visual_scene> app_scene;
      ref<scene_node> light_node;
      int direction_light;
      int moving_light;
      std::chrono::time_point<std::chrono::system_clock> previous_action; 

      //---- THIS IS FOR AN UNIT TEST!!! ----
      PredictiveAI predictiveAI;

      float time_lapse;
      int turn;
    public:
      /// this is called when we construct the class before everything is initialised.
      MiniFightAI(int argc, char **argv) : app(argc, argv) {
      }

      /// this is called once OpenGL is initialized
      void app_init() {
        random_gen.set_seed(time(NULL));
        app_scene =  new visual_scene();

        //---- THIS IS FOR AN UNIT TEST!!! ----
        predictiveAI.init(4); 
        player_two_AI = false;
        type_AI = 1;

        light_node = new scene_node();
        app_scene->add_child(light_node);
        light *_light = new light();
        light_instance *li = new light_instance();
        light_node->access_nodeToParent().translate(100, 100, 100);
        light_node->access_nodeToParent().rotateX(25);
        light_node->access_nodeToParent().rotateY(45);
        _light->set_color(vec4(1, 1, 1, 1));
        _light->set_kind(atom_directional);
        li->set_node(light_node);
        li->set_light(_light);
        app_scene->add_light_instance(li);

        app_scene->create_default_camera_and_lights();

        direction_light = -1;
        moving_light = 1000;
        stage_puppet.init(app_scene);

        player_one.init(app_scene);
        player_two.init(app_scene,-1);
        time_lapse = 0.3f;
        previous_action = std::chrono::system_clock::now();
        turn = 0;
      }

      void reset_game(){
        printf("TESTING!\n");
        player_one.reset_puppet();
        player_two.reset_puppet();
      }

      void button_p_vs_p(){
        player_two_AI = false;
      }

      void button_p_vs_ai(){
        player_two_AI = true;
      }

      void button_ai_one(){
        type_AI = 1;
      }

      void button_ai_two(){
        type_AI = 2;
      }

      void button_ai_three(){
        type_AI = 3;
      }

      void mouse(){
        if (is_key_going_down(key_lmb)){
          int x = 0; 
          int y = 0;
          get_mouse_pos(x, y); 
          int vx = 0;
          int vy = 0;
          get_viewport_size(vx, vy);
          float rx = 1.0f*x / vx;
          float ry = 1.0f*y / vy;
          if (ry >= 0.8f && ry <= 0.85f){
            if (rx <= 0.5f){
              if (rx < 0.19f){
                if (rx > 0.12f)
                  reset_game();
              }
              else if (rx < 0.35f){
                if (rx > 0.236f && rx < 0.30f)
                  button_p_vs_p();
              }
              else if (rx < 0.42f)
                button_p_vs_ai();
            }
            else{
              if (rx > 0.81f){
                if (rx < 0.88f)
                  button_ai_one();
              }
              else if (rx > 0.65f){
                if (rx < 0.764f && rx > 0.7f)
                  button_ai_two();
              }
              else if (rx > 0.58f)
                button_ai_three();
            }
            //printf("Pos mouse: x= %f, y= %f\n", 1.0f*x / vx, 1.0f*y / vy);
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

      void moveLight(){
        --moving_light;
        light_node->rotate(direction_light, vec3(1, 0, 0));
        if (moving_light < 0){
          direction_light = -direction_light;
          moving_light = 1000;
        }
      }

      /// this is called to draw the world
      void draw_world(int x, int y, int w, int h) {
        int vx = 0, vy = 0;
        get_viewport_size(vx, vy);
        app_scene->begin_render(vx, vy);
        // obtain input from mouse and keyboard
        mouse();
        keyboard();
        //moveLight();

        // execute actions in players
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_seconds = now - previous_action;
        if (elapsed_seconds.count() > time_lapse){
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
          //Memorize actions
          if (player_one.get_action() != NONE_ACTION && player_one.get_action() < FINISHING)
            predictiveAI.new_input(player_one.get_action());
          //Resolve actions
          if(player_one.execute_action(player_two))
            stage_puppet.hurt_player_one();
          if(player_two.execute_action(player_one))
            stage_puppet.hurt_player_two();
          //Reset timer
          previous_action = now;
        }

        // update matrices. assume 30 fps.
        app_scene->update(1.0f/30);

        // draw the scene
        app_scene->render((float)vx / vy);

      }
    };
  }
}
