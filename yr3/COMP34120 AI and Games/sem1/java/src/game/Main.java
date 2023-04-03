package game;
import java.io.*;

/**
 * The main application class. It also provides methods for communication
 * with the game engine.
 */
public class Main
{
    /**
     * Input from the game engine.
     */
    private static Reader input = new BufferedReader(new InputStreamReader(System.in));

    /**
     * Sends a message to the game engine.
     * @param msg The message.
     */
    public static void sendMsg (String msg)
    {
        System.out.print(msg);
        System.out.flush();
    }

    /**
     * Receives a message from the game engine. Messages are terminated by
     * a '\n' character.
     * @return The message.
     * @throws IOException if there has been an I/O error.
     */
    public static String recvMsg() throws IOException
    {
        StringBuilder message = new StringBuilder();
        int newCharacter;

        do
        {
            newCharacter = input.read();
            if (newCharacter == -1)
                throw new EOFException("Input ended unexpectedly.");
            message.append((char)newCharacter);
        } while((char)newCharacter != '\n');

        return message.toString();
    }

    public static int testMakeMove(Kalah game, Side side) {
        for (int well = 1; well <=7; well++) {
            if (game.isLegalMove(new Move(side, well))) {
                return well;
            }
        }
        return 1;
    }

    // South is always own side, and is MAXNODE
    public static NextMove AlphaBetaWithMemory(Board currentBoard, Side currentSide, int alpha, int beta, int depth) {
        if (depth == 0 || Kalah.gameOver(currentBoard)) {
            Kalah currentGame = new Kalah(currentBoard);
            NextMove leafNode = new NextMove();
            leafNode.bestMove = null;
            leafNode.value = currentBoard.getSeedsInStore(Side.SOUTH) - currentBoard.getSeedsInStore(Side.NORTH) + currentGame.rankSequence(currentBoard, currentSide);
            return leafNode;
        }

        Kalah currentGame = new Kalah(currentBoard);
        NextMove bestMove = new NextMove();
        NextMove temp;

        if (currentSide == Side.SOUTH) { // MAXIMIZING PLAYER
            bestMove.value = -10000; // initialize to "-inf"
//            int[] priority = currentGame.rankSequence(currentBoard, currentSide);
//            for(int i=0; i<priority.length; i++){
            for (int well = 1; well <= 7; well++) { // TODO: use ranking instead of sequential for-loop
                Move currentMove = new Move(currentSide, well);
                if (currentGame.isLegalMove(currentMove)) {
                    Board copy_currentBoard = new Board(currentBoard);
                    Side nextSide = Kalah.makeMove(copy_currentBoard, currentMove);
                    // no need to judge whether game is over
                    if (nextSide != currentSide) {
                        temp = AlphaBetaWithMemory(copy_currentBoard, currentSide.opposite(), alpha, beta,depth-1);
                    } else {
                        // bonus moves are considered in the same depth
                        temp = AlphaBetaWithMemory(copy_currentBoard, currentSide, alpha, beta, depth);
                    }
                    if (temp.value > bestMove.value) {
                        bestMove.value = temp.value;
                        bestMove.bestMove = currentMove;
                    }
                    alpha = Math.max(alpha, bestMove.value);
                    if (alpha >= beta) {
                        break;
                    }
                }
            }
            return bestMove;
        } else {
            bestMove.value = 10000; // initialize to "inf"
//            int[] priority = currentGame.rankSequence(currentBoard, currentSide);
//            for(int i=0; i<priority.length; i++){
            for (int well = 1; well <= 7; well++) { // TODO: use ranking instead of sequential for-loop
                Move currentMove = new Move(currentSide, well);
                if (currentGame.isLegalMove(currentMove)) {
                    Board copy_currentBoard = new Board(currentBoard);
                    Side nextSide =  Kalah.makeMove(copy_currentBoard, currentMove);
                    // no need to judge whether game is over
                    if (nextSide != currentSide) {
                        temp = AlphaBetaWithMemory(copy_currentBoard, nextSide, alpha, beta,depth-1);
                    } else {
                        // bonus moves are considered in the same depth
                        temp = AlphaBetaWithMemory(copy_currentBoard, currentSide, alpha, beta, depth);
                    }
                    if (temp.value < bestMove.value) {
                        bestMove.value = temp.value;
                        bestMove.bestMove = currentMove;
                    }
                    beta = Math.min(beta, bestMove.value);
                    if (alpha >= beta) {
                        break;
                    }
                }
            }
            return bestMove;
        }
    }


    /**
     * The main method, invoked when the program is started.
     * @param args Command line arguments.
     */
    public static void main(String[] args)
    {
       // Redirect System.err top print out debug info
       try {
//            System.setErr(new PrintStream(new BufferedOutputStream(new FileOutputStream("err.txt"))));
           PrintStream printStream = new PrintStream(new FileOutputStream("err.txt"));
           System.setErr(printStream);
       } catch (FileNotFoundException e) {
           e.printStackTrace();
       }

//        int round = 0;

       // Initialize variables
       boolean isMyTurn = false;
       int last_move = -1;
       String msg = "";
       MsgType msgType = MsgType.END;
       Protocol.MoveTurn moveTurn = new Protocol.MoveTurn();
       Kalah game = new Kalah(new Board(7, 7));
       Side mySide = Side.SOUTH;

       while (true) {
           // get input message string
           try {
               msg = recvMsg();
               msgType =  Protocol.getMessageType(msg);
           } catch (Exception e) {
               System.err.println(e);
           }

           if (msgType == MsgType.START) { // if the message is a START message
               try {
                   isMyTurn = Protocol.interpretStartMsg(msg); // determine my side
               } catch (InvalidMessageException e) {
                   System.err.println(e);
               }
               mySide = isMyTurn == false ? Side.NORTH : Side.SOUTH;
           } else if (msgType == MsgType.STATE) { // if the message is a STATE message
               try {
                   moveTurn = Protocol.interpretStateMsg(msg, game.getBoard());
               } catch (InvalidMessageException e) {
                   System.err.println(e);
               }
               if (moveTurn.again == true && moveTurn.end == false) { // the next move is my move and the game does not end yet
                   isMyTurn = true;
                   last_move = moveTurn.move; // get the last move
                   if (last_move == -1) {  // opp made a swap move
                       mySide = mySide.opposite();
                   }
               }  else if (moveTurn.again == false && moveTurn.end == false) { // the next move is opp's move
                   // do something
               }
           } else { // if the message is a END message
               break;
           }

           if (isMyTurn == true) {
              //  round++;
              //  if(round==1){
              //      if(mySide == Side.SOUTH){
              //          sendMsg(Protocol.createMoveMsg(7));
              //          isMyTurn = false;
              //      }else {
              //          if(last_move != 1 && last_move !=4) {
              //              sendMsg(Protocol.createSwapMsg());
              //              mySide = mySide.opposite();
              //              isMyTurn = false;
              //          }else {
              //              NextMove nextMove = AlphaBetaWithMemory(game.getBoard(), mySide, -10000, 10000, 8);
              //              sendMsg(Protocol.createMoveMsg(nextMove.bestMove.getHole()));
              //              isMyTurn = false;
              //          }
              //      }


              //  }else {
                  // sendMsg(Protocol.createMoveMsg(testMakeMove(game, mySide)));
                   NextMove nextMove = AlphaBetaWithMemory(game.getBoard(), mySide, -10000, 10000, 10);
                   sendMsg(Protocol.createMoveMsg(nextMove.bestMove.getHole()));
                   isMyTurn = false;
              //  }

           }
       }
    }
}
