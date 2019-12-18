from Utils import *

# TODO: Implement 'Swap' criteria
# NOTICE: opp_moves is string list, my_moves is int list, and all_moves is int list
def handleInput(inputString):
    if inputString.startswith('START;'):
        _side = inputString.split(";")[1]
        # Global_state.side = _side
        Global_state.set_side(_side)
        if(Global_state.get_side() is None):
            print_debug_info("Raise error from Client.handleInput(), side is None\n\n")
        if _side == 'South': # south side will make a move first
            Global_state.set_is_my_turn(True)
        else:
            Global_state.set_can_swap(True) # north side can choose to swap at the first move
    elif inputString.startswith('CHANGE;'):
        split_inputString = inputString.split(";")
        turn = split_inputString[3]
        if turn == 'YOU': # opp has made move, now our turn
            Global_state.set_is_my_turn(True)
            _opp_move = split_inputString[1]
            if _opp_move == "SWAP":
                ''' Opponent make SWAP move '''
                current_side = Global_state.get_side()
                if current_side == "South":
                    ''' our side must be South when opponent can make SWAP operation '''
                    Global_state.set_side("North")
                else:
                    ''' if not, throw errors '''
                    print_debug_info(f"In Client.py handleInput(), current_side is not South when opponent perform SWAP, values {current_side}\n")
                # Global_state.swap_side()
                Global_state.set_has_swapped_true()
                Global_state.round += 1
                Global_state.add_to_opp_moves("-1")
                Global_state.add_to_all_moves(-1) # swap is represented as set seedNumber to -1
            elif Global_state.is_last_move_mine == False: # make sure it is not our bonus move
                Global_state.round += 1
                Global_state.add_to_opp_moves(_opp_move)
                Global_state.add_to_all_moves(int(_opp_move))
        else: # turn == "OPP"
            # ignore it if last move is ours since server will also return my move
            if Global_state.is_last_move_mine == False:
                Global_state.round += 1
                _opp_move = split_inputString[1]
                Global_state.add_to_opp_moves(_opp_move)
                Global_state.add_to_all_moves(int(_opp_move))
        current_board = parse_board(split_inputString[2])
        Global_state.set_board(current_board)
    elif inputString.startswith('END'):
        return None # game over, do nothing
    # elif inputString.startswith(''):
    #     ## get game result
    #     pass
    else:
        print_debug_info('Unexpected information received - ' + inputString) # handle unexpected input string
        return None
    return True

''' seedNumber starts from 1, when pass -1, treat as SWAP'''
def makeOutput(seedNumber): # make a move
    if seedNumber == -1:
        Global_state.round += 1
        Global_state.set_has_swapped_true()
        Global_state.set_can_swap(False)
        Global_state.add_to_my_moves(-1)
        Global_state.add_to_all_moves(-1)
        return "SWAP\n"
    if seedNumber <= 0:
        print_debug_info("seedNumber is not positive")
        return None
    elif seedNumber > 7:
        print_debug_info("seedNumber greater than 7")
        return None
        
    Global_state.round += 1
    Global_state.add_to_my_moves(seedNumber)
    Global_state.add_to_all_moves(seedNumber)
    outputString = f"MOVE;{seedNumber}\n"
    return outputString