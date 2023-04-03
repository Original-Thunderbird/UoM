from ast import literal_eval
from Global_state import *


def print_debug_info(debugStr):
    filePath = './log.txt'
    with open('./log.txt', 'a') as f:
        f.write(debugStr)
    return None


''' 
Simulate move seed [seedNumber] on side [side] and returns own score well seed number after this move
'''
def move_seed(side, seedNumber):
    if side == "South":
        _board = Global_state.get_board()[1].copy() # have to use .copy() to avoid same reference
        _board.extend(Global_state.get_board()[0][:7])
    elif side == "North":
        _board = Global_state.get_board()[0].copy() # have to use .copy() to avoid same reference
        _board.extend(Global_state.get_board()[1][:7])
    else:
        print_debug_info("Raise error from Utils.move_seed(), side is wrong\n")
        return None
    _round = 15
    # index starts at 0, but seedNumber starts at 1
    seeds = _board[seedNumber-1]
    if seeds == 0:
        # print_debug_info("Raise error from Utils.move_seed(), this well is empty")
        return None

    for i in range(1, seeds+1):
        _board[(seedNumber-1+i) % _round] += 1
        _board[seedNumber-1] -= 1

    # print_debug_info(f" --- simulated board result --- \n{_board}\n ")
    # print_debug_info(f"Move {seedNumber} gives score {_board[7]}\n")
    return _board[7]

# search depth = 1
def cal_seedNumber_to_max_score():
    _side = Global_state.get_side()
    my_seeds_score = 0
    scores = []
    for i in range(1, 8):
        tmp = move_seed(_side, i)
        if tmp is not None:
            scores.append(tmp)
        else: # current well is empty
            scores.append(-1000)
    # print_debug_info(f'Board from Utils.cal_seedNun - {Global_state.get_board()}\n')
    # print_debug_info(f'Scores of moving each seed - {scores}\n')
    return scores.index(max(scores))+1


'''parse_board() has been tested'''


def parse_board(board_string):
    '''
    board_string example - 
    8,7,7,7,7,7,7,0,7,0,8,8,8,8,8,1
    '''
    board_list = literal_eval(
        "[" + board_string + "]")  # convert string to list
    if len(board_list) != 16:
        print_debug_info(f"Invalid board string {board_string}")
        return None
    # north_seeds = board_list[:8]
    # south_seeds = board_list[8:]
    # with open('./log.txt', 'a') as f:
    #     f.write(f'parsing board {board_list[:8]} --- {board_list[8:]}\n')
    return [board_list[:8], board_list[8:]]


def reset_game():
    Global_state.board = [[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0]]
    Global_state.side = ""
    Global_state.is_my_turn = False
    Global_state.my_moves = []
    Global_state.opp_moves = []  # record moves of opponent
