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
      ref<scene_node> bottom_cover;
      ref<scene_node> background;
      ref<scene_node> top;
      ref<scene_node> courtin;
      ref<scene_node> life_player_one;
      ref<scene_node> life_player_two;
      ref<scene_node> energy_player_one;
      ref<scene_node> energy_player_two;
      dynarray<ref<scene_node>> button_on;
      ref<material> red;
      ref<material> purple;
      ref<mesh_instance> background_mesh;
      ref<mesh_instance> mesh_player_one;
      ref<mesh_instance> mesh_player_two;
      ref<visual_scene> game_scene;
      dynarray<ref<material>> background_materials;

      bool started;
      float posx[3];
      float posy[3];
    public:
      StageUI(){}

      void init(ref<visual_scene> n_game_scene){
        posx[0] = 5.99f;
        posy[0] = -7.12f;
        posx[1] = 11.9f;
        posy[1] = -9.32f;
        posx[2] = 17.9f;
        posy[2] = -11.48f;
        game_scene = n_game_scene;
        started = false;

        image* topimage = new image("assets/AI/top.gif");
        image* topmask = new image("assets/AI/topmask.gif");
        image* courtinimage = new image("assets/AI/courtin.gif");
        image* courtinmask = new image("assets/AI/courtinmask.gif");
        image* buttonimage = new image("assets/AI/button.gif");
        image* buttonmask = new image("assets/AI/buttonmask.gif");
        material* top_mat = new material(vec4(1, 1, 1, 1), new param_shader("shaders/default.vs", "shaders/multitexture.fs"));
        material* courtin_mat = new material(vec4(1, 1, 1, 1), new param_shader("shaders/default.vs", "shaders/multitexture.fs"));
        material* button_mat = new material(vec4(1, 1, 1, 1), new param_shader("shaders/default.vs", "shaders/multitexture.fs"));
        top_mat->add_sampler(0, app_utils::get_atom("top"), topimage, new sampler());
        top_mat->add_sampler(1, app_utils::get_atom("topmask"), topmask, new sampler());
        courtin_mat->add_sampler(0, app_utils::get_atom("top"), courtinimage, new sampler());
        courtin_mat->add_sampler(1, app_utils::get_atom("topmask"), courtinmask, new sampler());
        button_mat->add_sampler(0, app_utils::get_atom("top"), buttonimage, new sampler());
        button_mat->add_sampler(1, app_utils::get_atom("topmask"), buttonmask, new sampler());
        material* bottom_mat = new material(new image("assets/AI/bottom.gif"));
        material* bottom_cover_mat = new material(new image("assets/AI/bottom_cover.gif"));

        background_materials.push_back(new material(new image("assets/AI/background.gif")));
        background_materials.push_back(new material(new image("assets/AI/background_2.gif")));
        background_materials.push_back(new material(new image("assets/AI/background_3.gif")));
        background_materials.push_back(new material(new image("assets/AI/background_4.gif")));
        background_materials.push_back(new material(new image("assets/AI/background_5.gif")));
        background_materials.push_back(new material(new image("assets/AI/background_6.gif")));

        material* black = new material(vec4(0.0f, 0.0f, 0.0f, 1.0f));
        material* blue = new material(vec4(0.0f, 0.0f, 1.0f, 1.0f));
        red = new material(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        purple = new material(vec4(.5f, 0.0f, .5f, 1.0f));

        mesh_box* bottom_box = new mesh_box(vec3(22.0f, 8.0f, 0.02f));
        mesh_box* bottom_cover_box = new mesh_box(vec3(22.0f, 8.0f, 0.02f));
        mesh_box* top_box = new mesh_box(vec3(22.0f, 8.5f, 0.05f));
        mesh_box* courtin_box = new mesh_box(vec3(20.0f, 8.5f, 0.03f));
        mesh_box* background_box = new mesh_box(vec3(20.0f, 8.5f, 1.0f));
        mesh_box* button_box = new mesh_box(vec3(2.1f, 0.95f, 0.001f));
        mesh_box* button_box_small = new mesh_box(vec3(0.95f, 0.95f, 0.001f));
        mesh_box* life_one_box = new mesh_box(vec3(8.0f, 0.5f, 0.1f));
        mesh_box* life_two_box = new mesh_box(vec3(8.0f, 0.5f, 0.1f));
        mesh_box* life_one_box_back = new mesh_box(vec3(8.15f, 0.6f, 0.1f));
        mesh_box* life_two_box_back = new mesh_box(vec3(8.15f, 0.6f, 0.1f));
        mesh_box* energy_one_box = new mesh_box(vec3(3.0f, 0.15f, 0.1f));
        mesh_box* energy_two_box = new mesh_box(vec3(3.0f, 0.15f, 0.1f));
        mesh_box* energy_one_box_back = new mesh_box(vec3(3.05f, 0.25f, 0.1f));
        mesh_box* energy_two_box_back = new mesh_box(vec3(3.05f, 0.25f, 0.1f));

        bottom = new scene_node();
        bottom->translate(vec3(0.0f, -10.0f, 1.0f));
        bottom_cover = new scene_node();
        bottom_cover->translate(vec3(0.0f, -10.0f, 2.0f));
        background = new scene_node();
        background->translate(vec3(0.0f, 6.0f, -1.0f));
        top = new scene_node();
        top->translate(vec3(0.0f, 6.0f, 0.0f));
        courtin = new scene_node();
        courtin->translate(vec3(0.0f, 6.0f, 0.0f));
        scene_node* button = nullptr;
        for (int i = 0; i < 3; ++i){
          scene_node* button = new scene_node();
          button->translate(vec3(-posx[2], posy[i], 1.1f));
          button_on.push_back(button);
          button = new scene_node();
          button->translate(vec3(posx[0], posy[i], 1.1f));
          button_on.push_back(button);
        }
        button = new scene_node();
        button->translate(vec3(0.0f, posy[0]-1.3f, 0.1f));
        button_on.push_back(button);
        button = new scene_node();
        button->translate(vec3(0.9f, posy[2], 0.1f));
        button_on.push_back(button);
        button = new scene_node();
        button->translate(vec3(-0.9f, posy[2], 0.1f));
        button_on.push_back(button);
        button = new scene_node();
        button->translate(vec3(posx[0], posy[2], 0.1f));
        button_on.push_back(button);
        button = new scene_node();
        button->translate(vec3(-posx[0], posy[2], 0.1f));
        button_on.push_back(button);
        life_player_one = new scene_node();
        life_player_one->translate(vec3(-12.0f, -4.5f, 1.2f));
        life_player_two = new scene_node();
        life_player_two->translate(vec3(12.0f, -4.5f, 1.2f));
        scene_node* life_one_node_back = new scene_node();
        life_one_node_back->translate(vec3(-12.05f, -4.5f, 1.1f));
        scene_node* life_two_node_back = new scene_node();
        life_two_node_back->translate(vec3(12.05f, -4.5f, 1.1f));
        energy_player_one = new scene_node();
        energy_player_one->translate(vec3(-16.9f, -4.8f, 1.4f));
        energy_player_two = new scene_node();
        energy_player_two->translate(vec3(16.9f, -4.8f, 1.4f));
        scene_node* energy_one_node_back = new scene_node();
        energy_one_node_back->translate(vec3(-16.9f, -4.78f, 1.3f));
        scene_node* energy_two_node_back = new scene_node();
        energy_two_node_back->translate(vec3(16.9f, -4.78f, 1.3f));

        game_scene->add_child(courtin);
        game_scene->add_child(bottom_cover);
        game_scene->add_child(bottom);
        game_scene->add_child(background);
        game_scene->add_child(top);
        for (int i = 0; i < 6; ++i){
          game_scene->add_child(button_on[i]);
        }
        game_scene->add_child(life_player_one);
        game_scene->add_child(life_player_two);
        game_scene->add_child(life_one_node_back);
        game_scene->add_child(life_two_node_back);

        game_scene->add_mesh_instance(new mesh_instance(bottom_cover, bottom_cover_box, bottom_cover_mat));
        game_scene->add_mesh_instance(new mesh_instance(courtin, courtin_box, courtin_mat));
        game_scene->add_mesh_instance(new mesh_instance(bottom, bottom_box, bottom_mat));
        int random_background = random_gen.get(0, 5);
        background_mesh = new mesh_instance(background, background_box, background_materials[random_background]);
        game_scene->add_mesh_instance(background_mesh);
        game_scene->add_mesh_instance(new mesh_instance(top, top_box, top_mat));
        mesh_player_one = new mesh_instance(life_player_one, life_one_box, red);
        game_scene->add_mesh_instance(mesh_player_one);
        mesh_player_two = new mesh_instance(life_player_two, life_two_box, red);
        game_scene->add_mesh_instance(mesh_player_two);
        game_scene->add_mesh_instance(new mesh_instance(life_one_node_back, life_one_box_back, black));
        game_scene->add_mesh_instance(new mesh_instance(life_two_node_back, life_two_box_back, black));
        game_scene->add_mesh_instance(new mesh_instance(energy_player_one, energy_one_box, blue));
        game_scene->add_mesh_instance(new mesh_instance(energy_player_two, energy_two_box, blue));
        game_scene->add_mesh_instance(new mesh_instance(energy_one_node_back, energy_one_box_back, black));
        game_scene->add_mesh_instance(new mesh_instance(energy_two_node_back, energy_two_box_back, black));
        for (int i = 0; i < 6; ++i){
          game_scene->add_mesh_instance(new mesh_instance(button_on[i], button_box, button_mat));
        }
        game_scene->add_mesh_instance(new mesh_instance(button_on[6], button_box, button_mat));
        game_scene->add_mesh_instance(new mesh_instance(button_on[7], button_box_small, button_mat));
        game_scene->add_mesh_instance(new mesh_instance(button_on[8], button_box_small, button_mat));
        game_scene->add_mesh_instance(new mesh_instance(button_on[9], button_box, button_mat));
        game_scene->add_mesh_instance(new mesh_instance(button_on[10], button_box, button_mat));
      }

      void animate_intro(float t){
        courtin->access_nodeToParent().loadIdentity();
        courtin->translate(vec3(0.0f, 6.0f * (1 - t) + 30.0f*t, 0.0f));

        if (!started){
          bottom_cover->access_nodeToParent().loadIdentity();
          bottom_cover->translate(vec3(0.0f, -10.0f* (1 - t) + -25.0f * t, 3.0f));
          if(t==1) started = true;
        }
      }

      void update_lifes(int life1, int life2){
        if (life1 <= 40)
          mesh_player_one->set_material(purple);
        else
          mesh_player_one->set_material(red);

        if (life2 <= 40)
          mesh_player_two->set_material(purple);
        else
          mesh_player_two->set_material(red);

        life_player_one->access_nodeToParent().loadIdentity();
        life_player_one->translate(vec3(-12.0f, -4.5f, 1.2f));
        life_player_one->translate(vec3(-8.0f, 0.0f, 0.0f));
        life_player_one->scale(vec3(life1 / 100.0f, 1.0f, 1.0f));
        life_player_one->translate(vec3(8.0f, 0.0f, 0.0f));

        life_player_two->access_nodeToParent().loadIdentity();
        life_player_two->translate(vec3(12.0f, -4.5f, 1.2f));
        life_player_two->translate(vec3(8.0f, 0.0f, 0.0f));
        life_player_two->scale(vec3(life2 / 100.0f, 1.0f, 1.0f));
        life_player_two->translate(vec3(-8.0f, 0.0f, 0.0f));
      }


      void update_energies(int energy1, int energy2){
        energy_player_one->access_nodeToParent().loadIdentity();
        energy_player_one->translate(vec3(-16.9f, -4.8f, 1.4f));
        energy_player_one->translate(vec3(-3.0f, 0.0f, 0.0f));
        energy_player_one->scale(vec3(energy1 / 100.0f, 1.0f, 1.0f));
        energy_player_one->translate(vec3(3.0f, 0.0f, 0.0f));

        energy_player_two->access_nodeToParent().loadIdentity();
        energy_player_two->translate(vec3(16.9f, -4.8f, 1.4f));
        energy_player_two->translate(vec3(3.0f, 0.0f, 0.0f));
        energy_player_two->scale(vec3(energy2 / 100.0f, 1.0f, 1.0f));
        energy_player_two->translate(vec3(-3.0f, 0.0f, 0.0f));
      }
      void reset_UI(){
        update_lifes(100, 100);

        int random_background = random_gen.get(0, 5);
        background_mesh->set_material(background_materials[random_background]);
      }

      void set_button_posx(int i, int x){
        button_on[i]->loadIdentity();
        int minus = i % 2 == 0 ? -1 : 1;
        button_on[i]->translate(vec3(minus*posx[x], posy[i/2], 1.1f));
      }

      void press_reset_button(bool on){
        button_on[6]->loadIdentity();
        button_on[6]->translate(vec3(0.0f, posy[0] - 1.3f, on ? 1.1f : 0.1f));
      }

      void press_plus_button(bool on){
        button_on[7]->loadIdentity();
        button_on[7]->translate(vec3(0.9f, posy[2], on? 1.1f : 0.1f));
      }

      void press_minus_button(bool on){
        button_on[8]->loadIdentity();
        button_on[8]->translate(vec3(-0.9f, posy[2], on ? 1.1f : 0.1f));
      }

      void press_forget1_button(bool on){
        button_on[9]->loadIdentity();
        button_on[9]->translate(vec3(-posx[0], posy[2], on ? 1.1f : 0.1f));
      }

      void press_forget2_button(bool on){
        button_on[10]->loadIdentity();
        button_on[10]->translate(vec3(posx[2], posy[2], on ? 1.1f : 0.1f));
      }
    };
  }
}

#endif