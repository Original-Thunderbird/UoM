from Client import *
from Utils import *
import sys
from treelib import *
from MinMax import *

# print debug information during game play to text file
sys.stderr = open('./problem-on-the-fly.txt', 'w')

SearchDepth = 3
# gameTree = Tree()
round = 0
my_move_list = []

sys.stderr = open('err.txt', 'w')

while(True):
    game_state = handleInput(input())
    if game_state:  # when 'END\n' receive, game_state is None
        if Global_state.is_my_turn == True:
            round += 1
            if len(my_move_list) > 0:
                my_move = my_move_list[0]
            else:
                my_move_list = MiniMax(SearchDepth)
                my_move = my_move_list[0]
            my_move_list = my_move_list[1:]
            print_debug_info(f'At round - {round}. Opponent move - {my_move}\n\n')
            print(makeOutput(my_move), end="")
            Global_state.is_my_turn = False
        else:
            # print_debug_info(f'At round - {round}. Not my move\n\n')
            pass
    else:
        # reset_game()
        break