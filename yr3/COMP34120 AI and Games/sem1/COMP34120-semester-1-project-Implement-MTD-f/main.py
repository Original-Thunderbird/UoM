from Client import *
from Utils import *
import sys

sys.stderr = open('err.txt', 'w')

round = 0
while(True):
    game_state = handleInput(input())
    print_debug_info(f"My side - {Global_state.get_side()}\n")
    print_debug_info(f'Current board - {Global_state.get_board()}\n')
    if game_state:  # when 'END\n' receive, game_state is None
        if Global_state.is_my_turn == True:
            round += 1
            my_move = cal_seedNumber_to_max_score()
            print_debug_info(f'At round - {round}. Opp move - {Global_state.get_opp_moves()[-1]}\n')
            print_debug_info(f'At round - {round}. My move - {my_move}\n')
            print(makeOutput(my_move), end="")
            print_debug_info(f'Now all_moves is {Global_state.get_all_moves()}\n\n')
            Global_state.is_my_turn = False
        else:
            print_debug_info(f'Now all_moves is {Global_state.get_all_moves()}\n\n')
            # print_debug_info(f'At round - {round}. Opp move - {Global_state.get_opp_moves()}\n\n')
    else:
        # reset_game()
        break
