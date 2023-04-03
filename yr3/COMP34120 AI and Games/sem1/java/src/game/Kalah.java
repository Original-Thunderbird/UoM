package game;

import java.util.ArrayList;

/**
 * This class deals with moves on a Kalah board.
 */
public class Kalah
{
    /**
     * The board to play on.
     */
    private final Board board;

    /**
     * @param board The board to play on.
     * @throws NullPointerException if "board" is null.
     */
    public Kalah (Board board) throws NullPointerException
    {
        if (board == null)
            throw new NullPointerException();
        this.board = board;
    }

    /**
     * @return The board this object operates on.
     */
    public Board getBoard ()
    {
        return board;
    }

    /**
     * Checks whether a given move is legal on the underlying board. The move
     * is not actually made.
     * @param move The move to check.
     * @return true if the move is legal, false if not.
     */
    public boolean isLegalMove (Move move)
    {
        return isLegalMove(board, move);
    }

    /**
     * Performs a move on the underlying board. The move must be legal. If
     * the move terminates the game, the remaining seeds of the opponent are
     * collected into their store as well (so that all holes are empty).<BR>
     * The "notifyObservers()" method of the board is called with the "move"
     * as argument.
     *
     * @param move The move to make.
     * @return The side who's turn it is after the move. Arbitrary if the
     *         game is over.
     * @see #isLegalMove(Move)
     * @see #gameOver()
     * @see java.util.Observable#notifyObservers(Object)
     */
    public Side makeMove (Move move)
    {
        return makeMove(board, move);
    }

    /**
     * Checks whether the game is over (based on the board).
     * @return "true" if the game is over, "false" otherwise.
     */
    public boolean gameOver()
    {
        return gameOver(board);
    }

    /**
     * Checks whether a given move is legal on a given board. The move
     * is not actually made.
     * @param board The board to check the move for.
     * @param move The move to check.
     * @return true if the move is legal, false if not.
     */
    public static boolean isLegalMove (Board board, Move move)
    {
        // check if the hole is existent and non-empty:
        return (move.getHole() <= board.getNoOfHoles())
                && (board.getSeeds(move.getSide(), move.getHole()) != 0);
    }

