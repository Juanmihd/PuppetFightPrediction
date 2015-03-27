//
// This will be an abstract class containing the puppets!
//

#ifndef _PUPPET_FIGHT_PUPPET_H_INCLUDED_
#define _PUPPET_FIGHT_PUPPET_H_INCLUDED_

namespace octet{
  namespace PuppetFight{
    random random_gen;
    enum height{
      HEIGHT_UP = 0, HEIGHT_MID = 1, HEIGHT_DOWN = 2
    };

    enum actions{
      NONE_ACTION = 0, MOVE_LEFT = 1, MOVE_RIGHT = 2, PUNCH_UP = 3, PUNCH_DOWN = 4, PUNCH_MID = 5, BLOCK_UP = 6, BLOCK_MID = 7, BLOCK_DOWN = 8, FINISHING = 9,
      OUCH_UP = 10, OUCH_MID = 11, OUCH_DOWN = 12, DEAD = 13
    };

    class Puppet{
      //Info of how to visualize
      ref<scene_node> node;
      dynarray<ref<material>> materials;
      ref<mesh_instance> puppet_instance;
      ref<visual_scene> game_scene;
      //Info of where it is
      int direction;
      int position;
      int life;
      int energy;
      int life_damage;
      actions prev_action;
      actions next_action;
      actions next_next_action;
      
      //Attack functions
      bool punch(height level, Puppet& enemy){
        energy -= 15;
        if (energy < 0) energy = 0;
        puppet_instance->set_material(materials[3 + (int)level]);
        if (collision_with(enemy)){
          return (!enemy.is_blocking(level));
        }
        return false;
      }

      void ouch(height level, Puppet& enemy){
        life -= life_damage*enemy.get_energy()*0.01f;
        puppet_instance->set_material(materials[10 + ((level == HEIGHT_UP) ? 0 : 1)]);
      }

      void block(height level){
        puppet_instance->set_material(materials[6 + (int)level]);
      }

      //Still!
      void still(){
        if (life>0)
          puppet_instance->set_material(materials[0]);
        else
          puppet_instance->set_material(materials[11]);
      }

      void init_materials(){
        dynarray<ref<image>> images;

        if (direction == 1){
          images.push_back(new image("assets/AI/PuppetRest01.gif"));
          images.push_back(new image("assets/AI/PuppetRest01mask.gif"));
          images.push_back(new image("assets/AI/PuppetRunLeft01.gif"));
          images.push_back(new image("assets/AI/PuppetRunLeft01Mask.gif"));
          images.push_back(new image("assets/AI/PuppetRunRight01.gif"));
          images.push_back(new image("assets/AI/PuppetRunRight01Mask.gif"));

          images.push_back(new image("assets/AI/PuppetPunchUp01.gif"));
          images.push_back(new image("assets/AI/PuppetPunchUp01mask.gif"));
          images.push_back(new image("assets/AI/PuppetPunchMid01.gif"));
          images.push_back(new image("assets/AI/PuppetPunchMid01Mask.gif"));
          images.push_back(new image("assets/AI/PuppetPunchLow01.gif"));
          images.push_back(new image("assets/AI/PuppetPunchLow01Mask.gif"));

          images.push_back(new image("assets/AI/PuppetBlockUp01.gif"));
          images.push_back(new image("assets/AI/PuppetBlockUp01mask.gif"));
          images.push_back(new image("assets/AI/PuppetBlockMid01.gif"));
          images.push_back(new image("assets/AI/PuppetBlockMid01Mask.gif"));
          images.push_back(new image("assets/AI/PuppetBlockLow01.gif"));
          images.push_back(new image("assets/AI/PuppetBlockLow01Mask.gif"));

          images.push_back(new image("assets/AI/PuppetOuchUp01.gif"));
          images.push_back(new image("assets/AI/PuppetOuchUp01Mask.gif"));
          images.push_back(new image("assets/AI/PuppetOuchMid01.gif"));
          images.push_back(new image("assets/AI/PuppetOuchMid01Mask.gif"));

          images.push_back(new image("assets/AI/PuppetLost.gif"));
        }
        else{
          images.push_back(new image("assets/AI/Puppet2Rest01.gif"));
          images.push_back(new image("assets/AI/Puppet2Rest01mask.gif"));
          images.push_back(new image("assets/AI/Puppet2RunLeft01.gif"));
          images.push_back(new image("assets/AI/Puppet2RunLeft01Mask.gif"));
          images.push_back(new image("assets/AI/Puppet2RunRight01.gif"));
          images.push_back(new image("assets/AI/Puppet2RunRight01Mask.gif"));

          images.push_back(new image("assets/AI/Puppet2PunchUp01.gif"));
          images.push_back(new image("assets/AI/Puppet2PunchUp01mask.gif"));
          images.push_back(new image("assets/AI/Puppet2PunchMid01.gif"));
          images.push_back(new image("assets/AI/Puppet2PunchMid01Mask.gif"));
          images.push_back(new image("assets/AI/Puppet2PunchLow01.gif"));
          images.push_back(new image("assets/AI/Puppet2PunchLow01Mask.gif"));

          images.push_back(new image("assets/AI/Puppet2BlockUp01.gif"));
          images.push_back(new image("assets/AI/Puppet2BlockUp01mask.gif"));
          images.push_back(new image("assets/AI/Puppet2BlockMid01.gif"));
          images.push_back(new image("assets/AI/Puppet2BlockMid01Mask.gif"));
          images.push_back(new image("assets/AI/Puppet2BlockLow01.gif"));
          images.push_back(new image("assets/AI/Puppet2BlockLow01Mask.gif"));

          images.push_back(new image("assets/AI/Puppet2OuchUp01.gif"));
          images.push_back(new image("assets/AI/Puppet2OuchUp01Mask.gif"));
          images.push_back(new image("assets/AI/Puppet2OuchMid01.gif"));
          images.push_back(new image("assets/AI/Puppet2OuchMid01Mask.gif"));

          images.push_back(new image("assets/AI/Puppet2Lost.gif"));
        }
        param_shader* param = new param_shader("shaders/default.vs", "shaders/multitexture.fs");
        for (int i = 0; i < 11; ++i){
          material* new_mat = new material(vec4(1, 1, 1, 1), param);
          new_mat->add_sampler(0, app_utils::get_atom("top"), images[2*i], new sampler());
          new_mat->add_sampler(1, app_utils::get_atom("topmask"), images[2*i+1], new sampler());
          materials.push_back(new_mat);
        }
        material* new_mat = new material(vec4(1, 1, 1, 1), param);
        new_mat->add_sampler(0, app_utils::get_atom("top"), images[22], new sampler());
        new_mat->add_sampler(1, app_utils::get_atom("topmask"), images[1], new sampler());
        materials.push_back(new_mat);
      }

