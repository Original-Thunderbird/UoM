from treelib import *
from Utils import *
from Global_state import *
from Utils_minimax import *

class nodeData:
    def __init__(self, nodeType):
        self.nodeType = nodeType    # Mini Node or Max Node
        self.alpha = float("-inf")     # Initialize alpha as -inf
        self.beta = float("inf")    # Initialize Beta as inf
        self.moves = []
        self.lastMoves = []
        self.bestMoves = []
        self.board = [[7, 7, 7, 7, 7, 7, 7, 0], [7, 7, 7, 7, 7, 7, 7, 0]]  # current board state
        self.is_my_turn = True
        self.is_pruned = False
        self.is_searched = False

    def get_is_my_turn(self):
        return self.is_my_turn

def MiniMax(SearchDepth):
    # TODO: Copy the overlaped subtree to the newTree
    # print_debug_info(f'show all moves!!!: {Global_state.get_all_moves()}\n\n')
    # print_debug_info(f'gameTree.contains(str(Global_state.get_all_moves())) : {gameTree.contains(str(Global_state.get_all_moves()))}\n\n')
    # if gameTree.contains(str(Global_state.get_all_moves())):
    #     print_debug_info(f'inside subtree\n\n')
    #     gameTree = Tree(gameTree.subtree(str(Global_state.get_all_moves())))
    #     print_debug_info(f'gameTree sub: {gameTree}\n\n')
    gameTree = Tree()
    maxValue = AddNode(gameTree, None, SearchDepth)
    # print_debug_info(f'gameTree : {gameTree}\n\n')
    
    bestMoves = gameTree.get_node(str(Global_state.get_all_moves())).data.bestMoves
    for i in range(0, len(bestMoves)):
        bestMoves[i] = bestMoves[i]+1

    # print_debug_info(f'Best moves from Minimax(+1) : {bestMoves}\n\n')
    tNode = gameTree.get_node(str(Global_state.get_all_moves()))
    tBoard = tNode.data.board
    # for n in range (0, len(bestMoves)):
    #     tBoard = move_seed_board(max, tBoard, bestMoves[n])
    # print_debug_info(f'Root Node: {tNode}\n\n')
    # print_debug_info(f'Board after Best moves: {tBoard}\n\n')
    return bestMoves
    