    /**
     * Performs a move on a given board. The move must be legal. If
     * the move terminates the game, the remaining seeds of the opponent are
     * collected into their store as well (so that all holes are empty).<BR>
     * The "notifyObservers()" method of the board is called with the "move"
     * as argument.
     *
     * @param board The board to make the move on.
     * @param move The move to make.
     * @return The side who's turn it is after the move. Arbitrary if the
     *         game is over.
     * @see #isLegalMove(Board, Move)
     * @see #gameOver(Board)
     * @see java.util.Observable#notifyObservers(Object)
     */
    public static Side makeMove (Board board, Move move)
    {
		/* from the documentation:
		  "1. The counters are lifted from this hole and sown in anti-clockwise direction, starting
		      with the next hole. The player's own kalahah is included in the sowing, but the
		      opponent's kalahah is skipped.
		   2. outcome:
		    	1. if the last counter is put into the player's kalahah, the player is allowed to
		    	   move again (such a move is called a Kalah-move);
		    	2. if the last counter is put in an empty hole on the player's side of the board
		    	   and the opposite hole is non-empty,
		    	   a capture takes place: all stones in the opposite opponents pit and the last
		    	   stone of the sowing are put into the player's store and the turn is over;
		    	3. if the last counter is put anywhere else, the turn is over directly.
		   3. game end:
		    	The game ends whenever a move leaves no counters on one player's side, in
		    	which case the other player captures all remaining counters. The player who
		    	collects the most counters is the winner."
		*/


        // pick seeds:
        int seedsToSow = board.getSeeds(move.getSide(), move.getHole());
        board.setSeeds(move.getSide(), move.getHole(), 0);

        int holes = board.getNoOfHoles();
        int receivingPits = 2*holes + 1;  // sow into: all holes + 1 store
        int rounds = seedsToSow / receivingPits;  // sowing rounds
        int extra = seedsToSow % receivingPits;  // seeds for the last partial round
    	/* the first "extra" number of holes get "rounds"+1 seeds, the
    	   remaining ones get "rounds" seeds */

        // sow the seeds of the full rounds (if any):c
        if (rounds != 0)
        {
            for (int hole = 1; hole <= holes; hole++)
            {
                board.addSeeds(Side.NORTH, hole, rounds);
                board.addSeeds(Side.SOUTH, hole, rounds);
            }
            board.addSeedsToStore(move.getSide(), rounds);
        }

        // sow the extra seeds (last round):
        Side sowSide = move.getSide();
        int sowHole = move.getHole();  // 0 means store
        for (; extra > 0; extra--)
        {
            // go to next pit:
            sowHole++;
            if (sowHole == 1)  // last pit was a store
                sowSide = sowSide.opposite();
            if (sowHole > holes)
            {
                if (sowSide == move.getSide())
                {
                    sowHole = 0;  // sow to the store now
                    board.addSeedsToStore(sowSide, 1);
                    continue;
                }
                else
                {
                    sowSide = sowSide.opposite();
                    sowHole = 1;
                }
            }
            // sow to hole:
            board.addSeeds(sowSide, sowHole, 1);
        }

        // capture:
        if ( (sowSide == move.getSide())  // last seed was sown on the moving player's side ...
                && (sowHole > 0)  // ... not into the store ...
                && (board.getSeeds(sowSide, sowHole) == 1)  // ... but into an empty hole (so now there's 1 seed) ...
                && (board.getSeedsOp(sowSide, sowHole) > 0) )  // ... and the opposite hole is non-empty
        {
            board.addSeedsToStore(move.getSide(), 1 + board.getSeedsOp(move.getSide(), sowHole));
            board.setSeeds(move.getSide(), sowHole, 0);
            board.setSeedsOp(move.getSide(), sowHole, 0);
        }

        // game over?
        Side finishedSide = null;
        if (holesEmpty(board, move.getSide()))
            finishedSide = move.getSide();
        else if (holesEmpty(board, move.getSide().opposite()))
            finishedSide = move.getSide().opposite();
    		/* note: it is possible that both sides are finished, but then
    		   there are no seeds to collect anyway */
        if (finishedSide != null)
        {
            // collect the remaining seeds:
            int seeds = 0;
            Side collectingSide = finishedSide.opposite();
            for (int hole = 1; hole <= holes; hole++)
            {
                seeds += board.getSeeds(collectingSide, hole);
                board.setSeeds(collectingSide, hole, 0);
            }
            board.addSeedsToStore(collectingSide, seeds);
        }

        board.notifyObservers(move);

        // who's turn is it?
        if (sowHole == 0)  // the store (implies (sowSide == move.getSide()))
            return move.getSide();  // move again
        else
            return move.getSide().opposite();
    }

    /**
     * Checks whether all holes on a given side are empty.
     * @param board The board to check.
     * @param side The side to check.
     * @return "true" iff all holes on side "side" are empty.
     */
    protected static boolean holesEmpty (Board board, Side side)
    {
        for (int hole = 1; hole <= board.getNoOfHoles(); hole++)
            if (board.getSeeds(side, hole) != 0)
                return false;
        return true;
    }

    /**
     * Checks whether the game is over (based on the board).
     * @param board The board to check the game state for.
     * @return "true" if the game is over, "false" otherwise.
     */
    public static boolean gameOver (Board board)
    {
        // The game is over if one of the agents can't make another move.

        return holesEmpty(board, Side.NORTH) || holesEmpty(board, Side.SOUTH);
    }

