import subprocess
import os
import time

play_times = 10 ## set the times of playing the game
north_win = 0
south_win = 0

''' comment out to abandon the log '''
player1_command = 'python3 mainMinimax_h.py'
player2_command = 'java -jar error404.jar'
print(f"Command - {player1_command}, {player2_command}\n")

# with open(os.devnull, 'w') as log_file:
with open('./result.txt', 'w') as log_file:
    start = time.time()
    games_result = dict()
    for i in range(play_times):
        ''' command used to play each game '''
        game = subprocess.run(['java', '-jar', 'ManKalah.jar', player1_command, player2_command], stderr=subprocess.PIPE, stdout=log_file)
        result = game.stderr.decode('utf-8')
        paragraphs = result.split("\n")
        for p in paragraphs:
            if "WINNER" in p:
                if p in games_result:
                    games_result[p] += 1
                else:
                    games_result[p] = 1
                print(p)
            if "SCORE" in p:
                print(f"{p}\n")

end = time.time()
print(games_result)
print(f"Time elapsed - {end-start}s")
print("\n\n ***************** NOW reverse player positions ***************** \n\n")
print(f"Command - {player2_command}, {player1_command}\n")

with open(os.devnull, 'w') as log_file:
# with open('./result.txt', 'w') as log_file:
    start = time.time()
    games_result = dict()
    for i in range(play_times):
        ''' command used to play each game '''
        game = subprocess.run(['java', '-jar', 'ManKalah.jar', player2_command, player1_command], stderr=subprocess.PIPE, stdout=log_file)
        result = game.stderr.decode('utf-8')
        paragraphs = result.split("\n")
        for p in paragraphs:
            if "WINNER" in p:
                if p in games_result:
                    games_result[p] += 1
                else:
                    games_result[p] = 1
                print(p)
            if "SCORE" in p:
                print(f"{p}\n")

end = time.time()
print(games_result)
print(f"Time elapsed - {end-start}s")