    public:
      Puppet() : next_action(NONE_ACTION), next_next_action(NONE_ACTION) {}

      void init(ref<visual_scene> n_game_scene, int n_direction = 1, material* n_material = nullptr){
        game_scene = n_game_scene;
        direction = n_direction; 

        init_materials();

        mesh_box* puppet_box = new mesh_box(vec3(2, 4, 200));
        node = new scene_node();
        reset_puppet();

        //node->translate(vec3(direction*-5.0f, 2, -199.9f));
        node->translate(vec3(direction*-5.0f, -8, -199.9f));
        position = -5 * direction;

        game_scene->add_child(node);
        puppet_instance = new mesh_instance(node, puppet_box, materials[0]);
        game_scene->add_mesh_instance(puppet_instance);

      }

      //movement actions      
      void move_left(){
        node->translate(vec3(-1.0f, 0.0f, 0.0f));
        puppet_instance->set_material(materials[1]);
        position -= 1;
      }

      void move_right(){
        node->translate(vec3(1.0f, 0.0f, 0.0f));
        puppet_instance->set_material(materials[2]);
        position += 1;
      }

      void set_life_damage(int n){
        life_damage = n;
      }

      void animate_intro(float t){
        node->access_nodeToParent().loadIdentity();
        node->translate(vec3(direction*-(6.f * (1 - t) + 5.0f*t), -8.f * (1 - t) + 1.99f*t, -199.9f));
        node->rotate(direction * (-45) * (1 - t), vec3(0, 0, 1));
      }

      void reset_puppet(){
        life = 100;
        energy = 50;
        life_damage = 10;
        node->access_nodeToParent().loadIdentity();
        node->translate(vec3(direction*-5.0f, 1.99f, -199.9f));
        position = -5 * direction;
      }

      int get_life(){
        return life;
      }

      int get_energy(){
        return energy;
      }
      
      void input_action(actions n_action){
        next_action = n_action;
      }

      actions random_action(){
        if (next_action >= FINISHING)
          return next_action;
        else
          return static_cast<actions> (random_gen.get(0, 8));
      }

