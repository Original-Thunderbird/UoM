from treelib import *
from Utils import *
from Global_state import *
from Utils_minimax import *
from heuristic import *
import copy
import random

class nodeData:
    def __init__(self, nodeType):
        self.nodeType = nodeType    # Mini Node or Max Node
        self.alpha = float("-inf")     # Initialize alpha as -inf
        self.beta = float("inf")    # Initialize Beta as inf
        self.moves = []         # moves from the start state to current node
        self.lastMoves = []         # moves from parent node to current node
        self.bestMoves = []         # best next move to get most rewards 
        self.board = [[7, 7, 7, 7, 7, 7, 7, 0], [7, 7, 7, 7, 7, 7, 7, 0]]  # current board state


# def MTD_f(first_guess, depth):
#     # Wikipedia implementation https://en.wikipedia.org/wiki/MTD-f
#     g = first_guess
#     upper_bound = float("inf")
#     lower_bound = float("-inf")
#     while lower_bound < upper_bound:
#         beta = max(g, lower_bound + 1)
#         # g = iter_MiniMax(gameTree, currentNode, SearchDepth, round, weight_vector, verbose)
#         if g < beta:
#             upper_bound = g
#         else:
#             lower_bound = g
#     return g # TODO: this should be changed to the action needed to be taken

def MiniMax(gameTree, SearchDepth, round, weight_vector):
    
    verbose = False
    #tempId = round, 0, Global_state.get_opp_last_move()
    tempId = Global_state.get_all_moves()

    # TODO implementing MTD-f according to https://askeplaat.wordpress.com/534-2/mtdf-algorithm/#abmem
    # first_guess = 1
    first_guess = random.randint(100, 500)
    upper_bound = float("inf")
    lower_bound = float("-inf")

    g = first_guess
    beta = first_guess
    while lower_bound < upper_bound:
        if g == lower_bound:
            beta = g + 1
        else:
            beta = g
        
        ''' 
        Revise original programme to call Alpha-beta pruning with memory
        '''
        # if tree already have this current node, use this subtree as the starting point
        if gameTree.contains(str(tempId)):
            # print_debug_info("Reach here\n")        
            gameTree = gameTree.subtree(str(tempId))
            currentNode = gameTree.get_node(str(tempId))

            # currentNode.data.alpha = float("-inf") 
            # currentNode.data.beta = float("inf") 
            # iter_MiniMax(gameTree, currentNode, SearchDepth, round, weight_vector, verbose)

            # TODO
            currentNode.data.alpha = beta - 1 
            currentNode.data.beta = beta
            g = iter_MiniMax(gameTree, currentNode, SearchDepth, round, weight_vector, verbose)

        # initialize the tree
        else:
            gameTree = Tree()
            newNode = gameTree.create_node(str(tempId), str(tempId), parent=None, data=nodeData("max"))
            # Convert the board to make sure the max player always at the south
            if Global_state.get_side() == "South":
                newNode.data.board = Global_state.get_board()
            elif Global_state.get_side() == "North":
                newNode.data.board[0] = Global_state.get_board()[1]
                newNode.data.board[1] = Global_state.get_board()[0]
            else:
                pass
            # iter_MiniMax(gameTree, newNode, SearchDepth, round, weight_vector, verbose)

            # TODO
            newNode.data.alpha = beta - 1 
            newNode.data.beta = beta
            g = iter_MiniMax(gameTree, newNode, SearchDepth, round, weight_vector, verbose)

        ''' 
        End of call of alpha-beta pruning with memory
        '''
        if g < beta:
            upper_bound = g
        else:
            lower_bound = g

    # calculate and return the best moves
    print_debug_info(f"At round {Global_state.round}, g is {g}\n")
    bestMoves = gameTree.get_node(str(tempId)).data.bestMoves
    for i in range(0, len(bestMoves)):
        bestMoves[i] = bestMoves[i]+1

    return (gameTree, bestMoves)


