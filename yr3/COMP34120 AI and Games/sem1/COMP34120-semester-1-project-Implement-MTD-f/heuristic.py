from Utils import *
import operator


def rank_sequence(my_board, my_side, round):

    # board that search is based on should be that in the parent of minimax search tree,
    # so should be a param passed in the function instead of that in Global_state that
    # remain unchanged throughout DFS, so the line below is commented out
    # my_board = Global_state.get_board().copy()
    priority_with_weight = []

    # see if ememy can capture or have bonus move
    opp_mov_pre = []
    opp_gain_sum_pre = 0
    my_weight = 0
    for opp_choice in range(0, 7):
        if my_board[1-my_side][opp_choice] == 0:  # jump empty holes
            continue
        opp_weight = 0
        opp_fin_pos_pre = my_board[1-my_side][opp_choice] + opp_choice
        # enemy capture
        if (0 <= opp_fin_pos_pre <= 6 or 0 <= opp_fin_pos_pre - 15 <= opp_choice) and my_board[1-my_side][opp_fin_pos_pre%15] == 0:
            opp_weight = 12 + 1 + my_board[my_side][6-opp_fin_pos_pre%15] + my_board[1-my_side][opp_choice]/15
            if opp_fin_pos_pre/15 > 0:
                opp_weight += 1  # # of seeds in own kalah:
        # enemy bonus move
        elif opp_fin_pos_pre%15 == 7:
            opp_weight = my_board[1-my_side][opp_choice]/15 + opp_choice  + 10
        opp_mov_pre.append((opp_choice, opp_weight))
        opp_gain_sum_pre += opp_weight
    # if round == 11:
    #     print_debug_info(f'before my possible move, opp gain list is: {opp_mov_pre}, total weight is:{opp_gain_sum_pre}\n')

    # if my_board[my_side][6] == 1:  # 1-(1)
    #     extra = 1
    #     low_bound_to_consider = 2
    # elif operator.eq(my_board[my_side][5:7], [1, 2]):  # 1,2-(1,2)
    #     extra = 2
    #     low_bound_to_consider = 3
    # elif operator.eq(my_board[my_side][4:7], [1, 1, 3]):
    #     extra = 5
    #     low_bound_to_consider = 3
    for my_choice in range(0, 7):
        if my_board[my_side][my_choice] == 0:  # jump empty holes
            continue
        # if slots arranged in a list, the slot that last seed lands in
        fin_position = my_board[my_side][my_choice] + my_choice
        # landing in own kalah, considering multiple loops
        my_weight = 0
        if fin_position%15 == 7:
            my_weight = my_board[my_side][my_choice]/15 + my_choice + 10
        # capture
        # elements on right hand isde of euuation:
        # weight : 12
        # the seed land in our empty slot : 1 
        # # of seeds in corresponding enemy's slot : my_board[1-my_side][6-fin_position] + my_board[my_side][my_choice]/15
        elif (0<= fin_position <= 6 or 0 <= fin_position - 15 <= my_choice) and my_board[my_side][fin_position%15] == 0:
            my_weight = 12 + 1 + my_board[1-my_side][6-fin_position%15] + my_board[my_side][my_choice]/15
            if fin_position/15 > 0:
                my_weight += 1  # # of seeds in own kalah:
        # defensive actions
        if 8<= fin_position <= 14:
            # construct virtual board
            temp_board = []
            temp_board.extend(my_board[my_side])
            temp_board.extend(my_board[1-my_side][:7])
            # my turn
            for i in range(1, temp_board[my_choice]+1):
                temp_board[(my_choice+i) % 15] += 1
                temp_board[my_choice] -= 1

            _opp_board = []
            _opp_board.append(temp_board[:8])
            _opp_board.append(temp_board[8:])
            _opp_board[1].append(my_board[1-my_side][7])

            opp_mov_aft = []
            opp_gain_sum_aft = 0

            for opp_choice in range(0, 7):
                if _opp_board[1-my_side][opp_choice] == 0:  # jump empty holes
                    continue
                opp_weight = 0
                opp_fin_pos_aft = _opp_board[1-my_side][opp_choice] + opp_choice
                # enemy capture
                if (0 <= opp_fin_pos_aft <= 6 or 0 <= opp_fin_pos_aft - 15 <= opp_choice) and _opp_board[1-my_side][opp_fin_pos_aft%15] == 0:
                    opp_weight = 12 + 1 + _opp_board[my_side][6-opp_fin_pos_aft%15] + _opp_board[1-my_side][opp_choice]/15
                    if opp_fin_pos_aft/15 > 0:
                        opp_weight += 1  # # of seeds in own kalah:
                # enemy bonus move
                elif (_opp_board[1-my_side][opp_choice]%15 + opp_choice) == 7:
                    opp_weight = _opp_board[1-my_side][opp_choice]/15 + opp_choice  + 10
                opp_mov_aft.append((opp_choice, opp_weight))
                opp_gain_sum_aft += opp_weight
            # if round == 11:
            #     print_debug_info(f'after move {my_choice} opp gain list is: {opp_mov_pre}, total weight is:{opp_gain_sum_pre}\n')
            diff = opp_gain_sum_pre - opp_gain_sum_aft
            my_weight += diff
        else:
            my_weight -= opp_gain_sum_pre
        priority_with_weight.append((my_choice, my_weight))    

    lst = len(priority_with_weight)
    for i in range(0, lst):
        for j in range(0, lst-i-1):
            if (priority_with_weight[j][1] < priority_with_weight[j + 1][1]):
                temp = priority_with_weight[j]
                priority_with_weight[j] = priority_with_weight[j + 1]
                priority_with_weight[j + 1] = temp
    # meaning of tuples in priority_with_weight: (branch number, weight), branch number is ordered in sequence that its weight goes down
    # example:priority_with_weight = [(branch_1, w1), (branch_2, w2), ..., (branch_n, wn)], w1 > w2 > ... > wn
    # if round == 11:
    #     print_debug_info(f'for side {my_side}, final ran list is:{priority_with_weight}\n\n')
    priority = []
    for tup in priority_with_weight:
        priority.append(tup[0])
    # priority: list of branch number
    return priority


def heuristic(my_side, my_board, w):
    h1 = my_board[1][7]
    h2 = -my_board[0][7]
    h3 = my_board[1][5]+my_board[1][6]
    h4 = my_board[1][2]+my_board[1][3]+my_board[0][4]
    h5 = my_board[1][0]+my_board[1][1]
    return w[0]*h1 + w[1]*h2 + w[2]*h3 + w[3]*h4 + w[4]*h5