      void AI_reaction_balanced(actions n_action, Puppet& player, int distance_treshold = 4){
        int random = random_gen.get(0, 100);

        int distance = math::abs(position - player.get_position());
        
        if (distance >= distance_treshold){
          if (direction == 1)
            next_action = MOVE_RIGHT;
          else
            next_action = MOVE_LEFT;
        }
        else{
          if (next_action < FINISHING)
            switch (n_action){
            case MOVE_LEFT:
              if (direction == 1){
                if (energy == 100)
                  if (!collision_with(player))
                    next_action = MOVE_RIGHT;
                  else
                    next_action = PUNCH_MID;
                else
                if (random* energy*0.01 < 50)
                  if (!collision_with(player))
                    next_action = MOVE_RIGHT;
                  else
                    next_action = PUNCH_MID;
                else
                  next_action = PUNCH_MID;
              }
              else if (random * energy*0.01 < 50)
                if (!collision_with(player))
                  next_action = MOVE_LEFT;
                else
                  next_action = PUNCH_MID;
              else
                if (position <= 14)
                  next_action = MOVE_RIGHT;
              break;
            case MOVE_RIGHT:
              if (direction == 1){
                if (random * energy*0.01 < 50)
                  if (!collision_with(player))
                    next_action = MOVE_RIGHT;
                  else
                    next_action = PUNCH_MID;
                else
                  if (position >= -14)
                    next_action = MOVE_LEFT;
                break;
              }
              else
                if (energy == 100)
                  if (!collision_with(player))
                    next_action = MOVE_LEFT;
                  else
                    next_action = PUNCH_MID;
              else
                if(random * energy*0.01 < 50)
                if (!collision_with(player))
                  next_action = MOVE_LEFT;
                else
                  next_action = PUNCH_MID;
              else
                next_action = PUNCH_MID;
              break;
            case PUNCH_UP:
              if (energy >= 90)
                next_action = PUNCH_MID;
              else if (energy <= 20)
                next_action = BLOCK_UP;
              else if (random * life*0.01f*energy*0.01 < 50)
                next_action = BLOCK_UP;
              else
                next_action = PUNCH_MID;
              break;
            case PUNCH_MID:
              if (energy >= 90)
                next_action = PUNCH_MID;
              else if (energy <= 20)
                next_action = BLOCK_MID;
              else if (random * life*0.01f*energy*0.01 < 50)
                next_action = BLOCK_MID;
              else
                next_action = PUNCH_DOWN;
              break;
            case PUNCH_DOWN:
              if (energy >= 90)
                next_action = PUNCH_MID;
              else if (energy <= 20)
                next_action = BLOCK_DOWN;
              else if (random * life*0.01f*energy*0.01 < 50)
                next_action = BLOCK_DOWN;
              else
                next_action = PUNCH_MID;
              break;
            case BLOCK_UP:
              if (energy <= 20){
                if (direction){
                  if (position >= -14)
                    next_action = MOVE_LEFT;
                  else next_action = BLOCK_UP;
                }
                else{
                  if (position <= 14)
                    next_action = MOVE_RIGHT;
                  else next_action = BLOCK_UP;
                }
              }
              else if (energy >= 90)
                next_action = PUNCH_DOWN;
              else next_action = PUNCH_DOWN;
              break;
            case BLOCK_MID:
              if (energy <= 20){
                if (direction == 1){
                  if (position >= -14)
                    next_action = MOVE_LEFT;
                  else next_action = BLOCK_UP;
                }
                else{
                  if (position <= 14)
                    next_action = MOVE_RIGHT;
                  else next_action = BLOCK_UP;
                }
              }
              else if (energy >= 90)
                next_action = PUNCH_UP;
              else next_action = PUNCH_UP;
              break;
            case BLOCK_DOWN:
              if (energy <= 20){
                if (direction==1){
                  if (position >= -14)
                    next_action = MOVE_LEFT;
                  else next_action = BLOCK_UP;
                }
                else{
                  if (position <= 14)
                    next_action = MOVE_RIGHT;
                  else next_action = BLOCK_UP;
                }
              }
              else next_action = PUNCH_MID;
              break;
          }
          
        }
      }

