# maintain global state tree

from Client import *
from Utils import *
import sys
from treelib import *
from MinMax_h import *

sys.stderr = open('err.txt', 'w')
SearchDepth = 7
gameTree = Tree()
my_move_list = []

Global_state.round = 0

while(True):
    game_state = handleInput(input())
    if game_state:  # when 'END\n' receive, game_state is None
        if Global_state.is_my_turn == True:
            if Global_state.is_last_move_mine == False:
                Global_state.opp_last_move.append(int(Global_state.get_opp_moves()[-1]) - 1)

            weight_vector = [Global_state.round, Global_state.round, 0, 0, 0]
            # simple way to eliminate the advantage of firsthand
            if Global_state.round == 0 and Global_state.get_side() == "South":
                my_move = 2
            else:
                if len(my_move_list) > 0:
                    my_move = my_move_list[0]
                else:
                    Global_state.round += 1
                    
                    gameTree, my_move_list = MiniMax(gameTree, SearchDepth, Global_state.round, weight_vector)
                    my_move = my_move_list[0]
                my_move_list.pop(0) # my_move_list = my_move_list[1:]
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
                # Global_state.round += 1
                Global_state.opp_last_move.append(int(Global_state.get_opp_moves()[-1]) - 1)
    else:
        # reset_game()
        break