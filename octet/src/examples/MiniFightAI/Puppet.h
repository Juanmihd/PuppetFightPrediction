//
// This will be an abstract class containing the puppets!
//

#ifndef _PUPPET_FIGHT_PUPPET_H_INCLUDED_
#define _PUPPET_FIGHT_PUPPET_H_INCLUDED_

namespace octet{
  namespace PuppetFight{
    enum height{
      DOWN = 0, MID = 1, HIGH = 2
    };

    enum actions{
      NONE_ACTION, MOVE_LEFT, MOVE_RIGHT, PUNCH_UP, PUNCH_DOWN, PUNCH_MID, BLOCK_UP, BLOCK_MID, BLOCK_DOWN
    };

    class Puppet{
      //Info of how to visualize
      ref<scene_node> node;

      ref<visual_scene> game_scene;
      //Info of where it is
      int direction;
      int position;
      actions next_action;

      //movement actions
      void move_left(){
        node->translate(vec3(-1.0f, 0.0f, 0.0f));
        position -= 1;
      }

      void move_right(){
        node->translate(vec3(1.0f, 0.0f, 0.0f));
        position += 1;
      }

      //Attack functions
      void punch(height level){
      }

      void block(height level){
      }
    public:
      Puppet() : next_action(NONE_ACTION) {}

      void init(ref<visual_scene> n_game_scene, int n_direction = 1, material* n_material = nullptr){
        game_scene = n_game_scene;
        direction = n_direction;

        material* puppet_mat = (n_material == nullptr) ? (direction > 0 ? new material(vec4(1, 0, 0, 1)) : new material(vec4(0.5f, 0, 0.5f, 1))) : n_material;
        mesh_box* puppet_box = new mesh_box(vec3(2, 4, 1));
        node = new scene_node();
        
        node->translate(vec3(direction*-5.0f, 0, 0));
        position = -5 * direction;

        game_scene->add_child(node);
        game_scene->add_mesh_instance(new mesh_instance(node, puppet_box, puppet_mat));
      }

      void input_action(actions n_action){
        next_action = n_action;
      }

      void execute_action(Puppet & enemy){
        switch (next_action){
        case MOVE_LEFT:
          move_left();
          break;
        case MOVE_RIGHT:
          move_right();
          break;
        }

        next_action = NONE_ACTION;
      }

      int get_position(){
        return position + (next_action == MOVE_RIGHT ? 1 : next_action == MOVE_LEFT? -1 : 0);
      }

      bool collision_with(Puppet & enemy){
        int distance = position - enemy.get_position();
        printf("Distance = %i\n", distance);
        if (distance < 0) distance *= -1.0f;
        return distance < 5;
      }
    };
  }
}


#endif