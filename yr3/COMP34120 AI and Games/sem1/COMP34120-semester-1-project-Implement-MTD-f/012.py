from Client import *
from Utils import *
import sys
from treelib import *
from MinMax_h import *
SearchDepth = 7
# gameTree = Tree()
Global_state.round = 0

my_move_list = []

sys.stderr = open('err.txt', 'w')

while(True):
    game_state = handleInput(input())
    if game_state:  # when 'END\n' receive, game_state is None
        if Global_state.is_my_turn == True:
            Global_state.round += 1
            if Global_state.is_last_move_mine == False:
                Global_state.opp_last_move.append(int(Global_state.get_opp_moves()[-1]) - 1)

            # w0 = 1 && w1 = 1 && w2 = 1 && w4 = 1; w0 = 1 && w1 = 1 && w2 = 1; w0 = 1; w0 = 1 && w1 = 1
            weight_vector = [Global_state.round, Global_state.round, Global_state.round, 0, 0] 
            # simple way to eliminate the advantage of firsthand
            if Global_state.round == 1 and Global_state.get_side() == "South":
                my_move = 2
            else:
                if len(my_move_list) > 0:
                    my_move = my_move_list[0]
                else:
                    my_move_list = MiniMax(SearchDepth, Global_state.round, weight_vector)
                    my_move = my_move_list[0]
                my_move_list = my_move_list[1:]
            print(makeOutput(my_move), end="")
            # print_debug_info(f"At Global_state.round {Global_state.round}, Opp's last move is {Global_state.get_opp_last_move()}\n\n")
            Global_state.opp_last_move = []
            Global_state.is_last_move_mine = True # set to True everytime after my turn
            Global_state.set_is_my_turn(False)
        else:
            if Global_state.is_last_move_mine == True:
                Global_state.is_last_move_mine = False
            else:
                # opp's bonus move
                Global_state.round += 1
                Global_state.opp_last_move.append(int(Global_state.get_opp_moves()[-1]) - 1)
    else:
        # reset_game()
        break
