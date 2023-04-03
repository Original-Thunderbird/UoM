from Global_state import *
from Utils import print_debug_info

def move_seed_board(nodeType, board, seedNumber):
    seedNumber+=1
    if nodeType == "max":
        # _board: [ board[1][0:8], board[0][0:7]]
        _board = board[1].copy()  # have to use .copy() to avoid same reference
        _board.extend(board[0][:7])  # not including kalah of North (board[0][7])
    elif nodeType == "mini":
        # _board: [ board[0][0:8], board[1][0:7]]
        _board = board[0].copy()  # have to use .copy() to avoid same reference
        _board.extend(board[1][:7])  # not including kalah of South (board[1][7])
    else:
        print_debug_info("Raise error from move_seed_board(), side is wrong\n")
        return None
    _round = 15
    # index starts at 0, but seedNumber starts at 1
    seeds = _board[seedNumber-1]
    if seeds == 0:
        print_debug_info("Raise error from move_seed_board(), this well is empty")
        return None
    # sow seeds

    for i in range(1, seeds+1):
        _board[(seedNumber-1+i) % _round] += 1
        _board[seedNumber-1] -= 1

    # properly works independently
    fin_index = (seedNumber-1+seeds) % _round
    if _board[fin_index] == 1 and fin_index >=0 and fin_index <=6 and _board[14-fin_index] != 0:
        _board[fin_index] = 0
        _board[7] += 1 +  _board[14-fin_index]
        _board[14-fin_index] = 0

    _fin_board = [[],[]]
    # restore format of board
    # return val meaning: board state, seeds in own kalah, seeds in opponent kalah
    # print_debug_info(f" --- simulated board result --- \n{nodeType}\n ")
    if nodeType == "max":
        _fin_board[0] = _board[8:].copy()
        _fin_board[0].append(board[0][7])
        _fin_board[1] = _board[0:8].copy()
        # print_debug_info(f"max - _fin_board = {_fin_board}\n")
        return _fin_board
    if nodeType == "mini":
        _fin_board[0] = _board[0:8].copy()
        _fin_board[1] = _board[8:].copy()
        _fin_board[1].append(board[1][7])
        # print_debug_info(f"mini - _fin_board = {_fin_board}\n")
        return _fin_board

def get_my_score(board):
    return board[1][7]

def get_opp_score(board):
    return board[0][7]