def AddNode(gameTree, parentNode, SearchDepth):
    # TODO: if previous subtree, go down without new node'''
    # if parentNode != None and parentNode.data.is_searched:
    #     print_debug_info(f'Inside no newNode------------\n\n')
    #     parentNode.data.is_pruned = False
    #     childrenList = parentNode.fpointer
    #     maxValue = -1000
    #     bestMoves = -1
    #     if len(childrenList) != 7:
    #         parentNode.data.is_searched
    #     for i in range(6, -1, -1):
    #         for j in range(0 ,len(childrenList)):
    #             if i == gameTree.get_node(childrenList[j]).data.lastMoves:
    #                 value = AddNode(gameTree, gameTree.get_node(childrenList[j]), SearchDepth)
            
    #         if maxValue < value:
    #             maxValue = value
    #             parentNode.data.bestMoves = gameTree.get_node(childrenList[i]).data.lastMoves
    #     return maxValue

    # if leaf create a new node
    # if no tree
    if parentNode == None:
        # print_debug_info(f'Head------------Inside AddNode parentNode == None, create first node\n\n')
        newNode = gameTree.create_node(str(Global_state.get_all_moves()), str(Global_state.get_all_moves()), parent=None, data=nodeData("max"))
        if Global_state.get_side() == "South":
            newNode.data.board = Global_state.get_board()
        elif Global_state.get_side() == "North":
            newNode.data.board[0] = Global_state.get_board()[1]
            newNode.data.board[1] = Global_state.get_board()[0]
        newNode.data.moves = Global_state.get_all_moves()
        
        # print_debug_info(f"Set newNode nodeType: {newNode.data.nodeType}\n\n")
        # print_debug_info(f'Set newNode nid as all_moves: {str(Global_state.get_all_moves())}\n\n')
        # print_debug_info(f'Set newNode board as global: {newNode.data.board}\n\n')
        # print_debug_info(f'Set newNode moves as global: {newNode.data.moves}\n\n')
        # print_debug_info(f'print newNode: {newNode}\n\n')

        return AddNode(gameTree, newNode, SearchDepth)

    else:
        # TODO: add rank here
        for i in range(6, -1, -1):
            if parentNode.data.is_pruned == True:
                # print_debug_info(f'In for loop-----------node: {parentNode} was pruned\n\n')
                break
            if (parentNode.data.nodeType == "mini" and parentNode.data.board[0][i]!=0) or (parentNode.data.nodeType == "max" and parentNode.data.board[1][i]!=0):
            # define mini node or max node
                if gameTree.depth(parentNode)%2 != 0:
                    nodeType = "max"
                else:
                    nodeType = "mini"

                lastMoves = []
                lastMoves.append(i)
                tempBoard = parentNode.data.board

                is_my_turn = True
                if (parentNode.data.nodeType == "mini" and (tempBoard[0][i] + i) == (7 or 22 or 37)) or (parentNode.data.nodeType == "max" and (tempBoard[1][i] + i) == (7 or 22 or 37)):
                    # print_debug_info(f'contunious moves-------------.........\n\n')
                    tempBoard = move_seed_board(parentNode.data.nodeType, tempBoard, i)
                    while (is_my_turn):
                        for j in range(6, -1, -1):
                            if (parentNode.data.nodeType == "mini" and (tempBoard[0][j] + j) == (7 or 22 or 37)) or (parentNode.data.nodeType == "max" and (tempBoard[1][j] + j) == (7 or 22 or 37)):
                                lastMoves.append(j)
                                tempBoard = move_seed_board(parentNode.data.nodeType, tempBoard, j)
                                break
                            if j == 0:
                                is_my_turn = False
                    # one more move after combo
                    for k in range(6, -1, -1):
                        if (parentNode.data.nodeType == "mini" and tempBoard[0][k] != 0) or (parentNode.data.nodeType == "max" and tempBoard[1][k] != 0):
                            lastMoves.append(k)
                            break
                else:
                    tempBoard = move_seed_board(parentNode.data.nodeType, tempBoard, i)

                # create a new node
                newNode = gameTree.create_node(str(parentNode.data.moves+lastMoves), str(parentNode.data.moves+lastMoves), parent = parentNode, data=nodeData(nodeType))
                
                newNode.data.alpha = parentNode.data.alpha
                newNode.data.beta = parentNode.data.beta
                newNode.data.moves = parentNode.data.moves+lastMoves
                newNode.data.board = tempBoard
                newNode.data.lastMoves = lastMoves

                # print_debug_info(f"Node------------Set newNode nid as : {newNode.data.moves}\n\n")
                # print_debug_info(f'parentNode: {parentNode}, create sub node to it\n\n')
                # print_debug_info(f"depth of node: {gameTree.depth(newNode)}\n\n")
                # print_debug_info(f"nodeType of new node: {nodeType}\n\n")

                ''' if reach the search depth'''
                if gameTree.depth(newNode) == SearchDepth:
                    if newNode.data.nodeType == "mini":
                        newNode.data.beta = get_my_score(newNode.data.board)-get_opp_score(newNode.data.board)
                        if parentNode.data.alpha < newNode.data.beta:
                            parentNode.data.alpha = newNode.data.beta
                    elif newNode.data.nodeType == "max":
                        newNode.data.alpha = get_my_score(newNode.data.board)-get_opp_score(newNode.data.board)
                        if parentNode.data.beta > newNode.data.alpha:
                            parentNode.data.beta = newNode.data.alpha
                else:
                    newH = AddNode(gameTree, newNode, SearchDepth)
                
                    # alpha-beta pruning
                    if parentNode.data.nodeType == "mini" and parentNode.data.beta > newH:
                        parentNode.data.beta = newH
                        parentNode.data.bestMoves = lastMoves
                        # print_debug_info(f'set beta {parentNode.data.beta} to node {parentNode} \n\n')
                        # print_debug_info(f'current bestMoves: {lastMoves} \n\n')
                    elif parentNode.data.nodeType == "max" and parentNode.data.alpha < newH:
                        parentNode.data.alpha = newH
                        parentNode.data.bestMoves = lastMoves
                        # print_debug_info(f'set alpha {parentNode.data.alpha} to node {parentNode} \n\n')
                        # print_debug_info(f'current bestMoves: {lastMoves} \n\n')
                
                if parentNode.data.alpha > parentNode.data.beta:
                    parentNode.data.is_pruned = True

        parentNode.data.is_searched = True
        if parentNode.data.nodeType == "mini":
            return parentNode.data.beta
        elif parentNode.data.nodeType == "max":
            return parentNode.data.alpha