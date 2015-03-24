//
// This class will contain the user interface to be drawn
//
// It will be a set of openGL boxes, representing the stage of the puppet show

#ifndef _PUPPET_FIGHT_UI_H_INCLUDED_
#define _PUPPET_FIGHT_UI_H_INCLUDED_

namespace octet {
  namespace PuppetFight{
    class StageUI{
      ref<scene_node> bottom;
      ref<scene_node> background;
      ref<scene_node> top;
      ref<scene_node> life_player_one;
      ref<scene_node> life_player_two;
      ref<material> red;
      ref<material> purple;

      ref<visual_scene> game_scene;
    public:
      StageUI(){}

      void init(ref<visual_scene> n_game_scene){
        game_scene = n_game_scene;

        image* topimage = new image("assets/AI/top.gif");
        image* topmask = new image("assets/AI/topmask.gif");
        material* top_mat = new material(vec4(1, 1, 1, 1), new param_shader("shaders/default.vs", "shaders/multitexture.fs"));
        top_mat->add_sampler(0, app_utils::get_atom("top"), topimage, new sampler());
        top_mat->add_sampler(1, app_utils::get_atom("topmask"), topmask, new sampler());
        material* bottom_mat = new material(new image("assets/AI/bottom.gif"));
        material* background_mat = new material(new image("assets/AI/background_3.gif"));
        material* black = new material(vec4(0, 0, 0, 1));
        red = new material(vec4(1, 0, 0, 1));
        purple = new material(vec4(.5f, 0, .5f, 1));

        mesh_box* bottom_box = new mesh_box(vec3(22, 8, 0.02));
        mesh_box* top_box = new mesh_box(vec3(22, 8.5f, 0.05));
        mesh_box* background_box = new mesh_box(vec3(20, 8.5, 1));
        mesh_box* life_one_box = new mesh_box(vec3(9, 1, 0.1));
        mesh_box* life_two_box = new mesh_box(vec3(9, 1, 0.1));
        mesh_box* life_one_box_back = new mesh_box(vec3(9.15f, 1.1f, 0.1f));
        mesh_box* life_two_box_back = new mesh_box(vec3(9.15f, 1.1f, 0.1f));

        bottom = new scene_node();
        bottom->translate(vec3(0,-10,1));
        background = new scene_node();
        background->translate(vec3(0, 6, -1));
        top = new scene_node();
        top->translate(vec3(0, 6, 0));
        life_player_one = new scene_node();
        life_player_one->translate(vec3(-11, -6, 1.2f));
        life_player_two = new scene_node();
        life_player_two->translate(vec3(11, -6, 1.2f));
        scene_node* life_one_node_back = new scene_node();
        life_one_node_back->translate(vec3(-11.1f, -6, 1));
        scene_node* life_two_node_back = new scene_node();
        life_two_node_back->translate(vec3(11.1f, -6, 1));
        
        game_scene->add_child(bottom);
        game_scene->add_child(background);
        game_scene->add_child(top);
        game_scene->add_child(life_player_one);
        game_scene->add_child(life_player_two);
        game_scene->add_child(life_one_node_back);
        game_scene->add_child(life_two_node_back);

        game_scene->add_mesh_instance(new mesh_instance(bottom, bottom_box, bottom_mat));
        game_scene->add_mesh_instance(new mesh_instance(background, background_box, background_mat));
        game_scene->add_mesh_instance(new mesh_instance(top, top_box, top_mat));
        game_scene->add_mesh_instance(new mesh_instance(life_player_one, life_one_box, red));
        game_scene->add_mesh_instance(new mesh_instance(life_player_two, life_two_box, red));
        game_scene->add_mesh_instance(new mesh_instance(life_one_node_back, life_one_box_back, black));
        game_scene->add_mesh_instance(new mesh_instance(life_two_node_back, life_two_box_back, black));
      }

      void hurt_player_one(){
        life_player_one->scale(vec3(0.9, 1, 1));
        life_player_one->translate(vec3(-1, 0, 0));
      }

      void hurt_player_two(){
        life_player_two->scale(vec3(0.9, 1, 1));
        life_player_two->translate(vec3(1, 0, 0));
      }

      void display_lifes(int life1, int life2){

      }
    };
  }
}

#endif