from Client import *
from Utils import *
import sys
from treelib import *
from MinMax import *

sys.stderr = open('err.txt', 'w')

SearchDepth = 3
round = 0
# may perform multiple moves in our turn in case the last stone fall in scoring well, to gather them in advance,
# each time return 1 move to the server, it will then know the last stone is in the scoring well an let us take the next move.
my_move_list = []

while(True):
    game_state = handleInput(input())
    if game_state:  # when 'END\n' received, game_state is None
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