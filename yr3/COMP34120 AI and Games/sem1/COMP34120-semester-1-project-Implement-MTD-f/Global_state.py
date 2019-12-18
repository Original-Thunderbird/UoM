'''
A singleton class, do not make instance of it
'''
from copy import deepcopy


class Global_state:
    # board[0] - North, board[1] - South
    board = [[7, 7, 7, 7, 7, 7, 7, 0], [7, 7, 7, 7, 7, 7, 7, 0]]
    opp_last_move = []
    round = 0
    side = ""
    is_my_turn = False
    my_moves = [-1000]  # record moves of ourself
    opp_moves = ["-1000"]  # record moves of opponent
    all_moves = []
    can_swap = False  # True when we can swap side
    has_swapped = False
    is_last_move_mine = False

    @staticmethod
    def get_opp_last_move():
        return Global_state.opp_last_move.copy()

    @staticmethod
    def get_is_my_turn():
        return Global_state.is_my_turn

    @staticmethod
    def set_is_my_turn(new_turn):
        if new_turn != True and new_turn != False:
            return None
        Global_state.is_my_turn = new_turn
        return True

    @staticmethod
    def set_can_swap(new_status):
        if new_status != True and new_status != False:
            return None
        Global_state.can_swap = new_status
        return True

    @staticmethod
    def set_has_swapped_true(): # this function is supposed to be called only once or never
        Global_state.has_swapped = True
        return True

    @staticmethod
    def get_side():
        return Global_state.side

    @staticmethod
    def set_side(new_side):
        if new_side != "North" and new_side != 'South':
            return None
        Global_state.side = new_side
        return True

    @staticmethod
    def get_board():
        return deepcopy(Global_state.board)

    @staticmethod
    def set_board(new_board):
        Global_state.board = new_board
        return True

    @staticmethod
    def get_my_score():
        if Global_state.get_side() == 'North':
            return Global_state.board[0][7]
        elif Global_state.get_side() == 'South':
            return Global_state.board[1][7]
        else:
            return None

    @staticmethod
    def set_my_score(new_score):
        if Global_state.get_side() == 'North':
            Global_state.board[0][7] = new_score
            return True
        elif Global_state.get_side() == 'South':
            Global_state.board[1][7] = new_score
            return True
        else:
            return None

    @staticmethod
    def get_opp_score():
        if Global_state.get_side() == 'North':
            return Global_state.board[1][7]
        elif Global_state.get_side() == 'South':
            return Global_state.board[0][7]
        else:
            return None

    @staticmethod
    def set_opp_score(new_score):
        if Global_state.get_side() == 'North':
            Global_state.board[1][7] = new_score
            return True
        elif Global_state.get_side() == 'South':
            Global_state.board[0][7] = new_score
            return True
        else:
            return None

    @staticmethod
    def get_my_moves():
        return Global_state.my_moves.copy()

    ''' able to add multiple moves at one time '''
    @staticmethod
    def add_to_my_moves(new_moves):
        if type(new_moves) == 'list':
            Global_state.my_moves.extend(new_moves)
        else:
            Global_state.my_moves.append(new_moves)
        return True

    @staticmethod
    def get_opp_moves():
        return Global_state.opp_moves.copy()

    ''' able to add multiple moves at one time '''
    @staticmethod
    def add_to_opp_moves(new_moves):
        if type(new_moves) == 'list':
            Global_state.opp_moves.extend(new_moves)
        else:
            Global_state.opp_moves.append(new_moves)
        return True

    # TODO: perform check on return values and whether influences Global_state.all_moves value
    @staticmethod
    def get_all_moves():
        moves = Global_state.all_moves.copy()
        for i in range(0, len(moves)):
            moves[i] = moves[i]-1
        return moves

    @staticmethod
    def print_all_moves():
        all_moves = Global_state.all_moves.copy()
        return all_moves

    @staticmethod
    def add_to_all_moves(new_moves):
        # if type(new_moves) == 'list':
        #     Global_state.all_moves.extend(new_moves)
        # else:
        #     Global_state.all_moves.append(new_moves)
        Global_state.all_moves = Global_state.all_moves + [new_moves]
        return True

    ''' swap_side() has been tested '''

    @staticmethod
    def swap_side():
        tmp = Global_state.get_opp_moves()
        Global_state.opp_moves = Global_state.get_my_moves()
        Global_state.my_moves = tmp
        return True