def iter_MiniMax(gameTree, parentNode, SearchDepth, round, weight_vector, verbose, moveCombo = []):

    # 0 for opp side, 1 for my side
    if parentNode.data.nodeType == "mini":
        side = 0
    else:
        side = 1

    comboBoard = parentNode.data.board.copy()

    # if a bonus move, execute prevous moves to the current board
    if len(moveCombo) != 0:
        for n in moveCombo:
            comboBoard = move_seed_board(parentNode.data.nodeType, comboBoard, n)
        
    # rank the chidren
    priority = rank_sequence(comboBoard, side, round)
    # if round == 11: 
    #     print_debug_info(f'priority sequence: {priority}.\n\n')
    
    # no valid move (end of game)
    if len(priority) == 0:
        if moveCombo == []:
            if side == 0:
                parentNode.data.beta = heuristic(side, comboBoard, weight_vector)
            else:
                parentNode.data.alpha = heuristic(side, comboBoard, weight_vector)
            return cal_node_value(parentNode) # TODO add return value
        else:
            # tempId = round, side, moveCombo.copy()
            tempId = parentNode.data.moves + [moveCombo.copy()]
            if gameTree.contains(str(tempId)):
                newNode = gameTree.get_node(str(tempId))
            else:
                ''' original code '''
                if side == 0:
                    newNode = gameTree.create_node(str(tempId), str(tempId), parent = parentNode, data=nodeData("max"))
                    newNode.data.alpha = parentNode.data.alpha
                else:
                    newNode = gameTree.create_node(str(tempId), str(tempId), parent = parentNode, data=nodeData("mini"))
                    newNode.data.beta = parentNode.data.beta
                newNode.data.moves = parentNode.data.moves+moveCombo.copy()
                newNode.data.lastMoves = moveCombo.copy()
                newNode.data.board = comboBoard.copy()
                ''' end of original code '''

        if side == 0:
            newNode.data.alpha = heuristic(side, newNode.data.board, weight_vector)
            if parentNode.data.beta > newNode.data.alpha:
                parentNode.data.beta = newNode.data.alpha
                parentNode.data.bestMoves = moveCombo.copy()
        else:
            newNode.data.beta = heuristic(side, newNode.data.board, weight_vector)
            if parentNode.data.alpha < newNode.data.beta:
                parentNode.data.alpha = newNode.data.beta
                parentNode.data.bestMoves = moveCombo.copy()
        return cal_node_value(parentNode) # TODO Add return value
    else:
        pass

    
    for i in priority:
    # for each well, see if possible to choose

        # only wells with seeds are possible
        if (parentNode.data.nodeType == "mini" and parentNode.data.board[0][i]!=0) or (parentNode.data.nodeType == "max" and parentNode.data.board[1][i]!=0):
            
            # define mini node or max node
            if side == 0:
                nodeType = "max"
            else:
                nodeType = "mini"

            lastMoves = moveCombo.copy()
            lastMoves.append(i)
            tempBoard = comboBoard.copy()

            # parentNode offers a bonus move opportunity
            if (parentNode.data.nodeType == "mini" and (tempBoard[0][i] + i) == (7 or 22 or 37)) or (parentNode.data.nodeType == "max" and (tempBoard[1][i] + i) == (7 or 22 or 37)):
                return iter_MiniMax(gameTree, parentNode, SearchDepth, round, weight_vector, verbose, moveCombo = lastMoves.copy())
                # return # TODO Add return value
            else:
                tempBoard = move_seed_board(parentNode.data.nodeType, deepcopy(tempBoard), i)

            #tempId2 = round, side, lastMoves.copy()
            tempId2 = parentNode.data.moves+[lastMoves]
            # print_debug_info(f'TempId2 : {str(tempId2)}.\n\n')
            if gameTree.contains(str(tempId2)):
                # print_debug_info("Reach here, same tempId2\n")
                newNode = gameTree.get_node(str(tempId2))
                newNode.data.alpha = parentNode.data.alpha
                newNode.data.beta = parentNode.data.beta
                # newNode.data.moves = parentNode.data.moves+lastMoves
            
            else:
                # create the new node
                newNode = gameTree.create_node(str(tempId2), str(tempId2), parent = parentNode, data=nodeData(nodeType))
                newNode.data.alpha = parentNode.data.alpha
                newNode.data.beta = parentNode.data.beta
                newNode.data.moves = parentNode.data.moves+lastMoves
                newNode.data.lastMoves = lastMoves.copy()
                newNode.data.board = tempBoard.copy()

            # performed enough search or reach terminal node
            if gameTree.depth(newNode) == SearchDepth:
                if newNode.data.nodeType == "mini":
                    newNode.data.beta = heuristic(side, newNode.data.board, weight_vector)
                    if parentNode.data.alpha < newNode.data.beta:
                        parentNode.data.alpha = newNode.data.beta
                        parentNode.data.bestMoves = lastMoves
                elif newNode.data.nodeType == "max":
                    newNode.data.alpha = heuristic(side, newNode.data.board, weight_vector)
                    if parentNode.data.beta > newNode.data.alpha:
                        parentNode.data.beta = newNode.data.alpha
                        parentNode.data.bestMoves = lastMoves
                
            else:
                # go to a deeper level and get an optimistic value returned, this time go from max node to min node or vice versa
                if side == 1:
                    iter_MiniMax(gameTree, newNode, SearchDepth, round+1, weight_vector, verbose)
                else:
                    iter_MiniMax(gameTree, newNode, SearchDepth, round, weight_vector, verbose)

                if parentNode.data.nodeType == "mini" and parentNode.data.beta > newNode.data.alpha:
                    parentNode.data.beta = newNode.data.alpha
                    parentNode.data.bestMoves = lastMoves
                    
                elif parentNode.data.nodeType == "max" and parentNode.data.alpha < newNode.data.beta:
                    parentNode.data.alpha = newNode.data.beta
                    parentNode.data.bestMoves = lastMoves

            # pruned
            if parentNode.data.alpha >= parentNode.data.beta:
                break

    # return the value if the node itself
    parentNode.data.is_searched = True
    return cal_node_value(parentNode) # TODO Add return value


def cal_node_value(node):
    value = 0
    nodeType = node.data.nodeType
    if nodeType == "mini":
        value = node.data.beta
    else: # nodeType == "max"
        value = node.data.alpha
    return value