      void AI_reaction_defense(actions n_action, Puppet& player){
        int random = random_gen.get(0, 10);
        if (next_action < FINISHING)
          switch (n_action){
          case MOVE_LEFT:
            if (direction == 1){
              if (position >= -14 && random < 2)
                next_action = MOVE_LEFT;
              else
                next_action = BLOCK_MID;
            }else if (position <= 14 && random < 2)
              next_action = MOVE_RIGHT;
            else
              next_action = BLOCK_MID;
            break;
          case MOVE_RIGHT:
            if (direction == 1){
              if (position >= -14 && random < 2)
                next_action = MOVE_LEFT;
              else
                next_action = BLOCK_MID;
            }else if (position <= 14 && random < 2)
              next_action = MOVE_RIGHT;
            else
              next_action = BLOCK_MID;
            break;
          case PUNCH_UP:
            next_action = BLOCK_UP;
            break;
          case PUNCH_MID:
            next_action = BLOCK_MID;
            break;
          case PUNCH_DOWN:
            next_action = BLOCK_DOWN;
            break;
          case BLOCK_UP:
            next_action = BLOCK_UP;
            break;
          case BLOCK_MID:
            next_action = BLOCK_MID;
            break;
          case BLOCK_DOWN:
            next_action = BLOCK_DOWN;
            break;
        }
      }

      void AI_reaction_mimic(actions n_action, Puppet& player){
        if (next_action < FINISHING)
          switch (n_action){
          case MOVE_LEFT:
            if (direction == 1){
              if (!collision_with(player))
                next_action = MOVE_RIGHT;
              else
                next_action = PUNCH_MID;
            }else if (position <= 14)
              next_action = MOVE_RIGHT;
            else
              next_action = BLOCK_MID;
            break;
          case MOVE_RIGHT:
            if (direction == 1){
              if (position >= -14)
                next_action = MOVE_LEFT;
              else
                next_action = BLOCK_MID;
            }else if (!collision_with(player))
              next_action = MOVE_LEFT;
            else
              next_action = PUNCH_MID;
            break;
          case PUNCH_UP:
            next_action = PUNCH_UP;
            break;
          case PUNCH_MID:
            next_action = PUNCH_MID;
            break;
          case PUNCH_DOWN:
            next_action = PUNCH_DOWN;
            break;
          case BLOCK_UP:
            next_action = BLOCK_UP;
            break;
          case BLOCK_MID:
            next_action = BLOCK_MID;
            break;
          case BLOCK_DOWN:
            next_action = BLOCK_DOWN;
            break;
        }
      }

      bool execute_action(Puppet& enemy){
        bool hurted = false;

        energy += 1;
        if (energy > 100) energy = 100;

        prev_action = next_action;

        if (life <= 0) next_action = DEAD;

        switch (next_action){
        case MOVE_LEFT:
          move_left();
          next_action = NONE_ACTION;
          break;
        case MOVE_RIGHT:
          move_right();
          next_action = NONE_ACTION;
          break;
        case PUNCH_UP:
          punch(HEIGHT_UP, enemy);
          next_action = FINISHING;
          break;
        case PUNCH_MID:
          punch(HEIGHT_MID, enemy);
          next_action = FINISHING;
          break;
        case PUNCH_DOWN:
          punch(HEIGHT_DOWN, enemy);
          next_action = FINISHING;
          break;
        case BLOCK_UP:
          block(HEIGHT_UP);
          next_action = NONE_ACTION;
          break;
        case BLOCK_MID:
          block(HEIGHT_MID);
          next_action = NONE_ACTION;
          break;
        case BLOCK_DOWN:
          block(HEIGHT_DOWN);
          next_action = NONE_ACTION;
          break;
        case OUCH_UP:
          ouch(HEIGHT_UP, enemy);
          hurted = true;
          next_action = NONE_ACTION;
          break;
        case OUCH_MID:
        case OUCH_DOWN:
          ouch(HEIGHT_MID, enemy);
          hurted = true;
          next_action = NONE_ACTION;
          break;
        case FINISHING:
        default:
          still();
          next_action = NONE_ACTION;
          break;
        }

        if (direction != 1)
          if (next_next_action != NONE_ACTION){
            next_action = next_next_action;
            next_next_action = NONE_ACTION;
          }

        return hurted;
      }

      int get_position(){
        return position + (next_action == MOVE_RIGHT ? 1 : next_action == MOVE_LEFT? -1 : 0);
      }

      bool collision_with(Puppet& enemy){
        int distance = position - enemy.get_position();
        if (distance < 0) distance *= -1;
        return distance < 4;
      }

      bool is_blocking(height attack_height){
        bool blocked = false;
        if (direction == 1)
          blocked = (prev_action == (actions)(BLOCK_UP + attack_height));
        else
          blocked = (next_action == (actions)(BLOCK_UP + attack_height));
        if (!blocked){
          if (direction == 1)
            next_action = OUCH_UP;
          else
            next_next_action = OUCH_UP;
        }
        return blocked;
      }

      bool is_finishing(){
        return next_action >= FINISHING;
      }

      actions get_action(){
        return next_action;
      }

    };
  }
}


#endif