    public int rankSequence (Board board, Side my_side)
    {
        // copy a new board for virtual moves
        my_side = Side.SOUTH;
        Board my_board = new Board(board);
//        board that search is based on should be that in the parent of minimax search tree,
//        so should be a param passed in the function instead of that in Global_state that
//        remain unchanged throughout DFS, so the line below is commented out
        ArrayList<TwoTuple> priority_with_weight = new ArrayList<>();

//        see if ememy can capture or have bonus move

        int opp_gain_sum_pre = 0;
        for(int opp_choice = 1; opp_choice <= my_board.getNoOfHoles(); opp_choice++){
            if (my_board.getSeeds(my_side.opposite(), opp_choice) == 0){
                continue;
            }
            int opp_weight = 0;
            // last well that the final stone lands in
            int opp_fin_pos_pre = my_board.getSeeds(my_side.opposite(), opp_choice) + opp_choice;
            //enemy capture
            if ((( opp_fin_pos_pre>=1 && opp_fin_pos_pre<=7) || ( opp_fin_pos_pre - 15 >=1 && opp_fin_pos_pre - 15 < opp_choice)) && (my_board.getSeeds(my_side.opposite(), opp_fin_pos_pre%15) == 0) ||
                    (my_board.getSeeds(my_side.opposite(), opp_choice) == 15)){
                opp_weight = 12 + 1 + my_board.getSeedsOp(my_side.opposite(), opp_fin_pos_pre%15) + my_board.getSeeds(my_side.opposite(), opp_choice)/15;
            }
            //enemy bonus move
            else if (opp_fin_pos_pre%15 == 8){
                opp_weight = my_board.getSeeds(my_side.opposite(), opp_choice)/15 + opp_choice + 10;
            }
            opp_gain_sum_pre += opp_weight;
        }

        for(int my_choice = 1; my_choice <= my_board.getNoOfHoles(); my_choice++){
            if (my_board.getSeeds(my_side, my_choice) == 0){
                continue;
            }
            int my_weight = 0;
            int fin_pos = my_board.getSeeds(my_side, my_choice) + my_choice;
            //capture
            //elements on right hand side of equation:
//            weight : 12
//            the seed land in our empty slot : 1
//            # of seeds in corresponding enemy's slot : my_board.getSeeds(my_side.opposite(), 8 - fin_pos%15)
//            # of seeds landed in scoring well:         my_board.getSeeds(my_side, my_choice)/15
            if ((( fin_pos>=1 && fin_pos<=7) || ( fin_pos - 15 >=1 && fin_pos - 15 < my_choice)) && (my_board.getSeeds(my_side, my_choice) == 0) ||
                    (my_board.getSeeds(my_side, my_choice) == 15)){
                my_weight = 12 + 1 + my_board.getSeedsOp(my_side, fin_pos%15) + my_board.getSeeds(my_side, my_choice)/15;
            }
            //bonus move
            else if (fin_pos%15 == 8){
                my_weight = my_board.getSeeds(my_side, my_choice)/15 + my_choice + 10;
            }
            //defensive action
            if (fin_pos >= 9 && fin_pos <= 15){
                Board opp_board = new Board(my_board);
                //make moves on my_board
                makeMove(opp_board, new Move(my_side, my_choice));

                int opp_gain_sum_aft = 0;
                for(int opp_choice = 1; opp_choice <= opp_board.getNoOfHoles(); opp_choice++){
                    if (opp_board.getSeeds(my_side.opposite(), opp_choice) == 0){
                        continue;
                    }
                    int opp_weight = 0;
                    // last well that the final stone lands in
                    int opp_fin_pos_aft = opp_board.getSeeds(my_side.opposite(), opp_choice) + opp_choice;
                    //enemy capture
                    if ((( opp_fin_pos_aft>=1 && opp_fin_pos_aft<=7) || ( opp_fin_pos_aft - 15 >=1 && opp_fin_pos_aft - 15 < opp_choice)) && (opp_board.getSeeds(my_side.opposite(), opp_fin_pos_aft%15) == 0) ||
                            (opp_board.getSeeds(my_side.opposite(), opp_choice) == 15)){
                        opp_weight = 12 + 1 + opp_board.getSeedsOp(my_side.opposite(), opp_fin_pos_aft%15) + opp_board.getSeeds(my_side.opposite(), opp_choice)/15;
                    }
                    //enemy bonus move
                    else if (opp_fin_pos_aft%15 == 8){
                        opp_weight = opp_board.getSeeds(my_side.opposite(), opp_choice)/15 + opp_choice + 10;
                    }
                    opp_gain_sum_aft += opp_weight;
                }
                int diff = opp_gain_sum_pre - opp_gain_sum_aft;
                my_weight += diff;
            }
            else {
                my_weight -= opp_gain_sum_pre;
            }
            priority_with_weight.add(new TwoTuple(my_choice, my_weight));
        }
//        Collections.sort(priority_with_weight);
////        meaning of tuples in priority_with_weight: (branch number, weight), branch number is ordered in sequence that its weight goes down
////        example:priority_with_weight = [(branch_1, w1), (branch_2, w2), ..., (branch_n, wn)], w1 > w2 > ... > wn
//        int[] priority = new int[priority_with_weight.size()];
//        int i = 0;
//        for (TwoTuple tuple:priority_with_weight){
//            priority[i] = tuple.getChoice();
//            i++;
//        }
//
//        return priority;
//        int weight_sum = 0;
//            for(TwoTuple tuple : priority_with_weight){
//            weight_sum +=  tuple.getWeight();
//        }
//
//            return weight_sum;
        int weight_max = 0;
        for(TwoTuple tuple : priority_with_weight){
            if(tuple.getWeight()>weight_max){
                weight_max =  tuple.getWeight();
            }

        }
        return weight_max;
    }
//
//    public static NextMove AlphaBetaWithMemory(Board currentBoard, Side currentSide, int depth) {
//        if (depth == 0 || Kalah.gameOver(currentBoard)) {
//            NextMove leafNode = new NextMove();
//            leafNode.bestMove = null;
//            leafNode.value = currentBoard.getSeedsInStore(Side.SOUTH) - currentBoard.getSeedsInStore(Side.NORTH);
//            return leafNode;
//        }
//
//        Kalah currentGame = new Kalah(currentBoard);
//        NextMove bestMove = new NextMove();
//        NextMove temp;
//
//        if (currentSide == Side.SOUTH) { // MAXIMIZING PLAYER
//            for (int well = 1; well <= 7; well++) { // TODO: use ranking instead of sequential for-loop
//                Move currentMove = new Move(currentSide, well);
//                if (currentGame.isLegalMove(currentMove)) {
//                    Board copy_currentBoard = new Board(currentBoard);
//                    Side nextSide = Kalah.makeMove(copy_currentBoard, currentMove);
//                    // no need to judge whether game is over
//                    if (nextSide != currentSide) {
//                        temp = AlphaBetaWithMemory(copy_currentBoard, currentSide.opposite(),depth-1);
//                    } else {
//                        // bonus moves are considered in the same depth
//                        temp = AlphaBetaWithMemory(copy_currentBoard, currentSide, depth);
//                    }
//                    if (temp.value > bestMove.value) {
//                        bestMove.value = temp.value;
//                        bestMove.bestMove = currentMove;
//                    }
//                    alpha = Math.max(alpha, bestMove.value);
//                }
//            }
//            return bestMove;
//        } else {
//            bestMove.value = 10000; // initialize to "inf"
////            int[] priority = currentGame.rankSequence(currentBoard, currentSide);
////            for(int i=0; i<priority.length; i++){
//            for (int well = 1; well <= 7; well++) { // TODO: use ranking instead of sequential for-loop
//                Move currentMove = new Move(currentSide, well);
//                if (currentGame.isLegalMove(currentMove)) {
//                    Board copy_currentBoard = new Board(currentBoard);
//                    Side nextSide =  Kalah.makeMove(copy_currentBoard, currentMove);
//                    // no need to judge whether game is over
//                    if (nextSide != currentSide) {
//                        temp = AlphaBetaWithMemory(copy_currentBoard, nextSide, alpha, beta,depth-1);
//                    } else {
//                        // bonus moves are considered in the same depth
//                        temp = AlphaBetaWithMemory(copy_currentBoard, currentSide, alpha, beta, depth);
//                    }
//                    if (temp.value < bestMove.value) {
//                        bestMove.value = temp.value;
//                        bestMove.bestMove = currentMove;
//                    }
//                    beta = Math.min(beta, bestMove.value);
//                    if (alpha >= beta) {
//                        break;
//                    }
//                }
//            }
//            return bestMove;
//        }
//    }
}


